/*
 * SPDX-FileCopyrightText: 2023 Istituto Italiano di Tecnologia (IIT)
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef YARP_GOOGLEDIALOGCXBOT_H
#define YARP_GOOGLEDIALOGCXBOT_H

#include <yarp/dev/DeviceDriver.h>
#include <yarp/dev/IChatBot.h>
#include <yarp/os/Network.h>
#include <algorithm>

#include <google/cloud/dialogflow_cx/agents_client.h>
#include <google/cloud/dialogflow/cx/v3/agent.pb.h>
#include <google/cloud/dialogflow_cx/sessions_client.h>
#include <google/cloud/dialogflow/cx/v3/session.grpc.pb.h>
#include <google/cloud/dialogflow/cx/v3/session.pb.h>
#include <grpcpp/grpcpp.h>

/**
 *  @ingroup dev_impl_other
 *
 * \section googleDialogFlowCxChatBot
 *
 * \brief `googleDialogFlowCxChatBot`: A yarp device to enable interaction with Google's DialogFlow CX service
 *
 *  Parameters required by this device are:
 * | Parameter name | SubParameter | Type    | Units          | Default Value | Required | Description                                                      | Notes |
 * |:--------------:|:------------:|:-------:|:--------------:|:-------------:|:--------:|:----------------------------------------------------------------:|:-----:|
 * | project        | -            | string  | -              | -             | Yes      | The name of the Google cloud project the wanted agent belongs to |       |
 * | location       | -            | string  | -              | -             | Yes      | The world region specified for the wanted agent                  |       |
 * | agent_name     | -            | string  | -              | -             | Yes      | The name of the agent                                            |       |
 * | language_code  | -            | string  | -              | en-US         | No       | The language code to use for the interaction with the bot        |       |
 *
 *
 * example of xml file with a fake odometer
 *
 * \code{.unparsed}
 * <?xml version="1.0" encoding="UTF-8"?>
 * <!DOCTYPE robot PUBLIC "-//YARP//DTD yarprobotinterface 3.0//EN" "http://www.yarp.it/DTD/yarprobotinterfaceV3.0.dtd">
 * <robot name="googleTest" build="2" portprefix="/googleBot" xmlns:xi="http://www.w3.org/2001/XInclude">
 *     <devices>
 *         <device name="googleDialogCx" type="googleDialogFlowCxChatBot">
 *             <param name="project">
 *                 <!-- Insert project name here -->
 *             </param>
 *             <param name="location">
 *                 <!-- Insert location id here -->
 *             </param>
 *             <param name="agent_name">
 *                 <!-- Insert agent display name here -->
 *             </param>
 *         </device>
 *         <device name="dialogWrap" type="chatBot_nws_yarp">
 *             <action phase="startup" level="5" type="attach">
 *                 <paramlist name="networks">
 *                     <elem name="subdeviceGoogle">
 *                         googleDialogCx
 *                     </elem>
 *                 </paramlist>
 *             </action>
 *             <action phase="shutdown" level="5" type="detach" />
 *         </device>
 *     </devices>
 * </robot>
 * \endcode
 */


class GoogleDialogFlowCxChatBot :
        public yarp::dev::DeviceDriver,
        public yarp::dev::IChatBot
{
public:
    GoogleDialogFlowCxChatBot() = default;
    GoogleDialogFlowCxChatBot(const GoogleDialogFlowCxChatBot&) = delete;
    GoogleDialogFlowCxChatBot(GoogleDialogFlowCxChatBot&&) noexcept = delete;
    GoogleDialogFlowCxChatBot& operator=(const GoogleDialogFlowCxChatBot&) = delete;
    GoogleDialogFlowCxChatBot& operator=(GoogleDialogFlowCxChatBot&&) noexcept = delete;
    ~GoogleDialogFlowCxChatBot() override = default;

    // DeviceDriver
    bool open(yarp::os::Searchable& config) override;
    bool close() override;

    // IChatBot
    bool interact(const std::string& messageIn, std::string& messageOut) override;
    bool setLanguage(const std::string& language) override;
    bool getLanguage(std::string& language) override;
    bool getStatus(std::string& status) override;
    bool resetBot() override;

private:
    std::string m_project;
    std::string m_location;
    std::string m_agentId;
    std::string m_sessionId;
    std::string m_languageCode;
    std::shared_ptr<grpc::Channel> m_channel;
    std::unique_ptr<google::cloud::dialogflow::cx::v3::Sessions::Stub> m_sessionStub;
    std::shared_ptr<google::cloud::dialogflow_cx::SessionsClient> m_session{nullptr};

    std::string _getRandSession_();
};

#endif // YARP_GOOGLEDIALOGCXBOT_H
