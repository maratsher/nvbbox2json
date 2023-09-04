cmake_minimum_required(VERSION 3.12)

project(gstnvobjmeta2json LANGUAGES C)

set(CUDA_VER 12.1)
set(CMAKE_CXX_COMPILER gcc)
set(LIB gstnvobjmeta2json)

set(DEEPSTREAM_INSTALL_DIR /opt/nvidia/deepstream/deepstream-6.3)

find_package(PkgConfig REQUIRED)
pkg_check_modules(PKGS REQUIRED gstreamer-1.0 gstreamer-base-1.0 gstreamer-video-1.0 json-c)
exec_program(${PKG_CONFIG_EXECUTABLE} ARGS --variable pluginsdir gstreamer-1.0 OUTPUT_VARIABLE PKG_GSTREAMER_PLUGIN_DIR)


add_compile_options(-fPIC -Wunused-variable)

include_directories(
    /usr/local/cuda-${CUDA_VER}/include
    ${DEEPSTREAM_INSTALL_DIR}/sources/includes
    ${PKGS_INCLUDE_DIRS}
)

link_directories(
    /usr/local/cuda-${CUDA_VER}/lib64/
    ${DEEPSTREAM_INSTALL_DIR}/lib
)

add_library(${LIB} SHARED gstnvobjmeta2json.c gstnvobjmeta2json.h)

target_link_libraries(${LIB} PRIVATE
    cudart nppc nppig npps nvdsgst_meta
    ${PKGS_LIBRARIES}
)
target_compile_options(${LIB} PRIVATE -Wl,-no-undefined)
set_target_properties(${LIB} PROPERTIES
    LINK_FLAGS -Wl,-rpath,${DEEPSTREAM_INSTALL_DIR}/lib
)

install(TARGETS ${LIB} LIBRARY DESTINATION ${PKG_GSTREAMER_PLUGIN_DIR})
