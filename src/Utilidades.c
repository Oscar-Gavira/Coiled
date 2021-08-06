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

#include "Externo.h"
#include "Utilidades.h"

/* Imprimir el movimiento en coordenadas E2E4 */
void ImprimirMovimientoCoordenadas(int Inicio, int Fin, int PiezaCoronacion)
{
	char Movi[6];
	memset(Movi, 0, 6 * sizeof(char));
	MovimientoCoordenadas(Inicio, Fin, PiezaCoronacion, Movi);
	printf(""STRING_FORMAT"", Movi);
}

/* Obtenemos el movimiento en coordenadas E2E4 */
void MovimientoCoordenadas(int Inicio, int Fin, int PiezaCoronacion, char *mov)
{
	mov[0] = 'a' + COLUMNA(Inicio);
	mov[1] = '1' + HORIZONTAL(Inicio);
	mov[2] = 'a' + COLUMNA(Fin);
	mov[3] = '1' + HORIZONTAL(Fin);

	switch (PiezaCoronacion)
	{
		case CaballoB:
			mov[4] = 'n';
			mov[5] = '\0';
			break;
		case AlfilB:
			mov[4] = 'b';
			mov[5] = '\0';
			break;
		case TorreB:
			mov[4] = 'r';
			mov[5] = '\0';
			break;
		case DamaB:
			mov[4] = 'q';
			mov[5] = '\0';
			break;
		case CaballoN:
			mov[4] = 'n';
			mov[5] = '\0';
			break;
		case AlfilN:
			mov[4] = 'b';
			mov[5] = '\0';
			break;
		case TorreN:
			mov[4] = 'r';
			mov[5] = '\0';
			break;
		case DamaN:
			mov[4] = 'q';
			mov[5] = '\0';
			break;
		default:
			mov[4] = '\0';
			mov[5] = '\0';
			break;
	}
}

/* Limpiamos cadenas */
char *Split_String(char *_string, char *contenedor, int longitud)
{
    int ll = 0;

	while (*_string == ' ')
	{
		_string++;
	}

	while (*_string != ' ' && *_string != '\0')
	{
        ll++;

		if (ll >= longitud)
		{
			contenedor[longitud - 1] = '\0';
			return _string;
		}

		*contenedor++ = *_string++;
	}
	*contenedor = '\0';

	return _string;
}

/* Obtenemos el tiempo */
U64 ObtenerTiempo()
{
	return (U64)clock();
}

int Entrada_StdIn ()
{
	static int init = 0, pipe;
	static HANDLE inh;
	DWORD dw;
	if (!init)
	{
		init = 1;
		inh = GetStdHandle(STD_INPUT_HANDLE);
		pipe = !GetConsoleMode(inh, &dw);
		if (!pipe)
		{
			SetConsoleMode(inh, dw & ~(ENABLE_MOUSE_INPUT | ENABLE_WINDOW_INPUT));
			FlushConsoleInputBuffer(inh);
		}
	}
	if (pipe)
	{
		if (!PeekNamedPipe(inh, NULL, 0, NULL, &dw, NULL))
			return 1;
		return dw > 0;
	}
	else
	{
		GetNumberOfConsoleInputEvents(inh, &dw);
		return dw <= 1 ? 0 : 1;
	}
}

/* Obtenemos el valor de la pieza */
int ValorPieza(int Pieza)
{
	switch (Pieza)
	{
		case 0:
			return 0;
			break;
		case PeonB:
			return PeonValor;
			break;
		case PeonN:
			return PeonValor;
			break;
		case CaballoB:
			return CaballoValor;
			break;
		case CaballoN:
			return CaballoValor;
			break;
		case AlfilB:
			return AlfilValor - 10;
			break;
		case AlfilN:
			return AlfilValor - 10;
			break;
		case TorreB:
			return TorreValor;
			break;
		case TorreN:
			return TorreValor;
			break;
		case DamaB:
			return DamaValor;
			break;
		case DamaN:
			return DamaValor;
			break;
		case ReyB:
			return ReyValor;
			break;
		case ReyN:
			return ReyValor;
			break;
		default:
			return 0;
			break;
	}
}

/* Comprueba si una cadena es numérica (sin decimales) */
int Es_Numero(char *p)
{
	int i = 0;
	if ((int)strlen(p) > MAX_DIR) return false;

	for (i = 0; i < MAX_DIR; i++)
	{
		if (p[i] == '\0')
		{
			if (i == 0)
			{
				if (i == 0) return false;
				break;
			}
			return true;
		}

		if (!Es_Un_Numero(p[i])) return false;
	}

	return true;
}

/* Verifica si es un carácter numérico */
int Es_Un_Numero(char c)
{
	int i = 0;
	int Ok = false;

	for (i = 48; i < 58; i++)
	{
		if (c == i)
		{
			Ok = true;
			break;
		}
	}

	return Ok;
}

int IndexOf_shift(char *base, char *str, int startIndex) {
	int result;
	int baselen = strlen(base);
	if ((int)strlen(str) > baselen || startIndex > baselen)
	{
		result = -1;
	}
	else 
	{
		if (startIndex < 0) startIndex = 0;
		char *pos = strstr(base + startIndex, str);
		if (pos == NULL) 
			result = -1;
		else 
			result = pos - base;
	}

	return result;
}
