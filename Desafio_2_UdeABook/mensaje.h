#ifndef MENSAJE_H
#define MENSAJE_H

#include "fecha.h"

class Usuario;

class Mensaje
{
private:

    Usuario* emisor;
    Usuario* receptor;
    char* contenido;//para guardar el texto con memoria dinamica
    bool visto;
    Fecha fechaenvio;//usamos la clase fecha pues vive dentro del mensaje

public:
    //constructores y destructor

    Mensaje(Usuario* emisorMsg, Usuario* receptorMsg, const char* texto, const Fecha& fecha);
    Mensaje(const Mensaje& otro);     // copia profunda del texto
    ~Mensaje();

    //getters

    Usuario* getEmisor() const;
    Usuario* getReceptor()const;
    char* getContenido()const;
    bool getVisto()const;
    Fecha geFechaenvio()const;

    /*
    No se ponen setters, pues mensaje es una clase inmutable,
    pues no hay un requisito de editar mensajes. Solo se pide enviarlos,
    leer los nuevos y buscarlos por rango de fechas. no se cambian en ningun caso.
    */

    //metodos
    void marcarComoVisto();
    static bool textoValido(const char* texto);   // validamos que sea no vacío y máximo 512 caracteres
};

#endif // MENSAJE_H
