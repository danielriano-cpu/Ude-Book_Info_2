#ifndef ALBUM_H
#define ALBUM_H
#include "foto.h"
#include <string>

class Album
{
private:
    string nombre;
    short privacidad;
    Foto** almacen;
    unsigned short fotosGuardadas;
    unsigned short capacidad;

public:
    Album(string titulo, short estadoPrivacidad);
    Foto* agregarFoto(Foto foto);
    string getNombre();
    void setNombre(string nuevoNombre);
    int getPrivacidad();
    void setPrivacidad(short nuevaPrivacidad);
    ~Album();
};

#endif // ALBUM_H
