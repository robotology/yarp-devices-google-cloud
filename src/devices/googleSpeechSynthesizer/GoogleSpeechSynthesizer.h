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

#include "google/cloud/texttospeech/v1/text_to_speech_client.h"

class GoogleSpeechSynthesizer :
        public yarp::dev::DeviceDriver,
        public yarp::dev::ISpeechSynthesizer
{
public:
    GoogleSpeechSynthesizer();
    GoogleSpeechSynthesizer(const GoogleSpeechSynthesizer&) = delete;
    GoogleSpeechSynthesizer(GoogleSpeechSynthesizer&&) noexcept = delete;
    GoogleSpeechSynthesizer& operator=(const GoogleSpeechSynthesizer&) = delete;
    GoogleSpeechSynthesizer& operator=(GoogleSpeechSynthesizer&&) noexcept = delete;
    ~GoogleSpeechSynthesizer() override = default;

    // DeviceDriver
    bool open(yarp::os::Searchable& config) override;
    bool close() override;

    // yarp::dev::ISpeechSynthesizer
    bool setLanguage(const std::string& language="auto") override;
    bool getLanguage(std::string& language) override;
    bool setVoice(const std::string& voice_name = "auto") override;
    bool getVoice(std::string& voice_name) override;
    bool setSpeed(const double speed=0) override;
    bool getSpeed(double& speed) override;
    bool setPitch(const double pitch) override;
    bool getPitch(double& pitch) override;
    bool synthesize(const std::string& text, yarp::sig::Sound& sound) override;

private:
    std::shared_ptr<google::cloud::texttospeech_v1::TextToSpeechClient>    m_synthClient{nullptr};
    std::shared_ptr<google::cloud::texttospeech::v1::SynthesisInput>       m_synthInput{nullptr};
    std::shared_ptr<google::cloud::texttospeech::v1::VoiceSelectionParams> m_synthVoiceSelParams{nullptr};
    std::shared_ptr<google::cloud::texttospeech::v1::Voice>                m_synthVoice{nullptr};
    std::shared_ptr<google::cloud::texttospeech::v1::AudioConfig>          m_synthAudioConfig{nullptr};
    std::string    m_voiceName;
    std::string    m_language;
    double         m_speed;
    double         m_pitch;

};

#endif // YARP_GOOGLESPEECHSYNTH_H
