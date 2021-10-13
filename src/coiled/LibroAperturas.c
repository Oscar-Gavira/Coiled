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

#include "LibroAperturas.h"

#ifdef USAR_SQLITE

/*
Las librerias para usar el libro de apertura mediante SqlIte son las oficiales.
Se pueden descargar de:
https://www.sqlite.org/index.html									Web oficial
https://www.sqlite.org/2021/sqlite-dll-win32-x86-3360000.zip		version de 32bit
https://www.sqlite.org/2021/sqlite-dll-win64-x64-3360000.zip		version de 64bit
solo renombrar el archivo a:
sqlite3_x64.dll
o
sqlite3_x86.dll
*/
HMODULE sqlite_hmod = NULL;
#ifdef _WIN32
#ifdef ARC_64BIT
	char SQLITE_NOMBRE[] = { "sqlite3_x64.dll" };
#else
	char SQLITE_NOMBRE[] = { "sqlite3_x86.dll" };
#endif
#else
#ifdef ARC_64BIT
	char SQLITE_NOMBRE[] = { "/usr/lib/x86_64-linux-gnu/libsqlite3.so.0" };
#else
	char SQLITE_NOMBRE[] = { "/usr/lib/i386-linux-gnu/libsqlite3.so.0" };
#endif
#endif

int Cargar_sqlite_dll()
{
	if ((sqlite_hmod = LoadLibrary(SQLITE_NOMBRE)) != 0)
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
		printf(""INFO_STRING""STRING_FORMAT" not found. Unable to use Book.\n", SQLITE_NOMBRE);
		fflush(stdout);
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
	sqlite3_reset = NULL;
	sqlite3_close_v2 = NULL;
	return true;
}

int ComprobarAccesoLibro()
{
	int error = 0;

	if (LibroSql.UsarLibro)
	{
		error = sqlite3_open_v2("file:Book.db", &LibroSql.ConexionBD, SQLITE_OPEN_READONLY | SQLITE_OPEN_URI, NULL);

		if (error)
		{
			printf(""INFO_STRING"Book.db not found. Unable to use Book.\n");
			fflush(stdout);
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
				strncat(LibroSql.Apertura, _Jugadas, longitud);

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

int BuscarJugadaLibro(char *Str)
{
	int error = 0;
	int i = 0;
	char VarianteApertura[MAX_LONG + 1];
	char j[5];
	memset(j, 0, 5 * sizeof(char));

	if (LibroSql.FinVariacion)
	{
		return false;
	}
	if (!LibroSql.UsarLibro)
	{
		return false;
	}

	error = sqlite3_open_v2("file:Book.db", &LibroSql.ConexionBD, SQLITE_OPEN_READONLY | SQLITE_OPEN_URI, NULL);

	if (error)
	{
		printf(""INFO_STRING"Opening book not loaded.\n");
		fflush(stdout);

		CerrarBaseDeDatos();
		LibroSql.FinVariacion = true;
		return false;
	}

	memset(LibroSql.Sql, 0, MAX_DIR * sizeof(char));
	memset(LibroSql.Jugada, 0, 5 * sizeof(char));
	memset(VarianteApertura, 0, (MAX_LONG + 1) * sizeof(char));

	if (Str[0] != '\0')
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
			/* Convertimos los enroques de la GUI Arena al formato del libro qu es de tipo UCI */
			if (IndexOfShift(Str, "O-O", 0) != -1 || IndexOfShift(Str, "O-O-O", 0) != -1)
			{
				Ajedrez960EnroqueVariante();
			}
#endif
		}

		strcat(LibroSql.Sql, Str);
		strcat(LibroSql.Sql, "%' ORDER BY RANDOM() LIMIT 1;\0");
	}
	else
	{
		memset(LibroSql.Sql, 0, MAX_DIR * sizeof(char));

		if (LibroSql.AperturaEstandar == true)
		{
			strcat(LibroSql.Sql, "SELECT Partida FROM ");
			strcat(LibroSql.Sql, LibroSql.SqlTabla);
			strcat(LibroSql.Sql, " ORDER BY RANDOM() LIMIT 1;\0");
		}
		else
		{
			strcat(LibroSql.Sql, "SELECT Partida FROM ");
			strcat(LibroSql.Sql, LibroSql.SqlTabla);
			strcat(LibroSql.Sql, " WHERE Variante = '");
			strcat(LibroSql.Sql, LibroSql.Variante);
			strcat(LibroSql.Sql, "' ORDER BY RANDOM() LIMIT 1;\0");
		}
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
		strncpy(VarianteApertura, (char*)sqlite3_column_text(LibroSql.stmt, 0), MAX_LONG);
	}

	if (error == SQLITE_DONE)
	{
		if (LibroSql.Apertura[0] != '\0')
		{
			while (LibroSql.Apertura[i] == VarianteApertura[i])
			{
				i++;
			}
			j[0] = VarianteApertura[i + 1];
			j[1] = VarianteApertura[i + 2];
			j[2] = VarianteApertura[i + 3];
			j[3] = VarianteApertura[i + 4];
		}
		else
		{
			j[0] = VarianteApertura[0];
			j[1] = VarianteApertura[1];
			j[2] = VarianteApertura[2];
			j[3] = VarianteApertura[3];
		}

		strncpy(LibroSql.Jugada, j, 5);

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
	else
	{
		/* Error en el libro de aperturas. */
		printf(""INFO_STRING"Opening book error.\n");
		fflush(stdout);
		printf(""INFO_STRING"Error code: "S32_FORMAT"\n", error);
		fflush(stdout);

		CerrarBaseDeDatos();
		LibroSql.FinVariacion = true;
		return false;
	}
}

/* Para convertir el enroque de la GUI arena O-O / O-O-O a coordenadas e1g1 o chess960 */
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
		if (IndexOfShift(LibroSql.Apertura, "O-O", 0) != -1)
		{
			Indice = IndexOfShift(LibroSql.Apertura, "O-O", 0);
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
				MovimientoCoordenadas(56 + IndexOfShift(LibroSql.Variante, "K", 0), TableroGlobal.Ajedrez960.TorreBlancaH, 0, mov);
			}
			else
			{
				MovimientoCoordenadas(IndexOfShift(LibroSql.Variante, "K", 0), TableroGlobal.Ajedrez960.TorreNegraH, 0, mov);
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
		if (IndexOfShift(LibroSql.Apertura, "O-O-O", 0) != -1)
		{
			Indice = IndexOfShift(LibroSql.Apertura, "O-O-O", 0);
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
				MovimientoCoordenadas(56 + IndexOfShift(LibroSql.Variante, "K", 0), TableroGlobal.Ajedrez960.TorreBlancaA, 0, mov);
			}
			else
			{
				MovimientoCoordenadas(IndexOfShift(LibroSql.Variante, "K", 0), TableroGlobal.Ajedrez960.TorreNegraA, 0, mov);
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

void CerrarBaseDeDatos()
{
	if (LibroSql.stmt != NULL)
	{
		if (sqlite3_reset(LibroSql.stmt) != SQLITE_OK)
		{
			printf(""INFO_STRING"Error on reboot sqlite3_stmt.\n");
			fflush(stdout);
		}
	}

	if (LibroSql.ConexionBD != NULL)
	{
		if (sqlite3_close_v2(LibroSql.ConexionBD) != SQLITE_OK)
		{
			printf(""INFO_STRING"Error closing database.\n");
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
	char fen[MAX_DIR];
	int pp = 0;
	char Ajedrez960Enroque[] = { 'a','b','c','d','e','f','g','h' };
	char Ajedrez960EnroqueBook[5];
	char *val = NULL;
	char *variante = NULL;

	if (LibroSql.AperturaEstandar == true)
	{
		printf(""INFO_STRING"Analyzing opening book...\n");
	}
	else
	{
		printf(""INFO_STRING"Analyzing opening book Chess960...\n");
	}

	fflush(stdout);

	error = sqlite3_open_v2("file:Book.db", &LibroSql.ConexionBD, SQLITE_OPEN_READONLY | SQLITE_OPEN_URI, NULL);

	/* Si la conexion no se puede realizar */
	if(error)
	{
		printf(""INFO_STRING"Opening book error.\n");
		fflush(stdout);
		CerrarBaseDeDatos();
		LibroSql.FinVariacion = true;
		return;
	}

	memset(LibroSql.Sql, 0, MAX_DIR * sizeof(char));
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
		printf(""INFO_STRING"Corrupted opening book.\n");
		fflush(stdout);
		CerrarBaseDeDatos();
        return;
    }


	while ((error = sqlite3_step(LibroSql.stmt)) == SQLITE_ROW)
    {
		memset(fen, 0, MAX_DIR * sizeof(char));
		memset(Ajedrez960EnroqueBook, '\0', 5 * sizeof(char));

		val = (char*)sqlite3_column_text(LibroSql.stmt, 0);
		variante = (char*)sqlite3_column_text(LibroSql.stmt, !LibroSql.AperturaEstandar);

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
			for (pp = 0; pp < (int)strlen(variante); pp++)
			{
				fen[pp+4] = tolower(variante[pp]);
				if (tolower(variante[pp]) == 'r')
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
			strcat(fen, variante);
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
					/* Aqui realiza el movimiento, para verificar si es correcto */
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
			printf(""INFO_STRING"Opening book correct.\n");
		}
		else
		{
			printf(""INFO_STRING"Opening book correct. Chess960.\n");
		}
		fflush(stdout);
		CerrarBaseDeDatos();
		return;
	}
	else
	{
		if (LibroSql.AperturaEstandar == true)
		{
			printf(""INFO_STRING"Opening book error.\n");
		}
		else
		{
			printf(""INFO_STRING"Opening book error. Chess960.\n");
		}
		fflush(stdout);
		printf(""INFO_STRING"Incorrect opening in: "STRING_FORMAT"\n", ErrorJugadas);
		fflush(stdout);
		printf(""INFO_STRING"Move: "STRING_FORMAT"\n", LibroSql.Jugada);
		fflush(stdout);
		printf(""INFO_STRING"Error code: "S32_FORMAT"\n", error);
		fflush(stdout);
		CerrarBaseDeDatos();
		return;
	}

	CerrarBaseDeDatos();
	return;
}
#endif
