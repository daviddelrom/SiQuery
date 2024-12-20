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
    std::string curso_Inscripcion_; // (Este atributo parece no estar siendo utilizado)

public:
    // Constructor por defecto
    Solicitud() : solicitante_(""), tipoSolicitante_(""), departamento_(""), curso_(""), estado_solicitud_("Pendiente") {}

    // Constructor parametrizado
    Solicitud(const std::string &solicitante, const std::string &tipoSolicitante,
                         const std::string &departamento, const std::string &curso)
        : solicitante_(solicitante), tipoSolicitante_(tipoSolicitante), departamento_(departamento),
          curso_(curso), estado_solicitud_("Pendiente") {}

    // Getters y Setters
    std::string getSolicitante() const { return solicitante_; }
    void setSolicitante(const std::string &solicitante) { solicitante_ = solicitante; }

    std::string getTipoSolicitante() const { return tipoSolicitante_; }
    void setTipoSolicitante(const std::string &tipoSolicitante) { tipoSolicitante_ = tipoSolicitante; }

    std::string getDepartamento() const { return departamento_; }
    void setDepartamento(const std::string &departamento) { departamento_ = departamento; }

    std::string getCurso() const { return curso_; }
    void setCurso(const std::string &curso) { curso_ = curso; }

    std::string getEstado() const { return estado_solicitud_; }
    void setEstado(const std::string &estado) { estado_solicitud_ = estado; }
};

#endif // SOLICITUD_H
