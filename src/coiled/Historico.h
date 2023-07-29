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

#ifndef HISTORICO_H
#define HISTORICO_H

#include "Externo.h"

/*******************************************************************************
									Metodos
*******************************************************************************/
void HistoricoIniciar(_ST_TableroX64 *Tablero);																/* Inicializar Historico y killer heuristico */
int MovimientoTranquilo(int *M);																			/* Es un movimiento que no genera cambio de material */
int MovimientoKiller(int *M, _ST_TableroX64 *Tablero);														/* Indica si el movimiento es un killer o killer mate */
int HistoricoEsMovimientoKiller(int *M, _ST_TableroX64 *Tablero);											/* Indica si el movimiento es un killer */
int HistoricoEsMovimientoKillerMate(int *M, _ST_TableroX64 *Tablero);										/* Indica si el movimiento es un killer mate */
void HistoricoActualizar(int depth, int *ply, int M, int kMate, int ML[], int Nmov, _ST_TableroX64 *Tablero);/* Actualizacion historico para el mejor movimiento y posible killer. */
void HistoricoActualizarCapturas(int depth, int M, int MC[], int Ncap, _ST_TableroX64 *Tablero);			/* Actualizacion historico captura */
int HistoricoMovimientoRefutacion(_ST_TableroX64 *Tablero);													/* Obtenemos el movimiento refutacion */
int HistoricoValor(int *M, _ST_TableroX64 *Tablero);														/* Historico valor de los movimientos */
int HistoricoCapturaValor(int *M, _ST_TableroX64 *Tablero);													/* Historico capturas valor */
#endif
