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

#include "nnue.h"

#ifdef USAR_NNUE

HMODULE NNUE_hmod = NULL;

char NNUE_NAME[] = { "nnue_avx2.dll" };


int NNUE_FLIP[64] =
	{
		56, 57, 58, 59, 60, 61, 62, 63,
		48, 49, 50, 51, 52, 53, 54, 55,
		40, 41, 42, 43, 44, 45, 46, 47,
		32, 33, 34, 35, 36, 37, 38, 39,
		24, 25, 26, 27, 28, 29, 30, 31,
		16, 17, 18, 19, 20, 21, 22, 23,
		 8,  9, 10, 11, 12, 13, 14, 15,
		 0,  1,  2,  3,  4,  5,  6,  7 };

int Cargar_nnue_dll()
{
	if ((NNUE_hmod = LoadLibrary(NNUE_NAME)) != 0)
	{
		NNUE_init = (NNUE_INIT)GetProcAddress(NNUE_hmod, "nnue_init"); 
		if (NNUE_init == NULL)
		{
			return false;
		}
		NNUE_evaluate = (NNUE_EVALUATE)GetProcAddress(NNUE_hmod, "nnue_evaluate");
		if (NNUE_evaluate == NULL)
		{
			return false;
		}

		return true;
	}
	else
	{
		Nnue.Usar = false;
		return false;
	}
}

int Descargar_nnue_dll()
{
	if (NNUE_hmod == NULL)
	{
		return true;
	}

	FreeLibrary(NNUE_hmod);

	NNUE_hmod = NULL;
	NNUE_init = NULL;
	NNUE_evaluate = NULL;

	Nnue.Dll_Cargada = false;
	return true;
}

int ProbarNNUE()
{
	int i = 0;
	int Npiezas = 2;		/* Los reyes */
	int VCuadros[64];
	int VPiezas[64];
	int PeonC[20];			/* 0 al 9 blancas - 10 al 19 negras */
	int indice_pb = 0;
	int indice_pn = 10;
	int CaballoC[10];		/* 0 al 4 blancas - 5 al 9 negras */
	int indice_cb = 0;
	int indice_cn = 5;
	int AlfilC[10];			/* 0 al 4 blancas - 5 al 9 negras */
	int indice_ab = 0;
	int indice_an = 5;
	int TorreC[10];			/* 0 al 4 blancas - 5 al 9 negras */
	int indice_tb = 0;
	int indice_tn = 5;
	int DamaC[10];			/* 0 al 4 blancas - 5 al 9 negras */
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
		if (TableroGlobal.Tablero[i] == CasillaVacia)
		{
			continue;
		}

		switch (TableroGlobal.Tablero[i])
		{
		case PeonB:
			PeonC[indice_pb] = NNUE_FLIP[i];
			indice_pb++;
			break;
		case CaballoB:
			CaballoC[indice_cb] = NNUE_FLIP[i];
			indice_cb++;
			break;
		case AlfilB:
			AlfilC[indice_ab] = NNUE_FLIP[i];
			indice_ab++;
			break;
		case TorreB:
			TorreC[indice_tb] = NNUE_FLIP[i];
			indice_tb++;
			break;
		case DamaB:
			DamaC[indice_db] = NNUE_FLIP[i];
			indice_db++;
			break;
		case ReyB:
			VCuadros[0] = NNUE_FLIP[i];
			VPiezas[0] = Nnue_wking;
			break;

		case PeonN:
			PeonC[indice_pn] = NNUE_FLIP[i];
			indice_pn++;
			break;
		case CaballoN:
			CaballoC[indice_cn] = NNUE_FLIP[i];
			indice_cn++;
			break;
		case AlfilN:
			AlfilC[indice_an] = NNUE_FLIP[i];
			indice_an++;
			break;
		case TorreN:
			TorreC[indice_tn] = NNUE_FLIP[i];
			indice_tn++;
			break;
		case DamaN:
			DamaC[indice_dn] = NNUE_FLIP[i];
			indice_dn++;
			break;
		case ReyN:
			VCuadros[1] = NNUE_FLIP[i];
			VPiezas[1] = Nnue_bking;
			break;
		default:
			break;
		}
	}

	//Nnue_wking = 1, Nnue_wqueen = 2, Nnue_wrook = 3, Nnue_wbishop = 4, Nnue_wknight = 5, Nnue_wpawn = 6,
	//Nnue_bking = 7, Nnue_bqueen = 8, Nnue_brook = 9, Nnue_bbishop = 10, Nnue_bknight = 11, Nnue_bpawn = 12,

	/*		DAMA		*/
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
	/*		TORRE		*/
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
	/*		ALFIL		*/
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
	/*		CABALLO		*/
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
	/*		PEON		*/
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

	//white = 0 black = 1
	return NNUE_evaluate((TableroGlobal.MueveBlancas == true) ? 0 : 1, VPiezas, VCuadros);
}

#endif
