#include "redsocial.h"
#include "medidorrecursos.h"

// Las listas arrancan grandes porque se cargan más de 1000 usuarios:
// así no tienen que crecer varias veces durante la carga
RedSocial::RedSocial() : usuarios(1024), mensajes(1024) {
}

// RedSocial es dueña de los usuarios y de los mensajes: los borra uno por uno.
// (Cada usuario borra sus álbumes y cada álbum sus fotos.)
// Los arreglos de las listas los liberan las propias Listas al final.
RedSocial::~RedSocial() {
    for (int i = 0; i < mensajes.getCantidad(); i++) {
        MedidorRecursos::restarMemoria(sizeof(Mensaje));
        delete mensajes[i];
        MedidorRecursos::contar();
    }
    for (int i = 0; i < usuarios.getCantidad(); i++) {
        MedidorRecursos::restarMemoria(sizeof(Usuario));
        delete usuarios[i];
        MedidorRecursos::contar();
    }
}

void RedSocial::agregarUsuario(Usuario* u) {
    usuarios.agregar(u);
}

// Búsqueda lineal por nombre de usuario. Devuelve nullptr si no existe.
Usuario* RedSocial::buscarUsuario(const string& nombre) const {
    for (int i = 0; i < usuarios.getCantidad(); i++) {
        MedidorRecursos::contar();
        if (usuarios[i]->getNombreUsuario() == nombre) {
            return usuarios[i];
        }
    }
    return nullptr;
}

void RedSocial::agregarMensaje(Mensaje* m) {
    mensajes.agregar(m);
}

int RedSocial::getCantidadUsuarios() const {
    return usuarios.getCantidad();
}