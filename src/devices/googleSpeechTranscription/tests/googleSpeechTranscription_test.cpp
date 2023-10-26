/*
 * SPDX-FileCopyrightText: 2023 Istituto Italiano di Tecnologia (IIT)
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <yarp/dev/ISpeechTranscription.h>
#include <yarp/os/Network.h>
#include <yarp/os/LogStream.h>
#include <yarp/os/ResourceFinder.h>
#include <yarp/dev/PolyDriver.h>
#include <yarp/dev/WrapperSingle.h>

#include <yarp/sig/Sound.h>
#include <yarp/sig/SoundFile.h>

#include <catch2/catch_amalgamated.hpp>
#include <harness.h>

using namespace yarp::dev;
using namespace yarp::os;

TEST_CASE("dev::googleSpeechTranscription", "[yarp::dev]")
{
    YARP_REQUIRE_PLUGIN("googleSpeechTranscription", "device");

    Network::setLocalMode(true);

    SECTION("Checking googleSpeechTranscription device")
    {
        yarp::dev::ISpeechTranscription* istr=nullptr;
        PolyDriver ddfake;

        //read a test sound file from disk
        yarp::sig::Sound snd;
        yarp::os::ResourceFinder rf;

        rf.setQuiet(false);
        rf.setVerbose(true);

        rf.setDefaultContext("googleSpeechTranscription_demo");
        std::string ss = rf.findFile("test_audio.wav");
        CHECK(!ss.empty());
        yarp::sig::file::read(snd,ss.c_str());
        CHECK(snd.getSamples()>0);

        //open the device
        {
            Property pdev_cfg;
            pdev_cfg.put("device", "googleSpeechTranscription");
            pdev_cfg.put("language_code", "it-IT");
            REQUIRE(ddfake.open(pdev_cfg));
            REQUIRE(ddfake.view(istr));
        }

        std::string lang = "en-US";
        CHECK(istr->setLanguage(lang));

        CHECK(istr->getLanguage(lang));
        CHECK(lang == "en-US");

        // Disabled to avoid wasting money in unnecessary calls to google APIs
        //std::string transcript;
        //double score;
        //CHECK(istr->transcribe(snd,transcript, score));
        //CHECK(transcript=="This is a text to speech test");
        //CHECK(score > 0.99);

        //"Close all polydrivers and check"
        {
            yarp::os::Time::delay(0.1);
            CHECK(ddfake.close());
        }
    }

    Network::setLocalMode(false);
}
