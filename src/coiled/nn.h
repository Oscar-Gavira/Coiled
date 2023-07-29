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

#ifndef NN_H
#define NN_H

#include "Utilidades.h"
#include "../nnue/nnue.h"
#ifdef USAR_NNUE

enum NNUE_Piezas					/* Valor de las piezas para consultas NNUE */
{
	Nnue_wking = 1, Nnue_wqueen = 2, Nnue_wrook = 3, Nnue_wbishop = 4, Nnue_wknight = 5, Nnue_wpawn = 6,
	Nnue_bking = 7, Nnue_bqueen = 8, Nnue_brook = 9, Nnue_bbishop = 10, Nnue_bknight = 11, Nnue_bpawn = 12,
};

/*******************************************************************************
									Metodos
*******************************************************************************/
int ProbarNNUE(_ST_TableroX64 *Tablero);
int InfoNnueTechnology();				/* Cargamos la libreria dinamica, para usar de la red neuronal. */
int CargarNnue();					/* Cargamos la red neuronal. */
int DescargarNnueDll();				/* Descargamos la libreria dinamica. */

#endif

#endif
