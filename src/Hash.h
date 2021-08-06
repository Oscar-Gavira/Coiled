/*
Coiled-NNUE is a UCI chess playing engine authored by Oscar Gavira.
Copyright (C) 2013-2021 Oscar Gavira.
<https://github.com/Oscar-Gavira/Coiled-NNUE>

Coiled-NNUE is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Coiled-NNUE is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef HASH_H
#define HASH_H

#include "Externo.h"
#include "Utilidades.h"

#ifdef USAR_HASH_TB

// 16 bit short (2 bytes * 8) = 16 bit */
// 0000 0000 0111 1111 */		/*EDAD -  7 bit*/   Valor máximo 127
// 0001 1111 1000 0000 */		/*DEPTH - 6 bit*/   Valor máximo 63
// 0110 0000 0000 0000 */		/*FLAG -  2 bit*/   Valor máximo 3
#define EDAD(e)		((e) & 0x7F)					/* 7bit - rango del 0 al 127 */
#define DEPTH(d)	(((d)>>7) & 0x3F)				/* 6bit - rango del 0 al 63 */
#define FLAG(f)		(((f)>>13) & 0x3)				/* 2bit = 1 o 2 o 3 (ALPHA, BETA, EXACT) */
#define MAX_EDAD	(127)							/* Edad maxima */
#define CELDAS		(4)

/******************************************************************************
Estructuras
******************************************************************************/

/* Estructura para manejar la tabla hash */
typedef struct tag_TT_Opciones
{
	U64 tt_Mb;
	U64 tt_Entradas;
	U64 tt_Completo;
	short tt_Edad;
} _ST_TT_Opciones;

/* Estructura de la tabla hash */
typedef struct	tag_TT_CacheC
{
	U64 Hash;
	int M;
	short Puntos;
	short Ev;
	short Flag;
} _ST_TT_CacheC;

typedef struct	tag_TT_Cache
{
	_ST_TT_CacheC Celdas[CELDAS];
} _ST_TT_Cache;

/******************************************************************************
Variables
******************************************************************************/
/* [Piezas][Tablero] En la matriz pieza, el 0 se utiliza para zobrist de Al Paso y el 7 se utiliza para los Enroque. Y [7][32] para el turno */
U64 zobrist_Tablero[14][64];
_ST_TT_Opciones TT_Opciones;
_ST_TT_Cache *TT_Datos;

/******************************************************************************
							Métodos
******************************************************************************/
short SAVEFLAG(int d, int e, int f);											/* Guarda Depth, Edad y flag en macro en una variable */
void ObtenerKey();																/* Obtengo el hash inicial */
void CrearTransposicion(int MB);												/* Creamos la tabla de hash aun tamaña especifico */
int RecuperarPosicion(int *hPuntos, int *hEv, int *hMov, int *hFlag);			/* Obtengo si la posición ya a sido analizada y que valores tiene */
void ConvertirValorTT(int *hPuntos);											/* Convertimos puntosa la TT */
int PodaHash(int *flag, int *depth, int *beta, int *alpha, int *hPuntos);		/* Se puedo podar mediante TT */
void AlmacenarPosicion(int depth, int puntos, int hEv, int flag, int hMov);		/* Guardo la posición analizada */
int RecuperarMovimientoHash();													/* Obtenemos el movimiento si esta almacenado */
void LimpiarTransposicion();													/* Vació la tabla hash */
int ObtenerHashCompleto();														/* Obtengo el estado de la tabla (Vacía, Medio llena, LLena) % */
void LiberarMemoria();															/* Liberamos la tabla hash */

#endif
#endif
