cmake_minimum_required(VERSION 3.16)

project(lickometer_black_mesa LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(LICKOMETER_USE_ARDUINO_STUBS
       "Use local Arduino/library compatibility headers for host builds" ON)

add_executable(lickometer
    main.cpp
    src/cap_sensor/cap_sensor.cpp
    src/mpr_manager/mpr_manager.cpp
    src/sd_writer/sd_writer.cpp
)

target_include_directories(lickometer PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}"
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
)

if(LICKOMETER_USE_ARDUINO_STUBS)
    target_sources(lickometer PRIVATE
        cmake/arduino_stubs/arduino_stubs.cpp
    )

    target_include_directories(lickometer SYSTEM PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/cmake/arduino_stubs"
    )

    target_compile_definitions(lickometer PRIVATE
        LICKOMETER_HOST_BUILD=1
    )
else()
    set(ARDUINO_INCLUDE_DIRS "" CACHE STRING
        "Semicolon-separated include paths for Arduino core and libraries")

    if(NOT ARDUINO_INCLUDE_DIRS)
        message(FATAL_ERROR
            "Set ARDUINO_INCLUDE_DIRS when LICKOMETER_USE_ARDUINO_STUBS is OFF")
    endif()

    target_include_directories(lickometer SYSTEM PRIVATE
        ${ARDUINO_INCLUDE_DIRS}
    )
endif()
