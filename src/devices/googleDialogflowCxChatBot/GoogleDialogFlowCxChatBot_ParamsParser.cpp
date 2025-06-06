/*
 * SPDX-FileCopyrightText: 2023-2023 Istituto Italiano di Tecnologia (IIT)
 * SPDX-License-Identifier: LGPL-2.1-or-later
 */


// Generated by yarpDeviceParamParserGenerator (2.0)
// This is an automatically generated file. Please do not edit it.
// It will be re-generated if the cmake flag ALLOW_DEVICE_PARAM_PARSER_GERNERATION is ON.

// Generated on: Fri Jun  6 15:30:04 2025


#include "GoogleDialogFlowCxChatBot_ParamsParser.h"
#include <yarp/os/LogStream.h>
#include <yarp/os/Value.h>

namespace {
    YARP_LOG_COMPONENT(GoogleDialogFlowCxChatBotParamsCOMPONENT, "yarp.device.GoogleDialogFlowCxChatBot")
}


GoogleDialogFlowCxChatBot_ParamsParser::GoogleDialogFlowCxChatBot_ParamsParser()
{
}


std::vector<std::string> GoogleDialogFlowCxChatBot_ParamsParser::getListOfParams() const
{
    std::vector<std::string> params;
    params.push_back("project");
    params.push_back("location");
    params.push_back("agent_name");
    params.push_back("language_code");
    return params;
}


bool GoogleDialogFlowCxChatBot_ParamsParser::getParamValue(const std::string& paramName, std::string& paramValue) const
{
    if (paramName =="project")
    {
        paramValue = m_project;
        return true;
    }
    if (paramName =="location")
    {
        paramValue = m_location;
        return true;
    }
    if (paramName =="agent_name")
    {
        paramValue = m_agent_name;
        return true;
    }
    if (paramName =="language_code")
    {
        paramValue = m_language_code;
        return true;
    }

    yError() <<"parameter '" << paramName << "' was not found";
    return false;

}


std::string GoogleDialogFlowCxChatBot_ParamsParser::getConfiguration() const
{
    //This is a sub-optimal solution.
    //Ideally getConfiguration() should return all parameters but it is currently
    //returning only user provided parameters (excluding default values)
    //This behaviour will be fixed in the near future.
    std::string s_cfg = m_provided_configuration;
    return s_cfg;
}

bool      GoogleDialogFlowCxChatBot_ParamsParser::parseParams(const yarp::os::Searchable & config)
{
    //Check for --help option
    if (config.check("help"))
    {
        yCInfo(GoogleDialogFlowCxChatBotParamsCOMPONENT) << getDocumentationOfDeviceParams();
    }

    m_provided_configuration = config.toString();
    yarp::os::Property prop_check(m_provided_configuration.c_str());
    //Parser of parameter project
    {
        if (config.check("project"))
        {
            m_project = config.find("project").asString();
            yCInfo(GoogleDialogFlowCxChatBotParamsCOMPONENT) << "Parameter 'project' using value:" << m_project;
        }
        else
        {
            yCError(GoogleDialogFlowCxChatBotParamsCOMPONENT) << "Mandatory parameter 'project' not found!";
            yCError(GoogleDialogFlowCxChatBotParamsCOMPONENT) << "Description of the parameter: The name of the Google cloud project the wanted agent belongs to";
            return false;
        }
        prop_check.unput("project");
    }

    //Parser of parameter location
    {
        if (config.check("location"))
        {
            m_location = config.find("location").asString();
            yCInfo(GoogleDialogFlowCxChatBotParamsCOMPONENT) << "Parameter 'location' using value:" << m_location;
        }
        else
        {
            yCError(GoogleDialogFlowCxChatBotParamsCOMPONENT) << "Mandatory parameter 'location' not found!";
            yCError(GoogleDialogFlowCxChatBotParamsCOMPONENT) << "Description of the parameter: The world region specified for the wanted agent";
            return false;
        }
        prop_check.unput("location");
    }

    //Parser of parameter agent_name
    {
        if (config.check("agent_name"))
        {
            m_agent_name = config.find("agent_name").asString();
            yCInfo(GoogleDialogFlowCxChatBotParamsCOMPONENT) << "Parameter 'agent_name' using value:" << m_agent_name;
        }
        else
        {
            yCError(GoogleDialogFlowCxChatBotParamsCOMPONENT) << "Mandatory parameter 'agent_name' not found!";
            yCError(GoogleDialogFlowCxChatBotParamsCOMPONENT) << "Description of the parameter: The name of the agent";
            return false;
        }
        prop_check.unput("agent_name");
    }

    //Parser of parameter language_code
    {
        if (config.check("language_code"))
        {
            m_language_code = config.find("language_code").asString();
            yCInfo(GoogleDialogFlowCxChatBotParamsCOMPONENT) << "Parameter 'language_code' using value:" << m_language_code;
        }
        else
        {
            yCInfo(GoogleDialogFlowCxChatBotParamsCOMPONENT) << "Parameter 'language_code' using DEFAULT value:" << m_language_code;
        }
        prop_check.unput("language_code");
    }

    /*
    //This code check if the user set some parameter which are not check by the parser
    //If the parser is set in strict mode, this will generate an error
    if (prop_check.size() > 0)
    {
        bool extra_params_found = false;
        for (auto it=prop_check.begin(); it!=prop_check.end(); it++)
        {
            if (m_parser_is_strict)
            {
                yCError(GoogleDialogFlowCxChatBotParamsCOMPONENT) << "User asking for parameter: "<<it->name <<" which is unknown to this parser!";
                extra_params_found = true;
            }
            else
            {
                yCWarning(GoogleDialogFlowCxChatBotParamsCOMPONENT) << "User asking for parameter: "<< it->name <<" which is unknown to this parser!";
            }
        }

       if (m_parser_is_strict && extra_params_found)
       {
           return false;
       }
    }
    */
    return true;
}


std::string      GoogleDialogFlowCxChatBot_ParamsParser::getDocumentationOfDeviceParams() const
{
    std::string doc;
    doc = doc + std::string("\n=============================================\n");
    doc = doc + std::string("This is the help for device: GoogleDialogFlowCxChatBot\n");
    doc = doc + std::string("\n");
    doc = doc + std::string("This is the list of the parameters accepted by the device:\n");
    doc = doc + std::string("'project': The name of the Google cloud project the wanted agent belongs to\n");
    doc = doc + std::string("'location': The world region specified for the wanted agent\n");
    doc = doc + std::string("'agent_name': The name of the agent\n");
    doc = doc + std::string("'language_code': The language code to use for the interaction with the bot\n");
    doc = doc + std::string("\n");
    doc = doc + std::string("Here are some examples of invocation command with yarpdev, with all params:\n");
    doc = doc + " yarpdev --device googleDialogFlowCxChatBot --project <mandatory_value> --location <mandatory_value> --agent_name <mandatory_value> --language_code en-US\n";
    doc = doc + std::string("Using only mandatory params:\n");
    doc = doc + " yarpdev --device googleDialogFlowCxChatBot --project <mandatory_value> --location <mandatory_value> --agent_name <mandatory_value>\n";
    doc = doc + std::string("=============================================\n\n");    return doc;
}
