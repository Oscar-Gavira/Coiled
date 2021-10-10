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

#ifndef EVALUACION_H
#define EVALUACION_H

#include "Externo.h"
#include "Hash.h"
#include "GeneradorDeMovimientos.h"

#ifdef USAR_NNUE
	#include "nnue.h"
#endif

/* Para crear el tablero contrario */
int ESPEJO[] = {
  56, 57, 58, 59, 60, 61, 62, 63,
  48, 49, 50, 51, 52, 53, 54, 55,
  40, 41, 42, 43, 44, 45, 46, 47,
  32, 33, 34, 35, 36, 37, 38, 39,
  24, 25, 26, 27, 28, 29, 30, 31,
  16, 17, 18, 19, 20, 21, 22, 23,
   8,  9, 10, 11, 12, 13, 14, 15,
   0,  1,  2,  3,  4,  5,  6,  7
};

#define Distancia(a, b) MAX(ABS(HORIZONTAL(a) - HORIZONTAL(b)), ABS(COLUMNA(a) - COLUMNA(b)))
#define MAX_ETAPA	(24)			/* ETAPA maximo */
#define FIN_ETAPA	 (9)			/* ETAPA Final */
_Valor	PuntosCero = { 0, 0 };		/* Valor cero */

_Valor PEONVALOR = {79, 127};
_Valor CABALLOVALOR = {248, 275};
_Valor ALFILVALOR = {253, 292};
_Valor TORREVALOR = {355, 526};
_Valor DAMAVALOR = {847, 939};
_Valor REYVALOR = {0, 0};

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

inline void SUMA(_Valor *a, _Valor *b);
void EvaluarPuntosPst();
int Interpolar(_Valor *puntos);
void EvaluarPeones(int Turno);
void EvaluarCaballo(int Cs, int Turno);
void EvaluarAlFil(int Turno);
void EvaluarTorre(int Turno);
void EvaluarDama(int Cs, int Turno);
void EvaluarSeguridadRey(int Turno);
void EvaluarSeguridadReyPeon(int Turno);
int ProteccionPeonesReyShelter(int Cl, int Turno);
int ProteccionPeonesReyStorm(int Cl, int Turno);
void EvaluarAjustePiezas();
void EvaluarComplejidad(_Valor *puntos);

typedef struct tag_Evaluacion {
	_Valor PST[2][6][64];
} _Evaluacion;

_Evaluacion Evaluacion;

/********************************************************************************************
VALORES ESTATICOS DE EVALUACION. PST, MOVILIDAD, PAREJA DE ALFILES, OUTPOST....
**********************************************************************************************/

int Tempo = 20;

/************************************** PST **************************************************/
_Valor PEON_PST[64] = {
{0, 0 }, {0, 0 }, {0, 0 }, {0, 0 }, {0, 0 }, {0, 0 }, {0, 0 }, {0, 0 }, {-35, -12 }, {-28, -3 }, {-32, -10 }, {-38, 0 }, {-17, 4 }, {-3, -9 }, {-4, -13 }, {-39, -20 }, {-35, -19 }, {-23, -8 }, {-31, -17 }, {-25, -16 }, {-11, -16 }, {-1, -19 }, {-10, -14 }, {-26, -22 }, {-31, -11 }, {-17, -3 }, {-16, -21 }, {-7, -29 }, {2, -24 }, {1, -19 }, {-18, -10 }, {-21, -20 }, {-22, 9 }, {-6, 0 }, {-9, -6 }, {6, -26 }, {20, -21 }, {14, -15 }, {-1, 1 }, {-15, -5 }, {4, 42 }, {-7, 44 }, {27, 11 }, {27, 7 }, {52, -7 }, {60, 10 }, {51, 31 }, {15, 20 }, {55, 56 }, {44, 71 }, {89, 62 }, {100, 14 }, {87, 52 }, {64, 17 }, {-11, 16 }, {-24, 42 }, {0, 0 }, {0, 0 }, {0, 0 }, {0, 0 }, {0, 0 }, {0, 0 }, {0, 0 }, {0, 0 }
};
_Valor CABALLO_PST[64] = {
{-76, -38 }, {-28, -42 }, {-41, -15 }, {-37, -8 }, {-24, -12 }, {-19, -26 }, {-16, -46 }, {-48, -29 }, {-34, -15 }, {-34, -2 }, {-21, 1 }, {-5, -6 }, {-9, 8 }, {0, 1 }, {-12, -16 }, {-16, -15 }, {-25, -27 }, {-14, 3 }, {0, 12 }, {24, 27 }, {21, 30 }, {6, 2 }, {9, -8 }, {-14, 4 }, {-20, -6 }, {6, 22 }, {10, 45 }, {13, 39 }, {27, 45 }, {25, 30 }, {26, 21 }, {-8, -2 }, {3, 21 }, {3, 25 }, {26, 45 }, {47, 53 }, {25, 50 }, {40, 46 }, {26, 26 }, {30, 16 }, {4, -6 }, {21, 24 }, {60, 34 }, {77, 31 }, {100, 29 }, {87, 40 }, {48, 44 }, {20, 9 }, {9, -20 }, {-5, 22 }, {37, 10 }, {71, 32 }, {52, 32 }, {61, 8 }, {3, 27 }, {26, 1 }, {-105, -43 }, {-37, 14 }, {16, 12 }, {40, 5 }, {-4, 0 }, {-20, 6 }, {-35, 7 }, {-63, -23 }
};
_Valor ALFIL_PST[64] = {
{-9, -18 }, {7, -10 }, {-11, -10 }, {-15, -3 }, {-10, -10 }, {-4, 2 }, {-10, -17 }, {-10, -40 }, {3, -10 }, {-6, -16 }, {10, -7 }, {-12, -2 }, {-3, 0 }, {0, -12 }, {11, -5 }, {-13, -13 }, {-16, -1 }, {10, 15 }, {-3, 10 }, {4, 13 }, {-2, 14 }, {-2, 1 }, {4, 0 }, {-1, -1 }, {-4, 4 }, {-7, 14 }, {3, 14 }, {3, 15 }, {15, 16 }, {-7, 9 }, {-7, -4 }, {11, -18 }, {-12, 9 }, {-2, 14 }, {10, 9 }, {37, 19 }, {7, 22 }, {31, 6 }, {0, -1 }, {-5, 6 }, {-2, 14 }, {14, 17 }, {16, 6 }, {16, 1 }, {35, 14 }, {63, 5 }, {60, 15 }, {11, 23 }, {-13, 3 }, {-4, 13 }, {-2, -1 }, {-12, 18 }, {-12, -1 }, {20, 0 }, {-27, 9 }, {2, -17 }, {-38, 21 }, {-78, 27 }, {-29, 35 }, {-34, 5 }, {-63, 34 }, {-24, 3 }, {-7, 16 }, {-25, -9 }
};
_Valor TORRE_PST[64] = {
{-14, -10 }, {-9, -12 }, {-3, -7 }, {3, -8 }, {4, -17 }, {3, -13 }, {6, -21 }, {-17, -15 }, {-29, -1 }, {-22, -14 }, {-8, -3 }, {-9, -10 }, {-13, -15 }, {-21, -16 }, {-1, -25 }, {-38, 1 }, {-44, 9 }, {-23, 0 }, {-14, -4 }, {-5, -10 }, {0, -14 }, {-16, -6 }, {-4, -8 }, {-9, -7 }, {-38, 26 }, {-20, 10 }, {-35, 20 }, {-6, 4 }, {-13, 6 }, {-17, 14 }, {18, 5 }, {-29, 18 }, {-21, 33 }, {-7, 28 }, {-13, 28 }, {14, 19 }, {5, 8 }, {7, 18 }, {37, 6 }, {24, 1 }, {-8, 30 }, {17, 25 }, {5, 26 }, {19, 24 }, {56, 14 }, {56, 17 }, {66, 13 }, {29, 14 }, {-14, 28 }, {7, 32 }, {31, 28 }, {49, 30 }, {47, 33 }, {72, 9 }, {27, 21 }, {69, 1 }, {23, 17 }, {19, 15 }, {26, 16 }, {13, 24 }, {19, 32 }, {43, 20 }, {59, 12 }, {66, 10 }
};
_Valor DAMA_PST[64] = {
{7, -24 }, {-2, -25 }, {10, -23 }, {8, -2 }, {15, -26 }, {-4, -47 }, {-15, -50 }, {1, -28 }, {-7, -8 }, {11, -25 }, {6, -12 }, {14, -3 }, {12, -13 }, {20, -41 }, {19, -44 }, {11, -56 }, {0, -20 }, {1, -12 }, {13, -8 }, {-6, -1 }, {11, 0 }, {7, 4 }, {9, 0 }, {10, -19 }, {-9, -11 }, {-9, 2 }, {-11, 11 }, {-10, 39 }, {-6, 20 }, {4, 2 }, {6, -12 }, {-7, -5 }, {-3, -3 }, {0, 15 }, {4, 26 }, {-12, 51 }, {-6, 47 }, {-31, 46 }, {-33, 27 }, {-10, -11 }, {3, 2 }, {-19, 38 }, {-5, 33 }, {-1, 42 }, {7, 52 }, {34, 42 }, {13, 34 }, {1, 28 }, {-6, 12 }, {-30, 35 }, {10, 25 }, {-3, 53 }, {-16, 51 }, {-4, 29 }, {3, 15 }, {52, 36 }, {-22, 17 }, {14, 18 }, {-6, 10 }, {-10, 5 }, {-6, 24 }, {28, 34 }, {46, 36 }, {32, 13 }
};
_Valor REY_PST[64] = {
{17, -67 }, {45, -22 }, {6, -20 }, {-54, -28 }, {-12, -36 }, {-35, -26 }, {17, -37 }, {9, -75 }, {56, -32 }, {22, -10 }, {0, -2 }, {-20, 1 }, {-22, 3 }, {-10, -5 }, {17, -18 }, {1, -33 }, {-15, -5 }, {41, -3 }, {12, 15 }, {-44, 24 }, {-16, 22 }, {-29, 8 }, {-24, -1 }, {-48, -26 }, {-34, 7 }, {7, 26 }, {6, 36 }, {-16, 43 }, {-13, 37 }, {-29, 34 }, {-38, 15 }, {-79, -11 }, {-28, 11 }, {-4, 47 }, {-4, 51 }, {-43, 49 }, {-13, 48 }, {-3, 48 }, {7, 32 }, {-45, 17 }, {-55, 28 }, {29, 61 }, {12, 76 }, {-43, 66 }, {-35, 48 }, {15, 76 }, {28, 61 }, {-10, 25 }, {-3, -17 }, {16, 51 }, {4, 56 }, {-20, 58 }, {-17, 66 }, {4, 61 }, {19, 51 }, {-20, 4 }, {-11, 10 }, {-6, 31 }, {-18, 51 }, {-11, 51 }, {-34, 58 }, {7, 51 }, {9, 41 }, {-47, -49 }
};
/********************************* MOVILIDAD **************************************************/
_Valor CABALLO_MOVILIDAD[9] = {
{-1, -30 }, {6, 1 }, {12, 4 }, {16, 11 }, {17, 16 }, {16, 25 }, {16, 23 }, {15, 23 }, {18, 19 }
};
_Valor ALFIL_MOVILIDAD[14] = {
{-12, -22 }, {-7, -32 }, {-2, -19 }, {1, -6 }, {6, 5 }, {14, 20 }, {17, 29 }, {22, 33 }, {19, 42 }, {23, 40 }, {21, 36 }, {34, 38 }, {34, 28 }, {29, 13 }
};
_Valor TORRE_MOVILIDAD[15] = {
{-15, -12 }, {-12, -12 }, {-5, -8 }, {-3, -5 }, {-4, 7 }, {-1, 16 }, {0, 20 }, {6, 22 }, {3, 31 }, {1, 42 }, {4, 43 }, {4, 47 }, {7, 47 }, {14, 47 }, {20, 37 }
};
_Valor DAMA_MOVILIDAD[28] = {
{35, -2 }, {-5, 5 }, {-8, -2 }, {-8, -23 }, {-6, -28 }, {-4, -29 }, {-6, -14 }, {-3, -12 }, {1, -10 }, {-1, -3 }, {5, -5 }, {7, -1 }, {6, 6 }, {6, 14 }, {8, 17 }, {4, 22 }, {2, 36 }, {6, 34 }, {4, 43 }, {7, 37 }, {17, 39 }, {21, 33 }, {30, 36 }, {24, 22 }, {41, 12 }, {41, -4 }, {36, 38 }, {34, 26 }
};
/******************************* ESTRUCTURA DE PEONES *****************************************/
_Valor PEON_PASADO[8] = {
{0, 0 }, {4, 4 }, {-19, 17 }, {-8, 32 }, {19, 51 }, {48, 67 }, {58, 90 }, {0, 0 }
};
_Valor PENALIZACION_PEON_DOBLE[8] = {
{-36, -46 }, {-9, -25 }, {-2, -31 }, {-23, -24 }, {-18, -21 }, {-20, -19 }, {0, -29 }, {-26, -44 }
};
_Valor PEON_AISLADO[8] = {
{-1, -5 }, {-5, -13 }, {-14, -21 }, {-13, -26 }, {-22, -22 }, {-14, -16 }, {-14, -10 }, {-20, -6 }
};
_Valor PEON_RETRASADO[8] = {
{4, -2 }, {-3, -7 }, {-2, -13 }, {-21, -14 }, {-8, -7 }, {-7, -1 }, {-13, -1 }, {1, -3 }
};

/******************************** SEGURIDAD REY *************************************************/
int COLUMNA_ABIERTA_SHELTER[8] = { -6, -5, -4, -4, -4, -4, -5, -6 };
int COLUMNA_SEMI_ABIERTA_SHELTER[8] = { -5, -4, -3, -3, -3, -3, -4, -5 };
int COLUMNA_STORM[8] = { 0, 0, -3, -2, -1, 0, 0, 0 };

_Valor SEGURIDAD_REY_ATAQUE_PIEZAS[5] = {
	{0, 0}, {48,  41}, {24,  35}, {36,   8}, {30,   6}
};
_Valor SEGURIDAD_REY_ATAQUES = { 45, 34 };
_Valor SEGURIDAD_REY_CABALLO_JAQUE = { 112, 117 };
_Valor SEGURIDAD_REY_ALFIL_JAQUE = { 59,  59 };
_Valor SEGURIDAD_REY_TORRE_JAQUE = { 90,  98 };
_Valor SEGURIDAD_REY_DAMA_JAQUE = { 93,  83 };
_Valor SEGURIDAD_REY_DAMA_ENEMIGA = { -237,-259 };

/******************************* PUNTOS EXTRAS *************************************************/
_Valor ALFIL_PAREJA = { 42, 52 };

_Valor TORRE_ABIERTO = { 34,   8 };
_Valor TORRE_SEMI_ABIERTO = { 10,   9 };
_Valor TORRE_EN_SEPTIMA = { 18, 0 };

int COMPLEJIDAD_PEONES_TOTALES = 8;
int COMPLEJIDAD_PEONES_AMBOS_LADOS = 82;
int COMPLEJIDAD_PEONES_FINALES = 76;
int COMPLEJIDAD_AJUSTE = -157;

#endif
