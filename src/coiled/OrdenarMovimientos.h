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

#ifndef ORDERNARMOVIMIENTOS_H
#define ORDERNARMOVIMIENTOS_H

#include "Externo.h"
#include "Historico.h"
#include "See.h"

void OrdenarMovimientosPorValor(int *hMov, _ST_Movimiento *M, _ST_TableroX64 *Tablero);			/* Asignamos valores a los movimientos generados */
void ObtenerMovimiento(int *indice, _ST_Movimiento *M, _ST_TableroX64 *Tablero);				/* Ordenamos el movimiento segun su valor de mayor a menor */
#endif
