#include "storage.h"
#include <SFML/Graphics.hpp>
#include <time.h>
#include <sstream>
#include <vector>
#include <iostream>

#define NAME "super user"
#define DATABASE "game.bd"


using namespace sf;


struct point {
	int x, y;
};


int main() {
	srand(time(0));
	RenderWindow app(
			VideoMode(1920, 1080),
			"IgRULka",
			Style::Fullscreen);
	app.setFramerateLimit(60);
	int playerScore = 0;
	std::ostringstream playerScoreString, recordsString;
	Font font;
	font.loadFromFile("../CyrilicOld.TTF");
	Text text("", font, 40);
	text.setStyle(sf::Text::Bold | sf::Text::Underlined);
	Text curent_records("", font, 30);
	curent_records.setStyle(sf::Text::Bold | sf::Text::Underlined);
	Texture hero, nb, background;
	Texture gameover, r, keyboardtabl;
	Texture leaderboard, test;
	background.loadFromFile("../images/back.png");
	nb.loadFromFile("../images/nb.png");
	hero.loadFromFile("../images/hero.png");
	gameover.loadFromFile("../images/gameover.png");
	r.loadFromFile("../images/R.png");
	keyboardtabl.loadFromFile("../images/KeyboardTabl.png");
	leaderboard.loadFromFile("../images/LeaderTabl.png");
	test.loadFromFile("../images/test.png");

	Storage *storage;
	storage = new Storage(NAME, DATABASE);
	std::vector<record_db> records;
	records = storage->get_records(7);

	Sprite sHero(hero), sNb(nb), sBackground(background);
	Sprite sGameover(gameover), sR(r), sKeyboardtabl(keyboardtabl);
	Sprite sLeaderboard(leaderboard), sTest(test);
	sBackground.setPosition(610, 0);
	sGameover.setPosition(680, 200);
	sR.setPosition(820, 550);
	sLeaderboard.setPosition(0, 500);
	point plat[15];
	for (int i = 0; i < 15; i++)
	{
		plat[i].x = 600 + rand() % 601;
		plat[i].y = rand() % 1080;
	}
	int x = 900;	//начальная позиция по X
	int y = 500;
	int h = 700;	//максимальная высота
	float dy = 0;

	int delta = 15; //сила прыжка
	float speed = 0.2; //скорость
	int change = 500;

	while (app.isOpen())
	{
		if ((Keyboard::isKeyPressed(Keyboard::Right)) ||
			(Keyboard::isKeyPressed(Keyboard::D)))
		{
			x += 5;
		}
		if ((Keyboard::isKeyPressed(Keyboard::Left)) ||
			(Keyboard::isKeyPressed(Keyboard::A)))
		{
			x -= 5;
		}
		if (Keyboard::isKeyPressed(Keyboard::Escape))
		{
			storage->add_record(playerScore);
			app.close();
		}
		dy += speed;
		y += dy;
		if (y > 1080) {
			dy = -delta;
		}
		if (y < h) {
			for (int i = 0; i < 15; i++)
			{
				y = h;
				plat[i].y = plat[i].y - dy;
				if (plat[i].y > 1080)
				{
					plat[i].y = 0; plat[i].x = 600 + rand() % 601;
				}
			}
			if (playerScore % change == 0){
				speed += 0.2;
				delta += 5;
			}
			playerScore++;
		}
		for (int i = 0; i < 15; i++)
		{
			if ((x + 37 > plat[i].x) && (x + 37 < plat[i].x + 110) &&
				(y + 80 > plat[i].y) && (y + 80 < plat[i].y + 30) &&
				(dy > 0))
			{
				dy = -delta;
			}
		}
		sHero.setPosition(x, y);
		app.clear(Color(128, 106, 89));
		app.draw(sKeyboardtabl);
		app.draw(sLeaderboard);
		app.draw(sBackground);
		playerScoreString.str("");
		playerScoreString << playerScore;
		text.setString("Your points = " + playerScoreString.str());
		text.setPosition(1400, 100);
		app.draw(text);

		recordsString.str("");
		for (int i=0; i<records.size(); ++i)
		{
			recordsString << i+1 << ". ";
			recordsString << records[i].name << ", ";
			recordsString << records[i].value << '\n';
		}
		curent_records.setString(recordsString.str());
		curent_records.setPosition(120, 700);
		app.draw(curent_records);
		for (int i = 0; i < 15; i++)
		{
			sNb.setPosition(plat[i].x, plat[i].y);
			app.draw(sNb);
		}
		if (x > 1245) x = 610;
		if (x < 610)  x = 1245;
		if (y > 1080)
		{
			dy = 0;
			app.draw(sGameover);
			app.draw(sR);
			if (Keyboard::isKeyPressed(Keyboard::R))
			{
				dy = -delta;
				storage->add_record(playerScore);
				playerScore = 0;
				delta = 15;
				speed = 0.2;
			}
		}
		app.draw(sHero);
		app.display();
	}
	return 0;
}
