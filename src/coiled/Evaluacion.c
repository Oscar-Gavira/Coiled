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

#include "Evaluacion.h"

int Evaluar()
{
	int puntos = 0;

#ifdef USAR_NNUE
	if (Nnue.Dll_Cargada == true && Nnue.Usar == true)
	{
		puntos = ProbarNNUE();

		if (TableroGlobal.Regla_50_Movimiento > 70 && TableroGlobal.Regla_50_Movimiento < 100)
		{
			puntos = puntos * (101 - TableroGlobal.Regla_50_Movimiento) / 30;
		}

		return puntos;
	}
#endif

	return 0;
}

int EvaluarTablas()
{
	/* Rey contra rey */
	if (!(Blancas.CaballosTotales + Blancas.AlfilTotales + Blancas.TorresTotales + Blancas.DamasTotales + Blancas.PeonTotales) &&
		!(Negras.CaballosTotales + Negras.AlfilTotales + Negras.TorresTotales + Negras.DamasTotales + Negras.PeonTotales))
	{
		return true;
	}
	/* No hay peones */
	if (!Blancas.PeonTotales && !Negras.PeonTotales)
	{
		/* No hay figuras superiores */
		if (!(Blancas.TorresTotales + Blancas.DamasTotales) && !(Negras.TorresTotales + Negras.DamasTotales))
		{
			/* Ninguno de los dos bandos tiene pareja de alfiles o alfil y caballo */
			if ((Blancas.CaballosTotales + Blancas.AlfilTotales <= 1) && (Negras.CaballosTotales + Negras.AlfilTotales <= 1))
			{
				return true;
			}
		}
	}
	return false;
}

void LimpiarPuntuacion(int todo)
{
	if (todo)
	{
		Blancas.PeonTotales = 0;
		Blancas.CaballosTotales = 0;
		Blancas.AlfilTotales = 0;
		Blancas.TorresTotales = 0;
		Blancas.DamasTotales = 0;

		Negras.PeonTotales = 0;
		Negras.CaballosTotales = 0;
		Negras.AlfilTotales = 0;
		Negras.TorresTotales = 0;
		Negras.DamasTotales = 0;
	}
}
