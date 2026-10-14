#ifndef FOTO_H
#define FOTO_H
#include <string>
#include "fecha.h"

using namespace std;

class Foto
{
private:
    //Atributos
    string rutaArchivo;
    Fecha fechaPublicacion;
    int likes;
public:
    //Constructor
    Foto(string ruta, Fecha fecha);
    //Metodo
    void sumarLikes();
    //Getters
    int getLikes() const;
    string getRutaArchivo() const;
};

#endif // FOTO_H
