#include "Cpu.h"

#ifdef USAR_NNUE

#ifdef _WIN32
void ObtenerCpu(_ST_Cpu *Cpu)
{
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
}

#else
void ObtenerCpu(_ST_Cpu *Cpu)
{
    int regs[4] = { 0, 0, 0, 0 };
    unsigned int ids = 0;
    __cpuid(0, regs[0], regs[1], regs[2], regs[3]);

    ids = regs[0];
    __cpuid(1, regs[0], regs[1], regs[2], regs[3]);

    Cpu->SSE2 = regs[3] >> 26 & 1;
    Cpu->SSE3 = regs[2] & 1;
    Cpu->SSE41 = regs[2] >> 19 & 1;

    if (ids >= 7)
    {
        __cpuid(7, regs[0], regs[1], regs[2], regs[3]);
        Cpu->AVX2 = regs[1] >> 5 & 1;
    }

}
#endif
#endif
