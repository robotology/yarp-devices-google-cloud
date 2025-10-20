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

std::shared_ptr<google::cloud::speech_v1::SpeechClient> GoogleSpeechTranscription::_getClient(google::cloud::Options opts)
{
    return std::make_shared<google::cloud::speech_v1::SpeechClient>(google::cloud::speech_v1::MakeSpeechConnection(opts));
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

    auto response = _getClient(opts)->Recognize(m_audioConfig,audio);

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
