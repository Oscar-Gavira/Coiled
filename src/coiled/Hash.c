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

#include "Hash.h"

#ifdef USAR_HASH_TB

short GuardarFlag(int d, int e, int f)
{
	// 0000 0000 0111 1111 */		/*EDAD -  7 bit*/   Valor máximo 127
	// 0001 1111 1000 0000 */		/*DEPTH - 6 bit*/   Valor máximo 63
	// 0110 0000 0000 0000 */		/*FLAG -  2 bit*/   Valor máximo 3

	if (d > (MAX_PLY / 2) - 1)		/* Evitamos superar los 6 bit */
		d = (MAX_PLY / 2) - 1;
	if (d < 0)						/* Evitamos valor negativo */
		d = 0;
	if (e > MAX_EDAD)				/* Evitamos superar los 7 bit */
		e = MAX_EDAD;
	if (e < 0)						/* Evitamos valor negativo */
		e = 0;
	if (f > TT_BETA)				/* Evitamos superar los 2 bit */
		f = TT_BETA;
	if (f < TT_DESCONOCIDO)			/* Evitamos valor negativo */
		f = TT_DESCONOCIDO;

	return  (short)((e) | ( (d) << 7) | ((f) << 13));
}

/* Obtengo el hash inicial */
void ObtenerKey()
{
	int i;

	TableroGlobal.Hash = 0;

	/* Hash posición de las piezas */
	for (i = 0; i < 64; ++i)
	{
		if (TableroGlobal.Tablero[i] != CasillaVacia)
		{
			HASH_PIEZA(TableroGlobal.Tablero[i],i);
		}
	}

	/* Hash Al paso */
	if (TableroGlobal.FichaAlPasoPosicion != 0)
	{
		HASH_EP(TableroGlobal.FichaAlPasoPosicion);
	}

	/* Hash Enroque */
	HASH_ENROQUE(1 + TableroGlobal.EnroqueN);
	HASH_ENROQUE(62 - TableroGlobal.EnroqueB);

	/* Hash turno */
	HASH_LADO;
}

/* Creamos la tabla de hash aun tamaña especifico */
void CrearTransposicion(U64 MB)
{
	if (MB < MB_HASH_TABLE_MIN)
	{
		MB = MB_HASH_TABLE_MIN;
	}
	if (MB > MB_HASH_TABLE_MAX)
	{
		MB = MB_HASH_TABLE_MAX;
	}

	TT_Opciones.tt_Mb = MB;

	TT_Opciones.tt_Entradas = TT_Opciones.tt_Mb * 1024 * 1024 / sizeof(_ST_TT_Cache);

	LiberarMemoria();

	if (TT_Opciones.tt_Entradas > 0)
	{
		TT_Datos = (_ST_TT_Cache*)malloc( sizeof(_ST_TT_Cache) * TT_Opciones.tt_Entradas );
	}
	else
	{
		Salir = true;
	}

#if defined(USAR_HASH_TB)
	if (TT_Datos == NULL )
	{
		printf("Insufficient memory for hash table.\n");
		fflush(stdout);

		Salir = true;
		return;
	}
#endif

	LimpiarTransposicion();
}

void ConvertirValorTT(int *hPuntos)
{
	if (*hPuntos >= MATE_EN(MAX_PLY * 2))
	{
		*hPuntos += -TableroGlobal.Ply;
	}
	else if (*hPuntos <= MATE(MAX_PLY * 2))
	{
		*hPuntos += TableroGlobal.Ply;
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
/* Obtengo si la posición ya a sido analizada y que valores tenia */
int RecuperarPosicion(int *hPuntos, int *hEv, int *hMov, int *hFlag)
{
	U64 idx = (U64)(TableroGlobal.Hash % TT_Opciones.tt_Entradas);
	int i = 0;
	
	for (i = 0; i < CELDAS; i++)
	{
		if (TT_Datos[idx].Celdas[i].Hash == TableroGlobal.Hash)
		{
			*hPuntos = (int)TT_Datos[idx].Celdas[i].Puntos;
			*hMov = TT_Datos[idx].Celdas[i].M;
			*hFlag = TT_Datos[idx].Celdas[i].Flag;
			*hEv = TT_Datos[idx].Celdas[i].Ev;
			return true;
		}
	}

	return false;
}
/* Guardo la posición analizada */
void AlmacenarPosicion(int depth, int puntos, int hEv, int hFlag, int hMov)
{
	U64 idx = (U64)(TableroGlobal.Hash % TT_Opciones.tt_Entradas);
	int idc = -1;
	int i = 0;
	int hEdad = TT_Opciones.tt_Edad - 2;

	for (i = 0; i < CELDAS; i++)
	{
		if (TT_Datos[idx].Celdas[i].Hash == TableroGlobal.Hash)
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
			TT_Opciones.tt_Completo++;
			idc = i;
			break;
		}
		else
		{
			/* De una interacción anterior, obtenemos el mas vieja. */
			if ((short)EDAD(TT_Datos[idx].Celdas[i].Flag) < hEdad)
			{
				TT_Opciones.tt_Completo++;
				hEdad = EDAD(TT_Datos[idx].Celdas[i].Flag);
				idc = i;
			}
			/* En el mismo momento, profundidad almacenada menor a la actual */
			if ((short)EDAD(TT_Datos[idx].Celdas[i].Flag) == TT_Opciones.tt_Edad && (int)DEPTH(TT_Datos[idx].Celdas[i].Flag) < depth)
			{
				idc = i;
			}
			/* Final de la Celdas y sin coincidencia. Guardamos. */
			if (i == CELDAS - 1 && idc == -1)
			{
				idc = i;
			}
		}
	}

	if (puntos >= MATE_EN(MAX_PLY * 2))
	{
		puntos += TableroGlobal.Ply;
	}
	else if (puntos <= MATE(MAX_PLY * 2))
	{
		puntos += -TableroGlobal.Ply;
	}

	TT_Datos[idx].Celdas[idc].Hash = TableroGlobal.Hash;
	TT_Datos[idx].Celdas[idc].Flag = GuardarFlag(depth, TT_Opciones.tt_Edad, hFlag);
	TT_Datos[idx].Celdas[idc].Puntos = puntos;
	TT_Datos[idx].Celdas[idc].Ev = hEv;
	TT_Datos[idx].Celdas[idc].M = hMov;
}
/* Obtenemos el movimiento si esta almacenado */
int RecuperarMovimientoHash()
{
	U64 idx = (U64)(TableroGlobal.Hash % TT_Opciones.tt_Entradas);
	int i = 0;

	for (i = 0; i < CELDAS; i++)
	{
		if (TT_Datos[idx].Celdas[i].Hash == TableroGlobal.Hash)
		{
			return TT_Datos[idx].Celdas[i].M;
		}
	}

	return NO_MOVIMIENTO;
}

/* Vació la tabla hash */
void LimpiarTransposicion()
{
	U64 i = 0;
	int ii = 0;

	for (i = 0; i < TT_Opciones.tt_Entradas; i++)
	{
		for (ii = 0; ii < CELDAS; ii++)
		{
			TT_Datos[i].Celdas[ii].Hash = 0;
			TT_Datos[i].Celdas[ii].Puntos = VALOR_TB_VACIO;
			TT_Datos[i].Celdas[ii].Ev = VALOR_TB_VACIO;
			TT_Datos[i].Celdas[ii].Flag = GuardarFlag(0, 1, TT_DESCONOCIDO);
			TT_Datos[i].Celdas[ii].M = NO_MOVIMIENTO;
		}
	}

	TT_Opciones.tt_Edad = 1;
	TT_Opciones.tt_Completo = 0;
}
/* Obtengo el estado de la tabla (0%, 50%, 100%) */
int ObtenerHashCompleto()
{
	if (TT_Opciones.tt_Completo > 0)
		return (int)( (float)((float)TT_Opciones.tt_Completo / (float)(TT_Opciones.tt_Entradas * CELDAS)) * 1000.0f);
	else
		return 0;
}
/* Liberamos la memoria */
void LiberarMemoria()
{
	if (TT_Datos != NULL)
		free(TT_Datos);
}

#endif
