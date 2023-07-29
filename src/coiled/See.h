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

#ifndef SEE_H
#define SEE_H

#include "Utilidades.h"
#include "GeneradorDeMovimientos.h"

int See(int *M, int turno, _ST_TableroX64 *Tablero);												/* Iniciamos la funcion See */
int ObtenerAtaquesMenores(int Cs, _ST_SeeTablero *SeeTablero, int *Turno, _ST_TableroX64 *Tablero);	/* Obtenemos piezas atacantes de menor a mayor */
int SeePeones(int Cs, _ST_SeeTablero *SeeTablero, int *Turno, _ST_TableroX64 *Tablero);				/* Buscamos defensa/ataque de peon. */
int SeeCaballo(int Cs, _ST_SeeTablero *SeeTablero, int *Turno, _ST_TableroX64 *Tablero);			/* Buscamos defensa/ataque de caballo blanco. */
int SeeAlfil(int Cs, _ST_SeeTablero *SeeTablero, int *Turno, _ST_TableroX64 *Tablero);				/* Buscamos defensa/ataque de alfil blanco. */
int SeeTorre(int Cs, _ST_SeeTablero *SeeTablero, int *Turno, _ST_TableroX64 *Tablero);				/* Buscamos defensa/ataque de torre blanco. */
int SeeDama(int Cs, _ST_SeeTablero *SeeTablero, int *Turno, _ST_TableroX64 *Tablero);				/* Buscamos defensa/ataque de dama blanco. */
int SeeRey(int Cs, _ST_SeeTablero *SeeTablero, int *Turno, _ST_TableroX64 *Tablero);				/* Buscamos defensa/ataque de rey blanco. */
int SeeHacerMovimiento(int *M, _ST_SeeTablero *SeeTablero, _ST_TableroX64 *Tablero);				/* Realiza el movimiento en See */
void SeeDeshacerMovimiento(_ST_SeeTablero *SeeTablero, _ST_TableroX64 *Tablero);					/* Deshace el movimiento de See */
#endif
