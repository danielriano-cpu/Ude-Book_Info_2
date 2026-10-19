#ifndef SUGERENCIA_H
#define SUGERENCIA_H
#include "usuario.h"

// Una sugerencia de amistad: el usuario sugerido y cuántos amigos en común tiene
// con quien inició sesión. Se ordenan de mayor a menor cantidad de amigos en común.
class Sugerencia
{
private:
    Usuario* usuario;
    int comunes;

public:
    //constructores
    Sugerencia();                                  // necesario para crear arreglos (new Sugerencia[n])
    Sugerencia(Usuario* u, int amigosEnComun);

    //gettters
    Usuario* getUsuario() const;
    int getComunes() const;

    // Una sugerencia "es mayor" si tiene más amigos en común
    bool operator>(const Sugerencia& otra) const;
};

#endif // SUGERENCIA_H