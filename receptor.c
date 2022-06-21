/*
 *Codigo, Proyecto: Páez, Castro, Traslaviña
 */

#include <semaphore.h>

#include <math.h>

#include <unistd.h>

#include <stdlib.h>

#include <stdio.h>

#include <sys/types.h>

#include <sys/stat.h>

#include <fcntl.h>

#include <pthread.h>

#include <string.h>

#include <signal.h>//Kill HIjo

#include<stdbool.h>// Booleanos

#include <sys/mman.h>//MEMORIA COMPARTIDA

#include "EstructurasBiblioteca.h"

#define CANTIDAD_LINEAS 1000
#define LINEAS_BUFFER 10 //VALOR DE N
#define MAXIMA_LONGITUD_CADENA 1000
void actualizarReporte(char statusR[], char nombreR[], int isbnR, int idEjemplarR, char dia[], char mes[], char anio[]);
void imprimirReporte();
void actualizarBD();
void leerBaseDatos();
bool bucle = true;
//Base de datos estructurado
librosRegistradosBD librosRegistrados[30];

int cantiLibrosRegistrados = 0;
reporteBD reporteProcesos[20];
pthread_key_t cantiReportes;
//Estructura reportes

FILE * archivoBD, * buffer, * Reporte;

char NOMBRE_BD[1024];
char NOMBRE_BUF[1024] = "Buffer.txt";
char NOMBRE_REP[1024] = "Reporte.txt";
char archivoSalida[1024] = "archivoSalida.txt";
//char bufferPeti[LINEAS_BUFFER][MAXIMA_LONGITUD_CADENA];
sem_t semExcmutEscritura, semExcmutLectura;
/*void *manipularDatos()
{
  bool sigue=true;
  librosRegistradosBD librosRegistradosH[20];
  int j=0, value;
  sleep(10);
  char vacio[]="NULL";
  char aux[1024];
  //sem_getvalue(&semLlenas,&value);
  sem_wait(&semExcmut);
  //sem_wait(&semLlenas);
  //sem_post(&semVacias);
  while(sigue)
    {
    sleep(5);
      leerBaseDatosHilo(librosRegistradosH);
      char bufferPeti[LINEAS_BUFFER][1024];
        char bufferArchivo[1024];
        int indice = 0;
        int auxContarCaracteres = 0;
        int auxContarLineas = 0;
        buffer = fopen(NOMBRE_BUF, "r");
        char buf[1024];
        printf("\nTODO BIEN\n");
        while (fgets(bufferArchivo, 1024, buffer)) {
          strtok(bufferArchivo, "\n");
          memcpy(bufferPeti[indice], bufferArchivo, 1024);
          strcpy(aux, bufferPeti[indice]);
          auxContarCaracteres = strlen(aux);
          if (auxContarCaracteres >= 5) {
            indice++;
            auxContarLineas++;
          }
        }
        sem_post(&semExcmut);
  for(int i=0;i<LINEAS_BUFFER;i++)
      {
    printf("BUFFER: %s\n",bufferPeti[i]);
    strcpy(aux,bufferPeti[i]);
    char *token2 = strtok(aux,",");
    if(strcmp(token2,"R")==0){

        for (int i = 0; i < cantiLibrosRegistrados; i++) { //Buscar libro
  if (strcmp(token2, librosRegistradosH[i].nombre) == 0) {
   //Si el libro existe
    token2 = strtok(NULL, ","); //ISBN
    if (librosRegistrados[i].isbn == atoi(token2)) {// Si el ISBN es correcto
      token2 = strtok(NULL, ","); //IdEjemplar
      for(int j = 0; j < librosRegistradosH[i].cantiEjemplares; j++) { //Buscar en los ejemplares cual esta disponible
        if (librosRegistradosH[i].arregloEjemplares[j].idEjemplar == atoi(token2)) { //Buscar si el ejemplar existe
        token2 = strtok(NULL, ","); //IdEjemplar
        printf("Nueva Fecha: %s",token2);
        token2 = strtok(NULL, "-");
          strcpy(librosRegistradosH[i].arregloEjemplares[j].fechaEjemplar.dia,token2);
          token2 = strtok(NULL, "-");
          strcpy(librosRegistradosH[i].arregloEjemplares[j].fechaEjemplar.mes,token2);
          token2 = strtok(NULL, "-");
          strcpy(librosRegistradosH[i].arregloEjemplares[j].fechaEjemplar.anio,token2);
          actualizarBDHilo(librosRegistradosH);
        }
      }
    }
  }
}
 }
    if(strcmp(token2,"D")==0){
        for (int i = 0; i < cantiLibrosRegistrados; i++) { //Buscar libro
  if (strcmp(token2, librosRegistradosH[i].nombre) == 0) {
   //Si el libro existe
    token2 = strtok(NULL, ","); //ISBN
    if (librosRegistradosH[i].isbn == atoi(token2)) {// Si el ISBN es correcto
      token2 = strtok(NULL, ","); //IdEjemplar
      for (int j = 0; j < librosRegistradosH[i].cantiEjemplares; j++) { //Buscar en los ejemplares cual esta disponible
        if (librosRegistradosH[i].arregloEjemplares[j].idEjemplar == atoi(token2)) { //Buscar si el ejemplar existe
          strcpy(librosRegistradosH[i].arregloEjemplares[j].status,"D");
          actualizarBDHilo(librosRegistradosH);
        }
      }
    }
  }
}
 }
 }
    //if(j==10){
     // sigue=false;
    //}
    //j++;
  }

}
*/
void llenarBuffer(char linea[]) {

  buffer = fopen(NOMBRE_BUF, "a");
  strcat(linea, "\n");
  fputs(linea, buffer);
  printf("\n\nLinea: %s\n\n", linea);
  fclose(buffer);
}

void actualizarReporte(char statusR[], char nombreR[], int isbnR, int idEjemplarR, char dia[], char mes[], char anio[]) {
  Reporte = fopen(NOMBRE_REP, "a");
  char auxEscribir[1024], aux2[1024];

  strcat(auxEscribir, statusR); //STATUS
  strcat(auxEscribir, ","); //STATUS,
  strcat(auxEscribir, nombreR); //STATUS,NOMBRELIBRO
  strcat(auxEscribir, ","); //STATUS,NOMBRELIBRO,
  sprintf(aux2, "%d", isbnR);
  strcat(auxEscribir, aux2); //STATUS,NOMBRELIBRO,ISBN
  strcat(auxEscribir, ","); //STATUS,NOMBRELIBRO,ISBN,
  sprintf(aux2, "%d", idEjemplarR);
  strcat(auxEscribir, aux2); //STATUS,NOMBRELIBRO,ISBN,IDEJEMPLAR
  strcat(auxEscribir, ","); //STATUS,NOMBRELIBRO,ISBN,IDEJEMPLAR,
  strcat(auxEscribir, dia); //STATUS,NOMBRELIBRO,ISBN,IDEJEMPLAR,dia
  strcat(auxEscribir, "-"); //STATUS,NOMBRELIBRO,ISBN,IDEJEMPLAR,
  strcat(auxEscribir, mes); //STATUS,NOMBRELIBRO,ISBN,IDEJEMPLAR,dia-mes
  strcat(auxEscribir, "-"); //STATUS,NOMBRELIBRO,ISBN,IDEJEMPLAR,
  strcat(auxEscribir, anio); //STATUS,NOMBRELIBRO,ISBN,IDEJEMPLAR,dia-mes
  strcat(auxEscribir, "\n");

  fputs(auxEscribir, Reporte);
  fclose(Reporte);
}
void imprimirReporte() {
  char palabras[CANTIDAD_LINEAS][MAXIMA_LONGITUD_CADENA];
  char buferArchivo[MAXIMA_LONGITUD_CADENA];
  int indice = 0;
  int auxContarCaracteres = 0;
  int auxContarLineas = 0;
  char auxBuf[1024];
  Reporte = fopen(NOMBRE_REP, "r");
  char buf[1024];
  printf("Entra imprimir\n");
  while (fgets(buferArchivo, MAXIMA_LONGITUD_CADENA, Reporte)) {
    strtok(buferArchivo, "\n");
    memcpy(palabras[indice], buferArchivo, MAXIMA_LONGITUD_CADENA);
    strcpy(auxBuf, palabras[indice]);
    auxContarCaracteres = strlen(auxBuf);
    indice++;
    auxContarLineas++;
  }

  printf("Contar lineas: %d\n", auxContarLineas);
  for (int i = 0; i < auxContarLineas; i++) {
    printf("%s\n", palabras[i]);
  }
  fclose(Reporte);
}
void enviarConfirmacion(char solicitudPipe[]) {

  sem_wait( & semExcmutLectura);
  leerBaseDatos(); //Leyendo el archivo de salida
  sem_post( & semExcmutLectura);

  char auxSeparar[1024], solicitud[1024], pipeRespuesta[1024];
  strcpy(auxSeparar, solicitudPipe);
  char * token = strtok(auxSeparar, "-");
  strcpy(pipeRespuesta, token); //Dividir pipe
  char auxMemoria[1024];
  token = strtok(NULL, "-"); //Separa solicitud
  strcpy(solicitud, token);
  strcpy(auxMemoria, solicitud);

  printf("PipeRespuesta%s\n", pipeRespuesta);
  int fdR;
  fdR = open(pipeRespuesta, O_WRONLY);
  char mensaje[1024] = "";
  strcpy(auxSeparar, solicitud);
  char * token2 = strtok(solicitud, ",");

  //INICIO EVALUAR PRESTADO
  //-------------------------------------------------------------
  if (strcmp(token2, "P") == 0) {
    int libroEncontrado = 0;
    int ejemplarEncontrado = 0;
    int ejemplarDisponible = 0;
    int isbnCorrecto = 0;

    token2 = strtok(NULL, ","); //Sacar nombre libro
    for (int i = 0; i < cantiLibrosRegistrados; i++) { //Buscar libro
      if (strcmp(token2, librosRegistrados[i].nombre) == 0) {
        //Si el libro existe
        libroEncontrado = 1;
        token2 = strtok(NULL, ","); //ISBN
        if (librosRegistrados[i].isbn == atoi(token2)) { // Si el ISBN es correcto
          isbnCorrecto = 1;
          token2 = strtok(NULL, ","); //IdEjemplar
          for (int j = 0; j < librosRegistrados[i].cantiEjemplares; j++) { //Buscar en los ejemplares cual esta disponible
            if (librosRegistrados[i].arregloEjemplares[j].idEjemplar == atoi(token2)) { //Buscar si el ejemplar existe
              ejemplarEncontrado = 1;
              if (strcmp(librosRegistrados[i].arregloEjemplares[j].status, "D") == 0) { //Ver si ese ejemplar esta disponible para prestar
                ejemplarDisponible = 1;
                sem_wait( & semExcmutLectura);

                strcpy(librosRegistrados[i].arregloEjemplares[j].status, "P");

                strcpy(mensaje, "Libro registrado como prestado");

                actualizarReporte(librosRegistrados[i].arregloEjemplares[j].status,
                  librosRegistrados[i].nombre,
                  librosRegistrados[i].isbn,
                  librosRegistrados[i].arregloEjemplares[j].idEjemplar,
                  librosRegistrados[i].arregloEjemplares[j].fechaEjemplar.dia,
                  librosRegistrados[i].arregloEjemplares[j].fechaEjemplar.mes,
                  librosRegistrados[i].arregloEjemplares[j].fechaEjemplar.anio
                );
                sem_post( & semExcmutLectura);

              }
            }
          }
        }
      }
    }

    if (libroEncontrado == 0) {
      strcpy(mensaje, "Libro no encontrado");
    } else if (isbnCorrecto == 0) {
      strcpy(mensaje, "ISBN incorrecto");
    } else if (ejemplarEncontrado == 0) {
      strcpy(mensaje, "Ejemplar no encontrado");
    } else if (ejemplarDisponible == 0) {
      strcpy(mensaje, "Ejemplar no disponible");

    } //-------------------------------------------------------------

  } //FIN EVALUAR PRESTADO

  //INICIO EVALUAR DEVOLVER
  //-------------------------------------------------------------
  if (strcmp(token2, "D") == 0) {
    int libroEncontrado = 0;
    int ejemplarEncontrado = 0;
    int ejemplarDisponible = 0;
    int isbnCorrecto = 0;

    token2 = strtok(NULL, ","); //Sacar nombre libro
    for (int i = 0; i < cantiLibrosRegistrados; i++) { //Buscar libro
      if (strcmp(token2, librosRegistrados[i].nombre) == 0) {
        //Si el libro existe
        libroEncontrado = 1;
        token2 = strtok(NULL, ","); //ISBN
        if (librosRegistrados[i].isbn == atoi(token2)) { // Si el ISBN es correcto
          isbnCorrecto = 1;
          token2 = strtok(NULL, ","); //IdEjemplar
          for (int j = 0; j < librosRegistrados[i].cantiEjemplares; j++) { //Buscar en los ejemplares cual esta disponible
            if (librosRegistrados[i].arregloEjemplares[j].idEjemplar == atoi(token2)) { //Buscar si el ejemplar existe
              ejemplarEncontrado = 1;

              if (strcmp(librosRegistrados[i].arregloEjemplares[j].status, "P") == 0) { //Ver si ese ejemplar esta prestado para luego devolverlo
                ejemplarDisponible = 1;
                //EL cambio lo hace el hilo
                sem_wait( & semExcmutLectura);
                strcpy(mensaje, "Libro registrado como devuelto");
                strcpy(librosRegistrados[i].arregloEjemplares[j].status, "D");
                actualizarReporte(librosRegistrados[i].arregloEjemplares[j].status,
                  librosRegistrados[i].nombre,
                  librosRegistrados[i].isbn,
                  librosRegistrados[i].arregloEjemplares[j].idEjemplar,
                  librosRegistrados[i].arregloEjemplares[j].fechaEjemplar.dia,
                  librosRegistrados[i].arregloEjemplares[j].fechaEjemplar.mes,
                  librosRegistrados[i].arregloEjemplares[j].fechaEjemplar.anio
                );
                sem_post( & semExcmutLectura);

              }
            }
          }
        }
      }
    }

    if (libroEncontrado == 0) {
      strcpy(mensaje, "Libro no encontrado");
    } else if (isbnCorrecto == 0) {
      strcpy(mensaje, "ISBN incorrecto");
    } else if (ejemplarEncontrado == 0) {
      strcpy(mensaje, "Ejemplar no encontrado");
    } else if (ejemplarDisponible == 0) {
      strcpy(mensaje, "Ejemplar no disponible");

    }
  }

  //-------------------------------------------------------------
  //FIN EVALUAR DEVUELTO

  if (strcmp(token2, "R") == 0) {
    int libroEncontrado = 0;
    int ejemplarEncontrado = 0;
    int ejemplarDisponible = 0;
    int isbnCorrecto = 0;
    int ejemplarPrestado = 0;
    int auxFecha = 0;
    char auxDia[1024];
    char auxMes[1024];
    char auxAnio[1024];
    token2 = strtok(NULL, ","); //Sacar nombre libro
    for (int i = 0; i < cantiLibrosRegistrados; i++) { //Buscar libro
      if (strcmp(token2, librosRegistrados[i].nombre) == 0) {
        //Si el libro existe
        libroEncontrado = 1;
        token2 = strtok(NULL, ","); //ISBN
        if (librosRegistrados[i].isbn == atoi(token2)) { // Si el ISBN es correcto
          isbnCorrecto = 1;
          token2 = strtok(NULL, ","); //IdEjemplar
          for (int j = 0; j < librosRegistrados[i].cantiEjemplares; j++) { //Buscar en los ejemplares primero que exista y luego que este disponible
            if (librosRegistrados[i].arregloEjemplares[j].idEjemplar == atoi(token2)) { //Buscar si el ejemplar existe
              ejemplarEncontrado = 1;
              if (strcmp(librosRegistrados[i].arregloEjemplares[j].status, "P") == 0) { //Verificar que este prestado para poder realizar la renovacion
                ejemplarPrestado = 1;
                sem_wait( & semExcmutLectura);
                if (atoi(librosRegistrados[i].arregloEjemplares[j].fechaEjemplar.dia) + 7 > 30) { //Si se pasa del mes

                  if (atoi(librosRegistrados[i].arregloEjemplares[j].fechaEjemplar.mes) + 1 > 12) { //Si se pasa de los 12 meses

                    auxFecha = (atoi(librosRegistrados[i].arregloEjemplares[j].fechaEjemplar.dia) + 7) % 30;
                    sprintf(auxDia, "0%d", auxFecha);
                    strcpy(auxMes, "01");
                    auxFecha = atoi(librosRegistrados[i].arregloEjemplares[j].fechaEjemplar.anio) + 1;
                    sprintf(auxAnio, "%d", auxFecha);

                    strcpy(librosRegistrados[i].arregloEjemplares[j].fechaEjemplar.dia, auxDia);
                    strcpy(librosRegistrados[i].arregloEjemplares[j].fechaEjemplar.mes, auxMes);
                    strcpy(librosRegistrados[i].arregloEjemplares[j].fechaEjemplar.anio, auxAnio);

                  } else { //Si continua en el mismo año
                    auxFecha = (atoi(librosRegistrados[i].arregloEjemplares[j].fechaEjemplar.dia) + 7) % 30;
                    sprintf(auxDia, "0%d", auxFecha);
                    auxFecha = atoi(librosRegistrados[i].arregloEjemplares[j].fechaEjemplar.mes) + 1;
                    if (auxFecha <= 9) {
                      sprintf(auxMes, "0%d", auxFecha);
                    } else {
                      sprintf(auxMes, "%d", auxFecha);
                    }
                    strcpy(librosRegistrados[i].arregloEjemplares[j].fechaEjemplar.dia, auxDia);
                    strcpy(librosRegistrados[i].arregloEjemplares[j].fechaEjemplar.mes, auxMes);
                  }
                } else { //Si no se pasa del mes

                  auxFecha = atoi(librosRegistrados[i].arregloEjemplares[j].fechaEjemplar.dia) + 7;
                  if (auxFecha <= 9) {
                    sprintf(auxDia, "0%d", auxFecha);
                  } else {
                    sprintf(auxDia, "%d", auxFecha);
                  }
                  strcpy(librosRegistrados[i].arregloEjemplares[j].fechaEjemplar.dia, auxDia);

                }
                strcpy(auxDia, librosRegistrados[i].arregloEjemplares[j].fechaEjemplar.dia);
                strcpy(auxMes, librosRegistrados[i].arregloEjemplares[j].fechaEjemplar.mes);
                strcpy(auxAnio, librosRegistrados[i].arregloEjemplares[j].fechaEjemplar.anio);

                //EL cambio lo hace el hilo

                strcpy(mensaje, "La nueva fecha para devolver el libro es; ");
                strcat(mensaje, auxDia);
                strcat(mensaje, "-");
                strcat(mensaje, auxMes);
                strcat(mensaje, "-");
                strcat(mensaje, auxAnio);
                //Mensaje con fecha actualizada
                //Parametro para el hilo
                actualizarReporte(librosRegistrados[i].arregloEjemplares[j].status,
                  librosRegistrados[i].nombre,
                  librosRegistrados[i].isbn,
                  librosRegistrados[i].arregloEjemplares[j].idEjemplar,
                  librosRegistrados[i].arregloEjemplares[j].fechaEjemplar.dia,
                  librosRegistrados[i].arregloEjemplares[j].fechaEjemplar.mes,
                  librosRegistrados[i].arregloEjemplares[j].fechaEjemplar.anio
                );
                sem_post( & semExcmutLectura);
              }
            }
          }
        }
      }
    }
    if (libroEncontrado == 0) {
      strcpy(mensaje, "Libro no encontrado");
    } else if (isbnCorrecto == 0) {
      strcpy(mensaje, "ISBN incorrecto");
    } else if (ejemplarEncontrado == 0) {
      strcpy(mensaje, "Ejemplar no encontrado");
    } else if (ejemplarPrestado == 0) {
      strcpy(mensaje, "El ejemplar esta disponible, por tanto no se puede renovar");
      //-------------------------------------------------------------
      //FIN EVALUAR RENOVADO
    }
  }

  //Separa
  //D,R o P

  actualizarBD();
  write(fdR, mensaje, sizeof(mensaje));
  close(fdR);

}

void actualizarBD() {
  archivoBD = fopen(archivoSalida, "w");
  char auxActualizar[1024];
  char auxIntChar[1024];
  int k = 0;
  for (int i = 0; i < cantiLibrosRegistrados; i++) {
    strcpy(auxActualizar, librosRegistrados[i].nombre); //Nombre
    strcat(auxActualizar, ","); //Nombre,
    sprintf(auxIntChar, "%d", librosRegistrados[i].isbn);
    strcat(auxActualizar, auxIntChar); //Nombre-ISBN
    strcat(auxActualizar, ","); //Nombre,ISBN,
    sprintf(auxIntChar, "%d", librosRegistrados[i].cantiEjemplares);
    strcat(auxActualizar, auxIntChar); //Nombre-ISBN,cantiEjemplares
    strcat(auxActualizar, "\n");
    fputs(auxActualizar, archivoBD);

    for (int j = 0; j < librosRegistrados[i].cantiEjemplares; j++) { //Por cada ejemplar

      sprintf(auxIntChar, "%d", librosRegistrados[i].arregloEjemplares[j].idEjemplar);
      strcpy(auxActualizar, auxIntChar); //IdEjemplar
      strcat(auxActualizar, ","); //IdEjemplar,
      strcat(auxActualizar, librosRegistrados[i].arregloEjemplares[j].status); //IdEjemplar,status
      strcat(auxActualizar, ","); //IdEjemplar,status,
      strcat(auxActualizar, librosRegistrados[i].arregloEjemplares[j].fechaEjemplar.dia); //IdEjemplar,status,dia
      strcat(auxActualizar, "-"); //IdEjemplar,status,dia-
      strcat(auxActualizar, librosRegistrados[i].arregloEjemplares[j].fechaEjemplar.mes); //IdEjemplar,status,dia-mes
      strcat(auxActualizar, "-"); //IdEjemplar,status,dia-mes-
      strcat(auxActualizar, librosRegistrados[i].arregloEjemplares[j].fechaEjemplar.anio); //IdEjemplar,status,dia-mes-año
      strcat(auxActualizar, "\n");
      fputs(auxActualizar, archivoBD);
    }
  }
  fclose(archivoBD);

}
void leerBaseDatos() {
  //Leer BD para convertirlo en estructura
  char palabras[CANTIDAD_LINEAS][MAXIMA_LONGITUD_CADENA];
  char buferArchivo[MAXIMA_LONGITUD_CADENA];
  int indice = 0;
  int auxContarCaracteres = 0;
  int auxContarLineas = 0;
  char auxBuf[1024];
  int bandera1 = 0, bandera2 = 0;
  archivoBD = fopen(archivoSalida, "r");
  char buf[1024];
  while (fgets(buferArchivo, MAXIMA_LONGITUD_CADENA, archivoBD)) {
    strtok(buferArchivo, "\n");
    memcpy(palabras[indice], buferArchivo, MAXIMA_LONGITUD_CADENA);
    strcpy(auxBuf, palabras[indice]);
    auxContarCaracteres = strlen(auxBuf);
    if (auxContarCaracteres >= 7) {
      indice++;
      auxContarLineas++;
    }
  }
  //Archivo ya guardado en la estructura palabras[]
  //printf("Lineas:%d\n", auxContarLineas);
  char auxSeparador[1024];
  char * auxLineas[1024];
  int indiceLibros = 0;
  int indiceEjemplares = 0;
  for (int j = 0; j < auxContarLineas; j++) {

    strcpy(buf, palabras[j]);
    strcpy(auxSeparador, buf);
    int k = 1;
    char * separadorSaltoLinea = strtok(auxSeparador, "\n");
    //Separado por lineas
    char * tmp;
    int num = 0;
    char * str = malloc(strlen(separadorSaltoLinea) + 1);
    char args[100][100];
    //Separaciòn de caracteres con memoria dinamica
    //Tomado de: https://poesiabinaria.net/2010/12/separar-palabras-de-una-cadena-de-caracteres-en-un-array-c/
    strcpy(str, separadorSaltoLinea);
    tmp = strtok(str, ",");
    do {
      strcpy(args[num], tmp);
      num++;
      tmp = strtok(NULL, ",");
    } while (tmp != NULL);

    if (atoi(args[1]) != 0) { //Verificar si es un libro
      strcpy(librosRegistrados[indiceLibros].nombre, args[0]);

      librosRegistrados[indiceLibros].isbn = atoi(args[1]);
      librosRegistrados[indiceLibros].cantiEjemplares = atoi(args[2]);
    } else if (atoi(args[1]) == 0) { //Quiere decir que es un ejemplar
      librosRegistrados[indiceLibros].arregloEjemplares[indiceEjemplares].idEjemplar = atoi(args[0]);
      strcpy(librosRegistrados[indiceLibros].arregloEjemplares[indiceEjemplares].status, args[1]);
      char auxFecha[1024];
      strcpy(auxFecha, args[2]);
      char * token = "";
      token = strtok(auxFecha, "-");
      strcpy(librosRegistrados[indiceLibros].arregloEjemplares[indiceEjemplares].fechaEjemplar.dia, token);
      token = strtok(NULL, "-");
      strcpy(librosRegistrados[indiceLibros].arregloEjemplares[indiceEjemplares].fechaEjemplar.mes, token);
      token = strtok(NULL, "-");
      strcpy(librosRegistrados[indiceLibros].arregloEjemplares[indiceEjemplares].fechaEjemplar.anio, token);
      indiceEjemplares++;
      if (librosRegistrados[indiceLibros].cantiEjemplares == indiceEjemplares) {
        indiceLibros++;
        indiceEjemplares = 0;
      }
    }
  }
  //Confirmar que todo este bien
  /* for(int i=0;i<cantiLibrosRegistrados;i++){ 
   printf("Nombre:%s\n",librosRegistrados[i].nombre);
   printf("ISBN:%d\n",librosRegistrados[i].isbn);
   printf("Cantidad de ejemplares %d\n",librosRegistrados[i].cantiEjemplares);
   for(int j=0;j<librosRegistrados[i].cantiEjemplares;j++){ 
   printf("   ID:%d\n",librosRegistrados[i].arregloEjemplares[j].idEjemplar);
   printf("   Status:%s\n",librosRegistrados[i].arregloEjemplares[j].status);
   printf("   Fecha:\n");
   printf("       Dia:%s\n",librosRegistrados[i].arregloEjemplares[j].fechaEjemplar.dia);
   printf("       Mes:%s\n",librosRegistrados[i].arregloEjemplares[j].fechaEjemplar.mes);
   printf("       Anio:%s\n",librosRegistrados[i].arregloEjemplares[j].fechaEjemplar.anio);
   }
   }*/
}
void leerBaseDatosInicial() {
  //Leer BD para convertirlo en estructura
  char palabras[CANTIDAD_LINEAS][MAXIMA_LONGITUD_CADENA];
  char buferArchivo[MAXIMA_LONGITUD_CADENA];
  int indice = 0;
  int auxContarCaracteres = 0;
  int auxContarLineas = 0;
  char auxBuf[1024];
  int bandera1 = 0, bandera2 = 0;
  archivoBD = fopen(NOMBRE_BD, "r");
  char buf[1024];
  while (fgets(buferArchivo, MAXIMA_LONGITUD_CADENA, archivoBD)) {
    strtok(buferArchivo, "\n");
    memcpy(palabras[indice], buferArchivo, MAXIMA_LONGITUD_CADENA);
    strcpy(auxBuf, palabras[indice]);
    auxContarCaracteres = strlen(auxBuf);
    if (auxContarCaracteres >= 7) {
      indice++;
      auxContarLineas++;
    }
  }

  //Archivo ya guardado en la estructura palabras[]
  printf("Lineas:%d\n", auxContarLineas);
  char auxSeparador[1024];
  char * auxLineas[1024];
  int indiceLibros = 0;
  int indiceEjemplares = 0;
  for (int j = 0; j < auxContarLineas; j++) {
    strcpy(buf, palabras[j]);
    strcpy(auxSeparador, buf);
    int k = 1;
    char * separadorSaltoLinea = strtok(auxSeparador, "\n");
    //Separado por lineas
    char * tmp;
    int num = 0;
    char * str = malloc(strlen(separadorSaltoLinea) + 1);
    char args[100][100];
    //Separaciòn de caracteres con memoria dinamica
    //Tomado de: https://poesiabinaria.net/2010/12/separar-palabras-de-una-cadena-de-caracteres-en-un-array-c/
    strcpy(str, separadorSaltoLinea);
    tmp = strtok(str, ",");
    do {
      strcpy(args[num], tmp);
      num++;
      tmp = strtok(NULL, ",");
    } while (tmp != NULL);

    if (atoi(args[1]) != 0) { //Verificar si es un libro
      strcpy(librosRegistrados[indiceLibros].nombre, args[0]);

      librosRegistrados[indiceLibros].isbn = atoi(args[1]);
      librosRegistrados[indiceLibros].cantiEjemplares = atoi(args[2]);
      cantiLibrosRegistrados++;
    } else if (atoi(args[1]) == 0) { //Quiere decir que es un ejemplar
      librosRegistrados[indiceLibros].arregloEjemplares[indiceEjemplares].idEjemplar = atoi(args[0]);
      strcpy(librosRegistrados[indiceLibros].arregloEjemplares[indiceEjemplares].status, args[1]);
      char auxFecha[1024];
      strcpy(auxFecha, args[2]);
      char * token = "";
      token = strtok(auxFecha, "-");
      strcpy(librosRegistrados[indiceLibros].arregloEjemplares[indiceEjemplares].fechaEjemplar.dia, token);
      token = strtok(NULL, "-");
      strcpy(librosRegistrados[indiceLibros].arregloEjemplares[indiceEjemplares].fechaEjemplar.mes, token);
      token = strtok(NULL, "-");
      strcpy(librosRegistrados[indiceLibros].arregloEjemplares[indiceEjemplares].fechaEjemplar.anio, token);
      indiceEjemplares++;
      if (librosRegistrados[indiceLibros].cantiEjemplares == indiceEjemplares) {
        indiceLibros++;
        indiceEjemplares = 0;
      }
    }
  }
  //Confirmar que todo este bien
  /* for(int i=0;i<cantiLibrosRegistrados;i++){ 
   printf("Nombre:%s\n",librosRegistrados[i].nombre);
   printf("ISBN:%d\n",librosRegistrados[i].isbn);
   printf("Cantidad de ejemplares %d\n",librosRegistrados[i].cantiEjemplares);
   for(int j=0;j<librosRegistrados[i].cantiEjemplares;j++){ 
   printf("   ID:%d\n",librosRegistrados[i].arregloEjemplares[j].idEjemplar);
   printf("   Status:%s\n",librosRegistrados[i].arregloEjemplares[j].status);
   printf("   Fecha:\n");
   printf("       Dia:%s\n",librosRegistrados[i].arregloEjemplares[j].fechaEjemplar.dia);
   printf("       Mes:%s\n",librosRegistrados[i].arregloEjemplares[j].fechaEjemplar.mes);
   printf("       Anio:%s\n",librosRegistrados[i].arregloEjemplares[j].fechaEjemplar.anio);
   }
   }*/

}
void * menu() {
  char opcion[100];
  printf("\nCentro de Comandos Proceso Receptor:\n");
  bool y = true;
  do {
    printf(">");
    scanf("%s", opcion);
    getchar();
    if (strlen(opcion) == 1) {
      switch (opcion[0]) {
      case 'r':
        imprimirReporte();
        break;
      case 's':
        printf("   Finalizando... \n");
        pthread_exit(NULL);
        remove(NOMBRE_BUF);
        y = false; //por si hay problemas saliendo de este hilo
        break;
      default:
        printf("   Comando invalido\n");
        break;
      }
    } else {
      printf("   Comando invalido\n");
    }
  } while (y);
  pthread_exit(NULL);
}
int main(int argc, char ** argv) {
  sem_init( & semExcmutLectura, 0, 1);

  pthread_t hilo_auxiliar1, hilo_auxiliar2;
  pthread_create( & hilo_auxiliar2, NULL, menu, NULL);
  //pthread_create (&hilo_auxiliar1, NULL, manipularDatos, NULL);

  char direcPipe[1024];

  FILE * salida;
  remove(NOMBRE_REP);
  int bandera1 = 0, bandera2 = 0, bandera3 = 0;
  for (int a = 0; a < argc; a++) {
    if (strcmp("-p", argv[a]) == 0) {
      strcpy(direcPipe, argv[a + 1]);
      bandera1 = 1;
    } else if (strcmp("-f", argv[a]) == 0) {
      strcpy(NOMBRE_BD, argv[a + 1]); /*baseDatos = fopen(argv[a+1],"r");*/
      bandera2 = 1;
    } else if (strcmp("-s", argv[a]) == 0) {
      strcpy(archivoSalida, argv[a + 1]); /*salida = fopen(argv[a + 1], "w");*/
      bandera3 = 1;
    }
  }
  if (bandera1 == 0) {

    printf("\nNo hay un pipe para iniciar la comunicación\n");
    return 0;
  }
  if (bandera2 == 0) {
    printf("No hay un archivo de entrada para la base de datos");
    return 0;
  }
  if (bandera3 == 0) {
    salida = fopen(archivoSalida, "w");
  }

  if (salida == NULL) {
    printf("Error al abrir el archivo de salida");
    return 0;
  }
  leerBaseDatosInicial();
  actualizarBD(); //Archivo de salida ya existe
  mkfifo(direcPipe, 0666);
  int fd, n, p;
  int pid = fork();

  fd = open(direcPipe, O_RDONLY);

  if (pid != 0) {
    pthread_join(hilo_auxiliar2, NULL);
    //llenar Buffer con mensaje específico para finalizar el bucle del HiloManipulador
    //pthread_join(hilo_auxiliar1, NULL);
    //exit(1);
    remove(direcPipe);

    bucle = false;
    kill(pid, SIGKILL);
    return 0;
  } else {

    char buf[1024];
    char pipeR[1024];

    while (bucle) {

      strcpy(pipeR, "");
      strcpy(buf, "");

      n = read(fd, buf, sizeof(buf));

      if (n == -1) {
        perror("Error de lectura");
        exit(1);
      }
      if (n == 0) {
        //break;
      }

      if (buf[0] == 'D' || buf[0] == 'P' || buf[0] == 'R' || buf[0] == 'p') {
        printf("\n RECIBIDO: %s\n", buf);

        enviarConfirmacion(buf);

      }

      strcpy(buf, "correcto");

    }
    close(fd);
  }
  printf("\nFIN DEL PROGRAMA\n(s/r)>");

}