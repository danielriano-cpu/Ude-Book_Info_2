#ifndef LISTA_H
#define LISTA_H

#include "medidorrecursos.h"

template <typename T>
class Lista

{
private:

    T* datos;        // arreglo dinámico de elementos
    int cantidad;    // cuántos elementos hay guardados
    int capacidad;   // cuántos caben antes de tener que crecer
    void crecer();   // auxiliar: duplica la capacidad

public:

    //Constructor
    Lista(int capacidadInicial = 10);
    //Destructor
    ~Lista();

    //Se prohibe copiar listas
    Lista(const Lista&) = delete;
    Lista& operator=(const Lista&) = delete;

    void agregar(const T& elemento);     // mete al final; si está lleno, crece primero
    T& operator[](int pos) const;        // sobrecargamos el operador de acceso por posición: lista[i]
    int getCantidad() const;             // cuántos elementos hay

};

//Como un template solo son los planos que esperan ser usados, declaramos e implementamos en el mismo .h

//Constructor
template <typename T>
Lista<T>::Lista(int capacidadInicial)
{
    if (capacidadInicial < 1) capacidadInicial = 1;   // evita un arreglo de tamaño 0

    cantidad = 0;
    capacidad = capacidadInicial;
    datos = new T[capacidad];
    MedidorRecursos::sumarMemoria(capacidad * sizeof(T)); //tras haber pedido el arreglo con new T, se le avisa al medidor
}                                                         //cuantos bytes pidió

//Destructor
template <typename T>
Lista<T>::~Lista()
{
    MedidorRecursos::restarMemoria(capacidad * sizeof(T));
    delete[] datos;
}

//Crecer: duplica la capacidad cuando el arreglo se llena
template <typename T>
void Lista<T>::crecer()
{
    int nuevaCapacidad = capacidad * 2;
    T* nuevo = new T[nuevaCapacidad];
    MedidorRecursos::sumarMemoria(nuevaCapacidad * sizeof(T));

    for (int i = 0; i < cantidad; i++) {
        nuevo[i] = datos[i];             // copia cada elemento al arreglo nuevo
        MedidorRecursos::contar();
    }

    MedidorRecursos::restarMemoria(capacidad * sizeof(T));
    delete[] datos;                      // libera el arreglo viejo

    datos = nuevo;                       // la lista pasa a usar el arreglo nuevo
    capacidad = nuevaCapacidad;
}

//Mete un elemento al final; si no hay espacio, crece primero
template <typename T>
void Lista<T>::agregar(const T& elemento)
{
    if (cantidad == capacidad) {
        crecer();
    }
    datos[cantidad] = elemento;
    cantidad++;
}

//Acceso por posición: lista[i]
template <typename T>
T& Lista<T>::operator[](int pos) const
{
    return datos[pos];
}

//Cuántos elementos hay
template <typename T>
int Lista<T>::getCantidad() const
{
    return cantidad;
}

#endif // LISTA_H
