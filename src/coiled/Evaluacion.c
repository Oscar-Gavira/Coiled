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

#include "Evaluacion.h"

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

_Valor	PuntosCero = { 0, 0 };		/* Valor cero */
_Evaluacion Evaluacion;

_Valor PEONVALOR = { 79, 127 };
_Valor CABALLOVALOR = { 248, 275 };
_Valor ALFILVALOR = { 253, 292 };
_Valor TORREVALOR = { 355, 526 };
_Valor DAMAVALOR = { 847, 939 };
_Valor REYVALOR = { 3000, 3000 };

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
/* valor por columna */
_Valor PENALIZACION_PEON_DOBLE[8] = {
{-36, -46 }, {-9, -25 }, {-2, -31 }, {-23, -24 }, {-18, -21 }, {-20, -19 }, {0, -29 }, {-26, -44 }
};
/* valor por columna */
_Valor PEON_AISLADO[8] = {
{-1, -5 }, {-5, -13 }, {-14, -21 }, {-13, -26 }, {-22, -22 }, {-14, -16 }, {-14, -10 }, {-20, -6 }
};
/* valor por columna */
_Valor PEON_RETRASADO[8] = {
{4, -2 }, {-3, -7 }, {-2, -13 }, {-21, -14 }, {-8, -7 }, {-7, -1 }, {-13, -1 }, {1, -3 }
};
/******************************** SEGURIDAD REY *************************************************/
/* -2, -2, -1, -1, -1, -1, -2, -2 */
/* -5, -8,  -6, -2,  0, -1, -9, -9 OK 2736 */
/* -6, -3,  -7,  0, -4, -6, -6, -6 */
/* -3, -3, -10,  0, -5, -7, -7, -6 */

/* -5, -6, -2, -5, -5, -5, -3, -4    - 2706 */
int SEGURIDAD_REY_SEMI_ABIERTA[8] = { -2, -2, -1, -1, -1, -1, -2, -2 };
int SEGURIDAD_REY_STORM[8] = { 0, 0, -3, -2, -1, 0, 0, 0 };

_Valor SEGURIDAD_REY_ATAQUE_PIEZAS[5] = { {0, 0}, {48,  41}, {24,  35}, {36,   8}, {30,   6} };
_Valor SEGURIDAD_REY_ATAQUES_ZONA = { 45, 24 };
_Valor SEGURIDAD_REY_CABALLO_JAQUE = { 112, 112 };
_Valor SEGURIDAD_REY_ALFIL_JAQUE = { 59,  59 };
_Valor SEGURIDAD_REY_TORRE_JAQUE = { 98,  98 };
_Valor SEGURIDAD_REY_DAMA_JAQUE = { 83,  83 };
_Valor SEGURIDAD_REY_DAMA_ENEMIGA = { -237,-259 };

/******************************* PUNTOS EXTRAS *************************************************/
_Valor ALFIL_PAREJA = { 42, 52 };

_Valor TORRE_ABIERTO = { 34,   8 };
_Valor TORRE_SEMI_ABIERTO = { 10,   9 };
_Valor TORRE_EN_SEPTIMA = { 18, 0 };

int COMPLEJIDAD_PEONES_TOTALES = 8;
int COMPLEJIDAD_PEONES_AMBOS_LADOS = 82;
int COMPLEJIDAD_PEONES_FINALES = 74;
int COMPLEJIDAD_AJUSTE = -157;

void CargarEvaluacion()
{
	int Cs = 0;

	for (Cs = 0; Cs < 64; Cs++) {
		Evaluacion.PST[NEGRAS][PEON][Cs] = PEON_PST[Cs]; SUMA(&Evaluacion.PST[NEGRAS][PEON][Cs], &PEONVALOR);
		Evaluacion.PST[BLANCAS][PEON][ESPEJO[Cs]] = PEON_PST[Cs]; SUMA(&Evaluacion.PST[BLANCAS][PEON][ESPEJO[Cs]], &PEONVALOR);
		Evaluacion.PST[NEGRAS][CABALLO][Cs] = CABALLO_PST[Cs]; SUMA(&Evaluacion.PST[NEGRAS][CABALLO][Cs], &CABALLOVALOR);
		Evaluacion.PST[BLANCAS][CABALLO][ESPEJO[Cs]] = CABALLO_PST[Cs]; SUMA(&Evaluacion.PST[BLANCAS][CABALLO][ESPEJO[Cs]], &CABALLOVALOR);
		Evaluacion.PST[NEGRAS][ALFIL][Cs] = ALFIL_PST[Cs]; SUMA(&Evaluacion.PST[NEGRAS][ALFIL][Cs], &ALFILVALOR);
		Evaluacion.PST[BLANCAS][ALFIL][ESPEJO[Cs]] = ALFIL_PST[Cs]; SUMA(&Evaluacion.PST[BLANCAS][ALFIL][ESPEJO[Cs]], &ALFILVALOR);
		Evaluacion.PST[NEGRAS][TORRE][Cs] = TORRE_PST[Cs]; SUMA(&Evaluacion.PST[NEGRAS][TORRE][Cs], &TORREVALOR);
		Evaluacion.PST[BLANCAS][TORRE][ESPEJO[Cs]] = TORRE_PST[Cs]; SUMA(&Evaluacion.PST[BLANCAS][TORRE][ESPEJO[Cs]], &TORREVALOR);
		Evaluacion.PST[NEGRAS][DAMA][Cs] = DAMA_PST[Cs]; SUMA(&Evaluacion.PST[NEGRAS][DAMA][Cs], &DAMAVALOR);
		Evaluacion.PST[BLANCAS][DAMA][ESPEJO[Cs]] = DAMA_PST[Cs]; SUMA(&Evaluacion.PST[BLANCAS][DAMA][ESPEJO[Cs]], &DAMAVALOR);
		Evaluacion.PST[NEGRAS][REY][Cs] = REY_PST[Cs]; //SUMA(&Evaluacion.PST[NEGRAS][REY][Cs], &REYVALOR);
		Evaluacion.PST[BLANCAS][REY][ESPEJO[Cs]] = REY_PST[Cs]; //SUMA(&Evaluacion.PST[BLANCAS][REY][ESPEJO[Cs]], &REYVALOR);
	}
}

void SUMA(_Valor *a, _Valor *b)
{
	a->Apertura += b->Apertura;
	a->Final += b->Final;
}
int Evaluar(_ST_TableroX64 *Tablero)
{
#ifdef USAR_NNUE
	if (Nnue.Usar == true) /* Con mas de un subproceso, genera errores. (crash, timeout) */
	{
		return ProbarNNUE(Tablero);
	}
#endif
	
	int puntos = 0;
	_Valor pGlobal = PuntosCero;

	Tablero->Etapa = (((Tablero->Blancas.CaballosTotales + Tablero->Blancas.AlfilTotales) * 1) + (Tablero->Blancas.TorresTotales * 2) + (Tablero->Blancas.DamasTotales * 4) + ((Tablero->Negras.CaballosTotales + Tablero->Negras.AlfilTotales) * 1) + (Tablero->Negras.TorresTotales * 2) + (Tablero->Negras.DamasTotales * 4));
	if (Tablero->Etapa > MAX_ETAPA)
		Tablero->Etapa = MAX_ETAPA;
	if (Tablero->Etapa < 0)
		Tablero->Etapa = 0;

	if (Tablero->Etapa <= FIN_ETAPA)
	{
		if (EvaluarTablas(Tablero))
		{
			return VALOR_EMPATE;
		}
	}

	if (Tablero->Etapa <= FIN_ETAPA)
	{
		if (EvaluarTablas(Tablero))
		{
			return VALOR_EMPATE;
		}
	}

	/* Limpiamos la puntuacion */
	LimpiarPuntuacion(Tablero, true);
	SUMA(&Tablero->Blancas.Puntos, &REYVALOR);
	SUMA(&Tablero->Negras.Puntos, &REYVALOR);
	EvaluarPuntosPst(Tablero);
	EvaluarTorre(Tablero, BLANCAS);
	EvaluarTorre(Tablero, NEGRAS);
	EvaluarSeguridadRey(Tablero, BLANCAS);
	EvaluarSeguridadRey(Tablero, NEGRAS);
	if (Tablero->Blancas.AlfilTotales > 1) SUMA(&Tablero->Blancas.Puntos, &ALFIL_PAREJA);
	if (Tablero->Negras.AlfilTotales > 1) SUMA(&Tablero->Negras.Puntos, &ALFIL_PAREJA);
	
	pGlobal.Apertura = Tablero->Blancas.Puntos.Apertura - Tablero->Negras.Puntos.Apertura;
	pGlobal.Final = Tablero->Blancas.Puntos.Final - Tablero->Negras.Puntos.Final;

	EvaluarComplejidad(Tablero, &pGlobal);

	puntos = EvaluarInterpolar(Tablero, &pGlobal);

	return Tempo + (Tablero->MueveBlancas ? puntos : -puntos);
}

int EvaluarTablas(_ST_TableroX64 *Tablero)
{
	if (!(Tablero->Blancas.CaballosTotales + Tablero->Blancas.AlfilTotales + Tablero->Blancas.TorresTotales + Tablero->Blancas.DamasTotales + Tablero->Blancas.PeonTotales) &&
		!(Tablero->Negras.CaballosTotales + Tablero->Negras.AlfilTotales + Tablero->Negras.TorresTotales + Tablero->Negras.DamasTotales + Tablero->Negras.PeonTotales))
	{
		return true;
	}
	if (!Tablero->Blancas.PeonTotales && !Tablero->Negras.PeonTotales)
	{
		if (!(Tablero->Blancas.TorresTotales + Tablero->Blancas.DamasTotales) && !(Tablero->Negras.TorresTotales + Tablero->Negras.DamasTotales))
		{
			if ((Tablero->Blancas.CaballosTotales + Tablero->Blancas.AlfilTotales <= 1) && (Tablero->Negras.CaballosTotales + Tablero->Negras.AlfilTotales <= 1))
			{
				return true;
			}
		}
	}
	return false;
}

void LimpiarPuntuacion(_ST_TableroX64 *Tablero, int todo)
{
	memset(Tablero->Blancas.PeonDoblados, 0, sizeof(Tablero->Blancas.PeonDoblados));
	memset(Tablero->Blancas.PosicionTorre, 0, sizeof(Tablero->Blancas.PosicionTorre));
	memset(Tablero->Blancas.ReyJaquePieza, 0, sizeof(Tablero->Blancas.ReyJaquePieza));

	Tablero->Blancas.Puntos = PuntosCero;
	Tablero->Blancas.ReyCuadrosAtacando = 0;
	Tablero->Blancas.ReyAtaquesPiezas = 0;
	Tablero->Blancas.ReyAtaquesValor = PuntosCero;
	Tablero->Blancas.GrupoPeonesQ = 0;
	Tablero->Blancas.GrupoPeonesK = 0;

	if (todo)
	{
		Tablero->Blancas.PeonTotales = 0;
		Tablero->Blancas.CaballosTotales = 0;
		Tablero->Blancas.AlfilTotales = 0;
		Tablero->Blancas.TorresTotales = 0;
		Tablero->Blancas.DamasTotales = 0;

		Tablero->Negras.PeonTotales = 0;
		Tablero->Negras.CaballosTotales = 0;
		Tablero->Negras.AlfilTotales = 0;
		Tablero->Negras.TorresTotales = 0;
		Tablero->Negras.DamasTotales = 0;
	}

	memset(Tablero->Negras.PeonDoblados, 0, sizeof(Tablero->Negras.PeonDoblados));
	memset(Tablero->Negras.PosicionTorre, 0, sizeof(Tablero->Negras.PosicionTorre));
	memset(Tablero->Negras.ReyJaquePieza, 0, sizeof(Tablero->Negras.ReyJaquePieza));

	Tablero->Negras.Puntos = PuntosCero;
	Tablero->Negras.ReyCuadrosAtacando = 0;
	Tablero->Negras.ReyAtaquesPiezas = 0;
	Tablero->Negras.ReyAtaquesValor = PuntosCero;
	Tablero->Negras.GrupoPeonesQ = 0;
	Tablero->Negras.GrupoPeonesK = 0;
}
void EvaluarPuntosPst(_ST_TableroX64 *Tablero)
{
	int i = 0;

	for (i = 0; i < 64; i++)
	{
		switch (Tablero->Tablero[i])
		{
		case CasillaVacia:
			continue;
			break;

		case PeonB:
			SUMA(&Tablero->Blancas.Puntos, &Evaluacion.PST[BLANCAS][PEON][i]);
			Tablero->Blancas.PeonTotales++;
			Tablero->Blancas.PeonDoblados[COLUMNA(i)]++;
			EvaluarPeones(Tablero, i, BLANCAS);
			break;

		case CaballoB:
			SUMA(&Tablero->Blancas.Puntos, &Evaluacion.PST[BLANCAS][CABALLO][i]);
			Tablero->Blancas.CaballosTotales++;
			EvaluarCaballo(Tablero, i, BLANCAS);
			break;

		case AlfilB:
			SUMA(&Tablero->Blancas.Puntos, &Evaluacion.PST[BLANCAS][ALFIL][i]);
			EvaluarAlFil(Tablero, i, BLANCAS);
			break;

		case TorreB:
			SUMA(&Tablero->Blancas.Puntos, &Evaluacion.PST[BLANCAS][TORRE][i]);
			Tablero->Blancas.PosicionTorre[Tablero->Blancas.TorresTotales++] = i;
			break;

		case DamaB:
			SUMA(&Tablero->Blancas.Puntos, &Evaluacion.PST[BLANCAS][DAMA][i]);
			Tablero->Blancas.DamasTotales++;
			EvaluarDama(Tablero, i, BLANCAS);
			break;

		case ReyB:
			SUMA(&Tablero->Blancas.Puntos, &Evaluacion.PST[BLANCAS][REY][i]);
			break;

		case PeonN:
			SUMA(&Tablero->Negras.Puntos, &Evaluacion.PST[NEGRAS][PEON][i]);
			Tablero->Negras.PeonTotales++;
			Tablero->Negras.PeonDoblados[COLUMNA(i)]++;
			EvaluarPeones(Tablero, i, NEGRAS);
			break;

		case CaballoN:
			SUMA(&Tablero->Negras.Puntos, &Evaluacion.PST[NEGRAS][CABALLO][i]);
			Tablero->Negras.CaballosTotales++;
			EvaluarCaballo(Tablero, i, NEGRAS);
			break;

		case AlfilN:
			SUMA(&Tablero->Negras.Puntos, &Evaluacion.PST[NEGRAS][ALFIL][i]);
			EvaluarAlFil(Tablero, i, NEGRAS);
			break;

		case TorreN:
			SUMA(&Tablero->Negras.Puntos, &Evaluacion.PST[NEGRAS][TORRE][i]);
			Tablero->Negras.PosicionTorre[Tablero->Negras.TorresTotales++] = i;
			break;

		case DamaN:
			SUMA(&Tablero->Negras.Puntos, &Evaluacion.PST[NEGRAS][DAMA][i]);
			Tablero->Negras.DamasTotales++;
			EvaluarDama(Tablero, i, NEGRAS);
			break;

		case ReyN:
			SUMA(&Tablero->Negras.Puntos, &Evaluacion.PST[NEGRAS][REY][i]);
			break;

		default:
			return;
			break;
		}
	}
}
int EvaluarInterpolar(_ST_TableroX64 *Tablero, _Valor *puntos)
{
	int mg_etapa = MIN(Tablero->Etapa, MAX_ETAPA);
	int eg_etapa = MAX_ETAPA - Tablero->Etapa;
	return (puntos->Apertura * mg_etapa + puntos->Final * eg_etapa) / MAX_ETAPA;
}
void EvaluarComplejidad(_ST_TableroX64 *Tablero, _Valor *puntos)
{
	int Complejidad;
	int Signo = (puntos->Final > 0) - (puntos->Final < 0);

	int PeonesEnAmbosLados = (Tablero->Blancas.GrupoPeonesQ + Tablero->Negras.GrupoPeonesQ) > 0 && (Tablero->Blancas.GrupoPeonesK + Tablero->Negras.GrupoPeonesK) > 0;

	Complejidad = COMPLEJIDAD_PEONES_TOTALES * (Tablero->Blancas.PeonTotales + Tablero->Negras.PeonTotales)
		+ COMPLEJIDAD_PEONES_AMBOS_LADOS * PeonesEnAmbosLados
		+ COMPLEJIDAD_PEONES_FINALES * !(Tablero->Blancas.CaballosTotales + Tablero->Negras.CaballosTotales + Tablero->Blancas.AlfilTotales + Tablero->Negras.AlfilTotales + Tablero->Blancas.TorresTotales + Tablero->Negras.TorresTotales + Tablero->Blancas.DamasTotales + Tablero->Negras.DamasTotales)
		+ COMPLEJIDAD_AJUSTE;

	int v = Signo * MAX(Complejidad, -ABS(puntos->Final));

	puntos->Final += v;
}

void EvaluarPeones(_ST_TableroX64 *Tablero, int Cs, int Turno)
{
	int x;
	int abierto = false;
	int pasado = false;
	int aislado = false;
	int retrasado = false;
	int retrasado2 = false;
	int doblado = false;
	int conectado = false;
	int Horizontal = 0;
	int Columna = 0;
	int Atras = 0;

	/* Estructura realizada */
	/* + Pasado : https://www.chessprogramming.org/Passed_Pawn */
	/* + Aislado: https://www.chessprogramming.org/Isolated_Pawn - http://en.wikipedia.org/wiki/Isolated_pawn */
	/* + Retrasado: https://www.chessprogramming.org/Backward_Pawn - http://en.wikipedia.org/wiki/Backward_pawn */
	/* + Doblado: https://www.chessprogramming.org/Doubled_Pawn */
	/* Por hacer */
	/* - Imparable: https://www.chessprogramming.org/Unstoppable_Passer */

	int DireccionPeon[3] = { -9, -8, -7 };
	int DireccionLado = 1;
	int ColumnaBorde[4] = { 0, 8, -1, 7 };

	int pPeon = Turno ? PeonB : PeonN;
	int pPeonOp = Turno ? PeonN : PeonB;
	_ST_Puntos *TempColor = Turno ? &Tablero->Blancas : &Tablero->Negras;

	if (!Turno)
	{
		DireccionPeon[0] = 7;
		DireccionPeon[1] = 8;
		DireccionPeon[2] = 9;
	}

	Horizontal = Turno ? HORIZONTAL(Cs) : 7 - HORIZONTAL(Cs);
	Columna = COLUMNA(Cs);
	Atras = Turno ? Horizontal * 8 + Cs : Cs - Horizontal * 8;

	abierto = true;
	for (x = Cs + DireccionPeon[1]; x > -1 && x < 64; x = x + DireccionPeon[1])
	{
		if (Tablero->Tablero[x] == pPeonOp)
		{
			abierto = false;
			break;
		}
		if (Tablero->Tablero[x] == pPeon)
		{
			doblado = true;
			abierto = false;
			break;
		}
	}

	if (Columna <= COLUMNA_D)
		TempColor->GrupoPeonesQ++;
	else
		TempColor->GrupoPeonesK++;

	if (abierto == true && pasado == false)
	{
		pasado = true;
		if (Columna > ColumnaBorde[0] && Columna < ColumnaBorde[1])
		{
			for (x = Cs + DireccionPeon[0]; x > -1 && x < 64; x = x + DireccionPeon[1])
			{
				if (Tablero->Tablero[x] == pPeonOp)
				{
					pasado = false;
					break;
				}
			}
		}
		if (pasado == true)
		{
			if (Columna > ColumnaBorde[2] && Columna < ColumnaBorde[3])
			{
				for (x = Cs + DireccionPeon[2]; x > -1 && x < 64; x = x + DireccionPeon[1])
				{
					if (Tablero->Tablero[x] == pPeonOp)
					{
						pasado = false;
						break;
					}
				}
			}
		}
	}

	aislado = true;
	if (Columna > ColumnaBorde[2] && Columna < ColumnaBorde[3])
	{
		for (x = Atras + DireccionLado; x > -1 && x < 64; x = x + DireccionPeon[1])
		{
			if (Tablero->Tablero[x] == pPeon)
			{
				aislado = false;
				break;
			}
		}
	}
	if (aislado == true)
	{
		if (Columna > ColumnaBorde[0] && Columna < ColumnaBorde[1])
		{
			for (x = Atras - DireccionLado; x > -1 && x < 64; x = x + DireccionPeon[1])
			{
				if (Tablero->Tablero[x] == pPeon)
				{
					aislado = false;
					break;
				}
			}
		}
	}

	if (aislado == false)
	{
		if (Columna > ColumnaBorde[2] && Columna < ColumnaBorde[3])
		{
			if (Tablero->Tablero[Cs + -DireccionPeon[0]] == pPeon)
			{
				conectado = true;
			}
		}
		if (Columna > ColumnaBorde[0] && Columna < ColumnaBorde[1])
		{
			if (Tablero->Tablero[Cs + -DireccionPeon[2]] == pPeon)
			{
				conectado = true;
			}
		}
	}

	if (conectado == false && aislado == false)
	{
		if (Columna > ColumnaBorde[0] && Columna < ColumnaBorde[1])
		{
			if (Tablero->Tablero[Cs + DireccionPeon[0]] == pPeon)
			{
				retrasado2++;
			}
		}
		if (Columna > ColumnaBorde[2] && Columna < ColumnaBorde[3])
		{
			if (Tablero->Tablero[Cs + DireccionPeon[2]] == pPeon)
			{
				retrasado2++;
			}
		}

		retrasado = (retrasado2 != 0) ? true : false;
	}

	if (doblado == true) {
		SUMA(&TempColor->Puntos, &PENALIZACION_PEON_DOBLE[Columna]);
	}
	if (aislado) {
		SUMA(&TempColor->Puntos, &PEON_AISLADO[Columna]);
	}
	else if (retrasado) {
		SUMA(&TempColor->Puntos, &PEON_RETRASADO[Columna]);
	}

	if (pasado) {
		SUMA(&TempColor->Puntos, &PEON_PASADO[Horizontal]);
	}
}
void EvaluarCaballo(_ST_TableroX64 *Tablero, int Cs, int Turno)
{
	int i = 0;
	int destino = 0;
	int Mov_Ca[8] = { -17, -15, -10, -6, 6, 10, 15, 17 };
	int mov = 0;
	int AttActivoUnaVez = false;

	int pRey = Turno ? ReyB : ReyN;
	int PosicionReyOp = Turno ? Tablero->PosicionReyN : Tablero->PosicionReyB;
	_ST_Puntos *TempColor = Turno ? &Tablero->Blancas : &Tablero->Negras;
	_ST_Puntos *TempColorOp = Turno ? &Tablero->Negras : &Tablero->Blancas;

	for (i = 0; i < 8; i++)
	{
		destino = Cs + Mov_Ca[i];

		if ((destino > -1 && destino < 64) && Tablero->TableroColor[Cs] != Tablero->TableroColor[destino])
		{
			if (Tablero->Tablero[destino] != pRey)
				mov++;

			if (DISTANCIA(destino, PosicionReyOp) == 1)
			{
				TempColorOp->ReyCuadrosAtacando += 1;
				if (AttActivoUnaVez == false)
				{
					TempColorOp->ReyAtaquesPiezas++;
					SUMA(&TempColorOp->ReyAtaquesValor, &SEGURIDAD_REY_ATAQUE_PIEZAS[CABALLO]);
				}
				AttActivoUnaVez = true;
			}
			if (DISTANCIA(destino, PosicionReyOp) == 0)
			{
				TempColorOp->ReyCuadrosAtacando += 1;
				if (AttActivoUnaVez == false)
				{
					TempColorOp->ReyAtaquesPiezas++;
					SUMA(&TempColorOp->ReyAtaquesValor, &SEGURIDAD_REY_ATAQUE_PIEZAS[CABALLO]);
				}
				AttActivoUnaVez = true;
				TempColorOp->ReyJaquePieza[CABALLO] = true;
			}
		}
	}

	SUMA(&TempColor->Puntos, &CABALLO_MOVILIDAD[mov]);
}
void EvaluarAlFil(_ST_TableroX64 *Tablero, int Cs, int Turno)
{
	int ii, destino;
	int Mov_Al[4] = { -7, -9, 7, 9 };
	int mov = 0;
	int AttActivoUnaVez = false;

	int pRey = Turno ? ReyB : ReyN;
	int PosicionReyOp = Turno ? Tablero->PosicionReyN : Tablero->PosicionReyB;
	_ST_Puntos* TempColor = Turno ? &Tablero->Blancas : &Tablero->Negras;
	_ST_Puntos* TempColorOp = Turno ? &Tablero->Negras : &Tablero->Blancas;

	for (ii = 0; ii < 4; ii++)
	{
		destino = Cs + Mov_Al[ii];

		while ((destino > -1 && destino < 64) && Tablero->TableroColor[Cs] == Tablero->TableroColor[destino])
		{
			if (Tablero->Tablero[destino] != pRey)
				mov++;

			if (DISTANCIA(destino, PosicionReyOp) == 1)
			{
				TempColorOp->ReyCuadrosAtacando += 1;
				if (AttActivoUnaVez == false)
				{
					TempColorOp->ReyAtaquesPiezas++;
					SUMA(&TempColorOp->ReyAtaquesValor, &SEGURIDAD_REY_ATAQUE_PIEZAS[ALFIL]);
				}
				AttActivoUnaVez = true;
			}
			if (DISTANCIA(destino, PosicionReyOp) == 0)
			{
				TempColorOp->ReyCuadrosAtacando += 1;
				TempColorOp->ReyJaquePieza[ALFIL] = true;
			}

			if (Tablero->Tablero[destino] != CasillaVacia) break;

			destino += Mov_Al[ii];
		}
	}

	SUMA(&TempColor->Puntos, &ALFIL_MOVILIDAD[mov]);
}
void EvaluarTorre(_ST_TableroX64 *Tablero, int Turno)
{
	int ii;
	int x;
	int destino;
	int Mov_To[4] = { 1, -1, -8, 8 };
	int Cs = 0;
	int colorCs = false;
	int mov = 0;
	int AttActivoUnaVez = false;
	int Horizontal = 0;
	int HorizontalReyOp = 0;
	int Columna = 0;

	int pRey = Turno ? ReyB : ReyN;
	int PosicionReyOp = Turno ? Tablero->PosicionReyN : Tablero->PosicionReyB;
	_ST_Puntos *TempColor = Turno ? &Tablero->Blancas : &Tablero->Negras;
	_ST_Puntos *TempColorOp = Turno ? &Tablero->Negras : &Tablero->Blancas;

	for (x = 0; x < TempColor->TorresTotales; x++)
	{
		Cs = TempColor->PosicionTorre[x];
		mov = 0;
		AttActivoUnaVez = false;
		Horizontal = Turno ? HORIZONTAL(Cs) : 7 - HORIZONTAL(Cs);
		HorizontalReyOp = Turno ? HORIZONTAL(PosicionReyOp) : 7 - HORIZONTAL(PosicionReyOp);
		Columna = COLUMNA(Cs);

		for (ii = 0; ii < 4; ii++)
		{
			destino = Cs + Mov_To[ii];
			colorCs = Tablero->TableroColor[Cs];

			while ((destino > -1 && destino < 64) && colorCs != Tablero->TableroColor[destino])
			{
				if (Tablero->Tablero[destino] != pRey)
					mov++;

				if (DISTANCIA(destino, PosicionReyOp) == 1)
				{
					TempColorOp->ReyCuadrosAtacando += 1;
					if (AttActivoUnaVez == false)
					{
						TempColorOp->ReyAtaquesPiezas++;
						SUMA(&TempColorOp->ReyAtaquesValor, &SEGURIDAD_REY_ATAQUE_PIEZAS[TORRE]);
					}
					AttActivoUnaVez = true;
				}
				if (DISTANCIA(destino, PosicionReyOp) == 0)
				{
					TempColorOp->ReyCuadrosAtacando += 1;
					TempColorOp->ReyJaquePieza[TORRE] = true;
				}

				if (Tablero->Tablero[destino] != CasillaVacia) break;

				colorCs = Tablero->TableroColor[destino];
				destino += Mov_To[ii];
			}
		}

		SUMA(&TempColor->Puntos, &TORRE_MOVILIDAD[mov]);
		if (Horizontal == HORIZONTAL_7 && HorizontalReyOp >= HORIZONTAL_7) SUMA(&TempColor->Puntos, &TORRE_EN_SEPTIMA);
		if (!TempColor->PeonDoblados[Columna])
		{
			if (!TempColorOp->PeonDoblados[Columna])
				SUMA(&TempColor->Puntos, &TORRE_ABIERTO);
			else
				SUMA(&TempColor->Puntos, &TORRE_SEMI_ABIERTO);
		}
	}
}
void EvaluarDama(_ST_TableroX64 *Tablero, int Cs, int Turno)
{
	int i, destino;
	int Mov_Da[8] = { -9, 9, -7, 7, 1, -1, 8, -8, };
	int colorCs = false;
	int mov = 0;
	int AttActivoUnaVez = false;

	int pRey = Turno ? ReyB : ReyN;
	int PosicionReyOp = Turno ? Tablero->PosicionReyN : Tablero->PosicionReyB;
	_ST_Puntos *TempColor = Turno ? &Tablero->Blancas : &Tablero->Negras;
	_ST_Puntos *TempColorOp = Turno ? &Tablero->Negras : &Tablero->Blancas;

	for (i = 0; i < 4; i++)
	{
		destino = Cs + Mov_Da[i];

		while ((destino > -1 && destino < 64) && Tablero->TableroColor[Cs] == Tablero->TableroColor[destino])
		{
			if (Tablero->Tablero[destino] != pRey)
				mov++;

			if (DISTANCIA(destino, PosicionReyOp) == 1)
			{
				TempColorOp->ReyCuadrosAtacando += 1;
				if (AttActivoUnaVez == false)
				{
					TempColorOp->ReyAtaquesPiezas++;
					SUMA(&TempColorOp->ReyAtaquesValor, &SEGURIDAD_REY_ATAQUE_PIEZAS[DAMA]);
				}
				AttActivoUnaVez = true;
			}
			if (DISTANCIA(destino, PosicionReyOp) == 0)
			{
				TempColorOp->ReyCuadrosAtacando += 1;
				TempColorOp->ReyJaquePieza[DAMA] = true;
			}

			if (Tablero->Tablero[destino] != CasillaVacia) break;

			destino += Mov_Da[i];
		}
	}

	for (i = 4; i < 8; i++)
	{
		destino = Cs + Mov_Da[i];
		colorCs = Tablero->TableroColor[Cs];

		while ((destino > -1 && destino < 64) && colorCs != Tablero->TableroColor[destino])
		{
			if (Tablero->Tablero[destino] != pRey)
				mov++;

			if (DISTANCIA(destino, PosicionReyOp) == 1)
			{
				TempColorOp->ReyCuadrosAtacando += 1;
				if (AttActivoUnaVez == false)
				{
					TempColorOp->ReyAtaquesPiezas++;
					SUMA(&TempColorOp->ReyAtaquesValor, &SEGURIDAD_REY_ATAQUE_PIEZAS[DAMA]);
				}
				AttActivoUnaVez = true;
			}
			if (DISTANCIA(destino, PosicionReyOp) == 0)
			{
				TempColorOp->ReyCuadrosAtacando += 1;
				TempColorOp->ReyJaquePieza[DAMA] = true;
			}

			if (Tablero->Tablero[destino] != CasillaVacia) break;

			colorCs = Tablero->TableroColor[destino];
			destino += Mov_Da[i];
		}
	}

	SUMA(&TempColor->Puntos, &DAMA_MOVILIDAD[mov]);
}

void EvaluarSeguridadRey(_ST_TableroX64 *Tablero, int Turno)
{
	_Valor PuntosAtaque = PuntosCero;
	_ST_Puntos *TempColor = Turno ? &Tablero->Blancas : &Tablero->Negras;
	_ST_Puntos *TempColorOp = Turno ? &Tablero->Negras : &Tablero->Blancas;

	int PosicionRey = Turno ? Tablero->PosicionReyB : Tablero->PosicionReyN;
	int ColumnaRey = MIN(6, MAX(1, COLUMNA(PosicionRey)));
	int Columna = 0;
	int Shelter = 0;
	int Storm = 0;
	int Puntos = 0;

	for (Columna = ColumnaRey - 1; Columna <= ColumnaRey + 1; Columna++)
	{
		Shelter = ProteccionPeonesReyShelter(Tablero, Columna, Turno);
		Storm = ProteccionPeonesReyStorm(Tablero, Columna, Turno);

		Puntos += (Shelter == 7 && Storm != 7) ? SEGURIDAD_REY_SEMI_ABIERTA[Columna] : 0;
		Puntos += SEGURIDAD_REY_STORM[Storm];
	}
	if (Puntos != 0) TempColor->Puntos.Apertura += Puntos * 0.8f;
	if (Puntos != 0) TempColor->Puntos.Final += Puntos;


	if (TempColor->ReyAtaquesPiezas > 1 - TempColor->DamasTotales)
	{
		SUMA(&PuntosAtaque, &TempColor->ReyAtaquesValor);

		PuntosAtaque.Apertura += SEGURIDAD_REY_ATAQUES_ZONA.Apertura * TempColor->ReyCuadrosAtacando
			+ SEGURIDAD_REY_CABALLO_JAQUE.Apertura * TempColor->ReyJaquePieza[CABALLO]
			+ SEGURIDAD_REY_ALFIL_JAQUE.Apertura * TempColor->ReyJaquePieza[ALFIL]
			+ SEGURIDAD_REY_TORRE_JAQUE.Apertura * TempColor->ReyJaquePieza[TORRE]
			+ SEGURIDAD_REY_DAMA_JAQUE.Apertura * TempColor->ReyJaquePieza[DAMA]
			+ SEGURIDAD_REY_DAMA_ENEMIGA.Apertura * !TempColorOp->DamasTotales;

		PuntosAtaque.Final += SEGURIDAD_REY_ATAQUES_ZONA.Final * TempColor->ReyCuadrosAtacando
			+ SEGURIDAD_REY_CABALLO_JAQUE.Final * TempColor->ReyJaquePieza[CABALLO]
			+ SEGURIDAD_REY_ALFIL_JAQUE.Final * TempColor->ReyJaquePieza[ALFIL]
			+ SEGURIDAD_REY_TORRE_JAQUE.Final * TempColor->ReyJaquePieza[TORRE]
			+ SEGURIDAD_REY_DAMA_JAQUE.Final * TempColor->ReyJaquePieza[DAMA]
			+ SEGURIDAD_REY_DAMA_ENEMIGA.Final * !TempColorOp->DamasTotales;

		if (-PuntosAtaque.Apertura * MAX(0, PuntosAtaque.Apertura) != 0) TempColor->Puntos.Apertura += -PuntosAtaque.Apertura * MAX(0, PuntosAtaque.Apertura) / 720;
		if (-MAX(0, PuntosAtaque.Final) != 0) TempColor->Puntos.Final += -MAX(0, PuntosAtaque.Final) / 20;
	}
}
int ProteccionPeonesReyShelter(_ST_TableroX64 *Tablero, int Cl, int Turno)
{
	_ST_Puntos *TempColor = Turno ? &Tablero->Blancas : &Tablero->Negras;
	
	if (TempColor->PeonDoblados[Cl] == 0) return 7;

	return 0;
}
int ProteccionPeonesReyStorm(_ST_TableroX64 *Tablero, int Cl, int Turno)
{
	int x = 0;
	int pPeonOp = Turno ? PeonN : PeonB;
	int DireccionPeon = Turno ? -8 : 8;
	_ST_Puntos *TempColorOp = Turno ? &Tablero->Negras : &Tablero->Blancas;

	if (TempColorOp->PeonDoblados[Cl] == 0) return 7;

	Cl = Turno ? 56 + Cl : Cl;

	for (x = Cl + DireccionPeon; x > -1 && x < 64; x = x + DireccionPeon)
	{
		if (Tablero->Tablero[x] == pPeonOp)
		{
			return Turno ? HORIZONTAL(x) : ABS((HORIZONTAL(x) - 7));
			break;
		}
	}
	return 7;
}

