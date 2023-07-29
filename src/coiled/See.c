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

#include "See.h"

int See(int *M, int turno, _ST_TableroX64 *Tablero)
{
	_ST_SeeTablero SeeMove;
	int i = 0;
	int Temp = 0;
	int Indice_Total = 0;
	int PiezaCapturada = CAPTURADA(*M);
	int Fin = CUADRADO_DESTINO(*M);
	int Total[40] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };

	SeeMove.SeePly = 0;

	if (PiezaCapturada != MFLAGCAP)
	{
		if (!SeeHacerMovimiento(M, &SeeMove, Tablero))
		{
			SeeDeshacerMovimiento(&SeeMove, Tablero);
			return 1;
		}

		turno = !turno;
		for (i = 0; i < 40; i++)
		{
			Total[Indice_Total] = ObtenerAtaquesMenores(Fin, &SeeMove, &turno, Tablero);
			if (Total[Indice_Total] == NO_MOVIMIENTO)
			{
				break;
			}
			else
			{
				Indice_Total++;
				turno = !turno;
				continue;
			}
		}

		while (SeeMove.SeePly > 0)
		{
			SeeDeshacerMovimiento(&SeeMove, Tablero);
		}

		if (Total[0] != NO_MOVIMIENTO)
		{
			Temp = ValorPieza(PiezaCapturada);
			if (CORONACION(*M) != MFLAGPROM)
				Temp -= ValorPieza(PeonB);

			for (i = 0; i < Indice_Total; i++)
			{
				if (Total[i] != NO_MOVIMIENTO)
				{
					if (i % 2)
						Temp += ValorPieza(CAPTURADA(Total[i]));
					else
						Temp += -ValorPieza(CAPTURADA(Total[i]));
				}
			}
		}
		else
		{
			Temp = ValorPieza(PiezaCapturada);
		}
	}

	return Temp;
}
int ObtenerAtaquesMenores(int Cs, _ST_SeeTablero *SeeTablero, int *Turno, _ST_TableroX64 *Tablero)
{
	int move = NO_MOVIMIENTO;

	if (*Turno)
	{
		move = SeePeones(Cs, SeeTablero, Turno, Tablero);
		if (move)
		{
			return move;
		}
		move = SeeCaballo(Cs, SeeTablero, Turno, Tablero);
		if (move)
		{
			return move;
		}
		move = SeeAlfil(Cs, SeeTablero, Turno, Tablero);
		if (move)
		{
			return move;
		}
		move = SeeTorre(Cs, SeeTablero, Turno, Tablero);
		if (move)
		{
			return move;
		}
		move = SeeDama(Cs, SeeTablero, Turno, Tablero);
		if (move)
		{
			return move;
		}
		move = SeeRey(Cs, SeeTablero, Turno, Tablero);
		if (move)
		{
			return move;
		}
	}
	else
	{
		move = SeePeones(Cs, SeeTablero, Turno, Tablero);
		if (move)
		{
			return move;
		}
		move = SeeCaballo(Cs, SeeTablero, Turno, Tablero);
		if (move)
		{
			return move;
		}
		move = SeeAlfil(Cs, SeeTablero, Turno, Tablero);
		if (move)
		{
			return move;
		}
		move = SeeTorre(Cs, SeeTablero, Turno, Tablero);
		if (move)
		{
			return move;
		}
		move = SeeDama(Cs, SeeTablero, Turno, Tablero);
		if (move)
		{
			return move;
		}
		move = SeeRey(Cs, SeeTablero, Turno, Tablero);
		if (move)
		{
			return move;
		}
	}

	return move;
}
int SeePeones(int Cs, _ST_SeeTablero *SeeTablero, int *Turno, _ST_TableroX64 *Tablero)
{
	int See_Mov_Pe[2] = { 7, 9 };
	int i;
	int destino;
	int SeeMov = NO_MOVIMIENTO;
	int P = *Turno ? PeonB : PeonN;
	int Coronacion = 0;
	int Inicio = 0;
	int Fin = 0;
	if (*Turno)
	{
		Inicio = CasillaVacia;
		Fin = ReyN;
		Coronacion = (Cs < 8) ? DamaB : MFLAGPROM;
	}
	else
	{
		See_Mov_Pe[0] = -7;
		See_Mov_Pe[1] = -9;
		Inicio = 0;
		Fin = ReyB;
		Coronacion = (Cs > 55) ? DamaN : MFLAGPROM;
	}

	for (i = 0; i < 2; i++)
	{
		destino = Cs + See_Mov_Pe[i];
		if (destino < 0 || destino > 63)
		{
			continue;
		}

		if (Tablero->TableroColor[Cs] == Tablero->TableroColor[destino] && Tablero->Tablero[destino] == P)
		{
			if (Tablero->Tablero[Cs] > Inicio && Tablero->Tablero[Cs] < Fin)
			{
				SeeMov = MOVIMIENTO(destino, Cs, Tablero->Tablero[destino], (Tablero->Tablero[Cs] == CasillaVacia) ? MFLAGCAP : Tablero->Tablero[Cs], Coronacion, 0);
				if (!SeeHacerMovimiento(&SeeMov, SeeTablero, Tablero))
				{
					SeeDeshacerMovimiento(SeeTablero, Tablero);
					continue;
				}
				return SeeMov;
			}
		}
	}
	return NO_MOVIMIENTO;
}
int SeeCaballo(int Cs, _ST_SeeTablero *SeeTablero, int *Turno, _ST_TableroX64 *Tablero)
{
	int i = 0;
	int destino = 0;
	int SeeMov = NO_MOVIMIENTO;
	int P = *Turno ? CaballoB : CaballoN;
	int Inicio = 0;
	int Fin = 0;
	int See_Mov_Ca[8] = { -17, -15, -10, -6, 6, 10, 15, 17 };

	if (*Turno)
	{
		Inicio = CasillaVacia;
		Fin = ReyN;
	}
	else
	{
		Inicio = 0;
		Fin = ReyB;
	}

	for (i = 0; i < 8; i++)
	{
		destino = Cs + See_Mov_Ca[i];
		if (destino < 0 || destino > 63)
		{
			continue;
		}

		if (Tablero->TableroColor[Cs] != Tablero->TableroColor[destino] && Tablero->Tablero[destino] == P)
		{
			if (Tablero->Tablero[Cs] > Inicio && Tablero->Tablero[Cs] < Fin)
			{
				SeeMov = MOVIMIENTO(destino, Cs, Tablero->Tablero[destino], (Tablero->Tablero[Cs] == CasillaVacia) ? MFLAGCAP : Tablero->Tablero[Cs], MFLAGPROM, 0);
		        if (!SeeHacerMovimiento(&SeeMov, SeeTablero, Tablero))
				{
					SeeDeshacerMovimiento(SeeTablero, Tablero);
					continue;
				}
				return SeeMov;
			}
		}
	}
	return NO_MOVIMIENTO;
}
int SeeAlfil(int Cs, _ST_SeeTablero *SeeTablero, int *Turno, _ST_TableroX64 *Tablero)
{
	int i = 0;
	int destino = 0;
	int SeeMov = NO_MOVIMIENTO;
	int P = *Turno ? AlfilB : AlfilN;
	int Inicio = 0;
	int Fin = 0;
	int See_Mov_Al[4] = { 9, -9, -7, 7 };

	if (*Turno)
	{
		Inicio = CasillaVacia;
		Fin = ReyN;
	}
	else
	{
		Inicio = 0;
		Fin = ReyB;
	}

	for ( i = 0; i < 4; i++)
	{
		destino = Cs + See_Mov_Al[i];
		if (destino < 0 || destino > 63)
		{
			continue;
		}

		while ((destino > -1 && destino < 64) && Tablero->TableroColor[Cs] == Tablero->TableroColor[destino] && Tablero->Tablero[destino] == CasillaVacia)
		{
			destino += See_Mov_Al[i];
		}
		if ((destino > -1 && destino < 64) && Tablero->TableroColor[Cs] == Tablero->TableroColor[destino] && Tablero->Tablero[destino] == P)
		{
			if (Tablero->Tablero[Cs] > Inicio && Tablero->Tablero[Cs] < Fin)
			{
				SeeMov = MOVIMIENTO(destino, Cs, Tablero->Tablero[destino], (Tablero->Tablero[Cs] == CasillaVacia) ? MFLAGCAP : Tablero->Tablero[Cs], MFLAGPROM, 0);
		        if (!SeeHacerMovimiento(&SeeMov, SeeTablero, Tablero))
				{
					SeeDeshacerMovimiento(SeeTablero, Tablero);
					continue;
				}
				return SeeMov;
			}
		}
	}
	return NO_MOVIMIENTO;
}
int SeeTorre(int Cs, _ST_SeeTablero *SeeTablero, int *Turno, _ST_TableroX64 *Tablero)
{
	int i = 0;
	int destino = 0;
	int SeeMov = NO_MOVIMIENTO;
	int colorCs = false;
	int P = *Turno ? TorreB : TorreN;
	int Inicio = 0;
	int Fin = 0;
	int See_Mov_To[4] = { 1, -1, -8, 8 };

	if (*Turno)
	{
		Inicio = CasillaVacia;
		Fin = ReyN;
	}
	else
	{
		Inicio = 0;
		Fin = ReyB;
	}

	for ( i = 0; i < 4; i++)
	{
		destino = Cs + See_Mov_To[i];
		if (destino < 0 || destino > 63)
		{
			continue;
		}

		colorCs = Tablero->TableroColor[Cs];

		while ((destino > -1 && destino < 64) && colorCs != Tablero->TableroColor[destino] && Tablero->Tablero[destino] == CasillaVacia)
		{
			colorCs = Tablero->TableroColor[destino];
			destino += See_Mov_To[i];
		}
		if ((destino > -1 && destino < 64) && colorCs != Tablero->TableroColor[destino] && Tablero->Tablero[destino] == P)
		{
			if (Tablero->Tablero[Cs] > Inicio && Tablero->Tablero[Cs] < Fin)
			{
				SeeMov = MOVIMIENTO(destino, Cs, Tablero->Tablero[destino], (Tablero->Tablero[Cs] == CasillaVacia) ? MFLAGCAP : Tablero->Tablero[Cs], MFLAGPROM, 0);
		        if (!SeeHacerMovimiento(&SeeMov, SeeTablero, Tablero))
				{
					SeeDeshacerMovimiento(SeeTablero, Tablero);
					continue;
				}
				return SeeMov;
			}
		}
	}
	return NO_MOVIMIENTO;
}
int SeeDama(int Cs, _ST_SeeTablero *SeeTablero, int *Turno, _ST_TableroX64 *Tablero)
{
	int i = 0;
	int destino = 0;
	int SeeMov = NO_MOVIMIENTO;
	int colorCs = false;
	int P = *Turno ? DamaB : DamaN;
	int Inicio = 0;
	int Fin = 0;
	int See_Mov_Da[8] = { -9, 9, -7, 7, 1, -1, 8, -8 };

	if (*Turno)
	{
		Inicio = CasillaVacia;
		Fin = ReyN;
	}
	else
	{
		Inicio = 0;
		Fin = ReyB;
	}
	for (i = 0; i < 4; i++)
	{
		destino = Cs + See_Mov_Da[i];
		if (destino < 0 || destino > 63)
		{
			continue;
		}

		while ((destino > -1 && destino < 64) && Tablero->TableroColor[Cs] == Tablero->TableroColor[destino] && Tablero->Tablero[destino] == CasillaVacia)
		{
			destino += See_Mov_Da[i];
		}
		if ((destino > -1 && destino < 64) && Tablero->TableroColor[Cs] == Tablero->TableroColor[destino] && Tablero->Tablero[destino] == P)
		{
			if (Tablero->Tablero[Cs] > Inicio && Tablero->Tablero[Cs] < Fin)
			{
				SeeMov = MOVIMIENTO(destino, Cs, Tablero->Tablero[destino], (Tablero->Tablero[Cs] == CasillaVacia) ? MFLAGCAP : Tablero->Tablero[Cs], MFLAGPROM, 0);
		        if (!SeeHacerMovimiento(&SeeMov, SeeTablero, Tablero))
				{
					SeeDeshacerMovimiento(SeeTablero, Tablero);
					continue;
				}
				return SeeMov;
			}
		}
	}

	for (i = 4; i < 8; i++)
	{
		destino = Cs + See_Mov_Da[i];
		if (destino < 0 || destino > 63)
		{
			continue;
		}

		colorCs = Tablero->TableroColor[Cs];

		while ((destino > -1 && destino < 64) && colorCs != Tablero->TableroColor[destino] && Tablero->Tablero[destino] == CasillaVacia)
		{
			colorCs = Tablero->TableroColor[destino];
			destino += See_Mov_Da[i];
		}
		if ((destino > -1 && destino < 64) && colorCs != Tablero->TableroColor[destino] && Tablero->Tablero[destino] == P)
		{
			if (Tablero->Tablero[Cs] > Inicio && Tablero->Tablero[Cs] < Fin)
			{
				SeeMov = MOVIMIENTO(destino, Cs, Tablero->Tablero[destino], (Tablero->Tablero[Cs] == CasillaVacia) ? MFLAGCAP : Tablero->Tablero[Cs], MFLAGPROM, 0);
		        if (!SeeHacerMovimiento(&SeeMov, SeeTablero, Tablero))
				{
					SeeDeshacerMovimiento(SeeTablero, Tablero);
					continue;
				}
				return SeeMov;
			}
		}
	}
	return NO_MOVIMIENTO;
}
int SeeRey(int Cs, _ST_SeeTablero *SeeTablero, int *Turno, _ST_TableroX64 *Tablero)
{
	int i = 0;
	int destino = 0;
	int SeeMov = NO_MOVIMIENTO;
	int P = *Turno ? ReyB : ReyN;
	int Inicio = 0;
	int Fin = 0;
	int See_Mov_Rey[8] = { -9, 9, -7, 7, 1, -1, 8, -8 };
	if (*Turno)
	{
		Inicio = CasillaVacia;
		Fin = ReyN;
	}
	else
	{
		Inicio = 0;
		Fin = ReyB;
	}
	for (i = 0; i < 4; i++)
	{
		destino = Cs + See_Mov_Rey[i];
		if (destino < 0 || destino > 63)
		{
			continue;
		}
		if ((destino > -1 && destino < 64) && Tablero->TableroColor[Cs] == Tablero->TableroColor[destino] && Tablero->Tablero[destino] == P)
		{
			if (Tablero->Tablero[Cs] > Inicio && Tablero->Tablero[Cs] < Fin)
			{
				SeeMov = MOVIMIENTO(destino, Cs, Tablero->Tablero[destino], (Tablero->Tablero[Cs] == CasillaVacia) ? MFLAGCAP : Tablero->Tablero[Cs], MFLAGPROM, 0);
		        if (!SeeHacerMovimiento(&SeeMov, SeeTablero, Tablero))
				{
					SeeDeshacerMovimiento(SeeTablero, Tablero);
					continue;
				}
				return SeeMov;
			}
		}
	}

	for (i = 4; i < 8; i++)
	{
		destino = Cs + See_Mov_Rey[i];
		if (destino < 0 || destino > 63)
		{
			continue;
		}
		if ((destino > -1 && destino < 64) && Tablero->TableroColor[Cs] != Tablero->TableroColor[destino] && Tablero->Tablero[destino] == P)
		{
			if (Tablero->Tablero[Cs] > Inicio && Tablero->Tablero[Cs] < Fin)
			{
				SeeMov = MOVIMIENTO(destino, Cs, Tablero->Tablero[destino], (Tablero->Tablero[Cs] == CasillaVacia) ? MFLAGCAP : Tablero->Tablero[Cs], MFLAGPROM, 0);
		        if (!SeeHacerMovimiento(&SeeMov, SeeTablero, Tablero))
				{
					SeeDeshacerMovimiento(SeeTablero, Tablero);
					continue;
				}
				return SeeMov;
			}
		}
	}
	return NO_MOVIMIENTO;
}
int SeeHacerMovimiento(int *M, _ST_SeeTablero *SeeTablero, _ST_TableroX64 *Tablero)
{
	int Inicio = CUADRADO_ORIGEN(*M);
	int Fin = CUADRADO_DESTINO(*M);

	SeeTablero->Estado[SeeTablero->SeePly].PosicionReyN = Tablero->PosicionReyN;
	SeeTablero->Estado[SeeTablero->SeePly].PosicionReyB = Tablero->PosicionReyB;
	SeeTablero->Estado[SeeTablero->SeePly].Movimiento = *M;

	switch (Tablero->Tablero[Inicio])
	{
		case ReyB:
			Tablero->PosicionReyB = Fin;
			break;

		case ReyN:
			Tablero->PosicionReyN = Fin;
			break;

		default:
		    break;
	}

	Tablero->Tablero[Fin] = Tablero->Tablero[Inicio];
	Tablero->Tablero[Inicio] = CasillaVacia;

	if (CORONACION(*M) != MFLAGPROM) Tablero->Tablero[Fin] = CORONACION(*M);

	SeeTablero->SeePly++;

	if (Jaque(Tablero, PIEZAMOVIDA(*M) < CasillaVacia ? true: false))
	{
		return false;
	}

	return true;
}
void SeeDeshacerMovimiento(_ST_SeeTablero *SeeTablero, _ST_TableroX64 *Tablero)
{
	int M = SeeTablero->Estado[SeeTablero->SeePly-1].Movimiento;
	int Inicio = CUADRADO_ORIGEN(M);
	int Fin = CUADRADO_DESTINO(M);

	SeeTablero->SeePly--;

	Tablero->Tablero[Inicio] = Tablero->Tablero[Fin];
	Tablero->Tablero[Fin] = CasillaVacia;

	Tablero->Tablero[Fin] = CAPTURADA(M);

	if (CORONACION(M) != MFLAGPROM)
		Tablero->Tablero[Inicio] = (PIEZAMOVIDA(M) < CasillaVacia) ? PeonB : PeonN;

	Tablero->PosicionReyB = SeeTablero->Estado[SeeTablero->SeePly].PosicionReyB;
	Tablero->PosicionReyN = SeeTablero->Estado[SeeTablero->SeePly].PosicionReyN;
}
