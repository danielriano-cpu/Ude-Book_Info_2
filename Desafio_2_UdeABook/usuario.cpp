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
    Amigos = new Usuario*[capacidadAmigos];

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

Usuario::~Usuario(){
    // Destruimos los objetos Álbumes porque le pertenecen al Usuario
    for (unsigned int i = 0; i < cantidadAlbumes ; i++) {
        delete albumes[i];
    }
    // Liberamos los 5 arreglos contenedores que pedimos con 'new[]' en el constructor
    delete[] Amigos;
    delete[] solicitudesAmistad;
    delete[] mensajesEnviados;
    delete[] mensajesRecibidos;
    delete[] albumes;
}
