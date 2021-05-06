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

void Player::SetPos(int x, int y,char c)
{
    grid[y][x] = c;
}

bool Player::PosOK(int x, int y)
{
    if (x < 3 && x>=0 && y < 3 && y>=0) 
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
    for (int y = 1; y < 3; y++)
    {
        sf::VertexArray yLines(sf::LinesStrip, 2);
        yLines[0].position = sf::Vector2f(0, y * scale);
        yLines[1].position = sf::Vector2f(scale * 3, y * scale);
        window.draw(yLines);
    }

    for (int x = 1; x < 3; x++)
    {
        sf::VertexArray xLines(sf::LinesStrip, 2);
        xLines[0].position = sf::Vector2f(x * scale, 0);
        xLines[1].position = sf::Vector2f(x * scale, 3 * scale);
        window.draw(xLines);
    }

    DrawXO(window);
}

void Player::DrawXO(sf::RenderWindow& window){
    for(int y=0; y<3; y++){
        for(int x=0; x<3; x++){
            if(grid[y][x]=='x'){
                sf::VertexArray Line1(sf::LinesStrip, 2);
                Line1[0].position = sf::Vector2f(x*scale,y*scale);
                Line1[1].position = sf::Vector2f((x+1)*scale, (y+1)*scale);
                Line1[0].color= sf::Color::Blue;
                Line1[1].color= sf::Color::Blue;
                sf::VertexArray Line2(sf::LinesStrip, 2);
                Line2[0].position = sf::Vector2f(x*scale,(y+1)*scale);
                Line2[1].position = sf::Vector2f((x+1)*scale, y*scale);
                Line2[0].color= sf::Color::Blue;
                Line2[1].color= sf::Color::Blue;
                window.draw(Line1);
                window.draw(Line2);
            }
            if(grid[y][x]=='o'){
                sf::VertexArray Line1(sf::LinesStrip, 2);
                Line1[0].position = sf::Vector2f(x*scale,y*scale);
                Line1[1].position = sf::Vector2f((x+1)*scale, (y+1)*scale);
                Line1[0].color= sf::Color::Red;
                Line1[1].color= sf::Color::Red;
                sf::VertexArray Line2(sf::LinesStrip, 2);
                Line2[0].position = sf::Vector2f(x*scale,(y+1)*scale);
                Line2[1].position = sf::Vector2f((x+1)*scale, y*scale);
                Line2[0].color= sf::Color::Red;
                Line2[1].color= sf::Color::Red;
                window.draw(Line1);
                window.draw(Line2);
            }
        }
    }
}

char Player::GetOppositeSign(){
    if(sign='x'){
        return 'o';
    }
    return 'x';
}