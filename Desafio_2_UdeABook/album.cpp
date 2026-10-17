#include "album.h"
#include <string>
using namespace std;

Album::Album(const string& titulo, const string& ruta, short estadoPrivacidad) {
    nombre = titulo;
    rutaAlmacenamiento = ruta;
    privacidad = estadoPrivacidad;
    capacidad = 10;
    fotosGuardadas = 0;
    almacen = new Foto*[capacidad];
}

Foto* Album::agregarFoto(const Foto& foto) {
    if (fotosGuardadas == capacidad) {
        capacidad = capacidad * 2;
        Foto** almacenTemp = new Foto*[capacidad];
        for (unsigned short i = 0; i < fotosGuardadas; i++) {
            almacenTemp[i] = almacen[i];
        }
        delete[] almacen;
        almacen = almacenTemp;
    }


    almacen[fotosGuardadas] = new Foto(foto);
    fotosGuardadas++;
    return almacen[fotosGuardadas - 1];
}

Album::~Album() {
    for (unsigned int i = 0; i < fotosGuardadas; i++) {
        delete almacen[i];
    }
    delete[] almacen;
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
    return fotosGuardadas;
}

Foto** Album::getAlmacen() const {
    return almacen;
}