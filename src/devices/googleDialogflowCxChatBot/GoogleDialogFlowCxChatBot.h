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

#include "GoogleDialogFlowCxChatBot_ParamsParser.h"

/**
 *  @ingroup dev_impl_other
 *
 * \section googleDialogFlowCxChatBot
 *
 * \brief `googleDialogFlowCxChatBot`: A yarp device to enable interaction with Google's DialogFlow CX service
 *
 *  Parameters required by this device are described in class GoogleDialogFlowCxChatBot_ParamsParser
 */


class GoogleDialogFlowCxChatBot :
        public yarp::dev::DeviceDriver,
        public GoogleDialogFlowCxChatBot_ParamsParser,
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
    std::string m_agentId;
    std::string m_sessionId;
    std::string m_currentPage;
    std::shared_ptr<grpc::Channel> m_channel;
    std::unique_ptr<google::cloud::dialogflow::cx::v3::Sessions::Stub> m_sessionStub;
    std::shared_ptr<google::cloud::dialogflow_cx::SessionsClient> m_session{nullptr};

    std::string _getRandSession_();
};

#endif // YARP_GOOGLEDIALOGCXBOT_H
