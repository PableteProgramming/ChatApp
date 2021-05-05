#include "includes.h"
class Player
{
private:
    char grid[3][3] = { '0' };
    char sign;
    void ClearGrid();
public:
    bool turn;
    Player(char,bool);
    void SetPos(int, int);
    bool PosOK(int, int);
};