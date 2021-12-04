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
#include "Historico.h"

/*******************************************************************************
							Variables
*******************************************************************************/
S64 Historico[64][64];													/* [Origen][Destino] */
int Historico_Killer[MAX_PLY][2];										/* [MAXPLY][2 jugadas] */
int Historico_Killer_Mate[MAX_PLY][2];									/* [MAXPLY][2 jugadas] */
int Historico_Refutacion[64][64];										/* [Origen][Destino] */
int Historico_Adversario[64][64][64];									/* [Origen][Destino][Origen] */
int Historico_Anterior[64][64][64];										/* [Origen][Destino][Origen] */

S64 Max_Historico = 0;
S64 Min_Historico = 0;
S64 Min_Historico_Adv = 0;
S64 Max_Historico_Adv = 0;
S64 Min_Historico_Ant = 0;
S64 Max_Historico_Ant = 0;

/* Es un movimiento tranquilo */
int MovimientoTranquilo(int *M)
{
	/*
	https://www.chessprogramming.org/Quiet_Moves
	Movimientos tranquilo, son todos los movimientos que no alteren material, por tanto, no se presentaron capturas ni promociones.
	Tambien se puede excluir movimientos que presentan las amenazas inminentes, tales como jaque.
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

/* Indica si el movimiento es un killer y Mate killer */
int MovimientoKiller(int *ply, int *M)
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

/* Inicializar Historico y killer heuristico */
void HistoricoIniciar()
{
	memset(Historico, 0, 64 * 64 * sizeof(S64));
	memset(Historico_Killer, 0, MAX_PLY * 2 * sizeof(int));
	memset(Historico_Killer_Mate, 0, MAX_PLY * 2 * sizeof(int));
	memset(Historico_Refutacion, 0, 64 * 64 * sizeof(int));
	memset(Historico_Adversario, 0, 64 * 64 * 64 * sizeof(int));
	memset(Historico_Anterior, 0, 64 * 64 * 64 * sizeof(int));

	Max_Historico = 0;
	Min_Historico = 0;
	Min_Historico_Adv = 0;
	Max_Historico_Adv = 0;
	Min_Historico_Ant = 0;
	Max_Historico_Ant = 0;
}

/* Actualizamos historico y killer */
void HistoricoActualizar(int depth, int *ply, int M, int kMate, int *ML, int Nmov)
{
	int i = 0;
	S64 h = 0;
	int Inicio = 0;
	int Fin = 0;
	int Puntos = 0;
	int DestinoCn = -1;
	int OrigenCn = -1;
	int OrigenAnt = -1;

	/* Killer */
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
	/* Refutation */
	if (*ply - 1 >= 0 && TableroGlobal.Estado[*ply - 1].Movimiento != NO_MOVIMIENTO)
	{
		DestinoCn = CUADRADO_DESTINO(TableroGlobal.Estado[*ply - 1].Movimiento);
		OrigenCn = CUADRADO_ORIGEN(TableroGlobal.Estado[*ply - 1].Movimiento);

		if (DestinoCn >= 0 && DestinoCn <= 63)
			Historico_Refutacion[OrigenCn][DestinoCn] = M;
	}

	if (*ply - 2 >= 0 && TableroGlobal.Estado[*ply - 2].Movimiento != NO_MOVIMIENTO)
	{
		OrigenAnt = CUADRADO_ORIGEN(TableroGlobal.Estado[*ply - 2].Movimiento);
	}

	/* Historico, Historico contador */
	for (i = 0; i < Nmov; i++)
	{
		Fin = CUADRADO_DESTINO(ML[i]);
		Inicio = CUADRADO_ORIGEN(ML[i]);

		Puntos = (ML[i] == M) ? (depth * depth) : -(depth * depth);

		/* Historico */
		Historico[Inicio][Fin] += Puntos;
		h = Historico[Inicio][Fin];
		if (h > Max_Historico) Max_Historico = h;
		else if (h < Min_Historico) Min_Historico = h;

		/* Historico Counter Move */
		if (OrigenCn >= 0 && OrigenCn <= 63)
		{
			Historico_Adversario[Inicio][Fin][OrigenCn] += Puntos;

			h = Historico_Adversario[Inicio][Fin][OrigenCn];
			if (h > Max_Historico_Adv) Max_Historico_Adv = h;
			else if (h < Min_Historico_Adv) Min_Historico_Adv = h;
		}

		/* Historico Followup Move */
		if (OrigenAnt >= 0 && OrigenAnt <= 63)
		{
			Historico_Anterior[Inicio][Fin][OrigenAnt] += Puntos;

			h = Historico_Anterior[Inicio][Fin][OrigenAnt];
			if (h > Max_Historico_Ant) Max_Historico_Ant = h;
			else if (h < Min_Historico_Ant) Min_Historico_Ant = h;
		}
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

/* Indica si es un movimiento killer mate */
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

int HistoricoMovimientoRefutacion()
{
	if (TableroGlobal.Ply - 1 >= 0 && TableroGlobal.Estado[TableroGlobal.Ply - 1].Movimiento != NO_MOVIMIENTO)
	{
		int DestinoCn = CUADRADO_DESTINO(TableroGlobal.Estado[TableroGlobal.Ply - 1].Movimiento);
		int OrigenCn = CUADRADO_ORIGEN(TableroGlobal.Estado[TableroGlobal.Ply - 1].Movimiento);

		if (DestinoCn >= 0 && DestinoCn <= 63)
			return Historico_Refutacion[OrigenCn][DestinoCn];
	}
	return NO_MOVIMIENTO;
}

int HistoricoValorAdversario(int *M)
{
	S64 v = 0;

	if (TableroGlobal.Ply - 1 >= 0 && TableroGlobal.Estado[TableroGlobal.Ply - 1].Movimiento != NO_MOVIMIENTO)
	{
		int OrigenCn = CUADRADO_ORIGEN(TableroGlobal.Estado[TableroGlobal.Ply - 1].Movimiento);

		if (OrigenCn >= 0 && OrigenCn <= 63)
		{
			v = Historico_Adversario[CUADRADO_ORIGEN(*M)][CUADRADO_DESTINO(*M)][OrigenCn];
			if (v > 0 && Max_Historico_Adv != 0)
				return (int)((S64)((S64)100 * v) / Max_Historico_Adv) + 1;	/* Los decimales 0,02 se convierten en 1. Valor maximo 101 */
			else if (v < 0 && Min_Historico_Adv != 0)
				return (int)((S64)((S64)-100 * v) / Min_Historico_Adv) + -1;	/* Los decimales -0,01 se convierten en -1. Valor maximo -101 */
			else
				return 0;
		}
	}
	return 0;
}
int HistoricoValorAnterior(int *M)
{
	S64 v = 0;

	if (TableroGlobal.Ply - 2 >= 0 && TableroGlobal.Estado[TableroGlobal.Ply - 2].Movimiento != NO_MOVIMIENTO)
	{
		int OrigenCn = CUADRADO_ORIGEN(TableroGlobal.Estado[TableroGlobal.Ply - 2].Movimiento);

		if (OrigenCn >= 0 && OrigenCn <= 63)
		{
			v = Historico_Anterior[CUADRADO_ORIGEN(*M)][CUADRADO_DESTINO(*M)][OrigenCn];
			if (v > 0 && Max_Historico_Ant != 0)
				return (int)((S64)((S64)100 * v) / Max_Historico_Ant) + 1;	/* Los decimales 0,02 se convierten en 1. Valor maximo 101 */
			else if (v < 0 && Min_Historico_Ant != 0)
				return (int)((S64)((S64)-100 * v) / Min_Historico_Ant) + -1;	/* Los decimales -0,01 se convierten en -1. Valor maximo -101 */
			else
				return 0;
		}
	}
	return 0;
}

/* Historico. Para la ordenacion de movimientos. Devuelve del -102 al 100. Porcentaje en vez de valor*/
int HistoricoValor(int *M)
{
	S64 v = Historico[CUADRADO_ORIGEN(*M)][CUADRADO_DESTINO(*M)];

	if (v > 0 && Max_Historico != 0)
		return (int)((S64)((S64)100 * v) / Max_Historico) + 1;	/* Los decimales 0,02 se convierten en 1. Valor maximo 101 */
	else if (v < 0 && Min_Historico != 0)
		return (int)((S64)((S64)-100 * v) / Min_Historico) + -1;	/* Los decimales -0,01 se convierten en -1. Valor maximo -101 */
	else
		return 0;
}
