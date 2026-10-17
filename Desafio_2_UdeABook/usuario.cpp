#include "usuario.h"
#include "album.h"
#include "mensaje.h"

Usuario::Usuario(const string& nombre, const Fecha& nacimiento, const string& correo,
                 const string& foto, const string& clave, const string& city,
                 const string& country, const Fecha& inscripcion): fechaNacimiento(nacimiento), fechaInscripcion(inscripcion) //inicializamos directamente las fechas ya que no tenemos sobrecargados los operadores de asignacion
{
    // Asignación de datos personales
    nombreUsuario = nombre;
    email = correo;
    fotoPerfil = foto;
    contrasenia = clave;
    ciudad = city;
    pais = country;

    // Inicialización del arreglo de Amigos
    cantidadAmigos = 0;
    capacidadAmigos = 10;
    amigos = new Usuario*[capacidadAmigos];

    // Inicialización del arreglo de Solicitudes
    cantidadSolicitudes = 0;
    capacidadSolicitudes = 10;
    solicitudesAmistad = new Usuario*[capacidadSolicitudes];

    // Inicialización de Mensajes Enviados
    cantidadMensajesEnviados = 0;
    capacidadMensajesEnviados = 10;
    mensajesEnviados = new Mensaje*[capacidadMensajesEnviados];

    // Inicialización de Mensajes Recibidos
    cantidadMensajesRecibidos = 0;
    capacidadMensajesRecibidos = 10;
    mensajesRecibidos = new Mensaje*[capacidadMensajesRecibidos];

    // Inicialización de Álbumes
    cantidadAlbumes = 0;
    capacidadAlbumes = 10;
    albumes = new Album*[capacidadAlbumes];
}

//Metodos de interaccion entre objetos
    void Usuario::agregarAmigo(Usuario* nuevoAmigo){
            if (cantidadAmigos == capacidadAmigos)
            {
                //Usamos la plantilla para redimensionar en una sola linea
                redimensionarArreglo(amigos, capacidadAmigos, cantidadAmigos);
            }
            amigos[cantidadAmigos] = nuevoAmigo;
            cantidadAmigos++;
    }

    void Usuario::agregarAlbum(Album* nuevoAlbum){
        if (cantidadAlbumes == capacidadAlbumes)
        {
            //Usamos la plantilla para redimensionar en una sola linea
            redimensionarArreglo(albumes, capacidadAlbumes, cantidadAlbumes);
        }
        albumes[cantidadAlbumes] = nuevoAlbum;
        cantidadAlbumes++;
    }

    void Usuario::enviarMensaje(Mensaje* nuevoMensaje){
        if (cantidadMensajesEnviados == capacidadMensajesEnviados) {
            //Usamos la plantilla para redimensionar en una sola linea
            redimensionarArreglo(mensajesEnviados, capacidadMensajesEnviados, cantidadMensajesEnviados);
        }
        mensajesEnviados[cantidadMensajesEnviados] = nuevoMensaje;
        cantidadMensajesEnviados++;
    }

    void Usuario::recibirMensaje(Mensaje* nuevoMensaje){
        if (cantidadMensajesRecibidos == capacidadMensajesRecibidos) {
            //Usamos la plantilla para redimensionar en una sola linea
            redimensionarArreglo(mensajesRecibidos, capacidadMensajesRecibidos, cantidadMensajesRecibidos);
        }
        mensajesRecibidos[cantidadMensajesRecibidos] = nuevoMensaje;
        cantidadMensajesRecibidos++;
    }

    void Usuario::recibirSolicitud(Usuario* solicitante) {
        if (cantidadSolicitudes == capacidadSolicitudes)
        {
            redimensionarArreglo(solicitudesAmistad, capacidadSolicitudes, cantidadSolicitudes);
        }
        solicitudesAmistad[cantidadSolicitudes] = solicitante;
        cantidadSolicitudes++;
    }

    void Usuario::enviarSolicitud(Usuario* destinatario) {
        if (destinatario != nullptr) {
            destinatario->recibirSolicitud(this);
        }
    }

    //Getters

    string Usuario::getNombreUsuario() const {
        return nombreUsuario;
    }

    string Usuario::getContrasenia() const {
        return contrasenia;
    }

    string Usuario::getFotoPerfil() const {
        return fotoPerfil;
    }


    unsigned int Usuario::getCantidadAmigos() const {
        return cantidadAmigos;
    }

    Usuario** Usuario::getAmigos() const {
        return amigos;
    }

    unsigned int Usuario::getCantidadAlbumes() const {
        return cantidadAlbumes;
    }

    Album** Usuario::getAlbumes() const {
        return albumes;
    }

    unsigned int Usuario::getCantidadSolicitudes() const {
        return cantidadSolicitudes;
    }

    Usuario** Usuario::getSolicitudesAmistad() const {
        return solicitudesAmistad;
    }


Usuario::~Usuario(){
    // Destruimos los objetos Álbumes porque le pertenecen al Usuario
    for (unsigned int i = 0; i < cantidadAlbumes ; i++) {
        delete albumes[i];
    }
    // Liberamos los 5 arreglos contenedores que pedimos con 'new[]' en el constructor
    delete[] amigos;
    delete[] solicitudesAmistad;
    delete[] mensajesEnviados;
    delete[] mensajesRecibidos;
    delete[] albumes;
}
