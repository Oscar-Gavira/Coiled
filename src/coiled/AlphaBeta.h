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

#ifndef ALPHABETA_H
#define ALPHABETA_H

#include "Tiempo.h"
#include "GeneradorDeMovimientos.h"
#include "OrdenarMovimientos.h"
#include "Estructuras.h"
#include "Evaluacion.h"
/******************************************************************************
							Metodos
******************************************************************************/
void A_Inicio(int Reiniciar, _ST_TableroX64 *Tablero);											/* Reinicia variables utilizados en el arbol AlphaBeta */
int AspirationWindows(_ST_TableroX64 *Tablero, int depth, int en_jaque, int PuntuacionAnterior);/* Rutina Aspiration Windows */
int AlphaBeta(_ST_TableroX64 *Tablero, int depth, int alpha, int beta, int en_jaque, int *sReduccion, int EsSingular);	/* Rutina AlphaBeta	Vp */
int AlphaBetaQs(_ST_TableroX64 *Tablero, int alpha, int beta);									/* Rutina AlphaBeta	Quiescence */
void ActualizarVp(int M, _ST_TableroX64 *Tablero);												/* Actualiza la variante principal triangular durante la busqueda. */
void ActualizarVpRoot(_ST_TableroX64 *Tablero);													/* Actualiza la variante principal de una busqueda terminada de root */
void ImprimirVp(_ST_TableroX64 *Tablero, int puntos);											/* Imprimimos la variante principal, entre otros datos */
void ImprimirMejorJugada(int Jugada, int J_Adv);												/* Imprimimos la mejor jugada y ponder */
void ComprobarTiempo(_ST_TableroX64 *Tablero);													/* Comprobamos tiempo y entradas */
int FigurasAdversarioNull(_ST_TableroX64 *Tablero);												/* Comprueba si se puede hacer Movimiento nulo */
int EsPuntuacionMate(int puntos);																/* Obtenemos si la puntuacion esta dentro del margen de mate */
int EsTabla(_ST_TableroX64 *Tablero, int en_jaque);												/* Obtenemos si es tablas por repeticion */
void Buscar(void *arg);																		    /* Inicializa la busqueda AlphaBeta */
void Iniciar_AlphaBeta();																		/* Inicia un vector para las reducciones en la busqueda */
#endif
