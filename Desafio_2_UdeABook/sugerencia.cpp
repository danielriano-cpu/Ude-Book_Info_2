#include "sugerencia.h"

//constructores
Sugerencia::Sugerencia() {
    usuario = nullptr;
    comunes = 0;
}

Sugerencia::Sugerencia(Usuario* u, int amigosEnComun) {
    usuario = u;
    comunes = amigosEnComun;
}

//getters
Usuario* Sugerencia::getUsuario() const { return usuario; }
int Sugerencia::getComunes() const { return comunes; }

bool Sugerencia::operator>(const Sugerencia& otra) const {
    return comunes > otra.comunes;
}