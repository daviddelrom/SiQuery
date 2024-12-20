# Archivos fuente del módulo display
set(DISPLAY_SOURCES main.cpp extern-functions.cpp)

# Agregar ejecutable
add_executable(ProyectoIS ${DISPLAY_SOURCES})

# Enlazar con la biblioteca del sistema
target_link_libraries(ProyectoIS SystemLib)
