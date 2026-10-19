#include "redsocial.h"
#include "medidorrecursos.h"
#include <fstream>
#include <iostream>

// Las listas arrancan grandes porque se cargan más de 1000 usuarios:
// así no tienen que crecer varias veces durante la carga
RedSocial::RedSocial() : usuarios(1024), mensajes(1024) {
}

// Borra usuarios y mensajes (es su dueña). Usuario borra sus álbumes y Album sus fotos
RedSocial::~RedSocial() {
    for (int i = 0; i < mensajes.getCantidad(); i++) {
        MedidorRecursos::restarMemoria(sizeof(Mensaje));
        delete mensajes[i];
        MedidorRecursos::contar();
    }
    for (int i = 0; i < usuarios.getCantidad(); i++) {
        MedidorRecursos::restarMemoria(sizeof(Usuario));
        delete usuarios[i];
        MedidorRecursos::contar();
    }
}

// Inserción ordenada: deja la lista en orden alfabético para poder usar búsqueda binaria
void RedSocial::agregarUsuario(Usuario* u) {
    string nombre = u->getNombreUsuario();
    int n = usuarios.getCantidad();

    // Atajo: si va después del último (caso normal con el archivo ordenado), va al final
    MedidorRecursos::contar();
    if (n == 0 || usuarios[n - 1]->getNombreUsuario() < nombre) {
        usuarios.agregar(u);
        return;
    }

    // Si no, se busca su posición con búsqueda binaria y se inserta ahí
    int izquierda = 0;
    int derecha = n;
    while (izquierda < derecha) {
        MedidorRecursos::contar();
        int medio = (izquierda + derecha) / 2;
        if (usuarios[medio]->getNombreUsuario() < nombre) {
            izquierda = medio + 1;
        } else {
            derecha = medio;
        }
    }
    usuarios.insertarEn(izquierda, u);
}

// Búsqueda binaria por nombre (~10 vueltas con 1000 usuarios). nullptr si no existe
Usuario* RedSocial::buscarUsuario(const string& nombre) const {
    int izquierda = 0;
    int derecha = usuarios.getCantidad() - 1;
    while (izquierda <= derecha) {
        MedidorRecursos::contar();
        int medio = (izquierda + derecha) / 2;
        string nombreMedio = usuarios[medio]->getNombreUsuario();
        if (nombreMedio == nombre) {
            return usuarios[medio];
        } else if (nombreMedio < nombre) {
            izquierda = medio + 1;      // está en la mitad derecha
        } else {
            derecha = medio - 1;        // está en la mitad izquierda
        }
    }
    return nullptr;
}

void RedSocial::agregarMensaje(Mensaje* m) {
    mensajes.agregar(m);
}

int RedSocial::getCantidadUsuarios() const {
    return usuarios.getCantidad();
}

int RedSocial::getCantidadMensajes() const {
    return mensajes.getCantidad();
}

//Auxiliares para la carga

// Parte la línea por ';'. La última parte no se corta (el texto puede tener ';')
int RedSocial::partirLinea(const string& linea, string partes[], int maxPartes) {
    int cantidad = 0;
    partes[0] = "";
    for (int i = 0; i < (int)linea.length(); i++) {
        MedidorRecursos::contar();
        char c = linea[i];
        if (c == ';' && cantidad < maxPartes - 1) {
            cantidad++;
            partes[cantidad] = "";
        } else if (c != '\r') {          // ignora el \r de los archivos hechos en Windows
            partes[cantidad] += c;
        }
    }
    return cantidad + 1;
}

// Convierte "123" en 123, recorriendo dígito por dígito.
int RedSocial::textoAEntero(const string& texto) {
    int numero = 0;
    for (int i = 0; i < (int)texto.length(); i++) {
        MedidorRecursos::contar();
        if (texto[i] >= '0' && texto[i] <= '9') {
            numero = numero * 10 + (texto[i] - '0');
        }
    }
    return numero;
}

// "dd/mm/aaaa" o "dd/mm/aaaa hh:mm" -> Fecha
Fecha RedSocial::textoAFecha(const string& texto) {
    int valores[5] = {0, 0, 0, 0, 0};    // dia, mes, anio, hora, minuto
    int pos = 0;
    for (int i = 0; i < (int)texto.length() && pos < 5; i++) {
        MedidorRecursos::contar();
        char c = texto[i];
        if (c >= '0' && c <= '9') {
            valores[pos] = valores[pos] * 10 + (c - '0');
        } else if (i > 0 && texto[i - 1] >= '0' && texto[i - 1] <= '9') {
            pos++;
        }
    }
    return Fecha(valores[0], valores[1], valores[2], valores[3], valores[4]);
}

// Carga de archivos

// Carga en orden: usuarios primero porque los demás archivos los buscan por nombre
bool RedSocial::cargarDatos(const string& carpeta) {
    if (!cargarUsuarios(carpeta + "usuarios.txt")) return false;
    if (!cargarAmistades(carpeta + "amistades.txt")) return false;
    if (!cargarSolicitudes(carpeta + "solicitudes.txt")) return false;
    if (!cargarAlbumes(carpeta + "albumes.txt")) return false;
    if (!cargarFotos(carpeta + "fotos.txt")) return false;
    if (!cargarMensajes(carpeta + "mensajes.txt")) return false;
    return true;
}

// Formato de cada línea:
// nombre;clave;email;fotoPerfil;dd/mm/aaaa(nacimiento);ciudad;pais;dd/mm/aaaa(inscripcion)
bool RedSocial::cargarUsuarios(const string& ruta) {
    ifstream archivo(ruta);
    if (!archivo.is_open()) {
        cout << "No se pudo abrir " << ruta << endl;
        return false;
    }
    string linea;
    string partes[8];
    while (getline(archivo, linea)) {
        MedidorRecursos::contar();
        if (partirLinea(linea, partes, 8) < 8) continue;   // línea vacía o incompleta: se salta
        Usuario* u = new Usuario(partes[0],                  // nombre
                                 textoAFecha(partes[4]),     // nacimiento
                                 partes[2],                  // email
                                 partes[3],                  // foto de perfil
                                 partes[1],                  // clave
                                 partes[5],                  // ciudad
                                 partes[6],                  // pais
                                 textoAFecha(partes[7]));    // inscripcion
        MedidorRecursos::sumarMemoria(sizeof(Usuario));
        agregarUsuario(u);
    }
    archivo.close();
    return true;
}

// Formato de cada línea: usuario1;usuario2
// La amistad es mutua: cada uno queda en la lista de amigos del otro.
bool RedSocial::cargarAmistades(const string& ruta) {
    ifstream archivo(ruta);
    if (!archivo.is_open()) {
        cout << "No se pudo abrir " << ruta << endl;
        return false;
    }
    string linea;
    string partes[2];
    while (getline(archivo, linea)) {
        MedidorRecursos::contar();
        if (partirLinea(linea, partes, 2) < 2) continue;
        Usuario* a = buscarUsuario(partes[0]);
        Usuario* b = buscarUsuario(partes[1]);
        if (a != nullptr && b != nullptr) {
            a->agregarAmigo(b);
            b->agregarAmigo(a);
        }
    }
    archivo.close();
    return true;
}

// Formato de cada línea: emisor;receptor  (solicitud pendiente)
// La solicitud queda guardada en el receptor, que es quien la acepta.
bool RedSocial::cargarSolicitudes(const string& ruta) {
    ifstream archivo(ruta);
    if (!archivo.is_open()) {
        cout << "No se pudo abrir " << ruta << endl;
        return false;
    }
    string linea;
    string partes[2];
    while (getline(archivo, linea)) {
        MedidorRecursos::contar();
        if (partirLinea(linea, partes, 2) < 2) continue;
        Usuario* emisor = buscarUsuario(partes[0]);
        Usuario* receptor = buscarUsuario(partes[1]);
        if (emisor != nullptr && receptor != nullptr) {
            receptor->recibirSolicitud(emisor);
        }
    }
    archivo.close();
    return true;
}

// Formato de cada línea: usuario;nombreAlbum;ruta;privacidad
// privacidad: 0 = privado, 1 = amigos, 2 = público
// El usuario es el dueño del álbum (su destructor lo borra).
bool RedSocial::cargarAlbumes(const string& ruta) {
    ifstream archivo(ruta);
    if (!archivo.is_open()) {
        cout << "No se pudo abrir " << ruta << endl;
        return false;
    }
    string linea;
    string partes[4];
    while (getline(archivo, linea)) {
        MedidorRecursos::contar();
        if (partirLinea(linea, partes, 4) < 4) continue;
        Usuario* dueno = buscarUsuario(partes[0]);
        if (dueno == nullptr) continue;
        Album* album = new Album(partes[1], partes[2], (short)textoAEntero(partes[3]));
        MedidorRecursos::sumarMemoria(sizeof(Album));
        dueno->agregarAlbum(album);
    }
    archivo.close();
    return true;
}

// Formato de cada línea: usuario;nombreAlbum;rutaFoto;dd/mm/aaaa;tamanio;likes
// El álbum es el dueño de sus fotos (agregarFoto hace el new y suma la memoria).
bool RedSocial::cargarFotos(const string& ruta) {
    ifstream archivo(ruta);
    if (!archivo.is_open()) {
        cout << "No se pudo abrir " << ruta << endl;
        return false;
    }
    string linea;
    string partes[6];
    while (getline(archivo, linea)) {
        MedidorRecursos::contar();
        if (partirLinea(linea, partes, 6) < 6) continue;
        Usuario* dueno = buscarUsuario(partes[0]);
        if (dueno == nullptr) continue;
        Album* album = buscarAlbum(dueno, partes[1]);
        if (album == nullptr) continue;
        Foto foto(partes[2], textoAFecha(partes[3]), (unsigned int)textoAEntero(partes[4]));
        Foto* guardada = album->agregarFoto(foto);
        guardada->setLikes(textoAEntero(partes[5]));
    }
    archivo.close();
    return true;
}

// Búsqueda lineal (cada usuario tiene pocos álbumes)
Album* RedSocial::buscarAlbum(Usuario* dueno, const string& nombreAlbum) {
    for (unsigned int i = 0; i < dueno->getCantidadAlbumes(); i++) {
        MedidorRecursos::contar();
        if (dueno->getAlbumes()[i]->getNombre() == nombreAlbum) {
            return dueno->getAlbumes()[i];
        }
    }
    return nullptr;
}

// Formato de cada línea: emisor;receptor;dd/mm/aaaa hh:mm;visto(0/1);texto
// El texto va de último para que pueda contener ';' (partirLinea no lo corta).
// RedSocial es la dueña del mensaje; emisor y receptor solo guardan el puntero.
bool RedSocial::cargarMensajes(const string& ruta) {
    ifstream archivo(ruta);
    if (!archivo.is_open()) {
        cout << "No se pudo abrir " << ruta << endl;
        return false;
    }
    string linea;
    string partes[5];
    while (getline(archivo, linea)) {
        MedidorRecursos::contar();
        if (partirLinea(linea, partes, 5) < 5) continue;
        Usuario* emisor = buscarUsuario(partes[0]);
        Usuario* receptor = buscarUsuario(partes[1]);
        if (emisor == nullptr || receptor == nullptr) continue;
        if (!Mensaje::textoValido(partes[4].c_str())) continue;   // vacío o de más de 512 caracteres

        Mensaje* m = new Mensaje(emisor, receptor, partes[4].c_str(), textoAFecha(partes[2]));
        MedidorRecursos::sumarMemoria(sizeof(Mensaje));
        if (partes[3] == "1") {
            m->marcarComoVisto();
        }
        agregarMensaje(m);
        emisor->enviarMensaje(m);
        receptor->recibirMensaje(m);
    }
    archivo.close();
    return true;
}

// ---------- Funcionalidades ----------

// II. Inicio de sesión: busca al usuario (búsqueda binaria) y compara la clave
Usuario* RedSocial::iniciarSesion(const string& nombre, const string& clave) const {
    Usuario* u = buscarUsuario(nombre);
    if (u != nullptr && u->getContrasenia() == clave) {
        return u;
    }
    return nullptr;
}

// VI. Enviar mensaje: valida el texto, crea el mensaje con la fecha actual y lo registra.
// RedSocial es la dueña del mensaje; emisor y receptor guardan el mismo puntero.
bool RedSocial::enviarMensaje(Usuario* emisor, Usuario* receptor, const string& texto) {
    if (!Mensaje::textoValido(texto.c_str())) return false;   // vacío o de más de 512 caracteres
    Mensaje* m = new Mensaje(emisor, receptor, texto.c_str(), Fecha::ahora());
    MedidorRecursos::sumarMemoria(sizeof(Mensaje));
    agregarMensaje(m);
    emisor->enviarMensaje(m);
    receptor->recibirMensaje(m);
    return true;
}

// Posición de un usuario en la lista (búsqueda binaria, la lista está siempre ordenada)
int RedSocial::indiceDe(const string& nombre) const {
    int izquierda = 0;
    int derecha = usuarios.getCantidad() - 1;
    while (izquierda <= derecha) {
        MedidorRecursos::contar();
        int medio = (izquierda + derecha) / 2;
        string nombreMedio = usuarios[medio]->getNombreUsuario();
        if (nombreMedio == nombre) return medio;
        if (nombreMedio < nombre) izquierda = medio + 1;
        else derecha = medio - 1;
    }
    return -1;
}

// V. Amigos de mis amigos que no son mis amigos, con sus amigos en común.
// conteo[] tiene una casilla por usuario (una fila de la matriz): cada amigo de un amigo suma 1
int RedSocial::calcularCandidatos(Usuario* u, Sugerencia* candidatos) const {
    int n = usuarios.getCantidad();
    int* conteo = new int[n];
    MedidorRecursos::sumarMemoria(n * sizeof(int));
    for (int i = 0; i < n; i++) {
        conteo[i] = 0;
        MedidorRecursos::contar();
    }

    // Yo y mis amigos actuales se marcan con -1: no se pueden sugerir
    conteo[indiceDe(u->getNombreUsuario())] = -1;
    for (unsigned int i = 0; i < u->getCantidadAmigos(); i++) {
        MedidorRecursos::contar();
        conteo[indiceDe(u->getAmigos()[i]->getNombreUsuario())] = -1;
    }

    // Por cada amigo mío, cada uno de sus amigos suma 1 (si no está marcado con -1)
    for (unsigned int i = 0; i < u->getCantidadAmigos(); i++) {
        Usuario* amigo = u->getAmigos()[i];
        for (unsigned int j = 0; j < amigo->getCantidadAmigos(); j++) {
            MedidorRecursos::contar();
            int posicion = indiceDe(amigo->getAmigos()[j]->getNombreUsuario());
            if (conteo[posicion] >= 0) {
                conteo[posicion]++;
            }
        }
    }

    // Los que quedaron con conteo > 0 son los candidatos
    int cantidad = 0;
    for (int i = 0; i < n; i++) {
        MedidorRecursos::contar();
        if (conteo[i] > 0) {
            candidatos[cantidad] = Sugerencia(usuarios[i], conteo[i]);
            cantidad++;
        }
    }

    MedidorRecursos::restarMemoria(n * sizeof(int));
    delete[] conteo;
    return cantidad;
}


// ---------- Guardado de archivos ----------

// Guarda lo que puede cambiar mientras el programa corre: amistades (al aceptar),
// solicitudes (al enviar o aceptar), fotos (los likes) y mensajes (nuevos y vistos).
// usuarios.txt y albumes.txt no se reescriben porque el programa no los modifica.
// Usa el mismo formato que la carga, así la próxima ejecución lee los cambios.
bool RedSocial::guardarDatos(const string& carpeta) const {
    if (!guardarAmistades(carpeta + "amistades.txt")) return false;
    if (!guardarSolicitudes(carpeta + "solicitudes.txt")) return false;
    if (!guardarFotos(carpeta + "fotos.txt")) return false;
    if (!guardarMensajes(carpeta + "mensajes.txt")) return false;
    return true;
}

// Escribe una fecha como dd/mm/aaaa (sin hora), con ceros a la izquierda
void RedSocial::escribirFechaSinHora(ostream& salida, const Fecha& f) {
    if (f.getDia() < 10) salida << "0";
    salida << f.getDia() << "/";
    if (f.getMes() < 10) salida << "0";
    salida << f.getMes() << "/" << f.getAnio();
}

// Formato: usuario1;usuario2
// Cada amistad está en los dos usuarios; se escribe una sola vez,
// solo cuando el nombre del amigo va después alfabéticamente.
bool RedSocial::guardarAmistades(const string& ruta) const {
    ofstream archivo(ruta);
    if (!archivo.is_open()) {
        cout << "No se pudo escribir " << ruta << endl;
        return false;
    }
    for (int i = 0; i < usuarios.getCantidad(); i++) {
        Usuario* u = usuarios[i];
        for (unsigned int j = 0; j < u->getCantidadAmigos(); j++) {
            MedidorRecursos::contar();
            Usuario* amigo = u->getAmigos()[j];
            if (u->getNombreUsuario() < amigo->getNombreUsuario()) {
                archivo << u->getNombreUsuario() << ";" << amigo->getNombreUsuario() << "\n";
            }
        }
    }
    archivo.close();
    return true;
}

// Formato: emisor;receptor  (las solicitudes están guardadas en el receptor)
bool RedSocial::guardarSolicitudes(const string& ruta) const {
    ofstream archivo(ruta);
    if (!archivo.is_open()) {
        cout << "No se pudo escribir " << ruta << endl;
        return false;
    }
    for (int i = 0; i < usuarios.getCantidad(); i++) {
        Usuario* receptor = usuarios[i];
        for (unsigned int j = 0; j < receptor->getCantidadSolicitudes(); j++) {
            MedidorRecursos::contar();
            archivo << receptor->getSolicitudesAmistad()[j]->getNombreUsuario() << ";"
                    << receptor->getNombreUsuario() << "\n";
        }
    }
    archivo.close();
    return true;
}

// Formato: usuario;nombreAlbum;rutaFoto;dd/mm/aaaa;tamanio;likes
bool RedSocial::guardarFotos(const string& ruta) const {
    ofstream archivo(ruta);
    if (!archivo.is_open()) {
        cout << "No se pudo escribir " << ruta << endl;
        return false;
    }
    for (int i = 0; i < usuarios.getCantidad(); i++) {
        Usuario* u = usuarios[i];
        for (unsigned int a = 0; a < u->getCantidadAlbumes(); a++) {
            Album* album = u->getAlbumes()[a];
            for (int f = 0; f < album->getFotos().getCantidad(); f++) {
                MedidorRecursos::contar();
                Foto* foto = album->getFotos()[f];
                archivo << u->getNombreUsuario() << ";" << album->getNombre() << ";"
                        << foto->getRutaArchivo() << ";";
                escribirFechaSinHora(archivo, foto->getFechaPublicacion());
                archivo << ";" << foto->getTamanioBytes() << ";" << foto->getLikes() << "\n";
            }
        }
    }
    archivo.close();
    return true;
}

// Formato: emisor;receptor;dd/mm/aaaa hh:mm;visto(0/1);texto
// La fecha se escribe con el operator<< de Fecha, que ya da ese formato.
bool RedSocial::guardarMensajes(const string& ruta) const {
    ofstream archivo(ruta);
    if (!archivo.is_open()) {
        cout << "No se pudo escribir " << ruta << endl;
        return false;
    }
    for (int i = 0; i < mensajes.getCantidad(); i++) {
        MedidorRecursos::contar();
        Mensaje* m = mensajes[i];
        archivo << m->getEmisor()->getNombreUsuario() << ";"
                << m->getReceptor()->getNombreUsuario() << ";"
                << m->getFechaenvio() << ";"
                << (m->getVisto() ? "1" : "0") << ";"
                << m->getContenido() << "\n";
    }
    archivo.close();
    return true;
}