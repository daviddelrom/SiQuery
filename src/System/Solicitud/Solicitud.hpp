#ifndef SOLICITUD_H
#define SOLICITUD_H

#include <string>

class Solicitud
{
private:
    std::string solicitante_;      // Nombre o ID del solicitante
    std::string tipoSolicitante_;  // Tipo del solicitante: "Estudiante" o "Profesor"
    std::string departamento_;     // Departamento (para profesores) o carrera (para estudiantes)
    std::string curso_;            // Curso académico al que aplica
    std::string estado_solicitud_; // Estado de la solicitud (pendiente, aceptada, rechazada)
    std::string curso_Inscripcion_;

public:
    // Constructor por defecto
    Solicitud() : solicitante_(""), tipoSolicitante_(""), departamento_(""), curso_(""), estado_solicitud_("Pendiente") {}

    // Constructor parametrizado
    Solicitud(const std::string &solicitante, const std::string &tipoSolicitante,
                         const std::string &departamento, const std::string &curso)
        : solicitante_(solicitante), tipoSolicitante_(tipoSolicitante), departamento_(departamento),
          curso_(curso), estado_solicitud_("Pendiente") {}

    // Getters y Setters
    std::string getSolicitante() const;
    void setSolicitante(const std::string &solicitante);

    std::string getTipoSolicitante() const;
    void setTipoSolicitante(const std::string &tipoSolicitante);

    std::string getDepartamento() const;
    void setDepartamento(const std::string &departamento);

    std::string getCurso() const;
    void setCurso(const std::string &curso);

    std::string getEstado() const;
    void setEstado(const std::string &estado);
};

#endif // SOLICITUD_H
