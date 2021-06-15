project(thread_pool_test)

add_executable(test_main
  main.cpp
)

target_include_directories(test_main
  PUBLIC
    ${thread_pool_SOURCE_DIR}/include
)

find_package(Threads)
target_link_libraries(test_main
  PRIVATE
    Threads::Threads
)
