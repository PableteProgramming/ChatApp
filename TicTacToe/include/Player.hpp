#include "includes.h"
#include <SFML/Graphics.hpp>

class Player
{
private:
    char grid[3][3] = { '0' };
    char sign;
    int scale;
    void ClearGrid();
    void DrawXO(sf::RenderWindow&);
public:
    bool turn;
    Player(char,bool, int);
    void SetPos(int, int,char);
    bool PosOK(int, int);
    void DrawGrid(sf::RenderWindow&);
    char GetSign(){return sign;};
    char GetOppositeSign();
    int Win();
};