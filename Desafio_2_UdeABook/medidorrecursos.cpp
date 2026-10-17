#include "medidorrecursos.h"
#include <iostream>

using namespace std;

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

void MedidorRecursos::sumarMemoria(long bytes){

    memoriaActual = memoriaActual + bytes;
}

void MedidorRecursos::restarMemoria(long bytes){

    memoriaActual = memoriaActual - bytes;
}

void MedidorRecursos::mostrarReporte(const char* funcionalidad){
    cout << endl;
    cout << "+------------------------------------------+" << endl;
    cout << "  RECURSOS: " << funcionalidad << endl;
    cout << "+------------------------------------------+" << endl;
    cout << "  Iteraciones:     " << iteraciones << endl;
    cout << "  Memoria en uso:  " << memoriaActual << " bytes" << endl;
    cout << "+------------------------------------------+" << endl;
}