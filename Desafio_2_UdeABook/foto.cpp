#include "foto.h"
//Constructor en donde se asignan los valores ingresados a los atributos
Foto::Foto(string ruta, Fecha fecha):rutaArchivo(ruta), fechaPublicacion(fecha), likes(0) {
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