#include<SDL.h>
#include<SDL_image.h>
#include<SDL_ttf.h>
#include<iostream>
using namespace std;

#define pozitieStart1X 481
#define pozitieStart1Y 573
#define pozitieStart2X 481
#define pozitieStart2Y 78

unsigned int jucator1X = 481;
unsigned int jucator2X = 481;
unsigned int jucator1Y = 573;
unsigned int jucator2Y = 78;

SDL_Renderer *_renderer;

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
int matriceJoc[17][17];
int matriceAux[17][17];
perete matricePerete[16][8];
pion jucator1, jucator2;

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

bool LiberPereteVertical(int linie, int coloana)
{
	if (matricePerete[linie][coloana].fixat == true)
		return false;

	if (linie >= 2 && matricePerete[linie - 2][coloana].fixat == true)
		return false;

	if (linie <= 12 && matricePerete[linie + 2][coloana].fixat == true)
		return false;

	if (matricePerete[linie + 1][coloana].fixat == true)
		return false;

	return true;
}

bool LiberPereteOrizontal(int linie, int coloana)
{
	if (matricePerete[linie][coloana].fixat == true)
		return false;

	if (coloana >= 1 && matricePerete[linie][coloana - 1].fixat == true)
		return false;

	if (coloana <= 6 && matricePerete[linie][coloana + 1].fixat == true)
		return false;

	if (matricePerete[linie - 1][coloana].fixat == true)
		return false;

	return true;
}

bool EstePereteOrizontalLiber(int x, int y)
{
	bool drumJucator1, drumJucator2;
	matriceAuxiliara(x, y, 2);
	drumJucator1 = ExistaDrrum(1);
	matriceAuxiliara(x, y, 2);
	drumJucator2 = ExistaDrum(2);

	if (drumJucator1 == true && drumJucator2 == true)
		return true;
	return false;
}

bool ExistaDrum(int jucator)
{
	int direcLinii[] = { 1, -1, 0, 0 };
	int direcColoana[] = { 0, 0, 1, -1 };
	int urmLinie[] = { 2, -2, 0, 0 };
	int urmColoana[] = { 0, 0, 2, -2 };
	int i;
	int lungime = 1;
	bool drum = false;
	pozitie coada[100];
	if (jucator == 1)
	{
		coada[1].linie = jucator1.linie;
		coada[1].coloana = jucator1.coloana;
	}
	if (jucator == 2)
	{
		{
			coada[1].linie = jucator2.linie;
			coada[1].coloana = jucator2.coloana;
		}
	}
	while (lungime != 0)
	{
		if (coada[lungime].linie == 0 && jucator == 1)
			return true;
		if (coada[lungime].linie == 16 && jucator == 2)
			return true;
		matriceAuxiliara[coada[lungime].linie][coada[lungime].coloana] = 3;
		drum = false;
		for (i = 0; i <= 3; i++)
			if (matriceAuxiliara[coada[lungime].linie + direcLinii[i]][coada[lungime].coloana + direcColoana[i]] != -1 && matriceAuxiliara[coada[lungime].linie + urmLinie[i]][coada[lungime].coloana + urmColoana[i]] != 3 && VerificaDepasireMatrice(coada[lungime].linie + urmLinie[i], coada[lungime].coloana + urmColoana[i]) == true)
			{
				lungime++;
				coada[lungime].linie = coada[lungime - 1].linie + urmLinie[i];
				coada[lungime].coloana = coada[lungime - 1].coloana + urmColoana[i];
				i = 4;
				drum = true;
			}
		if (drum == false)
			lungime--;
	}
	return false;
}

bool EstePereteVerticalLiber(int x, int y)
{
	bool drumJucator1, drumJucator2;

	int k, t;
	for (k = 0; k <= 16; k++)
		for (t = 0; t <= 16; t++)
			matriceAuxiliara[k][t] = matriceJoc[k][t];

		matriceAuxiliara[x][y * 2 + 1] = -1;
		matriceAuxiliara[x + 1][y * 2 + 1] = -1;
		matriceAuxiliara[x + 2][y * 2 + 1] = -1;

	drumJucator1 = ExistaDrum(1);

	matriceAuxiliara[x][y * 2 + 1] = -1;
	matriceAuxiliara[x + 1][y * 2 + 1] = -1;
	matriceAuxiliara[x + 2][y * 2 + 1] = -1;

	drumJucator2 = ExistaDrum(2);

	if (drumJucator1 == true && drumJucator2 == true)
		return true;
	return false;
}
void VerificaLocPerete(SDL_Event ev, bool &gata)
{
	int i, j;
	for (i = 0; i <= 15; i++)
	{
		for (j = 0; j <= 7; j++)
		{
			if (i % 2 == 0)
			{
				if (MousePereteVertical(ev, matricePerete[i][j].x, matricePerete[i][j].y) && LiberPereteVertical(i, j) && VerificaPereti() && EstePereteVerticalLiber(i, j) == 1)
				{
					if (ev.type == SDL_MOUSEBUTTONDOWN && matricePerete[i][j].fixat == 0)
					{
						if (incepeJucator1)
							peretiJucator1--;
						else
							peretiJucator2--;

						matriceJoc[i][j * 2 + 1] = -1;
						matriceJoc[i + 1][j * 2 + 1] = -1;
						matriceJoc[i + 2][j * 2 + 1] = -1;

						matricePerete[i][j].fixat = 1;
						gata = true;
					}
				}
			}
			else
			{
				if (MousePereteOrizontal(ev, matricePerete[i][j].x, matricePerete[i][j].y) && LiberPereteOrizontal(i, j) && VerificaPereti() && EstePereteOrizontalLiber(i, j) == 1)
				{
					if (ev.type == SDL_MOUSEBUTTONDOWN && matricePerete[i][j].fixat == 0)
					{
						if (incepeJucator1)
							peretiJucator1--;
						else
							peretiJucator2--;

						matriceJoc[i][j * 2] = -1;
						matriceJoc[i][j * 2 + 1] = -1;
						matriceJoc[i][j * 2 + 2] = -1;

						matricePerete[i][j].fixat = 1;
						gata = true;
					}
				}
			}
		}
	}
}
int VerificaPozitieAdversar(int xUrm, int yUrm, int xAdversar, int yAdversar)
{
	if (xUrm == xAdversar && yUrm == yAdversar)
		return 0;
	return 1;
}

bool VerificaDepasireMatrice(int x, int y)
{
	if (x < 0)
		return false;
	if (x >16)
		return false;
	if (y < 0)		return false;
	if (y>16)
		return false;
	return true;
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


void IncarcaImagine(const char *filePath, int x, int y, int w, int h)
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

int MutaJucator()
{
	int colorat = 0;
	int pereteColorat = 0;
	bool gata = false;
	SDL_Event ev;

	if (incepeJucator1 == true)
	{
		while (!gata)
		{
			while (SDL_PollEvent(&ev))
			{
				if (ev.type == SDL_QUIT)
				{
					gata = true;
					return 0;
				}

				pereteColorat = 0;

				if (ev.type == SDL_MOUSEBUTTONDOWN && ev.motion.x >= jucator1X && ev.motion.x <= jucator1X + 44 && ev.motion.y >= jucator1Y && ev.motion.y <= jucator1Y + 44)
				{
					if (colorat == 0)
					{
						RedaImagineJoc();
						ColoreazaMutariPosibile(jucator1X, jucator1Y, jucator1, 1);
						colorat = 1;
					}
					else
					{
						RedaImagineJoc();
						colorat = 0;
					}
				}

				if (colorat == 0)
				{
					ColoreazaPereti(ev, pereteColorat);
					VerificaLocPerete(ev, gata);
					if (gata)
					{
						incepeJucator1 = false;
						return 1;
					}
				}

				if (pereteColorat == 0 && colorat == 0)
					RedaImagineJoc();

				if (colorat == 1)
				{

					if (VerificaPozitieAdversar(jucator1X - 61, jucator1Y, jucator2X, jucator2Y) == 0 && jucator2X - 2 * 61 >= pozitieStart1X - (4 * 61) && matriceJoc[jucator1.linie][jucator1.coloana - 1] != -1)
					{
						if (matriceJoc[jucator1.linie][jucator1.coloana - 3] == -1)
						{
							if (ev.type == SDL_MOUSEBUTTONDOWN && ev.motion.x > jucator1X - 61 && ev.motion.x < jucator1X - 61 + 44 && ev.motion.y > jucator1Y - 61 && ev.motion.y < jucator1Y + 44 - 61 && matriceJoc[jucator2.linie - 1][jucator2.coloana] != -1)
							{
								jucator1X -= 61;
								jucator1Y -= 61;

								matriceJoc[jucator1.linie][jucator1.coloana] = 0;
								jucator1.coloana -= 2;
								jucator1.linie -= 2;
								matriceJoc[jucator1.linie][jucator1.coloana] = 1;

								incepeJucator1 = false;
								return 1;
							}

							if (ev.type == SDL_MOUSEBUTTONDOWN && ev.motion.x > jucator1X - 61 && ev.motion.x < jucator1X - 61 + 44 && ev.motion.y > jucator1Y + 61 && ev.motion.y < jucator1Y + 44 + 61 && matriceJoc[jucator2.linie + 1][jucator2.coloana] != -1)
							{
								jucator1X -= 61;
								jucator1Y += 61;

								matriceJoc[jucator1.linie][jucator1.coloana] = 0;
								jucator1.coloana -= 2;
								jucator1.linie += 2;
								matriceJoc[jucator1.linie][jucator1.coloana] = 1;

								incepeJucator1 = false;
								return 1;
							}
						}

						if (ev.type == SDL_MOUSEBUTTONDOWN && ev.motion.x > jucator1X - 2 * 61 && ev.motion.x < jucator1X - 2 * 61 + 44 && ev.motion.y > jucator1Y && ev.motion.y < jucator1Y + 44 && matriceJoc[jucator1.linie][jucator1.coloana - 3] != -1)
						{
							jucator1X -= 2 * 61;
							gata = true;

							matriceJoc[jucator1.linie][jucator1.coloana] = 0;
							jucator1.coloana -= 4;
							matriceJoc[jucator1.linie][jucator1.coloana] = 1;
						}
					}
					else
						if (jucator1X - 61 >= pozitieStart1X - (4 * 61) && VerificaPozitieAdversar(jucator1X - 61, jucator1Y, jucator2X, jucator2Y) == 1 && matriceJoc[jucator1.linie][jucator1.coloana - 1] != -1)
							if (ev.type == SDL_MOUSEBUTTONDOWN && ev.motion.x > jucator1X - 61 && ev.motion.x < jucator1X - 61 + 44 && ev.motion.y > jucator1Y && ev.motion.y < jucator1Y + 44)
							{
								jucator1X -= 61;
								gata = true;

								matriceJoc[jucator1.linie][jucator1.coloana] = 0;
								jucator1.coloana -= 2;
								matriceJoc[jucator1.linie][jucator1.coloana] = 1;
							}



					if (VerificaPozitieAdversar(jucator1X + 61, jucator1Y, jucator2X, jucator2Y) == 0 && jucator1X + 2 * 61 <= pozitieStart1X + (4 * 61) && matriceJoc[jucator1.linie][jucator1.coloana + 1] != -1)
					{
						if (matriceJoc[jucator1.linie][jucator1.coloana + 3] == -1)
						{
							if (ev.type == SDL_MOUSEBUTTONDOWN && ev.motion.x > jucator1X + 61 && ev.motion.x < jucator1X + 61 + 44 && ev.motion.y > jucator1Y + 61 && ev.motion.y < jucator1Y + 44 + 61 && matriceJoc[jucator2.linie + 1][jucator2.coloana] != -1)
							{

								jucator1X += 61;
								jucator1Y +=  61;

								matriceJoc[jucator1.linie][jucator1.coloana] = 0;
								jucator1.coloana += 2;
								jucator1.linie += 2;
								matriceJoc[jucator1.linie][jucator1.coloana] = 1;

								incepeJucator1 = false;
								return 1;
							}

							if (ev.type == SDL_MOUSEBUTTONDOWN && ev.motion.x > jucator1X + 61 && ev.motion.x < jucator1X + 61 + 44 && ev.motion.y > jucator1Y - 61 && ev.motion.y < jucator1Y + 44 - 61 && matriceJoc[jucator2.linie - 1][jucator2.coloana] != -1)
							{

								jucator1X += 61;
								jucator1Y -= 61;

								matriceJoc[jucator1.linie][jucator1.coloana] = 0;
								jucator1.coloana += 2;
								jucator1.linie -= 2;
								matriceJoc[jucator1.linie][jucator1.coloana] = 1;

								incepeJucator1 = false;
								return 1;
							}
						}

						if (ev.type == SDL_MOUSEBUTTONDOWN && ev.motion.x > jucator1X + 2 * 61 && ev.motion.x < jucator1X + 2 * 61 + 44 && ev.motion.y > jucator1Y && ev.motion.y < jucator1Y + 44 && matriceJoc[jucator1.linie][jucator1.coloana + 3] != -1)
						{
							jucator1X += 2 * 61;
							gata = true;

							matriceJoc[jucator1.linie][jucator1.coloana] = 0;
							jucator1.coloana += 4;
							matriceJoc[jucator1.linie][jucator1.coloana] = 1;
						}
					}
					else
						if (jucator1X + 61 <= pozitieStart1X + (4 * 61) && VerificaPozitieAdversar(jucator1X + 61, jucator1Y, jucator2X, jucator2Y) == 1 && matriceJoc[jucator1.linie][jucator1.coloana + 1] != -1)
							if (ev.type == SDL_MOUSEBUTTONDOWN && ev.motion.x > jucator1X + 61 && ev.motion.x < jucator1X + 61 + 44 && ev.motion.y > jucator1Y && ev.motion.y < jucator1Y + 44)
							{
								jucator1X += 61;
								gata = true;

								matriceJoc[jucator1.linie][jucator1.coloana] = 0;
								jucator1.coloana += 2;
								matriceJoc[jucator1.linie][jucator1.coloana] = 1;
							}


					if (jucator1Y + 61 <= pozitieStart1Y)
						if (VerificaPozitieAdversar(jucator1X, jucator1Y + 61, jucator2X, jucator2Y) == 0 && jucator1Y + 2 * 61 <= pozitieStart1Y && matriceJoc[jucator1.linie + 1][jucator1.coloana] != -1)
						{
							if (matriceJoc[jucator1.linie + 3][jucator1.coloana] == -1)
							{
								if (ev.type == SDL_MOUSEBUTTONDOWN && ev.motion.x > jucator1X - 61 && ev.motion.x < jucator1X - 61 + 44 && ev.motion.y > jucator1Y + 61 && ev.motion.y < jucator1Y + 44 + 61 && matriceJoc[jucator2.linie][jucator2.coloana - 1] != -1)
								{
									jucator1X -= 61;
									jucator1Y +=  61;

									matriceJoc[jucator1.linie][jucator1.coloana] = 0;
									jucator1.coloana -= 2;
									jucator1.linie += 2;
									matriceJoc[jucator1.linie][jucator1.coloana] = 1;

									incepeJucator1 = false;
									return 1;
								}

								if (ev.type == SDL_MOUSEBUTTONDOWN && ev.motion.x > jucator1X + 61 && ev.motion.x < jucator1X + 61 + 44 && ev.motion.y > jucator1Y + 61 && ev.motion.y < jucator1Y + 44 + 61 && matriceJoc[jucator2.linie][jucator2.coloana + 1] != -1)
								{

									jucator1X += 61;
									jucator1Y += 61;

									matriceJoc[jucator1.linie][jucator1.coloana] = 0;
									jucator1.coloana += 2;
									jucator1.linie += 2;
									matriceJoc[jucator1.linie][jucator1.coloana] = 1;

									incepeJucator1 = false;
									return 1;
								}
							}
							if (ev.type == SDL_MOUSEBUTTONDOWN && ev.motion.x > jucator1X && ev.motion.x < jucator1X + 44 && ev.motion.y > jucator1Y + 2 * 61 && ev.motion.y < jucator1Y + 2 * 61 + 44 && matriceJoc[jucator1.linie + 3][jucator1.coloana] != -1)
							{
								jucator1Y += 2 * 61;
								gata = true;

								matriceJoc[jucator1.linie][jucator1.coloana] = 0;
								jucator1.linie += 4;
								matriceJoc[jucator1.linie][jucator1.coloana] = 1;
							}
						}
						else
							if (VerificaPozitieAdversar(jucator1X, jucator1Y + 61, jucator2X, jucator2Y) == 1 && matriceJoc[jucator1.linie + 1][jucator1.coloana] != -1)
								if (ev.type == SDL_MOUSEBUTTONDOWN && ev.motion.x > jucator1X && ev.motion.x < jucator1X + 44 && ev.motion.y > jucator1Y + 61 && ev.motion.y < jucator1Y + 61 + 44)
								{
									jucator1Y += 61;
									gata = true;

									matriceJoc[jucator1.linie][jucator1.coloana] = 0;
									jucator1.linie += 2;
									matriceJoc[jucator1.linie][jucator1.coloana] = 1;
								}

					if (jucator1Y - 61 >= pozitieStart2Y)
					{
						if (VerificaPozitieAdversar(jucator1X, jucator1Y - 61, jucator2X, jucator2Y) == 0 && jucator2.linie == 0 && matriceJoc[jucator1.linie - 1][jucator1.coloana] != -1)
						{
							if (ev.type == SDL_MOUSEBUTTONDOWN && ev.motion.x > jucator1X - 61 && ev.motion.x < jucator1X - 61 + 44 && ev.motion.y > jucator1Y - 61 && ev.motion.y < jucator1Y + 44 - 61 && matriceJoc[jucator2.linie][jucator2.coloana - 1] != -1)
							{

								jucator1X -= 61;
								jucator1Y -=  61;

								matriceJoc[jucator1.linie][jucator1.coloana] = 0;
								jucator1.coloana -= 2;
								jucator1.linie -= 2;
								matriceJoc[jucator1.linie][jucator1.coloana] = 1;

								incepeJucator1 = false;
								return 1;
							}

							if (ev.type == SDL_MOUSEBUTTONDOWN && ev.motion.x > jucator1X + 61 && ev.motion.x < jucator1X + 61 + 44 && ev.motion.y > jucator1Y - 61 && ev.motion.y < jucator1Y + 44 - 61 && matriceJoc[jucator2.linie][jucator2.coloana + 1] != -1)
							{
								jucator1X += 61;
								jucator1Y -= 61;

								matriceJoc[jucator1.linie][jucator1.coloana] = 0;
								jucator1.coloana += 2;
								jucator1.linie -= 2;
								matriceJoc[jucator1.linie][jucator1.coloana] = 1;

								incepeJucator1 = false;
								return 1;
							}
						}

						if (VerificaPozitieAdversar(jucator1X, jucator1Y - 61, jucator2X, jucator2Y) == 0 && jucator1Y - 2 * 61 >= pozitieStart2Y && matriceJoc[jucator1.linie - 1][jucator1.coloana] != -1)
						{
							if (matriceJoc[jucator1.linie - 3][jucator1.coloana] == -1)
							{
								if (ev.type == SDL_MOUSEBUTTONDOWN && ev.motion.x > jucator1X - 61 && ev.motion.x < jucator1X - 61 + 44 && ev.motion.y > jucator1Y - 61 && ev.motion.y < jucator1Y + 44 - 61 && matriceJoc[jucator2.linie][jucator2.coloana - 1] != -1)
								{

									jucator1X -= 61;
									jucator1Y -= 61;

									matriceJoc[jucator1.linie][jucator1.coloana] = 0;
									jucator1.coloana -= 2;
									jucator1.linie -= 2;
									matriceJoc[jucator1.linie][jucator1.coloana] = 1;

									incepeJucator1 = false;
									return 1;
								}

								if (ev.type == SDL_MOUSEBUTTONDOWN && ev.motion.x > jucator1X + 61 && ev.motion.x < jucator1X + 61 + 44 && ev.motion.y > jucator1Y - 61 && ev.motion.y < jucator1Y + 44 - 61 && matriceJoc[jucator2.linie][jucator2.coloana + 1] != -1)
								{

									jucator1X += 61;
									jucator1Y -= 61;

									matriceJoc[jucator1.linie][jucator1.coloana] = 0;
									jucator1.coloana += 2;
									jucator1.linie -= 2;
									matriceJoc[jucator1.linie][jucator1.coloana] = 1;

									incepeJucator1 = false;
									return 1;
								}
							}

							if (ev.type == SDL_MOUSEBUTTONDOWN && ev.motion.x > jucator1X && ev.motion.x < jucator1X + 44 && ev.motion.y > jucator1Y - 2 * 61 && ev.motion.y < jucator1Y - 2 * 61 + 44 && matriceJoc[jucator1.linie - 3][jucator1.coloana] != -1) {
								jucator1Y -= 2 * 61;
								gata = true;

								matriceJoc[jucator1.linie][jucator1.coloana] = 0;
								jucator1.linie -= 4;
								matriceJoc[jucator1.linie][jucator1.coloana] = 1;
							}
						}
						else
							if (VerificaPozitieAdversar(jucator1X, jucator1Y - 61, jucator2X, jucator2Y) == 1 && matriceJoc[jucator1.linie - 1][jucator1.coloana] != -1)
								if (ev.type == SDL_MOUSEBUTTONDOWN && ev.motion.x > jucator1X && ev.motion.x < jucator1X + 44 && ev.motion.y > jucator1Y - 61 && ev.motion.y < jucator1Y - 61 + 44)
								{
									jucator1Y -= 61;
									gata = true;

									matriceJoc[jucator1.linie][jucator1.coloana] = 0;
									jucator1.linie -= 2;
									matriceJoc[jucator1.linie][jucator1.coloana] = 1;
								}
					}
				}
			}
		}
		incepeJucator1 = false;
	}

	else
	{
		while (!gata)
		{
			while (SDL_PollEvent(&ev))
			{
				if (ev.type == SDL_QUIT)
				{
					gata = true;
					return 0;
				}

				pereteColorat = 0;

				if (ev.type == SDL_MOUSEBUTTONDOWN && ev.motion.x >= jucator2X && ev.motion.x <= jucator2X + 44 && ev.motion.y >= jucator2Y && ev.motion.y <= jucator2Y + 44)
				{
					if (colorat == 0)
					{
						ColoreazaMutariPosibile(jucator2X, jucator2Y, jucator2, 2);
						colorat = 1;
					}
					else
					{
						RedaImagineJoc();
						colorat = 0;
					}
				}

				if (colorat == 0)
				{
					ColoreazaPereti(ev, pereteColorat);
					VerificaLocPerete(ev, gata);

					if (gata)
					{
						incepeJucator1 = true;
						return 1;
					}
				}

				if (pereteColorat == 0 && colorat == 0)
					RedaImagineJoc();

				if (colorat == 1)
				{
					if (VerificaPozitieAdversar(jucator2X - 61, jucator2Y, jucator1X, jucator1Y) == 0 && jucator2X - 2 * 61 >= pozitieStart1X - (4 * 61) && matriceJoc[jucator2.linie][jucator2.coloana - 1] != -1)
					{
						if (matriceJoc[jucator2.linie][jucator2.coloana - 3] == -1)
						{
							if (ev.type == SDL_MOUSEBUTTONDOWN && ev.motion.x > jucator2X - 61 && ev.motion.x < jucator2X - 61 + 44 && ev.motion.y > jucator2Y - 61 && ev.motion.y < jucator2Y + 44 - 61 && matriceJoc[jucator1.linie - 1][jucator1.coloana] != -1)
							{
								jucator2X -= 61;
								jucator2Y -= 61;

								matriceJoc[jucator2.linie][jucator2.coloana] = 0;
								jucator2.coloana -= 2;
								jucator2.linie -= 2;
								matriceJoc[jucator2.linie][jucator2.coloana] = 2;

								incepeJucator1 = true;
								return 1;
							}

							if (ev.type == SDL_MOUSEBUTTONDOWN && ev.motion.x > jucator2X - 61 && ev.motion.x < jucator2X - 61 + 44 && ev.motion.y > jucator2Y + 61 && ev.motion.y < jucator2Y + 44 + 61 && matriceJoc[jucator1.linie + 1][jucator1.coloana] != -1)
							{
								jucator2X -= 61;
								jucator2Y += 61;

								matriceJoc[jucator2.linie][jucator2.coloana] = 0;
								jucator2.coloana -= 2;
								jucator2.linie += 2;
								matriceJoc[jucator2.linie][jucator2.coloana] = 2;

								incepeJucator1 = true;
								return 1;
							}
						}

						if (ev.type == SDL_MOUSEBUTTONDOWN && ev.motion.x > jucator2X - 2 * 61 && ev.motion.x < jucator2X - 2 * 61 + 44 && ev.motion.y > jucator2Y && ev.motion.y < jucator2Y + 44 && matriceJoc[jucator2.linie][jucator2.coloana - 3] != -1)
						{
							jucator2X -= 2 * 61;
							gata = true;

							matriceJoc[jucator2.linie][jucator2.coloana] = 0;
							jucator2.coloana -= 4;
							matriceJoc[jucator2.linie][jucator2.coloana] = 2;
						}
					}
					else
						if (jucator2X - 61 >= pozitieStart1X - (4 * 61) && VerificaPozitieAdversar(jucator2X - 61, jucator2Y, jucator1X, jucator1Y) == 1 && matriceJoc[jucator2.linie][jucator2.coloana - 1] != -1)
							if (ev.type == SDL_MOUSEBUTTONDOWN && ev.motion.x > jucator2X - 61 && ev.motion.x < jucator2X - 61 + 44 && ev.motion.y > jucator2Y && ev.motion.y < jucator2Y + 44)
							{
								jucator2X -= 61;
								gata = true;

								matriceJoc[jucator2.linie][jucator2.coloana] = 0;
								jucator2.coloana -= 2;
								matriceJoc[jucator2.linie][jucator2.coloana] = 2;
							}



					if (VerificaPozitieAdversar(jucator2X + 61, jucator2Y, jucator1X, jucator1Y) == 0 && jucator2X + 2 * 61 <= pozitieStart1X + (4 * 61) && matriceJoc[jucator2.linie][jucator2.coloana + 1] != -1)
					{
						if (matriceJoc[jucator2.linie][jucator2.coloana + 3] == -1)
						{
							if (ev.type == SDL_MOUSEBUTTONDOWN && ev.motion.x > jucator2X + 61 && ev.motion.x < jucator2X + 61 + 44 && ev.motion.y > jucator2Y + 61 && ev.motion.y < jucator2Y + 44 + 61 && matriceJoc[jucator1.linie + 1][jucator1.coloana] != -1)
							{
								jucator2X += 61;
								jucator2Y += 61;

								matriceJoc[jucator2.linie][jucator2.coloana] = 0;
								jucator2.coloana += 2;
								jucator2.linie += 2;
								matriceJoc[jucator2.linie][jucator2.coloana] = 2;

								incepeJucator1 = true;
								return 1;
							}

							if (ev.type == SDL_MOUSEBUTTONDOWN && ev.motion.x > jucator2X + 61 && ev.motion.x < jucator2X + 61 + 44 && ev.motion.y > jucator2Y - 61 && ev.motion.y < jucator2Y + 44 - 61 && matriceJoc[jucator1.linie - 1][jucator1.coloana] != -1)
							{

								jucator2X += 61;
								jucator2Y -= 61;

								matriceJoc[jucator2.linie][jucator2.coloana] = 0;
								jucator2.coloana += 2;
								jucator2.linie -= 2;
								matriceJoc[jucator2.linie][jucator2.coloana] = 2;

								incepeJucator1 = true;
								return 1;
							}
						}

						if (ev.type == SDL_MOUSEBUTTONDOWN && ev.motion.x > jucator2X + 2 * 61 && ev.motion.x < jucator2X + 2 * 61 + 44 && ev.motion.y > jucator2Y && ev.motion.y < jucator2Y + 44 && matriceJoc[jucator2.linie][jucator2.coloana + 3] != -1)
						{
							jucator2X += 2 * 61;
							gata = true;

							matriceJoc[jucator2.linie][jucator2.coloana] = 0;
							jucator2.coloana += 4;
							matriceJoc[jucator2.linie][jucator2.coloana] = 2;
						}
					}
					else
						if (jucator2X + 61 <= pozitieStart1X + (4 * 61) && VerificaPozitieAdversar(jucator2X - 61, jucator2Y, jucator1X, jucator1Y) == 1 && matriceJoc[jucator2.linie][jucator2.coloana + 1] != -1)
							if (ev.type == SDL_MOUSEBUTTONDOWN && ev.motion.x > jucator2X + 61 && ev.motion.x < jucator2X + 61 + 44 && ev.motion.y > jucator2Y && ev.motion.y < jucator2Y + 44)
							{
								jucator2X += 61;
								gata = true;

								matriceJoc[jucator2.linie][jucator2.coloana] = 0;
								jucator2.coloana += 2;
								matriceJoc[jucator2.linie][jucator2.coloana] = 2;
							}



					if (jucator2Y + 61 <= pozitieStart1Y)
					{
						if (VerificaPozitieAdversar(jucator2X, jucator2Y + 61, jucator1X, jucator1Y) == 0 && jucator1.linie == 16 && matriceJoc[jucator2.linie + 1][jucator2.coloana] != -1)
						{
							if (ev.type == SDL_MOUSEBUTTONDOWN && ev.motion.x > jucator2X - 61 && ev.motion.x < jucator2X - 61 + 44 && ev.motion.y > jucator2Y + 61 && ev.motion.y < jucator2Y + 44 + 61 && matriceJoc[jucator1.linie][jucator1.coloana - 1] != -1)
							{
								jucator2X -= 61;
								jucator2Y += 61;

								matriceJoc[jucator2.linie][jucator2.coloana] = 0;
								jucator2.coloana -= 2;
								jucator2.linie += 2;
								matriceJoc[jucator2.linie][jucator2.coloana] = 2;

								incepeJucator1 = true;
								return 1;
							}

							if (ev.type == SDL_MOUSEBUTTONDOWN && ev.motion.x > jucator2X + 61 && ev.motion.x < jucator2X + 61 + 44 && ev.motion.y > jucator2Y + 61 && ev.motion.y < jucator2Y + 44 + 61 && matriceJoc[jucator1.linie][jucator1.coloana + 1] != -1)
							{

								jucator2X += 61;
								jucator2Y += 61;

								matriceJoc[jucator2.linie][jucator2.coloana] = 0;
								jucator2.coloana += 2;
								jucator2.linie += 2;
								matriceJoc[jucator2.linie][jucator2.coloana] = 2;

								incepeJucator1 = true;
								return 1;
							}
						}

						if (VerificaPozitieAdversar(jucator2X, jucator2Y + 61, jucator1X, jucator1Y) == 0 && jucator2Y + 2 * 61 <= pozitieStart1Y && matriceJoc[jucator2.linie + 1][jucator2.coloana] != -1)
						{
							if (matriceJoc[jucator2.linie + 3][jucator2.coloana] == -1)
							{
								if (ev.type == SDL_MOUSEBUTTONDOWN && ev.motion.x > jucator2X - 61 && ev.motion.x < jucator2X - 61 + 44 && ev.motion.y > jucator2Y + 61 && ev.motion.y < jucator2Y + 44 + 61 && matriceJoc[jucator1.linie][jucator1.coloana - 1] != -1)
								{
									jucator2X -= 61;
									jucator2Y += 61;

									matriceJoc[jucator2.linie][jucator2.coloana] = 0;
									jucator2.coloana -= 2;
									jucator2.linie += 2;
									matriceJoc[jucator2.linie][jucator2.coloana] = 2;

									incepeJucator1 = true;
									return 1;
								}

								if (ev.type == SDL_MOUSEBUTTONDOWN && ev.motion.x > jucator2X + 61 && ev.motion.x < jucator2X + 61 + 44 && ev.motion.y > jucator2Y + 61 && ev.motion.y < jucator2Y + 44 + 61 && matriceJoc[jucator1.linie][jucator1.coloana + 1] != -1)
								{
									jucator2X += 61;
									jucator2Y += 61;

									matriceJoc[jucator2.linie][jucator2.coloana] = 0;
									jucator2.coloana += 2;
									jucator2.linie += 2;
									matriceJoc[jucator2.linie][jucator2.coloana] = 2;

									incepeJucator1 = true;
									return 1;
								}
							}

							if (ev.type == SDL_MOUSEBUTTONDOWN && ev.motion.x > jucator2X && ev.motion.x < jucator2X + 44 && ev.motion.y > jucator2Y + 2 * 61 && ev.motion.y < jucator2Y + 2 * 61 + 44 && matriceJoc[jucator2.linie + 3][jucator2.coloana] != -1)
							{
								jucator2Y += 2 * 61;
								gata = true;

								matriceJoc[jucator2.linie][jucator2.coloana] = 0;
								jucator2.linie += 4;
								matriceJoc[jucator2.linie][jucator2.coloana] = 2;
							}
						}
						else
							if (VerificaPozitieAdversar(jucator2X, jucator2Y + 61, jucator1X, jucator1Y) == 1 && matriceJoc[jucator2.linie + 1][jucator2.coloana] != -1)
								if (ev.type == SDL_MOUSEBUTTONDOWN && ev.motion.x > jucator2X && ev.motion.x < jucator2X + 44 && ev.motion.y > jucator2Y + 61 && ev.motion.y < jucator2Y + 61 + 44)
								{
									jucator2Y += 61;
									gata = true;

									matriceJoc[jucator2.linie][jucator2.coloana] = 0;
									jucator2.linie += 2;
									matriceJoc[jucator2.linie][jucator2.coloana] = 2;
								}
					}


					if (jucator2Y - 61 >= pozitieStart2Y)
						if (VerificaPozitieAdversar(jucator2X, jucator2Y - 61, jucator1X, jucator1Y) == 0 && jucator2Y - 2 * 61 >= pozitieStart2Y && matriceJoc[jucator2.linie - 1][jucator2.coloana] != -1)
						{
							if (matriceJoc[jucator2.linie - 3][jucator2.coloana] == -1)
							{
								if (ev.type == SDL_MOUSEBUTTONDOWN && ev.motion.x > jucator2X - 61 && ev.motion.x < jucator2X - 61 + 44 && ev.motion.y > jucator2Y - 61 && ev.motion.y < jucator2Y + 44 - 61 && matriceJoc[jucator1.linie][jucator1.coloana - 1] != -1)
								{
									jucator2X -= 61;
									jucator2Y -= 61;

									matriceJoc[jucator2.linie][jucator2.coloana] = 0;
									jucator2.coloana -= 2;
									jucator2.linie -= 2;
									matriceJoc[jucator2.linie][jucator2.coloana] = 2;

									incepeJucator1 = true;
									return 1;
								}

								if (ev.type == SDL_MOUSEBUTTONDOWN && ev.motion.x > jucator2X + 61 && ev.motion.x < jucator2X + 61 + 44 && ev.motion.y > jucator2Y - 61 && ev.motion.y < jucator2Y + 44 - 61 && matriceJoc[jucator1.linie][jucator1.coloana + 1] != -1)
								{
									jucator2X += 61;
									jucator2Y -= 61;

									matriceJoc[jucator2.linie][jucator2.coloana] = 0;
									jucator2.coloana += 2;
									jucator2.linie -= 2;
									matriceJoc[jucator2.linie][jucator2.coloana] = 2;

									incepeJucator1 = true;
									return 1;
								}
							}

							if (ev.type == SDL_MOUSEBUTTONDOWN && ev.motion.x > jucator2X && ev.motion.x < jucator2X + 44 && ev.motion.y > jucator2Y - 2 * 61 && ev.motion.y < jucator2Y - 2 * 61 + 44 && matriceJoc[jucator2.linie - 3][jucator2.coloana] != -1)
							{
								jucator2Y -= 2 * 61;
								gata = true;

								matriceJoc[jucator2.linie][jucator2.coloana] = 0;
								jucator2.linie -= 4;
								matriceJoc[jucator2.linie][jucator2.coloana] = 2;
							}
						}
						else
							if (VerificaPozitieAdversar(jucator2X, jucator2Y - 61, jucator1X, jucator1Y) == 1 && matriceJoc[jucator2.linie - 1][jucator2.coloana] != -1)
								if (ev.type == SDL_MOUSEBUTTONDOWN && ev.motion.x > jucator2X && ev.motion.x < jucator2X + 44 && ev.motion.y > jucator2Y - 61 && ev.motion.y < jucator2Y - 61 + 44)
								{
									jucator2Y -= 61;
									gata = true;

									matriceJoc[jucator2.linie][jucator2.coloana] = 0;
									jucator2.linie -= 2;
									matriceJoc[jucator2.linie][jucator2.coloana] = 2;
								}
				}
			}
		}
		incepeJucator1 = true;
	}
}

void StartJoc()
{
	SDL_RenderClear(_renderer);
	IncarcaImagine("./imagini/tablaJoc.jpg", 0, 0, 1000, 700);
	IncarcaImagine("./imagini/jucatorUnu.png", 481, 573, 44, 44);
	IncarcaImagine("./imagini/jucatorDoi.png", 481, 78, 44, 44);
	while (true)
	{
		SDL_RenderPresent(_renderer);
	}
}

int StartMeniu()
{
	SDL_Event ev;
	bool mergeProgramul = true;

	while (mergeProgramul)
	{
		while (SDL_PollEvent(&ev))
		{			IncarcaImagine("./imagini/meniuPrincipal.jpg", 0, 0, 1000, 700);

			if (ev.type == SDL_QUIT || (ev.type == SDL_MOUSEBUTTONDOWN && (ev.motion.x > 315 && ev.motion.x < 665 && ev.motion.y > 470 && ev.motion.y < 570)))
			{
				mergeProgramul = false;
				return 0;
			}

			if (!(ev.motion.x > 315 && ev.motion.x < 665 && ev.motion.y > 220 && ev.motion.y < 320))
				IncarcaImagine("./imagini/start.jpg", 315, 220, 350, 100);
			else
				IncarcaImagine("./imagini/starthover.jpg", 315, 220, 350, 100);

			if (!(ev.motion.x > 315 && ev.motion.x < 665 && ev.motion.y > 345 && ev.motion.y < 445))
				IncarcaImagine("./imagini/help.jpg", 315, 345, 350, 100);
			else
				IncarcaImagine("./imagini/helpHover.jpg", 315, 345, 350, 100);

			if (!(ev.motion.x > 315 && ev.motion.x < 665 && ev.motion.y > 470 && ev.motion.y < 570))
				IncarcaImagine("./imagini/exit.jpg", 315, 470, 350, 100);
			else
				IncarcaImagine("./imagini/exitHover.jpg", 315, 470, 350, 100);

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

							if (nrPagina == 1) IncarcaImagine("./imagini/instructiuni1.jpg", 0, 0, 1000, 700);
							if (nrPagina == 2) IncarcaImagine("./imagini/instructiuni2.jpg", 0, 0, 1000, 700);
							if (nrPagina == 3) IncarcaImagine("./imagini/instructiuni3.jpg", 0, 0, 1000, 700);
							if (nrPagina == 4) IncarcaImagine("./imagini/instructiuni4.jpg", 0, 0, 1000, 700);

							if (nrPagina != 1)
								if (!(ev.motion.x >= 855 && ev.motion.x <= 905 && ev.motion.y >= 45 && ev.motion.y <= 95))
									IncarcaImagine("./imagini/butonStanga.jpg", 855, 45, 50, 50);
								else
									IncarcaImagine("./imagini/butonStangaHover.jpg", 855, 45, 50, 50);

							if (nrPagina != 4)
								if (!(ev.motion.x >= 915 && ev.motion.x <= 965 && ev.motion.y >= 45 && ev.motion.y <= 95))
									IncarcaImagine("./imagini/butonDreapta.jpg", 915, 45, 50, 50);
								else
									IncarcaImagine("./imagini/butonDreaptaHover.jpg", 915, 45, 50, 50);

							if (!(ev.motion.x > 30 && ev.motion.x < 105 && ev.motion.y > 45 && ev.motion.y < 95))
								IncarcaImagine("./imagini/back.jpg", 30, 45, 175, 50);
							else
								IncarcaImagine("./imagini/backHover.jpg", 30, 45, 175, 50);

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
	TTF_Init();
	
	SDL_Window *_window = nullptr;
	_renderer = nullptr;

	_window = SDL_CreateWindow("Quoridor", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1000, 700, NULL);
	_renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	
	int valoare = 0;
	valoare = StartMeniu();

	if (valoare == 1)
		StartJoc();

	SDL_DestroyRenderer(_renderer);
	SDL_DestroyWindow(_window);

	_window = nullptr;
	_renderer = nullptr;

	TTF_Quit();
	IMG_Quit();
	SDL_Quit();

	return 0;
}
