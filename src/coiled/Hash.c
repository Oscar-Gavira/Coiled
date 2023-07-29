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

#include "Hash.h"

_ST_TT_Cache *TT_Datos;

void ObtenerKey(_ST_TableroX64 *Tablero)
{
	int i;

	Tablero->Hash = 0;

	/* Hash posicion de las piezas */
	for (i = 0; i < 64; ++i)
	{
		if (Tablero->Tablero[i] != CasillaVacia)
		{
			HASH_PIEZA(Tablero, Tablero->Tablero[i],i); 
		}
	}

	/* Hash Al paso */
	if (Tablero->FichaAlPasoPosicion != 0)
	{
		HASH_EP(Tablero, Tablero->FichaAlPasoPosicion);
	}

	/* Hash Enroque */
	HASH_ENROQUE(Tablero, 1 + Tablero->EnroqueN);
	HASH_ENROQUE(Tablero, 62 - Tablero->EnroqueB);

	/* Hash turno */
	HASH_LADO(Tablero);
}
int CrearTransposicion()
{
	TT_Opciones.tt_Entradas = TT_Opciones.tt_Mb * 1024 * 1024 / sizeof(_ST_TT_Cache);

	if (TT_Datos != NULL && TT_Opciones.tt_Entradas > 0)
	{
		free(TT_Datos);
		TT_Datos = (_ST_TT_Cache*)malloc(sizeof(_ST_TT_Cache) * TT_Opciones.tt_Entradas);
	}
	else if (TT_Opciones.tt_Entradas > 0)
	{
		TT_Datos = (_ST_TT_Cache*)malloc(sizeof(_ST_TT_Cache) * TT_Opciones.tt_Entradas);
	}

#if defined(USAR_HASH_TB)
	if (TT_Datos == NULL )
	{
		printf(""INFO_STRING"Insufficient memory for hash table.\n");
		fflush(stdout);

		return false;
	}
#endif

	return true;
}
void ConvertirValorTT(_ST_TableroX64 *Tablero, int *hPuntos)
{
	if (*hPuntos >= VALOR_MATE_MAX)
	{
		*hPuntos += -Tablero->Ply;
	}
	else if (*hPuntos <= -VALOR_MATE_MAX)
	{
		*hPuntos += Tablero->Ply;
	}
}
int PodaHash(int *hFlag, int *beta, int *alpha, int *hPuntos)
{
	switch (FLAG(*hFlag))
	{
	case TT_EXACTO:
		return true; break;
	case TT_BETA:
		return (*hPuntos >= *beta) ? true : false; break;
	case TT_ALPHA:
		return (*hPuntos <= *alpha) ? true : false; break;
	default:
		break;
	}
	return false;
}
int RecuperarPosicion(_ST_TableroX64 *Tablero, int *hPuntos, int *hEv, int *hMov, int *hFlag)
{
	U64 idx = (U64)(Tablero->Hash % TT_Opciones.tt_Entradas);
#if defined(ARC_64BIT)
	U32 Hash32 = (U32)(Tablero->Hash >> 32);
#else
	U32 Hash32 = Tablero->Hash;
#endif
	int i = 0;

	for (i = 0; i < CELDAS; i++)
	{
		if (TT_Datos[idx].Celdas[i].Hash == Hash32)
		{
			*hMov = TT_Datos[idx].Celdas[i].M;
			*hPuntos = TT_Datos[idx].Celdas[i].Puntos;
			*hEv = TT_Datos[idx].Celdas[i].Ev;
			*hFlag = TT_Datos[idx].Celdas[i].Flag;
			return true;
		}
	}

	return false;
}
void AlmacenarPosicion(_ST_TableroX64 *Tablero, int depth, int puntos, int hEv, int hFlag, int hMov)
{
	U64 idx = (U64)(Tablero->Hash % TT_Opciones.tt_Entradas);
#if defined(ARC_64BIT)
	U32 Hash32 = (U32)(Tablero->Hash >> 32);
#else
	U32 Hash32 = Tablero->Hash;
#endif
	int idcE = -1;
	int idcD = -1;
	int idc = -1;
	int i = 0;
	int hEdad = TT_Opciones.tt_Edad;
	int hDepth = depth;

	for (i = 0; i < CELDAS; i++)
	{
		if (TT_Datos[idx].Celdas[i].Hash == Hash32)
		{
			if (FLAG(TT_Datos[idx].Celdas[i].Flag) != TT_EXACTO
				&& depth < DEPTH(TT_Datos[idx].Celdas[i].Flag) - 3) {
				return;
			}

			idc = i;
			if (hMov == NO_MOVIMIENTO)
				hMov = TT_Datos[idx].Celdas[i].M;

			break;
		}
		else if (TT_Datos[idx].Celdas[i].Hash == 0)
		{
			if (Tablero->IdSubProcesos == 0) TT_Opciones.tt_HashCompleto++;
			idc = i;
			break;
		}
		else
		{
			if (EDAD(TT_Datos[idx].Celdas[i].Flag) < hEdad && DEPTH(TT_Datos[idx].Celdas[i].Flag) < depth)
			{
				hEdad = EDAD(TT_Datos[idx].Celdas[i].Flag);
				idcE = i;
			}
			if (EDAD(TT_Datos[idx].Celdas[i].Flag) == TT_Opciones.tt_Edad && DEPTH(TT_Datos[idx].Celdas[i].Flag) < hDepth)
			{
				hDepth = DEPTH(TT_Datos[idx].Celdas[i].Flag);
				idcD = i;
			}
			if (i == CELDAS - 1 && idcE == -1 && idcD == -1)
			{
				idc = i;
			}
		}
	}

	if (idc == -1 && (idcE != -1 || idcD != -1))
	{
		if (idcE != -1)
			idc = idcE; 
		else
			idc = idcD;
	}

	if (puntos >= VALOR_MATE_MAX)
	{
		puntos += Tablero->Ply;
	}
	else if (puntos <= -VALOR_MATE_MAX)
	{
		puntos += -Tablero->Ply;
	}

	TT_Datos[idx].Celdas[idc].Hash = Hash32;
	TT_Datos[idx].Celdas[idc].M = hMov;
	TT_Datos[idx].Celdas[idc].Puntos = (short)puntos;
	TT_Datos[idx].Celdas[idc].Ev = (short)hEv;
	TT_Datos[idx].Celdas[idc].Flag = GuardarFlag(TT_Opciones.tt_Edad, depth, hFlag);
}
void LimpiarTransposicion()
{
	memset(TT_Datos, 0, TT_Opciones.tt_Entradas * sizeof(_ST_TT_Cache));
	TT_Opciones.tt_Edad = 0;
	TT_Opciones.tt_HashCompleto = 0;
}
int ObtenerHashCompleto()
{
	return MIN(1000, (int)( (float)((float)(TT_Opciones.tt_HashCompleto * TipoJuego.NumeroDeSubProcesos) / (float)(TT_Opciones.tt_Entradas * CELDAS)) * 1000.0f));
}
void LiberarMemoria()
{
	if (TT_Datos != NULL)
		free(TT_Datos);
}
