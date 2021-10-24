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

#include "Evaluacion.h"

void CargarEvaluacion()
{
	int Cs = 0;

	/* Unimos PST y Valor de la pieza */
	for (Cs = 0; Cs < 64; Cs++) {
		Evaluacion.PST[NEGRAS][PEON][Cs] = PEON_PST[Cs]; SUMA(&Evaluacion.PST[NEGRAS][PEON][Cs], &PEONVALOR);
		Evaluacion.PST[BLANCAS][PEON][ESPEJO[Cs]] = PEON_PST[Cs]; SUMA(&Evaluacion.PST[BLANCAS][PEON][ESPEJO[Cs]], &PEONVALOR);
		Evaluacion.PST[NEGRAS][CABALLO][Cs] = CABALLO_PST[Cs]; SUMA(&Evaluacion.PST[NEGRAS][CABALLO][Cs], &CABALLOVALOR);
		Evaluacion.PST[BLANCAS][CABALLO][ESPEJO[Cs]] = CABALLO_PST[Cs]; SUMA(&Evaluacion.PST[BLANCAS][CABALLO][ESPEJO[Cs]], &CABALLOVALOR);
		Evaluacion.PST[NEGRAS][ALFIL][Cs] = ALFIL_PST[Cs]; SUMA(&Evaluacion.PST[NEGRAS][ALFIL][Cs], &ALFILVALOR);
		Evaluacion.PST[BLANCAS][ALFIL][ESPEJO[Cs]] = ALFIL_PST[Cs]; SUMA(&Evaluacion.PST[BLANCAS][ALFIL][ESPEJO[Cs]], &ALFILVALOR);
		Evaluacion.PST[NEGRAS][TORRE][Cs] = TORRE_PST[Cs]; SUMA(&Evaluacion.PST[NEGRAS][TORRE][Cs], &TORREVALOR);
		Evaluacion.PST[BLANCAS][TORRE][ESPEJO[Cs]] = TORRE_PST[Cs]; SUMA(&Evaluacion.PST[BLANCAS][TORRE][ESPEJO[Cs]], &TORREVALOR);
		Evaluacion.PST[NEGRAS][DAMA][Cs] = DAMA_PST[Cs]; SUMA(&Evaluacion.PST[NEGRAS][DAMA][Cs], &DAMAVALOR);
		Evaluacion.PST[BLANCAS][DAMA][ESPEJO[Cs]] = DAMA_PST[Cs]; SUMA(&Evaluacion.PST[BLANCAS][DAMA][ESPEJO[Cs]], &DAMAVALOR);
		Evaluacion.PST[NEGRAS][REY][Cs] = REY_PST[Cs]; SUMA(&Evaluacion.PST[NEGRAS][REY][Cs], &REYVALOR);
		Evaluacion.PST[BLANCAS][REY][ESPEJO[Cs]] = REY_PST[Cs]; SUMA(&Evaluacion.PST[BLANCAS][REY][ESPEJO[Cs]], &REYVALOR);
	}
}
void SUMA(_Valor *a, _Valor *b)
{
	a->Apertura += b->Apertura;
	a->Final += b->Final;
}
int Evaluar()
{
	int puntos = 0;
	_Valor pGlobal = PuntosCero;

#ifdef USAR_NNUE
	if (Nnue.Dll_Cargada == true && Nnue.Usar == true)
	{
		puntos = ProbarNNUE();

		if (TableroGlobal.Regla_50_Movimiento > 70)
		{
			if (puntos * (101 - TableroGlobal.Regla_50_Movimiento) != 0)
				puntos = puntos * (101 - TableroGlobal.Regla_50_Movimiento) / 30;
			else
				puntos = 0;
		}

		return puntos;
	}
#endif

	/* ETAPA de 24. 24 Es apertura y como baja nos hacercamos a finales */
	TableroGlobal.Etapa = (((Blancas.CaballosTotales + Blancas.AlfilTotales) * 1) + (Blancas.TorresTotales * 2) + (Blancas.DamasTotales * 4) + ((Negras.CaballosTotales + Negras.AlfilTotales) * 1) + (Negras.TorresTotales * 2) + (Negras.DamasTotales * 4));
	/* Evitamos Phase fuera de rango */
	if (TableroGlobal.Etapa > MAX_ETAPA)
		TableroGlobal.Etapa = MAX_ETAPA;
	if (TableroGlobal.Etapa < 0)
		TableroGlobal.Etapa = 0;

	/* Evaluamos material para detectar empate */
	if (TableroGlobal.Etapa <= FIN_ETAPA)
	{
		/* Evaluar material insuficiente modo basico. */
		if (EvaluarTablas())
		{
			return VALOR_EMPATE;
		}
	}

	/* Limpiamos la puntuacion */
	LimpiarPuntuacion(true);

	/* Posicion de las piezas en el tablero */
	EvaluarPuntosPst();

	EvaluarPeones(BLANCAS);
	EvaluarPeones(NEGRAS);
	EvaluarAlFil(BLANCAS);
	EvaluarAlFil(NEGRAS);
	EvaluarTorre(BLANCAS);
	EvaluarTorre(NEGRAS);
	EvaluarSeguridadRey(BLANCAS);
	EvaluarSeguridadRey(NEGRAS);
	/* Pareja de alfiles */
	if (Blancas.AlfilTotales > 1) SUMA(&Blancas.Puntos, &ALFIL_PAREJA);
	if (Negras.AlfilTotales > 1) SUMA(&Negras.Puntos, &ALFIL_PAREJA);

	/* balance de puntos */
	pGlobal.Apertura = Blancas.Puntos.Apertura - Negras.Puntos.Apertura;
	pGlobal.Final = Blancas.Puntos.Final - Negras.Puntos.Final;

	EvaluarComplejidad(&pGlobal);

	/* Interpolar puntos de apertura y final */
	puntos = Interpolar(&pGlobal);

	return Tempo + (TableroGlobal.MueveBlancas ? puntos : -puntos);
}
int EvaluarTablas()
{
	/* Rey contra rey */
	if (!(Blancas.CaballosTotales + Blancas.AlfilTotales + Blancas.TorresTotales + Blancas.DamasTotales + Blancas.PeonTotales) &&
		!(Negras.CaballosTotales + Negras.AlfilTotales + Negras.TorresTotales + Negras.DamasTotales + Negras.PeonTotales))
	{
		return true;
	}
	/* No hay peones */
	if (!Blancas.PeonTotales && !Negras.PeonTotales)
	{
		/* No hay figuras superiores */
		if (!(Blancas.TorresTotales + Blancas.DamasTotales) && !(Negras.TorresTotales + Negras.DamasTotales))
		{
			/* Ninguno de los dos bandos tiene pareja de alfiles o alfil y caballo */
			if ((Blancas.CaballosTotales + Blancas.AlfilTotales <= 1) && (Negras.CaballosTotales + Negras.AlfilTotales <= 1))
			{
				return true;
			}
		}
	}
	return false;
}
void LimpiarPuntuacion(int todo)
{
	memset(Blancas.PeonDoblados, 0, 8 * sizeof(int));
	Blancas.Puntos = PuntosCero;
	Blancas.ReyCuadrosAtacando = 0;
	Blancas.ReyAtaquesPiezas = 0;
	Blancas.ReyAtaquesValor = PuntosCero;
	memset(Blancas.ReyJaquePieza, 0, 5 * sizeof(int));
	Blancas.ReyJaque = false;
	Blancas.GrupoPeonesQ = 0;
	Blancas.GrupoPeonesK = 0;

	if (todo)
	{
		Blancas.PeonTotales = 0;
		Blancas.CaballosTotales = 0;
		Blancas.AlfilTotales = 0;
		Blancas.TorresTotales = 0;
		Blancas.DamasTotales = 0;
	}

	/***************************************************************/
	memset(Negras.PeonDoblados, 0, 8 * sizeof(int));
	Negras.Puntos = PuntosCero;
	Negras.ReyCuadrosAtacando = 0;
	Negras.ReyAtaquesPiezas = 0;
	Negras.ReyAtaquesValor = PuntosCero;
	memset(Negras.ReyJaquePieza, 0, 5 * sizeof(int));
	Negras.ReyJaque = false;
	Negras.GrupoPeonesQ = 0;
	Negras.GrupoPeonesK = 0;

	if (todo)
	{
		Negras.PeonTotales = 0;
		Negras.CaballosTotales = 0;
		Negras.AlfilTotales = 0;
		Negras.TorresTotales = 0;
		Negras.DamasTotales = 0;
	}
}
void EvaluarPuntosPst()
{
	int i = 0;
	int PeB = 0;
	int PeN = 0;
	int AlfB = 0;
	int AlfN = 0;
	int TorB = 0;
	int TorN = 0;
	int CabB = 0;
	int CabN = 0;
	int DamB = 0;
	int DamN = 0;

	for (i = 0; i < 64; i++)
	{
		switch (TableroGlobal.Tablero[i])
		{
		case CasillaVacia:
			continue;
			break;

		case PeonB:
			SUMA(&Blancas.Puntos, &Evaluacion.PST[BLANCAS][PEON][i]);
			Blancas.PosicionPeon[PeB++] = i;
			Blancas.PeonDoblados[COLUMNA(i)]++;
			Blancas.PeonTotales++;
			break;

		case CaballoB:
			SUMA(&Blancas.Puntos, &Evaluacion.PST[BLANCAS][CABALLO][i]);
			Blancas.PosicionCaballo[CabB++] = i;
			Blancas.CaballosTotales++;
			EvaluarCaballo(i, BLANCAS);
			break;

		case AlfilB:
			SUMA(&Blancas.Puntos, &Evaluacion.PST[BLANCAS][ALFIL][i]);
			Blancas.PosicionAlfil[AlfB++] = i;
			Blancas.AlfilTotales++;
			break;

		case TorreB:
			SUMA(&Blancas.Puntos, &Evaluacion.PST[BLANCAS][TORRE][i]);
			Blancas.PosicionTorre[TorB++] = i;
			Blancas.TorresTotales++;
			break;

		case DamaB:
			SUMA(&Blancas.Puntos, &Evaluacion.PST[BLANCAS][DAMA][i]);
			Blancas.PosicionDama[DamB++] = i;
			Blancas.DamasTotales++;
			EvaluarDama(i, BLANCAS);
			break;

		case ReyB:
			SUMA(&Blancas.Puntos, &Evaluacion.PST[BLANCAS][REY][i]);
			break;

		case PeonN:
			SUMA(&Negras.Puntos, &Evaluacion.PST[NEGRAS][PEON][i]);
			Negras.PosicionPeon[PeN++] = i;
			Negras.PeonDoblados[COLUMNA(i)]++;
			Negras.PeonTotales++;
			break;

		case CaballoN:
			SUMA(&Negras.Puntos, &Evaluacion.PST[NEGRAS][CABALLO][i]);
			Negras.PosicionCaballo[CabN++] = i;
			Negras.CaballosTotales++;
			EvaluarCaballo(i, NEGRAS);
			break;

		case AlfilN:
			SUMA(&Negras.Puntos, &Evaluacion.PST[NEGRAS][ALFIL][i]);
			Negras.PosicionAlfil[AlfN++] = i;
			Negras.AlfilTotales++;
			break;

		case TorreN:
			SUMA(&Negras.Puntos, &Evaluacion.PST[NEGRAS][TORRE][i]);
			Negras.PosicionTorre[TorN++] = i;
			Negras.TorresTotales++;
			break;

		case DamaN:
			SUMA(&Negras.Puntos, &Evaluacion.PST[NEGRAS][DAMA][i]);
			Negras.PosicionDama[DamN++] = i;
			Negras.DamasTotales++;
			EvaluarDama(i, NEGRAS);
			break;

		case ReyN:
			SUMA(&Negras.Puntos, &Evaluacion.PST[NEGRAS][REY][i]);
			break;

		default:
			return;
			break;
		}
	}
}
int Interpolar(_Valor *puntos)
{
	int mg_etapa = MIN(TableroGlobal.Etapa, MAX_ETAPA);
	int eg_etapa = MAX_ETAPA - TableroGlobal.Etapa;

	if ((puntos->Apertura * mg_etapa + puntos->Final * eg_etapa) != 0)
		return ((puntos->Apertura * mg_etapa + puntos->Final * eg_etapa) / MAX_ETAPA);
	else
		return 0;
}
void EvaluarComplejidad(_Valor *puntos)
{
	int Complejidad;
	int Signo = (puntos->Final > 0) - (puntos->Final < 0);

	int PeonesEnAmbosLados = (Blancas.GrupoPeonesQ + Negras.GrupoPeonesQ) > 0 && (Blancas.GrupoPeonesK + Negras.GrupoPeonesK) > 0;

	Complejidad = COMPLEJIDAD_PEONES_TOTALES * (Blancas.PeonTotales + Negras.PeonTotales)
		+ COMPLEJIDAD_PEONES_AMBOS_LADOS * PeonesEnAmbosLados
		+ COMPLEJIDAD_PEONES_FINALES * !(Blancas.CaballosTotales + Negras.CaballosTotales + Blancas.AlfilTotales + Negras.AlfilTotales + Blancas.TorresTotales + Negras.TorresTotales + Blancas.DamasTotales + Negras.DamasTotales)
		+ COMPLEJIDAD_AJUSTE;

	int v = Signo * MAX(Complejidad, -ABS(puntos->Final));

	puntos->Final += v;
}

void EvaluarPeones(int Turno)
{
	int indice, x;
	int abierto = false;
	int pasado = false;
	int aislado = false;
	int retrasado = false;
	int retrasado2 = false;
	int doblado = false;
	int conectado = false;
	int Horizontal = 0;
	int Columna = 0;
	int Cs = 0;
	int Atras = 0;

	/* Estructura realizada */
	/* + Pasado : https://www.chessprogramming.org/Passed_Pawn */
	/* + Aislado: https://www.chessprogramming.org/Isolated_Pawn - http://en.wikipedia.org/wiki/Isolated_pawn */
	/* + Retrasado: https://www.chessprogramming.org/Backward_Pawn - http://en.wikipedia.org/wiki/Backward_pawn */
	/* + Doblado: https://www.chessprogramming.org/Doubled_Pawn */
	/* Por hacer */
	/* - Imparable: https://www.chessprogramming.org/Unstoppable_Passer */

	int DireccionPeon[3] = { -9, -8, -7 };	/* -9, -8, -7 */
	int DireccionLado = 1;
	int ColumnaBorde[4] = { 0, 8, -1, 7 }; /* 0, 8, -1, 7 */

	int pPeon = Turno ? PeonB : PeonN;
	int pPeonOp = Turno ? PeonN : PeonB;
	//int PosicionReyOp = Turno ? TableroGlobal.PosicionReyN : TableroGlobal.PosicionReyB;
	_ST_Puntos *TempColor = Turno ? &Blancas : &Negras;
	//_ST_Puntos *TempColorOp = Turno ? &Negras : &Blancas;

	if (!Turno)
	{
		DireccionPeon[0] = 7;
		DireccionPeon[1] = 8;
		DireccionPeon[2] = 9;
	}

	for (indice = 0; indice < TempColor->PeonTotales; indice++)
	{
		abierto = false;
		pasado = false;
		aislado = false;
		retrasado = false;
		retrasado2 = false;
		doblado = false;
		conectado = false;
		Cs = TempColor->PosicionPeon[indice];
		Horizontal = Turno ? HORIZONTAL(Cs) : 7 - HORIZONTAL(Cs);
		Columna = COLUMNA(Cs);
		Atras = Turno ? Horizontal * 8 + Cs : Cs - Horizontal * 8;

		/* Detectamos peon abierto - No tiene ningun peon delante. */
		abierto = true;
		for (x = Cs + DireccionPeon[1]; x > -1 && x < 64; x = x + DireccionPeon[1])
		{
			if (TableroGlobal.Tablero[x] == pPeonOp)
			{
				abierto = false;
				break;
			}
			if (TableroGlobal.Tablero[x] == pPeon)
			{
				doblado = true;
				abierto = false;
				break;
			}
		}

		//if (Columna > ColumnaBorde[0] && Columna < ColumnaBorde[1])
		//	if (TableroGlobal.Tablero[Cs + DireccionPeon[0]] == PosicionReyOp) TempColorOp->ReyCuadrosAtacando += 1;
		//if (Columna > ColumnaBorde[2] && Columna < ColumnaBorde[3])
		//	if (TableroGlobal.Tablero[Cs + DireccionPeon[2]] == PosicionReyOp) TempColorOp->ReyCuadrosAtacando += 1;
				
		if (Columna <= COLUMNA_D)
			TempColor->GrupoPeonesQ++;
		else
			TempColor->GrupoPeonesK++;

		/* Pasado */
		if (abierto == true && pasado == false)
		{
			pasado = true;
			if (Columna > ColumnaBorde[0] && Columna < ColumnaBorde[1])
			{
				for (x = Cs + DireccionPeon[0]; x > -1 && x < 64; x = x + DireccionPeon[1])
				{
					if (TableroGlobal.Tablero[x] == pPeonOp)
					{
						pasado = false;
						break;
					}
				}
			}
			if (pasado == true)
			{
				if (Columna > ColumnaBorde[2] && Columna < ColumnaBorde[3])
				{
					for (x = Cs + DireccionPeon[2]; x > -1 && x < 64; x = x + DireccionPeon[1])
					{
						if (TableroGlobal.Tablero[x] == pPeonOp)
						{
							pasado = false;
							break;
						}
					}
				}
			}
		}

		/* Aislado */
		aislado = true;
		if (Columna > ColumnaBorde[2] && Columna < ColumnaBorde[3])
		{
			for (x = Atras + DireccionLado; x > -1 && x < 64; x = x + DireccionPeon[1])
			{
				if (TableroGlobal.Tablero[x] == pPeon)
				{
					aislado = false;
					break;
				}
			}
		}
		if (aislado == true)
		{
			if (Columna > ColumnaBorde[0] && Columna < ColumnaBorde[1])
			{
				for (x = Atras - DireccionLado; x > -1 && x < 64; x = x + DireccionPeon[1])
				{
					if (TableroGlobal.Tablero[x] == pPeon)
					{
						aislado = false;
						break;
					}
				}
			}
		}

		/* Conectado, mirando hacia atras */
		if (aislado == false)
		{
			if (Columna > ColumnaBorde[2] && Columna < ColumnaBorde[3])
			{
				if (TableroGlobal.Tablero[Cs + -DireccionPeon[0]] == pPeon)
				{
					conectado = true;
				}
			}
			if (Columna > ColumnaBorde[0] && Columna < ColumnaBorde[1])
			{
				if (TableroGlobal.Tablero[Cs + -DireccionPeon[2]] == pPeon)
				{
					conectado = true;
				}
			}
		}

		/* Retrasado */
		if (conectado == false && aislado == false)
		{
			if (Columna > ColumnaBorde[0] && Columna < ColumnaBorde[1])
			{
				if (TableroGlobal.Tablero[Cs + DireccionPeon[0]] == pPeon)
				{
					retrasado2++;
				}
			}
			if (Columna > ColumnaBorde[2] && Columna < ColumnaBorde[3])
			{
				if (TableroGlobal.Tablero[Cs + DireccionPeon[2]] == pPeon)
				{
					retrasado2++;
				}
			}

			retrasado = (retrasado2 != 0) ? true : false;
		}

		/* Asignamos puntos */
		if (doblado == true) {
			SUMA(&TempColor->Puntos, &PENALIZACION_PEON_DOBLE[Columna]);
		}
		if (aislado) {
			SUMA(&TempColor->Puntos, &PEON_AISLADO[Columna]);
		}
		else if (retrasado) {
			SUMA(&TempColor->Puntos, &PEON_RETRASADO[Columna]);
		}

		if (pasado) {
			SUMA(&TempColor->Puntos, &PEON_PASADO[Horizontal]);
		}
	}
}
void EvaluarCaballo(int Cs, int Turno)
{
	int i = 0;
	int destino = 0;
	int Mov_Ca[8] = { -17, -15, -10, -6, 6, 10, 15, 17 };
	int mov = 0;
	int AttActivoUnaVez = false;

	int pRey = Turno ? ReyB : ReyN;
	int PosicionReyOp = Turno ? TableroGlobal.PosicionReyN : TableroGlobal.PosicionReyB;
	_ST_Puntos *TempColor = Turno ? &Blancas : &Negras;
	_ST_Puntos *TempColorOp = Turno ? &Negras : &Blancas;

	for (i = 0; i < 8; i++)
	{
		destino = Cs + Mov_Ca[i];

		if ((destino > -1 && destino < 64) && TableroGlobal.TableroColor[Cs] != TableroGlobal.TableroColor[destino])
		{
			/* Movilidad */
			if (TableroGlobal.Tablero[destino] != pRey)
				mov++;

			/* Ataques - Seguridad del rey */
			if (Distancia(destino, PosicionReyOp) == 1)
			{
				TempColorOp->ReyCuadrosAtacando += 1;
				if (AttActivoUnaVez == false)
				{
					TempColorOp->ReyAtaquesPiezas++;
					SUMA(&TempColorOp->ReyAtaquesValor, &SEGURIDAD_REY_ATAQUE_PIEZAS[CABALLO]);
				}
				AttActivoUnaVez = true;
			}
			if (Distancia(destino, PosicionReyOp) == 0)
			{
				TempColorOp->ReyCuadrosAtacando += 1;
				if (AttActivoUnaVez == false)
				{
					TempColorOp->ReyAtaquesPiezas++;
					SUMA(&TempColorOp->ReyAtaquesValor, &SEGURIDAD_REY_ATAQUE_PIEZAS[CABALLO]);
				}
				AttActivoUnaVez = true;
				TempColorOp->ReyJaquePieza[CABALLO] = true;
				TempColorOp->ReyJaque = true;
			}
		}
	}

	/* Movilidad del caballo */
	SUMA(&TempColor->Puntos, &CABALLO_MOVILIDAD[mov]);
}
void EvaluarAlFil(int Turno)
{
	int ii, destino, x, Cs;
	int Mov_Al[4] = { -7, -9, 7, 9 };
	int mov = 0;
	int AttActivoUnaVez = false;

	int pRey = Turno ? ReyB : ReyN;
	int PosicionReyOp = Turno ? TableroGlobal.PosicionReyN : TableroGlobal.PosicionReyB;
	_ST_Puntos *TempColor = Turno ? &Blancas : &Negras;
	_ST_Puntos *TempColorOp = Turno ? &Negras : &Blancas;

	for (x = 0; x < TempColor->AlfilTotales; x++)
	{
		Cs = TempColor->PosicionAlfil[x];
		mov = 0;
		AttActivoUnaVez = false;

		for (ii = 0; ii < 4; ii++)
		{
			destino = Cs + Mov_Al[ii];

			while ((destino > -1 && destino < 64) && TableroGlobal.TableroColor[Cs] == TableroGlobal.TableroColor[destino])
			{
				/* Movilidad */
				if (TableroGlobal.Tablero[destino] != pRey)
					mov++;

				/* Ataques - Seguridad del rey */
				if (Distancia(destino, PosicionReyOp) == 1)
				{
					TempColorOp->ReyCuadrosAtacando += 1;
					if (AttActivoUnaVez == false)
					{
						TempColorOp->ReyAtaquesPiezas++;
						SUMA(&TempColorOp->ReyAtaquesValor, &SEGURIDAD_REY_ATAQUE_PIEZAS[ALFIL]);
					}
					AttActivoUnaVez = true;
				}
				if (Distancia(destino, PosicionReyOp) == 0)
				{
					TempColorOp->ReyCuadrosAtacando += 1;
					TempColorOp->ReyJaquePieza[ALFIL] = true;
					TempColorOp->ReyJaque = true;
				}

				if (TableroGlobal.Tablero[destino] != CasillaVacia) break;

				destino += Mov_Al[ii];
			}
		}

		/* Movilidad */
		SUMA(&TempColor->Puntos, &ALFIL_MOVILIDAD[mov]);
	}
}
void EvaluarTorre(int Turno)
{
	int ii;
	int x;
	int destino;
	int Mov_To[4] = { 1, -1, -8, 8 };
	int Cs = 0;
	int colorCs = false;
	int mov = 0;
	int AttActivoUnaVez = false;
	int Horizontal = 0;
	int HorizontalReyOp = 0;
	int Columna = 0;

	int pRey = Turno ? ReyB : ReyN;
	int PosicionReyOp = Turno ? TableroGlobal.PosicionReyN : TableroGlobal.PosicionReyB;
	_ST_Puntos *TempColor = Turno ? &Blancas : &Negras;
	_ST_Puntos *TempColorOp = Turno ? &Negras : &Blancas;

	for (x = 0; x < TempColor->TorresTotales; x++)
	{
		Cs = TempColor->PosicionTorre[x];
		mov = 0;
		AttActivoUnaVez = false;
		Horizontal = Turno ? HORIZONTAL(Cs) : 7 - HORIZONTAL(Cs);
		HorizontalReyOp = Turno ? HORIZONTAL(PosicionReyOp) : 7 - HORIZONTAL(PosicionReyOp);
		Columna = COLUMNA(Cs);

		for (ii = 0; ii < 4; ii++)
		{
			destino = Cs + Mov_To[ii];
			colorCs = TableroGlobal.TableroColor[Cs];

			while ((destino > -1 && destino < 64) && colorCs != TableroGlobal.TableroColor[destino])
			{
				/* Movilidad */
				if (TableroGlobal.Tablero[destino] != pRey)
					mov++;

				/* Ataques - Seguridad del rey */
				if (Distancia(destino, PosicionReyOp) == 1)
				{
					TempColorOp->ReyCuadrosAtacando += 1;
					if (AttActivoUnaVez == false)
					{
						TempColorOp->ReyAtaquesPiezas++;
						SUMA(&TempColorOp->ReyAtaquesValor, &SEGURIDAD_REY_ATAQUE_PIEZAS[TORRE]);
					}
					AttActivoUnaVez = true;
				}
				if (Distancia(destino, PosicionReyOp) == 0)
				{
					TempColorOp->ReyCuadrosAtacando += 1;
					TempColorOp->ReyJaquePieza[TORRE] = true;
					TempColorOp->ReyJaque = true;
				}

				if (TableroGlobal.Tablero[destino] != CasillaVacia)
					break;

				colorCs = TableroGlobal.TableroColor[destino];
				destino += Mov_To[ii];
			}
		}

		/* Movilidad del torre */
		SUMA(&TempColor->Puntos, &TORRE_MOVILIDAD[mov]);
		/* Torre en septima */
		if (Horizontal == HORIZONTAL_7 && HorizontalReyOp >= HORIZONTAL_7) SUMA(&TempColor->Puntos, &TORRE_EN_SEPTIMA);
		/* Torre en semi o abierta */
		if (!TempColor->PeonDoblados[Columna])
		{
			if (!TempColorOp->PeonDoblados[Columna])
				SUMA(&TempColor->Puntos, &TORRE_ABIERTO);
			else
				SUMA(&TempColor->Puntos, &TORRE_SEMI_ABIERTO);
		}
	}
}
void EvaluarDama(int Cs, int Turno)
{
	int i, destino;// , f;
	int Mov_Da[8] = { -9, 9, -7, 7, 1, -1, 8, -8, };
	int colorCs = false;
	int mov = 0;
	int AttActivoUnaVez = false;

	int pRey = Turno ? ReyB : ReyN;
	int PosicionReyOp = Turno ? TableroGlobal.PosicionReyN : TableroGlobal.PosicionReyB;
	_ST_Puntos *TempColor = Turno ? &Blancas : &Negras;
	_ST_Puntos *TempColorOp = Turno ? &Negras : &Blancas;

	for (i = 0; i < 4; i++)
	{
		destino = Cs + Mov_Da[i];

		while ((destino > -1 && destino < 64) && TableroGlobal.TableroColor[Cs] == TableroGlobal.TableroColor[destino])
		{
			/* Movilidad */
			if (TableroGlobal.Tablero[destino] != pRey)
				mov++;

			/* Ataques - Seguridad del rey */
			if (Distancia(destino, PosicionReyOp) == 1)
			{
				TempColorOp->ReyCuadrosAtacando += 1;
				if (AttActivoUnaVez == false)
				{
					TempColorOp->ReyAtaquesPiezas++;
					SUMA(&TempColorOp->ReyAtaquesValor, &SEGURIDAD_REY_ATAQUE_PIEZAS[DAMA]);
				}
				AttActivoUnaVez = true;
			}
			if (Distancia(destino, PosicionReyOp) == 0)
			{
				TempColorOp->ReyCuadrosAtacando += 1;
				TempColorOp->ReyJaquePieza[DAMA] = true;
				TempColorOp->ReyJaque = true;
			}

			if (TableroGlobal.Tablero[destino] != CasillaVacia) break;

			destino += Mov_Da[i];
		}
	}

	for (i = 4; i < 8; i++)
	{
		destino = Cs + Mov_Da[i];
		colorCs = TableroGlobal.TableroColor[Cs];

		while ((destino > -1 && destino < 64) && colorCs != TableroGlobal.TableroColor[destino])
		{
			/* Movilidad */
			if (TableroGlobal.Tablero[destino] != pRey)
				mov++;

			/* Ataques - Seguridad del rey */
			if (Distancia(destino, PosicionReyOp) == 1)
			{
				TempColorOp->ReyCuadrosAtacando += 1;
				if (AttActivoUnaVez == false)
				{
					TempColorOp->ReyAtaquesPiezas++;
					SUMA(&TempColorOp->ReyAtaquesValor, &SEGURIDAD_REY_ATAQUE_PIEZAS[DAMA]);
				}
				AttActivoUnaVez = true;
			}
			if (Distancia(destino, PosicionReyOp) == 0)
			{
				TempColorOp->ReyCuadrosAtacando += 1;
				TempColorOp->ReyJaquePieza[DAMA] = true;
				TempColorOp->ReyJaque = true;
			}

			if (TableroGlobal.Tablero[destino] != CasillaVacia) break;

			colorCs = TableroGlobal.TableroColor[destino];
			destino += Mov_Da[i];
		}
	}

	/* Movilidad del dama */
	SUMA(&TempColor->Puntos, &DAMA_MOVILIDAD[mov]);
}
void EvaluarSeguridadRey(int Turno)
{
	_Valor PuntosAtaque = PuntosCero;
	_ST_Puntos *TempColor = Turno ? &Blancas : &Negras;
	_ST_Puntos *TempColorOp = Turno ? &Negras : &Blancas;

	int PosicionRey = Turno ? TableroGlobal.PosicionReyB : TableroGlobal.PosicionReyN;
	int ColumnaRey = COLUMNA(PosicionRey);
	int AuxColumna = -1;
	int Columna = 0;
	int Shelter = 0;
	int Storm = 0;
	int Puntos = 0;

	/* Posicion del rey entre la columna 1 y 6 para evitar que se salga del tablero */
	for (Columna = MAX(1, ColumnaRey - 1); Columna <= MIN(6, ColumnaRey + 1); Columna++)
	{
		Shelter = ProteccionPeonesReyShelter(Columna + AuxColumna, Turno);
		Storm = ProteccionPeonesReyStorm(Columna + AuxColumna, Turno);

		Puntos += (Shelter != 7 || Storm != 7) ? 0 : COLUMNA_ABIERTA_SHELTER[Columna];
		Puntos += (Shelter == 7 && Storm != 7) ? COLUMNA_SEMI_ABIERTA_SHELTER[Columna] : 0;
		Puntos += Storm != 7 ? COLUMNA_STORM[Storm] : 0;

		if (Puntos != 0) TempColor->Puntos.Apertura += Puntos;
		if (Puntos != 0) TempColor->Puntos.Final += Puntos;

		AuxColumna++;
	}

	if (TempColor->ReyAtaquesPiezas > 1 - TempColor->DamasTotales)
	{
		SUMA(&PuntosAtaque, &TempColor->ReyAtaquesValor);

		PuntosAtaque.Apertura += SEGURIDAD_REY_ATAQUES_ZONA.Apertura * TempColor->ReyCuadrosAtacando
			+ SEGURIDAD_REY_CABALLO_JAQUE.Apertura * TempColor->ReyJaquePieza[CABALLO]
			+ SEGURIDAD_REY_ALFIL_JAQUE.Apertura * TempColor->ReyJaquePieza[ALFIL]
			+ SEGURIDAD_REY_TORRE_JAQUE.Apertura * TempColor->ReyJaquePieza[TORRE]
			+ SEGURIDAD_REY_DAMA_JAQUE.Apertura * TempColor->ReyJaquePieza[DAMA]
			+ SEGURIDAD_REY_DAMA_ENEMIGA.Apertura * !TempColorOp->DamasTotales;

		PuntosAtaque.Final += SEGURIDAD_REY_ATAQUES_ZONA.Final * TempColor->ReyCuadrosAtacando
			+ SEGURIDAD_REY_CABALLO_JAQUE.Final * TempColor->ReyJaquePieza[CABALLO]
			+ SEGURIDAD_REY_ALFIL_JAQUE.Final * TempColor->ReyJaquePieza[ALFIL]
			+ SEGURIDAD_REY_TORRE_JAQUE.Final * TempColor->ReyJaquePieza[TORRE]
			+ SEGURIDAD_REY_DAMA_JAQUE.Final * TempColor->ReyJaquePieza[DAMA]
			+ SEGURIDAD_REY_DAMA_ENEMIGA.Final * !TempColorOp->DamasTotales;

		if (-PuntosAtaque.Apertura * MAX(0, PuntosAtaque.Apertura) != 0) TempColor->Puntos.Apertura += -PuntosAtaque.Apertura * MAX(0, PuntosAtaque.Apertura) / 720;
		if (-MAX(0, PuntosAtaque.Final) != 0) TempColor->Puntos.Final += -MAX(0, PuntosAtaque.Final) / 20;
	}
}

int ProteccionPeonesReyShelter(int Cs, int Turno)
{
	int x = 0;
	int pPeon = Turno ? PeonB : PeonN;
	int DireccionPeon = Turno ? -8 : 8;
	Cs = Turno ? 56 + Cs : Cs; /* Empezamos a buscar desde el inicio de la columna. */

	for (x = Cs + DireccionPeon; x > -1 && x < 64; x = x + DireccionPeon)
	{
		if (TableroGlobal.Tablero[x] == pPeon)
		{
			return Turno ? HORIZONTAL(x) : 7 - HORIZONTAL(x);
			break;
		}
	}
	return 7;
}
int ProteccionPeonesReyStorm(int Cs, int Turno)
{
	int x = 0;
	int pPeonOp = Turno ? PeonN : PeonB;
	int DireccionPeon = Turno ? -8 : 8;
	Cs = Turno ? 56 + Cs : Cs; /* Empezamos a buscar desde el inicio de la columna. */

	for (x = Cs + DireccionPeon; x > -1 && x < 64; x = x + DireccionPeon)
	{
		if (TableroGlobal.Tablero[x] == pPeonOp)
		{
			return Turno ? HORIZONTAL(x) : ABS((HORIZONTAL(x) - 7));
			break;
		}
	}
	return 7;
}
