#include "medidorrecursos.h"
#include <iostream>

long MedidorRecursos::iteraciones = 0;
long MedidorRecursos::memoriaActual = 0;

//getters

long MedidorRecursos::getIteraciones(){return iteraciones;}
long MedidorRecursos::getMemoria(){return memoriaActual;}

//metodos

void MedidorRecursos::contar(){

    iteraciones++;
}
void MedidorRecursos::reiniciarIteraciones(){

    iteraciones = 0;
}