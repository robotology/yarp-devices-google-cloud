/*
 * SPDX-FileCopyrightText: 2023 Istituto Italiano di Tecnologia (IIT)
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef YARP_GOOGLESPEECHTRANSCR_H
#define YARP_GOOGLESPEECHTRANSCR_H

#include <yarp/dev/DeviceDriver.h>
#include <yarp/dev/ISpeechTranscription.h>
#include <yarp/sig/Sound.h>
#include <yarp/os/Network.h>
#include <algorithm>
#include <memory>
#include <vector>

#include "google/cloud/speech/v1/speech_client.h"
#include "google/protobuf/repeated_ptr_field.h"
#include "google/cloud/grpc_options.h"

#include "GoogleSpeechTranscription_ParamsParser.h"


/**
 *  @ingroup dev_impl_other
 *
 * \section googleSpeechTranscription
 *
 * \brief `googleSpeechTranscription`: A yarp device for speech transcription using google cloud cpp libraries
 *
 *  Parameters required by this device are described in class GoogleSpeechTranscription_ParamsParser
 *
 */

class GoogleSpeechTranscription :
        public yarp::dev::DeviceDriver,
        public GoogleSpeechTranscription_ParamsParser,
        public yarp::dev::ISpeechTranscription
{
public:
    GoogleSpeechTranscription();
    GoogleSpeechTranscription(const GoogleSpeechTranscription&) = delete;
    GoogleSpeechTranscription(GoogleSpeechTranscription&&) noexcept = delete;
    GoogleSpeechTranscription& operator=(const GoogleSpeechTranscription&) = delete;
    GoogleSpeechTranscription& operator=(GoogleSpeechTranscription&&) noexcept = delete;
    ~GoogleSpeechTranscription() override = default;

    // DeviceDriver
    bool open(yarp::os::Searchable& config) override;
    bool close() override;

    // yarp::dev::ISpeechTranscription
    yarp::dev::ReturnValue setLanguage(const std::string& language="auto") override;
    yarp::dev::ReturnValue getLanguage(std::string& language) override;
    yarp::dev::ReturnValue transcribe(const yarp::sig::Sound& sound, std::string& transcription, double& score) override;

private:

    std::shared_ptr<google::cloud::speech_v1::SpeechClient> _getClient(google::cloud::Options opts);

    bool         m_offline{false};
    google::cloud::speech::v1::RecognitionConfig             m_audioConfig;
};

#endif // YARP_GOOGLESPEECHTRANSCR_H
