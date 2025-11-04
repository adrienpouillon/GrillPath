// GrillePath.cpp : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.
//

#include <iostream>
#include <vector>
#include <cstdio>
#include <conio.h>
#include <windows.h>

void Draw(char player, char ia, char obstacle);

std::vector< std::vector<char>> TILES;

int MODE1 = 4;

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

std::vector< std::vector<char>> StartTab()
{
    return {
                { '*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*' },
                { '*','*',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','*' },
                { '*',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','*','*','*','*','*','*','*' },
                { '*',' ',' ','*',' ',' ',' ',' ',' ',' ',' ',' ',' ','*',' ',' ',' ',' ',' ','*' },
                { '*',' ',' ',' ',' ',' ',' ',' ',' ','*',' ',' ',' ','*',' ',' ',' ',' ',' ','*' },
                { '*',' ',' ',' ',' ',' ',' ',' ',' ',' ','*',' ',' ','*',' ',' ',' ',' ',' ','*' },
                { '*',' ',' ',' ',' ',' ',' ','*',' ',' ','*','*','*','*',' ',' ',' ',' ',' ','*' },
                { '*',' ','*',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','*','*',' ','*' },
                { '*',' ','*','*','*',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','*',' ',' ','*' },
                { '*',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','*','*','*',' ','*' },
                { '*',' ',' ',' ',' ',' ','*','*',' ',' ','*',' ',' ',' ',' ',' ',' ','*',' ','*' },
                { '*',' ','*','*',' ',' ',' ','*','*',' ','*',' ',' ',' ',' ',' ',' ','*',' ','*' },
                { '*',' ','*',' ',' ','*',' ',' ',' ',' ','*','*',' ',' ',' ',' ','*','*',' ','*' },
                { '*',' ',' ',' ',' ',' ',' ',' ',' ',' ','*',' ',' ',' ',' ',' ',' ',' ',' ','*' },
                { '*',' ',' ','*',' ','*','*','*','*',' ','*',' ',' ',' ','*','*','*','*',' ','*' },
                { '*','*',' ',' ',' ',' ','*',' ',' ',' ','*',' ',' ',' ','*',' ',' ','*',' ','*' },
                { '*',' ',' ','*','*',' ',' ',' ',' ',' ','*',' ',' ',' ',' ',' ',' ','*',' ','*' },
                { '*',' ',' ',' ','*',' ',' ',' ',' ',' ','*',' ',' ',' ',' ',' ',' ','*',' ','*' },
                { '*',' ','*',' ',' ',' ','*',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','*',' ','*' },
                { '*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*' }
           };
}

void InputPlayer(Vector2* cursorPlayer, char chara, char player, char obstacle)
{
    Vector2 newCursorPlayer = *cursorPlayer;
    switch (chara)
    {
    case 'z':
        newCursorPlayer.SetX(newCursorPlayer.GetX() - 1);
        break;
    case 's':
        newCursorPlayer.SetX(newCursorPlayer.GetX() + 1);
        break;
    case 'a':
        newCursorPlayer.SetY(newCursorPlayer.GetY() - 1);
        break;
    case 'e':
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

    if (TILES[newCursorPlayer.GetX()][newCursorPlayer.GetY()] == obstacle)
    {
        newCursorPlayer = *cursorPlayer;
    }

    TILES[cursorPlayer->GetX()][cursorPlayer->GetY()] = ' ';
    TILES[newCursorPlayer.GetX()][newCursorPlayer.GetY()] = player;
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

bool IAMoveX(bool haveMove, Vector2* newCursorIA, bool condition, int iaDir, char obstacle)
{
    if (haveMove == false && TILES[iaDir][newCursorIA->GetY()] != obstacle)
    {
        if (condition)
        {
            newCursorIA->SetX(iaDir);
            return true;
        }
    }
    return haveMove;
}

bool IAMoveY(bool haveMove, Vector2* newCursorIA, bool condition, int iaDir, char obstacle)
{
    if (haveMove == false && TILES[newCursorIA->GetX()][iaDir] != obstacle)
    {
        if (condition)
        {
            newCursorIA->SetY(iaDir);
            return true;
        }
    }
    return haveMove;
}

bool IAMoveX(bool haveMove, Vector2* newCursorIA, bool condition, int iaDir, char path, char trail, char player)
{
    if (haveMove == false && TILES[iaDir][newCursorIA->GetY()] == path || TILES[iaDir][newCursorIA->GetY()] == trail || TILES[iaDir][newCursorIA->GetY()] == player)
    {
        if (condition)
        {
            newCursorIA->SetX(iaDir);
            return true;
        }
    }
    return haveMove;
}

bool IAMoveY(bool haveMove, Vector2* newCursorIA, bool condition, int iaDir, char path, char trail, char player)
{
    if (haveMove == false && TILES[newCursorIA->GetX()][iaDir] == path || TILES[newCursorIA->GetX()][iaDir] == trail || TILES[newCursorIA->GetX()][iaDir] == player)
    {
        if (condition)
        {
            newCursorIA->SetY(iaDir);
            return true;
        }
    }
    return haveMove;
}

Vector2 IA3(Vector2* cursorIA, Vector2 cursorPlayer, char path, char trail, char player, char ia, char obstacle)
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

        haveMove = IAMoveX(haveMove, &newCursorIA, iaX < pBaitX, iaRight, path, trail, player);
        haveMove = IAMoveX(haveMove, &newCursorIA, iaX > pBaitX, iaLeft, path, trail, player);
        haveMove = IAMoveY(haveMove, &newCursorIA, iaY < pBaitY, iaDown, path, trail, player);
        haveMove = IAMoveY(haveMove, &newCursorIA, iaY > pBaitY, iaUp, path, trail, player);

        if (haveMove == false)
        {
            switch (GenerateRandomNumber(0, 3))
            {
            case 0:
                haveMove = IAMoveX(haveMove, &newCursorIA, iaX == pBaitX, iaRight, path, trail, player);
                if (haveMove) { baitX++; break; }
            case 1:
                haveMove = IAMoveX(haveMove, &newCursorIA, iaX == pBaitX, iaLeft, path, trail, player);
                if (haveMove) { baitX--; break; }
            case 2 :
                haveMove = IAMoveY(haveMove, &newCursorIA, iaY == pBaitY, iaDown, path, trail, player);
                if (haveMove) { baitY++; break; }
            case 3:
                haveMove = IAMoveY(haveMove, &newCursorIA, iaY == pBaitY, iaUp, path, trail, player);
                if (haveMove) { baitY--; break; }
            case 4:
                haveMove = IAMoveX(haveMove, &newCursorIA, iaX == pBaitX, iaRight, path, trail, player);
                if (haveMove) { baitX++; break; }
            case 5:
                haveMove = IAMoveX(haveMove, &newCursorIA, iaX == pBaitX, iaLeft, path, trail, player);
                if (haveMove) { baitX--; break; }
            case 6:
                haveMove = IAMoveY(haveMove, &newCursorIA, iaY == pBaitY, iaDown, path, trail, player);
                if (haveMove) { baitY++; break; }
            case 7:
                haveMove = IAMoveY(haveMove, &newCursorIA, iaY == pBaitY, iaUp, path, trail, player);
                if (haveMove) { baitY--; break; }
            }
        }

        TILES[newCursorIA.GetX()][newCursorIA.GetY()] = '-';
        Draw(player, ia, obstacle);
        TILES[newCursorIA.GetX()][newCursorIA.GetY()] = '+';
    }

    return newCursorIA;
}

/*Vector2 IA4(Vector2* cursorIA, Vector2 cursorPlayer)
{
    Vector2 newCursorIA = *cursorIA;

    float dis_m = abs(newCursorIA.GetX() - cursorPlayer.GetX()) + abs(newCursorIA.GetY() - cursorPlayer.GetX());

    int pX = cursorPlayer.GetX();
    int pY = cursorPlayer.GetY();

    int baitX = 0;
    int baitY = 0;

    int nbBait = 0;
    int nbBaitMax = 10;

    int min = -1;
    int max = 1;

    while (newCursorIA.GetX() != pX || newCursorIA.GetY() != pY)
    {
        if (nbBait > nbBaitMax)
        {
            baitX = 0;
            baitY = 0;
            nbBait = 0;
            nbBaitMax += 10;
            max += GenerateRandomNumber(0, 1);
        }

        int iaX = newCursorIA.GetX();
        int iaY = newCursorIA.GetY();

        int pBaitX = pX + baitX;
        int pBaitY = pY + baitY;

        int iaRight = iaX + 1;
        int iaLeft = iaX - 1;
        int iaDown = iaY + 1;
        int iaUp = iaY - 1;

        bool haveMove = false;

        haveMove = IAMoveX(haveMove, &newCursorIA, iaX < pBaitX, iaRight, '*');
        haveMove = IAMoveX(haveMove, &newCursorIA, iaX > pBaitX, iaLeft, '*');
        haveMove = IAMoveY(haveMove, &newCursorIA, iaY < pBaitY, iaDown, '*');
        haveMove = IAMoveY(haveMove, &newCursorIA, iaY > pBaitY, iaUp, '*');

        if (haveMove == false)
        {
            switch (GenerateRandomNumber(0, 8))
            {
            case 0:
                haveMove = IAMoveX(haveMove, &newCursorIA, iaX == pBaitX, iaRight, '*');
                if (haveMove) { baitX++; break; }
            case 1:
                haveMove = IAMoveX(haveMove, &newCursorIA, iaX == pBaitX, iaLeft, '*');
                if (haveMove) { baitX--; break; }
            case 2:
                haveMove = IAMoveY(haveMove, &newCursorIA, iaY == pBaitY, iaDown, '*');
                if (haveMove) { baitY++; break; }
            case 3:
                haveMove = IAMoveY(haveMove, &newCursorIA, iaY == pBaitY, iaUp, '*');
                if (haveMove) { baitY--; break; }
            case 4:
                haveMove = IAMoveX(haveMove, &newCursorIA, iaX == pBaitX, iaRight, '*');
                if (haveMove) { baitX++; break; }
            case 5:
                haveMove = IAMoveX(haveMove, &newCursorIA, iaX == pBaitX, iaLeft, '*');
                if (haveMove) { baitX--; break; }
            case 6:
                haveMove = IAMoveY(haveMove, &newCursorIA, iaY == pBaitY, iaDown, '*');
                if (haveMove) { baitY++; break; }
            case 7:
                haveMove = IAMoveY(haveMove, &newCursorIA, iaY == pBaitY, iaUp, '*');
                if (haveMove) { baitY--; break; }
            case 8 :
                if (haveMove == false) 
                {
                    int rand = GenerateRandomNumber(-1, 1);
                    if (rand == 1)
                    {
                        baitX += GenerateRandomNumber(-max, max);
                        break;
                    }
                    else if (rand == -1)
                    {
                        baitY += GenerateRandomNumber(-max, max);
                        break;
                    }
                    if (rand == 0)
                    {
                        baitX += GenerateRandomNumber(-max, max);
                        baitY += GenerateRandomNumber(-max, max);
                        break;
                    }
                }
            }
            nbBait++;
        }

        TILES[newCursorIA.GetX()][newCursorIA.GetY()] = '-';
        Draw();
        TILES[newCursorIA.GetX()][newCursorIA.GetY()] = '+';
    }

    return newCursorIA;
}*/

Vector2 IA4(Vector2* cursorIA, Vector2 cursorPlayer, char path, char trail, char player, char ia, char obstacle)
{
    Vector2 newCursorIA = *cursorIA;

    float dis_m = abs(newCursorIA.GetX() - cursorPlayer.GetX()) + abs(newCursorIA.GetY() - cursorPlayer.GetX());

    int pX = cursorPlayer.GetX();
    int pY = cursorPlayer.GetY();

    int baitX = 0;
    int baitY = 0;

    int nbBait = 0;
    int nbBaitMax = 10;

    int min = -1;
    int max = 1;

    while (newCursorIA.GetX() != pX || newCursorIA.GetY() != pY)
    {
        if (nbBait > nbBaitMax)
        {
            baitX = 0;
            baitY = 0;
            nbBait = 0;
            nbBaitMax += 10;
            max += GenerateRandomNumber(0, 1);
        }

        int iaX = newCursorIA.GetX();
        int iaY = newCursorIA.GetY();

        int pBaitX = pX + baitX;
        int pBaitY = pY + baitY;

        int iaRight = iaX + 1;
        int iaLeft = iaX - 1;
        int iaDown = iaY + 1;
        int iaUp = iaY - 1;

        bool haveMove = false;

        haveMove = IAMoveX(haveMove, &newCursorIA, iaX < pBaitX, iaRight, path, trail, player);
        haveMove = IAMoveX(haveMove, &newCursorIA, iaX > pBaitX, iaLeft, path, trail, player);
        haveMove = IAMoveY(haveMove, &newCursorIA, iaY < pBaitY, iaDown, path, trail, player);
        haveMove = IAMoveY(haveMove, &newCursorIA, iaY > pBaitY, iaUp, path, trail, player);

        if (haveMove == false)
        {
            switch (GenerateRandomNumber(0, 8))
            {
            case 0:
                haveMove = IAMoveX(haveMove, &newCursorIA, iaX == pBaitX, iaRight, path, trail, player);
                if (haveMove) { baitX++; break; }
            case 1:
                haveMove = IAMoveX(haveMove, &newCursorIA, iaX == pBaitX, iaLeft, path, trail, player);
                if (haveMove) { baitX--; break; }
            case 2:
                haveMove = IAMoveY(haveMove, &newCursorIA, iaY == pBaitY, iaDown, path, trail, player);
                if (haveMove) { baitY++; break; }
            case 3:
                haveMove = IAMoveY(haveMove, &newCursorIA, iaY == pBaitY, iaUp, path, trail, player);
                if (haveMove) { baitY--; break; }
            case 4:
                haveMove = IAMoveX(haveMove, &newCursorIA, iaX == pBaitX, iaRight, path, trail, player);
                if (haveMove) { baitX++; break; }
            case 5:
                haveMove = IAMoveX(haveMove, &newCursorIA, iaX == pBaitX, iaLeft, path, trail, player);
                if (haveMove) { baitX--; break; }
            case 6:
                haveMove = IAMoveY(haveMove, &newCursorIA, iaY == pBaitY, iaDown, path, trail, player);
                if (haveMove) { baitY++; break; }
            case 7:
                haveMove = IAMoveY(haveMove, &newCursorIA, iaY == pBaitY, iaUp, path, trail, player);
                if (haveMove) { baitY--; break; }
            case 8:
                if (haveMove == false)
                {
                    int rand = GenerateRandomNumber(-1, 1);
                    if (rand == 1)
                    {
                        baitX += GenerateRandomNumber(-max, max);
                        break;
                    }
                    else if (rand == -1)
                    {
                        baitY += GenerateRandomNumber(-max, max);
                        break;
                    }
                    if (rand == 0)
                    {
                        baitX += GenerateRandomNumber(-max, max);
                        baitY += GenerateRandomNumber(-max, max);
                        break;
                    }
                }
            }
            nbBait++;
        }

        TILES[newCursorIA.GetX()][newCursorIA.GetY()] = '-';
        Draw(player, ia, obstacle);
        TILES[newCursorIA.GetX()][newCursorIA.GetY()] = trail;
    }

    return newCursorIA;
}

int TileHaveNbChara(char chara1)
{
    int nb = 0;
    int lenghti = TILES.size();
    for (int i = 0; i < lenghti; i++)
    {
        int lenghtj = TILES[i].size();
        for (int j = 0; j < lenghtj; j++)
        {
            if (TILES[i][j] == chara1)
            {
                nb++;
            }
        }
    }
    return nb;
}

void InputIA(Vector2* cursorIA, Vector2 cursorPlayer, char chara, char player, char ia, char obstacle)
{
    Vector2 newCursorIA = *cursorIA;
    
    switch (chara)
    {
    case ' ':
        int nb = 1;
        std::vector<char> trail = { ' ', '+', '1', '2', '3', '4', '5', '6', '7', '8', '9' };
        /*switch (MODE1)
        {
        case 1:
            newCursorIA = IA1(cursorIA, cursorPlayer);
            break;
        case 2:
            newCursorIA = IA2(cursorIA, cursorPlayer);
            break;
        case 3:
            newCursorIA = IA3(cursorIA, cursorPlayer, trail[nb - 1], trail[nb], player, ia, obstacle);
            break;
        case 4:
            newCursorIA = IA4(cursorIA, cursorPlayer, trail[nb - 1], trail[nb], player, ia, obstacle);
            break;
        }
        nb += 1;*/
        while (true)
        {
            if (MODE1 == 1) { newCursorIA = IA1(cursorIA, cursorPlayer); }
            if (MODE1 == 2) { newCursorIA = IA2(cursorIA, cursorPlayer); }
            if (MODE1 == 3) { newCursorIA = IA3(cursorIA, cursorPlayer, trail[nb - 1], trail[nb], player, ia, obstacle); }
            if (MODE1 == 4) { newCursorIA = IA4(cursorIA, cursorPlayer, trail[nb - 1], trail[nb], player, ia, obstacle); }
            nb++;
            if (TileHaveNbChara(trail[nb - 1]) == 0 || nb > trail.size())
            {
                break;
            }
        }
        break;
    }

    TILES[cursorIA->GetX()][cursorIA->GetY()] = '+';
    TILES[newCursorIA.GetX()][newCursorIA.GetY()] = ia;
    *cursorIA = newCursorIA;
}

void Input(Vector2* cursorPlayer, Vector2* cursorIA, char player, char ia, char obstacle)
{
    int chara = _getch();
    //A=97 W=119 S=115 D=100 ENTER=10
    if (chara != 'r')
    {
        InputPlayer(cursorPlayer, chara, player, obstacle);
        InputIA(cursorIA, *cursorPlayer, chara, player, ia, obstacle);
    }
    else
    {
        TILES = StartTab();
        InputPlayer(cursorPlayer, chara, player, obstacle);
        InputIA(cursorIA, *cursorPlayer, chara, player, ia, obstacle);
    }
}

void Draw(char player, char ia, char obstacle)
{
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    system("cls");
    int lenghti = TILES.size();
    for (int i = 0; i < lenghti; i++)
    {
        int lenghtj = TILES[i].size();
        for (int j = 0; j < lenghtj; j++)
        {
            std::cout << "[";
            char c = TILES[i][j];
            if (c == '*')
            {
                SetConsoleTextAttribute(console, (FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN) * 2.f );
                std::cout << c;
                SetConsoleTextAttribute(console, FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN);
            }
            else if (c == '+')
            {
                SetConsoleTextAttribute(console,FOREGROUND_GREEN);
                std::cout << c;
                SetConsoleTextAttribute(console, FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN);
            }
            else if (c == '1' || c == '2' || c == '3' || c == '4' || c == '5' || c == '6' || c == '7' || c == '8' || c == '9')
            {
                SetConsoleTextAttribute(console, FOREGROUND_GREEN * ((int)c/10));
                std::cout << c;
                SetConsoleTextAttribute(console, FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN);
            }
            else if (c == '-' || c == ia)
            {
                SetConsoleTextAttribute(console,FOREGROUND_RED);
                std::cout << c;
                SetConsoleTextAttribute(console, FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN);
            }
            else if (c == player)
            {
                SetConsoleTextAttribute(console, FOREGROUND_BLUE | FOREGROUND_RED);
                std::cout << c;
                SetConsoleTextAttribute(console, FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN);
            }
            else
            {
                std::cout << c;
            }
            std::cout << "]";
        }
        std::cout << std::endl;
    }
}

int main()
{
    srand(0);
    TILES = StartTab();
    Vector2 cursorPlayer(8, 5);
    TILES[cursorPlayer.GetX()][cursorPlayer.GetY()] = '0';
    Vector2 cursorIA(5, 5);
    TILES[cursorIA.GetX()][cursorIA.GetY()] = 'x';
    bool end = false;

    char obstacle = '*';
    char player = '0';
    char ia = 'x';
    while (true)
    {
        Draw(player, ia, obstacle);

        Input(&cursorPlayer, &cursorIA, player, ia, obstacle);


        if (end)
        {
            break;
        }
    }

    std::cout << "Hello World!\n";
}

