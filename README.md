![Release](https://img.shields.io/github/v/release/Oscar-Gavira/Coiled)
[![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)](https://github.com/Oscar-Gavira/Coiled/blob/main/LICENSE)
![Language: C](https://img.shields.io/badge/Language-gnu11-blue)
[![c-Compiler](https://github.com/Oscar-Gavira/Coiled/actions/workflows/coiled.yml/badge.svg)](https://github.com/Oscar-Gavira/Coiled/actions/workflows/coiled.yml)


# ![imagen](https://user-images.githubusercontent.com/33672268/136560248-95e1ebf4-9405-414c-9827-6f1f3cf76b05.png) Coiled - Motor de ajedrez.

## :clipboard: Características

|                            |    CARACTERISTICAS                                                                                 |
| -------------------------- | -------------------------------------------------------------------------------------------------- |
| Sistema operativo          | Windows y Linux. 32&64 bit                                                                         |
| Protocolo de comunicación  | [UCI](https://github.com/Oscar-Gavira/Coiled/blob/main/UCI.md)                                     |
| Código abierto             | Si.                                                                                                |
| Tipo de licencia           | [GNU General Public License v3.0](https://github.com/Oscar-Gavira/Coiled/blob/master/LICENSE)      |
| Representación del tablero | Mailbox. 8x8 unidimensional.                                                                       |
| Forma de juego             | Ajedrez estándar o Chess960.                                                                       |
| Tipo de evaluación         | Interna o Nnue.                                                                                    |
| Multihilo                  | Si.                                                                                                |
| Tablas de finales          | No.                                                                                                |
| Modo de juego:             | Por tiempo: Convencional, Convencional e incremental, por movimiento. Profundidad. Mate. Infinito. |
| Otros                      | Reglas de la FIDE. Incluyendo triple repetición, 50 movimientos. Tablas por material insuficiente. |
| Lenguaje de programación   | C                                                                                                  |
| ELO estimado               | ± 2850 Interna. ± 3400 con NNUE. Incremento ± 75 con 4CPU                                          |


## :rocket: Introducción

Coiled es un motor de ajedrez de protocolo UCI escrito en C. Diseñado para ofrecer una experiencia de juego excepcional a través de distintas interfaces de usuario o GUI como: Cute Chess, Tarrasch, Banksia, Arena, Fritz...

Inicialmente el motor lo realicé en Visual basic .Net ya que la idea era unirlo a la parte gráfica (AjedreNet). Pero su rendimiento era bajo. Finalmente opté por reconstruirlo en C.

Reconozco que me cuesta manejar C, ya que no he programado nunca en este lenguaje de programación, sobre todo el trato con cadenas (lo que sería un string). Sin embargo, la sintaxis es parecida a PHP (sintaxis que sí conozco) y casi todo el programa se basa en poderosos cálculos matemáticos.

Lanzado por primera vez en agosto del 2013, Coiled ha evolucionado constantemente. Mediante actualizaciones periódicas, se han corregido errores, incrementado la fuerza de ELO y añadido nuevas funciones.

Hoy en día, Coiled es capaz de jugar al ajedrez estándar y a la variante Chess960, además de utilizar tablas de finales y soportar redes neuronales.

¿Deseas conocer más sobre las técnicas utilizadas en Coiled? Nuestra [Wiki](https://www.chessprogramming.org/Coiled) de programación de ajedrez ofrece una completa documentación en inglés.

Coiled, contribuye a la comunidad de investigación de ajedrez y programación. Por eso, este proyecto es de código abierto, lo que permite a jugadores y desarrolladores colaborar y mejorar la experiencia de juego para todos.

Si eres un apasionado del ajedrez, un desarrollador entusiasta o simplemente alguien interesado en la inteligencia artificial, te invitamos a probar Coiled y disfrutar de su poder y flexibilidad. ¡Te garantizamos una experiencia de ajedrez sin igual!


## :ballot_box_with_check: Requisitos mínimo

| SISTEMA OPERATIVO | Windows 32 bit                                            | Windows 64 bit.                                           |
| ----------------- | --------------------------------------------------------- | --------------------------------------------------------- |
| PROCESADOR        | Intel Pentium 4 o Amd K7. Con soporte MMX, SSE, SSE2      | Core 2 o Ahtlon 64. Con soporte MMX, SSE, SSE2 y SSE3     |
| RAM               | 2 MB.                                                     | 4 MB.                                                     |


## :nut_and_bolt: Redes neuronales (NNUE)

Coiled incluye la red neuronal del autor Fisherman. Pero no es necesario su utilización, ya que coiled dispone de su evaluación interna.
Puede cargar redes neuronales compatibles (nnue antiguas de 20MB) y realizar pruebas, comparaciones, torneos con las misma NN para los participantes...

| Autor     | Creado              | Url                                                          |
| --------- | ------------------- | ------------------------------------------------------------ |
| Fisherman |  21-03-20 23:34:03  | https://tests.stockfishchess.org/api/nn/nn-a9f9e868162a.nnue |


## :wrench: Instalación

Coiled es un motor y como tal no incluye de un tablero de ajedrez, solo calcula los movimientos.
Es por eso que requiere de una parte gráfica para que el usuario final pueda jugar cómodamente.
Existen diversos programas para esta tarea, pudiendo elegir el que más agrade al usuario.

GUI de entornos gratuito:

[Arena Chess Gui](http://www.playwitharena.de/)

[Tarrasch Chess GUI](https://www.triplehappy.com/)

[Banksia GUI](https://banksiagui.com/)

[Cute Chess GUI](https://cutechess.com/)

[Un pequeño listado de GUI](https://www.chessprogramming.org/GUI)

Consultar el manual de instalación de un motor de ajedrez en dicha GUI.


## :trophy: Progreso de ELO 


| VERSION NNUE        | [Chess Engines Diary](https://chessengines.blogspot.com/) | [SPCC](https://www.sp-cc.de/) | [Owl Chess Blitz](http://chessowl.blogspot.com/) |
| ------------------- | --------------------------------------------------------- | ----------------------------- | ------------------------------------------------ |
| Coiled 1.1          | 3326 ±                                                    | 3343 ±                        |  3110 ±                                          |


| VERSION NO NNUE     | [CCRL Blitz](http://ccrl.chessdom.com/ccrl/404/)  | [CCRL (40/15)](http://ccrl.chessdom.com/ccrl/4040/) | [CEGT (40/4)](http://www.cegt.net/40_4_Ratinglist/40_4_single/rangliste.html) | [CCRL 40/2 FRC](http://ccrl.chessdom.com/ccrl/404FRC/) |
| ------------------- | ------------------------------------------------- | --------------------------------------------------- | ----------------------------------------------------------------------------- | ------------------------------------------------------ |
| Coiled 0.1a         | 1692 ±                                            |                                                     |                                                                               |                                                        |
| Coiled 0.2b         | 1998 ±                                            |                                                     | 1824 ±                                                                        |                                                        |
| Coiled 0.4          | 2510 ±                                            | 2565 ±                                              | 2334 ±                                                                        |                                                        |
| Coiled 0.5          | 2613 ±                                            |                                                     |                                                                               |                                                        |
| Coiled 0.6          | 2619 ±                                            | 2590 ±                                              | 2445 ±                                                                        |                                                        |
| Coiled 1.1          | 2764 ±                                            | 2790 ±                                              |                                                                               | 2652 ±                                                 |


## :gear: Opciones Uci

|                     OPCIONES                        |                                          DESCRIPCION                                                                           |
| --------------------------------------------------- | ------------------------------------------------------------------------------------------------------------------------------ |
| Hash                                                | Para ajustar el tamaño de la tabla hash en MB.                                                                                 |
| Threads                                             | Numero de subprocesos que usará mientras realiza la búsqueda. (por defecto: 1)                                                 |
| MoveOverhead                                        | Reserva X milisegundos para la comunicación del movimiento y la interfaz gráfica de usuario. (por defecto: 50)                 |
| NnueFile \*                                         | Para asignar la red neuronal a utilizar. Sin red neuronal, juega con la evaluación interna. (por defecto: Vació)               |
| UCI\_Chess960                                       | Activa/desactiva el modo de juego Chess960. Se configura automáticamente por la GUI. (por defecto: false)                      |


\* Opciones disponibles, solo en la versión de 64bit.

## :open_book: Comandos adicionales no oficiales UCI

Existe la posibilidad de realizar un Perft. Esta opción sirve para descubrir fallos en la generación de movimientos.

Active la opción 'UCI_Chess960' para realizar 'perft' en tableros Chess960.

Puedes realizar una búsqueda de todos los movimientos hasta la profundidad que se desee, con divide.

Escribiendo los siguientes comandos:

Desde una posición en concreto:

- setoption name Threads value 4

- position fen 8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - -

- perft <X>



## :hammer_and_wrench: Compilando

Puede descargar Coiled ya compilado desde [aquí](https://github.com/Oscar-Gavira/Coiled/releases)

Si prefiere compilar Coiled para mejor compatibilidad con su CPU.

Requisitos: Tener instalado Git. Ubuntu: sudo apt install git.

En Linux:
```bash
$ git clone https://github.com/Oscar-Gavira/Coiled
$ cd Coiled/src
$ make
$ ./Coiled_1.2_avx2_x64
```

En Windows, usar:
* [MSYS2.](https://www.msys2.org/) Versión actual: gcc version 13.1.0 (Rev6, Built by MSYS2 project) Thread model: posix. Supported LTO compression algorithms: zlib zstd

Una vez instalado MSYS2. Abrimos MSYS2 MINGW64 o MSYS2 CLANG64.
<img src="https://github.com/Oscar-Gavira/Coiled/blob/main/src/makefile_msys.png" width="75%" height="75%" />

```bash
$ git clone https://github.com/Oscar-Gavira/Coiled
$ cd Coiled/src
$ make
$ cp -r release c:/Users/[Tu Usuario]/Documents/Coiled
```

Ahora puedes ir al navegador de archivos en Documentos, y encontrar la carpeta Coiled, donde estará compilado Coiled.


## :pencil: Términos de Uso

Coiled es gratuito y se distribuye bajo la licencia pública general GNU versión 3 (GPL v3). Básicamente, esto significa que eres libre de hacer casi exactamente lo que quieras con el programa, incluso distribuirlo entre tus amigos, hacer que esté disponible para descargarlo de tu sitio web, venderlo (ya sea solo o como parte de algún paquete de software más grande), o utilizándolo como punto de partida para un proyecto de software propio.

La única limitación real es que siempre que distribuya Coiled de alguna manera, siempre debe incluir el código fuente completo o un puntero al lugar donde se puede encontrar el código fuente. Si realiza algún cambio en el código fuente, estos cambios también deben estar disponibles bajo la GPL.

Para obtener todos los detalles, lea la copia de la GPL v3 que se encuentra en el archivo llamado [LICENSE](https://github.com/Oscar-Gavira/Coiled/blob/master/LICENSE).


## :gift: Agradecimientos

Estoy muy agradecido a las siguientes personas:

- A mi familia y amigos, que siempre están ahí para ayudar.

- A todos aquellos que han decidido compartir el código fuente de sus motores (Me han aclarado funciones y entendimiento de ellos).

- A la web CPW [https://www.chessprogramming.org/](https://www.chessprogramming.org/) por todo el contenido sobre las técnicas aplicables a un motor ajedrez.

- A todos aquellos que han jugado con el motor y han publicado su experiencia y opiniones.

- A los archivos de rendimientos .epd de posiciones estratégicas tipo: WAC, IQ6, mes400... En especial a la web [https://sites.google.com/site/strategictestsuite/](https://sites.google.com/site/strategictestsuite/)

- A la web [CCRL 40/4](http://www.computerchess.org.uk/ccrl/404/), [CEGT 40/4](http://www.cegt.net/40_4_Ratinglist/40_4_single/rangliste.html), [Chess Engines Diary](https://chessengines.blogspot.com/), [Owl Chess Blitz](http://chessowl.blogspot.com/), [BRUCE - Bullet Rating of UCI](https://e4e6.com/), [SPCC](https://www.sp-cc.de/) por realizar un torneo y darle una clasificación de ELO.

- Alex Chess del foro talkchess, por el torneo realizado en [chess.com](https://www.chess.com/forum/view/game-analysis/freeware-against-commercial-chess-software-arena-3-5-1-vs-chessbase-fritz-17)

- A toda web que haya publicado una clasificación ELO o/y haya añadido un enlace a mi web.

- A Pedro Castro por ser el primero en probar y realizar un torneo. Autor de la web Motores de ajedrez hispanos y autor de DanaSah.

- A la web [Motores de Ajedrez Hispanos](https://sites.google.com/site/motoresdeajedrez/Home) por realizar su primer torneo y darle una clasificación de ELO en su primera versión.

- Ronald de Man's, por el proyecto Fathom con el que poder implementar el uso de las tablas de finales Syzygy. [https://github.com/jdart1/Fathom](https://github.com/jdart1/Fathom)

- A Daniel Shawul, por el proyecto [nnue-probe](https://github.com/dshawul/nnue-probe/) que ha sido de utilidad para implementar el uso de redes neuronales, utilizando una versión modificada del excelente código [Cfish](https://github.com/syzygy1/Cfish)

- Grupo de Stockfish. [Aquí](https://tests.stockfishchess.org/nns) encontraras una gran variedad de redes neuronales.

Seleccionar la red neuronal descargada mediante las opciones UCI.

---
con :heart: por [Oscar](https://github.com/Oscar-Gavira) 😊

