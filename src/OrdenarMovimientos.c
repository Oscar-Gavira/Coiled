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

#include "OrdenarMovimientos.h"

/* Ordena los movimientos */
void OrdenarMovimientosTodos(int *Capacidad, int *hMov, _ST_Movimiento *M)
{
	int i = 0;
	int PiezaCapturada = 0;
	int PiezaCoronacion = 0;
	int _PiezaMovida = 0;

	for (i = 0; i < *Capacidad; ++i)
	{

		M[i].Ordenar = 0;

#ifdef USAR_HASH_TB
		if (M[i].Movimiento == *hMov)
		{
			M[i].Ordenar = ORDENAR_HASH;
			continue;
		}
#endif

		if (MovimientoInquieto(&M[i].Movimiento))
		{
			if (HistoricoEsMovimientoKillerMate(&TableroGlobal.Ply, &M[i].Movimiento))
			{
				M[i].Ordenar = ORDENAR_KILL_MATE + HistoricoValor(&M[i].Movimiento);
				continue;
			}
			if (HistoricoEsMovimientoKiller(&TableroGlobal.Ply, &M[i].Movimiento))
			{
				M[i].Ordenar = ORDENAR_KILL + HistoricoValor(&M[i].Movimiento);
				continue;
			}
			M[i].Ordenar = HistoricoValor(&M[i].Movimiento);
		}
		else
		{
			PiezaCapturada = CAPTURADA(M[i].Movimiento);
			_PiezaMovida = PIEZAMOVIDA(M[i].Movimiento);
			PiezaCoronacion = CORONACION(M[i].Movimiento);
			
			if (_PiezaMovida > CasillaVacia)
				_PiezaMovida = _PiezaMovida - 7;

			if (PiezaCapturada > CasillaVacia)
				PiezaCapturada = PiezaCapturada - 7;

			if (PiezaCoronacion != MFLAGPROM && PiezaCoronacion > CasillaVacia)
				PiezaCoronacion = PiezaCoronacion - 7;

			if (CAPTURA_ALPASO(M[i].Movimiento) == MFLAGEP)
			{
				M[i].Ordenar += ORDENAR_CAPTURAS + (64 * _PiezaMovida - _PiezaMovida);
                continue;
			}
			if (PiezaCoronacion != MFLAGPROM && PiezaCapturada != MFLAGCAP)
			{
                M[i].Ordenar += ORDENAR_CORONACION + (64 * PiezaCapturada - _PiezaMovida) + PiezaCoronacion;
                continue;
			}
			if (PiezaCoronacion != MFLAGPROM)
			{
				M[i].Ordenar += ORDENAR_CORONACION_CAPTURA + PiezaCoronacion;
                continue;
			}
			if (PiezaCapturada != MFLAGCAP)
			{
				M[i].Ordenar += ORDENAR_CAPTURAS + (64 * PiezaCapturada - _PiezaMovida);
				continue;
			}
		}
	}
	M[*Capacidad].Ordenar = -ORDENAR_HASH;

}

/* Ordenamos los movimientos. */
void OrdenaMovimiento(int *indice, int *Capacidad, _ST_Movimiento *M)
{
	int i, j, Max;
	_ST_Movimiento Tmp;

	Max = M[*indice].Ordenar;
	j = *indice;
	for (i = *indice + 1; i < *Capacidad; i++)
	{
		if (M[i].Ordenar > Max)
		{
			Max = M[i].Ordenar;
			j = i;
		}
	}

	if (j != *indice)
	{
		Tmp.Movimiento = M[*indice].Movimiento;
		Tmp.Ordenar = M[*indice].Ordenar;

		M[*indice].Movimiento = M[j].Movimiento;
		M[*indice].Ordenar = M[j].Ordenar;

		M[j].Movimiento = Tmp.Movimiento;
		M[j].Ordenar = Tmp.Ordenar;
	}
}
