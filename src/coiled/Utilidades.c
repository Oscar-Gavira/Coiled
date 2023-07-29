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

#include "Utilidades.h"

void ImprimirMovimientoCoordenadas(int Inicio, int Fin, int PiezaCoronacion)
{
	char Movi[6];
	memset(Movi, 0, 6 * sizeof(char));
	MovimientoCoordenadas(Inicio, Fin, PiezaCoronacion, Movi);
	printf(""STRING_FORMAT"", Movi);
}
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
void SplitString(char *string, char *contenedor, int longitud)
{
	int i = 0;
	int len = strlen(string);
	int indice = 0;

	while (string[indice] == ' ')
	{
		string++;
	}

	while (string[indice] != ' ' && string[indice] != '\0')
	{
		if (indice >= longitud - 1)
		{
			contenedor[longitud - 1] = '\0';
			break;
		}

		contenedor[indice] = string[indice];
		indice++;
	}
	contenedor[indice] = '\0';
	
	while (string[indice] == ' ')
	{
		indice++;
	}
	for (i = 0; i < len - indice; i++)
	{
		string[i] = string[i + indice];
	}
	string[i] = '\0';
}
void SubString(char *string, int indice)
{
	int i = 0;
	int len = strlen(string);

	for (i = 0; i < len - indice; i++)
	{
		string[i] = string[i + indice];
	}
	string[i] = '\0';
}
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
void VerificarDir(char *base, int dir)
{
	int baselen = strlen(base);
	int i = 0;

	char buscar = '\\';
	char remplazar = '/';

	for (i = 0; i < baselen; i++)
	{
		if (base[i] == buscar)
		{
			base[i] = remplazar;
		}
	}

	if (dir == true)
	{
		if (base[baselen - 1] != remplazar)
			base[baselen] = remplazar;
	}
}
int ArchivoExiste(char *archivo)
{
	FILE *fp;

	if ((fp = fopen(archivo, "r")) == NULL)
		return false;

	fclose(fp);
	return true;
}

#ifndef _WIN32
void Sleep(int milisegundos)
{
	static int SECS_TO_SLEEP = 0, NSEC_TO_SLEEP = 1000000;
	struct timespec remaining, request = { SECS_TO_SLEEP,  milisegundos * NSEC_TO_SLEEP };

	if (nanosleep(&request, &remaining) != -1)
	{
		printf("OK.\n");
	}
}
#endif
