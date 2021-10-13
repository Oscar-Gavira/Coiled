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

#ifdef USAR_TABLAS_DE_FINALES

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

/**************************************************************************************
								Metodos
**************************************************************************************/
int Cargar_gaviota_dll();
void CargarGaviotaTB();
void CacheGaviotaTB();
int Descargar_gaviota_dll();
unsigned Probar_gaviota(int *puntos, int *ply);
#endif

#endif
