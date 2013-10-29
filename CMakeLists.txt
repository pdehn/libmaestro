cmake_minimum_required(VERSION 2.6)
project(LibMaestro)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake_modules/")

find_package(libusb-1.0 REQUIRED)
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include/" ${LIBUSB_1_INCLUDE_DIRS})

add_library(maestro STATIC src/maestro.cpp)
target_link_libraries(maestro ${LIBUSB_1_LIBRARIES})

add_executable(maestro-app src/maestro-app.cpp)
target_link_libraries(maestro-app maestro)

install(TARGETS maestro DESTINATION lib)
install(FILES include/maestro.h DESTINATION include)
