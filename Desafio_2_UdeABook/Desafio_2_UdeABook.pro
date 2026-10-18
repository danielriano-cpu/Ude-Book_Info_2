TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        album.cpp \
        fecha.cpp \
        foto.cpp \
        main.cpp \
        medidorrecursos.cpp \
        mensaje.cpp \
        redsocial.cpp \
        usuario.cpp

HEADERS += \
    album.h \
    fecha.h \
    foto.h \
    lista.h \
    medidorrecursos.h \
    mensaje.h \
    redsocial.h \
    usuario.h

DISTFILES += \
    datos/albumes.txt \
    datos/amistades.txt \
    datos/fotos.txt \
    datos/mensajes.txt \
    datos/solicitudes.txt \
    datos/usuarios.txt
