set(SOURCE_FILES
    src/Environment.cpp)

set(HEADERS
    include/Cafe/Misc/Environment.h)

add_library(Cafe.Environment ${SOURCE_FILES} ${HEADERS})

target_include_directories(Cafe.Environment PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>)

target_link_libraries(Cafe.Environment
    PUBLIC Cafe.Encoding.UTF-8 Cafe.TextUtils.Format)

if(CMAKE_SYSTEM_NAME MATCHES "Windows")
    target_link_libraries(Cafe.Environment PUBLIC
        Cafe.Encoding.UTF-16)
endif()

install(TARGETS Cafe.Environment
    EXPORT Environment)

install(DIRECTORY include/
    TYPE INCLUDE
    FILES_MATCHING PATTERN "*.h")

install(EXPORT Environment
    DESTINATION cmake/Environment)
