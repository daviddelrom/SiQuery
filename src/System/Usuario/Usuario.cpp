#include "Usuario.hpp"

// Crear y añadir una solicitud
bool User::crearSolicitud(const std::string &tipoSolicitante,
                        const std::string &universidadOrigen,
                        const std::string &universidadDestino,
                        const std::string &departamento,
                        const std::map<std::pair<std::string, std::string>, int> &plazasPorCarrera,
                        const std::map<std::string, int> &plazasPorDepartamento,
                        const std::string &curso, const std::string &curso_Academico)
{
    if (tipoSolicitante == "Alumno")
    {
        auto it = plazasPorCarrera.find({curso, tipoSolicitante});
        if (it != plazasPorCarrera.end() && it->second > 0)
        {
            Solicitud nuevaSolicitud(usuario, tipoSolicitante, curso, curso_Academico);
            solicitudes.push_back(nuevaSolicitud);
            std::cout << "Solicitud creada para el alumno en el curso \"" << curso << "\".\n";
            return true;
        }
        else
        {
            std::cout << "No hay plazas disponibles para el curso \"" << curso << "\" o la titulación no coincide.\n";
            return false;
        }
    }
    else if (tipoSolicitante == "Profesor")
    {
        auto it = plazasPorDepartamento.find(departamento);
        if (it != plazasPorDepartamento.end() && it->second > 0)
        {
            Solicitud nuevaSolicitud(usuario, tipoSolicitante, departamento, curso_Academico);
            solicitudes.push_back(nuevaSolicitud);
            std::cout << "Solicitud creada para el profesor en el departamento \"" << departamento << "\".\n";
            return true;
        }
        else
        {
            std::cout << "No hay plazas disponibles en el departamento \"" << departamento << "\".\n";
        }
    }
    else
    {
        std::cout << "Tipo de solicitante inválido: \"" << tipoSolicitante << "\".\n";
        return false;
    }
    return true;
}