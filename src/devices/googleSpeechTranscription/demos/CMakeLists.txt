# SPDX-FileCopyrightText: 2023-2023 Istituto Italiano di Tecnologia (IIT)
# SPDX-License-Identifier: BSD-3-Clause

set(appname googleSpeechTranscription_demo)

file(GLOB conf      ${CMAKE_CURRENT_SOURCE_DIR}/yarprobotinterface_xml/*.xml)
file(GLOB data      ${CMAKE_CURRENT_SOURCE_DIR}/*.wav)
file(GLOB apps      ${CMAKE_CURRENT_SOURCE_DIR}/*.xml)

yarp_install(FILES ${conf}    DESTINATION ${YARP_CONTEXTS_INSTALL_DIR}/${appname})
yarp_install(FILES ${data}    DESTINATION ${YARP_CONTEXTS_INSTALL_DIR}/${appname})
yarp_install(FILES ${apps}    DESTINATION ${YARP_APPLICATIONS_INSTALL_DIR})
