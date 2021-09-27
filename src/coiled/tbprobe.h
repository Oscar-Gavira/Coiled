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

#ifndef TBPROBE_H
#define TBPROBE_H

#include "Externo.h"

#ifdef USAR_TBPROBE

/**************************************************************************************
						ENUMERACIONES/ESTRUCTURA
			Internas para manejar la tablas bases Gaviota
**************************************************************************************/
enum TBvalores														/* Valores devueltos por la tabla de finales */
{
   TBDRAW = 0,														/* Valor de empate */
   TBWMATE = 1,														/* Valor ganan las blancas */
   TBBMATE = 2,														/* Valor ganan las negras */
   TBFORBID = 3,													/* Valor a descartar */
   TBUNKNOWN = 7													/* No existe en la tabla de finales */
};

#define TB_LOSS                     0
#define TB_DRAW                     1
#define TB_WIN                      2

/**************************************************************************************
						VARIABLE PARA MANEJAR GTB
**************************************************************************************/
typedef struct tag_Gaviota
{
	int Usar;														/* Se puede usar la tablas de gaviota (true/false) */
	U64 Acierto;													/* Cuando buscamos en la tabla y encontramos resultados, se va incrementando */
	int Dll_Cargada;												/* Esta cargada la DLL */
	unsigned int Tablas_Disponibles;								/* Que tablas de finales están disponibles */
	char Directorio[MAX_DIR];										/* Obtenemos las rutas a las tablas de finales */
	int DirectorioNuevo;											/* Indica si hay cambio de ruta a las tablas de finales */
	int	Compresion;													/* Compresión de las tablas de finales */
	int Informacion;												/* Muestra información true/false */
	U64 CacheMB;													/* 32 MB */
	int CacheNueva;													/* Indica si hay un cambio en el tamaña de la cache true/false */
	int Fraccion;													/* Fracción de tipo WDL y DTZ */
	const char **paths;												/* Rutas para acceder a las tablas de finales */
	int Limite;														/* Indica a partir de que numero de pieza busca en las tablas de finales */
} _ST_Gaviota;

_ST_Gaviota Gaviota;
/**************************************************************************************
						METODOS DISPONIBLE DE LA DLL
**************************************************************************************/
/* Inicia la carga de la información */
typedef char * (CDECL *TBINIT)	(int verbosity, int compression_scheme, const char **paths);
/* Reinicia - Recarga la nueva información (Unido a tbcahe_restart) */
typedef char * (CDECL *TBRESTART) (int verbosity, int compression_scheme, const char **paths);
/* Descarga la tb */
typedef void (CDECL *TBDONE) (void);
/* Busca información en la cache y si no encuentra ira al hdd (El común a usar) */
typedef int (CDECL *TBPROBE_HARD) (unsigned stm, unsigned epsq, unsigned castles, const unsigned *inp_wSQ, const unsigned *inp_bSQ, const unsigned char *inp_wPC, const unsigned char *inp_bPC, unsigned *tbinfo, unsigned *plies);
typedef int (CDECL *TBPROBE_SOFT) (unsigned stm, unsigned epsq, unsigned castles, const unsigned *inp_wSQ, const unsigned *inp_bSQ, const unsigned char *inp_wPC, const unsigned char *inp_bPC, unsigned *tbinfo, unsigned *plies);
typedef int (CDECL *TBIS_INITIALIZED) (void);
/* Obtenemos información acerca de la tablas cargadas */
typedef unsigned int (CDECL *TBAVAILABILITY) (void);
/* Inicia la cache */
typedef int (CDECL *TBCACHE_INIT) (size_t cache_mem, int wdl_fraction);
/* Descarga la cache */
typedef void (CDECL *TBCACHE_DONE) (void);
typedef int (CDECL *TBCACHE_IS_ON) (void);
typedef const char ** (CDECL *TBPATHS_INIT) (void);
typedef const char ** (CDECL *TBPATHS_ADD) (const char **ps, const char *newpath);
typedef const char ** (CDECL *TBPATHS_DONE) (const char **ps);

TBINIT TBinit;									/* Inicializa la tabla de finales */
TBIS_INITIALIZED TBis_initialized;				/* Indica si la tabla esta inicializada */
TBRESTART TBrestart;							/* Reinicia la tabla de finales */
TBDONE TBdone;									/* Libera la tabla de finales */
TBPROBE_HARD TBprobe_hard;						/* Acceso a las tablas de finales en root. En HDD. (DTM) */
TBPROBE_SOFT TBprobe_soft;						/* Acceso a las tablas de finales en root. En Cache. (DTM) */
TBAVAILABILITY TBavailability;					/* Indica la tablas disponibles */
TBCACHE_INIT TBcache_init;						/* Inicia la cache y fracción */
TBCACHE_DONE TBcache_done;						/* Borra la cache */
TBCACHE_IS_ON TBcache_is_on;
TBPATHS_INIT TBpaths_init;						/* Inicializa las rutas */
TBPATHS_ADD TBpaths_add;						/* Añade rutas */
TBPATHS_DONE TBpaths_done;						/* Libera ruta */

/**************************************************************************************
								Métodos
**************************************************************************************/
int Cargar_gaviota_dll();
int Descargar_gaviota_dll();
unsigned Probar_gaviota(int *puntos, int *ply);
#endif

#endif
