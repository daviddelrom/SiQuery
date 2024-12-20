#ifndef PROFESOR_H
#define PROFESOR_H

#include "Usuario.hpp"
#include <map>
#include <string>
#include <iostream>

class Profesor : public User
{
private:
    std::string universidadOrigen;  // Universidad de origen
    std::string universidadDestino; // Universidad de destino
    std::string cursoAcademico;     // Año o periodo académico
    std::string departamento;       // Departamento al que pertenece el profesor

public:
    // Constructor que inicializa los parámetros mínimos del Profesor
Profesor(const std::string &usuario, const std::string &universidadOrigen, const std::string &departamento)
    : User(usuario), universidadOrigen(universidadOrigen), departamento(departamento) {}

    // Métodos getters para los atributos privados
    std::string getUniversidadOrigen() const { return universidadOrigen; }
    std::string getUniversidadDestino() const { return universidadDestino; }
    std::string getCursoAcademico() const { return cursoAcademico; }
    std::string getDepartamento() const { return departamento; }

    
    std::string tipoSolicitante() const 
    {
        return "Profesor";
    }

    // Método para mostrar los detalles del profesor
    void mostrarDetalles() const
    {
        std::cout << "Usuario: " << getUsuario() << "\n";
        std::cout << "Universidad Origen: " << universidadOrigen << "\n";
        std::cout << "Universidad Destino: " << universidadDestino << "\n";
        std::cout << "Curso Académico: " << cursoAcademico << "\n";
        std::cout << "Departamento: " << departamento << "\n";
    }
};

#endif // PROFESOR_H
