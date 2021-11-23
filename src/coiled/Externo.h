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

#ifndef EXTERNO_H
#define EXTERNO_H

#include "Estructuras.h"

/*******************************************************************************
                                Variables globales
*******************************************************************************/
extern _ST_TableroX64 TableroGlobal;						/* Tablero de la posicion actual. */
extern _ST_TipoJuego TipoJuego;							    /* Informacion de la partida por tiempo o por profundidad fija */
extern int Salir;										    /* Para salir de la aplicacion */
extern _ST_Puntos Blancas;									/* Almacena la puntuacion de las blancas */
extern _ST_Puntos Negras;									/* Almacena la puntuacion de las negras */
extern U64 zobrist_Tablero[14][64];                         /* Almacena las llaves zobrist */
extern _ST_TT_Opciones TT_Opciones;

/************************************************************************************
                    Metodos externos See.c
************************************************************************************/
extern void Inicializar_See();							    /* Inicializa valores MVV/LVA. */

/************************************************************************************
                    Metodos externos Inicio.c
************************************************************************************/
extern void Movimiento(char* ptr, int* Ok, int Ultimo);	    /* Realiza un movimiento y comprobamos si es legal. */
extern void Position_Fen_Startpos(char* ptr);			    /* Carga una posicion FEN */

/************************************************************************************
                    Metodos externos AlphaBeta.c
************************************************************************************/
extern void A_Inicio(int Reiniciar);                        /* Reinicia variables utilizados en el arbol AlphaBeta */
extern void Buscar();									    /* Inicializa la busqueda AlphaBeta */
extern void Iniciar_AlphaBeta();                            /* Inicia un vector para las reducciones en la busqueda */

/************************************************************************************
                    Metodos externos Evaluacion.c
************************************************************************************/
extern int Evaluar();						                /* Evaluacion del tablero */
extern int EvaluarTablas();                                 /* Comprueba si la posicion actual es tablas por falta de material. */
extern void LimpiarPuntuacion(int todo);                    /* Limpia los puntos */
extern void CargarEvaluacion();

/************************************************************************************
                    Metodos externos Utilidades.c
************************************************************************************/
extern void MovimientoCoordenadas(int Inicio, int Fin, int PiezaCoronacion, char* mov);	/* Obtenemos el movimiento en coordenadas E2E4 */
extern void SplitString(char* string, char* contenedor, int longitud);					/* Limpiamos cadenas con espacios */

/************************************************************************************
                    Metodos externos LibroAperturas.c
************************************************************************************/
#ifdef USAR_SQLITE
extern _ST_EstructuraBd LibroSql;
extern SQLITE3_OPEN_V2 sqlite3_open_v2;
extern SQLITE_PREPARE_V2 sqlite3_prepare_v2;
extern SQLITE_STEP sqlite3_step;
extern SQLITE_COLUMN_TEXT sqlite3_column_text;
extern SQLITE_RESET sqlite3_reset;
extern SQLITE_CLOSE_V2 sqlite3_close_v2;
#endif

/************************************************************************************
                    Metodos externos nnue.c
************************************************************************************/
#ifdef USAR_NNUE
extern _ST_Nnue Nnue;
extern int ProbarNNUE();
extern NNUE_INIT NNUE_init;
extern NNUE_EVALUATE NNUE_evaluate;
extern NNUE_TECHNOLOGY NNUE_technology;
#endif

/************************************************************************************
                    Tablas de finales tbprobe, syzygy, egbb
************************************************************************************/
#ifdef USAR_TABLAS_DE_FINALES
extern _ST_TablaDeFinales TablaDeFinales;

/* egbb */
extern EGBB_LOAD_EGBB EGBB_load_egbb;				/* Cargamos la tablas de finales. */
extern EGBB_PROBE_EGBB EGBB_probe_egbb;				/* Acceso a las tablas de finales. */

#ifdef ARC_64BIT
/* syzygy */
extern SG_INITS SG_inits;							/* Inicializa la tabla de finales */
extern SG_FREE SG_free;						    	/* Libera la tabla de finales */
extern SG_PROBE_WDL SG_probe_wdl;					/* Acceso a las tablas de finales en la busqueda. */
extern SG_LARGEST SG_man;							/* Indica la tablas disponibles. 3, 4, 5, 6 o 7 piezas. */
#endif

/* tbprobe */
extern TBINIT TBinit;								/* Inicializa la tabla de finales */
extern TBIS_INITIALIZED TBis_initialized;			/* Indica si la tabla esta inicializada */
extern TBRESTART TBrestart;							/* Reinicia la tabla de finales */
extern TBDONE TBdone;								/* Libera la tabla de finales */
extern TBPROBE_HARD TBprobe_hard;					/* Acceso a las tablas de finales en root. En HDD. (DTM) */
extern TBPROBE_SOFT TBprobe_soft;					/* Acceso a las tablas de finales en root. En Cache. (DTM) */
extern TBAVAILABILITY TBavailability;				/* Indica la tablas disponibles */
extern TBCACHE_INIT TBcache_init;					/* Inicia la cache y fraccion */
extern TBCACHE_DONE TBcache_done;					/* Borra la cache */
extern TBCACHE_IS_ON TBcache_is_on;
extern TBPATHS_INIT TBpaths_init;					/* Inicializa las rutas */
extern TBPATHS_ADD TBpaths_add;						/* Anade rutas */
extern TBPATHS_DONE TBpaths_done;					/* Libera ruta */

#endif

#endif
