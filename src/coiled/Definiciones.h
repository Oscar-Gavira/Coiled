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

#ifndef DEFINICIONES_H
#define DEFINICIONES_H

#define NOMBRE "Coiled"
#define VER  "1.1"
#define AUTOR  "Oscar Gavira"

#define USAR_SQLITE														/* Para compilar el uso de sqlite. Libro de aperturas. */
#define USAR_AJEDREZ960													/* Para compilar el uso de Ajedrez960. */
#define ARC_64BIT														/* Para compilar a 64 bit. */
#define USAR_TABLAS_DE_FINALES											/* Para compilar con las tablas de finales */

#ifdef ARC_64BIT
	#define USAR_NNUE													/* En version de 64 bit activamos la opcion de evaluacion NNUE */
#endif

//#define DEBUG															/* Activa/desactiva los mensajes en formato debug "info string ....." */
#ifdef DEBUG
	#define INFO_STRING "info string "
#else
	#define INFO_STRING ""
#endif

#ifdef _WIN32
#include <stdio.h>														/* Printf, FILE, fclose, fflush, stdout, fgets... */
#include <time.h>														/* Tiempo */
#include <ctype.h>														/* toupper */
#include <string.h>														/* Memset, strcat, strcopy */
#include <stdlib.h>														/* atoi, atoll, exit(EXIT_SUCCESS), srand, rand */
#include <stdint.h>														/* Tipo de variables y valores maximos */
#include <limits.h>														/* Valores maximos y minimos de cada tipo de variable int, short, char...*/
#include <inttypes.h>													/* Los formatos para imprimir un string, int, U32, U64... %d, %s, llu... */
#include <math.h>														/* log(x) */
#include <windows.h>													/* Necesario para vincular librerias, SqliIte y TbProbe y EntradaStdIn() */
#else
#include <stdio.h>														/* Printf, FILE, fclose, fflush, stdout, fgets... */
#include <string.h>														/* Memset, strcat, strcopy */
#include <ctype.h>														/* toupper */
#include <stdlib.h>														/* atoi, atoll, exit(EXIT_SUCCESS), srand, rand */
#include <stdint.h>														/* Tipo de variables y valores maximos */
#include <limits.h>														/* Valores maximos y minimos de cada tipo de variable int, short, char...*/
#include <inttypes.h>													/* Los formatos para imprimir un string, int, U32, U64... %d, %s, llu... */
#include <math.h>														/* log(x) */
#include <unistd.h>														/* Necesario para vincular librerias, SqliIte y TbProbe y EntradaStdIn() */
#include <sys/time.h>													/* Tiempo */
#include <dlfcn.h>
#define CDECL
#define HMODULE void*
#define LoadLibrary(x) dlopen(x,RTLD_LAZY)
#define FreeLibrary(x) dlclose(x)
#define GetProcAddress dlsym
#endif

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#include <crtdbg.h>
#pragma warning (disable : 4996)
#pragma warning (disable : 4820)
#endif

#if defined(__GNUC__) && (__GNUC__ >= 8)
#pragma GCC diagnostic ignored "-Wcast-function-type"
#endif

#if defined(ARC_64BIT)
#define U64_FORMAT "%"PRIu64""
#define S32_FORMAT "%"PRId32""
#define STRING_FORMAT "%s"
#define MAX_PLY					(128)									/* Maximos ply */
#define MB_HASH_TABLE_MAX		(4096)									/* Tamano de la tabla hash maximo */
#define MB_HASH_TABLE			(64)									/* Tamano de la tabla hash por defecto */
#define MB_HASH_TABLE_MIN		(8)										/* Tamano de la tabla hash minimo */
#ifdef USAR_TABLAS_DE_FINALES
#define MB_TABLAS_CACHE_MAX		(1024)									/* Tamano maximo de cache, para las tablas de finales de gaviota. */
#define MB_TABLAS_CACHE			(32)									/* Tamano por defecto de cache, para las tablas de finales de gaviota. */
#define MB_TABLAS_CACHE_MIN		(8)										/* Tamano minimo de cache, para las tablas de finales de gaviota. */
#define TB_LOSS                     0
#define TB_DRAW                     2
#define TB_WIN                      4
#endif

typedef uint64_t U64;
typedef int64_t S64;													/* 9.223.372.036.854.775.807 / -9.223.372.036.854.775.806. Para el historico. */

#else
#define U64_FORMAT "%"PRIu32""
#define S32_FORMAT "%"PRId32""
#define STRING_FORMAT "%s"
#define MAX_PLY					(64)									/* Maximos ply */
#define MB_HASH_TABLE_MAX		(2048)									/* Tamano de la tabla hash maximo */
#define MB_HASH_TABLE			(16)									/* Tamano de la tabla hash por defecto */
#define MB_HASH_TABLE_MIN		(2)										/* Tamano de la tabla hash minimo */
#ifdef USAR_TABLAS_DE_FINALES
#define MB_TABLAS_CACHE_MAX		(512)									/* Tamano maximo de cache, para las tablas de finales de gaviota. */
#define MB_TABLAS_CACHE			(16)									/* Tamano por defecto de cache, para las tablas de finales de gaviota. */
#define MB_TABLAS_CACHE_MIN		(4)										/* Tamano minimo de cache, para las tablas de finales de gaviota. */
#define TB_LOSS                     0
#define TB_DRAW                     2
#define TB_WIN                      4
#endif

typedef uint32_t U64;
typedef int S64;														/* 2.147.483.647 / -2.147.483.646. Para el historico. (Si alcanza mucha profundidad en la busqueda FALLOS...) */

#endif

#define ABS(x)				(((x) > (0)) ? (x) : -(x))
#define MAX(a,b)			(((a) > (b)) ? (a) : (b))
#define MIN(a,b)			(((a) < (b)) ? (a) : (b))
#define COLUMNA(x)			((x & 7))									/* Obtenemos la columna donde esta situada la pieza de A a H (0 a 7) */
#define HORIZONTAL(x)		((7 - (x >> 3)))							/* Obtenemos la linea horizontal donde esta situada la pieza 1 a 8 (0 a 7) */

#define MAX_JUGADAS			(256)										/* Numero maximo de jugadas que se pueden obtener */
#define MAX_HISTORICO		(800)										/* Maximo numero de jugadas de historico.  */
#define COMPROBAR_ENTRADAS	(1024)										/* Cada x tiempo comprobamos entradas */

#define MAX_DIR				(256)
#define START_POS			"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"
#define false (0)
#define true  (1)

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

#define MFLAGCAP  (0xF)													/* Mascara para la captura */
#define MFLAGPROM (0xF)													/* Mascara para la coronacion */
#define MFLAGMOV  (0xF)													/* Mascara para el movimiento */

#define MFLAGEP (0x4000000)												/* Captura la paso */
#define MFLAGPS (0x8000000)												/* Movimiento al paso */
#define MFLAGCA (0x10000000)											/* Enroque */

#define NO_MOVIMIENTO (0)

/* Cuadro origen, cuadro destino, pieza movida, pieza capturada, coronacion, EP/Dos adelante/Enroque */
#define MOVIMIENTO(o,d,p,c,co,f) ( (o) | ((d) << 7) | ( (p) << 14 ) | ( (c) << 18 ) | ( (co) << 22 ) | (f))

#define HASH_PIEZA(p, c) (TableroGlobal.Hash ^= zobrist_Tablero[(p)][(c)])
#define HASH_ENROQUE(e) (TableroGlobal.Hash ^= zobrist_Tablero[7][(e)])
#define HASH_LADO (TableroGlobal.Hash ^= zobrist_Tablero[7][32])
#define HASH_EP(p) (TableroGlobal.Hash ^= zobrist_Tablero[0][(p)])

#define CUADRADO_ORIGEN(m) ((m) & 0x7F)									/* Obtenemos el indice de donde esta la pieza */
#define CUADRADO_DESTINO(m) (((m)>>7) & 0x7F)							/* Obtenemos el indice de donde va la pieza */
#define PIEZAMOVIDA(m) (((m)>>14) & MFLAGMOV)							/* Devuelve la pieza (1,2,3,4,5,6 - 8,9,10,11,12,13) */
#define CAPTURADA(m) (((m)>>18) & MFLAGCAP)								/* Devuelve MFLAGCAP o la pieza (1,2,3,4,5,6 - 8,9,10,11,12,13) */
#define CORONACION(m) (((m)>>22) & MFLAGPROM)							/* Devuelve MFLAGPROM o la pieza (2,3,4,5 - 9,10,11,12) */

#define CAPTURA_ALPASO(m) (m & MFLAGEP)									/* Devuelve 0 o MFLAGEP. Indica si el movimiento es una captura al paso */
#define GENERA_ALPASO(m) (m & MFLAGPS)									/* Devuelve 0 o MFLAGPS. Indica dos a delante de un peon */
#define ENROQUE(m) (m & MFLAGCA)										/* Devuelve 0 o MFLAGCA. Indica movimiento de enroque */

// 16 bit short (2 bytes * 8) = 16 bit */
// 0000 0000 0111 1111 */		/*EDAD -  7 bit*/   Valor maximo 127
// 0001 1111 1000 0000 */		/*DEPTH - 6 bit*/   Valor maximo 63
// 0110 0000 0000 0000 */		/*FLAG -  2 bit*/   Valor maximo 3
#define EDAD(e)		((e) & 0x7F)					/* 7bit - rango del 0 al 127 */
#define DEPTH(d)	(((d)>>7) & 0x3F)				/* 6bit - rango del 0 al 63 */
#define FLAG(f)		(((f)>>13) & 0x3)				/* 2bit = 1 o 2 o 3 (ALPHA, BETA, EXACT) */
#define MAX_EDAD	(127)							/* Edad maxima */
#define CELDAS		(3)

/* Valor de las piezas en el tablero */
#define PeonB  (1)														/* Valor del peon blanco en el tablero */
#define CaballoB  (2)													/* Valor del caballo blanco en el tablero */
#define AlfilB  (3)														/* Valor del alfil blanco en el tablero */
#define TorreB  (4)														/* Valor de la torre blanco en el tablero */
#define DamaB  (5)														/* Valor de la dama blanco en el tablero */
#define ReyB  (6)														/* Valor del rey blanco en el tablero */
#define CasillaVacia  (7)												/* Valor vacio en el tablero */
#define PeonN  (8)														/* Valor del peon negro en el tablero */
#define CaballoN  (9)													/* Valor del caballo negro en el tablero */
#define AlfilN  (10)													/* Valor del alfil negro en el tablero */
#define TorreN  (11)													/* Valor de la torre negro en el tablero */
#define DamaN  (12)														/* Valor de la dama negro en el tablero */
#define ReyN  (13)														/* Valor del rey negro en el tablero */
#define NoPieza (14)

/* Enumeracion para saber que enroques estan permitidos o esta realizado */
#define Enroque (-1)													/* Indica si el enroque se ha realizado */
#define Ninguno	(0)														/* Ya no hay posibilidad de enroque (Enroque perdido) */
#define Corto	(1)														/* Solo enroque corto */
#define Largo	(2)														/* Solo enroque largo */
#define LosDos	(3)														/* Largo y corto */
/* Enumeracion para obtener todos los movimientos o Capturas y coronaciones */
#define Todos (1)				    									/* Generara todas los movimientos posibles */
#define CapturasCoronacion (2)											/* Generar todas las capturas y coronaciones posibles */

#define VALOR_MATE		(32000)
#define MATE(p)			(-VALOR_MATE + p)
#define MATE_EN(p)		(VALOR_MATE - p)
#define VALOR_EMPATE	(0)
#define VALOR_TB_VACIO	(-VALOR_MATE - 1)

#define ASPIRATION_WINDOWS
#ifdef ASPIRATION_WINDOWS
	#define ASPIRATION (15)
#endif
#define USAR_IDD
#define USAR_MATE_DISTANCE_PRUNING
#define USAR_HASH_PRUNING
#define USAR_BETA_PRUNING
#define USAR_NULL_MOVE_PRUNING
#define USAR_LATE_MOVE_PRUNING
#define USAR_FUTILITY_PRUNING_HISTORY
#define USAR_STATIC_EXCHANGE_EVALUATION_PRUNING
#define USAR_EXTENSIONS
#define USAR_REDUCTION
#define USAR_HASH_TB
#define TT_DESCONOCIDO	(0)
#define TT_EXACTO 		(1)
#define TT_ALPHA		(2)
#define TT_BETA			(3)

#endif
