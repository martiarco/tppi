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
} TipoMovimiento;
typedef struct 
{
	char id;
	int cantidadBlobs;
} TipoJugador;
typedef struct
{
	char** matriz;
	int filas,columnas;
} tipoTablero;
typedef TipoJugador TipoJugadores[2];
#define FUERA_TABLERO(x,y,fil,col) (x<0 || x>fil || y<0 || y>col)
#define MAX(x,y) (x > y? x : y)
#define ELEGIR_JUGADOR(x) (x == 0? 'A' : 'Z')

//falta
int imprimirMenu(void)
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
	printf("Comienza el jugador 1 (%c)", jugadores[0].id);
	tablero.matriz = leerDimension();
	while(movPosibles(&tablero, jugador.id) != 0)
	{ 
		imprimirTablero(&tablero);
		printf("Turno del jugador %c\n", jugadores[i].id);
		//quejugador e imprime quejugador
		entrada = leerEntrada();
		decidirEntrada(entrada, &jugadores[i]);
		if(i == 1)
			i=0;
		else
			i=1;
	}
}

char** leerDimension()
{
	tipoTablero tablero;
	do
	{ 
	printf("Ingrese las dimensiones del tablero. Tablero minimo 5x5. Tablero maximo 30x30\n");
	tablero.filas = getnum("Dimension de filas = \n");
	tablero.columnas = getnum("Dimension de columnas = \n");
	}
	while(tablero.filas >30 || tablero.filas<5 || tablero.columnas > 30 || tablero.columnas <5);
	tablero.matriz = generaMatriz(tablero.filas, tablero.columnas);
	return tablero.matriz; 
}

int randInt(int a, int b)
{
    return ((rand() / ((double)RAND_MAX + 1)) * (b - a + 1)) + a;
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

char** generaMatriz(int fil,int col)
{
	char**p, i;
	if(p=malloc(fil*sizeof(int)))
	{
		for(i=0;i<col;i++)
		{
			if(!(p[i]=malloc(col*sizeof(char))));
				return NULL;
		}
	p[0][0] = 'A';
	p[fil-1][0] = 'A';
	p[0][col-1] = 'Z';
	p[fil-1][col-1] = 'Z';
	}
	return p;
}
int esPosible(tipoTablero*tablero, tipoPosicion celda)
{
	int i, j;
	for(i=celda.fil-2; i<=celda.fil+2; i++)
	{
		if(i>-1 && i<tablero->filas)
		{									 //recibe el lugar donde hay un blob y revisa
			for(j=celda.col-2; j<=celda.col+2; j++)
			{
				if(j>-1 && j<tablero->columnas)
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
	int flag=0, i, j;    //devuelve 1 si hay movimientos posibles para el jugador
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
		if(i>-1 && i<tablero->filas)
		{
			for(j=celda.col-1; j<=celda.col+1; j++)
			{	
				if(j>-1 && j<tablero->columnas)
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
int jugada(tipoTablero*tablero, TipoMovimiento*celdas, char jugador, int distancia)
{
	int cant = 0;
	tablero->matriz[celdas->destino.fil][celdas->destino.col] = jugador;
	if(distancia == 2)
		tablero->matriz[celdas->origen.fil][celdas->origen.col] = 0;
	else
		cant++;
	cant += comeMancha(tablero,celdas->destino,  jugador);
	return cant;
}

//devuelve 0 si hay un error en el movimiento, sino devuelve el tipo de error
int validarMovimiento(tipoTablero*tablero, TipoMovimiento*celdas,  char jugador, int distancia)
{
	if (tablero->matriz[celdas->destino.fil][celdas->destino.col] > 0)
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

int decidirEntrada(char * entrada, tipoJugador * jugador)
{
	switch (entrada[0])
	case 's':
	//
	break;
	case 'q':
	//
	break;
	case '[':
	tipoMovimiento celdas; 
	int chequeo;

	chequeo = validarJugada(entrada, &celdas);
		if (chequeo == BIEN)
		{
			//llamar funcion
			int distancia = MAX(abs(celdas.origen.fil - celdas.destino.fil), abs(celdas.origen.col - celdas.destino.col));
			int error;
			error =	validarMovimiento(&tablero,&celdas, jugador->id, distancia);
			if (error == 0)
			{
				jugador->cantidadBlobs += jugada(&tablero,&celdas,jugador->id,distancia);
			}
			else
			{
				//imprimir errores
			}
		}
		else
		printf("Error en la entrada\n");
		free(entrada);
		leerEntrada();
	break;
	default: 
	printf("Error en la entrada\n");
	free(entrada);
	leerEntrada();

}

char* leerEntrada()
{
	char*entrada=NULL;
	int mem=0;
	char *aux;
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
			return NULL;
		}
		entrada = aux;
		entrada[mem++]=c;
	}
	entrada[mem]='\n';
	return entrada;
}


int validarJugada(char * entrada, tipoMovimiento *celdas)
{
	
	char chequeo;
	int cantidad;
	cantidad = sscanf(entrada, "[%d,%d] [%d,%d]%c", celdas->origen.fil, celdas->origen.col, celdas->destino.fil, celdas->destino.col, &chequeo);

	if (cantidad == 5 && chequeo ==  '\n')
	{
		return BIEN;
	}
	else
	return ERROR;
}

