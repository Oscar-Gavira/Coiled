# Coiled-NNUE

[Coiled-NNUE](https://www.oscargavira.es/?sec=Coiled_Informacion) es un motor de ajedrez de protocolo UCI escrito en C. Es un programa de línea de comandos (Aplicación de consola), que no está diseñado para usarse directamente, sino a través de una interfaz de usuario o GUI, con capacidad para tratar el protocolo UCI, como: Cute Chess, Tarrasch, Banksia, c-chess-cli, Arena, Fritz, [Ver más](https://www.chessprogramming.org/GUI#Chess_GUIs)

La principal fuente de información para el desarrollo de motores de ajedrez son las páginas Wiki de programación de ajedrez (https://www.chessprogramming.org/Main_Page).

También he utilizado la información disponible en Internet, código fuente de otros motores como: Rodent, Tucano, Ethereal, Demolito, Bison, Tscp, Cpw, Crafty, Olithink...

El motor esta preparado para usar redes neuronales.
En esta [web](https://tests.stockfishchess.org/nns) encontraras una gran variedad, de entre todas ellas hay cuatro o cinco oficiales o que ha pasado los test de pruebas (Mejor ELO).
Puede que hayan algunos archivos .nnue que no pueda cargarlos sobre todo los de tamaño de 40MB. Supongo que por falta de compatibilidad de la .dll y la red neuronal.



## El protocolo UCI y las opciones disponibles

La Interfaz Universal de Ajedrez (UCI) es un protocolo estándar que se utiliza para comunicarse con
un motor de ajedrez, y es la forma recomendada de hacerlo para las interfaces gráficas de usuario típicas
(GUI) o herramientas de ajedrez. Coiled-NNUE implementa la mayoría de sus opciones como se describe
en [el protocolo UCI] (https://www.shredderchess.com/download/div/uci.zip). 



## Parámetros UCI

Actualmente, Coiled-NNUE tiene las siguientes parámetros UCI:

  * #### Hash
    Para ajustar el tamaño de la tabla hash en MB.

  * #### ShowPv
    Limita la información enviada a la GUI en el proceso de calculo.

  * #### OwnBook
    Para usar el libro de apertura propio del motor.

  * #### OwnBookLimit
    Para limitar el numero de jugadas máximas obtenidas desde el libro de apertura.

  * #### GaviotaUse
    Activa/desactiva el uso de tablas de finales de Gaviota.

  * #### GaviotaPath
    Para asignar la carpetas/directorios de las tablas de finales de Gaviota.

  * #### GaviotaCache
    Asigna la cache de las tablas de finales de Gaviota.

  * #### GaviotaProbeLimit
    Indica a partir de cuantas piezas busca en la tabla de finales.

  * #### NnueUse *
    Activa/desactiva el uso de de una red neuronal (nnue).

  * #### NnuePath *
    Para asignar el archivo de la red neuronal (nnue).

  * #### UCI_Chess960 **
    Activa/desactiva el modo de juego Chess960. Acepta las anotaciones Shredder-FEN (AHah) o X-FEN (KQkq).

  * #### UCI_Chess960CastlingSign **
    Indica si utiliza el enroque estándar del protocolo UCI o el de la GUI Arena.


  [*] Esta versión solo funciona con redes neuronales, no dispone de evaluación interna.
  
  [**] Si activa UCI_Chess960 y utiliza la GUI de Arena, en la opción UCI_Chess960CastlingSign debes seleccionar la configuración: O-O/O-O-O. Para un correcto funcionamiento con la GUI Arena.

	

## Archivos

Esta distribución de Coiled-NNUE consta de los siguientes archivos:

   * [Readme.md](https://github.com/Oscar-Gavira/Coiled-NNUE/blob/main/README.md), el archivo que está leyendo actualmente.

   * [License](https://github.com/Oscar-Gavira/Coiled-NNUE/blob/main/LICENSE), un archivo de texto que contiene la licencia pública general GNU versión 3.

   * [src](https://github.com/Oscar-Gavira/Coiled-NNUE/tree/main/src), un subdirectorio que contiene el código fuente completo de Coiled-NNUE. 
	 
   * [nnue](https://github.com/Oscar-Gavira/Coiled-NNUE/tree/main/nnue), un subdirectorio que contiene el código fuente para probar las redes neuronales NNUE. Genera una librería para ser utilizado por Coiled-NNUE.
	 
   * [gtbprobe](https://github.com/Oscar-Gavira/Coiled-NNUE/tree/main/gtbprobe), un subdirectorio que contiene el código fuente para probar las bases de finales de gaviota. Genera una librería para ser utilizado por Coiled-NNUE.
   


## Términos de Uso

Coiled-NNUE es gratuito y se distribuye bajo la licencia pública general GNU versión 3 (GPL v3). Básicamente, esto significa que eres libre de hacer casi exactamente lo que quieras con el programa, incluso distribuirlo entre tus amigos, hacer que esté disponible para descargarlo de tu sitio web, venderlo (ya sea solo o como parte de algún paquete de software más grande), o utilizándolo como punto de partida para un proyecto de software propio.

La única limitación real es que siempre que distribuya Coiled-NNUE de alguna manera, siempre debe incluir el código fuente completo o un puntero al lugar donde se puede encontrar el código fuente. Si realiza algún cambio en el código fuente, estos cambios también deben estar disponibles bajo la GPL.

Para obtener todos los detalles, lea la copia de la GPL v3 que se encuentra en el archivo llamado [LICENSE](https://github.com/Oscar-Gavira/Coiled-NNUE/blob/main/LICENSE). 



## Agradecimientos

Estoy muy agradecido a las siguientes personas:

- A mi familia y amigos, que siempre están ahí para ayudar.

- A todos aquellos que han decidido compartir el código fuente de sus motores (Me han aclarado funciones y entendimiento de ellos).

- A la web CPW https://www.chessprogramming.org/Main_Page por todo el contenido sobre las técnicas aplicables a un motor ajedrez.

- A todos aquellos que han jugado con el motor y han publicado su experiencia y opiniones.

- A los archivos de rendimientos .epd de posiciones estratégicas tipo: WAC, IQ6, mes400... En especial a la web https://sites.google.com/site/strategictestsuite/

- A la web CCRL 40/4 y a CEGT 40/4, por realizar un torneo y darle una clasificación de ELO.

- A toda web que haya publicado una clasificación ELO o/y haya añadido un enlace a mi web.

- A Pedro Castro por ser el primero en probar y realizar un torneo. Autor de la web Motores de ajedrez hispanos y autor de DanaSah.

