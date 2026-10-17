#ifndef ALBUM_H
#define ALBUM_H
#include "foto.h"
#include <string>

class Album
{
private:
    string nombre;
    string rutaAlmacenamiento;
    short privacidad;
    Foto** almacen;
    unsigned short fotosGuardadas;
    unsigned short capacidad;

public:
    Album(const string& titulo, const string& ruta, short estadoPrivacidad);    //Constructor

    Foto* agregarFoto(const Foto& foto);             //Metodo

    //Getters y setters
    string getNombre() const;
    void setNombre(const string& nuevoNombre);

    int getPrivacidad() const;
    void setPrivacidad(short nuevaPrivacidad);

    string getRutaAlmacenamiento() const;
    void setRutaAlmacenamiento(const string& nuevaRuta);

    unsigned short getFotosGuardadas() const;

    Foto** getAlmacen() const;

    //Desctructor
    ~Album();
};

#endif // ALBUM_H
