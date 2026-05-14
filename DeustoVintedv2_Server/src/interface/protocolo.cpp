#include <iostream>
#include <cstring>
#include "protocolo.h"
#include "../database/database.h"

using namespace std;

ProtocoloServidor::ProtocoloServidor(SOCKET s, sqlite3* database) {
    this->comm_socket = s;
    this->db = database;

    // Inicializamos los buffers a cero por seguridad
    memset(sendBuff, 0, 512);
    memset(recvBuff, 0, 512);
}

void ProtocoloServidor::enviarRespuesta(const char* msg) {
    memset(sendBuff, 0, 512);
    strcpy(sendBuff, msg);
    send(comm_socket, sendBuff, 512, 0);
}

char* ProtocoloServidor::recibirDato() {
    memset(recvBuff, 0, 512);
    recv(comm_socket, recvBuff, 512, 0);
    return recvBuff;
}

void ProtocoloServidor::atenderCliente() {
    bool salir = false;
    do {
        char* comando = recibirDato();

        if (strlen(comando) == 0) break;

        cout << "Comando recibido: " << comando << endl;

        if (strcmp(comando, "LOGIN") == 0) {
            atenderLogin();
        } else if (strcmp(comando, "REGISTER") == 0) {
        	atenderRegistro();
        } else if (strcmp(comando, "SUBIR_PROD") == 0) {
            atenderSubirProducto();
        } else if (strcmp(comando, "ELIMINAR_PROD") == 0) {
            atenderEliminarProducto();
        } else if (strcmp(comando, "MOD_PROD") == 0) {
            atenderModificarProducto();
        } else if (strcmp(comando, "COMPRAR") == 0) {
            atenderComprar();
        } else if (strcmp(comando, "MIS_VENTAS") == 0) {
            atenderVender();
        } else if (strcmp(comando, "VER_PERFIL") == 0) {
            atenderVerPerfil();
        } else if (strcmp(comando, "IP") == 0) {
            enviarRespuesta("127.0.0.1");
        } else if (strcmp(comando, "EXIT") == 0) {
            cout << "El cliente ha solicitado cerrar sesion." << endl;
            salir = true;
        }
    } while (!salir);
}

// --- IMPLEMENTACIÓN DE LOS MANEJADORES ---

void ProtocoloServidor::atenderLogin() {
    char* user = recibirDato();
    char* pass = recibirDato();

    int resultado = comprobarUsuarioBD(this->db, user, pass);

    if (resultado == 1) {
        cout << "[SISTEMA] Login correcto: " << user << endl;
        enviarRespuesta("OK");
    } else {
        cout << "[SISTEMA] Login fallido: " << user << endl;
        enviarRespuesta("ERROR");
    }
}

void ProtocoloServidor::atenderRegistro() {
    char* correo = recibirDato();
    char* pass = recibirDato();
    char* nombre = recibirDato();

    cout << "[DEBUG] Procesando registro para: " << correo << endl;

    int resultado = registrarUsuarioBD(this->db, correo, pass, nombre);

    if (resultado == 1) {
        enviarRespuesta("OK");
    } else {
        enviarRespuesta("ERROR");
    }

    delete[] correo;
    delete[] pass;
    delete[] nombre;
}

void ProtocoloServidor::atenderSubirProducto(){

}
void ProtocoloServidor::atenderEliminarProducto(){

}
void ProtocoloServidor::atenderModificarProducto(){

}
void ProtocoloServidor::atenderComprar(){

}
void ProtocoloServidor::atenderVender(){

}
void ProtocoloServidor::atenderVerPerfil(){

}










