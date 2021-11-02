#################################COMPILADOR#############################################
CC = gcc
CXX = g++

#################################COMPILAR EN WINDOWS CON MAKEFILE Y CODEBLOCK###########
ifeq ($(WindowsX86),true)
BIT = 32
WINDOWS = true
ICON = windres.exe -J rc -O coff -i 
ICONFILECOMP = coiled/ver.rc -o coiled/ver.res
ICONFILE = coiled/ver.res
else ifeq ($(WindowsX64),true)
BIT = 64
WINDOWS = true
ICON = windres.exe -J rc -O coff -i 
ICONFILECOMP = coiled/ver.rc -o coiled/ver.res
ICONFILE = coiled/ver.res
else ifeq ($(LinuxX86),true)
BIT = 32
WINDOWS = false
ICON =
ICONFILE =
else ifeq ($(LinuxX64),true)
BIT = 64
WINDOWS = false
ICON =
ICONFILE =
endif

##################################WINDOWS O LINUX#########################################
ifeq ($(WINDOWS),false)
EXT = .so
DEL1 = rm -rf egbbdll/*.o
DEL2 = rm -rf fathom/*.o
DEL3 = rm -rf gtbprobe/*.o
DEL4 = rm -rf gtbprobe/sysport/*.o
DEL5 = rm -rf gtbprobe/compression/*.o
DEL6 = rm -rf gtbprobe/compression/lzma/*.o
DEL7 = rm -rf nnue/*.o
DEL8 = rm -rf coiled/*.o
DEL9 = 
else
EXT = .dll
EXT2 = .exe
DEL1 = del egbbdll\*.o
DEL2 = del fathom\*.o
DEL3 = del gtbprobe\*.o
DEL4 = del gtbprobe\sysport\*.o
DEL5 = del gtbprobe\compression\*.o
DEL6 = del gtbprobe\compression\lzma\*.o
DEL7 = del nnue\*.o
DEL8 = del coiled\*.o
DEL9 = del coiled\*.res
endif

##################################32 O 64 BIT############################################
ifeq ($(BIT),64)
ARCHITECTURE = x64
X86_64 = -march=x86-64 -mtune=generic
else
ARCHITECTURE = x86
X86_64 = -march=i686 -mtune=generic
endif

################################COILED##################################################
EXE = Coiled
VERSION = 1.1

ifeq ($(WINDOWS),false)
LIBS = -lm -ldl
else
LIBS = -lm
endif

ifeq ($(BIT),64)
DEFINES += -DARC_64BIT
endif

WFLAGS = -g -Wall -Wextra -Wshadow
CFLAGS = -O2 -std=c11 $(X86_64)

#################################EGBBDLL###############################################
ifeq ($(WINDOWS),false)
EGBB = libegbb
EGBBLIBS = -lm -shared
else
EGBB = egbb
EGBBLIBS = -lm -shared -static
endif

#EGBBDEFINES = -DPARALLEL
ifeq ($(BIT),64)
EGBBDEFINES += -DARC_64BIT
endif
#GBBDEFINES += -DHAS_POPCNT
#GBBDEFINES += -DHAS_PREFETCH -DUSE_SPINLOCK

EGBBUNUSED = -Wno-unused-variable -Wno-unused-result 
EGBBUNUSED += -Wno-unused-but-set-variable -Wno-maybe-uninitialized -Wno-psabi

WEGBBFLAGS = -g -Wall -Wextra -Wshadow -fstrict-aliasing -fno-exceptions -fno-rtti $(EGBBUNUSED)
CEGBBFLAGS += -O2 -std=c++11 $(X86_64) -fomit-frame-pointer

##############################FATHOM#####################################################
ifeq ($(WINDOWS),false)
FATHOM = libsyzygy
FATHOMLIBS = -lm -shared
else
FATHOM = syzygy
FATHOMLIBS = -lm -shared -static
endif

FATHOMDEFINES = -DTB_NO_THREADS

WFATHOMFLAGS = -g -Wall -Wextra -Wshadow -I..
CFATHOMFLAGS = -O2 -std=c99 $(X86_64)

##############################GAVIOTA####################################################
ifeq ($(WINDOWS),false)
GTB = libgtb
GTBDEFINES = -DGCCLINUX -DNDEBUG -DZ_PREFIX
else
GTB = gtb
GTBDEFINES = -DMINGW -DNDEBUG -DZ_PREFIX
endif

GTBLIBS = -shared

WGTBFLAGS = -g -Wall -Wextra
CGTBFLAGS = -std=c99 -O2 $(X86_64)

#############################NNUE########################################################
ifeq ($(WINDOWS),false)
NNUE = libnnue
NNUELIBS = -lm -shared
else
NNUE = nnue
NNUELIBS = -lm -shared -static
endif

ifeq ($(BIT),64)
NNUEDEFINES = -DIS_64BIT
endif

TECNOLOGY1 = avx2
TECNOLOGY2 = sse4.1
TECNOLOGY3 = sse3
TECNOLOGY4 = sse2

NNUEDEFINES1 = $(X86_64) -mavx2 -msse4.1 -msse3 -msse2 -msse -DUSE_AVX2
NNUEDEFINES2 = $(X86_64) -msse4.1 -msse3 -msse2 -msse -DUSE_SSE41
NNUEDEFINES3 = $(X86_64) -msse3 -msse2 -msse -DUSE_SSE3
NNUEDEFINES4 = $(X86_64) -msse2 -msse -DUSE_SSE2

WNNUEFLAGS = -g -Wall -Wno-unused-variable -Wno-unused-result -Wno-unused-but-set-variable -Wno-maybe-uninitialized
CNNUEFLAGS = -std=c++11 -O2 -fomit-frame-pointer -fstrict-aliasing -fno-exceptions -fno-rtti

#########################################################################################
	
.PHONY: all
all: help
	
.PHONY: WindowsX86 LinuxX86
X86 LinuxX86: egbb gtb engine 
	
.PHONY: WindowsX64 LinuxX64
X64 LinuxX64: egbb syzygy gtb nn engine 
	
engine:
	$(CC) $(CFLAGS) $(DEFINES) -c coiled/AlphaBeta.c -o coiled/AlphaBeta.o
	$(CC) $(CFLAGS) $(DEFINES) -c coiled/Cpu.c -o coiled/Cpu.o
	$(CC) $(CFLAGS) $(DEFINES) -c coiled/egbb.c -o coiled/egbb.o
	$(CC) $(CFLAGS) $(DEFINES) -c coiled/Evaluacion.c -o coiled/Evaluacion.o
	$(CC) $(CFLAGS) $(DEFINES) -c coiled/Fen.c -o coiled/Fen.o
	$(CC) $(CFLAGS) $(DEFINES) -c coiled/GeneradorDeMovimientos.c -o coiled/GeneradorDeMovimientos.o
	$(CC) $(CFLAGS) $(DEFINES) -c coiled/Hash.c -o coiled/Hash.o
	$(CC) $(CFLAGS) $(DEFINES) -c coiled/Historico.c -o coiled/Historico.o
	$(CC) $(CFLAGS) $(DEFINES) -c coiled/Inicio.c -o coiled/Inicio.o
	$(CC) $(CFLAGS) $(DEFINES) -c coiled/LibroAperturas.c -o coiled/LibroAperturas.o
	$(CC) $(CFLAGS) $(DEFINES) -c coiled/nnue.c -o coiled/nnue.o
	$(CC) $(CFLAGS) $(DEFINES) -c coiled/OrdenarMovimientos.c -o coiled/OrdenarMovimientos.o
	$(CC) $(CFLAGS) $(DEFINES) -c coiled/Perft.c -o coiled/Perft.o
	$(CC) $(CFLAGS) $(DEFINES) -c coiled/See.c -o coiled/See.o
	$(CC) $(CFLAGS) $(DEFINES) -c coiled/syzygy.c -o coiled/syzygy.o
	$(CC) $(CFLAGS) $(DEFINES) -c coiled/tbprobe.c -o coiled/tbprobe.o
	$(CC) $(CFLAGS) $(DEFINES) -c coiled/Utilidades.c -o coiled/Utilidades.o
	$(ICON) $(ICONFILECOMP)
	$(CC) -o $(EXE)_$(VERSION)_$(ARCHITECTURE)$(EXT2) coiled/AlphaBeta.o coiled/Cpu.o coiled/egbb.o coiled/Evaluacion.o coiled/Fen.o coiled/GeneradorDeMovimientos.o coiled/Hash.o coiled/Historico.o coiled/Inicio.o coiled/LibroAperturas.o coiled/nnue.o coiled/OrdenarMovimientos.o coiled/Perft.o coiled/See.o coiled/syzygy.o coiled/tbprobe.o coiled/Utilidades.o $(ICONFILE) $(LIBS)
	
egbb:
	$(CXX) $(CEGBBFLAGS) $(EGBBDEFINES) -c -fPIC -o egbbdll/egbbdll.o egbbdll/egbbdll.cpp
	$(CXX) $(CEGBBFLAGS) $(EGBBDEFINES) -c -fPIC -o egbbdll/moves.o egbbdll/moves.cpp
	$(CXX) $(CEGBBFLAGS) $(EGBBDEFINES) -c -fPIC -o egbbdll/index.o egbbdll/index.cpp
	$(CXX) $(CEGBBFLAGS) $(EGBBDEFINES) -c -fPIC -o egbbdll/decompress.o egbbdll/decompress.cpp
	$(CXX) $(CEGBBFLAGS) $(EGBBDEFINES) -c -fPIC -o egbbdll/codec.o egbbdll/codec.cpp
	$(CXX) $(CEGBBFLAGS) $(EGBBDEFINES) -c -fPIC -o egbbdll/cache.o egbbdll/cache.cpp
	$(CXX) -o $(EGBB)_$(ARCHITECTURE)$(EXT) egbbdll/egbbdll.o egbbdll/moves.o egbbdll/index.o egbbdll/decompress.o egbbdll/codec.o egbbdll/cache.o $(EGBBLIBS)
	
syzygy:
	$(CC) $(CFATHOMFLAGS) $(FATHOMDEFINES) -c -fPIC -o fathom/tbprobe.o fathom/tbprobe.c
	$(CC) -o $(FATHOM)_$(ARCHITECTURE)$(EXT) fathom/tbprobe.o $(FATHOMLIBS)
	
gtb:
	$(CC) $(CGTBFLAGS) $(GTBDEFINES) -c -fPIC -o gtbprobe/gtb-probe.o gtbprobe/gtb-probe.c
	$(CC) $(CGTBFLAGS) $(GTBDEFINES) -c -fPIC -o gtbprobe/gtb-dec.o gtbprobe/gtb-dec.c
	$(CC) $(CGTBFLAGS) $(GTBDEFINES) -c -fPIC -o gtbprobe/gtb-att.o gtbprobe/gtb-att.c
	$(CC) $(CGTBFLAGS) $(GTBDEFINES) -c -fPIC -o gtbprobe/sysport/sysport.o gtbprobe/sysport/sysport.c
	$(CC) $(CGTBFLAGS) $(GTBDEFINES) -c -fPIC -o gtbprobe/compression/wrap.o gtbprobe/compression/wrap.c
	$(CC) $(CGTBFLAGS) $(GTBDEFINES) -c -fPIC -o gtbprobe/compression/lzma/LzmaDec.o gtbprobe/compression/lzma/LzmaDec.c
	$(CC) $(CGTBFLAGS) $(GTBDEFINES) -c -fPIC -o gtbprobe/compression/lzma/Alloc.o gtbprobe/compression/lzma/Alloc.c
	$(CC) $(CGTBFLAGS) $(GTBDEFINES) -c -fPIC -o gtbprobe/compression/lzma/LzFind.o gtbprobe/compression/lzma/LzFind.c
	$(CC) $(CGTBFLAGS) $(GTBDEFINES) -c -fPIC -o gtbprobe/compression/lzma/Lzma86Dec.o gtbprobe/compression/lzma/Lzma86Dec.c
	$(CC) $(CGTBFLAGS) $(GTBDEFINES) -c -fPIC -o gtbprobe/compression/lzma/Bra86.o gtbprobe/compression/lzma/Bra86.c
	$(CC) -o $(GTB)_$(ARCHITECTURE)$(EXT) gtbprobe/gtb-probe.o gtbprobe/gtb-dec.o gtbprobe/gtb-att.o gtbprobe/sysport/sysport.o gtbprobe/compression/wrap.o gtbprobe/compression/lzma/LzmaDec.o gtbprobe/compression/lzma/Alloc.o gtbprobe/compression/lzma/LzFind.o gtbprobe/compression/lzma/Lzma86Dec.o gtbprobe/compression/lzma/Bra86.o -Wl,-soname=libgtb.so.1 $(GTBLIBS)
	
nn:
	$(CXX) $(CNNUEFLAGS) $(NNUEDEFINES1) -c -fPIC -o nnue/misc.o nnue/misc.cpp
	$(CXX) $(CNNUEFLAGS) $(NNUEDEFINES1) -c -fPIC -o nnue/nnue.o nnue/nnue.cpp
	$(CXX) -o $(NNUE)_$(TECNOLOGY1)$(EXT) nnue/misc.o nnue/nnue.o $(NNUELIBS)
	
	
	$(CXX) $(CNNUEFLAGS) $(NNUEDEFINES2) -c -fPIC -o nnue/misc.o nnue/misc.cpp
	$(CXX) $(CNNUEFLAGS) $(NNUEDEFINES2) -c -fPIC -o nnue/nnue.o nnue/nnue.cpp
	$(CXX) -o $(NNUE)_$(TECNOLOGY2)$(EXT) nnue/misc.o nnue/nnue.o $(NNUELIBS)
	
	
	$(CXX) $(CNNUEFLAGS) $(NNUEDEFINES3) -c -fPIC -o nnue/misc.o nnue/misc.cpp
	$(CXX) $(CNNUEFLAGS) $(NNUEDEFINES3) -c -fPIC -o nnue/nnue.o nnue/nnue.cpp
	$(CXX) -o $(NNUE)_$(TECNOLOGY3)$(EXT) nnue/misc.o nnue/nnue.o $(NNUELIBS)
	
	
	$(CXX) $(CNNUEFLAGS) $(NNUEDEFINES4) -c -fPIC -o nnue/misc.o nnue/misc.cpp
	$(CXX) $(CNNUEFLAGS) $(NNUEDEFINES4) -c -fPIC -o nnue/nnue.o nnue/nnue.cpp
	$(CXX) -o $(NNUE)_$(TECNOLOGY4)$(EXT) nnue/misc.o nnue/nnue.o $(NNUELIBS)
	
clean:
	$(DEL1)
	$(DEL2)
	$(DEL3)
	$(DEL4)
	$(DEL5)
	$(DEL6)
	$(DEL7)
	$(DEL8)
	$(DEL9)
	
help:
	@echo ""
	@echo "1. make LinuxX86=true X86 - Compile all for linux x86"
	@echo "2. make LinuxX64=true X64 - Compile all for linux x64"
	@echo "3. make WindowsX86=true X86 - Compile all for windows x86"
	@echo "4. make WindowsX64=true X64 - Compile all for windows x64"
	@echo "5. make clean - remove all *.o"
	@echo ""
	
