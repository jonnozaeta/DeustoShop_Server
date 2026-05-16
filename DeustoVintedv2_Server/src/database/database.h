/*
 * database.h
 *
 *  Created on: 14 may 2026
 *      Author: jon.ozaeta
 */

#ifndef SRC_DATABASE_DATABASE_H_
#define SRC_DATABASE_DATABASE_H_

#include "sqlite3.h"

int comprobarUsuarioBD(sqlite3* db, char* usuario, char* pass);
int registrarUsuarioBD(sqlite3* db, char* usuario, char* pass, char* nombre);

int insertarProductoBD(sqlite3* db, const char* nombre, const char* descripcion, const char* talla, const char* sexo, int precio, int id_usuario);
int eliminarProductoBD(sqlite3* db, int id_prod) ;
int modificarProductoBD(sqlite3* db, int id_prod, const char* nombre, const char* desc, const char* talla, const char* sexo, int precio);



#endif /* SRC_DATABASE_DATABASE_H_ */
