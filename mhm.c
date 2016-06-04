#include <stdio.h>
#include "getnum.h"
#include <stdlib.h>
#include <math.h>
#include "time.h"

#define ERROR_MANCHA 100
#define ERROR_TABLERO 101
#define ERROR_DISTANCIA 102
#define ERROR_ORIGEN 103
#define BLOQUE 5
#define ERROR 0
#define BIEN 1
#define EXIT 2

typedef struct 
{
	int fil,col;
} tipoPosicion;
typedef struct 
{
	tipoPosicion origen,destino;
} tipoMovimiento;
typedef struct 
{
	char id;
	int cantidadBlobs;
} tipoJugador;
typedef struct
{
	char** matriz;
	int filas,columnas;
} tipoTablero;
typedef tipoJugador tipoJugadores[2];

#define FUERA_TABLERO(x,y,fil,col) (x<0 || x>fil || y<0 || y>col)
#define MAX(x,y) (x > y? x : y)
#define ELEGIR_JUGADOR(x) (x == 0? 'A' : 'Z')

//falta
int randInt(int a, int b)
{
    return ((rand() / ((double)RAND_MAX + 1)) * (b - a + 1)) + a;
} 

char* leerEntrada()
{
	char*entrada=NULL;
	int mem=0;
	char *aux=NULL;
	char c;
	printf("Accion: ");
	while ((c=getchar())!='\n')
	{
		if((mem % BLOQUE)==0)
		{
			aux = realloc(entrada,(mem+BLOQUE)*sizeof(char));
		}
		if (aux == NULL)
		{
			printf("No hay memoria\n");
			free(entrada);
			return NULL;
		}
		entrada = aux;
		entrada[mem++]=c;
	}
	entrada[mem]='\n';
	return entrada;
}

char** generaMatriz(int fil,int col)
{
	char **resp =NULL, **aux1=NULL, *aux2=NULL;
	int i, j;
	aux1=malloc(fil*sizeof(char*));
	if(aux1 != NULL)
	{
		resp = aux1; 
		for(i=0;i<fil;i++)
		{
			aux2 = malloc(col*sizeof(char));
			if(aux2 != NULL)
				resp[i] = aux2;
			else
			{
				free(resp);
				return NULL;
			}
		}
		for(i=0; i<fil; i++)
		{
			for(j=0; j<col; j++)
			{
				resp[i][j] = '0';
			}
		}
		resp[0][0] = 'A';
		resp[fil-1][0] = 'A';
		resp[0][col-1] = 'Z';
		resp[fil-1][col-1] = 'Z';
	}
	return resp;
}

char** leerDimension(tipoTablero * tablero)
{
	do
	{ 
	tablero->filas = getint("Ingrese la cantidad de filas (entre 5 y 30) = ");
	tablero->columnas = getint("Ingrese la cantidad de columnas (entre 5 y 30) = ");
	}
	while(tablero->filas >30 || tablero->filas<5 || tablero->columnas > 30 || tablero->columnas <5);
	return generaMatriz(tablero->filas, tablero->columnas); 
}

void imprimirTablero(tipoTablero*tablero)
{
	int i, j;
	for(i=0; i<tablero->filas; i++)
	{
		for(j=0; j<tablero->columnas; j++)
			printf("%c ", tablero->matriz[i][j]);
		putchar('\n');
	}
	putchar('\n');
}

int esPosible(tipoTablero*tablero, tipoPosicion celda)
{
	int i, j;
	for(i=celda.fil-2; i<=celda.fil+2; i++)
	{
		if(i > -1 && i<tablero->filas)
		{									 //recibe el lugar donde hay un blob y revisa
			for(j=celda.col-2; j<=celda.col+2; j++)
			{
				if(j > -1 && j<tablero->columnas)
				{											 // si hay un vacio a distancia 2 o menos
					if(tablero->matriz[i][j]=='0')
						return 1;
				}
			}
		}
	}
	return 0;
}

int movPosibles(tipoTablero*tablero, char jugador)
{ 
	int flag=0;    //devuelve 1 si hay movimientos posibles para el jugador
	tipoPosicion celda;
	for(celda.fil=0; celda.fil<tablero->filas && !flag; celda.fil++)
	{
		for(celda.col=0; celda.col<tablero->columnas && !flag; celda.col++)
		{
			if(tablero->matriz[celda.fil][celda.col] == jugador)
				flag = esPosible(tablero,celda);
		}
	}
	return flag;
}

//devuelve cuantas manchas come del otro jugador
int comeMancha(tipoTablero*tablero, tipoPosicion celda, char jugador)
{
	int i, j, cant = 0; 
	for(i=celda.fil-1; i<=celda.fil+1; i++)
	{
		if(i > -1 && i<tablero->filas)
		{
			for(j=celda.col-1; j<=celda.col+1; j++)
			{	
				if(j > -1 && j<tablero->columnas)
				{
					if(tablero->matriz[i][j] != jugador && tablero->matriz[i][j] != '0')
					{
						tablero->matriz[i][j] = jugador;
						cant++;
					}
				}
			}
		}
	}
	return cant;
}

// devuelve cantidad de blobs que aumenta el jugador en una jugada
int jugada(tipoTablero*tablero, tipoMovimiento*celdas, char jugador, int distancia)
{
	int cant = 0;
	tablero->matriz[celdas->destino.fil][celdas->destino.col] = jugador;
	if(distancia == 2)
		tablero->matriz[celdas->origen.fil][celdas->origen.col] = '0';
	else
		cant++;
	cant += comeMancha(tablero,celdas->destino,  jugador);
	return cant;
}

//devuelve 0 si hay un error en el movimiento, sino devuelve el tipo de error
int validarMovimiento(tipoTablero*tablero, tipoMovimiento*celdas,  char jugador, int distancia)
{
	printf("%d lel\n",FUERA_TABLERO(celdas->origen.fil, celdas->origen.col,tablero->filas,tablero->columnas));
	if (tablero->matriz[celdas->destino.fil][celdas->destino.col] != '0')
		return ERROR_MANCHA;
	else if(FUERA_TABLERO(celdas->origen.fil, celdas->origen.col,tablero->filas,tablero->columnas) || FUERA_TABLERO(celdas->destino.fil, celdas->destino.col, tablero->filas, tablero->columnas))
		return ERROR_TABLERO;
	else if (distancia > 2 || distancia == 0)
		return ERROR_DISTANCIA;
	else if (tablero->matriz[celdas->origen.fil][celdas->origen.col] != jugador)
		return ERROR_ORIGEN;
	else
		return 0;
}

int validarJugada(char * entrada, tipoMovimiento *celdas)
{
	
	char chequeo;
	int cantidad;
	cantidad = sscanf(entrada, "[%d,%d] [%d,%d]%c", &(celdas->origen.fil), &(celdas->origen.col), &(celdas->destino.fil), &(celdas->destino.col), &chequeo);

	if (cantidad == 5 && chequeo ==  '\n')
	{
		return BIEN;
	}
	else
	return ERROR;
}
void imprimirErrores(int tipoError, tipoMovimiento*celdas)
{
	printf("Error. ");
	if(tipoError == ERROR_MANCHA)
		printf("Hay una mancha en la posición [%d,%d]\n", celdas->destino.fil, celdas->destino.col);
	else if(tipoError == ERROR_ORIGEN)
		printf("No hay una mancha de tu jugador en la celda [%d,%d]\n", celdas->origen.fil, celdas->origen.col);
	else if(tipoError == ERROR_DISTANCIA)
		printf("Distancia inválida\n");
	else
		printf("Movimiento fuera del tablero\n");
}

int decidirEntrada(tipoTablero*tablero, char * entrada, tipoJugador * jugador)
{
	switch (entrada[0])
	{
		case 's':
		//
		break;
		case 'q':
		break;
		case '[':
		{
			tipoMovimiento celdas; 
			int chequeo;
			chequeo = validarJugada(entrada, &celdas);
				if (chequeo == BIEN)
				{
					//llamar funcion
					int distancia = MAX(abs(celdas.origen.fil - celdas.destino.fil), abs(celdas.origen.col - celdas.destino.col));
					int error;
					error =	validarMovimiento(tablero,&celdas, jugador->id, distancia);
					if (error == 0)
					{
						jugador->cantidadBlobs += jugada(tablero,&celdas,jugador->id,distancia);
						
					}
					else
					{
						imprimirErrores(error, &celdas);
						return 0;
						
					}
				}
				else
				{
					printf("Error en la entrada\n");
					free(entrada);
					leerEntrada();
				}
			break;
		}			
		default:
		{ 
			printf("Error en la entrada\n");
			free(entrada);
			leerEntrada();
		}
	}
	return 1;
}

void paraJugar()
{
	tipoTablero tablero;
	tipoJugadores jugadores;
	int i = 0;
	char*entrada;
	jugadores[0].id = ELEGIR_JUGADOR(randInt(0,1));
	if(jugadores[0].id == 'A')
		jugadores[1].id = 'Z';
	else
		jugadores[1].id = 'A';
	printf("Comienza el jugador 1 (%c)\n", jugadores[0].id);
	tablero.matriz = leerDimension(&tablero);
	printf("lol%d \n", movPosibles(&tablero, jugadores[i].id));
	while(movPosibles(&tablero, jugadores[i].id) != 0)
	{ 
		imprimirTablero(&tablero);
		printf("Turno del jugador %c\n", jugadores[i].id);
		entrada = leerEntrada();
		
		if (decidirEntrada(&tablero, entrada, &jugadores[i]))
		{ 
			if(i == 1)
				i=0;
			else
				i=1;
		}
	}
}

void seleccionarOpcion(int opcion)
{
	switch (opcion)
	{
		case 1:
			paraJugar();
		break;
		case 2:
			paraJugar();
		break;
		case 3:
			//TO DO 
			//load
		break;
		case 4:
			exit(0);
		break;
	}
}

int imprimirMenu()
{
	int opcion;
	do
	{ 
	printf("Menu:\n1.Juego de dos jugadores\n2.Juego contra computadora\n3.Recuperar un juego grabado\n4.Terminar\n");
	opcion = getint("Por favor seleccione una opcion ingresando el correspondiente nro del 1 al 4\n");
	}
	while(opcion<1 || opcion>4);
	seleccionarOpcion(opcion);
	return BIEN;
}
int main()
{
	srand(time(NULL));
	imprimirMenu();
	return 0;
}
