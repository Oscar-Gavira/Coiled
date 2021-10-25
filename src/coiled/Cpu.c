#include "Cpu.h"

#ifdef USAR_NNUE

void ObtenerCpu(_ST_Cpu *Cpu)
{
#ifndef _MSC_VER
    //https://gcc.gnu.org/onlinedocs/gcc-4.9.4/gcc/X86-Built-in-Functions.html
    //A partir de gcc 4.8.1 y mingw. Linux y windows.
    //__builtin_cpu_supports() es: cmov, mmx, popcnt, sse, sse2, sse3, ssse3, sse4.1, sse4.2, avx y avx2.
    Cpu->SSE2 = __builtin_cpu_supports("sse2");
    Cpu->SSE3 = __builtin_cpu_supports("sse3");
    Cpu->SSE41 = __builtin_cpu_supports("sse4.1");
    Cpu->AVX2 = __builtin_cpu_supports("avx2");
#else
    int cpuinfo[4];
    unsigned int ids = 0;

    __cpuid(cpuinfo, 0);
    ids = cpuinfo[0];

    __cpuid(cpuinfo, 1);
    Cpu->SSE2 = cpuinfo[3] >> 26 & 1;
    Cpu->SSE3 = cpuinfo[2] & 1;
    Cpu->SSE41 = cpuinfo[2] >> 19 & 1;

    if (ids >= 7)
    {
        __cpuid(cpuinfo, 7);
        Cpu->AVX2 = cpuinfo[1] >> 5 & 1;
    }
#endif
}

#endif
