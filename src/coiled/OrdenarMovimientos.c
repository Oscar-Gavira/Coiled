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

#include "OrdenarMovimientos.h"

void OrdenarMovimientosPorValor(int *hMov, _ST_Movimiento *M, _ST_TableroX64 *Tablero)
{
	int i = 0;
	int PiezaCapturada = 0;
	int PiezaCoronacion = 0;
	int _PiezaMovida = 0;
	int Refu = HistoricoMovimientoRefutacion(Tablero);
	int v = 0;
	M->HayHash = false;

	for (i = 0; i < M->CantidadDeMovimiento; ++i)
	{

		M->Ordenar[i] = 0;

		if (M->Movimiento[i] == *hMov)
		{
			M->HayHash = true;
			M->Ordenar[i] = ORDENAR_HASH;
			continue;
		}

		if (MovimientoTranquilo(&M->Movimiento[i]))
		{
			if (HistoricoEsMovimientoKillerMate(&M->Movimiento[i], Tablero))
			{
				M->Ordenar[i] = ORDENAR_KILL_MATE + HistoricoValor(&M->Movimiento[i], Tablero);
				continue;
			}
			if (HistoricoEsMovimientoKiller(&M->Movimiento[i], Tablero))
			{
				M->Ordenar[i] = ORDENAR_KILL + HistoricoValor(&M->Movimiento[i], Tablero);
				continue;
			}
			if (Refu == M->Movimiento[i])
			{
				M->Ordenar[i] = ORDENAR_REFUTACION;
				continue;
			}

			M->Ordenar[i] = HistoricoValor(&M->Movimiento[i], Tablero);
			if (M->Ordenar[i] < 0)
				M->Ordenar[i] += -100;
			if (M->Ordenar[i] == 0) /* Sin historico, ordenamos por figura */
				M->Ordenar[i] = -PIEZAMOVIDA(M->Movimiento[i]);

		}
		else
		{
			PiezaCapturada = CAPTURADA(M->Movimiento[i]);
			_PiezaMovida = PIEZAMOVIDA(M->Movimiento[i]);
			PiezaCoronacion = CORONACION(M->Movimiento[i]);
			
			if (_PiezaMovida > CasillaVacia)
				_PiezaMovida = _PiezaMovida - 7;

			if (PiezaCapturada != MFLAGCAP && PiezaCapturada > CasillaVacia)
				PiezaCapturada = PiezaCapturada - 7;

			if (PiezaCoronacion != MFLAGPROM && PiezaCoronacion > CasillaVacia)
				PiezaCoronacion = PiezaCoronacion - 7;

			if (CAPTURA_ALPASO(M->Movimiento[i]) == MFLAGEP)
			{
				M->Ordenar[i] = ORDENAR_CAPTURAS;
                continue;
			}
			v = HistoricoCapturaValor(&M->Movimiento[i], Tablero);
			if (PiezaCoronacion != MFLAGPROM)
			{
				if (PiezaCoronacion == DamaB)
					M->Ordenar[i] = ORDENAR_CORONACION_DAMA;
				else
				{
					if (v > 0)
						M->Ordenar[i] = 1000 + ORDENAR_CORONACION + (64 * PiezaCoronacion) + v;
					else if (v < 0)
						M->Ordenar[i] = 500 + ORDENAR_CORONACION + (64 * PiezaCoronacion) + v;
					else
						M->Ordenar[i] = ORDENAR_CORONACION + PiezaCoronacion;

				}

				continue;
			}
			if (PiezaCapturada != MFLAGCAP)
			{
				if (v > 0)
					M->Ordenar[i] = 1000 + ORDENAR_CAPTURAS + (64 * PiezaCapturada - _PiezaMovida) + v;
				else if (v < 0)
					M->Ordenar[i] = 500 + ORDENAR_CAPTURAS + (64 * PiezaCapturada - _PiezaMovida) + v;
				else
					M->Ordenar[i] = ORDENAR_CAPTURAS + (64 * PiezaCapturada - _PiezaMovida);

				continue;
			}
		}
	}
}
void ObtenerMovimiento(int *indice, _ST_Movimiento *M, _ST_TableroX64 *Tablero)
{
	int i, j, Max;
	int Tmp1 = 0;
	int Tmp2 = 0;
	int pSee = 0;

	Max = M->Ordenar[*indice];
	j = *indice;
	for (i = *indice + 1; i < M->CantidadDeMovimiento; i++)
	{
		if (M->Ordenar[i] > Max)
		{
			Max = M->Ordenar[i];
			j = i;
		}
	}

	if (j != *indice)
	{
		Tmp1 = M->Movimiento[*indice];
		Tmp2 = M->Ordenar[*indice];

		M->Movimiento[*indice] = M->Movimiento[j];
		M->Ordenar[*indice] = M->Ordenar[j];

		M->Movimiento[j] = Tmp1;
		M->Ordenar[j] = Tmp2;
	}

	/* Mala captura */
	if (M->Ordenar[*indice] > ORDENAR_CAPTURAS && M->Ordenar[*indice] < ORDENAR_HASH)
	{
		pSee = See(&M->Movimiento[*indice], Tablero->MueveBlancas, Tablero);
		if (pSee < 0)
		{
			if (M->HayHash == false)
			{
				if (CORONACION(M->Movimiento[*indice]) != MFLAGPROM)
				{
					if (ValorPieza(CORONACION(M->Movimiento[*indice])) == SeeDamaValor)
						M->Ordenar[*indice] = 20000 + pSee;
					else
						M->Ordenar[*indice] = 15000 + pSee;
				}
				else if (CAPTURADA(M->Movimiento[*indice]) != MFLAGCAP)
				{
					M->Ordenar[*indice] = 10000 + pSee;
				}
			}
			else
			{
				M->Ordenar[*indice] = -ORDENAR_CAPTURAS + pSee;
			}
			if (*indice + 1 < M->CantidadDeMovimiento)
				ObtenerMovimiento(indice, M, Tablero);
		}
	}
}
