# -*- mode: CMAKE; -*-
# these are the install targets for the client package.
# we can't use RUNTIME DESTINATION here.

set(STRIP_DIR "${CMAKE_RUNTIME_OUTPUT_DIRECTORY_X}/strip")
execute_process(COMMAND mkdir -p ${STRIP_DIR})

set(FILE ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_X}/${BIN_ARANGOBENCH}${CMAKE_EXECUTABLE_SUFFIX})
set(STRIP_FILE ${STRIP_DIR}/${BIN_ARANGOBENCH}${CMAKE_EXECUTABLE_SUFFIX})
if (NOT MSVC AND CMAKE_STRIP)
  execute_process(COMMAND "rm" -f ${STRIP_FILE})
  execute_process(COMMAND "cp" ${FILE} ${STRIP_DIR})
  execute_process(COMMAND "${CMAKE_STRIP}" ${STRIP_FILE})
  set(FILE ${STRIP_FILE})
endif()
install(
  PROGRAMS ${FILE}
  DESTINATION ${CMAKE_INSTALL_BINDIR})
install_config(arangobench)


set(FILE ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_X}/${BIN_ARANGODUMP}${CMAKE_EXECUTABLE_SUFFIX})
set(STRIP_FILE ${STRIP_DIR}/${BIN_ARANGODUMP}${CMAKE_EXECUTABLE_SUFFIX})
if (NOT MSVC AND CMAKE_STRIP)
  execute_process(COMMAND "rm" -f ${STRIP_FILE})
  execute_process(COMMAND "cp" ${FILE} ${STRIP_DIR})
  execute_process(COMMAND "${CMAKE_STRIP}" ${STRIP_FILE})
  set(FILE ${STRIP_FILE})
endif()
install(
  PROGRAMS ${FILE}
  DESTINATION ${CMAKE_INSTALL_BINDIR})
install_config(arangodump)


set(FILE ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_X}/${BIN_ARANGOIMP}${CMAKE_EXECUTABLE_SUFFIX})
set(STRIP_FILE ${STRIP_DIR}/${BIN_ARANGOIMP}${CMAKE_EXECUTABLE_SUFFIX})
if (NOT MSVC AND CMAKE_STRIP)
  execute_process(COMMAND "rm" -f ${STRIP_FILE})
  execute_process(COMMAND "cp" ${FILE} ${STRIP_DIR})
  execute_process(COMMAND "${CMAKE_STRIP}" ${STRIP_FILE})
  set(FILE ${STRIP_FILE})
endif()
install(
  PROGRAMS ${FILE}
  DESTINATION ${CMAKE_INSTALL_BINDIR})
install_config(arangoimp)


set(FILE ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_X}/${BIN_ARANGORESTORE}${CMAKE_EXECUTABLE_SUFFIX})
set(STRIP_FILE ${STRIP_DIR}/${BIN_ARANGORESTORE}${CMAKE_EXECUTABLE_SUFFIX})
if (NOT MSVC AND CMAKE_STRIP)
  execute_process(COMMAND "rm" -f ${STRIP_FILE})
  execute_process(COMMAND "cp" ${FILE} ${STRIP_DIR})
  execute_process(COMMAND "${CMAKE_STRIP}" ${STRIP_FILE})
  set(FILE ${STRIP_FILE})
endif()
install(
  PROGRAMS ${FILE}
  DESTINATION ${CMAKE_INSTALL_BINDIR})
install_config(arangorestore)


set(FILE ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_X}/${BIN_ARANGOEXPORT}${CMAKE_EXECUTABLE_SUFFIX})
set(STRIP_FILE ${STRIP_DIR}/${BIN_ARANGOEXPORT}${CMAKE_EXECUTABLE_SUFFIX})
if (NOT MSVC AND CMAKE_STRIP)
  execute_process(COMMAND "rm" -f ${STRIP_FILE})
  execute_process(COMMAND "cp" ${FILE} ${STRIP_DIR})
  execute_process(COMMAND "${CMAKE_STRIP}" ${STRIP_FILE})
  set(FILE ${STRIP_FILE})
endif()
install(
  PROGRAMS ${FILE}
  DESTINATION ${CMAKE_INSTALL_BINDIR})
install_config(arangoexport)


set(FILE ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_X}/${BIN_ARANGOSH}${CMAKE_EXECUTABLE_SUFFIX})
set(STRIP_FILE ${STRIP_DIR}/${BIN_ARANGOSH}${CMAKE_EXECUTABLE_SUFFIX})
if (NOT MSVC AND CMAKE_STRIP)
  execute_process(COMMAND "rm" -f ${STRIP_FILE})
  execute_process(COMMAND "cp" ${FILE} ${STRIP_DIR})
  execute_process(COMMAND "${CMAKE_STRIP}" ${STRIP_FILE})
  set(FILE ${STRIP_FILE})
endif()
install(
  PROGRAMS ${FILE}
  DESTINATION ${CMAKE_INSTALL_BINDIR})
install_config(arangosh)


set(FILE ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_X}/${BIN_ARANGOVPACK}${CMAKE_EXECUTABLE_SUFFIX})
set(STRIP_FILE ${STRIP_DIR}/${BIN_ARANGOVPACK}${CMAKE_EXECUTABLE_SUFFIX})
if (NOT MSVC AND CMAKE_STRIP)
  execute_process(COMMAND "rm" -f ${STRIP_FILE})
  execute_process(COMMAND "cp" ${FILE} ${STRIP_DIR})
  execute_process(COMMAND "${CMAKE_STRIP}" ${STRIP_FILE})
  set(FILE ${STRIP_FILE})
endif()
install(
  PROGRAMS ${FILE}
  DESTINATION ${CMAKE_INSTALL_BINDIR})

install_command_alias(${BIN_ARANGOSH}
  ${CMAKE_INSTALL_BINDIR}
  foxx-manager)
install_config(foxx-manager)

