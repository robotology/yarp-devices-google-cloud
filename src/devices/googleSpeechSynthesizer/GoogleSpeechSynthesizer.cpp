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

bool GoogleSpeechSynthesizer::open(yarp::os::Searchable &config)
{
    m_synthClient = std::make_shared<texttospeech::TextToSpeechClient>(texttospeech::MakeTextToSpeechConnection());
    m_synthVoiceSelParams = std::make_shared<google::cloud::texttospeech::v1::VoiceSelectionParams>();
    m_synthVoice = std::make_shared<google::cloud::texttospeech::v1::Voice>();
    m_synthInput = std::make_shared<google::cloud::texttospeech::v1::SynthesisInput>();
    m_synthAudioConfig = std::make_shared<google::cloud::texttospeech::v1::AudioConfig>();

    m_synthVoiceSelParams->set_language_code("en-US");
    m_synthVoiceSelParams->set_name("en-US-Wavenet-C");

    m_synthAudioConfig->set_audio_encoding(google::cloud::texttospeech::v1::MP3);

    return true;
}

bool GoogleSpeechSynthesizer::close()
{
    return true;
}

bool GoogleSpeechSynthesizer::setLanguage(const std::string& language)
{
    m_synthVoiceSelParams->set_language_code(language);

    return true;
}

bool GoogleSpeechSynthesizer::getLanguage(std::string& language)
{
    language = m_synthVoiceSelParams->language_code();

    return true;
}

bool GoogleSpeechSynthesizer::setVoice(const std::string& voice_name)
{
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
    if(m_synthAudioConfig->speaking_rate() != speed)
    {
        yCError(GOOGLESPEECHSYNTH) << "Error while setting the speach rate (speed)";
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
    if(m_synthAudioConfig->pitch() != pitch)
    {
        yCError(GOOGLESPEECHSYNTH) << "Error while setting the pitch value";
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
    YARP_UNUSED(sound);
    m_synthInput->set_text(text);
    yCDebug(GOOGLESPEECHSYNTH) << m_synthVoiceSelParams->language_code();
    google::cloud::v2_14::StatusOr<google::cloud::texttospeech::v1::SynthesizeSpeechResponse> response = m_synthClient->SynthesizeSpeech(*m_synthInput,*m_synthVoiceSelParams,*m_synthAudioConfig);
    if (!response) {
        yCError(GOOGLESPEECHSYNTH) << "Error synthesizing speech. Google status:\n\t" << response.status().message() << "\n";
        return false;
    }

    /// TODO: REMOVE
    std::string filename = "hello_world.mp3";
    std::ofstream outfile(filename, std::ios::binary);
    outfile.write(response->audio_content().data(), response->audio_content().size());
    outfile.close();
    /// TODO: REMOVE

    sound.clear();
    if(!yarp::sig::file::read_bytestream(sound, response->audio_content().data(), response->audio_content().size(), ".mp3"))
    {
        yCError(GOOGLESPEECHSYNTH) << "Error while transfering data from google response to yarp::sigSound sound object";
    }

    return true;
}