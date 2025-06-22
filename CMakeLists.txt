cmake_minimum_required(VERSION 3.10)
project(mgnetframe)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(ENABLE_DEBUG_MODE ON)

#set all third party lib output dir
set(LIB_OUTPUT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib)
add_custom_command(
    OUTPUT ${LIB_OUTPUT_DIR}
    COMMAND ${CMAKE_COMMAND} -E make_directory ${LIB_OUTPUT_DIR}
)
add_custom_target(target_lib_output_dir DEPENDS ${LIB_OUTPUT_DIR})

# install protobuf
set(PROTOBUF_INSTALL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/protobuf)
set(PROTOBUF_INSTALL_TEMP_DIR ${CMAKE_BINARY_DIR}/protobuf-3.21.11)
add_custom_command(
    OUTPUT ${PROTOBUF_INSTALL_DIR}
    COMMAND ${CMAKE_COMMAND} -E make_directory ${PROTOBUF_INSTALL_DIR}
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${PROTOBUF_INSTALL_TEMP_DIR}
    COMMAND ${CMAKE_COMMAND} -E make_directory ${PROTOBUF_INSTALL_TEMP_DIR}
    COMMAND unzip ${CMAKE_CURRENT_SOURCE_DIR}/protobuf-3.21.11.zip -d ${CMAKE_BINARY_DIR}
    COMMAND chmod +x ${PROTOBUF_INSTALL_TEMP_DIR}/configure
    COMMAND cd ${PROTOBUF_INSTALL_TEMP_DIR} && ./configure --prefix=${PROTOBUF_INSTALL_DIR}
    COMMAND cd ${PROTOBUF_INSTALL_TEMP_DIR} && make && make install
    COMMAND mv ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/lib/libprotobuf* ${CMAKE_CURRENT_SOURCE_DIR}/lib
    COMMENT "protobuf installing..."
)
add_custom_target(target_protobuf DEPENDS ${PROTOBUF_INSTALL_DIR} target_lib_output_dir)

# install spdlog
set(SPDLOG_INSTALL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/spdlog)
set(SPDLOG_INSTALL_TEMP_DIR ${CMAKE_BINARY_DIR}/spdlog-1.15.2)
add_custom_command(
    OUTPUT ${SPDLOG_INSTALL_DIR}
    COMMAND ${CMAKE_COMMAND} -E make_directory ${SPDLOG_INSTALL_DIR}
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${SPDLOG_INSTALL_TEMP_DIR}
    COMMAND ${CMAKE_COMMAND} -E make_directory ${SPDLOG_INSTALL_TEMP_DIR}
    COMMAND tar -xzvf ${CMAKE_CURRENT_SOURCE_DIR}/spdlog-1.15.2.tar.gz -C ${CMAKE_BINARY_DIR}
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMAND ${CMAKE_COMMAND} -E make_directory ${SPDLOG_INSTALL_TEMP_DIR}/build
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${SPDLOG_INSTALL_TEMP_DIR}/include/spdlog ${SPDLOG_INSTALL_DIR}
    COMMAND cd ${SPDLOG_INSTALL_TEMP_DIR}/build && cmake -DSPDLOG_BUILD_SHARED=ON .. && make
    COMMAND mv ${SPDLOG_INSTALL_TEMP_DIR}/build/libspdlog* ${CMAKE_CURRENT_SOURCE_DIR}/lib
    COMMENT "spdlog installing..."
)
add_custom_target(target_spdlog DEPENDS ${SPDLOG_INSTALL_DIR} target_lib_output_dir)

#install hiredis
set(HIREDIS_INSTALL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/hiredis)
set(HIREDIS_INSTALL_TEMP_DIR ${CMAKE_BINARY_DIR}/hiredis-1.3.0)
add_custom_command(
    OUTPUT ${HIREDIS_INSTALL_DIR}
    COMMAND ${CMAKE_COMMAND} -E make_directory ${HIREDIS_INSTALL_DIR}
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${HIREDIS_INSTALL_TEMP_DIR}
    COMMAND ${CMAKE_COMMAND} -E make_directory ${HIREDIS_INSTALL_TEMP_DIR}
    COMMAND tar -xzvf ${CMAKE_CURRENT_SOURCE_DIR}/hiredis-1.3.0.tar.gz -C ${CMAKE_BINARY_DIR}
    COMMAND sed -i.bak "s#PREFIX?=/usr/local#PREFIX?=${HIREDIS_INSTALL_DIR}#" 
                        "${HIREDIS_INSTALL_TEMP_DIR}/Makefile"
    COMMAND sed -i.bak "s#INCLUDE_PATH?=include/hiredis#INCLUDE_PATH?=include#" 
                        "${HIREDIS_INSTALL_TEMP_DIR}/Makefile"
    COMMAND cd ${HIREDIS_INSTALL_TEMP_DIR} && make && make install
    COMMAND mv ${HIREDIS_INSTALL_DIR}/lib/libhiredis* ${CMAKE_CURRENT_SOURCE_DIR}/lib
    COMMENT "hiredis installing..."
)
add_custom_target(target_hiredis DEPENDS ${HIREDIS_INSTALL_DIR} target_lib_output_dir)

#install libwebsockets
set(LIBWEBSOCKETS_INSTALL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libwebsockets)
set(LIBWEBSOCKETS_INSTALL_TEMP_DIR ${CMAKE_BINARY_DIR}/libwebsockets-4.3.5)
add_custom_command(
    OUTPUT ${LIBWEBSOCKETS_INSTALL_DIR}
    COMMAND ${CMAKE_COMMAND} -E make_directory ${LIBWEBSOCKETS_INSTALL_DIR}
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${LIBWEBSOCKETS_INSTALL_TEMP_DIR}
    COMMAND ${CMAKE_COMMAND} -E make_directory ${LIBWEBSOCKETS_INSTALL_TEMP_DIR}
    COMMAND tar -xzvf ${CMAKE_CURRENT_SOURCE_DIR}/libwebsockets-4.3.5.tar.gz -C ${CMAKE_BINARY_DIR}
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMAND ${CMAKE_COMMAND} -E make_directory ${LIBWEBSOCKETS_INSTALL_TEMP_DIR}/build
    COMMAND cd ${LIBWEBSOCKETS_INSTALL_TEMP_DIR}/build && cmake .. && make
    COMMAND mv ${LIBWEBSOCKETS_INSTALL_TEMP_DIR}/build/lib/libwebsockets* ${CMAKE_CURRENT_SOURCE_DIR}/lib
    COMMAND mv ${LIBWEBSOCKETS_INSTALL_TEMP_DIR}/build/include/ ${LIBWEBSOCKETS_INSTALL_DIR}
    COMMENT "libwebsockets installing..."
)
add_custom_target(target_libwebsockets DEPENDS ${LIBWEBSOCKETS_INSTALL_DIR} target_lib_output_dir)

# install all third party libs
add_custom_target(thirdparty_build
    DEPENDS
        target_protobuf
        target_spdlog
        target_hiredis
        target_libwebsockets
)

# set if make shared library
OPTION(BUILD_SHARED_LIBS "Build shared libraries" ON)

# dependency include path
set(INCLUDE_PATH 
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/include
    ${CMAKE_CURRENT_SOURCE_DIR}/hiredis
    ${CMAKE_CURRENT_SOURCE_DIR}/libwebsockets/include
)
include_directories(${INCLUDE_PATH})
message(STATUS "INCLUDE_PATH: ${INCLUDE_PATH}")

# set ouput lib path
set(LIBRARY_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/lib)
message(STATUS "LIBRARY_OUTPUT_PATH: ${LIBRARY_OUTPUT_PATH}")

# get all source file
aux_source_directory(./base BASE_SRC)
set(FRAME_SRC ${BASE_SRC})

if (ENABLE_DEBUG_MODE)
    add_definitions(-D_DEBUG)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -fsanitize=address")
endif()

add_library(mgnetframe ${FRAME_SRC})
add_dependencies(mgnetframe thirdparty_build)

# set linked library path
target_link_directories(mgnetframe PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/lib)
target_link_libraries(mgnetframe PUBLIC spdlog mysqlclient protobuf hiredis websockets)