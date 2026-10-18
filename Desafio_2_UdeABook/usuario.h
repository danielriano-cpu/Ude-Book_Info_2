#ifndef USUARIO_H
#define USUARIO_H
#include <string>
#include "fecha.h"
#include "medidorrecursos.h"

using namespace std;
class Mensaje;
class Album;

// Plantilla para redimensionar cualquier arreglo de punteros
template <typename T>
void redimensionarArreglo(T**& arreglo, unsigned int& capacidad, unsigned int cantidad) {
    unsigned int nuevaCapacidad = capacidad * 2;
    T** temp = new T*[nuevaCapacidad];
    MedidorRecursos::sumarMemoria(nuevaCapacidad * sizeof(T*));
    for (unsigned int i = 0; i < cantidad; i++) {
        temp[i] = arreglo[i];
        MedidorRecursos::contar();
    }
    MedidorRecursos::restarMemoria(capacidad * sizeof(T*));
    delete[] arreglo;
    arreglo = temp;
    capacidad = nuevaCapacidad;
}
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

    Usuario** amigos;
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

    // Inhabilitamos la copia y asignación para evitar punteros duplicados en memoria
    Usuario(const Usuario& otro) = delete;
    Usuario& operator=(const Usuario& otro) = delete;


    ~Usuario(); //Destructor

    //Metodos de interaccion entre objetos
    void agregarAmigo(Usuario* nuevoAmigo);

    void agregarAlbum(Album* nuevoAlbum);

    void recibirMensaje(Mensaje* nuevoMensaje);

    void enviarMensaje(Mensaje* nuevoMensaje);

    void recibirSolicitud(Usuario* solicitante);

    void enviarSolicitud(Usuario* destinatario);

    //Getters
    string getNombreUsuario() const;
    string getContrasenia() const;
    string getFotoPerfil() const;
    unsigned int getCantidadAmigos() const;
    Usuario** getAmigos() const;
    unsigned int getCantidadAlbumes() const;
    Album** getAlbumes() const;
    unsigned int getCantidadSolicitudes() const;
    Usuario** getSolicitudesAmistad() const;
};

#endif // USUARIO_H
