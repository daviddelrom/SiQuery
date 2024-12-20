
#include <cassert>
#include "../src/SolicitudSICUE.h"

void test_seleccion_universidades() {
    SolicitudSICUE solicitud;
    solicitud.mostrar_universidades_disponibles();

    bool seleccion_valida = solicitud.seleccionar_universidad(1);
    assert(seleccion_valida && "La universidad debería ser seleccionada correctamente.");

    solicitud.mostrar_seleccion();
    assert(solicitud.universidades_seleccionadas.size() == 1 && "Debe haberse seleccionado una universidad.");
}

int main() {
    test_seleccion_universidades();
    std::cout << "Test de selección de universidades pasó correctamente." << std::endl;
    return 0;
}
