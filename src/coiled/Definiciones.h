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

#ifndef DEFINICIONES_H
#define DEFINICIONES_H

#define NOMBRE "Coiled"
#define VER  "1.2"
#define AUTOR  "Oscar Gavira"

//#define ARC_64BIT														/* Para compilar a 64 bit. Incluido en el Makefile */
#define USAR_AJEDREZ960													/* Compilar con la opcion, Ajedrez960. */
#define PERFT                                                           /* Compilar con la opcion, Perft (Depurar) */

#ifdef ARC_64BIT
    #define USAR_NNUE													/* Compilar con la opcion, NNUE. */
#endif

/*
UCI - debug[on | off]
switch the debug mode of the engine conand off.
In debug mode the engine should send additional infos to the GUI, e.g.with the "info string" command,
to help debugging
*/
//#define DEBUG															/* Activa/desactiva los mensajes en formato debug "info string ....." */
#ifdef DEBUG
	#define INFO_STRING "info string "
#else
	#define INFO_STRING ""
#endif


#ifdef _MSC_VER
    #define _CRT_SECURE_NO_WARNINGS
    #include <crtdbg.h>
    #pragma warning (disable : 4996)
    #pragma warning (disable : 4820)
    #pragma warning (disable : 28159)
    #pragma warning(disable : 5105)
    #define WIN32_LEAN_AND_MEAN
    #include <windows.h>
    #undef WIN32_LEAN_AND_MEAN

    typedef HANDLE THREAD_ID;
    #define CREAR_SUBPROCESO(h, f, p)   (h = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)f, p, 0, 0))
    #define ESPERAR_SUBPROCESO(h)       { WaitForSingleObject(h, INFINITE); CloseHandle(h); }
#else
#include <pthread.h>

typedef pthread_t THREAD_ID;
typedef void* (*pt_start_fn)(void*);

#define CREAR_SUBPROCESO(h, f, p)   pthread_create(&(h), NULL, (pt_start_fn)f, p)
#define ESPERAR_SUBPROCESO(h)       pthread_join(h, NULL)
#endif

#ifdef _WIN32
#include <stdio.h>														/* Printf, FILE, fclose, fflush, stdout, fgets... */
#include <string.h>														/* Memset, strcat, strcopy */
#include <ctype.h>														/* toupper */
#include <stdlib.h>														/* atoi, atoll, exit(EXIT_SUCCESS), srand, rand */
#include <stdint.h>														/* Tipo de variables y valores maximos */
#include <limits.h>														/* Valores maximos y minimos de cada tipo de variable int, short, char...*/
#include <inttypes.h>													/* Los formatos para imprimir un string, int, U32, U64... %d, %s, llu... */
#include <math.h>														/* log(x) */
#include <time.h>														/* Tiempo */
#include <windows.h>
#else
#include <stdio.h>														/* Printf, FILE, fclose, fflush, stdout, fgets... */
#include <string.h>														/* Memset, strcat, strcopy */
#include <ctype.h>														/* toupper */
#include <stdlib.h>														/* atoi, atoll, exit(EXIT_SUCCESS), srand, rand */
#include <stdint.h>														/* Tipo de variables y valores maximos */
#include <limits.h>														/* Valores maximos y minimos de cada tipo de variable int, short, char...*/
#include <inttypes.h>													/* Los formatos para imprimir un string, int, U32, U64... %d, %s, llu... */
#include <math.h>														/* log(x) */
#include <time.h>
#include <sys/time.h>													/* Tiempo */
#include <unistd.h>														/* Necesario para vincular librerias. */
#endif

#if defined(__GNUC__) && (__GNUC__ >= 8)
#pragma GCC diagnostic ignored "-Wcast-function-type"
#endif

#if defined(ARC_64BIT)
#define U64_FORMAT "%"PRIu64""
#define S32_FORMAT "%"PRId32""
#define STRING_FORMAT "%s"
#define MAX_PLY					128								    	/* Maximos ply */
#define MB_HASH_TABLE_MAX		8192							    	/* Tamano de la tabla hash maximo */
#define MB_HASH_TABLE			4								    	/* Tamano de la tabla hash por defecto */
#define MB_HASH_TABLE_MIN		4								    	/* Tamano de la tabla hash minimo */

typedef uint64_t U64;
typedef uint32_t U32;
typedef int64_t S64;											    	/* 9.223.372.036.854.775.807 / -9.223.372.036.854.775.806. Para el historico. */

#else
#define U64_FORMAT "%"PRIu32""
#define S32_FORMAT "%"PRId32""
#define STRING_FORMAT "%s"
#define MAX_PLY					64								    	/* Maximos ply */
#define MB_HASH_TABLE_MAX		2048									/* Tamano de la tabla hash maximo */
#define MB_HASH_TABLE			2								    	/* Tamano de la tabla hash por defecto */
#define MB_HASH_TABLE_MIN		2										/* Tamano de la tabla hash minimo */

typedef uint32_t U64;
typedef uint32_t U32;
typedef int32_t S64;													/* 2.147.483.647 / -2.147.483.646. Para el historico. (Si alcanza mucha profundidad en la busqueda FALLOS...) */
#endif

#define ABS(x)				(((x) > (0)) ? (x) : -(x))
#define MAX(a,b)			(((a) > (b)) ? (a) : (b))
#define MIN(a,b)			(((a) < (b)) ? (a) : (b))
#define COLUMNA(x)			((x & 7))									/* Obtenemos la columna donde esta situada la pieza de A a H (0 a 7) */
#define HORIZONTAL(x)		((7 - (x >> 3)))							/* Obtenemos la linea horizontal donde esta situada la pieza 1 a 8 (0 a 7) */

#define MAX_JUGADAS			256										    /* Numero maximo de jugadas que se pueden obtener */
#define MAX_HISTORICO		(101 + MAX_PLY)		    				    /* Maximo numero de jugadas de historico.  */
#define COMPROBAR_ENTRADAS	1023									    /* Cada x tiempo comprobamos entradas */

#define DISTANCIA(a, b)     (MAX(ABS(HORIZONTAL(a) - HORIZONTAL(b)), ABS(COLUMNA(a) - COLUMNA(b))))
#define MAX_ETAPA           24			/* ETAPA maximo */
#define FIN_ETAPA           5			/* ETAPA Final */
#define FIN_ETAPA_R         8			/* ETAPA Final */

#define PREVENIR_TIEMPO_EXCEDIDO_MAX        1000
#define PREVENIR_TIEMPO_EXCEDIDO_DEFECTO    50


#define MAX_DIR				256
#define START_POS			"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 0"
#define false               0
#define true                1

/* Macros para el movimiento */
/*
0000 0000 0000 0000 0000 0000 0111 1111 -> Origen						7 bits
0000 0000 0000 0000 0011 1111 1000 0000 -> Destino						7 bits
0000 0000 0000 0011 1100 0000 0000 0000	-> Pieza movida					4 bits
0000 0000 0011 1100 0000 0000 0000 0000 -> Captura						4 bits
0000 0011 1100 0000 0000 0000 0000 0000 -> Coronacion					4 bits
0000 0100 0000 0000 0000 0000 0000 0000 -> EP (Captura)					1 bits
0000 1000 0000 0000 0000 0000 0000 0000 -> Peon dos adelante			1 bits
0001 0000 0000 0000 0000 0000 0000 0000 -> Enroque						1 bits
*/

#define MFLAGCAP            0xF													/* Mascara para la captura */
#define MFLAGPROM           0xF													/* Mascara para la coronacion */
#define MFLAGMOV            0xF													/* Mascara para el movimiento */

#define MFLAGEP             0x4000000											/* Captura la paso */
#define MFLAGPS             0x8000000											/* Movimiento al paso */
#define MFLAGCA             0x10000000  										/* Enroque */

#define NO_MOVIMIENTO       0

/* cuadro origen, cuadro destino, pieza movida, pieza capturada, coronacion, EP/Dos adelante/Enroque */
#define MOVIMIENTO(o,d,p,c,co,f) ( (o) | ((d) << 7) | ( (p) << 14 ) | ( (c) << 18 ) | ( (co) << 22 ) | (f))

#define HASH_PIEZA(x, p, c) (x->Hash ^= zobrist_Tablero[(p)][(c)])
#define HASH_ENROQUE(x, e) (x->Hash ^= zobrist_Tablero[7][(e)])
#define HASH_LADO(x) (x->Hash ^= zobrist_Tablero[7][32])
#define HASH_EP(x, p) (x->Hash ^= zobrist_Tablero[0][(p)])

#define CUADRADO_ORIGEN(m) ((m) & 0x7F)									/* Obtenemos el indice de donde esta la pieza */
#define CUADRADO_DESTINO(m) (((m)>>7) & 0x7F)							/* Obtenemos el indice de donde va la pieza */
#define PIEZAMOVIDA(m) (((m)>>14) & MFLAGMOV)							/* Devuelve la pieza (1,2,3,4,5,6 - 8,9,10,11,12,13) */
#define CAPTURADA(m) (((m)>>18) & MFLAGCAP)								/* Devuelve MFLAGCAP o la pieza (1,2,3,4,5,6 - 8,9,10,11,12,13) */
#define CORONACION(m) (((m)>>22) & MFLAGPROM)							/* Devuelve MFLAGPROM o la pieza (2,3,4,5 - 9,10,11,12) */

#define CAPTURA_ALPASO(m) (m & MFLAGEP)									/* Devuelve 0 o MFLAGEP. Indica si el movimiento es una captura al paso */
#define GENERA_ALPASO(m) (m & MFLAGPS)									/* Devuelve 0 o MFLAGPS. Indica dos a delante de un peon */
#define ENROQUE(m) (m & MFLAGCA)										/* Devuelve 0 o MFLAGCA. Indica movimiento de enroque */

// 0000 0000 0000 0000 0000 0011 1111 1111 */		/*EDAD -  10 bit*/	Valor maximo 1023
// 0000 0000 0000 0011 1111 1100 0000 0000 */		/*DEPTH - 8 bit*/   Valor maximo 255
// 0000 0000 0000 1100 0000 0000 0000 0000 */		/*FLAG -  2 bit*/   Valor maximo 3
#define GuardarFlag(e, d, f)    ((e) | ((d) << 10) | ((f) << 18))       /* Guarda Depth, Edad y flag en macro en una variable */
#define EDAD(e)		((e) & 0x3FF)					/* 10bit - rango del 0 al 1023 */
#define DEPTH(d)	(((d)>>10) & 0xFF)				/* 8bit - rango del 0 al 255 */
#define FLAG(f)		(((f)>>18) & 0x3)				/* 2bit = 1 o 2 o 3 (ALPHA, BETA, EXACT) */
#define CELDAS		2

/* Valor de las piezas en el tablero */
#define PeonB           1													/* Valor del peon blanco en el tablero */
#define CaballoB        2													/* Valor del caballo blanco en el tablero */
#define AlfilB          3													/* Valor del alfil blanco en el tablero */
#define TorreB          4													/* Valor de la torre blanco en el tablero */
#define DamaB           5													/* Valor de la dama blanco en el tablero */
#define ReyB            6													/* Valor del rey blanco en el tablero */
#define CasillaVacia    7   												/* Valor vacio en el tablero */
#define PeonN           8       											/* Valor del peon negro en el tablero */
#define CaballoN        9													/* Valor del caballo negro en el tablero */
#define AlfilN          10													/* Valor del alfil negro en el tablero */
#define TorreN          11													/* Valor de la torre negro en el tablero */
#define DamaN           12													/* Valor de la dama negro en el tablero */
#define ReyN            13													/* Valor del rey negro en el tablero */
#define NoPieza         14

/* Enumeracion para saber que enroques estan permitidos o esta realizado */
#define Enroque         -1  											/* Indica si el enroque se ha realizado */
#define Ninguno         0												/* Ya no hay posibilidad de enroque (Enroque perdido) */
#define Corto           1												/* Solo enroque corto */
#define Largo           2												/* Solo enroque largo */
#define LosDos          3												/* Largo y corto */
/* Enumeracion para obtener todos los movimientos o Capturas y coronaciones */
#define Todos           1		    									/* Generara todas los movimientos posibles */
#define QsCapturas      2   	        								/* Generar todas las capturas y coronaciones posibles */

/* Valor de las piezas para see */
#define SeePeonValor		100								/* Valor del peon */
#define SeeCaballoValor		325								/* Valor del caballo */
#define SeeAlfilValor		325								/* Valor del alfil */
#define SeeTorreValor		500								/* Valor de la torre */
#define SeeDamaValor		1000							/* Valor de la dama */
#define SeeReyValor			10000							/* Valor del rey */

/* Valores para ordenar los movimientos */
#define ORDENAR_HASH				32700
#define	ORDENAR_CORONACION_DAMA		32500
#define	ORDENAR_CORONACION			30000
#define	ORDENAR_CAPTURAS			27500
#define ORDENAR_KILL_MATE			25000
#define	ORDENAR_KILL				22500
#define	ORDENAR_REFUTACION			20000

#define VALOR_MATE              32000
#define VALOR_MATE_MAX          (VALOR_MATE - MAX_PLY)
#define VALOR_EMPATE	        0
#define VALOR_TB_VACIO	        (VALOR_MATE + 1)

#define ASPIRATION_WINDOWS
#ifdef ASPIRATION_WINDOWS
	#define ASPIRATION          10
#endif
#define USAR_IDD
#define USAR_MATE_DISTANCE_PRUNING
#define USAR_HASH_PRUNING
#define USAR_BETA_PRUNING
#define USAR_NULL_MOVE_PRUNING
#define USAR_LATE_MOVE_PRUNING
#define USAR_FUTILITY_PRUNING
#define USAR_STATIC_EXCHANGE_EVALUATION_PRUNING
#define USAR_EXTENSIONS
#define USAR_REDUCTION
#define USAR_HASH_TB
#define TT_DESCONOCIDO      0
#define TT_EXACTO 	        1
#define TT_ALPHA            2
#define TT_BETA             3
#endif
