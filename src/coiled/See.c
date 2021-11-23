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
#include "See.h"
#include "Utilidades.h"
#include "GeneradorDeMovimientos.h"

/* Almacena la posicion del rey y el movimiento realizado en el SEE */
typedef struct tag_SeeDeshacerMovimiento
{
	int Movimiento;
	int PosicionReyB;
	int PosicionReyN;
} _ST_SeeDeshacerMovimiento;

/* Estructura para deshacer los movimientos realizados durante SEE */
typedef struct tag_SeeTablero
{
	int Ply;
	_ST_SeeDeshacerMovimiento Estado[40];
} _ST_SeeTablero;

int See_Mov_Pe[2]  = {7, 9};
int See_Mov_Ca[8]  = {-17, -15, -10, -6, 6, 10, 15, 17};
int See_Mov_Al[4]  = {9, -9, -7, 7};
int See_Mov_To[4]  = {1, -1, -8, 8};
int See_Mov_Da[8]  = {-9, 9, -7, 7, 1, -1, 8, -8};

_ST_SeeTablero SeeMove;

/* Inicializamos See */
void Inicializar_See()
{
	int i = 0;

	for (i = 0; i < 40; i++)
	{
		SeeMove.Estado[i].Movimiento = NO_MOVIMIENTO;
		SeeMove.Estado[i].PosicionReyB = 0;
		SeeMove.Estado[i].PosicionReyN = 0;
	}

	SeeMove.Ply = 0;
}

int See(int *M, int turno)
{
	int i = 0;
	int Temp = 0;
	int Total[40];
	int Indice_Total = 0;
	int PiezaCapturada = CAPTURADA(*M);
	int Fin = CUADRADO_DESTINO(*M);

	SeeMove.Ply = 0;

	if (ENROQUE(*M) == MFLAGCA)
		return 1;
	if (CAPTURA_ALPASO(*M) == MFLAGEP)
		return 1;

	/* Ordenacion. No se ha realizado el movimiento. */
	if (TableroGlobal.Tablero[Fin] == PiezaCapturada)
	{
		if (!SeeHacerMovimiento(M))
		{
			SeeDeshacerMovimiento();
			return 1;
		}

		turno = !turno;
	}

	if (PiezaCapturada != MFLAGCAP)
	{
		for (i = 0; i < 40; i++)
		{
			/* Comprobamos ataques */
			Total[Indice_Total] = ObtenerAtaquesMenores(Fin, &turno);
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
	}

	while (SeeMove.Ply > 0)
	{
		SeeDeshacerMovimiento();
	}

	if (PiezaCapturada != MFLAGCAP)
	{
		if (Total[0] != NO_MOVIMIENTO)
		{
			Temp = ValorPieza(PiezaCapturada);
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

int ObtenerAtaquesMenores(int Cs, int *turno)
{
	int move = NO_MOVIMIENTO;

	if (*turno)
	{
		move = SeePeonesB(&Cs);
		if (move)
		{
			return move;
		}
		move = SeeCaballoB(&Cs);
		if (move)
		{
			return move;
		}
		move = SeeAlfilB(&Cs);
		if (move)
		{
			return move;
		}
		move = SeeTorreB(&Cs);
		if (move)
		{
			return move;
		}
		move = SeeDamaB(&Cs);
		if (move)
		{
			return move;
		}
		move = SeeReyB(&Cs);
		if (move)
		{
			return move;
		}
	}
	else
	{
		move = SeePeonesN(&Cs);
		if (move)
		{
			return move;
		}
		move = SeeCaballoN(&Cs);
		if (move)
		{
			return move;
		}
		move = SeeAlfilN(&Cs);
		if (move)
		{
			return move;
		}
		move = SeeTorreN(&Cs);
		if (move)
		{
			return move;
		}
		move = SeeDamaN(&Cs);
		if (move)
		{
			return move;
		}
		move = SeeReyN(&Cs);
		if (move)
		{
			return move;
		}
	}

	return move;
}

/* Buscamos defensa de peon. */
int SeePeonesN(int *Cs)
{
	int i;
	int destino;
	int SeeMov = NO_MOVIMIENTO;

	for (i = 0; i < 2; i++)
	{
		destino = *Cs - See_Mov_Pe[i];
		if (destino < 0 || destino > 63)
		{
			continue;
		}

		if (TableroGlobal.TableroColor[*Cs] == TableroGlobal.TableroColor[destino] && TableroGlobal.Tablero[destino] == PeonN)
		{
			if (TableroGlobal.Tablero[*Cs] < CasillaVacia)
			{
				SeeMov = MOVIMIENTO(destino, *Cs, TableroGlobal.Tablero[destino], (TableroGlobal.Tablero[*Cs] == CasillaVacia) ? MFLAGCAP : TableroGlobal.Tablero[*Cs], (*Cs > 55) ? DamaN : MFLAGPROM, 0);
				if (!SeeHacerMovimiento(&SeeMov))
				{
					SeeDeshacerMovimiento();
					continue;
				}
				return SeeMov;
			}
		}
	}
	return NO_MOVIMIENTO;
}
/* Buscamos defensa de peon. */
int SeePeonesB(int *Cs)
{
	int i;
	int destino;
	int SeeMov = NO_MOVIMIENTO;

	for (i = 0; i < 2; i++)
	{
		destino = *Cs + See_Mov_Pe[i];
		if (destino < 0 || destino > 63)
		{
			continue;
		}

		if (TableroGlobal.TableroColor[*Cs] == TableroGlobal.TableroColor[destino] && TableroGlobal.Tablero[destino] == PeonB)
		{
			if (TableroGlobal.Tablero[*Cs] > CasillaVacia)
			{
				SeeMov = MOVIMIENTO(destino, *Cs, TableroGlobal.Tablero[destino], (TableroGlobal.Tablero[*Cs] == CasillaVacia) ? MFLAGCAP : TableroGlobal.Tablero[*Cs], (*Cs < 8) ? DamaB : MFLAGPROM, 0);
		        if (!SeeHacerMovimiento(&SeeMov))
				{
					SeeDeshacerMovimiento();
					continue;
				}
				return SeeMov;
			}
		}
	}
	return NO_MOVIMIENTO;
}
/* Buscamos defensa de caballo. */
int SeeCaballoB(int *Cs)
{
	int i = 0;
	int destino = 0;
	int SeeMov = NO_MOVIMIENTO;

	for (i = 0; i < 8; i++)
	{
		destino = *Cs + See_Mov_Ca[i];
		if (destino < 0 || destino > 63)
		{
			continue;
		}

		if (TableroGlobal.TableroColor[*Cs] != TableroGlobal.TableroColor[destino] && TableroGlobal.Tablero[destino] == CaballoB)
		{
			if (TableroGlobal.Tablero[*Cs] > CasillaVacia)
			{
				SeeMov = MOVIMIENTO(destino, *Cs, TableroGlobal.Tablero[destino], (TableroGlobal.Tablero[*Cs] == CasillaVacia) ? MFLAGCAP : TableroGlobal.Tablero[*Cs], MFLAGPROM, 0);
		        if (!SeeHacerMovimiento(&SeeMov))
				{
					SeeDeshacerMovimiento();
					continue;
				}
				return SeeMov;
			}
		}
	}
	return NO_MOVIMIENTO;
}
/* Buscamos defensa de caballo. */
int SeeCaballoN(int *Cs)
{
	int i = 0;
	int destino = 0;
	int SeeMov = NO_MOVIMIENTO;

	for (i = 0; i < 8; i++)
	{
		destino = *Cs + See_Mov_Ca[i];
		if (destino < 0 || destino > 63)
		{
			continue;
		}

		if (TableroGlobal.TableroColor[*Cs] != TableroGlobal.TableroColor[destino] && TableroGlobal.Tablero[destino] == CaballoN)
		{
			if (TableroGlobal.Tablero[*Cs] < CasillaVacia)
			{
				SeeMov = MOVIMIENTO(destino, *Cs, TableroGlobal.Tablero[destino], (TableroGlobal.Tablero[*Cs] == CasillaVacia) ? MFLAGCAP : TableroGlobal.Tablero[*Cs], MFLAGPROM, 0);
		        if (!SeeHacerMovimiento(&SeeMov))
				{
					SeeDeshacerMovimiento();
					continue;
				}
				return SeeMov;
			}
		}
	}
	return NO_MOVIMIENTO;
}
/* Buscamos defensa de alfil. */
int SeeAlfilB(int *Cs)
{
	int i = 0;
	int destino = 0;
	int SeeMov = NO_MOVIMIENTO;

	for ( i = 0; i < 4; i++)
	{
		destino = *Cs + See_Mov_Al[i];
		if (destino < 0 || destino > 63)
		{
			continue;
		}

		while ((destino > -1 && destino < 64) && TableroGlobal.TableroColor[*Cs] == TableroGlobal.TableroColor[destino] && TableroGlobal.Tablero[destino] == CasillaVacia)
		{
			destino += See_Mov_Al[i];
		}
		if ((destino > -1 && destino < 64) && TableroGlobal.TableroColor[*Cs] == TableroGlobal.TableroColor[destino] && TableroGlobal.Tablero[destino] == AlfilB)
		{
			if (TableroGlobal.Tablero[*Cs] > CasillaVacia)
			{
				SeeMov = MOVIMIENTO(destino, *Cs, TableroGlobal.Tablero[destino], (TableroGlobal.Tablero[*Cs] == CasillaVacia) ? MFLAGCAP : TableroGlobal.Tablero[*Cs], MFLAGPROM, 0);
		        if (!SeeHacerMovimiento(&SeeMov))
				{
					SeeDeshacerMovimiento();
					continue;
				}
				return SeeMov;
			}
		}
	}
	return NO_MOVIMIENTO;
}
/* Buscamos defensa de alfil. */
int SeeAlfilN(int *Cs)
{
	int i = 0;
	int destino = 0;
	int SeeMov = NO_MOVIMIENTO;

	for ( i = 0; i < 4; i++)
	{
		destino = *Cs + See_Mov_Al[i];
		if (destino < 0 || destino > 63)
		{
			continue;
		}

		while ((destino > -1 && destino < 64) && TableroGlobal.TableroColor[*Cs] == TableroGlobal.TableroColor[destino] && TableroGlobal.Tablero[destino] == CasillaVacia)
		{
			destino += See_Mov_Al[i];
		}
		if ((destino > -1 && destino < 64) && TableroGlobal.TableroColor[*Cs] == TableroGlobal.TableroColor[destino] && TableroGlobal.Tablero[destino] == AlfilN)
		{
			if (TableroGlobal.Tablero[*Cs] < CasillaVacia)
			{
				SeeMov = MOVIMIENTO(destino, *Cs, TableroGlobal.Tablero[destino], (TableroGlobal.Tablero[*Cs] == CasillaVacia) ? MFLAGCAP : TableroGlobal.Tablero[*Cs], MFLAGPROM, 0);
		        if (!SeeHacerMovimiento(&SeeMov))
				{
					SeeDeshacerMovimiento();
					continue;
				}
				return SeeMov;
			}
		}
	}
	return NO_MOVIMIENTO;
}
/* Buscamos defensa de torre. */
int SeeTorreB(int *Cs)
{
	int i = 0;
	int destino = 0;
	int SeeMov = NO_MOVIMIENTO;
	int colorCs = false;

	for ( i = 0; i < 4; i++)
	{
		destino = *Cs + See_Mov_To[i];
		if (destino < 0 || destino > 63)
		{
			continue;
		}

		colorCs = TableroGlobal.TableroColor[*Cs];

		while ((destino > -1 && destino < 64) && colorCs != TableroGlobal.TableroColor[destino] && TableroGlobal.Tablero[destino] == CasillaVacia)
		{
			colorCs = TableroGlobal.TableroColor[destino];
			destino += See_Mov_To[i];
		}
		if ((destino > -1 && destino < 64) && colorCs != TableroGlobal.TableroColor[destino] && TableroGlobal.Tablero[destino] == TorreB)
		{
			if (TableroGlobal.Tablero[*Cs] > CasillaVacia)
			{
				SeeMov = MOVIMIENTO(destino, *Cs, TableroGlobal.Tablero[destino], (TableroGlobal.Tablero[*Cs] == CasillaVacia) ? MFLAGCAP : TableroGlobal.Tablero[*Cs], MFLAGPROM, 0);
		        if (!SeeHacerMovimiento(&SeeMov))
				{
					SeeDeshacerMovimiento();
					continue;
				}
				return SeeMov;
			}
		}
	}
	return NO_MOVIMIENTO;
}
/* Buscamos defensa de torre. */
int SeeTorreN(int *Cs)
{
	int i = 0;
	int destino = 0;
	int SeeMov = NO_MOVIMIENTO;
	int colorCs = false;

	for (i = 0; i < 4; i++)
	{
		destino = *Cs + See_Mov_To[i];
		if (destino < 0 || destino > 63)
		{
			continue;
		}

		colorCs = TableroGlobal.TableroColor[*Cs];

		while ((destino > -1 && destino < 64) && colorCs != TableroGlobal.TableroColor[destino] && TableroGlobal.Tablero[destino] == CasillaVacia)
		{
			colorCs = TableroGlobal.TableroColor[destino];
			destino += See_Mov_To[i];
		}
		if ((destino > -1 && destino < 64) && colorCs != TableroGlobal.TableroColor[destino] && TableroGlobal.Tablero[destino] == TorreN)
		{
			if (TableroGlobal.Tablero[*Cs] < CasillaVacia)
			{
				SeeMov = MOVIMIENTO(destino, *Cs, TableroGlobal.Tablero[destino], (TableroGlobal.Tablero[*Cs] == CasillaVacia) ? MFLAGCAP : TableroGlobal.Tablero[*Cs], MFLAGPROM, 0);
				if (!SeeHacerMovimiento(&SeeMov))
				{
					SeeDeshacerMovimiento();
					continue;
				}
				return SeeMov;
			}
		}
	}
	return NO_MOVIMIENTO;
}
/* Buscamos defensa de dama. */
int SeeDamaB(int *Cs)
{
	int i = 0;
	int destino = 0;
	int SeeMov = NO_MOVIMIENTO;
	int colorCs = false;

	/* Diagonales */
	for (i = 0; i < 4; i++)
	{
		destino = *Cs + See_Mov_Da[i];
		if (destino < 0 || destino > 63)
		{
			continue;
		}

		while ((destino > -1 && destino < 64) && TableroGlobal.TableroColor[*Cs] == TableroGlobal.TableroColor[destino] && TableroGlobal.Tablero[destino] == CasillaVacia)
		{
			destino += See_Mov_Da[i];
		}
		if ((destino > -1 && destino < 64) && TableroGlobal.TableroColor[*Cs] == TableroGlobal.TableroColor[destino] && TableroGlobal.Tablero[destino] == DamaB)
		{
			if (TableroGlobal.Tablero[*Cs] > CasillaVacia)
			{
				SeeMov = MOVIMIENTO(destino, *Cs, TableroGlobal.Tablero[destino], (TableroGlobal.Tablero[*Cs] == CasillaVacia) ? MFLAGCAP : TableroGlobal.Tablero[*Cs], MFLAGPROM, 0);
		        if (!SeeHacerMovimiento(&SeeMov))
				{
					SeeDeshacerMovimiento();
					continue;
				}
				return SeeMov;
			}
		}
	}

	for (i = 4; i < 8; i++)
	{
		destino = *Cs + See_Mov_Da[i];
		if (destino < 0 || destino > 63)
		{
			continue;
		}

		colorCs = TableroGlobal.TableroColor[*Cs];

		while ((destino > -1 && destino < 64) && colorCs != TableroGlobal.TableroColor[destino] && TableroGlobal.Tablero[destino] == CasillaVacia)
		{
			colorCs = TableroGlobal.TableroColor[destino];
			destino += See_Mov_Da[i];
		}
		if ((destino > -1 && destino < 64) && colorCs != TableroGlobal.TableroColor[destino] && TableroGlobal.Tablero[destino] == DamaB)
		{
			if (TableroGlobal.Tablero[*Cs] > CasillaVacia)
			{
				SeeMov = MOVIMIENTO(destino, *Cs, TableroGlobal.Tablero[destino], (TableroGlobal.Tablero[*Cs] == CasillaVacia) ? MFLAGCAP : TableroGlobal.Tablero[*Cs], MFLAGPROM, 0);
		        if (!SeeHacerMovimiento(&SeeMov))
				{
					SeeDeshacerMovimiento();
					continue;
				}
				return SeeMov;
			}
		}
	}
	return NO_MOVIMIENTO;
}
/* Buscamos defensa de dama. */
int SeeDamaN(int *Cs)
{
	int i = 0;
	int destino = 0;
	int SeeMov = NO_MOVIMIENTO;
	int colorCs = false;

	for ( i = 0; i < 4; i++)
	{
		destino = *Cs + See_Mov_Da[i];
		if (destino < 0 || destino > 63)
		{
			continue;
		}

		while ((destino > -1 && destino < 64) && TableroGlobal.TableroColor[*Cs] == TableroGlobal.TableroColor[destino] && TableroGlobal.Tablero[destino] == CasillaVacia)
		{
			destino += See_Mov_Da[i];
		}
		if ((destino > -1 && destino < 64) && TableroGlobal.TableroColor[*Cs] == TableroGlobal.TableroColor[destino] && TableroGlobal.Tablero[destino] == DamaN)
		{
			if (TableroGlobal.Tablero[*Cs] < CasillaVacia)
			{
				SeeMov = MOVIMIENTO(destino, *Cs, TableroGlobal.Tablero[destino], (TableroGlobal.Tablero[*Cs] == CasillaVacia) ? MFLAGCAP : TableroGlobal.Tablero[*Cs], MFLAGPROM, 0);
		        if (!SeeHacerMovimiento(&SeeMov))
				{
					SeeDeshacerMovimiento();
					continue;
				}
				return SeeMov;
			}
		}
	}

	for ( i = 4; i < 8; i++)
	{
		destino = *Cs + See_Mov_Da[i];
		if (destino < 0 || destino > 63)
		{
			continue;
		}

		colorCs = TableroGlobal.TableroColor[*Cs];

		while ((destino > -1 && destino < 64) && colorCs != TableroGlobal.TableroColor[destino] && TableroGlobal.Tablero[destino] == CasillaVacia)
		{
			colorCs = TableroGlobal.TableroColor[destino];
			destino += See_Mov_Da[i];
		}
		if ((destino > -1 && destino < 64) && colorCs != TableroGlobal.TableroColor[destino] && TableroGlobal.Tablero[destino] == DamaN)
		{
			if (TableroGlobal.Tablero[*Cs] < CasillaVacia)
			{
				SeeMov = MOVIMIENTO(destino, *Cs, TableroGlobal.Tablero[destino], (TableroGlobal.Tablero[*Cs] == CasillaVacia) ? MFLAGCAP : TableroGlobal.Tablero[*Cs], MFLAGPROM, 0);
		        if (!SeeHacerMovimiento(&SeeMov))
				{
					SeeDeshacerMovimiento();
					continue;
				}
				return SeeMov;
			}
		}
	}
	return NO_MOVIMIENTO;
}
/* Buscamos defensa de rey. */
int SeeReyB(int *Cs)
{
	int i = 0;
	int destino = 0;
	int SeeMov = NO_MOVIMIENTO;

	for (i = 0; i < 4; i++)
	{
		destino = *Cs + See_Mov_Da[i];
		if (destino < 0 || destino > 63)
		{
			continue;
		}
		if ((destino > -1 && destino < 64) && TableroGlobal.TableroColor[*Cs] == TableroGlobal.TableroColor[destino] && TableroGlobal.Tablero[destino] == ReyB)
		{
			if (TableroGlobal.Tablero[*Cs] > CasillaVacia)
			{
				SeeMov = MOVIMIENTO(destino, *Cs, TableroGlobal.Tablero[destino], (TableroGlobal.Tablero[*Cs] == CasillaVacia) ? MFLAGCAP : TableroGlobal.Tablero[*Cs], MFLAGPROM, 0);
		        if (!SeeHacerMovimiento(&SeeMov))
				{
					SeeDeshacerMovimiento();
					continue;
				}
				return SeeMov;
			}
		}
	}

	for (i = 4; i < 8; i++)
	{
		destino = *Cs + See_Mov_Da[i];
		if (destino < 0 || destino > 63)
		{
			continue;
		}
		if ((destino > -1 && destino < 64) && TableroGlobal.TableroColor[*Cs] != TableroGlobal.TableroColor[destino] && TableroGlobal.Tablero[destino] == ReyB)
		{
			if (TableroGlobal.Tablero[*Cs] > CasillaVacia)
			{
				SeeMov = MOVIMIENTO(destino, *Cs, TableroGlobal.Tablero[destino], (TableroGlobal.Tablero[*Cs] == CasillaVacia) ? MFLAGCAP : TableroGlobal.Tablero[*Cs], MFLAGPROM, 0);
		        if (!SeeHacerMovimiento(&SeeMov))
				{
					SeeDeshacerMovimiento();
					continue;
				}
				return SeeMov;
			}
		}
	}
	return NO_MOVIMIENTO;
}
/* Buscamos defensa de rey. */
int SeeReyN(int *Cs)
{
	int i = 0;
	int destino = 0;
	int SeeMov = NO_MOVIMIENTO;

	for ( i = 0; i < 4; i++)
	{
		destino = *Cs + See_Mov_Da[i];
		if (destino < 0 || destino > 63)
		{
			continue;
		}
		if ((destino > -1 && destino < 64) && TableroGlobal.TableroColor[*Cs] == TableroGlobal.TableroColor[destino] && TableroGlobal.Tablero[destino] == ReyN)
		{
			if (TableroGlobal.Tablero[*Cs] < CasillaVacia)
			{
				SeeMov = MOVIMIENTO(destino, *Cs, TableroGlobal.Tablero[destino], (TableroGlobal.Tablero[*Cs] == CasillaVacia) ? MFLAGCAP : TableroGlobal.Tablero[*Cs], MFLAGPROM, 0);
		        if (!SeeHacerMovimiento(&SeeMov))
				{
					SeeDeshacerMovimiento();
					continue;
				}
				return SeeMov;
			}
		}
	}

	for ( i = 4; i < 8; i++)
	{
		destino = *Cs + See_Mov_Da[i];
		if (destino < 0 || destino > 63)
		{
			continue;
		}

		if ((destino > -1 && destino < 64) && TableroGlobal.TableroColor[*Cs] != TableroGlobal.TableroColor[destino] && TableroGlobal.Tablero[destino] == ReyN)
		{
			if (TableroGlobal.Tablero[*Cs] < CasillaVacia)
			{
				SeeMov = MOVIMIENTO(destino, *Cs, TableroGlobal.Tablero[destino], (TableroGlobal.Tablero[*Cs] == CasillaVacia) ? MFLAGCAP : TableroGlobal.Tablero[*Cs], MFLAGPROM, 0);
		        if (!SeeHacerMovimiento(&SeeMov))
				{
					SeeDeshacerMovimiento();
					continue;
				}
				return SeeMov;
			}
		}
	}
	return NO_MOVIMIENTO;
}

/* Hacer movimientos */
int SeeHacerMovimiento(int *M)
{
	int Inicio = CUADRADO_ORIGEN(*M);
	int Fin = CUADRADO_DESTINO(*M);

	SeeMove.Estado[SeeMove.Ply].PosicionReyN = TableroGlobal.PosicionReyN;
	SeeMove.Estado[SeeMove.Ply].PosicionReyB = TableroGlobal.PosicionReyB;
	SeeMove.Estado[SeeMove.Ply].Movimiento = *M;

	switch (TableroGlobal.Tablero[Inicio])
	{
		case ReyB:
			TableroGlobal.PosicionReyB = Fin;
			break;

		case ReyN:
			TableroGlobal.PosicionReyN = Fin;
			break;

		default:
		    break;
	}

	/* Realizamos el movimiento */
	TableroGlobal.Tablero[Fin] = TableroGlobal.Tablero[Inicio];
	TableroGlobal.Tablero[Inicio] = CasillaVacia;

	if (CORONACION(*M) != MFLAGPROM)
	{
		TableroGlobal.Tablero[Fin] = CORONACION(*M);
	}

	SeeMove.Ply++;

	if (Jaque(PIEZAMOVIDA(*M) < CasillaVacia ? true: false))
	{
		return false;
	}

	return true;
}
/* Deshacemos el movimiento */
void SeeDeshacerMovimiento()
{
	int M = SeeMove.Estado[SeeMove.Ply-1].Movimiento;
	int Inicio = CUADRADO_ORIGEN(M);
	int Fin = CUADRADO_DESTINO(M);

	SeeMove.Ply--;

	/* Deshacemos el movimiento */
	TableroGlobal.Tablero[Inicio] = TableroGlobal.Tablero[Fin];
	TableroGlobal.Tablero[Fin] = CasillaVacia;

	TableroGlobal.Tablero[Fin] = CAPTURADA(M);

	if (CORONACION(M) != MFLAGPROM)
	{
		TableroGlobal.Tablero[Inicio] = (PIEZAMOVIDA(M) < CasillaVacia) ? PeonB : PeonN;
	}

	TableroGlobal.PosicionReyB = SeeMove.Estado[SeeMove.Ply].PosicionReyB;
	TableroGlobal.PosicionReyN = SeeMove.Estado[SeeMove.Ply].PosicionReyN;
}
