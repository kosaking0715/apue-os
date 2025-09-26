cmake_minimum_required(VERSION 3.16)
project(apue-os C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

find_package(Threads REQUIRED)

add_compile_definitions(_POSIX_C_SOURCE=200809L _XOPEN_SOURCE=700)
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
  add_compile_definitions(_GNU_SOURCE)
endif()

set(APUE_BIN_DIR "${CMAKE_BINARY_DIR}/bin")

add_subdirectory(lib)
add_subdirectory(intro)

include_directories("${PROJECT_SOURCE_DIR}/include")

set(APUE_SKIP_LIST
  "${PROJECT_SOURCE_DIR}/environ/cmd2.c"
  "${PROJECT_SOURCE_DIR}/signals/setops.c"
  "${PROJECT_SOURCE_DIR}/threads/barrier.c"
  "${PROJECT_SOURCE_DIR}/threadctl/timeout.c"
)

function(_apue_should_skip out_var abspath)
  set(skip FALSE)
  foreach(item IN LISTS APUE_SKIP_LIST)
    if(item STREQUAL "${abspath}")
      set(skip TRUE)
      break()
    endif()
  endforeach()
  set(${out_var} ${skip} PARENT_SCOPE)
endfunction()

function(_apue_has_main out_var abspath)
  file(READ "${abspath}" _apue_file_content)

  string(REGEX MATCH
    "^[ \t]*([A-Za-z_][A-Za-z0-9_]*[ \t*]+)?main[ \t\r\n]*\\("
    _apue_match
    "${_apue_file_content}"
  )

  if(_apue_match)
    set(${out_var} TRUE PARENT_SCOPE)
  else()
    set(${out_var} FALSE PARENT_SCOPE)
  endif()
endfunction()

function(add_apue_directory DIRNAME)
  file(GLOB SRC_LIST CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/${DIRNAME}/*.c")
  if(NOT SRC_LIST)
    message(STATUS "No C sources in ${DIRNAME}")
    return()
  endif()

  set(HELPER_SRCS)
  set(MAIN_SRCS)

  foreach(src ${SRC_LIST})
    get_filename_component(abs "${src}" ABSOLUTE)
    _apue_should_skip(SHOULD_SKIP "${abs}")
    if(SHOULD_SKIP)
      message(STATUS "Skip ${src}")
      continue()
    endif()

    _apue_has_main(HAS_MAIN "${abs}")
    if(HAS_MAIN)
      list(APPEND MAIN_SRCS "${abs}")
    else()
      list(APPEND HELPER_SRCS "${abs}")
    endif()
  endforeach()

  set(objlib "")
  if(HELPER_SRCS)
    string(REPLACE "/" "_" objlib "objs_${DIRNAME}")
    add_library(${objlib} OBJECT ${HELPER_SRCS})
    target_compile_definitions(${objlib} PRIVATE _FILE_OFFSET_BITS=64)
    target_include_directories(${objlib} PRIVATE "${PROJECT_SOURCE_DIR}/include")
  endif()

  foreach(src ${MAIN_SRCS})
    get_filename_component(name_we "${src}" NAME_WE)
    string(REPLACE "/" "_" target "${DIRNAME}_${name_we}")

    add_executable(${target} "${src}")
    target_link_libraries(${target} PRIVATE apue Threads::Threads m)
    if(objlib)
      target_link_libraries(${target} PRIVATE ${objlib})
    endif()
    set_target_properties(${target} PROPERTIES
      RUNTIME_OUTPUT_DIRECTORY "${APUE_BIN_DIR}/${DIRNAME}"
    )
    target_compile_definitions(${target} PRIVATE _FILE_OFFSET_BITS=64)
    target_include_directories(${target} PRIVATE "${PROJECT_SOURCE_DIR}/include")
  endforeach()
endfunction()

set(APUE_SUBDIRS
  fileio
  filedir
  environ
  proc
  signals
  ipc1
  ipc2
  sockets
  threads
  threadctl
  stdio
  standards
  advio
  pty
  daemons
  termios
  printer
  relation
  db
  figlinks
)

foreach(d IN LISTS APUE_SUBDIRS)
  add_apue_directory(${d})
endforeach()

message(STATUS "Executables will be placed under: ${APUE_BIN_DIR}")
