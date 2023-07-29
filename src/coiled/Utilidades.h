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


#ifndef UTILIDADES_H
#define UTILIDADES_H

#include "Externo.h"

/******************************************************************************
							Metodos
******************************************************************************/
void MovimientoCoordenadas(int Inicio, int Fin, int PiezaCoronacion, char *mov);	/* Obtenemos el movimiento en coordenadas E2E4 */
void ImprimirMovimientoCoordenadas(int Inicio, int Fin, int PiezaCoronacion);		/* Imprimir el movimiento en coordenadas E2E4 */
int ValorPieza(int Pieza);															/* Obtenemos el valor de la pieza */
void VerificarDir(char *base, int dir);												/* Trasforma la barra invertida, por barra inclinada. */
int ArchivoExiste(char *archivo);													/* Comprobamos la existencia del archivo */
void SubString(char *string, int indice);
void SplitString(char *string, char *contenedor, int longitud);						/* Limpiamos cadenas con espacios */

#ifndef _WIN32
void Sleep(int milisegundos);
#endif

#endif
