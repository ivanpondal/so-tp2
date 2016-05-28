#ifndef Encabezado_h
#define Encabezado_h


#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <vector>
#include <iostream>
#include <list>

#define PORT 5481

#define MENSAJE_MAXIMO 1024
#define DATO_MAXIMO 100

#define VACIO 0x2D
#define BARCO 0x42
#define BOMBA 0x2A

#define MSG_EQUIPO 10
#define MSG_PARTE_BARCO 20
#define MSG_BARCO_TERMINADO 30
#define MSG_BOMBA 40
#define MSG_UPDATE 50
#define MSG_LISTO 60
#define MSG_INVALID 99


int recibir(int s, char* buf);
int enviar(int s, char* buf);


#endif