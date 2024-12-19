#ifndef SYSTEM_H
#define SYSTEM_H

#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/prepared_statement.h>
#include <iostream>
#include <string>

class System
{
private:
    sql::mysql::MySQL_Driver *driver;
    sql::Connection *con;
    const std::string &db_url = "tcp://127.0.0.1:3306"; // Dirección de MySQL
    const std::string &db_user = "root";                // Usuario MySQL
    const std::string &db_password = "123";             // Contraseña
    const std::string &db_name = "IS";                  // Nombre de la base de datos

public:
    // Constructor que establece la conexión a la base de datos MySQL
    System()
    {
        driver = sql::mysql::get_mysql_driver_instance();
        con = driver->connect(db_url, db_user, db_password);
        con->setSchema(db_name);
    }
    // Destructor que cierra la conexión a la base de datos
    ~System()
    {
        delete con;
    }

    sql::Connection *getConnection()
    {
        return con;
    }

    // Función para iniciar sesión con un usuario y contraseña
    int iniciarSesion(const std::string &usuario, const std::string &contrasena);

    // Función para obtener tipo de Usuario 0 = Profesor 1 = Alumno 2 = Administrador
    int obtenerTipoUsuario(const std::string &usuario);

    //Función para obtener la lista de Universidades para hacer Sicue disponibles
    std::vector<std::string> obtenerUniversidadesConPlazasDisponiblesParaProfesor();
};
#endif
