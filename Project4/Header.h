enum IDS { GRACZ, PRZESZKODA, MONETA, ZYCIE };

struct Spadochroniarz
{
	int ID;
	int x;
	int y;
	int lives;
	int speed;
	int boundx;
	int boundy;
	int score;
	int score1;
	int score2;
	int score3;

	ALLEGRO_BITMAP *image;
	ALLEGRO_BITMAP *image1;
	ALLEGRO_BITMAP *image2;
};

struct Przeszkoda
{
	int ID;
	int x;
	int y;
	bool live;
	int speed;
	int boundx;
	int boundy;

	ALLEGRO_BITMAP *image;
};

struct Monety
{
	int ID;
	int x;
	int y;
	bool live;
	int speed;
	int boundx;
	int boundy;

	ALLEGRO_BITMAP *image;
};

struct Zycia
{
	int ID;
	int x;
	int y;
	bool live;
	int speed;
	int boundx;
	int boundy;

	ALLEGRO_BITMAP *image;
};

struct Tlo
{
	float x;
	float y;
	float velX;
	float velY;
	int dirX;
	int dirY;

	int width;
	int height;

	ALLEGRO_BITMAP *image;

};

struct Tlo1
{
	float x;
	float y;
	float velX;
	float velY;
	int dirX;
	int dirY;

	int width;
	int height;

	ALLEGRO_BITMAP *image;

};

struct Tlo2
{
	float x;
	float y;
	float velX;
	float velY;
	int dirX;
	int dirY;

	int width;
	int height;

	ALLEGRO_BITMAP *image;

};
