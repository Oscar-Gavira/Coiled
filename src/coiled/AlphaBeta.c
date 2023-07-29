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

#include "AlphaBeta.h"

#ifdef USAR_REDUCTION
	int lmr[64][64];
#endif
#ifdef USAR_LATE_MOVE_PRUNING
	int lmp[2][9];
#endif

void Iniciar_AlphaBeta()
{
	int dp;
	int mv;

#ifdef USAR_REDUCTION
	for (dp = 0; dp < 64; dp++)
	{
		for (mv = 0; mv < 64; mv++)
		{
			lmr[dp][mv] = (int)(0.75f + log(dp) * log(mv) / 2.25f);
			if (lmr[dp][mv] < 1.0f) lmr[dp][mv] = 0;
		}
	}
#endif

#ifdef USAR_LATE_MOVE_PRUNING
	for (dp = 1; dp < 9; dp++)
	{
		lmp[0][dp] = (int)(2.5f + 2.0f * (float)dp * (float)dp / 4.5f);
		lmp[1][dp] = (int)(4.0f + 4.0f * (float)dp * (float)dp / 4.5f);
	}
#endif
}
void A_Inicio(int Reiniciar, _ST_TableroX64 *Tablero)
{
	memset(Tablero->Vp.vp_triangular, 0, sizeof(Tablero->Vp.vp_triangular));
	memset(Tablero->Vp.vp_root, 0, sizeof(Tablero->Vp.vp_root));
	memset(Tablero->Vp.vp_terminada, 0, sizeof(Tablero->Vp.vp_terminada));
	memset(Tablero->Vp.vp_Ev, 0, sizeof(Tablero->Vp.vp_Ev));
	memset(Tablero->Estado, 0, sizeof(Tablero->Estado));
	memset(Tablero->Historico_Repeticion, 0, sizeof(Tablero->Historico_Repeticion));

	HistoricoIniciar(Tablero);

	Tablero->selDepth = 0;
	Tablero->Puntos = -VALOR_MATE;
	Tablero->MueveBlancas = true;
	Tablero->Regla_50_Movimiento = 0;
	Tablero->Etapa = 0;
	Tablero->DepthAct = 0;
	Tablero->Ply = 0;
	Tablero->Hply = 0;
	Tablero->Nodos = 0;

	if (Tablero->IdSubProcesos == 0)
	{
#ifdef USAR_HASH_TB
		if (Reiniciar == true)
		{
			LimpiarTransposicion();
		}
		else
		{
			TT_Opciones.tt_Edad++;
		}
#endif
		TipoJuego.Activo = 0;
		TipoJuego.TiempoIdeal = 0;
		TipoJuego.TiempoMed = 0;
		TipoJuego.TiempoMax = 0;
		TipoJuego.TiempoFactor = 0;
		TipoJuego.Inicio = 0;
		TipoJuego.MaxDepth = 0;
		TipoJuego.XTiempo = 0;
		TipoJuego.TiempoConsumido = 0;
		TipoJuego.BuscarMate = 0;
		TipoJuego.SubProcesosActivo = false;
	}
}
void Buscar(void *arg)
{
	_ST_TableroX64 *Tablero = (_ST_TableroX64*)arg;
	int en_jaque = false;
	int Puntos = -VALOR_TB_VACIO;
	int x = 0;
	int MateEn = 0;
	int Depth = 1;

	Tablero->DepthAct = 0;
	Tablero->Puntos = -VALOR_TB_VACIO;
	en_jaque = Jaque(Tablero, Tablero->MueveBlancas);

	for (; Depth <= TipoJuego.MaxDepth; Depth++)
	{

		Puntos = AspirationWindows(Tablero, Depth, en_jaque, Tablero->Puntos);

		if (TipoJuego.Interrumpir == true) break;

		if (TipoJuego.Activo == 1 && Depth >= 4 && Tablero->IdSubProcesos == 0)
			TiempoActualizar(&TipoJuego, Puntos, Tablero->Puntos, Tablero->Vp.vp_root[0], Tablero->Vp.vp_terminada[0]);

		for (x = 0; Tablero->Vp.vp_root[x] != NO_MOVIMIENTO; x++)
		{
			Tablero->Vp.vp_terminada[x] = Tablero->Vp.vp_root[x];
		}
		Tablero->Vp.vp_terminada[x] = 0;

		if (TipoJuego.BuscarMate > 0 && EsPuntuacionMate(Tablero->Puntos) == true && EsPuntuacionMate(Puntos) == true && Tablero->Puntos == Puntos)
		{
			MateEn = (VALOR_MATE - ABS(Puntos) + 1) / 2 * (Puntos > 0 ? 1 : -1);
			if (TipoJuego.BuscarMate == ABS(MateEn))
				if (Tablero->IdSubProcesos == 0) TipoJuego.Interrumpir = true;
		}

		Tablero->Puntos = Puntos;
		Tablero->DepthAct = Depth;

		if (Tablero->IdSubProcesos == 0)
		{
			ImprimirVp(Tablero, Puntos);

			if (TipoJuego.Activo == 1)
				TipoJuego.Interrumpir = TerminarTiempoIdeal(&TipoJuego);
			if (TipoJuego.Activo == 2)
				TipoJuego.Interrumpir = TiempoTranscurrido(&TipoJuego) >= TipoJuego.TiempoMed;
		}
		if (TipoJuego.Interrumpir == true)
			break;
	}
}
int AspirationWindows(_ST_TableroX64 *Tablero, int depth, int en_jaque, int PuntuacionAnterior)
{
#ifdef ASPIRATION_WINDOWS
	int ventana = ASPIRATION;
	int alpha = -VALOR_MATE, beta = VALOR_MATE;
	int depthOrg = depth;
	int puntos = 0;
	int MiNULL = 0;

	if (depth > 6)
	{
		alpha = MAX(-VALOR_MATE, PuntuacionAnterior - ventana);
		beta = MIN(VALOR_MATE, PuntuacionAnterior + ventana);
	}

	while (true)
	{
		puntos = AlphaBeta(Tablero, depth, alpha, beta, en_jaque, &MiNULL, false);

		if (puntos > alpha && puntos < beta) return puntos;

		if (puntos <= alpha)
		{
			beta = (int)((float)(alpha + beta) * 0.5f);
			alpha = MAX(-VALOR_MATE, alpha - ventana);
			depth = depthOrg;
		}
		else if (puntos >= beta) 
		{
			beta = MIN(VALOR_MATE, beta + ventana);
			depth = MAX(1, depth - 1);
		}

		ventana = ventana + ventana / 2;
		if (TipoJuego.Interrumpir == true) return puntos;
	}
#else
	return AlphaBeta(Tablero, depth, -VALOR_MATE, VALOR_MATE, en_jaque, &MiNULL, false);
#endif
}
int AlphaBeta(_ST_TableroX64 *Tablero, int depth, int alpha, int beta, int en_jaque, int *sReduccion, int EsSingular)
{
	int i = 0;
	_ST_Movimiento ListaMovimiento;
	int mInquietos[MAX_JUGADAS] = {0};
	int mInquietosTotal = 0;
	int mCapturas[MAX_JUGADAS] = {0};
	int mCapturasTotal = 0;
	int MovimientosLegales = 0;
	int puntos = -VALOR_TB_VACIO;
	int puntos_max = -VALOR_TB_VACIO;
	int da_jaque = false;
	int reducciones = 0;
	int Ply = Tablero->Ply;
	int Zw = !(alpha != beta - 1);
	int nuevo_depth = 0;
	int vAlpha = alpha;
	int Mejor_Movimiento = NO_MOVIMIENTO;
	int Ev = -VALOR_TB_VACIO;
	int EsMovimientoTranquilo = false;
	int EsMovimientoKiller = false;
	int EsMovimientoRefutacion = false;
	int SaltarTranquilo = false;
	int mAlpha, mBeta;
	int hMov = NO_MOVIMIENTO;
	int hEv = VALOR_TB_VACIO;
#ifdef USAR_HASH_TB
	int hEncontrado = false;
	int hPuntos = VALOR_TB_VACIO;
	int hFlag = TT_DESCONOCIDO;
#endif
#ifdef USAR_STATIC_EXCHANGE_EVALUATION_PRUNING
	int SeeMargen = 0;
	int PiezaMov = 0;
	int ValorPiezaCap = 0;
#endif
#if defined(USAR_LATE_MOVE_PRUNING) || defined(USAR_REDUCTION)
	int Mejorando = false;
#endif
#ifdef USAR_REDUCTION
	int Historico = 0;
#endif

	int Refu = NO_MOVIMIENTO;
	int sExtension = 0;
	int SingularBeta = 0;
	int MiNULL = 0;

	if (Zw == false)
	{
		Tablero->selDepth = MAX(Tablero->selDepth, Ply);
		Tablero->Vp.vp_triangular[Ply][0] = NO_MOVIMIENTO;
	}

	if (!en_jaque && depth < 1)
		return AlphaBetaQs(Tablero, alpha, beta);

	depth = MAX(1, depth);
	Tablero->Nodos++;
	if (Tablero->IdSubProcesos == 0) ComprobarTiempo(Tablero);
	if (TipoJuego.Interrumpir == true) return VALOR_EMPATE;

	if (Tablero->Ply != 0)
	{
		if (EsTabla(Tablero, en_jaque) == true)
			return 1 - (Tablero->Nodos & 2);

		if (Tablero->Ply > (MAX_PLY - 2))
			return Evaluar(Tablero);

#ifdef USAR_MATE_DISTANCE_PRUNING
		mAlpha = alpha > -VALOR_MATE + Ply ? alpha : -VALOR_MATE + Ply;
		mBeta = beta < VALOR_MATE - Ply ? beta : VALOR_MATE - Ply - 1;
		if (mAlpha >= mBeta)
			return mAlpha;
#endif
	}

#ifdef USAR_HASH_TB
	if (EsSingular == false && (hEncontrado = RecuperarPosicion(Tablero, &hPuntos, &hEv, &hMov, &hFlag)) == true)
	{
		ConvertirValorTT(Tablero, &hPuntos);
#ifdef USAR_HASH_PRUNING
		if (Zw && DEPTH(hFlag) >= depth)
		{
			if (PodaHash(&hFlag, &beta, &alpha, &hPuntos) == true)
			{
				return hPuntos;
			}
		}
#endif
	}
#endif

	Ev = Tablero->Vp.vp_Ev[Ply] = hEncontrado ? hEv : Evaluar(Tablero);

#if defined(USAR_LATE_MOVE_PRUNING) || defined(USAR_REDUCTION)
	Mejorando = Ply <= 1 ? true : Tablero->Vp.vp_Ev[Ply] > Tablero->Vp.vp_Ev[Ply - 2];
#endif

	if (Zw == true && !en_jaque)
	{
#ifdef USAR_BETA_PRUNING
		if (depth <= 8
			&& EsSingular == false
			&& Ev - (50 * depth) > beta)
			return Ev;
#endif

#ifdef USAR_NULL_MOVE_PRUNING
		if (depth > 1
			&& Ev >= beta
			&& EsSingular == false
			&& FigurasAdversarioNull(Tablero)
			&& Ply > 0
			&& Tablero->Estado[Ply - 1].Movimiento != NO_MOVIMIENTO
			&& (!hEncontrado || FLAG(hFlag) == TT_BETA || hPuntos >= beta))
		{
			nuevo_depth = depth - (4 + depth / 6 + MIN(3, (Ev - beta) / 200));
			Tablero->Vp.vp_Ev[Ply - 1] = -Tablero->Vp.vp_Ev[Ply - 1];
			HacerMovimientoNull(Tablero);
			puntos = -AlphaBeta(Tablero, nuevo_depth, -beta, -beta + 1, en_jaque, &MiNULL, false);
			DeshacerMovimientoNull(Tablero);
			Tablero->Vp.vp_Ev[Ply - 1] = -Tablero->Vp.vp_Ev[Ply - 1];

			if (puntos >= beta)
				return puntos < VALOR_MATE_MAX ? puntos : beta;
		}
#endif
	}

#ifdef USAR_IDD
	if (EsSingular == false && hMov == NO_MOVIMIENTO && depth >= 4) depth -= 1;
#endif

	GenerarMovimientos(Tablero, Todos, &ListaMovimiento);
	OrdenarMovimientosPorValor(&hMov, &ListaMovimiento, Tablero);
	Refu = HistoricoMovimientoRefutacion(Tablero);

	for (i = 0; i < ListaMovimiento.CantidadDeMovimiento; i++)
	{
		ObtenerMovimiento(&i, &ListaMovimiento, Tablero);

		if (EsSingular == ListaMovimiento.Movimiento[i])
			continue;

		EsMovimientoKiller = MovimientoKiller(&ListaMovimiento.Movimiento[i], Tablero);
		EsMovimientoRefutacion = Refu == ListaMovimiento.Movimiento[i];
		EsMovimientoTranquilo = MovimientoTranquilo(&ListaMovimiento.Movimiento[i]);

#ifdef USAR_LATE_MOVE_PRUNING
		if (SaltarTranquilo == true && EsMovimientoTranquilo == true)
			continue;
#endif

#ifdef USAR_LATE_MOVE_PRUNING
		if (puntos_max > -VALOR_MATE_MAX
			&& depth <= 8
			&& i >= lmp[Mejorando][depth])
		{
			SaltarTranquilo = true;
		}
#endif

#ifdef USAR_FUTILITY_PRUNING
		if (puntos_max > -VALOR_MATE_MAX
			&& depth <= 9
			&& EsMovimientoTranquilo == true
			&& en_jaque == false
			&& Ev + (85 * depth) <= alpha)
		{
			SaltarTranquilo = true;
		}
#endif

#ifdef USAR_REDUCTION
		sExtension = 0;
		Historico = EsMovimientoTranquilo == true ? HistoricoValor(&ListaMovimiento.Movimiento[i], Tablero) : 0;
#endif

#ifdef USAR_STATIC_EXCHANGE_EVALUATION_PRUNING
		if (puntos_max > -VALOR_MATE_MAX
			&& depth <= 9	/* 3 */
			&& CAPTURADA(ListaMovimiento.Movimiento[i]) != MFLAGCAP
			&& ListaMovimiento.Ordenar[i] < ORDENAR_REFUTACION)
		{
			PiezaMov = ValorPieza(PIEZAMOVIDA(ListaMovimiento.Movimiento[i]));
			ValorPiezaCap = ValorPieza(CAPTURADA(ListaMovimiento.Movimiento[i]));
			SeeMargen = 23 * depth * depth;

			ValorPiezaCap = ValorPiezaCap - PiezaMov + SeeMargen;
			if (ValorPiezaCap < 0)
				continue;

			//if (ListaMovimiento.Ordenar[i] < 0
			//	&& (ListaMovimiento.Ordenar[i] + ORDENAR_CAPTURAS) < -250 * depth)
			//	continue;
			//else if (ListaMovimiento.Ordenar[i] < 10000
			//	&& (ListaMovimiento.Ordenar[i] - 10000) < -250 * depth)
			//	continue;
			//else if (ListaMovimiento.Ordenar[i] < 15000
			//	&& (ListaMovimiento.Ordenar[i] - 15000) < -250 * depth)
			//	continue;
			//else if (ListaMovimiento.Ordenar[i] < 20000
			//	&& (ListaMovimiento.Ordenar[i] - 20000) < -250 * depth)
			//	continue;
		}
#endif

#ifdef USAR_EXTENSIONS
		if (*sReduccion > 1
			&& depth >= 6
			&& en_jaque == false
			&& hEncontrado == true
			&& ListaMovimiento.Movimiento[i] == hMov
			&& ABS(hPuntos) < VALOR_MATE_MAX
			&& DEPTH(hFlag) >= depth
			&& FLAG(hFlag) != TT_ALPHA)
		{
			SingularBeta = MIN(hPuntos - (depth * 2), beta);

			puntos = AlphaBeta(Tablero, depth / 2, SingularBeta - 1, SingularBeta, en_jaque, &MiNULL, ListaMovimiento.Movimiento[i]);

			if (puntos < SingularBeta)
			{
				depth += *sReduccion;
				*sReduccion = 1;
				sExtension = 1;
			}
			else if (puntos >= beta)
			{
				return puntos < VALOR_MATE_MAX ? puntos : beta;
			}
			else if (hEncontrado && hPuntos >= beta)
			{
				sExtension = -1;
			}
		}
#endif

		if (HacerMovimiento(Tablero, &ListaMovimiento.Movimiento[i], true) == false)
		{
			DeshacerMovimiento(Tablero);
			continue;
		}

		nuevo_depth = depth + sExtension;
		reducciones = 1;
		MovimientosLegales++;
		da_jaque = Jaque(Tablero, Tablero->MueveBlancas);
		if (EsMovimientoTranquilo == true)
		{
			mInquietos[mInquietosTotal++] = ListaMovimiento.Movimiento[i];
		}
		else
		{
			if (CAPTURADA(ListaMovimiento.Movimiento[i]) != MFLAGCAP || CORONACION(ListaMovimiento.Movimiento[i]) != MFLAGPROM)
				mCapturas[mCapturasTotal++] = ListaMovimiento.Movimiento[i];
		}

#ifdef USAR_EXTENSIONS
		if (Tablero->Ply != 0 && da_jaque == true && sExtension == 0)
			nuevo_depth++;
#endif

#ifdef USAR_REDUCTION
		if (depth > 2 && MovimientosLegales > 1 && EsMovimientoTranquilo == true)
		{
			reducciones = lmr[MIN(depth, 63)][MIN(MovimientosLegales, 63)];
			reducciones += (Zw == true);
			reducciones += (Mejorando == false);
			reducciones += (Historico < 0);

			reducciones -= (Historico > 0);
			reducciones -= EsMovimientoKiller == true ? 1 : EsMovimientoRefutacion == true ? 1 : 0;

			reducciones = MIN(depth - 1, MAX(reducciones, 1));
		}
#endif

		if (MovimientosLegales == 1)
		{
			puntos = -AlphaBeta(Tablero, nuevo_depth - 1, -beta, -alpha, da_jaque, &MiNULL, false);
		}
		else
		{
			puntos = -AlphaBeta(Tablero, nuevo_depth - reducciones, -alpha - 1, -alpha, da_jaque, &reducciones, false);

			if (puntos > alpha && reducciones != 1)
				puntos = -AlphaBeta(Tablero, nuevo_depth - 1, -alpha - 1, -alpha, da_jaque, &MiNULL, false);

			if (puntos > alpha && (Tablero->Ply == 0 || puntos < beta))
				puntos = -AlphaBeta(Tablero, nuevo_depth - 1, -beta, -alpha, da_jaque, &MiNULL, false);
		}

		DeshacerMovimiento(Tablero);

		if (puntos > puntos_max) {
			if (EsMovimientoTranquilo == true && SaltarTranquilo == true) SaltarTranquilo = false;
			puntos_max = puntos;

			if (puntos > alpha) {
				alpha = puntos;
				Mejor_Movimiento = ListaMovimiento.Movimiento[i];

				if (Zw == false) {
					ActualizarVp(Mejor_Movimiento, Tablero);
					if (Tablero->Ply == 0) ActualizarVpRoot(Tablero);
				}

				if (puntos >= beta) {
					if (EsMovimientoTranquilo == true) HistoricoActualizar(depth, &Ply, Mejor_Movimiento, EsPuntuacionMate(puntos), mInquietos, mInquietosTotal, Tablero);
					HistoricoActualizarCapturas(depth, Mejor_Movimiento, mCapturas, mCapturasTotal, Tablero);
					break;
				}
			}
		}
	}

	if (MovimientosLegales == 0 && Ply != 0) {
		if (en_jaque == true) {
			return -VALOR_MATE + Ply;
		}
		else {
			return VALOR_EMPATE;
		}
	}

#ifdef USAR_HASH_TB
	if (puntos_max != -VALOR_TB_VACIO && EsSingular == false)
	{
		hFlag = puntos_max >= beta ? TT_BETA : puntos_max > vAlpha ? TT_EXACTO : TT_ALPHA;
		AlmacenarPosicion(Tablero, depth, puntos_max, Ev, hFlag, Mejor_Movimiento);
	}
#endif

	return puntos_max;
}
int AlphaBetaQs(_ST_TableroX64 *Tablero, int alpha, int beta)
{
	_ST_Movimiento ListaMovimiento;
	int i = 0;
	int puntos = -VALOR_MATE;
	int puntos_max = -VALOR_MATE;
	int vAlpha = alpha;
	int Mejor_Movimiento = NO_MOVIMIENTO;
	int Ev = 0;
	int hMov = NO_MOVIMIENTO;
	int hEv = VALOR_TB_VACIO;
#ifdef USAR_HASH_TB
	int hPuntos = VALOR_TB_VACIO;
	int hFlag = TT_DESCONOCIDO;
	int hEncontrado = false;
#endif

	Tablero->Nodos++;
	if (Tablero->IdSubProcesos == 0) ComprobarTiempo(Tablero);
	if (TipoJuego.Interrumpir == true) return VALOR_EMPATE;

	if (EsTabla(Tablero, false) == true)
		return 1 - (Tablero->Nodos & 2);

	if (Tablero->Ply > MAX_PLY - 1)
		return Evaluar(Tablero);

#ifdef USAR_HASH_TB
	if ((hEncontrado = RecuperarPosicion(Tablero, &hPuntos, &hEv, &hMov, &hFlag)) == true)
	{
		ConvertirValorTT(Tablero, &hPuntos);
#ifdef USAR_HASH_PRUNING
		if (PodaHash(&hFlag, &beta, &alpha, &hPuntos) == true)
			return hPuntos;
#endif
	}
#endif

	Ev = (hEncontrado) ? hEv : Evaluar(Tablero);

	puntos_max = Ev;
	alpha = MAX(alpha, Ev);
	if (alpha >= beta)
		return Ev;

	GenerarMovimientos(Tablero, QsCapturas, &ListaMovimiento);
	OrdenarMovimientosPorValor(&hMov, &ListaMovimiento, Tablero);

	for (i = 0; i < ListaMovimiento.CantidadDeMovimiento; i++)
	{
		ObtenerMovimiento(&i, &ListaMovimiento, Tablero);

		if (HacerMovimiento(Tablero, &ListaMovimiento.Movimiento[i], true) == false)
		{
			DeshacerMovimiento(Tablero);
			continue;
		}

#ifdef USAR_STATIC_EXCHANGE_EVALUATION_PRUNING
		if (puntos_max > -VALOR_MATE_MAX
			&& CORONACION(ListaMovimiento.Movimiento[i]) == MFLAGPROM
			&& ValorPieza(PIEZAMOVIDA(ListaMovimiento.Movimiento[i])) != SeeReyValor
			&& ValorPieza(PIEZAMOVIDA(ListaMovimiento.Movimiento[i])) > ValorPieza(CAPTURADA(ListaMovimiento.Movimiento[i]))
			&& ListaMovimiento.Ordenar[i] < ORDENAR_REFUTACION)
		{
			DeshacerMovimiento(Tablero);
			continue;
		}
#endif
		puntos = -AlphaBetaQs(Tablero, -beta, -alpha);

		DeshacerMovimiento(Tablero);

		if (puntos > puntos_max)
		{
			puntos_max = puntos;
			if (puntos > alpha)
			{
				alpha = puntos;
				Mejor_Movimiento = ListaMovimiento.Movimiento[i];

				if (puntos >= beta)
					break;
			}
		}
	}

#ifdef USAR_HASH_TB
	hFlag = puntos_max >= beta ? TT_BETA : puntos_max > vAlpha ? TT_EXACTO : TT_ALPHA;
	AlmacenarPosicion(Tablero, 0, puntos_max, Ev, hFlag, Mejor_Movimiento);
#endif

	return puntos_max;
}
void ActualizarVp(int M, _ST_TableroX64 *Tablero)
{
	int i = 0;

	if (Tablero->Ply < MAX_PLY - 1)
	{
		Tablero->Vp.vp_triangular[Tablero->Ply][0] = M;
		for (i = 1; Tablero->Vp.vp_triangular[Tablero->Ply + 1][i - 1] != NO_MOVIMIENTO; i++)
		{
			Tablero->Vp.vp_triangular[Tablero->Ply][i] = Tablero->Vp.vp_triangular[Tablero->Ply + 1][i - 1];
			if (Tablero->Vp.vp_triangular[Tablero->Ply][i] == NO_MOVIMIENTO)
				break;
		}

		Tablero->Vp.vp_triangular[Tablero->Ply][i] = NO_MOVIMIENTO;
		Tablero->Vp.vp_triangular[Tablero->Ply + 1][0] = NO_MOVIMIENTO;
	}
}
void ActualizarVpRoot(_ST_TableroX64 *Tablero)
{
	int x = 0;
	for (x = 0; Tablero->Vp.vp_triangular[0][x] != NO_MOVIMIENTO; x++)
	{
		Tablero->Vp.vp_root[x] = Tablero->Vp.vp_triangular[0][x];
	}
	Tablero->Vp.vp_root[x] = NO_MOVIMIENTO;
}
void ImprimirVp(_ST_TableroX64 *Tablero, int puntos)
{
	TipoJuego.TiempoConsumido = TiempoTranscurrido(&TipoJuego) + 1;
	U64 A_Nps = (U64)((Tablero->Nodos * TipoJuego.NumeroDeSubProcesos) / (TipoJuego.TiempoConsumido / 1000.000f));
	int pvi = 0;
	int mate = 0;

	printf("info depth "S32_FORMAT" seldepth "S32_FORMAT" time "S32_FORMAT" nodes "U64_FORMAT" nps "U64_FORMAT" ", Tablero->DepthAct, Tablero->selDepth, TipoJuego.TiempoConsumido, (Tablero->Nodos * TipoJuego.NumeroDeSubProcesos), A_Nps);

	printf("hashfull "S32_FORMAT" ", ObtenerHashCompleto());

	if (EsPuntuacionMate(puntos) == false)
	{
			printf("score cp "S32_FORMAT" ", puntos);
	}
	else
	{
		mate = (VALOR_MATE - ABS(puntos) + 1) / 2 * (puntos > 0 ? 1 : -1);
		printf("score mate "S32_FORMAT" ", mate);
	}
	printf("pv");
	for (pvi = 0; Tablero->Vp.vp_terminada[pvi] != NO_MOVIMIENTO; pvi++)
	{
		printf(" ");
#ifdef USAR_AJEDREZ960
		if (TipoJuego.Ajedrez960 == true)
		{
			ImprimirMovimientoCoordenadas(CUADRADO_ORIGEN(Tablero->Vp.vp_terminada[pvi]), CUADRADO_DESTINO(Tablero->Vp.vp_terminada[pvi]), CORONACION(Tablero->Vp.vp_terminada[pvi]));
		}
		else
		{
#endif
			ImprimirMovimientoCoordenadas(CUADRADO_ORIGEN(Tablero->Vp.vp_terminada[pvi]), CUADRADO_DESTINO(Tablero->Vp.vp_terminada[pvi]), CORONACION(Tablero->Vp.vp_terminada[pvi]));
#ifdef USAR_AJEDREZ960
		}
#endif
	}
	printf("\n");
	fflush(stdout);
}
void ImprimirMejorJugada(int Jugada, int J_Adv)
{
	printf("bestmove ");
	if (Jugada != NO_MOVIMIENTO)
	{
#ifdef USAR_AJEDREZ960
		if (TipoJuego.Ajedrez960 == true)
		{
			ImprimirMovimientoCoordenadas(CUADRADO_ORIGEN(Jugada), CUADRADO_DESTINO(Jugada), CORONACION(Jugada));
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
			ImprimirMovimientoCoordenadas(CUADRADO_ORIGEN(J_Adv), CUADRADO_DESTINO(J_Adv), CORONACION(J_Adv));
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
int EsTabla(_ST_TableroX64 *Tablero, int en_jaque)
{
#ifdef USAR_HASH_TB
	int i = 0;
	int Repeticion = 0;

#endif
	if (Tablero->Regla_50_Movimiento >= 100 && en_jaque == false)
		return true;

#ifdef USAR_HASH_TB
	for (i = Tablero->Hply - 2; i >= 0; i -= 2)
	{
		if (i < Tablero->Hply - Tablero->Regla_50_Movimiento)
			break;

		if (Tablero->Historico_Repeticion[i] == Tablero->Hash
			&& (i > Tablero->Hply - Tablero->Ply || ++Repeticion == 2))
			return true;
	}

	return EvaluarTablas(Tablero);
#endif
}
void ComprobarTiempo(_ST_TableroX64 *Tablero)
{
	if (!(Tablero->Nodos & COMPROBAR_ENTRADAS))
		TerminarTiempoLimite(&TipoJuego);
}
int FigurasAdversarioNull(_ST_TableroX64 *Tablero)
{
	if (Tablero->MueveBlancas == true)
	{
		return ((Tablero->Blancas.TorresTotales + Tablero->Blancas.DamasTotales + Tablero->Blancas.AlfilTotales + Tablero->Blancas.CaballosTotales) >= 1);
	}
	else
	{
		return ((Tablero->Negras.TorresTotales + Tablero->Negras.DamasTotales + Tablero->Negras.AlfilTotales + Tablero->Negras.CaballosTotales) >= 1);
	}

	return false;
}
int EsPuntuacionMate(int puntos)
{
	if (puntos > VALOR_MATE_MAX && puntos < VALOR_TB_VACIO)
	{
		return true;
	}

	if (puntos < -VALOR_MATE_MAX && puntos > -VALOR_TB_VACIO)
	{
		return true;
	}

	return false;
}
