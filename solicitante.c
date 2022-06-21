/*
 *Codigo, Proyecto: Páez, Castro, Traslaviña
 */
#include <math.h>

#include <unistd.h>

#include <stdlib.h>

#include <stdio.h>

#include <sys/types.h>

#include <sys/stat.h>

#include <fcntl.h>

#include <pthread.h>

#include <time.h>

#include <string.h>

#include<stdbool.h>// Booleanos

#define MAXIMA_LONGITUD_CADENA 1024
#define CANTIDAD_LINEAS 100
void envio(char * pipe, char * linea) {
  srand(time(NULL));
  int fd, n, newPipe, fdR, p;
  char direc[1024];
  strcpy(direc, pipe);
  char buf[1024], bufR[1024];
  char pipeRespuesta[1024];
  char newPipeChar[1024];
  newPipe = rand();
  sprintf(newPipeChar, "%i", newPipe);
  strcpy(pipeRespuesta, "pipeR");
  strcat(pipeRespuesta, newPipeChar);
  mkfifo(pipeRespuesta, 0666);
  //LA CREACION DEL PIPE SE DEBE HACER EN EL MAIN
  fd = open(direc, O_WRONLY);
  sleep(1);
  //strcpy(buf,linea);
  strcpy(buf, "");
  strcpy(buf, pipeRespuesta);
  strcat(buf, "-");
  strcat(buf, linea);
  write(fd, buf, sizeof(buf));
  printf("\nEnviando...-> %s\n", buf);
  fdR = open(pipeRespuesta, O_RDONLY);
  n = read(fdR, bufR, sizeof(bufR));
  printf("\nRESPUESTA :%s\n", bufR);
  close(fd);
  remove(pipeRespuesta);
}

void * menu(void * arg) {
  char * pipe = (char * ) arg;
  char opcion[50];
  printf("\nOperaciones Proceso Solicitante \n(s=salida;i=ingreso):\n");
  bool y = true;
  do {
    printf(">");
    scanf("%s", opcion);
    getchar();
    if (strlen(opcion) == 1) {
      switch (opcion[0]) {
      case 'i':
        printf("DIGITE:\n");
        bool correcto = true; //Verifica que la entrada sea correcta
        char operacion[MAXIMA_LONGITUD_CADENA], nombre[MAXIMA_LONGITUD_CADENA], ISBN[MAXIMA_LONGITUD_CADENA];
        char EJ[MAXIMA_LONGITUD_CADENA];
        do {
          printf("Operacion a Realizar:");
          fgets(operacion, MAXIMA_LONGITUD_CADENA, stdin);
          if ((strlen(operacion) > 0) && (operacion[strlen(operacion) - 1] == '\n')) {
            operacion[strlen(operacion) - 1] = '\0';
          }
          if ((strlen(operacion) == 1) && (operacion[0] == 'D' || operacion[0] == 'R' || operacion[0] == 'P')) {
            correcto = false;
          } else {
            printf("Entrada NO valida\n");
          }
        } while (correcto);
        printf("Nombre del Libro:");
        fgets(nombre, MAXIMA_LONGITUD_CADENA, stdin);
        // Remover salto de línea
        if ((strlen(nombre) > 0) && (nombre[strlen(nombre) - 1] == '\n')) {
          nombre[strlen(nombre) - 1] = '\0';
        }
        printf("ISBN:");
        scanf("%s", ISBN);
        printf("ID Ejemplar:");
        scanf("%s", EJ);
        char linea[100];
        strcpy(linea, operacion);
        strcat(linea, ",");
        strcat(linea, nombre);
        strcat(linea, ",");
        strcat(linea, ISBN);
        strcat(linea, ",");
        strcat(linea, EJ);
        printf("Solicitando:%s\n", linea);
        if (fork() == 0) {
          envio(pipe, linea);
          exit(0);
        }
        break;
      case 's':
        printf("   Finalizando Solicitante... \n");
        pthread_exit(NULL);
        y = false; //por si hay problemas saliendo de este hilo
        break;
      default:
        printf("    Digite 's' o 'i' invalido\n");
        break;
      }
    } else {
      printf("   Digite 's' o 'i'\n");
    }
  } while (y);
  pthread_exit(NULL);
}
int main(int argc, char ** argv) {
  char direcPipe[1024];
  int aux = 0;
  char palabras[CANTIDAD_LINEAS][MAXIMA_LONGITUD_CADENA];
  char buferArchivo[MAXIMA_LONGITUD_CADENA];
  FILE * archivo;
  //LO SIGUIENTE ES PARA EVALUAR LA ENTRADA DE PARAMETROS CUANDO SE EJECUTE $ ./solicitante [–i file] –p pipeReceptor 
  int bandera1 = 0, bandera2 = 0;
  for (int a = 0; a < argc; a++) {
    if (strcmp("-p", argv[a]) == 0) {
      strcpy(direcPipe, argv[a + 1]);
      bandera1 = 1;
    } else if (strcmp("-i", argv[a]) == 0) {
      archivo = fopen(argv[a + 1], "r");
      bandera2 = 1;
    }
  }
  if (bandera1 == 0) {
    printf("\n No hay un pipe para iniciar la comunicación");
    return 0;
  }
  mkfifo(direcPipe, 0666); //0666: los permisos que queremos darle.
  if (archivo == NULL) {
    printf("Sin apertura archivo o no existe\n");
  }
  pthread_t hilo_MENU;
  pthread_create( & hilo_MENU, NULL, menu, (void * ) direcPipe);
  if (bandera2 == 1) /*HAY UN ARCHIVO LECTURA*/ {
    int indice = 0;
    while (fgets(buferArchivo, MAXIMA_LONGITUD_CADENA, archivo)) {
      strtok(buferArchivo, "\n");
      memcpy(palabras[indice], buferArchivo, MAXIMA_LONGITUD_CADENA);
      int cantiCaracteres = 0;
      char auxBuf[1024];
      strcpy(auxBuf, palabras[indice]);
      cantiCaracteres = strlen(auxBuf);
      if (cantiCaracteres >= 7) {
        indice++;
        aux++;
      }
    }
    fclose(archivo);
    for (int g = 0; g < aux; g++) {
      sleep(1);
      if (fork() == 0) {
        envio(direcPipe, palabras[g]);
        exit(0);
      }
    }
    sleep(1);
    printf("TOTAL PETICIONES ARCHIVO: %i\nEsperando al receptor...\n", aux);
  }
  pthread_join(hilo_MENU, NULL);
  return 0;
}