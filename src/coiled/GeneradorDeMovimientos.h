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

#ifndef GENERADORDEMOVIMIENTOS_H
#define GENERADORDEMOVIMIENTOS_H

#include "Externo.h"

#define MargenIzquierdo(x)	((x) - COLUMNA((x)))										/* Obtenemos el margen izquierdo del tablero */

/*******************************************************************************
									Metodos
*******************************************************************************/
void ObtenerMovimientoPeon(int Inicio, int TipoMov, _ST_Movimiento *M);					/* Obtener movimientos Peon */
void ObtenerMovimientoTorre(int Inicio, int TipoMov, _ST_Movimiento *M);				/* Obtener movimientos Torre */
void ObtenerMovimientoCaballo(int Inicio, int TipoMov, _ST_Movimiento *M);				/* Obtener movimientos Caballo */
void ObtenerMovimientoAlfil(int Inicio, int TipoMov, _ST_Movimiento *M);				/* Obtener movimientos Alfil */
void ObtenerMovimientoRey(int Inicio, int TipoMov, _ST_Movimiento *M);					/* Obtener movimientos Rey */
void AddMoveCoronacion(int Inicio, int Fin, int TipoMov, _ST_Movimiento *M);			/* Anadimos movimiento coronacion */
void AddMovePosicionFichaAlPaso(int Inicio, int Fin, int TipoMov, _ST_Movimiento *M);	/* Anadimos movimiento al paso */
void AddMoveCapturaAlPaso(int Inicio, int Fin, _ST_Movimiento *M);						/* Anadimos movimiento captura al paso */
void AddMoveEnroque(int Inicio, int Fin, int TipoMov, _ST_Movimiento *M);				/* Anadimos movimiento enroque */
void AddMoveRey(int Inicio, int Fin, int TipoMov, _ST_Movimiento *M);		            /* Anadimos movimiento rey */
void AddMoveNormal(int Inicio, int Fin, int TipoMov, _ST_Movimiento *M);				/* Anadimos movimiento normales */
int Jaque2(int PosicionRey);															/* Verifica jaque en el movimiento enroque */
int JaqueBlancas(int PosicionRey);														/* Indica si las blancas estan en jaque */
int JaqueNegras(int PosicionRey);														/* Indica si las negras estan en jaque */
int GenerarMovimientos(int TipoMov, _ST_Movimiento *M);									/* Generamos todos los movimientos posibles.
																						   TipoMov = Todos o Capturas y coronaciones */
void HacerMovimientoNormal(int M);
void HacerMovimientoEnroque(int M);
void HacerMovimientoAlPaso(int M);
void HacerMovimientoCoronacion(int M);
void HacerMovimientoRey(int M);

int HacerMovimiento(int M, int Verificar);												/* Realizamos el movimiento pasado */
void DeshacerMovimiento();																/* Deshacemos el movimiento */

#ifdef USAR_NULL_MOVE_PRUNING
	void HacerMovimientoNull();															/* Hacemos movimiento nulo */
	void DeshacerMovimientoNull();														/* Deshacemos movimiento nulo */
#endif

int Jaque(int Mueve_Blancas);															/* Obtenemos si un bando esta en jaque o se da jaque */
#endif
