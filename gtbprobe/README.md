Source: https://github.com/michiguel/Gaviota-Tablebases

                 Gaviota Tablebases Probing Code API
                Copyright (c) 2010-2011 Miguel A. Ballicora
-----------------------------------------------------------------------------

This software provides C code to probe the Gaviota Endgame Tablebases.
It is released under then X11 ("MIT") license (see license.txt).

This API (Application Programming Interface) is designed to be as portable 
as possible. Functions could be called from Linux or Windows. 
Most likely it will work in other operating systems but that has not been 
tested. This API is a beta version and as such, it is not guaranteed any 
type of backward compatibility or to remain untouched, at least until 
version 1.0 is released. 

A very small set of tablebase files is included in this distribution 
for testing purposes (only 3 pieces). They are compressed with four
different compression schemes. For a more complete set, please download 
Gaviota from

http://sites.google.com/site/gaviotachessengine/

and generate the 4 and 5 piece tablebases. Instructions how to generate
and compressed them are in the website. More information can always be found:

http://sites.google.com/site/gaviotachessengine/Home/endgame-tablebases-1

Alternatively, already compressed tablebase files (ready to go!) can be 
downloaded from

http://oics.olympuschess.com/tracker/index.php (Many thanks to Josh Shriver)

"tbprobe" is distributed here as a tablebase probing example. The current API
is relatively "low level" to optimize performance. We hope the small program 
tbprobe is self explanatory. A thorough and detailed documentation may be 
released later. However, it seems that everybody who attempted to implement
this probing code was successful relatively easy.

We plan to support an interface with a FEN notation; thus, it is expected 
that some other functions maybe added to this API.

Four different types of compression are included. It is possible that in the
future some other compression schemes could be provided, but only if they
represent a serious improvement in speed or memory size. To maximize
backward compatibility between versions of programs and TBs, it is strongly
