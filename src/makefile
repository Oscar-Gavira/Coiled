############################################################################################
CC = gcc
#[native/generic]. Genera una compilacion en modo cpu generica o cpu local donde se compila.
CPU = native
#[true/false]. Para compilar con opciones de depuracion.
DEBUG = false
#Genera en modo forzado 32 o 64. Valor [true/false/auto].
X32 = auto
#Si genera error al limpiar, des-comentar esta linea
#RM = del

############################################################################################
#Nombre 
EXE = Coiled
#Versión
VERSION = 1.2
#Tecnología AVX2 o SSE2 para NNUE
TECNOLOGY1 = avx2
TECNOLOGY2 = sse2

############################################################################################
#En caso de compilar en Linux OS obtiene el valor de 'Linux', mediante $(shell uname -s)
#En caso de compilar en windows OS tiene el valor predeterminado de Windows_NT. Y $(shell uname -s) no funciona.
KERNEL = $(shell uname -s)
ifeq ($(KERNEL),Linux)
    OS = Linux
endif

ifeq ($(OS),Windows_NT)
    ifeq ($(PROCESSOR_ARCHITECTURE),AMD64)
        ARCHITECTURE = x64
        BIT = 64
    endif
    ifeq ($(PROCESSOR_ARCHITECTURE),x86)
        ARCHITECTURE = x86
        BIT = 32
    endif

    ifeq ($(CC),clang)
        LIBS = -lpthread -static
    endif
    ifeq ($(CC),gcc)
        LIBS = -lm -lpthread -static
    endif

    DEFINES = 

    EXT = .exe
    DEL1 = nnue/*.o
    DEL2 = coiled/*.o
    DEL3 = coiled/resource/*.res
    ICON = windres.exe -J rc -O coff -i 
    ICONFILECOMP = coiled/resource/ver.rc -o coiled/resource/ver.res
    ICONFILE = coiled/resource/ver.res
endif
ifeq ($(OS),Linux)
    UNAME_P := $(shell uname -m)
    ifeq ($(UNAME_P),x86_64)
        ARCHITECTURE = x64
        BIT = 64
    else
        ARCHITECTURE = x86
        BIT = 32
    endif

    LIBS = -lm -lpthread

    DEFINES = 

    EXT = 
    DEL1 = nnue/*.o
    DEL2 = coiled/*.o
    DEL3 = 
endif

############################################################################################
ifeq ($(X32),false)
    ARCHITECTURE = x64
    BIT = 64
endif
ifeq ($(X32),true)
    ARCHITECTURE = x86
    BIT = 32
endif

############################################################################################
ifeq ($(BIT),64)
    ifeq ($(CPU),native)
        X86_64 = -m64 -march=native -mtune=native
        NNUEDEFINES = -DUSE_AVX2
        DEFINES += -DARC_64BIT
    endif
    ifeq ($(CPU),generic)
        X86_64 = -m64 -march=x86-64 -mtune=generic -msse -msse2
        NNUEDEFINES = -mavx2 -mavx -msse4.1 -msse3 -DUSE_AVX2
        DEFINES += -DARC_64BIT
    endif
endif
ifeq ($(BIT),32)
    ifeq ($(CPU),native)
        X86_64 = -m32 -march=native -mtune=native
    endif
    ifeq ($(CPU),generic)
        X86_64 = -m32 -march=pentium4 -mtune=generic
    endif
endif

############################################################################################
WFLAGS = -g -Wall -Wextra -Wshadow -pedantic -Wpedantic -pedantic-errors
CFLAGS = -O3 -std=gnu11

ifeq ($(DEBUG),true)
CFLAGS += $(WFLAGS)
DEFINES += -DDEBUG
endif

############################################################################################
.PHONY: all coiled64 coiled32 clean help
	
ifeq ($(BIT),32)
all: coiled32 clean
endif
ifeq ($(BIT),64)
all: coiled64 clean
endif
	
coiled64:
	$(CC) $(CFLAGS) $(X86_64) $(DEFINES) -c coiled/AlphaBeta.c -o coiled/AlphaBeta.o
	$(CC) $(CFLAGS) $(X86_64) $(DEFINES) -c coiled/Evaluacion.c -o coiled/Evaluacion.o
	$(CC) $(CFLAGS) $(X86_64) $(DEFINES) -c coiled/GeneradorDeMovimientos.c -o coiled/GeneradorDeMovimientos.o
	$(CC) $(CFLAGS) $(X86_64) $(DEFINES) -c coiled/Hash.c -o coiled/Hash.o
	$(CC) $(CFLAGS) $(X86_64) $(DEFINES) -c coiled/Historico.c -o coiled/Historico.o
	$(CC) $(CFLAGS) $(X86_64) $(DEFINES) -c coiled/Inicio.c -o coiled/Inicio.o
	$(CC) $(CFLAGS) $(X86_64) $(DEFINES) -c coiled/nn.c -o coiled/nn.o
	$(CC) $(CFLAGS) $(X86_64) $(DEFINES) -c coiled/OrdenarMovimientos.c -o coiled/OrdenarMovimientos.o
	$(CC) $(CFLAGS) $(X86_64) $(DEFINES) -c coiled/See.c -o coiled/See.o
	$(CC) $(CFLAGS) $(X86_64) $(DEFINES) -c coiled/Utilidades.c -o coiled/Utilidades.o
	$(CC) $(CFLAGS) $(X86_64) $(DEFINES) -c coiled/Tiempo.c -o coiled/Tiempo.o
	$(CC) $(CFLAGS) $(X86_64) $(DEFINES) -c nnue/misc.c -o nnue/misc.o
	$(CC) $(CFLAGS) $(X86_64) $(DEFINES) -c nnue/nnue.c -o nnue/nnue.o
	$(ICON) $(ICONFILECOMP)
	$(CC) -o $(EXE)_$(VERSION)_$(TECNOLOGY2)_$(ARCHITECTURE)$(EXT) coiled/nn.o nnue/misc.o nnue/nnue.o coiled/AlphaBeta.o coiled/Evaluacion.o coiled/GeneradorDeMovimientos.o coiled/Hash.o coiled/Historico.o coiled/Inicio.o coiled/OrdenarMovimientos.o coiled/See.o coiled/Utilidades.o coiled/Tiempo.o $(ICONFILE) $(LIBS)
	$(RM) $(DEL1)
	
	$(CC) $(CFLAGS) $(X86_64) $(DEFINES) $(NNUEDEFINES) -c nnue/misc.c -o nnue/misc.o
	$(CC) $(CFLAGS) $(X86_64) $(DEFINES) $(NNUEDEFINES) -c nnue/nnue.c -o nnue/nnue.o
	$(ICON) $(ICONFILECOMP)
	$(CC) -o $(EXE)_$(VERSION)_$(TECNOLOGY1)_$(ARCHITECTURE)$(EXT) coiled/nn.o nnue/misc.o nnue/nnue.o coiled/AlphaBeta.o coiled/Evaluacion.o coiled/GeneradorDeMovimientos.o coiled/Hash.o coiled/Historico.o coiled/Inicio.o coiled/OrdenarMovimientos.o coiled/See.o coiled/Utilidades.o coiled/Tiempo.o $(ICONFILE) $(LIBS)

	
coiled32:
	$(CC) $(CFLAGS) $(X86_64) $(DEFINES) -c coiled/AlphaBeta.c -o coiled/AlphaBeta.o
	$(CC) $(CFLAGS) $(X86_64) $(DEFINES) -c coiled/Evaluacion.c -o coiled/Evaluacion.o
	$(CC) $(CFLAGS) $(X86_64) $(DEFINES) -c coiled/GeneradorDeMovimientos.c -o coiled/GeneradorDeMovimientos.o
	$(CC) $(CFLAGS) $(X86_64) $(DEFINES) -c coiled/Hash.c -o coiled/Hash.o
	$(CC) $(CFLAGS) $(X86_64) $(DEFINES) -c coiled/Historico.c -o coiled/Historico.o
	$(CC) $(CFLAGS) $(X86_64) $(DEFINES) -c coiled/Inicio.c -o coiled/Inicio.o
	$(CC) $(CFLAGS) $(X86_64) $(DEFINES) -c coiled/nn.c -o coiled/nn.o
	$(CC) $(CFLAGS) $(X86_64) $(DEFINES) -c coiled/OrdenarMovimientos.c -o coiled/OrdenarMovimientos.o
	$(CC) $(CFLAGS) $(X86_64) $(DEFINES) -c coiled/See.c -o coiled/See.o
	$(CC) $(CFLAGS) $(X86_64) $(DEFINES) -c coiled/Utilidades.c -o coiled/Utilidades.o
	$(CC) $(CFLAGS) $(X86_64) $(DEFINES) -c coiled/Tiempo.c -o coiled/Tiempo.o
	$(ICON) $(ICONFILECOMP)
	$(CC) -o $(EXE)_$(VERSION)_$(ARCHITECTURE)$(EXT) coiled/AlphaBeta.o coiled/Evaluacion.o coiled/GeneradorDeMovimientos.o coiled/Hash.o coiled/Historico.o coiled/Inicio.o coiled/nn.o coiled/OrdenarMovimientos.o coiled/See.o coiled/Utilidades.o coiled/Tiempo.o $(ICONFILE) $(LIBS)
	
clean:
	$(RM) $(DEL1)
	$(RM) $(DEL2)
	$(RM) $(DEL3)
	
version:
	$(CC) -v
	
help:
	@echo ""
	@echo "1. make - Compile all for 32 or 64 bit, depends on your operating system"
	@echo "2. make clean - remove all *.o"
	@echo ""
	@echo "Additional parameters:"
	@echo "Add DEBUG=true - Compile in debug mode."
	@echo "Add X32=true - Force compile to 32bit."
	@echo "Add CPU=native - Compile for your CPU."
	@echo "Add CC=clang - Compile with Clang."
	@echo ""
	@echo "Examples:"
	@echo "make CC=clang CPU=native"
	@echo "make CC=clang X32=true"
	@echo "make CPU=native"
	@echo "make X32=true DEBUG=true"
	@echo "make DEBUG=true"
	@echo "make ..."
