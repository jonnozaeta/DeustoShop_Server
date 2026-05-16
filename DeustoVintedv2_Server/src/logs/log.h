/*
 * log.h
 *
 *  Created on: 14 may 2026
 *      Author: jon.ozaeta
 */

#ifndef SRC_LOGS_LOG_H_
#define SRC_LOGS_LOG_H_

void registrar_log_inicio_sesion(const char* correo);
void registrar_log_registro_usuario(const char* correo, const char* nombre);
void log_write(const char* mensaje);

#endif /* SRC_LOGS_LOG_H_ */
