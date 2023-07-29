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

#include "GeneradorDeMovimientos.h"

/*******************************************************************************
							Variables
*******************************************************************************/

void GenerarMovimientos(_ST_TableroX64 *Tablero, int TipoMov, _ST_Movimiento *M)
{
    int X = 0;

	M->CantidadDeMovimiento = 0;

	switch (Tablero->MueveBlancas)
	{
		case true:
			for (X = 63; X > -1; X--)
			{
				if (Tablero->Tablero[X] > ReyB)
				{
					continue;
				}

				switch (Tablero->Tablero[X])
				{
					case PeonB:
						ObtenerMovimientoPeon(Tablero, X, TipoMov, M);
						break;

					case CaballoB:
						ObtenerMovimientoCaballo(Tablero, X, TipoMov, M);
						break;

					case AlfilB:
						ObtenerMovimientoAlfil(Tablero, X, TipoMov, M);
						break;

					case TorreB:
						ObtenerMovimientoTorre(Tablero, X, TipoMov, M);
						break;

					case DamaB:
						ObtenerMovimientoTorre(Tablero, X, TipoMov, M);
						ObtenerMovimientoAlfil(Tablero, X, TipoMov, M);
						break;

					case ReyB:
						ObtenerMovimientoRey(Tablero, X, TipoMov, M);
						break;

                    default:
						Salir = true;
                        continue;
                        break;
				}
			}
			break;
		case false:
			for (X = 0; X < 64; X++)
			{
				if (Tablero->Tablero[X] < PeonN)
				{
					continue;
				}

				switch (Tablero->Tablero[X])
				{
					case PeonN:
						ObtenerMovimientoPeon(Tablero, X, TipoMov, M);
						break;

					case CaballoN:
						ObtenerMovimientoCaballo(Tablero, X, TipoMov, M);
						break;

					case AlfilN:
						ObtenerMovimientoAlfil(Tablero, X, TipoMov, M);
						break;

					case TorreN:
						ObtenerMovimientoTorre(Tablero, X, TipoMov, M);
						break;

					case DamaN:
						ObtenerMovimientoTorre(Tablero, X, TipoMov, M);
						ObtenerMovimientoAlfil(Tablero, X, TipoMov, M);
						break;

					case ReyN:
						ObtenerMovimientoRey(Tablero, X, TipoMov, M);
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
}
void ObtenerMovimientoPeon(_ST_TableroX64 *Tablero, int Inicio, int TipoMov, _ST_Movimiento *M)
{
	switch (Tablero->MueveBlancas)
	{
		case true:
            if (Tablero->FichaAlPasoPosicion)
            {
				if (Inicio - 7 > -1 && Inicio + 1 == Tablero->FichaAlPasoPosicion && Tablero->TableroColor[Inicio] == Tablero->TableroColor[Inicio - 7])
				{
					AddMoveCapturaAlPaso(Tablero, Inicio, Inicio - 7, M);
				}
				if (Inicio - 9 > -1 && Inicio - 1 == Tablero->FichaAlPasoPosicion && Tablero->TableroColor[Inicio] == Tablero->TableroColor[Inicio - 9])
				{
					AddMoveCapturaAlPaso(Tablero, Inicio, Inicio - 9, M);
				}
            }
            if (Inicio - 7 > -1 && Tablero->TableroColor[Inicio] == Tablero->TableroColor[Inicio - 7] && Tablero->Tablero[Inicio - 7] > CasillaVacia)
            {
                if (Inicio - 7 < 8)
				{
					AddMoveCoronacion(Tablero, Inicio, Inicio - 7, TipoMov, M);
				}
				else
				{
					AddMoveNormal(Tablero, Inicio, Inicio - 7, TipoMov, M);
				}
            }
            if (Inicio - 9 > -1 && Tablero->TableroColor[Inicio] == Tablero->TableroColor[Inicio - 9] && Tablero->Tablero[Inicio - 9] > CasillaVacia)
            {
                if (Inicio - 9 < 8)
				{
					AddMoveCoronacion(Tablero, Inicio, Inicio - 9, TipoMov, M);
				}
				else
				{
					AddMoveNormal(Tablero, Inicio, Inicio - 9, TipoMov, M);
				}
            }
            if (Inicio - 8 > -1 && Tablero->Tablero[Inicio - 8] == CasillaVacia)
            {
                if (Inicio - 8 < 8)
				{
					AddMoveCoronacion(Tablero, Inicio, Inicio - 8, TipoMov, M);
				}
				else
				{
					AddMoveNormal(Tablero, Inicio, Inicio - 8, TipoMov, M);
				}
            }
            if (Inicio > 47 && Tablero->Tablero[Inicio - 8] == CasillaVacia && Tablero->Tablero[Inicio - 16] == CasillaVacia)
			{
				AddMovePosicionFichaAlPaso(Tablero, Inicio, Inicio - 16, TipoMov, M);
			}

            break;
        case false:
            if (Tablero->FichaAlPasoPosicion)
            {
				if (Inicio + 7 < 64 && Inicio - 1 == Tablero->FichaAlPasoPosicion && Tablero->TableroColor[Inicio] == Tablero->TableroColor[Inicio + 7])
				{
					AddMoveCapturaAlPaso(Tablero, Inicio, Inicio + 7, M);
				}
				if (Inicio + 9 < 64 && Inicio + 1 == Tablero->FichaAlPasoPosicion && Tablero->TableroColor[Inicio] == Tablero->TableroColor[Inicio + 9])
				{
					AddMoveCapturaAlPaso(Tablero, Inicio, Inicio + 9, M);
				}
            }
            if (Inicio + 7 < 64 && Tablero->TableroColor[Inicio] == Tablero->TableroColor[Inicio + 7] && Tablero->Tablero[Inicio + 7] < CasillaVacia)
            {
                if (Inicio + 7 > 55)
				{
					AddMoveCoronacion(Tablero, Inicio, Inicio + 7, TipoMov, M);
				}
				else
				{
					AddMoveNormal(Tablero, Inicio, Inicio + 7, TipoMov, M);
				}
            }
            if (Inicio + 9 < 64 && Tablero->TableroColor[Inicio] == Tablero->TableroColor[Inicio + 9] && Tablero->Tablero[Inicio + 9] < CasillaVacia)
            {
                if (Inicio + 9 > 55)
				{
					AddMoveCoronacion(Tablero, Inicio, Inicio + 9, TipoMov, M);
				}
				else
				{
					AddMoveNormal(Tablero, Inicio, Inicio + 9, TipoMov, M);
				}
            }
            if (Inicio + 8 < 64 && Tablero->Tablero[Inicio + 8] == CasillaVacia)
            {
                if (Inicio + 8 > 55)
				{
					AddMoveCoronacion(Tablero, Inicio, Inicio + 8, TipoMov, M);
				}
				else
				{
					AddMoveNormal(Tablero, Inicio, Inicio + 8, TipoMov, M);
				}
            }
            if (Inicio < 16 && Tablero->Tablero[Inicio + 8] == CasillaVacia && Tablero->Tablero[Inicio + 16] == CasillaVacia)
			{
				AddMovePosicionFichaAlPaso(Tablero, Inicio, Inicio + 16, TipoMov, M);
			}

            break;
        default:
            Salir = true;
            break;
    }
}
void ObtenerMovimientoTorre(_ST_TableroX64 *Tablero, int Inicio, int TipoMov, _ST_Movimiento *M)
{
	int T = 0;
	int MargenIzquierdo2 = Inicio - COLUMNA(Inicio);

    switch (Tablero->MueveBlancas)
	{
		case true:
            for (T = Inicio + 8; T < 64; T += 8)
            {
                if (Tablero->Tablero[T] > ReyB)
				{
					AddMoveNormal(Tablero, Inicio, T, TipoMov, M);
				}
                if (Tablero->Tablero[T] != CasillaVacia)
				{
					break;
				}
            }
            for (T = Inicio - 8; T > -1; T -= 8)
            {
                if (Tablero->Tablero[T] > ReyB)
				{
					AddMoveNormal(Tablero, Inicio, T, TipoMov, M);
				}
                if (Tablero->Tablero[T] != CasillaVacia)
				{
					break;
				}
            }
            for (T = Inicio - 1; T >= MargenIzquierdo2 && T > -1; T--)
            {
                if (Tablero->Tablero[T] > ReyB)
				{
					AddMoveNormal(Tablero, Inicio, T, TipoMov, M);
				}
                if (Tablero->Tablero[T] != CasillaVacia)
				{
					break;
				}
            }
            for (T = Inicio + 1; T <= MargenIzquierdo2 + 7 && T < 64; T++)
            {
                if (Tablero->Tablero[T] > ReyB)
				{
					AddMoveNormal(Tablero, Inicio, T, TipoMov, M);
				}
                if (Tablero->Tablero[T] != CasillaVacia)
				{
					break;
				}
            }

			break;
		case false:
            for (T = Inicio + 8; T < 64; T += 8)
            {
                if (Tablero->Tablero[T] < PeonN)
				{
					AddMoveNormal(Tablero, Inicio, T, TipoMov, M);
				}
                if (Tablero->Tablero[T] != CasillaVacia)
				{
					break;
				}
            }
            for (T = Inicio - 8; T > -1; T -= 8)
            {
                if (Tablero->Tablero[T] < PeonN)
				{
					AddMoveNormal(Tablero, Inicio, T, TipoMov, M);
				}
                if (Tablero->Tablero[T] != CasillaVacia)
				{
					break;
				}
            }
            for (T = Inicio - 1; T >= MargenIzquierdo2; T--)
            {
                if (Tablero->Tablero[T] < PeonN)
				{
					AddMoveNormal(Tablero, Inicio, T, TipoMov, M);
				}
                if (Tablero->Tablero[T] != CasillaVacia)
				{
					break;
				}
            }
            for (T = Inicio + 1; T <= MargenIzquierdo2 + 7; T++)
            {
                if (Tablero->Tablero[T] < PeonN)
				{
					AddMoveNormal(Tablero, Inicio, T, TipoMov, M);
				}
                if (Tablero->Tablero[T] != CasillaVacia)
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
void ObtenerMovimientoCaballo(_ST_TableroX64 *Tablero, int Inicio, int TipoMov, _ST_Movimiento *M)
{
	switch (Tablero->MueveBlancas)
	{
		case true:
            if (Inicio + 10 < 64 && Tablero->TableroColor[Inicio] != Tablero->TableroColor[Inicio + 10] && Tablero->Tablero[Inicio + 10] > ReyB)
			{
				AddMoveNormal(Tablero, Inicio, Inicio + 10, TipoMov, M);
			}
            if (Inicio - 6 > -1 && Tablero->TableroColor[Inicio] != Tablero->TableroColor[Inicio - 6] && Tablero->Tablero[Inicio - 6] > ReyB)
			{
				AddMoveNormal(Tablero, Inicio, Inicio - 6, TipoMov, M);
			}
            if (Inicio + 6 < 64 && Tablero->TableroColor[Inicio] != Tablero->TableroColor[Inicio + 6] && Tablero->Tablero[Inicio + 6] > ReyB)
			{
				AddMoveNormal(Tablero, Inicio, Inicio + 6, TipoMov, M);
			}
            if (Inicio - 10 > -1 && Tablero->TableroColor[Inicio] != Tablero->TableroColor[Inicio - 10] && Tablero->Tablero[Inicio - 10] > ReyB)
			{
				AddMoveNormal(Tablero, Inicio, Inicio - 10, TipoMov, M);
			}
            if (Inicio + 17 < 64 && Tablero->TableroColor[Inicio] != Tablero->TableroColor[Inicio + 17] && Tablero->Tablero[Inicio + 17] > ReyB)
			{
				AddMoveNormal(Tablero, Inicio, Inicio + 17, TipoMov, M);
			}
            if (Inicio + 15 < 64 && Tablero->TableroColor[Inicio] != Tablero->TableroColor[Inicio + 15] && Tablero->Tablero[Inicio + 15] > ReyB)
			{
				AddMoveNormal(Tablero, Inicio, Inicio + 15, TipoMov, M);
			}
            if (Inicio - 15 > -1 && Tablero->TableroColor[Inicio] != Tablero->TableroColor[Inicio - 15] && Tablero->Tablero[Inicio - 15] > ReyB)
			{
				AddMoveNormal(Tablero, Inicio, Inicio - 15, TipoMov, M);
			}
            if (Inicio - 17 > -1 && Tablero->TableroColor[Inicio] != Tablero->TableroColor[Inicio - 17] && Tablero->Tablero[Inicio - 17] > ReyB)
			{
				AddMoveNormal(Tablero, Inicio, Inicio - 17, TipoMov, M);
			}

            break;
		case false:
            if (Inicio + 10 < 64 && Tablero->TableroColor[Inicio] != Tablero->TableroColor[Inicio + 10] && Tablero->Tablero[Inicio + 10] < PeonN)
			{
				AddMoveNormal(Tablero, Inicio, Inicio + 10, TipoMov, M);
			}
            if (Inicio - 6 > -1 && Tablero->TableroColor[Inicio] != Tablero->TableroColor[Inicio - 6] && Tablero->Tablero[Inicio - 6] < PeonN)
			{
				AddMoveNormal(Tablero, Inicio, Inicio - 6, TipoMov, M);
			}
            if (Inicio + 6 < 64 && Tablero->TableroColor[Inicio] != Tablero->TableroColor[Inicio + 6] && Tablero->Tablero[Inicio + 6] < PeonN)
			{
				AddMoveNormal(Tablero, Inicio, Inicio + 6, TipoMov, M);
			}
            if (Inicio - 10 > -1 && Tablero->TableroColor[Inicio] != Tablero->TableroColor[Inicio - 10] && Tablero->Tablero[Inicio - 10] < PeonN)
			{
				AddMoveNormal(Tablero, Inicio, Inicio - 10, TipoMov, M);
			}
            if (Inicio + 17 < 64 && Tablero->TableroColor[Inicio] != Tablero->TableroColor[Inicio + 17] && Tablero->Tablero[Inicio + 17] < PeonN)
			{
				AddMoveNormal(Tablero, Inicio, Inicio + 17, TipoMov, M);
			}
            if (Inicio + 15 < 64 && Tablero->TableroColor[Inicio] != Tablero->TableroColor[Inicio + 15] && Tablero->Tablero[Inicio + 15] < PeonN)
			{
				AddMoveNormal(Tablero, Inicio, Inicio + 15, TipoMov, M);
			}
            if (Inicio - 15 > -1 && Tablero->TableroColor[Inicio] != Tablero->TableroColor[Inicio - 15] && Tablero->Tablero[Inicio - 15] < PeonN)
			{
				AddMoveNormal(Tablero, Inicio, Inicio - 15, TipoMov, M);
			}
            if (Inicio - 17 > -1 && Tablero->TableroColor[Inicio] != Tablero->TableroColor[Inicio - 17] && Tablero->Tablero[Inicio - 17] < PeonN)
			{
				AddMoveNormal(Tablero, Inicio, Inicio - 17, TipoMov, M);
			}

            break;
        default:
            Salir = true;
            break;
	}
}
void ObtenerMovimientoAlfil(_ST_TableroX64 *Tablero, int Inicio, int TipoMov, _ST_Movimiento *M)
{
	int D = 0;
	switch (Tablero->MueveBlancas)
	{
		case true:
            for (D = Inicio + 9; D < 64; D += 9)
            {
                if (Tablero->TableroColor[Inicio] != Tablero->TableroColor[D])
				{
					break;
				}
                if (Tablero->Tablero[D] > ReyB)
				{
					AddMoveNormal(Tablero, Inicio, D, TipoMov, M);
				}
                if (Tablero->Tablero[D] != CasillaVacia)
				{
					break;
				}
            }
            for (D = Inicio + 7; D < 64; D += 7)
            {
                if (Tablero->TableroColor[Inicio] != Tablero->TableroColor[D])
				{
					break;
				}
                if (Tablero->Tablero[D] > ReyB)
				{
					AddMoveNormal(Tablero, Inicio, D, TipoMov, M);
				}
                if (Tablero->Tablero[D] != CasillaVacia)
				{
					break;
				}
            }
            for (D = Inicio - 7; D > -1; D -= 7)
            {
                if (Tablero->TableroColor[Inicio] != Tablero->TableroColor[D])
				{
					break;
				}
                if (Tablero->Tablero[D] > ReyB)
				{
					AddMoveNormal(Tablero, Inicio, D, TipoMov, M);
				}
                if (Tablero->Tablero[D] != CasillaVacia)
				{
					break;
				}
            }
            for (D = Inicio - 9; D > -1; D -= 9)
            {
                if (Tablero->TableroColor[Inicio] != Tablero->TableroColor[D])
				{
					break;
				}
                if (Tablero->Tablero[D] > ReyB)
				{
					AddMoveNormal(Tablero, Inicio, D, TipoMov, M);
				}
                if (Tablero->Tablero[D] != CasillaVacia)
				{
					break;
				}
            }

            break;
    	case false:
            for (D = Inicio + 9; D < 64; D += 9)
            {
                if (Tablero->TableroColor[Inicio] != Tablero->TableroColor[D])
				{
					break;
				}
                if (Tablero->Tablero[D] < PeonN)
				{
					AddMoveNormal(Tablero, Inicio, D, TipoMov, M);
				}
                if (Tablero->Tablero[D] != CasillaVacia)
				{
					break;
				}
            }
            for (D = Inicio + 7; D < 64; D += 7)
            {
                if (Tablero->TableroColor[Inicio] != Tablero->TableroColor[D])
				{
					break;
				}
                if (Tablero->Tablero[D] < PeonN)
				{
					AddMoveNormal(Tablero, Inicio, D, TipoMov, M);
				}
                if (Tablero->Tablero[D] != CasillaVacia)
				{
					break;
				}
            }
            for (D = Inicio - 7; D > -1; D -= 7)
            {
                if (Tablero->TableroColor[Inicio] != Tablero->TableroColor[D])
				{
					break;
				}
                if (Tablero->Tablero[D] < PeonN)
				{
					AddMoveNormal(Tablero, Inicio, D, TipoMov, M);
				}
                if (Tablero->Tablero[D] != CasillaVacia)
				{
					break;
				}
            }
            for (D = Inicio - 9; D > -1; D -= 9)
            {
                if (Tablero->TableroColor[Inicio] != Tablero->TableroColor[D])
				{
					break;
				}
                if (Tablero->Tablero[D] < PeonN)
				{
					AddMoveNormal(Tablero, Inicio, D, TipoMov, M);
				}
                if (Tablero->Tablero[D] != CasillaVacia)
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
void ObtenerMovimientoRey(_ST_TableroX64 *Tablero, int Inicio, int TipoMov, _ST_Movimiento *M)
{
#ifdef USAR_AJEDREZ960
	int uu = 0;
	int Ok = false;
#endif
	switch (Tablero->MueveBlancas)
	{
		case true:
			if (TipoJuego.Ajedrez960 == false)
			{
				if (Tablero->EnroqueB == LosDos || Tablero->EnroqueB == Corto)
				{
					if (Inicio + 3 < 64 && Tablero->Tablero[60] == ReyB && Tablero->Tablero[Inicio + 1] == CasillaVacia && Tablero->Tablero[Inicio + 2] == CasillaVacia && Tablero->Tablero[Inicio + 3] == TorreB)
					{
						if (!Jaque2(Tablero, Inicio) && !Jaque2(Tablero, Inicio + 1))
						{
							AddMoveEnroque(Tablero, Inicio, Inicio + 2, TipoMov, M);
						}
					}
				}
				if (Tablero->EnroqueB == LosDos || Tablero->EnroqueB == Largo)
				{
					if (Inicio - 4 > -1 && Tablero->Tablero[60] == ReyB && Tablero->Tablero[Inicio - 1] == CasillaVacia && Tablero->Tablero[Inicio - 2] == CasillaVacia && Tablero->Tablero[Inicio - 3] == CasillaVacia && Tablero->Tablero[Inicio - 4] == TorreB)
					{
						if (!Jaque2(Tablero, Inicio) && !Jaque2(Tablero, Inicio - 1))
						{
							AddMoveEnroque(Tablero, Inicio, Inicio - 2, TipoMov, M);
						}
					}
				}
			}
#ifdef USAR_AJEDREZ960
			if (TipoJuego.Ajedrez960 == true)
			{
				if (Tablero->EnroqueB == LosDos || Tablero->EnroqueB == Corto)
				{
					Ok = false;
					if (Tablero->Tablero[Inicio] == ReyB && Tablero->Tablero[Tablero->Ajedrez960.TorreBlancaH] == TorreB)
					{
						for (uu = Inicio; uu < 63; uu++)
						{
							if (Tablero->Tablero[uu] == CasillaVacia || Tablero->Tablero[uu] == TorreB || Tablero->Tablero[uu] == ReyB)
							{
								if (Tablero->Tablero[uu] == TorreB && uu != Tablero->Ajedrez960.TorreBlancaH)
								{
									Ok = false;
									break;
								}

								if (!Jaque2(Tablero, uu))
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
							if (Tablero->Tablero[61] == CasillaVacia)
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
							if (Tablero->Tablero[Tablero->Ajedrez960.TorreBlancaH] == TorreB)
								AddMoveEnroque(Tablero, Inicio, Tablero->Ajedrez960.TorreBlancaH, TipoMov, M);
							else
								Salir = true;
						}
					}
				}
				if (Tablero->EnroqueB == LosDos || Tablero->EnroqueB == Largo)
				{
					Ok = false;
					if (Tablero->Tablero[Inicio] == ReyB && Tablero->Tablero[Tablero->Ajedrez960.TorreBlancaA] == TorreB)
					{
						for (uu = Inicio; uu > 57; uu--)
						{
							if (Tablero->Tablero[uu] == CasillaVacia || Tablero->Tablero[uu] == TorreB || Tablero->Tablero[uu] == ReyB)
							{
								if (Tablero->Tablero[uu] == TorreB && uu != Tablero->Ajedrez960.TorreBlancaA)
								{
									Ok = false;
									break;
								}

								if (!Jaque2(Tablero, uu))
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
							for (uu = Tablero->Ajedrez960.TorreBlancaA; uu < 60; uu++)
							{
								if (Tablero->Tablero[uu] == CasillaVacia || Tablero->Tablero[uu] == TorreB || Tablero->Tablero[uu] == ReyB)
								{
									if (Tablero->Tablero[uu] == TorreB && uu != Tablero->Ajedrez960.TorreBlancaA)
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
							if (Tablero->Tablero[58] == CasillaVacia && Tablero->Tablero[59] == CasillaVacia)
							{
								if (!Jaque2(Tablero, 57) && !Jaque2(Tablero, 58))
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
							if (Tablero->Tablero[Tablero->Ajedrez960.TorreBlancaA] == TorreB)
								AddMoveEnroque(Tablero, Inicio, Tablero->Ajedrez960.TorreBlancaA, TipoMov, M);
							else
								Salir = true;
						}
					}
				}
			}
#endif
            if (Inicio + 8 < 64 && Tablero->Tablero[Inicio + 8] > ReyB)
			{
				AddMoveRey(Tablero, Inicio, Inicio + 8, TipoMov, M);
			}
            if (Inicio - 8 > -1 && Tablero->Tablero[Inicio - 8] > ReyB)
			{
				AddMoveRey(Tablero, Inicio, Inicio - 8, TipoMov, M);
			}
            if (Inicio + 1 < 64 && Tablero->TableroColor[Inicio] != Tablero->TableroColor[Inicio + 1] && Tablero->Tablero[Inicio + 1] > ReyB)
			{
				AddMoveRey(Tablero, Inicio, Inicio + 1, TipoMov, M);
			}
            if (Inicio - 1 > -1 && Tablero->TableroColor[Inicio] != Tablero->TableroColor[Inicio - 1] && Tablero->Tablero[Inicio - 1] > ReyB)
			{
				AddMoveRey(Tablero, Inicio, Inicio - 1, TipoMov, M);
			}
            if (Inicio + 9 < 64 && Tablero->TableroColor[Inicio] == Tablero->TableroColor[Inicio + 9] && Tablero->Tablero[Inicio + 9] > ReyB)
			{
				AddMoveRey(Tablero, Inicio, Inicio + 9, TipoMov, M);
			}
            if (Inicio - 9 > -1 && Tablero->TableroColor[Inicio] == Tablero->TableroColor[Inicio - 9] && Tablero->Tablero[Inicio - 9] > ReyB)
			{
				AddMoveRey(Tablero, Inicio, Inicio - 9, TipoMov, M);
			}
            if (Inicio + 7 < 64 && Tablero->TableroColor[Inicio] == Tablero->TableroColor[Inicio + 7] && Tablero->Tablero[Inicio + 7] > ReyB)
			{
				AddMoveRey(Tablero, Inicio, Inicio + 7, TipoMov, M);
			}
            if (Inicio - 7 > -1 && Tablero->TableroColor[Inicio] == Tablero->TableroColor[Inicio - 7] && Tablero->Tablero[Inicio - 7] > ReyB)
			{
				AddMoveRey(Tablero, Inicio, Inicio - 7, TipoMov, M);
			}

            break;
		case false:
			if (TipoJuego.Ajedrez960 == false)
			{
				if (Tablero->EnroqueN == LosDos || Tablero->EnroqueN == Corto)
				{
					if (Inicio + 3 < 64 && Tablero->Tablero[4] == ReyN && Tablero->Tablero[Inicio + 1] == CasillaVacia && Tablero->Tablero[Inicio + 2] == CasillaVacia && Tablero->Tablero[Inicio + 3] == TorreN)
					{
						if (!Jaque2(Tablero, Inicio) && !Jaque2(Tablero, Inicio + 1))
						{
							AddMoveEnroque(Tablero, Inicio, Inicio + 2, TipoMov, M);
						}
					}
				}
				if (Tablero->EnroqueN == LosDos || Tablero->EnroqueN == Largo)
				{
					if (Inicio - 4 > -1 && Tablero->Tablero[4] == ReyN && Tablero->Tablero[Inicio - 1] == CasillaVacia && Tablero->Tablero[Inicio - 2] == CasillaVacia && Tablero->Tablero[Inicio - 3] == CasillaVacia && Tablero->Tablero[Inicio - 4] == TorreN)
					{
						if (!Jaque2(Tablero, Inicio) && !Jaque2(Tablero, Inicio - 1))
						{
							AddMoveEnroque(Tablero, Inicio, Inicio - 2, TipoMov, M);
						}
					}
				}
			}
#ifdef USAR_AJEDREZ960
			if (TipoJuego.Ajedrez960 == true)
			{
				if (Tablero->EnroqueN == LosDos || Tablero->EnroqueN == Corto)
				{
					Ok = false;
					if (Tablero->Tablero[Inicio] == ReyN && Tablero->Tablero[Tablero->Ajedrez960.TorreNegraH] == TorreN)
					{
						for (uu = Inicio; uu < 7; uu++)
						{
							if (Tablero->Tablero[uu] == CasillaVacia || Tablero->Tablero[uu] == TorreN || Tablero->Tablero[uu] == ReyN)
							{
								if (Tablero->Tablero[uu] == TorreN && uu != Tablero->Ajedrez960.TorreNegraH)
								{
									Ok = false;
									break;
								}

								if (!Jaque2(Tablero, uu))
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
							if (Tablero->Tablero[5] == CasillaVacia)
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
							if (Tablero->Tablero[Tablero->Ajedrez960.TorreNegraH] == TorreN)
								AddMoveEnroque(Tablero, Inicio, Tablero->Ajedrez960.TorreNegraH, TipoMov, M);
							else
								Salir = true;
						}
					}
				}
				if (Tablero->EnroqueN == LosDos || Tablero->EnroqueN == Largo)
				{
					Ok = false;
					if (Tablero->Tablero[Inicio] == ReyN && Tablero->Tablero[Tablero->Ajedrez960.TorreNegraA] == TorreN)
					{
						for (uu = Inicio; uu > 1; uu--)
						{
							if (Tablero->Tablero[uu] == CasillaVacia || Tablero->Tablero[uu] == TorreN || Tablero->Tablero[uu] == ReyN)
							{
								if (Tablero->Tablero[uu] == TorreN && uu != Tablero->Ajedrez960.TorreNegraA)
								{
									Ok = false;
									break;
								}

								if (!Jaque2(Tablero, uu))
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
							for (uu = Tablero->Ajedrez960.TorreNegraA; uu < 4; uu++)
							{
								if (Tablero->Tablero[uu] == CasillaVacia || Tablero->Tablero[uu] == TorreN || Tablero->Tablero[uu] == ReyN)
								{
									if (Tablero->Tablero[uu] == TorreN && uu != Tablero->Ajedrez960.TorreNegraA)
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
							if (Tablero->Tablero[2] == CasillaVacia && Tablero->Tablero[3] == CasillaVacia)
							{
								if (!Jaque2(Tablero, 1) && !Jaque2(Tablero, 2))
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
							if (Tablero->Tablero[Tablero->Ajedrez960.TorreNegraA] == TorreN)
								AddMoveEnroque(Tablero, Inicio, Tablero->Ajedrez960.TorreNegraA, TipoMov, M);
							else
								Salir = true;
						}
					}
				}
			}
#endif
            if (Inicio + 8 < 64 && Tablero->Tablero[Inicio + 8] < PeonN)
			{
				AddMoveRey(Tablero, Inicio, Inicio + 8, TipoMov, M);
			}
            if (Inicio - 8 > -1 && Tablero->Tablero[Inicio - 8] < PeonN)
			{
				AddMoveRey(Tablero, Inicio, Inicio - 8, TipoMov, M);
			}
            if (Inicio + 1 < 64 && Tablero->TableroColor[Inicio] != Tablero->TableroColor[Inicio + 1] && Tablero->Tablero[Inicio + 1] < PeonN)
			{
				AddMoveRey(Tablero, Inicio, Inicio + 1, TipoMov, M);
			}
            if (Inicio - 1 > -1 && Tablero->TableroColor[Inicio] != Tablero->TableroColor[Inicio - 1] && Tablero->Tablero[Inicio - 1] < PeonN)
			{
				AddMoveRey(Tablero, Inicio, Inicio - 1, TipoMov, M);
			}
            if (Inicio + 9 < 64 && Tablero->TableroColor[Inicio] == Tablero->TableroColor[Inicio + 9] && Tablero->Tablero[Inicio + 9] < PeonN)
			{
				AddMoveRey(Tablero, Inicio, Inicio + 9, TipoMov, M);
			}
            if (Inicio - 9 > -1 && Tablero->TableroColor[Inicio] == Tablero->TableroColor[Inicio - 9] && Tablero->Tablero[Inicio - 9] < PeonN)
			{
				AddMoveRey(Tablero, Inicio, Inicio - 9, TipoMov, M);
			}
            if (Inicio + 7 < 64 && Tablero->TableroColor[Inicio] == Tablero->TableroColor[Inicio + 7] && Tablero->Tablero[Inicio + 7] < PeonN)
			{
				AddMoveRey(Tablero, Inicio, Inicio + 7, TipoMov, M);
			}
            if (Inicio - 7 > -1 && Tablero->TableroColor[Inicio] == Tablero->TableroColor[Inicio - 7] && Tablero->Tablero[Inicio - 7] < PeonN)
			{
				AddMoveRey(Tablero, Inicio, Inicio - 7, TipoMov, M);
			}

            break;
        default:
            Salir = true;
            break;
	}
}
void AddMoveCoronacion(_ST_TableroX64 *Tablero, int Inicio, int Fin, int TipoMov, _ST_Movimiento *M)
{
	int p = 0;
	int turno = Tablero->Tablero[Inicio] < CasillaVacia ? true: false;
	int Captura = (Tablero->Tablero[Fin] == CasillaVacia ? MFLAGCAP : Tablero->Tablero[Fin]);

	if (TipoMov == QsCapturas && Captura == MFLAGCAP)
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
					M->Movimiento[M->CantidadDeMovimiento++] = MOVIMIENTO(Inicio, Fin, Tablero->Tablero[Inicio], Captura, DamaB, 0);
				}
				if (p == 1)
				{
					M->Movimiento[M->CantidadDeMovimiento++] = MOVIMIENTO(Inicio, Fin, Tablero->Tablero[Inicio], Captura, TorreB, 0);
				}
				if (p == 2)
				{
					M->Movimiento[M->CantidadDeMovimiento++] = MOVIMIENTO(Inicio, Fin, Tablero->Tablero[Inicio], Captura, AlfilB, 0);
				}
				if (p == 3)
				{
					M->Movimiento[M->CantidadDeMovimiento++] = MOVIMIENTO(Inicio, Fin, Tablero->Tablero[Inicio], Captura, CaballoB, 0);
				}

                break;
			case false:
				if (p == 0)
				{
					M->Movimiento[M->CantidadDeMovimiento++] = MOVIMIENTO(Inicio, Fin, Tablero->Tablero[Inicio], Captura, DamaN, 0);
				}
				if (p == 1)
				{
					M->Movimiento[M->CantidadDeMovimiento++] = MOVIMIENTO(Inicio, Fin, Tablero->Tablero[Inicio], Captura, TorreN, 0);
				}
				if (p == 2)
				{
					M->Movimiento[M->CantidadDeMovimiento++] = MOVIMIENTO(Inicio, Fin, Tablero->Tablero[Inicio], Captura, AlfilN, 0);
				}
				if (p == 3)
				{
					M->Movimiento[M->CantidadDeMovimiento++] = MOVIMIENTO(Inicio, Fin, Tablero->Tablero[Inicio], Captura, CaballoN, 0);
				}

				break;
            default:
                Salir = true;
                break;
		}
	}
}
void AddMovePosicionFichaAlPaso(_ST_TableroX64 *Tablero, int Inicio, int Fin, int TipoMov, _ST_Movimiento *M)
{
	if (TipoMov == QsCapturas)
	{
		return;
	}

	M->Movimiento[M->CantidadDeMovimiento++] = MOVIMIENTO(Inicio, Fin, Tablero->Tablero[Inicio], MFLAGCAP, MFLAGPROM, MFLAGPS);
}
void AddMoveCapturaAlPaso(_ST_TableroX64 *Tablero, int Inicio, int Fin, _ST_Movimiento *M)
{
	M->Movimiento[M->CantidadDeMovimiento++] = MOVIMIENTO(Inicio, Fin, Tablero->Tablero[Inicio], MFLAGCAP, MFLAGPROM, MFLAGEP);
}
void AddMoveEnroque(_ST_TableroX64 *Tablero, int Inicio, int Fin, int TipoMov, _ST_Movimiento *M)
{
	if (TipoMov == QsCapturas)
	{
		return;
	}

	M->Movimiento[M->CantidadDeMovimiento++] = MOVIMIENTO(Inicio, Fin, Tablero->Tablero[Inicio], MFLAGCAP, MFLAGPROM, MFLAGCA);
}
void AddMoveRey(_ST_TableroX64 *Tablero, int Inicio, int Fin, int TipoMov, _ST_Movimiento *M)
{
	int Captura = (Tablero->Tablero[Fin] == CasillaVacia ? MFLAGCAP : Tablero->Tablero[Fin]);

	if (TipoMov == QsCapturas && Captura == MFLAGCAP)
	{
		return;
	}

	M->Movimiento[M->CantidadDeMovimiento++] = MOVIMIENTO(Inicio, Fin, Tablero->Tablero[Inicio], Captura, MFLAGPROM, 0);
}
void AddMoveNormal(_ST_TableroX64 *Tablero, int Inicio, int Fin, int TipoMov, _ST_Movimiento *M)
{
	int Captura = (Tablero->Tablero[Fin] == CasillaVacia ? MFLAGCAP : Tablero->Tablero[Fin]);

	if (TipoMov == QsCapturas && Captura == MFLAGCAP)
	{
		return;
	}

	M->Movimiento[M->CantidadDeMovimiento++] = MOVIMIENTO(Inicio, Fin, Tablero->Tablero[Inicio], Captura, MFLAGPROM, 0);
}
int Jaque(_ST_TableroX64 *Tablero, int Mueve_Blancas)
{
	switch (Mueve_Blancas)
	{
		case true:
			return JaqueBlancas(Tablero, Tablero->PosicionReyB);
			break;
		case false:
			return JaqueNegras(Tablero, Tablero->PosicionReyN);
			break;
        default:
            Salir = true;
            return false;
            break;
	}
}
int Jaque2(_ST_TableroX64 *Tablero, int PosicionRey)
{
	switch (Tablero->MueveBlancas)
	{
		case true:
			return JaqueBlancas(Tablero, PosicionRey);
			break;
		case false:
			return JaqueNegras(Tablero, PosicionRey);
			break;
        default:
            Salir = true;
            return false;
            break;
	}
}
int JaqueBlancas(_ST_TableroX64 *Tablero, int PosicionRey)
{
	int T = 0;
	int D = 0;
	int MargenIzquierda2 = PosicionRey - COLUMNA(PosicionRey);

	if (PosicionRey - 7 > -1 && Tablero->TableroColor[PosicionRey] == Tablero->TableroColor[PosicionRey - 7] && Tablero->Tablero[PosicionRey - 7] == PeonN)
	{
		return true;
	}
	if (PosicionRey - 9 > -1 && Tablero->TableroColor[PosicionRey] == Tablero->TableroColor[PosicionRey - 9] && Tablero->Tablero[PosicionRey - 9] == PeonN)
	{
		return true;
	}

	if (PosicionRey + 10 < 64 && Tablero->TableroColor[PosicionRey] != Tablero->TableroColor[PosicionRey + 10] && Tablero->Tablero[PosicionRey + 10] == CaballoN)
	{
		return true;
	}
	if (PosicionRey - 6 > -1 && Tablero->TableroColor[PosicionRey] != Tablero->TableroColor[PosicionRey - 6] && Tablero->Tablero[PosicionRey - 6] == CaballoN)
	{
		return true;
	}
	if (PosicionRey + 6 < 64 && Tablero->TableroColor[PosicionRey] != Tablero->TableroColor[PosicionRey + 6] && Tablero->Tablero[PosicionRey + 6] == CaballoN)
	{
		return true;
	}
	if (PosicionRey - 10 > -1 && Tablero->TableroColor[PosicionRey] != Tablero->TableroColor[PosicionRey - 10] && Tablero->Tablero[PosicionRey - 10] == CaballoN)
	{
		return true;
	}
	if (PosicionRey + 17 < 64 && Tablero->TableroColor[PosicionRey] != Tablero->TableroColor[PosicionRey + 17] && Tablero->Tablero[PosicionRey + 17] == CaballoN)
	{
		return true;
	}
	if (PosicionRey + 15 < 64 && Tablero->TableroColor[PosicionRey] != Tablero->TableroColor[PosicionRey + 15] && Tablero->Tablero[PosicionRey + 15] == CaballoN)
	{
		return true;
	}
	if (PosicionRey - 15 > -1 && Tablero->TableroColor[PosicionRey] != Tablero->TableroColor[PosicionRey - 15] && Tablero->Tablero[PosicionRey - 15] == CaballoN)
	{
		return true;
	}
	if (PosicionRey - 17 > -1 && Tablero->TableroColor[PosicionRey] != Tablero->TableroColor[PosicionRey - 17] && Tablero->Tablero[PosicionRey - 17] == CaballoN)
	{
		return true;
	}

	for (D = PosicionRey + 9; D < 64; D += 9)
	{
		if (Tablero->TableroColor[PosicionRey] != Tablero->TableroColor[D])
		{
			break;
		}
		if (Tablero->Tablero[D] == AlfilN)
		{
			return true;
		}
		if (Tablero->Tablero[D] == DamaN)
		{
			return true;
		}
		if (Tablero->Tablero[D] != CasillaVacia)
		{
			break;
		}
	}

	for (D = PosicionRey + 7; D < 64; D += 7)
	{
		if (Tablero->TableroColor[PosicionRey] != Tablero->TableroColor[D])
		{
			break;
		}
		if (Tablero->Tablero[D] == AlfilN)
		{
			return true;
		}
		if (Tablero->Tablero[D] == DamaN)
		{
			return true;
		}
		if (Tablero->Tablero[D] != CasillaVacia)
		{
			break;
		}
	}

	for (D = PosicionRey - 7; D > -1; D -= 7)
	{
		if (Tablero->TableroColor[PosicionRey] != Tablero->TableroColor[D])
		{
			break;
		}
		if (Tablero->Tablero[D] == AlfilN)
		{
			return true;
		}
		if (Tablero->Tablero[D] == DamaN)
		{
			return true;
		}
		if (Tablero->Tablero[D] != CasillaVacia)
		{
			break;
		}
	}

	for (D = PosicionRey - 9; D > -1; D -= 9)
	{
		if (Tablero->TableroColor[PosicionRey] != Tablero->TableroColor[D])
		{
			break;
		}
		if (Tablero->Tablero[D] == AlfilN)
		{
			return true;
		}
		if (Tablero->Tablero[D] == DamaN)
		{
			return true;
		}
		if (Tablero->Tablero[D] != CasillaVacia)
		{
			break;
		}
	}

	for (T = PosicionRey + 8; T < 64; T += 8)
	{
		if (Tablero->Tablero[T] == TorreN)
		{
			return true;
		}
		if (Tablero->Tablero[T] == DamaN)
		{
			return true;
		}
		if (Tablero->Tablero[T] != CasillaVacia)
		{
			break;
		}
	}
	for (T = PosicionRey - 8; T > -1; T -= 8)
	{
		if (Tablero->Tablero[T] == TorreN)
		{
			return true;
		}
		if (Tablero->Tablero[T] == DamaN)
		{
			return true;
		}
		if (Tablero->Tablero[T] != CasillaVacia)
		{
			break;
		}
	}
	for (T = PosicionRey - 1; T >= MargenIzquierda2; T--)
	{
		if (Tablero->Tablero[T] == TorreN)
		{
			return true;
		}
		if (Tablero->Tablero[T] == DamaN)
		{
			return true;
		}
		if (Tablero->Tablero[T] != CasillaVacia)
		{
			break;
		}
	}
	for (T = PosicionRey + 1; T <= MargenIzquierda2 + 7; T++)
	{
		if (Tablero->Tablero[T] == TorreN)
		{
			return true;
		}
		if (Tablero->Tablero[T] == DamaN)
		{
			return true;
		}
		if (Tablero->Tablero[T] != CasillaVacia)
		{
			break;
		}
	}

	if (PosicionRey + 8 < 64 && Tablero->Tablero[PosicionRey + 8] == ReyN)
	{
		return true;
	}
	if (PosicionRey - 8 > -1 && Tablero->Tablero[PosicionRey - 8] == ReyN)
	{
		return true;
	}
	if (PosicionRey + 1 < 64 && Tablero->TableroColor[PosicionRey] != Tablero->TableroColor[PosicionRey + 1] && Tablero->Tablero[PosicionRey + 1] == ReyN)
	{
		return true;
	}
	if (PosicionRey - 1 > -1 && Tablero->TableroColor[PosicionRey] != Tablero->TableroColor[PosicionRey - 1] && Tablero->Tablero[PosicionRey - 1] == ReyN)
	{
		return true;
	}
	if (PosicionRey + 9 < 64 && Tablero->TableroColor[PosicionRey] == Tablero->TableroColor[PosicionRey + 9] && Tablero->Tablero[PosicionRey + 9] == ReyN)
	{
		return true;
	}
	if (PosicionRey - 9 > -1 && Tablero->TableroColor[PosicionRey] == Tablero->TableroColor[PosicionRey - 9] && Tablero->Tablero[PosicionRey - 9] == ReyN)
	{
		return true;
	}
	if (PosicionRey + 7 < 64 && Tablero->TableroColor[PosicionRey] == Tablero->TableroColor[PosicionRey + 7] && Tablero->Tablero[PosicionRey + 7] == ReyN)
	{
		return true;
	}
	if (PosicionRey - 7 > -1 && Tablero->TableroColor[PosicionRey] == Tablero->TableroColor[PosicionRey - 7] && Tablero->Tablero[PosicionRey - 7] == ReyN)
	{
		return true;
	}

	return false;
}
int JaqueNegras(_ST_TableroX64 *Tablero, int PosicionRey)
{
	int T = 0;
	int D = 0;
	int MargenIzquierda2 = PosicionRey - COLUMNA(PosicionRey);;

	if (PosicionRey + 7 < 64 && Tablero->TableroColor[PosicionRey] == Tablero->TableroColor[PosicionRey + 7] && Tablero->Tablero[PosicionRey + 7] == PeonB)
	{
		return true;
	}
	if (PosicionRey + 9 < 64 && Tablero->TableroColor[PosicionRey] == Tablero->TableroColor[PosicionRey + 9] && Tablero->Tablero[PosicionRey + 9] == PeonB)
	{
		return true;
	}

	if (PosicionRey + 10 < 64 && Tablero->TableroColor[PosicionRey] != Tablero->TableroColor[PosicionRey + 10] && Tablero->Tablero[PosicionRey + 10] == CaballoB)
	{
		return true;
	}
	if (PosicionRey - 6 > -1 && Tablero->TableroColor[PosicionRey] != Tablero->TableroColor[PosicionRey - 6] && Tablero->Tablero[PosicionRey - 6] == CaballoB)
	{
		return true;
	}
	if (PosicionRey + 6 < 64 && Tablero->TableroColor[PosicionRey] != Tablero->TableroColor[PosicionRey + 6] && Tablero->Tablero[PosicionRey + 6] == CaballoB)
	{
		return true;
	}
	if (PosicionRey - 10 > -1 && Tablero->TableroColor[PosicionRey] != Tablero->TableroColor[PosicionRey - 10] && Tablero->Tablero[PosicionRey - 10] == CaballoB)
	{
		return true;
	}
	if (PosicionRey + 17 < 64 && Tablero->TableroColor[PosicionRey] != Tablero->TableroColor[PosicionRey + 17] && Tablero->Tablero[PosicionRey + 17] == CaballoB)
	{
		return true;
	}
	if (PosicionRey + 15 < 64 && Tablero->TableroColor[PosicionRey] != Tablero->TableroColor[PosicionRey + 15] && Tablero->Tablero[PosicionRey + 15] == CaballoB)
	{
		return true;
	}
	if (PosicionRey - 15 > -1 && Tablero->TableroColor[PosicionRey] != Tablero->TableroColor[PosicionRey - 15] && Tablero->Tablero[PosicionRey - 15] == CaballoB)
	{
		return true;
	}
	if (PosicionRey - 17 > -1 && Tablero->TableroColor[PosicionRey] != Tablero->TableroColor[PosicionRey - 17] && Tablero->Tablero[PosicionRey - 17] == CaballoB)
	{
		return true;
	}

	for (D = PosicionRey + 9; D < 64; D += 9)
	{
		if (Tablero->TableroColor[PosicionRey] != Tablero->TableroColor[D])
		{break;}
		if (Tablero->Tablero[D] == AlfilB)
		{return true;}
		if (Tablero->Tablero[D] == DamaB)
		{return true;}
		if (Tablero->Tablero[D] != CasillaVacia)
		{break;}
	}
	for (D = PosicionRey + 7; D < 64; D += 7)
	{
		if (Tablero->TableroColor[PosicionRey] != Tablero->TableroColor[D])
		{
			break;
		}
		if (Tablero->Tablero[D] == AlfilB)
		{
			return true;
		}
		if (Tablero->Tablero[D] == DamaB)
		{
			return true;
		}
		if (Tablero->Tablero[D] != CasillaVacia)
		{
			break;
		}
	}
	for (D = PosicionRey - 7; D > -1; D -= 7)
	{
		if (Tablero->TableroColor[PosicionRey] != Tablero->TableroColor[D])
		{
			break;
		}
		if (Tablero->Tablero[D] == AlfilB)
		{
			return true;
		}
		if (Tablero->Tablero[D] == DamaB)
		{
			return true;
		}
		if (Tablero->Tablero[D] != CasillaVacia)
		{
			break;
		}
	}
	for (D = PosicionRey - 9; D > -1; D -= 9)
	{
		if (Tablero->TableroColor[PosicionRey] != Tablero->TableroColor[D])
		{
			break;
		}
		if (Tablero->Tablero[D] == AlfilB)
		{
			return true;
		}
		if (Tablero->Tablero[D] == DamaB)
		{
			return true;
		}
		if (Tablero->Tablero[D] != CasillaVacia)
		{
			break;
		}
	}

	for (T = PosicionRey + 8; T < 64; T += 8)
	{
		if (Tablero->Tablero[T] == TorreB)
		{
			return true;
		}
		if (Tablero->Tablero[T] == DamaB)
		{
			return true;
		}
		if (Tablero->Tablero[T] != CasillaVacia)
		{
			break;
		}
	}
	for (T = PosicionRey - 8; T > -1; T -= 8)
	{
		if (Tablero->Tablero[T] == TorreB)
		{
			return true;
		}
		if (Tablero->Tablero[T] == DamaB)
		{
			return true;
		}
		if (Tablero->Tablero[T] != CasillaVacia)
		{
			break;
		}
	}
	for (T = PosicionRey - 1; T >= MargenIzquierda2; T--)
	{
		if (Tablero->Tablero[T] == TorreB)
		{
			return true;
		}
		if (Tablero->Tablero[T] == DamaB)
		{
			return true;
		}
		if (Tablero->Tablero[T] != CasillaVacia)
		{
			break;
		}
	}

	for (T = PosicionRey + 1; T <= MargenIzquierda2 + 7; T++)
	{
		if (Tablero->Tablero[T] == TorreB)
		{
			return true;
		}
		if (Tablero->Tablero[T] == DamaB)
		{
			return true;
		}
		if (Tablero->Tablero[T] != CasillaVacia)
		{
			break;
		}
	}

	if (PosicionRey + 8 < 64 && Tablero->Tablero[PosicionRey + 8] == ReyB)
	{
		return true;
	}
	if (PosicionRey - 8 > -1 && Tablero->Tablero[PosicionRey - 8] == ReyB)
	{
		return true;
	}
	if (PosicionRey + 1 < 64 && Tablero->TableroColor[PosicionRey] != Tablero->TableroColor[PosicionRey + 1] && Tablero->Tablero[PosicionRey + 1] == ReyB)
	{
		return true;
	}
	if (PosicionRey - 1 > -1 && Tablero->TableroColor[PosicionRey] != Tablero->TableroColor[PosicionRey - 1] && Tablero->Tablero[PosicionRey - 1] == ReyB)
	{
		return true;
	}
	if (PosicionRey + 9 < 64 && Tablero->TableroColor[PosicionRey] == Tablero->TableroColor[PosicionRey + 9] && Tablero->Tablero[PosicionRey + 9] == ReyB)
	{
		return true;
	}
	if (PosicionRey - 9 > -1 && Tablero->TableroColor[PosicionRey] == Tablero->TableroColor[PosicionRey - 9] && Tablero->Tablero[PosicionRey - 9] == ReyB)
	{
		return true;
	}
	if (PosicionRey + 7 < 64 && Tablero->TableroColor[PosicionRey] == Tablero->TableroColor[PosicionRey + 7] && Tablero->Tablero[PosicionRey + 7] == ReyB)
	{
		return true;
	}
	if (PosicionRey - 7 > -1 && Tablero->TableroColor[PosicionRey] == Tablero->TableroColor[PosicionRey - 7] && Tablero->Tablero[PosicionRey - 7] == ReyB)
	{
		return true;
	}

	return false;
}
int HacerMovimiento(_ST_TableroX64 *Tablero, int *M, int Verificar)
{
	int Fin = CUADRADO_DESTINO(*M);
	int _PiezaMovida = PIEZAMOVIDA(*M);

	Tablero->Estado[Tablero->Ply].Hash = Tablero->Hash;
	Tablero->Estado[Tablero->Ply].EnroqueB = Tablero->EnroqueB;
	Tablero->Estado[Tablero->Ply].EnroqueN = Tablero->EnroqueN;
	Tablero->Estado[Tablero->Ply].PosicionReyN = Tablero->PosicionReyN;
	Tablero->Estado[Tablero->Ply].PosicionReyB = Tablero->PosicionReyB;
	Tablero->Estado[Tablero->Ply].FichaAlPasoPosicion = Tablero->FichaAlPasoPosicion;
	Tablero->Estado[Tablero->Ply].Movimiento = *M;
	Tablero->Estado[Tablero->Ply].Regla_50_Movimiento = Tablero->Regla_50_Movimiento;
	Tablero->Historico_Repeticion[Tablero->Hply++] = Tablero->Hash;

	if (GENERA_ALPASO(*M) == MFLAGPS)
	{
		HASH_EP(Tablero, Fin);
		Tablero->FichaAlPasoPosicion = Fin;
	}

	if (_PiezaMovida == PeonB || _PiezaMovida == PeonN || CAPTURADA(*M) != MFLAGCAP || CAPTURA_ALPASO(*M) == MFLAGEP)
		Tablero->Regla_50_Movimiento = 0;
	else
		Tablero->Regla_50_Movimiento++;

	if (ENROQUE(*M) == MFLAGCA)
		HacerMovimientoEnroque(Tablero, *M);
	else if (CAPTURA_ALPASO(*M) == MFLAGEP && GENERA_ALPASO(*M) != MFLAGPS && Tablero->FichaAlPasoPosicion != 0)
		HacerMovimientoAlPaso(Tablero, *M);
	else if (CORONACION(*M) != MFLAGPROM)
		HacerMovimientoCoronacion(Tablero, *M);
	else
		HacerMovimientoNormal(Tablero, *M);

	if (Tablero->FichaAlPasoPosicion != 0 && GENERA_ALPASO(*M) != MFLAGPS)
		Tablero->FichaAlPasoPosicion = 0;

	HASH_LADO(Tablero);

	if (Tablero->Ply < (MAX_PLY - 1)) Tablero->Ply++;

	Tablero->MueveBlancas = !Tablero->MueveBlancas;

	if (Verificar == true && Jaque(Tablero, _PiezaMovida < CasillaVacia ? true : false))
		return false;

	return true;
}
void HacerMovimientoNormal(_ST_TableroX64 *Tablero, int M)
{
	int Inicio = CUADRADO_ORIGEN(M);
	int Fin = CUADRADO_DESTINO(M);
	int PiezaCapturada = CAPTURADA(M);

	HacerMovimientoRey(Tablero, M);

	HASH_PIEZA(Tablero, Tablero->Tablero[Inicio], Inicio);
	if (PiezaCapturada < NoPieza) HASH_PIEZA(Tablero, PiezaCapturada, Fin);
	HASH_PIEZA(Tablero, Tablero->Tablero[Inicio], Fin);

	switch (PiezaCapturada)
	{
	case PeonB:
		Tablero->Blancas.PeonTotales--;
		break;
	case CaballoB:
		Tablero->Blancas.CaballosTotales--;
		break;
	case AlfilB:
		Tablero->Blancas.AlfilTotales--;
		break;
	case TorreB:
		Tablero->Blancas.TorresTotales--;
		break;
	case DamaB:
		Tablero->Blancas.DamasTotales--;
		break;

	case PeonN:
		Tablero->Negras.PeonTotales--;
		break;
	case CaballoN:
		Tablero->Negras.CaballosTotales--;
		break;
	case AlfilN:
		Tablero->Negras.AlfilTotales--;
		break;
	case TorreN:
		Tablero->Negras.TorresTotales--;
		break;
	case DamaN:
		Tablero->Negras.DamasTotales--;
		break;

	case MFLAGCAP:
		break;

	default:
		Salir = true;
		break;
	}

	Tablero->Tablero[Fin] = Tablero->Tablero[Inicio];
	Tablero->Tablero[Inicio] = CasillaVacia;
}
void HacerMovimientoEnroque(_ST_TableroX64 *Tablero, int M)
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
				HASH_PIEZA(Tablero, Tablero->Tablero[63], 63);
				HASH_PIEZA(Tablero, Tablero->Tablero[63], 61);
				HASH_PIEZA(Tablero, Tablero->Tablero[Inicio], Inicio);
				HASH_PIEZA(Tablero, Tablero->Tablero[Inicio], Fin);
				HASH_ENROQUE(Tablero, 62 - Enroque);
				Tablero->Tablero[61] = Tablero->Tablero[63];
				Tablero->Tablero[63] = CasillaVacia;
				Tablero->EnroqueB = Enroque;
				Tablero->Tablero[Fin] = Tablero->Tablero[Inicio];
				Tablero->Tablero[Inicio] = CasillaVacia;
				Tablero->PosicionReyB = Fin;
				break;
			case 58:
				HASH_PIEZA(Tablero, Tablero->Tablero[56], 56);
				HASH_PIEZA(Tablero, Tablero->Tablero[56], 59);
				HASH_PIEZA(Tablero, Tablero->Tablero[Inicio], Inicio);
				HASH_PIEZA(Tablero, Tablero->Tablero[Inicio], Fin);
				HASH_ENROQUE(Tablero, 62 - Enroque);
				Tablero->Tablero[59] = Tablero->Tablero[56];
				Tablero->Tablero[56] = CasillaVacia;
				Tablero->EnroqueB = Enroque;
				Tablero->Tablero[Fin] = Tablero->Tablero[Inicio];
				Tablero->Tablero[Inicio] = CasillaVacia;
				Tablero->PosicionReyB = Fin;
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
				HASH_PIEZA(Tablero, Tablero->Tablero[7], 7);
				HASH_PIEZA(Tablero, Tablero->Tablero[7], 5);
				HASH_PIEZA(Tablero, Tablero->Tablero[Inicio], Inicio);
				HASH_PIEZA(Tablero, Tablero->Tablero[Inicio], Fin);
				HASH_ENROQUE(Tablero, 1 + Enroque);
				Tablero->Tablero[5] = Tablero->Tablero[7];
				Tablero->Tablero[7] = CasillaVacia;
				Tablero->EnroqueN = Enroque;
				Tablero->Tablero[Fin] = Tablero->Tablero[Inicio];
				Tablero->Tablero[Inicio] = CasillaVacia;
				Tablero->PosicionReyN = Fin;
				break;
			case 2:
				HASH_PIEZA(Tablero, Tablero->Tablero[0], 0);
				HASH_PIEZA(Tablero, Tablero->Tablero[0], 3);
				HASH_PIEZA(Tablero, Tablero->Tablero[4], 4);
				HASH_PIEZA(Tablero, Tablero->Tablero[4], Fin);
				HASH_ENROQUE(Tablero, 1 + Enroque);
				Tablero->Tablero[3] = Tablero->Tablero[0];
				Tablero->Tablero[0] = CasillaVacia;
				Tablero->EnroqueN = Enroque;
				Tablero->Tablero[Fin] = Tablero->Tablero[Inicio];
				Tablero->Tablero[Inicio] = CasillaVacia;
				Tablero->PosicionReyN = Fin;
				break;
			default:
				Salir = true;
				break;
			}
		}
	}
#ifdef USAR_AJEDREZ960
	else
	{
		if (Tablero->Tablero[Inicio] == ReyB)
		{
			if (Inicio < Fin)
			{
				HASH_PIEZA(Tablero, Tablero->Tablero[Tablero->Ajedrez960.TorreBlancaH], Tablero->Ajedrez960.TorreBlancaH);
				HASH_PIEZA(Tablero, Tablero->Tablero[Tablero->Ajedrez960.TorreBlancaH], 61);
				HASH_PIEZA(Tablero, Tablero->Tablero[Inicio], Inicio);
				HASH_PIEZA(Tablero, Tablero->Tablero[Inicio], 62);
				HASH_ENROQUE(Tablero, 62 - Enroque);
				Tablero->Tablero[Tablero->Ajedrez960.TorreBlancaH] = CasillaVacia;
				Tablero->Tablero[Inicio] = CasillaVacia;

				Tablero->Tablero[61] = TorreB;
				Tablero->EnroqueB = Enroque;

				Tablero->Tablero[62] = ReyB;
				Tablero->PosicionReyB = 62;
			}
			if (Inicio > Fin)
			{
				HASH_PIEZA(Tablero, Tablero->Tablero[Tablero->Ajedrez960.TorreBlancaA], Tablero->Ajedrez960.TorreBlancaA);
				HASH_PIEZA(Tablero, Tablero->Tablero[Tablero->Ajedrez960.TorreBlancaA], 59);
				HASH_PIEZA(Tablero, Tablero->Tablero[Inicio], Inicio);
				HASH_PIEZA(Tablero, Tablero->Tablero[Inicio], 58);
				HASH_ENROQUE(Tablero, 62 - Enroque);
				Tablero->Tablero[Tablero->Ajedrez960.TorreBlancaA] = CasillaVacia;
				Tablero->Tablero[Inicio] = CasillaVacia;

				Tablero->Tablero[59] = TorreB;
				Tablero->EnroqueB = Enroque;

				Tablero->Tablero[58] = ReyB;
				Tablero->PosicionReyB = 58;
			}
		}
		if (Tablero->Tablero[Inicio] == ReyN)
		{
			if (Inicio < Fin)
			{
				HASH_PIEZA(Tablero, Tablero->Tablero[Tablero->Ajedrez960.TorreNegraH], Tablero->Ajedrez960.TorreNegraH);
				HASH_PIEZA(Tablero, Tablero->Tablero[Tablero->Ajedrez960.TorreNegraH], 5);
				HASH_PIEZA(Tablero, Tablero->Tablero[Inicio], Inicio);
				HASH_PIEZA(Tablero, Tablero->Tablero[Inicio], 6);
				HASH_ENROQUE(Tablero, 1 + Enroque);
				Tablero->Tablero[Tablero->Ajedrez960.TorreNegraH] = CasillaVacia;
				Tablero->Tablero[Inicio] = CasillaVacia;

				Tablero->Tablero[5] = TorreN;
				Tablero->EnroqueN = Enroque;

				Tablero->Tablero[6] = ReyN;
				Tablero->PosicionReyN = 6;
			}

			if (Inicio > Fin)
			{
				HASH_PIEZA(Tablero, Tablero->Tablero[Tablero->Ajedrez960.TorreNegraA], Tablero->Ajedrez960.TorreNegraA);
				HASH_PIEZA(Tablero, Tablero->Tablero[Tablero->Ajedrez960.TorreNegraA], 3);
				HASH_PIEZA(Tablero, Tablero->Tablero[Inicio], Inicio);
				HASH_PIEZA(Tablero, Tablero->Tablero[Inicio], 2);
				HASH_ENROQUE(Tablero, 1 + Enroque);
				Tablero->Tablero[Tablero->Ajedrez960.TorreNegraA] = CasillaVacia;
				Tablero->Tablero[Inicio] = CasillaVacia;

				Tablero->Tablero[3] = TorreN;
				Tablero->EnroqueN = Enroque;

				Tablero->Tablero[2] = ReyN;
				Tablero->PosicionReyN = 2;
			}
		}
	}
#endif
}
void HacerMovimientoAlPaso(_ST_TableroX64 *Tablero, int M)
{
	int Inicio = CUADRADO_ORIGEN(M);
	int Fin = CUADRADO_DESTINO(M);
	int turno = PIEZAMOVIDA(M) < CasillaVacia ? true : false;

	if (CAPTURA_ALPASO(M) == MFLAGEP && Tablero->FichaAlPasoPosicion != 0)
	{
		HASH_EP(Tablero, Tablero->FichaAlPasoPosicion);
		HASH_PIEZA(Tablero, Tablero->Tablero[Inicio], Inicio);
		HASH_PIEZA(Tablero, Tablero->Tablero[Inicio], Fin);

		Tablero->Tablero[Tablero->FichaAlPasoPosicion] = CasillaVacia;
		Tablero->FichaAlPasoPosicion = 0;
		(turno) ? Tablero->Negras.PeonTotales-- : Tablero->Blancas.PeonTotales--;
		Tablero->Tablero[Fin] = Tablero->Tablero[Inicio];
		Tablero->Tablero[Inicio] = CasillaVacia;
	}
}
void HacerMovimientoCoronacion(_ST_TableroX64 *Tablero, int M)
{
	int Inicio = CUADRADO_ORIGEN(M);
	int Fin = CUADRADO_DESTINO(M);
	int turno = PIEZAMOVIDA(M) < CasillaVacia ? true : false;
	int coronacion = CORONACION(M);
	int PiezaCapturada = CAPTURADA(M);

	(turno) ? Tablero->Blancas.PeonTotales-- : Tablero->Negras.PeonTotales--;
	switch (PiezaCapturada)
	{
	case PeonB:
		Tablero->Blancas.PeonTotales--;
		break;
	case CaballoB:
		Tablero->Blancas.CaballosTotales--;
		break;
	case AlfilB:
		Tablero->Blancas.AlfilTotales--;
		break;
	case TorreB:
		Tablero->Blancas.TorresTotales--;
		break;
	case DamaB:
		Tablero->Blancas.DamasTotales--;
		break;

	case PeonN:
		Tablero->Negras.PeonTotales--;
		break;
	case CaballoN:
		Tablero->Negras.CaballosTotales--;
		break;
	case AlfilN:
		Tablero->Negras.AlfilTotales--;
		break;
	case TorreN:
		Tablero->Negras.TorresTotales--;
		break;
	case DamaN:
		Tablero->Negras.DamasTotales--;
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
		Tablero->Blancas.CaballosTotales++;
		break;
	case AlfilB:
		Tablero->Blancas.AlfilTotales++;
		break;
	case TorreB:
		Tablero->Blancas.TorresTotales++;
		break;
	case DamaB:
		Tablero->Blancas.DamasTotales++;
		break;

	case CaballoN:
		Tablero->Negras.CaballosTotales++;
		break;
	case AlfilN:
		Tablero->Negras.AlfilTotales++;
		break;
	case TorreN:
		Tablero->Negras.TorresTotales++;
		break;
	case DamaN:
		Tablero->Negras.DamasTotales++;
		break;
	default:
		Salir = true;
		break;
	}

	HASH_PIEZA(Tablero, Tablero->Tablero[Inicio], Inicio);
	if (PiezaCapturada < NoPieza) HASH_PIEZA(Tablero, PiezaCapturada, Fin);
	HASH_PIEZA(Tablero, coronacion, Fin);

	Tablero->Tablero[Fin] = coronacion;
	Tablero->Tablero[Inicio] = CasillaVacia;
}
void HacerMovimientoRey(_ST_TableroX64 *Tablero, int M)
{
	int Inicio = CUADRADO_ORIGEN(M);
	int Fin = CUADRADO_DESTINO(M);
	int EnroqueB = Tablero->EnroqueB;
	int EnroqueN = Tablero->EnroqueN;

	switch (Tablero->Tablero[Inicio])
	{
	case ReyB:
		if (Tablero->EnroqueB > Ninguno) Tablero->EnroqueB = Ninguno;
		Tablero->PosicionReyB = Fin;
		break;
	case ReyN:
		if (Tablero->EnroqueN > Ninguno) Tablero->EnroqueN = Ninguno;
		Tablero->PosicionReyN = Fin;
		break;
	case TorreB:
		if (Inicio == Tablero->Ajedrez960.TorreBlancaH && Tablero->EnroqueB == LosDos)
			Tablero->EnroqueB = Largo;
		if (Inicio == Tablero->Ajedrez960.TorreBlancaH && Tablero->EnroqueB == Corto)
			Tablero->EnroqueB = Ninguno;
		if (Inicio == Tablero->Ajedrez960.TorreBlancaA && Tablero->EnroqueB == LosDos)
			Tablero->EnroqueB = Corto;
		if (Inicio == Tablero->Ajedrez960.TorreBlancaA && Tablero->EnroqueB == Largo)
			Tablero->EnroqueB = Ninguno;
		break;
	case TorreN:
		if (Inicio == Tablero->Ajedrez960.TorreNegraH && Tablero->EnroqueN == LosDos)
			Tablero->EnroqueN = Largo;
		if (Inicio == Tablero->Ajedrez960.TorreNegraH && Tablero->EnroqueN == Corto)
			Tablero->EnroqueN = Ninguno;
		if (Inicio == Tablero->Ajedrez960.TorreNegraA && Tablero->EnroqueN == LosDos)
			Tablero->EnroqueN = Corto;
		if (Inicio == Tablero->Ajedrez960.TorreNegraA && Tablero->EnroqueN == Largo)
			Tablero->EnroqueN = Ninguno;
		break;
	default:
		break;
	}

	if (EnroqueB != Tablero->EnroqueB)
		HASH_ENROQUE(Tablero, 62 - Tablero->EnroqueB);
	if (EnroqueN != Tablero->EnroqueN)
		HASH_ENROQUE(Tablero, 1 + Tablero->EnroqueB);
}
void DeshacerMovimiento(_ST_TableroX64 *Tablero)
{
	int M = Tablero->Estado[Tablero->Ply - 1].Movimiento;
	int Inicio = CUADRADO_ORIGEN(M);
	int Fin = CUADRADO_DESTINO(M);
	int PiezaMovida = PIEZAMOVIDA(M);
	int PiezaCapturada = CAPTURADA(M);
	int turno = PiezaMovida < CasillaVacia ? true: false;

	if (ENROQUE(M) == MFLAGCA)
	{
		if (TipoJuego.Ajedrez960 == false)
		{
			if (turno)
			{
				switch (Fin)
				{
				case 62:
					Tablero->Tablero[63] = Tablero->Tablero[61];
					Tablero->Tablero[61] = CasillaVacia;
					break;
				case 58:
					Tablero->Tablero[56] = Tablero->Tablero[59];
					Tablero->Tablero[59] = CasillaVacia;
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
					Tablero->Tablero[7] = Tablero->Tablero[5];
					Tablero->Tablero[5] = CasillaVacia;
					break;
				case 2:
					Tablero->Tablero[0] = Tablero->Tablero[3];
					Tablero->Tablero[3] = CasillaVacia;
					break;
				default:
					Salir = true;
					break;
				}
			}
		}
#ifdef USAR_AJEDREZ960
		else
		{
			if (PiezaMovida == ReyB)
			{
				if (Inicio < Fin)
				{
					Tablero->Tablero[62] = CasillaVacia;
					Tablero->Tablero[61] = CasillaVacia;

					Tablero->Tablero[Inicio] = ReyB;
					Tablero->Tablero[Tablero->Ajedrez960.TorreBlancaH] = TorreB;
				}
				else
				{
					Tablero->Tablero[58] = CasillaVacia;
					Tablero->Tablero[59] = CasillaVacia;

					Tablero->Tablero[Inicio] = ReyB;
					Tablero->Tablero[Tablero->Ajedrez960.TorreBlancaA] = TorreB;
				}
			}
			if (PiezaMovida == ReyN)
			{
				if (Inicio < Fin)
				{
					Tablero->Tablero[6] = CasillaVacia;
					Tablero->Tablero[5] = CasillaVacia;

					Tablero->Tablero[Inicio] = ReyN;
					Tablero->Tablero[Tablero->Ajedrez960.TorreNegraH] = TorreN;
				}
				else
				{
					Tablero->Tablero[2] = CasillaVacia;
					Tablero->Tablero[3] = CasillaVacia;

					Tablero->Tablero[Inicio] = ReyN;
					Tablero->Tablero[Tablero->Ajedrez960.TorreNegraA] = TorreN;
				}
			}
		}
#endif
	}

	if (TipoJuego.Ajedrez960 == false)
	{
		Tablero->Tablero[Inicio] = Tablero->Tablero[Fin];
		Tablero->Tablero[Fin] = CasillaVacia;
	}
	else
	{
		if (ENROQUE(M) != MFLAGCA)
		{
			Tablero->Tablero[Inicio] = Tablero->Tablero[Fin];
			Tablero->Tablero[Fin] = CasillaVacia;
		}
	}

	if (CAPTURA_ALPASO(M) == MFLAGEP && Tablero->Estado[Tablero->Ply-1].FichaAlPasoPosicion != 0)
	{
		Tablero->Tablero[Tablero->Estado[Tablero->Ply-1].FichaAlPasoPosicion] = PiezaMovida < CasillaVacia ? PeonN : PeonB;
		(PiezaMovida < CasillaVacia) ? Tablero->Negras.PeonTotales++ : Tablero->Blancas.PeonTotales++;
	}
	else if (PiezaCapturada < NoPieza)
	{
		Tablero->Tablero[Fin] = PiezaCapturada;
		switch (PiezaCapturada)
		{
			case PeonB:
				Tablero->Blancas.PeonTotales++;
				break;
			case CaballoB:
				Tablero->Blancas.CaballosTotales++;
				break;
			case AlfilB:
				Tablero->Blancas.AlfilTotales++;
				break;
			case TorreB:
				Tablero->Blancas.TorresTotales++;
				break;
			case DamaB:
				Tablero->Blancas.DamasTotales++;
				break;

			case PeonN:
				Tablero->Negras.PeonTotales++;
				break;
			case CaballoN:
				Tablero->Negras.CaballosTotales++;
				break;
			case AlfilN:
				Tablero->Negras.AlfilTotales++;
				break;
			case TorreN:
				Tablero->Negras.TorresTotales++;
				break;
			case DamaN:
				Tablero->Negras.DamasTotales++;
				break;

			default:
				Salir = true;
				break;
		}
	}

	if (CORONACION(M) != MFLAGPROM)
	{
		Tablero->Tablero[Inicio] = (PiezaMovida < CasillaVacia) ? PeonB : PeonN;
		(PiezaMovida < CasillaVacia) ? Tablero->Blancas.PeonTotales++ : Tablero->Negras.PeonTotales++;
		switch (CORONACION(M))
		{
			case CaballoB:
				Tablero->Blancas.CaballosTotales--;
				break;
			case AlfilB:
				Tablero->Blancas.AlfilTotales--;
				break;
			case TorreB:
				Tablero->Blancas.TorresTotales--;
				break;
			case DamaB:
				Tablero->Blancas.DamasTotales--;
				break;

			case CaballoN:
				Tablero->Negras.CaballosTotales--;
				break;
			case AlfilN:
				Tablero->Negras.AlfilTotales--;
				break;
			case TorreN:
				Tablero->Negras.TorresTotales--;
				break;
			case DamaN:
				Tablero->Negras.DamasTotales--;
				break;

			default:
				Salir = true;
				break;
		}

	}

	Tablero->Hply--;
	Tablero->Ply--;
	Tablero->MueveBlancas = !Tablero->MueveBlancas;

	Tablero->Hash = Tablero->Estado[Tablero->Ply].Hash;
	Tablero->PosicionReyB = Tablero->Estado[Tablero->Ply].PosicionReyB;
	Tablero->PosicionReyN = Tablero->Estado[Tablero->Ply].PosicionReyN;
	Tablero->EnroqueB = Tablero->Estado[Tablero->Ply].EnroqueB;
	Tablero->EnroqueN = Tablero->Estado[Tablero->Ply].EnroqueN;
	Tablero->FichaAlPasoPosicion = Tablero->Estado[Tablero->Ply].FichaAlPasoPosicion;
	Tablero->Regla_50_Movimiento = Tablero->Estado[Tablero->Ply].Regla_50_Movimiento;
}
#ifdef USAR_NULL_MOVE_PRUNING
void HacerMovimientoNull(_ST_TableroX64 *Tablero)
{
	Tablero->Estado[Tablero->Ply].Hash = Tablero->Hash;
	Tablero->Estado[Tablero->Ply].PosicionReyB = Tablero->PosicionReyB;
	Tablero->Estado[Tablero->Ply].PosicionReyN = Tablero->PosicionReyN;
	Tablero->Estado[Tablero->Ply].EnroqueB = Tablero->EnroqueB;
	Tablero->Estado[Tablero->Ply].EnroqueN = Tablero->EnroqueN;
	Tablero->Estado[Tablero->Ply].FichaAlPasoPosicion = Tablero->FichaAlPasoPosicion;
	Tablero->Estado[Tablero->Ply].Regla_50_Movimiento = Tablero->Regla_50_Movimiento;
	Tablero->Estado[Tablero->Ply].Movimiento = NO_MOVIMIENTO;
	Tablero->Historico_Repeticion[Tablero->Hply++] = Tablero->Hash;

	HASH_LADO(Tablero);

	if (Tablero->FichaAlPasoPosicion)
	{
		HASH_EP(Tablero, Tablero->FichaAlPasoPosicion);
		Tablero->FichaAlPasoPosicion = 0;
	}

	Tablero->Regla_50_Movimiento++;
	Tablero->MueveBlancas = !Tablero->MueveBlancas;
	Tablero->Ply++;

}
void DeshacerMovimientoNull(_ST_TableroX64 *Tablero)
{
	Tablero->Ply--;
	Tablero->MueveBlancas = !Tablero->MueveBlancas;
	Tablero->Hply--;
	Tablero->Hash = Tablero->Estado[Tablero->Ply].Hash;
	Tablero->PosicionReyB = Tablero->Estado[Tablero->Ply].PosicionReyB;
	Tablero->PosicionReyN = Tablero->Estado[Tablero->Ply].PosicionReyN;
	Tablero->EnroqueB = Tablero->Estado[Tablero->Ply].EnroqueB;
	Tablero->EnroqueN = Tablero->Estado[Tablero->Ply].EnroqueN;
	Tablero->FichaAlPasoPosicion = Tablero->Estado[Tablero->Ply].FichaAlPasoPosicion;
	Tablero->Regla_50_Movimiento = Tablero->Estado[Tablero->Ply].Regla_50_Movimiento;
}
#endif
