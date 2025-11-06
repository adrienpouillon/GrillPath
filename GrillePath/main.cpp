// GrillePath.cpp : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.
//

#include <iostream>
#include <vector>
#include <cstdio>
#include <conio.h>
#include <windows.h>
#include <queue>

#define DEFAULT_WEIGHT 0

void DrawChara(char player, char ia, char obstacle);
void DrawWeight(char player, char ia, char obstacle);

struct Cell
{
    char mChara;
    int mWeight;
    Vector2 mXY;
    bool mVisited;
    Cell* mCallMe;
    int mDisStart;
    int mDisEnd;

    Cell() { mChara = ' '; mWeight = 0; mXY = Vector2(0, 0);  mVisited = false; mCallMe = nullptr; }
    Cell(char chara, Vector2 xy) { mChara = chara; mWeight = 0; mXY = xy;  mVisited = false; mCallMe = nullptr; }
    Cell(char chara, Vector2 xy, bool visited, Cell* callMe) { mChara = chara; mXY = xy; mVisited = visited; mCallMe = callMe; }
    Cell(char chara, int weight, Vector2 xy) { mChara = chara; mWeight = weight; mXY = xy; }
    Cell(char chara, int weight, Vector2 xy, bool visited, Cell* callMe) { mChara = chara; mWeight = weight; mXY = xy; mVisited = visited; mCallMe = callMe; }
    
    void SetChara(char chara) { mChara = chara; }
    char GetChara() { return mChara; }

    void SetWeight(int weight) { mWeight = weight; }
    int GetWeight() { return mWeight; }
    void IncreaseWeight() { mWeight++; }
    void DecreaseWeight() { mWeight--; }

    void SetXY(Vector2 xy) { mXY = xy; }
    Vector2 GetXY() { return mXY; }

    void SetVisited(bool visited) { mVisited = visited; }
    bool GetVisited() { return mVisited; }

    void SetCallMe(Cell* callMe) { mCallMe = callMe; }
    Cell* GetCallMe() { return mCallMe; }

    void SetDisStart(int dis) { mDisStart = dis; }
    int GetDisStart() { return mDisStart; }

    void SetDisEnd(int dis) { mDisEnd = dis; }
    int GetDisEnd() { return mDisEnd; }
};

struct Vector2
{
    int mX;
    int mY;
    Vector2() { mX = 0; mY = 0; }
    Vector2(int x, int y) { mX = x; mY = y; }
    void SetX(int x) { mX = x; }
    void SetY(int y) { mY = y; }
    void SetXY(int x, int y) { mX = x; mY = y; }
    int GetX() { return mX; }
    int GetY() { return mY; }
};

struct CompareBFS
{
    bool operator()(Cell* a, Cell* b)
    {
        return a->GetDisStart() > b->GetDisStart();
    }
};

struct CompareASTAR
{
    bool operator()(Cell* a, Cell* b)
    {
        return a->GetDisStart() + a->GetDisEnd() > b->GetDisStart() + b->GetDisEnd();
    }
};

std::vector< std::vector<Cell>> TILES;
//std::vector< std::vector<char>> TILES;

int MODE1 = 6;

int GenerateRandomNumber(int min, int max)
{
    int range = max - min + 1;
    int value = rand() % range + min;
    return value;
}

std::vector< std::vector<Cell>> StartTab()
{
    std::vector< std::vector<char>> charas{
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
    std::vector< std::vector<Cell>> tiles = std::vector< std::vector<Cell>>();
    int lenghti = charas.size();
    for (int i = 0; i < lenghti; i++)
    {
        tiles.push_back(std::vector<Cell>());
        int lenghtj = charas[i].size();
        for (int j = 0; j < lenghtj; j++)
        {
            char c = charas[i][j];
            if (c == '*')
            {
                tiles[i].push_back(Cell(c, 1000, Vector2(i,j)));
            }
            else
            {
                tiles[i].push_back(Cell(c, DEFAULT_WEIGHT, Vector2(i, j)));
            }
        }
    }
    return tiles;
}

void LowerAllWeight(int thresholdWeight)
{
    int lenghti = TILES.size();
    for (int i = 0; i < lenghti; i++)
    {
        int lenghtj = TILES[i].size();
        for (int j = 0; j < lenghtj; j++)
        {
            TILES[i][j].DecreaseWeight();
            int w = TILES[i][j].GetWeight();
            if (w > thresholdWeight)
            {
                TILES[i][j].SetWeight(w * -1);
            }
        }
    }
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

    if (TILES[newCursorPlayer.GetX()][newCursorPlayer.GetY()].GetChara() == obstacle)
    {
        newCursorPlayer = *cursorPlayer;
    }

    TILES[cursorPlayer->GetX()][cursorPlayer->GetY()].SetChara(' ');
    Cell* cellPlayer = &TILES[newCursorPlayer.GetX()][newCursorPlayer.GetY()];
    cellPlayer->SetChara(player);
    cellPlayer->SetWeight(0);

    *cursorPlayer = newCursorPlayer;
}

Vector2 IA1(Vector2* cursorIA, Vector2 cursorPlayer)
{
    Vector2 newCursorIA = *cursorIA;


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

        TILES[newCursorIA.GetX()][newCursorIA.GetY()].SetChara('+');
    }

    return newCursorIA;
}

bool IAMoveX(bool haveMove, Vector2* newCursorIA, bool condition, int iaDir, char obstacle)
{
    if (haveMove == false && TILES[iaDir][newCursorIA->GetY()].GetChara() != obstacle)
    {
        if (condition)
        {
            newCursorIA->SetX(iaDir);
            TILES[iaDir][newCursorIA->GetY()].IncreaseWeight();
            return true;
        }
    }
    return haveMove;
}

bool IAMoveY(bool haveMove, Vector2* newCursorIA, bool condition, int iaDir, char obstacle)
{
    if (haveMove == false && TILES[newCursorIA->GetX()][iaDir].GetChara() != obstacle)
    {
        if (condition)
        {
            newCursorIA->SetY(iaDir);
            TILES[newCursorIA->GetX()][iaDir].IncreaseWeight();
            return true;
        }
    }
    return haveMove;
}

bool IAMoveX(bool haveMove, Vector2* newCursorIA, bool condition, int iaDir, char path, char trail, char player)
{
    if (haveMove == false && (TILES[iaDir][newCursorIA->GetY()].GetChara() == path
                          || TILES[iaDir][newCursorIA->GetY()].GetChara() == trail
                          || TILES[iaDir][newCursorIA->GetY()].GetChara() == player))
    {
        if (condition)
        {
            newCursorIA->SetX(iaDir);
            TILES[iaDir][newCursorIA->GetY()].IncreaseWeight();
            return true;
        }
    }
    return haveMove;
}

bool IAMoveY(bool haveMove, Vector2* newCursorIA, bool condition, int iaDir, char path, char trail, char player)
{
    if (haveMove == false && (TILES[newCursorIA->GetX()][iaDir].GetChara() == path
                          || TILES[newCursorIA->GetX()][iaDir].GetChara() == trail
                          || TILES[newCursorIA->GetX()][iaDir].GetChara() == player))
    {
        if (condition)
        {
            newCursorIA->SetY(iaDir);
            TILES[newCursorIA->GetX()][iaDir].IncreaseWeight();
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

        TILES[newCursorIA.GetX()][newCursorIA.GetY()].SetChara('-');
        DrawChara(player, ia, obstacle);
        TILES[newCursorIA.GetX()][newCursorIA.GetY()].SetChara(trail);
    }

    return newCursorIA;
}

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

        TILES[newCursorIA.GetX()][newCursorIA.GetY()].SetChara('-');
        DrawChara(player, ia, obstacle);
        TILES[newCursorIA.GetX()][newCursorIA.GetY()].SetChara(trail);
    }

    return newCursorIA;
}

int LessWeight(int w1, int w2, int w3, int w4)
{
    // Remplacer les valeurs négatives par un nombre très grand (ignorées)
    if (w1 < 0) w1 = INT_MAX;
    if (w2 < 0) w2 = INT_MAX;
    if (w3 < 0) w3 = INT_MAX;
    if (w4 < 0) w4 = INT_MAX;

    int minW = (std::min)((std::min)(w1, w2), (std::min)(w3, w4));

    if (minW == INT_MAX)
        return 0; // aucun poids valide

    int result = 0;
    if (w1 == minW) result = result * 10 + 1;
    if (w2 == minW) result = result * 10 + 2;
    if (w3 == minW) result = result * 10 + 3;
    if (w4 == minW) result = result * 10 + 4;

    return result;
}

Vector2 IA5(Vector2* cursorIA, Vector2 cursorPlayer, char path, char trail, char player, char ia, char obstacle)
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

        int weightRight = TILES[iaRight][newCursorIA.GetY()].GetWeight();
        int weightLeft = TILES[iaLeft][newCursorIA.GetY()].GetWeight();
        int weightDown = TILES[newCursorIA.GetX()][iaDown].GetWeight();
        int weightUp = TILES[newCursorIA.GetX()][iaUp].GetWeight();
        
        int lessWeight = LessWeight(weightRight, weightLeft, weightDown, weightUp);
        switch (lessWeight)
        {
        case 1:
            haveMove = IAMoveX(haveMove, &newCursorIA, iaX < pBaitX, iaRight, path, trail, player);
            if (haveMove) { baitX++; }
            break;
        case 2:
            haveMove = IAMoveX(haveMove, &newCursorIA, iaX > pBaitX, iaLeft, path, trail, player);
            if (haveMove) { baitX--; }
            break;
        case 3:
            haveMove = IAMoveY(haveMove, &newCursorIA, iaY < pBaitY, iaDown, path, trail, player);
            if (haveMove) { baitY++; }
            break;
        case 4:
            haveMove = IAMoveY(haveMove, &newCursorIA, iaY > pBaitY, iaUp, path, trail, player);
            if (haveMove) { baitY--; }
            break;
        case 12:
            haveMove = IAMoveX(haveMove, &newCursorIA, iaX < pBaitX, iaRight, path, trail, player);
            haveMove = IAMoveX(haveMove, &newCursorIA, iaX > pBaitX, iaLeft, path, trail, player);
            break;
        case 13:
            haveMove = IAMoveX(haveMove, &newCursorIA, iaX < pBaitX, iaRight, path, trail, player);
            haveMove = IAMoveY(haveMove, &newCursorIA, iaY < pBaitY, iaDown, path, trail, player);
            break;
        case 14:
            haveMove = IAMoveX(haveMove, &newCursorIA, iaX < pBaitX, iaRight, path, trail, player);
            haveMove = IAMoveY(haveMove, &newCursorIA, iaY > pBaitY, iaUp, path, trail, player);
            break;
        case 23:
            haveMove = IAMoveX(haveMove, &newCursorIA, iaX > pBaitX, iaLeft, path, trail, player);
            haveMove = IAMoveY(haveMove, &newCursorIA, iaY < pBaitY, iaDown, path, trail, player);
            break;
        case 24:
            haveMove = IAMoveX(haveMove, &newCursorIA, iaX > pBaitX, iaLeft, path, trail, player);
            haveMove = IAMoveY(haveMove, &newCursorIA, iaY > pBaitY, iaUp, path, trail, player);
            break;
        case 34:
            haveMove = IAMoveY(haveMove, &newCursorIA, iaY < pBaitY, iaDown, path, trail, player);
            haveMove = IAMoveY(haveMove, &newCursorIA, iaY > pBaitY, iaUp, path, trail, player);
            break;
        case 123:
            haveMove = IAMoveX(haveMove, &newCursorIA, iaX < pBaitX, iaRight, path, trail, player);
            haveMove = IAMoveX(haveMove, &newCursorIA, iaX > pBaitX, iaLeft, path, trail, player);
            haveMove = IAMoveY(haveMove, &newCursorIA, iaY < pBaitY, iaDown, path, trail, player);
            break;
        case 124:
            haveMove = IAMoveX(haveMove, &newCursorIA, iaX < pBaitX, iaRight, path, trail, player);
            haveMove = IAMoveX(haveMove, &newCursorIA, iaX > pBaitX, iaLeft, path, trail, player);
            haveMove = IAMoveY(haveMove, &newCursorIA, iaY > pBaitY, iaUp, path, trail, player);
            break;
        case 134:
            haveMove = IAMoveX(haveMove, &newCursorIA, iaX < pBaitX, iaRight, path, trail, player);
            haveMove = IAMoveY(haveMove, &newCursorIA, iaY < pBaitY, iaDown, path, trail, player);
            haveMove = IAMoveY(haveMove, &newCursorIA, iaY > pBaitY, iaUp, path, trail, player);
            break;
        case 234:
            haveMove = IAMoveX(haveMove, &newCursorIA, iaX > pBaitX, iaLeft, path, trail, player);
            haveMove = IAMoveY(haveMove, &newCursorIA, iaY < pBaitY, iaDown, path, trail, player);
            haveMove = IAMoveY(haveMove, &newCursorIA, iaY > pBaitY, iaUp, path, trail, player);
            break;
        case 1234:
            haveMove = IAMoveX(haveMove, &newCursorIA, iaX < pBaitX, iaRight, path, trail, player);
            haveMove = IAMoveX(haveMove, &newCursorIA, iaX > pBaitX, iaLeft, path, trail, player);
            haveMove = IAMoveY(haveMove, &newCursorIA, iaY < pBaitY, iaDown, path, trail, player);
            haveMove = IAMoveY(haveMove, &newCursorIA, iaY > pBaitY, iaUp, path, trail, player);
            break;
        }

        if (haveMove == false)
        {
            haveMove = IAMoveX(haveMove, &newCursorIA, iaX < pBaitX && weightRight > -1, iaRight, path, trail, player);
            haveMove = IAMoveX(haveMove, &newCursorIA, iaX > pBaitX && weightLeft > -1, iaLeft, path, trail, player);
            haveMove = IAMoveY(haveMove, &newCursorIA, iaY < pBaitY && weightDown > -1, iaDown, path, trail, player);
            haveMove = IAMoveY(haveMove, &newCursorIA, iaY > pBaitY && weightUp > -1, iaUp, path, trail, player);
        }

        if (haveMove == false)
        {
            switch (GenerateRandomNumber(0, 8))
            {
            case 0:
                haveMove = IAMoveX(haveMove, &newCursorIA, iaX == pBaitX && weightRight > -1, iaRight, path, trail, player);
                if (haveMove) { baitX++; break; }
            case 1:
                haveMove = IAMoveX(haveMove, &newCursorIA, iaX == pBaitX && weightLeft > -1, iaLeft, path, trail, player);
                if (haveMove) { baitX--; break; }
            case 2:
                haveMove = IAMoveY(haveMove, &newCursorIA, iaY == pBaitY && weightDown > -1, iaDown, path, trail, player);
                if (haveMove) { baitY++; break; }
            case 3:
                haveMove = IAMoveY(haveMove, &newCursorIA, iaY == pBaitY && weightUp > -1, iaUp, path, trail, player);
                if (haveMove) { baitY--; break; }
            case 4:
                haveMove = IAMoveX(haveMove, &newCursorIA, iaX == pBaitX && weightRight > -1, iaRight, path, trail, player);
                if (haveMove) { baitX++; break; }
            case 5:
                haveMove = IAMoveX(haveMove, &newCursorIA, iaX == pBaitX && weightLeft > -1, iaLeft, path, trail, player);
                if (haveMove) { baitX--; break; }
            case 6:
                haveMove = IAMoveY(haveMove, &newCursorIA, iaY == pBaitY && weightDown > -1, iaDown, path, trail, player);
                if (haveMove) { baitY++; break; }
            case 7:
                haveMove = IAMoveY(haveMove, &newCursorIA, iaY == pBaitY && weightUp > -1, iaUp, path, trail, player);
                if (haveMove) { baitY--; break; }
            case 8:
                if (haveMove == false)
                {
                    int rand = GenerateRandomNumber(-1, 1);
                    switch (lessWeight)
                    {
                    case 1:
                        baitX++;
                        break;
                    case 2:
                        baitX--;
                        break;
                    case 3:
                        baitY++;
                        break;
                    case 4:
                        baitY--;
                        break;
                    case 12:
                        baitX++;
                        baitX--;
                        break;
                    case 13:
                        baitX++;
                        baitY++;
                        break;
                    case 14:
                        baitX++;
                        baitY--;
                        break;
                    case 23:
                        baitX--;
                        baitY++;
                        break;
                    case 24:
                        baitX--;
                        baitY--;
                        break;
                    case 34:
                        baitY++;
                        baitY--;
                        break;
                    case 123:
                        baitX++;
                        baitX--;
                        baitY++;
                        break;
                    case 124:
                        baitX++;
                        baitX--;
                        baitY--;
                        break;
                    case 134:
                        baitX++;
                        baitY++;
                        baitY--;
                        break;
                    case 234:
                        baitX--;
                        baitY++;
                        baitY--;
                        break;
                    case 1234:
                        baitX++;
                        baitX--;
                        baitY++;
                        baitY--;
                        break;
                    }
                }
            }
            nbBait++;
        }

        TILES[newCursorIA.GetX()][newCursorIA.GetY()].SetChara('-');
        DrawWeight(player, ia, obstacle);
        TILES[newCursorIA.GetX()][newCursorIA.GetY()].SetChara(trail);
    }

    return newCursorIA;
}

int DisToTarget(Cell* current, Cell* end)
{
    Vector2 currentPos = current->GetXY();
    Vector2 endPos = end->GetXY();
    return abs(currentPos.GetX() - endPos.GetY()) + abs(currentPos.GetY() - endPos.GetY());
}

Vector2 IA6(Vector2* cursorIA, Vector2 cursorPlayer, char path, char trail, char player, char ia, char obstacle)
{
    Vector2 newCursorIA = *cursorIA;

    std::priority_queue<Cell*, std::vector<Cell*>, CompareBFS> queue = std::priority_queue<Cell*, std::vector<Cell*>, CompareBFS>();
    Cell* cellStart = &TILES[cursorIA->GetX()][cursorIA->GetY()];
    Cell* cellEnd = &TILES[cursorPlayer.GetX()][cursorPlayer.GetY()];
    cellStart->SetDisStart(0);
    cellStart->SetDisEnd(DisToTarget(cellStart, cellEnd));
    queue.push(cellStart);


    while ((int)queue.size() != 0)
    {
        Cell* cellIA = queue.top();
        queue.pop();

        if (cellIA->GetVisited() == true)
        {
            continue;
        }

        newCursorIA = cellIA->GetXY();
        if (newCursorIA.GetX() == cursorPlayer.GetX() && newCursorIA.GetY() == cursorPlayer.GetY())
        {
            return newCursorIA;
        }

        cellIA->SetVisited(true);

        int iaX = newCursorIA.GetX();
        int iaY = newCursorIA.GetY();

        int iaRight = iaX + 1;
        int iaLeft = iaX - 1;
        int iaDown = iaY + 1;
        int iaUp = iaY - 1;

        int disStart = cellIA->GetDisStart();

        std::vector<Cell*> neighbor = std::vector<Cell*>();

        neighbor.push_back(&TILES[iaRight][iaY]);
        neighbor.push_back(&TILES[iaLeft][iaY]);
        neighbor.push_back(&TILES[iaX][iaDown]);
        neighbor.push_back(&TILES[iaX][iaUp]);

        for (Cell* cellN : neighbor)
        {
            if (cellN->GetVisited() == false && (cellN->GetChara() == path || cellN->GetChara() == trail || cellN->GetChara() == player))
            {
                cellN->SetDisStart(disStart + 1);
                cellN->SetDisEnd(DisToTarget(cellN, cellEnd));
                cellN->SetCallMe(cellIA);

                queue.push(cellN);
            }
        }
        TILES[newCursorIA.GetX()][newCursorIA.GetY()].SetChara('-');
        DrawChara(player, ia, obstacle);
        TILES[newCursorIA.GetX()][newCursorIA.GetY()].SetChara(trail);
    }

    return Vector2(-1, -1);
}

void IAPath(Vector2* cursorIA, Vector2 cursorPlayer, char trail, char player, char ia, char obstacle)
{
    Vector2 newCursorIA = *cursorIA;
    Cell* cellEnd = &TILES[cursorPlayer.GetX()][cursorPlayer.GetY()];
    Cell* cellBegin = &TILES[cursorIA->GetX()][cursorIA->GetY()];
    while (cellEnd != cellBegin)
    {
        newCursorIA = cellEnd->GetXY();

        cellEnd = cellEnd->GetCallMe();

        TILES[newCursorIA.GetX()][newCursorIA.GetY()].SetChara('-');
        DrawChara(player, ia, obstacle);
        TILES[newCursorIA.GetX()][newCursorIA.GetY()].SetChara(trail);
    }
}

Vector2 IA7(Vector2* cursorIA, Vector2 cursorPlayer, char path, char trail, char player, char ia, char obstacle)
{
    Vector2 newCursorIA = *cursorIA;

    std::priority_queue<Cell*, std::vector<Cell*>, CompareASTAR> queue = std::priority_queue<Cell*, std::vector<Cell*>, CompareASTAR>();
    Cell* cellStart = &TILES[cursorIA->GetX()][cursorIA->GetY()];
    Cell* cellEnd = &TILES[cursorPlayer.GetX()][cursorPlayer.GetY()];
    cellStart->SetDisStart(0);
    cellStart->SetDisEnd(DisToTarget(cellStart, cellEnd));
    queue.push(cellStart);


    while ((int)queue.size() != 0)
    {
        Cell* cellIA = queue.top();
        queue.pop();

        if (cellIA->GetVisited() == true)
        {
            continue;
        }

        newCursorIA = cellIA->GetXY();
        if (newCursorIA.GetX() == cursorPlayer.GetX() && newCursorIA.GetY() == cursorPlayer.GetY())
        {
            return newCursorIA;
        }

        cellIA->SetVisited(true);

        int iaX = newCursorIA.GetX();
        int iaY = newCursorIA.GetY();

        int iaRight = iaX + 1;
        int iaLeft = iaX - 1;
        int iaDown = iaY + 1;
        int iaUp = iaY - 1;

        int disStart = cellIA->GetDisStart();

        std::vector<Cell*> neighbor = std::vector<Cell*>();

        neighbor.push_back(&TILES[iaRight][iaY]);
        neighbor.push_back(&TILES[iaLeft][iaY]);
        neighbor.push_back(&TILES[iaX][iaDown]);
        neighbor.push_back(&TILES[iaX][iaUp]);

        for (Cell* cellN : neighbor)
        {
            if (cellN->GetVisited() == false && (cellN->GetChara() == path || cellN->GetChara() == trail || cellN->GetChara() == player))
            {
                cellN->SetDisStart(disStart + 1);
                cellN->SetDisEnd(DisToTarget(cellN, cellEnd));
                cellN->SetCallMe(cellIA);

                queue.push(cellN);
            }
        }
        TILES[newCursorIA.GetX()][newCursorIA.GetY()].SetChara('-');
        DrawChara(player, ia, obstacle);
        TILES[newCursorIA.GetX()][newCursorIA.GetY()].SetChara(trail);
    }

    return Vector2(-1, -1);
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
            if (TILES[i][j].GetChara() == chara1)
            {
                nb++;
            }
        }
    }
    return nb;
}

int GetMaxWeight()
{
    int bigWeight = 0;
    int lenghti = TILES.size();
    for (int i = 0; i < lenghti; i++)
    {
        int lenghtj = TILES[i].size();
        for (int j = 0; j < lenghtj; j++)
        {
            int weight = TILES[i][j].GetWeight();
            if (weight > bigWeight)
            {
                bigWeight = weight;
            }
        }
    }
    return bigWeight;
}

void InputIA(Vector2* cursorIA, Vector2 cursorPlayer, char chara, char player, char ia, char obstacle)
{
    Vector2 newCursorIA = *cursorIA;
    
    
    switch (chara)
    {
    case ' ':
        TILES = StartTab();
        TILES[cursorPlayer.GetX()][cursorPlayer.GetY()].SetChara(player);
        TILES[cursorIA->GetX()][cursorIA->GetY()].SetChara(ia);

        int nb = 1;
        std::vector<char> trail = { ' ', '+', '1', '2', '3', '4', '5', '6', '7', '8', '9' };
        while (true)
        {
            if (MODE1 == 1) { newCursorIA = IA1(cursorIA, cursorPlayer); }
            if (MODE1 == 2) { newCursorIA = IA2(cursorIA, cursorPlayer); }
            if (MODE1 == 3) { newCursorIA = IA3(cursorIA, cursorPlayer, trail[nb - 1], trail[nb], player, ia, obstacle); }
            if (MODE1 == 4) { newCursorIA = IA4(cursorIA, cursorPlayer, trail[nb - 1], trail[nb], player, ia, obstacle); }
            if (MODE1 == 5) { newCursorIA = IA5(cursorIA, cursorPlayer, trail[nb - 1], trail[nb], player, ia, obstacle); }
            if (MODE1 == 6) { newCursorIA = IA6(cursorIA, cursorPlayer, trail[nb - 1], trail[nb], player, ia, obstacle); IAPath(cursorIA, cursorPlayer, trail[nb + 1], player, ia, obstacle); }
            if (MODE1 == 7) { newCursorIA = IA6(cursorIA, cursorPlayer, trail[nb - 1], trail[nb], player, ia, obstacle); IAPath(cursorIA, cursorPlayer, trail[nb + 1], player, ia, obstacle); }

            if (MODE1 < 6)
            {
                int nb_former_trail = TileHaveNbChara(trail[nb - 1]);

                int bigWeight = GetMaxWeight();

                if ((nb_former_trail == 0 && bigWeight < 20) || nb > trail.size())
                {
                    break;
                }
                int weightToDesactive = 20 + (nb * 10);
                LowerAllWeight(weightToDesactive);
            }
            else
            {
                Cell* cellIA = &TILES[cursorIA->GetX()][cursorIA->GetY()];
                Cell* cellPlayer = &TILES[cursorIA->GetX()][cursorIA->GetY()];
                if (newCursorIA.GetX() != -1 && newCursorIA.GetY() != -1)
                {
                    cellPlayer->SetChara('@');
                    cellIA->SetChara(ia);
                }
                else
                {
                    cellPlayer->SetChara(ia);
                }
            }
            nb++;
        }
        break;
    }

    TILES[cursorIA->GetX()][cursorIA->GetY()].SetChara('@');
    TILES[newCursorIA.GetX()][newCursorIA.GetY()].SetChara(ia);
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

float HighWeight(int weight)
{
    if ((weight > 20 && weight < 900) || (weight < 0))
    {
        return 0.1f;
    }
    return 1.f;
}

void DrawChara(char player, char ia, char obstacle)
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
            Cell cell = TILES[i][j];
            char c = cell.GetChara();
            int w = cell.GetWeight();
            if (c == '*')
            {
                SetConsoleTextAttribute(console, ((FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN) * 2.f) * HighWeight(w));
                std::cout << c;
                SetConsoleTextAttribute(console, FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN);
            }
            else if (c == '+')
            {
                SetConsoleTextAttribute(console,(FOREGROUND_GREEN) * HighWeight(w));
                std::cout << c;
                SetConsoleTextAttribute(console, FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN);
            }
            else if (c == '1' || c == '2' || c == '3' || c == '4' || c == '5' || c == '6' || c == '7' || c == '8' || c == '9')
            {
                SetConsoleTextAttribute(console, (FOREGROUND_GREEN * ((int)c/10) | FOREGROUND_RED * ((int)c / 20) | FOREGROUND_BLUE * ((int)c / 30)) * HighWeight(w));
                std::cout << c;
                SetConsoleTextAttribute(console, FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN);
            }
            else if (c == '-' || c == ia)
            {
                SetConsoleTextAttribute(console,(FOREGROUND_RED) * HighWeight(w));
                std::cout << c;
                SetConsoleTextAttribute(console, FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN);
            }
            else if (c == player)
            {
                SetConsoleTextAttribute(console, (FOREGROUND_BLUE | FOREGROUND_RED) * HighWeight(w));
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

void DrawNumber(int nb)
{
    if (nb > -1)
    {
        if (nb < 10)
        {
            std::cout << "000" << nb;
        }
        else if (nb < 100)
        {
            std::cout << "00" << nb;
        }
        else if (nb < 1000)
        {
            std::cout << "0" << nb;
        }
        else if (nb < 10000)
        {
            std::cout << nb;
        }
    }
    else
    {
        nb *= -1;
        if (nb < 10)
        {
            std::cout << "-00" << nb;
        }
        else if (nb < 100)
        {
            std::cout << "-0" << nb;
        }
        else if (nb < 1000)
        {
            std::cout << "-" << nb;
        }
        else if (nb < 10000)
        {
            std::cout << "" << nb;
        }
    }
}

void DrawWeight(char player, char ia, char obstacle)
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
            char c = TILES[i][j].GetChara();
            int w = TILES[i][j].GetWeight();
            if (c == '*')
            {
                SetConsoleTextAttribute(console, (FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN) * 2.f);
                DrawNumber(w);
                SetConsoleTextAttribute(console, FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN);
            }
            else if (c == '+')
            {
                SetConsoleTextAttribute(console, FOREGROUND_GREEN);
                DrawNumber(w);
                SetConsoleTextAttribute(console, FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN);
            }
            else if (c == '1' || c == '2' || c == '3' || c == '4' || c == '5' || c == '6' || c == '7' || c == '8' || c == '9')
            {
                SetConsoleTextAttribute(console, FOREGROUND_GREEN * ((int)c / 10));
                DrawNumber(w);
                SetConsoleTextAttribute(console, FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN);
            }
            else if (c == '-' || c == ia)
            {
                SetConsoleTextAttribute(console, FOREGROUND_RED);
                DrawNumber(w);
                SetConsoleTextAttribute(console, FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN);
            }
            else if (c == player)
            {
                SetConsoleTextAttribute(console, FOREGROUND_BLUE | FOREGROUND_RED);
                DrawNumber(w);
                SetConsoleTextAttribute(console, FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN);
            }
            else
            {
                DrawNumber(w);
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
    TILES[cursorPlayer.GetX()][cursorPlayer.GetY()].SetChara('0');
    Vector2 cursorIA(5, 5);
    TILES[cursorIA.GetX()][cursorIA.GetY()].SetChara('x');
    bool end = false;

    char obstacle = '*';
    char player = '0';
    char ia = 'x';
    while (true)
    {
        DrawChara(player, ia, obstacle);

        Input(&cursorPlayer, &cursorIA, player, ia, obstacle);


        if (end)
        {
            break;
        }
    }

    std::cout << "Hello World!\n";
}

