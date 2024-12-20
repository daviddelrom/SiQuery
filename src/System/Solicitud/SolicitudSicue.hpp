#ifndef SOLICITUD_SICUE_H
#define SOLICITUD_SICUE_H
#include "Solicitud.hpp"
#include <vector>

class SolicitudSICUE : public Solicitud
{
private:
    std::string universidadOrigen_;               // Universidad de origen
    std::vector<std::string> universidadDestino_;  // Universidad(es) de destino

public:
    // Constructor parametrizado con vector de destinos
    SolicitudSICUE(const std::string &solicitante, const std::string &tipoSolicitante,
                   const std::string &universidadOrigen, const std::vector<std::string> &universidadDestino,
                   const std::string &departamento, const std::string &curso)
        : Solicitud(solicitante, tipoSolicitante, departamento, curso), universidadOrigen_(universidadOrigen),
          universidadDestino_(universidadDestino)
    {}

    // Constructor parametrizado con un solo destino
    SolicitudSICUE(const std::string &solicitante, const std::string &tipoSolicitante,
                   const std::string &universidadOrigen, const std::string &universidadDestino,
                   const std::string &departamento, const std::string &curso)
        : Solicitud(solicitante, tipoSolicitante, departamento, curso), universidadOrigen_(universidadOrigen)
    {
        universidadDestino_.push_back(universidadDestino); // Agregar el único destino al vector
    }

    // Getters y Setters para universidad origen y destino
    std::string getUniversidadOrigen() const { return universidadOrigen_; }

    void setUniversidadOrigen(const std::string &universidadOrigen) { universidadOrigen_ = universidadOrigen; }

    std::vector<std::string> getUniversidadDestino() const { return universidadDestino_; }

    void setUniversidadDestino(const std::string &universidadDestino)
    {
        universidadDestino_.push_back(universidadDestino); // Agregar un nuevo destino al vector
    }

    void setUniversidadDestino(const std::vector<std::string> &universidadesDestino)
    {
        universidadDestino_ = universidadesDestino; // Asignar varios destinos
    }
};

#endif
