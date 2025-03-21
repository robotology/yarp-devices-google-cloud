/*
 * SPDX-FileCopyrightText: 2023 Istituto Italiano di Tecnologia (IIT)
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif

#include "GoogleSpeechTranscription.h"

#include <yarp/sig/SoundFile.h>

#include <yarp/os/LogComponent.h>
#include <yarp/os/LogStream.h>
#include <fstream>

#include <cmath>

using namespace yarp::os;
using namespace yarp::dev;


YARP_LOG_COMPONENT(GOOGLESPEECHTRANSCR, "yarp.googleSpeechTranscription", yarp::os::Log::TraceType);


GoogleSpeechTranscription::GoogleSpeechTranscription()
{

}

bool GoogleSpeechTranscription::open(yarp::os::Searchable &config)
{
    if(config.check("__offline"))
    {
        m_offline = config.find("__offline").asInt32() == 1;
    }
    if(!parseParams(config))
    {
        yCError(GOOGLESPEECHTRANSCR) << "Unable to correctly parse device params. Check previous errors for more info";
        return false;
    }
    m_audioConfig.set_language_code(m_language_code);
    m_audioConfig.set_encoding(google::cloud::speech::v1::RecognitionConfig::LINEAR16);
    m_audioConfig.set_sample_rate_hertz(m_sample_rate_hertz);
    m_client = std::make_shared<google::cloud::speech_v1::SpeechClient>(google::cloud::speech_v1::MakeSpeechConnection());

    return true;
}

bool GoogleSpeechTranscription::close()
{
    return true;
}

yarp::dev::ReturnValue GoogleSpeechTranscription::setLanguage(const std::string& language)
{
    if(language == "auto")
    {
        yCError(GOOGLESPEECHTRANSCR) << "The \"auto\" option is not supported by this device";

        return yarp::dev::ReturnValue::return_code::return_value_error_generic;
    }

    m_audioConfig.set_language_code(language);
    return yarp::dev::ReturnValue::return_code::return_value_ok;
}

yarp::dev::ReturnValue GoogleSpeechTranscription::getLanguage(std::string& language)
{
    language = m_audioConfig.language_code();

    return yarp::dev::ReturnValue::return_code::return_value_ok;
}

yarp::dev::ReturnValue GoogleSpeechTranscription::transcribe(const yarp::sig::Sound& sound, std::string& transcription, double& score)
{
    transcription="";
    score = 0.0;

    if (sound.getSamples() == 0 ||
        sound.getChannels() == 0)
    {
        yCError(GOOGLESPEECHTRANSCR) << "Invalid Sound sample received";
        return yarp::dev::ReturnValue::return_code::return_value_error_generic;
    }

    google::cloud::speech::v1::RecognitionAudio audio;
    auto rawData_tmp = sound.getNonInterleavedAudioRawData();
    auto rawData = std::vector<short>(rawData_tmp.begin(), rawData_tmp.end());
    audio.set_content((char*)rawData.data(),rawData.size()*2);

    auto response = m_client->Recognize(m_audioConfig,audio);

    if(!response)
    {
        yCError(GOOGLESPEECHTRANSCR) << "Could not perform audio transcription:" << response.status().message();
        return yarp::dev::ReturnValue::return_code::return_value_error_generic;
    }

    yCDebug(GOOGLESPEECHTRANSCR) << "Results size:" << response->results_size();
    for(int i=0; i<response->results_size(); i++)
    {
        auto result = response->results(i);
        yCDebug(GOOGLESPEECHTRANSCR) << i << "Alternative size:" << result.alternatives_size();
        for(int j=0; j<result.alternatives_size(); j++)
        {
            auto alternative = result.alternatives(j);
            float tempConf = alternative.confidence();
            yCDebug(GOOGLESPEECHTRANSCR) << "Alternative:" << alternative.SerializeAsString() << "Confidence:" << tempConf;
            if(tempConf > score)
            {
                score = tempConf;
                transcription = alternative.transcript();
            }
        }
    }
    yCDebug(GOOGLESPEECHTRANSCR) << "Transcription:" << transcription << "Score:" << score;
    return yarp::dev::ReturnValue::return_code::return_value_ok;
}
