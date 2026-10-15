#include "album.h"
#include <string>
using namespace std;
//Constructor

Album::Album (string titulo, short estadoPrivacidad){
    nombre = titulo;
    privacidad = estadoPrivacidad;
    capacidad = 10;
    fotosGuardadas = 0;
    almacen = new Foto*[capacidad];

}
//Metodo
Foto* Album::agregarFoto(Foto foto){
    if(fotosGuardadas == capacidad){
        capacidad = capacidad*2;
        Foto** almacenTemp = new Foto*[capacidad];
        for(unsigned short i = 0; i<fotosGuardadas; i++){
            almacenTemp[i] = almacen[i];
        }
        delete[] almacen;
        almacen = new Foto*[capacidad];
        almacen = almacenTemp;
    }
    almacen[fotosGuardadas] = new Foto(foto);
    fotosGuardadas++;
    return almacen[fotosGuardadas - 1];
}
//Destructor
Album::~Album() {
    //Destruimos cada foto individualmente
    for (unsigned int i = 0; i < fotosGuardadas; i++) {
        delete almacen[i];
    }

    //Destruimos el almacen principal que ya quedó vacía
    delete[] almacen;
}
//getters y setters
string Album::getNombre() {
    return nombre;
}

void Album::setNombre(string nuevoNombre) {
    nombre = nuevoNombre;
}

int Album::getPrivacidad() {
    return privacidad;
}

void Album::setPrivacidad(short nuevaPrivacidad) {
    privacidad = nuevaPrivacidad;
}