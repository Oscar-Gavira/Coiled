/*
Coiled-NNUE is a UCI chess playing engine authored by Oscar Gavira.
Copyright (C) 2013-2021 Oscar Gavira.
<https://github.com/Oscar-Gavira/Coiled-NNUE>

Coiled-NNUE is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Coiled-NNUE is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#include "Externo.h"
#include "Historico.h"

/*******************************************************************************
							Variables
*******************************************************************************/
int Historico_Bueno[64][64];													/* [Origen][Destino] */
int Historico_Killer[MAX_PLY][2];												/* [MAXPLY][2 jugadas] */
int Historico_Killer_Mate[MAX_PLY][2];											/* [MAXPLY][2 jugadas] */

int Max_Historico = 0;
int Min_Historico = 0;

/* Inicializar Histórico y killer heurístico */
void HistoricoIniciar()
{
	memset(Historico_Bueno, 0, 64 * 64 * sizeof(int));
	memset(Historico_Killer, 0, MAX_PLY * 2 * sizeof(int));
	memset(Historico_Killer_Mate, 0, MAX_PLY * 2 * sizeof(int));
	Max_Historico = 0;
	Min_Historico = 0;
}

/* Actualizamos histórico y killer */
void HistoricoActualizar(int depth, int *ply, int M, int kMate, int *ML, int Nmov)
{
	int i = 0;
	int v = 0;
	int Inicio = CUADRADO_ORIGEN(M);
	int Fin = CUADRADO_DESTINO(M);

	if (kMate)
	{
		if (M != Historico_Killer_Mate[*ply][0])
		{
			Historico_Killer_Mate[*ply][1] = Historico_Killer_Mate[*ply][0];
			Historico_Killer_Mate[*ply][0] = M;
		}
	}
	else
	{
		if (M != Historico_Killer[*ply][0])
		{
			Historico_Killer[*ply][1] = Historico_Killer[*ply][0];
			Historico_Killer[*ply][0] = M;
		}
	}

	Historico_Bueno[Inicio][Fin] += depth * depth;
	v = Historico_Bueno[Inicio][Fin];
	if (v > Max_Historico)
	{
		Max_Historico = v;
	}

	for (i = 0; i < Nmov - 1; i++)
	{
		Inicio = CUADRADO_ORIGEN(ML[i]);
		Fin = CUADRADO_DESTINO(ML[i]);

		Historico_Bueno[Inicio][Fin] += -(depth * depth);
		v = Historico_Bueno[Inicio][Fin];
		if (v < Min_Historico)
		{
			Min_Historico = v;
		}

	}
}

/* Es un movimiento importante */
int MovimientoInquieto(int *M)
{
	/*
	https://www.chessprogramming.org/Quiet_Moves
	Movimientos tranquilo, son todos los movimientos que no alteren material, por tanto, no se presentaron capturas ni promociones.
	También se puede excluir movimientos que presentan las amenazas inminentes, tales como cheque.
	*/

	if (CAPTURADA(*M) != MFLAGCAP)
	{
		return false;
	}
	if (CORONACION(*M) != MFLAGPROM)
	{
		return false;
	}
	if (CAPTURA_ALPASO(*M) == MFLAGEP)
	{
		return false;
	}

	return true;
}


/* Indica si el movimiento es un killer y Mate killer (Podas y reducciones)*/
int ComprobarMovimientoKiller(int *ply, int *M)
{
	if (!HistoricoEsMovimientoKiller(ply, M))
	{
		return HistoricoEsMovimientoKillerMate(ply, M);
	}
	else
	{
		return true;
	}
}

/* Indica si es un movimiento killer */
int HistoricoEsMovimientoKiller(int *ply, int *M)
{
	if (*M == Historico_Killer[*ply][0])
	{
		return true;
	}
	if (*M == Historico_Killer[*ply][1])
	{
		return true;
	}

	return false;
}

/* Indica si es un movimiento killer mate (Ordenación)*/
int HistoricoEsMovimientoKillerMate(int *ply, int *M)
{
	if (*M == Historico_Killer_Mate[*ply][0])
	{
		return true;
	}
	if (*M == Historico_Killer_Mate[*ply][1])
	{
		return true;
	}

	return false;
}

/* Histórico de valores. Para la ordenación de movimientos. Devuelve del -102 al 100. Porcentaje en vez de valor*/
int HistoricoValor(int *M)
{
	int v = Historico_Bueno[CUADRADO_ORIGEN(*M)][CUADRADO_DESTINO(*M)];

	if (v > 0 && Max_Historico != 0)
		return (int)((100 * v) / Max_Historico); /* Lo decimales 0,02 se convierten en 0 */
	else if (v < 0 && Min_Historico != 0)
		return (int)((-100 * v) / Min_Historico) + -1;	/* Los decimales -0,01 se convierten en -1. Valor máximo -101 */
	else
		return -102;
}

