#include "../src/SolicitudSICUE.h"
#include <cassert>

void test_flujo_completo_solicitud() {
  SolicitudSICUE solicitud;
  solicitud.mostrar_universidades_disponibles();
  solicitud.seleccionar_universidad(1);
  solicitud.mostrar_seleccion();

  std::string curso = "2025/2026";
  bool solicitud_exitosa = solicitud.procesar_solicitud(curso);
  assert(solicitud_exitosa &&
         "La solicitud SICUE debería haberse procesado correctamente.");
  assert(solicitud.universidades_seleccionadas.size() == 1 &&
         "Debe haberse seleccionado una universidad.");
}

int main() {
  test_flujo_completo_solicitud();
  std::cout << "Test de flujo completo de solicitud pasó correctamente."
            << std::endl;
  return 0;
}
