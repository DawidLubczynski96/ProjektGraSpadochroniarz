#include <allegro5\allegro.h>
#include <allegro5\allegro_primitives.h>
#include <allegro5\allegro_font.h>
#include <allegro5\allegro_ttf.h>
#include <allegro5\allegro_image.h>
#include "Header.h"

//GLOBAL=============================
const int WIDTH = 500;
const int HEIGHT = 700;
const int NUM_PRZESZKODY = 15;							//przeszkody na ekranie
enum KEYS { LEFT, RIGHT };
bool keys[2] = { false, false };


//prototypy
void InitLudek(Spadochroniarz &ludek);
void DrawLudek(Spadochroniarz &ludek);
void MoveLudekLeft(Spadochroniarz &ludek);
void MoveLudekRight(Spadochroniarz &ludek);

void InitPrzeszkoda(Przeszkoda przeszkody[], int size);
void DrawPrzeszkoda(Przeszkoda przeszkody[], int size);
void StartPrzeszkoda(Przeszkoda przeszkody[], int size);
void UpdatePrzeszkoda(Przeszkoda przeszkody[], int size);
void CollidePrzeszkoda(Przeszkoda przeszkody[], int sizeB, Spadochroniarz &ludek);



int main(void)
{
	//prymitywy 
	bool done = false;
	bool redraw = true;
	const int FPS = 60;
	bool KoniecGry = false;

	//int pos_x = WIDTH / 2;
	//int pos_y = HEIGHT / 2 - 175;

	//int ludWidth = 0;
	//int ludHeight = 0;

	//obiekty
	Spadochroniarz ludek;
	Przeszkoda przeszkody[NUM_PRZESZKODY];

	//zmienne Allegro
	ALLEGRO_DISPLAY *display = NULL;
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;
	ALLEGRO_TIMER *timer = NULL;
	ALLEGRO_FONT *font18 = NULL;
	ALLEGRO_FONT *font32 = NULL;

	//ALLEGRO_BITMAP *lud = NULL;
	

	//initialization functions
	if (!al_init())										//initialize Allegro
		return -1;

	display = al_create_display(WIDTH, HEIGHT);			//create our display object

	if (!display)										//test display object
		return -1;

	//if (!lud)											//sprawdzenie bitmapy
	//	return -1;

	al_init_primitives_addon();
	al_install_keyboard();
	al_init_font_addon();
	al_init_ttf_addon();
	//al_init_image_addon();

	event_queue = al_create_event_queue();
	timer = al_create_timer(1.0 / FPS);
	

	srand(time(NULL));
	InitLudek(ludek);
	InitPrzeszkoda(przeszkody, NUM_PRZESZKODY);

	//lud = al_load_bitmap("spadochroniarz.bmp");
	//ludWidth = al_get_bitmap_width(lud);
	//ludHeight = al_get_bitmap_height(lud);

	font18 = al_load_font("ARIALNBI.TTF", 18, 0);		//(czcionka, rozmiar, flagi)
	font32 = al_load_font("ARIALNBI.TTF", 32, 0);


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

			if (keys[LEFT])
				MoveLudekLeft(ludek);
			if (keys[RIGHT])
				MoveLudekRight(ludek);
			
			if (!KoniecGry)
			{
				StartPrzeszkoda(przeszkody, NUM_PRZESZKODY);			//inicjuje przeszkode
				UpdatePrzeszkoda(przeszkody, NUM_PRZESZKODY);			//ponowne rysowanie po wyjsciu za ekran
				CollidePrzeszkoda(przeszkody, NUM_PRZESZKODY, ludek);
				
				if (ludek.lives <= 0)
					KoniecGry = true;
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
				done = true;
				break;
			case ALLEGRO_KEY_LEFT:
				keys[LEFT] = true;
				break;
			case ALLEGRO_KEY_RIGHT:
				keys[RIGHT] = true;
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
			}
		}
		
		if (redraw && al_is_event_queue_empty(event_queue))
		{
			redraw = false;

			//zmiana pozycji keys = 1 jesli mamy klawisz wcisniety (1*10=10) przesuwa o 10 pikseli
			//jesli keysup=0 wtedy 0*10 nie przesuwa 
			//pos_x -= keys[LEFT] * 10;
			//pos_x += keys[RIGHT] * 10;

			if (!KoniecGry)
			{
				DrawLudek(ludek);
				DrawPrzeszkoda(przeszkody, NUM_PRZESZKODY);

				al_draw_textf(font18, al_map_rgb(0, 0, 0), WIDTH / 2 - 245, 5, 0, "Punkty: %i", ludek.score);
				al_draw_textf(font18, al_map_rgb(0, 0, 0), WIDTH / 2 + 190, 5, 0, "Zycia: %i", ludek.lives);
			}
			else
			{
				al_draw_textf(font32, al_map_rgb(0, 0, 0), WIDTH / 2, HEIGHT / 2, ALLEGRO_ALIGN_CENTRE, "KONIEC GRY. WYNIK: %i", ludek.score);
			}
			//al_draw_bitmap(lud, WIDTH / 2 - ludWidth / 2, HEIGHT / 2 - ludHeight / 2, 0);
		

			al_flip_display();
			al_clear_to_color(al_map_rgb(0, 255, 0));
		}
	}
	
	
	//al_destroy_bitmap(lud);
	al_destroy_display(display);						//destroy our display object
	
	return 0;
}

void InitLudek(Spadochroniarz &ludek)
{
	ludek.x = WIDTH / 2;
	ludek.y = HEIGHT / 2 - 175;
	ludek.ID = GRACZ;
	ludek.lives = 5;
	ludek.speed = 8;
	ludek.boundx = 28;									//toletancja kolizji
	ludek.boundy = 28;
	ludek.score = 0;
}

void DrawLudek(Spadochroniarz &ludek)
{
	al_draw_filled_rectangle(ludek.x, ludek.y, ludek.x + 30, ludek.y + 30, al_map_rgb(255, 0, 255));
	//al_draw_bitmap(lud, WIDTH / 2 - ludWidth / 2, HEIGHT / 2 - ludHeight / 2, 0);
}

void MoveLudekLeft(Spadochroniarz &ludek)
{
	ludek.x -= ludek.speed;
	if (ludek.x < 0)
		ludek.x = 0;
}
void MoveLudekRight(Spadochroniarz &ludek)
{
	ludek.x += ludek.speed;
	if (ludek.x > 471)
		ludek.x = 470;
}

void InitPrzeszkoda(Przeszkoda przeszkody[], int size)
{
	for (int i = 0; i < size; i++)
	{
		przeszkody[i].ID = PRZESZKODA;
		przeszkody[i].live = false;
		przeszkody[i].speed = 5;
		przeszkody[i].boundx = 65;					//tolerancja kolizji
		przeszkody[i].boundy = 13;
	}
}
void DrawPrzeszkoda(Przeszkoda przeszkody[], int size)
{
	for (int i = 0; i < size; i++)
	{
		if (przeszkody[i].live)
		{
			al_draw_filled_rectangle(przeszkody[i].x, przeszkody[i].y, przeszkody[i].x - 70, przeszkody[i].y - 15, al_map_rgb(255, 0, 0));
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
				przeszkody[i].x = 30 + rand() % (WIDTH - 60);		//50 pikseli od prawej i lewej
				przeszkody[i].y = HEIGHT;

				break;
			}
			//break;
		}
	}
}
void UpdatePrzeszkoda(Przeszkoda przeszkody[], int size)
{
	for (int i = 0; i < size; i++)
	{
		if (przeszkody[i].live)
		{
			przeszkody[i].y -= przeszkody[i].speed;

			//if (przeszkody[i].y < 0)
			//	przeszkody[i].live = false;						//do kosza
		}
	}
}
void CollidePrzeszkoda(Przeszkoda przeszkody[], int sizeB, Spadochroniarz &ludek)
{
	for (int i = 0; i < sizeB; i++)
	{
		if (przeszkody[i].live)
		{																		//ograniczenia:
			if(ludek.x + 30 > (przeszkody[i].x - przeszkody[i].boundx) &&		//prawa strona kwadratu
				ludek.x < ((przeszkody[i].x - 70) + przeszkody[i].boundx) &&	//lewa strona kwadratu
				ludek.y > (przeszkody[i].y - przeszkody[i].boundy) &&			//dol kwadratu
				ludek.y < (przeszkody[i].y + przeszkody[i].boundy))			//gora kwadratu
			
			/*if (przeszkody[i].x - przeszkody[i].boundx < ludek.x + ludek.boundx &&
				przeszkody[i].x + przeszkody[i].boundx > ludek.x - ludek.boundx &&
				przeszkody[i].y - przeszkody[i].boundy < ludek.y + ludek.boundy &&
				przeszkody[i].y + przeszkody[i].boundy > ludek.y - ludek.boundy)*/
			{
				ludek.lives--;
				przeszkody[i].live = false;
			}
			else if (przeszkody[i].y < 0)
			{
					przeszkody[i].live = false;
					ludek.score++;
			}
		}
	}
}