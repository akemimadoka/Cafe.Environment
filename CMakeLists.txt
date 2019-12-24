set(SOURCE_FILES
    src/Cafe/Misc/Environment.cpp)

set(HEADERS
    src/Cafe/Misc/Environment.h)

add_library(Cafe.Environment ${SOURCE_FILES} ${HEADERS})

target_include_directories(Cafe.Environment PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
    $<INSTALL_INTERFACE:include>)

target_link_libraries(Cafe.Environment
    PUBLIC Cafe.Encoding.UTF-8 Cafe.TextUtils.Format)

if(CMAKE_SYSTEM_NAME MATCHES "Windows")
    target_link_libraries(Cafe.Environment PUBLIC
        Cafe.Encoding.UTF-16)
endif()

install(TARGETS Cafe.Environment
    EXPORT Environment)

install(DIRECTORY src/
    TYPE INCLUDE
    FILES_MATCHING PATTERN "*.h")

install(EXPORT Environment
    DESTINATION cmake/Environment)
