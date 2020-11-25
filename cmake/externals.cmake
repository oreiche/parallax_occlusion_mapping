cmake_minimum_required(VERSION 3.18)

macro(setup_externals)
    set(EXTERNALS_ROOT_DIR "${CMAKE_BINARY_DIR}")
    set(EXTERNALS_STAGE_NAME "externals")
    set(EXTERNALS_FETCH_NAME "downloads")

    if ("${CMAKE_PROJECT_NAME}" STREQUAL "CMAKE_TRY_COMPILE")
        # Search for existing externals root directory in parent directories
        while (NOT EXISTS "${EXTERNALS_ROOT_DIR}/${EXTERNALS_STAGE_NAME}")
            get_filename_component(EXTERNALS_PARENT_DIR
                                   "${EXTERNALS_ROOT_DIR}" DIRECTORY)
            if ("${EXTERNALS_PARENT_DIR}" STREQUAL "${EXTERNALS_ROOT_DIR}")
                message(FATAL_ERROR "Could not find externals root directory.")
            endif()
            set(EXTERNALS_ROOT_DIR "${EXTERNALS_PARENT_DIR}")
        endwhile()
    endif()

    if (NOT EXTERNALS_STAGE_DIR)
        set(EXTERNALS_STAGE_DIR "${EXTERNALS_ROOT_DIR}/${EXTERNALS_STAGE_NAME}")
        file(MAKE_DIRECTORY "${EXTERNALS_STAGE_DIR}")
        # Convenience link to stage directory from source directory
        file(CREATE_LINK "${EXTERNALS_STAGE_DIR}"
                        "${PROJECT_SOURCE_DIR}/.externals" SYMBOLIC)
    endif()

    if (NOT EXTERNALS_FETCH_DIR)
        set(EXTERNALS_FETCH_DIR "${EXTERNALS_ROOT_DIR}/${EXTERNALS_FETCH_NAME}")
        file(MAKE_DIRECTORY "${EXTERNALS_FETCH_DIR}")
    endif()
endmacro()

# Create external from HTTP archive.
# @param NAME     <string>  Name of the external to create.
# @param URL      <string>  URL for downlading archive.
# @param SHA256   <string>  SHA256 hash of archive. (optional)
# @param STRIP    <string>  Prefix to strip archive path. (optional)
# @param PATTERNS <strings> Select files and directories to extract. (optional)
# @param TIMEOUT  <int>     Timeout for downloading archive. (optional)
# @param VERBOSE            Show download progress. (optional)
# @return Sets variable ${NAME}_ROOT to root directory of unpacked archive.
macro(http_archive)
    setup_externals()

    set(options VERBOSE)
    set(oneValueArgs NAME URL SHA256 STRIP TIMEOUT)
    set(multiValueArgs PATTERNS)
    cmake_parse_arguments(HTTP_ARCHIVE
         "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    if (NOT HTTP_ARCHIVE_NAME OR NOT HTTP_ARCHIVE_URL)
        message(FATAL_ERROR "[externals] Missing mandatory argument")
    endif()

    unset(DOWNLOAD_EXTRA_ARGS)
    unset(EXTRACT_EXTRA_ARGS)

    if (HTTP_ARCHIVE_SHA256)
        set(DOWNLOAD_EXTRA_ARGS ${DOWNLOAD_EXTRA_ARGS}
            EXPECTED_HASH SHA256=${HTTP_ARCHIVE_SHA256})
    endif()
    if (HTTP_ARCHIVE_TIMEOUT)
        set(DOWNLOAD_EXTRA_ARGS ${DOWNLOAD_EXTRA_ARGS}
            TIMEOUT ${HTTP_ARCHIVE_TIMEOUT})
    endif()
    if (HTTP_ARCHIVE_VERBOSE)
        set(DOWNLOAD_EXTRA_ARGS ${DOWNLOAD_EXTRA_ARGS}
            SHOW_PROGRESS)
    endif()
    if (HTTP_ARCHIVE_PATTERNS)
        set(EXTRACT_EXTRA_ARGS ${EXTRACT_EXTRA_ARGS}
            PATTERNS ${HTTP_ARCHIVE_PATTERNS})
    endif()

    set(DOWNLOAD_FILE "${EXTERNALS_FETCH_DIR}/${HTTP_ARCHIVE_NAME}_archive")
    set(EXTRACT_PATH "${EXTERNALS_FETCH_DIR}/${HTTP_ARCHIVE_NAME}")
    set(STAGE_PATH "${EXTERNALS_STAGE_DIR}/${HTTP_ARCHIVE_NAME}")

    message(STATUS "[externals] Downloading ${HTTP_ARCHIVE_NAME}")

    file(DOWNLOAD "${HTTP_ARCHIVE_URL}"
        ${DOWNLOAD_FILE}
        ${DOWNLOAD_EXTRA_ARGS}
        STATUS RETVAL)
    list(GET RETVAL 0 EXIT_CODE)
    list(GET RETVAL 1 EXIT_MESSAGE)

    if (${EXIT_CODE} EQUAL 0)
        # Download call successful, file might have existed before
        if ("${EXIT_MESSAGE}" STREQUAL "\"No error\""
            OR NOT EXISTS "${STAGE_PATH}")
            # New file was s[uccessfully downloaded
            message(STATUS "[externals] Extracting ${HTTP_ARCHIVE_NAME}")
            file(ARCHIVE_EXTRACT
                INPUT "${DOWNLOAD_FILE}"
                DESTINATION "${EXTRACT_PATH}"
                ${EXTRACT_EXTRA_ARGS})
            file(REMOVE_RECURSE "${STAGE_PATH}")
            file(RENAME "${EXTRACT_PATH}/${HTTP_ARCHIVE_STRIP}"
                        "${STAGE_PATH}")
            file(REMOVE_RECURSE "${EXTRACT_PATH}")
        else()
            message(STATUS "[externals] ${HTTP_ARCHIVE_NAME} is up to date")
        endif()
        set(${HTTP_ARCHIVE_NAME}_ROOT "${STAGE_PATH}")
    else()
        message(FATAL_ERROR "[externals] Download failed with ${RETVAL}")
    endif()
endmacro()
