#ifndef FECHA_H
#define FECHA_H
#include <iostream>

class Fecha
{
private:

    unsigned char dia;
    unsigned char mes;
    unsigned short anio;
    unsigned char hora;
    unsigned char minuto;

public:
    //constructores
    Fecha();
    Fecha(int d,int m, int a, int h=0, int min=0);
    Fecha(const Fecha &otra);

    //getters
    int getDia() const;
    int getMes() const;
    int getAnio() const;
    int getHora() const;
    int getMinuto() const;

    //no se ponen setters, pues fecha es una clase inmutable, osea las fechas no se cambian en ningun caso.

    //métodos
    static bool esValida(int d, int m, int a, int h = 0, int min = 0);

    //Declaraciones para la sobrecarga de operadores

    bool operator < (const Fecha &otra)const;
    bool operator==(const Fecha &otra) const;
    bool operator<=(const Fecha &otra) const;
    friend std::ostream& operator<<(std::ostream& os, const Fecha &f);


};

#endif // FECHA_H
