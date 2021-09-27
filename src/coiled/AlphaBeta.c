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
	int lmp[2][9];
#endif
#ifdef USAR_FUTILITY_PRUNING_HISTORY
	int HistoricoFutilityPoda[] = { 81, 10 };
#endif

int vp_triangular[MAX_PLY][MAX_PLY];
int vp_terminada_root[MAX_PLY];
int vp_terminada[MAX_PLY];
int vp_Ev[MAX_PLY];
int selDepth = 0;
int _lowerbound = 0;

void Iniciar_AlphaBeta()
{
	int dp;
	int mv;

#ifdef USAR_REDUCTION
	for (dp = 0; dp < 64; dp++)
	{
		for (mv = 0; mv < 64; mv++)
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
		lmp[0][dp] = 2.5 + 2 * dp * dp / 4.5;
		lmp[1][dp] = 4.0 + 4 * dp * dp / 4.5;

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
#ifdef USAR_TBSYZYGY
	Syzygy.Acierto = 0;
#endif
	for (i = 0; i < MAX_PLY; i++)
	{
		for (y = 0; y < MAX_PLY; y++)
		{
			vp_triangular[i][y] = 0;
		}
		vp_terminada_root[i] = 0;
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
			TT_Opciones.tt_Edad = 1;
	}
#endif
}

void Buscar()
{
	int _pv = 1;
	int en_jaque = false;
	int Puntos = 0;
	int PuntosAnteriores = 0;
	int x = 0;
	int MateEn = 0;

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
			if (vp_terminada[0] != NO_MOVIMIENTO)
			{
				break;
			}
		}

		_lowerbound = 0;

		Puntos = AspirationWindows(TipoJuego.DepthAct, en_jaque, Puntos);

		/* Actualizamos vp_terminada al terminar la interacción con vp_terminada_root, si no se ha interrumpido la búsqueda */
		if (TipoJuego.Interrumpir == false)
		{
			for (x = 0; vp_terminada_root[x] != NO_MOVIMIENTO; x++)
			{
				vp_terminada[x] = vp_terminada_root[x];
			}
			vp_terminada[x] = 0;
		}


		if (TipoJuego.DepthAct > 1)
		{
			if (TipoJuego.BuscarMate == 0 && EsPuntuacionMate(PuntosAnteriores) == true && EsPuntuacionMate(Puntos) == true && PuntosAnteriores == Puntos)
			{
				if (TipoJuego.Infinito == false)
				{
					break;
				}
			}

			if (TipoJuego.BuscarMate > 0 && EsPuntuacionMate(PuntosAnteriores) == true && EsPuntuacionMate(Puntos) == true && PuntosAnteriores == Puntos)
			{
				if (Puntos > 0)
				{
					if ((int)((float)((float)(VALOR_MATE - Puntos) / 2.0f) + 0.5f) == 0)
						MateEn = 1;
					else
						MateEn = (int)((float)((float)(VALOR_MATE - Puntos) / 2.0f) + 0.5f);
				}
				else
				{
					if ((int)((float)((float)(-VALOR_MATE - Puntos) / 2.0f) - 0.5f) == 0)
						MateEn = -1;
					else
						MateEn = (int)((float)((float)(-VALOR_MATE - Puntos) / 2.0f) - 0.5f);
				}
			}
		}

		if (TipoJuego.Interrumpir == false)
		{
			PuntosAnteriores = Puntos;
		}
		else
		{
			Puntos = PuntosAnteriores;
		}

		if (TipoJuego.Interrumpir == false)
		{
			_pv = TipoJuego.DepthAct;
			TipoJuego.MejorJugada = vp_terminada[0];
			TipoJuego.MejorJugadaAdv = vp_terminada[1];
			ImprimirVp(_pv, Puntos, _lowerbound);
		}

		if (MateEn != 0 && TipoJuego.BuscarMate >= ABS(MateEn))
			break;
	}

	ImprimirMejorJugada(TipoJuego.MejorJugada, TipoJuego.MejorJugadaAdv);
	A_Inicio(false);
}

int AspirationWindows(int depth, int en_jaque, int PuntuacionAnterior)
{
	int puntos = 0;

#ifdef ASPIRATION_WINDOWS
	int windows = ASPIRATION;
	int alpha =-VALOR_MATE, beta = VALOR_MATE;
	int depthOrg = depth;

	if (depth > 6)
	{
		alpha = MAX(-VALOR_MATE, PuntuacionAnterior - windows);
		beta = MIN(VALOR_MATE, PuntuacionAnterior + windows);
	}

	while (1)
	{
		puntos = AlphaBeta(depth, alpha, beta, en_jaque, false, NO_MOVIMIENTO);
		if (TipoJuego.Interrumpir == true)
		{
			return puntos;
		}
		if (puntos > alpha && puntos < beta)
		{
			_lowerbound = MAX(alpha, MIN(puntos, beta)) >= beta ? 1 : MAX(alpha, MIN(puntos, beta)) <= alpha ? -1 : 0;
			return puntos;
		}

		if (puntos <= alpha)
		{
			beta = (alpha + beta) / 2;
			alpha = MAX(-VALOR_MATE, alpha - windows);
			depth = depthOrg;
		}

		else if (puntos >= beta) 
		{
			beta = MIN(VALOR_MATE, beta + windows);
			depth = depth - (abs(puntos) <= VALOR_MATE / 2);
		}

		windows += ASPIRATION;
	}
#else
	puntos = AlphaBeta(depth, -VALOR_MATE, VALOR_MATE, en_jaque, false, NO_MOVIMIENTO);
	return puntos;
#endif
}

int AlphaBeta(int depth, int alpha, int beta, int en_jaque, int Es_Nulo, int SingularMov)
{
	int i = 0;
	_ST_Movimiento ListaMovimiento[MAX_JUGADAS];
	int NumeroDeMovimientos = 0;
	int mInquietos[MAX_JUGADAS / 2];
	int mInquietosTotal = 0;
	int MovimientosLegales = 0;
	int puntos = 0;
	int puntos_max = -VALOR_MATE;
	int da_jaque = false;
	int reducciones = 0;
	int Ply = TableroGlobal.Ply;
	int Root = (TableroGlobal.Ply == 0);
	int Zw = !(alpha != beta - 1);
	int nuevo_depth = 0;
	int vAlpha = alpha;
	int Mejor_Movimiento = NO_MOVIMIENTO;
	int Ev = 0;
	int EsMovimientoTranquilo = false;
	int EsMovimientoKiller = false;
	int EsMovimientoRefutacion = NO_MOVIMIENTO;
	int SaltarTranquilo = false;

#ifdef USAR_STATIC_EXCHANGE_EVALUATION_PRUNING
	int SeeMargen = 0;
	int Coronacion = 0;
	int PiezaMov = 0;
	int PiezaCap = 0;
#endif
#if defined(USAR_LATE_MOVE_PRUNING) || defined(USAR_FUTILITY_PRUNING_HISTORY) || defined(USAR_REDUCTION)
	int Mejorando = false;
#endif
#ifdef USAR_REDUCTION
	int Historico = 101;
#endif
	int hMov = NO_MOVIMIENTO;
	int hEv = VALOR_TB_VACIO;
#ifdef USAR_HASH_TB
	int hEncontrado = false;
	int hPuntos = -VALOR_MATE + (MAX_PLY * 2);
	int hFlag = TT_DESCONOCIDO;
	vp_Ev[Ply] = VALOR_TB_VACIO;
#endif
#if defined(USAR_TBPROBE) || defined(USAR_TBSYZYGY)
	int AccederTablasDeFinales = false;
	int NumeroFichas = 0;
	unsigned tbResultado = 0;
#endif

	selDepth = Root ? 0 : MAX(selDepth, Ply);
	if (Zw == false)
		vp_triangular[Ply][0] = NO_MOVIMIENTO;

	if (en_jaque && depth < 1)
		depth = MAX(1, depth);

	if (depth < 1)
		return AlphaBetaQs(alpha, beta);

	TipoJuego.Nodos++;

	ComprobarTiempo();
	if (TipoJuego.Interrumpir == true) return VALOR_EMPATE;

	if (Root == false)
	{
		if (EsTablaRepeticion() == true)
			return 1 - (TipoJuego.Nodos & 2);

		if (Ply > (MAX_PLY - 2))
			return Evaluar();

#ifdef USAR_MATE_DISTANCE_PRUNING
		alpha = alpha > MATE(Ply) ? alpha : MATE(Ply);
		beta = beta < MATE_EN(Ply+1) ? beta : MATE_EN(Ply+1);
		if (alpha >= beta)
			return alpha;
#endif
	}

#ifdef USAR_HASH_TB
	if ((hEncontrado = RecuperarPosicion(&hPuntos, &hEv, &hMov, &hFlag)) == true)
	{
		ConvertirValorTT(&hPuntos);

	#ifdef USAR_HASH_PRUNING
		if (Zw == true && DEPTH(hFlag) >= depth)
		{
			if (PodaHash(&hFlag, &beta, &alpha, &hPuntos) == true)
			{
				return hPuntos;
			}
		}
	#endif
	}
#endif
#ifdef USAR_TBPROBE
	/* Comprobamos acceso a las tablas de finales de gaviota, solo en pv */
	if (Gaviota.Usar == true && Gaviota.Dll_Cargada == true)
	{
		/* Cuantas piezas tenemos */
		NumeroFichas = Blancas.PeonTotales + Blancas.CaballosTotales + Blancas.AlfilTotales + Blancas.TorresTotales + Blancas.DamasTotales
			+ Negras.PeonTotales + Negras.CaballosTotales + Negras.AlfilTotales + Negras.TorresTotales + Negras.DamasTotales + 2; /* + 2 son los reyes */

		if ((Gaviota.Tablas_Disponibles & 1) != 0 && NumeroFichas == 3 && Gaviota.Limite >= NumeroFichas) /* Tenemos 3 piezas */
		{
			AccederTablasDeFinales = true;
		}
		if ((Gaviota.Tablas_Disponibles & 4) != 0 && NumeroFichas <= 4 && Gaviota.Limite >= NumeroFichas) /* Tenemos 4 piezas */
		{
			AccederTablasDeFinales = true;
		}
		if ((Gaviota.Tablas_Disponibles & 16) != 0 && NumeroFichas <= 5 && Gaviota.Limite >= NumeroFichas) /* Tenemos 5 piezas */
		{
			AccederTablasDeFinales = true;
		}
		/* Búsqueda. Acceso en depth 1 */
		if (AccederTablasDeFinales == true && Ply > 1 && depth <= 1)
		{
			puntos = 0;
			if ((tbResultado = Probar_gaviota(&puntos, &Ply)) != TBUNKNOWN)
			{
				Gaviota.Acierto++;

				if (puntos > 0)
					tbResultado = TB_WIN;
				if (puntos < 0)
					tbResultado = TB_LOSS;

				hPuntos = puntos < 0 ? -VALOR_TB + Ply
					: puntos > 0 ? VALOR_TB - Ply : 0;


				hFlag = tbResultado == TB_LOSS ? TT_ALPHA
					: tbResultado == TB_WIN ? TT_BETA : TT_EXACTO;

				if (hFlag == TT_EXACTO
					|| (hFlag == TT_BETA && hPuntos >= beta)
					|| (hFlag == TT_ALPHA && hPuntos <= alpha)) 
				{
					ConvertirValorTT(&hPuntos);
					AlmacenarPosicion(depth, hPuntos, VALOR_TB_VACIO, hFlag, TableroGlobal.Estado[Ply - 1].Movimiento);
					return hPuntos;
				}
			}
		}
		/* Root */
		if (AccederTablasDeFinales == true && Ply == 1 && TipoJuego.DepthAct == depth + 1)
		{
			puntos = 0;
			if (Probar_gaviota(&puntos, &Ply) != TBUNKNOWN)
			{
				Gaviota.Acierto++;
				return puntos;
			}
		}
	}
#endif
#ifdef USAR_TBSYZYGY
	/* Comprobamos acceso a las tablas de finales de gaviota, solo en pv */
	if (Syzygy.Usar == true && Syzygy.Dll_Cargada == true)
	{
		AccederTablasDeFinales = true;

		NumeroFichas = Blancas.PeonTotales + Blancas.CaballosTotales + Blancas.AlfilTotales + Blancas.TorresTotales + Blancas.DamasTotales
			+ Negras.PeonTotales + Negras.CaballosTotales + Negras.AlfilTotales + Negras.TorresTotales + Negras.DamasTotales + 2;

		if ((U64)NumeroFichas > *SG_man || NumeroFichas > Syzygy.Limite)
			AccederTablasDeFinales = false;
		/* Búsqueda. Acceso en depth 1 */
		if (AccederTablasDeFinales == true && Ply > 1 && depth <= 1)
		{
			if ((tbResultado = ProbarSyzygy(true, &hMov)) != SG_RESULT_FAILED)
			{
				Syzygy.Acierto++;

				hPuntos = tbResultado == SG_LOSS ? -VALOR_TB + Ply
					: tbResultado == SG_WIN ? VALOR_TB - Ply : 0;

				hFlag = tbResultado == SG_LOSS ? TT_ALPHA
					: tbResultado == SG_WIN ? TT_BETA : TT_EXACTO;

				if (hFlag == TT_EXACTO
					|| (hFlag == TT_BETA && hPuntos >= beta)
					|| (hFlag == TT_ALPHA && hPuntos <= alpha))
				{
					ConvertirValorTT(&hPuntos);
					AlmacenarPosicion(depth, hPuntos, VALOR_TB_VACIO, hFlag, TableroGlobal.Estado[Ply - 1].Movimiento);
					return hPuntos;
				}
			}
		}
		/* Root */
		if (AccederTablasDeFinales == true && TipoJuego.DepthAct == depth)
		{
			if ((tbResultado = ProbarSyzygy(false, &hMov)) != SG_RESULT_FAILED)
			{
				Syzygy.Acierto++;
				vp_terminada_root[0] = hMov;
				vp_terminada_root[1] = NO_MOVIMIENTO;
				selDepth = 1;
				TipoJuego.DepthAct = TipoJuego.MaxDepth;
				return 0;
			}
		}
	}
#endif

	Ev = vp_Ev[Ply] = (hEv != VALOR_TB_VACIO) ? hEv : Evaluar();

#if defined(USAR_LATE_MOVE_PRUNING) || defined(USAR_FUTILITY_PRUNING_HISTORY) || defined(USAR_REDUCTION)
	Mejorando = Ply > 1 && Ev > vp_Ev[Ply - 2];
#endif

#ifdef USAR_BETA_PRUNING
	if (Zw == true
		&& en_jaque == false
		&& depth <= 8
		&& Ev - (85 * depth) > beta)
		return Ev;
#endif
#ifdef USAR_NULL_MOVE_PRUNING
	if (Zw == true
		&& en_jaque == false
		&& Es_Nulo == false
		&& depth > 1
		&& Ev >= beta
		&& FigurasAdversarioNull(TableroGlobal.MueveBlancas)
		&& (!hEncontrado || FLAG(hFlag) == TT_BETA || hPuntos >= beta))
	{
		nuevo_depth = depth - (4 + depth / 6 + MIN(3, (Ev - beta) / 200));
		HacerMovimientoNull();
		puntos = -AlphaBeta(nuevo_depth, -beta, -beta + 1, en_jaque, true, NO_MOVIMIENTO);
		DeshacerMovimientoNull();

		if (puntos >= beta) return beta;
	}
#endif
#ifdef USAR_IDD
	if (hMov == NO_MOVIMIENTO && depth >= 4) depth -= 1;
#endif

	NumeroDeMovimientos = GenerarMovimientos(Todos, ListaMovimiento);
	OrdenarMovimientosTodos(&NumeroDeMovimientos, &hMov, ListaMovimiento);
	SaltarTranquilo = false;

	for (i = 0; i < NumeroDeMovimientos; i++)
	{
		OrdenaMovimiento(&i, &NumeroDeMovimientos, ListaMovimiento);

		EsMovimientoKiller = MovimientoKiller(&Ply, &ListaMovimiento[i].Movimiento);
		EsMovimientoRefutacion = HistoricoMovimientoRefutacion() == ListaMovimiento[i].Movimiento;
		EsMovimientoTranquilo = MovimientoTranquilo(&ListaMovimiento[i].Movimiento);

		if (SaltarTranquilo && (EsMovimientoTranquilo || EsMovimientoKiller || EsMovimientoRefutacion))
			continue;

#ifdef USAR_LATE_MOVE_PRUNING
		if (depth <= 8
			&& puntos_max > MATE((MAX_PLY * 2))
			&& i >= lmp[Mejorando][depth])
		{
			SaltarTranquilo = true;
		}
#endif

		Historico = EsMovimientoTranquilo == true ? HistoricoValor(&ListaMovimiento[i].Movimiento) : 101;

#ifdef USAR_FUTILITY_PRUNING_HISTORY
		if (depth <= 8
			&& puntos_max > MATE((MAX_PLY * 2))
			&& EsMovimientoTranquilo
			&& Ev + (65 * depth) <= alpha
			&& Historico < HistoricoFutilityPoda[Mejorando])
		{
			SaltarTranquilo = true;
		}
#endif

#ifdef USAR_STATIC_EXCHANGE_EVALUATION_PRUNING
		if (puntos_max > MATE((MAX_PLY * 2))
			&& depth <= 9
			&& ListaMovimiento[i].Ordenar < ORDENAR_CAPTURAS)
		{
			Coronacion = CORONACION(ListaMovimiento[i].Movimiento);
			PiezaMov = Coronacion == MFLAGPROM ? PIEZAMOVIDA(ListaMovimiento[i].Movimiento) : Coronacion;
			PiezaCap = ValorPieza(CAPTURADA(ListaMovimiento[i].Movimiento));
			SeeMargen = (EsMovimientoTranquilo) ? -64 * depth : -19 * depth * depth;

			if (Coronacion != MFLAGPROM)
				PiezaCap += ValorPieza(Coronacion) - ValorPieza(PeonB);
			else if (CAPTURA_ALPASO(ListaMovimiento[i].Movimiento) == MFLAGEP)
				PiezaCap = ValorPieza(PeonB);
			else if (ENROQUE(ListaMovimiento[i].Movimiento) == MFLAGCA)
				PiezaCap = 0;

			PiezaCap = PiezaCap - SeeMargen;
			if (PiezaCap < 0) continue;
			PiezaCap -= ValorPieza(PiezaMov);
			if (PiezaCap < 0)
				if (See(&ListaMovimiento[i].Movimiento, TableroGlobal.MueveBlancas) < 0)
					continue;
		}
#endif

		if (HacerMovimiento(ListaMovimiento[i].Movimiento, true) == false)
		{
			DeshacerMovimiento();
			continue;
		}

		nuevo_depth = depth;
		reducciones = 1;
		MovimientosLegales++;
		da_jaque = Jaque(TableroGlobal.MueveBlancas);
		if (EsMovimientoTranquilo == true)
			mInquietos[mInquietosTotal++] = ListaMovimiento[i].Movimiento;

		if (Root == true && ObtenerTiempo() - TipoJuego.Inicio > 4000) ExtraInfo(&ListaMovimiento[i].Movimiento, &MovimientosLegales);

#ifdef USAR_EXTENSIONS
		if (da_jaque && Root == false) nuevo_depth++;
#endif

#ifdef USAR_REDUCTION
		if (depth > 2 && MovimientosLegales > 1 && EsMovimientoTranquilo == true)
		{
			reducciones = lmr[MIN(depth, 63)][MIN(MovimientosLegales, 63)];

			reducciones += Zw + !Mejorando;
			reducciones += (en_jaque && ValorPieza(PIEZAMOVIDA(ListaMovimiento[i].Movimiento)) == SeeReyValor);

			reducciones -= (Historico > 0) ? 1 : (Historico < 0) ? -1 : 0;
			reducciones -= EsMovimientoKiller ? 1 : EsMovimientoRefutacion ? 1 : ListaMovimiento[i].Movimiento == hMov ? 1 : 0;

			reducciones = MIN(depth - 1, MAX(reducciones, 1));
		}
#endif

		if (MovimientosLegales == 1)
		{
			puntos = -AlphaBeta(nuevo_depth - 1, -beta, -alpha, da_jaque, false, NO_MOVIMIENTO);
		}
		else
		{
			puntos = -AlphaBeta(nuevo_depth - reducciones, -alpha-1, -alpha, da_jaque, false, NO_MOVIMIENTO);

			if (puntos > alpha && reducciones != 1)
				puntos = -AlphaBeta(nuevo_depth - 1, -alpha - 1, -alpha, da_jaque, false, NO_MOVIMIENTO);

			if (puntos > alpha && puntos < beta)
				puntos = -AlphaBeta(nuevo_depth - 1, -beta, -alpha, da_jaque, false, NO_MOVIMIENTO);
		}

		DeshacerMovimiento();

		if (puntos > puntos_max) {
			puntos_max = puntos;
			
			if (puntos > alpha) {
				alpha = puntos;
				Mejor_Movimiento = ListaMovimiento[i].Movimiento;
				
				if (Zw == false) {
					ActualizarVp(Mejor_Movimiento);
					if (Root == true) ActualizarVpRoot();
				}

				if (puntos >= beta) {
					if (MovimientoTranquilo(&Mejor_Movimiento) == true)
						HistoricoActualizar(depth, &Ply, Mejor_Movimiento, EsPuntuacionMate(puntos), mInquietos, mInquietosTotal);

					break;
				}
			}
		}
	}

	if (MovimientosLegales == 0) {
		if (en_jaque == true) {
			return MATE(Ply);
		}
		else {
			return VALOR_EMPATE;
		}
	}

#ifdef USAR_HASH_TB
	AlmacenarPosicion(depth, puntos_max, Ev, puntos_max >= beta
		? TT_BETA : puntos_max > vAlpha ? TT_EXACTO : TT_ALPHA, Mejor_Movimiento);
#endif

	return puntos_max;
}

/* Rutina Quiescence */
int AlphaBetaQs(int alpha, int beta)
{
	_ST_Movimiento ListaMovimiento[MAX_JUGADAS];
	int i = 0;
	int puntos = -VALOR_MATE;
	int NumeroDeMovimientos = 0;
	int puntos_max = -VALOR_MATE;
	int Ev = 0;
	int hMov = NO_MOVIMIENTO;
	int hEv = VALOR_TB_VACIO;
#ifdef USAR_HASH_PRUNING
	int hPuntos = 0;
	int hFlag = TT_DESCONOCIDO;
#endif

	TipoJuego.Nodos++;

	ComprobarTiempo();
	if (TipoJuego.Interrumpir == true) return VALOR_EMPATE;

	if (EsTablaRepeticion() == true) return 1 - (TipoJuego.Nodos & 2);

	if (TableroGlobal.Ply > (MAX_PLY - 2)) return Evaluar();

#ifdef USAR_HASH_PRUNING
	if (RecuperarPosicion(&hPuntos, &hEv, &hMov, &hFlag) == true)
	{
		ConvertirValorTT(&hPuntos);
		if (PodaHash(&hFlag, &beta, &alpha, &hPuntos) == true) return hPuntos;
	}
#endif

	Ev = (hEv != VALOR_TB_VACIO) ? hEv : Evaluar();
	
	/* Stand pat */
	puntos_max = Ev;
	alpha = MAX(alpha, Ev);
	if (alpha >= beta) return Ev;

	NumeroDeMovimientos = GenerarMovimientos(CapturasCoronacion, ListaMovimiento);
	OrdenarMovimientosTodos(&NumeroDeMovimientos, &hMov, ListaMovimiento);

	for (i = 0; i < NumeroDeMovimientos; i++)
	{
		OrdenaMovimiento(&i, &NumeroDeMovimientos, ListaMovimiento);

		if (HacerMovimiento(ListaMovimiento[i].Movimiento, true) == false)
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

#ifdef USAR_STATIC_EXCHANGE_EVALUATION_PRUNING
		if (puntos_max > MATE((MAX_PLY * 2))
			&& CORONACION(ListaMovimiento[i].Movimiento) == MFLAGPROM
			&& ValorPieza(PIEZAMOVIDA(ListaMovimiento[i].Movimiento)) != SeeReyValor
			&& ValorPieza(PIEZAMOVIDA(ListaMovimiento[i].Movimiento)) > ValorPieza(CAPTURADA(ListaMovimiento[i].Movimiento))
			&& See(&ListaMovimiento[i].Movimiento, TableroGlobal.MueveBlancas) < 0)
		{
			DeshacerMovimiento();
			continue;
		}
#endif

		puntos = -AlphaBetaQs(-beta, -alpha);

		DeshacerMovimiento();

		if (puntos > puntos_max)
		{
			puntos_max = puntos;
			if (puntos > alpha)
			{
				alpha = puntos;
				if (puntos >= beta) return puntos;
			}
		}
	}

	return puntos_max;
}

/* Actualiza la variante principal */
void ActualizarVp(int M)
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

void ActualizarVpRoot()
{
	int x = 0;
	for (x = 0; vp_triangular[0][x] != NO_MOVIMIENTO && x < MAX_PLY; x++)
	{
		vp_terminada_root[x] = vp_triangular[0][x];
	}
	vp_terminada_root[x] = NO_MOVIMIENTO;
}

/* Imprimimos la variante principal */
void ImprimirVp(int M_Depth, int puntos, int lowerbound)
{
	U64 A_Nps = 0;
	int pvi = 0;
	U64 tiempo = (ObtenerTiempo() - TipoJuego.Inicio);
	int mate = 0;

#if defined(USAR_TBPROBE) || defined(USAR_TBSYZYGY)
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

	TipoJuego.TiempoTranscurrido = ObtenerTiempo() - TipoJuego.Inicio;
	if (TipoJuego.TiempoTranscurrido != 0 && TipoJuego.Nodos != 0)
	{
		A_Nps = (U64)((float)TipoJuego.Nodos / (float)TipoJuego.TiempoTranscurrido) * 1000;
	}
	else
	{
		A_Nps = 0;
	}

	printf("info depth "S32_FORMAT" seldepth "S32_FORMAT"", M_Depth, selDepth);
	printf(" time "U64_FORMAT" nodes "U64_FORMAT" nps "U64_FORMAT" ", TipoJuego.TiempoTranscurrido, TipoJuego.Nodos, A_Nps);

#if defined(USAR_TBPROBE) || defined(USAR_TBSYZYGY)
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
#endif
#ifdef USAR_TBSYZYGY
	if (Syzygy.Usar == true)
	{
		tbhits = Syzygy.Acierto;
		#ifdef USAR_HASH_TB
		printf("tbhits "U64_FORMAT" hashfull "S32_FORMAT" score ", tbhits, ObtenerHashCompleto());
		#else
		printf("tbhits "U64_FORMAT" score ", tbhits);
		#endif
	}
#endif
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

	printf(" pv");
	for (pvi = 0; vp_terminada[pvi] != NO_MOVIMIENTO; pvi++)
	{
		printf(" ");
#ifdef USAR_AJEDREZ960
		if (TipoJuego.Ajedrez960 == true)
		{
			/* Formato GUI Arena Chess960 */
			if (ENROQUE(vp_terminada[pvi]) == MFLAGCA && TipoJuego.Ajedrez960Enroque == true)
			{
				if (CUADRADO_ORIGEN(vp_terminada[pvi]) < CUADRADO_DESTINO(vp_terminada[pvi]))
				{
					printf("O-O");
				}
				else
				{
					printf("O-O-O");
				}
			}
			else /* Formato estandar */
			{
				ImprimirMovimientoCoordenadas(CUADRADO_ORIGEN(vp_terminada[pvi]), CUADRADO_DESTINO(vp_terminada[pvi]), CORONACION(vp_terminada[pvi]));
			}
		}
		else
		{
#endif
			ImprimirMovimientoCoordenadas(CUADRADO_ORIGEN(vp_terminada[pvi]), CUADRADO_DESTINO(vp_terminada[pvi]), CORONACION(vp_terminada[pvi]));
#ifdef USAR_AJEDREZ960
		}
#endif
	}
	printf("\n");
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
int EsTablaRepeticion()
{
#ifdef USAR_HASH_TB
	int i = 0;
	int Repeticion = 0;

#endif

	if (TableroGlobal.Regla_50_Movimiento >= 100) return true;

#ifdef USAR_HASH_TB
	for (i = TableroGlobal.Hply - 2; i >= 0; i -= 2)
	{
		if (i < TableroGlobal.Hply - TableroGlobal.Regla_50_Movimiento)
			break;

		if (TableroGlobal.Historico[i] == TableroGlobal.Hash
			&& (i > TableroGlobal.Hply - TableroGlobal.Ply || ++Repeticion == 2))
			return true;
	}
#endif

	return EvaluarTablas();
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

		if (EntradaStdIn())
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
