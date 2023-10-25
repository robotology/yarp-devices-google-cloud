# SPDX-FileCopyrightText: 2023 Istituto Italiano di Tecnologia (IIT)
# SPDX-License-Identifier: BSD-3-Clause

yarp_prepare_plugin(googleSpeechTranscription
  CATEGORY device
  TYPE GoogleSpeechTranscription
  INCLUDE GoogleSpeechTranscription.h
  INTERNAL ON
)

find_package(google_cloud_cpp_speech REQUIRED)

if(NOT SKIP_googleSpeechTranscription)
  yarp_add_plugin(yarp_googleSpeechTranscription)

  target_sources(yarp_googleSpeechTranscription
    PRIVATE
      GoogleSpeechTranscription.cpp
      GoogleSpeechTranscription.h
  )

  target_link_libraries(yarp_googleSpeechTranscription
    PRIVATE
      YARP::YARP_os
      YARP::YARP_sig
      YARP::YARP_dev
      google-cloud-cpp::speech
  )

  yarp_install(
    TARGETS yarp_googleSpeechTranscription
    EXPORT yarp-device-googleSpeechTranscription
    COMPONENT yarp-device-googleSpeechTranscription
    LIBRARY DESTINATION ${YARP_DYNAMIC_PLUGINS_INSTALL_DIR}
    ARCHIVE DESTINATION ${YARP_STATIC_PLUGINS_INSTALL_DIR}
    YARP_INI DESTINATION ${YARP_PLUGIN_MANIFESTS_INSTALL_DIR}
  )

  if(YARP_COMPILE_TESTS)
    add_subdirectory(tests)
    add_subdirectory(demos)
  endif()

  set_property(TARGET yarp_googleSpeechTranscription PROPERTY FOLDER "Plugins/Device")
endif()
