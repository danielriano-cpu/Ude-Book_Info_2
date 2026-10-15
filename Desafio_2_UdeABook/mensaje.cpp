#include "mensaje.h"

//Constructores y destructor

// Constructor principal. El mensaje nace como no leído.
Mensaje::Mensaje(Usuario* emisorMsg, Usuario* receptorMsg, const char* texto, const Fecha& fecha)
    : fechaenvio(fecha)            // la fecha se construye directamente como copia
{
    emisor = emisorMsg;
    receptor = receptorMsg;
    visto = false;
    contenido = copiarTexto(texto);    // copia propia del texto
}

// Constructor de copia: copia profunda del texto
Mensaje::Mensaje(const Mensaje& otro)
    : fechaenvio(otro.fechaenvio)
{
    emisor = otro.emisor;          // los usuarios se comparten (solo punteros)
    receptor = otro.receptor;
    visto = otro.visto;
    contenido = copiarTexto(otro.contenido);   // texto nuevo, no el mismo puntero
}

// Destructor: libera el texto que pidió con new en copiarTexto
Mensaje::~Mensaje() {
    // TODO medidor: restar (longitudTexto(contenido) + 1) bytes
    delete[] contenido;
}

// Getters

Usuario* Mensaje::getEmisor() const { return emisor; }
Usuario* Mensaje::getReceptor() const { return receptor; }
const char* Mensaje::getContenido() const { return contenido; }
bool Mensaje::getVisto() const { return visto; }
const Fecha& Mensaje::getFechaenvio() const { return fechaenvio; }

// Métodos
// Paso de no leído a leído
void Mensaje::marcarComoVisto() {
    visto = true;
}

// El texto es válido si existe, no está vacío y tiene máximo 512 caracteres
bool Mensaje::textoValido(const char* texto) {
    if (texto == nullptr) return false;
    int n = longitudTexto(texto);  // devuelve true solo si el largo está entre 1 y 512
    return n >= 1 && n <= 512;
}

// Operador de asignación
Mensaje& Mensaje::operator=(const Mensaje& otro) {
    if (this == &otro) return *this;   // evita m = m, que borraría su propio texto

    // TODO medidor: restar los bytes del texto viejo
    delete[] contenido;                // libera el texto viejo
    contenido = copiarTexto(otro.contenido);

    emisor = otro.emisor;
    receptor = otro.receptor;
    visto = otro.visto;
    fechaenvio = otro.fechaenvio;
    return *this;                      // permite encadenar: a = b = c
}

// Un mensaje es "menor" si se envió antes. Sirve para ordenar por fecha.
bool Mensaje::operator<(const Mensaje& otro) const {
    return fechaenvio < otro.fechaenvio;
}

// ¿El mensaje se envió entre inicio y fin? (ambas inclusive)
bool Mensaje::estaEnRango(const Fecha& inicio, const Fecha& fin) const {
    return inicio <= fechaenvio && fechaenvio <= fin;
}

// Cuenta los caracteres de un texto hasta encontrar el '\0'
int Mensaje::longitudTexto(const char* texto) {
    int n = 0;
    while (texto[n] != '\0') {
        n++;                       // TODO medidor: contar iteración
    }
    return n;
}

// Crea un arreglo nuevo del tamaño exacto y copia el texto en él
char* Mensaje::copiarTexto(const char* origen) {
    int n = longitudTexto(origen);
    char* destino = new char[n + 1];   // +1 para el '\0'
    // TODO medidor: sumar (n + 1) bytes
    for (int i = 0; i <= n; i++) {
        destino[i] = origen[i];        // TODO medidor: contar iteración
    }
    return destino;
}