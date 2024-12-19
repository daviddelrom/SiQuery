#ifndef SOLICITUD_BECA_H
#define SOLICITUD_BECA_H

#include "Solicitud.hpp"

class SolicitudBeca : public Solicitud
{
public:
    // Constructor parametrizado para solicitud de beca (sin universidad de destino ni origen)
    SolicitudBeca(const std::string &solicitante, const std::string &tipoSolicitante,
                  const std::string &departamento, const std::string &curso);
};

#endif // SOLICITUD_BECA_H