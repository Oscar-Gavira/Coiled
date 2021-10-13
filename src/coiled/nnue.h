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

#ifndef NNUE_H
#define NNUE_H

#include "Externo.h"

#ifdef USAR_NNUE

/* Valor de las piezas para consultas NNUE */
enum NNUE_Piezas
{
	Nnue_wking = 1, Nnue_wqueen = 2, Nnue_wrook = 3, Nnue_wbishop = 4, Nnue_wknight = 5, Nnue_wpawn = 6,
	Nnue_bking = 7, Nnue_bqueen = 8, Nnue_brook = 9, Nnue_bbishop = 10, Nnue_bknight = 11, Nnue_bpawn = 12,
};

/**************************************************************************************
Metodos
**************************************************************************************/
int Cargar_nnue_dll();
void CargarNnue();
int Descargar_nnue_dll();

#endif

#endif
