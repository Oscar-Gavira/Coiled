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

#include "Tiempo.h"

/* Obtenemos el tiempo */
U64 ObtenerTiempo()
{
#if defined(_WIN32)
    return GetTickCount();
#else
    struct timeval tv;

    gettimeofday(&tv, NULL);
    return (U64)(tv.tv_sec * 1000 + tv.tv_usec / 1000);
#endif
}
/* Tiempo trascurrido */
U64 TiempoTrascurrido(_ST_TipoJuego *Tiempo)
{
    return ObtenerTiempo() - Tiempo->Inicio;
}
/* Configuracion del tiempo */
void TiempoInicio(_ST_TipoJuego *Tiempo, int Movestogo, int TiempoTotal, int inc)
{
    Tiempo->TiempoFactor = 0;
    TiempoTotal = MAX(TiempoTotal, Tiempo->PrevenirTiempoExcedido);

    if (Movestogo != 0)
    {
        Tiempo->Tiempo = 0.70f * ((TiempoTotal - Tiempo->PrevenirTiempoExcedido) / Movestogo) + inc;
        Tiempo->TiempoMax1 = 0.90f * ((TiempoTotal - Tiempo->PrevenirTiempoExcedido) / Movestogo) + inc;
        Tiempo->TiempoMax2 = 1.10f * ((TiempoTotal - Tiempo->PrevenirTiempoExcedido) / Movestogo) + inc;
    }
    else {
        Tiempo->Tiempo = 0.90f * ((TiempoTotal - Tiempo->PrevenirTiempoExcedido) + 25 * inc) / 50;
        Tiempo->TiempoMax1 = 1.95f * ((TiempoTotal - Tiempo->PrevenirTiempoExcedido) + 25 * inc) / 50;
        Tiempo->TiempoMax2 = 2.90f * ((TiempoTotal - Tiempo->PrevenirTiempoExcedido) + 25 * inc) / 50;
    }

    Tiempo->Tiempo = MIN(Tiempo->Tiempo, TiempoTotal - Tiempo->PrevenirTiempoExcedido);
    Tiempo->TiempoMax1 = MIN(Tiempo->TiempoMax1, TiempoTotal - Tiempo->PrevenirTiempoExcedido);
    Tiempo->TiempoMax2= MIN(Tiempo->TiempoMax2, TiempoTotal - Tiempo->PrevenirTiempoExcedido);
}

/* Ajuste del tiempo por perdida o aumento de puntos. */
int TiempoActualizar(_ST_TipoJuego *Tiempo, int Valor, int ValorAnterior, int MejorJugada, int MejorJugadaAnterior, int depth)
{
    float FactorPv = 0.0f;

    /* Si es de tipo movetime 2000, mate X, infinito, depth... no actualizamos. */
    if (Tiempo->Activo != true || depth < 4) return false;

    // Aumentamos tiempo si la puntuacion cae 
    if (ValorAnterior > Valor + 10) Tiempo->Tiempo *= 1.050f;
    if (ValorAnterior > Valor + 20) Tiempo->Tiempo *= 1.050f;
    if (ValorAnterior > Valor + 40) Tiempo->Tiempo *= 1.050f;

    // Aumentar tiempo si la puntuacion se eleva
    if (ValorAnterior + 15 < Valor) Tiempo->Tiempo *= 1.050f;
    if (ValorAnterior + 30 < Valor) Tiempo->Tiempo *= 1.050f;

    /* Estabilidad de la variante principal. */
    Tiempo->TiempoFactor = MAX(0, Tiempo->TiempoFactor - 1);
    if (MejorJugada != MejorJugadaAnterior)
        Tiempo->TiempoFactor = 5;

    /* Si la variante principal es estable, cortamos. */
    FactorPv = Tiempo->Tiempo;
    FactorPv *= 1.0f + Tiempo->TiempoFactor * 0.185f;
    return TiempoTrascurrido(Tiempo) > MIN(FactorPv, Tiempo->TiempoMax1);
}

/* Si excedemos el tiempo maximo, cortamos. */
int TerminarTiempoLimite(_ST_TipoJuego *Tiempo)
{
    if (Tiempo->Activo >= true
        && Tiempo->DepthAct > 1
        && TiempoTrascurrido(Tiempo) >= Tiempo->TiempoMax2)
    {
        Tiempo->Interrumpir = true;
        return true;
    }
    return false;
}

