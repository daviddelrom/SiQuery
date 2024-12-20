#ifndef SYSTEM_H
#define SYSTEM_H

#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/prepared_statement.h>
#include <iostream>
#include <string>
#include "../System/Solicitud/Solicitud.hpp" // Incluir las cabeceras de las clases
#include "../System/Solicitud/SolicitudSicue.hpp"

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

    std::string obtenerDepartamentoProfesor(const std::string &username);
    std::vector<std::string> obtenerUniversidadesConPlazasDisponiblesParaProfesor(const std::string &departamento);
    void insertarSolicitudesSicueProfesor(int solicitante_id, const std::string &nombre_solicitante, const std::string &universidad_origen, const std::vector<std::string> &universidades_destino, const std::string &departamento, const std::string &curso);
    int obtenerUserIdPorNombre(const std::string &nombre);
    std::vector<std::string> obtenerUniversidadesYaSolicitadas(const std::string &usuario);
    void modificarSolicitudSicueProfesor(const std::string &nombre_solicitante, const std::string &nuevoCurso, const std::string &nuevaUniversidadDestino);
    void actualizarPlazasOcupadas(const std::string &universidadOrigen, const std::string &universidadDestino);
    std::string obtenerCursosDeSolicitudes(const std::string &solicitante_id);
    bool tienePlazasDisponiblesParaProfesor(const std::string &universidad);
    std::vector<std::string> obtenerUniversidadesConPlazasDisponiblesParaAlumno(const std::string &campo, int anioCurso);
    std::string obtenerNombreUniversidadPorId(int universidadId);
    int obtenerUniversidadOrigenProfesor(const std::string &nombreProfesor);
    void insertarSolicitudesSicueAlumno(int solicitante_id, const std::string &nombre_solicitante, const std::string &universidad_origen, const std::vector<std::string> &universidades_destino, const std::string &carrera, const std::string &curso);
    int obtenerIdUsuarioPorUsuario(const std::string &usuario);
    std::string obtenerUniversidadOrigenPorIdUsuario(int idUsuario);
    int obtenerAnyoAcademicoPorIdUsuario(int idUsuario);
    int obtenerAñoCursoAlumno(const std::string &usuario, const std::string &carrera);
    std::string obtenerCarreraPorIdUsuario(int userId);
    std::string obtenerCursoAcademicoPorIdUsuario(int idUsuario);
    std::vector<std::string> obtenerUniversidadesYaSolicitadasAlumno(const std::string &usuario);
    bool esPlazaDisponibleParaCurso(const std::string &universidad, const std::string &carrera, int añoCurso);
    std::string obtenerNombreUniversidadPorCarrera(const std::string &carrera);
    std::string obtenerCampoPorCarrera(const std::string &nombre_carrera);
    void modificarSolicitudSicueAlumno(const std::string &nombre_solicitante, const std::string &nuevoCurso, const std::string &nuevaUniversidadDestino);
    int obtenerUniversidad(int universidad_id);
    void consultarPlazas();
};
#endif
