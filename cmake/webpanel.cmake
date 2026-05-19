set(WEBPANEL_DIR "${PROJECT_SOURCE_DIR}/webpanel")
set(WEBPANEL_DIST_DIR "${WEBPANEL_DIR}/dist")

find_program(NPM_EXECUTABLE npm REQUIRED)

set(WEBPANEL_NPM_STAMP "${WEBPANEL_DIR}/node_modules/.npm-ci.stamp")
set(WEBPANEL_BUILD_STAMP "${CMAKE_CURRENT_BINARY_DIR}/webpanel-build.stamp")

file(GLOB_RECURSE WEBPANEL_SOURCES CONFIGURE_DEPENDS
    "${WEBPANEL_DIR}/src/*"
)

add_custom_command(
    OUTPUT "${WEBPANEL_NPM_STAMP}"
    COMMAND "${NPM_EXECUTABLE}" ci --no-audit --no-fund
    COMMAND "${CMAKE_COMMAND}" -E touch "${WEBPANEL_NPM_STAMP}"
    WORKING_DIRECTORY "${WEBPANEL_DIR}"
    DEPENDS
        "${WEBPANEL_DIR}/package.json"
        "${WEBPANEL_DIR}/package-lock.json"
    COMMENT "Installing webpanel npm dependencies"
    VERBATIM
)

add_custom_target(webpanel_npm_deps
    DEPENDS "${WEBPANEL_NPM_STAMP}"
)

add_custom_command(
    OUTPUT "${WEBPANEL_BUILD_STAMP}"
    COMMAND "${NPM_EXECUTABLE}" run build
    COMMAND "${CMAKE_COMMAND}" -E touch "${WEBPANEL_BUILD_STAMP}"
    WORKING_DIRECTORY "${WEBPANEL_DIR}"
    DEPENDS
        webpanel_npm_deps
        "${WEBPANEL_DIR}/package.json"
        "${WEBPANEL_DIR}/package-lock.json"
        "${WEBPANEL_DIR}/webpack.config.js"
        "${WEBPANEL_DIR}/tsconfig.json"
        ${WEBPANEL_SOURCES}
    COMMENT "Building webpanel"
    VERBATIM
)

add_custom_target(webpanel_build
    DEPENDS "${WEBPANEL_BUILD_STAMP}"
)

fatfs_create_rawflash_image(webpanel "${WEBPANEL_DIST_DIR}"
    FLASH_IN_PROJECT
)

add_dependencies(fatfs_webpanel_bin webpanel_build)
