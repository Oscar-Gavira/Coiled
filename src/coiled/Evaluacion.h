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

#ifndef EVALUACION_H
#define EVALUACION_H

#include "Externo.h"
#include "Hash.h"
#include "GeneradorDeMovimientos.h"

#ifdef USAR_NNUE
#include "nn.h"
#endif

enum eHorizontal
{
	HORIZONTAL_1, HORIZONTAL_2, HORIZONTAL_3, HORIZONTAL_4, HORIZONTAL_5, HORIZONTAL_6, HORIZONTAL_7, HORIZONTAL_8
};
enum eVertical
{
	COLUMNA_A, COLUMNA_B, COLUMNA_C, COLUMNA_D, COLUMNA_E, COLUMNA_F, COLUMNA_G, COLUMNA_H
};
enum {
	NEGRAS, BLANCAS
};
enum {
	PEON, CABALLO, ALFIL, TORRE, DAMA, REY
};

void SUMA(_Valor *a, _Valor *b);
void EvaluarPuntosPst(_ST_TableroX64 *Tablero);
int EvaluarInterpolar(_ST_TableroX64 *Tablero, _Valor *puntos);
void EvaluarPeones(_ST_TableroX64 *Tablero,int Cs, int Turno);
void EvaluarCaballo(_ST_TableroX64 *Tablero,int Cs, int Turno);
void EvaluarAlFil(_ST_TableroX64 *Tablero,int Cs, int Turno);
void EvaluarTorre(_ST_TableroX64 *Tablero, int Turno);
void EvaluarDama(_ST_TableroX64 *Tablero, int Cs, int Turno);
void EvaluarSeguridadRey(_ST_TableroX64 *Tablero, int Turno);
int ProteccionPeonesReyShelter(_ST_TableroX64 *Tablero, int Cl, int Turno);
int ProteccionPeonesReyStorm(_ST_TableroX64 *Tablero, int Cl, int Turno);
void EvaluarComplejidad(_ST_TableroX64 *Tablero, _Valor *puntos);
void LimpiarPuntuacion(_ST_TableroX64 *Tablero, int todo);
int Evaluar(_ST_TableroX64 *Tablero);
void CargarEvaluacion();
int EvaluarTablas(_ST_TableroX64 *Tablero);

#endif
