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

#ifndef TIEMPO_H
#define TIEMPO_H

#include "Externo.h"

/******************************************************************************
							Metodos
******************************************************************************/
int ObtenerTiempo();																									/* Obtenemos tiempo */
int TiempoTranscurrido(_ST_TipoJuego *Tiempo);																			/* Obtenemos el tiempo trascurrido */
void TiempoInicio(_ST_TipoJuego *Tiempo, int Movestogo, int TiempoTotal, int inc);										/* Configuracion del tiempo */
void TiempoActualizar(_ST_TipoJuego *Tiempo, int Valor, int ValorAnterior, int MejorJugada, int MejorJugadaAnterior);	/* Incrementamos tiempo y cortamos la busqueda */
int TerminarTiempoIdeal(_ST_TipoJuego *Tiempo);																			/* Fin de la busque si pasamos del tiempo ideal. */
void TerminarTiempoLimite(_ST_TipoJuego *Tiempo);																		/* Tiempo limite */

#endif
