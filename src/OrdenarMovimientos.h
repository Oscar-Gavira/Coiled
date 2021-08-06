/*
Coiled-NNUE is a UCI chess playing engine authored by Oscar Gavira.
Copyright (C) 2013-2021 Oscar Gavira.
<https://github.com/Oscar-Gavira/Coiled-NNUE>

Coiled-NNUE is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Coiled-NNUE is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef ORDERNARMOVIMIENTOS_H
#define ORDERNARMOVIMIENTOS_H

#include "Externo.h"
#include "Utilidades.h"
#include "Historico.h"
#include "GeneradorDeMovimientos.h"

/* Valores para ordenar los movimientos */
#define ORDENAR_HASH				(6000000)
#define	ORDENAR_CAPTURAS			(5000000)
#define	ORDENAR_CORONACION			(4000000)
#define	ORDENAR_CORONACION_CAPTURA	(3500000)
#define ORDENAR_KILL_MATE			(2000000)
#define	ORDENAR_KILL				(1000000)

extern void OrdenarMovimientosTodos(int *Capacidad, int *hMov, _ST_Movimiento *M);			/* Asignamos valores a los movimientos generados */
extern void OrdenaMovimiento(int *indice, int *Capacidad, _ST_Movimiento *M);				/* Ordenamos el movimiento según su valor de mayor a menor */
#endif