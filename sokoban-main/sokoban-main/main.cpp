#define SFML_NO_DEPRECATED_WARNINGS

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <stdio.h>

using namespace sf;
using namespace std;

#define SIZEX 12
#define SIZEY 12
#define SIZEF 48

#define WALL 1
#define PLACE 4
#define BOX 2
#define PLACEBOX 6
#define EMPTY 0

char board[SIZEX][SIZEY] = {
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        { 1, 0, 0, 0, 0, 4, 4, 4, 0, 0, 0, 1 },
        { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
        { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
        { 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1 },
        { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
        { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
        { 1, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 1 },
        { 1, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 1 },
        { 1, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 1 },
        { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 }
};
Vector2i human_vect(6, 9);

Sprite box;
Sprite place;
Sprite human;
Sprite wall;
Sprite pustio;
Sprite placebox;

vector <Vector2i> boxes;
vector <Vector2i> places;

void loadPosition()
{
    for (int i = 0; i < SIZEX; i++)
    {
        for (int j = 0; j < SIZEY; j++)
        {
            if (board[i][j] == 1)
            {
                wall.setPosition(i * SIZEF, j * SIZEF);
            }
            else
            if (board[i][j] == 2)
            {
                boxes.push_back(Vector2i(i, j));
                box.setPosition(i * SIZEF, j * SIZEF);
            }
            else
            if (board[i][j] == 4)
            {
                place.setPosition(i * SIZEF, j * SIZEF);
            }
            else
            if (board[i][j] == 0)
                pustio.setPosition(i * SIZEF, j * SIZEF);
            else
            if (board[i][j] == 6)
                placebox.setPosition(i * SIZEF, j * SIZEF);
        }
    }
    human.setPosition(human_vect.x * SIZEF, human_vect.y * SIZEF);

}
void make_step(int dx, int dy)
{
    if (!(
            human_vect.x > 0 && dx == -1 ||
            human_vect.x < SIZEX - 1 && dx == 1 ||
            human_vect.y > 0 && dy == -1 ||
            human_vect.y < SIZEY - 1 && dy == 1
    ))
        return;
    char &to = board[human_vect.x + dx][human_vect.y + dy];
    if (to == EMPTY || to == PLACE)
    {
        human_vect.x += dx;
        human_vect.y += dy;
    }
    else
    if (to == BOX || to == PLACEBOX)
    {
        char &toto = board[human_vect.x + dx * 2][human_vect.y + dy * 2];
        if (toto == EMPTY || toto == PLACE)
        {
            to = to - (to & BOX);
            toto += BOX;
            human_vect.x += dx;
            human_vect.y += dy;
        }
    }
}
bool no_free_boxes()
{
    for (int i = 0; i < SIZEX; i++)
    {
        for (int j = 0; j < SIZEY; j++)
        {
            if (board[i][j] == 2)
                return false;
        }
    }
    return true;
}

int main(int argv, char *argc[])
{

    RenderWindow window(VideoMode(SIZEX * SIZEF, SIZEY * SIZEF), "Sokoban!");
    window.setFramerateLimit(60);

    Texture t2, t7;
    t2.loadFromFile("images/background.jpg");
    t7.loadFromFile("images/figures.png");

    box.setTexture(t7);		box.setTextureRect(IntRect(0, 0, SIZEF, SIZEF));
    place.setTexture(t7);   place.setTextureRect(IntRect(SIZEF, 0, SIZEF, SIZEF));
    human.setTexture(t7);   human.setTextureRect(IntRect(2 * SIZEF, 0, SIZEF, SIZEF));
    wall.setTexture(t7);    wall.setTextureRect(IntRect(3 * SIZEF, 0, SIZEF, SIZEF));
    pustio.setTexture(t7);   pustio.setTextureRect(IntRect(4 * SIZEF, 0, SIZEF, SIZEF));
    placebox.setTexture(t7);   placebox.setTextureRect(IntRect(5 * SIZEF, 0, SIZEF, SIZEF));

    loadPosition();

    //int even = 0;
    while (window.isOpen())
    {
        Event e;
        while (window.pollEvent(e))
        {
            if (e.type == Event::Closed)
                window.close();
            if (Keyboard::isKeyPressed(Keyboard::Escape) )
            {
                window.close();
            }
            if (Keyboard::isKeyPressed(Keyboard::Left))
            {
                make_step(-1, 0);
            }
            if (Keyboard::isKeyPressed(Keyboard::Right))
            {
                make_step(1, 0);
            }
            if (Keyboard::isKeyPressed(Keyboard::Up))
            {
                make_step(0, -1);
            }
            if (Keyboard::isKeyPressed(Keyboard::Down))
            {
                make_step(0, 1);
            }
        }
        window.clear();
        for (int i = 0; i < SIZEX; i++)
        {
            for (int j = 0; j < SIZEY; j++)
            {
                if (board[i][j] == 1)
                {
                    wall.setPosition(i * SIZEF, j * SIZEF);
                    window.draw(wall);
                }
                else
                if (board[i][j] == 2)
                {
                    box.setPosition(i * SIZEF, j * SIZEF);
                    window.draw(box);
                }
                else
                if (board[i][j] == 4)
                {
                    place.setPosition(i * SIZEF, j * SIZEF);
                    window.draw(place);
                }
                else
                if (board[i][j] == 0)
                {
                    pustio.setPosition(i * SIZEF, j * SIZEF);
                    window.draw(pustio);
                }
                else
                if (board[i][j] == 6)
                {
                    placebox.setPosition(i * SIZEF, j * SIZEF);
                    window.draw(placebox);
                }
            }
        }
        human.setPosition(human_vect.x * SIZEF, human_vect.y * SIZEF);
        window.draw(human);
        if (no_free_boxes())
        {
            cout << "You win!!!" << endl;
        }
        window.display();
    }
    return 0;
}
