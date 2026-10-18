#ifndef ALBUM_H
#define ALBUM_H
#include "foto.h"
#include "lista.h"
#include <string>

class Album
{
private:
    string nombre;
    string rutaAlmacenamiento;
    short privacidad;
    Lista<Foto*> fotos;     // fotos del álbum (el arreglo dinámico lo maneja Lista)

public:
    Album(const string& titulo, const string& ruta, short estadoPrivacidad);    //Constructor

    // Se prohíbe copiar álbumes
    Album(const Album&) = delete;
    Album& operator=(const Album&) = delete;

    Foto* agregarFoto(const Foto& foto);             //Metodo

    //Getters y setters
    string getNombre() const;
    void setNombre(const string& nuevoNombre);

    int getPrivacidad() const;
    void setPrivacidad(short nuevaPrivacidad);

    string getRutaAlmacenamiento() const;
    void setRutaAlmacenamiento(const string& nuevaRuta);

    unsigned short getFotosGuardadas() const;

    const Lista<Foto*>& getFotos() const;

    //Desctructor
    ~Album();
};

#endif // ALBUM_H
