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

#ifndef LIBROAPERTURAS_H
#define LIBROAPERTURAS_H

#include "Externo.h"
#include "Utilidades.h"

#ifdef USAR_SQLITE

/***********************************************************************************
							Metodos
***********************************************************************************/
int Cargar_sqlite_dll();											/* Cargamos la libreria para manejar el libro de aperturas sqlite */
int Descargar_sqlite_dll();											/* Descargamos la libreria sqlite */
int ComprobarAccesoLibro();											/* Comprobamos si podemos acceder a la base de datos (Libro de apertura) */
void ListaJugadas(char *_Jugadas, int longitud);					/* Lista de los movimientos realizados hasta el momento (e2e4 c7c5 g1f3 d7d6 f1b5 b8c6...)
																	   para realizar la busqueda en la base de datos con LIKE %	*/
void ObtenerJugadaLibro(char *move);								/* Obtenemos el movimiento a realizar obtenido de la Bd. */
int BuscarJugadaLibro(char *Str);									/* Buscamos en la Bd si existe una variacion. */
void Ajedrez960EnroqueVariante();
void CerrarBaseDeDatos();											/* Cerramos la conexion con la Bd. */
void VerificarLibroApertura();										/* Rutina para verificar las aperturas de la Bd.
																	   Analiza una por una todas las aperturas y de cada apertura todas las jugadas que contiene para saber si hay algun movimiento mal. */
#endif

#endif
