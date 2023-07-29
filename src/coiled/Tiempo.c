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

#include "Tiempo.h"

int ObtenerTiempo()
{
#if defined(_WIN32)
    return (int)GetTickCount();
#else
    struct timeval tv;

    gettimeofday(&tv, NULL);
    return (int)(tv.tv_sec * 1000 + tv.tv_usec / 1000);
#endif
}
int TiempoTranscurrido(_ST_TipoJuego *Tiempo)
{
    return ObtenerTiempo() - Tiempo->Inicio;
}
void TiempoInicio(_ST_TipoJuego *Tiempo, int Movestogo, int TiempoTotal, int inc)
{
    Tiempo->TiempoFactor = 0;

   if (Movestogo != 0)
    {
        Movestogo += 1;
        Tiempo->TiempoIdeal = (int)(0.67f * (float)((TiempoTotal - Tiempo->PrevenirTiempoExcedido) / Movestogo) + inc);
        Tiempo->TiempoMed = (int)(1.20f * (float)((TiempoTotal - Tiempo->PrevenirTiempoExcedido) / Movestogo) + inc);
        Tiempo->TiempoMax = (int)(1.80f * (float)((TiempoTotal - Tiempo->PrevenirTiempoExcedido) / Movestogo) + inc);
    }
    else {
        Tiempo->TiempoIdeal = (int)(0.85f * (float)((TiempoTotal - Tiempo->PrevenirTiempoExcedido) + 25 * inc) / 50);
        Tiempo->TiempoMed = (int)(1.90f * (float)((TiempoTotal - Tiempo->PrevenirTiempoExcedido) + 25 * inc) / 50);
        Tiempo->TiempoMax = (int)(2.55f * (float)((TiempoTotal - Tiempo->PrevenirTiempoExcedido) + 25 * inc) / 50);
    }

    Tiempo->TiempoIdeal = MIN(Tiempo->TiempoIdeal, TiempoTotal - Tiempo->PrevenirTiempoExcedido);
    Tiempo->TiempoMed = MIN(Tiempo->TiempoMed, TiempoTotal - Tiempo->PrevenirTiempoExcedido * 2);
    Tiempo->TiempoMax = MIN(Tiempo->TiempoMax, TiempoTotal - Tiempo->PrevenirTiempoExcedido * 3);
}
void TiempoActualizar(_ST_TipoJuego *Tiempo, int Valor, int ValorAnterior, int MejorJugada, int MejorJugadaAnterior)
{
    if (ValorAnterior > Valor + 10) Tiempo->TiempoIdeal = Tiempo->TiempoIdeal * 1.050f;
    if (ValorAnterior > Valor + 20) Tiempo->TiempoIdeal = Tiempo->TiempoIdeal * 1.050f;
    if (ValorAnterior > Valor + 40) Tiempo->TiempoIdeal = Tiempo->TiempoIdeal * 1.050f;

    if (ValorAnterior + 15 < Valor) Tiempo->TiempoIdeal = Tiempo->TiempoIdeal * 1.050f;
    if (ValorAnterior + 30 < Valor) Tiempo->TiempoIdeal = Tiempo->TiempoIdeal * 1.050f;

    Tiempo->TiempoFactor = MAX(0, Tiempo->TiempoFactor - 1);
    if (MejorJugada != MejorJugadaAnterior) Tiempo->TiempoFactor = 3;
}
int TerminarTiempoIdeal(_ST_TipoJuego *Tiempo)
{
    int TiempoEntreDepth = 0;
    int FactorPvValor = FactorPvValor = (Tiempo->TiempoMax - Tiempo->TiempoIdeal) / 3;
    int FactorPv = Tiempo->TiempoIdeal + Tiempo->TiempoFactor * FactorPvValor;

    Tiempo->TiempoConsumido = TiempoTranscurrido(Tiempo);
    TiempoEntreDepth = Tiempo->TiempoConsumido - Tiempo->XTiempo;
    Tiempo->XTiempo = Tiempo->TiempoConsumido;

    if (Tiempo->TiempoConsumido + TiempoEntreDepth > Tiempo->TiempoMed)
        return true;

    return Tiempo->TiempoConsumido > MIN(FactorPv, Tiempo->TiempoMed);
}
void TerminarTiempoLimite(_ST_TipoJuego *Tiempo)
{
    if (Tiempo->Activo != 0 && TiempoTranscurrido(Tiempo) >= Tiempo->TiempoMax) Tiempo->Interrumpir = true;
}
