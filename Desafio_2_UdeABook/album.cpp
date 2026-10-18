#include "album.h"
#include <string>
#include "medidorrecursos.h"
using namespace std;

Album::Album(const string& titulo, const string& ruta, short estadoPrivacidad) {
    nombre = titulo;
    rutaAlmacenamiento = ruta;
    privacidad = estadoPrivacidad;
}

Foto* Album::agregarFoto(const Foto& foto) {
    Foto* nueva = new Foto(foto);
    MedidorRecursos::sumarMemoria(sizeof(Foto));
    fotos.agregar(nueva);           // si la lista está llena, ella misma crece
    return nueva;
}

Album::~Album() {
    // El álbum es dueño de sus fotos: las borra una por una
    for (int i = 0; i < fotos.getCantidad(); i++) {
        MedidorRecursos::restarMemoria(sizeof(Foto));
        delete fotos[i];
        MedidorRecursos::contar();
    }
    // El arreglo de punteros lo libera el destructor de Lista automáticamente
}

string Album::getNombre() const {
    return nombre;
}

void Album::setNombre(const string& nuevoNombre) {
    nombre = nuevoNombre;
}

string Album::getRutaAlmacenamiento() const {
    return rutaAlmacenamiento;
}

void Album::setRutaAlmacenamiento(const string& nuevaRuta) {
    rutaAlmacenamiento = nuevaRuta;
}

int Album::getPrivacidad() const {
    return privacidad;
}

void Album::setPrivacidad(short nuevaPrivacidad) {
    privacidad = nuevaPrivacidad;
}

unsigned short Album::getFotosGuardadas() const {
    return fotos.getCantidad();
}

const Lista<Foto*>& Album::getFotos() const {
    return fotos;
}