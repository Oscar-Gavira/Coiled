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

#ifndef ALPHABETA_H
#define ALPHABETA_H

#include "Externo.h"
#include "Historico.h"
#include "Hash.h"
#include "See.h"
#include "Utilidades.h"
#include "OrdenarMovimientos.h"
#include "GeneradorDeMovimientos.h"
#include "Fen.h"

#if defined(USAR_TBPROBE)
	#include "tbprobe.h"
#endif

/******************************************************************************
							Métodos
******************************************************************************/
int AspirationWindows(int depth, int en_jaque, int PuntuacionAnterior);							/* Rutina Aspiration Windows */
int AlphaBeta(int depth, int alpha, int beta, int en_jaque, int Es_Nulo);						/* Rutina AlphaBeta	Vp */
int AlphaBeta_Qs(int alpha, int beta);															/* Rutina AlphaBeta	Quiescence */
void Actualizar_Vp(int M);																		/* Actualiza la variante principal */
void Actualizar_Vp_Raiz();
void ImprimirVp(int M_Depth, int *informado, int puntos, int lowerbound);						/* Imprimimos la variante principal, entre otros datos */
void ExtraInfo(int *M, int *currmovenumber);													/* Información extra. */
void ImprimirMejorJugada(int Jugada, int J_Adv);												/* Imprimimos la mejor jugada y ponder */
void ComprobarTiempo();																			/* Comprobamos tiempo y entradas */
int FigurasAdversarioNull(int turno);															/* Comprueba si se puede hacer Movimiento nulo, si el adversario dispone de 2 figuras */
int EsPuntuacionMate(int puntos);																/* Obtenemos si la puntuación esta dentro del margen de mate */
int Es_Tabla_Repeticion(int *en_jaque);															/* Obtenemos si es tablas por repetición */
#endif
