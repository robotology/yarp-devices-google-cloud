/*
 * SPDX-FileCopyrightText: 2023 Istituto Italiano di Tecnologia (IIT)
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <yarp/dev/ISpeechSynthesizer.h>
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

TEST_CASE("dev::googleSpeechSynthesizer_test", "[yarp::dev]")
{
    YARP_REQUIRE_PLUGIN("googleSpeechSynthesizer", "device");

    Network::setLocalMode(true);

    SECTION("Checking googleSpeechSynthesizer device")
    {
        ISpeechSynthesizer* iSynth{nullptr};
        PolyDriver dd;

        //read a test sound file from disk
        yarp::sig::Sound snd;
        yarp::os::ResourceFinder rf;

        rf.setQuiet(false);
        rf.setVerbose(true);

        rf.setDefaultContext("googleSpeechSynthesizer_demo");
        std::string ss = rf.findFile("test_audio.wav");
        CHECK(!ss.empty());
        yarp::sig::file::read(snd,ss.c_str());
        CHECK(snd.getSamples()>0);

        //"Checking opening device"
        {
            Property pcfg;
            const std::string init_lang{"it-IT"};
            const std::string init_voice{"it-IT-Wavenet-A"};
            pcfg.put("device", "googleSpeechSynthesizer");
            pcfg.put("language_code",init_lang);
            pcfg.put("voice_name",init_voice);
            REQUIRE(dd.open(pcfg));
            REQUIRE(dd.view(iSynth));
        }

        const std::string lang_to_set{"en-GB"};
        const std::string voice_to_set{"en-GB-Neural2-C"};
        std::string lang_code;
        std::string voice_name;

        std::string toSynthesize{"This is a text to speech test"};
        yarp::sig::Sound outputSound;
        CHECK(iSynth->setLanguage(lang_to_set));
        CHECK(iSynth->setVoice(voice_to_set));
        CHECK(iSynth->getLanguage(lang_code));
        CHECK(lang_code == lang_to_set);
        CHECK(iSynth->getVoice(voice_name));
        CHECK(voice_name == voice_to_set);

        // For the moment being, actually using the API for test only purposes doesn't seem like a good idea since it will waste money
        //CHECK(iSynth->synthesize(toSynthesize,outputSound));
        //CHECK(outputSound == snd);

        //"Close all polydrivers and check"
        {
            CHECK(dd.close());
        }
    }

    Network::setLocalMode(false);
}
