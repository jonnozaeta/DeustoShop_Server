/*
 * database.cpp
 *
 *  Created on: 14 may 2026
 *      Author: jon.ozaeta
 */

#include "sqlite3.h"
#include <iostream>
#include <string>

using namespace std;

int comprobarUsuarioBD(sqlite3* db, char* usuario, char* pass) {							//Posiblemente Mal
    sqlite3_stmt* stmt;
    const char* sql = "SELECT * FROM USUARIO WHERE CORREO = ? AND PASSWORD = ?;";
    int resultado = 0;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, usuario, -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 2, pass, -1, SQLITE_STATIC);

        if (sqlite3_step(stmt) == SQLITE_ROW) {
            resultado = 1;
        }
    }
    sqlite3_finalize(stmt);
    return resultado;
}

int registrarUsuarioBD(sqlite3* db, char* usuario, char* pass, char* nombre) {				//Mal
    sqlite3_stmt* stmt;
    const char* sql = "INSERT INTO Usuario (correo, password, nombre_us, valoracion_media) VALUES (?, ?, ?, ?);";
    int resultado = 0;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK) {

        sqlite3_bind_text(stmt, 1, usuario, -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 2, pass, -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 3, nombre, -1, SQLITE_TRANSIENT);
        sqlite3_bind_int(stmt, 4, 0);

        if (sqlite3_step(stmt) == SQLITE_DONE) {
            resultado = 1;
            cout << "[SQLITE] Usuario " << nombre << " registrado con exito." << endl;
        } else {
            cout << "[SQLITE] Error al insertar: " << sqlite3_errmsg(db) << endl;
        }
    } else {
        cout << "[SQLITE] Error en Prepare: " << sqlite3_errmsg(db) << endl;
    }

    sqlite3_finalize(stmt);
    return resultado;
}
