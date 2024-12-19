#ifndef SOLICITUD_SICUE_H
#define SOLICITUD_SICUE_H 
#include "Solicitud.hpp"

class SolicitudSICUE : public Solicitud
{
private:
    std::string universidadOrigen;  // Universidad de origen
    std::string universidadDestino; // Universidad de destino

public:
    // Constructor parametrizado
    SolicitudSICUE(const std::string &solicitante, const std::string &tipoSolicitante,
                   const std::string &universidadOrigen, const std::string &universidadDestino,
                   const std::string &departamento, const std::string &curso);

    // Getters y Setters para universidad origen y destino
    std::string getUniversidadOrigen() const;
    void setUniversidadOrigen(const std::string &universidadOrigen);

    std::string getUniversidadDestino() const;
    void setUniversidadDestino(const std::string &universidadDestino);
};
#endif