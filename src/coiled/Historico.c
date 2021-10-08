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
int HistoricoRefutacion[7][64][64];										/* [Pieza][Origen][Destino] */

S64 Max_Historico = 0;
S64 Min_Historico = 0;

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
	memset(HistoricoRefutacion, 0, 7 * 64 * 64 * sizeof(int));

	Max_Historico = 0;
	Min_Historico = 0;
}

/* Actualizamos historico y killer */
void HistoricoActualizar(int depth, int *ply, int M, int kMate, int *ML, int Nmov)
{
	int i = 0;
	S64 h = 0;
	int Inicio = 0;
	int Fin = 0;
	int Puntos = 0;
	int PiezaCn = NO_MOVIMIENTO;
	int DestinoCn = 0;
	int OrigenCn = 0;

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

	if (*ply > 0)
	{
		PiezaCn = PIEZAMOVIDA(TableroGlobal.Estado[TableroGlobal.Ply - 1].Movimiento);
		DestinoCn = CUADRADO_DESTINO(PiezaCn);
		OrigenCn = CUADRADO_ORIGEN(PiezaCn);
		if (PiezaCn > CasillaVacia)
			PiezaCn = PiezaCn - 7;
		
		if (PiezaCn != NO_MOVIMIENTO)
			HistoricoRefutacion[PiezaCn][OrigenCn][DestinoCn] = M;
	}

	for (i = 0; i < Nmov; i++)
	{
		Fin = CUADRADO_DESTINO(ML[i]);
		Inicio = CUADRADO_ORIGEN(ML[i]);

		Puntos = (ML[i] == M) ? (depth * depth) : -(depth * depth);

		Historico[Inicio][Fin] += Puntos;
		h = Historico[Inicio][Fin];
		if (h > Max_Historico) Max_Historico = h;
		else if (h < Min_Historico) Min_Historico = h;
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
	if (TableroGlobal.Ply > 0)
	{
		int PiezaCn = PIEZAMOVIDA(TableroGlobal.Estado[TableroGlobal.Ply - 1].Movimiento);
		int DestinoCn = 0;
		int OrigenCn = 0;

		if (PiezaCn != NO_MOVIMIENTO)
		{
			DestinoCn = CUADRADO_DESTINO(PiezaCn);
			OrigenCn = CUADRADO_ORIGEN(PiezaCn);

			if (PiezaCn > CasillaVacia)
				PiezaCn = PiezaCn - 7;

			if (PiezaCn != NO_MOVIMIENTO)
				return HistoricoRefutacion[PiezaCn][OrigenCn][DestinoCn];
		}
	}
	return NO_MOVIMIENTO;
}

/* Historico. Para la ordenacion de movimientos. Devuelve del -102 al 100. Porcentaje en vez de valor*/
int HistoricoValor(int *M)
{
	S64 v = Historico[CUADRADO_ORIGEN(*M)][CUADRADO_DESTINO(*M)];

	if (v > 0 && Max_Historico != 0)
		return (int)((S64)((S64)100 * v) / Max_Historico) + 1;	/* Los decimales 0,02 se convierten en 1. Valor maximo 101 */
	else if (v < 0 && Min_Historico != 0)
		return (int)((S64)((S64 )-100 * v) / Min_Historico) + -1;	/* Los decimales -0,01 se convierten en -1. Valor maximo -101 */
	else
		return 0;
}
