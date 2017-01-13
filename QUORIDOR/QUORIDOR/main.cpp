#include<SDL.h>
#include<SDL_image.h>

#include<iostream>
using namespace std;

struct pion {
	int linie;
	int coloana;
};
struct perete {
	int x;
	int y;
	bool fixat;
};

struct pozitie {
	int linie;
	int coloana;
};
struct patratTablaJoc {
	int x;
	int y;
	int w;
	int h;
};

patratTablaJoc matricePatrate[9][9];
pion jucator1, jucator2;
short matriceJoc[17][17];
short matriceAux[17][17];

void MatriceAuxiliara(int x, int y, int pozitie)
{
    int i, j;
	for (i = 0; i < 17; i++)
		for (j = 0; j < 17; j++)
			matriceAux[i][j] = matriceJoc[i][j];
	if (pozitie == 1)
	{
		matriceAux[x][y * 2 + 1] = -1;
		matriceAux[x + 1][y * 2 + 1] = -1;
		matriceAux[x + 2][y * 2 + 1] = -1;
	}
	else
	{
		matriceAux[x][y * 2] = -1;
		matriceAux[x][y * 2 + 1] = -1;
		matriceAux[x][y * 2 + 2] = -1;

	}

}

bool MousePereteVertical(SDL_Event event, int x, int y)
{
	if (event.motion.x >= x && event.motion.x <= x + 18 && event.motion.y >= y && event.motion.y <= y + 43)
		return true;
	return false;
}
bool MousePereteOrizontal(SDL_Event event, int x, int y)
{
	if (event.motion.x >= x && event.motion.x <= x + 40 && event.motion.y >= y && event.motion.y <= y + 15)
		return true;
	return false;
}
void IncarcaImagine(const char *filePath, SDL_Renderer *_renderer, int x, int y, int w, int h)
{
	SDL_Surface *_suprafata = nullptr;
	SDL_Texture *_textura = nullptr;

	SDL_Rect imgRect;
	imgRect.x = x;
	imgRect.y = y;
	imgRect.w = w;
	imgRect.h = h;

	_suprafata = IMG_Load(filePath);
	_textura = SDL_CreateTextureFromSurface(_renderer, _suprafata);

	SDL_RenderCopy(_renderer, _textura, NULL, &imgRect);

	SDL_FreeSurface(_suprafata);
	SDL_DestroyTexture(_textura);

	_suprafata = nullptr;
	_textura = nullptr;
}

void InitializareTablaJoc()
{
	int linie = 0;
	int coloana = 0;
	int pozitieX = 234;
	int pozitieY = 78;
	int variatie = 61;

	for (linie = 0; linie <= 9; linie++)
	{
		pozitieX = 234;
		for (coloana = 0; coloana <= 9; coloana++)
		{
			matricePatrate[linie][coloana].w = matricePatrate[linie][coloana].h = 44;
			matricePatrate[linie][coloana].x = pozitieX;
			matricePatrate[linie][coloana].y = pozitieY;
			pozitieX += variatie;
		}
		pozitieY += variatie;
	}
}

void StartJoc(SDL_Renderer *_renderer)
{
	SDL_RenderClear(_renderer);
	IncarcaImagine("./imagini/tablaJoc.jpg", _renderer, 0, 0, 1000, 700);
	IncarcaImagine("./imagini/jucatorUnu.png", _renderer, 481, 573, 44, 44);
	IncarcaImagine("./imagini/jucatorDoi.png", _renderer, 481, 78, 44, 44);
	while (true)
	{
		SDL_RenderPresent(_renderer);
	}
}


int StartMeniu(SDL_Renderer *_renderer)
{
	SDL_Event ev;
	bool mergeProgramul = true;

	while (mergeProgramul)
	{
		while (SDL_PollEvent(&ev))
		{
			IncarcaImagine("./imagini/meniuPrincipal.jpg", _renderer, 0, 0, 1000, 700);

			if (ev.type == SDL_QUIT || (ev.type == SDL_MOUSEBUTTONDOWN && (ev.motion.x > 315 && ev.motion.x < 665 && ev.motion.y > 470 && ev.motion.y < 570)))
			{
				mergeProgramul = false;
				return 0;
			}

			if (!(ev.motion.x > 315 && ev.motion.x < 665 && ev.motion.y > 220 && ev.motion.y < 320))
				IncarcaImagine("./imagini/start.jpg", _renderer, 315, 220, 350, 100);
			else
				IncarcaImagine("./imagini/starthover.jpg", _renderer, 315, 220, 350, 100);

			if (!(ev.motion.x > 315 && ev.motion.x < 665 && ev.motion.y > 345 && ev.motion.y < 445))
				IncarcaImagine("./imagini/help.jpg", _renderer, 315, 345, 350, 100);
			else
				IncarcaImagine("./imagini/helpHover.jpg", _renderer, 315, 345, 350, 100);

			if (!(ev.motion.x > 315 && ev.motion.x < 665 && ev.motion.y > 470 && ev.motion.y < 570))
				IncarcaImagine("./imagini/exit.jpg", _renderer, 315, 470, 350, 100);
			else
				IncarcaImagine("./imagini/exitHover.jpg", _renderer, 315, 470, 350, 100);

			if (ev.type == SDL_MOUSEBUTTONDOWN)
			{
				if (ev.motion.x > 315 && ev.motion.x < 665 && ev.motion.y > 220 && ev.motion.y < 320)
					return 1;

				if (ev.motion.x > 315 && ev.motion.x < 665 && ev.motion.y > 345 && ev.motion.y < 445)
				{
					bool merge = true;
					int nrPagina = 1;
					while (merge)
					{
						while (SDL_PollEvent(&ev))
						{
							SDL_RenderClear(_renderer);

							if (nrPagina == 1) IncarcaImagine("./imagini/instructiuni1.jpg", _renderer, 0, 0, 1000, 700);
							if (nrPagina == 2) IncarcaImagine("./imagini/instructiuni2.jpg", _renderer, 0, 0, 1000, 700);
							if (nrPagina == 3) IncarcaImagine("./imagini/instructiuni3.jpg", _renderer, 0, 0, 1000, 700);
							if (nrPagina == 4) IncarcaImagine("./imagini/instructiuni4.jpg", _renderer, 0, 0, 1000, 700);

							if (nrPagina != 1)
								if (!(ev.motion.x >= 855 && ev.motion.x <= 905 && ev.motion.y >= 45 && ev.motion.y <= 95))
									IncarcaImagine("./imagini/butonStanga.jpg", _renderer, 855, 45, 50, 50);
								else
									IncarcaImagine("./imagini/butonStangaHover.jpg", _renderer, 855, 45, 50, 50);

							if (nrPagina != 4)
								if (!(ev.motion.x >= 915 && ev.motion.x <= 965 && ev.motion.y >= 45 && ev.motion.y <= 95))
									IncarcaImagine("./imagini/butonDreapta.jpg", _renderer, 915, 45, 50, 50);
								else
									IncarcaImagine("./imagini/butonDreaptaHover.jpg", _renderer, 915, 45, 50, 50);

							if (!(ev.motion.x > 30 && ev.motion.x < 105 && ev.motion.y > 45 && ev.motion.y < 95))
								IncarcaImagine("./imagini/back.jpg", _renderer, 30, 45, 175, 50);
							else
								IncarcaImagine("./imagini/backHover.jpg", _renderer, 30, 45, 175, 50);

							if (ev.type == SDL_QUIT)
							{
								mergeProgramul = false;
								merge = false;
								return 0;
							}

							if (ev.type == SDL_MOUSEBUTTONDOWN)
							{
								if (ev.motion.x > 30 && ev.motion.x < 105 && ev.motion.y > 45 && ev.motion.y < 95)
									merge = false;
								if (nrPagina != 1 && ev.motion.x >= 855 && ev.motion.x <= 905 && ev.motion.y >= 45 && ev.motion.y <= 95)
									nrPagina--;
								if (nrPagina != 4 && ev.motion.x >= 915 && ev.motion.x <= 965 && ev.motion.y >= 45 && ev.motion.y <= 95)
									nrPagina++;

							}
						}
						SDL_RenderPresent(_renderer);
					}
				}

			}
		}
		SDL_RenderPresent(_renderer);
	}

	return 0;
}

int main(int argc, char* argv[])
{
	SDL_Init(SDL_INIT_EVERYTHING);
	IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);

	SDL_Window *_window = nullptr;
	SDL_Renderer *_renderer = nullptr;

	_window = SDL_CreateWindow("Quoridor", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1000, 700, NULL);
	_renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	
	int valoare = 0
		;
	valoare = StartMeniu(_renderer);

	if (valoare == 1)
		StartJoc(_renderer);

	SDL_DestroyRenderer(_renderer);
	SDL_DestroyWindow(_window);

	_window = nullptr;
	_renderer = nullptr;

	IMG_Quit();
	SDL_Quit();

	return 0;
}
