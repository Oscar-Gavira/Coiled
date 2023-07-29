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

#include "nn.h"

#ifdef USAR_NNUE

int InfoNnueTechnology()
{
	switch (nnue_technology())
	{
	case 0:
		printf(""INFO_STRING"NnueTechnology activated: none\n");
		break;
	case 4:
		printf(""INFO_STRING"NnueTechnology activated: sse2\n");
		break;
	case 7:
		printf(""INFO_STRING"NnueTechnology activated: avx2\n");
		break;
	default:
		break;
	}
	fflush(stdout);
	return true;
}

int CargarNnue()
{
	if (ArchivoExiste(Nnue.ArchivoNnue) == true)
	{
		if (nnue_init(Nnue.ArchivoNnue) == true)
		{
			Nnue.DirectorioNuevo = false;
			printf(""INFO_STRING"Loaded NNUE: "STRING_FORMAT"\n", Nnue.ArchivoNnue);
			Nnue.Usar = true;
			fflush(stdout);
			InfoNnueTechnology();
			return true;
		}
		else
		{
			Nnue.Usar = false;
			printf(""INFO_STRING"NnueFile Unsupported.\n");
			fflush(stdout);
			return false;
		}
	}
	else
	{
		Nnue.Usar = false;
		printf(""INFO_STRING"NnueFile not found. "STRING_FORMAT"\n", Nnue.ArchivoNnue);
		fflush(stdout);
		return false;
	}
	return true;
}

int ProbarNNUE(_ST_TableroX64 *Tablero)
{
	int i = 0;
	int Npiezas = 2;
	int VCuadros[64];
	int VPiezas[64];
	int PeonC[20];
	int indice_pb = 0;
	int indice_pn = 10;
	int CaballoC[10];
	int indice_cb = 0;
	int indice_cn = 5;
	int AlfilC[10];
	int indice_ab = 0;
	int indice_an = 5;
	int TorreC[10];
	int indice_tb = 0;
	int indice_tn = 5;
	int DamaC[10];
	int indice_db = 0;
	int indice_dn = 5;

	memset(VCuadros, 0, 64 * sizeof(int));
	memset(PeonC, 0, 20 * sizeof(int));
	memset(CaballoC, 0, 10 * sizeof(int));
	memset(AlfilC, 0, 10 * sizeof(int));
	memset(TorreC, 0, 10 * sizeof(int));
	memset(DamaC, 0, 10 * sizeof(int));

	for (i = 0; i < 64; ++i)
	{
		if (Tablero->Tablero[i] == CasillaVacia)
		{
			continue;
		}

		switch (Tablero->Tablero[i])
		{
		case PeonB:
			PeonC[indice_pb] = (i) ^ 0x38;
			indice_pb++;
			break;
		case CaballoB:
			CaballoC[indice_cb] = (i) ^ 0x38;
			indice_cb++;
			break;
		case AlfilB:
			AlfilC[indice_ab] = (i) ^ 0x38;
			indice_ab++;
			break;
		case TorreB:
			TorreC[indice_tb] = (i) ^ 0x38;
			indice_tb++;
			break;
		case DamaB:
			DamaC[indice_db] = (i) ^ 0x38;
			indice_db++;
			break;
		case ReyB:
			VCuadros[0] = (i) ^ 0x38;
			VPiezas[0] = Nnue_wking;
			break;

		case PeonN:
			PeonC[indice_pn] = (i) ^ 0x38;
			indice_pn++;
			break;
		case CaballoN:
			CaballoC[indice_cn] = (i) ^ 0x38;
			indice_cn++;
			break;
		case AlfilN:
			AlfilC[indice_an] = (i) ^ 0x38;
			indice_an++;
			break;
		case TorreN:
			TorreC[indice_tn] = (i) ^ 0x38;
			indice_tn++;
			break;
		case DamaN:
			DamaC[indice_dn] = (i) ^ 0x38;
			indice_dn++;
			break;
		case ReyN:
			VCuadros[1] = (i) ^ 0x38;
			VPiezas[1] = Nnue_bking;
			break;
		default:
			break;
		}
	}

	if (indice_db != 0 || indice_dn != 5)
	{
		for (i = 0; i < indice_db; ++i)
		{
			VCuadros[Npiezas] = DamaC[i];
			VPiezas[Npiezas] = Nnue_wqueen;
			Npiezas++;
		}
		for (i = 5; i < indice_dn; ++i)
		{
			VCuadros[Npiezas] = DamaC[i];
			VPiezas[Npiezas] = Nnue_bqueen;
			Npiezas++;
		}
	}
	if (indice_tb != 0 || indice_tn != 5)
	{
		for (i = 0; i < indice_tb; ++i)
		{
			VCuadros[Npiezas] = TorreC[i];
			VPiezas[Npiezas] = Nnue_wrook;
			Npiezas++;
		}
		for (i = 5; i < indice_tn; ++i)
		{
			VCuadros[Npiezas] = TorreC[i];
			VPiezas[Npiezas] = Nnue_brook;
			Npiezas++;
		}
	}
	if (indice_ab != 0 || indice_an != 5)
	{
		for (i = 0; i < indice_ab; ++i)
		{
			VCuadros[Npiezas] = AlfilC[i];
			VPiezas[Npiezas] = Nnue_wbishop;
			Npiezas++;
		}
		for (i = 5; i < indice_an; ++i)
		{
			VCuadros[Npiezas] = AlfilC[i];
			VPiezas[Npiezas] = Nnue_bbishop;
			Npiezas++;
		}
	}
	if (indice_cb != 0 || indice_cn != 5)
	{
		for (i = 0; i < indice_cb; ++i)
		{
			VCuadros[Npiezas] = CaballoC[i];
			VPiezas[Npiezas] = Nnue_wknight;
			Npiezas++;
		}
		for (i = 5; i < indice_cn; ++i)
		{
			VCuadros[Npiezas] = CaballoC[i];
			VPiezas[Npiezas] = Nnue_bknight;
			Npiezas++;
		}
	}
	if (indice_pb != 0 || indice_pn != 10)
	{
		for (i = 0; i < indice_pb; ++i)
		{
			VCuadros[Npiezas] = PeonC[i];
			VPiezas[Npiezas] = Nnue_wpawn;
			Npiezas++;
		}
		for (i = 10; i < indice_pn; ++i)
		{
			VCuadros[Npiezas] = PeonC[i];
			VPiezas[Npiezas] = Nnue_bpawn;
			Npiezas++;
		}
	}

	VCuadros[Npiezas] = 0;
	VPiezas[Npiezas] = 0;
	return nnue_evaluate((Tablero->MueveBlancas == true) ? 0 : 1, VPiezas, VCuadros);
}

#endif
