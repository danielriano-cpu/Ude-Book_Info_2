#include "usuario.h"
#include "album.h"
#include "mensaje.h"
#include "medidorrecursos.h"
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
    MedidorRecursos::sumarMemoria(capacidadAmigos * sizeof(Usuario*));

    // Inicialización del arreglo de Solicitudes
    cantidadSolicitudes = 0;
    capacidadSolicitudes = 10;
    solicitudesAmistad = new Usuario*[capacidadSolicitudes];
    MedidorRecursos::sumarMemoria(capacidadSolicitudes * sizeof(Usuario*));

    // Inicialización de Mensajes Enviados
    cantidadMensajesEnviados = 0;
    capacidadMensajesEnviados = 10;
    mensajesEnviados = new Mensaje*[capacidadMensajesEnviados];
    MedidorRecursos::sumarMemoria(capacidadMensajesEnviados * sizeof(Mensaje*));

    // Inicialización de Mensajes Recibidos
    cantidadMensajesRecibidos = 0;
    capacidadMensajesRecibidos = 10;
    mensajesRecibidos = new Mensaje*[capacidadMensajesRecibidos];
    MedidorRecursos::sumarMemoria(capacidadMensajesRecibidos * sizeof(Mensaje*));

    // Inicialización de Álbumes
    cantidadAlbumes = 0;
    capacidadAlbumes = 10;
    albumes = new Album*[capacidadAlbumes];
    MedidorRecursos::sumarMemoria(capacidadAlbumes * sizeof(Album*));
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

    void Usuario::eliminarSolicitud(Usuario* solicitante) {
        for (unsigned int i = 0; i < cantidadSolicitudes; i++) {
            MedidorRecursos::contar(); // Reportamos iteración
            if (solicitudesAmistad[i] == solicitante) {
                // Encontramos la solicitud, ahora rodamos los de la derecha hacia la izquierda
                for (unsigned int j = i; j < cantidadSolicitudes - 1; j++) {
                    MedidorRecursos::contar();
                    solicitudesAmistad[j] = solicitudesAmistad[j + 1];
                }
                cantidadSolicitudes--; // Reducimos el contador
                break;
            }
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
        MedidorRecursos::restarMemoria(sizeof(Album));
        delete albumes[i];
        MedidorRecursos::contar();
    }

    // Liberamos los 5 arreglos (con la capacidad actual, por si crecieron)
    MedidorRecursos::restarMemoria(capacidadAmigos * sizeof(Usuario*));
    delete[] amigos;
    MedidorRecursos::restarMemoria(capacidadSolicitudes * sizeof(Usuario*));
    delete[] solicitudesAmistad;
    MedidorRecursos::restarMemoria(capacidadMensajesEnviados * sizeof(Mensaje*));
    delete[] mensajesEnviados;
    MedidorRecursos::restarMemoria(capacidadMensajesRecibidos * sizeof(Mensaje*));
    delete[] mensajesRecibidos;
    MedidorRecursos::restarMemoria(capacidadAlbumes * sizeof(Album*));
    delete[] albumes;
}
