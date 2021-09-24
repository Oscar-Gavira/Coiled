Source: https://github.com/AndyGrant/Ethereal/tree/master/src/perft

# Perft
* Perft, (prueba de rendimiento, enumeración de movimiento) una función de depuración para recorrer el árbol de generación de movimientos estrictamente legales para contar todos los nodos finales, hasta una profundidad indicada.


# Requisitos
* Python version >= 3.7.3


# Línea de comando
* python3 perft.py "Coiled_07_x64.exe" "standard.epd" > standard.txt
* python3 perft.py "Coiled_07_x64_Chess960.exe" "fischer.epd" --chess960 true > fischer.txt

La opción "> standard.txt" o " > fischer.txt" es para volcar la salida a un archivo.


# Ayuda
Usage:
* python3 perft.py (motor) (epd) --chess960 true --depth 3

Para más ayuda:
* python3 perft.py -h

