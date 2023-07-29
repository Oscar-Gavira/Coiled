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

#ifndef EXTERNO_H
#define EXTERNO_H

#include "Estructuras.h"

/*******************************************************************************
                                Variables globales
*******************************************************************************/
extern U64 zobrist_Tablero[14][64];                                             /* Almacena las llaves zobrist */
extern _ST_TipoJuego TipoJuego;                                                 /* Informacion de la partida por tiempo o por profundidad fija */
extern _ST_TT_Opciones TT_Opciones;                                             /* Para manejar la tabla hash */

extern int Salir;										                        /* Para salir de la aplicacion */

/************************************************************************************
                    Metodos externos nnue.c
************************************************************************************/
#ifdef USAR_NNUE
extern _ST_Nnue Nnue;                                                           /* Estructura con informacion sobre la red neuronal */
#endif

#endif
