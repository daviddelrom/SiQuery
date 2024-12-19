#ifndef USUARIO_H
#define USUARIO_H

#include <string>
#include <vector>
#include "../Solicitud/Solicitud.hpp"
#include <iostream>
#include <map>

class User
{
private:
    std::string usuario;
    std::string password;
    static std::vector<Solicitud> solicitudes;

public:
    // Constructor por defecto
    User() : usuario(""), password("") {}

    // Constructor parametrizado
    User(const std::string &usuario, const std::string &password)
        : usuario(usuario), password(password) {}

    // Getter para usuario
    std::string getUsuario() const
    {
        return usuario;
    }

    // Setter para usuario
    void setUsuario(const std::string &usuario)
    {
        this->usuario = usuario;
    }

    // Getter para password
    std::string getPassword() const
    {
        return password;
    }

    // Setter para password
    void setPassword(const std::string &password)
    {
        this->password = password;
    }

    // Obtener todas las solicitudes
    std::vector<Solicitud> getSolicitudes() const
    {
        return solicitudes;
    }

    static void setSolicitud(Solicitud newSolicitud)
    {
        solicitudes.push_back(newSolicitud);
    }

    // Crear y añadir una solicitud
    bool crearSolicitud(const std::string &tipoSolicitante,
                        const std::string &universidadOrigen,
                        const std::string &universidadDestino,
                        const std::string &departamento,
                        const std::map<std::pair<std::string, std::string>, int> &plazasPorCarrera,
                        const std::map<std::string, int> &plazasPorDepartamento,
                        const std::string &curso, const std::string &curso_Academico);

    void modificarSolicitud();
};

#endif // USUARIO_H
