#include "includes.h"
#include <SFML/Graphics.hpp>

class Player
{
private:
    char grid[3][3] = { '0' };
    char sign;
    int scale;
    void ClearGrid();
public:
    bool turn;
    Player(char,bool, int);
    void SetPos(int, int);
    bool PosOK(int, int);
    void DrawGrid(sf::RenderWindow&);
};