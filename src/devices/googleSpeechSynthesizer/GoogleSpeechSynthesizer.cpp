/*
 * SPDX-FileCopyrightText: 2023 Istituto Italiano di Tecnologia (IIT)
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif

#include "GoogleSpeechSynthesizer.h"

#include <yarp/os/LogComponent.h>
#include <yarp/os/LogStream.h>

#include <cmath>

using namespace yarp::os;
using namespace yarp::dev;
namespace texttospeech = ::google::cloud::texttospeech_v1;


YARP_LOG_COMPONENT(GOOGLESPEECHSYNTH, "yarp.googleSpeechSynthesizer", yarp::os::Log::TraceType);


GoogleSpeechSynthesizer::GoogleSpeechSynthesizer()
{

}

bool GoogleSpeechSynthesizer::open(yarp::os::Searchable &config)
{
    yCError(GOOGLESPEECHSYNTH) << "Open";
    return true;
}

bool GoogleSpeechSynthesizer::close()
{
    yCError(GOOGLESPEECHSYNTH) << "Close";
    return true;
}
