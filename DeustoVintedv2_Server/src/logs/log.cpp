/*
 * log.cpp
 *
 *  Created on: 14 may 2026
 *      Author: jon.ozaeta
 */


#include "log.h"
#include <fstream>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <cstring>

using namespace std;

void registrar_log_inicio_sesion(const char* correo) {
	ofstream archivo("C:\\Users\\iker.perez.dulce\\git\\DeustoShop_Server\\DeustoVintedv2_Server\\data\\log\\user_log.txt", ios::app);

    if (archivo.is_open()) {
        time_t t = time(nullptr);
        tm* tm_info = localtime(&t);

        archivo << "[" << put_time(tm_info, "%Y-%m-%d %H:%M:%S") << "] "
                << "LOGIN: El usuario con correo " << correo << " ha accedido al sistema."
                << endl;

        archivo.close();
    } else {
        cerr << "[ERROR] No se pudo abrir el log en data/logs/user_log.txt" << endl;
    }
}

void registrar_log_registro_usuario(const char* correo, const char* nombre) {
	ofstream archivo("C:\\Users\\iker.perez.dulce\\git\\DeustoShop_Server\\DeustoVintedv2_Server\\data\\log\\user_log.txt", ios::app);

    if (archivo.is_open()) {
        time_t t = time(nullptr);
        tm* tm_info = localtime(&t);

        archivo << "[" << put_time(tm_info, "%Y-%m-%d %H:%M:%S") << "] "
                << "NUEVO REGISTRO: Usuario '" << nombre << "' creado (Email: " << correo << ")."
                << endl;

        archivo.close();
    } else {
        cerr << "[ERROR] No se pudo abrir el log en data/logs/user_log.txt" << endl;
    }
}

void log_write(const char* mensaje) {
    ofstream archivo("data/log/user_log.txt", ios::app);
    if (archivo.is_open()) {
        time_t ahora = time(0);
        char* dt = ctime(&ahora);
        int longitud = strlen((const char*)dt);
        dt[longitud - 1] = '\0';

        archivo << "[" << dt << "] " << mensaje << endl;
        archivo.close();
    }
}
