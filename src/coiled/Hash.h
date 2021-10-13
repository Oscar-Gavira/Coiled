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

#include "Externo.h"

#ifndef HASH_H
#define HASH_H

#ifdef USAR_HASH_TB

/******************************************************************************
							Metodos
******************************************************************************/
/* Guarda Depth, Edad y flag en macro en una variable */
short GuardarFlag(int d, int e, int f);
/* Obtengo el hash inicial */
void ObtenerKey();
/* Creamos la tabla de hash aun tamana especifico */
void CrearTransposicion(U64 MB);
/* Obtengo si la posicion ya a sido analizada y que valores tiene */
int RecuperarPosicion(int *hPuntos, int *hEv, int *hMov, int *hFlag);
/* Convertimos puntosa la TT */
void ConvertirValorTT(int *hPuntos);
/* Se puedo podar mediante TT */
int PodaHash(int *hFlag, int *beta, int *alpha, int *hPuntos);
/* Guardo la posicion analizada */
void AlmacenarPosicion(int depth, int puntos, int hEv, int hFlag, int hMov);
/* Obtenemos el movimiento si esta almacenado */
int RecuperarMovimientoHash();
/* Vacio la tabla hash */
void LimpiarTransposicion();
/* Obtengo el estado de la tabla (Vacia, Medio llena, LLena) % */
int ObtenerHashCompleto();
/* Liberamos la tabla hash */
void LiberarMemoria();

#endif
#endif
