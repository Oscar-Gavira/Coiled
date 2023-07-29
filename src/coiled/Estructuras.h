/*
Coiled is a UCI compliant chess engine written in C
Copyright (C) 2023 Oscar Gavira. <https://github.com/Oscar-Gavira/Coiled>

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

/******************************************************************************
Estructuras
******************************************************************************/
#ifdef USAR_NNUE
typedef struct tag_nnue {
	int Usar;												/* Se puede usar la red neuronal (true/false) */
	char ArchivoNnue[MAX_DIR];								/* Obtenemos las rutas a la red neuronal */
	int DirectorioNuevo;									/* Indica si hay cambio de ruta a la red neuronal */
} _ST_Nnue;
#endif

#ifdef USAR_HASH_TB
/* Estructura para manejar la tabla hash */
typedef struct tag_TT_Opciones
{
	U64 tt_Mb;
	U64 tt_Entradas;
	U64 tt_HashCompleto;
	int tt_Edad;
} _ST_TT_Opciones;

/* Estructura de la tabla hash */
typedef struct tag_TT_CacheC
{
	U32 Hash;
	int M;
	short Puntos;
	short Ev;
	int Flag;
} _ST_TT_CacheC;

typedef struct tag_TT_Cache
{
	_ST_TT_CacheC Celdas[CELDAS];
} _ST_TT_Cache;
#endif

#ifdef PERFT
typedef struct tag_Perft
{
	U64 A_Capturas;						/* Capturas totales */
	U64 A_Ep;							/* Capturas al paso totales */
	U64 A_Enroques;						/* Enroques totales */
	U64 A_Coronacion;					/* Coronaciones totales */
	U64 A_Jaque;						/* Jaques totales */
	U64 A_JaqueMate;					/* Jaque Mates totales */
	int MaxDepth;						/* Profundidad maxima */
	int Tiempo;							/* Tiempo */
} _ST_Perft;

typedef struct tag_Divide
{
	U64 D_Nodos;						/* Nodos por jugada principal (Para el Divide) */
	char D_Jugada[6];					/* Almacenamos las coordenadas de cada jugada realizada (Para el Divide) */
} _ST_Divide;
#endif

typedef struct tag_SeeDeshacerMovimiento
{
	int Movimiento;											/* Movimiento */
	short PosicionReyB;										/* Posicion del rey blanco */
	short PosicionReyN;										/* Posicion del rey negro */
} _ST_SeeDeshacerMovimiento;

typedef struct tag_SeeTablero
{
	_ST_SeeDeshacerMovimiento Estado[40];					/* Información del progreso de pacturas */
	short SeePly;											/* Numero de capturas */
} _ST_SeeTablero;

typedef struct tag_TipoJuego
{
	int SubProcesosActivo;									/* false o true - indica si hay una busqueda activa. */
	int Interrumpir;										/* false o true - Termina la busqueda al exceder el tiempo o se recibe el comando stop */
	
	int TiempoIdeal;										/* Tiempo ideal por movimiento. */
	int TiempoMed;											/* Tiempo medio por movimiento. */
	int TiempoMax;											/* Tiempo maximo por movimiento. */
	int TiempoConsumido;									/* Tiempo transcurrido */
	int XTiempo;											/* Tiempo entre depth */
	int TiempoFactor;										/* Estabilidad de la Pv */

	int PrevenirTiempoExcedido;								/* Para evitar un Timeout en un juego por tiempo. Descuenta tiempo en ms al tiempo de busqueda, para poder enviar el mejor movimiento sin exceder el tiempo limite. */
	int Inicio;												/* Tiempo inicial. */
	int Activo;												/* Si el juego es por 1 = tiempo. 2 = movetime. 0 = Mate, Infinite, profundidad (depth) */
	int MaxDepth;											/* Maxima profundidad. Busqueda tipo go depth 15*/
	int BuscarMate;											/* Busca hasta localizar un mate en x */
	/* Configuracion UCI */
	int Ajedrez960;											/* Activa/desactiva el modo de juego Ajedrez960 */
	int NumeroDeSubProcesos;								/* Numero de subprocesos seleccionados, por defecto 1 */
	int NumeroDeSubProcesosMax;								/* Maximo numero de subprocesos. */
} _ST_TipoJuego;

typedef struct tag_Movimiento
{
	int Movimiento[MAX_JUGADAS];							/* Movimiento en macro */
	short Ordenar[MAX_JUGADAS];								/* Valor numerico para ordenar las jugadas */
	short HayHash;											/* Indica si hay Hash en dicha interaccion */
	int CantidadDeMovimiento;								/* Numero de jugadas obtenidos */
} _ST_Movimiento;

typedef struct tag_Valor
{
	int Apertura;											/* Valor en apertura de juego */
	int Final;												/* Valor en final de juego */
} _Valor;

typedef struct tag_Evaluacion {
	_Valor PST[2][6][64];
} _Evaluacion;

typedef struct tag_Puntos {
	/* Numero de piezas */
	int PeonTotales;										/* Peones totales. */
	int CaballosTotales;									/* Caballos totales. */
	int AlfilTotales;										/* Alfiles totales. */
	int TorresTotales;										/* Torres totales. */
	int DamasTotales;										/* Damas totales. */
	_Valor Puntos;											/* Puntos totales. pst, movilidad, ataques, pareja de alfiles... */

	int PosicionTorre[8];									/* Se van anadiendo segun se encuentren. ( Si solo hay dos torres estaran en [0] y [1] ). Valor = Indice del vector. Valor vacio = -1  */
	int PeonDoblados[8];									/* Almacena un valor por cada columna del vector (Horizontal A... H). Detectando si hay peones doblados siendo el valor > 1 doblado. Valor vacio 0 */
	int GrupoPeonesQ;										/* Indica si hay peones en la columna a,b,c,d. */
	int GrupoPeonesK;										/* Indica si hay peones en la columna e,f,g,h. */

	/* Seguridad del rey */
	int ReyCuadrosAtacando;									/* Numero de cuadros atacando */
	int ReyAtaquesPiezas;									/* Numero de piezas atacando el area del rey */
	_Valor ReyAtaquesValor;									/* Valor de las piezas que atacan */
	int ReyJaquePieza[5];									/* Pieza que genera jaque. */
} _ST_Puntos;

/* No se puede poner en #ifdef USAR_AJEDREZ960, ya que se usa para el enroque normal */
typedef struct tag_Ajedrez960 {
	int TorreBlancaA;										/* Indice del vector, posicion de la torre */
	int TorreBlancaH;										/* Indice del vector, posicion de la torre */
	int TorreNegraA;										/* Indice del vector, posicion de la torre */
	int TorreNegraH;										/* Indice del vector, posicion de la torre */
} _ST_Ajedrez960;

typedef struct tag_DeshacerMovimiento
{
	U64 Hash;												/* Hash anterior */
	int Movimiento;											/* Movimiento en macro */
	int PosicionReyB;										/* Indice del vector, posicion del rey Blanco */
	int PosicionReyN;										/* Indice del vector, posicion del rey Negro */
	int EnroqueB;											/* Enroque permitido con torres */
	int EnroqueN;											/* Enroque permitido con torres */
	short FichaAlPasoPosicion;								/* Indice del vector, pieza al paso. */
	short Regla_50_Movimiento;								/* Regla 50 movimientos */
} _ST_DeshacerMovimiento;

typedef struct tag_VariantePrincipal {
	int vp_triangular[MAX_PLY][MAX_PLY];					/* Vp usada durante la busqueda */
	int vp_root[MAX_PLY];									/* Vp terminada en root */
	int vp_terminada[MAX_PLY];								/* Vp terminada y la mejor */
	int vp_Ev[MAX_PLY];										/* Almacena la puntuacion del ply actual */
} _ST_Vp;

typedef struct tag_Historico {
	S64 Historico_Movimientos[64][64];						/* [Origen][Destino] */
	int Historico_Killer[MAX_PLY][2];						/* [MAXPLY][2 jugadas] */
	int Historico_Killer_Mate[MAX_PLY][2];					/* [MAXPLY][2 jugadas] */
	int HistoricoRefutacion[2][6][64][64];					/* [Color][Pieza][Origen][Destino] */
	S64 HistoricoCaptura[64][64];							/* [Origen][Destino] */
	S64 Max_Historico;										/* Valor maximo almacenado */
	S64 Min_Historico;										/* Valor minimo almacenado */
	S64 Max_Historico_Cap;									/* Valor maximo almacenado */
	S64 Min_Historico_Cap;									/* Valor minimo almacenado */
} _ST_Historico;

/* Estructura del tablero */
typedef struct tag_TableroX64 {
	short IdSubProcesos;									/* Id del subproceso */
#ifdef PERFT
	short ActSubProcesos;									/*  */
#endif

	_ST_Puntos Blancas;										/* Almacenamos informacion de la evaluacion de las blancas */
	_ST_Puntos Negras;										/* Almacenamos informacion de la evaluacion de las negras */

	_ST_Vp Vp;												/* Variante principal usado por el subproceso principal */
	_ST_Historico His;										/* Historico de la busqueda */

	int Etapa;												/* Estado del juego. Apertura. medio juego, final. */
	int DepthAct;											/* Depth alcanzado en la busqueda  */
	int Puntos;												/* Puntos obtenidos en la busqueda */
	U64 Nodos;												/* Nodos totales */

	int PosicionReyB;										/* Indice del vector, posicion del rey Blanco */
	int PosicionReyN;										/* Indice del vector, posicion del rey Negro */
	int EnroqueB;											/* Enroque permitido con torres */
	int EnroqueN;											/* Enroque permitido con torres */
	_ST_Ajedrez960 Ajedrez960;								/* Posicion de las torres. Desde el rey hasta la torre no puede haber ninguna ficha ni pasar por casilla amenazada por el adversario. */
	int FichaAlPasoPosicion;								/* Indice del vector, pieza al paso. */

	int Ply;												/* Ply */
	int selDepth;											/* Profundidad de busqueda maxima */
	int MueveBlancas;										/* turno de jugador */
	int Regla_50_Movimiento;								/* Regla 50 movimientos */
	_ST_DeshacerMovimiento Estado[MAX_PLY];					/* Almacena los avances de la jugada, para poder deshacer despues */

#ifdef USAR_HASH_TB
	U64 Historico_Repeticion[MAX_HISTORICO];				/* (101 + MAX_PLY) */
	int Hply;												/* Contador de los movimientos que se van realizando en la variable Historico */
	U64 Hash;												/* Hash del tablero */
#endif

	int TableroColor[64];									/* Para detectar los margenes del tablero */
	int Tablero[64];										/* Array donde estan las piezas en el tablero */
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
