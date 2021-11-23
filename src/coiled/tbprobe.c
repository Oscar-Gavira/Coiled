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

#include "tbprobe.h"

#ifdef USAR_TABLAS_DE_FINALES

HMODULE TB_hmod = NULL;
#define TBNOCUADRO (64)

enum TBPiezas
{
   TBNOPIECE, TBPAWN, TBKNIGHT, TBBISHOP, TBROOK, TBQUEEN, TBKING
};

enum TBColor
{
   TBWHITE_TO_MOVE, TBBLACK_TO_MOVE
};

enum TBEnroque
{
   TBNOCASTLE = 0,
   TBWOO  = 8,
   TBWOOO = 4,
   TBBOO  = 2,
   TBBOOO = 1
};

#ifdef _WIN32
#ifdef ARC_64BIT
	char GTB_NOMBRE[] = { "gtb_x64.dll" };
#else
	char GTB_NOMBRE[] = { "gtb_x86.dll" };
#endif
#else
#ifdef ARC_64BIT
	char GTB_NOMBRE[] = { "./libgtb_x64.so" };
#else
	char GTB_NOMBRE[] = { "./libgtb_x86.so" };
#endif
#endif
int Cargar_gaviota_dll()
{

	if ((TB_hmod = LoadLibrary(GTB_NOMBRE)) != 0)
	{
		TBinit = (TBINIT)GetProcAddress(TB_hmod, "tb_init");
		if (TBinit == NULL)
		{
			return false;
		}
		TBrestart = (TBRESTART)GetProcAddress(TB_hmod, "tb_restart");
		if (TBrestart == NULL)
		{
			return false;
		}
		TBdone = (TBDONE)GetProcAddress(TB_hmod, "tb_done");
		if (TBdone == NULL)
		{
			return false;
		}
		TBprobe_hard = (TBPROBE_HARD)GetProcAddress(TB_hmod, "tb_probe_hard");
		if (TBprobe_hard == NULL)
		{
			return false;
		}
		TBprobe_soft = (TBPROBE_SOFT)GetProcAddress(TB_hmod, "tb_probe_soft");
		if (TBprobe_soft == NULL)
		{
			return false;
		}
		TBis_initialized = (TBIS_INITIALIZED)GetProcAddress(TB_hmod, "tb_is_initialized");
		if (TBis_initialized == NULL)
		{
			return false;
		}
		TBavailability = (TBAVAILABILITY)GetProcAddress(TB_hmod, "tb_availability");
		if (TBavailability == NULL)
		{
			return false;
		}
		TBcache_init = (TBCACHE_INIT)GetProcAddress(TB_hmod, "tbcache_init");
		if (TBcache_init == NULL)
		{
			return false;
		}
		TBcache_done = (TBCACHE_DONE)GetProcAddress(TB_hmod, "tbcache_done");
		if (TBcache_done == NULL)
		{
			return false;
		}
		TBcache_is_on = (TBCACHE_IS_ON)GetProcAddress(TB_hmod, "tbcache_is_on");
		if (TBcache_is_on == NULL)
		{
			return false;
		}
		TBpaths_init = (TBPATHS_INIT)GetProcAddress(TB_hmod, "tbpaths_init");
		if (TBpaths_init == NULL)
		{
			return false;
		}
		TBpaths_add = (TBPATHS_ADD)GetProcAddress(TB_hmod, "tbpaths_add");
		if (TBpaths_add == NULL)
		{
			return false;
		}
		TBpaths_done = (TBPATHS_DONE)GetProcAddress(TB_hmod, "tbpaths_done");
		if (TBpaths_done == NULL)
		{
			return false;
		}
		return true;
	}
	else
	{
		printf(""INFO_STRING""STRING_FORMAT" not found. Unable to use gaviota end table.\n", GTB_NOMBRE);
		fflush(stdout);
		TablaDeFinales.Usar = 0;
		return false;
	}
}

void CargarGaviotaTB()
{
	/* Se puede usar las tablas de finales de gaviota */
	if (TablaDeFinales.Dll_CargadaGv == true && TablaDeFinales.Directorio[0] != '\0')
	{
		char* initinfo = NULL;
		if (TBis_initialized() == false)
		{
			TablaDeFinales.paths = TBpaths_init();
			TablaDeFinales.paths = TBpaths_add(TablaDeFinales.paths, TablaDeFinales.Directorio);
			initinfo = TBinit(false, 4, TablaDeFinales.paths); /* 4 = CP4*/
		}
		else
		{
			if (TablaDeFinales.DirectorioNuevo == true)
			{
				TablaDeFinales.DirectorioNuevo = false;
				TablaDeFinales.paths = TBpaths_init();
				TablaDeFinales.paths = TBpaths_add(TablaDeFinales.paths, TablaDeFinales.Directorio);
				initinfo = TBrestart(false, 4, TablaDeFinales.paths);
			}
		}

		TablaDeFinales.Piezas = TBavailability();

		if (initinfo != NULL)
		{
			printf(""STRING_FORMAT"\n", initinfo);
			fflush(stdout);
		}
		if (TablaDeFinales.Piezas != 0 && TBis_initialized())
		{
			printf(""INFO_STRING"Gaviota initialized: Ok\n");
			fflush(stdout);
		}
		else
		{
			printf(""INFO_STRING"Gaviota initialized: Failed\n");
			fflush(stdout);
		}

		/* Cargamos la cache */
		CacheGaviotaTB();
	}
}

void CacheGaviotaTB()
{
	U64 CacheMB = TablaDeFinales.CacheMB * (U64)1024 * (U64)1024;
	if (TablaDeFinales.CacheNueva == true || TBcache_is_on() == false)
	{
		TablaDeFinales.CacheNueva = false;
		TBcache_init(CacheMB, 0);
		if (TBcache_is_on())
		{
			printf(""INFO_STRING"Gaviota state cache: Ok\n");
			fflush(stdout);
		}
		else
		{
			printf(""INFO_STRING"Gaviota state cache: Failed\n");
			fflush(stdout);
		}
	}
}

int Descargar_gaviota_dll()
{
	if (TB_hmod == NULL)
	{
		return true;
	}

	TBcache_done();
	TBdone();
	TBpaths_done(TablaDeFinales.paths);
	FreeLibrary(TB_hmod);

	TB_hmod = NULL;
	TBinit = NULL;
	TBrestart = NULL;
	TBdone = NULL;
	TBprobe_hard = NULL;
	TBprobe_soft = NULL;
	TBis_initialized = NULL;
	TBavailability = NULL;
	TBcache_init = NULL;
	TBcache_done = NULL;
	TBcache_is_on = NULL;
	TBpaths_init = NULL;
	TBpaths_add = NULL;
	TBpaths_done = NULL;

	TablaDeFinales.Dll_CargadaGv = false;
	return true;
}

unsigned Probar_gaviota(int *puntos)
{
	int i = 0;
	int enroque = TBNOCASTLE;
	int NpiezasB = 0;
	int NpiezasN = 0;

	unsigned int stm;												/* Quien mueve */
	unsigned int epsquare;											/* Posicion de peon al paso */
	unsigned int ws[17];											/* Lista de cuadrados de las blancas */
	unsigned int bs[17];											/* Lista de cuadrados de las negras */
	unsigned char wp[17];											/* Que piezas hay en el cuadrado. Blancas. */
	unsigned char bp[17];											/* Que piezas hay en el cuadrado. Negras. */
	unsigned pliestomate;											/* Distancia al mate */
	int TBavailable;												/* Existe la informacion en la tabla de finales */
	unsigned info = TBUNKNOWN;										/* informacion */

	if (TableroGlobal.EnroqueB == Corto)
	{
		enroque = TBWOO;
	}
	if (TableroGlobal.EnroqueB == Largo)
	{
		enroque = TBWOOO;
	}
	if (TableroGlobal.EnroqueB == LosDos)
	{
		enroque = TBWOO | TBWOOO;
	}
	if (TableroGlobal.EnroqueN == Corto)
	{
		enroque |= TBBOO;
	}
	if (TableroGlobal.EnroqueN == Largo)
	{
		enroque |= TBBOOO;
	}
	if (TableroGlobal.EnroqueN == LosDos)
	{
		enroque |= TBBOO | TBBOOO;
	}

	if (TableroGlobal.MueveBlancas == true)
		stm = TBWHITE_TO_MOVE;
	else
		stm = TBBLACK_TO_MOVE;

	if (TableroGlobal.FichaAlPasoPosicion == 0)
	{
		epsquare = TBNOCUADRO;
	}
	else
	{
		if (TableroGlobal.MueveBlancas == false)
		{
			epsquare = (TableroGlobal.FichaAlPasoPosicion + 8) ^ 0x38;
		}
		else
		{
			epsquare = (TableroGlobal.FichaAlPasoPosicion - 8) ^ 0x38;
		}
	}



	memset(ws, 0, 17 * sizeof(unsigned int));
	memset(wp, 0, 17 * sizeof(unsigned char));
	memset(bs, 0, 17 * sizeof(unsigned int));
	memset(bp, 0, 17 * sizeof(unsigned char));

	pliestomate = 0;

	for (i = 0; i < 64; i++)
	{
		switch (TableroGlobal.Tablero[i])
		{
		case PeonB:
			ws[NpiezasB] = (i) ^ 0x38;
			wp[NpiezasB] = TBPAWN;
			NpiezasB++;
			break;
		case CaballoB:
			ws[NpiezasB] = (i) ^ 0x38;
			wp[NpiezasB] = TBKNIGHT;
			NpiezasB++;
			break;
		case AlfilB:
			ws[NpiezasB] = (i) ^ 0x38;
			wp[NpiezasB] = TBBISHOP;
			NpiezasB++;
			break;
		case TorreB:
			ws[NpiezasB] = (i) ^ 0x38;
			wp[NpiezasB] = TBROOK;
			NpiezasB++;
			break;
		case DamaB:
			ws[NpiezasB] = (i) ^ 0x38;
			wp[NpiezasB] = TBQUEEN;
			NpiezasB++;
			break;
		case ReyB:
			ws[NpiezasB] = (i) ^ 0x38;
			wp[NpiezasB] = TBKING;
			NpiezasB++;
			break;

		case PeonN:
			bs[NpiezasN] = (i) ^ 0x38;
			bp[NpiezasN] = TBPAWN;
			NpiezasN++;
			break;
		case CaballoN:
			bs[NpiezasN] = (i) ^ 0x38;
			bp[NpiezasN] = TBKNIGHT;
			NpiezasN++;
			break;
		case AlfilN:
			bs[NpiezasN] = (i) ^ 0x38;
			bp[NpiezasN] = TBBISHOP;
			NpiezasN++;
			break;
		case TorreN:
			bs[NpiezasN] = (i) ^ 0x38;
			bp[NpiezasN] = TBROOK;
			NpiezasN++;
			break;
		case DamaN:
			bs[NpiezasN] = (i) ^ 0x38;
			bp[NpiezasN] = TBQUEEN;
			NpiezasN++;
			break;
		case ReyN:
			bs[NpiezasN] = (i) ^ 0x38;
			bp[NpiezasN] = TBKING;
			NpiezasN++;
			break;
		default:
			continue;
			break;
		}
	}

	ws[NpiezasB] = TBNOCUADRO;
	wp[NpiezasB] = TBNOPIECE;

	bs[NpiezasN] = TBNOCUADRO;
	bp[NpiezasN] = TBNOPIECE;

	/* Comprobamos en memoria/cache */
	TBavailable = TBprobe_soft(stm, epsquare, enroque, ws, bs, wp, bp, &info, &pliestomate);
	/* Comprobamos en el HDD */
	if (TBavailable == false)
		TBavailable = TBprobe_hard(stm, epsquare, enroque, ws, bs, wp, bp, &info, &pliestomate);

	if (TBavailable == true)
	{
		if (info == TBDRAW)
		{
			*puntos = VALOR_EMPATE;
			return true;
		}
		else if (info == TBWMATE && stm == TBWHITE_TO_MOVE)
		{
			*puntos = 1;
			return true;
		}
		else if (info == TBBMATE && stm == TBBLACK_TO_MOVE)
		{
			*puntos = 1;
			return true;
		}
		else if (info == TBWMATE && stm == TBBLACK_TO_MOVE)
		{
			*puntos = -1;
			return true;
		}
		else if (info == TBBMATE && stm == TBWHITE_TO_MOVE)
		{
			*puntos = -1;
			return true;
		}
		return TBUNKNOWN;
	}
	else
	{
		return TBUNKNOWN;
	}
}

#endif
