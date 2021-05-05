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

Player::Player(char s,bool t)
{
    ClearGrid();
    sign = s;
    turn= t;
}

void Player::SetPos(int x, int y)
{
    grid[y][x] = sign;
}

bool Player::PosOK(int x, int y)
{
    if (x < 3 && y < 3) 
    {
        if (grid[y][x] == '0')
        {
            return true;
        }
    }
    return false;
}