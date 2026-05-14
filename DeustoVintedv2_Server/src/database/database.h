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

#endif /* SRC_DATABASE_DATABASE_H_ */
