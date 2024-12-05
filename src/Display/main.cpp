#include <iostream>
#include <string>

int main()
{
    int opcion;
    do
    {
        std::cout << std::endl;
        std::cout << "Introduzca una opcion en el menu de opciones principal..." << std::endl;
        std::cout << std::endl;
        std::cout << "1. Iniciar sesion." << std::endl;
        std::cout << std::endl;
        std::cout << "2. Visualizar." << std::endl;
        std::cout << std::endl;
        std::cout << "3. Salir." << std::endl;
        std::cout << std::endl;
        std::cout << "Ingrese una opcion: ";
        std::cin >> opcion;
        switch (opcion)
        {
        case 1:
        {
        }

        case 2:

            break;

        case 3:

            return 0;
        default:
            std::cout << std::endl;
            std::cout << "Opcion incorrecta, introduce una opcion valida de nuevo..." << std::endl;
            break;
        }
    } while (opcion != 3);
}