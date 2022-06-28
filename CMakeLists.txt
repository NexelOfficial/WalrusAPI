cmake_minimum_required(VERSION 3.5)
project(TravelScottAPI CXX)

include(CheckIncludeFileCXX)
include(FetchContent)
FetchContent_Declare(cpr GIT_REPOSITORY https://github.com/libcpr/cpr.git
                         GIT_TAG 21f42cf882d0b7e5ae9e3434574fc47e187728de) # The commit hash for 1.8.0. Replace with the latest from: https://github.com/libcpr/cpr/releases
FetchContent_MakeAvailable(cpr)

check_include_file_cxx(any HAS_ANY)
check_include_file_cxx(string_view HAS_STRING_VIEW)
check_include_file_cxx(coroutine HAS_COROUTINE)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++2a -std=gnu++2a")


file(GLOB SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/services/*.*pp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/services/tools/*.*pp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/data/*.*pp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/parsers/hafas/*.*pp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/parsers/beurope/*.*pp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/parsers/*.*pp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/formatters/*.*pp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/formatters/hafas/*.*pp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/formatters/b-europe/*.*pp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/formatters/uov/*.*pp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/formatters/tequila/*.*pp
)

add_executable(${PROJECT_NAME} ${SOURCES})

# add_executable(${PROJECT_NAME} src/main.cpp)

# ##############################################################################
# If you include the drogon source code locally in your project, use this method
# to add drogon 
# add_subdirectory(drogon) 
# target_link_libraries(${PROJECT_NAME} PRIVATE drogon)
#
# and comment out the following lines
find_package(Drogon CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE Drogon::Drogon)
target_link_libraries(${PROJECT_NAME} PRIVATE cpr::cpr)

# ##############################################################################

if (CMAKE_CXX_STANDARD LESS 17)
    # With C++14, use boost to support any, string_view and filesystem
    message(STATUS "use c++14")
    find_package(Boost 1.61.0 REQUIRED)
    target_link_libraries(${PROJECT_NAME} PUBLIC Boost::boost)
elseif (CMAKE_CXX_STANDARD LESS 20)
    message(STATUS "use c++17")
else ()
    message(STATUS "use c++20")
endif ()

aux_source_directory(src/controllers CTL_SRC)
aux_source_directory(src/filters FILTER_SRC)
aux_source_directory(src/plugins PLUGIN_SRC)
aux_source_directory(src/models MODEL_SRC)

drogon_create_views(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/views
                    ${CMAKE_CURRENT_BINARY_DIR})
# use the following line to create views with namespaces.
# drogon_create_views(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/views
#                     ${CMAKE_CURRENT_BINARY_DIR} TRUE)

target_include_directories(${PROJECT_NAME}
                           PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
                                   ${CMAKE_CURRENT_SOURCE_DIR}/models)
target_sources(${PROJECT_NAME}
               PRIVATE
               ${SRC_DIR}
               ${CTL_SRC}
               ${FILTER_SRC}
               ${PLUGIN_SRC}
               ${MODEL_SRC})
# ##############################################################################
# uncomment the following line for dynamically loading views 
# set_property(TARGET ${PROJECT_NAME} PROPERTY ENABLE_EXPORTS ON)

# ##############################################################################