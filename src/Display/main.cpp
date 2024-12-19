#include <iostream>
#include <string>
#include "../System/System.hpp"
#include <regex>

void mostrarMenuProfesor()
{
    std::cout << "\n=====================\n";
    std::cout << "1. Solicitud de SICUE\n";
    std::cout << "2. Desloguearse\n";
    std::cout << "=====================\n";
    std::cout << "Seleccione una opción: ";
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

    // Verificar que el año de inicio no sea menor que el año actual
    if (inicio < year_actual)
    {
        std::cout << "Error: El año de inicio no puede ser anterior al año actual.\n";
        return false;
    }

    // Verificar que el curso no difiera más de 3 años del actual
    if (inicio > (year_actual + 3))
    {
        std::cout << "Error: El curso no puede diferir más de tres años del año actual.\n";
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

    // Verificar que no se esté solicitando el curso actual
    if (inicio == year_actual)
    {
        std::cout << "Error: No se puede solicitar el curso actual. El plazo de solicitud ya ha pasado.\n";
        return false;
    }

    // Si todo es válido
    return true;
}


void solicitudSicueProfesor()
{
    std::cout << "\nFormulario de Solicitud de SICUE\n";
    std::cout << "Introduzca el curso que desea hacer el intercambio con el formato YYYY/YYYY: ";
    std::string Curso;
    std::cin >> Curso;
     
    // Verificar si el curso es válido
    if(esFechaValida(Curso))
    {
        std::cout << "La solicitud SICUE ha sido procesada para el curso: " << Curso << std::endl;
    }
    else
    {
        std::cout << "La solicitud SICUE no ha sido procesada debido a un error en la fecha del curso.\n";
    }
}

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
            // Solicitar las credenciales
            std::cout << "Ingrese su usuario de la UCO: ";
            std::cin >> usuario;
            std::cout << "Ingrese su contraseña: ";
            std::cin >> contrasena;

            // Llamar a la función iniciarSesion para verificar las credenciales
            int resultado = system.iniciarSesion(usuario, contrasena);

            if (resultado == 1)
            {
                estado_usuario = 1;
                int tipo_user = system.obtenerTipoUsuario(usuario);
                if (tipo_user == 0)
                {
                    std::cout << "\n¡Bienvenido Profesor!" << std::endl;
                    mostrarMenuProfesor();
                    solicitudSicueProfesor();
                }
                else if (tipo_user == 1)
                {
                    std::cout << "\n¡Bienvenido Alumno!" << std::endl;
                }
                else
                {
                    std::cout << "\n¡Bienvenido Administrador!" << std::endl;
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
