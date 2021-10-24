#################################COMPILADOR#############################################
CC = gcc
CXX = g++
BIT = 

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
SRC = coiled/*.c

LIBS = -lm -ldl

ifeq ($(BIT),64)
DEFINES += -DARC_64BIT
endif

WFLAGS = -g -Wall -Wextra -Wshadow
CFLAGS = -O2 -std=c11 $(X86_64)

#################################EGBBDLL###############################################
EGBB = libegbb

EGBBLIBS = -lpthread -lm -shared

EGBBDEFINES = -DPARALLEL
ifeq ($(BIT),64)
EGBBDEFINES += -DARC_64BIT
endif
GBBDEFINES += -DHAS_POPCNT
#GBBDEFINES += -DHAS_PREFETCH -DUSE_SPINLOCK

EGBBUNUSED = -Wno-unused-variable -Wno-unused-result 
EGBBUNUSED += -Wno-unused-but-set-variable -Wno-maybe-uninitialized -Wno-psabi

WEGBBFLAGS = -g -Wall -Wextra -Wshadow -fstrict-aliasing -fno-exceptions -fno-rtti $(EGBBUNUSED)
CEGBBFLAGS += -O2 -std=c++11 $(X86_64) -fomit-frame-pointer -flto

##############################FATHOM#####################################################
FATHOM = libsyzygy

FATHOMLIBS = -lpthread -lm -shared

FATHOMDEFINES = -DTB_NO_THREADS

WFATHOMFLAGS = -g -Wall -Wextra -Wshadow -I..
CFATHOMFLAGS = -O2 -std=c99 $(X86_64)

##############################GAVIOTA####################################################
GTB = libgtb

GTBLIBS = -shared

GTBDEFINES = -DGCCLINUX -DNDEBUG -DZ_PREFIX

WGTBFLAGS = -g -Wall -Wextra
CGTBFLAGS = -std=c99 -O2 $(X86_64)

#############################NNUE########################################################
NNUE = libnnue

NNUELIBS = -lm -shared 

NNUEDEFINES = -DIS_64BIT

TECNOLOGY1 = avx2
TECNOLOGY2 = sse4.1
TECNOLOGY3 = sse3
TECNOLOGY4 = sse2

NNUEDEFINES1 = -mavx2 -msse4.1 -msse3 -msse2 -msse -DUSE_AVX2
NNUEDEFINES2 = -msse4.1 -msse3 -msse2 -msse -DUSE_SSE41
NNUEDEFINES3 = -msse3 -msse2 -msse -DUSE_SSE3
NNUEDEFINES4 = -msse2 -msse -DUSE_SSE2

WNNUEFLAGS = -g -Wall -Wno-unused-variable -Wno-unused-result -Wno-unused-but-set-variable -Wno-maybe-uninitialized
CNNUEFLAGS = -std=c++11 -O2 -fomit-frame-pointer -fstrict-aliasing -fno-exceptions -fno-rtti

#########################################################################################
.PHONY: all
all: Coiled egbb syzygy gtb nn
    
Coiled:
	$(CC) $(CFLAGS) $(SRC) $(DEFINES) -o $(EXE)_$(VERSION)_$(ARCHITECTURE) $(LIBS)
	
egbb:
	$(CXX) $(CEGBBFLAGS) $(EGBBDEFINES) -c -fPIC -o egbbdll/egbbdll.o egbbdll/egbbdll.cpp
	$(CXX) $(CEGBBFLAGS) $(EGBBDEFINES) -c -fPIC -o egbbdll/moves.o egbbdll/moves.cpp
	$(CXX) $(CEGBBFLAGS) $(EGBBDEFINES) -c -fPIC -o egbbdll/index.o egbbdll/index.cpp
	$(CXX) $(CEGBBFLAGS) $(EGBBDEFINES) -c -fPIC -o egbbdll/decompress.o egbbdll/decompress.cpp
	$(CXX) $(CEGBBFLAGS) $(EGBBDEFINES) -c -fPIC -o egbbdll/codec.o egbbdll/codec.cpp
	$(CXX) $(CEGBBFLAGS) $(EGBBDEFINES) -c -fPIC -o egbbdll/cache.o egbbdll/cache.cpp
	$(CXX) -o $(EGBB)_$(ARCHITECTURE).so egbbdll/egbbdll.o egbbdll/moves.o egbbdll/index.o egbbdll/decompress.o egbbdll/codec.o egbbdll/cache.o $(EGBBLIBS)
	
syzygy:
	$(CC) $(CFATHOMFLAGS) $(FATHOMDEFINES) -c -fPIC -o fathom/tbprobe.o fathom/tbprobe.c
	$(CC) -o $(FATHOM)_$(ARCHITECTURE).so fathom/tbprobe.o $(FATHOMLIBS)

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
	$(CC) -o $(GTB)_$(ARCHITECTURE).so gtbprobe/gtb-probe.o gtbprobe/gtb-dec.o gtbprobe/gtb-att.o gtbprobe/sysport/sysport.o gtbprobe/compression/wrap.o gtbprobe/compression/lzma/LzmaDec.o gtbprobe/compression/lzma/Alloc.o gtbprobe/compression/lzma/LzFind.o gtbprobe/compression/lzma/Lzma86Dec.o gtbprobe/compression/lzma/Bra86.o -Wl,-soname=libgtb.so.1 $(GTBLIBS)
	
nn:
	$(CXX) $(CNNUEFLAGS) $(NNUEDEFINES1) -c -fPIC -o nnue/misc.o nnue/misc.cpp
	$(CXX) $(CNNUEFLAGS) $(NNUEDEFINES1) -c -fPIC -o nnue/nnue.o nnue/nnue.cpp
	$(CXX) -o $(NNUE)_$(TECNOLOGY1).so nnue/misc.o nnue/nnue.o $(NNUELIBS)
	rm -rf nnue/*.o
		
	$(CXX) $(CNNUEFLAGS) $(NNUEDEFINES2) -c -fPIC -o nnue/misc.o nnue/misc.cpp
	$(CXX) $(CNNUEFLAGS) $(NNUEDEFINES2) -c -fPIC -o nnue/nnue.o nnue/nnue.cpp
	$(CXX) -o $(NNUE)_$(TECNOLOGY2).so nnue/misc.o nnue/nnue.o $(NNUELIBS)
	rm -rf nnue/*.o
		
	$(CXX) $(CNNUEFLAGS) $(NNUEDEFINES3) -c -fPIC -o nnue/misc.o nnue/misc.cpp
	$(CXX) $(CNNUEFLAGS) $(NNUEDEFINES3) -c -fPIC -o nnue/nnue.o nnue/nnue.cpp
	$(CXX) -o $(NNUE)_$(TECNOLOGY3).so nnue/misc.o nnue/nnue.o $(NNUELIBS)
	rm -rf nnue/*.o
		
	$(CXX) $(CNNUEFLAGS) $(NNUEDEFINES4) -c -fPIC -o nnue/misc.o nnue/misc.cpp
	$(CXX) $(CNNUEFLAGS) $(NNUEDEFINES4) -c -fPIC -o nnue/nnue.o nnue/nnue.cpp
	$(CXX) -o $(NNUE)_$(TECNOLOGY4).so nnue/misc.o nnue/nnue.o $(NNUELIBS)
    
clean:
	rm -rf egbbdll/*.o
	rm -rf fathom/*.o
	rm -rf gtbprobe/*.o
	rm -rf gtbprobe/sysport/*.o
	rm -rf gtbprobe/compression/*.o
	rm -rf gtbprobe/compression/lzma/*.o
	rm -rf nnue/*.o

help:
	@echo ""
	@echo ""
	@echo "1. make - Compile all x86"
	@echo "2. make BIT=64 - Compile all x64"
	@echo "3. make clean - remove all *.o"
	@echo "4. make Coiled - Compile coiled only"
	@echo "5. make egbb - Compile egbb only"
	@echo "6. make syzygy - Compile syzygy only"
	@echo "7. make gtb - Compile gtb only"
	@echo "8. make nn - Compile nnue only"
	@echo ""
	
