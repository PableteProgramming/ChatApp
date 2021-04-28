class Player
{
private:
    char grid[3][3] = { '0' };
    char sign;
    void ClearGrid();
public:
    Player(char);
    void SetPos(int, int);
};