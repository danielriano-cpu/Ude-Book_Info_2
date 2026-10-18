#include <iostream>
#include <string>
#include "redsocial.h"
#include "medidorrecursos.h"
using namespace std;

// ---------- Funciones del menú (una por opción) ----------
// Reciben la red por referencia (sin copias) y el usuario con sesión iniciada

void administrarAmistades(RedSocial& red, Usuario* sesion) {
    cout << "\n[Administrar amistades: pendiente]" << endl;
}

void visitarPerfil(RedSocial& red, Usuario* sesion) {
    cout << "\n[Visitar perfil de un amigo: pendiente]" << endl;
}

void verSugerencias(RedSocial& red, Usuario* sesion) {
    cout << "\n[Ver sugerencias de amistad: pendiente]" << endl;
}

void administrarMensajes(RedSocial& red, Usuario* sesion) {
    cout << "\n[Administrar mensajes: pendiente]" << endl;
}

// Lee una línea y la convierte en número (sin usar métodos de búsqueda de string).
// Si el usuario escribe algo que no es número, devuelve -1.
int leerOpcion() {
    string linea;
    getline(cin, linea);
    if (linea.length() == 0) return -1;
    int numero = 0;
    for (int i = 0; i < (int)linea.length(); i++) {
        if (linea[i] < '0' || linea[i] > '9') return -1;
        numero = numero * 10 + (linea[i] - '0');
    }
    return numero;
}

// ---------- Programa principal ----------

int main() {
    RedSocial red;

    // I. Carga de datos (no aparece en el menú)
    MedidorRecursos::reiniciarIteraciones();
    if (!red.cargarDatos("datos/")) {
        cout << "No se pudieron cargar los datos." << endl;
        return 1;
    }
    cout << "Ude@Book: " << red.getCantidadUsuarios() << " usuarios cargados." << endl;
    MedidorRecursos::mostrarReporte("Carga de datos");

    // II. Inicio de sesión
    Usuario* sesion = nullptr;
    while (sesion == nullptr) {
        string nombre, clave;
        cout << "\n===== INICIAR SESION =====" << endl;
        cout << "Usuario: ";
        getline(cin, nombre);
        cout << "Clave: ";
        getline(cin, clave);
        if (!cin) return 0;                       // se cerró la entrada

        MedidorRecursos::reiniciarIteraciones();
        sesion = red.iniciarSesion(nombre, clave);
        if (sesion == nullptr) {
            cout << "Usuario o clave incorrectos." << endl;
        } else {
            cout << "\nBienvenido, " << sesion->getNombreUsuario() << "!" << endl;
        }
        MedidorRecursos::mostrarReporte("Inicio de sesion");
    }

    // Menú principal
    int opcion = -1;
    while (opcion != 0) {
        cout << "\n========= MENU PRINCIPAL =========" << endl;
        cout << "1. Administrar amistades" << endl;
        cout << "2. Visitar perfil de un amigo" << endl;
        cout << "3. Ver sugerencias de amistad" << endl;
        cout << "4. Administrar mensajes" << endl;
        cout << "0. Salir" << endl;
        cout << "Opcion: ";
        opcion = leerOpcion();
        if (!cin) break;

        MedidorRecursos::reiniciarIteraciones();
        switch (opcion) {
        case 1:
            administrarAmistades(red, sesion);
            MedidorRecursos::mostrarReporte("Administrar amistades");
            break;
        case 2:
            visitarPerfil(red, sesion);
            MedidorRecursos::mostrarReporte("Visitar perfil de un amigo");
            break;
        case 3:
            verSugerencias(red, sesion);
            MedidorRecursos::mostrarReporte("Sugerencias de amistad");
            break;
        case 4:
            administrarMensajes(red, sesion);
            MedidorRecursos::mostrarReporte("Administrar mensajes");
            break;
        case 0:
            cout << "Hasta pronto." << endl;
            break;
        default:
            cout << "Opcion no valida." << endl;
        }
    }
    return 0;
}