#ifndef EXTERN_FUNCTIONS
#define EXTERN_FUNCTIONS
#include <iostream>
#include <string>
#include "../System/System.hpp"
#include <regex>
#include <limits>
#include "../System/Usuario/Usuario.hpp"
#include "../System/Usuario/Profesor.hpp"
#include "../System/Usuario/Alumno.hpp"

void mostrarMenuProfesor();
bool esFechaValida(const std::string &curso);
void modificarSolicitudSicueProfesor(Profesor profe);
void solicitudSicueProfesor(Profesor profe);
void solicitudSicueAlumno(Alumno alumno);

void mostrarMenuAlumno();
void modificarSolicitudSicueAlumno(Alumno alumno);
void mostrarMenuAdmin();
#endif