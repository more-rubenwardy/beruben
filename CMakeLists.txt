# Set up project
cmake_minimum_required(VERSION 2.6)
project(beruben)

# Create directories
file(MAKE_DIRECTORY "bin")

# Interpreter
set(PROJECT_SRC
	src/main.cpp
	src/beruben.cpp
)
add_executable(${PROJECT_NAME} ${PROJECT_SRC})
set_target_properties(${PROJECT_NAME}
    PROPERTIES
    OUTPUT_NAME bin/beruben
)

# Installing
install(TARGETS ${PROJECT_NAME} DESTINATION bin)

