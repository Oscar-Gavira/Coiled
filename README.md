Coiled - Motor de ajedrez por Oscar Gavira.
===============================================

Protocolo de comunicación: UCI

Código abierto: Si

Tipo de licencia: GNU General Public License v3.0

Tabla de contenido
==================

1) Introducción.

2) Código abierto.

3) Instalación.

4) Requisitos.

5) Progreso de ELO.

6) Técnicas utilizadas.

7) Opciones UCI.

8) Rendimiento.

9) Licencia.

10) Términos de Uso.

11) Agradecimientos.

Introducción
============

Coiled es un motor de ajedrez de protocolo UCI escrito en C. Es un programa de línea de comandos (Aplicación de consola), que no está diseñado para usarse directamente, sino a través de una interfaz de usuario o GUI, con capacidad para tratar el protocolo UCI, como: Cute Chess, Tarrasch, Banksia, c-chess-cli, Arena, Fritz...

Su origen empezó hace un tiempo, cuando decidí actualizar el programa de AjedrezNet (AjedrezNet es una GUI que es capaz de gestionar motores de ajedrez de protocolo XBoard, realizado en vb6) a una versión más moderna, utilizando la plataforma Framework .Net, con mejor soporte para Windows Vista, Windows 7 y 10.

En primer lugar, intenté realizar la migración de AjedrezNet a la plataforma Framework.Net y el resultado no me terminó de gustar, por ello empecé a reescribir todo el código con una estructura mejorada. Pero tampoco me gustaba, así que decidí generar mi propio motor de ajedrez.

Actualmente, la GUI, que viene siendo la parte gráfica (AjedrezNet), ha pasado a un segundo plano para mí, por lo que no estoy centrando mi trabajo en él, sino que estoy volcado en el motor Coiled.

En un principio el motor lo realicé en Visual basic .Net ya que la idea era unirlo a la parte gráfica (AjedreNet). Pero su rendimiento era bajo. Finalmente opté por hacerlo en el lenguaje de programación C.

Reconozco que me ha costado y me cuesta manejar C, ya que no he programado nunca en este formato, sobre todo el trato con cadenas (lo que sería un string). Sin embargo, la sintaxis es parecida a PHP (sintaxis que sí conozco) y casi todo el programa son funciones matemáticas y poco trato con cadenas.

En agosto del 2013, Coiled fue publicado por primera vez para el usuario final. Desde entonces, he ido realizando actualizaciones, depurando errores, incrementando su fuerza de ELO, etc.

Actualmente Coiled es capaz de jugar a la variante Chess960, tablas de finales, soporta redes neuronales aunque hay algunas redes neuronales que no pueda cargar, sobre todo los de tamaño de 40MB.

Esta versión a sido compilado para Windows con la plataforma CodeBlocks y MinGW versión 8.1.0

Para leer más sobre algunas de las técnicas utilizadas en Coiled, consulte la página Wiki de programación de ajedrez de [Coiled.](https://www.chessprogramming.org/Coiled) (inglés)

Código abierto
==============

Ahora Coiled es de código abierto.

Quiero aportar mi pequeño granito de arena a la comunidad de investigación, que tanto me ha aportado a mi en la creación de Coiled.

Espero no infligir nada, no ofender a nadie y de ser así hablar del tema.

Toda ayuda y mejora sera bien recibida.

También quiero decir que es mi primer proyecto de código abierto.

El proyecto esta alojado en [github](https://github.com/Oscar-Gavira/Coiled).

Instalación
===========

Coiled es un motor y como tal no incluye de un tablero de ajedrez, solo calcula los movimientos. Es por eso que requiere de una parte gráfica para que el usuario final pueda jugar cómodamente. Existen diversos programas para esta tarea, pudiendo elegir el que más agrade al usuario. Algunos de ellos son:

Entornos gratuito

[Arena Chess Gui](http://www.playwitharena.de/)

[Tarrasch Chess GUI](https://www.triplehappy.com/)

[Lucas Chess GUI](https://lucaschess.pythonanywhere.com/)

[PyChess GUI](https://pychess.github.io/)

[ChessX GUI](https://chessx.sourceforge.io/)

[Cute Chess GUI](https://cutechess.com/)

[Un pequeño listado de GUI](https://www.chessprogramming.org/GUI)

Consultar el manual de instalación de un motor de ajedrez en dicha GUI.

Requisitos hardware
===================

Actualmente Coiled solo funciona en 64bit por las redes neuronales :

**SISTEMA OPERATIVO** Windows 64 bit.

**PROCESADOR** Intel o Ahtlon con soporte SSE3 o SSE4.1 o AVX2.

**RAM** 128MB Mínimo.


Progreso de ELO
===============

Versión 0.3 No publicada. Se incrementó en 250. Dando así una fuerza de 2250 ELO.

Coiled-NNUE 0.7	± 3050 ELO.

La versión actual de Coiled 0.9 calculo que alcanza los 3250 ELO.


### CCRL (40/4)

Coiled 0.1a	± 1694 ELO.

Coiled 0.2b	± 1996 ELO.

Coiled 0.4	± 2508 ELO.

Coiled 0.5/0.6	± 2600 ELO.

### CEGT (40/4)

Coiled 0.2b	± 1824 ELO.

Coiled 0.4	± 2334 ELO.

Coiled 0.5/0.6	± 2446 ELO.


Técnicas utilizadas
===================

Protocolo: UCI.

Representación del tablero: Mailbox. 8x8 unidimensional tipo entero.

Generador: Sencillo.

Búsqueda: Profundidad iterativa. Ventana de aspiración. PVS / Alpha-Beta. Quiescence.

Reducciones y podas: Distancia mate. Movimiento nulo poda. Reducciones de movimientos finales (LMR). Poda de movimientos finales (LMP). Tabla de transposición poda. See (Static Exchange Evaluation).

Extensiones: Jaque.

Ordenación: Movimiento Hash. Profundización iterativa Interna (IID). MVV/LVA. Killer heurístico. Histórico heurístico. Movimiento refutación. See (Static Exchange Evaluation).

Evaluación: Redes neuronales (NNUE). Evaluación interna, en construcción.

Otros: Detección final del juego con las reglas de la FIDE. Incluyendo triple repetición y la regla de 50 movimientos. Tablas por material insuficiente.

Soporta redes neuronales: Si. (Formato NNUE)

Tabla de transposición: Si.

Tabla de finales: Si. Gaviota. Por Miguel Ballicora. \* Si. Syzygy. Por Ronald de Man. \*\*

Libro de aperturas: Si.

Chess960: Si.

Lenguaje de programación: C

Modo de juego:

Por tiempo: Convencional. Convencional e incremental. Por movimiento.
Profundidad. Mate. Infinito.

ELO estimado: ± 3250.

\* Formato de compresión .cp4 (LZMA). Disponibles en internet. [http://oics.olympuschess.com/tracker/](http://oics.olympuschess.com/tracker/)

\*\* Disponibles en internet. [http://oics.olympuschess.com/tracker/](http://oics.olympuschess.com/tracker/)

Opciones Uci
============

Hash

Para ajustar el tamaño de la tabla hash en MB.

ShowPv

Limita la información enviada a la GUI en el proceso de calculo.

OwnBook

Para usar el libro de apertura propio del motor. (por defecto: false)

OwnBookLimit

Para limitar el numero de jugadas máximas obtenidas desde el libro de apertura. (por defecto: 8)

GaviotaUse

Activa/desactiva el uso de tablas de finales de Gaviota. (por defecto: false)

GaviotaPath

Para asignar la carpetas/directorios de las tablas de finales de Gaviota. (por defecto: Vacío)

GaviotaCache

Asigna la cache de las tablas de finales de Gaviota. (por defecto: 32MB)

GaviotaProbeLimit

Indica a partir de cuantas piezas busca en la tabla de finales de gaviota. (por defecto: 5)

SyzygyUse \*

Activa/desactiva el uso de tablas de finales Syzygy. (por defecto: false)

SyzygyPath \*

Para asignar la carpetas/directorios de las tablas de finales Syzygy. (por defecto: Vacío)

SyzygyLimit \*

Indica a partir de cuantas piezas busca en la tabla de finales Syzygy. (por defecto: 5)

NnueUse \*

Activa/desactiva el uso de redes neuronales. (por defecto: true)

NnuePath \*

Para asignar la ruta de la red neuronal. (por defecto: red_neuronal.nnue)

NnueTechnology \*

Indica el tipo de tecnología que utiliza la red neuronal para mayor compatibilidad con las CPU. AVX2 o SSE4.1 o SSE3. (por defecto: AVX2)

UCI\_Chess960 \*\*

Activa/desactiva el modo de juego Chess960. (por defecto: false)

UCI\_Chess960CastlingSign \*\*

Indica si utiliza el enroque estándar del protocolo UCI o el de la GUI Arena (por defecto: UCI)

\* Opciones disponibles, solo en la versión de 64bits.

\*\* Si activa UCI\_Chess960 y utiliza la GUI de Arena, en la opción UCI\_Chess960CastlingSign debes seleccionar la configuración: O-O/O-O-O. Para un correcto funcionamiento con la GUI Arena.

Rendimiento
===========

Existe la posibilidad de realizar un Perft. Esta opción sirve para descubrir fallos en la generación de movimientos.

Puedes realizar una búsqueda de todos los movimientos hasta la profundidad que se desee, con divide, escribiendo los siguientes comandos:

- Desde una posición en concreto:

position fen 8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - -

perft \<X\>

perftdiv \<X\>

- También se puede verificar el libro de aperturas escribiendo:

book

Estas opciones también están disponibles para el modo Chess960, solo has de activar el modo UCI\_Chess960.

Licencia
========

GNU General Public License v3.0

Términos de Uso
===============

Coiled es gratuito y se distribuye bajo la licencia pública general GNU versión 3 (GPL v3). Básicamente, esto significa que eres libre de hacer casi exactamente lo que quieras con el programa, incluso distribuirlo entre tus amigos, hacer que esté disponible para descargarlo de tu sitio web, venderlo (ya sea solo o como parte de algún paquete de software más grande), o utilizándolo como punto de partida para un proyecto de software propio.

La única limitación real es que siempre que distribuya Coiled de alguna manera, siempre debe incluir el código fuente completo o un puntero al lugar donde se puede encontrar el código fuente. Si realiza algún cambio en el código fuente, estos cambios también deben estar disponibles bajo la GPL.

Para obtener todos los detalles, lea la copia de la GPL v3 que se encuentra en el archivo llamado [LICENSE](https://github.com/Oscar-Gavira/Coiled/blob/master/LICENSE).

Agradecimientos
===============

Estoy muy agradecido a las siguientes personas:

- A mi familia y amigos, que siempre están ahí para ayudar.

- A todos aquellos que han decidido compartir el código fuente de sus motores (Me han aclarado funciones y entendimiento de ellos).

- A la web CPW [https://www.chessprogramming.org/](https://www.chessprogramming.org/) por todo el contenido sobre las técnicas aplicables a un motor ajedrez.

- A todos aquellos que han jugado con el motor y han publicado su experiencia y opiniones.

- A los archivos de rendimientos .epd de posiciones estratégicas tipo: WAC, IQ6, mes400... En especial a la web [https://sites.google.com/site/strategictestsuite/](https://sites.google.com/site/strategictestsuite/)

- A la web [CCRL 40/4](http://www.computerchess.org.uk/ccrl/404/) y a [CEGT 40/4](http://www.cegt.net/40_4_Ratinglist/40_4_single/rangliste.html), por realizar un torneo y darle una clasificación de ELO.

- A toda web que haya publicado una clasificación ELO o/y haya añadido un enlace a mi web.

- A Pedro Castro por ser el primero en probar y realizar un torneo. Autor de la web Motores de ajedrez hispanos y autor de DanaSah.

- A la web [Motores de Ajedrez Hispanos](https://sites.google.com/site/motoresdeajedrez/Home) por realizar su primer torneo y darle una clasificación de ELO en su primera versión.

- Ronald de Man's, por el proyecto Fathom con el que poder implementar el uso de las tablas de finales Syzygy. [https://github.com/jdart1/Fathom](https://github.com/jdart1/Fathom)

- A Daniel Shawul, por el proyecto [nnue-probe](https://github.com/dshawul/nnue-probe/) que ha sido de utilidad para implementar el uso de redes neuronales, utilizando una versión modificada del excelente código [Cfish](https://github.com/syzygy1/Cfish)

- Grupo de Stockfish. [Aquí](https://tests.stockfishchess.org/nns) encontraras una gran variedad de redes neuronales.

Seleccionar la red neuronal descargada mediante las opciones UCI o renombrar el archivo a red_neuronal.nnue y colóquelo en el mismo directorio que Coiled_09_x64.exe
