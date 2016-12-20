#include <allegro5\allegro.h>
#include <allegro5\allegro_primitives.h>
#include <allegro5\allegro_font.h>
#include <allegro5\allegro_ttf.h>
#include <allegro5\allegro_image.h>
#include "Header.h"

//GLOBALNE=============================
const int WIDTH = 500;
const int HEIGHT = 700;
const int NUM_PRZESZKODY = 10;							//max ilosc przeszkod na ekranie
const int NUM_MONETY = 1;								//max ilosc monet na ekranie
const int NUM_ZYCIA = 1;								//max ilosc zyc na ekranie
enum KEYS { SPACE, ONE, TWO, LEFT, RIGHT };
bool keys[5] = { false, false, false, false, false };
enum STATUS { MENU, GRA, KONIEC };

//obiekty
Spadochroniarz ludek;
Przeszkoda przeszkody[NUM_PRZESZKODY];
Monety moneta[NUM_MONETY];
Zycia zycie[NUM_ZYCIA];

//prototypy
void InitLudek(Spadochroniarz &ludek, ALLEGRO_BITMAP *image, ALLEGRO_BITMAP *image1, ALLEGRO_BITMAP *image2);
void DrawLudekLeft(Spadochroniarz &ludek);
void DrawLudekRight(Spadochroniarz &ludek);
void DrawLudekProsto(Spadochroniarz &ludek);
void MoveLudekLeft(Spadochroniarz &ludek);
void MoveLudekRight(Spadochroniarz &ludek);

void InitPrzeszkoda(Przeszkoda przeszkody[], int size, ALLEGRO_BITMAP *image);
void DrawPrzeszkoda(Przeszkoda przeszkody[], int size);
void StartPrzeszkoda(Przeszkoda przeszkody[], int size);
void UpdatePrzeszkoda(Przeszkoda przeszkody[], int size, Spadochroniarz &ludek);
void CollidePrzeszkoda(Przeszkoda przeszkody[], int sizeB, Spadochroniarz &ludek);

void InitMonety(Monety moneta[], int size, ALLEGRO_BITMAP *image);
void DrawMonety(Monety moneta[], int size);
void StartMonety(Monety moneta[], int size);
void UpdateMonety(Monety moneta[], int size, Spadochroniarz &ludek);
void CollideMonety(Monety moneta[], int sizeB, Spadochroniarz &ludek);

void InitZycia(Zycia zycie[], int size, ALLEGRO_BITMAP *image);
void DrawZycia(Zycia zycie[], int size);
void StartZycia(Zycia zycie[], int size);
void UpdateZycia(Zycia zycie[], int size, Spadochroniarz &ludek);
void CollideZycia(Zycia zycie[], int sizeB, Spadochroniarz &ludek);

void ChangeStatus(int &status, int nowyStatus);

int main(void)
{
	//prymitywy 
	bool done = false;
	bool redraw = true;
	const int FPS = 60;
	int status = -1;

	//zmienne Allegro
	ALLEGRO_DISPLAY *display = NULL;
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;
	ALLEGRO_TIMER *timer = NULL;
	ALLEGRO_FONT *font18 = NULL;
	ALLEGRO_FONT *font32 = NULL;
	ALLEGRO_FONT *font48 = NULL;
	ALLEGRO_BITMAP *ludekImageLeft;			//image
	ALLEGRO_BITMAP *ludekImageRight;		//image1
	ALLEGRO_BITMAP *ludekImageProsto;		//image2
	ALLEGRO_BITMAP *przeszkodyImage;		
	ALLEGRO_BITMAP *monetaImage;
	ALLEGRO_BITMAP *zycieImage;

	//initialization functions
	if (!al_init())										//dzialanie allegro
		return -1;

	display = al_create_display(WIDTH, HEIGHT);			

	if (!display)										//dzialanie ekranu
		return -1;

	al_init_primitives_addon();
	al_install_keyboard();
	al_init_font_addon();
	al_init_ttf_addon();
	al_init_image_addon();

	event_queue = al_create_event_queue();
	timer = al_create_timer(1.0 / FPS);
	
	ludekImageLeft = al_load_bitmap("spadochroniarzLEWO.png");
	al_convert_mask_to_alpha(ludekImageLeft, al_map_rgb(255, 255, 255));

	ludekImageRight = al_load_bitmap("spadochroniarzPRAWO.png");
	al_convert_mask_to_alpha(ludekImageRight, al_map_rgb(255, 255, 255));

	ludekImageProsto = al_load_bitmap("spadochroniarzPROSTO.png");
	al_convert_mask_to_alpha(ludekImageProsto, al_map_rgb(255, 255, 255));

	przeszkodyImage = al_load_bitmap("deska.png");
	al_convert_mask_to_alpha(przeszkodyImage, al_map_rgb(255, 255, 255));

	monetaImage = al_load_bitmap("5zlotych.png");
	al_convert_mask_to_alpha(monetaImage, al_map_rgb(255, 255, 255));

	zycieImage = al_load_bitmap("serce.png");
	al_convert_mask_to_alpha(zycieImage, al_map_rgb(255, 255, 255));

	srand(time(NULL));

	ChangeStatus(status, MENU);

	InitLudek(ludek, ludekImageLeft, ludekImageRight, ludekImageProsto);
	InitPrzeszkoda(przeszkody, NUM_PRZESZKODY, przeszkodyImage);
	InitMonety(moneta, NUM_MONETY, monetaImage);
	InitZycia(zycie, NUM_ZYCIA, zycieImage);


	font18 = al_load_font("ARIALNBI.TTF", 18, 0);		//(czcionka, rozmiar, flagi)
	font32 = al_load_font("ARIALNBI.TTF", 32, 0);
	font48 = al_load_font("ARIALNBI.TTF", 48, 0);


	al_register_event_source(event_queue, al_get_keyboard_event_source());
	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	al_register_event_source(event_queue, al_get_display_event_source(display));

	al_start_timer(timer);
	while (!done)
	{
		ALLEGRO_EVENT ev;
		al_wait_for_event(event_queue, &ev);

		if (ev.type == ALLEGRO_EVENT_TIMER)
		{
			redraw = true;

			if (status == MENU)
			{

			}
			else if (status == GRA)
			{
				StartPrzeszkoda(przeszkody, NUM_PRZESZKODY);			//inicjuje przeszkode
				UpdatePrzeszkoda(przeszkody, NUM_PRZESZKODY, ludek);	//ponowne rysowanie po wyjsciu za ekran
				CollidePrzeszkoda(przeszkody, NUM_PRZESZKODY, ludek);

				StartMonety(moneta, NUM_MONETY);						//inicjuje monete
				UpdateMonety(moneta, NUM_MONETY, ludek);				//ponowne rysowanie po wyjsciu za ekran
				CollideMonety(moneta, NUM_MONETY, ludek);

				StartZycia(zycie, NUM_ZYCIA);							//inicjuje dodatkowe ¿ycia
				UpdateZycia(zycie, NUM_ZYCIA, ludek);					//ponowne rysowanie po wyjsciu za ekran
				CollideZycia(zycie, NUM_ZYCIA, ludek);

				if (ludek.lives <= 0)
					ChangeStatus(status, KONIEC);
			}
			else if (status == KONIEC)
			{

			}
		}

		else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)				//zamykanie gry krzyzykiem
		{
			done = true;
		}

		else if (ev.type == ALLEGRO_EVENT_KEY_DOWN)
		{
			switch (ev.keyboard.keycode)
			{
			case ALLEGRO_KEY_ESCAPE:
				if (status == MENU)
					done = true;
				else if (status == KONIEC)
					done = true;
				break;
			case ALLEGRO_KEY_LEFT:
				keys[LEFT] = true;
				break;
			case ALLEGRO_KEY_RIGHT:
				keys[RIGHT] = true;
				break;
			case ALLEGRO_KEY_SPACE:
				keys[SPACE] = true;		
				if (status == KONIEC)
					ChangeStatus(status, GRA);
				break;
			case ALLEGRO_KEY_1:
				keys[1] = true;
				if (status == MENU)
					ChangeStatus(status, GRA);
				break;
			case ALLEGRO_KEY_2:
				keys[1] = true;
				if (status == MENU)
				done = true;
				break;
			}
		}

		else if (ev.type == ALLEGRO_EVENT_KEY_UP)
		{
			switch (ev.keyboard.keycode)
			{
			case ALLEGRO_KEY_ESCAPE:
				done = false;
				break;
			case ALLEGRO_KEY_LEFT:
				keys[LEFT] = false;
				break;
			case ALLEGRO_KEY_RIGHT:
				keys[RIGHT] = false;
				break;
			case ALLEGRO_KEY_SPACE:
				keys[SPACE] = false;
				break;
			case ALLEGRO_KEY_1:
				keys[1] = false;
				break;
			case ALLEGRO_KEY_2:
				keys[2] = false;
				break;
			}
		}
		
		if (redraw && al_is_event_queue_empty(event_queue))
		{
			redraw = false;
			
			if (status == MENU)
			{
				al_draw_textf(font48, al_map_rgb(0, 0, 0), WIDTH / 2, HEIGHT / 2 - 300, ALLEGRO_ALIGN_CENTRE, "SPADOCHRONIARZ");
				al_draw_textf(font18, al_map_rgb(0, 0, 0), WIDTH / 2, HEIGHT / 2 - 200, ALLEGRO_ALIGN_CENTRE, "Nacisnij 1 lub 2");
				al_draw_textf(font32, al_map_rgb(0, 0, 0), WIDTH / 2, HEIGHT / 2, ALLEGRO_ALIGN_CENTRE, "1. Nowa gra");
				al_draw_textf(font32, al_map_rgb(0, 0, 0), WIDTH / 2, HEIGHT / 2 + 100, ALLEGRO_ALIGN_CENTRE, "2. Wyjscie z gry");
			}
			else if (status == GRA)
			{
				if (keys[LEFT])
					MoveLudekLeft(ludek);

				if (keys[RIGHT])
					MoveLudekRight(ludek);

				if (!keys[LEFT] && !keys[RIGHT])
					DrawLudekProsto(ludek);

				DrawPrzeszkoda(przeszkody, NUM_PRZESZKODY);
				DrawMonety(moneta, NUM_MONETY);
				DrawZycia(zycie, NUM_ZYCIA);

				al_draw_textf(font18, al_map_rgb(0, 0, 0), WIDTH / 2 - 245, 5, 0, "Punkty: %i", ludek.score);
				al_draw_textf(font18, al_map_rgb(0, 0, 0), WIDTH / 2 + 190, 5, 0, "Zycia: %i", ludek.lives);
			}
			else if (status == KONIEC)
			{
				al_draw_textf(font48, al_map_rgb(0, 0, 0), WIDTH / 2, HEIGHT / 2 - 300, ALLEGRO_ALIGN_CENTRE, "KONIEC GRY");
				al_draw_textf(font32, al_map_rgb(0, 0, 0), WIDTH / 2, HEIGHT / 2 - 200, ALLEGRO_ALIGN_CENTRE, "WYNIK: %i", ludek.score);
				al_draw_textf(font32, al_map_rgb(0, 0, 0), WIDTH / 2, HEIGHT / 2 - 100, ALLEGRO_ALIGN_CENTRE, "UDERZONE PRZESZKODY: %i", ludek.score1);
				al_draw_textf(font32, al_map_rgb(0, 0, 0), WIDTH / 2, HEIGHT / 2, ALLEGRO_ALIGN_CENTRE, "OMINIETE PRZESZKODY: %i", ludek.score2);
				al_draw_textf(font32, al_map_rgb(0, 0, 0), WIDTH / 2, HEIGHT / 2 + 100, ALLEGRO_ALIGN_CENTRE, "ZEBRANE MONETY: %i", ludek.score3);
				al_draw_textf(font18, al_map_rgb(0, 0, 0), WIDTH / 2, HEIGHT / 2 + 200, ALLEGRO_ALIGN_CENTRE, "Nacisnij spacje aby zagrac ponownie");
				al_draw_textf(font18, al_map_rgb(0, 0, 0), WIDTH / 2, HEIGHT / 2 + 300, ALLEGRO_ALIGN_CENTRE, "Nacisnij esc aby wyjsc z gry");
			}
			al_flip_display();
			al_clear_to_color(al_map_rgb(50, 200, 255));
		}
	}
	
	al_destroy_bitmap(zycieImage);
	al_destroy_bitmap(monetaImage);
	al_destroy_bitmap(przeszkodyImage);
	al_destroy_bitmap(ludekImageLeft);
	al_destroy_bitmap(ludekImageRight);
	al_destroy_bitmap(ludekImageProsto);
	al_destroy_event_queue(event_queue);
	al_destroy_timer(timer);
	al_destroy_font(font18);
	al_destroy_font(font32);
	al_destroy_font(font48);
	al_destroy_display(display);						
	
	return 0;
}

void InitLudek(Spadochroniarz &ludek, ALLEGRO_BITMAP *image = NULL, ALLEGRO_BITMAP *image1 = NULL, ALLEGRO_BITMAP *image2 = NULL)
{
	ludek.x = WIDTH / 2;			//polozenie poczatkowe - szerokosc
	ludek.y = HEIGHT / 2 - 175;		//polozenie poczatkowe - wysokosc
	ludek.ID = GRACZ;
	ludek.lives = 5;				//zycia postaci
	ludek.speed = 8;				//predkosc postaci
	ludek.boundx = 28;				//toletancja kolizji - szerokosc
	ludek.boundy = 50;				//toletancja kolizji - wysokosc
	ludek.score = 0;				//punkty
	ludek.score1 = 0;				//uderzone przeszkody
	ludek.score2 = 0;				//ominiete przeszkody
	ludek.score3 = 0;				//zebrane monety

	if (image != NULL)
		ludek.image = image;		//ludek w lewo
	if (image1 != NULL)
		ludek.image1 = image1;		//ludek w prawo
	if (image2 != NULL)
		ludek.image2 = image2;		//ludek prosto
}

void DrawLudekLeft(Spadochroniarz &ludek)
{
	//al_draw_filled_rectangle(ludek.x, ludek.y, ludek.x + 30, ludek.y + 30, al_map_rgb(255, 0, 255));
	al_draw_bitmap(ludek.image, ludek.x, ludek.y, 0);
}
void DrawLudekRight(Spadochroniarz &ludek)
{
	al_draw_bitmap(ludek.image1, ludek.x, ludek.y, 0);
}
void DrawLudekProsto(Spadochroniarz &ludek)
{
	al_draw_bitmap(ludek.image2, ludek.x, ludek.y, 0);
}

void MoveLudekLeft(Spadochroniarz &ludek)
{
	ludek.x -= ludek.speed;
	DrawLudekLeft(ludek);
	if (ludek.x < 0)
		ludek.x = 0;
}
void MoveLudekRight(Spadochroniarz &ludek)
{
	ludek.x += ludek.speed;
	DrawLudekRight(ludek);
	if (ludek.x > 469)
		ludek.x = 468;
}

void InitPrzeszkoda(Przeszkoda przeszkody[], int size, ALLEGRO_BITMAP *image = NULL)
{
	for (int i = 0; i < size; i++)
	{
		przeszkody[i].ID = PRZESZKODA;
		przeszkody[i].live = false;
		przeszkody[i].speed = 5;
		przeszkody[i].boundx = 64;					//tolerancja kolizji
		przeszkody[i].boundy = 11;

		if (image != NULL)
			przeszkody[i].image = image;
	}
}
void DrawPrzeszkoda(Przeszkoda przeszkody[], int size)
{
	for (int i = 0; i < size; i++)
	{
		if (przeszkody[i].live)
		{	
			al_draw_bitmap(przeszkody[i].image, przeszkody[i].x, przeszkody[i].y, 0);

			//al_draw_filled_rectangle(przeszkody[i].x, przeszkody[i].y, przeszkody[i].x - 70, przeszkody[i].y - 15, al_map_rgb(255, 0, 0));
			//{
				//ograniczenia:
			/*if (przeszkody[i].x > ((przeszkody[i+1].x - 70) - przeszkody[i+1].boundx) &&		//prawa strona 
					przeszkody[i].x - 70 < (przeszkody[i+1].x + przeszkody[i+1].boundx) &&			//lewa strona 
					przeszkody[i].y > ((przeszkody[i+1].y - 15) - przeszkody[i+1].boundy) &&		//dol 
					przeszkody[i].y - 15 < (przeszkody[i+1].y + przeszkody[i+1].boundy))			//gora
				{
					przeszkody[i].live = false;
				}
				else if (!(przeszkody[i].x > ((przeszkody[i + 1].x - 70) - przeszkody[i + 1].boundx) &&		//prawa strona 
					przeszkody[i].x - 70 < (przeszkody[i + 1].x + przeszkody[i + 1].boundx) &&			//lewa strona 
					przeszkody[i].y > ((przeszkody[i + 1].y - 15) - przeszkody[i + 1].boundy) &&		//dol 
					przeszkody[i].y - 15 < (przeszkody[i + 1].y + przeszkody[i + 1].boundy)))			//gora
				{
					przeszkody[i].live = true;
					al_draw_filled_rectangle(przeszkody[i].x, przeszkody[i].y, przeszkody[i].x - 70, przeszkody[i].y - 15, al_map_rgb(255, 0, 0));

				}*/
			//}
			/*else
			{
				przeszkody[i].live = true;
				al_draw_filled_rectangle(przeszkody[i].x, przeszkody[i].y, przeszkody[i].x - 70, przeszkody[i].y - 15, al_map_rgb(255, 0, 0));
			}*/
			/*else if (przeszkody[i].live)
			{
				al_draw_filled_rectangle(przeszkody[i].x, przeszkody[i].y, przeszkody[i].x - 70, przeszkody[i].y - 15, al_map_rgb(255, 0, 0));
			}*/

			/*else if (!(przeszkody[i].x - 90 <(przeszkody[i].x - przeszkody[i].boundx) &&		//prawa strona 
				przeszkody[i].x > ((przeszkody[i].x + 90) + przeszkody[i].boundx) &&	//lewa strona 
				przeszkody[i].y - 30 < (przeszkody[i].y - przeszkody[i].boundy) &&		//dol 
				przeszkody[i].y > ((przeszkody[i].y + 30) + przeszkody[i].boundy)))
			{
				przeszkody[i].live = true;
				al_draw_filled_rectangle(przeszkody[i].x, przeszkody[i].y, przeszkody[i].x - 70, przeszkody[i].y - 15, al_map_rgb(255, 0, 0));
			}*/

		}
	}
}
void StartPrzeszkoda(Przeszkoda przeszkody[], int size)
{
	for (int i = 0; i < size; i++)
	{
		if (!przeszkody[i].live)
		{
			if (rand() % 100 == 0)									//generowanie losowej liczby 
			{
				przeszkody[i].live = true;
				przeszkody[i].x = rand() % (WIDTH - 75);			//przeszkody mieszcz¹ce sie na ekranie
				przeszkody[i].y = HEIGHT;

				break;
			}
		}
	}
}
void UpdatePrzeszkoda(Przeszkoda przeszkody[], int size, Spadochroniarz &ludek)
{
	for (int i = 0; i < size; i++)
	{
		if (przeszkody[i].live)
		{
			przeszkody[i].y -= przeszkody[i].speed;

			if (ludek.score > 100)
			{
				przeszkody[i].speed = 6;
			}
			if (ludek.score > 200)
			{
				przeszkody[i].speed = 7;
			}
			if (ludek.score > 300)
			{
				przeszkody[i].speed = 8;
			}
			if (ludek.score > 400)
			{
				przeszkody[i].speed = 9;
			}
			if (ludek.score > 500)
			{
				przeszkody[i].speed = 10;
			}
		}
	}
}
void CollidePrzeszkoda(Przeszkoda przeszkody[], int sizeB, Spadochroniarz &ludek)
{
	for (int i = 0; i < sizeB; i++)
	{
		if (przeszkody[i].live)
		{																			//ograniczenia:
			if (ludek.x - 30 > (przeszkody[i].x - przeszkody[i].boundx) &&			//prawa 
				ludek.x < (przeszkody[i].x + przeszkody[i].boundx) &&				//lewa 
				ludek.y + 15 > (przeszkody[i].y - przeszkody[i].boundy) &&			//dol 
				ludek.y < (przeszkody[i].y + przeszkody[i].boundy))					//gora 
			
				/*if (ludek.x + 30 >(przeszkody[i].x - przeszkody[i].boundx) &&		//prawa strona kwadratu
					ludek.x < ((przeszkody[i].x - 70) + przeszkody[i].boundx) &&	//lewa strona kwadratu
					ludek.y + 30 >(przeszkody[i].y - przeszkody[i].boundy) &&		//dol kwadratu
					ludek.y < ((przeszkody[i].y - 15) + przeszkody[i].boundy))*/	//gora kwadratu

				/*if (przeszkody[i].x - przeszkody[i].boundx < ludek.x + ludek.boundx &&
				przeszkody[i].x + przeszkody[i].boundx > ludek.x - ludek.boundx &&
				przeszkody[i].y - przeszkody[i].boundy < ludek.y + ludek.boundy &&
				przeszkody[i].y + przeszkody[i].boundy > ludek.y - ludek.boundy)*/
			{
				przeszkody[i].live = false;
				ludek.lives--;
				ludek.score1++;
			}

			else if (przeszkody[i].y < 0)
			{
					przeszkody[i].live = false;
					ludek.score++;
					ludek.score2++;
			}
		}
	}
}

void InitMonety(Monety moneta[], int size, ALLEGRO_BITMAP *image = NULL)
{
	for (int i = 0; i < size; i++)
	{
		moneta[i].ID = MONETA;
		moneta[i].live = false;
		moneta[i].speed = 5;
		moneta[i].boundx = 16;					//tolerancja kolizji
		moneta[i].boundy = 16;

		if (image != NULL)
			moneta[i].image = image;
	}
}
void DrawMonety(Monety moneta[], int size)
{
	for (int i = 0; i < size; i++)
	{
		if (moneta[i].live)
		{
			al_draw_bitmap(moneta[i].image, moneta[i].x, moneta[i].y, 0);
		}
	}
}
void StartMonety(Monety moneta[], int size)
{
	for (int i = 0; i < size; i++)
	{
		if (!moneta[i].live)
		{
			if (rand() % 300 == 0)									//generowanie losowej liczby 
			{														//im wiekszy % tym rzadziej sie pojawia
				moneta[i].live = true;
				moneta[i].x = rand() % (WIDTH - 75);				//przeszkody mieszcz¹ce sie na ekranie
				moneta[i].y = HEIGHT;

				break;
			}
		}
	}
}
void UpdateMonety(Monety moneta[], int size, Spadochroniarz &ludek)
{
	for (int i = 0; i < size; i++)
	{
		if (moneta[i].live)
		{
			moneta[i].y -= moneta[i].speed;

			if (ludek.score > 100)
			{
				moneta[i].speed = 6;
			}
			if (ludek.score > 200)
			{
				moneta[i].speed = 7;
			}
			if (ludek.score > 300)
			{
				moneta[i].speed = 8;
			}
			if (ludek.score > 400)
			{
				moneta[i].speed = 9;
			}
			if (ludek.score > 500)
			{
				moneta[i].speed = 10;
			}
		}
	}
}
void CollideMonety(Monety moneta[], int sizeB, Spadochroniarz &ludek)
{
	for (int i = 0; i < sizeB; i++)
	{
		if (moneta[i].live)
		{																	//ograniczenia:
			if (ludek.x + 20 > (moneta[i].x - moneta[i].boundx) &&			//prawa 
				ludek.x < (moneta[i].x + moneta[i].boundx) &&				//lewa 
				ludek.y + 20 > (moneta[i].y - moneta[i].boundy) &&			//dol 
				ludek.y < (moneta[i].y + moneta[i].boundy))					//gora 

			{
				moneta[i].live = false;
				ludek.score += 10;
				ludek.score3++;
			}

			else if (moneta[i].y < 0)
			{
				moneta[i].live = false;
			}
		}
	}
}

void InitZycia(Zycia zycie[], int size, ALLEGRO_BITMAP *image = NULL)
{
	for (int i = 0; i < size; i++)
	{
		zycie[i].ID = MONETA;
		zycie[i].live = false;
		zycie[i].speed = 5;
		zycie[i].boundx = 22;					//tolerancja kolizji
		zycie[i].boundy = 20;

		if (image != NULL)
			zycie[i].image = image;
	}
}
void DrawZycia(Zycia zycie[], int size)
{
	for (int i = 0; i < size; i++)
	{
		if (zycie[i].live)
		{
			al_draw_bitmap(zycie[i].image, zycie[i].x, zycie[i].y, 0);
		}
	}
}
void StartZycia(Zycia zycie[], int size)
{
	for (int i = 0; i < size; i++)
	{
		if (!zycie[i].live)
		{
			if (rand() % 1000 == 0)									//generowanie losowej liczby 
			{
				zycie[i].live = true;
				zycie[i].x = rand() % (WIDTH - 75);					//przeszkody mieszcz¹ce sie na ekranie
				zycie[i].y = HEIGHT;

				break;
			}
		}
	}
}
void UpdateZycia(Zycia zycie[], int size, Spadochroniarz &ludek)
{
	for (int i = 0; i < size; i++)
	{
		if (zycie[i].live)
		{
			zycie[i].y -= zycie[i].speed;

			if (ludek.score > 100)
			{
				zycie[i].speed = 6;
			}
			if (ludek.score > 200)
			{
				zycie[i].speed = 7;
			}
			if (ludek.score > 300)
			{
				zycie[i].speed = 8;
			}
			if (ludek.score > 400)
			{
				zycie[i].speed = 9;
			}
			if (ludek.score > 500)
			{
				zycie[i].speed = 10;
			}
		}
	}
}
void CollideZycia(Zycia zycie[], int sizeB, Spadochroniarz &ludek)
{
	for (int i = 0; i < sizeB; i++)
	{
		if (zycie[i].live)
		{																	//ograniczenia:
			if (ludek.x + 15 >(zycie[i].x - zycie[i].boundx) &&				//prawa 
				ludek.x < (zycie[i].x + zycie[i].boundx) &&					//lewa 
				ludek.y + 15 >(zycie[i].y - zycie[i].boundy) &&				//dol 
				ludek.y < (zycie[i].y + zycie[i].boundy))					//gora 

			{
				zycie[i].live = false;
				ludek.lives++;
			}

			else if (zycie[i].y < 0)
			{
				zycie[i].live = false;
			}
		}
	}
}

void ChangeStatus(int &status, int nowyStatus)
{
	if (status == MENU)
	{

	}
	else if (status == GRA)
	{

	}
	else if (status == KONIEC)
	{

	}

	status = nowyStatus;

	if (status == MENU)
	{

	}
	else if (status == GRA)
	{
		InitLudek(ludek);
		InitPrzeszkoda(przeszkody, NUM_PRZESZKODY);
		InitMonety(moneta, NUM_MONETY);
		InitZycia(zycie, NUM_ZYCIA);
	}
	else if (status == KONIEC)
	{

	}
}