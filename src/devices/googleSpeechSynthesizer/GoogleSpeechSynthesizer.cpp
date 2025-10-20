/*
 * SPDX-FileCopyrightText: 2023 Istituto Italiano di Tecnologia (IIT)
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif

#include "GoogleSpeechSynthesizer.h"

#include <yarp/sig/SoundFile.h>

#include <yarp/os/LogComponent.h>
#include <yarp/os/LogStream.h>
#include <fstream>

#include <cmath>

using namespace yarp::os;
using namespace yarp::dev;
namespace texttospeech = ::google::cloud::texttospeech_v1;


YARP_LOG_COMPONENT(GOOGLESPEECHSYNTH, "yarp.googleSpeechSynthesizer", yarp::os::Log::TraceType);


GoogleSpeechSynthesizer::GoogleSpeechSynthesizer()
{

}

bool GoogleSpeechSynthesizer::_voiceSupported(const std::string& voice_name)
{
    if(m_offline)
    {
        return true;
    }
    if(m_synthVoices.size() == 0)
    {
        yCError(GOOGLESPEECHSYNTH) << "You haven't set the language code yet. Voice name cannot be set without a language code";
    }

    google::protobuf::RepeatedPtrField<google::cloud::texttospeech::v1::Voice>::iterator it;
    for(it = m_synthVoices.begin(); it != m_synthVoices.end(); it++)
    {
        if(it->name() == voice_name)
        {
            return true;
        }
    }

    yCError(GOOGLESPEECHSYNTH) << "Unsupported voice name";
    return false;
}

std::shared_ptr<google::cloud::texttospeech_v1::TextToSpeechClient> GoogleSpeechSynthesizer::_getClient(google::cloud::Options opts)
{
    return std::make_shared<texttospeech::TextToSpeechClient>(texttospeech::MakeTextToSpeechConnection(opts));
}

bool GoogleSpeechSynthesizer::open(yarp::os::Searchable &config)
{
    if(config.check("__offline"))
    {
        m_offline = config.find("__offline").asInt32() == 1;
    }

    if (!parseParams(config))
    {
        yCError(GOOGLESPEECHSYNTH) << "Failed to parse parameters";
        return false;
    }

    // m_synthClient = std::make_shared<texttospeech::TextToSpeechClient>(texttospeech::MakeTextToSpeechConnection());
    m_synthVoiceSelParams = std::make_shared<google::cloud::texttospeech::v1::VoiceSelectionParams>();
    m_synthInput = std::make_shared<google::cloud::texttospeech::v1::SynthesisInput>();
    m_synthAudioConfig = std::make_shared<google::cloud::texttospeech::v1::AudioConfig>();

    // Populate default voices map
    for(const auto& voice : m_default_voices)
    {
        std::string lang_code;
        size_t first_dash = voice.find('-');
        if (first_dash == std::string::npos) {
            // No dash found: fallback to using the whole name
            lang_code = voice;
        } else {
            size_t second_dash = voice.find('-', first_dash + 1);
            if (second_dash == std::string::npos) {
                // Only one dash: language code is the part before it
                lang_code = voice.substr(0, first_dash);
            } else {
                // Two or more dashes: language code is up to the second dash
                lang_code = voice.substr(0, second_dash);
            }
        }
        if(m_defaultVoicesMap.find(lang_code) != m_defaultVoicesMap.end())
        {
            yCWarning(GOOGLESPEECHSYNTH) << "The default voice for language code" << lang_code << "was already set to" << m_defaultVoicesMap[lang_code] << ". Overriding it with the new value:" << voice;
            continue;
        }
        m_defaultVoicesMap[lang_code] = voice;
    }

    if(!setLanguage(m_language_code) && !m_offline)
    {
        return false;
    }

    if(config.check("voice_name"))
    {
        if(!setVoice(config.find("voice_name").asString()))
        {
            return false;
        }
    };

    if(!setSpeed(m_voice_speed))
    {
       return false;
    }
    if(!setPitch(m_voice_pitch))
    {
       return false;
    }

    m_synthAudioConfig->set_audio_encoding(google::cloud::texttospeech::v1::MP3);

    return true;
}

bool GoogleSpeechSynthesizer::close()
{
    return true;
}

yarp::dev::ReturnValue GoogleSpeechSynthesizer::setLanguage(const std::string& language)
{
    if(m_offline)
    {
        m_synthVoiceSelParams->set_language_code(language);
        return yarp::dev::ReturnValue::return_code::return_value_ok;
    }
    if(language == "auto")
    {
        yCError(GOOGLESPEECHSYNTH) << "The \"auto\" option is not supported by this device";

        return yarp::dev::ReturnValue::return_code::return_value_error_generic;
    }
    if(language == m_synthVoiceSelParams->language_code() && m_synthVoices.size() != 0)
    {
        yCWarning(GOOGLESPEECHSYNTH) << "The language code is already set to:" << language;
        return yarp::dev::ReturnValue::return_code::return_value_error_generic;
    }
    std::string start_voice;

    // Configure client with timeout options
    google::cloud::Options opts;
    // Connection timeout (initial connection establishment)
    // This is what you want to be short for detecting no internet
    opts.set<google::cloud::GrpcChannelArgumentsOption>({
        {"grpc.initial_reconnect_backoff_ms", "1000"},      // 1 second
        {"grpc.min_reconnect_backoff_ms", "1000"},
        {"grpc.max_reconnect_backoff_ms", "5000"},          // 5 seconds max
        {"grpc.http2.min_ping_interval_without_data_ms", "30000"},
        {"grpc.keepalive_timeout_ms", "10000"},             // 10 seconds
    });

    google::cloud::StatusOr<google::cloud::texttospeech::v1::ListVoicesResponse> response = _getClient(opts)->ListVoices(language);
    if (!response) {
        yCError(GOOGLESPEECHSYNTH) << "Error in getting the list of available voices. Google status:\n\t" << response.status().message() << "\n";
        return yarp::dev::ReturnValue::return_code::return_value_error_generic;
    }
    m_synthVoices = response->voices();
    m_synthVoiceSelParams->set_language_code(language);
    if(m_defaultVoicesMap.find(language) == m_defaultVoicesMap.end())
    {
        yCInfo(GOOGLESPEECHSYNTH) << "The provided language code is not supported. Setting the voice name to the first available voice for the selected language code:" << m_synthVoices[0].name();
        start_voice = m_synthVoices[0].name();
    }
    else
    {
        yCInfo(GOOGLESPEECHSYNTH) << "Setting the voice name to the default voice for the selected language code:" << m_defaultVoicesMap[language];
        start_voice = m_defaultVoicesMap[language];
    }

    setVoice(start_voice);

    return yarp::dev::ReturnValue::return_code::return_value_ok;
}

yarp::dev::ReturnValue GoogleSpeechSynthesizer::getLanguage(std::string& language)
{
    language = m_synthVoiceSelParams->language_code();

    return yarp::dev::ReturnValue::return_code::return_value_ok;
}

yarp::dev::ReturnValue GoogleSpeechSynthesizer::setVoice(const std::string& voice_name)
{
    if(voice_name == "auto")
    {
        m_synthVoiceSelParams->set_name(m_synthVoices[0].name());
        yCInfo(GOOGLESPEECHSYNTH) << "auto option selected. Setting the voice name to:" << m_synthVoiceSelParams->name();

        return yarp::dev::ReturnValue::return_code::return_value_ok;
    }

    if(m_offline)
    {
        m_synthVoiceSelParams->set_name(voice_name);
        return yarp::dev::ReturnValue::return_code::return_value_ok;
    }

    if(!_voiceSupported(voice_name))
    {
        return yarp::dev::ReturnValue::return_code::return_value_error_generic;
    }

    m_synthVoiceSelParams->set_name(voice_name);

    return yarp::dev::ReturnValue::return_code::return_value_ok;
}

yarp::dev::ReturnValue GoogleSpeechSynthesizer::getVoice(std::string& voice_name)
{
    voice_name = m_synthVoiceSelParams->name();

    return yarp::dev::ReturnValue::return_code::return_value_ok;
}

yarp::dev::ReturnValue GoogleSpeechSynthesizer::setSpeed(const double speed)
{
    m_synthAudioConfig->set_speaking_rate(speed);
    if(speed < SPEED_RANGE.first || speed > SPEED_RANGE.second)
    {
        yCError(GOOGLESPEECHSYNTH) << "Error while setting the speach rate (speed). The value is outside the allowed range [" << SPEED_RANGE.first << SPEED_RANGE.second << "]";
        return yarp::dev::ReturnValue::return_code::return_value_error_generic;
    }

    return yarp::dev::ReturnValue::return_code::return_value_ok;
}

yarp::dev::ReturnValue GoogleSpeechSynthesizer::getSpeed(double& speed)
{
    speed = m_synthAudioConfig->speaking_rate();

    return yarp::dev::ReturnValue::return_code::return_value_ok;
}

yarp::dev::ReturnValue GoogleSpeechSynthesizer::setPitch(const double pitch)
{
    m_synthAudioConfig->set_pitch(pitch);
    if(pitch < PITCH_RANGE.first || pitch > PITCH_RANGE.second)
    {
        yCError(GOOGLESPEECHSYNTH) << "Error while setting the speach pitch. The value is outside the allowed range [" << PITCH_RANGE.first << PITCH_RANGE.second << "]";
        return yarp::dev::ReturnValue::return_code::return_value_error_generic;
    }

    return yarp::dev::ReturnValue::return_code::return_value_ok;
}

yarp::dev::ReturnValue GoogleSpeechSynthesizer::getPitch(double& pitch)
{
    pitch = m_synthAudioConfig->pitch();

    return yarp::dev::ReturnValue::return_code::return_value_ok;
}

yarp::dev::ReturnValue GoogleSpeechSynthesizer::synthesize(const std::string& text, yarp::sig::Sound& sound)
{
    m_synthInput->set_text(text);
    yCDebug(GOOGLESPEECHSYNTH) << "Synthesizing text:" << text;

    // Configure client with timeout options
    google::cloud::Options opts;
    // Connection timeout (initial connection establishment)
    // This is what you want to be short for detecting no internet
    opts.set<google::cloud::GrpcChannelArgumentsOption>({
        {"grpc.initial_reconnect_backoff_ms", "1000"},      // 1 second
        {"grpc.min_reconnect_backoff_ms", "1000"},
        {"grpc.max_reconnect_backoff_ms", "5000"},          // 5 seconds max
        {"grpc.http2.min_ping_interval_without_data_ms", "30000"},
        {"grpc.keepalive_timeout_ms", "10000"},             // 10 seconds
    });

    google::cloud::StatusOr<google::cloud::texttospeech::v1::SynthesizeSpeechResponse> response = _getClient(opts)->SynthesizeSpeech(*m_synthInput,*m_synthVoiceSelParams,*m_synthAudioConfig);
    if (!response) {
        yCError(GOOGLESPEECHSYNTH) << "Error synthesizing speech. Google status:\n\t" << response.status().message() << "\n";
        return yarp::dev::ReturnValue::return_code::return_value_error_generic;
    }

    sound.clear();
    if(!yarp::sig::file::read_bytestream(sound, response->audio_content().data(), response->audio_content().size(), ".mp3"))
    {
        yCError(GOOGLESPEECHSYNTH) << "Error while transfering data from google response to yarp::sigSound sound object";
        return yarp::dev::ReturnValue::return_code::return_value_error_generic;
    }
    yCDebug(GOOGLESPEECHSYNTH) << "Synthesized sound channels:" << sound.getChannels() << "channels";
    yCDebug(GOOGLESPEECHSYNTH) << "Synthesized sound length:" << sound.getSamples() << "samples";

    return yarp::dev::ReturnValue::return_code::return_value_ok;
}
