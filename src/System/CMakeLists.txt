# Agregar subdirectorios
add_subdirectory(Usuario)
add_subdirectory(Solicitud)

# Archivos fuente del módulo system
set(SYSTEM_SOURCES System.cpp)

# Crear una biblioteca para el sistema
add_library(SystemLib ${SYSTEM_SOURCES})

# Incluir encabezados de MySQL Connector/C++
target_include_directories(SystemLib PUBLIC /usr/include/mysql-cppconn/jdbc)

# Enlazar con las bibliotecas de MySQL Connector/C++
target_link_libraries(SystemLib mysqlcppconn)

# Enlazar con las bibliotecas de subdirectorios y mysqlcppconn
target_link_libraries(SystemLib mysqlcppconn UsuarioLib SolicitudLib)
