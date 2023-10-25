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


/**
 *  @ingroup dev_impl_other
 *
 * \section googleSpeechTranscription
 *
 * \brief `googleSpeechTranscription`: A yarp device for speech transcription using google cloud cpp libraries
 *
 *  Parameters required by this device are:
 * | Parameter name | SubParameter | Type    | Units          | Default Value | Required | Description                                        | Notes |
 * |:--------------:|:------------:|:-------:|:--------------:|:-------------:|:--------:|:--------------------------------------------------:|:-----:|
 * | language_code  | -            | string  | -              | -             | Yes      | Language for speech synthesis (e.g. "ita", "eng")  |       |
 *
 *
 * example of xml file with a fake odometer
 *
 * \code{.unparsed}
 * <?xml version="1.0" encoding="UTF-8"?>
 * <!DOCTYPE robot PUBLIC "-//YARP//DTD yarprobotinterface 3.0//EN" "http://www.yarp.it/DTD/yarprobotinterfaceV3.0.dtd">
 * <robot name="googleTest" build="2" portprefix="/googleTranscr" xmlns:xi="http://www.w3.org/2001/XInclude">
 *     <devices>
 *         <device name="googleTranscr" type="googleSpeechTranscription">
 *             <param name="language_code">
 *                 it-IT
 *             </param>
 *         </device>
 *
 *         <device name="synthWrap" type="speechTranscription_nws_yarp">
 *             <action phase="startup" level="5" type="attach">
 *                 <paramlist name="networks">
 *                     <elem name="subdeviceGoogle">
 *                         googleTranscr
 *                     </elem>
 *                 </paramlist>
 *             </action>
 *             <action phase="shutdown" level="5" type="detach" />
 *         </device>
 *     </devices>
 * </robot>
 * \endcode
 */

class GoogleSpeechTranscription :
        public yarp::dev::DeviceDriver,
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
    bool setLanguage(const std::string& language="auto") override;
    bool getLanguage(std::string& language) override;
    bool transcribe(const yarp::sig::Sound& sound, std::string& transcription, double& score) override;

private:
    int          m_sampleRate;
    bool         m_offline{false};
    std::string  m_languageCode;
    google::cloud::speech::v1::RecognitionConfig             m_audioConfig;
    std::shared_ptr<google::cloud::speech_v1::SpeechClient>  m_client{nullptr};
};

#endif // YARP_GOOGLESPEECHTRANSCR_H
