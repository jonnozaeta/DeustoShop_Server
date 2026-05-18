#include <iostream>
#include <cstring>
#include "protocolo.h"
#include "../logs/log.h"
#include "../database/database.h"
#include <string>

using namespace std;

ProtocoloServidor::ProtocoloServidor(SOCKET s, sqlite3* database) {
    this->comm_socket = s;
    this->db = database;

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
        } else if (strcmp(comando, "VENDER") == 0) {
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
    char* paquete = recibirDato();
    if (paquete == NULL) return;

    char* user = strtok(paquete, "|");
    char* pass = strtok(NULL, "|");

    if (user != NULL && pass != NULL) {
        int resultado = comprobarUsuarioBD(this->db, user, pass);

        if (resultado == 1) {
            cout << "[SISTEMA] Login correcto: " << user << endl;

            registrar_log_inicio_sesion(user);

            enviarRespuesta("OK");
        } else {
            cout << "[SISTEMA] Login fallido para: " << user << endl;
            enviarRespuesta("ERROR");
        }
    } else {
        cout << "[ERROR] Paquete de Login mal formado" << endl;
        enviarRespuesta("ERROR");
    }

    delete[] paquete;
}

void ProtocoloServidor::atenderRegistro() {
    char* paquete = recibirDato();
    if (paquete == NULL) return;

    char* correo = strtok(paquete, "|");
    char* pass = strtok(NULL, "|");
    char* nombre = strtok(NULL, "|");

    if (correo != NULL && pass != NULL && nombre != NULL) {
        cout << "[DEBUG] Registro - Correo: " << correo << " Nombre: " << nombre << endl;

        int resultado = registrarUsuarioBD(this->db, correo, pass, nombre);

        if (resultado == 1) {

        	registrar_log_registro_usuario(correo, nombre);

            enviarRespuesta("OK");
        } else {
            enviarRespuesta("ERROR");
        }
    } else {
        cout << "[ERROR] Paquete de Registro mal formado" << endl;
        enviarRespuesta("ERROR");
    }

    delete[] paquete;
}

//CREARLOGS PARA REGISTRAR EVENTOS IMPORTANTES

void ProtocoloServidor::atenderSubirProducto() {
    char* paquete = recibirDato();
    if (paquete == NULL || strlen(paquete) == 0) return;

    char datos[512];
    strcpy(datos, paquete);

    char* nombre = strtok(datos, "|");
    char* desc = strtok(NULL, "|");
    char* talla = strtok(NULL, "|");
    char* sexo = strtok(NULL, "|");
    char* precioStr = strtok(NULL, "|");
    char* idUserStr = strtok(NULL, "|");

    if (nombre && desc && talla && sexo && precioStr && idUserStr) {
        int res = insertarProductoBD(this->db, nombre, desc, talla, sexo, atoi(precioStr), atoi(idUserStr));
        if (res == 1) {
            cout << "[SISTEMA] Producto creado: " << nombre << endl;
            enviarRespuesta("OK");
        } else {
            enviarRespuesta("ERROR");
        }
    } else {
        enviarRespuesta("ERROR");
    }
}

void ProtocoloServidor::atenderEliminarProducto() {
    char* paquete = recibirDato();
    if (paquete == NULL || strlen(paquete) == 0) return;

    int id_prod = atoi(paquete);

    if (id_prod > 0) {
        int res = eliminarProductoBD(this->db, id_prod);
        if (res == 1) {
            cout << "[SISTEMA] Producto eliminado ID: " << id_prod << endl;
            enviarRespuesta("OK");
        } else {
            enviarRespuesta("ERROR");
        }
    } else {
        enviarRespuesta("ERROR");
    }
}

void ProtocoloServidor::atenderModificarProducto() {
    char* paquete = recibirDato();
    if (paquete == NULL || strlen(paquete) == 0) return;

    char datos[512];
    strcpy(datos, paquete);

    char* idStr = strtok(datos, "|");
    char* nombre = strtok(NULL, "|");
    char* desc = strtok(NULL, "|");
    char* talla = strtok(NULL, "|");
    char* sexo = strtok(NULL, "|");
    char* precioStr = strtok(NULL, "|");

    if (idStr && nombre && desc && talla && sexo && precioStr) {
        int res = modificarProductoBD(this->db, atoi(idStr), nombre, desc, talla, sexo, atoi(precioStr));
        if (res == 1) {
            cout << "[SISTEMA] Producto ID " << idStr << " modificado." << endl;
            enviarRespuesta("OK");
        } else {
            enviarRespuesta("ERROR");
        }
    } else {
        enviarRespuesta("ERROR");
    }
}

void ProtocoloServidor::atenderComprar(){

	char *paquete = recibirDato();

	if(paquete ==NULL || strlen(paquete)== 0){
		enviarRespuesta("ERROR");
		return;
	}

	char datos[512];
	strcpy(datos, paquete);

	char *idProdStr = strtok(datos, "|");
	char *correoUsuario = strtok(NULL,"|");

	if(idProdStr != NULL &&correoUsuario != NULL){
		int idProd = atoi(idProdStr);
		int res = comprarProductoBD(this->db, idProd,correoUsuario);

		if(res ==1){
			cout << "[SISTEMA] Producto comprado. ID producto: " << idProd <<endl;
			enviarRespuesta("OK");
		} else{
			enviarRespuesta("ERROR");
		}
	} else{
		enviarRespuesta("ERROR");
	}

}
void ProtocoloServidor::atenderVender(){

	char *paquete = recibirDato();

	if(paquete == NULL || strlen(paquete) == 0){
		enviarRespuesta("ERROR");
		return;
	}

	char datos[512];
	strcpy(datos, paquete);
	char *idProdStr = strtok(datos, "|");
	char *correoVendedor =strtok(NULL, "|");
	char *correoComprador = strtok(NULL, "|");

	if(idProdStr != NULL && correoVendedor != NULL && correoComprador != NULL){
		int idProd = atoi(idProdStr);
		int res = venderProductoBD(this->db, idProd, correoVendedor, correoComprador);

		if(res ==1){
			cout<<"[SISTEMA] Producto vendido. ID producto: "<< idProd << endl;
			enviarRespuesta("OK");
		}else{
			enviarRespuesta("ERROR");
		}
	}else{
		enviarRespuesta("ERROR");
	}
}
void ProtocoloServidor::atenderVerPerfil(){

	char *correoUsuario =recibirDato();

	if(correoUsuario == NULL || strlen(correoUsuario) == 0){
		enviarRespuesta("ERROR");
		return;
	}

	char resultado[512];
	memset(resultado, 0,512);
	int res =obtenerPerfilBD(this->db, correoUsuario, resultado);

	if(res==1){
		enviarRespuesta(resultado);
	}else {
		enviarRespuesta("ERROR");
	}

}










