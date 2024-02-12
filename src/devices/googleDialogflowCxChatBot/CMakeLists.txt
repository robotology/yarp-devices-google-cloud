# SPDX-FileCopyrightText: 2023 Istituto Italiano di Tecnologia (IIT)
# SPDX-License-Identifier: BSD-3-Clause

yarp_prepare_plugin(googleDialogFlowCxChatBot
  CATEGORY device
  TYPE GoogleDialogFlowCxChatBot
  INCLUDE GoogleDialogFlowCxChatBot.h
  INTERNAL ON
)

find_package(google_cloud_cpp_dialogflow_cx REQUIRED)

if(NOT SKIP_googleDialogFlowCxChatBot)
  yarp_add_plugin(yarp_googleDialogFlowCxChatBot)

  target_sources(yarp_googleDialogFlowCxChatBot
    PRIVATE
      GoogleDialogFlowCxChatBot.cpp
      GoogleDialogFlowCxChatBot.h
      GoogleDialogFlowCxChatBot_ParamsParser.cpp
      GoogleDialogFlowCxChatBot_ParamsParser.h
  )

  target_link_libraries(yarp_googleDialogFlowCxChatBot
    PRIVATE
      YARP::YARP_os
      YARP::YARP_sig
      YARP::YARP_dev
      google-cloud-cpp::dialogflow_cx
      protobuf::libprotobuf
      gRPC::grpc++
  )

  yarp_install(
    TARGETS yarp_googleDialogFlowCxChatBot
    EXPORT yarp-device-googleDialogFlowCxChatBot
    COMPONENT yarp-device-googleDialogFlowCxChatBot
    LIBRARY DESTINATION ${YARP_DYNAMIC_PLUGINS_INSTALL_DIR}
    ARCHIVE DESTINATION ${YARP_STATIC_PLUGINS_INSTALL_DIR}
    YARP_INI DESTINATION ${YARP_PLUGIN_MANIFESTS_INSTALL_DIR}
  )

  if(YARP_COMPILE_TESTS)
    add_subdirectory(tests)
  endif()

  set_property(TARGET yarp_googleDialogFlowCxChatBot PROPERTY FOLDER "Plugins/Device")
endif()
