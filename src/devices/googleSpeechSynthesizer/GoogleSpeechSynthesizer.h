/*
 * SPDX-FileCopyrightText: 2023 Istituto Italiano di Tecnologia (IIT)
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef YARP_GOOGLESPEECHSYNTH_H
#define YARP_GOOGLESPEECHSYNTH_H

#include <yarp/dev/DeviceDriver.h>
#include "google/cloud/texttospeech/v1/text_to_speech_client.h"

class GoogleSpeechSynthesizer :
        public yarp::dev::DeviceDriver
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
};

#endif // YARP_GOOGLESPEECHSYNTH_H
