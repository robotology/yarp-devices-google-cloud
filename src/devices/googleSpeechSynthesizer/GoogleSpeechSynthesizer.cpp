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

bool GoogleSpeechSynthesizer::open(yarp::os::Searchable &config)
{
    if(!config.check("language_code"))
    {
        yCError(GOOGLESPEECHSYNTH) << "No language code specified";

        return false;
    }
    std::string language = config.find("language_code").asString();

    double speed = config.check("voice_speed",yarp::os::Value(1.0),"").asFloat64();
    double pitch = config.check("voice_pitch",yarp::os::Value(0.0),"").asFloat64();

    m_synthClient = std::make_shared<texttospeech::TextToSpeechClient>(texttospeech::MakeTextToSpeechConnection());
    m_synthVoiceSelParams = std::make_shared<google::cloud::texttospeech::v1::VoiceSelectionParams>();
    m_synthInput = std::make_shared<google::cloud::texttospeech::v1::SynthesisInput>();
    m_synthAudioConfig = std::make_shared<google::cloud::texttospeech::v1::AudioConfig>();

    if(!setLanguage(language))
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

    if(!setSpeed(speed))
    {
       return false;
    }
    if(!setPitch(pitch))
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

bool GoogleSpeechSynthesizer::setLanguage(const std::string& language)
{
    if(language == "auto")
    {
        yCError(GOOGLESPEECHSYNTH) << "The \"auto\" option is not supported by this device";

        return false;
    }
    if(language == m_synthVoiceSelParams->language_code() && m_synthVoices.size() != 0)
    {
        yCWarning(GOOGLESPEECHSYNTH) << "The language code is already set to:" << language;
        return true;
    }
    google::cloud::v2_12::StatusOr<google::cloud::texttospeech::v1::ListVoicesResponse> response = m_synthClient->ListVoices(language);
    if (!response) {
        yCError(GOOGLESPEECHSYNTH) << "Error in getting the list of available voices. Google status:\n\t" << response.status().message() << "\n";
        return false;
    }
    m_synthVoices = response->voices();
    m_synthVoiceSelParams->set_language_code(language);

    setVoice(m_synthVoices[0].name());

    return true;
}

bool GoogleSpeechSynthesizer::getLanguage(std::string& language)
{
    language = m_synthVoiceSelParams->language_code();

    return true;
}

bool GoogleSpeechSynthesizer::setVoice(const std::string& voice_name)
{
    if(voice_name == "auto")
    {
        m_synthVoiceSelParams->set_name(m_synthVoices[0].name());
        yCInfo(GOOGLESPEECHSYNTH) << "auto option selected. Setting the voice name to:" << m_synthVoiceSelParams->name();

        return true;
    }

    if(!_voiceSupported(voice_name))
    {
        return false;
    }

    m_synthVoiceSelParams->set_name(voice_name);

    return true;
}

bool GoogleSpeechSynthesizer::getVoice(std::string& voice_name)
{
    voice_name = m_synthVoiceSelParams->name();

    return true;
}

bool GoogleSpeechSynthesizer::setSpeed(const double speed)
{
    m_synthAudioConfig->set_speaking_rate(speed);
    if(speed < SPEED_RANGE.first || speed > SPEED_RANGE.second)
    {
        yCError(GOOGLESPEECHSYNTH) << "Error while setting the speach rate (speed). The value is outside the allowed range [" << SPEED_RANGE.first << SPEED_RANGE.second << "]";
        return false;
    }

    return true;
}

bool GoogleSpeechSynthesizer::getSpeed(double& speed)
{
    speed = m_synthAudioConfig->speaking_rate();

    return true;
}

bool GoogleSpeechSynthesizer::setPitch(const double pitch)
{
    m_synthAudioConfig->set_pitch(pitch);
    if(pitch < PITCH_RANGE.first || pitch > PITCH_RANGE.second)
    {
        yCError(GOOGLESPEECHSYNTH) << "Error while setting the speach pitch. The value is outside the allowed range [" << PITCH_RANGE.first << PITCH_RANGE.second << "]";
        return false;
    }

    return true;
}

bool GoogleSpeechSynthesizer::getPitch(double& pitch)
{
    pitch = m_synthAudioConfig->pitch();

    return true;
}

bool GoogleSpeechSynthesizer::synthesize(const std::string& text, yarp::sig::Sound& sound)
{
    m_synthInput->set_text(text);
    google::cloud::v2_12::StatusOr<google::cloud::texttospeech::v1::SynthesizeSpeechResponse> response = m_synthClient->SynthesizeSpeech(*m_synthInput,*m_synthVoiceSelParams,*m_synthAudioConfig);
    if (!response) {
        yCError(GOOGLESPEECHSYNTH) << "Error synthesizing speech. Google status:\n\t" << response.status().message() << "\n";
        return false;
    }

    sound.clear();
    if(!yarp::sig::file::read_bytestream(sound, response->audio_content().data(), response->audio_content().size(), ".mp3"))
    {
        yCError(GOOGLESPEECHSYNTH) << "Error while transfering data from google response to yarp::sigSound sound object";
    }

    return true;
}
