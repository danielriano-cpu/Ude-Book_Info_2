#ifndef USUARIO_H
#define USUARIO_H
#include <string>
#include "fecha.h"


using namespace std;
class Mensaje;
class Album;
class Usuario
{
private:
    string nombreUsuario;
    Fecha fechaNacimiento;
    string email;
    string fotoPerfil;
    string contrasenia;
    string ciudad;
    string pais;
    Fecha fechaInscripcion;

    Usuario** Amigos;
    unsigned int cantidadAmigos, capacidadAmigos;

    Usuario** solicitudesAmistad;
    unsigned int cantidadSolicitudes, capacidadSolicitudes;

    Mensaje** mensajesEnviados;
    unsigned int cantidadMensajesEnviados, capacidadMensajesEnviados;

    Mensaje** mensajesRecibidos;
    unsigned int cantidadMensajesRecibidos, capacidadMensajesRecibidos;

    Album** albumes;
    unsigned int cantidadAlbumes, capacidadAlbumes;

public:
    //Constructor
    Usuario(const string& nombre, const Fecha& nacimiento, const string& correo,
            const string& foto, const string& clave, const string& city,
            const string& country, const Fecha& insc);
    ~Usuario(); //Destructor

    //Metodos de interaccion entre objetos
    void agregarAmigo(Usuario* nuevoAmigo);

    void agregarAlbum(Album* nuevoAlbum);

    void recibirMensaje(Mensaje* nuevoMensaje);

    void enviarMensaje(Mensaje* nuevoMensaje);

    void recibirSolicitud(Usuario* solicitante);

    //Getters
    string getNombreUsuario() const;
    string getContrasenia() const;
    string getFotoPerfil() const;
    unsigned int getCantidadAmigos() const;
    Usuario** getAmigos() const;
};

#endif // USUARIO_H
