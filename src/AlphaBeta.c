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

#include "AlphaBeta.h"
#ifdef USAR_NNUE
#include "nnue.h"
#endif
/******************************************************************************
Variables
******************************************************************************/

#ifdef USAR_REDUCTION
	int lmr[MAX_PLY][MAX_PLY];
#endif
#ifdef USAR_LATE_MOVE_PRUNING
	int lmpc[2][9];
#endif

int vp_triangular[MAX_PLY][MAX_PLY];
int vp_terminada_raiz[MAX_PLY];
int vp_terminada[MAX_PLY];
int vp_Ev[MAX_PLY];
int selDepth = 0;
int lowerbound = 0;

void Iniciar_AlphaBeta()
{
	int dp;
	int mv;

#ifdef USAR_REDUCTION
	for (dp = 0; dp < MAX_PLY; dp++)
	{
		for (mv = 0; mv < MAX_PLY; mv++)
		{
			lmr[dp][mv] = (int)(0.75 + (float)log(dp) * (float)log(MIN(mv, MAX_PLY - 1)) / 2.25f);

			if (lmr[dp][mv] < 1.0f)
				lmr[dp][mv] = 0;
		}
	}
#endif

#ifdef USAR_LATE_MOVE_PRUNING
	for (dp = 1; dp < 9; dp++)
	{
		lmpc[0][dp] = 2.5 + 2 * dp * dp / 4.5;
		lmpc[1][dp] = 4.0 + 4 * dp * dp / 4.5;
	}
#endif
}

void A_Inicio(int full)
{
	int i = 0;
	int y = 0;
	TipoJuego.Interrumpir = false;
	TipoJuego.Nodos = 0;
#ifdef USAR_TBPROBE
	Gaviota.Acierto = 0;
#endif

	for (i = 0; i < MAX_PLY; i++)
	{
		for (y = 0; y < MAX_PLY; y++)
		{
			vp_triangular[i][y] = 0;
		}
		vp_terminada_raiz[i] = 0;
		vp_terminada[i] = 0;
	}

	HistoricoIniciar();

#ifdef USAR_HASH_TB
	if (full == true)
	{
		LimpiarTransposicion();
	}
	else
	{
		TT_Opciones.tt_Completo = 0;
		TT_Opciones.tt_Edad++;
		if (TT_Opciones.tt_Edad > MAX_EDAD)
			LimpiarTransposicion();
	}
#endif
}

void Buscar()
{
	int _pv = 1;
	int en_jaque = false;
	int PuntosMate = 0;
	int val = 0;
	int prev_score = 0;
	int Informado = false;
	int x = 0;
	int mate = 0;

	TableroGlobal.Ply = 0;
	TipoJuego.Inicio = ObtenerTiempo();

	TipoJuego.DepthAct = 1;
	if (TipoJuego.MaxDepth < TipoJuego.DepthAct)
	{
		TipoJuego.MaxDepth = TipoJuego.DepthAct;
	}
	if (TipoJuego.MaxDepth >= (float)MAX_PLY / 2.0f)
	{
		TipoJuego.MaxDepth = ((float)MAX_PLY / 2.0f) - 1;
	}

	en_jaque = Jaque(TableroGlobal.MueveBlancas);

	for (; TipoJuego.DepthAct < TipoJuego.MaxDepth; TipoJuego.DepthAct++)
	{
		if (TipoJuego.Interrumpir == true)
		{
			if (TipoJuego.DepthAct > 1)
			{
				break;
			}
		}

		lowerbound = 0;

		val = AspirationWindows(TipoJuego.DepthAct, en_jaque, val);

		/* Actualizamos vp_terminada al terminar la interacción con vp_terminada_raiz, si no se ha interrumpido la búsqueda */
		if (TipoJuego.Interrumpir == false)
		{
			for (x = 0; vp_terminada_raiz[x] != NO_MOVIMIENTO; x++)
			{
				vp_terminada[x] = vp_terminada_raiz[x];
			}
			vp_terminada[x] = 0;
		}


		if (TipoJuego.DepthAct > 1)
		{
			//if (TipoJuego.BuscarMate == 0 && EsPuntuacionMate(PuntosMate) == true && EsPuntuacionMate(val) == true && PuntosMate == val)
			//{
			//	if (TipoJuego.Infinito == false)
			//	{
			//		break;
			//	}
			//}

			if (TipoJuego.BuscarMate > 0 && EsPuntuacionMate(PuntosMate) == true && EsPuntuacionMate(val) == true && PuntosMate == val)
			{
				if (val > 0)
				{
					if ((int)((float)((float)(VALOR_MATE - val) / 2.0f) + 0.5f) == 0)
						mate = 1;
					else
						mate = (int)((float)((float)(VALOR_MATE - val) / 2.0f) + 0.5f);

					if (TipoJuego.BuscarMate >= ABS(mate))
					{
						if (TipoJuego.Interrumpir == false)
						{
							_pv = TipoJuego.DepthAct;
							TipoJuego.MejorJugada = vp_terminada[0];
							TipoJuego.MejorJugadaAdv = vp_terminada[1];
							ImprimirVp(_pv, &Informado, val, lowerbound);
						}
						break;
					}
				}
				else
				{
					if ((int)((float)((float)(-VALOR_MATE - val) / 2.0f) - 0.5f) == 0)
						mate = -1;
					else
						mate = (int)((float)((float)(-VALOR_MATE - val) / 2.0f) - 0.5f);

					if (TipoJuego.BuscarMate >= ABS(mate))
					{
						if (TipoJuego.Interrumpir == false)
						{
							_pv = TipoJuego.DepthAct;
							TipoJuego.MejorJugada = vp_terminada[0];
							TipoJuego.MejorJugadaAdv = vp_terminada[1];
							ImprimirVp(_pv, &Informado, val, lowerbound);
						}
						break;
					}
				}
			}

			if (TipoJuego.Interrumpir == false)
			{
				PuntosMate = val;
			}
			else
			{
				val = PuntosMate;
			}


			if (val > prev_score + 50)
			{
				lowerbound = 1;
			}
			else if (val < prev_score - 50)
			{
				lowerbound = -1;
			}
		}

		prev_score = val;

		if (TipoJuego.Interrumpir == false)
		{
			_pv = TipoJuego.DepthAct;
			TipoJuego.MejorJugada = vp_terminada[0];
			TipoJuego.MejorJugadaAdv = vp_terminada[1];
			ImprimirVp(_pv, &Informado, val, lowerbound);
		}

		lowerbound = 0;
	}

	if (Informado == false && vp_terminada[0] != NO_MOVIMIENTO)
	{
		if (TipoJuego.DepthAct > (float)MAX_PLY / 2.0f)
		{
			TipoJuego.DepthAct = (float)MAX_PLY / 2.0f;
		}
		_pv = TipoJuego.DepthAct;
		TipoJuego.MejorJugada = vp_terminada[0];
		TipoJuego.MejorJugadaAdv = vp_terminada[1];
		int _in = TipoJuego.Infinito;
		TipoJuego.Infinito = true;
		ImprimirVp(_pv, &Informado, val, lowerbound);
		TipoJuego.Infinito = _in;
	}

	ImprimirMejorJugada(TipoJuego.MejorJugada, TipoJuego.MejorJugadaAdv);
	A_Inicio(false);

}

int AspirationWindows(int depth, int en_jaque, int PuntuacionAnterior)
{
	int val = 0;

#ifdef ASPIRATION_WINDOWS
	int windows = ASPIRATION;
	int alpha =-VALOR_MATE, beta = VALOR_MATE;
	int depthc = depth;

	if (depth > 6)
	{
		alpha = MAX(-VALOR_MATE, PuntuacionAnterior - windows);
		beta = MIN(VALOR_MATE, PuntuacionAnterior + windows);
	}

	while (1)
	{
		val = AlphaBeta(depth, alpha, beta, en_jaque, false);
		if (TipoJuego.Interrumpir == true)
		{
			break;
		}
		if (val > alpha && val < beta)
		{
			lowerbound = MAX(alpha, MIN(val, beta)) >= beta ? 1 : MAX(alpha, MIN(val, beta)) <= alpha ? -1 : 0;
			return val;
		}

		if (val <= alpha) {
			beta = (alpha + beta) / 2;
			alpha = MAX(-VALOR_MATE, alpha - windows);
			depth = depthc;
		}

		else if (val >= beta) {
			beta = MIN(VALOR_MATE, beta + windows);
			depth = depth - (abs(val) <= VALOR_MATE / 2);
		}

		windows += ASPIRATION;
	}
#else
	val = AlphaBeta(depth, -VALOR_MATE, VALOR_MATE, en_jaque, false);
	return val;
#endif

}

int AlphaBeta(int depth, int alpha, int beta, int en_jaque, int Es_Nulo)
{
	int i = 0;
	_ST_Movimiento nextMoves[MAX_JUGADAS];
	int mInquietos[MAX_JUGADAS];
	int mInquietosTotal = 0;
	int NumeroDeMovimientos = 0;
	int MovimientosLegales = 0;
	int puntos = 0;
	int puntos_max = -VALOR_MATE;
	int da_jaque = false;
	int reducciones = 0;
	int Ply = TableroGlobal.Ply;
	int Root = (TableroGlobal.Ply == 0);
	int Zw = !(alpha != beta - 1);
	int nuevo_depth = 0;
	int oldAlpha = alpha;
	int turno = TableroGlobal.MueveBlancas;
	int Mejor_Movimiento = NO_MOVIMIENTO;
	int Ev = 0;
#ifdef USAR_MATE_DISTANCE_PRUNING
	int mAlpha = 0;
	int mBeta = 0;
#endif
#ifdef USAR_LATE_MOVE_PRUNING
	int Mejorando = false;
#endif
#ifdef USAR_REDUCTION
	int HistoricoV = 0;
#endif
	int hMov = NO_MOVIMIENTO;
	int hEv = 0;
#ifdef USAR_HASH_TB
	int hPuntos = -VALOR_MATE + (MAX_PLY * 2);
	int hFlag = TT_DESCONOCIDO;
	vp_Ev[Ply] = 0;
#endif
#ifdef USAR_TBPROBE
	int AccederTablasdefinales = false;
	int NumeroFichas = 0;
#endif

	selDepth = Root ? 0 : MAX(selDepth, Ply);
	TipoJuego.Nodos++;
	if (Ply > 0 && Zw == false)
	{
		vp_triangular[Ply][0] = NO_MOVIMIENTO;
	}

	if (TipoJuego.BuscarNodos > 0 && (U64)TipoJuego.BuscarNodos < TipoJuego.Nodos + 1 && TipoJuego.DepthAct > 1)
	{
		TipoJuego.Interrumpir = true;
		return VALOR_EMPATE;
	}

	if (en_jaque && depth < 1)
	{
		depth++;
	}

	if (depth < 1 || Ply > (MAX_PLY - 2))
	{
		return AlphaBeta_Qs(alpha, beta);
	}

	if (Root == false)
	{
		ComprobarTiempo();
		if (TipoJuego.Interrumpir == true)
		{
			return VALOR_EMPATE;
		}

		if (Es_Tabla_Repeticion(&en_jaque) == true)
		{
			return VALOR_EMPATE;
		}

#ifdef USAR_MATE_DISTANCE_PRUNING
		mAlpha = alpha > MATE(Ply) ? alpha : MATE(Ply);
		mBeta = beta < MATE_EN(Ply) ? beta : MATE_EN(Ply);
		if (mAlpha >= mBeta)
		{
			return mAlpha;
		}
#endif
	}

#ifdef USAR_HASH_TB
	if (RecuperarPosicion(&hPuntos, &hEv, &hMov, &hFlag) == true)
	{
		ConvertirValorTT(&hPuntos);

	#ifdef USAR_HASH_PRUNING
		if (Zw == true)
		{
			if (PodaHash(&hFlag, &depth, &beta, &alpha, &hPuntos) == true)
				return hPuntos;
		}
	#endif
	}
#endif

#ifdef USAR_TBPROBE
	/* Comprobamos acceso a las tablas de finales de gaviota, solo en pv */
	if (Gaviota.Usar == true && Zw == true && Gaviota.Dll_Cargada == true && TableroGlobal.Ply > 0)
	{
		/* Cuantas piezas tenemos */
		NumeroFichas = Blancas.PeonTotales + Blancas.CaballosTotales + Blancas.AlfilTotales + Blancas.TorresTotales + Blancas.DamasTotales
			+ Negras.PeonTotales + Negras.CaballosTotales + Negras.AlfilTotales + Negras.TorresTotales + Negras.DamasTotales + 2; /* + 2 son los reyes */

		if ((Gaviota.Tablas_Disponibles & 1) != 0 && NumeroFichas == 3 && Gaviota.Limite >= NumeroFichas) /* Tenemos 3 piezas */
		{
			AccederTablasdefinales = true;
		}
		if ((Gaviota.Tablas_Disponibles & 4) != 0 && NumeroFichas <= 4 && Gaviota.Limite >= NumeroFichas) /* Tenemos 4 piezas */
		{
			AccederTablasdefinales = true;
		}
		if ((Gaviota.Tablas_Disponibles & 16) != 0 && NumeroFichas <= 5 && Gaviota.Limite >= NumeroFichas) /* Tenemos 5 piezas */
		{
			AccederTablasdefinales = true;
		}

		if (AccederTablasdefinales == true && ((TipoJuego.DepthAct <= 3 && depth >= 1 && depth <= 3) || (TableroGlobal.Regla_50_Movimiento == 0 && depth <= 1)) )
		{
			puntos = 0;
			if (Probar_gaviota(&puntos, &Ply))
			{
				return puntos;
			}
		}
	}
#endif

	Ev = vp_Ev[Ply] = (hEv != 0) ? hEv : Evaluar();
#ifdef USAR_LATE_MOVE_PRUNING
	Mejorando = Ply >= 2 && Ev > vp_Ev[Ply - 2];
#endif

#ifdef USAR_STATIC_MOVE_PRUNING
	if (Zw == true
		&& en_jaque == false
		&& depth <= 8
		&& Ev - 85 * depth > beta)
	{
		return Ev;
	}
#endif

#ifdef USAR_NULL_MOVE_PRUNING
	if (Zw == true
		&& en_jaque == false
		&& Es_Nulo == false
		&& depth > 1
		&& Ev >= beta
		&& FigurasAdversarioNull(turno)
		&& (!hMov || FLAG(hFlag) == TT_BETA || hPuntos >= beta))
	{
		nuevo_depth = depth - (4 + depth / 6 + MIN(3, (Ev - beta) / 200));
		HacerMovimientoNull();
		puntos = -AlphaBeta(nuevo_depth, -beta, -beta + 1, en_jaque, true);
		DeshacerMovimientoNull();

		if (puntos >= beta)
		{
			return beta;
		}
	}
#endif

#ifdef USAR_IDD
	if (hMov == NO_MOVIMIENTO && Zw == false && depth >= 4 && en_jaque == false)
	{
		puntos = AlphaBeta(depth - 2, alpha, beta, en_jaque, true);
		if (TipoJuego.Interrumpir == true)
		{
			return VALOR_EMPATE;
		}

		hMov = RecuperarMovimientoHash();
	}
#endif

	NumeroDeMovimientos = GenerarMovimientos(Todos, nextMoves);
	OrdenarMovimientosTodos(&NumeroDeMovimientos, &hMov, nextMoves);

	for (i = 0; i < NumeroDeMovimientos; i++)
	{
		OrdenaMovimiento(&i, &NumeroDeMovimientos, nextMoves);

#ifdef USAR_LATE_MOVE_PRUNING
		if (puntos_max > MATE((MAX_PLY * 2))
			&& depth <= 8
			&& MovimientoInquieto(&nextMoves[i].Movimiento) == true
			&& i >= lmpc[Mejorando][depth])
		{
			continue;
		}
#endif

		if (HacerMovimiento(nextMoves[i].Movimiento, true) == false)
		{
			DeshacerMovimiento();
			continue;
		}

		if (MovimientoInquieto(&nextMoves[i].Movimiento) == true)
			mInquietos[mInquietosTotal++] = nextMoves[i].Movimiento;

		MovimientosLegales++;
		reducciones = 0;
		da_jaque = Jaque(TableroGlobal.MueveBlancas);
		nuevo_depth = depth - 1;

		if (Root == true && ObtenerTiempo() - TipoJuego.Inicio > 4000)
		{
			ExtraInfo(&nextMoves[i].Movimiento, &MovimientosLegales);
		}

#ifdef USAR_EXTENSIONS
		if (Root == false && da_jaque == true && (Zw == false || depth <= 5))
		{
			nuevo_depth += 1;
		}
		else if (Root == true && en_jaque == true)
		{
			nuevo_depth += 1;
		}
#endif

#ifdef USAR_REDUCTION
		if (en_jaque == false
			&& da_jaque == false
			&& MovimientosLegales > 2
			&& lmr[depth][MIN(MovimientosLegales, MAX_PLY - 1)] > 0
			&& nextMoves[i].Movimiento != hMov
			&& MovimientoInquieto(&nextMoves[i].Movimiento) == true
			&& ComprobarMovimientoKiller(&Ply, &nextMoves[i].Movimiento) == false
			)
		{
			HistoricoV = HistoricoValor(&nextMoves[i].Movimiento);
			if (HistoricoV == -102)
			{
				HistoricoV = 0;
			}

			reducciones = lmr[depth][MIN(MovimientosLegales, MAX_PLY - 1)];
			reducciones += (Zw == false && HistoricoV < -15);
			reducciones += (Zw == true && HistoricoV < 0);

			reducciones = MIN(depth - 1, MAX(reducciones, 1));

			nuevo_depth += -reducciones;
		}
#endif

#ifdef USAR_PVS
		if (puntos_max == -VALOR_MATE)
		{
			puntos = -AlphaBeta(nuevo_depth, -beta, -alpha, da_jaque, false);
		}
		else
		{
			puntos = -AlphaBeta(nuevo_depth, -alpha-1, -alpha, da_jaque, false);

			if (puntos > alpha && reducciones > 0)
			{
				puntos = -AlphaBeta(nuevo_depth, -beta, -alpha, da_jaque, false);

				if (puntos >= beta)
				{
					puntos = -AlphaBeta(nuevo_depth + reducciones, -beta, -alpha, da_jaque, false);
				}
			}
			if (puntos > alpha)
			{
				puntos = -AlphaBeta(nuevo_depth + reducciones, -beta, -alpha, da_jaque, false);
			}
		}
#else
		puntos = -AlphaBeta(nuevo_depth, -beta, -alpha, da_jaque, false);
#endif

		DeshacerMovimiento();

		if (TipoJuego.Interrumpir == true)
		{
			return VALOR_EMPATE;
		}

		if (puntos > puntos_max)
		{
			puntos_max = puntos;

			if (puntos > alpha)
			{
				alpha = puntos;
				Mejor_Movimiento = nextMoves[i].Movimiento;

				if (Zw == false || Root == true)
				{
					Actualizar_Vp(Mejor_Movimiento);
					if (Root == true)
					{
						Actualizar_Vp_Raiz();
					}
				}

				if (puntos >= beta)
				{
					if (en_jaque == false && MovimientoInquieto(&Mejor_Movimiento) == true)
					{
						HistoricoActualizar(depth, &Ply, Mejor_Movimiento, EsPuntuacionMate(puntos), mInquietos, mInquietosTotal);
					}
					break;
				}
			}
		}
	}

	if (MovimientosLegales == 0)
	{
		if (en_jaque == true)
		{
			return MATE(Ply);
		}
		else
		{
			return VALOR_EMPATE;
		}
	}

#ifdef USAR_HASH_TB
	AlmacenarPosicion(depth, puntos_max, Ev, puntos_max >= beta
		? TT_BETA : puntos_max > oldAlpha ? TT_EXACTO : TT_ALPHA, Mejor_Movimiento);
#endif

	return puntos_max;
}

/* Rutina Quiescence */
int AlphaBeta_Qs(int alpha, int beta)
{
	_ST_Movimiento nextMoves[MAX_JUGADAS];
	int i = 0;
	int puntos = 0;
	int NumeroDeMovimientos = 0;
	int puntos_max = -VALOR_MATE;
	int Ev = 0;

	int hMov = NO_MOVIMIENTO;
	int hEv = 0;
#ifdef USAR_HASH_PRUNING
	int hPuntos = 0;
	int hFlag = TT_DESCONOCIDO;
	int hDepth = -1;
#endif

	TipoJuego.Nodos++;

	ComprobarTiempo();
	if (TipoJuego.Interrumpir == true) return VALOR_EMPATE;
	if (Es_Tabla_Repeticion(&Ev) == true) return VALOR_EMPATE;
	if (TableroGlobal.Ply > (MAX_PLY - 2)) return Evaluar();

#ifdef USAR_HASH_PRUNING
	if (RecuperarPosicion(&hPuntos, &hEv, &hMov, &hFlag) == true)
	{
		ConvertirValorTT(&hPuntos);
		if (PodaHash(&hFlag, &hDepth, &beta, &alpha, &hPuntos) == true) return hPuntos;
	}
#endif

	Ev = (hEv != 0) ? hEv : Evaluar();

	puntos_max = Ev;
	alpha = MAX(alpha, Ev);
	if (alpha >= beta) return Ev;

	NumeroDeMovimientos = GenerarMovimientos(CapturasCoronacion, nextMoves);
	OrdenarMovimientosTodos(&NumeroDeMovimientos, &hMov, nextMoves);

	for (i = 0; i < NumeroDeMovimientos; i++)
	{
		OrdenaMovimiento(&i, &NumeroDeMovimientos, nextMoves);

		if (HacerMovimiento(nextMoves[i].Movimiento, true) == false)
		{
			DeshacerMovimiento();
			continue;
		}
		/* Sin Jaque/evasiones. Solo capturas.*/
		if (Jaque(TableroGlobal.MueveBlancas) == true)
		{
			DeshacerMovimiento();
			continue;
		}

		if (CORONACION(nextMoves[i].Movimiento) == MFLAGPROM
			&& ValorPieza(PIEZAMOVIDA(nextMoves[i].Movimiento)) != ReyValor
			&& ValorPieza(PIEZAMOVIDA(nextMoves[i].Movimiento)) > ValorPieza(CAPTURADA(nextMoves[i].Movimiento))
			&& See(&nextMoves[i].Movimiento, TableroGlobal.MueveBlancas) < 0)
		{
			DeshacerMovimiento();
			continue;
		}

		puntos = -AlphaBeta_Qs(-beta, -alpha);

		DeshacerMovimiento();

		if (TipoJuego.Interrumpir == true) return VALOR_EMPATE;

		if (puntos > puntos_max)
		{
			puntos_max = puntos;
			if (puntos > alpha)
			{
				alpha = puntos;
				if (puntos >= beta) break;
			}
		}
	}

	return puntos_max;
}

/* Actualiza la variante principal */
void Actualizar_Vp(int M)
{
	int i = 0;

	if (TableroGlobal.Ply < MAX_PLY - 1)
	{
		vp_triangular[TableroGlobal.Ply][0] = M;
		for (i = 1; vp_triangular[TableroGlobal.Ply + 1][i - 1] != NO_MOVIMIENTO && i < MAX_PLY; i++)
		{
			vp_triangular[TableroGlobal.Ply][i] = vp_triangular[TableroGlobal.Ply + 1][i - 1];
			if (vp_triangular[TableroGlobal.Ply][i] == NO_MOVIMIENTO)
				break;
		}

		vp_triangular[TableroGlobal.Ply][i] = NO_MOVIMIENTO;
		vp_triangular[TableroGlobal.Ply + 1][0] = NO_MOVIMIENTO;
	}
}

void Actualizar_Vp_Raiz()
{
	int x = 0;
	for (x = 0; vp_triangular[0][x] != NO_MOVIMIENTO && x < MAX_PLY; x++)
	{
		vp_terminada_raiz[x] = vp_triangular[0][x];
	}
	vp_terminada_raiz[x] = NO_MOVIMIENTO;
}

/* Imprimimos la variante principal */
void ImprimirVp(int M_Depth, int *informado, int puntos, int lowerbound)
{
	U64 A_Nps = 0;
	int pvi = 0;
	U64 tiempo = (ObtenerTiempo() - TipoJuego.Inicio);
	char PV[MAX_PLY * 7];
	char Movi[6];
	int mate = 0;

#ifdef USAR_TBPROBE
	U64 tbhits = 0;
#endif

	if (tiempo != 0)
		tiempo = (tiempo <= (float)TipoJuego.Tiempo / 3.0f);
	else
		tiempo = true;

	if (TipoJuego.Infinito == false && TipoJuego.MostrarVp == 0)
	{
		return;
	}

	if (TipoJuego.Infinito == false
		&& TipoJuego.MostrarVp == 1
		&& (
		(TipoJuego.Activo == false && M_Depth < (float)TipoJuego.MaxDepth / 2.0f)
		|| (TipoJuego.Activo == true && tiempo == true)
		|| (TipoJuego.Activo == true && TipoJuego.Interrumpir == false && tiempo == true)
		)
		)
	{
		return;
	}

	*informado = true;

	TipoJuego.TiempoTranscurrido = ObtenerTiempo() - TipoJuego.Inicio;
	if (TipoJuego.TiempoTranscurrido != 0 && TipoJuego.Nodos != 0)
	{
		A_Nps = (U64)((float)TipoJuego.Nodos / (float)TipoJuego.TiempoTranscurrido) * 1000;
	}
	else
	{
		A_Nps = 0;
	}

	memset(PV, 0, selDepth * 7 * sizeof(char));
	for (pvi = 0; vp_terminada[pvi] != NO_MOVIMIENTO; pvi++)
	{
		strcat(PV, " ");

		memset(Movi, 0, 6 * sizeof(char));
#ifdef USAR_AJEDREZ960
		if (TipoJuego.Ajedrez960 == true)
		{
			if (ENROQUE(vp_terminada[pvi]) == MFLAGCA && TipoJuego.Ajedrez960Enroque == true)
			{
				if (CUADRADO_ORIGEN(vp_terminada[pvi]) < CUADRADO_DESTINO(vp_terminada[pvi]))
				{
					strcat(Movi, "O-O");
				}
				else
				{
					strcat(Movi, "O-O-O");
				}
			}
			else
			{
				MovimientoCoordenadas(CUADRADO_ORIGEN(vp_terminada[pvi]), CUADRADO_DESTINO(vp_terminada[pvi]), CORONACION(vp_terminada[pvi]), Movi);
			}
		}
		else
		{
#endif
			MovimientoCoordenadas(CUADRADO_ORIGEN(vp_terminada[pvi]), CUADRADO_DESTINO(vp_terminada[pvi]), CORONACION(vp_terminada[pvi]), Movi);
#ifdef USAR_AJEDREZ960
		}
#endif

		strcat(PV, Movi);
	}

	printf("info depth "S32_FORMAT" seldepth "S32_FORMAT"", M_Depth, selDepth);
	printf(" time "U64_FORMAT" nodes "U64_FORMAT" nps "U64_FORMAT" ", TipoJuego.TiempoTranscurrido, TipoJuego.Nodos, A_Nps);

#ifdef USAR_TBPROBE
	if (Gaviota.Usar == true)
	{
		tbhits = Gaviota.Acierto;
		#ifdef USAR_HASH_TB
		printf("tbhits "U64_FORMAT" hashfull "S32_FORMAT" score ", tbhits, ObtenerHashCompleto());
		#else
		printf("tbhits "U64_FORMAT" score ", tbhits);
		#endif
	}
	else
	{
		#ifdef USAR_HASH_TB
		printf("hashfull "S32_FORMAT" score ", ObtenerHashCompleto());
		#else
		printf(" score ");
		#endif
	}
#else
	#ifdef USAR_HASH_TB
	printf("hashfull "S32_FORMAT" score ", ObtenerHashCompleto());
	#else
	printf(" score ");
	#endif
#endif

	if (EsPuntuacionMate(puntos) == false)
	{
		printf("cp "S32_FORMAT"", puntos);
		if (lowerbound == 1)
		{
			printf(" lowerbound");
		}
		else if (lowerbound == -1)
		{
			printf(" upperbound");
		}
	}
	else
	{
		if (puntos > 0)
		{
			if ((int)((float)((float)(VALOR_MATE - puntos) / 2.0f) + 0.5f) == 0)
				mate = 1;
			else
				mate = (int)((float)((float)(VALOR_MATE - puntos) / 2.0f) + 0.5f);
		}
		else
		{
			if ((int)((float)((float)(-VALOR_MATE - puntos) / 2.0f) - 0.5f) == 0)
				mate = -1;
			else
				mate = (int)((float)((float)(-VALOR_MATE - puntos) / 2.0f) - 0.5f);
		}

		printf("mate "S32_FORMAT"", mate);
	}
	printf(" pv"STRING_FORMAT"\n", PV);
	fflush(stdout);
}

void ExtraInfo(int *M, int *currmovenumber)
{
	/* Imprimimos información adicional */
	printf("info depth "S32_FORMAT"", TipoJuego.DepthAct);
	printf(" currmove ");
	ImprimirMovimientoCoordenadas(CUADRADO_ORIGEN(*M), CUADRADO_DESTINO(*M), CORONACION(*M));
	printf(" currmovenumber "S32_FORMAT"", *currmovenumber);
	printf("\n");
	fflush(stdout);
}

/* Imprimimos la mejor jugada y ponder */
void ImprimirMejorJugada(int Jugada, int J_Adv)
{
	printf("bestmove ");
	if (Jugada != NO_MOVIMIENTO)
	{
#ifdef USAR_AJEDREZ960
		if (TipoJuego.Ajedrez960 == true)
		{
			if (ENROQUE(Jugada) == MFLAGCA && TipoJuego.Ajedrez960Enroque == true)
			{
				if (CUADRADO_ORIGEN(Jugada) < CUADRADO_DESTINO(Jugada))
				{
					printf(""STRING_FORMAT"", "O-O");
				}
				else
				{
					printf(""STRING_FORMAT"", "O-O-O");
				}
			}
			else
			{
				ImprimirMovimientoCoordenadas(CUADRADO_ORIGEN(Jugada), CUADRADO_DESTINO(Jugada), CORONACION(Jugada));
			}
		}
		else
		{
#endif
			ImprimirMovimientoCoordenadas(CUADRADO_ORIGEN(Jugada), CUADRADO_DESTINO(Jugada), CORONACION(Jugada));
#ifdef USAR_AJEDREZ960
		}
#endif
	}
	else
	{
		printf("0000");
	}

	if (J_Adv != NO_MOVIMIENTO)
	{
		printf(" ponder ");
#ifdef USAR_AJEDREZ960
		if (TipoJuego.Ajedrez960 == true)
		{
			if (ENROQUE(J_Adv) == MFLAGCA && TipoJuego.Ajedrez960Enroque == true)
			{
				if (CUADRADO_ORIGEN(J_Adv) < CUADRADO_DESTINO(J_Adv))
				{
					/* Enroque corto */
					printf(""STRING_FORMAT"", "O-O");
				}
				else/* Enroque largo */
				{
					printf(""STRING_FORMAT"", "O-O-O");
				}
			}
			else
			{
				ImprimirMovimientoCoordenadas(CUADRADO_ORIGEN(J_Adv), CUADRADO_DESTINO(J_Adv), CORONACION(J_Adv));
			}
		}
		else
		{
#endif
			ImprimirMovimientoCoordenadas(CUADRADO_ORIGEN(J_Adv), CUADRADO_DESTINO(J_Adv), CORONACION(J_Adv));
#ifdef USAR_AJEDREZ960
		}
#endif
	}
	printf("\n");
	fflush(stdout);
}
/* Comprueba si el movimiento genera tabla */
int Es_Tabla_Repeticion(int *en_jaque)
{
#ifdef USAR_HASH_TB
	int i = 0;
	int reps = 0;

#endif

	if (TableroGlobal.Regla_50_Movimiento > 100)
	{
		return true;
	}

#ifdef USAR_HASH_TB
	for (i = TableroGlobal.Hply - 2; i >= 0; i -= 2)
	{
		if (i < TableroGlobal.Hply - TableroGlobal.Regla_50_Movimiento)
			break;

		if (TableroGlobal.Historico[i] == TableroGlobal.Hash
			&& (i > TableroGlobal.Hply - TableroGlobal.Ply || ++reps == 2))
			return true;
	}

	/* Si empezamos partida con FEN y hay Moviminetos realizados. Se desborda el vector ya que el programa desconoce los movimientos y mira en el vector en negativo */
	// Pues Hply es 0 y Regla_50_Movimiento valor obtenido del FEN 20, 36, 15...
	//if ((TableroGlobal.Hply - TableroGlobal.Regla_50_Movimiento) > -1)
	//{
	//	for (i = TableroGlobal.Hply - TableroGlobal.Regla_50_Movimiento; i < TableroGlobal.Hply; i++)
	//	{
	//		if (TableroGlobal.Historico[i] == TableroGlobal.Hash)
	//		{
	//			reps++;
	//			if (reps >= 2)
	//			{
	//				return true;
	//			}
	//		}
	//	}
	//}
	if (*en_jaque == false && TableroGlobal.Regla_50_Movimiento == 0 && Material_Insuficiente())
	{
		return true;
	}


#endif

	return false;
}
/* Comprobamos tiempo y entradas */
void ComprobarTiempo()
{
	char entrada[80];
	int longitud = 80;
	char *pchar = NULL;
	char *pchar2 = NULL;

	if (!(TipoJuego.Nodos % COMPROBAR_ENTRADAS))
	{
		if (TipoJuego.DepthAct > 1)
		{
			if (TipoJuego.Activo == true)
			{
				if ((ObtenerTiempo() - TipoJuego.Inicio) >= TipoJuego.Tiempo)
				{
					if (vp_terminada[0] != NO_MOVIMIENTO)
					{
						TipoJuego.Interrumpir = true;
					}
				}
			}
		}

		if (Entrada_StdIn())
		{
			memset(entrada, 0 , 80 * sizeof(char));
			if (fgets(entrada, longitud, stdin) == NULL)
			{
				Salir = true;
				return;
			}

			pchar = strchr(entrada, '\n');
			if (pchar != NULL)
			{
				entrada[pchar - entrada] = '\0';
			}
			pchar2 = strchr(entrada, '\r');
			if (pchar2 != NULL)
			{
				entrada[pchar2 - entrada] = '\0';
			}
			if (strcmp(entrada, "stop") == 0)
			{
				TipoJuego.Interrumpir = true;
			}
		}
	}
}

int FigurasAdversarioNull(int turno)
{
	if (turno == true)
	{
		return ((Blancas.TorresTotales + Blancas.DamasTotales + Blancas.AlfilTotales + Blancas.CaballosTotales) >= 1);
	}
	else
	{
		return ((Negras.TorresTotales + Negras.DamasTotales + Negras.AlfilTotales + Negras.CaballosTotales) >= 1);
	}

	return false;
}

/* Obtenemos si la puntuación esta dentro del margen de mate */
int EsPuntuacionMate(int puntos)
{
	if (puntos > MATE_EN( (MAX_PLY*2) ) && puntos != VALOR_MATE)
	{
		return true;
	}

	if (puntos < MATE( (MAX_PLY*2) ) && puntos != -VALOR_MATE)
	{
		return true;
	}

	return false;
}
