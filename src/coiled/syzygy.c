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

#include "syzygy.h"

#if defined(USAR_TABLAS_DE_FINALES) && defined(ARC_64BIT)

HMODULE SG_hmod = NULL;

#ifdef _WIN32
	char SG_NOMBRE[] = { "syzygy_x64.dll" };
#else
	char SG_NOMBRE[] = { "./libsyzygy_x64.so" };
#endif

U64 SG_Mascara[64];

int Cargar_Syzygy_dll()
{
	if ((SG_hmod = LoadLibrary(SG_NOMBRE)) != 0)
	{
		SG_inits = (SG_INITS)GetProcAddress(SG_hmod, "tb_init");
		if (SG_inits == NULL)
		{
			return false;
		}
		SG_free = (SG_FREE)GetProcAddress(SG_hmod, "tb_free");
		if (SG_free == NULL)
		{
			return false;
		}
		SG_probe_wdl = (SG_PROBE_WDL)GetProcAddress(SG_hmod, "tb_probe_wdl_impl");
		if (SG_probe_wdl == NULL)
		{
			return false;
		}
		SG_man = (SG_LARGEST)GetProcAddress(SG_hmod, "TB_LARGEST");
		if (SG_man == 0)
		{
			return false;
		}
		return true;
	}
	else
	{
		printf(""INFO_STRING""STRING_FORMAT" not found. Unable to use syzygy end table.\n", SG_NOMBRE);
		fflush(stdout);
		TablaDeFinales.Usar = 0;
		return false;
	}
}

void CargarSyzygy()
{
	/* Se puede usar las tablas de finales de gaviota */
	if (TablaDeFinales.Dll_CargadaSg == true && TablaDeFinales.Directorio[0] != '\0')
	{
		/* Si es posible cargar la DLL */
		TablaDeFinales.DirectorioNuevo = false;
		SG_inits(TablaDeFinales.Directorio);
		if (SG_man == 0)
			printf(""INFO_STRING"Syzygy initialized: FAIL.\n");
		else
			printf(""INFO_STRING"Syzygy initialized: OK\n");
	}
}

int Descargar_Syzygy_dll()
{
	if (SG_hmod == NULL)
	{
		return true;
	}

	SG_free();
	FreeLibrary(SG_hmod);

	SG_hmod = NULL;
	SG_inits = NULL;
	SG_free = NULL;
	SG_probe_wdl = NULL;
	SG_man = 0;

	TablaDeFinales.Dll_CargadaSg = false;
	return true;
}

void Iniciar_Mascara()
{
	for (int i = 0; i < 64; i++)
	{
		SG_Mascara[i] = 0ULL;
	}
	for (int i = 0; i < 64; i++)
	{
		SG_Mascara[(i) ^ 0x38] |= 1ULL << i;
	}
}

unsigned ProbarSyzygy()
{
	int i = 0;

	U64 SG_Blancas = 0ULL, SG_Negras = 0ULL, SG_Rey = 0ULL, SG_Dama = 0ULL;
	U64 SG_Torre = 0ULL, SG_Alfil = 0ULL, SG_Caballo = 0ULL, SG_Peon = 0ULL;

	if ((TableroGlobal.EnroqueB > Ninguno || TableroGlobal.EnroqueN > Ninguno) || TableroGlobal.FichaAlPasoPosicion != 0)
		return SG_RESULT_FAILED;

	for (i = 0; i < 64; i++)
	{
		switch (TableroGlobal.Tablero[i])
		{
		case PeonB:
			SG_Blancas |= SG_Mascara[i];
			SG_Peon |= SG_Mascara[i];
			break;
		case CaballoB:
			SG_Blancas |= SG_Mascara[i];
			SG_Caballo |= SG_Mascara[i];
			break;
		case AlfilB:
			SG_Blancas |= SG_Mascara[i];
			SG_Alfil |= SG_Mascara[i];
			break;
		case TorreB:
			SG_Blancas |= SG_Mascara[i];
			SG_Torre |= SG_Mascara[i];
			break;
		case DamaB:
			SG_Blancas |= SG_Mascara[i];
			SG_Dama |= SG_Mascara[i];
			break;
		case ReyB:
			SG_Blancas |= SG_Mascara[i];
			SG_Rey |= SG_Mascara[i];
			break;

		case PeonN:
			SG_Negras |= SG_Mascara[i];
			SG_Peon |= SG_Mascara[i];
			break;
		case CaballoN:
			SG_Negras |= SG_Mascara[i];
			SG_Caballo |= SG_Mascara[i];
			break;
		case AlfilN:
			SG_Negras |= SG_Mascara[i];
			SG_Alfil |= SG_Mascara[i];
			break;
		case TorreN:
			SG_Negras |= SG_Mascara[i];
			SG_Torre |= SG_Mascara[i];
			break;
		case DamaN:
			SG_Negras |= SG_Mascara[i];
			SG_Dama |= SG_Mascara[i];
			break;
		case ReyN:
			SG_Negras |= SG_Mascara[i];
			SG_Rey |= SG_Mascara[i];
			break;
		default:
			continue;
			break;
		}
	}

	return SG_probe_wdl(
		SG_Blancas,
		SG_Negras,
		SG_Rey,
		SG_Dama,
		SG_Torre,
		SG_Alfil,
		SG_Caballo,
		SG_Peon,
		0,		/*EP*/
		TableroGlobal.MueveBlancas);
}

#endif
