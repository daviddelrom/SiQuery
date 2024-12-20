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

std::vector<std::string> System::obtenerUniversidadesConPlazasDisponiblesParaProfesor(const std::string &departamento)
{
    std::vector<std::string> universidades;
    std::vector<int> universidadesIds;

    try
    {
        // Paso 1: Obtener los ID de universidades asociadas con el departamento
        sql::PreparedStatement *stmtTitulacion = this->getConnection()->prepareStatement(
            "SELECT DISTINCT t.universidad_id "
            "FROM Titulacion t "
            "WHERE t.departamento = ?");

        // Establecer el parámetro para el departamento
        stmtTitulacion->setString(1, departamento);

        // Ejecutar la consulta
        sql::ResultSet *resTitulacion = stmtTitulacion->executeQuery();

        // Iterar sobre los resultados y almacenar los IDs de universidades
        while (resTitulacion->next())
        {
            int universidad_id = resTitulacion->getInt("universidad_id");
            universidadesIds.push_back(universidad_id);
        }

        // Liberar recursos
        delete stmtTitulacion;
        delete resTitulacion;

        // Paso 2: Para cada universidad_id, comprobar si tiene plazas disponibles para profesores
        for (int universidad_id : universidadesIds)
        {
            sql::PreparedStatement *stmtUniversidad = this->getConnection()->prepareStatement(
                "SELECT nombre "
                "FROM Universidad "
                "WHERE universidad_id = ? "
                "AND numero_ocupados_profesores < maximo_profesores");

            stmtUniversidad->setInt(1, universidad_id);

            // Ejecutar la consulta
            sql::ResultSet *resUniversidad = stmtUniversidad->executeQuery();

            // Si la universidad tiene plazas disponibles, agregamos su nombre al vector
            if (resUniversidad->next())
            {
                std::string nombre_universidad = resUniversidad->getString("nombre");
                universidades.push_back(nombre_universidad);
            }

            // Liberar recursos
            delete stmtUniversidad;
            delete resUniversidad;
        }
    }
    catch (sql::SQLException &e)
    {
        std::cerr << "Error al conectar o consultar la base de datos: " << e.what() << std::endl;
    }

    return universidades; // Devuelve las universidades con plazas disponibles para profesores en el departamento especificado
}

std::string System::obtenerDepartamentoProfesor(const std::string &username)
{
    std::string departamento;

    try
    {
        // Paso 1: Obtener el user_id del profesor desde la tabla User
        sql::PreparedStatement *stmtUser = this->getConnection()->prepareStatement(
            "SELECT user_id FROM User WHERE nombre = ? AND tipo_user = 'profesor'");

        stmtUser->setString(1, username);
        sql::ResultSet *resUser = stmtUser->executeQuery();

        int userId = -1;
        if (resUser->next())
        {
            userId = resUser->getInt("user_id");
        }

        delete stmtUser;
        delete resUser;

        // Verificar si se encontró el user_id
        if (userId == -1)
        {
            std::cerr << "Error: No se encontró el profesor con el nombre de usuario proporcionado." << std::endl;
            return departamento;
        }

        // Paso 2: Obtener el departamento del profesor desde la tabla Profesor
        sql::PreparedStatement *stmtProfesor = this->getConnection()->prepareStatement(
            "SELECT departamento FROM Profesor WHERE profesor_id = ?");

        stmtProfesor->setInt(1, userId);
        sql::ResultSet *resProfesor = stmtProfesor->executeQuery();

        if (resProfesor->next())
        {
            departamento = resProfesor->getString("departamento");
        }

        // Liberar recursos
        delete stmtProfesor;
        delete resProfesor;
    }
    catch (sql::SQLException &e)
    {
        std::cerr << "Error al conectar o consultar la base de datos: " << e.what() << std::endl;
    }
    return departamento; // Devuelve el departamento del profesor, o una cadena vacía si no se encontró
}

void System::insertarSolicitudesSicueProfesor(int solicitante_id, const std::string &nombre_solicitante, const std::string &universidad_origen, const std::vector<std::string> &universidades_destino, const std::string &departamento, const std::string &curso)
{
    try
    {
        for (const auto &universidad_destino : universidades_destino)
        {
            // Paso 1: Insertar la solicitud SICUE en la tabla SolicitudSicueProfesor
            sql::PreparedStatement *stmtSolicitud = this->getConnection()->prepareStatement(
                "INSERT INTO SolicitudSicueProfesor (nombre_solicitante, solicitante_id, universidad_origen, universidad_destino, departamento, curso, estado_solicitud, fecha_solicitud) "
                "VALUES (?, ?, ?, ?, ?, ?, 'Pendiente', NOW())");

            // Establecer los parámetros de la solicitud
            stmtSolicitud->setString(1, nombre_solicitante);
            stmtSolicitud->setInt(2, solicitante_id);
            stmtSolicitud->setString(3, universidad_origen);
            stmtSolicitud->setString(4, universidad_destino);
            stmtSolicitud->setString(5, departamento);
            stmtSolicitud->setString(6, curso);

            // Ejecutar la consulta
            stmtSolicitud->executeUpdate();
            delete stmtSolicitud;

            // Paso 2: Obtener el ID de la universidad de destino
            sql::PreparedStatement *stmtUniversidad = this->getConnection()->prepareStatement(
                "SELECT universidad_id FROM Universidad WHERE nombre = ?");
            stmtUniversidad->setString(1, universidad_destino);
            sql::ResultSet *resUniversidad = stmtUniversidad->executeQuery();

            int universidad_id = -1;
            if (resUniversidad->next())
            {
                universidad_id = resUniversidad->getInt("universidad_id");
            }
            delete stmtUniversidad;
            delete resUniversidad;

            // Verificar si se encontró el ID de la universidad
            if (universidad_id == -1)
            {
                std::cerr << "Error: No se encontró la universidad " << universidad_destino << " en la base de datos." << std::endl;
                continue; // Saltar a la siguiente universidad
            }

            // Paso 3: Incrementar el número de profesores ocupados en la universidad de destino
            sql::PreparedStatement *stmtActualizar = this->getConnection()->prepareStatement(
                "UPDATE Universidad SET numero_ocupados_profesores = numero_ocupados_profesores + 1 WHERE universidad_id = ?");
            stmtActualizar->setInt(1, universidad_id);

            // Ejecutar la actualización
            stmtActualizar->executeUpdate();
            delete stmtActualizar;

            std::cout << "La solicitud SICUE para la universidad " << universidad_destino
                      << " ha sido procesada correctamente y el número de profesores ocupados ha sido actualizado.\n";
        }
    }
    catch (sql::SQLException &e)
    {
        std::cerr << "Error al conectar o consultar la base de datos: " << e.what() << std::endl;
    }
}

bool System::tienePlazasDisponiblesParaProfesor(const std::string &universidad)
{
    try
    {
        // Paso 1: Obtener el ID de la universidad
        sql::PreparedStatement *stmtUniversidad = this->getConnection()->prepareStatement(
            "SELECT universidad_id, numero_ocupados_profesores, maximo_profesores "
            "FROM Universidad WHERE nombre = ?");

        stmtUniversidad->setString(1, universidad);
        sql::ResultSet *resUniversidad = stmtUniversidad->executeQuery();

        int universidad_id = -1;
        int numero_ocupados_profesores = 0;
        int maximo_profesores = 0;

        // Verificar si encontramos la universidad
        if (resUniversidad->next())
        {
            universidad_id = resUniversidad->getInt("universidad_id");
            numero_ocupados_profesores = resUniversidad->getInt("numero_ocupados_profesores");
            maximo_profesores = resUniversidad->getInt("maximo_profesores");
        }

        // Liberar recursos
        delete stmtUniversidad;
        delete resUniversidad;

        // Verificar si se encontró la universidad
        if (universidad_id == -1)
        {
            std::cerr << "Error: No se encontró la universidad " << universidad << " en la base de datos." << std::endl;
            return false; // Si no se encuentra la universidad, devolver false
        }

        // Paso 2: Comprobar si hay plazas disponibles
        if (numero_ocupados_profesores < maximo_profesores)
        {
            return true; // Hay plazas disponibles
        }
        else
        {
            return false; // No hay plazas disponibles
        }
    }
    catch (sql::SQLException &e)
    {
        std::cerr << "Error al conectar o consultar la base de datos: " << e.what() << std::endl;
        return false; // En caso de error, devolvemos false
    }
}

int System::obtenerUserIdPorNombre(const std::string &nombre)
{
    int userId = -1; // Valor por defecto en caso de error o si no se encuentra el usuario

    try
    {
        // Preparar la consulta SQL para buscar el user_id por nombre
        sql::PreparedStatement *stmt = this->getConnection()->prepareStatement(
            "SELECT user_id FROM User WHERE nombre = ?");

        // Establecer el parámetro del nombre
        stmt->setString(1, nombre);

        // Ejecutar la consulta
        sql::ResultSet *res = stmt->executeQuery();

        // Si se encuentra el usuario, obtener el user_id
        if (res->next())
        {
            userId = res->getInt("user_id");
        }
        else
        {
            std::cerr << "Usuario con el nombre '" << nombre << "' no encontrado en la base de datos." << std::endl;
        }

        // Liberar recursos
        delete stmt;
        delete res;
    }
    catch (sql::SQLException &e)
    {
        std::cerr << "Error al conectar o consultar la base de datos: " << e.what() << std::endl;
    }

    return userId; // Devuelve el user_id o -1 si no se encontró el usuario
}

int System::obtenerUniversidadOrigenProfesor(const std::string &nombreProfesor)
{
    int universidadOrigenId = -1; // Almacena el universidad_id de origen del profesor

    try
    {
        // Paso 1: Obtener el user_id del profesor desde la tabla User
        sql::PreparedStatement *stmtUser = this->getConnection()->prepareStatement(
            "SELECT universidad_origen FROM User WHERE nombre = ? AND tipo_user = 'profesor'");

        // Establecer el parámetro del nombre del profesor
        stmtUser->setString(1, nombreProfesor);

        // Ejecutar la consulta
        sql::ResultSet *resUser = stmtUser->executeQuery();

        // Si se encuentra el registro, obtener el universidad_id de origen
        if (resUser->next())
        {
            universidadOrigenId = resUser->getInt("universidad_origen");
        }
        else
        {
            std::cerr << "No se encontró el profesor con el nombre '" << nombreProfesor << "' en la base de datos." << std::endl;
        }

        // Liberar recursos
        delete stmtUser;
        delete resUser;
    }
    catch (sql::SQLException &e)
    {
        std::cerr << "Error al conectar o consultar la base de datos: " << e.what() << std::endl;
    }

    return universidadOrigenId; // Devuelve el universidad_id de origen o -1 si no se encuentra
}

std::string System::obtenerNombreUniversidadPorId(int universidadId)
{
    std::string nombreUniversidad; // Almacena el nombre de la universidad

    try
    {
        // Paso 1: Obtener el nombre de la universidad desde la tabla Universidad
        sql::PreparedStatement *stmtUniversidad = this->getConnection()->prepareStatement(
            "SELECT nombre FROM Universidad WHERE universidad_id = ?");

        // Establecer el parámetro del universidad_id
        stmtUniversidad->setInt(1, universidadId);

        // Ejecutar la consulta
        sql::ResultSet *resUniversidad = stmtUniversidad->executeQuery();

        // Si se encuentra el registro, obtener el nombre de la universidad
        if (resUniversidad->next())
        {
            nombreUniversidad = resUniversidad->getString("nombre");
        }
        else
        {
            std::cerr << "No se encontró la universidad con ID '" << universidadId << "' en la base de datos." << std::endl;
        }

        // Liberar recursos
        delete stmtUniversidad;
        delete resUniversidad;
    }
    catch (sql::SQLException &e)
    {
        std::cerr << "Error al conectar o consultar la base de datos: " << e.what() << std::endl;
    }

    return nombreUniversidad; // Devuelve el nombre de la universidad o una cadena vacía si no se encuentra
}

std::vector<std::string> System::obtenerUniversidadesYaSolicitadas(const std::string &usuario)
{
    std::vector<std::string> universidadesSolicitadas;

    try
    {
        // Obtener el solicitante_id del usuario
        int solicitante_id = this->obtenerUserIdPorNombre(usuario);

        if (solicitante_id == -1)
        {
            std::cerr << "Error: No se encontró el usuario especificado." << std::endl;
            return universidadesSolicitadas;
        }

        // Preparar la consulta SQL
        sql::PreparedStatement *stmtSolicitudes = this->getConnection()->prepareStatement(
            "SELECT DISTINCT universidad_destino "
            "FROM SolicitudSicueProfesor "
            "WHERE solicitante_id = ?");

        stmtSolicitudes->setInt(1, solicitante_id);

        // Ejecutar la consulta
        sql::ResultSet *res = stmtSolicitudes->executeQuery();

        // Almacenar los nombres de las universidades
        while (res->next())
        {
            universidadesSolicitadas.push_back(res->getString("universidad_destino"));
        }

        // Liberar recursos
        delete stmtSolicitudes;
        delete res;
    }
    catch (sql::SQLException &e)
    {
        std::cerr << "Error al conectar o consultar la base de datos: " << e.what() << std::endl;
    }

    return universidadesSolicitadas;
}

void System::actualizarPlazasOcupadas(const std::string &universidadOrigen, const std::string &universidadDestino)
{
    try
    {
        sql::Connection *conn = this->getConnection(); // Obtener conexión a la base de datos

        // Primero, decrementamos las plazas ocupadas en la universidad de origen
        sql::PreparedStatement *stmtDecrementar = conn->prepareStatement(
            "UPDATE Universidad SET numero_ocupados_alumnos = numero_ocupados_alumnos - 1 WHERE nombre = ?");
        stmtDecrementar->setString(1, universidadOrigen);
        stmtDecrementar->executeUpdate();

        // Luego, incrementamos las plazas ocupadas en la nueva universidad de destino
        sql::PreparedStatement *stmtIncrementar = conn->prepareStatement(
            "UPDATE Universidad SET numero_ocupados_alumnos = numero_ocupados_alumnos + 1 WHERE nombre = ?");
        stmtIncrementar->setString(1, universidadDestino);
        stmtIncrementar->executeUpdate();

        std::cout << "Plazas ocupadas actualizadas correctamente.\n";
    }
    catch (sql::SQLException &e)
    {
        std::cerr << "Error en la actualización de plazas ocupadas: " << e.what() << std::endl;
    }
}

void System::modificarSolicitudSicueProfesor(const std::string &nombre_solicitante, const std::string &nuevoCurso, const std::string &nuevaUniversidadDestino)
{
    try
    {
        System system;                                 // Crear una instancia de la clase System
        sql::Connection *conn = this->getConnection(); // Obtener conexión a la base de datos
        // Preparar la consulta SQL para buscar la solicitud del solicitante en la tabla
        sql::PreparedStatement *stmt = conn->prepareStatement(
            "SELECT * FROM SolicitudSicueALUMNO WHERE nombre_solicitante = ?");

        // Establecer el parámetro para el nombre del solicitante
        stmt->setString(1, nombre_solicitante);

        // Ejecutar la consulta
        sql::ResultSet *res = stmt->executeQuery();

        // Verificar si se encontró la solicitud
        if (!res->next())
        {
            std::cout << "No se encontró una solicitud para el solicitante: " << nombre_solicitante << std::endl;
            delete stmt;
            delete res;
            return;
        }

        // Obtener los datos actuales de la solicitud
        std::string universidadOrigen = res->getString("universidad_origen");
        std::string universidadDestino = res->getString("universidad_destino");
        std::string cursoActual = res->getString("curso");

        // Modificar los campos solicitados por el usuario
        std::string nuevoCursoModificado = nuevoCurso.empty() ? cursoActual : nuevoCurso;
        std::string nuevaUniversidadDestinoModificada = nuevaUniversidadDestino.empty() ? universidadDestino : nuevaUniversidadDestino;

        // Si se ha cambiado la universidad de destino, actualizamos las plazas ocupadas
        if (nuevaUniversidadDestinoModificada != universidadDestino)
        {
            // Llamamos a la función para actualizar las plazas ocupadas en la base de datos
            system.actualizarPlazasOcupadas(universidadDestino, nuevaUniversidadDestinoModificada);
        }

        // Preparar la consulta SQL para actualizar la solicitud
        sql::PreparedStatement *updateStmt = conn->prepareStatement(
            "UPDATE SolicitudSicueALUMNO SET curso = ?, universidad_destino = ? WHERE nombre_solicitante = ?");

        // Establecer los parámetros de la consulta UPDATE
        updateStmt->setString(1, nuevoCursoModificado);
        updateStmt->setString(2, nuevaUniversidadDestinoModificada);
        updateStmt->setString(3, nombre_solicitante);

        // Ejecutar la consulta de actualización
        int rowsUpdated = updateStmt->executeUpdate();

        // Verificar si se actualizó correctamente
        if (rowsUpdated > 0)
        {
            std::cout << "La solicitud ha sido modificada correctamente.\n";
        }
        else
        {
            std::cout << "No se pudo modificar la solicitud.\n";
        }

        // Liberar los recursos
        delete stmt;
        delete res;
        delete updateStmt;
    }
    catch (sql::SQLException &e)
    {
        std::cerr << "Error al conectar o modificar la base de datos: " << e.what() << std::endl;
    }
}

std::string System::obtenerCursosDeSolicitudes(const std::string &solicitante_id)
{
    std::string primerCurso = ""; // Variable para almacenar el primer curso encontrado

    try
    {
        System system; // Crear una instancia de la clase System
        // Preparar la consulta SQL para obtener todos los cursos de las solicitudes del solicitante
        sql::PreparedStatement *stmt = system.getConnection()->prepareStatement(
            "SELECT curso FROM SolicitudSicueProfesor WHERE nombre_solicitante = ?");

        // Establecer el parámetro para el solicitante_id
        stmt->setString(1, solicitante_id);

        // Ejecutar la consulta
        sql::ResultSet *res = stmt->executeQuery();

        // Verificar si hay resultados y obtener el primer curso
        if (res->next())
        {
            primerCurso = res->getString("curso");
        }

        // Liberar recursos
        delete stmt;
        delete res;
    }
    catch (sql::SQLException &e)
    {
        std::cerr << "Error al obtener los cursos de las solicitudes: " << e.what() << std::endl;
    }

    return primerCurso; // Retorna el primer curso encontrado (vacío si no se encontró ninguno)
}

std::vector<std::string> System::obtenerUniversidadesConPlazasDisponiblesParaAlumno(const std::string &campo, int anioCurso)
{
    std::vector<std::string> universidades;

    try
    {
        // Paso 1: Obtener los IDs de universidades con carreras en el campo especificado y año de curso válido
        sql::PreparedStatement *stmtCarrera = this->getConnection()->prepareStatement(
            "SELECT DISTINCT universidad_id "
            "FROM Carrera "
            "WHERE campo = ? "
            "AND anioCurso >= ?");

        // Establecer los parámetros para la consulta
        stmtCarrera->setString(1, campo);
        stmtCarrera->setInt(2, anioCurso);

        // Ejecutar la consulta
        sql::ResultSet *resCarrera = stmtCarrera->executeQuery();

        // Paso 2: Recorrer los IDs de las universidades y verificar la disponibilidad de plazas
        while (resCarrera->next())
        {
            int universidadId = resCarrera->getInt("universidad_id");

            // Verificar si la universidad tiene plazas disponibles
            sql::PreparedStatement *stmtPlazas = this->getConnection()->prepareStatement(
                "SELECT (maximo_alumnos - numero_ocupados_alumnos) AS plazas_disponibles "
                "FROM Universidad "
                "WHERE universidad_id = ?");
            stmtPlazas->setInt(1, universidadId);

            sql::ResultSet *resPlazas = stmtPlazas->executeQuery();
            if (resPlazas->next() && resPlazas->getInt("plazas_disponibles") > 0)
            {
                // Si hay plazas disponibles, obtener el nombre de la universidad
                sql::PreparedStatement *stmtNombre = this->getConnection()->prepareStatement(
                    "SELECT nombre FROM Universidad WHERE universidad_id = ?");
                stmtNombre->setInt(1, universidadId);

                sql::ResultSet *resNombre = stmtNombre->executeQuery();
                if (resNombre->next())
                {
                    std::string universidad = resNombre->getString("nombre");
                    universidades.push_back(universidad);
                }

                // Liberar recursos de la consulta de nombres
                delete stmtNombre;
                delete resNombre;
            }

            // Liberar recursos de la consulta de plazas
            delete stmtPlazas;
            delete resPlazas;
        }

        // Liberar recursos de la consulta principal
        delete stmtCarrera;
        delete resCarrera;

        return universidades;
    }
    catch (sql::SQLException &e)
    {
        std::cerr << "Error al conectar o consultar la base de datos: AQUI " << e.what() << std::endl;
    }

    return universidades; // Si hubo algún error, devolver un vector vacío
}

void System::insertarSolicitudesSicueAlumno(int solicitante_id, const std::string &nombre_solicitante, const std::string &universidad_origen, const std::vector<std::string> &universidades_destino, const std::string &carrera, const std::string &curso)
{
    try
    {
        // Paso 1: Obtener el año de curso del alumno
        int añoCursoAlumno = this->obtenerAñoCursoAlumno(nombre_solicitante, carrera);

        // Verificar que el curso solicitado sea compatible con el año de curso del alumno
        std::string añoCursoSolicitado = curso.substr(0, 4); // Obtener el año de la solicitud de curso (YYYY)
        if (std::stoi(añoCursoSolicitado) <= añoCursoAlumno)
        {
            std::cerr << "El año del curso solicitado no es válido para el alumno (no puede solicitar SICUE para un año inferior o igual al suyo).\n";
            return; // Salir si el año del curso solicitado no es válido
        }

        // Paso 2: Verificar si hay plazas disponibles en las universidades de destino
        for (const auto &universidad_destino : universidades_destino)
        {

            // Paso 3: Insertar la solicitud SICUE en la tabla SolicitudSicueAlumno
            sql::PreparedStatement *stmtSolicitud = this->getConnection()->prepareStatement(
                "INSERT INTO SolicitudSicueALUMNO (nombre_solicitante, solicitante_id, universidad_origen, universidad_destino, carrera, curso, estado_solicitud, fecha_solicitud) "
                "VALUES (?, ?, ?, ?, ?, ?, 'Pendiente', NOW())");

            // Establecer los parámetros de la solicitud
            stmtSolicitud->setString(1, nombre_solicitante);
            stmtSolicitud->setInt(2, solicitante_id);
            stmtSolicitud->setString(3, universidad_origen);
            stmtSolicitud->setString(4, universidad_destino);
            stmtSolicitud->setString(5, carrera);
            stmtSolicitud->setString(6, curso);

            // Ejecutar la consulta
            stmtSolicitud->executeUpdate();
            delete stmtSolicitud;

            // Paso 4: Obtener el ID de la universidad de destino
            sql::PreparedStatement *stmtUniversidad = this->getConnection()->prepareStatement(
                "SELECT universidad_id FROM Universidad WHERE nombre = ?");
            stmtUniversidad->setString(1, universidad_destino);
            sql::ResultSet *resUniversidad = stmtUniversidad->executeQuery();

            int universidad_id = -1;
            if (resUniversidad->next())
            {
                universidad_id = resUniversidad->getInt("universidad_id");
            }
            delete stmtUniversidad;
            delete resUniversidad;

            // Verificar si se encontró el ID de la universidad
            if (universidad_id == -1)
            {
                std::cerr << "Error: No se encontró la universidad " << universidad_destino << " en la base de datos." << std::endl;
                continue; // Saltar a la siguiente universidad
            }

            // Paso 5: Incrementar el número de alumnos ocupados en la universidad de destino
            sql::PreparedStatement *stmtActualizar = this->getConnection()->prepareStatement(
                "UPDATE Universidad SET numero_ocupados_alumnos = numero_ocupados_alumnos + 1 WHERE universidad_id = ?");
            stmtActualizar->setInt(1, universidad_id);

            // Ejecutar la actualización
            stmtActualizar->executeUpdate();
            delete stmtActualizar;

            std::cout << "La solicitud SICUE para la universidad " << universidad_destino
                      << " ha sido procesada correctamente y el número de alumnos ocupados ha sido actualizado.\n";
        }
    }
    catch (sql::SQLException &e)
    {
        std::cerr << "Error al conectar o consultar la base de datos: " << e.what() << std::endl;
    }
}

int System::obtenerIdUsuarioPorUsuario(const std::string &usuario)
{
    try
    {
        // Preparar la consulta SQL para obtener la ID del usuario desde la base de datos
        sql::PreparedStatement *stmt = this->getConnection()->prepareStatement(
            "SELECT user_id FROM User WHERE nombre = ?");

        // Establecer el nombre de usuario en la consulta
        stmt->setString(1, usuario);

        // Ejecutar la consulta
        sql::ResultSet *res = stmt->executeQuery();

        if (res->next()) // Si se encuentra el usuario
        {
            // Obtener la ID de usuario de la base de datos
            int id_usuario = res->getInt("user_id");

            delete stmt;
            delete res;
            return id_usuario; // Devolver la ID del usuario
        }
        else
        {
            delete stmt;
            delete res;
            return -1; // No se encontró el usuario
        }
    }
    catch (sql::SQLException &e)
    {
        std::cerr << "Error al conectar o consultar la base de datos: " << e.what() << std::endl;
        return -1; // Error de conexión o consulta
    }
}

std::string System::obtenerUniversidadOrigenPorIdUsuario(int idUsuario)
{
    try
    {
        // Preparar la consulta SQL para obtener la universidad de origen del usuario desde la base de datos
        sql::PreparedStatement *stmt = this->getConnection()->prepareStatement(
            "SELECT universidad_origen FROM User WHERE user_id = ?");

        // Establecer la ID del usuario en la consulta
        stmt->setInt(1, idUsuario);

        // Ejecutar la consulta
        sql::ResultSet *res = stmt->executeQuery();

        if (res->next()) // Si se encuentra el usuario
        {
            // Obtener la universidad de origen
            std::string universidad_origen = res->getString("universidad_origen");

            delete stmt;
            delete res;
            return universidad_origen; // Devolver la universidad de origen
        }
        else
        {
            delete stmt;
            delete res;
            return ""; // No se encontró el usuario o universidad
        }
    }
    catch (sql::SQLException &e)
    {
        std::cerr << "Error al conectar o consultar la base de datos: " << e.what() << std::endl;
        return ""; // Error de conexión o consulta
    }
}

int System::obtenerAnyoAcademicoPorIdUsuario(int idUsuario)
{
    try
    {
        // Preparar la consulta SQL para obtener el año académico del usuario desde la base de datos
        sql::PreparedStatement *stmt = this->getConnection()->prepareStatement(
            "SELECT curso_academico FROM User WHERE user_id = ?");

        // Establecer la ID del usuario en la consulta
        stmt->setInt(1, idUsuario);

        // Ejecutar la consulta
        sql::ResultSet *res = stmt->executeQuery();

        if (res->next()) // Si se encuentra el usuario
        {
            // Obtener el año académico
            int anyo_academico = res->getInt("curso_academico");

            delete stmt;
            delete res;
            return anyo_academico; // Devolver el año académico
        }
        else
        {
            delete stmt;
            delete res;
            return -1; // No se encontró el usuario o el año académico
        }
    }
    catch (sql::SQLException &e)
    {
        std::cerr << "Error al conectar o consultar la base de datos: " << e.what() << std::endl;
        return -1; // Error de conexión o consulta
    }
}

int System::obtenerAñoCursoAlumno(const std::string &usuario, const std::string &carrera)
{
    try
    {
        // Preparamos la consulta SQL para obtener el año académico de un alumno según su usuario y carrera
        sql::PreparedStatement *stmt = this->getConnection()->prepareStatement(
            "SELECT año_academico FROM Alumno "
            "JOIN User ON Alumno.user_id = User.user_id "
            "WHERE User.nombre = ? AND Alumno.carrera = ?");

        // Establecer los parámetros de la consulta
        stmt->setString(1, usuario); // Nombre del usuario
        stmt->setString(2, carrera); // Carrera del alumno

        // Ejecutar la consulta
        sql::ResultSet *res = stmt->executeQuery();

        if (res->next()) // Si encontramos el alumno
        {
            // Obtener el año académico de la consulta
            int añoCurso = res->getInt("año_academico");

            delete stmt;
            delete res;

            return añoCurso; // Devolver el año académico del alumno
        }
        else
        {
            delete stmt;
            delete res;

            std::cout << "No se encontró el alumno o la carrera especificada.\n";
            return -1; // Error si no se encuentra el alumno o la carrera
        }
    }
    catch (sql::SQLException &e)
    {
        std::cerr << "Error al consultar la base de datos: " << e.what() << std::endl;
        return -1; // Error de conexión o consulta
    }
}

std::string System::obtenerCarreraPorIdUsuario(int userId)
{
    try
    {
        // Preparar la consulta SQL para obtener la carrera de un alumno por su user_id
        sql::PreparedStatement *stmt = this->getConnection()->prepareStatement(
            "SELECT carrera FROM Alumno "
            "WHERE Alumno.user_id = ?");

        // Establecer el parámetro para el user_id
        stmt->setInt(1, userId);

        // Ejecutar la consulta
        sql::ResultSet *res = stmt->executeQuery();

        if (res->next()) // Si encontramos el alumno con el user_id proporcionado
        {
            // Obtener la carrera de la consulta
            std::string carrera = res->getString("carrera");

            delete stmt;
            delete res;

            return carrera; // Devolver la carrera del alumno
        }
        else
        {
            delete stmt;
            delete res;

            std::cout << "No se encontró el alumno con el user_id especificado.\n";
            return ""; // Si no se encuentra el alumno, devolver una cadena vacía
        }
    }
    catch (sql::SQLException &e)
    {
        std::cerr << "Error al consultar la base de datos: " << e.what() << std::endl;
        return ""; // Error de conexión o consulta, devolver una cadena vacía
    }
}

std::string System::obtenerNombreUniversidadPorCarrera(const std::string &carrera)
{
    try
    {
        // Paso 1: Obtener el universidad_id desde la tabla Carrera
        sql::PreparedStatement *stmtCarrera = this->getConnection()->prepareStatement(
            "SELECT universidad_id FROM Carrera "
            "WHERE carrera = ?");

        // Establecer el parámetro de la carrera
        stmtCarrera->setString(1, carrera);

        // Ejecutar la consulta
        sql::ResultSet *resCarrera = stmtCarrera->executeQuery();

        if (resCarrera->next())
        {
            // Obtener el universidad_id como entero
            int universidadId = resCarrera->getInt("universidad_id");

            delete stmtCarrera;
            delete resCarrera;

            // Paso 2: Usar el universidad_id para obtener el nombre de la universidad desde la tabla Universidad
            sql::PreparedStatement *stmtUniversidad = this->getConnection()->prepareStatement(
                "SELECT nombre FROM Universidad "
                "WHERE universidad_id = ?");

            stmtUniversidad->setInt(1, universidadId);

            sql::ResultSet *resUniversidad = stmtUniversidad->executeQuery();

            if (resUniversidad->next())
            {
                // Obtener el nombre de la universidad
                std::string nombreUniversidad = resUniversidad->getString("nombre");

                delete stmtUniversidad;
                delete resUniversidad;

                return nombreUniversidad; // Devolver el nombre de la universidad
            }
            else
            {
                delete stmtUniversidad;
                delete resUniversidad;

                std::cout << "No se encontró la universidad con el ID especificado.\n";
                return ""; // Si no se encuentra la universidad, devolver una cadena vacía
            }
        }
        else
        {
            delete stmtCarrera;
            delete resCarrera;

            std::cout << "No se encontró una carrera con el nombre especificado.\n";
            return ""; // Si no se encuentra la carrera, devolver una cadena vacía
        }
    }
    catch (sql::SQLException &e)
    {
        std::cerr << "Error al consultar la base de datos: " << e.what() << std::endl;
        return ""; // En caso de error, devolver una cadena vacía
    }
}

std::string System::obtenerCursoAcademicoPorIdUsuario(int idUsuario)
{
    try
    {
        // Preparar la consulta SQL para obtener el curso académico del usuario
        sql::PreparedStatement *stmt = this->getConnection()->prepareStatement(
            "SELECT curso_academico FROM User WHERE user_id = ?");

        // Establecer el ID del usuario en la consulta
        stmt->setInt(1, idUsuario);

        // Ejecutar la consulta
        sql::ResultSet *res = stmt->executeQuery();

        if (res->next())
        { // Si se encuentra el usuario
            // Obtener el curso académico
            std::string curso_academico = res->getString("curso_academico");

            delete stmt;
            delete res;
            return curso_academico; // Devolver el curso académico
        }
        else
        {
            delete stmt;
            delete res;
            return ""; // No se encontró el usuario
        }
    }
    catch (sql::SQLException &e)
    {
        std::cerr << "Error al conectar o consultar la base de datos: " << e.what() << std::endl;
        return ""; // Error de conexión o consulta
    }
}

std::vector<std::string> System::obtenerUniversidadesYaSolicitadasAlumno(const std::string &usuario)
{
    std::vector<std::string> universidadesSolicitadas;

    try
    {
        // Obtener el solicitante_id del usuario
        int solicitante_id = this->obtenerUserIdPorNombre(usuario);

        if (solicitante_id == -1)
        {
            std::cerr << "Error: No se encontró el usuario especificado." << std::endl;
            return universidadesSolicitadas;
        }

        // Preparar la consulta SQL
        sql::PreparedStatement *stmtSolicitudes = this->getConnection()->prepareStatement(
            "SELECT DISTINCT universidad_destino "
            "FROM SolicitudSicueALUMNO "
            "WHERE solicitante_id = ?");

        stmtSolicitudes->setInt(1, solicitante_id);

        // Ejecutar la consulta
        sql::ResultSet *res = stmtSolicitudes->executeQuery();

        // Almacenar los nombres de las universidades
        while (res->next())
        {
            universidadesSolicitadas.push_back(res->getString("universidad_destino"));
        }

        // Liberar recursos
        delete stmtSolicitudes;
        delete res;
    }
    catch (sql::SQLException &e)
    {
        std::cerr << "Error al conectar o consultar la base de datos: " << e.what() << std::endl;
    }

    return universidadesSolicitadas;
}

bool System::esPlazaDisponibleParaCurso(const std::string &universidad, const std::string &carrera, int añoCurso)
{
    try
    {
        // Preparar la consulta SQL para obtener los datos de la universidad y verificar disponibilidad de plazas
        sql::PreparedStatement *stmt = this->getConnection()->prepareStatement(
            "SELECT u.numero_ocupados_alumnos, u.maximo_alumnos "
            "FROM Universidad u "
            "WHERE u.nombre = ?");

        stmt->setString(1, universidad); // Establecer el nombre de la universidad

        // Ejecutar la consulta
        sql::ResultSet *res = stmt->executeQuery();

        // Comprobar si hay resultados
        if (res->next())
        {
            int numeroOcupados = res->getInt("numero_ocupados_alumnos");
            int maximoAlumnos = res->getInt("maximo_alumnos");

            // Si el número de alumnos ocupados es menor que el máximo permitido, hay plazas disponibles
            if ((maximoAlumnos - numeroOcupados) > 0)
            {
                // Liberar recursos
                delete stmt;
                delete res;
                return true; // Hay plazas disponibles
            }
        }

        // Liberar recursos
        delete stmt;
        delete res;

        return false; // No hay plazas disponibles
    }
    catch (sql::SQLException &e)
    {
        std::cerr << "Error al conectar o consultar la base de datos: " << e.what() << std::endl;
        return false; // Si ocurre un error, no consideramos que haya plazas disponibles
    }
}

std::string System::obtenerCampoPorCarrera(const std::string &nombre_carrera)
{
    try
    {
        // Preparar la consulta para obtener el campo de la carrera
        sql::PreparedStatement *stmt = this->getConnection()->prepareStatement(
            "SELECT campo "
            "FROM Carrera "
            "WHERE carrera = ?");

        // Establecer el parámetro para la carrera
        stmt->setString(1, nombre_carrera);

        // Ejecutar la consulta
        sql::ResultSet *res = stmt->executeQuery();

        std::string campo = "";

        // Si encontramos una fila que coincide con la carrera, obtenemos el campo
        if (res->next())
        {
            campo = res->getString("campo");
        }

        // Liberar recursos
        delete stmt;
        delete res;

        return campo;
    }
    catch (sql::SQLException &e)
    {
        std::cerr << "Error al obtener el campo: " << e.what() << std::endl;
    }

    return ""; // Si ocurre un error o no se encuentra la carrera, devolver una cadena vacía
}

void System::modificarSolicitudSicueAlumno(const std::string &nombre_solicitante, const std::string &nuevoCurso, const std::string &nuevaUniversidadDestino)
{
    try
    {
        System system;                                 // Crear una instancia de la clase System
        sql::Connection *conn = this->getConnection(); // Obtener conexión a la base de datos
        // Preparar la consulta SQL para buscar la solicitud del solicitante en la tabla
        sql::PreparedStatement *stmt = conn->prepareStatement(
            "SELECT * FROM SolicitudSicueALUMNO WHERE nombre_solicitante = ?");

        // Establecer el parámetro para el nombre del solicitante
        stmt->setString(1, nombre_solicitante);

        // Ejecutar la consulta
        sql::ResultSet *res = stmt->executeQuery();

        // Verificar si se encontró la solicitud
        if (!res->next())
        {
            std::cout << "No se encontró una solicitud para el solicitante: " << nombre_solicitante << std::endl;
            delete stmt;
            delete res;
            return;
        }

        // Obtener los datos actuales de la solicitud
        std::string universidadOrigen = res->getString("universidad_origen");
        std::string universidadDestino = res->getString("universidad_destino");
        std::string cursoActual = res->getString("curso");

        // Modificar los campos solicitados por el usuario
        std::string nuevoCursoModificado = nuevoCurso.empty() ? cursoActual : nuevoCurso;
        std::string nuevaUniversidadDestinoModificada = nuevaUniversidadDestino.empty() ? universidadDestino : nuevaUniversidadDestino;

        // Si se ha cambiado la universidad de destino, actualizamos las plazas ocupadas
        if (nuevaUniversidadDestinoModificada != universidadDestino)
        {
            // Llamamos a la función para actualizar las plazas ocupadas en la base de datos
            system.actualizarPlazasOcupadas(universidadDestino, nuevaUniversidadDestinoModificada);
        }

        // Preparar la consulta SQL para actualizar la solicitud
        sql::PreparedStatement *updateStmt = conn->prepareStatement(
            "UPDATE SolicitudSicueALUMNO SET curso = ?, universidad_destino = ? WHERE nombre_solicitante = ?");

        // Establecer los parámetros de la consulta UPDATE
        updateStmt->setString(1, nuevoCursoModificado);
        updateStmt->setString(2, nuevaUniversidadDestinoModificada);
        updateStmt->setString(3, nombre_solicitante);

        // Ejecutar la consulta de actualización
        int rowsUpdated = updateStmt->executeUpdate();

        // Verificar si se actualizó correctamente
        if (rowsUpdated > 0)
        {
            std::cout << "La solicitud ha sido modificada correctamente.\n";
        }
        else
        {
            std::cout << "No se pudo modificar la solicitud.\n";
        }

        // Liberar los recursos
        delete stmt;
        delete res;
        delete updateStmt;
    }
    catch (sql::SQLException &e)
    {
        std::cerr << "Error al conectar o modificar la base de datos: " << e.what() << std::endl;
    }
}

int System::obtenerUniversidad(int universidad_id)
{
    try
    {
        // Preparar la consulta SQL para obtener todos los datos de la universidad
        sql::PreparedStatement *stmt = this->getConnection()->prepareStatement(
            "SELECT * FROM Universidad WHERE universidad_id = ?");

        // Establece el ID de la universidad en la consulta
        stmt->setInt(1, universidad_id);

        // Ejecutar la consulta
        sql::ResultSet *res = stmt->executeQuery();

        if (res->next()) // Si se encuentra la universidad
        {
            // Obtener los datos de la universidad
            int id = res->getInt("universidad_id");
            std::string nombre = res->getString("nombre");
            std::string direccion = res->getString("direccion");
            std::string ciudad = res->getString("ciudad");
            int maximo_alumnos = res->getInt("maximo_alumnos");
            int numero_ocupados_alumnos = res->getInt("numero_ocupados_alumnos");
            int maximo_profesores = res->getInt("maximo_profesores");
            int numero_ocupados_profesores = res->getInt("numero_ocupados_profesores");

            // Imprimir los detalles de la universidad
            std::cout << "Universidad ID: " << id << std::endl;
            std::cout << "Nombre: " << nombre << std::endl;
            std::cout << "Dirección: " << direccion << std::endl;
            std::cout << "Ciudad: " << ciudad << std::endl;
            std::cout << "Máximo Alumnos: " << maximo_alumnos << std::endl;
            std::cout << "Plazas Ocupadas (Alumnos): " << numero_ocupados_alumnos << std::endl;
            std::cout << "Máximo Profesores: " << maximo_profesores << std::endl;
            std::cout << "Plazas Ocupadas (Profesores): " << numero_ocupados_profesores << std::endl;

            // Limpiar recursos
            delete stmt;
            delete res;
            return 1; // Universidad encontrada y mostrada
        }
        else
        {
            // Si no se encuentra la universidad
            delete stmt;
            delete res;
            return -2; // Universidad no encontrada
        }
    }
    catch (sql::SQLException &e)
    {
        std::cerr << "Error al conectar o consultar la base de datos: " << e.what() << std::endl;
        return 0; // Error de conexión o consulta
    }
}

void System::consultarPlazas()
{
    try
    {
        // Preparar la consulta SQL para obtener todas las universidades
        sql::PreparedStatement *stmt = this->getConnection()->prepareStatement(
            "SELECT * FROM Universidad");

        // Ejecutar la consulta
        sql::ResultSet *res = stmt->executeQuery();

        // Imprimir los resultados de las universidades
        std::cout << "==================== Universidades ====================" << std::endl;
        while (res->next()) // Recorre todas las universidades
        {
            // Obtener los datos de la universidad
            int id = res->getInt("universidad_id");
            std::string nombre = res->getString("nombre");
            std::string direccion = res->getString("direccion");
            std::string ciudad = res->getString("ciudad");
            int maximo_alumnos = res->getInt("maximo_alumnos");
            int numero_ocupados_alumnos = res->getInt("numero_ocupados_alumnos");
            int maximo_profesores = res->getInt("maximo_profesores");
            int numero_ocupados_profesores = res->getInt("numero_ocupados_profesores");

            // Imprimir los detalles de cada universidad
            std::cout << "--------------------------------------------------------" << std::endl;
            std::cout << "Universidad ID: " << id << std::endl;
            std::cout << "Nombre: " << nombre << std::endl;
            std::cout << "Dirección: " << direccion << std::endl;
            std::cout << "Ciudad: " << ciudad << std::endl;
            std::cout << "Máximo Alumnos: " << maximo_alumnos << std::endl;
            std::cout << "Plazas Ocupadas (Alumnos): " << numero_ocupados_alumnos << std::endl;
            std::cout << "Máximo Profesores: " << maximo_profesores << std::endl;
            std::cout << "Plazas Ocupadas (Profesores): " << numero_ocupados_profesores << std::endl;
            std::cout << "--------------------------------------------------------" << std::endl;
        }

        // Limpiar recursos
        delete stmt;
        delete res;
    }
    catch (sql::SQLException &e)
    {
        std::cerr << "Error al conectar o consultar la base de datos: " << e.what() << std::endl;
    }
}
