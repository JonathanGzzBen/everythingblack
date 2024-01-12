cmake_minimum_required(VERSION 3.11)

project(ApagarMonitores)

add_executable(${PROJECT_NAME} src/main.c)

set_target_properties(${PROJECT_NAME} PROPERTIES C_STANDARD 99 CXX_STANDARD_REQUIRED ON)
