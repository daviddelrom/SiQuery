#include <iostream>
#include <string>
#include "../System/System.hpp"
#include <regex>
#include <limits>
#include "../System/Usuario/Usuario.hpp"
#include "../System/Usuario/Profesor.hpp"
#include "extern-functions.hpp"

int main()
{
    System system; // Crear una instancia de la clase System
    std::string usuario;
    std::string contrasena;
    std::string opcion;
    int estado_usuario;
    while (true)
    {
        std::cout << "\n=====================\n";
        std::cout << "1. Iniciar sesión\n";
        std::cout << "2. Salir\n";
        std::cout << "=====================\n";
        std::cout << "Seleccione una opción: ";
        std::cin >> opcion;
        if (opcion == "1")
        {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            // Solicitar usuario
            std::cout << "Ingrese su usuario: ";
            std::getline(std::cin, usuario); // Lee la línea completa del usuario (incluyendo espacios)
            // Solicitar contraseña
            std::cout << "Ingrese su contraseña: ";
            std::getline(std::cin, contrasena); // Lee la línea completa de la contraseña
            // Llamar a la función iniciarSesion para verificar las credenciales
            int resultado = system.iniciarSesion(usuario, contrasena);

            if (resultado == 1)
            {
                int tipo_user = system.obtenerTipoUsuario(usuario);
                if (tipo_user == 0)
                {
                    std::cout << "\n¡Bienvenido Profesor!" << std::endl;
                    Profesor profe(usuario, system.obtenerNombreUniversidadPorId(system.obtenerUniversidadOrigenProfesor(usuario)), system.obtenerDepartamentoProfesor(usuario));
                    std::string opcion_profesor;
                    do
                    {
                        mostrarMenuProfesor();
                        std::cout << "Opción: ";
                        std::cin >> opcion_profesor;

                        if (opcion_profesor == "1")
                        {
                            solicitudSicueProfesor(profe); // Ejecuta la funcionalidad de la opción 1
                            return 0;
                        }
                        else if (opcion_profesor == "2")
                        {
                            modificarSolicitudSicueProfesor(profe);
                            return 0;
                        }
                        else if (opcion_profesor == "3")
                        {
                            std::cout << "Deslogueandose...\n";
                            return 0; // Sale del bucle
                        }
                        else
                        {
                            std::cout << "Opción incorrecta. Por favor, seleccione una opción válida.\n";
                        }
                    } while (true);
                }
                else if (tipo_user == 1)
                {
                    std::cout << "\n¡Bienvenido Alumno!" << std::endl;
                    // Llamada al constructor de la clase Alumno
                    int idUsuario = system.obtenerIdUsuarioPorUsuario(usuario); // Obtener ID del usuario solo una vez

                    Alumno alumno(usuario,
                                  system.obtenerUniversidadOrigenPorIdUsuario(idUsuario),
                                  system.obtenerCursoAcademicoPorIdUsuario(idUsuario),
                                  system.obtenerCarreraPorIdUsuario(idUsuario));
                    std::string opcion_alumno;
                    do
                    {
                        mostrarMenuAlumno();
                        std::cout << "Opción: ";
                        std::cin >> opcion_alumno;

                        if (opcion_alumno == "1")
                        {
                            solicitudSicueAlumno(alumno);
                            return 0;
                        }
                        else if (opcion_alumno == "2")
                        {
                            modificarSolicitudSicueAlumno(alumno);
                            return 0;
                        }
                        else if (opcion_alumno == "3")
                        {
                            std::cout << "Deslogueandose...\n";
                            return 0; // Sale del bucle
                        }
                        else
                        {
                            std::cout << "Opción incorrecta. Por favor, seleccione una opción válida.\n";
                        }
                    } while (true);
                }
                else
                {
                    std::cout << "\n¡Bienvenido Administrador!" << std::endl;
                    mostrarMenuAdmin();
                }
            }
            else if (resultado == -1)
            {
                // Si el usuario no introdujo la contraseña correcta
                std::cout << "Contraseña Incorrecta. Por favor, intente nuevamente." << std::endl;
            }
            else if (resultado == -2)
            {
                // Si el usuario no existe
                std::cout << "El usuario no existe. Por favor, verifique su identificador de la UCO e intente nuevamente." << std::endl;
            }
        }

        else if (opcion == "2")
        {
            std::cout << "Saliendo del sistema. Hasta luego.\n";
            return 0; // Terminar el programa
        }
        else
        {
            std::cout << "Opción no válida. Intente nuevamente.\n";
        }
    }

    return 0;
}