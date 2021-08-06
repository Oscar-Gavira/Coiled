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

#include "LibroAperturas.h"

#ifdef USAR_SQLITE

/*
Las librerías para usar el libro de apertura mediante SqlIte son las oficiales. 
Se pueden descargar de:
https://www.sqlite.org/index.html									Web oficial
https://www.sqlite.org/2021/sqlite-dll-win32-x86-3360000.zip		versión de 32bit
https://www.sqlite.org/2021/sqlite-dll-win64-x64-3360000.zip		versión de 64bit
solo renombrar el archivo a:
sqlite3_x64.dll
o
sqlite3_x86.dll
*/
HMODULE sqlite_hmod = NULL;

#ifdef ARC_64BIT
	char SQLITE_NAME[] = { "sqlite3_x64.dll" };
#else
	char SQLITE_NAME[] = { "sqlite3_x86.dll" };
#endif

int Cargar_sqlite_dll()
{
	if ((sqlite_hmod = LoadLibrary(SQLITE_NAME)) != 0)
	{
		sqlite3_open_v2 = (SQLITE3_OPEN_V2)GetProcAddress(sqlite_hmod, "sqlite3_open_v2");
		if (sqlite3_open_v2 == NULL)
		{
			return false;
		}
		sqlite3_prepare_v2 = (SQLITE_PREPARE_V2)GetProcAddress(sqlite_hmod, "sqlite3_prepare_v2");
		if (sqlite3_prepare_v2 == NULL)
		{
			return false;
		}
		sqlite3_step = (SQLITE_STEP)GetProcAddress(sqlite_hmod, "sqlite3_step");
		if (sqlite3_step == NULL)
		{
			return false;
		}
		sqlite3_column_text = (SQLITE_COLUMN_TEXT)GetProcAddress(sqlite_hmod, "sqlite3_column_text");
		if (sqlite3_column_text == NULL)
		{
			return false;
		}
		sqlite3_column_int = (SQLITE_COLUMN_INT)GetProcAddress(sqlite_hmod, "sqlite3_column_int");
		if (sqlite3_column_int == NULL)
		{
			return false;
		}
		sqlite3_reset = (SQLITE_RESET)GetProcAddress(sqlite_hmod, "sqlite3_reset");
		if (sqlite3_reset == NULL)
		{
			return false;
		}
		sqlite3_close_v2 = (SQLITE_CLOSE_V2)GetProcAddress(sqlite_hmod, "sqlite3_close_v2");
		if (sqlite3_close_v2 == NULL)
		{
			return false;
		}

		return true;
	}
	else
	{
		LibroSql.UsarLibro = false;
		return false;
	}
}

int Descargar_sqlite_dll()
{
	if (sqlite_hmod == NULL)
	{
		return true;
	}

	FreeLibrary(sqlite_hmod);

	sqlite_hmod = NULL;
	sqlite3_open_v2 = NULL;
	sqlite3_prepare_v2 = NULL;
	sqlite3_step = NULL;
	sqlite3_column_text = NULL;
	sqlite3_column_int = NULL;
	sqlite3_reset = NULL;
	sqlite3_close_v2 = NULL;
	return true;
}

int ComprobarAccesoLibro()
{
	int error = 0;

	if (LibroSql.UsarLibro)
	{
		error = sqlite3_open_v2("file:Book.db", &LibroSql.ConexionBD, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE | SQLITE_OPEN_URI, NULL);

		if (error)
		{
			CerrarBaseDeDatos();
			return false;
		}
		CerrarBaseDeDatos();
		return true;
	}
	else
	{
		CerrarBaseDeDatos();
		return false;
	}
}

void ListaJugadas(char *_Jugadas, int longitud)
{
	if (!LibroSql.FinVariacion && LibroSql.UsarLibro)
	{
		memset(LibroSql.Apertura, 0, (MAX_LONG + 1) * sizeof(char));
		if (strlen(_Jugadas) < (size_t)LibroSql.LimiteJugadas * 10)
		{
			if ( (strlen(LibroSql.Apertura) - longitud) > 0)
				strcat(LibroSql.Apertura, _Jugadas);

			return;
		}
	}

	LibroSql.FinVariacion = true;
}

void ObtenerJugadaLibro(char *move)
{
	if (!LibroSql.FinVariacion && LibroSql.UsarLibro)
	{
		move[0] = LibroSql.Jugada[0];
		move[1] = LibroSql.Jugada[1];
		move[2] = LibroSql.Jugada[2];
		move[3] = LibroSql.Jugada[3];

		return;
	}

	memset(LibroSql.Jugada, 0, 5 * sizeof(char));
	memset(LibroSql.Apertura, 0, (MAX_LONG + 1) * sizeof(char));

	return;
}

int BuscarJugadaLibro()
{
	int error = 0;
	char Aperturas[15][MAX_LONG + 1];
	int Aperturas_Count = 0;
	int Naperturas = 0;
	int randomNumber = 0;
	char randomNumberC[MAX_DIR];

	if (LibroSql.FinVariacion)
	{
		return false;
	}
	if (!LibroSql.UsarLibro)
	{
		return false;
	}

	error = sqlite3_open_v2("file:Book.db", &LibroSql.ConexionBD, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE | SQLITE_OPEN_URI, NULL);

	if (error)
	{
		printf("Opening book not loaded.\n");
		fflush(stdout);

		CerrarBaseDeDatos();
		LibroSql.FinVariacion = true;
		return false;
	}

	memset(LibroSql.Sql, 0, (MAX_DIR * 4) * sizeof(char));
	memset(LibroSql.Jugada, 0, 5 * sizeof(char));
	memset(Aperturas, 0, 15 * (MAX_LONG + 1) * sizeof(char));

	if (LibroSql.Apertura[0] != '\0')
	{
		if (LibroSql.AperturaEstandar == true)
		{
			strcat(LibroSql.Sql, "SELECT Partida FROM ");
			strcat(LibroSql.Sql, LibroSql.SqlTabla);
			strcat(LibroSql.Sql, " WHERE Partida LIKE '");
		}
		else
		{
			strcat(LibroSql.Sql, "SELECT Partida, Variante FROM ");
			strcat(LibroSql.Sql, LibroSql.SqlTabla);
			strcat(LibroSql.Sql, " WHERE Variante = '");
			strcat(LibroSql.Sql, LibroSql.Variante);
			strcat(LibroSql.Sql, "' AND Partida LIKE '");
#ifdef USAR_AJEDREZ960
			if (IndexOf_shift(LibroSql.Apertura, "O-O", 0) != -1 || IndexOf_shift(LibroSql.Apertura, "O-O-O", 0) != -1)
			{
				Ajedrez960EnroqueVariante();
			}
#endif
		}

		strcat(LibroSql.Sql, LibroSql.Apertura);
		strcat(LibroSql.Sql, "%' LIMIT 15;\0");
	}
	else
	{
		Naperturas = ObtenerNumeroAperturas(LibroSql.ConexionBD);
		randomNumber = rand() % (Naperturas + 1);

		memset(LibroSql.Sql, 0, (MAX_DIR * 4) * sizeof(char));
		memset(randomNumberC, 0, MAX_DIR * sizeof(char));

		if (LibroSql.AperturaEstandar == true)
		{
			strcat(LibroSql.Sql, "SELECT Partida FROM ");
			strcat(LibroSql.Sql, LibroSql.SqlTabla);
			strcat(LibroSql.Sql, " LIMIT ");
		}
		else
		{
			strcat(LibroSql.Sql, "SELECT Partida FROM ");
			strcat(LibroSql.Sql, LibroSql.SqlTabla);
			strcat(LibroSql.Sql, " WHERE Variante = '");
			strcat(LibroSql.Sql, LibroSql.Variante);
			strcat(LibroSql.Sql, "' LIMIT ");
		}

		sprintf(randomNumberC, "%d", randomNumber);
		fflush(stdout);
		strcat(LibroSql.Sql, randomNumberC);
		strcat(LibroSql.Sql, ", 15;\0");
	}

	error = sqlite3_prepare_v2(LibroSql.ConexionBD, LibroSql.Sql, -1, &LibroSql.stmt, NULL);

    if (error)
    {
		CerrarBaseDeDatos();
		LibroSql.FinVariacion = true;
		return false;
	}

	while ((error = sqlite3_step(LibroSql.stmt)) == SQLITE_ROW)
	{
		if (Aperturas_Count < 15)
		{
			strncpy(Aperturas[Aperturas_Count++], (char*)sqlite3_column_text(LibroSql.stmt, 0), MAX_LONG);
		}
	}

	if (error == SQLITE_DONE && Aperturas_Count)
	{
		int Aleatorio = rand() % Aperturas_Count;
		char j[4];
		memset(j, 0, 4 * sizeof(char));

		if (LibroSql.Apertura[0] != '\0')
		{
			int i = 0;

			while (LibroSql.Apertura[i] == Aperturas[Aleatorio][i])
			{
				i++;
			}
			j[0] = Aperturas[Aleatorio][i + 1];
			j[1] = Aperturas[Aleatorio][i + 2];
			j[2] = Aperturas[Aleatorio][i + 3];
			j[3] = Aperturas[Aleatorio][i + 4];

			strncpy(LibroSql.Jugada, j, 4 * sizeof(char));
		}
		else
		{
			j[0] = Aperturas[Aleatorio][0];
			j[1] = Aperturas[Aleatorio][1];
			j[2] = Aperturas[Aleatorio][2];
			j[3] = Aperturas[Aleatorio][3];

			strncpy(LibroSql.Jugada, j, 4 * sizeof(char));
		}

		if (LibroSql.Jugada[0] != '\0')
		{
			CerrarBaseDeDatos();
			LibroSql.FinVariacion = false;
			return true;
		}
		else
		{
			CerrarBaseDeDatos();
			LibroSql.FinVariacion = true;
			return false;
		}
	}
	else if (error == SQLITE_DONE && !Aperturas_Count)
	{
		CerrarBaseDeDatos();
		LibroSql.FinVariacion = true;
		return false;
	}
	else
	{
		/* Error en el libro de aperturas. */
		printf("Opening book error.\n");
		fflush(stdout);
		printf("Error code: "S32_FORMAT"\n", error);
		fflush(stdout);

		CerrarBaseDeDatos();
		LibroSql.FinVariacion = true;
		return false;
	}

	/* Cerramos la ConexionBD para liberar memoria */
	CerrarBaseDeDatos();
	LibroSql.FinVariacion = true;
	return false;
}

void Ajedrez960EnroqueVariante()
{
	char Apertura[MAX_LONG + 1];
	int Indice = 0;
	int i = 0;
	int j = 0;
	char mov[6];
	int Blancas960 = true;

	memset(mov, 0, 6 * sizeof(char));
	memset(Apertura, '\0', (MAX_LONG + 1) * sizeof(char));

	while ((int)strlen(LibroSql.Apertura) > 0)
	{
		if (IndexOf_shift(LibroSql.Apertura, "O-O", 0) != -1)
		{
			Indice = IndexOf_shift(LibroSql.Apertura, "O-O", 0);
			strcat(Apertura, LibroSql.Apertura);

			for (i = 0; i < Indice; i++)
			{
				if (LibroSql.Apertura[i] == ' ')
				{
					Blancas960 = !Blancas960;
				}
			}

			if (Blancas960 == true)
			{
				MovimientoCoordenadas(56 + IndexOf_shift(LibroSql.Variante, "K", 0), TableroGlobal.Ajedrez960.TorreBlancaH, 0, mov);
			}
			else
			{
				MovimientoCoordenadas(IndexOf_shift(LibroSql.Variante, "K", 0), TableroGlobal.Ajedrez960.TorreNegraH, 0, mov);
			}
			strcat(Apertura, mov);

			j = Indice + 3;

			for (i = 0; i < (int)strlen(LibroSql.Apertura); i++)
			{
				if (j + i <= MAX_LONG)
					LibroSql.Apertura[i] = LibroSql.Apertura[j + i];
			}

			continue;
		}
		if (IndexOf_shift(LibroSql.Apertura, "O-O-O", 0) != -1)
		{
			Indice = IndexOf_shift(LibroSql.Apertura, "O-O-O", 0);
			strcat(Apertura, LibroSql.Apertura);

			for (i = 0; i < Indice; i++)
			{
				if (LibroSql.Apertura[i] == ' ')
				{
					Blancas960 = !Blancas960;
				}
			}

			if (Blancas960 == true)
			{
				MovimientoCoordenadas(56 + IndexOf_shift(LibroSql.Variante, "K", 0), TableroGlobal.Ajedrez960.TorreBlancaA, 0, mov);
			}
			else
			{
				MovimientoCoordenadas(IndexOf_shift(LibroSql.Variante, "K", 0), TableroGlobal.Ajedrez960.TorreNegraA, 0, mov);
			}
			strcat(Apertura, mov);

			j = Indice + 5;

			for (i = 0; i < (int)strlen(LibroSql.Apertura); i++)
			{
				if (j + i <= MAX_LONG)
					LibroSql.Apertura[i] = LibroSql.Apertura[j + i];
			}

			continue;
		}

		strcat(Apertura, LibroSql.Apertura);
		break;
	}


	memset(LibroSql.Apertura, 0, (MAX_LONG + 1) * sizeof(char));
	
	Apertura[strlen(Apertura)] = '\0';
	strcpy(LibroSql.Apertura, Apertura);
	
	memset(Apertura, 0, (MAX_LONG + 1) * sizeof(char));
}

int ObtenerNumeroAperturas()
{
	int error = 0;
	int Aperturas_Count = 0;
	/* Objetos para realizar la conexión */

	memset(LibroSql.Sql, 0, (MAX_DIR * 4) * sizeof(char));

	if (LibroSql.AperturaEstandar == true)
	{
		strcat(LibroSql.Sql, "SELECT COUNT(Partida) FROM ");
		strcat(LibroSql.Sql, LibroSql.SqlTabla);
		strcat(LibroSql.Sql, ";\0");
	}
	else
	{
		strcat(LibroSql.Sql, "SELECT COUNT(Partida) FROM ");
		strcat(LibroSql.Sql, LibroSql.SqlTabla);
		strcat(LibroSql.Sql, " WHERE Variante = '");
		strcat(LibroSql.Sql, LibroSql.Variante);
		strcat(LibroSql.Sql, "';\0");
	}

	error = sqlite3_prepare_v2(LibroSql.ConexionBD, LibroSql.Sql, -1, &LibroSql.stmt, NULL);

	if (error)
	{
		return 0;
	}

	while ((error = sqlite3_step(LibroSql.stmt)) == SQLITE_ROW)
	{
		Aperturas_Count = sqlite3_column_int(LibroSql.stmt, 0);
	}

	if (LibroSql.stmt != NULL)
	{
		sqlite3_reset(LibroSql.stmt);
	}

	return Aperturas_Count;
}

void CerrarBaseDeDatos()
{
	if (LibroSql.stmt != NULL)
	{
		if (sqlite3_reset(LibroSql.stmt) != SQLITE_OK)
		{
			printf("Error on reboot sqlite3_stmt.\n");
			fflush(stdout);
		}
	}

	if (LibroSql.ConexionBD != NULL)
	{
		if (sqlite3_close_v2(LibroSql.ConexionBD) != SQLITE_OK)
		{
			printf("Error closing database.\n");
			fflush(stdout);
		}
	}

	memset(LibroSql.Apertura, 0, (MAX_LONG + 1) * sizeof(char));
}

void VerificarLibroApertura()
{
	int error = 0;
	char ErrorJugadas[MAX_LONG + 1];
	int Ok = false;
	int ErrorJugada = false;
	char fen[80];
	int pp = 0;

	printf("Analyzing opening book...\n");
	fflush(stdout);

	error = sqlite3_open_v2("file:Book.db", &LibroSql.ConexionBD, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE | SQLITE_OPEN_URI, NULL);

	/* Si la conexión no se puede realizar */
	if(error)
	{
		printf("Opening book error.\n");
		fflush(stdout);
		CerrarBaseDeDatos();
		LibroSql.FinVariacion = true;
		return;
	}

	memset(LibroSql.Sql, 0, (MAX_DIR * 4) * sizeof(char));
	if (LibroSql.AperturaEstandar == true)
	{
		strcat(LibroSql.Sql, "SELECT Partida FROM ");
		strcat(LibroSql.Sql, LibroSql.SqlTabla);
		strcat(LibroSql.Sql, ";\0");
	}
	else
	{
		strcat(LibroSql.Sql, "SELECT Partida, Variante FROM ");
		strcat(LibroSql.Sql, LibroSql.SqlTabla);
		strcat(LibroSql.Sql, ";\0");
	}

	error = sqlite3_prepare_v2(LibroSql.ConexionBD, LibroSql.Sql, -1, &LibroSql.stmt, NULL);

	/* Si no existe coincidencia */
    if (error == SQLITE_ERROR)
    {
		printf("Corrupted opening book.\n");
		fflush(stdout);
		CerrarBaseDeDatos();
        return;
    }

	char Ajedrez960Enroque[] = { 'a','b','c','d','e','f','g','h' };
	char Ajedrez960EnroqueBook[5];

	while ((error = sqlite3_step(LibroSql.stmt)) == SQLITE_ROW)
    {
		memset(fen, 0, 80 * sizeof(char));
		memset(Ajedrez960EnroqueBook, '\0', 5 * sizeof(char));
		int vv = LibroSql.AperturaEstandar == true ? 0 : 1;

		char *val = (char*)sqlite3_column_text(LibroSql.stmt, 0);
		char *var = (char*)sqlite3_column_text(LibroSql.stmt, vv);

		if (ErrorJugada == true)
		{
			break;
		}

		memset(LibroSql.Jugada, 0, 5 * sizeof(char));
		memset(ErrorJugadas, 0, (MAX_LONG + 1) * sizeof(char));

		if (strlen(val) <= MAX_LONG)
		{
			memcpy(ErrorJugadas, val, MAX_LONG * sizeof(char));
		}

		strcat(val, " ");
		if (LibroSql.AperturaEstandar == true)
		{
			char buffer[57];
			strcpy(buffer, START_POS);
			Position_Fen_Startpos(buffer);
		}
		else
		{
			strcat(fen, "fen ");
			for (pp = 0; pp < (int)strlen(var); pp++)
			{
				fen[pp+4] = tolower(var[pp]);
				if (tolower(var[pp]) == 'r')
				{
					if (Ajedrez960EnroqueBook[0] == '\0')
						Ajedrez960EnroqueBook[0] = toupper(Ajedrez960Enroque[pp]); /* Blancas */
					else
						Ajedrez960EnroqueBook[1] = toupper(Ajedrez960Enroque[pp]); /* Blancas */

					if (Ajedrez960EnroqueBook[2] == '\0')
						Ajedrez960EnroqueBook[2] = Ajedrez960Enroque[pp];	/* Negras */
					else
						Ajedrez960EnroqueBook[3] = Ajedrez960Enroque[pp];	/* Negras */
				}
			}

			Ajedrez960EnroqueBook[strlen(Ajedrez960EnroqueBook)] = '\0';

			strcat(fen, "/pppppppp/8/8/8/8/PPPPPPPP/");
			strcat(fen, var);
			strcat(fen, " w ");
			strcat(fen, Ajedrez960EnroqueBook);
			strcat(fen, " - 0 1");

			Position_Fen_Startpos(fen);
		}

		while (1)
		{
			Ok = false;
			if (LibroSql.Jugada[0] != '\0')
			{
				if (val[0] != '\0')
				{
					int indice = 0;

					char *e = strchr(val, ' ');
					if (e != NULL)
					{
						if (*e == '\0')
						{
							*e = 0;
						}
						indice = (int)(e - val);
						if (indice < 0)
						{
							indice = (int)strlen(val);
						}
					}
					if (indice > 4)
					{
						ErrorJugada = true;
						Salir = false;
						break;
					}

					memset(LibroSql.Jugada, 0, 5 * sizeof(char));
					strncpy(LibroSql.Jugada, val, indice * sizeof(char));
					val+=indice+1;
					/* Aquí realiza el movimiento, para verificar si es correcto */
					if (LibroSql.Jugada[0] != '\0')
					{
						Movimiento(LibroSql.Jugada, &Ok, true);
					}
					else
					{
						break;
					}

					if (Ok == false)
					{
						ErrorJugada = true;
						Salir = false;
						break;
					}
				}
				else
				{
					break;
				}
			}
			else
			{
				strncpy(LibroSql.Jugada, val, 4 * sizeof(char));
				val+=5;	/* Desplazamos los caracteres */
				if (LibroSql.Jugada[0] != '\0')
					Movimiento(LibroSql.Jugada, &Ok, true);
				else
					Ok = true;

				if (Ok == false)
				{
					ErrorJugada = true;
					Salir = false;
					break;
				}
			}
		}
	}
	if(error == SQLITE_DONE)
	{
		if (LibroSql.AperturaEstandar == true)
		{
			printf("Opening book correct.\n");
		}
		else
		{
			printf("Opening book correct. Chess960.\n");
		}
		fflush(stdout);
		CerrarBaseDeDatos();
		return;
	}
	else
	{
		if (LibroSql.AperturaEstandar == true)
		{
			printf("Opening book error.\n");
		}
		else
		{
			printf("Opening book error. Chess960.\n");
		}
		fflush(stdout);
		printf("Incorrect opening in: "STRING_FORMAT"\n", ErrorJugadas);
		fflush(stdout);
		printf("Move: "STRING_FORMAT"\n", LibroSql.Jugada);
		fflush(stdout);
		printf("Error code: "S32_FORMAT"\n", error);
		fflush(stdout);
		CerrarBaseDeDatos();
		return;
	}

	CerrarBaseDeDatos();
	return;
}
#endif
