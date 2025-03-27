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

    parseParams(config);

#ifdef USE_DISPLAY_NAME
    if(!config.check("display_name"))
    {
        yCError(GOOGLEDIALOGFLOWCXBOT) << "display_name parameter must be specified";
        return false;
    }

    std::string displayName = config.find("display_name").asString();

    google::cloud::dialogflow_cx::AgentsClient agentsClient(google::cloud::dialogflow_cx::MakeAgentsConnection(m_location));

    dialogFlow_cx_v3::ListAgentsRequest request;

    std::string parent = "projects/"+m_project;
    request.set_parent(parent);

    double oldTime = yarp::os::Time::now();
    yCWarning(GOOGLEDIALOGFLOWCXBOT) << "Time" << oldTime;
    //auto agentsList = agentsClient.ListAgents(parent);
    google::cloud::Options opt;
    auto agentsList = agentsClient.ListAgents(request,opt);
    yCWarning(GOOGLEDIALOGFLOWCXBOT) << "Time" << yarp::os::Time::now() - oldTime;

    for (const auto& agent : agentsList) {
        std::this_thread::sleep_for(30s);
        if (agent->display_name() == displayName) {
            m_agentId = agent->name();
            break;
        }
    }
#else
    m_agentId = "projects/" + m_project + "/locations/" + m_location + "/agents/" + m_agent_name;
#endif
    m_sessionId = m_agentId + "/sessions/" + _getRandSession_();
    m_channel = grpc::CreateChannel("dialogflow.googleapis.com", grpc::GoogleDefaultCredentials());
    m_sessionStub = dialogFlow_cx_v3::Sessions::NewStub(m_channel);

    // Get the currently active agent page
    std::string message{"*"};
    std::string resp;
    bool isItATest = (m_agent_name == "test_agent") && (m_project == "test_project");
    if(!interact(message,resp) && !isItATest)
    {
        yCError(GOOGLEDIALOGFLOWCXBOT) << "Could not retrieve the current chatbot status";

        return false;
    }

    return true;
}

bool GoogleDialogFlowCxChatBot::close()
{
    return true;
}

yarp::dev::ReturnValue GoogleDialogFlowCxChatBot::interact(const std::string& messageIn, std::string& messageOut)
{
    dialogFlow_cx_v3::DetectIntentRequest req;

    // Create a query input
    dialogFlow_cx_v3::QueryInput query_input;
    auto parameters = query_input.mutable_language_code();
    *parameters = m_language_code;
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
            m_currentPage = response.query_result().current_page().display_name();
            const dialogFlow_cx_v3::QueryResult& query_result = response.query_result();
            if(query_result.response_messages_size() <= 0)
            {
                yCDebug(GOOGLEDIALOGFLOWCXBOT) << "An empty fulfillment was returned. Check the chatbot structure for safety";
                messageOut = "";
                return ReturnValue_ok;
            }
            messageOut = query_result.response_messages().Get(0).text().text().Get(0);
        } else {
            yCError(GOOGLEDIALOGFLOWCXBOT) << "No query result in the response.";

            return yarp::dev::ReturnValue::return_code::return_value_error_generic;
        }
    } else {
        yCError(GOOGLEDIALOGFLOWCXBOT) << "Failed to detect intent. Error: " << status.error_message();
        return yarp::dev::ReturnValue::return_code::return_value_error_generic;
    }

    return ReturnValue_ok;
}

yarp::dev::ReturnValue GoogleDialogFlowCxChatBot::setLanguage(const std::string& language)
{
    m_language_code = language;
    return ReturnValue_ok;
}

yarp::dev::ReturnValue GoogleDialogFlowCxChatBot::getLanguage(std::string& language)
{
    language = m_language_code;
    return ReturnValue_ok;
}

yarp::dev::ReturnValue GoogleDialogFlowCxChatBot::getStatus(std::string& status)
{
    status = m_currentPage;

    return ReturnValue_ok;
}

yarp::dev::ReturnValue GoogleDialogFlowCxChatBot::resetBot()
{
    m_sessionId = m_agentId + "/sessions/" + _getRandSession_();
    return ReturnValue_ok;
}
