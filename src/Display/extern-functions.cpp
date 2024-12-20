#include "extern-functions.hpp"

void mostrarMenuProfesor()
{
    std::cout << "\n=====================\n";
    std::cout << "1. Solicitud de SICUE\n";
    std::cout << "2. Modificar solicitud SICUE\n";
    std::cout << "3. Desloguearse\n";
    std::cout << "=====================\n";
}

bool esFechaValida(const std::string &curso)
{
    // Verificar que el curso tenga el formato YYYY/YYYY con exactamente 4 dígitos, barra y otros 4 dígitos.
    std::regex formato(R"(\d{4}/\d{4})");
    if (!std::regex_match(curso, formato))
    {
        std::cout << "Error: El formato del curso es incorrecto. Debe ser YYYY/YYYY, sin caracteres especiales.\n";
        return false;
    }

    // Obtener el año actual
    time_t t = time(0); // Obtener el tiempo actual
    tm *now = localtime(&t);
    int year_actual = now->tm_year + 1900; // El año actual

    // Separar los años de la cadena curso
    size_t pos = curso.find('/');
    std::string year_inicio = curso.substr(0, pos);
    std::string year_fin = curso.substr(pos + 1);

    // Convertir los años de la cadena a enteros
    int inicio = std::stoi(year_inicio);
    int fin = std::stoi(year_fin);

    // Verificar que el año de inicio sea al menos el próximo año
    if (inicio <= year_actual)
    {
        std::cout << "Error: El año de inicio debe ser al menos el próximo año.\n";
        return false;
    }

    // Verificar que el curso no difiera más de 2 años del año actual
    if (inicio > (year_actual + 2))
    {
        std::cout << "Error: El curso no puede diferir más de dos años del año actual.\n";
        return false;
    }

    // Verificar que el segundo año sea igual o mayor que el primer año
    if (fin < inicio)
    {
        std::cout << "Error: El año de fin no puede ser anterior al año de inicio.\n";
        return false;
    }

    // Verificar que la duración del curso no sea mayor a 2 años
    if (fin - inicio > 2)
    {
        std::cout << "Error: La duración del curso no puede ser mayor a dos años.\n";
        return false;
    }

    // Si todo es válido
    return true;
}

void modificarSolicitudSicueProfesor(Profesor profe)
{
    System system; // Crear una instancia de la clase System
    std::string Curso;
    std::vector<std::string> solicitudes = system.obtenerUniversidadesYaSolicitadas(profe.getUsuario());

    // Verificar si el profesor tiene solicitudes SICUE previas
    if (solicitudes.empty())
    {
        std::cout << "No tiene solicitudes SICUE previas.\n";
        return;
    }

    // Mostrar las solicitudes existentes
    std::cout << "Seleccione la solicitud que desea modificar:\n";
    for (size_t i = 0; i < solicitudes.size(); ++i)
    {
        std::cout << i + 1 << ". Universidad de destino: " << solicitudes[i] << ", Curso: " << Curso << std::endl;
    }

    int seleccion;
    std::cout << "Ingrese el número de la solicitud que desea modificar (0 para cancelar): ";
    std::cin >> seleccion;

    // Validación de la selección
    if (seleccion == 0)
    {
        std::cout << "Modificación cancelada.\n";
        return;
    }

    // Verificar si la selección es válida
    if (seleccion < 1 || seleccion > solicitudes.size())
    {
        std::cout << "Selección inválida. Modificación cancelada.\n";
        return;
    }

    // Mostrar los campos que se pueden modificar
    std::string universidadDestinoActualAModificar = solicitudes[seleccion - 1]; // Obtener la universidad de destino seleccionada
    std::string cursoActual = Curso;

    // Modificar el curso (si lo desea modificar)
    std::string nuevoCurso;
    std::cout << "Curso actual: " << cursoActual << "\n";
    std::cout << "Introduzca el nuevo curso (YYYY/YYYY) o presione Enter para mantener el anterior: ";
    std::cin.ignore(); // Limpiar el buffer de entrada
    std::getline(std::cin, nuevoCurso);
    // Si no se introdujo un nuevo curso, mantener el anterior
    if (nuevoCurso.empty())
    {
        nuevoCurso = cursoActual;
    }

    // Filtrar las universidades disponibles
    std::vector<std::string> universidadesPosibles = system.obtenerUniversidadesConPlazasDisponiblesParaProfesor(profe.getDepartamento());

    // Eliminar universidades ya solicitadas y la universidad actual en la que el profesor está
    universidadesPosibles.erase(std::remove_if(universidadesPosibles.begin(), universidadesPosibles.end(),
                                               [&solicitudes, universidadDestinoActualAModificar](const std::string &universidad)
                                               {
                                                   return std::find(solicitudes.begin(), solicitudes.end(), universidad) != solicitudes.end() || universidad == universidadDestinoActualAModificar;
                                               }),
                                universidadesPosibles.end());

    // Declarar la variable nuevaUniversidadDestino antes de la validación
    std::string nuevaUniversidadDestino = universidadDestinoActualAModificar; // Inicializarla con la universidad actual

    // Verificar si hay universidades disponibles
    if (universidadesPosibles.empty())
    {
        std::cout << "No hay universidades disponibles para su departamento que no haya solicitado o en las que ya esté.\n";
        std::cout << "Pasando al siguiente paso...\n";
    }
    else
    {
        // Mostrar las universidades disponibles solo si hay alguna
        std::cout << "Universidades disponibles para su departamento (que no ha solicitado y no está en su universidad actual):\n";
        for (size_t i = 0; i < universidadesPosibles.size(); ++i)
        {
            std::cout << i + 1 << ". " << universidadesPosibles[i] << std::endl;
        }

        std::cout << "Universidad de destino actual: " << universidadDestinoActualAModificar << "\n";
        std::cout << "Introduzca la nueva universidad de destino o presione Enter para mantener la anterior: ";
        std::getline(std::cin, nuevaUniversidadDestino);

        // Si no se introdujo una nueva universidad, mantener la anterior
        if (nuevaUniversidadDestino.empty())
        {
            nuevaUniversidadDestino = universidadDestinoActualAModificar;
        }

        // Si se modificó la universidad de destino, validamos que esté disponible
        if (nuevaUniversidadDestino != universidadDestinoActualAModificar)
        {
            if (std::find(universidadesPosibles.begin(), universidadesPosibles.end(), nuevaUniversidadDestino) == universidadesPosibles.end())
            {
                std::cout << "La universidad seleccionada no tiene plazas disponibles. Modificación cancelada.\n";
                return;
            }
        }
    }

    // Si no se ha realizado ninguna modificación, no hacemos nada
    if (nuevoCurso == cursoActual && nuevaUniversidadDestino == universidadDestinoActualAModificar)
    {
        std::cout << "No se han realizado cambios en la solicitud.\n";
        return;
    }

    // Realizamos la modificación de la solicitud
    system.modificarSolicitudSicueProfesor(profe.getUsuario(), nuevoCurso, nuevaUniversidadDestino);
}

void solicitudSicueProfesor(Profesor profe)
{
    System system; // Crear una instancia de la clase System
    std::vector<std::string> universidadesDisponibles;
    universidadesDisponibles = system.obtenerUniversidadesConPlazasDisponiblesParaProfesor(profe.getDepartamento());

    // Eliminar la universidad de origen del profesor
    universidadesDisponibles.erase(
        std::remove(universidadesDisponibles.begin(), universidadesDisponibles.end(), profe.getUniversidadOrigen()),
        universidadesDisponibles.end());

    // Obtener las universidades ya solicitadas por el profesor
    std::vector<std::string> universidadesYaSolicitadas = system.obtenerUniversidadesYaSolicitadas(profe.getUsuario());

    // Eliminar las universidades ya solicitadas del vector de disponibles
    for (const auto &universidad : universidadesYaSolicitadas)
    {
        universidadesDisponibles.erase(
            std::remove(universidadesDisponibles.begin(), universidadesDisponibles.end(), universidad),
            universidadesDisponibles.end());
    }

    // Validación: Asegurarse de que quedan universidades con plazas disponibles
    if (!universidadesDisponibles.empty())
    {
        std::cout << "\nFormulario de Solicitud de SICUE\n";
        // Mostrar las universidades disponibles para el profesor
        std::cout << "\nUniversidades con plazas disponibles para su departamento:\n";
        for (size_t i = 0; i < universidadesDisponibles.size(); ++i)
        {
            std::cout << i + 1 << ". " << universidadesDisponibles[i] << std::endl;
        }

        std::vector<std::string> universidadesSeleccionadas;
        int seleccion;
        int maxOpciones = 8;

        while (universidadesSeleccionadas.size() < maxOpciones)
        {
            std::cout << "\nSeleccione la universidad a la que desea solicitar el intercambio (ingrese el número, o 0 para terminar): ";
            std::cin >> seleccion;

            // Si el usuario ingresa 0, terminamos la selección
            if (seleccion == 0)
            {
                break;
            }

            // Validar la selección
            if (seleccion >= 1 && seleccion <= universidadesDisponibles.size())
            {
                std::string universidadSeleccionada = universidadesDisponibles[seleccion - 1];

                // Verificar si ya ha seleccionado esta universidad
                if (std::find(universidadesSeleccionadas.begin(), universidadesSeleccionadas.end(), universidadSeleccionada) != universidadesSeleccionadas.end())
                {
                    std::cout << "Ya ha seleccionado esta universidad. Elija otra.\n";
                }
                else
                {
                    // Verificar si la universidad seleccionada tiene plazas disponibles antes de agregarla
                    if (system.tienePlazasDisponiblesParaProfesor(universidadSeleccionada))
                    {
                        universidadesSeleccionadas.push_back(universidadSeleccionada);
                        std::cout << "Ha seleccionado la universidad: " << universidadSeleccionada << std::endl;

                        if (universidadesSeleccionadas.size() < maxOpciones)
                        {
                            std::cout << "Puede seleccionar " << maxOpciones - universidadesSeleccionadas.size() << " universidad(es) más.\n";
                        }
                    }
                    else
                    {
                        std::cout << "La universidad seleccionada ya no tiene plazas disponibles para profesores.\n";
                    }
                }
            }
            else
            {
                std::cout << "Selección inválida. Intente nuevamente.\n";
            }
        }

        // Si el profesor ha seleccionado alguna universidad
        if (!universidadesSeleccionadas.empty())
        {
            std::cout << "\nHa seleccionado las siguientes universidades:\n";
            for (const auto &universidad : universidadesSeleccionadas)
            {
                std::cout << "- " << universidad << std::endl;
            }

            // Pedir el curso
            std::cout << "Introduzca el curso que desea hacer el intercambio con el formato YYYY/YYYY: ";
            std::string Curso;
            std::cin >> Curso;

            // Verificar si el curso es válido
            if (esFechaValida(Curso))
            {
                system.insertarSolicitudesSicueProfesor(system.obtenerUserIdPorNombre(profe.getUsuario()), profe.getUsuario(), profe.getUniversidadOrigen(), universidadesSeleccionadas, profe.getDepartamento(), Curso);
                std::cout << "Volviendo al menú de profesor." << std::endl;
                return;
            }
            else
            {
                std::cout << "La solicitud SICUE no ha sido procesada debido a un error en la fecha del curso.\n";
                std::cout << "Volviendo al menú de profesor." << std::endl;
                return;
            }
        }
        else
        {
            std::cout << "No ha seleccionado ninguna universidad. La solicitud no ha sido procesada.\n";
            std::cout << "Volviendo al menú de profesor." << std::endl;
            return;
        }
    }
    else
    {
        std::cout << "\nNo hay ningún programa SICUE disponible para usted.\n";
        std::cout << "Volviendo al menú de profesor." << std::endl;
        return;
    }
}

// ALUMNOS
void mostrarMenuAlumno()
{
    std::cout << "\n=====================\n";
    std::cout << "1. Solicitud de SICUE\n";
    std::cout << "2. Modificar solicitud SICUE\n";
    std::cout << "3. Desloguearse\n";
    std::cout << "=====================\n";
}

void solicitudSicueAlumno(Alumno alumno)
{
    System system; // Crear una instancia de la clase System
    std::vector<std::string> universidadesDisponibles;
    int alumno_id = system.obtenerIdUsuarioPorUsuario(alumno.getUsuario());

    // Obtener el campo de la carrera a partir del alumno
    std::string nombre_carrera = system.obtenerCarreraPorIdUsuario(alumno_id);
    std::string nombre_universidad = system.obtenerNombreUniversidadPorCarrera(nombre_carrera);
    std::string campo = system.obtenerCampoPorCarrera(nombre_carrera);

    // Obtener universidades con plazas disponibles para el campo
    universidadesDisponibles = system.obtenerUniversidadesConPlazasDisponiblesParaAlumno(campo, system.obtenerAnyoAcademicoPorIdUsuario(alumno_id));

    // Eliminar la universidad de origen del alumno
    universidadesDisponibles.erase(
        std::remove(universidadesDisponibles.begin(), universidadesDisponibles.end(), nombre_universidad),
        universidadesDisponibles.end());

    // Obtener las universidades ya solicitadas por el alumno
    std::vector<std::string> universidadesYaSolicitadas = system.obtenerUniversidadesYaSolicitadas(alumno.getUsuario());

    // Eliminar las universidades ya solicitadas del vector de disponibles
    for (const auto &universidad : universidadesYaSolicitadas)
    {
        universidadesDisponibles.erase(
            std::remove(universidadesDisponibles.begin(), universidadesDisponibles.end(), universidad),
            universidadesDisponibles.end());
    }

    // Verificar si hay universidades disponibles
    if (!universidadesDisponibles.empty())
    {
        std::cout << "\nFormulario de Solicitud de SICUE\n";
        // Mostrar las universidades disponibles para el alumno
        std::cout << "\nUniversidades con plazas disponibles para su carrera:\n";
        for (size_t i = 0; i < universidadesDisponibles.size(); ++i)
        {
            std::cout << i + 1 << ". " << universidadesDisponibles[i] << std::endl;
        }

        std::vector<std::string> universidadesSeleccionadas;
        int seleccion;
        int maxOpciones = 8;

        // Proceso de selección de universidades
        while (universidadesSeleccionadas.size() < maxOpciones)
        {
            std::cout << "\nSeleccione la universidad a la que desea solicitar el intercambio (ingrese el número, o 0 para terminar): ";
            std::cin >> seleccion;

            // Si el usuario ingresa 0, terminamos la selección
            if (seleccion == 0)
            {
                break;
            }

            // Validar la selección
            if (seleccion >= 1 && seleccion <= universidadesDisponibles.size())
            {
                std::string universidadSeleccionada = universidadesDisponibles[seleccion - 1];

                // Verificar si ya ha seleccionado esta universidad
                if (std::find(universidadesSeleccionadas.begin(), universidadesSeleccionadas.end(), universidadSeleccionada) != universidadesSeleccionadas.end())
                {
                    std::cout << "Ya ha seleccionado esta universidad. Elija otra.\n";
                }
                else
                {
                    universidadesSeleccionadas.push_back(universidadSeleccionada);
                    std::cout << "Ha seleccionado la universidad: " << universidadSeleccionada << std::endl;

                    if (universidadesSeleccionadas.size() < maxOpciones)
                    {
                        std::cout << "Puede seleccionar " << maxOpciones - universidadesSeleccionadas.size() << " universidad(es) más.\n";
                    }
                }
            }
            else
            {
                std::cout << "Selección inválida. Intente nuevamente.\n";
            }
        }

        // Si el alumno ha seleccionado alguna universidad
        if (!universidadesSeleccionadas.empty())
        {
            std::cout << "\nHa seleccionado las siguientes universidades:\n";
            for (const auto &universidad : universidadesSeleccionadas)
            {
                std::cout << "- " << universidad << std::endl;

                // Verificar si la universidad seleccionada tiene plazas para el año curso adecuado
                bool esValida = system.esPlazaDisponibleParaCurso(universidad, alumno.getTitulacion(), system.obtenerAnyoAcademicoPorIdUsuario(alumno_id));
                if (!esValida)
                {
                    std::cout << "La universidad " << universidad << " no tiene plazas disponibles para su curso. No se procesará la solicitud para esta universidad.\n";
                    universidadesSeleccionadas.erase(std::remove(universidadesSeleccionadas.begin(), universidadesSeleccionadas.end(), universidad), universidadesSeleccionadas.end());
                }
            }

            // Si al menos una universidad válida ha sido seleccionada
            if (!universidadesSeleccionadas.empty())
            {
                // Pedir el curso
                std::cout << "Introduzca el curso que desea hacer el intercambio con el formato YYYY/YYYY: ";
                std::string Curso;
                std::cin >> Curso;

                // Verificar si el curso es válido
                if (esFechaValida(Curso))
                {
                    // Verificar que el alumno está en el curso adecuado para la solicitud
                    int añoCursoAlumno = system.obtenerAñoCursoAlumno(alumno.getUsuario(), alumno.getTitulacion());
                    if (añoCursoAlumno == 1 || añoCursoAlumno == 2 || añoCursoAlumno == 3 || añoCursoAlumno == 4)
                    {
                        // Comprobar si el curso solicitado es adecuado
                        system.insertarSolicitudesSicueAlumno(system.obtenerUserIdPorNombre(alumno.getUsuario()), alumno.getUsuario(), alumno.getUniversidadOrigen(), universidadesSeleccionadas, alumno.getTitulacion(), Curso);
                        std::cout << "Solicitud SICUE procesada con éxito. Volviendo al menú de alumno." << std::endl;
                        return;
                    }
                    else
                    {
                        std::cout << "El curso académico del alumno no corresponde con el intercambio solicitado. La solicitud SICUE no ha sido procesada.\n";
                        std::cout << "Volviendo al menú de alumno." << std::endl;
                        return;
                    }
                }
                else
                {
                    std::cout << "La solicitud SICUE no ha sido procesada debido a un error en la fecha del curso.\n";
                    std::cout << "Volviendo al menú de alumno." << std::endl;
                    return;
                }
            }
            else
            {
                std::cout << "No ha seleccionado universidades válidas. La solicitud no ha sido procesada.\n";
                std::cout << "Volviendo al menú de alumno." << std::endl;
                return;
            }
        }
        else
        {
            std::cout << "No ha seleccionado ninguna universidad. La solicitud no ha sido procesada.\n";
            std::cout << "Volviendo al menú de alumno." << std::endl;
            return;
        }
    }
    else
    {
        std::cout << "\nNo hay ningún programa SICUE disponible para usted.\n";
        std::cout << "Volviendo al menú de alumno." << std::endl;
        return;
    }
}

void modificarSolicitudSicueAlumno(Alumno alumno)
{
    System system; // Crear una instancia de la clase System
    int id_alumno_aux = system.obtenerIdUsuarioPorUsuario(alumno.getUsuario());
    std::string Curso = system.obtenerCursoAcademicoPorIdUsuario(id_alumno_aux);
    std::vector<std::string> solicitudes = system.obtenerUniversidadesYaSolicitadasAlumno(alumno.getUsuario());

    // Verificar si el alumno tiene solicitudes SICUE previas
    if (solicitudes.empty())
    {
        std::cout << "No tiene solicitudes SICUE previas.\n";
        return;
    }

    // Mostrar las solicitudes existentes
    std::cout << "Seleccione la solicitud que desea modificar:\n";
    for (size_t i = 0; i < solicitudes.size(); ++i)
    {
        std::cout << i + 1 << ". Universidad de destino: " << solicitudes[i] << ", Curso: " << Curso << std::endl;
    }

    int seleccion;
    std::cout << "Ingrese el número de la solicitud que desea modificar (0 para cancelar): ";
    std::cin >> seleccion;

    // Validación de la selección
    if (seleccion == 0)
    {
        std::cout << "Modificación cancelada.\n";
        return;
    }

    // Verificar si la selección es válida
    if (seleccion < 1 || seleccion > solicitudes.size())
    {
        std::cout << "Selección inválida. Modificación cancelada.\n";
        return;
    }

    // Mostrar los campos que se pueden modificar
    std::string universidadDestinoActualAModificar = solicitudes[seleccion - 1]; // Obtener la universidad de destino seleccionada
    std::string cursoActual = Curso;

    // Modificar el curso (si lo desea modificar)
    std::string nuevoCurso;
    std::cout << "Curso actual: " << cursoActual << "\n";
    std::cout << "Introduzca el nuevo curso (YYYY/YYYY) o presione Enter para mantener el anterior: ";
    std::cin.ignore(); // Limpiar el buffer de entrada
    std::getline(std::cin, nuevoCurso);
    // Si no se introdujo un nuevo curso, mantener el anterior
    if (nuevoCurso.empty())
    {
        nuevoCurso = cursoActual;
    }

    // Filtrar las universidades disponibles
    int alumno_id = system.obtenerIdUsuarioPorUsuario(alumno.getUsuario());
    std::string nombre_carrera = system.obtenerCarreraPorIdUsuario(alumno_id);
    std::string nombre_universidad = system.obtenerNombreUniversidadPorCarrera(nombre_carrera);
    std::string campo = system.obtenerCampoPorCarrera(nombre_carrera);

    // Obtener universidades con plazas disponibles para el campo
    std::vector<std::string> universidadesPosibles = system.obtenerUniversidadesConPlazasDisponiblesParaAlumno(campo, system.obtenerAnyoAcademicoPorIdUsuario(alumno_id));

    // Eliminar universidades ya solicitadas y la universidad actual en la que el alumno está
    universidadesPosibles.erase(std::remove_if(universidadesPosibles.begin(), universidadesPosibles.end(),
                                               [&solicitudes, universidadDestinoActualAModificar](const std::string &universidad)
                                               {
                                                   return std::find(solicitudes.begin(), solicitudes.end(), universidad) != solicitudes.end() || universidad == universidadDestinoActualAModificar;
                                               }),
                                universidadesPosibles.end());

    // Declarar la variable nuevaUniversidadDestino antes de la validación
    std::string nuevaUniversidadDestino = universidadDestinoActualAModificar; // Inicializarla con la universidad actual

    // Verificar si hay universidades disponibles
    if (universidadesPosibles.empty())
    {
        std::cout << "No hay universidades disponibles para su campo que no haya solicitado o en las que ya esté.\n";
        std::cout << "Pasando al siguiente paso...\n";
    }
    else
    {
        // Mostrar las universidades disponibles solo si hay alguna
        std::cout << "Universidades disponibles para su campo (que no ha solicitado y no está en su universidad actual):\n";
        for (size_t i = 0; i < universidadesPosibles.size(); ++i)
        {
            std::cout << i + 1 << ". " << universidadesPosibles[i] << std::endl;
        }

        std::cout << "Universidad de destino actual: " << universidadDestinoActualAModificar << "\n";
        std::cout << "Introduzca la nueva universidad de destino o presione Enter para mantener la anterior: ";
        std::getline(std::cin, nuevaUniversidadDestino);

        // Si no se introdujo una nueva universidad, mantener la anterior
        if (nuevaUniversidadDestino.empty())
        {
            nuevaUniversidadDestino = universidadDestinoActualAModificar;
        }
        // Si no se ha realizado ninguna modificación, no hacemos nada
        if (nuevoCurso == cursoActual && nuevaUniversidadDestino == universidadDestinoActualAModificar)
        {
            std::cout << "No se han realizado cambios en la solicitud.\n";
            return;
        }

        // Realizamos la modificación de la solicitud
        system.modificarSolicitudSicueProfesor(alumno.getUsuario(), nuevoCurso, nuevaUniversidadDestino);
    }
}

void mostrarMenuAdmin() {
        int opcion;
        System system;
        while (true) {
            std::cout << "\n--- Menú Administrador ---" << std::endl;
            std::cout << "1. Consultar plazas" << std::endl;
            std::cout << "2. Salir" << std::endl;
            std::cout << "Seleccione una opción: ";
            std::cin >> opcion;

            switch (opcion) {
                case 1:
                    system.consultarPlazas();  // Llamada a la función que consulta las plazas
                    break;
                case 2:
                    std::cout << "Saliendo del sistema... ¡Hasta luego!" << std::endl;
                    return;  // Sale del bucle y termina la ejecución
                default:
                    std::cout << "Opción no válida. Intente nuevamente." << std::endl;
                    break;
            }
        }
    }


