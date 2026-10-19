#ifndef REDSOCIAL_H
#define REDSOCIAL_H
#include <string>
#include "lista.h"
#include "usuario.h"
#include "mensaje.h"
#include "album.h"
using namespace std;

class RedSocial
{
private:
    Lista<Usuario*> usuarios;   // todos los usuarios de la plataforma (RedSocial es su dueña)
    Lista<Mensaje*> mensajes;   // todos los mensajes enviados (RedSocial es su dueña)

    // Auxiliares para la carga de archivos
    static int partirLinea(const string& linea, string partes[], int maxPartes);
    static int textoAEntero(const string& texto);

    // Carga de cada archivo (las llama cargarDatos)
    bool cargarUsuarios(const string& ruta);

    bool cargarAmistades(const string& ruta);
    bool cargarSolicitudes(const string& ruta);

    bool cargarAlbumes(const string& ruta);
    bool cargarFotos(const string& ruta);

    bool cargarMensajes(const string& ruta);

    // Busca un álbum por nombre dentro de los álbumes de un usuario
    static Album* buscarAlbum(Usuario* dueno, const string& nombreAlbum);

public:

    //constructor
    RedSocial();
    //destructor
    ~RedSocial();

    // Se prohíbe copiar la red: copiaría punteros y se borrarían dos veces
    RedSocial(const RedSocial&) = delete;
    RedSocial& operator=(const RedSocial&) = delete;

    void agregarUsuario(Usuario* u);
    Usuario* buscarUsuario(const string& nombre) const;
    void agregarMensaje(Mensaje* m);
    int getCantidadUsuarios() const;
    int getCantidadMensajes() const;

    // II. Inicio de sesión: devuelve el usuario si nombre y clave coinciden, o nullptr
    Usuario* iniciarSesion(const string& nombre, const string& clave) const;

    bool enviarMensaje(Usuario* emisor, Usuario* receptor, const string& texto);   // VI

    // Convierte "dd/mm/aaaa" o "dd/mm/aaaa hh:mm" en Fecha (lo usan la carga y el menú)
    static Fecha textoAFecha(const string& texto);

    // Carga todos los .txt de la carpeta indicada (ej: "datos/")
    bool cargarDatos(const string& carpeta);
};

#endif // REDSOCIAL_H