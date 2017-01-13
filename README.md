# Quoridor
<h3><b>-- Proiect la disciplina "Introducere în programare" 2016-2017 --</b></h3>

Proiectul este realizat în Visual Studio 2015, în limbaj c++ și folosind SDL2.

Pentru a seta SDL2 în Visual Studio, deschideți proiectul și mergeți la <b>'Project'->'Properties'</b>, categoria <b>'C/C++'->'General'</b> și adăugați folderele "include" din arhiva <b>"SDL2(lib+include).zip"</b>.

După aceea, la categoria <b>'Linker'->'General'->'Additional Library Directories'</b>, adaugati bibliotecile din arhivă (folderele "lib" potrivita versiunii voastre de Visual Studio).

In categoria <b>'Linker'->'Input'</b> treceti la <b>'Additional Dependencies'</b>: 

- SDL2.lib

- SDL2main.lib

- SDL2_image.lib

- SDL2_ttf.lib
