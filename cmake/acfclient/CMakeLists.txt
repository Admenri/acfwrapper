# Copyright (c) 2023 KuanGu Authors.

# Target executable names.
set(ACF_TARGET "acfclient")

# acfclient sources.
set(ACFCLIENT_SRCS
  client_window.cc
  client_window.h
  main.cc
  message_loop_manager.cc
  message_loop_manager.h
  )
source_group(acfclient FILES ${ACFCLIENT_SRCS})

set(ACFCLIENT_MAIN_SRCS
  ${ACFCLIENT_SRCS}
  )

#
# Windows configuration.
#

set(ACFCLIENT_SRCS
  ${ACFCLIENT_MAIN_SRCS}
)

# Executable target.
add_executable(${ACF_TARGET} ${ACFCLIENT_SRCS})
SET_EXECUTABLE_TARGET_PROPERTIES(${ACF_TARGET})
add_dependencies(${ACF_TARGET} libacf_dll_wrapper)
target_link_libraries(${ACF_TARGET} libacf_lib libacf_dll_wrapper)
target_include_directories(${ACF_TARGET} PRIVATE "${ACF_ROOT}/acfclient")
