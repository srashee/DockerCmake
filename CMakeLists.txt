cmake_minimum_required(VERSION 1.0)

# Defining the project
project(SelectServer)

# Creating source variables with link to the source files
set (SOURCES
    src/selectServer.c
)

# Adding definition for the binary
add_executable(SelectServer ${SOURCES})

# Setting include directories
target_include_directories(SelectServer
    PRIVATE
        $(PROJECT_SOURCE_DIR)/windows
        $(PROJECT_SOURCE_DIR)/linux
)
