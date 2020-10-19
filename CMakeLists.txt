set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

cmake_minimum_required(VERSION 3.0)
project(hadess-sensorfw-proxy)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

include(GNUInstallDirs)
find_package(PkgConfig REQUIRED)

pkg_check_modules(GIO REQUIRED gio-2.0)
pkg_check_modules(GIO_UNIX REQUIRED gio-unix-2.0)
pkg_check_modules(GUDEV REQUIRED gudev-1.0)

find_package(Qt5Core REQUIRED)
find_package(Qt5Network REQUIRED)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -pthread")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -pthread")

add_subdirectory(sensorfw-core)

add_executable(
    hadess-sensorfw-proxy

    iio-sensor-proxy.cpp
    iio-sensor-proxy-resources.cpp
    orientation.cpp
)

target_link_libraries(hadess-sensorfw-proxy PUBLIC
    ${GIO_LDFLAGS} ${GIO_LIBRARIES}
    ${GIO_UNIX_LDFLAGS} ${GIO_UNIX_LIBRARIES}
    ${GUDEV_LDFLAGS} ${GUDEV_LIBRARIES}

    sensorfw-core
)

target_include_directories(hadess-sensorfw-proxy PUBLIC
    ${GIO_INCLUDE_DIRS}
    ${GIO_UNIX_INCLUDE_DIRS}
    ${GUDEV_INCLUDE_DIRS}
)

install(TARGETS hadess-sensorfw-proxy RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
