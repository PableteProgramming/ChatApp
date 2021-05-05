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

Player::Player(char s,bool t, int _s)
{
    ClearGrid();
    sign = s;
    turn = t;
    scale = _s;

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

void Player::DrawGrid(sf::RenderWindow& window)
{
    for (int y = 0; y < 2; y++)
    {
        sf::VertexArray yLines(sf::LinesStrip, 2);
        yLines[0].position = sf::Vector2f(0, y * scale);
        yLines[1].position = sf::Vector2f(scale * 3, y * scale);
        window.draw(yLines);
    }

    for (int x = 0; x < 2; x++)
    {
        sf::VertexArray xLines(sf::LinesStrip, 2);
        xLines[0].position = sf::Vector2f(x * scale, 0);
        xLines[1].position = sf::Vector2f(x * scale, 3 * scale);
        window.draw(xLines);
    }
}