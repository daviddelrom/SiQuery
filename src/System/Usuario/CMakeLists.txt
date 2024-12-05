# Archivos fuente del módulo Usuario
set(USUARIO_SOURCES
    Usuario.cpp
    Alumno.cpp
    Profesor.cpp
    Administrador.cpp
)

# Crear una biblioteca para el módulo Usuario
add_library(UsuarioLib ${USUARIO_SOURCES})
