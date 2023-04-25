set(EXAMPLE_PROJECT ${PROJECT_NAME}Example)

add_executable(${EXAMPLE_PROJECT} main.cpp)

target_link_libraries(${EXAMPLE_PROJECT} PUBLIC ${PROJECT_NAME})