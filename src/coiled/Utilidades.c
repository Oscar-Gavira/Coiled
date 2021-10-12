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
#include "See.h"

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

void SplitString(char *_string, char *contenedor, int longitud)
{
    int ll = 0;
	int i = 0;
	int len = strlen(_string);
	char *ptr = _string;

	while (*ptr == ' ')
	{
		ll++;
		ptr++;
	}

	while (*ptr != ' ' && *ptr != '\0')
	{
        ll++;

		if (ll >= longitud)
		{
			contenedor[longitud - 1] = '\0';
			break;
		}

		*contenedor++ = *ptr++;
	}
	*contenedor = '\0';
	
	while (*ptr == ' ')
	{
		ll++;
		ptr++;
	}
	for (i = 0; i < len; i++)
	{
		_string[i] = _string[i + ll];
	}
}

/* Obtenemos el tiempo */
U64 ObtenerTiempo()
{
#if defined(_WIN32) || defined(_WIN64)
	return GetTickCount();
#else
	struct timeval tv;

	gettimeofday(&tv, NULL);
	return tv.tv_sec * 1000 + tv.tv_usec / 1000;
#endif
}

int EntradaStdIn ()
{
#ifdef _WIN32
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
#else
	fd_set readfds;
	struct timeval tv;

	FD_ZERO(&readfds);
	FD_SET(STDIN_FILENO, &readfds);
	tv.tv_sec = 0;
	tv.tv_usec = 0;
	select(STDIN_FILENO + 1, &readfds, NULL, NULL, &tv);
	return FD_ISSET(STDIN_FILENO, &readfds);
#endif
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
			return SeePeonValor;
			break;
		case PeonN:
			return SeePeonValor;
			break;
		case CaballoB:
			return SeeCaballoValor;
			break;
		case CaballoN:
			return SeeCaballoValor;
			break;
		case AlfilB:
			return SeeAlfilValor;
			break;
		case AlfilN:
			return SeeAlfilValor;
			break;
		case TorreB:
			return SeeTorreValor;
			break;
		case TorreN:
			return SeeTorreValor;
			break;
		case DamaB:
			return SeeDamaValor;
			break;
		case DamaN:
			return SeeDamaValor;
			break;
		case ReyB:
			return SeeReyValor;
			break;
		case ReyN:
			return SeeReyValor;
			break;
		default:
			return 0;
			break;
	}
}

int IndexOfShift(char *base, char *str, int startIndex) {
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
