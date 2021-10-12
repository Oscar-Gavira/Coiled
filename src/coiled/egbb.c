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

#include "egbb.h"

#ifdef USAR_TABLAS_DE_FINALES

HMODULE EGBB_hmod = NULL;

enum EGBBColor 
{
	EGBB_WHITE, EGBB_BLACK
};
enum EGBBPiezas
{
	EGBB_EMPTY, EGBB_WKING, EGBB_WQUEEN, EGBB_WROOK, EGBB_WBISHOP, EGBB_WKNIGHT, EGBB_WPAWN,
	EGBB_BKING, EGBB_BQUEEN, EGBB_BROOK, EGBB_BBISHOP, EGBB_BKNIGHT, EGBB_BPAWN
};

int _NOTFOUND = 99999;

int EGBBflip[64] =
{
	56, 57, 58, 59, 60, 61, 62, 63,
	48, 49, 50, 51, 52, 53, 54, 55,
	40, 41, 42, 43, 44, 45, 46, 47,
	32, 33, 34, 35, 36, 37, 38, 39,
	24, 25, 26, 27, 28, 29, 30, 31,
	16, 17, 18, 19, 20, 21, 22, 23,
	 8,  9, 10, 11, 12, 13, 14, 15,
	 0,  1,  2,  3,  4,  5,  6,  7};

#ifdef _WIN32
#ifdef ARC_64BIT
	char EGBB_NOMBRE[] = { "egbb_x64.dll" };
#else
	char EGBB_NOMBRE[] = { "egbb_x86.dll" };
#endif
#else
#ifdef ARC_64BIT
	char EGBB_NOMBRE[] = { "egbb_x64.so" };
#else
	char EGBB_NOMBRE[] = { "egbb_x86.so" };
#endif
#endif
int Cargar_egbb_dll()
{
	if ((EGBB_hmod = LoadLibrary(EGBB_NOMBRE)) != 0)
	{
		EGBB_probe_egbb = (EGBB_PROBE_EGBB)GetProcAddress(EGBB_hmod, "probe_egbb_xmen");
		if (EGBB_probe_egbb == NULL)
		{
			return false;
		}
		EGBB_load_egbb = (EGBB_LOAD_EGBB)GetProcAddress(EGBB_hmod, "load_egbb_xmen");
		if (EGBB_load_egbb == NULL)
		{
			return false;
		}

		return true;
	}
	else
	{
		printf(""INFO_STRING""STRING_FORMAT" not found. Unable to use BitBases end table.\n", EGBB_NOMBRE);
		fflush(stdout);
		TablaDeFinales.Usar = 0;
		return false;
	}
}

void CargarEgbb()
{
	U64 CacheMB = TablaDeFinales.CacheMB * (U64)1024 * (U64)1024;

	if (TablaDeFinales.Dll_CargadaBb == true && TablaDeFinales.Directorio[0] != '\0')
	{
		if (TablaDeFinales.Piezas == 0)
		{
			TablaDeFinales.Piezas = EGBB_load_egbb(TablaDeFinales.Directorio, CacheMB, 2); /* 2 = SMART_LOAD */
		}
		else
		{
			if (TablaDeFinales.DirectorioNuevo == true)
			{
				TablaDeFinales.DirectorioNuevo = false;
				TablaDeFinales.Piezas = EGBB_load_egbb(TablaDeFinales.Directorio, CacheMB, 2); /* 2 = SMART_LOAD */
			}
		}
	}
}

int Descargar_egbb_dll()
{
	if (EGBB_hmod == NULL)
	{
		return true;
	}

	FreeLibrary(EGBB_hmod);

	EGBB_load_egbb = NULL;
	EGBB_probe_egbb = NULL;

	TablaDeFinales.Dll_CargadaBb = false;
	return true;
}

int Probar_egbb(int *puntos)
{
	int i = 0;
	unsigned int stm;		/* Quien mueve */
	int Npiezas = 2;		/* Los reyes */
	int VCuadros[64];
	int VPiezas[10];
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
			PeonC[indice_pb] = EGBBflip[i];
			indice_pb++;
			break;
		case CaballoB:
			CaballoC[indice_cb] = EGBBflip[i];
			indice_cb++;
			break;
		case AlfilB:
			AlfilC[indice_ab] = EGBBflip[i];
			indice_ab++;
			break;
		case TorreB:
			TorreC[indice_tb] = EGBBflip[i];
			indice_tb++;
			break;
		case DamaB:
			DamaC[indice_db] = EGBBflip[i];
			indice_db++;
			break;
		case ReyB:
			VCuadros[0] = EGBBflip[i];
			VPiezas[0] = EGBB_WKING;
			break;

		case PeonN:
			PeonC[indice_pn] = EGBBflip[i];
			indice_pn++;
			break;
		case CaballoN:
			CaballoC[indice_cn] = EGBBflip[i];
			indice_cn++;
			break;
		case AlfilN:
			AlfilC[indice_an] = EGBBflip[i];
			indice_an++;
			break;
		case TorreN:
			TorreC[indice_tn] = EGBBflip[i];
			indice_tn++;
			break;
		case DamaN:
			DamaC[indice_dn] = EGBBflip[i];
			indice_dn++;
			break;
		case ReyN:
			VCuadros[1] = EGBBflip[i];
			VPiezas[1] = EGBB_BKING;
			break;
		default:
			break;
		}
	}

	/*		DAMA		*/
	if (indice_db != 0 || indice_dn != 5)
	{
		for (i = 0; i < indice_db; ++i)
		{
			VCuadros[Npiezas] = DamaC[i];
			VPiezas[Npiezas] = EGBB_WQUEEN;
			Npiezas++;
		}
		for (i = 5; i < indice_dn; ++i)
		{
			VCuadros[Npiezas] = DamaC[i];
			VPiezas[Npiezas] = EGBB_BQUEEN;
			Npiezas++;
		}
	}
	/*		TORRE		*/
	if (indice_tb != 0 || indice_tn != 5)
	{
		for (i = 0; i < indice_tb; ++i)
		{
			VCuadros[Npiezas] = TorreC[i];
			VPiezas[Npiezas] = EGBB_WROOK;
			Npiezas++;
		}
		for (i = 5; i < indice_tn; ++i)
		{
			VCuadros[Npiezas] = TorreC[i];
			VPiezas[Npiezas] = EGBB_BROOK;
			Npiezas++;
		}
	}
	/*		ALFIL		*/
	if (indice_ab != 0 || indice_an != 5)
	{
		for (i = 0; i < indice_ab; ++i)
		{
			VCuadros[Npiezas] = AlfilC[i];
			VPiezas[Npiezas] = EGBB_WBISHOP;
			Npiezas++;
		}
		for (i = 5; i < indice_an; ++i)
		{
			VCuadros[Npiezas] = AlfilC[i];
			VPiezas[Npiezas] = EGBB_BBISHOP;
			Npiezas++;
		}
	}
	/*		CABALLO		*/
	if (indice_cb != 0 || indice_cn != 5)
	{
		for (i = 0; i < indice_cb; ++i)
		{
			VCuadros[Npiezas] = CaballoC[i];
			VPiezas[Npiezas] = EGBB_WKNIGHT;
			Npiezas++;
		}
		for (i = 5; i < indice_cn; ++i)
		{
			VCuadros[Npiezas] = CaballoC[i];
			VPiezas[Npiezas] = EGBB_BKNIGHT;
			Npiezas++;
		}
	}
	/*		PEON		*/
	if (indice_pb != 0 || indice_pn != 10)
	{
		for (i = 0; i < indice_pb; ++i)
		{
			VCuadros[Npiezas] = PeonC[i];
			VPiezas[Npiezas] = EGBB_WPAWN;
			Npiezas++;
		}
		for (i = 10; i < indice_pn; ++i)
		{
			VCuadros[Npiezas] = PeonC[i];
			VPiezas[Npiezas] = EGBB_BPAWN;
			Npiezas++;
		}
	}

	VCuadros[Npiezas] = EGBB_EMPTY;
	VPiezas[Npiezas] = EGBB_EMPTY;

	if (TableroGlobal.MueveBlancas == true)
		stm = EGBB_WHITE;
	else
		stm = EGBB_BLACK;

	/* Comprobamos en memoria/cache */
	*puntos = EGBB_probe_egbb(stm, VPiezas, VCuadros);

	if (*puntos != _NOTFOUND) {
		return 1;
	}

	return 0;
}

#endif
