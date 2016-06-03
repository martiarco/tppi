BACK END
# include <stdio.h>
#define JUGADOR1 0
#define JUGADOR2 1
int cambiar_jugador(int x)
{
	if(x%2)
		return JUGADOR1;
	else
		return JUGADOR2;
}

#include <stdio.h>
#include "getnum.h"
#include <stdlib.h>
#include <math.h>
#include "time.h"

#define BLOQUE 5
#define ERROR 0
#define BIEN 1
#define EXIT 2
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

	switch opcion
	case 1:
	paraJugar();
	
	case 2:
	paraJugar();

	case 3:
	//TO DO 
	//load
	case 4:
	exit(0);

	

}

int paraJugar(opcion)
{
	tipoTablero tablero;
	tablero.matriz = leerDimension();
	while(movPosibles(&tablero, jugador.id) != 0)
	{ 
	imprimirTablero(&tablero);
	//quejugador e imprime quejugador
	leerEntrada();
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
	decidirEntrada(entrada);
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

