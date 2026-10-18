#ifndef REDSOCIAL_H
#define REDSOCIAL_H
#include <string>
#include "lista.h"
#include "usuario.h"
#include "mensaje.h"
using namespace std;

class RedSocial
{
private:
    Lista<Usuario*> usuarios;   // todos los usuarios de la plataforma (RedSocial es su dueña)
    Lista<Mensaje*> mensajes;   // todos los mensajes enviados (RedSocial es su dueña)

    // Auxiliares para la carga de archivos
    static int partirLinea(const string& linea, string partes[], int maxPartes);
    static int textoAEntero(const string& texto);
    static Fecha textoAFecha(const string& texto);

    // Carga de cada archivo (las llama cargarDatos)
    bool cargarUsuarios(const string& ruta);

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

    // Carga todos los .txt de la carpeta indicada (ej: "datos/")
    bool cargarDatos(const string& carpeta);
};

#endif // REDSOCIAL_H