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

#include "Historico.h"

void HistoricoIniciar(_ST_TableroX64 *Tablero)
{
	memset(Tablero->His.Historico_Movimientos, 0, sizeof(Tablero->His.Historico_Movimientos));
	memset(Tablero->His.Historico_Killer, 0, sizeof(Tablero->His.Historico_Killer));
	memset(Tablero->His.Historico_Killer_Mate, 0, sizeof(Tablero->His.Historico_Killer_Mate));
	memset(Tablero->His.HistoricoRefutacion, 0, sizeof(Tablero->His.HistoricoRefutacion));
	memset(Tablero->His.HistoricoCaptura, 0, sizeof(Tablero->His.HistoricoCaptura));
	Tablero->His.Max_Historico = 0;
	Tablero->His.Min_Historico = 0;
	Tablero->His.Max_Historico_Cap = 0;
	Tablero->His.Min_Historico_Cap = 0;
}
int MovimientoTranquilo(int *M)
{
	/*
	https://www.chessprogramming.org/Quiet_Moves
	Movimientos tranquilo, son todos los movimientos que no alteren material, por tanto, no se presentaron capturas ni promociones.
	Tambien se puede excluir movimientos que presentan las amenazas inminentes, tales como jaque.
	*/

	if (CAPTURADA(*M) != MFLAGCAP)
		return false;
	if (CORONACION(*M) != MFLAGPROM)
		return false;
	if (CAPTURA_ALPASO(*M) == MFLAGEP)
		return false;

	return true;
}
int MovimientoKiller(int *M, _ST_TableroX64 *Tablero)
{
	if (!HistoricoEsMovimientoKiller(M, Tablero))
		return HistoricoEsMovimientoKillerMate(M, Tablero);
	else
		return true;
}
int HistoricoEsMovimientoKiller(int *M, _ST_TableroX64 *Tablero)
{
	if (*M == Tablero->His.Historico_Killer[Tablero->Ply][0]) return true;
	if (*M == Tablero->His.Historico_Killer[Tablero->Ply][1]) return true;
	return false;
}
int HistoricoEsMovimientoKillerMate(int *M, _ST_TableroX64 *Tablero)
{
	if (*M == Tablero->His.Historico_Killer_Mate[Tablero->Ply][0]) return true;
	if (*M == Tablero->His.Historico_Killer_Mate[Tablero->Ply][1]) return true;
	return false;
}
void HistoricoActualizar(int depth, int *ply, int M, int kMate, int ML[], int Nmov, _ST_TableroX64 *Tablero)
{
	int i = 0;
	S64 h = 0;
	int Inicio = 0;
	int Fin = 0;
	int Puntos = 0;
	int PiezaCn = NO_MOVIMIENTO;
	int DestinoCn = 0;
	int OrigenCn = 0;
	int Color = 0;

	if (kMate)
	{
		if (M != Tablero->His.Historico_Killer_Mate[*ply][0])
		{
			Tablero->His.Historico_Killer_Mate[*ply][1] = Tablero->His.Historico_Killer_Mate[*ply][0];
			Tablero->His.Historico_Killer_Mate[*ply][0] = M;
		}
	}
	else
	{
		if (M != Tablero->His.Historico_Killer[*ply][0])
		{
			Tablero->His.Historico_Killer[*ply][1] = Tablero->His.Historico_Killer[*ply][0];
			Tablero->His.Historico_Killer[*ply][0] = M;
		}
	}

	if (*ply > 0 && Tablero->Estado[*ply - 1].Movimiento != NO_MOVIMIENTO)
	{
		PiezaCn = PIEZAMOVIDA(Tablero->Estado[*ply - 1].Movimiento);
		DestinoCn = CUADRADO_DESTINO(Tablero->Estado[*ply - 1].Movimiento);
		OrigenCn = CUADRADO_ORIGEN(Tablero->Estado[*ply - 1].Movimiento);

		if (PiezaCn > NO_MOVIMIENTO && PiezaCn < NoPieza)
		{
			if (PiezaCn > CasillaVacia)
			{
				PiezaCn = PiezaCn - 7;
				Color = 1;
			}

			PiezaCn -= 1;

			Tablero->His.HistoricoRefutacion[Color][PiezaCn][OrigenCn][DestinoCn] = M;
		}
	}

	for (i = 0; i < Nmov; i++)
	{
		Fin = CUADRADO_DESTINO(ML[i]);
		Inicio = CUADRADO_ORIGEN(ML[i]);
		Puntos = (ML[i] == M) ? (depth * depth) : -(depth * depth);
		Tablero->His.Historico_Movimientos[Inicio][Fin] += Puntos;
		h = Tablero->His.Historico_Movimientos[Inicio][Fin];
		if (h > Tablero->His.Max_Historico) Tablero->His.Max_Historico = h;
		else if (h < Tablero->His.Min_Historico) Tablero->His.Min_Historico = h;
	}
}
void HistoricoActualizarCapturas(int depth, int M, int MC[], int Ncap, _ST_TableroX64 *Tablero)
{
	int i = 0;
	S64 h = 0;
	int Inicio = 0;
	int Fin = 0;
	int Puntos = 0;

	for (i = 0; i < Ncap; i++)
	{
		Fin = CUADRADO_DESTINO(MC[i]);
		Inicio = CUADRADO_ORIGEN(MC[i]);
		Puntos = (MC[i] == M) ? (depth * depth) : -(depth * depth);
		Tablero->His.HistoricoCaptura[Inicio][Fin] += Puntos;
		h = Tablero->His.HistoricoCaptura[Inicio][Fin];
		if (h > Tablero->His.Max_Historico_Cap) Tablero->His.Max_Historico_Cap = h;
		else if (h < Tablero->His.Min_Historico_Cap) Tablero->His.Min_Historico_Cap = h;
	}
}
int HistoricoMovimientoRefutacion(_ST_TableroX64 *Tablero)
{
	if (Tablero->Ply > 1 && Tablero->Estado[Tablero->Ply - 1].Movimiento != NO_MOVIMIENTO)
	{
		int PiezaCn = PIEZAMOVIDA(Tablero->Estado[Tablero->Ply - 1].Movimiento);
		int DestinoCn = CUADRADO_DESTINO(Tablero->Estado[Tablero->Ply - 1].Movimiento);
		int OrigenCn = CUADRADO_ORIGEN(Tablero->Estado[Tablero->Ply - 1].Movimiento);
		int Color = 0;

		if (PiezaCn > NO_MOVIMIENTO && PiezaCn < NoPieza)
		{
			if (PiezaCn > CasillaVacia)
			{
				PiezaCn = PiezaCn - 7;
				Color = 1;
			}

			PiezaCn -= 1;

			return Tablero->His.HistoricoRefutacion[Color][PiezaCn][OrigenCn][DestinoCn];
		}
	}
	return NO_MOVIMIENTO;
}
int HistoricoValor(int *M, _ST_TableroX64 *Tablero)
{
	int Inicio = CUADRADO_ORIGEN(*M);
	int Fin = CUADRADO_DESTINO(*M);
	S64 v = Tablero->His.Historico_Movimientos[Inicio][Fin];
	if (v > 0 && Tablero->His.Max_Historico != 0)
		return (int)((S64)((S64)100 * v) / Tablero->His.Max_Historico) + 1;
	else if (v < 0 && Tablero->His.Min_Historico != 0)
		return (int)((S64)((S64 )-100 * v) / Tablero->His.Min_Historico) + -1;
	else
		return 0;
}
int HistoricoCapturaValor(int *M, _ST_TableroX64 *Tablero)
{
	int Inicio = CUADRADO_ORIGEN(*M);
	int Fin = CUADRADO_DESTINO(*M);
	S64 v = Tablero->His.HistoricoCaptura[Inicio][Fin];
	if (v > 0 && Tablero->His.Max_Historico_Cap != 0)
		return (int)((S64)((S64)100 * v) / Tablero->His.Max_Historico_Cap) + 1;
	else if (v < 0 && Tablero->His.Min_Historico_Cap != 0)
		return (int)((S64)((S64)-100 * v) / Tablero->His.Min_Historico_Cap) + -1;
	else
		return 0;
}

