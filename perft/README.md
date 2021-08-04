Source: https://github.com/AndyGrant/Ethereal/tree/master/src/perft

# Perft
* Perft, (prueba de rendimiento, enumeración de movimiento) una función de depuración para recorrer el árbol de generación de movimientos estrictamente legales para contar todos los nodos finales, hasta una profundidad indicada.


# Requisitos
* Python version >= 3.7.3


# Línea de comando
Para la versión fischer.epd, se ha de compilar con la opción UCI_Chess960 = true por defecto. O las fallara todas.
* perft.py "Coiled_07_x64.exe" "standard.epd" > standard.txt
* perft.py "Coiled_07_x64_Chess960.exe" "fischer.epd" > fischer.txt

La opción "> standard.txt" es para volcar la salida a un archivo.


# Ayuda
Usage:
* perft.py (motor) (epd)

