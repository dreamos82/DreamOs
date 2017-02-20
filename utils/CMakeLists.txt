# ------------------------------------------------------------------------------
# Initialize the project.
project(initfscp C ASM)

# ------------------------------------------------------------------------------
# Set the project name.
set(PROJECT_NAME initfscp)

# ------------------------------------------------------------------------------
# Add the includes.
include_directories(${CMAKE_SOURCE_DIR}/utils/include)

# ------------------------------------------------------------------------------
# Add the source files.
set(SOURCE_FILES ${CMAKE_SOURCE_DIR}/utils/initfscp.c)

# ------------------------------------------------------------------------------
# Add the executable.
add_executable(initfscp ${SOURCE_FILES})


