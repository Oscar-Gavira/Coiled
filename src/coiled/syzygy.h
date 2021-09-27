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

#ifndef TBSYZYGY_H
#define TBSYZYGY_H

#include "Externo.h"

#ifdef USAR_TBSYZYGY

typedef struct tag_Syzygy
{
	int Usar;
	U64 Acierto;
	int Dll_Cargada;
	char Directorio[MAX_DIR];
	int DirectorioNuevo;
	int Limite;
} _ST_Syzygy;

_ST_Syzygy Syzygy;

#define SG_LOSS                     0 
#define SG_DRAW                     2
#define SG_WIN                      4

#define SG_RESULT_WDL_MASK          0x0000000F
#define SG_RESULT_TO_MASK           0x000003F0
#define SG_RESULT_FROM_MASK         0x0000FC00
#define SG_RESULT_PROMOTES_MASK     0x00070000
#define SG_RESULT_EP_MASK           0x00080000
#define SG_RESULT_DTZ_MASK          0xFFF00000
#define SG_RESULT_WDL_SHIFT         0
#define SG_RESULT_TO_SHIFT          4
#define SG_RESULT_FROM_SHIFT        10
#define SG_RESULT_PROMOTES_SHIFT    16
#define SG_RESULT_EP_SHIFT          19
#define SG_RESULT_DTZ_SHIFT         20

#define SG_GET_WDL(_res)                        \
    (((_res) & SG_RESULT_WDL_MASK) >> SG_RESULT_WDL_SHIFT)
#define SG_GET_TO(_res)                         \
    (((_res) & SG_RESULT_TO_MASK) >> SG_RESULT_TO_SHIFT)
#define SG_GET_FROM(_res)                       \
    (((_res) & SG_RESULT_FROM_MASK) >> SG_RESULT_FROM_SHIFT)
#define SG_GET_PROMOTES(_res)                   \
    (((_res) & SG_RESULT_PROMOTES_MASK) >> SG_RESULT_PROMOTES_SHIFT)
#define SG_GET_EP(_res)                         \
    (((_res) & SG_RESULT_EP_MASK) >> SG_RESULT_EP_SHIFT)
#define SG_GET_DTZ(_res)                        \
    (((_res) & SG_RESULT_DTZ_MASK) >> SG_RESULT_DTZ_SHIFT)

#define SG_SET_WDL(_res, _wdl)                  \
    (((_res) & ~SG_RESULT_WDL_MASK) |           \
     (((_wdl) << SG_RESULT_WDL_SHIFT) & SG_RESULT_WDL_MASK))

#define SG_RESULT_FAILED            0xFFFFFFFF
#define SG_RESULT_CHECKMATE         SG_SET_WDL(0, SG_WIN)
#define SG_RESULT_STALEMATE         SG_SET_WDL(0, SG_DRAW)

/**************************************************************************************
METODOS DISPONIBLE DE LA DLL
**************************************************************************************/
typedef U64 (CDECL *SG_LARGEST);
typedef int (CDECL *SG_INITS) (const char *ruta);
typedef void(CDECL *SG_FREE)(void);
typedef unsigned (CDECL *SG_PROBE_WDL) (
	U64 white,
	U64 black,
	U64 kings,
	U64 queens,
	U64 rooks,
	U64 bishops,
	U64 knights,
	U64 pawns,
	unsigned ep,
	int     turn);
typedef unsigned (CDECL *SG_PROBE_ROOT) (
	U64 white,
	U64 black,
	U64 kings,
	U64 queens,
	U64 rooks,
	U64 bishops,
	U64 knights,
	U64 pawns,
	unsigned rule50,
	unsigned ep,
	int     turn,
	unsigned *results);

SG_INITS SG_inits;							/* Inicializa la tabla de finales */
SG_FREE SG_free;							/* Libera la tabla de finales */
SG_PROBE_ROOT SG_probe_root;				/* Acceso a las tablas de finales en root. */
SG_PROBE_WDL SG_probe_wdl;					/* Acceso a las tablas de finales en la búsqueda. */
SG_LARGEST SG_man;							/* Indica la tablas disponibles. 3, 4, 5, 6 o 7 piezas. */

/**************************************************************************************
Métodos
**************************************************************************************/
int Cargar_Syzygy_dll();
int Descargar_Syzygy_dll();
unsigned ProbarSyzygy(int WDL, int *mov);
void Iniciar_Mascara();

#endif

#endif
