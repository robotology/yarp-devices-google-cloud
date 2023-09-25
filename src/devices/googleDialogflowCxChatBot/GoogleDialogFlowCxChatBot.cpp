/*
 * SPDX-FileCopyrightText: 2023 Istituto Italiano di Tecnologia (IIT)
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif

#include "GoogleDialogFlowCxChatBot.h"

#include <yarp/sig/SoundFile.h>

#include <yarp/os/LogComponent.h>
#include <yarp/os/LogStream.h>
#include <fstream>

#include <thread>
#include <chrono>

#include <cmath>

using namespace yarp::os;
using namespace yarp::dev;
using namespace std::chrono_literals;

namespace dialogFlow_cx_v3 = ::google::cloud::dialogflow::cx::v3;

YARP_LOG_COMPONENT(GOOGLEDIALOGFLOWCXBOT, "yarp.googleDialogFlowCxChatBot", yarp::os::Log::TraceType);

std::string GoogleDialogFlowCxChatBot::_getRandSession_() {
    std::string session;
    srand(time(NULL));
    for(int i = 0; i<16; i++) {
        session += std::to_string(rand() % 10);
    }
    return session;
}

bool GoogleDialogFlowCxChatBot::open(yarp::os::Searchable &config)
{
    yCDebug(GOOGLEDIALOGFLOWCXBOT) << "Configuration: \n" << config.toString().c_str();

    if(!config.check("project"))
    {
        yCError(GOOGLEDIALOGFLOWCXBOT) << "project parameter must be specified";
        return false;
    }
    m_project = config.find("project").asString();

    if(!config.check("location"))
    {
        yCError(GOOGLEDIALOGFLOWCXBOT) << "location parameter must be specified";
        return false;
    }
    m_location = config.find("location").asString();

    if(config.check("language_code"))
    {
        m_languageCode = config.find("language_code").asString();
    }

#ifdef USE_DISPLAY_NAME
    if(!config.check("display_name"))
    {
        yCError(GOOGLEDIALOGFLOWCXBOT) << "display_name parameter must be specified";
        return false;
    }

    std::string displayName = config.find("display_name").asString();

    yCWarning(GOOGLEDIALOGFLOWCXBOT) << "Siamo qui" << m_location;

    google::cloud::dialogflow_cx::AgentsClient agentsClient(google::cloud::dialogflow_cx::MakeAgentsConnection(m_location));

    yCWarning(GOOGLEDIALOGFLOWCXBOT) << "Siamo qui 2" << "";

    dialogFlow_cx_v3::ListAgentsRequest request;
    yCWarning(GOOGLEDIALOGFLOWCXBOT) << "Siamo qui 3" << m_location;

    std::string parent = "projects/"+m_project;
    request.set_parent(parent);

    yCWarning(GOOGLEDIALOGFLOWCXBOT) << "Siamo qui 4" << request.IsInitialized();;

    double oldTime = yarp::os::Time::now();
    yCWarning(GOOGLEDIALOGFLOWCXBOT) << "Time" << oldTime;
    //auto agentsList = agentsClient.ListAgents(parent);
    google::cloud::v2_15::Options opt;
    auto agentsList = agentsClient.ListAgents(request,opt);
    yCWarning(GOOGLEDIALOGFLOWCXBOT) << "Time" << yarp::os::Time::now() - oldTime;
    yCWarning(GOOGLEDIALOGFLOWCXBOT) << "Siamo qui 5" << m_location;

    for (const auto& agent : agentsList) {
        yCWarning(GOOGLEDIALOGFLOWCXBOT) << "Siamo qui 6" << agent.ok() <<"wait for 30 seconds";
        std::this_thread::sleep_for(30s);
        yCWarning(GOOGLEDIALOGFLOWCXBOT) << "Siamo qui 7" <<"waited for 30 seconds";
        if (agent->display_name() == displayName) {
            m_agentId = agent->name();
            yCWarning(GOOGLEDIALOGFLOWCXBOT) << "Siamo qui 7" << m_agentId;
            break;
        }
    }
#else
    if(!config.check("agent_name"))
    {
        yCError(GOOGLEDIALOGFLOWCXBOT) << "agent_name parameter must be specified";
        return false;
    }
    m_agentId = "projects/" + m_project + "/locations/" + m_location + "/agents/" + config.find("agent_name").asString();
#endif
    m_sessionId = m_agentId + "/sessions/" + _getRandSession_();
    m_channel = grpc::CreateChannel("dialogflow.googleapis.com", grpc::GoogleDefaultCredentials());
    m_sessionStub = dialogFlow_cx_v3::Sessions::NewStub(m_channel);
    return true;
}

bool GoogleDialogFlowCxChatBot::close()
{
    return true;
}

bool GoogleDialogFlowCxChatBot::interact(const std::string& messageIn, std::string& messageOut)
{
    YARP_UNUSED(messageIn);
    YARP_UNUSED(messageOut);
    dialogFlow_cx_v3::DetectIntentRequest req;

    // Create a query input
    dialogFlow_cx_v3::QueryInput query_input;
    auto parameters = query_input.mutable_language_code();
    *parameters = m_languageCode;
    dialogFlow_cx_v3::TextInput* text_input = query_input.mutable_text();
    text_input->set_text(messageIn);

    // Create a DetectIntent request
    dialogFlow_cx_v3::DetectIntentRequest detect_intent_request;
    detect_intent_request.set_session(m_sessionId);
    *detect_intent_request.mutable_query_input() = query_input;

    // Send the request to Dialogflow CX
    grpc::ClientContext context;
    dialogFlow_cx_v3::DetectIntentResponse response;

    grpc::Status status = m_sessionStub->DetectIntent(&context, detect_intent_request, &response);

    // Handle the response
    if (status.ok()) {
        if (response.has_query_result()) {
            const dialogFlow_cx_v3::QueryResult& query_result = response.query_result();
            messageOut = query_result.response_messages().Get(0).text().text().Get(0);
        } else {
            yCError(GOOGLEDIALOGFLOWCXBOT) << "No query result in the response.";

            return false;
        }
    } else {
        yCError(GOOGLEDIALOGFLOWCXBOT) << "Failed to detect intent. Error: " << status.error_message();
        return false;
    }

    return true;
}

bool GoogleDialogFlowCxChatBot::setLanguage(const std::string& language)
{
    m_languageCode = language;
    return true;
}

bool GoogleDialogFlowCxChatBot::getLanguage(std::string& language)
{
    language = m_languageCode;
    return true;
}

bool GoogleDialogFlowCxChatBot::resetBot()
{
    m_sessionId = m_agentId + "/sessions/" + _getRandSession_();
    return true;
}
