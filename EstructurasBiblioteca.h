#define MAXCANTILIBROS 20
#include <stdio.h>
typedef struct fechaEstructura{
char dia[4];
char mes[4];
char anio[40];
} fecha;
typedef struct ejemplaresBaseDatos{
char status[4];
fecha fechaEjemplar;
int idEjemplar;
} datosEjemplares;

typedef struct librosBaseDatos{
char nombre[1024];
int isbn;
int cantiEjemplares;
datosEjemplares arregloEjemplares[MAXCANTILIBROS];
} librosRegistradosBD;

typedef struct reportesBaseDatos{
char status[4];
char nombre[1024];
int isbn;
int ejemplarManipulado;
fecha fechaReporte;

} reporteBD;

