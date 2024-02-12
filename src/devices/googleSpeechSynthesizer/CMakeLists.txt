# SPDX-FileCopyrightText: 2023 Istituto Italiano di Tecnologia (IIT)
# SPDX-License-Identifier: BSD-3-Clause

yarp_prepare_plugin(googleSpeechSynthesizer
  CATEGORY device
  TYPE GoogleSpeechSynthesizer
  INCLUDE GoogleSpeechSynthesizer.h
  INTERNAL ON
)

find_package(google_cloud_cpp_texttospeech REQUIRED)

if(NOT SKIP_googleSpeechSynthesizer)
  yarp_add_plugin(yarp_googleSpeechSynthesizer)

  target_sources(yarp_googleSpeechSynthesizer
    PRIVATE
      GoogleSpeechSynthesizer.cpp
      GoogleSpeechSynthesizer.h
      GoogleSpeechSynthesizer_ParamsParser.cpp
      GoogleSpeechSynthesizer_ParamsParser.h
  )

  target_link_libraries(yarp_googleSpeechSynthesizer
    PRIVATE
      YARP::YARP_os
      YARP::YARP_sig
      YARP::YARP_dev
      google-cloud-cpp::texttospeech
  )

  yarp_install(
    TARGETS yarp_googleSpeechSynthesizer
    EXPORT yarp-device-googleSpeechSynthesizer
    COMPONENT yarp-device-googleSpeechSynthesizer
    LIBRARY DESTINATION ${YARP_DYNAMIC_PLUGINS_INSTALL_DIR}
    ARCHIVE DESTINATION ${YARP_STATIC_PLUGINS_INSTALL_DIR}
    YARP_INI DESTINATION ${YARP_PLUGIN_MANIFESTS_INSTALL_DIR}
  )

  add_subdirectory(demos)

  if(YARP_COMPILE_TESTS)
    add_subdirectory(tests)
  endif()

  set_property(TARGET yarp_googleSpeechSynthesizer PROPERTY FOLDER "Plugins/Device")
endif()
