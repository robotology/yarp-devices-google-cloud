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
 *  Parameters required by this device are:
 * | Parameter name | SubParameter | Type    | Units          | Default Value | Required | Description                                                                                                                                                                                                                                    | Notes |
 * |:--------------:|:------------:|:-------:|:--------------:|:-------------:|:--------:|:----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------:|:-----:|
 * | language_code  | -            | string  | -              | -             | Yes      | Language for speech synthesis (e.g. "ita", "eng")                                                                                                                                                                                              |       |
 * | voice_name     | -            | string  | -              | -             | No       | The voice set for speech synthesis. If not set, the device will pick the first available voice for the selected language code (This page holds the complete list of the available voices: https://cloud.google.com/text-to-speech/docs/voices) |       |
 * | voice_speed    | -            | double  | -              | 1.0           | No       | Speaking rate/speed, in the range [0.25, 4.0]. 1.0 is the normal native speed supported by the specific voice. 2.0 is twice as fast, and 0.5 is half as fast.                                                                                  |       |
 * | voice_pitch    | -            | double  | -              | 0.0           | No       | Speaking pitch, in the range [-20.0, 20.0]. 20 means increase 20 semitones from the original pitch. -20 means decrease 20 semitones from the original pitch.                                                                                   |       |
 *
 *
 * example of xml file with a fake odometer
 *
 * \code{.unparsed}
 * <?xml version="1.0" encoding="UTF-8"?>
 * <!DOCTYPE robot PUBLIC "-//YARP//DTD yarprobotinterface 3.0//EN" "http://www.yarp.it/DTD/yarprobotinterfaceV3.0.dtd">
 * <robot name="googleTest" build="2" portprefix="/googleSynth" xmlns:xi="http://www.w3.org/2001/XInclude">
 *     <devices>
 *         <device name="googleSynth" type="googleSpeechSynthesizer">
 *             <param name="language_code">
 *                 it-IT
 *             </param>
 *             <param name="voice_name">
 *                 it-IT-Standard-B
 *             </param>
 *         </device>
 *
 *         <device name="synthWrap" type="speechSynthesizer_nws_yarp">
 *             <action phase="startup" level="5" type="attach">
 *                 <paramlist name="networks">
 *                     <elem name="subdeviceGoogle">
 *                         googleSynth
 *                     </elem>
 *                 </paramlist>
 *             </action>
 *             <action phase="shutdown" level="5" type="detach" />
 *         </device>
 *     </devices>
 * </robot>
 * \endcode
 */

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
    std::shared_ptr<google::cloud::texttospeech_v1::TextToSpeechClient>        m_synthClient{nullptr};
    std::shared_ptr<google::cloud::texttospeech::v1::SynthesisInput>           m_synthInput{nullptr};
    std::shared_ptr<google::cloud::texttospeech::v1::VoiceSelectionParams>     m_synthVoiceSelParams{nullptr};
    std::shared_ptr<google::cloud::texttospeech::v1::AudioConfig>              m_synthAudioConfig{nullptr};
    google::protobuf::RepeatedPtrField<google::cloud::texttospeech::v1::Voice> m_synthVoices{nullptr};
};

#endif // YARP_GOOGLESPEECHSYNTH_H
