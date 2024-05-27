# collect all source files from the src directory
set(SOURCES "src/main.cpp" "src/backup_factory.cpp")

# create the executable
add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE src/include)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_23)
target_link_libraries(${PROJECT_NAME} argparse)

install(TARGETS ${PROJECT_NAME})

