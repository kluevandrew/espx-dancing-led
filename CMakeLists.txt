# !!! WARNING !!! AUTO-GENERATED FILE, PLEASE DO NOT MODIFY IT AND USE
# https://docs.platformio.org/page/projectconf/section_env_build.html#build-flags
#
# If you need to override existing CMake configuration or add extra,
# please create `CMakeListsUser.txt` in the root of project.
# The `CMakeListsUser.txt` will not be overwritten by PlatformIO.

cmake_minimum_required(VERSION 3.2)
project(espx-dancing-led)

include(CMakeListsPrivate.${BUILD_TARGET}.txt)

if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/CMakeListsUser.txt)
include(CMakeListsUser.txt)
endif()

add_custom_target(
        PLATFORMIO_DEPS_INSTALL ALL
        COMMAND ${PLATFORMIO_CMD} lib -e esp8266 -e esp32 install
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

add_custom_target(
        PLATFORMIO_DEPS_UPDATE ALL
        COMMAND ${PLATFORMIO_CMD} lib -e esp8266 -e esp32 update
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

# > PLATFORMIO_BUILD
add_custom_target(
        PLATFORMIO_BUILD_8266 ALL
        COMMAND ${PLATFORMIO_CMD} -f -c clion run --environment esp8266
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)
add_custom_target(
        PLATFORMIO_BUILD_32 ALL
        COMMAND ${PLATFORMIO_CMD} -f -c clion run --environment esp32
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)
add_custom_target(
    PLATFORMIO_BUILD ALL
    COMMAND ${PLATFORMIO_CMD} -f -c clion run --environment esp8266 --environment esp32
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)
# < BUILD

# > PLATFORMIO_BUILD_VERBOSE
add_custom_target(
        PLATFORMIO_BUILD_VERBOSE_8266 ALL
        COMMAND ${PLATFORMIO_CMD} -f -c clion run --verbose --environment esp8266
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)
add_custom_target(
        PLATFORMIO_BUILD_VERBOSE_32 ALL
        COMMAND ${PLATFORMIO_CMD} -f -c clion run --verbose --environment esp32
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)
add_custom_target(
    PLATFORMIO_BUILD_VERBOSE ALL
    COMMAND ${PLATFORMIO_CMD} -f -c clion run --verbose --environment esp8266 --environment esp32
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)
# < PLATFORMIO_BUILD_VERBOSE

# > PLATFORMIO_UPLOAD
add_custom_target(
        PLATFORMIO_UPLOAD_8266 ALL
        COMMAND ${PLATFORMIO_CMD} -f -c clion run --target upload --environment esp8266
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)
add_custom_target(
        PLATFORMIO_UPLOAD_8266_RELEASE ALL
        COMMAND ${PLATFORMIO_CMD} -f -c clion run --target upload --environment esp8266:release
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)
add_custom_target(
        PLATFORMIO_UPLOAD_8266_OTA ALL
        COMMAND ${PLATFORMIO_CMD} -f -c clion run --target upload --environment esp8266:ota
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)
add_custom_target(
        PLATFORMIO_UPLOAD_8266_OTA_RELEASE ALL
        COMMAND ${PLATFORMIO_CMD} -f -c clion run --target upload --environment esp8266:ota:release
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)
add_custom_target(
        PLATFORMIO_UPLOAD_32 ALL
        COMMAND ${PLATFORMIO_CMD} -f -c clion run --target upload --environment esp32
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)
add_custom_target(
        PLATFORMIO_UPLOAD_32_RELEASE ALL
        COMMAND ${PLATFORMIO_CMD} -f -c clion run --target upload --environment esp32:release
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)
add_custom_target(
        PLATFORMIO_UPLOAD_32_OTA ALL
        COMMAND ${PLATFORMIO_CMD} -f -c clion run --target upload --environment esp32:ota
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)
add_custom_target(
        PLATFORMIO_UPLOAD_32_OTA_RELEASE ALL
        COMMAND ${PLATFORMIO_CMD} -f -c clion run --target upload --environment esp32:ota:release
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)
add_custom_target(
    PLATFORMIO_UPLOAD ALL
    COMMAND ${PLATFORMIO_CMD} -f -c clion run --target upload --environment esp8266 --environment esp32
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)
add_custom_target(
    PLATFORMIO_UPLOAD_RELEASE ALL
    COMMAND ${PLATFORMIO_CMD} -f -c clion run --target upload --environment esp8266:release --environment esp32:release
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)
add_custom_target(
        PLATFORMIO_UPLOAD_OTA ALL
        COMMAND ${PLATFORMIO_CMD} -f -c clion run --target upload --environment esp8266:ota --environment esp32:ota
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)
add_custom_target(
        PLATFORMIO_UPLOAD_OTA_RELEASE ALL
        COMMAND ${PLATFORMIO_CMD} -f -c clion run --target upload --environment esp8266:ota:release --environment esp32:ota:release
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)
# < PLATFORMIO_UPLOAD

# > PLATFORMIO_UPLOADFS
add_custom_target(
        PLATFORMIO_UPLOADFS_8266 ALL
        COMMAND ${PLATFORMIO_CMD} -f -c clion run --target uploadfs --environment esp8266
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)
add_custom_target(
        PLATFORMIO_UPLOADFS_32 ALL
        COMMAND ${PLATFORMIO_CMD} -f -c clion run --target uploadfs --environment esp32
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)
add_custom_target(
        PLATFORMIO_UPLOADFS ALL
        COMMAND ${PLATFORMIO_CMD} -f -c clion run --target uploadfs --environment esp8266 --environment esp32
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)
# < PLATFORMIO_UPLOAD

add_custom_target(
    PLATFORMIO_CLEAN ALL
    COMMAND ${PLATFORMIO_CMD} -f -c clion run --target clean
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

add_custom_target(
    PLATFORMIO_MONITOR ALL
    COMMAND ${PLATFORMIO_CMD} -f -c clion device monitor
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

add_custom_target(
    PLATFORMIO_TEST ALL
    COMMAND ${PLATFORMIO_CMD} -f -c clion test
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

add_custom_target(
    PLATFORMIO_PROGRAM ALL
    COMMAND ${PLATFORMIO_CMD} -f -c clion run --target program
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

add_custom_target(
    PLATFORMIO_UPDATE_ALL ALL
    COMMAND ${PLATFORMIO_CMD} -f -c clion update
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

add_custom_target(
    PLATFORMIO_REBUILD_PROJECT_INDEX ALL
    COMMAND ${PLATFORMIO_CMD} -f -c clion init --ide clion
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

add_executable(${PROJECT_NAME} ${SRC_LIST})