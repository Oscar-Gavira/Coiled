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
#include "Definiciones.h"

#ifndef ESTRUCTURAS_H
#define ESTRUCTURAS_H

/******************************************************************************
Estructuras
******************************************************************************/

#ifdef USAR_SQLITE
typedef struct sqlite3_stmt sqlite3_stmt;
typedef struct sqlite3 sqlite3;

typedef int (CDECL *SQLITE3_OPEN_V2)	(
	const char *filename,   /* Database filename (UTF-8) */
	sqlite3 **ppDb,         /* OUT: SQLite db handle */
	int flags,              /* Flags */
	const char *zVfs        /* Name of VFS module to use */
	);
/* Reinicia - Recarga la nueva informacion (Unido a tbcahe_restart) */
typedef int (CDECL *SQLITE_PREPARE_V2) (
	sqlite3 *db,            /* Database handle */
	const char *zSql,       /* SQL statement, UTF-8 encoded */
	int nByte,              /* Maximum length of zSql in bytes. */
	sqlite3_stmt **ppStmt,  /* OUT: Statement handle */
	const char **pzTail     /* OUT: Pointer to unused portion of zSql */
	);
typedef int (CDECL *SQLITE_STEP) (sqlite3_stmt*);
typedef const unsigned char *(CDECL *SQLITE_COLUMN_TEXT) (sqlite3_stmt*, int iCol);
typedef int (CDECL *SQLITE_RESET) (sqlite3_stmt* pStmt);
typedef int (CDECL *SQLITE_CLOSE_V2) (sqlite3*);

typedef struct tag_EstructuraBd {
	sqlite3 *ConexionBD;											/* Puntero para conectar con la base de datos tipo FILE para archivos */
	sqlite3_stmt *stmt;												/* Para obtener informacion de la tabla, columnas, filas, datos... (En .net seria un DataSet) */

	int FinVariacion;												/* Si ya no encuentra jugada en el libro. true = dejamos de buscar / false = todavia pueden haber */
	int UsarLibro;													/* Si podemos usar el libro de aperturas o no */
	int LimiteJugadas;												/* Si podemos usar el libro de aperturas o no */

	char Apertura[MAX_LONG + 1];									/* Almacena los movimientos realizados en la apertura */
	char Jugada[5];													/* La jugada proporcionada por la Bd segun la apertura */
	char Sql[MAX_DIR];												/* Sentencia Sql segun version */

	char SqlTabla[9];												/* Tablas: Book y Chess960. Nombre de la tabla donde se almacenan las aperturas */
	char Variante[9];												/* Almacena la posicion NRQNBKRB, RNQBBNKR, BQNBRKNR para asi buscar el tablero y obtener la apertura Ajedrez960 */
	int AperturaEstandar;											/* True = Estandar, False = Ajedrez960. Indica si obtiene la jugada del libro de aperturas estandar o de Ajedrez960 */
	int Dll_Cargada;												/* Esta cargada la DLL */
} _ST_EstructuraBd;
#endif

#ifdef USAR_NNUE
typedef int (CDECL *NNUE_INIT)(const char* ruta);
typedef int (CDECL *NNUE_EVALUATE)(int player, int* pieces, int* squares);

typedef struct tag_nnue {
	int Usar;															/* Se puede usar la tablas de gaviota (true/false) */
	int Dll_Cargada;													/* Esta cargada la DLL */
	int Tecnologia;														/* 1 = SSE2	2 = SSE3	3 = SSE4.1		4 = AVX2 */
	char Directorio[MAX_DIR];											/* Obtenemos las rutas a las tablas de finales */
	int DirectorioNuevo;												/* Indica si el directorio es diferente (Nueva NNUE) */
} _ST_Nnue;
#endif

#ifdef USAR_HASH_TB
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
#endif

#ifdef USAR_TABLAS_DE_FINALES
/* egbb */
typedef int (CDECL *EGBB_PROBE_EGBB) (int player, int* piece, int* square);
typedef int (CDECL *EGBB_LOAD_EGBB) (char* path, int cache_size, int load_options);

/* syzygy */
typedef U64(CDECL *SG_LARGEST);
typedef int (CDECL *SG_INITS) (const char *ruta);
typedef void(CDECL *SG_FREE)(void);
typedef unsigned (CDECL *SG_PROBE_WDL) (U64 white, U64 black, U64 kings, U64 queens, U64 rooks, U64 bishops, U64 knights, U64 pawns, unsigned ep, int turn);

/* tbprobe */
/* Inicia la carga de la informacion */
typedef char *(CDECL *TBINIT) (int verbosity, int compression_scheme, const char **paths);
/* Reinicia - Recarga la nueva informacion (Unido a tbcahe_restart) */
typedef char *(CDECL* TBRESTART) (int verbosity, int compression_scheme, const char **paths);
/* Descarga la tb */
typedef void (CDECL *TBDONE) (void);
/* Busca informacion en la cache y si no encuentra ira al hdd (El comun a usar) */
typedef int (CDECL *TBPROBE_HARD) (unsigned stm, unsigned epsq, unsigned castles, const unsigned *inp_wSQ, const unsigned *inp_bSQ, const unsigned char *inp_wPC, const unsigned char *inp_bPC, unsigned *tbinfo, unsigned *plies);
typedef int (CDECL *TBPROBE_SOFT) (unsigned stm, unsigned epsq, unsigned castles, const unsigned *inp_wSQ, const unsigned *inp_bSQ, const unsigned char *inp_wPC, const unsigned char *inp_bPC, unsigned *tbinfo, unsigned *plies);
typedef int (CDECL *TBIS_INITIALIZED) (void);
/* Obtenemos informacion acerca de la tablas cargadas */
typedef unsigned int (CDECL *TBAVAILABILITY) (void);
/* Inicia la cache */
typedef int (CDECL *TBCACHE_INIT) (size_t cache_mem, int wdl_fraction);
/* Descarga la cache */
typedef void (CDECL *TBCACHE_DONE) (void);
typedef int (CDECL *TBCACHE_IS_ON) (void);
typedef const char **(CDECL *TBPATHS_INIT) (void);
typedef const char **(CDECL *TBPATHS_ADD) (const char **ps, const char *newpath);
typedef const char **(CDECL *TBPATHS_DONE) (const char **ps);

typedef struct tag_TablaDeFinales
{
	int Usar;														/* 0 = None		1 = Syzygy		2 = Gaviota		3 = BitBases */
	int UsarNuevo;													/* Indica si hay cambio de tablas de finales */
	U64 Acierto;													/* Cuando buscamos en la tabla y encontramos resultados, se va incrementando */
	int Dll_CargadaSg;												/* Esta cargada la DLL Syzygy */
	int Dll_CargadaGv;												/* Esta cargada la DLL Gaviota */
	int Dll_CargadaBb;												/* Esta cargada la DLL BitBases */
	unsigned int Piezas;											/* Que tablas de finales estan disponibles 3 o 4 o 5 o 6 piezas */
	char Directorio[MAX_DIR];										/* Obtenemos las rutas a las tablas de finales */
	int DirectorioNuevo;											/* Indica si hay cambio de ruta a las tablas de finales */
	U64 CacheMB;													/* 32 MB */
	int CacheNueva;													/* Indica si hay un cambio en el tamana de la cache true/false */
	int Limite;														/* Indica a partir de que numero de pieza busca en las tablas de finales */
	const char **paths;												/* Gaviota. Rutas para acceder a las tablas de finales */
} _ST_TablaDeFinales;
#endif

typedef struct tag_TipoJuego
{
	int Interrumpir;					/* false o true - Cuando se excede el tiempo o se recibe el comando stop */
	float Tiempo;						/* Tiempo ideal por movimiento. */
	float TiempoMax1;					/* Tiempo medio por movimiento. */
	float TiempoMax2;					/* Tiempo maximo por movimiento. */
	int TiempoFactor;					/* Estabilidad de la Pv */
	int PrevenirTiempoExcedido;			/* Para evitar un Timeout en un juego por tiempo. Descuenta tiempo en ms al tiempo de busqueda, para poder enviar el mejor movimiento sin exceder el tiempo limite. */
	U64 Inicio;							/* Tiempo inicial. */
	U64 TiempoTrascurrido;				/* Para detectar si excedemos el tiempo limite por jugada */
	int Activo;							/* Si el juego es por 1 = tiempo. 2 = movetime. 0 = Mate, Infinite, profundidad (depth) */
	int Infinito;						/* Activa el modo Analisis por asi decirlo, piensa hasta recibir un stop o una entrada */
	int MaxDepth;						/* Maxima profundidad. Busqueda tipo go depth 15*/
	int DepthAct;						/* Depth actual */
	U64 Nodos;							/* Nodos totales */
	int MejorJugada;					/* Mejor jugada */
	int MejorJugadaAdv;					/* Mejor jugada para el adversario */
	int BuscarMate;						/* Busca hasta localizar un mate en x */
	int Ajedrez960;						/* Activa/desactiva el modo de juego Ajedrez960 */
	int Ajedrez960Enroque;				/* Valor de 0 = UCI estandar. Valor de 1 O-O/O-O-O GUI Arena */
	int JugadaIlegal;					/* Se activa si recibimos un movimiento incorrecto position fen rkbbnrqn/pppppppp/8/8/8/8/PPPPPPPP/RKBBNRQN w KQkq - 0 1 moves h1g3 .... f7f8*/
} _ST_TipoJuego;

typedef struct tag_Movimiento
{
	int Movimiento;						/* Movimiento en macro */
	int Ordenar;						/* Valor numerico para ordenar las jugadas */
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
    int PosicionReyB;					/* Indice del vector, posicion del rey Blanco */
	int PosicionReyN;					/* Indice del vector, posicion del rey Negro */
	int EnroqueB;						/* Enroque permitido con torres */
	int EnroqueN;						/* Enroque permitido con torres */
	int FichaAlPasoPosicion;			/* Indice del vector, pieza al paso. */
	int Regla_50_Movimiento;			/* Regla 50 movimientos */
} _ST_DeshacerMovimiento;

/***********************************************************************************
Tipo para almacenar datos de la evaluacion
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
	_Valor Puntos;						/* Puntos totales. pst, movilidad, ataques, pareja de alfiles... */

	int PosicionTorre[8];				/* Se van anadiendo segun se encuentren. ( Si solo hay dos torres estaran en [0] y [1] ). Valor = Indice del vector. Valor vacio = -1  */
	int PeonDoblados[8];				/* Almacena un valor por cada columna del vector (Horizontal A... H). Detectando si hay peones doblados siendo el valor > 1 doblado. Valor vacio 0 */
	int GrupoPeonesQ;					/* Indica si hay peones en la columna a,b,c,d. */
	int GrupoPeonesK;					/* Indica si hay peones en la columna e,f,g,h. */

	/* Seguridad del rey */
	int ReyCuadrosAtacando;				/* Numero de cuadros atacando al rededor del rey */
	int ReyAtaquesPiezas;				/* Numero de piezas atacando el area del rey */
	_Valor ReyAtaquesValor;				/* Valor de las piezas que atacan */
	int ReyJaquePieza[5];				/* Pieza que genera jaque. */
	int ReyJaque;						/* Indica si hay jaque */
} _ST_Puntos;

/* Estructura del tablero */
typedef struct tag_TableroX64 {
    int PosicionReyB;					/* Indice del vector, posicion del rey Blanco */
	int PosicionReyN;					/* Indice del vector, posicion del rey Negro */
	int EnroqueB;						/* Enroque permitido con torres */
	int EnroqueN;						/* Enroque permitido con torres */
	_ST_Ajedrez960 Ajedrez960;			/* Posicion de las torres. Desde el rey hasta la torre no puede haber ninguna ficha ni pasar por casilla amenazada por el adversario. */
	int FichaAlPasoPosicion;			/* Indice del vector, pieza al paso. */

	int Ply;							/* Ply */
	int MueveBlancas;					/* turno de jugador */
	int Regla_50_Movimiento;			/* Regla 50 movimientos */
	int Etapa;							/* Estado del juego. Apertura. medio juego, final. */
	_ST_DeshacerMovimiento Estado[MAX_PLY];/* Almacena los avances de la jugada, para poder deshacer despues */

#ifdef USAR_HASH_TB
	U64 Historico[MAX_HISTORICO];		/* 800 movimientos. Lo normal son 200 o 250 jugadas, que es igual a 400 o 500 movimientos*/
	int Hply;							/* Contador incremental de los movimientos que se van realizando en la variable Historico */
	U64 Hash;							/* Hash del tablero */
#endif

	int TableroColor[64];				/* Para detectar los margenes del tablero */
	int Tablero[64];					/* Array donde estan las piezas en el tablero */
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
