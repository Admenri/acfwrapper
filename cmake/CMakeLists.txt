# Copyright (c) 2023-2024 Admenri. All rights reserved. 

# For VS2019 and Xcode 12+ support.
cmake_minimum_required(VERSION 3.19)

# Only generate Debug and Release configuration types.
set(CMAKE_CONFIGURATION_TYPES Debug Release)

# Project name.
# TODO: Change this line to match your project name when you copy this file.
project(acf)

# Use folders in the resulting project files.
set_property(GLOBAL PROPERTY OS_FOLDERS ON)

set(ACF_ROOT "${CMAKE_CURRENT_SOURCE_DIR}")
set(ACF_LIBTYPE STATIC)

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
set(ACF_LIB_RELEASE "${ACF_ROOT}/lib/x64/libacf.lib")
elseif(CMAKE_SIZEOF_VOID_P EQUAL 4)
set(ACF_LIB_RELEASE "${ACF_ROOT}/lib/x86/libacf.lib")
endif()

# Path to the include directory.
set(ACF_INCLUDE_PATH "${ACF_ROOT}")

# Path to the libcef_dll_wrapper target.
set(ACF_LIBACF_DLL_WRAPPER_PATH "${ACF_ROOT}/libacf_dll")
set(ACF_ACFCLIENT_PATH "${ACF_ROOT}/acfclient")

# Consumers using different runtime types may want to pass different flags
set(ACF_RUNTIME_LIBRARY_FLAG "/MT" CACHE STRING "Optional flag specifying which runtime to use")
if (ACF_RUNTIME_LIBRARY_FLAG)
  list(APPEND ACF_COMPILER_FLAGS ${ACF_RUNTIME_LIBRARY_FLAG})
endif()

list(APPEND ACF_COMPILER_FLAGS
  /MP           # Multiprocess compilation
  /Gy           # Enable function-level linking
  /W4           # Warning level 4
  /WX           # Treat warnings as errors
  /wd4100       # Ignore "unreferenced formal parameter" warning
  /wd4127       # Ignore "conditional expression is constant" warning
  /wd4244       # Ignore "conversion possible loss of data" warning
  /wd4324       # Ignore "structure was padded due to alignment specifier" warning
  /wd4481       # Ignore "nonstandard extension used: override" warning
  /wd4512       # Ignore "assignment operator could not be generated" warning
  /wd4701       # Ignore "potentially uninitialized local variable" warning
  /wd4702       # Ignore "unreachable code" warning
  /wd4996       # Ignore "function or variable may be unsafe" warning
)
list(APPEND ACF_CXX_COMPILER_FLAGS
  /std:c++17    # Use the C++17 language standard
)
list(APPEND ACF_EXE_LINKER_FLAGS
  /MANIFEST:NO        # No default manifest (see ADD_WINDOWS_MANIFEST macro usage)
  /LARGEADDRESSAWARE  # Allow 32-bit processes to access 3GB of RAM
)
list(APPEND ACF_COMPILER_DEFINES
  WIN32 _WIN32 _WINDOWS             # Windows platform
  UNICODE _UNICODE                  # Unicode build
  # Targeting Windows 10. We can't say `=_WIN32_WINNT_WIN10` here because
  # some files do `#if WINVER < 0x0600` without including windows.h before,
  # and then _WIN32_WINNT_WIN10 isn't yet known to be 0x0A00.
  WINVER=0x0A00
  _WIN32_WINNT=0x0A00
  NTDDI_VERSION=NTDDI_WIN10_FE
  NOMINMAX                          # Use the standard's templated min/max
  WIN32_LEAN_AND_MEAN               # Exclude less common API declarations
  _HAS_EXCEPTIONS=0                 # Disable exceptions
)

# Add a logical target that can be used to link the specified libraries into an
# executable target.
macro(ADD_LOGICAL_TARGET target release_lib)
  add_library(${target} ${ACF_LIBTYPE} IMPORTED)
  set_target_properties(${target} PROPERTIES
    IMPORTED_LOCATION "${release_lib}"
  )
endmacro()

# Set common target properties. Use SET_LIBRARY_TARGET_PROPERTIES() or
# SET_EXECUTABLE_TARGET_PROPERTIES() instead of calling this macro directly.
macro(SET_COMMON_TARGET_PROPERTIES target)
  # Compile flags.
  target_compile_options(${target} PRIVATE ${ACF_COMPILER_FLAGS} ${ACF_CXX_COMPILER_FLAGS})

  # Include directories.
  target_include_directories(${target} PRIVATE ${ACF_INCLUDE_PATH})

  # Compile definitions.
  target_compile_definitions(${target} PRIVATE ${ACF_COMPILER_DEFINES})
endmacro()

# Set library-specific properties.
macro(SET_LIBRARY_TARGET_PROPERTIES target)
  SET_COMMON_TARGET_PROPERTIES(${target})
endmacro()

# Set executable-specific properties.
macro(SET_EXECUTABLE_TARGET_PROPERTIES target)
  SET_COMMON_TARGET_PROPERTIES(${target})
endmacro()

# Logical target used to link the libacf library.
ADD_LOGICAL_TARGET("libacf_lib" "${ACF_LIB_RELEASE}")

# Include the libacf_dll_wrapper target.
# Comes from the libcef_dll/CMakeLists.txt file in the binary distribution
# directory.
add_subdirectory(${ACF_LIBACF_DLL_WRAPPER_PATH} libacf_dll_wrapper)

add_subdirectory(${ACF_ACFCLIENT_PATH} acfclient)
