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

#ifndef EXTERNO_H
#define EXTERNO_H

#include "Estructuras.h"

extern void Inicializar_See();							    /* Inicializa valores MVV/LVA. */

/*******************************************************************************
								Variables globales
*******************************************************************************/
extern _ST_TableroX64 TableroGlobal;						/* Tablero de la posicion actual. */
extern _ST_TipoJuego TipoJuego;							    /* Informacion de la partida por tiempo o por profundidad fija */
extern int Salir;										    /* Para salir de la aplicacion */
extern _ST_Puntos Blancas;									/* Almacena la puntuacion de las blancas */
extern _ST_Puntos Negras;									/* Almacena la puntuacion de las negras */
#ifdef USAR_TABLAS_DE_FINALES
    extern _ST_TablaDeFinales TablaDeFinales;
#endif
extern U64 zobrist_Tablero[14][64];                         /* Almacena las llaves zobrist */
extern _ST_TT_Opciones TT_Opciones;

/************************************************************************************
                    Metodos externos Inicio.c
************************************************************************************/
extern void Movimiento(char *ptr, int *Ok, int Ultimo);	    /* Realiza un movimiento y comprobamos si es legal. */
extern void Position_Fen_Startpos(char *ptr);			    /* Carga una posicion FEN */

/************************************************************************************
                    Metodos externos AlphaBeta.c
************************************************************************************/
extern void A_Inicio(int full);							    /* Reinicia variables utilizados en el arbol AlphaBeta */
extern void Buscar();									    /* Inicializa la busqueda AlphaBeta */
extern void Iniciar_AlphaBeta();                            /* Inicia un vector para las reducciones en la busqueda */

/************************************************************************************
                    Metodos externos Evaluacion.c
************************************************************************************/
extern int Evaluar();						                /* Evaluacion del tablero */
extern int EvaluarTablas();                                 /* Comprueba si la posicion actual es tablas por falta de material. */
extern void LimpiarPuntuacion(int todo);                    /* Limpia los puntos */
extern void CargarEvaluacion();   
#endif
