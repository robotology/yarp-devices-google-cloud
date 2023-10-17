/*
 * SPDX-FileCopyrightText: 2023 Istituto Italiano di Tecnologia (IIT)
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <yarp/dev/IChatBot.h>
#include <yarp/os/Network.h>
#include <yarp/os/LogStream.h>
#include <yarp/os/ResourceFinder.h>
#include <yarp/dev/PolyDriver.h>
#include <yarp/dev/WrapperSingle.h>

#include <catch2/catch_amalgamated.hpp>
#include <harness.h>

using namespace yarp::dev;
using namespace yarp::os;

TEST_CASE("dev::googleDialogFlowCxChatBot_test", "[yarp::dev]")
{
    YARP_REQUIRE_PLUGIN("googleDialogFlowCxChatBot", "device");

    Network::setLocalMode(true);

    SECTION("Checking googleDialogFlowCxChatBot device")
    {
        IChatBot* iChat{nullptr};
        PolyDriver dd;

        rf.setQuiet(false);
        rf.setVerbose(true);

        //"Checking opening device"
        {
            Property pcfg;
            const std::string init_lang{"it-IT"};
            const std::string init_voice{"it-IT-Wavenet-A"};
            pcfg.put("device", "googleDialogFlowCxChatBot");
            pcfg.put("language_code",init_lang);
            pcfg.put("location","global");
            // Fake value for test purpouses
            pcfg.put("project","test_project");
            pcfg.put("agent","test_agent");
            REQUIRE(dd.open(pcfg));
            REQUIRE(dd.view(iChat));
        }


        //Checking interface methods
        {
            const std::string lang_to_set{"en-US"};
            const std::string messageIn{"Hello"};
            std::string lang_set;
            std::string messageOut;
            bool resp;
            resp = iChat->setLanguage(lang_to_set);
            CHECK(resp);
            resp = iChat->getLanguage(lang_set);
            CHECK(resp);
            CHECK(lang_set==lang_to_set);
            //Without the proper credentials and agent id the following methods have to fail
            resp = iChat->interact(messageIn,messageOut);
            CHECK(!resp);
            resp = iChat->resetBot();
            CHECK(!resp);
        }


        //"Close all polydrivers and check"
        {
            CHECK(dd.close());
        }
    }

    Network::setLocalMode(false);
}
