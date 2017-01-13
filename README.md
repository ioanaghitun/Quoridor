# Quoridor
-- Proiect la disciplina "Introducere în programare" 2016-2017 --

Proiectul este realizat în Visual Studio 2015, în limbaj c++ și folosind SDL2.

Pentru a seta SDL2 in Visual Studio, deschideti proiectul si mergeti la 'Project'->'Properties',
la categoria 'Linker'->'General'->'Additional Library Directories' adaugati biblioteca din arhiva "SDL2(lib+include).zip" folderul "lib".
La categoria 'C/C++'->'General' adaugati folderul "include" din aceeasi arhiva. In categoria 'Linker'->'Input' treceti la 'Additional Dependencies': 
"SDL2.lib
SDL2main.lib
SDL2_image.lib
SDL2_ttf.lib"
