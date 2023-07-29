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

#ifndef HASH_H
#define HASH_H

#include "Externo.h"

#ifdef USAR_HASH_TB

/******************************************************************************
							Metodos
******************************************************************************/
void ObtenerKey(_ST_TableroX64 *Tablero);																/* Obtengo el hash inicial */
int CrearTransposicion();																				/* Creamos la tabla de hash aun tamana especifico */
int RecuperarPosicion(_ST_TableroX64 *Tablero, int *hPuntos, int *hEv, int *hMov, int *hFlag);			/* Obtengo si la posicion ya a sido analizada y que valores tiene */
void ConvertirValorTT(_ST_TableroX64 *Tablero, int *hPuntos);											/* Convertimos puntosa la TT */
int PodaHash(int *hFlag, int *beta, int *alpha, int *hPuntos);											/* Cortamos la busqueda mediante TT */
void AlmacenarPosicion(_ST_TableroX64 *Tablero, int depth, int puntos, int hEv, int hFlag, int hMov);	/* Guardo la posicion analizada */
void LimpiarTransposicion();																			/* Vacio la tabla hash */
int ObtenerHashCompleto();																				/* Obtengo el estado de la tabla (Vacia, Medio llena, LLena) % */
void LiberarMemoria();																					/* Liberamos la tabla hash */
#endif
#endif
