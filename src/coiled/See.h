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

#ifndef SEE_H
#define SEE_H

/* Valor de las piezas para see */
#define SeePeonValor		(100)								/* Valor del peón */
#define SeeCaballoValor		(325)								/* Valor del caballo */
#define SeeAlfilValor		(325)								/* Valor del alfil */
#define SeeTorreValor		(500)								/* Valor de la torre */
#define SeeDamaValor		(1000)								/* Valor de la dama */
#define SeeReyValor			(10000)								/* Valor del rey */

int See(int *M, int turno);										/* Iniciamos la función See */
int ObtenerAtaquesMenores(int Cs, int turno);					/* Obtenemos piezas atacantes de menor a mayor */
int SeePeonesB(int Cs);											/* Buscamos defensa/ataque de peón blanco. */
int SeePeonesN(int Cs);											/* Buscamos defensa/ataque de peón negro. */
int SeeCaballoB(int Cs);										/* Buscamos defensa/ataque de caballo blanco. */
int SeeCaballoN(int Cs);										/* Buscamos defensa/ataque de caballo negro. */
int SeeAlfilB(int Cs);											/* Buscamos defensa/ataque de alfil blanco. */
int SeeAlfilN(int Cs);											/* Buscamos defensa/ataque de alfil negro. */
int SeeTorreB(int Cs);											/* Buscamos defensa/ataque de torre blanco. */
int SeeTorreN(int Cs);											/* Buscamos defensa/ataque de torre negro. */
int SeeDamaB(int Cs);											/* Buscamos defensa/ataque de dama blanco. */
int SeeDamaN(int Cs);											/* Buscamos defensa/ataque de dama negro. */
int SeeReyB(int Cs);											/* Buscamos defensa/ataque de rey blanco. */
int SeeReyN(int Cs);											/* Buscamos defensa/ataque de rey negro. */
int SeeHacerMovimiento(int *M);									/* Realiza el movimiento en See */
void SeeDeshacerMovimiento();									/* Deshace el movimiento de See */
#endif
