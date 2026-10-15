cmake_minimum_required(VERSION 3.25)
project(BCC6001-AP1 CXX)

set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)

FetchContent_Declare(
    libgexf
    GIT_REPOSITORY https://github.com/ml1nk/libgexf.git
    GIT_TAG        master
)

FetchContent_MakeAvailable(libgexf)

find_package(LibXml2 REQUIRED)

file(GLOB GEXF_SOURCES CONFIGURE_DEPENDS ${libgexf_SOURCE_DIR}/libgexf/*.cpp)

add_library(gexf STATIC ${GEXF_SOURCES})
target_include_directories(gexf PUBLIC ${libgexf_SOURCE_DIR})
target_link_libraries(gexf PUBLIC LibXml2::LibXml2)
set_target_properties(gexf PROPERTIES
    CXX_STANDARD 26
    CXX_STANDARD_REQUIRED ON
)

target_compile_options(gexf PUBLIC -Wno-dynamic-exception-spec)

add_executable(BCC6001-AP1 main.cpp)
target_include_directories(BCC6001-AP1 PRIVATE include)
target_link_libraries(BCC6001-AP1 PRIVATE gexf)
set_target_properties(BCC6001-AP1 PROPERTIES
    CXX_STANDARD 26
    CXX_STANDARD_REQUIRED ON
)