#
# Aspia Project
# Copyright (C) 2016-2022 Dmitry Chapyshev <dmitry@aspia.ru>
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program. If not, see <https://www.gnu.org/licenses/>.
#

cmake_minimum_required(VERSION 3.17.0)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

project(syntexremote)

set(ASPIA_VERSION_MAJOR 2)
set(ASPIA_VERSION_MINOR 6)
set(ASPIA_VERSION_PATCH 0)

add_definitions(-DASPIA_VERSION_MAJOR=${ASPIA_VERSION_MAJOR}
                -DASPIA_VERSION_MINOR=${ASPIA_VERSION_MINOR}
                -DASPIA_VERSION_PATCH=${ASPIA_VERSION_PATCH})

if (UNIX AND NOT APPLE)
    message(STATUS "Linux detected")
    set(LINUX TRUE)
endif()

if (APPLE)
    message(STATUS "MacOSX detected")
endif()

if (WIN32)
    message(STATUS "Windows detected")
endif()

message(STATUS "System processor: ${CMAKE_SYSTEM_PROCESSOR}")
message(STATUS "Host system processor: ${CMAKE_HOST_SYSTEM_PROCESSOR}")


find_package(PkgConfig REQUIRED)
find_package(Qt5 REQUIRED Core Gui Network PrintSupport Widgets Xml)
find_package(Qt5LinguistTools)
find_package(CURL REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(Protobuf REQUIRED)
find_package(GTest)

# Link all Abseil dependencies into protobuf::libprotobuf-lite
pkg_check_modules(PROTOBUF_LITE REQUIRED protobuf-lite)
target_link_libraries(protobuf::libprotobuf-lite INTERFACE ${PROTOBUF_LITE_LDFLAGS})

# ASIO
find_path(ASIO_INCLUDE_DIR asio.hpp)
add_library(asio INTERFACE IMPORTED GLOBAL)
set_target_properties(asio PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${ASIO_INCLUDE_DIR}")

# RapidJSON
find_path(RAPIDJSON_INCLUDE_DIR rapidjson/rapidjson.h)
add_library(RapidJSON INTERFACE IMPORTED GLOBAL)
set_target_properties(RapidJSON PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${RAPIDJSON_INCLUDE_DIR}")

# libyuv
find_path(LIBYUV_INCLUDE_DIR libyuv.h)
find_library(LIBYUV_LIBRARY yuv)
add_library(yuv INTERFACE IMPORTED GLOBAL)
target_link_libraries(yuv INTERFACE "${LIBYUV_LIBRARY}")
target_include_directories(yuv INTERFACE "${LIBYUV_INCLUDE_DIR}")

# libvpx
pkg_check_modules(VPX REQUIRED vpx)
add_library(unofficial::libvpx::libvpx INTERFACE IMPORTED GLOBAL)
target_link_libraries(unofficial::libvpx::libvpx INTERFACE ${VPX_LIBRARIES})
target_include_directories(unofficial::libvpx::libvpx INTERFACE ${VPX_INCLUDE_DIRS})
target_link_directories(unofficial::libvpx::libvpx INTERFACE ${VPX_LIBRARY_DIRS})

# sqlite3
pkg_check_modules(SQLITE3 REQUIRED sqlite3)
add_library(unofficial::sqlite3::sqlite3 INTERFACE IMPORTED GLOBAL)
target_link_libraries(unofficial::sqlite3::sqlite3 INTERFACE ${SQLITE3_LIBRARIES})
target_include_directories(unofficial::sqlite3::sqlite3 INTERFACE ${SQLITE3_INCLUDE_DIRS})
target_link_directories(unofficial::sqlite3::sqlite3 INTERFACE ${SQLITE3_LIBRARY_DIRS})

# opus
pkg_check_modules(OPUS REQUIRED opus)
add_library(Opus::opus INTERFACE IMPORTED GLOBAL)
target_link_libraries(Opus::opus INTERFACE ${OPUS_LIBRARIES})
target_include_directories(Opus::opus INTERFACE ${OPUS_INCLUDE_DIRS})
target_link_directories(Opus::opus INTERFACE ${OPUS_LIBRARY_DIRS})

# zstd
pkg_check_modules(ZSTD REQUIRED libzstd)
add_library(zstd::libzstd_static INTERFACE IMPORTED GLOBAL)
target_link_libraries(zstd::libzstd_static INTERFACE ${ZSTD_LIBRARIES})
target_include_directories(zstd::libzstd_static INTERFACE ${ZSTD_INCLUDE_DIRS})
target_link_directories(zstd::libzstd_static INTERFACE ${ZSTD_LIBRARY_DIRS})

# mimalloc
add_library(mimalloc-static INTERFACE IMPORTED GLOBAL)
set(mimalloc_FOUND FALSE)

message(STATUS "MIMALLOC found: ${mimalloc_FOUND}")

if (WIN32)
    find_package(Qt5WinExtras REQUIRED)
endif()

if (UNIX)
    find_package(ICU REQUIRED uc dt)
endif()

if (LINUX)
    find_library(XFIXES_LIB NAMES libXfixes Xfixes REQUIRED)
    message(STATUS "XFixes library: ${XFIXES_LIB}")

    find_library(XDAMAGE_LIB NAMES libXdamage Xdamage REQUIRED)
    message(STATUS "XDamage library: ${XDAMAGE_LIB}")

    find_library(XRANDR_LIB NAMES libXrandr Xrandr REQUIRED)
    message(STATUS "XRandr library: ${XRANDR_LIB}")

    find_library(XTST_LIB NAMES libXtst Xtst REQUIRED)
    message(STATUS "XTst library: ${XTST_LIB}")

    find_library(XI_LIB NAMES libXi Xi REQUIRED)
    message(STATUS "Xi library: ${XI_LIB}")
endif()

if (APPLE)
    set(CMAKE_OSX_ARCHITECTURES "x86_64")
    find_library(FOUNDATION_LIB Foundation)
    find_library(COREAUDIO_LIB CoreAudio)
    find_library(AUDIOTOOLBOX_LIB AudioToolbox)
    find_program(CODESIGN_BIN NAMES codesign)
endif()

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

enable_testing()

add_subdirectory(source)
