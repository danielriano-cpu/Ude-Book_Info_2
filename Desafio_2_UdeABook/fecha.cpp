#include "fecha.h"
#include <chrono>

//Constructor por defecto
Fecha::Fecha(){

    dia = 1;
    mes = 1;
    anio = 2000;
    hora = 0;
    minuto = 0;
}

// Constructor con parámetros: asigna lo que llega
Fecha::Fecha(int d, int m, int a, int h, int min) {

    dia = d;
    mes = m;
    anio = a;
    hora = h;
    minuto = min;
}

//constructor de copia
Fecha::Fecha(const Fecha &otra){

    dia = otra.dia;
    mes = otra.mes;
    anio = otra.anio;
    hora = otra.hora;
    minuto = otra.minuto;
}

//getters
int Fecha::getDia() const { return dia; }
int Fecha::getMes() const { return mes; }
int Fecha::getAnio() const { return anio; }
int Fecha::getHora() const { return hora; }
int Fecha::getMinuto() const { return minuto; }

//Unico metodo que tenemos para la clase Fecha
bool Fecha::esValida(int d, int m, int a, int h, int min){

    // Año dentro de un rango razonable
    if (a < 1940 || a > 2080) return false;

    // Mes entre 1 y 12
    if (m < 1 || m > 12) return false;

    // Cuántos días tiene ese mes
    int diasDelMes;
    if (m == 2) {
        bool bisiesto = (a % 4 == 0 && a % 100 != 0) || (a % 400 == 0);
        diasDelMes = bisiesto ? 29 : 28;//se usa el operador ternario "?" un if/else en una sola linea "si es bisiesto 29, si no 28"
    } else if (m == 4 || m == 6 || m == 9 || m == 11) {
        diasDelMes = 30;
    } else {
        diasDelMes = 31;
    }

    // Día entre 1 y los días de ese mes
    if (d < 1 || d > diasDelMes) return false;

    // Hora entre 0 y 23, minuto entre 0 y 59
    if (h < 0 || h > 23) return false;
    if (min < 0 || min > 59) return false;

    return true;   // pasó todas las validaciones
}

//Sobrecarga del operador "<" el cual compara dos fechas desde lo más general a lo más especifico, y lo usamos para pordenar los mensajes por fecha de envío
bool Fecha::operator<(const Fecha &otra) const {
    if (anio != otra.anio) return anio < otra.anio;
    if (mes  != otra.mes)  return mes  < otra.mes;
    if (dia  != otra.dia)  return dia  < otra.dia;
    if (hora  != otra.hora)  return hora  < otra.hora;
    if (minuto  != otra.minuto)  return minuto  < otra.minuto;

    return false;

}

//Sobrecarga del operador "==" el cual mira si dos fechas coinciden en todos sus campos y entonces son iguales
bool Fecha::operator==(const Fecha &otra) const {
    return dia == otra.dia && mes == otra.mes && anio == otra.anio
           && hora == otra.hora && minuto == otra.minuto;
}

//sobrecarga del operador "<=" el cual reutiliza < y == para no repetir código y se usa en la búsqueda de mensajes por rango de fechas (ambas inclusive)
bool Fecha::operator<=(const Fecha &otra) const{
    return (*this < otra) || (*this == otra);
}

//funcion amiga que sobrecarga el operador "<<" el cual permite imprimir una fecha con cout << fecha, sin tener que implementar un metodo imprimeFecha()
std::ostream& operator<<(std::ostream& os, const Fecha &f){
    // Se rellena con un 0 a la izquierda cuando el número tiene un solo dígito: 09/03/2026 07:05
    if (f.dia < 10) os << "0";
    os << (int)f.dia << "/";
    if (f.mes < 10) os << "0";
    os << (int)f.mes << "/" << f.anio << " ";
    if (f.hora < 10) os << "0";
    os << (int)f.hora << ":";
    if (f.minuto < 10) os << "0";
    os << (int)f.minuto;
    return os;
}

// Días que tiene el mes m del año a (tiene en cuenta los años bisiestos)
int Fecha::diasDelMes(int m, int a) {
    if (m == 2) {
        bool bisiesto = (a % 4 == 0 && a % 100 != 0) || (a % 400 == 0);
        return bisiesto ? 29 : 28;
    }
    if (m == 4 || m == 6 || m == 9 || m == 11) return 30;
    return 31;
}

// Fecha y hora actuales. <chrono> (librería estándar de C++) da los segundos
// transcurridos desde el 1/1/1970; a partir de ahí se cuentan años y meses completos.
Fecha Fecha::ahora() {
    long long segundos = std::chrono::duration_cast<std::chrono::seconds>(
                             std::chrono::system_clock::now().time_since_epoch()).count();
    segundos -= 5 * 3600;                          // Colombia está en UTC-5

    long long dias = segundos / 86400;             // 86400 segundos tiene un día
    int segundosDelDia = segundos % 86400;

    int anio = 1970;
    while (dias >= (diasDelMes(2, anio) == 29 ? 366 : 365)) {   // se descuentan años completos
        dias -= (diasDelMes(2, anio) == 29 ? 366 : 365);
        anio++;
    }
    int mes = 1;
    while (dias >= diasDelMes(mes, anio)) {        // se descuentan meses completos
        dias -= diasDelMes(mes, anio);
        mes++;
    }
    int dia = dias + 1;                            // lo que sobra son los días del mes actual
    return Fecha(dia, mes, anio, segundosDelDia / 3600, (segundosDelDia % 3600) / 60);
}