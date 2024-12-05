cmake_minimum_required(VERSION 3.28)
project(SurvivalsGame LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.x
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

# Automatically add all .cpp files in src directory
file(GLOB_RECURSE SOURCES "src/*.cpp")

add_executable(main ${SOURCES})
target_link_libraries(main PRIVATE sfml-graphics sfml-window sfml-system sfml-audio)
target_compile_features(main PRIVATE cxx_std_17)

# Добавьте после команды add_executable
add_custom_command(TARGET main
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E remove_directory "$<TARGET_FILE_DIR:main>/Assets"
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_SOURCE_DIR}/src/Assets" "$<TARGET_FILE_DIR:main>/Assets"
    COMMENT "Копирование папки Assets в bin"
)


if(WIN32)
    # Путь к openal32.dll в вашем проекте
    set(OPENAL_DLL_PATH "${CMAKE_SOURCE_DIR}/ThirdParty/OpenAL/bin/openal32.dll")

    add_custom_command(
        TARGET main
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${OPENAL_DLL_PATH}"
            "$<TARGET_FILE_DIR:main>"
    )
endif()