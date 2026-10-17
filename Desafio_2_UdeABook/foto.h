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
    unsigned int tamanioBytes;
    int likes;
public:
    //Constructor
    Foto(const string& ruta, const Fecha& fecha, unsigned int tamanio = 0);
    //Metodo
    void sumarLikes();
    //Getters
    int getLikes() const;
    string getRutaArchivo() const;
    Fecha getFechaPublicacion() const;
    unsigned int getTamanioBytes() const;
};

#endif // FOTO_H
