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
