#include "foto.h"
//Constructor en donde se asignan los valores ingresados a los atributos
Foto::Foto(const string& ruta, const Fecha& fecha, unsigned int tamanio):rutaArchivo(ruta), fechaPublicacion(fecha), tamanioBytes(tamanio), likes(0) {
}
//Metodo sumar likes
void Foto::sumarLikes(){
    likes++;
}
//Getters de likes y rutas
int Foto::getLikes()const{
    return likes;
}

string Foto::getRutaArchivo()const{
    return rutaArchivo;
}
Fecha Foto::getFechaPublicacion() const{
    return fechaPublicacion;
}

unsigned int Foto::getTamanioBytes() const{
    return tamanioBytes;
}
