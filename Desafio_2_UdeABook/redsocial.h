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
};

#endif // REDSOCIAL_H