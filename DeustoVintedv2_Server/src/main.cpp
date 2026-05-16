#include <iostream>
#include <winsock2.h>
#include "database/sqlite3.h"
#include "interface/protocolo.h"

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 6000

using namespace std;

int main() {

    sqlite3* db;
    //CAMBIAR RUTA PARA EL CORRECTO FUNCIONAMIENTO
    if (sqlite3_open("C:\\Users\\iker.perez.dulce\\git\\DeustoShop_Server\\DeustoVintedv2_Server\\data\\DataBase.db", &db) != SQLITE_OK) {
        cout << "Error abriendo la base de datos" << endl;
        return -1;
    }

    WSADATA wsaData;
    SOCKET conn_socket;
    struct sockaddr_in server;
    struct sockaddr_in client;
    int stsize = sizeof(struct sockaddr_in);

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        cout << "Error al inicializar Winsock" << endl;
        sqlite3_close(db);
        return -1;
    }

    conn_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (conn_socket == INVALID_SOCKET) {
        WSACleanup();
        sqlite3_close(db);
        return -1;
    }

    server.sin_addr.s_addr = inet_addr(SERVER_IP);
    server.sin_family = AF_INET;
    server.sin_port = htons(SERVER_PORT);

    if (bind(conn_socket, (struct sockaddr*)&server, sizeof(server)) == SOCKET_ERROR) {
        closesocket(conn_socket);
        WSACleanup();
        sqlite3_close(db);
        return -1;
    }

    if (listen(conn_socket, 1) == SOCKET_ERROR) {
        closesocket(conn_socket);
        WSACleanup();
        sqlite3_close(db);
        return -1;
    }

    cout << "Servidor esperando conexiones en el puerto " << SERVER_PORT << "..." << endl;

    SOCKET comm_socket = accept(conn_socket, (struct sockaddr*)&client, &stsize);

    if (comm_socket != INVALID_SOCKET) {
        cout << "Cliente conectado desde: " << inet_ntoa(client.sin_addr) << endl;

        ProtocoloServidor proto(comm_socket, db);
        proto.atenderCliente();

        closesocket(comm_socket);
    }

    closesocket(conn_socket);
    WSACleanup();
    sqlite3_close(db);

    return 0;
}
