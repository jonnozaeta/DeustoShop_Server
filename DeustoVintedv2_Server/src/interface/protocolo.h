/*
 * interface.h
 *
 *  Created on: 14 may 2026
 *      Author: jon.ozaeta
 */

#ifndef PROTOCOLO_H_
#define PROTOCOLO_H_

#include <winsock2.h>
#include <iostream>
#include <cstring>
#include "../database/sqlite3.h"

class ProtocoloServidor {
private:
    SOCKET comm_socket;
    char recvBuff[512];
    char sendBuff[512];

    sqlite3* db;

    // Métodos privados auxiliares
    void enviarRespuesta(const char* msg);
    char* recibirDato();

public:
    // Constructor
    ProtocoloServidor(SOCKET s, sqlite3* database);

    void atenderCliente();

    // Manejadores específicos para cada comando del protocolo

    void atenderLogin();
    void atenderRegistro();

    //Por implementar
    void atenderSubirProducto();
    void atenderEliminarProducto();
    void atenderModificarProducto();
    void atenderComprar();
    void atenderVender();
    void atenderVerPerfil();
};

#endif
