#include <Player.hpp>

void Player::ClearGrid()
{
    for (int y = 0; y < 3; y++)
    {
        for (int x = 0; x < 3; x++)
        {
            grid[y][x] = '0';
        }
    }
}

Player::Player(char s,bool t,ClientClass* c)
{
    ClearGrid();
    sign = s;
    turn= t;
    client=c;
}

void Player::SetPos(int x, int y)
{
    grid[y][x] = sign;
}