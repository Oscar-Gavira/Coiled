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

#ifndef LIBROAPERTURAS_H
#define LIBROAPERTURAS_H

#include "Externo.h"
#include "Utilidades.h"
#include "GeneradorDeMovimientos.h"

#ifdef USAR_SQLITE

typedef struct sqlite3_stmt sqlite3_stmt;
typedef struct sqlite3 sqlite3;

#define SQLITE_OPEN_READONLY         0x00000001  /* Ok for sqlite3_open_v2() */
#define SQLITE_OPEN_URI              0x00000040  /* Ok for sqlite3_open_v2() */

#define SQLITE_OK           0   /* Successful result */
#define SQLITE_ERROR        1   /* Generic error */
#define SQLITE_ROW         100  /* sqlite3_step() has another row ready */
#define SQLITE_DONE        101  /* sqlite3_step() has finished executing */

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
typedef const unsigned char* (CDECL* SQLITE_COLUMN_TEXT) (sqlite3_stmt*, int iCol);
typedef int (CDECL *SQLITE_RESET) (sqlite3_stmt* pStmt);
typedef int (CDECL *SQLITE_CLOSE_V2) (sqlite3*);

SQLITE3_OPEN_V2 sqlite3_open_v2;
SQLITE_PREPARE_V2 sqlite3_prepare_v2;
SQLITE_STEP sqlite3_step;
SQLITE_COLUMN_TEXT sqlite3_column_text;
SQLITE_RESET sqlite3_reset;
SQLITE_CLOSE_V2 sqlite3_close_v2;

#define MAX_LONG 100												/* longitud maxima del contenido de jugadas en el libro de aperturas */
/******************************************************************************
							Estructura
******************************************************************************/
/* Manejo de SqlIte */
typedef struct tag_EstructuraBd
{
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
	int AperturaEstandar;											/*	True = Estandar, False = Ajedrez960. Indica si obtiene la jugada del libro de aperturas estandar o de Ajedrez960 */
	int Dll_Cargada;												/* Esta cargada la DLL */
} _ST_EstructuraBd;
/******************************************************************************
							Variables
******************************************************************************/
_ST_EstructuraBd LibroSql;											/* Manejo del libro de aperturas */

/***********************************************************************************
							Metodos
***********************************************************************************/
int Cargar_sqlite_dll();											/* Cargamos la libreria para manejar el libro de aperturas sqlite */
int Descargar_sqlite_dll();											/* Descargamos la libreria sqlite */
int ComprobarAccesoLibro();											/* Comprobamos si podemos acceder a la base de datos (Libro de apertura) */
void ListaJugadas(char *_Jugadas, int longitud);					/* Lista de los movimientos realizados hasta el momento (e2e4 c7c5 g1f3 d7d6 f1b5 b8c6...)
																	   para realizar la busqueda en la base de datos con LIKE %	*/
void ObtenerJugadaLibro(char *move);								/* Obtenemos el movimiento a realizar obtenido de la Bd. */
int BuscarJugadaLibro(char *Str);											/* Buscamos en la Bd si existe una variacion. */
void Ajedrez960EnroqueVariante();
void CerrarBaseDeDatos();											/* Cerramos la conexion con la Bd. */
void VerificarLibroApertura();										/* Rutina para verificar las aperturas de la Bd.
																	   Analiza una por una todas las aperturas y de cada apertura todas las jugadas que contiene para saber si hay algun movimiento mal. */
#endif

#endif
