/*
 * SPDX-FileCopyrightText: 2023-2023 Istituto Italiano di Tecnologia (IIT)
 * SPDX-License-Identifier: LGPL-2.1-or-later
 */


// Generated by yarpDeviceParamParserGenerator (1.0)
// This is an automatically generated file. Please do not edit it.
// It will be re-generated if the cmake flag ALLOW_DEVICE_PARAM_PARSER_GERNERATION is ON.

// Generated on: Wed Apr 10 11:16:30 2024


#ifndef GOOGLESPEECHSYNTHESIZER_PARAMSPARSER_H
#define GOOGLESPEECHSYNTHESIZER_PARAMSPARSER_H

#include <yarp/os/Searchable.h>
#include <yarp/dev/IDeviceDriverParams.h>
#include <string>
#include <cmath>

/**
* This class is the parameters parser for class GoogleSpeechSynthesizer.
*
* These are the used parameters:
* | Group name | Parameter name | Type   | Units | Default Value | Required | Description                                                                                                                                                                                                                                    | Notes |
* |:----------:|:--------------:|:------:|:-----:|:-------------:|:--------:|:----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------:|:-----:|
* | -          | language_code  | string | -     | -             | 1        | Language for speech synthesis (e.g. \"ita\", \"eng\")                                                                                                                                                                                          | -     |
* | -          | voice_name     | string | -     | -             | 0        | The voice set for speech synthesis. If not set, the device will pick the first available voice for the selected language code (This page holds the complete list of the available voices: https://cloud.google.com/text-to-speech/docs/voices) | -     |
* | -          | voice_speed    | double | -     | 1             | 0        | Speaking rate/speed, in the range [0.25, 4.0]. 1.0 is the normal native speed supported by the specific voice. 2.0 is twice as fast, and 0.5 is half as fast.                                                                                  | -     |
* | -          | voice_pitch    | double | -     | 0             | 0        | Speaking pitch, in the range [-20.0, 20.0]. 20 means increase 20 semitones from the original pitch. -20 means decrease 20 semitones from the original pitch.                                                                                   | -     |
*
* The device can be launched by yarpdev using one of the following examples (with and without all optional parameters):
* \code{.unparsed}
* yarpdev --device googleSpeechSynthesizer --language_code <mandatory_value> --voice_name <optional_value> --voice_speed 1 --voice_pitch 0
* \endcode
*
* \code{.unparsed}
* yarpdev --device googleSpeechSynthesizer --language_code <mandatory_value>
* \endcode
*
*/

class GoogleSpeechSynthesizer_ParamsParser : public yarp::dev::IDeviceDriverParams
{
public:
    GoogleSpeechSynthesizer_ParamsParser();
    ~GoogleSpeechSynthesizer_ParamsParser() override = default;

public:
    const std::string m_device_classname = {"GoogleSpeechSynthesizer"};
    const std::string m_device_name = {"googleSpeechSynthesizer"};
    bool m_parser_is_strict = false;
    struct parser_version_type
    {
         int major = 1;
         int minor = 0;
    };
    const parser_version_type m_parser_version = {};

    const std::string m_language_code_defaultValue = {""};
    const std::string m_voice_name_defaultValue = {""};
    const std::string m_voice_speed_defaultValue = {"1"};
    const std::string m_voice_pitch_defaultValue = {"0"};

    std::string m_language_code = {}; //This default value is autogenerated. It is highly recommended to provide a suggested value also for mandatory parameters.
    std::string m_voice_name = {}; //This default value of this string is an empty string. It is highly recommended to provide a suggested value also for optional string parameters.
    double m_voice_speed = {1};
    double m_voice_pitch = {0};

    bool          parseParams(const yarp::os::Searchable & config) override;
    std::string   getDeviceClassName() const override { return m_device_classname; }
    std::string   getDeviceName() const override { return m_device_name; }
    std::string   getDocumentationOfDeviceParams() const override;
    std::vector<std::string> getListOfParams() const override;
};

#endif
