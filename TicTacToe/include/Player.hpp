#include "ClientClass.hpp"
class Player
{
private:
    char grid[3][3] = { '0' };
    char sign;
    void ClearGrid();
public:
    bool turn;
    ClientClass* client;
    Player(char,bool,ClientClass*);
    void SetPos(int, int);
};