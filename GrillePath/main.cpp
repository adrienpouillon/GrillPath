// GrillePath.cpp : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.
//

#include <iostream>
#include <vector>
#include <cstdio>
#include <conio.h>

void Draw();

std::vector< std::vector<char>> TILES =
{
    {'*','*','*','*','*','*','*','*','*','*' },
    {'*',' ',' ',' ',' ',' ',' ',' ',' ','*' },
    {'*',' ',' ',' ',' ',' ',' ',' ',' ','*' },
    {'*',' ',' ','*',' ',' ',' ',' ',' ','*' },
    {'*',' ',' ',' ',' ',' ',' ',' ',' ','*' },
    {'*',' ',' ',' ',' ',' ',' ',' ',' ','*' },
    {'*',' ',' ',' ',' ',' ',' ','*',' ','*' },
    {'*',' ','*',' ',' ',' ',' ',' ',' ','*' },
    {'*',' ','*','*','*',' ',' ',' ',' ','*' },
    {'*',' ',' ',' ',' ',' ',' ',' ',' ','*' },
    {'*','*','*','*','*','*','*','*','*','*' },
};

int MODE = 3;

struct Vector2
{
    int mX;
    int mY;
    Vector2() { mX = 0; mY = 0; }
    Vector2(int x, int y) { mX = x; mY = y; }
    void SetX(int x) { mX = x;}
    void SetY(int y) { mY = y; }
    void SetXY(int x, int y) {mX = x; mY = y; }
    int GetX() { return mX;}
    int GetY() { return mY;}
};

int GenerateRandomNumber(int min, int max)
{
    int range = max - min + 1;
    int value = rand() % range + min;
    return value;
}

void InputPlayer(Vector2* cursorPlayer, char chara)
{
    Vector2 newCursorPlayer = *cursorPlayer;
    switch (chara)
    {
    case 'w':
        newCursorPlayer.SetX(newCursorPlayer.GetX() - 1);
        break;
    case 's':
        newCursorPlayer.SetX(newCursorPlayer.GetX() + 1);
        break;
    case 'a':
        newCursorPlayer.SetY(newCursorPlayer.GetY() - 1);
        break;
    case 'd':
        newCursorPlayer.SetY(newCursorPlayer.GetY() + 1);
        break;
    }

    if (newCursorPlayer.GetX() < 0)
    {
        newCursorPlayer.SetX(0);
    }
    else
    {
        int lenght = TILES.size() - 1;
        if (newCursorPlayer.GetX() > lenght)
        {
            newCursorPlayer.SetX(lenght - 1);
        }
    }

    if (newCursorPlayer.GetY() < 0)
    {
        newCursorPlayer.SetY(0);
    }
    else
    {
        int lenght = TILES[0].size() - 1;
        if (newCursorPlayer.GetY() > lenght)
        {
            newCursorPlayer.SetY(lenght - 1);
        }
    }

    if (TILES[newCursorPlayer.GetX()][newCursorPlayer.GetY()] == '*')
    {
        newCursorPlayer = *cursorPlayer;
    }

    TILES[cursorPlayer->GetX()][cursorPlayer->GetY()] = ' ';
    TILES[newCursorPlayer.GetX()][newCursorPlayer.GetY()] = '0';
    *cursorPlayer = newCursorPlayer;
    //std::cout << cursorPlayer->GetX() << std::endl;
    //std::cout << cursorPlayer->GetY() << std::endl;
    //std::cout << newCursorPlayer.GetX() << std::endl;
    //std::cout << newCursorPlayer.GetY() << std::endl;
    
}

Vector2 IA1(Vector2* cursorIA, Vector2 cursorPlayer)
{
    Vector2 newCursorIA = *cursorIA;
    //newCursorIA.SetX(0);
    //newCursorIA.SetY(0);

    newCursorIA.SetX(cursorPlayer.GetX());
    newCursorIA.SetY(cursorPlayer.GetY());

    return newCursorIA;
}

Vector2 IA2(Vector2* cursorIA, Vector2 cursorPlayer)
{
    Vector2 newCursorIA = *cursorIA;
    
    float dis_m = abs(newCursorIA.GetX() - cursorPlayer.GetX()) + abs(newCursorIA.GetY() - cursorPlayer.GetX());

    while(newCursorIA.GetX() != cursorPlayer.GetX() || newCursorIA.GetY() != cursorPlayer.GetY())
    {
        float iaX = newCursorIA.GetX();
        float iaY = newCursorIA.GetY();

        float pX = cursorPlayer.GetX();
        float pY = cursorPlayer.GetY();
        if (iaX < pX)
        {
            newCursorIA.SetX(iaX + 1);
        }
        else if (iaX > pX)
        {
            newCursorIA.SetX(iaX - 1);
        }
        else if (iaY < pY)
        {
            newCursorIA.SetY(iaY + 1);
        }
        else if (iaY > pY)
        {
            newCursorIA.SetY(iaY - 1);
        }

        TILES[newCursorIA.GetX()][newCursorIA.GetY()] = '+';
    }

    return newCursorIA;
}

bool IAMove(bool haveMove, Vector2* newCursorIA, bool condition, int iaDir)
{
    if (haveMove == false && TILES[newCursorIA->GetX()][newCursorIA->GetY()] != '*')
    {
        if (condition)
        {
            newCursorIA->SetX(iaDir);
            return true;
        }
    }
    return haveMove;
}

Vector2 IA3(Vector2* cursorIA, Vector2 cursorPlayer)
{
    Vector2 newCursorIA = *cursorIA;

    float dis_m = abs(newCursorIA.GetX() - cursorPlayer.GetX()) + abs(newCursorIA.GetY() - cursorPlayer.GetX());

    int pX = cursorPlayer.GetX();
    int pY = cursorPlayer.GetY();

    int baitX = 0;
    int baitY = 0;

    while (newCursorIA.GetX() != pX || newCursorIA.GetY() != pY)
    {
        int iaX = newCursorIA.GetX();
        int iaY = newCursorIA.GetY();

        int pBaitX = pX + baitX;
        int pBaitY = pY + baitY;

        int iaRight = iaX + 1;
        int iaLeft = iaX - 1;
        int iaDown = iaY + 1;
        int iaUp = iaY - 1;

        bool haveMove = false;

        haveMove = IAMove(haveMove, &newCursorIA, iaX < pBaitX, iaRight);
        haveMove = IAMove(haveMove, &newCursorIA, iaX > pBaitX, iaLeft);
        haveMove = IAMove(haveMove, &newCursorIA, iaY < pBaitY, iaDown);
        haveMove = IAMove(haveMove, &newCursorIA, iaY > pBaitY, iaUp);

        if (haveMove == false)
        {
            switch (GenerateRandomNumber(0, 3))
            {
            case 0:
                haveMove = IAMove(haveMove, &newCursorIA, iaX == pBaitX, iaRight);
                if (haveMove) { pBaitX++; break; }
            case 1:
                haveMove = IAMove(haveMove, &newCursorIA, iaX == pBaitX, iaLeft);
                if (haveMove) { pBaitX--; break;}
            case 2 :
                haveMove = IAMove(haveMove, &newCursorIA, iaY == pBaitY, iaDown);
                if (haveMove) { pBaitY++; break;}
            case 3:
                haveMove = IAMove(haveMove, &newCursorIA, iaY == pBaitY, iaUp);
                if (haveMove) { pBaitY--; break; }
            case 4:
                haveMove = IAMove(haveMove, &newCursorIA, iaX == pBaitX, iaRight);
                if (haveMove) { pBaitX++; break; }
            case 5:
                haveMove = IAMove(haveMove, &newCursorIA, iaX == pBaitX, iaLeft);
                if (haveMove) { pBaitX--; break; }
            case 6:
                haveMove = IAMove(haveMove, &newCursorIA, iaY == pBaitY, iaDown);
                if (haveMove) { pBaitY++; break; }
            case 7:
                haveMove = IAMove(haveMove, &newCursorIA, iaY == pBaitY, iaUp);
                if (haveMove) { pBaitY--; break; }
            }
            
        }

        TILES[newCursorIA.GetX()][newCursorIA.GetY()] = '+';
        Draw();
    }

    return newCursorIA;
}

void InputIA(Vector2* cursorIA, Vector2 cursorPlayer, char chara)
{
    Vector2 newCursorIA = *cursorIA;
    
    switch (chara)
    {
    case ' ':
        
        switch (MODE)
        {
        case 1:
            newCursorIA = IA1(cursorIA, cursorPlayer);
            break;
        case 2:
            newCursorIA = IA2(cursorIA, cursorPlayer);
            break;
        case 3:
            newCursorIA = IA3(cursorIA, cursorPlayer);
            break;
        case 4:
            newCursorIA = IA1(cursorIA, cursorPlayer);
            break;
        }

        break;
    }

    TILES[cursorIA->GetX()][cursorIA->GetY()] = '+';
    TILES[newCursorIA.GetX()][newCursorIA.GetY()] = 'x';
    *cursorIA = newCursorIA;
}

void Input(Vector2* cursorPlayer, Vector2* cursorIA)
{
    int chara = _getch();
    //A=97 W=119 S=115 D=100 ENTER=10
    
    //InputIA(cursorIA, *cursorPlayer);
    InputPlayer(cursorPlayer, chara);
    InputIA(cursorIA, *cursorPlayer, chara);
}

void Update(Vector2* cursorPlayer, Vector2* cursorIA)
{

}

void Draw()
{
    system("cls");
    int lenghti = TILES.size();
    for (int i = 0; i < lenghti; i++)
    {
        int lenghtj = TILES[i].size();
        for (int j = 0; j < lenghtj; j++)
        {
            std::cout << "[";
            std::cout << TILES[i][j];
            std::cout << "]";
        }
        std::cout << std::endl;
    }
}

int main()
{
    Vector2 cursorPlayer(8, 5);
    TILES[cursorPlayer.GetX()][cursorPlayer.GetY()] = '0';
    Vector2 cursorIA(5, 5);
    TILES[cursorIA.GetX()][cursorIA.GetY()] = 'x';
    bool end = false;
    while (true)
    {
        Draw();

        Input(&cursorPlayer, &cursorIA);

        Update(&cursorPlayer, &cursorIA);


        if (end)
        {
            break;
        }
    }

    std::cout << "Hello World!\n";
}

