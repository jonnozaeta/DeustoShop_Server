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

int comprobarUsuarioBD(sqlite3* db, char* usuario, char* pass) {
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

int registrarUsuarioBD(sqlite3* db, char* usuario, char* pass, char* nombre) {
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

int insertarProductoBD(sqlite3* db, const char* nombre, const char* descripcion, const char* talla, const char* sexo, int precio, int id_usuario) {
    sqlite3_stmt* stmt;
    const char* sql = "INSERT INTO Productos (nombre_pro, [desc], talla, sexo, precio, id_usuario) VALUES (?, ?, ?, ?, ?, ?);";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) return 0;

    sqlite3_bind_text(stmt, 1, nombre, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, descripcion, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, talla, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 4, sexo, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 5, precio);
    sqlite3_bind_int(stmt, 6, id_usuario);

    int rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    return (rc == SQLITE_DONE) ? 1 : 0;
}

int eliminarProductoBD(sqlite3* db, int id_prod) {
    sqlite3_stmt* stmt;
    const char* sql = "DELETE FROM Productos WHERE id_prod = ?;";

    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) return 0;

    sqlite3_bind_int(stmt, 1, id_prod);

    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    return (rc == SQLITE_DONE) ? 1 : 0;
}

int modificarProductoBD(sqlite3* db, int id_prod, const char* nombre, const char* desc, const char* talla, const char* sexo, int precio) {
    sqlite3_stmt* stmt;
    const char* sql = "UPDATE Productos SET nombre_pro = ?, [desc] = ?, talla = ?, sexo = ?, precio = ? WHERE id_prod = ?;";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) return 0;

    sqlite3_bind_text(stmt, 1, nombre, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, desc, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, talla, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 4, sexo, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 5, precio);
    sqlite3_bind_int(stmt, 6, id_prod);

    int rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    return (rc == SQLITE_DONE) ? 1 : 0;
}
