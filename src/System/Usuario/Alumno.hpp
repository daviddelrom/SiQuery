#ifndef ALUMNO_H
#define ALUMNO_H

#include "Usuario.hpp"
#include <map>
#include <string>
#include <iostream>

class Alumno : public User
{
private:
    std::string universidadOrigen;  // Universidad de origen
    std::string universidadDestino; // Universidad de destino
    std::string cursoAcademico;     // Año o periodo académico
    std::string titulacion;         // Titulación a la que pertenece el alumno

public:
    // Constructor que inicializa tanto los parámetros heredados como los específicos del alumno
     Alumno(const std::string &usuario,
           const std::string &universidadOrigen,
           const std::string &cursoAcademico,
           const std::string &titulacion)
        : User(usuario),  // Llama al constructor de la clase base User
          universidadOrigen(universidadOrigen),
          cursoAcademico(cursoAcademico),
          titulacion(titulacion)
    {
        
    }

    // Métodos getters para los atributos privados
    std::string getUniversidadOrigen() const { return universidadOrigen; }
    std::string getUniversidadDestino() const { return universidadDestino; }
    std::string getCursoAcademico() const { return cursoAcademico; }
    std::string getTitulacion() const { return titulacion; }

    // Sobrescribimos el método tipoSolicitante() para devolver "Alumno"
    std::string tipoSolicitante() const
    {
        return "Alumno";
    }

    // Método para mostrar los detalles del alumno
    void mostrarDetalles() const
    {
        std::cout << "Usuario: " << getUsuario() << "\n";
        std::cout << "Universidad Origen: " << universidadOrigen << "\n";
        std::cout << "Universidad Destino: " << universidadDestino << "\n";
        std::cout << "Curso Académico: " << cursoAcademico << "\n";
        std::cout << "Titulación: " << titulacion << "\n";
    }
};

#endif // ALUMNO_H
