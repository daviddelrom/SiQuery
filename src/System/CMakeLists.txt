# Agregar subdirectorios
add_subdirectory(Usuario)
add_subdirectory(Solicitud)
add_subdirectory(Informe)

# Archivos fuente del módulo system
set(SYSTEM_SOURCES System.cpp)

# Crear una biblioteca para el sistema
add_library(SystemLib ${SYSTEM_SOURCES})

# Enlazar con las bibliotecas de subdirectorios
target_link_libraries(SystemLib UsuarioLib SolicitudLib InformeLib)
