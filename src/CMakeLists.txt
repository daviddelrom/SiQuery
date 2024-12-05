# Versión mínima de CMake
cmake_minimum_required(VERSION 3.15)

# Nombre del proyecto
project(ProyectoIS LANGUAGES CXX)

# Estándar C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Directorios de salida
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/source/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/source/obj)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/source/obj)

# Agregar subdirectorios
add_subdirectory(Display)
add_subdirectory(System)
