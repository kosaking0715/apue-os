file(GLOB LIB_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_LIST_DIR}/*.c")

list(REMOVE_ITEM LIB_SOURCES "${CMAKE_CURRENT_LIST_DIR}/strerror.c")

add_library(apue STATIC ${LIB_SOURCES})

target_include_directories(apue PUBLIC
  "${PROJECT_SOURCE_DIR}/include"
)

target_link_libraries(apue PUBLIC Threads::Threads m)

target_compile_definitions(apue PUBLIC _POSIX_C_SOURCE=200809L _XOPEN_SOURCE=700)
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
  target_compile_definitions(apue PUBLIC _GNU_SOURCE)
endif()
