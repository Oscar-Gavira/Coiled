/*
Coiled is a UCI chess playing engine authored by Oscar Gavira.
Copyright (C) 2013-2021 Oscar Gavira.
<https://github.com/Oscar-Gavira/Coiled>

Coiled is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Coiled is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef PERFT_H
#define PERFT_H

#include "Externo.h"

typedef struct tag_Perft
{
	U64 A_Nodos;						/* Nodos totales */
	U64 A_Capturas;						/* Capturas totales */
	U64 A_Ep;							/* Capturas al paso totales */
	U64 A_Enroques;						/* Enroques totales */
	U64 A_Coronacion;					/* Coronaciones totales */
	U64 A_Jaque;						/* Jaques totales */
	U64 A_JaqueMate;					/* Jaque Mates totales */
} _ST_Perft;

typedef struct tag_Divide
{
	U64 D_Nodos;						/* Nodos por jugada principal (Para el Divide) */
	char D_Jugada[6];					/* Almacenamos las coordenadas de cada jugada realizada (Para el Divide) */
} _ST_Divide;

void Perft(int div);																		/* Llamada principal de la funcion Perft */
int MiniMax(int depth, _ST_Perft *P_Divide, int *P_Mov, U64 *PosicionesPorJugada);			/* Metodo recursivo para la funcion Perft */

#endif
