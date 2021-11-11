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

#include "Estructuras.h"

#ifndef TIEMPO_H
#define TIEMPO_H

U64 ObtenerTiempo();
U64 TiempoTrascurrido(_ST_TipoJuego *Tiempo);
void TiempoInicio(_ST_TipoJuego *Tiempo, int Movestogo, int TiempoTotal, int inc);
int TiempoActualizar(_ST_TipoJuego *Tiempo, int Valor, int ValorAnterior, int MejorJugada, int MejorJugadaAnterior, int depth);
int TerminarTiempoLimite(_ST_TipoJuego *Tiempo);

#endif

