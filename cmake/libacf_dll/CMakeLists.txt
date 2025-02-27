# Copyright (c) 2023-2024 Admenri. All rights reserved.

set(ACF_TARGET libacf_dll_wrapper)

set(LIBACF_SRCS
  template_util.h
  transfer_util.cc
  transfer_util.h
  wrapper_types.h
  )
source_group(libacf_dll FILES ${LIBACF_SRCS})

set(LIBACF_CPPTOC_SRCS
  cpptoc/browser_handler_cpptoc.cc
  cpptoc/browser_handler_cpptoc.h
  cpptoc/complete_handler_cpptoc.cc
  cpptoc/complete_handler_cpptoc.h
  cpptoc/complete_value_handler_cpptoc.cc
  cpptoc/complete_value_handler_cpptoc.h
  cpptoc/cookie_visitor_cpptoc.cc
  cpptoc/cookie_visitor_cpptoc.h
  cpptoc/cpptoc_ref_counted.h
  cpptoc/cpptoc_scoped.h
  cpptoc/dev_tools_message_observer_cpptoc.cc
  cpptoc/dev_tools_message_observer_cpptoc.h
  cpptoc/domdocument_callback_cpptoc.cc
  cpptoc/domdocument_callback_cpptoc.h
  cpptoc/domnode_callback_cpptoc.cc
  cpptoc/domnode_callback_cpptoc.h
  cpptoc/environment_handler_cpptoc.cc
  cpptoc/environment_handler_cpptoc.h
  cpptoc/execute_javascript_callback_cpptoc.cc
  cpptoc/execute_javascript_callback_cpptoc.h
  cpptoc/function_factory_cpptoc.cc
  cpptoc/function_factory_cpptoc.h
  cpptoc/function_handler_cpptoc.cc
  cpptoc/function_handler_cpptoc.h
  cpptoc/resource_handler_cpptoc.cc
  cpptoc/resource_handler_cpptoc.h
  cpptoc/resource_request_handler_cpptoc.cc
  cpptoc/resource_request_handler_cpptoc.h
  cpptoc/response_filter_cpptoc.cc
  cpptoc/response_filter_cpptoc.h
  cpptoc/string_visitor_cpptoc.cc
  cpptoc/string_visitor_cpptoc.h
  cpptoc/urlrequest_client_cpptoc.cc
  cpptoc/urlrequest_client_cpptoc.h
  cpptoc/web_message_factory_cpptoc.cc
  cpptoc/web_message_factory_cpptoc.h
  cpptoc/web_message_handler_cpptoc.cc
  cpptoc/web_message_handler_cpptoc.h
  cpptoc/web_socket_client_handler_cpptoc.cc
  cpptoc/web_socket_client_handler_cpptoc.h
  )
source_group(libacf_dll\\\\cpptoc FILES ${LIBACF_CPPTOC_SRCS})

set(LIBACF_CTOCPP_SRCS
  ctocpp/binary_value_ctocpp.cc
  ctocpp/binary_value_ctocpp.h
  ctocpp/browser_ctocpp.cc
  ctocpp/browser_ctocpp.h
  ctocpp/callback_ctocpp.cc
  ctocpp/callback_ctocpp.h
  ctocpp/context_menu_callback_ctocpp.cc
  ctocpp/context_menu_callback_ctocpp.h
  ctocpp/context_menu_model_ctocpp.cc
  ctocpp/context_menu_model_ctocpp.h
  ctocpp/context_menu_params_ctocpp.cc
  ctocpp/context_menu_params_ctocpp.h
  ctocpp/cookie_ctocpp.cc
  ctocpp/cookie_ctocpp.h
  ctocpp/cookie_manager_ctocpp.cc
  ctocpp/cookie_manager_ctocpp.h
  ctocpp/ctocpp_ref_counted.h
  ctocpp/ctocpp_scoped.h
  ctocpp/dictionary_value_ctocpp.cc
  ctocpp/dictionary_value_ctocpp.h
  ctocpp/domdocument_ctocpp.cc
  ctocpp/domdocument_ctocpp.h
  ctocpp/domnode_ctocpp.cc
  ctocpp/domnode_ctocpp.h
  ctocpp/environment_ctocpp.cc
  ctocpp/environment_ctocpp.h
  ctocpp/file_dialog_callback_ctocpp.cc
  ctocpp/file_dialog_callback_ctocpp.h
  ctocpp/frame_ctocpp.cc
  ctocpp/frame_ctocpp.h
  ctocpp/java_script_dialog_callback_ctocpp.cc
  ctocpp/java_script_dialog_callback_ctocpp.h
  ctocpp/list_value_ctocpp.cc
  ctocpp/list_value_ctocpp.h
  ctocpp/login_delegate_ctocpp.cc
  ctocpp/login_delegate_ctocpp.h
  ctocpp/new_window_delegate_ctocpp.cc
  ctocpp/new_window_delegate_ctocpp.h
  ctocpp/permission_prompt_callback_ctocpp.cc
  ctocpp/permission_prompt_callback_ctocpp.h
  ctocpp/post_data_ctocpp.cc
  ctocpp/post_data_ctocpp.h
  ctocpp/post_data_element_ctocpp.cc
  ctocpp/post_data_element_ctocpp.h
  ctocpp/profile_ctocpp.cc
  ctocpp/profile_ctocpp.h
  ctocpp/promise_resolver_ctocpp.cc
  ctocpp/promise_resolver_ctocpp.h
  ctocpp/registration_ctocpp.cc
  ctocpp/registration_ctocpp.h
  ctocpp/request_ctocpp.cc
  ctocpp/request_ctocpp.h
  ctocpp/resource_read_callback_ctocpp.cc
  ctocpp/resource_read_callback_ctocpp.h
  ctocpp/resource_skip_callback_ctocpp.cc
  ctocpp/resource_skip_callback_ctocpp.h
  ctocpp/response_ctocpp.cc
  ctocpp/response_ctocpp.h
  ctocpp/urlrequest_ctocpp.cc
  ctocpp/urlrequest_ctocpp.h
  ctocpp/value_ctocpp.cc
  ctocpp/value_ctocpp.h
  ctocpp/web_message_proxy_ctocpp.cc
  ctocpp/web_message_proxy_ctocpp.h
  ctocpp/web_socket_client_ctocpp.cc
  ctocpp/web_socket_client_ctocpp.h
  )
source_group(libacf_dll\\\\ctocpp FILES ${LIBACF_CTOCPP_SRCS})

set(LIBACF_INCLUDE_SRCS
  ../include/acf_browser.h
  ../include/acf_callback.h
  ../include/acf_context_menu.h
  ../include/acf_devtools_observer.h
  ../include/acf_dom.h
  ../include/acf_environment.h
  ../include/acf_frame.h
  ../include/acf_function_handler.h
  ../include/acf_parser.h
  ../include/acf_profile.h
  ../include/acf_registration.h
  ../include/acf_request.h
  ../include/acf_resource_request_handler.h
  ../include/acf_response.h
  ../include/acf_urlrequest.h
  ../include/acf_values.h
  ../include/acf_web_message_handler.h
  ../include/acf_websocket.h
  )
source_group(include FILES ${LIBACF_INCLUDE_SRCS})

set(LIBACF_INCLUDE_CAPI_SRCS
  ../include/capi/acf_base_capi.h
  ../include/capi/acf_browser_capi.h
  ../include/capi/acf_callback_capi.h
  ../include/capi/acf_context_menu_capi.h
  ../include/capi/acf_devtools_observer_capi.h
  ../include/capi/acf_dom_capi.h
  ../include/capi/acf_environment_capi.h
  ../include/capi/acf_frame_capi.h
  ../include/capi/acf_function_handler_capi.h
  ../include/capi/acf_parser_capi.h
  ../include/capi/acf_profile_capi.h
  ../include/capi/acf_registration_capi.h
  ../include/capi/acf_request_capi.h
  ../include/capi/acf_resource_request_handler_capi.h
  ../include/capi/acf_response_capi.h
  ../include/capi/acf_urlrequest_capi.h
  ../include/capi/acf_values_capi.h
  ../include/capi/acf_web_message_handler_capi.h
  ../include/capi/acf_websocket_capi.h
  )
source_group(include\\\\capi FILES ${LIBACF_INCLUDE_CAPI_SRCS})

set(LIBACF_INCLUDE_INTERNAL_SRCS
  ../include/internal/acf_defines.h
  ../include/internal/acf_enums.h
  ../include/internal/acf_scoped_refptr.h
  ../include/internal/acf_string.h
  ../include/internal/acf_string_list.h
  ../include/internal/acf_string_map.h
  ../include/internal/acf_string_multimap.h
  ../include/internal/acf_string_types.h
  ../include/internal/acf_string_wrappers.h
  ../include/internal/acf_types.h
  ../include/internal/acf_types_wrappers.h
  )
source_group(include\\\\internal FILES ${LIBACF_INCLUDE_INTERNAL_SRCS})

set(LIBACF_WRAPPER_SRCS
  wrapper/libacf_dll_wrapper.cc
  )
source_group(libacf_dll\\\\wrapper FILES ${LIBACF_WRAPPER_SRCS})

add_library(${ACF_TARGET}
  ${LIBACF_SRCS}
  ${LIBACF_CPPTOC_SRCS}
  ${LIBACF_CTOCPP_SRCS}
  ${LIBACF_INCLUDE_SRCS}
  ${LIBACF_INCLUDE_CAPI_SRCS}
  ${LIBACF_INCLUDE_INTERNAL_SRCS}
  ${LIBACF_WRAPPER_SRCS}
  )
SET_LIBRARY_TARGET_PROPERTIES(${ACF_TARGET})

# Creating the CEF wrapper library. Do not define this for dependent targets.
target_compile_definitions(${ACF_TARGET} PRIVATE -DWRAPPING_ACF_SHARED)

# Remove the default "lib" prefix from the resulting library.
set_target_properties(${ACF_TARGET} PROPERTIES PREFIX "")
