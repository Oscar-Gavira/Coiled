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

#ifndef HISTORICO_H
#define HISTORICO_H

/*******************************************************************************
									Métodos
*******************************************************************************/
void HistoricoIniciar();															/* Inicializar Histórico y killer heurístico */
void HistoricoActualizar(int depth, int *ply, int M, int kMate, int *ML, int Nmov);	/* Actualización histórico para el mejor movimiento y posible killer. */
int MovimientoTranquilo(int *M);													/* Es un movimiento que no genera cambio de material */
int MovimientoKiller(int *ply, int *M);												/* Indica si el movimiento es un killer o killer mate */
int HistoricoEsMovimientoKiller(int *ply, int *M);									/* Indica si el movimiento es un killer */
int HistoricoEsMovimientoKillerMate(int *ply, int *M);								/* Indica si el movimiento es un killer mate */
int HistoricoValor(int *M);															/* Histórico valor de los movimientos */
int HistoricoMovimientoRefutacion();												/* Obtenemos el movimiento refutación */
#endif