/*
 * SPDX-FileCopyrightText: 2023-2023 Istituto Italiano di Tecnologia (IIT)
 * SPDX-License-Identifier: LGPL-2.1-or-later
 */


// Generated by yarpDeviceParamParserGenerator (1.0)
// This is an automatically generated file. Please do not edit it.
// It will be re-generated if the cmake flag ALLOW_DEVICE_PARAM_PARSER_GERNERATION is ON.

// Generated on: Wed Apr 10 11:34:08 2024


#ifndef GOOGLESPEECHTRANSCRIPTION_PARAMSPARSER_H
#define GOOGLESPEECHTRANSCRIPTION_PARAMSPARSER_H

#include <yarp/os/Searchable.h>
#include <yarp/dev/IDeviceDriverParams.h>
#include <string>
#include <cmath>

/**
* This class is the parameters parser for class GoogleSpeechTranscription.
*
* These are the used parameters:
* | Group name | Parameter name    | Type   | Units | Default Value | Required | Description                                           | Notes |
* |:----------:|:-----------------:|:------:|:-----:|:-------------:|:--------:|:-----------------------------------------------------:|:-----:|
* | -          | language_code     | string | -     | -             | 1        | Language for speech synthesis (e.g. \"ita\", \"eng\") | -     |
* | -          | sample_rate_hertz | int    | Hz    | 16000         | 0        | The input audio sample rate                           | -     |
*
* The device can be launched by yarpdev using one of the following examples (with and without all optional parameters):
* \code{.unparsed}
* yarpdev --device googleSpeechTranscription --language_code <mandatory_value> --sample_rate_hertz 16000
* \endcode
*
* \code{.unparsed}
* yarpdev --device googleSpeechTranscription --language_code <mandatory_value>
* \endcode
*
*/

class GoogleSpeechTranscription_ParamsParser : public yarp::dev::IDeviceDriverParams
{
public:
    GoogleSpeechTranscription_ParamsParser();
    ~GoogleSpeechTranscription_ParamsParser() override = default;

public:
    const std::string m_device_classname = {"GoogleSpeechTranscription"};
    const std::string m_device_name = {"googleSpeechTranscription"};
    bool m_parser_is_strict = false;
    struct parser_version_type
    {
         int major = 1;
         int minor = 0;
    };
    const parser_version_type m_parser_version = {};

    const std::string m_language_code_defaultValue = {""};
    const std::string m_sample_rate_hertz_defaultValue = {"16000"};

    std::string m_language_code = {}; //This default value is autogenerated. It is highly recommended to provide a suggested value also for mandatory parameters.
    int m_sample_rate_hertz = {16000};

    bool          parseParams(const yarp::os::Searchable & config) override;
    std::string   getDeviceClassName() const override { return m_device_classname; }
    std::string   getDeviceName() const override { return m_device_name; }
    std::string   getDocumentationOfDeviceParams() const override;
    std::vector<std::string> getListOfParams() const override;
};

#endif
