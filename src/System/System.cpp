#include "System.hpp"

int System::iniciarSesion(const std::string &usuario, const std::string &contrasena)
{
    try
    {
        // Preparar la consulta SQL para obtener el usuario y la contraseña desde la base de datos
        sql::PreparedStatement *stmt = this->getConnection()->prepareStatement(
            "SELECT nombre, password FROM User WHERE nombre = ?");

        // Establece el nombre de usuario en la consulta
        stmt->setString(1, usuario);

        // Ejecutar la consulta
        sql::ResultSet *res = stmt->executeQuery();

        if (res->next()) // Si se encuentra un usuario
        {
            // Obtener el nombre y la contraseña de la base de datos
            std::string db_usuario = res->getString("nombre");
            std::string db_contrasena = res->getString("password");

            // Comparar la contraseña ingresada con la de la base de datos
            if (db_usuario == usuario && db_contrasena == contrasena)
            {
                delete stmt;
                delete res;
                return 1; // Inicio de sesión exitoso
            }
            else
            {
                delete stmt;
                delete res;
                return -1; // Usuario o contraseña incorrectos
            }
        }
        else
        {
            delete stmt;
            delete res;
            return -2; // No se encontró el usuario
        }
    }
    catch (sql::SQLException &e)
    {
        std::cerr << "Error al conectar o consultar la base de datos: " << e.what() << std::endl;
        return 0; // Error de conexión o consulta
    }
}

int System::obtenerTipoUsuario(const std::string &usuario)
{
    try
    {
        // Preparar la consulta SQL para obtener el tipo de usuario desde la base de datos
        sql::PreparedStatement *stmt = this->getConnection()->prepareStatement(
            "SELECT tipo_user FROM User WHERE nombre = ?");

        // Establece el nombre de usuario en la consulta
        stmt->setString(1, usuario);

        // Ejecutar la consulta
        sql::ResultSet *res = stmt->executeQuery();

        if (res->next()) // Si se encuentra el usuario
        {
            // Obtener el tipo de usuario de la base de datos
            std::string tipo_usuario = res->getString("tipo_user");

            // Comparar el tipo de usuario y devolver el valor correspondiente
            if (tipo_usuario == "alumno")
            {
                delete stmt;
                delete res;
                return 1; // El usuario es un alumno
            }
            else if (tipo_usuario == "profesor")
            {
                delete stmt;
                delete res;
                return 0; // El usuario es un profesor
            }
            else
            {
                delete stmt;
                delete res;
                return -1; // Tipo de usuario desconocido
            }
        }
        else
        {
            delete stmt;
            delete res;
            return -2; // No se encontró el usuario
        }
    }
    catch (sql::SQLException &e)
    {
        std::cerr << "Error al conectar o consultar la base de datos: " << e.what() << std::endl;
        return 0; // Error de conexión o consulta
    }
}

std::vector<std::string> System::obtenerUniversidadesConPlazasDisponiblesParaProfesor()
{
    std::vector<std::string> universidades;
    try
    {
        // Preparar la consulta SQL para obtener universidades con plazas disponibles para profesores
        sql::PreparedStatement *stmt = this->getConnection()->prepareStatement(
            "SELECT nombre FROM Universidad WHERE numero_ocupados_profesores < numero_maximo_profesores");

        // Ejecutar la consulta
        sql::ResultSet *res = stmt->executeQuery();

        // Iterar sobre los resultados y agregar los nombres de las universidades con plazas disponibles
        while (res->next())
        {
            std::string nombre_universidad = res->getString("nombre");
            universidades.push_back(nombre_universidad);
        }

        // Liberar recursos
        delete stmt;
        delete res;
    }
    catch (sql::SQLException &e)
    {
        std::cerr << "Error al conectar o consultar la base de datos: " << e.what() << std::endl;
    }

    return universidades; // Devuelve las universidades con plazas disponibles para profesores
}
