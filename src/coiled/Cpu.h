#ifndef CPU_H
#define CPU_H

#include "Definiciones.h"

#ifdef USAR_NNUE

#ifdef _MSC_VER
    #include <intrin.h>
#endif

typedef struct tag_CPU
{
	int SSE2;
	int SSE3;
	int SSE41;
	int AVX2;
} _ST_Cpu;

void ObtenerCpu(_ST_Cpu *Cpu);

#endif
#endif
