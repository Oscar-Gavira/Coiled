![Release](https://img.shields.io/github/v/release/Oscar-Gavira/Coiled)
[![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)

# ![imagen](https://user-images.githubusercontent.com/33672268/136560248-95e1ebf4-9405-414c-9827-6f1f3cf76b05.png) Coiled - Motor de ajedrez.

## :clipboard: Características

|                            |    CARACTERISTICAS                                                                              |
| -------------------------- | ----------------------------------------------------------------------------------------------- |
| Protocolo de comunicación  | [UCI](https://www.shredderchess.com/download/div/uci.zip)                                       |
| Código abierto             | Si                                                                                              |
| Tipo de licencia           | [GNU General Public License v3.0](https://github.com/Oscar-Gavira/Coiled/blob/master/LICENSE)   |
| Sistema operativo          | Windows y Linux. 32&64 bit                                                                      |
| Representación del tablero | Mailbox. 8x8 unidimensional.                                                                    |
| Ajedrez960 / FRC           | Si.                                                                                             |
| Soporta redes neuronales   | Si. (NNUE)                                                                                      |
| Tablas de finales          | Si. Syzygy. Por Ronald de Man. Gaviota. Por Miguel Ballicora. BitBases. Por daniel shawul.      |
| Libro de aperturas         | Si. Formato SqlIte                                                                              |
| Lenguaje de programación   | C                                                                                               |
| Ponder                     | No                                                                                              |
| Multihilo                  | No                                                                                              |
| ELO estimado               | ± 3325 NNUE. ± 2700 Interna.                                                                    |


- Tablas de finales disponibles en internet. [http://oics.olympuschess.com/tracker/](http://oics.olympuschess.com/tracker/)


## :rocket: Introducción

Coiled es un motor de ajedrez de protocolo UCI escrito en C. Es un programa de línea de comandos (Aplicación de consola), que no está diseñado para usarse directamente, sino a través de una interfaz de usuario o GUI, con capacidad para tratar el protocolo UCI, como: Cute Chess, Tarrasch, Banksia, Arena, Fritz...

En un principio el motor lo realicé en Visual basic .Net, pero su rendimiento era bajo. Finalmente opté por hacerlo en el lenguaje de programación C.

Reconozco que me ha costado y me cuesta manejar C, ya que no he programado nunca en este formato, sobre todo el trato con cadenas (lo que sería un string). Sin embargo, la sintaxis es parecida a PHP (sintaxis que sí conozco) y casi todo el programa son funciones matemáticas y poco trato con cadenas.

En agosto del 2013, Coiled fue publicado por primera vez para el usuario final. Desde entonces, he ido realizando actualizaciones, depurando errores, incrementando su fuerza de ELO, etc.

Actualmente Coiled es capaz de jugar a la variante Chess960 de usar tablas de finales, soporta redes neuronales.

Para leer más sobre algunas de las técnicas utilizadas en Coiled, consulte la página Wiki de programación de ajedrez de [Coiled.](https://www.chessprogramming.org/Coiled) (inglés)


## :ballot_box_with_check: Requisitos mínimo

| SISTEMA OPERATIVO | Windows 32 bit             | Windows 64 bit.                                           |
| ----------------- | -------------------------- | --------------------------------------------------------- |
| PROCESADOR        | Intel Pentium Pro o Amd K7 | Intel Core 2 o Ahtlon 64 con soporte MMX, SSE, SSE2, SSE3 |
| RAM               | 16 MB.                     | 16 MB.                                                    |


## :nut_and_bolt: Redes neuronales (NNUE)


Actualmente Coiled utiliza la red neuronal de Fisherman.

| Autor     | Creado              | Url                                                          |
| --------- | ------------------- | ------------------------------------------------------------ |
| Fisherman |  21-03-20 23:34:03  | https://tests.stockfishchess.org/api/nn/nn-a9f9e868162a.nnue |



## :wrench: Instalación

GUI de entornos gratuito:

[Arena Chess Gui](http://www.playwitharena.de/)

[Tarrasch Chess GUI](https://www.triplehappy.com/)

[Banksia GUI](https://banksiagui.com/)

[Cute Chess GUI](https://cutechess.com/)

[Un pequeño listado de GUI](https://www.chessprogramming.org/GUI)

Consultar el manual de instalación de un motor de ajedrez en dicha GUI.


## :trophy: Clasificación ELO 


| VERSION NNUE |   ELO  |
| ------------ | ------ |
| Coiled 0.7   | 3050 ± |
| Coiled 0.8   | 3250 ± |
| Coiled 0.9   | 3250 ± |
| Coiled 1.0   | 3250 ± |
| Coiled 1.1   | 3300 ± |

- [CCRL (40/4)](http://ccrl.chessdom.com/ccrl/404/)

| VERSION NO NNUE     |   ELO  |
| ------------------- | ------ |
| Coiled 0.1a         | 1694 ± |
| Coiled 0.2b         | 1996 ± |
| Coiled 0.4          | 2508 ± |
| Coiled 0.5          | 2600 ± |
| Coiled 1.1          | 2725 ± |


- [CEGT (40/4)](http://www.cegt.net/40_4_Ratinglist/40_4_single/rangliste.html)

| VERSION NO NNUE     |   ELO  |
| ------------------- | ------ |
| Coiled 0.2b         | 1824 ± |
| Coiled 0.4          | 2334 ± |
| Coiled 0.6          | 2446 ± |


## :gear: Opciones Uci
|                                    OPCIONES                                                                |                                          DESCRIPCION                                                                     |
| ---------------------------------------------------------------------------------------------------------- | ------------------------------------------------------------------------------------------------------------------------ |
| Hash                                                                                                       | Para ajustar el tamaño de la tabla hash en MB.                                                                           |
| OwnBook                                                                                                    | Para usar el libro de apertura propio del motor. (por defecto: false)                                                    |
| OwnBookLimit                                                                                               | Para limitar el número de jugadas máximas obtenidas desde el libro de apertura. (por defecto: 8)                         |
| PreventTimeout                                                                                             | Para evitar Timeout en un juego por tiempo. Valor en milisegundo. Descuenta X ms al tiempo por jugada. (por defecto: 150)|
| EndGamesTablebases                                                                                         | Seleccione entre la tabla de finales a usar: Ninguna, Syzygy, Gaviota y BitBases. (por defecto: Ninguna)                 |
| EndGamesTablebasesPath                                                                                     | Para asignar la carpeta/directorio de las tablas de finales. Syzygy, Gaviota y BitBases. (por defecto: Vacío)            |
| EndGamesTablebasesCache                                                                                    | Para ajustar el tamaño de la cache en MB de las tablas de finales. Gaviota y BitBases. (por defecto: 32MB)               |
| EndGamesTablebasesLimit                                                                                    | Indica a partir de cuantas piezas busca en la tabla de finales. Syzygy, Gaviota y BitBases. (por defecto: 5)             |
| NnuePath \*                                                                                                | Para asignar la ruta de la red neuronal. (por defecto: Vació)                                                            |
| NnueTechnology \*                                                                                          | Indica el tipo de tecnología que utiliza la red neuronal. AVX2, SSE4.1, SSE3 o SSE2. (por defecto: Ninguno)              |
| UCI\_Chess960 \*\*                                                                                         | Activa/desactiva el modo de juego Chess960. (por defecto: false)                                                         |
| UCI\_Chess960CastlingSign \*\*                                                                             | Indica si utiliza el enroque estándar del protocolo UCI o el de la GUI Arena (por defecto: UCI)                          |



\* Opciones disponibles, solo en la versión de 64bits.

\*\* Si activa UCI\_Chess960 y utiliza la GUI de Arena, en la opción UCI\_Chess960CastlingSign debes seleccionar la configuración: O-O/O-O-O. Para un correcto funcionamiento con la GUI Arena.

## :open_book: Comandos adicionales no oficiales UCI

- perft [x]: Ejecutando perft hasta [profundidad] en la posición actual.

- perftdiv [x]: Ejecutando perft divide hasta [profundidad] en la posición actual.

- book: Verifica el libro de aperturas.

Estas opciones también están disponibles para el modo Chess960, solo has de activar el modo UCI_Chess960.
    
## :hammer_and_wrench: Construido y compilado con

* [CodeBlocks](https://www.codeblocks.org/)
* [Visual Studio](https://visualstudio.microsoft.com/es/)
* MinGW 8.1 incluido en codeblocks.

- Puede usar cualquier otro editor, al igual que puede usar otro compilador. 
- Para compilar en linux: [makefile](https://github.com/Oscar-Gavira/Coiled/blob/master/src/makefile)
- Formas de compilar: make help
- Para compilar en Windows 32 bit: [makefile](https://github.com/Oscar-Gavira/Coiled/blob/master/src/makefile) con [codeblocks](https://github.com/Oscar-Gavira/Coiled/blob/master/src/Coiled_Windows_x86.cbp)
- Para compilar en Windows 64 bit: [makefile](https://github.com/Oscar-Gavira/Coiled/blob/master/src/makefile) con [codeblocks](https://github.com/Oscar-Gavira/Coiled/blob/master/src/Coiled_Windows_x64.cbp)

<img src="https://github.com/Oscar-Gavira/Coiled/blob/master/src/makefile_codeblocks.jpg" width="50%" height="50%" />



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

- A la web [CCRL 40/4](http://www.computerchess.org.uk/ccrl/404/), [CEGT 40/4](http://www.cegt.net/40_4_Ratinglist/40_4_single/rangliste.html), [Chess Engines Diary](https://chessengines.blogspot.com/), [Owl Chess Blitz](http://chessowl.blogspot.com/) por realizar un torneo y darle una clasificación de ELO.


- Alex Chess del foro talkchess, por el torneo realizado en [chess.com](https://www.chess.com/forum/view/game-analysis/freeware-against-commercial-chess-software-arena-3-5-1-vs-chessbase-fritz-17)


- A toda web que haya publicado una clasificación ELO o/y haya añadido un enlace a mi web.

- A Pedro Castro por ser el primero en probar y realizar un torneo. Autor de la web Motores de ajedrez hispanos y autor de DanaSah.

- A la web [Motores de Ajedrez Hispanos](https://sites.google.com/site/motoresdeajedrez/Home) por realizar su primer torneo y darle una clasificación de ELO en su primera versión.

- Ronald de Man's, por el proyecto Fathom con el que poder implementar el uso de las tablas de finales Syzygy. [https://github.com/jdart1/Fathom](https://github.com/jdart1/Fathom)

- A Daniel Shawul, por el proyecto [nnue-probe](https://github.com/dshawul/nnue-probe/) que ha sido de utilidad para implementar el uso de redes neuronales, utilizando una versión modificada del excelente código [Cfish](https://github.com/syzygy1/Cfish)

- Grupo de Stockfish. [Aquí](https://tests.stockfishchess.org/nns) encontraras una gran variedad de redes neuronales.

Seleccionar la red neuronal descargada mediante las opciones UCI o renombrar el archivo a red_neuronal.nnue (Nombre por defecto) y colóquelo en el mismo directorio del motor Coiled.

---
con :heart: por [Oscar](https://github.com/Oscar-Gavira) 😊

