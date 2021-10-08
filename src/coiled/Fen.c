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

#include "Externo.h"
#include "Utilidades.h"
#include "Fen.h"
#ifdef USAR_SQLITE
	#include "LibroAperturas.h"
#endif


/* Cargamos una posicion FEN */
int CargarFen(char *epd)
{
	/* Vector de coordenadas para el peon al paso */
	char TableroCoordenadasL[] = {
		'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h',
		'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h',
		'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h',
		'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h',
		'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h',
		'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h',
		'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h',
		'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'};
	char TableroCoordenadasN[] = {
		'8', '8', '8', '8', '8', '8', '8', '8',
		'7', '7', '7', '7', '7', '7', '7', '7',
		'6', '6', '6', '6', '6', '6', '6', '6',
		'5', '5', '5', '5', '5', '5', '5', '5',
		'4', '4', '4', '4', '4', '4', '4', '4',
		'3', '3', '3', '3', '3', '3', '3', '3',
		'2', '2', '2', '2', '2', '2', '2', '2',
		'1', '1', '1', '1', '1', '1', '1', '1' };

	char TableroCoordenadas[3];

	char pieza_char[] = { ' ','P','N','B','R','Q','K',' ','p','n','b','r','q','k',' ' };

	int i = 0;
	int j = 0;
	int pieza = 0;
	char contenedor[80];
	memset(contenedor, 0, 80 * sizeof(char));
	memset(TableroCoordenadas, 0, 3 * sizeof(char));

	char Ajedrez960Enroque[] = { 'A','B','C','D','E','F','G','H','a','b','c','d','e','f','g','h' };
	char FenEnroque[] = { 'Q','z','z','z','z','z','z','K','q','z','z','z','z','z','z','k' };
	int Ajedrez960TableroTorre[] = { 56, 57, 58, 59, 60, 61, 62, 63, 0, 1, 2, 3, 4, 5, 6, 7 };
	int cas = 0;
#ifdef USAR_SQLITE
	int Variante = 0;
#endif

	TableroGlobal.EnroqueB = Ninguno;
	TableroGlobal.EnroqueN = Ninguno;
	TableroGlobal.FichaAlPasoPosicion = 0;
	TableroGlobal.PosicionReyB = 0;
	TableroGlobal.PosicionReyN = 0;
	TableroGlobal.Ajedrez960.TorreNegraA = 0;
	TableroGlobal.Ajedrez960.TorreNegraH = 0;
	TableroGlobal.Ajedrez960.TorreBlancaA = 0;
	TableroGlobal.Ajedrez960.TorreBlancaH = 0;

	for (i = 0; i < 64; i += 8)
	{
		j = 0;
		while (j < 8)
		{
			if (*epd >= '1' && *epd <= '8')
			{
				for (pieza = 0; pieza < *epd - '0'; pieza++)
				{
					TableroGlobal.Tablero[i + j] = CasillaVacia;			/* Asignamos Cuadro vacio */
					j++;
				}
			}
			else
			{
				for (pieza = 0; (pieza_char[pieza] != *epd); pieza++)
				{
					if (pieza >= NoPieza)
					{
						return false;
					}
				}
				if (pieza >= NoPieza)
				{
					return false;
				}
				TableroGlobal.Tablero[i + j] = pieza;
#ifdef USAR_SQLITE

				if (i >= 56 && pieza > 1 && pieza < 7)
				{
					LibroSql.Variante[Variante++] = pieza_char[pieza];
				}
#endif
				switch (pieza)										/* Que pieza es? */
				{
					case PeonB:										/* Peon blanco */
						Blancas.PeonTotales++; 						/* Incrementamos el numero de peones */
						break;
					case CaballoB:									/* Caballo blanco */
						Blancas.CaballosTotales++;					/* Incrementamos el numero de caballos */
						break;
					case AlfilB:									/* Alfil blanco */
						Blancas.AlfilTotales++;						/* Incrementamos el numero de alfiles */
						break;
					case TorreB:									/* Torre blanco */
						Blancas.TorresTotales++;					/* Incrementamos el numero de torres */
						break;
					case DamaB:										/* Dama blanco */
						Blancas.DamasTotales++;						/* Incrementamos el numero de damas */
						break;
					case ReyB:										/* Rey blanco */
						TableroGlobal.PosicionReyB = i + j;			/* Almacenamos la posicion del rey */
						break;

					case PeonN:										/* Peon negro */
						Negras.PeonTotales++;						/* Incrementamos el numero de peones */
						break;
					case CaballoN:									/* Caballo negro */
						Negras.CaballosTotales++;					/* Incrementamos el numero de caballo */
						break;
					case AlfilN:									/* Alfil negro */
						Negras.AlfilTotales++;						/* Incrementamos el numero de alfil */
						break;
					case TorreN:									/* Torre negro */
						Negras.TorresTotales++;						/* Incrementamos el numero de torre */
						break;
					case DamaN:										/* Dama negro */
						Negras.DamasTotales++;						/* Incrementamos el numero de dama */
						break;
					case ReyN:										/* Rey negro */
						TableroGlobal.PosicionReyN = i + j;			/* Almacenamos la posicion del rey */
						break;
					default:
						return false;
						break;
				}
				j++;
			}
			if (strcmp(epd, "") != 0)
			{
				epd++;
			}
		}
		if (pieza >= NoPieza)
		{
			return false;
		}
		if (strcmp(epd, "") != 0)
		{
			epd++;
		}
	}

	if (pieza >= NoPieza)
	{
		return false;
	}

	if (strcmp(epd, "") != 0)
	{
		if (*epd++ == 'w')
		{
			TableroGlobal.MueveBlancas = true;
		}
		else
		{
			TableroGlobal.MueveBlancas = false;
		}
	}
	else
	{
		return false;
	}

	if (strcmp(epd, "") != 0)
	{
		epd++;
	}

	if (*epd == '-')
	{
		if (strcmp(epd, "") != 0)
		{
			epd++;
		}
	}
	else
	{
		for (cas = 0; cas < 16; cas++)
		{
			if (*epd == FenEnroque[cas])
			{
				if (cas < 8)
				{
					if (COLUMNA(cas) < COLUMNA(TableroGlobal.PosicionReyB))
					{
						for (i = Ajedrez960TableroTorre[cas]; i > -1; i-=8)
						{
							if (TableroGlobal.Tablero[i] == TorreB)
							{
								if (HORIZONTAL(Ajedrez960TableroTorre[cas]) == 0)
								{
									TableroGlobal.Ajedrez960.TorreBlancaA = Ajedrez960TableroTorre[cas];
									break;
								}
							}
						}

						TableroGlobal.EnroqueB = TableroGlobal.EnroqueB + Largo;
						if (strcmp(epd, "") != 0)
						{
							epd++;
						}
						cas = -1;
					}
					else/* enroque corto*/
					{
						for (i = Ajedrez960TableroTorre[cas]; i > -1; i-=8)
						{
							if (TableroGlobal.Tablero[i] == TorreB)
							{
								if (HORIZONTAL(Ajedrez960TableroTorre[cas]) == 0)
								{
									TableroGlobal.Ajedrez960.TorreBlancaH = Ajedrez960TableroTorre[cas];
									break;
								}
							}
						}

						TableroGlobal.EnroqueB = TableroGlobal.EnroqueB + Corto;
						if (strcmp(epd, "") != 0)
						{
							epd++;
						}
						cas = -1;
					}
				}
				else /* negras */
				{
					if (COLUMNA(cas) < COLUMNA(TableroGlobal.PosicionReyN))
					{
						for (i = Ajedrez960TableroTorre[cas]; i < 64; i += 8)
						{
							if (TableroGlobal.Tablero[i] == TorreN)
							{
								if (HORIZONTAL(Ajedrez960TableroTorre[cas]) == 7)
								{
									TableroGlobal.Ajedrez960.TorreNegraA = Ajedrez960TableroTorre[cas];
									break;
								}
							}
						}

						TableroGlobal.EnroqueN = TableroGlobal.EnroqueN + Largo;
						if (strcmp(epd, "") != 0)
						{
							epd++;
						}
						cas = -1;
					}
					else/* enroque corto*/
					{
						for (i = Ajedrez960TableroTorre[cas]; i < 64; i+=8)
						{
							if (TableroGlobal.Tablero[i] == TorreN)
							{
								if (HORIZONTAL(Ajedrez960TableroTorre[cas]) == 7)
								{
									TableroGlobal.Ajedrez960.TorreNegraH = Ajedrez960TableroTorre[cas];
									break;
								}
							}
						}

						TableroGlobal.EnroqueN = TableroGlobal.EnroqueN + Corto;
						if (strcmp(epd, "") != 0)
						{
							epd++;
						}
						cas = -1;
					}
				}
			}
		}

		/* XFEN - de Ajedrez960 AHah */
		for (cas = 0; cas < 16; cas++)
		{
			if (*epd == Ajedrez960Enroque[cas])
			{
				if (cas < 8)
				{
					if (COLUMNA(cas) < COLUMNA(TableroGlobal.PosicionReyB))
					{
						for (i = Ajedrez960TableroTorre[cas]; i > -1; i-=8)
						{
							if (TableroGlobal.Tablero[i] == TorreB)
							{
								if (HORIZONTAL(Ajedrez960TableroTorre[cas]) == 0)
								{
									TableroGlobal.Ajedrez960.TorreBlancaA = Ajedrez960TableroTorre[cas];
									break;
								}
							}
						}

						TableroGlobal.EnroqueB = TableroGlobal.EnroqueB + Largo;
						if (strcmp(epd, "") != 0)
						{
							epd++;
						}
						cas = -1;
					}
					else/* enroque corto*/
					{
						for (i = Ajedrez960TableroTorre[cas]; i > -1; i-=8)
						{
							if (TableroGlobal.Tablero[i] == TorreB)
							{
								if (HORIZONTAL(Ajedrez960TableroTorre[cas]) == 0)
								{
									TableroGlobal.Ajedrez960.TorreBlancaH = Ajedrez960TableroTorre[cas];
									break;
								}
							}
						}

						TableroGlobal.EnroqueB = TableroGlobal.EnroqueB + Corto;
						if (strcmp(epd, "") != 0)
						{
							epd++;
						}
						cas = -1;
					}
				}
				else /* negras */
				{
					if (COLUMNA(cas) < COLUMNA(TableroGlobal.PosicionReyN))
					{
						for (i = Ajedrez960TableroTorre[cas]; i < 64; i += 8)
						{
							if (TableroGlobal.Tablero[i] == TorreN)
							{
								if (HORIZONTAL(Ajedrez960TableroTorre[cas]) == 7)
								{
									TableroGlobal.Ajedrez960.TorreNegraA = Ajedrez960TableroTorre[cas];
									break;
								}
							}
						}

						TableroGlobal.EnroqueN = TableroGlobal.EnroqueN + Largo;
						if (strcmp(epd, "") != 0)
						{
							epd++;
						}
						cas = -1;
					}
					else/* enroque corto*/
					{
						for (i = Ajedrez960TableroTorre[cas]; i < 64; i += 8)
						{
							if (TableroGlobal.Tablero[i] == TorreN)
							{
								if (HORIZONTAL(Ajedrez960TableroTorre[cas]) == 7)
								{
									TableroGlobal.Ajedrez960.TorreNegraH = Ajedrez960TableroTorre[cas];
									break;
								}
							}
						}

						TableroGlobal.EnroqueN = TableroGlobal.EnroqueN + Corto;
						if (strcmp(epd, "") != 0)
						{
							epd++;
						}
						cas = -1;
					}
				}
			}
		}
	}

	if (strcmp(epd, "") != 0)
	{
		epd++;
	}

	epd = SplitString(epd, contenedor, sizeof(contenedor));
	if (strcmp(contenedor, "-") ==  0)
	{
		TableroGlobal.FichaAlPasoPosicion = 0;
	}
	else
	{
		for (i = 0; i < 64; i++)
		{
			TableroCoordenadas[0] = TableroCoordenadasL[i];
			TableroCoordenadas[1] = TableroCoordenadasN[i];
			if (strcmp(contenedor, TableroCoordenadas) ==  0)
			{
				break;
			}
		}

		if (i >= 40 && i <= 47)
		{
			TableroGlobal.FichaAlPasoPosicion = i - 8;				/* Blancas */
		}
		if (i >= 16 && i <= 23)
		{
			TableroGlobal.FichaAlPasoPosicion = i + 8;				/* Negras */
		}
	}


	if (strcmp(epd, "") != 0)
	{
		memset(contenedor, 0, 80 * sizeof(char));
		epd++;
		epd = SplitString(epd, contenedor, sizeof(contenedor));
		TableroGlobal.Regla_50_Movimiento = MAX(0, (int)atoll(contenedor));
		if (TableroGlobal.Regla_50_Movimiento > 100) TableroGlobal.Regla_50_Movimiento = 100;
#ifdef USAR_HASH_TB
		TableroGlobal.Hply = TableroGlobal.Regla_50_Movimiento;
#endif
	}

	if (TableroGlobal.Regla_50_Movimiento < 0 || TableroGlobal.Regla_50_Movimiento > 100)
	{
#ifdef USAR_HASH_TB
		TableroGlobal.Hply = 0;
#endif
		TableroGlobal.Regla_50_Movimiento = 0;
	}

	return true;
}
