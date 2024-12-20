#ifndef ADMINISTRADOR_HPP
#define ADMINISTRADOR_HPP

#include <string>
#include <vector>

class Administrador {
private:
    int adminId;                     // ID único del administrador
    std::string nombre;              // Nombre del administrador
    std::string correo;              // Correo electrónico del administrador
    std::string contrasena;          // Contraseña para autenticación

public:
    // Constructor
    Administrador(int id, const std::string& nombre, const std::string& correo);

    // Métodos relacionados con usuarios
    void crearUsuario(const std::string& nombre, const std::string& correo, const std::string& tipo);
    void editarUsuario(int userId, const std::string& campo, const std::string& nuevoValor);
    void eliminarUsuario(int userId);
    void resetearContrasena(int userId);

    // Métodos relacionados con universidades
    void registrarUniversidad(const std::string& nombre, int maximoProfesores, int maximoEstudiantes);
    void editarUniversidad(int universidadId, const std::string& campo, const std::string& nuevoValor);
    void eliminarUniversidad(int universidadId);

    // Métodos relacionados con solicitudes
    void revisarSolicitud(int solicitudId, const std::string& nuevoEstado);
    std::vector<std::string> generarReporteSolicitudes();

    // Métodos de supervisión
    void consultarPlazasDisponibles(int universidadId);
    void ajustarPlazas(int universidadId, int nuevasPlazasProfesores, int nuevasPlazasEstudiantes);

    // Métodos de configuración del sistema
    void definirParametrosSistema(const std::string& parametro, const std::string& valor);
    void gestionarRoles(int userId, const std::string& nuevoRol);

    // Métodos de auditoría
    void generarReporteActividad();
};

#endif // ADMINISTRADOR_HPP
