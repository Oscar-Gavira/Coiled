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

#include "GeneradorDeMovimientos.h"
#include "Hash.h"

/*******************************************************************************
							Variables
*******************************************************************************/
int IndiceMovimiento = 0;													/* Numero de jugadas obtenidos */

/* Generamos todos los movimientos posibles.
TipoMov = Todos o Capturas y coronaciones */
int GenerarMovimientos(int TipoMov, _ST_Movimiento *M)
{
    int X = 0;

	IndiceMovimiento = 0;													/* Inicializamos el numero de jugadas que se van ha obtener */

	switch (TableroGlobal.MueveBlancas)
	{
		case true:															/* Busco las fichas para obtener los posibles movimientos */
			for (X = 63; X > -1; X--)										/* Empezamos a buscar de abajo para arriba */
			{
				if (TableroGlobal.Tablero[X] > ReyB)						/* Si en el tablero no existe una pieza blanca continuemos */
				{
					continue;
				}

				switch (TableroGlobal.Tablero[X])							/* Que pieza es? */
				{
					case PeonB:
						ObtenerMovimientoPeon(X, TipoMov, M);				/* Obtenemos los movimientos del peon */
						break;

					case CaballoB:
						ObtenerMovimientoCaballo(X, TipoMov, M);			/* Obtenemos los movimientos del caballo */
						break;

					case AlfilB:
						ObtenerMovimientoAlfil(X, TipoMov, M);				/* Obtenemos los movimientos del alfil */
						break;

					case TorreB:
						ObtenerMovimientoTorre(X, TipoMov, M);				/* Obtenemos los movimientos del torre */
						break;

					case DamaB:
						ObtenerMovimientoTorre(X, TipoMov, M);				/* Obtenemos los movimientos del dama */
						ObtenerMovimientoAlfil(X, TipoMov, M);
						break;

					case ReyB:
						ObtenerMovimientoRey(X, TipoMov, M);				/* Obtenemos los movimientos del rey */
						break;

                    default:
						Salir = true;
                        continue;
                        break;
				}
			}
			break;
		case false:
			for (X = 0; X < 64; X++)										/* Empezamos a buscar de arriba a bajo */
			{
				if (TableroGlobal.Tablero[X] < PeonN)						/* Si en el tablero no existe una pieza negra continuemos */
				{
					continue;
				}

				switch (TableroGlobal.Tablero[X])							/* Que pieza es? */
				{
					case PeonN:
						ObtenerMovimientoPeon(X, TipoMov, M);				/* Obtenemos los movimientos del peon */
						break;

					case CaballoN:
						ObtenerMovimientoCaballo(X, TipoMov, M);			/* Obtenemos los movimientos del caballo */
						break;

					case AlfilN:
						ObtenerMovimientoAlfil(X, TipoMov, M);				/* Obtenemos los movimientos del alfil */
						break;

					case TorreN:
						ObtenerMovimientoTorre(X, TipoMov, M);				/* Obtenemos los movimientos del torre */
						break;

					case DamaN:
						ObtenerMovimientoTorre(X, TipoMov, M);				/* Obtenemos los movimientos del dama */
						ObtenerMovimientoAlfil(X, TipoMov, M);
						break;

					case ReyN:
						ObtenerMovimientoRey(X, TipoMov, M);				/* Obtenemos los movimientos del rey */
						break;

                    default:
						Salir = true;
                        continue;
                        break;
				}
			}
			break;
        default:
            Salir = true;
            break;
	}

	return IndiceMovimiento;													/* Devolvemos el numero de jugadas obtenidas */
}

/* Obtenemos los movimiento del peon */
void ObtenerMovimientoPeon(int Inicio, int TipoMov, _ST_Movimiento *M)			/* Obtenemos los movimientos del peon */
{
	switch (TableroGlobal.MueveBlancas)
	{
		case true:
			/* Al Paso */
            if (TableroGlobal.FichaAlPasoPosicion)
            {
                /* Derecha */
				if (Inicio - 7 > -1 && Inicio + 1 == TableroGlobal.FichaAlPasoPosicion && TableroGlobal.TableroColor[Inicio] == TableroGlobal.TableroColor[Inicio - 7])
				{
					AddMoveCapturaAlPaso(Inicio, Inicio - 7, M);
				}
                /* Izquierda */
				if (Inicio - 7 > -1 && Inicio - 1 == TableroGlobal.FichaAlPasoPosicion && TableroGlobal.TableroColor[Inicio] == TableroGlobal.TableroColor[Inicio - 9])
				{
					AddMoveCapturaAlPaso(Inicio, Inicio - 9, M);
				}
            }
			/* Come en diagonal Derecha y Coronacion */
            if (Inicio - 7 > -1 && TableroGlobal.TableroColor[Inicio] == TableroGlobal.TableroColor[Inicio - 7] && TableroGlobal.Tablero[Inicio - 7] > CasillaVacia)
            {
                if (Inicio - 7 < 8)
				{
					AddMoveCoronacion(Inicio, Inicio - 7, TipoMov, M);
				}
				else
				{
					AddMoveNormal(Inicio, Inicio - 7, TipoMov, M);
				}
            }
            /* Come en diagonal Izquierda y Coronacion */
            if (Inicio - 9 > -1 && TableroGlobal.TableroColor[Inicio] == TableroGlobal.TableroColor[Inicio - 9] && TableroGlobal.Tablero[Inicio - 9] > CasillaVacia)
            {
                if (Inicio - 9 < 8)
				{
					AddMoveCoronacion(Inicio, Inicio - 9, TipoMov, M);
				}
				else
				{
					AddMoveNormal(Inicio, Inicio - 9, TipoMov, M);
				}
            }
            /* Movimiento normal */
            if (Inicio - 8 > -1 && TableroGlobal.Tablero[Inicio - 8] == CasillaVacia)
            {
                if (Inicio - 8 < 8)
				{
					AddMoveCoronacion(Inicio, Inicio - 8, TipoMov, M);
				}
				else
				{
					AddMoveNormal(Inicio, Inicio - 8, TipoMov, M);
				}
            }
            /* Dos adelante */
            if (Inicio > 47 && TableroGlobal.Tablero[Inicio - 8] == CasillaVacia && TableroGlobal.Tablero[Inicio - 16] == CasillaVacia)
			{
				AddMovePosicionFichaAlPaso(Inicio, Inicio - 16, TipoMov, M);
			}

            break;
        case false:
            /* Al Paso */
            if (TableroGlobal.FichaAlPasoPosicion)
            {
                /* Derecha */
				if (Inicio + 7 < 64 && Inicio - 1 == TableroGlobal.FichaAlPasoPosicion && TableroGlobal.TableroColor[Inicio] == TableroGlobal.TableroColor[Inicio + 7])
				{
					AddMoveCapturaAlPaso(Inicio, Inicio + 7, M);
				}
                /* Izquierda */
				if (Inicio + 9 < 64 && Inicio + 1 == TableroGlobal.FichaAlPasoPosicion && TableroGlobal.TableroColor[Inicio] == TableroGlobal.TableroColor[Inicio + 9])
				{
					AddMoveCapturaAlPaso(Inicio, Inicio + 9, M);
				}
            }
			/* Come en diagonal Derecha y Coronacion */
            if (Inicio + 7 < 64 && TableroGlobal.TableroColor[Inicio] == TableroGlobal.TableroColor[Inicio + 7] && TableroGlobal.Tablero[Inicio + 7] < CasillaVacia)
            {
                if (Inicio + 7 > 55)
				{
					AddMoveCoronacion(Inicio, Inicio + 7, TipoMov, M);
				}
				else
				{
					AddMoveNormal(Inicio, Inicio + 7, TipoMov, M);
				}
            }
            /* Come en diagonal Izquierda y Coronacion */
            if (Inicio + 9 < 64 && TableroGlobal.TableroColor[Inicio] == TableroGlobal.TableroColor[Inicio + 9] && TableroGlobal.Tablero[Inicio + 9] < CasillaVacia)
            {
                if (Inicio + 9 > 55)
				{
					AddMoveCoronacion(Inicio, Inicio + 9, TipoMov, M);
				}
				else
				{
					AddMoveNormal(Inicio, Inicio + 9, TipoMov, M);
				}
            }
            /* Movimiento normal y coronacion */
            if (Inicio + 8 < 64 && TableroGlobal.Tablero[Inicio + 8] == CasillaVacia)
            {
                if (Inicio + 8 > 55)
				{
					AddMoveCoronacion(Inicio, Inicio + 8, TipoMov, M);
				}
				else
				{
					AddMoveNormal(Inicio, Inicio + 8, TipoMov, M);
				}
            }
            /* Dos adelante */
            if (Inicio < 16 && TableroGlobal.Tablero[Inicio + 8] == CasillaVacia && TableroGlobal.Tablero[Inicio + 16] == CasillaVacia)
			{
				AddMovePosicionFichaAlPaso(Inicio, Inicio + 16, TipoMov, M);
			}

            break;
        default:
            Salir = true;
            break;
    }
}

/* Obtenemos los movimientos del torre */
void ObtenerMovimientoTorre(int Inicio, int TipoMov, _ST_Movimiento *M)
{
	int T = 0;
	int MargenIzquierdo2 = MargenIzquierdo(Inicio);												/* Obtenemos el cuadro de la izquierda (Columna A) */

    switch (TableroGlobal.MueveBlancas)
	{
		case true:
            for (T = Inicio + 8; T < 64; T += 8)												/* Arriba */
            {
                if (TableroGlobal.Tablero[T] > ReyB)
				{
					AddMoveNormal(Inicio, T, TipoMov, M);
				}
                if (TableroGlobal.Tablero[T] != CasillaVacia)
				{
					break;
				}
            }
            for (T = Inicio - 8; T > -1; T -= 8)												/* Bajo */
            {
                if (TableroGlobal.Tablero[T] > ReyB)
				{
					AddMoveNormal(Inicio, T, TipoMov, M);
				}
                if (TableroGlobal.Tablero[T] != CasillaVacia)
				{
					break;
				}
            }
            for (T = Inicio - 1; T >= MargenIzquierdo2 && T > -1; T--)							/* Izquierda */
            {
                if (TableroGlobal.Tablero[T] > ReyB)
				{
					AddMoveNormal(Inicio, T, TipoMov, M);
				}
                if (TableroGlobal.Tablero[T] != CasillaVacia)
				{
					break;
				}
            }
            for (T = Inicio + 1; T <= MargenIzquierdo2 + 7 && T < 64; T++)						/* Derecha */
            {
                if (TableroGlobal.Tablero[T] > ReyB)
				{
					AddMoveNormal(Inicio, T, TipoMov, M);
				}
                if (TableroGlobal.Tablero[T] != CasillaVacia)
				{
					break;
				}
            }

			break;
		case false:
            for (T = Inicio + 8; T < 64; T += 8)												/* Arriba */
            {
                if (TableroGlobal.Tablero[T] < PeonN)
				{
					AddMoveNormal(Inicio, T, TipoMov, M);
				}
                if (TableroGlobal.Tablero[T] != CasillaVacia)
				{
					break;
				}
            }
            for (T = Inicio - 8; T > -1; T -= 8)												/* Bajo */
            {
                if (TableroGlobal.Tablero[T] < PeonN)
				{
					AddMoveNormal(Inicio, T, TipoMov, M);
				}
                if (TableroGlobal.Tablero[T] != CasillaVacia)
				{
					break;
				}
            }
            for (T = Inicio - 1; T >= MargenIzquierdo2; T--)									/* Izquierda */
            {
                if (TableroGlobal.Tablero[T] < PeonN)
				{
					AddMoveNormal(Inicio, T, TipoMov, M);
				}
                if (TableroGlobal.Tablero[T] != CasillaVacia)
				{
					break;
				}
            }
            for (T = Inicio + 1; T <= MargenIzquierdo2 + 7; T++)								/* Derecha */
            {
                if (TableroGlobal.Tablero[T] < PeonN)
				{
					AddMoveNormal(Inicio, T, TipoMov, M);
				}
                if (TableroGlobal.Tablero[T] != CasillaVacia)
				{
					break;
				}
            }
            break;
        default:
            Salir = true;
            break;
	}
}

/* Obtenemos los movimientos del caballo */
void ObtenerMovimientoCaballo(int Inicio, int TipoMov, _ST_Movimiento *M)
{
	switch (TableroGlobal.MueveBlancas)
	{
		case true:
            if (Inicio + 10 < 64 && TableroGlobal.TableroColor[Inicio] != TableroGlobal.TableroColor[Inicio + 10] && TableroGlobal.Tablero[Inicio + 10] > ReyB)
			{
				AddMoveNormal(Inicio, Inicio + 10, TipoMov, M);
			}
            if (Inicio - 6 > -1 && TableroGlobal.TableroColor[Inicio] != TableroGlobal.TableroColor[Inicio - 6] && TableroGlobal.Tablero[Inicio - 6] > ReyB)
			{
				AddMoveNormal(Inicio, Inicio - 6, TipoMov, M);
			}
            if (Inicio + 6 < 64 && TableroGlobal.TableroColor[Inicio] != TableroGlobal.TableroColor[Inicio + 6] && TableroGlobal.Tablero[Inicio + 6] > ReyB)
			{
				AddMoveNormal(Inicio, Inicio + 6, TipoMov, M);
			}
            if (Inicio - 10 > -1 && TableroGlobal.TableroColor[Inicio] != TableroGlobal.TableroColor[Inicio - 10] && TableroGlobal.Tablero[Inicio - 10] > ReyB)
			{
				AddMoveNormal(Inicio, Inicio - 10, TipoMov, M);
			}
            if (Inicio + 17 < 64 && TableroGlobal.TableroColor[Inicio] != TableroGlobal.TableroColor[Inicio + 17] && TableroGlobal.Tablero[Inicio + 17] > ReyB)
			{
				AddMoveNormal(Inicio, Inicio + 17, TipoMov, M);
			}
            if (Inicio + 15 < 64 && TableroGlobal.TableroColor[Inicio] != TableroGlobal.TableroColor[Inicio + 15] && TableroGlobal.Tablero[Inicio + 15] > ReyB)
			{
				AddMoveNormal(Inicio, Inicio + 15, TipoMov, M);
			}
            if (Inicio - 15 > -1 && TableroGlobal.TableroColor[Inicio] != TableroGlobal.TableroColor[Inicio - 15] && TableroGlobal.Tablero[Inicio - 15] > ReyB)
			{
				AddMoveNormal(Inicio, Inicio - 15, TipoMov, M);
			}
            if (Inicio - 17 > -1 && TableroGlobal.TableroColor[Inicio] != TableroGlobal.TableroColor[Inicio - 17] && TableroGlobal.Tablero[Inicio - 17] > ReyB)
			{
				AddMoveNormal(Inicio, Inicio - 17, TipoMov, M);
			}

            break;
		case false:
            if (Inicio + 10 < 64 && TableroGlobal.TableroColor[Inicio] != TableroGlobal.TableroColor[Inicio + 10] && TableroGlobal.Tablero[Inicio + 10] < PeonN)
			{
				AddMoveNormal(Inicio, Inicio + 10, TipoMov, M);
			}
            if (Inicio - 6 > -1 && TableroGlobal.TableroColor[Inicio] != TableroGlobal.TableroColor[Inicio - 6] && TableroGlobal.Tablero[Inicio - 6] < PeonN)
			{
				AddMoveNormal(Inicio, Inicio - 6, TipoMov, M);
			}
            if (Inicio + 6 < 64 && TableroGlobal.TableroColor[Inicio] != TableroGlobal.TableroColor[Inicio + 6] && TableroGlobal.Tablero[Inicio + 6] < PeonN)
			{
				AddMoveNormal(Inicio, Inicio + 6, TipoMov, M);
			}
            if (Inicio - 10 > -1 && TableroGlobal.TableroColor[Inicio] != TableroGlobal.TableroColor[Inicio - 10] && TableroGlobal.Tablero[Inicio - 10] < PeonN)
			{
				AddMoveNormal(Inicio, Inicio - 10, TipoMov, M);
			}
            if (Inicio + 17 < 64 && TableroGlobal.TableroColor[Inicio] != TableroGlobal.TableroColor[Inicio + 17] && TableroGlobal.Tablero[Inicio + 17] < PeonN)
			{
				AddMoveNormal(Inicio, Inicio + 17, TipoMov, M);
			}
            if (Inicio + 15 < 64 && TableroGlobal.TableroColor[Inicio] != TableroGlobal.TableroColor[Inicio + 15] && TableroGlobal.Tablero[Inicio + 15] < PeonN)
			{
				AddMoveNormal(Inicio, Inicio + 15, TipoMov, M);
			}
            if (Inicio - 15 > -1 && TableroGlobal.TableroColor[Inicio] != TableroGlobal.TableroColor[Inicio - 15] && TableroGlobal.Tablero[Inicio - 15] < PeonN)
			{
				AddMoveNormal(Inicio, Inicio - 15, TipoMov, M);
			}
            if (Inicio - 17 > -1 && TableroGlobal.TableroColor[Inicio] != TableroGlobal.TableroColor[Inicio - 17] && TableroGlobal.Tablero[Inicio - 17] < PeonN)
			{
				AddMoveNormal(Inicio, Inicio - 17, TipoMov, M);
			}

            break;
        default:
            Salir = true;
            break;
	}
}

/* Obtenemos los movimientos del alfil */
void ObtenerMovimientoAlfil(int Inicio, int TipoMov, _ST_Movimiento *M)
{
	int D = 0;
	switch (TableroGlobal.MueveBlancas)
	{
		case true:
            for (D = Inicio + 9; D < 64; D += 9)													/* \ Diagonal bajo derecha */
            {
                if (TableroGlobal.TableroColor[Inicio] != TableroGlobal.TableroColor[D])
				{
					break;
				}
                if (TableroGlobal.Tablero[D] > ReyB)
				{
					AddMoveNormal(Inicio, D, TipoMov, M);
				}
                if (TableroGlobal.Tablero[D] != CasillaVacia)
				{
					break;
				}
            }
            for (D = Inicio + 7; D < 64; D += 7)													/* / Diagonal bajo izquierda */
            {
                if (TableroGlobal.TableroColor[Inicio] != TableroGlobal.TableroColor[D])
				{
					break;
				}
                if (TableroGlobal.Tablero[D] > ReyB)
				{
					AddMoveNormal(Inicio, D, TipoMov, M);
				}
                if (TableroGlobal.Tablero[D] != CasillaVacia)
				{
					break;
				}
            }
            for (D = Inicio - 7; D > -1; D -= 7)													/* / Diagonal arriba derecha */
            {
                if (TableroGlobal.TableroColor[Inicio] != TableroGlobal.TableroColor[D])
				{
					break;
				}
                if (TableroGlobal.Tablero[D] > ReyB)
				{
					AddMoveNormal(Inicio, D, TipoMov, M);
				}
                if (TableroGlobal.Tablero[D] != CasillaVacia)
				{
					break;
				}
            }
            for (D = Inicio - 9; D > -1; D -= 9)													/* \ Diagonal arriba izquierda */
            {
                if (TableroGlobal.TableroColor[Inicio] != TableroGlobal.TableroColor[D])
				{
					break;
				}
                if (TableroGlobal.Tablero[D] > ReyB)
				{
					AddMoveNormal(Inicio, D, TipoMov, M);
				}
                if (TableroGlobal.Tablero[D] != CasillaVacia)
				{
					break;
				}
            }

            break;
    	case false:
            for (D = Inicio + 9; D < 64; D += 9)													/* \ Diagonal bajo derecha */
            {
                if (TableroGlobal.TableroColor[Inicio] != TableroGlobal.TableroColor[D])
				{
					break;
				}
                if (TableroGlobal.Tablero[D] < PeonN)
				{
					AddMoveNormal(Inicio, D, TipoMov, M);
				}
                if (TableroGlobal.Tablero[D] != CasillaVacia)
				{
					break;
				}
            }
            for (D = Inicio + 7; D < 64; D += 7)													/* / Diagonal bajo izquierda */
            {
                if (TableroGlobal.TableroColor[Inicio] != TableroGlobal.TableroColor[D])
				{
					break;
				}
                if (TableroGlobal.Tablero[D] < PeonN)
				{
					AddMoveNormal(Inicio, D, TipoMov, M);
				}
                if (TableroGlobal.Tablero[D] != CasillaVacia)
				{
					break;
				}
            }
            for (D = Inicio - 7; D > -1; D -= 7)													/* / Diagonal arriba derecha */
            {
                if (TableroGlobal.TableroColor[Inicio] != TableroGlobal.TableroColor[D])
				{
					break;
				}
                if (TableroGlobal.Tablero[D] < PeonN)
				{
					AddMoveNormal(Inicio, D, TipoMov, M);
				}
                if (TableroGlobal.Tablero[D] != CasillaVacia)
				{
					break;
				}
            }
            for (D = Inicio - 9; D > -1; D -= 9)													/* \ Diagonal arriba izquierda */
            {
                if (TableroGlobal.TableroColor[Inicio] != TableroGlobal.TableroColor[D])
				{
					break;
				}
                if (TableroGlobal.Tablero[D] < PeonN)
				{
					AddMoveNormal(Inicio, D, TipoMov, M);
				}
                if (TableroGlobal.Tablero[D] != CasillaVacia)
				{
					break;
				}
            }
            break;
        default:
            Salir = true;
            break;
	}
}

/* Obtenemos los movimientos del rey */
void ObtenerMovimientoRey(int Inicio, int TipoMov, _ST_Movimiento *M)
{
#ifdef USAR_AJEDREZ960
	int uu = 0;
	int Ok = false;
#endif
	switch (TableroGlobal.MueveBlancas)
	{
		case true:
			if (TipoJuego.Ajedrez960 == false)
			{
				/* Enroque Izquierda */
				if (TableroGlobal.EnroqueB == LosDos || TableroGlobal.EnroqueB == Corto)
				{
					if (Inicio + 3 < 64 && TableroGlobal.Tablero[60] == ReyB && TableroGlobal.Tablero[Inicio + 1] == CasillaVacia && TableroGlobal.Tablero[Inicio + 2] == CasillaVacia && TableroGlobal.Tablero[Inicio + 3] == TorreB)
					{
						if (!Jaque2(Inicio) && !Jaque2(Inicio + 1))
						{
							AddMoveEnroque(Inicio, Inicio + 2, TipoMov, M);
						}
					}
				}
				/* Enroque Derecha */
				if (TableroGlobal.EnroqueB == LosDos || TableroGlobal.EnroqueB == Largo)
				{
					if (Inicio - 4 > -1 && TableroGlobal.Tablero[60] == ReyB && TableroGlobal.Tablero[Inicio - 1] == CasillaVacia && TableroGlobal.Tablero[Inicio - 2] == CasillaVacia && TableroGlobal.Tablero[Inicio - 3] == CasillaVacia && TableroGlobal.Tablero[Inicio - 4] == TorreB)
					{
						if (!Jaque2(Inicio) && !Jaque2(Inicio - 1))
						{
							AddMoveEnroque(Inicio, Inicio - 2, TipoMov, M);
						}
					}
				}
			}
#ifdef USAR_AJEDREZ960
			if (TipoJuego.Ajedrez960 == true)
			{
				/* Enroque corto */
				if (TableroGlobal.EnroqueB == LosDos || TableroGlobal.EnroqueB == Corto)
				{
					Ok = false;
					if (TableroGlobal.Tablero[Inicio] == ReyB && TableroGlobal.Tablero[TableroGlobal.Ajedrez960.TorreBlancaH] == TorreB)
					{
						for (uu = Inicio; uu < 63; uu++)
						{
							if (TableroGlobal.Tablero[uu] == CasillaVacia || TableroGlobal.Tablero[uu] == TorreB || TableroGlobal.Tablero[uu] == ReyB)
							{
								if (TableroGlobal.Tablero[uu] == TorreB && uu != TableroGlobal.Ajedrez960.TorreBlancaH)/* Evitar saltar torre de A */
								{
									Ok = false;
									break;
								}

								if (!Jaque2(uu))
								{
									Ok = true;
								}
								else
								{
									Ok = false;
									break;
								}
							}
							else
							{
								Ok = false;
								break;
							}
						}

						if (Ok == true && Inicio == 62)
						{
							if (TableroGlobal.Tablero[61] == CasillaVacia)
							{
								Ok = true;
							}
							else
							{
								Ok = false;
							}
						}

						if (Ok == true)
						{
							if (TableroGlobal.Tablero[TableroGlobal.Ajedrez960.TorreBlancaH] == TorreB)
								AddMoveEnroque(Inicio, TableroGlobal.Ajedrez960.TorreBlancaH, TipoMov, M);
							else
								Salir = true;
						}
					}
				}
				/* Enroque largo */
				if (TableroGlobal.EnroqueB == LosDos || TableroGlobal.EnroqueB == Largo)
				{
					Ok = false;
					if (TableroGlobal.Tablero[Inicio] == ReyB && TableroGlobal.Tablero[TableroGlobal.Ajedrez960.TorreBlancaA] == TorreB)
					{
						for (uu = Inicio; uu > 57; uu--)
						{
							if (TableroGlobal.Tablero[uu] == CasillaVacia || TableroGlobal.Tablero[uu] == TorreB || TableroGlobal.Tablero[uu] == ReyB)
							{
								if (TableroGlobal.Tablero[uu] == TorreB && uu != TableroGlobal.Ajedrez960.TorreBlancaA)/* Evitar saltar torres de H*/
								{
									Ok = false;
									break;
								}

								if (!Jaque2(uu))
								{
									Ok = true;
								}
								else
								{
									Ok = false;
									break;
								}
							}
							else
							{
								Ok = false;
								break;
							}
						}

						if (Ok == true && Inicio >= 58)
						{
							for (uu = TableroGlobal.Ajedrez960.TorreBlancaA; uu < 60; uu++)
							{
								if (TableroGlobal.Tablero[uu] == CasillaVacia || TableroGlobal.Tablero[uu] == TorreB || TableroGlobal.Tablero[uu] == ReyB)
								{
									if (TableroGlobal.Tablero[uu] == TorreB && uu != TableroGlobal.Ajedrez960.TorreBlancaA)/* Evitar que la torre salte por encima de otra torre propia */
									{
										Ok = false;
										break;
									}

									Ok = true;
								}
								else
								{
									Ok = false;
									break;
								}
							}
						}

						if (Ok == false && Inicio == 57)
						{
							if (TableroGlobal.Tablero[58] == CasillaVacia && TableroGlobal.Tablero[59] == CasillaVacia)
							{
								/* Donde estoy y a donde voy no jaques.*/
								if (!Jaque2(57) && !Jaque2(58))
								{
									Ok = true;
								}
								else
								{
									Ok = false;
								}
							}
							else
							{
								Ok = false;
							}
						}

						if (Ok == true)
						{
							if (TableroGlobal.Tablero[TableroGlobal.Ajedrez960.TorreBlancaA] == TorreB)
								AddMoveEnroque(Inicio, TableroGlobal.Ajedrez960.TorreBlancaA, TipoMov, M);
							else
								Salir = true;
						}
					}
				}
			}
#endif
            /* Bajo */
            if (Inicio + 8 < 64 && TableroGlobal.Tablero[Inicio + 8] > ReyB)
			{
				AddMoveRey(Inicio, Inicio + 8, TipoMov, M);
			}
            /* Arriba */
            if (Inicio - 8 > -1 && TableroGlobal.Tablero[Inicio - 8] > ReyB)
			{
				AddMoveRey(Inicio, Inicio - 8, TipoMov, M);
			}
            /* Derecha */
            if (Inicio + 1 < 64 && TableroGlobal.TableroColor[Inicio] != TableroGlobal.TableroColor[Inicio + 1] && TableroGlobal.Tablero[Inicio + 1] > ReyB)
			{
				AddMoveRey(Inicio, Inicio + 1, TipoMov, M);
			}
			/* Izquierda */
            if (Inicio - 1 > -1 && TableroGlobal.TableroColor[Inicio] != TableroGlobal.TableroColor[Inicio - 1] && TableroGlobal.Tablero[Inicio - 1] > ReyB)
			{
				AddMoveRey(Inicio, Inicio - 1, TipoMov, M);
			}
            /* \ Diagonal arriba Izquierda */
            if (Inicio + 9 < 64 && TableroGlobal.TableroColor[Inicio] == TableroGlobal.TableroColor[Inicio + 9] && TableroGlobal.Tablero[Inicio + 9] > ReyB)
			{
				AddMoveRey(Inicio, Inicio + 9, TipoMov, M);
			}
            /* \ Diagonal Bajo derecha */
            if (Inicio - 9 > -1 && TableroGlobal.TableroColor[Inicio] == TableroGlobal.TableroColor[Inicio - 9] && TableroGlobal.Tablero[Inicio - 9] > ReyB)
			{
				AddMoveRey(Inicio, Inicio - 9, TipoMov, M);
			}
            /* \ Diagonal arriba derecha */
            if (Inicio + 7 < 64 && TableroGlobal.TableroColor[Inicio] == TableroGlobal.TableroColor[Inicio + 7] && TableroGlobal.Tablero[Inicio + 7] > ReyB)
			{
				AddMoveRey(Inicio, Inicio + 7, TipoMov, M);
			}
            /* \ Diagonal arriba izquierda */
            if (Inicio - 7 > -1 && TableroGlobal.TableroColor[Inicio] == TableroGlobal.TableroColor[Inicio - 7] && TableroGlobal.Tablero[Inicio - 7] > ReyB)
			{
				AddMoveRey(Inicio, Inicio - 7, TipoMov, M);
			}

            break;
		case false:
            /* Enroque Izquierda */
			if (TipoJuego.Ajedrez960 == false)
			{
				if (TableroGlobal.EnroqueN == LosDos || TableroGlobal.EnroqueN == Corto)
				{
					if (Inicio + 3 < 64 && TableroGlobal.Tablero[4] == ReyN && TableroGlobal.Tablero[Inicio + 1] == CasillaVacia && TableroGlobal.Tablero[Inicio + 2] == CasillaVacia && TableroGlobal.Tablero[Inicio + 3] == TorreN)
					{
						if (!Jaque2(Inicio) && !Jaque2(Inicio + 1))
						{
							AddMoveEnroque(Inicio, Inicio + 2, TipoMov, M);
						}
					}
				}
				/* Enroque Derecha */
				if (TableroGlobal.EnroqueN == LosDos || TableroGlobal.EnroqueN == Largo)
				{
					if (Inicio - 4 > -1 && TableroGlobal.Tablero[4] == ReyN && TableroGlobal.Tablero[Inicio - 1] == CasillaVacia && TableroGlobal.Tablero[Inicio - 2] == CasillaVacia && TableroGlobal.Tablero[Inicio - 3] == CasillaVacia && TableroGlobal.Tablero[Inicio - 4] == TorreN)
					{
						if (!Jaque2(Inicio) && !Jaque2(Inicio - 1))
						{
							AddMoveEnroque(Inicio, Inicio - 2, TipoMov, M);
						}
					}
				}
			}
#ifdef USAR_AJEDREZ960
			if (TipoJuego.Ajedrez960 == true)
			{
				/* Enroque corto */
				if (TableroGlobal.EnroqueN == LosDos || TableroGlobal.EnroqueN == Corto)
				{
					Ok = false;
					if (TableroGlobal.Tablero[Inicio] == ReyN && TableroGlobal.Tablero[TableroGlobal.Ajedrez960.TorreNegraH] == TorreN)
					{
						for (uu = Inicio; uu < 7; uu++)
						{
							if (TableroGlobal.Tablero[uu] == CasillaVacia || TableroGlobal.Tablero[uu] == TorreN || TableroGlobal.Tablero[uu] == ReyN)
							{
								if (TableroGlobal.Tablero[uu] == TorreN && uu != TableroGlobal.Ajedrez960.TorreNegraH)/* Evitar saltar torre de A */
								{
									Ok = false;
									break;
								}

								if (!Jaque2(uu))
								{
									Ok = true;
								}
								else
								{
									Ok = false;
									break;
								}
							}
							else
							{
								Ok = false;
								break;
							}
						}

						if (Ok == true && Inicio == 6)
						{
							if (TableroGlobal.Tablero[5] == CasillaVacia)
							{
								Ok = true;
							}
							else
							{
								Ok = false;
							}
						}

						if (Ok == true)
						{
							if (TableroGlobal.Tablero[TableroGlobal.Ajedrez960.TorreNegraH] == TorreN)
								AddMoveEnroque(Inicio, TableroGlobal.Ajedrez960.TorreNegraH, TipoMov, M);
							else
								Salir = true;
						}
					}
				}
				/* Enroque largo */
				if (TableroGlobal.EnroqueN == LosDos || TableroGlobal.EnroqueN == Largo)
				{
					Ok = false;
					if (TableroGlobal.Tablero[Inicio] == ReyN && TableroGlobal.Tablero[TableroGlobal.Ajedrez960.TorreNegraA] == TorreN)
					{
						for (uu = Inicio; uu > 1; uu--)
						{
							if (TableroGlobal.Tablero[uu] == CasillaVacia || TableroGlobal.Tablero[uu] == TorreN || TableroGlobal.Tablero[uu] == ReyN)
							{
								if (TableroGlobal.Tablero[uu] == TorreN && uu != TableroGlobal.Ajedrez960.TorreNegraA)/* Evitar saltar torres de H */
								{
									Ok = false;
									break;
								}

								if (!Jaque2(uu))
								{
									Ok = true;
								}
								else
								{
									Ok = false;
									break;
								}
							}
							else
							{
								Ok = false;
								break;
							}
						}

						if (Ok == true && Inicio >= 2)
						{
							for (uu = TableroGlobal.Ajedrez960.TorreNegraA; uu < 4; uu++)
							{
								if (TableroGlobal.Tablero[uu] == CasillaVacia || TableroGlobal.Tablero[uu] == TorreN || TableroGlobal.Tablero[uu] == ReyN)
								{
									if (TableroGlobal.Tablero[uu] == TorreN && uu != TableroGlobal.Ajedrez960.TorreNegraA)/* Evitar que la torre salte por encima de otra torre propia */
									{
										Ok = false;
										break;
									}
									Ok = true;
								}
								else
								{
									Ok = false;
									break;
								}
							}
						}

						if (Ok == false && Inicio == 1)
						{
							if (TableroGlobal.Tablero[2] == CasillaVacia && TableroGlobal.Tablero[3] == CasillaVacia)
							{
								/* Donde estoy y a donde voy no jaques.*/
								if (!Jaque2(1) && !Jaque2(2))
								{
									Ok = true;
								}
								else
								{
									Ok = false;
								}
							}
							else
							{
								Ok = false;
							}
						}

						if (Ok == true)
						{
							if (TableroGlobal.Tablero[TableroGlobal.Ajedrez960.TorreNegraA] == TorreN)
								AddMoveEnroque(Inicio, TableroGlobal.Ajedrez960.TorreNegraA, TipoMov, M);
							else
								Salir = true;
						}
					}
				}
			}
#endif
			/* Arriba */
            if (Inicio + 8 < 64 && TableroGlobal.Tablero[Inicio + 8] < PeonN)
			{
				AddMoveRey(Inicio, Inicio + 8, TipoMov, M);
			}
            /* Bajo */
            if (Inicio - 8 > -1 && TableroGlobal.Tablero[Inicio - 8] < PeonN)
			{
				AddMoveRey(Inicio, Inicio - 8, TipoMov, M);
			}
            /* Izquierda */
            if (Inicio + 1 < 64 && TableroGlobal.TableroColor[Inicio] != TableroGlobal.TableroColor[Inicio + 1] && TableroGlobal.Tablero[Inicio + 1] < PeonN)
			{
				AddMoveRey(Inicio, Inicio + 1, TipoMov, M);
			}
            /* Derecha */
            if (Inicio - 1 > -1 && TableroGlobal.TableroColor[Inicio] != TableroGlobal.TableroColor[Inicio - 1] && TableroGlobal.Tablero[Inicio - 1] < PeonN)
			{
				AddMoveRey(Inicio, Inicio - 1, TipoMov, M);
			}
            /* \ Diagonal arriba Izquierda */
            if (Inicio + 9 < 64 && TableroGlobal.TableroColor[Inicio] == TableroGlobal.TableroColor[Inicio + 9] && TableroGlobal.Tablero[Inicio + 9] < PeonN)
			{
				AddMoveRey(Inicio, Inicio + 9, TipoMov, M);
			}
            /* \ Diagonal Bajo derecha */
            if (Inicio - 9 > -1 && TableroGlobal.TableroColor[Inicio] == TableroGlobal.TableroColor[Inicio - 9] && TableroGlobal.Tablero[Inicio - 9] < PeonN)
			{
				AddMoveRey(Inicio, Inicio - 9, TipoMov, M);
			}
            /* \ Diagonal arriba derecha */
            if (Inicio + 7 < 64 && TableroGlobal.TableroColor[Inicio] == TableroGlobal.TableroColor[Inicio + 7] && TableroGlobal.Tablero[Inicio + 7] < PeonN)
			{
				AddMoveRey(Inicio, Inicio + 7, TipoMov, M);
			}
            /* \ Diagonal arriba izquierda */
            if (Inicio - 7 > -1 && TableroGlobal.TableroColor[Inicio] == TableroGlobal.TableroColor[Inicio - 7] && TableroGlobal.Tablero[Inicio - 7] < PeonN)
			{
				AddMoveRey(Inicio, Inicio - 7, TipoMov, M);
			}

            break;
        default:
            Salir = true;
            break;
	}
}

/* Anadimos el movimiento a la lista, de tipo coronacion */
void AddMoveCoronacion(int Inicio, int Fin, int TipoMov, _ST_Movimiento *M)
{
	/* Generamos todas las posibles coronaciones Caballo, Alfil, Torre y Dama. */
	int p = 0;
	int turno = TableroGlobal.Tablero[Inicio] < CasillaVacia ? true: false;
	int Captura = (TableroGlobal.Tablero[Fin] == CasillaVacia ? MFLAGCAP : TableroGlobal.Tablero[Fin]);

	if (TipoMov == CapturasCoronacion && TableroGlobal.Tablero[Fin] == CasillaVacia)
	{
		return;
	}

	for (p = 0; p < 4; p++)
	{
		switch (turno)
		{
			case true:
				if (p == 0)
				{
					M[IndiceMovimiento++].Movimiento = MOVIMIENTO(Inicio, Fin, TableroGlobal.Tablero[Inicio], Captura, DamaB, 0);
				}
				if (p == 1)
				{
					M[IndiceMovimiento++].Movimiento = MOVIMIENTO(Inicio, Fin, TableroGlobal.Tablero[Inicio], Captura, TorreB, 0);
				}
				if (p == 2)
				{
					M[IndiceMovimiento++].Movimiento = MOVIMIENTO(Inicio, Fin, TableroGlobal.Tablero[Inicio], Captura, AlfilB, 0);
				}
				if (p == 3)
				{
					M[IndiceMovimiento++].Movimiento = MOVIMIENTO(Inicio, Fin, TableroGlobal.Tablero[Inicio], Captura, CaballoB, 0);
				}

                break;
			case false:
				if (p == 0)
				{
					M[IndiceMovimiento++].Movimiento = MOVIMIENTO(Inicio, Fin, TableroGlobal.Tablero[Inicio], Captura, DamaN, 0);
				}
				if (p == 1)
				{
					M[IndiceMovimiento++].Movimiento = MOVIMIENTO(Inicio, Fin, TableroGlobal.Tablero[Inicio], Captura, TorreN, 0);
				}
				if (p == 2)
				{
					M[IndiceMovimiento++].Movimiento = MOVIMIENTO(Inicio, Fin, TableroGlobal.Tablero[Inicio], Captura, AlfilN, 0);
				}
				if (p == 3)
				{
					M[IndiceMovimiento++].Movimiento = MOVIMIENTO(Inicio, Fin, TableroGlobal.Tablero[Inicio], Captura, CaballoN, 0);
				}

				break;
            default:
                Salir = true;
                break;
		}
	}
}

/* Anadimos el movimiento a la lista, de tipo al paso */
void AddMovePosicionFichaAlPaso(int Inicio, int Fin, int TipoMov, _ST_Movimiento *M)
{
	/* Solo Capturas y coronaciones - Lista de jugadas Quies */
	if (TipoMov == CapturasCoronacion)
	{
		return;
	}

	M[IndiceMovimiento++].Movimiento = MOVIMIENTO(Inicio, Fin, TableroGlobal.Tablero[Inicio], MFLAGCAP, MFLAGPROM, MFLAGPS);
}

/* Anadimos el movimiento a la lista, de tipo captura al paso */
void AddMoveCapturaAlPaso(int Inicio, int Fin, _ST_Movimiento *M)
{
	M[IndiceMovimiento++].Movimiento = MOVIMIENTO(Inicio, Fin, TableroGlobal.Tablero[Inicio], MFLAGCAP, MFLAGPROM, MFLAGEP);
}

/* Anadimos el movimiento a la lista, de tipo enroque */
void AddMoveEnroque(int Inicio, int Fin, int TipoMov, _ST_Movimiento *M)
{
	/* Solo Capturas y coronaciones - Lista de jugadas Quies */
	if (TipoMov == CapturasCoronacion)
	{
		return;
	}

	M[IndiceMovimiento++].Movimiento = MOVIMIENTO(Inicio, Fin, TableroGlobal.Tablero[Inicio], MFLAGCAP, MFLAGPROM, MFLAGCA);
}

/* Anadimos el movimiento a la lista, movimiento del rey rey */
void AddMoveRey(int Inicio, int Fin, int TipoMov, _ST_Movimiento *M)
{
	int Captura = (TableroGlobal.Tablero[Fin] == CasillaVacia ? MFLAGCAP : TableroGlobal.Tablero[Fin]);

	/* Solo Capturas y coronaciones - Lista de jugadas Quies */
	if (TipoMov == CapturasCoronacion && TableroGlobal.Tablero[Fin] == CasillaVacia)
	{
		return;
	}

	M[IndiceMovimiento++].Movimiento = MOVIMIENTO(Inicio, Fin, TableroGlobal.Tablero[Inicio], Captura, MFLAGPROM, 0);
}

/* Anadimos el movimiento a la lista, movimientos normales */
void AddMoveNormal(int Inicio, int Fin, int TipoMov, _ST_Movimiento *M)
{
	int Captura = (TableroGlobal.Tablero[Fin] == CasillaVacia ? MFLAGCAP : TableroGlobal.Tablero[Fin]);

	/* Solo Capturas y coronaciones - Lista de jugadas Quies */
	if (TipoMov == CapturasCoronacion && TableroGlobal.Tablero[Fin] == CasillaVacia)
	{
		return;
	}

	M[IndiceMovimiento++].Movimiento = MOVIMIENTO(Inicio, Fin, TableroGlobal.Tablero[Inicio], Captura, MFLAGPROM, 0);
}

/* Obtenemos si si un bando esta en jaque */
int Jaque(int Mueve_Blancas)
{
	switch (Mueve_Blancas)
	{
		case true:
			return JaqueBlancas(TableroGlobal.PosicionReyB);
			break;
		case false:
			return JaqueNegras(TableroGlobal.PosicionReyN);
			break;
        default:
            Salir = true;
            return false;
            break;
	}
}

/* Esta funcion es para comprobar si el enroque esta permitido, comprobando las dos casillas siguiente al rey f1/g1, f8/g8 y c1/d1, c8/d8 */
int Jaque2(int PosicionRey)
{
	switch (TableroGlobal.MueveBlancas)
	{
		case true:
			return JaqueBlancas(PosicionRey);
			break;
		case false:
			return JaqueNegras(PosicionRey);
			break;
        default:
            Salir = true;
            return false;
            break;
	}
}

/* Devuelve true si la posicion del rey esta en jaque (Utilizado en Jaque y Jaque2) */
int JaqueBlancas(int PosicionRey)
{
	int T = 0; /* Vertical */
	int D = 0; /* Diagonal */
	int MargenIzquierda2 = MargenIzquierdo(PosicionRey); /* Margen Izquierda */

	/* PEON */
	if (PosicionRey - 7 > -1 && TableroGlobal.TableroColor[PosicionRey] == TableroGlobal.TableroColor[PosicionRey - 7] && TableroGlobal.Tablero[PosicionRey - 7] == PeonN)
	{
		return true;
	}
	if (PosicionRey - 9 > -1 && TableroGlobal.TableroColor[PosicionRey] == TableroGlobal.TableroColor[PosicionRey - 9] && TableroGlobal.Tablero[PosicionRey - 9] == PeonN)
	{
		return true;
	}

	/* CABALLO */
	if (PosicionRey + 10 < 64 && TableroGlobal.TableroColor[PosicionRey] != TableroGlobal.TableroColor[PosicionRey + 10] && TableroGlobal.Tablero[PosicionRey + 10] == CaballoN)
	{
		return true;
	}
	if (PosicionRey - 6 > -1 && TableroGlobal.TableroColor[PosicionRey] != TableroGlobal.TableroColor[PosicionRey - 6] && TableroGlobal.Tablero[PosicionRey - 6] == CaballoN)
	{
		return true;
	}
	if (PosicionRey + 6 < 64 && TableroGlobal.TableroColor[PosicionRey] != TableroGlobal.TableroColor[PosicionRey + 6] && TableroGlobal.Tablero[PosicionRey + 6] == CaballoN)
	{
		return true;
	}
	if (PosicionRey - 10 > -1 && TableroGlobal.TableroColor[PosicionRey] != TableroGlobal.TableroColor[PosicionRey - 10] && TableroGlobal.Tablero[PosicionRey - 10] == CaballoN)
	{
		return true;
	}
	if (PosicionRey + 17 < 64 && TableroGlobal.TableroColor[PosicionRey] != TableroGlobal.TableroColor[PosicionRey + 17] && TableroGlobal.Tablero[PosicionRey + 17] == CaballoN)
	{
		return true;
	}
	if (PosicionRey + 15 < 64 && TableroGlobal.TableroColor[PosicionRey] != TableroGlobal.TableroColor[PosicionRey + 15] && TableroGlobal.Tablero[PosicionRey + 15] == CaballoN)
	{
		return true;
	}
	if (PosicionRey - 15 > -1 && TableroGlobal.TableroColor[PosicionRey] != TableroGlobal.TableroColor[PosicionRey - 15] && TableroGlobal.Tablero[PosicionRey - 15] == CaballoN)
	{
		return true;
	}
	if (PosicionRey - 17 > -1 && TableroGlobal.TableroColor[PosicionRey] != TableroGlobal.TableroColor[PosicionRey - 17] && TableroGlobal.Tablero[PosicionRey - 17] == CaballoN)
	{
		return true;
	}

	/* ALFIL */
	/* \ Diagonal bajo derecha */
	for (D = PosicionRey + 9; D < 64; D += 9)
	{
		if (TableroGlobal.TableroColor[PosicionRey] != TableroGlobal.TableroColor[D])
		{
			break;
		}
		if (TableroGlobal.Tablero[D] == AlfilN)
		{
			return true;
		}
		if (TableroGlobal.Tablero[D] == DamaN)
		{
			return true;
		}
		if (TableroGlobal.Tablero[D] != CasillaVacia)
		{
			break;
		}
	}

	/* / Diagonal bajo izquierda */
	for (D = PosicionRey + 7; D < 64; D += 7)
	{
		if (TableroGlobal.TableroColor[PosicionRey] != TableroGlobal.TableroColor[D])
		{
			break;
		}
		if (TableroGlobal.Tablero[D] == AlfilN)
		{
			return true;
		}
		if (TableroGlobal.Tablero[D] == DamaN)
		{
			return true;
		}
		if (TableroGlobal.Tablero[D] != CasillaVacia)
		{
			break;
		}
	}

	/* / Diagonal arriba derecha */
	for (D = PosicionRey - 7; D > -1; D -= 7)
	{
		if (TableroGlobal.TableroColor[PosicionRey] != TableroGlobal.TableroColor[D])
		{
			break;
		}
		if (TableroGlobal.Tablero[D] == AlfilN)
		{
			return true;
		}
		if (TableroGlobal.Tablero[D] == DamaN)
		{
			return true;
		}
		if (TableroGlobal.Tablero[D] != CasillaVacia)
		{
			break;
		}
	}

	/* \ Diagonal arriba izquierda */
	for (D = PosicionRey - 9; D > -1; D -= 9)
	{
		if (TableroGlobal.TableroColor[PosicionRey] != TableroGlobal.TableroColor[D])
		{
			break;
		}
		if (TableroGlobal.Tablero[D] == AlfilN)
		{
			return true;
		}
		if (TableroGlobal.Tablero[D] == DamaN)
		{
			return true;
		}
		if (TableroGlobal.Tablero[D] != CasillaVacia)
		{
			break;
		}
	}

	/* TORRE */
	for (T = PosicionRey + 8; T < 64; T += 8)
	{
		if (TableroGlobal.Tablero[T] == TorreN)
		{
			return true;
		}
		if (TableroGlobal.Tablero[T] == DamaN)
		{
			return true;
		}
		if (TableroGlobal.Tablero[T] != CasillaVacia)
		{
			break;
		}
	}
	for (T = PosicionRey - 8; T > -1; T -= 8)
	{
		if (TableroGlobal.Tablero[T] == TorreN)
		{
			return true;
		}
		if (TableroGlobal.Tablero[T] == DamaN)
		{
			return true;
		}
		if (TableroGlobal.Tablero[T] != CasillaVacia)
		{
			break;
		}
	}
	for (T = PosicionRey - 1; T >= MargenIzquierda2; T--)
	{
		if (TableroGlobal.Tablero[T] == TorreN)
		{
			return true;
		}
		if (TableroGlobal.Tablero[T] == DamaN)
		{
			return true;
		}
		if (TableroGlobal.Tablero[T] != CasillaVacia)
		{
			break;
		}
	}
	for (T = PosicionRey + 1; T <= MargenIzquierda2 + 7; T++)
	{
		if (TableroGlobal.Tablero[T] == TorreN)
		{
			return true;
		}
		if (TableroGlobal.Tablero[T] == DamaN)
		{
			return true;
		}
		if (TableroGlobal.Tablero[T] != CasillaVacia)
		{
			break;
		}
	}

	/* REY */
	if (PosicionRey + 8 < 64 && TableroGlobal.Tablero[PosicionRey + 8] == ReyN)
	{
		return true;
	}
	if (PosicionRey - 8 > -1 && TableroGlobal.Tablero[PosicionRey - 8] == ReyN)
	{
		return true;
	}
	if (PosicionRey + 1 < 64 && TableroGlobal.TableroColor[PosicionRey] != TableroGlobal.TableroColor[PosicionRey + 1] && TableroGlobal.Tablero[PosicionRey + 1] == ReyN)
	{
		return true;
	}
	if (PosicionRey - 1 > -1 && TableroGlobal.TableroColor[PosicionRey] != TableroGlobal.TableroColor[PosicionRey - 1] && TableroGlobal.Tablero[PosicionRey - 1] == ReyN)
	{
		return true;
	}
	if (PosicionRey + 9 < 64 && TableroGlobal.TableroColor[PosicionRey] == TableroGlobal.TableroColor[PosicionRey + 9] && TableroGlobal.Tablero[PosicionRey + 9] == ReyN)
	{
		return true;
	}
	if (PosicionRey - 9 > -1 && TableroGlobal.TableroColor[PosicionRey] == TableroGlobal.TableroColor[PosicionRey - 9] && TableroGlobal.Tablero[PosicionRey - 9] == ReyN)
	{
		return true;
	}
	if (PosicionRey + 7 < 64 && TableroGlobal.TableroColor[PosicionRey] == TableroGlobal.TableroColor[PosicionRey + 7] && TableroGlobal.Tablero[PosicionRey + 7] == ReyN)
	{
		return true;
	}
	if (PosicionRey - 7 > -1 && TableroGlobal.TableroColor[PosicionRey] == TableroGlobal.TableroColor[PosicionRey - 7] && TableroGlobal.Tablero[PosicionRey - 7] == ReyN)
	{
		return true;
	}

	return false;
}

/* Devuelve true si la posicion del rey esta en jaque (Utilizado en Jaque y Jaque2) */
int JaqueNegras(int PosicionRey)
{
	int T = 0; /* Vertical */
	int D = 0; /* Diagonal */
	int MargenIzquierda2 = MargenIzquierdo(PosicionRey); /* Margen Izquierda */

	/* PEON */
	if (PosicionRey + 7 < 64 && TableroGlobal.TableroColor[PosicionRey] == TableroGlobal.TableroColor[PosicionRey + 7] && TableroGlobal.Tablero[PosicionRey + 7] == PeonB)
	{
		return true;
	}
	if (PosicionRey + 9 < 64 && TableroGlobal.TableroColor[PosicionRey] == TableroGlobal.TableroColor[PosicionRey + 9] && TableroGlobal.Tablero[PosicionRey + 9] == PeonB)
	{
		return true;
	}

	/* CABALLO */
	if (PosicionRey + 10 < 64 && TableroGlobal.TableroColor[PosicionRey] != TableroGlobal.TableroColor[PosicionRey + 10] && TableroGlobal.Tablero[PosicionRey + 10] == CaballoB)
	{
		return true;
	}
	if (PosicionRey - 6 > -1 && TableroGlobal.TableroColor[PosicionRey] != TableroGlobal.TableroColor[PosicionRey - 6] && TableroGlobal.Tablero[PosicionRey - 6] == CaballoB)
	{
		return true;
	}
	if (PosicionRey + 6 < 64 && TableroGlobal.TableroColor[PosicionRey] != TableroGlobal.TableroColor[PosicionRey + 6] && TableroGlobal.Tablero[PosicionRey + 6] == CaballoB)
	{
		return true;
	}
	if (PosicionRey - 10 > -1 && TableroGlobal.TableroColor[PosicionRey] != TableroGlobal.TableroColor[PosicionRey - 10] && TableroGlobal.Tablero[PosicionRey - 10] == CaballoB)
	{
		return true;
	}
	if (PosicionRey + 17 < 64 && TableroGlobal.TableroColor[PosicionRey] != TableroGlobal.TableroColor[PosicionRey + 17] && TableroGlobal.Tablero[PosicionRey + 17] == CaballoB)
	{
		return true;
	}
	if (PosicionRey + 15 < 64 && TableroGlobal.TableroColor[PosicionRey] != TableroGlobal.TableroColor[PosicionRey + 15] && TableroGlobal.Tablero[PosicionRey + 15] == CaballoB)
	{
		return true;
	}
	if (PosicionRey - 15 > -1 && TableroGlobal.TableroColor[PosicionRey] != TableroGlobal.TableroColor[PosicionRey - 15] && TableroGlobal.Tablero[PosicionRey - 15] == CaballoB)
	{
		return true;
	}
	if (PosicionRey - 17 > -1 && TableroGlobal.TableroColor[PosicionRey] != TableroGlobal.TableroColor[PosicionRey - 17] && TableroGlobal.Tablero[PosicionRey - 17] == CaballoB)
	{
		return true;
	}

	/* ALFIL */
	for (D = PosicionRey + 9; D < 64; D += 9)
	{
		if (TableroGlobal.TableroColor[PosicionRey] != TableroGlobal.TableroColor[D])
		{break;}
		if (TableroGlobal.Tablero[D] == AlfilB)
		{return true;}
		if (TableroGlobal.Tablero[D] == DamaB)
		{return true;}
		if (TableroGlobal.Tablero[D] != CasillaVacia)
		{break;}
	}
	for (D = PosicionRey + 7; D < 64; D += 7)
	{
		if (TableroGlobal.TableroColor[PosicionRey] != TableroGlobal.TableroColor[D])
		{
			break;
		}
		if (TableroGlobal.Tablero[D] == AlfilB)
		{
			return true;
		}
		if (TableroGlobal.Tablero[D] == DamaB)
		{
			return true;
		}
		if (TableroGlobal.Tablero[D] != CasillaVacia)
		{
			break;
		}
	}
	for (D = PosicionRey - 7; D > -1; D -= 7)
	{
		if (TableroGlobal.TableroColor[PosicionRey] != TableroGlobal.TableroColor[D])
		{
			break;
		}
		if (TableroGlobal.Tablero[D] == AlfilB)
		{
			return true;
		}
		if (TableroGlobal.Tablero[D] == DamaB)
		{
			return true;
		}
		if (TableroGlobal.Tablero[D] != CasillaVacia)
		{
			break;
		}
	}
	for (D = PosicionRey - 9; D > -1; D -= 9)
	{
		if (TableroGlobal.TableroColor[PosicionRey] != TableroGlobal.TableroColor[D])
		{
			break;
		}
		if (TableroGlobal.Tablero[D] == AlfilB)
		{
			return true;
		}
		if (TableroGlobal.Tablero[D] == DamaB)
		{
			return true;
		}
		if (TableroGlobal.Tablero[D] != CasillaVacia)
		{
			break;
		}
	}

	/* TORRE */
	for (T = PosicionRey + 8; T < 64; T += 8)
	{
		if (TableroGlobal.Tablero[T] == TorreB)
		{
			return true;
		}
		if (TableroGlobal.Tablero[T] == DamaB)
		{
			return true;
		}
		if (TableroGlobal.Tablero[T] != CasillaVacia)
		{
			break;
		}
	}
	for (T = PosicionRey - 8; T > -1; T -= 8)
	{
		if (TableroGlobal.Tablero[T] == TorreB)
		{
			return true;
		}
		if (TableroGlobal.Tablero[T] == DamaB)
		{
			return true;
		}
		if (TableroGlobal.Tablero[T] != CasillaVacia)
		{
			break;
		}
	}
	for (T = PosicionRey - 1; T >= MargenIzquierda2; T--)
	{
		if (TableroGlobal.Tablero[T] == TorreB)
		{
			return true;
		}
		if (TableroGlobal.Tablero[T] == DamaB)
		{
			return true;
		}
		if (TableroGlobal.Tablero[T] != CasillaVacia)
		{
			break;
		}
	}

	for (T = PosicionRey + 1; T <= MargenIzquierda2 + 7; T++)
	{
		if (TableroGlobal.Tablero[T] == TorreB)
		{
			return true;
		}
		if (TableroGlobal.Tablero[T] == DamaB)
		{
			return true;
		}
		if (TableroGlobal.Tablero[T] != CasillaVacia)
		{
			break;
		}
	}

	/* REY */
	if (PosicionRey + 8 < 64 && TableroGlobal.Tablero[PosicionRey + 8] == ReyB)
	{
		return true;
	}
	if (PosicionRey - 8 > -1 && TableroGlobal.Tablero[PosicionRey - 8] == ReyB)
	{
		return true;
	}
	if (PosicionRey + 1 < 64 && TableroGlobal.TableroColor[PosicionRey] != TableroGlobal.TableroColor[PosicionRey + 1] && TableroGlobal.Tablero[PosicionRey + 1] == ReyB)
	{
		return true;
	}
	if (PosicionRey - 1 > -1 && TableroGlobal.TableroColor[PosicionRey] != TableroGlobal.TableroColor[PosicionRey - 1] && TableroGlobal.Tablero[PosicionRey - 1] == ReyB)
	{
		return true;
	}
	if (PosicionRey + 9 < 64 && TableroGlobal.TableroColor[PosicionRey] == TableroGlobal.TableroColor[PosicionRey + 9] && TableroGlobal.Tablero[PosicionRey + 9] == ReyB)
	{
		return true;
	}
	if (PosicionRey - 9 > -1 && TableroGlobal.TableroColor[PosicionRey] == TableroGlobal.TableroColor[PosicionRey - 9] && TableroGlobal.Tablero[PosicionRey - 9] == ReyB)
	{
		return true;
	}
	if (PosicionRey + 7 < 64 && TableroGlobal.TableroColor[PosicionRey] == TableroGlobal.TableroColor[PosicionRey + 7] && TableroGlobal.Tablero[PosicionRey + 7] == ReyB)
	{
		return true;
	}
	if (PosicionRey - 7 > -1 && TableroGlobal.TableroColor[PosicionRey] == TableroGlobal.TableroColor[PosicionRey - 7] && TableroGlobal.Tablero[PosicionRey - 7] == ReyB)
	{
		return true;
	}

	return false;
}

/* Realizamos el movimiento pasado */
int HacerMovimiento(int M, int Verificar)
{
	int Fin = CUADRADO_DESTINO(M);
	int _PiezaMovida = PIEZAMOVIDA(M);

	/* Almacenamos el estado del tablero, antes de hacer el movimiento. Para podes des-hacerlo despues. */
	#ifdef USAR_HASH_TB
	TableroGlobal.Estado[TableroGlobal.Ply].Hash = TableroGlobal.Hash;
	#endif
	TableroGlobal.Estado[TableroGlobal.Ply].EnroqueB = TableroGlobal.EnroqueB;
	TableroGlobal.Estado[TableroGlobal.Ply].EnroqueN = TableroGlobal.EnroqueN;
	TableroGlobal.Estado[TableroGlobal.Ply].PosicionReyN = TableroGlobal.PosicionReyN;
	TableroGlobal.Estado[TableroGlobal.Ply].PosicionReyB = TableroGlobal.PosicionReyB;
	TableroGlobal.Estado[TableroGlobal.Ply].FichaAlPasoPosicion = TableroGlobal.FichaAlPasoPosicion;
	TableroGlobal.Estado[TableroGlobal.Ply].Movimiento = M;
	TableroGlobal.Estado[TableroGlobal.Ply].Regla_50_Movimiento = TableroGlobal.Regla_50_Movimiento;
#ifdef USAR_HASH_TB
	TableroGlobal.Historico[TableroGlobal.Hply++] = TableroGlobal.Hash;
#endif

	if (GENERA_ALPASO(M) == MFLAGPS)														/* Movimiento peon dos a delante */
	{
#ifdef USAR_HASH_TB
		HASH_EP(Fin);
#endif
		TableroGlobal.FichaAlPasoPosicion = Fin;
	}

	if (_PiezaMovida == PeonB || _PiezaMovida == PeonN || CAPTURADA(M) != MFLAGCAP || CAPTURA_ALPASO(M) == MFLAGEP)
		TableroGlobal.Regla_50_Movimiento = 0;
	else
		TableroGlobal.Regla_50_Movimiento++;

	if (ENROQUE(M) == MFLAGCA)
		HacerMovimientoEnroque(M);
	else if (CAPTURA_ALPASO(M) == MFLAGEP && GENERA_ALPASO(M) != MFLAGPS && TableroGlobal.FichaAlPasoPosicion != 0)
		HacerMovimientoAlPaso(M);
	else if (CORONACION(M) != MFLAGPROM)
		HacerMovimientoCoronacion(M);
	else
		HacerMovimientoNormal(M);

	if (TableroGlobal.FichaAlPasoPosicion != 0 && GENERA_ALPASO(M) != MFLAGPS)
		TableroGlobal.FichaAlPasoPosicion = 0;

#ifdef USAR_HASH_TB
	HASH_LADO;
#endif

	if (TableroGlobal.Ply < (MAX_PLY - 1)) TableroGlobal.Ply++;

	TableroGlobal.MueveBlancas = !TableroGlobal.MueveBlancas;

	if (Verificar == true && Jaque(_PiezaMovida < CasillaVacia ? true : false))
		return false;

	return true;
}

void HacerMovimientoNormal(int M)
{
	int Inicio = CUADRADO_ORIGEN(M);
	int Fin = CUADRADO_DESTINO(M);
	int PiezaCapturada = CAPTURADA(M);

	HacerMovimientoRey(M); /* HASH */

#ifdef USAR_HASH_TB
	/* Quitamos la pieza */
	HASH_PIEZA(TableroGlobal.Tablero[Inicio], Inicio);
	/* Quitamos la pieza capturada */
	if (PiezaCapturada != MFLAGCAP) HASH_PIEZA(PiezaCapturada, Fin);
	/* Anadimos la pieza */
	HASH_PIEZA(TableroGlobal.Tablero[Inicio], Fin);
#endif

	switch (PiezaCapturada)
	{
	case PeonB:
		Blancas.PeonTotales--;
		break;
	case CaballoB:
		Blancas.CaballosTotales--;
		break;
	case AlfilB:
		Blancas.AlfilTotales--;
		break;
	case TorreB:
		Blancas.TorresTotales--;
		break;
	case DamaB:
		Blancas.DamasTotales--;
		break;

	case PeonN:
		Negras.PeonTotales--;
		break;
	case CaballoN:
		Negras.CaballosTotales--;
		break;
	case AlfilN:
		Negras.AlfilTotales--;
		break;
	case TorreN:
		Negras.TorresTotales--;
		break;
	case DamaN:
		Negras.DamasTotales--;
		break;

	case MFLAGCAP:
		break;

	default:
		Salir = true;
		break;
	}

	TableroGlobal.Tablero[Fin] = TableroGlobal.Tablero[Inicio];
	TableroGlobal.Tablero[Inicio] = CasillaVacia;
}
void HacerMovimientoEnroque(int M)
{
	int Inicio = CUADRADO_ORIGEN(M);
	int Fin = CUADRADO_DESTINO(M);
	int turno = PIEZAMOVIDA(M) < CasillaVacia ? true : false;

	if (TipoJuego.Ajedrez960 == false)
	{
		if (turno)
		{
			switch (Fin)
			{
			case 62:
#ifdef USAR_HASH_TB
				/* Quitamos la torre */
				HASH_PIEZA(TableroGlobal.Tablero[63], 63);
				/* Ponemos la torre */
				HASH_PIEZA(TableroGlobal.Tablero[63], 61);
				/* Quitamos la rey */
				HASH_PIEZA(TableroGlobal.Tablero[Inicio], Inicio);
				/* Ponemos la rey */
				HASH_PIEZA(TableroGlobal.Tablero[Inicio], Fin);
				/* Ponemos el nuevo enroque */
				HASH_ENROQUE(62 - Enroque);
#endif
				TableroGlobal.Tablero[61] = TableroGlobal.Tablero[63];
				TableroGlobal.Tablero[63] = CasillaVacia;
				TableroGlobal.EnroqueB = Enroque;
				TableroGlobal.Tablero[Fin] = TableroGlobal.Tablero[Inicio];
				TableroGlobal.Tablero[Inicio] = CasillaVacia;
				TableroGlobal.PosicionReyB = Fin;
				break;
			case 58:
#ifdef USAR_HASH_TB
					/* Quitamos la torre */
				HASH_PIEZA(TableroGlobal.Tablero[56], 56);
				/* Ponemos la torre */
				HASH_PIEZA(TableroGlobal.Tablero[56], 59);
				/* Quitamos la rey */
				HASH_PIEZA(TableroGlobal.Tablero[Inicio], Inicio);
				/* Ponemos la rey */
				HASH_PIEZA(TableroGlobal.Tablero[Inicio], Fin);
				/* Ponemos el nuevo enroque */
				HASH_ENROQUE(62 - Enroque);
#endif
				TableroGlobal.Tablero[59] = TableroGlobal.Tablero[56];
				TableroGlobal.Tablero[56] = CasillaVacia;
				TableroGlobal.EnroqueB = Enroque;
				TableroGlobal.Tablero[Fin] = TableroGlobal.Tablero[Inicio];
				TableroGlobal.Tablero[Inicio] = CasillaVacia;
				TableroGlobal.PosicionReyB = Fin;
				break;
			default:
				Salir = true;
				break;
			}
		}
		else
		{
			switch (Fin)
			{
			case 6:
#ifdef USAR_HASH_TB
					/* Quitamos la torre */
				HASH_PIEZA(TableroGlobal.Tablero[7], 7);
				/* Ponemos la torre */
				HASH_PIEZA(TableroGlobal.Tablero[7], 5);
				/* Quitamos la rey */
				HASH_PIEZA(TableroGlobal.Tablero[Inicio], Inicio);
				/* Ponemos la rey */
				HASH_PIEZA(TableroGlobal.Tablero[Inicio], Fin);
				/* Ponemos el nuevo enroque */
				HASH_ENROQUE(1 + Enroque);
#endif
				TableroGlobal.Tablero[5] = TableroGlobal.Tablero[7];
				TableroGlobal.Tablero[7] = CasillaVacia;
				TableroGlobal.EnroqueN = Enroque;
				TableroGlobal.Tablero[Fin] = TableroGlobal.Tablero[Inicio];
				TableroGlobal.Tablero[Inicio] = CasillaVacia;
				TableroGlobal.PosicionReyN = Fin;
				break;
			case 2:
#ifdef USAR_HASH_TB
					/* Quitamos la torre */
				HASH_PIEZA(TableroGlobal.Tablero[0], 0);
				/* Ponemos la torre */
				HASH_PIEZA(TableroGlobal.Tablero[0], 3);
				/* Quitamos la rey */
				HASH_PIEZA(TableroGlobal.Tablero[4], 4);
				/* Ponemos la rey */
				HASH_PIEZA(TableroGlobal.Tablero[4], Fin);
				/* Ponemos el nuevo enroque */
				HASH_ENROQUE(1 + Enroque);
#endif
				TableroGlobal.Tablero[3] = TableroGlobal.Tablero[0];
				TableroGlobal.Tablero[0] = CasillaVacia;
				TableroGlobal.EnroqueN = Enroque;
				TableroGlobal.Tablero[Fin] = TableroGlobal.Tablero[Inicio];
				TableroGlobal.Tablero[Inicio] = CasillaVacia;
				TableroGlobal.PosicionReyN = Fin;
				break;
			default:
				Salir = true;
				break;
			}
		}
	}
#ifdef USAR_AJEDREZ960
	else /* Enroque tipo Ajedrez960 */
	{
		/* Enroque de las blancas */
		if (TableroGlobal.Tablero[Inicio] == ReyB)
		{
			if (TableroGlobal.Tablero[Fin] != TorreB)
			{
				TipoJuego.Interrumpir = true;
				Salir = true;
			}

			if (Inicio < Fin) /* Enroque corto */
			{
#ifdef USAR_HASH_TB
				/* Quitamos la torre */
				HASH_PIEZA(TableroGlobal.Tablero[TableroGlobal.Ajedrez960.TorreBlancaH], TableroGlobal.Ajedrez960.TorreBlancaH);
				/* Ponemos la torre */
				HASH_PIEZA(TableroGlobal.Tablero[TableroGlobal.Ajedrez960.TorreBlancaH], 61);
				/* Quitamos la rey */
				HASH_PIEZA(TableroGlobal.Tablero[Inicio], Inicio);
				/* Ponemos la rey */
				HASH_PIEZA(TableroGlobal.Tablero[Inicio], 62);
				/* Ponemos el nuevo enroque */
				HASH_ENROQUE(62 - Enroque);
#endif
				/* Movimiento de la torre */
				TableroGlobal.Tablero[TableroGlobal.Ajedrez960.TorreBlancaH] = CasillaVacia;											/* Vaciamos la casilla de la torre */
				TableroGlobal.Tablero[Inicio] = CasillaVacia;

				TableroGlobal.Tablero[61] = TorreB;																			/* Cambiamos la torre de posicion */
				TableroGlobal.EnroqueB = Enroque;																			/* Actualizamos el estado del enroque */

				/* Movimiento del rey */
				TableroGlobal.Tablero[62] = ReyB;
				TableroGlobal.PosicionReyB = 62;											/* Actualizamos la posicion del rey */
			}																		/* Enroque Largo - Posicion del rey o donde se ha desplazado. */
			if (Inicio > Fin) /* Enroque largo */
			{
#ifdef USAR_HASH_TB
				/* Quitamos la torre */
				HASH_PIEZA(TableroGlobal.Tablero[TableroGlobal.Ajedrez960.TorreBlancaA], TableroGlobal.Ajedrez960.TorreBlancaA);
				/* Ponemos la torre */
				HASH_PIEZA(TableroGlobal.Tablero[TableroGlobal.Ajedrez960.TorreBlancaA], 59);
				/* Quitamos la rey */
				HASH_PIEZA(TableroGlobal.Tablero[Inicio], Inicio);
				/* Ponemos la rey */
				HASH_PIEZA(TableroGlobal.Tablero[Inicio], 58);
				/* Ponemos el nuevo enroque */
				HASH_ENROQUE(62 - Enroque);
#endif
				/* Movimiento de la torre */
				TableroGlobal.Tablero[TableroGlobal.Ajedrez960.TorreBlancaA] = CasillaVacia;											/* Vaciamos la casilla de la torre */
				TableroGlobal.Tablero[Inicio] = CasillaVacia;

				TableroGlobal.Tablero[59] = TorreB;						/* Cambiamos la torre de posicion */
				TableroGlobal.EnroqueB = Enroque;										/* Actualizamos el estado del enroque */

				/* Movimiento del rey */
				TableroGlobal.Tablero[58] = ReyB;
				TableroGlobal.PosicionReyB = 58;											/* Actualizamos la posicion del rey */
			}
		}
		/* Enroque de las negras */
		if (TableroGlobal.Tablero[Inicio] == ReyN)
		{
			if (TableroGlobal.Tablero[Fin] != TorreN)
			{
				TipoJuego.Interrumpir = true;
				Salir = true;
			}

			if (Inicio < Fin) /* Enroque corto */
			{
#ifdef USAR_HASH_TB
				/* Quitamos la torre */
				HASH_PIEZA(TableroGlobal.Tablero[TableroGlobal.Ajedrez960.TorreNegraH], TableroGlobal.Ajedrez960.TorreNegraH);
				/* Ponemos la torre */
				HASH_PIEZA(TableroGlobal.Tablero[TableroGlobal.Ajedrez960.TorreNegraH], 5);
				/* Quitamos la rey */
				HASH_PIEZA(TableroGlobal.Tablero[Inicio], Inicio);
				/* Ponemos la rey */
				HASH_PIEZA(TableroGlobal.Tablero[Inicio], 6);
				/* Ponemos el nuevo enroque */
				HASH_ENROQUE(1 + Enroque);
#endif
				/* Movimiento de la torre */
				TableroGlobal.Tablero[TableroGlobal.Ajedrez960.TorreNegraH] = CasillaVacia;											/* Vaciamos la casilla de la torre */
				TableroGlobal.Tablero[Inicio] = CasillaVacia;

				TableroGlobal.Tablero[5] = TorreN;						/* Cambiamos la torre de posicion */
				TableroGlobal.EnroqueN = Enroque;										/* Actualizamos el estado del enroque */

				/* Movimiento del rey */
				TableroGlobal.Tablero[6] = ReyN;
				TableroGlobal.PosicionReyN = 6;											/* Actualizamos la posicion del rey */
			}																		/* Enroque Largo - Posicion del rey o donde se ha desplazado. */

			if (Inicio > Fin) /* Enroque largo */
			{
#ifdef USAR_HASH_TB
				/* Quitamos la torre */
				HASH_PIEZA(TableroGlobal.Tablero[TableroGlobal.Ajedrez960.TorreNegraA], TableroGlobal.Ajedrez960.TorreNegraA);
				/* Ponemos la torre */
				HASH_PIEZA(TableroGlobal.Tablero[TableroGlobal.Ajedrez960.TorreNegraA], 3);
				/* Quitamos la rey */
				HASH_PIEZA(TableroGlobal.Tablero[Inicio], Inicio);
				/* Ponemos la rey */
				HASH_PIEZA(TableroGlobal.Tablero[Inicio], 2);
				/* Ponemos el nuevo enroque */
				HASH_ENROQUE(1 + Enroque);
#endif
				/* Movimiento de la torre */
				TableroGlobal.Tablero[TableroGlobal.Ajedrez960.TorreNegraA] = CasillaVacia;											/* Vaciamos la casilla de la torre */
				TableroGlobal.Tablero[Inicio] = CasillaVacia;

				TableroGlobal.Tablero[3] = TorreN;										/* Cambiamos la torre de posicion */
				TableroGlobal.EnroqueN = Enroque;										/* Actualizamos el estado del enroque */

				/* Movimiento del rey */
				TableroGlobal.Tablero[2] = ReyN;
				TableroGlobal.PosicionReyN = 2;											/* Actualizamos la posicion del rey */
			}
		}
	}
#endif
}
void HacerMovimientoAlPaso(int M)
{
	int Inicio = CUADRADO_ORIGEN(M);
	int Fin = CUADRADO_DESTINO(M);
	int turno = PIEZAMOVIDA(M) < CasillaVacia ? true : false;

	if (CAPTURA_ALPASO(M) == MFLAGEP && TableroGlobal.FichaAlPasoPosicion != 0)
	{
#ifdef USAR_HASH_TB
		/* Ponemos ep */
		HASH_EP(TableroGlobal.FichaAlPasoPosicion);
		/* Quitamos la pieza */
		HASH_PIEZA(TableroGlobal.Tablero[Inicio], Inicio);
		/* Ponemos la pieza */
		HASH_PIEZA(TableroGlobal.Tablero[Inicio], Fin);
#endif


		TableroGlobal.Tablero[TableroGlobal.FichaAlPasoPosicion] = CasillaVacia;
		TableroGlobal.FichaAlPasoPosicion = 0;
		(turno) ? Negras.PeonTotales-- : Blancas.PeonTotales--;
		TableroGlobal.Tablero[Fin] = TableroGlobal.Tablero[Inicio];
		TableroGlobal.Tablero[Inicio] = CasillaVacia;
	}
}
void HacerMovimientoCoronacion(int M)
{
	int Inicio = CUADRADO_ORIGEN(M);
	int Fin = CUADRADO_DESTINO(M);
	int turno = PIEZAMOVIDA(M) < CasillaVacia ? true : false;
	int coronacion = CORONACION(M);
	int PiezaCapturada = CAPTURADA(M);

	(turno) ? Blancas.PeonTotales-- : Negras.PeonTotales--;
	switch (PiezaCapturada)
	{
	case PeonB:
		Blancas.PeonTotales--;
		break;
	case CaballoB:
		Blancas.CaballosTotales--;
		break;
	case AlfilB:
		Blancas.AlfilTotales--;
		break;
	case TorreB:
		Blancas.TorresTotales--;
		break;
	case DamaB:
		Blancas.DamasTotales--;
		break;

	case PeonN:
		Negras.PeonTotales--;
		break;
	case CaballoN:
		Negras.CaballosTotales--;
		break;
	case AlfilN:
		Negras.AlfilTotales--;
		break;
	case TorreN:
		Negras.TorresTotales--;
		break;
	case DamaN:
		Negras.DamasTotales--;
		break;

	case MFLAGCAP:
		break;

	default:
		Salir = true;
		break;
	}
	switch (coronacion)
	{
	case CaballoB:
		Blancas.CaballosTotales++;
		break;
	case AlfilB:
		Blancas.AlfilTotales++;
		break;
	case TorreB:
		Blancas.TorresTotales++;
		break;
	case DamaB:
		Blancas.DamasTotales++;
		break;

	case CaballoN:
		Negras.CaballosTotales++;
		break;
	case AlfilN:
		Negras.AlfilTotales++;
		break;
	case TorreN:
		Negras.TorresTotales++;
		break;
	case DamaN:
		Negras.DamasTotales++;
		break;
	default:
		Salir = true;
		break;
	}

	#ifdef USAR_HASH_TB
	/* Quitamos el peon */
	HASH_PIEZA(TableroGlobal.Tablero[Inicio], Inicio);
	/* Quitamos la pieza capturada */
	if (PiezaCapturada != MFLAGCAP) HASH_PIEZA(PiezaCapturada, Fin);
	/* Anadimos la pieza coronada */
	HASH_PIEZA(coronacion, Fin);
#endif

	TableroGlobal.Tablero[Fin] = coronacion;
	TableroGlobal.Tablero[Inicio] = CasillaVacia;
}
void HacerMovimientoRey(int M)
{
	int Inicio = CUADRADO_ORIGEN(M);
	int Fin = CUADRADO_DESTINO(M);
	int EnroqueB = TableroGlobal.EnroqueB;
	int EnroqueN = TableroGlobal.EnroqueN;

	switch (TableroGlobal.Tablero[Inicio])
	{
	case ReyB:
		if (TableroGlobal.EnroqueB > Ninguno) TableroGlobal.EnroqueB = Ninguno;
		TableroGlobal.PosicionReyB = Fin;
		break;
	case ReyN:
		if (TableroGlobal.EnroqueN > Ninguno) TableroGlobal.EnroqueN = Ninguno;
		TableroGlobal.PosicionReyN = Fin;
		break;
	case TorreB:
		if (Inicio == TableroGlobal.Ajedrez960.TorreBlancaH && TableroGlobal.EnroqueB == LosDos)
			TableroGlobal.EnroqueB = Largo;
		if (Inicio == TableroGlobal.Ajedrez960.TorreBlancaH && TableroGlobal.EnroqueB == Corto)
			TableroGlobal.EnroqueB = Ninguno;
		if (Inicio == TableroGlobal.Ajedrez960.TorreBlancaA && TableroGlobal.EnroqueB == LosDos)
			TableroGlobal.EnroqueB = Corto;
		if (Inicio == TableroGlobal.Ajedrez960.TorreBlancaA && TableroGlobal.EnroqueB == Largo)
			TableroGlobal.EnroqueB = Ninguno;
		break;
	case TorreN:
		if (Inicio == TableroGlobal.Ajedrez960.TorreNegraH && TableroGlobal.EnroqueN == LosDos)
			TableroGlobal.EnroqueN = Largo;
		if (Inicio == TableroGlobal.Ajedrez960.TorreNegraH && TableroGlobal.EnroqueN == Corto)
			TableroGlobal.EnroqueN = Ninguno;
		if (Inicio == TableroGlobal.Ajedrez960.TorreNegraA && TableroGlobal.EnroqueN == LosDos)
			TableroGlobal.EnroqueN = Corto;
		if (Inicio == TableroGlobal.Ajedrez960.TorreNegraA && TableroGlobal.EnroqueN == Largo)
			TableroGlobal.EnroqueN = Ninguno;
		break;
	default:
		break;
	}

#ifdef USAR_HASH_TB
	if (EnroqueB != TableroGlobal.EnroqueB)
		HASH_ENROQUE(62 - TableroGlobal.EnroqueB);
	if (EnroqueN != TableroGlobal.EnroqueN)
		HASH_ENROQUE(1 + TableroGlobal.EnroqueB);
#endif

}
/* Deshacemos el movimiento */
void DeshacerMovimiento()
{
	int M = TableroGlobal.Estado[TableroGlobal.Ply - 1].Movimiento;								/* Obtenemos el movimiento realizado anterior mente */
	int Inicio = CUADRADO_ORIGEN(M);															/* Obtenemos Inicio */
	int Fin = CUADRADO_DESTINO(M);																/* Obtenemos Fin */
	int PiezaMovida = PIEZAMOVIDA(M);															/* Obtenemos la pieza movida */
	int PiezaCapturada = CAPTURADA(M);															/* Obtenemos la pieza capturada */
	int turno = PiezaMovida < CasillaVacia ? true: false;										/* Obtenemos turno */

	/* Enroque */
	if (ENROQUE(M) == MFLAGCA)
	{
		if (TipoJuego.Ajedrez960 == false)
		{
			if (turno)
			{
				switch (Fin)
				{
				case 62:
					TableroGlobal.Tablero[63] = TableroGlobal.Tablero[61];
					TableroGlobal.Tablero[61] = CasillaVacia;
					break;
				case 58:
					TableroGlobal.Tablero[56] = TableroGlobal.Tablero[59];
					TableroGlobal.Tablero[59] = CasillaVacia;
					break;
				default:
					Salir = true;
					break;
				}
			}
			else
			{
				switch (Fin)
				{
				case 6:
					TableroGlobal.Tablero[7] = TableroGlobal.Tablero[5];
					TableroGlobal.Tablero[5] = CasillaVacia;
					break;
				case 2:
					TableroGlobal.Tablero[0] = TableroGlobal.Tablero[3];
					TableroGlobal.Tablero[3] = CasillaVacia;
					break;
				default:
					Salir = true;
					break;
				}
			}
		}
#ifdef USAR_AJEDREZ960
		else	/*	Ajedrez960	*/
		{
			if (PiezaMovida == ReyB)
			{
				if (Inicio < Fin)
				{
					TableroGlobal.Tablero[62] = CasillaVacia;
					TableroGlobal.Tablero[61] = CasillaVacia;

					TableroGlobal.Tablero[Inicio] = ReyB;
					TableroGlobal.Tablero[TableroGlobal.Ajedrez960.TorreBlancaH] = TorreB;
				}
				else
				{
					TableroGlobal.Tablero[58] = CasillaVacia;
					TableroGlobal.Tablero[59] = CasillaVacia;

					TableroGlobal.Tablero[Inicio] = ReyB;
					TableroGlobal.Tablero[TableroGlobal.Ajedrez960.TorreBlancaA] = TorreB;
				}
			}
			if (PiezaMovida == ReyN)
			{
				if (Inicio < Fin)
				{
					TableroGlobal.Tablero[6] = CasillaVacia;
					TableroGlobal.Tablero[5] = CasillaVacia;

					TableroGlobal.Tablero[Inicio] = ReyN;
					TableroGlobal.Tablero[TableroGlobal.Ajedrez960.TorreNegraH] = TorreN;
				}
				else
				{
					TableroGlobal.Tablero[2] = CasillaVacia;
					TableroGlobal.Tablero[3] = CasillaVacia;

					TableroGlobal.Tablero[Inicio] = ReyN;
					TableroGlobal.Tablero[TableroGlobal.Ajedrez960.TorreNegraA] = TorreN;
				}
			}
		}
#endif
	}

	/* Deshacemos el movimiento */
	if (TipoJuego.Ajedrez960 == false)
	{
		TableroGlobal.Tablero[Inicio] = TableroGlobal.Tablero[Fin];
		TableroGlobal.Tablero[Fin] = CasillaVacia;
	}
	else
	{
		if (ENROQUE(M) != MFLAGCA)															/* Enroque. Cambiamos la posicion de la torre. */
		{
			TableroGlobal.Tablero[Inicio] = TableroGlobal.Tablero[Fin];
			TableroGlobal.Tablero[Fin] = CasillaVacia;
		}
	}

	/* Capturas*/
	if (CAPTURA_ALPASO(M) == MFLAGEP && TableroGlobal.Estado[TableroGlobal.Ply-1].FichaAlPasoPosicion != 0)	/* Al paso. Restauramos el peon. */
	{
		TableroGlobal.Tablero[TableroGlobal.Estado[TableroGlobal.Ply-1].FichaAlPasoPosicion] = PiezaMovida < CasillaVacia ? PeonN : PeonB;
		(PiezaMovida < CasillaVacia) ? Negras.PeonTotales++ : Blancas.PeonTotales++;								/* Devolvemos el peon a totales */
	}
	else if (PiezaCapturada != MFLAGCAP)																				/* Capturas */
	{
		TableroGlobal.Tablero[Fin] = PiezaCapturada;														/* Restablecemos la pieza capturada */
		switch (PiezaCapturada)																				/* Devolvemos el peon a totales */
		{
			case PeonB:
				Blancas.PeonTotales++;
				break;
			case CaballoB:
				Blancas.CaballosTotales++;
				break;
			case AlfilB:
				Blancas.AlfilTotales++;
				break;
			case TorreB:
				Blancas.TorresTotales++;
				break;
			case DamaB:
				Blancas.DamasTotales++;
				break;

			case PeonN:
				Negras.PeonTotales++;
				break;
			case CaballoN:
				Negras.CaballosTotales++;
				break;
			case AlfilN:
				Negras.AlfilTotales++;
				break;
			case TorreN:
				Negras.TorresTotales++;
				break;
			case DamaN:
				Negras.DamasTotales++;
				break;

			default:
				Salir = true;
				break;
		}
	}

	/* Coronacion */
	if (CORONACION(M) != MFLAGPROM)																					/* Coronacion. Cambiamos la pieza por un peon. */
	{
		TableroGlobal.Tablero[Inicio] = (PiezaMovida < CasillaVacia) ? PeonB : PeonN;								/* Restablecemos el peon */
		(PiezaMovida < CasillaVacia) ? Blancas.PeonTotales++ : Negras.PeonTotales++;								/* Devolvemos el peon a totales */
		switch (CORONACION(M))																						/* Quitamos la pieza coronada de totales */
		{
			case CaballoB:
				Blancas.CaballosTotales--;
				break;
			case AlfilB:
				Blancas.AlfilTotales--;
				break;
			case TorreB:
				Blancas.TorresTotales--;
				break;
			case DamaB:
				Blancas.DamasTotales--;
				break;

			case CaballoN:
				Negras.CaballosTotales--;
				break;
			case AlfilN:
				Negras.AlfilTotales--;
				break;
			case TorreN:
				Negras.TorresTotales--;
				break;
			case DamaN:
				Negras.DamasTotales--;
				break;

			default:
				Salir = true;
				break;
		}

	}

#ifdef USAR_HASH_TB
	TableroGlobal.Hply--;
#endif
	TableroGlobal.Ply--;
	TableroGlobal.MueveBlancas = !TableroGlobal.MueveBlancas;
	/* Restablecemos el estado del tablero. */
	#ifdef USAR_HASH_TB
	TableroGlobal.Hash = TableroGlobal.Estado[TableroGlobal.Ply].Hash;
	#endif
	TableroGlobal.PosicionReyB = TableroGlobal.Estado[TableroGlobal.Ply].PosicionReyB;
	TableroGlobal.PosicionReyN = TableroGlobal.Estado[TableroGlobal.Ply].PosicionReyN;
	TableroGlobal.EnroqueB = TableroGlobal.Estado[TableroGlobal.Ply].EnroqueB;
	TableroGlobal.EnroqueN = TableroGlobal.Estado[TableroGlobal.Ply].EnroqueN;
	TableroGlobal.FichaAlPasoPosicion = TableroGlobal.Estado[TableroGlobal.Ply].FichaAlPasoPosicion;
	TableroGlobal.Regla_50_Movimiento = TableroGlobal.Estado[TableroGlobal.Ply].Regla_50_Movimiento;
}

#ifdef USAR_NULL_MOVE_PRUNING
/* Hacemos movimiento nulo */
void HacerMovimientoNull()
{
	/* Almacenamos el estado del tablero, antes de hacer el movimiento. Para podes des-hacerlo despues. */
	#ifdef USAR_HASH_TB
	TableroGlobal.Estado[TableroGlobal.Ply].Hash = TableroGlobal.Hash;
	#endif
	TableroGlobal.Estado[TableroGlobal.Ply].PosicionReyB = TableroGlobal.PosicionReyB;
	TableroGlobal.Estado[TableroGlobal.Ply].PosicionReyN = TableroGlobal.PosicionReyN;
	TableroGlobal.Estado[TableroGlobal.Ply].EnroqueB = TableroGlobal.EnroqueB;
	TableroGlobal.Estado[TableroGlobal.Ply].EnroqueN = TableroGlobal.EnroqueN;
	TableroGlobal.Estado[TableroGlobal.Ply].FichaAlPasoPosicion = TableroGlobal.FichaAlPasoPosicion;
	TableroGlobal.Estado[TableroGlobal.Ply].Regla_50_Movimiento = TableroGlobal.Regla_50_Movimiento;
	TableroGlobal.Estado[TableroGlobal.Ply].Movimiento = NO_MOVIMIENTO;


#ifdef USAR_HASH_TB
	HASH_LADO;

	if (TableroGlobal.FichaAlPasoPosicion)
	{
		HASH_EP(TableroGlobal.FichaAlPasoPosicion);
		TableroGlobal.FichaAlPasoPosicion = 0;
	}

#endif

	/* Quitamos posibilidad de captura al paso */
	TableroGlobal.Regla_50_Movimiento++;
	TableroGlobal.MueveBlancas = !TableroGlobal.MueveBlancas;
	TableroGlobal.Ply++;

#ifdef USAR_HASH_TB
	TableroGlobal.Historico[TableroGlobal.Hply++] = TableroGlobal.Hash;
#endif
}

/* Deshacemos movimiento nulo */
void DeshacerMovimientoNull()
{
	TableroGlobal.Ply--;

	TableroGlobal.MueveBlancas = !TableroGlobal.MueveBlancas;
#ifdef USAR_HASH_TB
	TableroGlobal.Hply--;
#endif
	#ifdef USAR_HASH_TB
	TableroGlobal.Hash = TableroGlobal.Estado[TableroGlobal.Ply].Hash;
	#endif

	TableroGlobal.PosicionReyB = TableroGlobal.Estado[TableroGlobal.Ply].PosicionReyB;
	TableroGlobal.PosicionReyN = TableroGlobal.Estado[TableroGlobal.Ply].PosicionReyN;
	TableroGlobal.EnroqueB = TableroGlobal.Estado[TableroGlobal.Ply].EnroqueB;
	TableroGlobal.EnroqueN = TableroGlobal.Estado[TableroGlobal.Ply].EnroqueN;
	TableroGlobal.FichaAlPasoPosicion = TableroGlobal.Estado[TableroGlobal.Ply].FichaAlPasoPosicion;
	TableroGlobal.Regla_50_Movimiento = TableroGlobal.Estado[TableroGlobal.Ply].Regla_50_Movimiento;
}

#endif
