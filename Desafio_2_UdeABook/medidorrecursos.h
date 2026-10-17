#ifndef MEDIDORRECURSOS_H
#define MEDIDORRECURSOS_H

class MedidorRecursos
{
private:

    static long iteraciones;
    static long memoriaActual;  // bytes reservados con new que aún no se han liberado

public:

    //No tenemos constructores debido a que en medidor nunca se crea ningún objeto.

    //getters

    static long getIteraciones();
    static long getMemoria();

    //metodos

    static void contar();
    static void reiniciarIteraciones();
    static void sumarMemoria(long bytes);
    static void restarMemoria(long bytes);
    static void mostrarReporte(const char* funcionalidad);

};

#endif // MEDIDORRECURSOS_H