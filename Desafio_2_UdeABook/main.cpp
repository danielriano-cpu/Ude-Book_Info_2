#include <iostream>
#include <string>
#include "redsocial.h"
#include "medidorrecursos.h"
#include <random>
using namespace std;

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

// ---------- Ordenamiento: merge sort genérico (lo usan V y VI) ----------
// Ordena el arreglo de menor a mayor según esMenor(a, b), que devuelve true si 'a' va antes que 'b'.
// Es estable (los "iguales" conservan su orden). Usa un arreglo auxiliar.

// Mezcla dos mitades ya ordenadas
template <typename T>
void mezclar(T* arreglo, T* auxiliar, int inicio, int medio, int fin, bool (*esMenor)(T, T)) {
    int i = inicio;        // recorre la mitad izquierda
    int j = medio + 1;     // recorre la mitad derecha
    int k = inicio;        // posición donde se escribe en el auxiliar
    while (i <= medio && j <= fin) {
        MedidorRecursos::contar();
        if (esMenor(arreglo[j], arreglo[i])) {   // el de la derecha es estrictamente menor
            auxiliar[k] = arreglo[j];
            j++;
        } else {                                 // si son iguales, gana el de la izquierda (estable)
            auxiliar[k] = arreglo[i];
            i++;
        }
        k++;
    }
    // Lo que sobre de cualquiera de las dos mitades ya está ordenado
    while (i <= medio) { MedidorRecursos::contar(); auxiliar[k] = arreglo[i]; i++; k++; }
    while (j <= fin)   { MedidorRecursos::contar(); auxiliar[k] = arreglo[j]; j++; k++; }
    // Se copia el tramo mezclado de vuelta al arreglo original
    for (int p = inicio; p <= fin; p++) {
        MedidorRecursos::contar();
        arreglo[p] = auxiliar[p];
    }
}

// Parte el tramo en dos mitades, ordena cada una (recursión) y las mezcla
template <typename T>
void mergeSortRecursivo(T* arreglo, T* auxiliar, int inicio, int fin, bool (*esMenor)(T, T)) {
    if (inicio >= fin) return;               // un solo elemento ya está ordenado
    int medio = (inicio + fin) / 2;
    mergeSortRecursivo(arreglo, auxiliar, inicio, medio, esMenor);
    mergeSortRecursivo(arreglo, auxiliar, medio + 1, fin, esMenor);
    mezclar(arreglo, auxiliar, inicio, medio, fin, esMenor);
}

// La que se llama desde afuera: pide el auxiliar UNA sola vez y lo reutiliza en toda la recursión
template <typename T>
void mergeSort(T* arreglo, int cantidad, bool (*esMenor)(T, T)) {
    if (cantidad < 2) return;
    T* auxiliar = new T[cantidad];
    MedidorRecursos::sumarMemoria(cantidad * sizeof(T));
    mergeSortRecursivo(arreglo, auxiliar, 0, cantidad - 1, esMenor);
    MedidorRecursos::restarMemoria(cantidad * sizeof(T));
    delete[] auxiliar;
}

// Muestra las fotos de un álbum como un carrusel circular:
// después de la última sigue la primera, y antes de la primera va la última.
// Dar/quitar like: si el usuario no le ha dado like a la foto en esta visita, lo da;
// si ya lo dio, lo quita. Se recuerda en un arreglo dinámico de bool (uno por foto).
void mostrarCarrusel(Album* album) {
    int cantidad = album->getFotosGuardadas();
    if (cantidad == 0) {
        cout << "El album esta vacio." << endl;
        return;
    }
    bool* yaDioLike = new bool[cantidad];
    MedidorRecursos::sumarMemoria(cantidad * sizeof(bool));
    for (int i = 0; i < cantidad; i++) {
        yaDioLike[i] = false;
        MedidorRecursos::contar();
    }

    int actual = 0;
    int opcion = -1;
    while (opcion != 0) {
        MedidorRecursos::contar();
        Foto* foto = album->getFotos()[actual];
        Fecha fecha = foto->getFechaPublicacion();
        cout << "\n+------------------------------------------+" << endl;
        cout << "  ALBUM: " << album->getNombre()
             << "   (foto " << (actual + 1) << " de " << cantidad << ")" << endl;
        cout << "+------------------------------------------+" << endl;
        cout << "  Archivo:  " << foto->getRutaArchivo() << endl;
        cout << "  Fecha:    " << (fecha.getDia() < 10 ? "0" : "") << fecha.getDia() << "/"
             << (fecha.getMes() < 10 ? "0" : "") << fecha.getMes() << "/" << fecha.getAnio() << endl;
        cout << "  Likes:    " << foto->getLikes()
             << (yaDioLike[actual] ? "   <3 te gusta" : "") << endl;
        cout << "+------------------------------------------+" << endl;
        cout << "  [1] Siguiente  [2] Anterior  [3] "
             << (yaDioLike[actual] ? "Quitar like" : "Dar like") << "  [0] Salir" << endl;
        cout << "Opcion: ";
        opcion = leerOpcion();
        if (!cin) break;

        if (opcion == 1) {
            actual = (actual + 1) % cantidad;                 // circular hacia adelante
        } else if (opcion == 2) {
            actual = (actual - 1 + cantidad) % cantidad;      // circular hacia atrás
        } else if (opcion == 3) {
            if (yaDioLike[actual]) {
                foto->quitarLike();
                yaDioLike[actual] = false;
            } else {
                foto->sumarLikes();
                yaDioLike[actual] = true;
            }
        } else if (opcion != 0) {
            cout << "Opcion no valida." << endl;
        }
    }

    MedidorRecursos::restarMemoria(cantidad * sizeof(bool));
    delete[] yaDioLike;
}

// II. Visualización de mejores amigos (fotos de perfil)
// Prioridad según amigos en común: <= 10 -> 1 ; 11 a 20 -> 2.5 ; > 20 -> 3.5.
// Para no usar decimales se multiplican por 2: pesos 2, 5 y 7 (la proporción es la misma).
int pesoPorComunes(int comunes) {
    if (comunes > 20) return 7;
    if (comunes >= 11) return 5;
    return 2;
}

// Elige hasta 3 amigos al azar, con probabilidad proporcional a su peso, sin repetir:
// es como una rifa donde cada amigo tiene tantas boletas como su peso. Los ya usados
// (usado[i] == true) no participan, así el segundo grupo es distinto del primero.
int elegirTres(int* pesos, bool* usado, int cantidad, int* elegidos, mt19937& generador) {
    int elegidosCount = 0;
    while (elegidosCount < 3) {
        int total = 0;                                   // boletas que quedan en la rifa
        for (int i = 0; i < cantidad; i++) {
            MedidorRecursos::contar();
            if (!usado[i]) total += pesos[i];
        }
        if (total == 0) break;                           // no quedan amigos disponibles

        uniform_int_distribution<int> rifa(0, total - 1);
        int boleta = rifa(generador);                    // boleta ganadora
        for (int i = 0; i < cantidad; i++) {             // se busca de quién es esa boleta
            MedidorRecursos::contar();
            if (usado[i]) continue;
            if (boleta < pesos[i]) {
                usado[i] = true;
                elegidos[elegidosCount] = i;
                elegidosCount++;
                break;
            }
            boleta -= pesos[i];
        }
    }
    return elegidosCount;
}

void mostrarMejoresAmigos(Usuario* sesion) {
    int cantidad = sesion->getCantidadAmigos();
    if (cantidad == 0) {
        cout << "Aun no tienes amigos para mostrar." << endl;
        return;
    }
    int* pesos = new int[cantidad];
    bool* usado = new bool[cantidad];
    MedidorRecursos::sumarMemoria(cantidad * (sizeof(int) + sizeof(bool)));
    for (int i = 0; i < cantidad; i++) {
        MedidorRecursos::contar();
        pesos[i] = pesoPorComunes(sesion->amigosEnComun(sesion->getAmigos()[i]));
        usado[i] = false;
    }

    random_device semilla;               // semilla distinta en cada ejecución: sin patrón fijo
    mt19937 generador(semilla());
    int elegidos[3];

    for (int ronda = 1; ronda <= 2; ronda++) {
        int n = elegirTres(pesos, usado, cantidad, elegidos, generador);
        cout << "\n+------------------------------------------+" << endl;
        cout << "  TUS MEJORES AMIGOS" << (ronda == 2 ? " (actualizado)" : "") << endl;
        cout << "+------------------------------------------+" << endl;
        for (int k = 0; k < n; k++) {
            Usuario* amigo = sesion->getAmigos()[elegidos[k]];
            cout << "  " << amigo->getNombreUsuario() << "  ->  " << amigo->getFotoPerfil() << endl;
        }
        cout << "+------------------------------------------+" << endl;
        if (ronda == 2) break;
        cout << "1. Ver otras 3 fotos (solo una vez)   0. Continuar" << endl;
        cout << "Opcion: ";
        if (leerOpcion() != 1) break;
    }

    MedidorRecursos::restarMemoria(cantidad * (sizeof(int) + sizeof(bool)));
    delete[] pesos;
    delete[] usado;
}

// ---------- Funciones del menú (una por opción) ----------
// Reciben la red por referencia (sin copias) y el usuario con sesión iniciada

// III. Administrar amistades: enviar y aceptar solicitudes
void administrarAmistades(RedSocial& red, Usuario* sesion) {
    cout << "\n--- Administrar amistades ---" << endl;
    cout << "1. Enviar solicitud de amistad" << endl;
    cout << "2. Ver y aceptar solicitudes recibidas" << endl;
    cout << "0. Volver" << endl;
    cout << "Opcion: ";
    int opcion = leerOpcion();

    if (opcion == 1) {
        string nombre;
        cout << "Nombre de usuario: ";
        getline(cin, nombre);
        Usuario* otro = red.buscarUsuario(nombre);

        // Restricciones implícitas del enunciado
        if (otro == nullptr) {
            cout << "Ese usuario no existe." << endl;
        } else if (otro == sesion) {
            cout << "No puedes enviarte una solicitud a ti mismo." << endl;
        } else if (sesion->esAmigo(otro)) {
            cout << "Ya eres amigo de " << nombre << "." << endl;
        } else if (sesion->tieneSolicitudDe(otro)) {
            cout << nombre << " ya te envio una solicitud: aceptala en la opcion 2." << endl;
        } else if (otro->tieneSolicitudDe(sesion)) {
            cout << "Ya le enviaste una solicitud a " << nombre << "." << endl;
        } else {
            sesion->enviarSolicitud(otro);
            cout << "Solicitud enviada a " << nombre << "." << endl;
        }

    } else if (opcion == 2) {
        int cantidad = sesion->getCantidadSolicitudes();
        if (cantidad == 0) {
            cout << "No tienes solicitudes pendientes." << endl;
            return;
        }
        cout << "\nSolicitudes recibidas:" << endl;
        for (int i = 0; i < cantidad; i++) {
            MedidorRecursos::contar();
            cout << "  " << (i + 1) << ". " << sesion->getSolicitudesAmistad()[i]->getNombreUsuario() << endl;
        }
        cout << "Numero de la solicitud a aceptar (0 para volver): ";
        int elegida = leerOpcion();
        if (elegida >= 1 && elegida <= cantidad) {
            Usuario* solicitante = sesion->getSolicitudesAmistad()[elegida - 1];
            if (sesion->aceptarSolicitud(solicitante)) {
                cout << "Ahora tu y " << solicitante->getNombreUsuario() << " son amigos." << endl;
            }
        }
    }
}

// IV. Visitar perfil de un amigo: lista sus álbumes visibles (amigos o público)
// y muestra las fotos del álbum elegido en modo carrusel circular.
void visitarPerfil(RedSocial& red, Usuario* sesion) {
    string nombre;
    cout << "\nNombre de usuario del amigo: ";
    getline(cin, nombre);

    Usuario* amigo = red.buscarUsuario(nombre);
    if (amigo == nullptr) {
        cout << "Ese usuario no existe." << endl;
        return;
    }
    if (!sesion->esAmigo(amigo)) {
        cout << nombre << " no es tu amigo." << endl;
        return;
    }

    // Se listan solo los álbumes de amigos (1) o públicos (2); los privados (0) se saltan.
    // visibles[] guarda la posición real de cada álbum mostrado, para poder elegirlo por número.
    int cantidadAlbumes = amigo->getCantidadAlbumes();
    int* visibles = new int[cantidadAlbumes + 1];
    MedidorRecursos::sumarMemoria((cantidadAlbumes + 1) * sizeof(int));
    int cantidadVisibles = 0;

    cout << "\nAlbumes de " << nombre << ":" << endl;
    for (int i = 0; i < cantidadAlbumes; i++) {
        MedidorRecursos::contar();
        Album* a = amigo->getAlbumes()[i];
        if (a->getPrivacidad() == 1 || a->getPrivacidad() == 2) {
            visibles[cantidadVisibles] = i;
            cantidadVisibles++;
            cout << "  " << cantidadVisibles << ". " << a->getNombre()
                 << (a->getPrivacidad() == 2 ? " (publico, " : " (amigos, ")
                 << a->getFotosGuardadas() << " fotos)" << endl;
        }
    }

    if (cantidadVisibles == 0) {
        cout << "  No tiene albumes visibles." << endl;
    } else {
        cout << "Elige un album (0 para volver): ";
        int elegido = leerOpcion();
        if (elegido >= 1 && elegido <= cantidadVisibles) {
            Album* album = amigo->getAlbumes()[visibles[elegido - 1]];
            mostrarCarrusel(album);
        }
    }

    MedidorRecursos::restarMemoria((cantidadAlbumes + 1) * sizeof(int));
    delete[] visibles;
}

// V. Ver sugerencias de amistad: K amigos de amigos ordenados por amigos en común
// Criterio de orden para merge sort en las sugerencias: va primero la que tiene
// más amigos en común (usa el operator> sobrecargado de Sugerencia).
bool masAmigosEnComun(Sugerencia a, Sugerencia b) {
    return a > b;
}

void verSugerencias(RedSocial& red, Usuario* sesion) {
    cout << "\nCuantas sugerencias quieres ver (1 a 20)? ";
    int k = leerOpcion();
    if (k < 1 || k > 20) {
        cout << "Debe ser un numero entre 1 y 20." << endl;
        return;
    }

    // Candidatos: amigos de mis amigos que no son mis amigos, con sus amigos en común
    int total = red.getCantidadUsuarios();
    Sugerencia* candidatos = new Sugerencia[total];
    MedidorRecursos::sumarMemoria(total * sizeof(Sugerencia));
    int cantidad = red.calcularCandidatos(sesion, candidatos);

    // Se ordenan de más a menos amigos en común (los empates quedan en orden alfabético)
    mergeSort(candidatos, cantidad, masAmigosEnComun);

    int mostrar = (k < cantidad) ? k : cantidad;
    if (mostrar == 0) {
        cout << "No hay sugerencias por ahora." << endl;
    } else {
        cout << "\nSugerencias de amistad:" << endl;
        for (int i = 0; i < mostrar; i++) {
            MedidorRecursos::contar();
            cout << "  " << (i + 1) << ". " << candidatos[i].getUsuario()->getNombreUsuario()
                 << "  (" << candidatos[i].getComunes() << " amigos en comun)" << endl;
        }

        cout << "Numero de la sugerencia a la que quieres enviar solicitud (0 para volver): ";
        int elegida = leerOpcion();
        if (elegida >= 1 && elegida <= mostrar) {
            Usuario* otro = candidatos[elegida - 1].getUsuario();
            if (sesion->tieneSolicitudDe(otro)) {
                cout << otro->getNombreUsuario() << " ya te envio una solicitud: aceptala en Administrar amistades." << endl;
            } else if (otro->tieneSolicitudDe(sesion)) {
                cout << "Ya le enviaste una solicitud a " << otro->getNombreUsuario() << "." << endl;
            } else {
                sesion->enviarSolicitud(otro);
                cout << "Solicitud enviada a " << otro->getNombreUsuario() << "." << endl;
            }
        }
    }

    MedidorRecursos::restarMemoria(total * sizeof(Sugerencia));
    delete[] candidatos;
}

// VI. Administrar mensajes: enviar, leer nuevos y buscar por rango de fechas
// Criterio de orden para merge sort: el mensaje enviado antes va primero.
// Usa el operator< sobrecargado de Mensaje (compara las fechas de envío).
bool mensajeMasAntiguo(Mensaje* a, Mensaje* b) {
    return *a < *b;
}

// Imprime un mensaje: fecha, emisor y texto
void imprimirMensaje(Mensaje* m) {
    cout << "  [" << m->getFechaenvio() << "] " << m->getEmisor()->getNombreUsuario()
    << ": " << m->getContenido() << endl;
}

void administrarMensajes(RedSocial& red, Usuario* sesion) {
    cout << "\n--- Administrar mensajes ---" << endl;
    cout << "1. Enviar mensaje" << endl;
    cout << "2. Leer nuevos mensajes" << endl;
    cout << "3. Buscar mensajes por rango de fechas" << endl;
    cout << "0. Volver" << endl;
    cout << "Opcion: ";
    int opcion = leerOpcion();

    if (opcion == 1) {
        // ----- Enviar mensaje a un amigo -----
        string nombre, texto;
        cout << "Nombre del amigo: ";
        getline(cin, nombre);
        Usuario* amigo = red.buscarUsuario(nombre);
        if (amigo == nullptr) {
            cout << "Ese usuario no existe." << endl;
            return;
        }
        if (!sesion->esAmigo(amigo)) {
            cout << nombre << " no es tu amigo." << endl;
            return;
        }
        cout << "Mensaje (maximo 512 caracteres): ";
        getline(cin, texto);
        if (red.enviarMensaje(sesion, amigo, texto)) {
            cout << "Mensaje enviado a " << nombre << "." << endl;
        } else {
            cout << "El mensaje no puede estar vacio ni pasar de 512 caracteres." << endl;
        }

    } else if (opcion == 2) {
        // ----- Leer nuevos: se juntan los no leídos, se ordenan por fecha y se marcan -----
        int recibidos = sesion->getCantidadMensajesRecibidos();
        Mensaje** nuevos = new Mensaje*[recibidos + 1];
        MedidorRecursos::sumarMemoria((recibidos + 1) * sizeof(Mensaje*));
        int cantidadNuevos = 0;
        for (int i = 0; i < recibidos; i++) {
            MedidorRecursos::contar();
            Mensaje* m = sesion->getMensajesRecibidos()[i];
            if (!m->getVisto()) {
                nuevos[cantidadNuevos] = m;
                cantidadNuevos++;
            }
        }

        if (cantidadNuevos == 0) {
            cout << "No tienes mensajes nuevos." << endl;
        } else {
            mergeSort(nuevos, cantidadNuevos, mensajeMasAntiguo);
            cout << "\nTienes " << cantidadNuevos << " mensajes nuevos:" << endl;
            for (int i = 0; i < cantidadNuevos; i++) {
                MedidorRecursos::contar();
                imprimirMensaje(nuevos[i]);
                nuevos[i]->marcarComoVisto();
            }
        }
        MedidorRecursos::restarMemoria((recibidos + 1) * sizeof(Mensaje*));
        delete[] nuevos;

    } else if (opcion == 3) {
        // ----- Buscar por rango de fechas (ambas inclusive) -----
        string textoInicio, textoFin;
        cout << "Fecha de inicio (dd/mm/aaaa): ";
        getline(cin, textoInicio);
        cout << "Fecha de fin (dd/mm/aaaa): ";
        getline(cin, textoFin);
        Fecha inicio = RedSocial::textoAFecha(textoInicio);
        Fecha finDia = RedSocial::textoAFecha(textoFin);
        if (!Fecha::esValida(inicio.getDia(), inicio.getMes(), inicio.getAnio()) ||
            !Fecha::esValida(finDia.getDia(), finDia.getMes(), finDia.getAnio())) {
            cout << "Alguna de las fechas no es valida." << endl;
            return;
        }
        // El fin se toma hasta las 23:59 para que el último día quede incluido completo
        Fecha fin(finDia.getDia(), finDia.getMes(), finDia.getAnio(), 23, 59);
        if (fin < inicio) {
            cout << "La fecha de inicio no puede ser posterior a la de fin." << endl;
            return;
        }

        int encontrados = 0;
        cout << "\nMensajes recibidos entre " << textoInicio << " y " << textoFin << ":" << endl;
        for (unsigned int i = 0; i < sesion->getCantidadMensajesRecibidos(); i++) {
            MedidorRecursos::contar();
            Mensaje* m = sesion->getMensajesRecibidos()[i];
            if (m->estaEnRango(inicio, fin)) {
                imprimirMensaje(m);
                encontrados++;
            }
        }
        cout << "Total: " << encontrados << " mensajes." << endl;
    }
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
            mostrarMejoresAmigos(sesion);
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