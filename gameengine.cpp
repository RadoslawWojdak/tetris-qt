#include "gameengine.h"

const int GameEngine::MAX_LEVEL = 100;

GameEngine::GameEngine(int rows, int columns, int level) :
    rows(static_cast<unsigned int>(rows)),
    cols(static_cast<unsigned int>(columns)),
    level(level),
    score(0)
{
    map = new bool*[this->cols];
    for (unsigned int i = 0; i < this->cols; i++)
        map[i] = new bool[this->rows];

    mapWithBlock = new bool*[this->cols];
    for (unsigned int i = 0; i < this->cols; i++)
        mapWithBlock[i] = new bool[this->rows];

    clearMap();
    clearMapWithBlock();

    newBlock();
}

GameEngine::~GameEngine()
{
    for (unsigned int i = 0; i < cols; i++)
    {
        delete[] map[i];
        delete[] mapWithBlock[i];
    }
    delete[] map;
    delete[] mapWithBlock;
}

void GameEngine::clearMap()
{
    for (unsigned int i = 0; i < cols; i++)
        for (unsigned int j = 0; j < rows; j++)
            map[i][j] = false;
}

void GameEngine::clearMapWithBlock()
{
    for (unsigned int i = 0; i < cols; i++)
        for (unsigned int j = 0; j < rows; j++)
            mapWithBlock[i][j] = false;
}

void GameEngine::refreshMapWithBlock()
{
    clearMapWithBlock();

    int x = static_cast<int>(blockPosX);
    int y = static_cast<int>(blockPosY);

    for (int i = 0; i < static_cast<int>(cols); i++)
        for (int j = 0; j < static_cast<int>(rows); j++)
        {
            if ((i - x >= 0 && i - x <= 3) && (j - y >= 0 && j - y <= 3))
                mapWithBlock[i][j] = block[i - x][j - y];
            else
                mapWithBlock[i][j] = map[i][j];
        }
}

void GameEngine::clearBlock()
{
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            block[i][j] = false;
}

void GameEngine::newBlock()
{
    clearBlock();

    blockPosX = cols / 2 - 2;
    blockPosY = 0;

    block[0][0] = true;
    block[1][0] = true;
    block[1][1] = true;
    block[2][1] = true;
}

bool GameEngine::isBlockOutside()
{
    if (blockPosX < 0)
        return true;

    if (blockPosX > static_cast<int>(cols) - 4)
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                if (block[i][j] && blockPosX + i >= static_cast<int>(cols))
                    return true;

    return false;
}

void GameEngine::moveBlockToTheSide(Direction dir)
{
    switch(dir)
    {
    case DIR_LEFT:
    {
        blockPosX--;
        if (isBlockOutside())
            blockPosX++;

        break;
    }
    case DIR_RIGHT:
    {
        blockPosX++;
        if (isBlockOutside())
            blockPosX--;

        break;
    }
    default: break;
    }
}

void GameEngine::moveBlockDown()
{
    blockPosY++;
}

int GameEngine::getLevel() const
{
    return level;
}

int GameEngine::getScore() const
{
    return score;
}

unsigned int GameEngine::getRows() const
{
    return rows;
}

unsigned int GameEngine::getColumns() const
{
    return cols;
}

bool **GameEngine::getBoard()
{
    refreshMapWithBlock();
    return mapWithBlock;
}
