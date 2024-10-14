cmake_minimum_required(VERSION 3.16)
project(UTCore VERSION 0.0.10)

option(BUILD_EXAMPLES "Build examples" ON)
option(BUILD_TESTING "Build the testing tree" ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pthread")

file(GLOB HEADERS include/ut/core/*.h)
file(GLOB SOURCES include/ut/core/*.cpp)

add_library(${PROJECT_NAME} SHARED ${SOURCES})
add_library(UT::Core ALIAS ${PROJECT_NAME})

set_target_properties(
    ${PROJECT_NAME} PROPERTIES
        PUBLIC_HEADER "${HEADERS}"
        EXPORT_NAME Core
)

target_include_directories(
    ${PROJECT_NAME} PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include/ut/core>
)

install(
    TARGETS ${PROJECT_NAME}
    EXPORT ${PROJECT_NAME}Targets
    PUBLIC_HEADER
        DESTINATION "include/ut/core"
)

install(
    EXPORT ${PROJECT_NAME}Targets
    NAMESPACE UT::
    FILE "${PROJECT_NAME}Targets.cmake"
    DESTINATION "lib/cmake/${PROJECT_NAME}"
)

# Generate local "${PROJECT_NAME}Targets.cmake"
file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Targets.cmake)
export(
    EXPORT ${PROJECT_NAME}Targets
    FILE ${PROJECT_NAME}Targets.cmake
)

include(CMakePackageConfigHelpers)

configure_package_config_file(
    ${CMAKE_CURRENT_LIST_DIR}/config.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
    INSTALL_DESTINATION "lib/cmake/${PROJECT_NAME}"
)

write_basic_package_version_file(
    ${PROJECT_NAME}ConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion
)

install(
    FILES
        ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
        ${PROJECT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
    DESTINATION "lib/cmake/${PROJECT_NAME}"
)

if(BUILD_EXAMPLES)
    add_subdirectory(example)
endif()

if(BUILD_TESTING)
    include(CTest)
    target_compile_options(${PROJECT_NAME} PRIVATE -coverage)
    target_link_options(${PROJECT_NAME} PRIVATE -coverage)
    add_subdirectory(tests)
    add_test(NAME FunctionDelegateCall COMMAND FunctionDelegateCallTest)
    add_test(NAME MethodDelegateCall COMMAND MethodDelegateCallTest)
    add_test(NAME LambdaDelegateCall COMMAND LambdaDelegateCallTest)
    add_test(NAME RemovedMethodCall COMMAND RemovedMethodCallTest)
    add_test(NAME RemovedMethodCallV2 COMMAND RemovedMethodCallV2Test)
    add_test(NAME RemovedLambdaCall COMMAND RemovedLambdaCallTest)
endif()