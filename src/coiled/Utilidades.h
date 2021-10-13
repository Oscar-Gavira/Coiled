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

#ifndef UTILIDADES_H
#define UTILIDADES_H

/******************************************************************************
							Metodos
******************************************************************************/
void ImprimirMovimientoCoordenadas(int Inicio, int Fin, int PiezaCoronacion);		/* Imprimir el movimiento en coordenadas E2E4 */
U64 ObtenerTiempo();																/* Obtenemos el tiempo */
int EntradaStdIn();																/* Comprobamos entradas durante la busqueda. */
int ValorPieza(int Pieza);															/* Obtenemos el valor de la pieza */
int IndexOfShift(char *base, char *str, int startIndex);							/* Devuelve el indice de la coincidencia dentro de un string. Si no hay devuelve -1 */
#endif
