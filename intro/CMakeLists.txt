add_executable(intro_hello hello.c)

target_link_libraries(intro_hello PRIVATE apue Threads::Threads m)
target_include_directories(intro_hello PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_compile_definitions(intro_hello PRIVATE _FILE_OFFSET_BITS=64)
