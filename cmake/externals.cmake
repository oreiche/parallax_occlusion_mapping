cmake_minimum_required(VERSION 3.18)

macro(setup_externals)
    if (NOT EXTERNALS_STAGE_DIR)
        set(EXTERNALS_STAGE_DIR "${CMAKE_BINARY_DIR}/externals")
        file(MAKE_DIRECTORY "${EXTERNALS_STAGE_DIR}")
        # Convenience link to stage directory from source directory
        file(CREATE_LINK "${EXTERNALS_STAGE_DIR}"
                        "${PROJECT_SOURCE_DIR}/.externals" SYMBOLIC)
    endif()
    if (NOT EXTERNALS_FETCH_DIR)
        set(EXTERNALS_FETCH_DIR "${CMAKE_BINARY_DIR}/downloads")
        file(MAKE_DIRECTORY "${EXTERNALS_FETCH_DIR}")
    endif()
endmacro()

# Create external from HTTP archive.
# @param NAME    <string> Name of the external to create.
# @param URL     <string> URL for downlading archive.
# @param SHA256  <string> SHA256 hash of archive.
# @param STRIP   <string> Prefix to strip archive path. (optional)
# @param TIMEOUT <int>    Timeout for downloading archive. (optional)
# @param VERBOSE          Show download progress. (optional)
# @return Sets variable ${NAME}_ROOT to root directory of unpacked archive.
macro(http_archive)
    setup_externals()

    set(options VERBOSE)
    set(oneValueArgs NAME URL SHA256 STRIP TIMEOUT)
    cmake_parse_arguments(HTTP_ARCHIVE "${options}" "${oneValueArgs}" "" ${ARGN})

    if (NOT HTTP_ARCHIVE_NAME OR NOT HTTP_ARCHIVE_URL OR NOT HTTP_ARCHIVE_SHA256)
        message(FATAL_ERROR "Missing mandatory argument")
    endif()
    if (NOT HTTP_ARCHIVE_TIMEOUT)
        set(HTTP_ARCHIVE_TIMEOUT 60)
    endif()
    if (HTTP_ARCHIVE_VERBOSE)
        set(DOWNLOAD_EXTRA_ARGS SHOW_PROGRESS)
    endif()

    set(DOWNLOAD_FILE "${EXTERNALS_FETCH_DIR}/${HTTP_ARCHIVE_NAME}_archive")
    set(EXTRACT_PATH "${EXTERNALS_FETCH_DIR}/${HTTP_ARCHIVE_NAME}")
    set(STAGE_PATH "${EXTERNALS_STAGE_DIR}/${HTTP_ARCHIVE_NAME}")

    message(STATUS "Downloading external ${HTTP_ARCHIVE_NAME}")

    file(DOWNLOAD "${HTTP_ARCHIVE_URL}"
        ${DOWNLOAD_FILE}
        ${DOWNLOAD_EXTRA_ARGS}
        TIMEOUT ${HTTP_ARCHIVE_TIMEOUT}
        EXPECTED_HASH SHA256=${HTTP_ARCHIVE_SHA256}
        STATUS RETVAL)
    list(GET RETVAL 0 EXIT_CODE)
    list(GET RETVAL 1 EXIT_MESSAGE)

    if (${EXIT_CODE} EQUAL 0)
        # Download call successful, file might have existed before
        if ("${EXIT_MESSAGE}" STREQUAL "\"No error\"")
            # New file was successfully downloaded
            message(STATUS "Extracting ${HTTP_ARCHIVE_NAME} to external")
            file(ARCHIVE_EXTRACT
                INPUT "${DOWNLOAD_FILE}"
                DESTINATION "${EXTRACT_PATH}")
            file(REMOVE_RECURSE "${STAGE_PATH}")
            file(RENAME "${EXTRACT_PATH}/${HTTP_ARCHIVE_STRIP}"
                        "${STAGE_PATH}")
            file(REMOVE_RECURSE "${EXTRACT_PATH}")
        else()
            message(STATUS "External ${HTTP_ARCHIVE_NAME} is up to date")
        endif()
        set(${HTTP_ARCHIVE_NAME}_ROOT "${STAGE_PATH}")
    else()
        message(FATAL_ERROR "Failed to download ${HTTP_ARCHIVE_NAME} with ${RETVAL}")
    endif()
endmacro()
