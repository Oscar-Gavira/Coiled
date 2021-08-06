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

#include "Perft.h"
#include "Utilidades.h"
#include "GeneradorDeMovimientos.h"

void Perft(int div)
{
	int i = 0;
	int NumeroJugadas = 0;
	_ST_Movimiento nextMoves[MAX_JUGADAS];
	_ST_Perft Perft_Divide_Total;
	_ST_Divide	Perft_Divide_Jugada;

	Perft_Divide_Total.A_Nodos = 0;
	Perft_Divide_Total.A_Capturas = 0;
	Perft_Divide_Total.A_Ep = 0;
	Perft_Divide_Total.A_Enroques = 0;
	Perft_Divide_Total.A_Coronacion = 0;
	Perft_Divide_Total.A_Jaque = 0;
	Perft_Divide_Total.A_JaqueMate = 0;

	memset(Perft_Divide_Jugada.D_Jugada, 0, 6 * sizeof(char));

	TipoJuego.Inicio = ObtenerTiempo();
	NumeroJugadas = GenerarMovimientos(Todos, nextMoves);

	if (div == true) printf("Divide\n");
	if (div == true) printf("========================================\n");
	if (div == true) fflush(stdout);
	for (i = 0; i < NumeroJugadas; i++)
	{
		/* Hacemos el movimiento y compruebo su legalidad */
		if (!HacerMovimiento(nextMoves[i].Movimiento, true))
		{
			/* Si no es legal retrocedemos la posición y tomamos otro */
			DeshacerMovimiento();
			continue;
		}

		Perft_Divide_Jugada.D_Nodos = 0;
		MiniMax(1, &Perft_Divide_Total, &nextMoves[i].Movimiento, &Perft_Divide_Jugada.D_Nodos);
		MovimientoCoordenadas(CUADRADO_ORIGEN(nextMoves[i].Movimiento), CUADRADO_DESTINO(nextMoves[i].Movimiento), CORONACION(nextMoves[i].Movimiento), Perft_Divide_Jugada.D_Jugada);


		if (div == true) printf(""STRING_FORMAT": ", Perft_Divide_Jugada.D_Jugada);
		if (div == true) printf(""U64_FORMAT"\n", Perft_Divide_Jugada.D_Nodos);
		DeshacerMovimiento();
	}

	if (div == true) printf("\n");
	if (div == true) printf("========================================\n");
	TipoJuego.TiempoTranscurrido = (ObtenerTiempo() - TipoJuego.Inicio) + 1;

	if (div == false)
	{
		printf(""U64_FORMAT"\n", Perft_Divide_Total.A_Nodos);
	}
	else
	{
		printf("Nodes: "U64_FORMAT"\n", Perft_Divide_Total.A_Nodos);
	}
	if (div == true)
	{
		if (Perft_Divide_Total.A_Nodos != 0 && TipoJuego.TiempoTranscurrido != 0)
		{
			printf("Nps: "U64_FORMAT"\n", Perft_Divide_Total.A_Nodos / TipoJuego.TiempoTranscurrido * 1000);
		}
		else
		{
			printf("Nps: 0");
		}
		printf("Time(ms): "U64_FORMAT"\n", (U64)(ObtenerTiempo() - TipoJuego.Inicio));
		printf("\n");
		printf("========================================\n");
		printf("Captures: "U64_FORMAT"\n", Perft_Divide_Total.A_Capturas);
	    printf("Captures Ep: "U64_FORMAT"\n", Perft_Divide_Total.A_Ep);
	    printf("Promotions: "U64_FORMAT"\n", Perft_Divide_Total.A_Coronacion);
	    printf("Castles: "U64_FORMAT"\n", Perft_Divide_Total.A_Enroques);
	}
	fflush(stdout);
}
int MiniMax(int depth, _ST_Perft *P_Divide_Total, int *P_Mov, U64 *Jugada)
{
	_ST_Movimiento nextMoves[MAX_JUGADAS];
	int NumeroJugadas = 0;
	int i = 0;

	if (depth == TipoJuego.MaxDepth)
	{
		if (CAPTURADA(*P_Mov) != MFLAGCAP)
		{
			P_Divide_Total->A_Capturas++;
		}
		if (CAPTURA_ALPASO(*P_Mov) == MFLAGEP)
		{
			P_Divide_Total->A_Ep++;
			P_Divide_Total->A_Capturas++;
		}
		if (ENROQUE(*P_Mov) == MFLAGCA)
		{
			P_Divide_Total->A_Enroques++;
		}
		if (CORONACION(*P_Mov) != MFLAGPROM)
		{
			P_Divide_Total->A_Coronacion++;
		}

		P_Divide_Total->A_Nodos++;
		*Jugada += 1;

		return 0;
	}

	NumeroJugadas = GenerarMovimientos(Todos, nextMoves);

	for (i = 0; i < NumeroJugadas; i++)
	{
		if (!HacerMovimiento(nextMoves[i].Movimiento, true))
        {
            DeshacerMovimiento();
            continue;
        }

		MiniMax(depth + 1, P_Divide_Total, &nextMoves[i].Movimiento, Jugada);

		DeshacerMovimiento();
	}


	return 0;
}

