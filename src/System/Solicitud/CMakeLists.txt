# Archivos fuente del módulo Solicitud
set(SOLICITUD_SOURCES
    Solicitud.cpp
    SolicitudBeca.cpp
    SolicitudSicue.cpp
)

# Crear una biblioteca para el módulo Solicitud
add_library(SolicitudLib ${SOLICITUD_SOURCES})
