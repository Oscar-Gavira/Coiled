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

#ifndef GENERADORDEMOVIMIENTOS_H
#define GENERADORDEMOVIMIENTOS_H

#include "Hash.h"

/*******************************************************************************
									Metodos
*******************************************************************************/
void ObtenerMovimientoPeon(_ST_TableroX64 *Tablero, int Inicio, int TipoMov, _ST_Movimiento *M);				/* Obtener movimientos Peon */
void ObtenerMovimientoTorre(_ST_TableroX64 *Tablero, int Inicio, int TipoMov, _ST_Movimiento *M);				/* Obtener movimientos Torre */
void ObtenerMovimientoCaballo(_ST_TableroX64 *Tablero, int Inicio, int TipoMov, _ST_Movimiento *M);				/* Obtener movimientos Caballo */
void ObtenerMovimientoAlfil(_ST_TableroX64 *Tablero, int Inicio, int TipoMov, _ST_Movimiento *M);				/* Obtener movimientos Alfil */
void ObtenerMovimientoRey(_ST_TableroX64 *Tablero, int Inicio, int TipoMov, _ST_Movimiento *M);					/* Obtener movimientos Rey */
void AddMoveCoronacion(_ST_TableroX64 *Tablero, int Inicio, int Fin, int TipoMov, _ST_Movimiento *M);			/* Anadimos movimiento coronacion */
void AddMovePosicionFichaAlPaso(_ST_TableroX64 *Tablero, int Inicio, int Fin, int TipoMov, _ST_Movimiento *M);	/* Anadimos movimiento al paso */
void AddMoveCapturaAlPaso(_ST_TableroX64 *Tablero, int Inicio, int Fin, _ST_Movimiento *M);						/* Anadimos movimiento captura al paso */
void AddMoveEnroque(_ST_TableroX64 *Tablero, int Inicio, int Fin, int TipoMov, _ST_Movimiento *M);				/* Anadimos movimiento enroque */
void AddMoveRey(_ST_TableroX64 *Tablero, int Inicio, int Fin, int TipoMov, _ST_Movimiento *M);		            /* Anadimos movimiento rey */
void AddMoveNormal(_ST_TableroX64 *Tablero, int Inicio, int Fin, int TipoMov, _ST_Movimiento *M);				/* Anadimos movimiento normales */
int Jaque2(_ST_TableroX64 *Tablero, int PosicionRey);															/* Verifica jaque en el movimiento enroque */
int JaqueBlancas(_ST_TableroX64 *Tablero, int PosicionRey);														/* Indica si las blancas estan en jaque */
int JaqueNegras(_ST_TableroX64 *Tablero, int PosicionRey);														/* Indica si las negras estan en jaque */
void GenerarMovimientos(_ST_TableroX64 *Tablero, int TipoMov, _ST_Movimiento *M);								/* Generamos todos los movimientos posibles.
																													TipoMov = Todos o Capturas y coronaciones */
void HacerMovimientoNormal(_ST_TableroX64 *Tablero, int M);														/* Hacemos un movimiento normal */
void HacerMovimientoEnroque(_ST_TableroX64 *Tablero, int M);													/* Hacemos un movimiento de enroque */
void HacerMovimientoAlPaso(_ST_TableroX64 *Tablero, int M);														/* Hacemos un movimiento al paso  */
void HacerMovimientoCoronacion(_ST_TableroX64 *Tablero, int M);													/* Hacemos un movimiento coronacion */
void HacerMovimientoRey(_ST_TableroX64 *Tablero, int M);														/* Hacemos un movimiento rey */
int HacerMovimiento(_ST_TableroX64 *Tablero, int *M, int Verificar);											/* Realizamos el movimiento */
void DeshacerMovimiento(_ST_TableroX64 *Tablero);																/* Deshacemos el movimiento */

#ifdef USAR_NULL_MOVE_PRUNING
	void HacerMovimientoNull(_ST_TableroX64 *Tablero);															/* Hacemos movimiento nulo */
	void DeshacerMovimientoNull(_ST_TableroX64 *Tablero);														/* Deshacemos movimiento nulo */
#endif

int Jaque(_ST_TableroX64 *Tablero, int Mueve_Blancas);															/* Obtenemos si un bando esta en jaque o se da jaque */
#endif
