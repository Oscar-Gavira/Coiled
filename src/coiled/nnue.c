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

#include "nnue.h"

#ifdef USAR_NNUE

HMODULE NNUE_hmod = NULL;

int Cargar_nnue_dll()
{
	char NNUE_NOMBRE[25];
	memset(NNUE_NOMBRE, 0, 25 * sizeof(char));
#ifdef _WIN32
	switch (Nnue.Tecnologia)
	{
	case 1:
		strcpy(NNUE_NOMBRE, "nnue_sse2.dll");
		break;
	case 2:
		strcpy(NNUE_NOMBRE, "nnue_sse3.dll");
		break;
	case 3:
		strcpy(NNUE_NOMBRE, "nnue_sse4.1.dll");
		break;
	case 4:
		strcpy(NNUE_NOMBRE, "nnue_avx2.dll");
		break;
	default:
		break;
	}
#else
	switch (Nnue.Tecnologia)
	{
	case 1:
		strcpy(NNUE_NOMBRE, "./libnnue_sse2.so");
		break;
	case 2:
		strcpy(NNUE_NOMBRE, "./libnnue_sse3.so");
		break;
	case 3:
		strcpy(NNUE_NOMBRE, "./libnnue_sse4.1.so");
		break;
	case 4:
		strcpy(NNUE_NOMBRE, "./libnnue_avx2.so");
		break;
	default:
		break;
	}
#endif

	if ((NNUE_hmod = LoadLibrary(NNUE_NOMBRE)) != 0)
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
		printf(""INFO_STRING""STRING_FORMAT" not found. Unable to use Nnue.\n", NNUE_NOMBRE);

		printf(""INFO_STRING"Your CPU does not support NnueTechnology ");
		switch (Nnue.Tecnologia)
		{
		case 1:
			printf("SSE2.");
			break;
		case 2:
			printf("SSE3.");
			break;
		case 3:
			printf("SSE4.1.");
			break;
		case 4:
			printf("AVX2.");
			break;
		default:
			break;
		}
		printf(" Try another NnueTechnology.\n");
		fflush(stdout);
		Nnue.Usar = false;
		return false;
	}
}

void CargarNnue()
{
	/* Se puede usar las tablas de finales de gaviota */
	if (Nnue.Dll_Cargada == true && Nnue.Directorio[0] != '\0')
	{
		/* Si es posible cargar la DLL */
		Nnue.DirectorioNuevo = false;
		if (NNUE_init(Nnue.Directorio) == false)
		{
			Nnue.Usar = false;
			printf(""INFO_STRING"NNUE file not found or Unsupported.\n");
			fflush(stdout);
		}
		else
		{
			printf(""INFO_STRING"Loading NNUE: "STRING_FORMAT"\n", Nnue.Directorio);
			fflush(stdout);
		}
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
