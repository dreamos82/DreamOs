# ------------------------------------------------------------------------------
# Initialize the project.
project(initfscp C ASM)

# ------------------------------------------------------------------------------
# Set the project name.
set(PROJECT_NAME initfscp)

# ------------------------------------------------------------------------------
# Add the includes.
include_directories(inc)

# ------------------------------------------------------------------------------
# Add the source files.
set(SOURCE_FILES src/initfscp.c)

# ------------------------------------------------------------------------------
# Add the executable.
add_executable(initfscp ${SOURCE_FILES})


