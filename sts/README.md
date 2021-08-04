Source: https://github.com/fsmosca/STS-Rating


# STS-Rating
A method to rate chess engines using STS test suite.


# Requirements
* Python version >= 3.7.3


# Command line
* python3 sts_rating.py -f "STS1-STS15_LAN_v3.epd" -e Stockfish.exe -t 1 -h 128 --getrating --log --contempt 50
* sts_rating.py -f "STS1-STS15_LAN_v3.epd" -e "Stockfish.exe" -t 1 -h 128 --proto uci --movetime 2000 --log


# Help
Usage:
* program -f (epdfile) -e (engname) -t (numthreads) --movetime (timeinms)

Options:
* -f or --file <string value>, for epd file input.
* -e or --engine <string value>, name of engine
* -h or --hash <integer value>, hash size in MB, default is 32 MB
* -t or --threads <integer value>, for threads, Cores and Max CPUs setting
* --movetime <integer value>, time in ms, default is 1000ms
* --log, save engine log
* --getrating, calculate CCRL 40/4 rating estimate for uci engines only
* --mps <integer value>, moves per session for winboard engines
* --tc <integer value in minutes or mm:ss>, timce control for winboard engines
* --st <integer or float value>, for winboard engines and search time in seconds, can be 1.2 or 1
* --proto <string, uci or wb>, protocol the engine supports
* --san, will read engine move in SAN format, only for WB engines
* --contempt, for uci engines that supports such option

Example:
* sts_rating.py -f sts01.epd -e "sf 6.exe" -h 128 -t 2 --movetime 3000
* sts_rating.py -f "all sts.epd" -e sf6.exe -h 128 --getrating

  
# Credits
* Dann Corbit and Swaminathan  
https://sites.google.com/site/strategictestsuite/about-1
