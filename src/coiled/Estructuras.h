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

/******************************************************************************
						ESTRUCTURAS/ENUMERACIONES
*******************************************************************************/
#ifndef ESTRUCTURAS_H
#define ESTRUCTURAS_H

#include "Definiciones.h"

typedef struct tag_TipoJuego
{
	int Interrumpir;					/* false o true - Cuando se excede el tiempo o se recibe el comando stop */
	U64 Tiempo;							/* Tiempo total por movimiento. */
	U64 Inicio;							/* Tiempo inicial. */
	U64 TiempoTranscurrido;				/* Para detectar si excedemos el tiempo limite por jugada */
	int Activo;							/* Si el juego es por tiempo o por profundidad (depth) */
	int Infinito;						/* Activa el modo Análisis por así decirlo, piensa hasta recibir un stop o una entrada */
	int MaxDepth;						/* Máxima profundidad. Búsqueda tipo go depth 15*/
	int DepthAct;						/* Depth actual */
	U64 Nodos;							/* Nodos totales */
	U64 NJugadasTotales;				/* Nº de jugadas totales a dividir por el tiempo total del juego */
	int MejorJugada;					/* Mejor jugada */
	int MejorJugadaAdv;					/* Mejor jugada para el adversario */
	int MostrarVp;						/* Muestra la variante principal de tres formas: 0 = Nada, 1 = Mixto o 2 = Entero */
	int BuscarMate;						/* Busca hasta localizar un mate en x */
	int Ajedrez960;						/* Activa/desactiva el modo de juego Ajedrez960 */
	int Ajedrez960Enroque;				/* Valor de 0 = UCI estándar. Valor de 1 O-O/O-O-O GUI Arena */
	int JugadaIlegal;					/* Se activa si recibimos un movimiento incorrecto position fen rkbbnrqn/pppppppp/8/8/8/8/PPPPPPPP/RKBBNRQN w KQkq - 0 1 moves h1g3 .... f7f8*/
} _ST_TipoJuego;

typedef struct tag_Movimiento
{
	int Movimiento;						/* Movimiento en macro */
	int Ordenar;						/* Valor numérico para ordenar las jugadas */
} _ST_Movimiento;

/* No se puede poner en #ifdef, ya que se usa para el enroque normal */
typedef struct tag_Ajedrez960 {
	int TorreBlancaA;
	int TorreBlancaH;
	int TorreNegraA;
	int TorreNegraH;
} _ST_Ajedrez960;

typedef struct tag_DeshacerMovimiento
{
	U64 Hash;							/* Hash anterior */
	int Movimiento;						/* Movimiento en macro */
    int PosicionReyB;					/* Índice del vector, posición del rey Blanco */
	int PosicionReyN;					/* Índice del vector, posición del rey Negro */
	int EnroqueB;						/* Enroque permitido con torres */
	int EnroqueN;						/* Enroque permitido con torres */
	int FichaAlPasoPosicion;			/* Índice del vector, pieza al paso. */
	int Regla_50_Movimiento;			/* Regla 50 movimientos */
} _ST_DeshacerMovimiento;

/***********************************************************************************
Tipo para almacenar datos de la evaluación
***********************************************************************************/
typedef struct tag_Valor
{
	int Apertura;											/* Valor en apertura de juego */
	int Final;												/* Valor en final de juego */
} _Valor;

typedef struct tag_Puntos {
	/* Numero de piezas */
	int PeonTotales;					/* Peones totales. */
	int CaballosTotales;				/* Caballos totales. */
	int AlfilTotales;					/* Alfiles totales. */
	int TorresTotales;					/* Torres totales. */
	int DamasTotales;					/* Damas totales. */
} _ST_Puntos;

/* Estructura del tablero */
typedef struct tag_TableroX64 {
    int PosicionReyB;					/* Índice del vector, posición del rey Blanco */
	int PosicionReyN;					/* Índice del vector, posición del rey Negro */
	int EnroqueB;						/* Enroque permitido con torres */
	int EnroqueN;						/* Enroque permitido con torres */
	_ST_Ajedrez960 Ajedrez960;			/* Posición de las torres. Desde el rey hasta la torre no puede haber ninguna ficha ni pasar por casilla amenazada por el adversario. */
	int FichaAlPasoPosicion;			/* Índice del vector, pieza al paso. */

	int Ply;							/* Ply */
	int MueveBlancas;					/* turno de jugador */
	int Regla_50_Movimiento;			/* Regla 50 movimientos */
	int Etapa;							/* Estado del juego. Apertura. medio juego, final. */
	_ST_DeshacerMovimiento Estado[MAX_PLY];/* Almacena los avances de la jugada, para poder deshacer después */

#ifdef USAR_HASH_TB
	U64 Historico[MAX_HISTORICO];		/* 800 movimientos. Lo normal son 200 o 250 jugadas, que es igual a 400 o 500 movimientos*/
	int Hply;							/* Contador incremental de los movimientos que se van realizando en la variable Histórico */
	U64 Hash;							/* Hash del tablero */
#endif

	int TableroColor[64];				/* Para detectar los márgenes del tablero */
	int Tablero[64];					/* Array donde están las piezas en el tablero */
	/*		INDICES								PIEZAS
      8- 00 01 02 03 04 05 06 07     =   11 09 10 12 13 10 09 11		NEGRAS.		VALOR de HORIZONTAL = 7:
	  7- 08 09 10 11 12 13 14 15     =   08 08 08 08 08 08 08 08
	  6- 16 17 18 19 20 21 22 23     =   07 07 07 07 07 07 07 07
	  5- 24 25 26 27 28 29 30 31     =   07 07 07 07 07 07 07 07
	  4- 32 33 34 35 36 37 38 39     =   07 07 07 07 07 07 07 07
	  3- 40 41 42 43 44 45 46 47     =   07 07 07 07 07 07 07 07
	  2- 48 49 50 51 52 53 54 55     =   01 01 01 01 01 01 01 01
	  1- 56 57 58 59 60 61 62 63     =   04 02 03 05 06 03 02 04		BLANCAS.	VALOR de HORIZONTAL = 0:

	  0	  VALOR DE COLUMNA.    7

	  C						   C
	  O						   O
	  L						   L
	  U						   U
	  M						   M
	  N						   N
	  A						   A
	*/
} _ST_TableroX64;

#endif
