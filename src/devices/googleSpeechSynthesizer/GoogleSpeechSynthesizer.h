/*
 * SPDX-FileCopyrightText: 2023 Istituto Italiano di Tecnologia (IIT)
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef YARP_GOOGLESPEECHSYNTH_H
#define YARP_GOOGLESPEECHSYNTH_H

#include <yarp/dev/DeviceDriver.h>
#include <yarp/dev/ISpeechSynthesizer.h>
#include <yarp/sig/Sound.h>
#include <yarp/os/Network.h>
#include <algorithm>

#include "google/cloud/texttospeech/v1/text_to_speech_client.h"
#include "google/protobuf/repeated_ptr_field.h"

#include "GoogleSpeechSynthesizer_ParamsParser.h"

// Constants
const std::pair SPEED_RANGE{0.25, 4.0};
const std::pair PITCH_RANGE{-20.0, 20.0};

/**
 *  @ingroup dev_impl_other
 *
 * \section googleSpeechSynthesizer
 *
 * \brief `googleSpeechSynthesizer`: A yarp device for speech synthesis using google cloud cpp libraries
 *
 *  Parameters required by this device are described in class GoogleSpeechSynthesizer_ParamsParser
 *
 */

class GoogleSpeechSynthesizer :
        public yarp::dev::DeviceDriver,
        public GoogleSpeechSynthesizer_ParamsParser,
        public yarp::dev::ISpeechSynthesizer
{
public:
    GoogleSpeechSynthesizer();
    GoogleSpeechSynthesizer(const GoogleSpeechSynthesizer&) = delete;
    GoogleSpeechSynthesizer(GoogleSpeechSynthesizer&&) noexcept = delete;
    GoogleSpeechSynthesizer& operator=(const GoogleSpeechSynthesizer&) = delete;
    GoogleSpeechSynthesizer& operator=(GoogleSpeechSynthesizer&&) noexcept = delete;
    ~GoogleSpeechSynthesizer() override = default;

    /**
     * Checks whether or not a particular voice name is supported by the currently selected language code
     * \param voice_name the name of of the voice
     * \return true on success
    */
    bool _voiceSupported(const std::string& voice_name);

    // DeviceDriver
    bool open(yarp::os::Searchable& config) override;
    bool close() override;

    // yarp::dev::ISpeechSynthesizer
    yarp::dev::ReturnValue setLanguage(const std::string& language="auto") override;
    yarp::dev::ReturnValue getLanguage(std::string& language) override;
    yarp::dev::ReturnValue setVoice(const std::string& voice_name = "auto") override;
    yarp::dev::ReturnValue getVoice(std::string& voice_name) override;
    yarp::dev::ReturnValue setSpeed(const double speed=0) override;
    yarp::dev::ReturnValue getSpeed(double& speed) override;
    yarp::dev::ReturnValue setPitch(const double pitch) override;
    yarp::dev::ReturnValue getPitch(double& pitch) override;
    yarp::dev::ReturnValue synthesize(const std::string& text, yarp::sig::Sound& sound) override;

private:
    bool m_offline{false};
    std::shared_ptr<google::cloud::texttospeech_v1::TextToSpeechClient>        m_synthClient{nullptr};
    std::shared_ptr<google::cloud::texttospeech::v1::SynthesisInput>           m_synthInput{nullptr};
    std::shared_ptr<google::cloud::texttospeech::v1::VoiceSelectionParams>     m_synthVoiceSelParams{nullptr};
    std::shared_ptr<google::cloud::texttospeech::v1::AudioConfig>              m_synthAudioConfig{nullptr};
    google::protobuf::RepeatedPtrField<google::cloud::texttospeech::v1::Voice> m_synthVoices{nullptr};
};

#endif // YARP_GOOGLESPEECHSYNTH_H
