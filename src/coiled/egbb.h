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

#ifndef EGBBDLL_H
#define EGBBDLL_H

#include "Externo.h"

#ifdef USAR_TABLAS_DE_FINALES

/**************************************************************************************
						METODOS DISPONIBLE DE LA DLL
**************************************************************************************/
typedef int (CDECL *EGBB_PROBE_EGBB) (int player, int *piece, int *square);
typedef int (CDECL *EGBB_LOAD_EGBB) (char *path, int cache_size, int load_options);

EGBB_LOAD_EGBB EGBB_load_egbb;							/* Cargamos la tablas de finales. */
EGBB_PROBE_EGBB EGBB_probe_egbb;						/* Acceso a las tablas de finales. */

/**************************************************************************************
								Metodos
**************************************************************************************/
int Cargar_egbb_dll();
void CargarEgbb();
int Descargar_egbb_dll();
int Probar_egbb(int *puntos);
#endif

#endif
