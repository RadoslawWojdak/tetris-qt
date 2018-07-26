#include "gameengine.h"

#include <iostream>
#include <time.h>

const int GameEngine::MAX_LEVEL = 9;

GameEngine::GameEngine(int rows, int columns, int level) :
    rows(static_cast<unsigned int>(rows)),
    cols(static_cast<unsigned int>(columns)),
    level(level),
    score(0),
    clearedLines(0)
{
    srand(static_cast<unsigned int>(time(nullptr)));

    map = new BlockType*[this->cols];
    for (unsigned int i = 0; i < this->cols; i++)
        map[i] = new BlockType[this->rows];

    clearMap();

    nextBlockType = randomBlock();
    createNewBlock();
}

GameEngine::~GameEngine()
{
    for (unsigned int i = 0; i < cols; i++)
        delete[] map[i];
    delete[] map;
}

void GameEngine::clearMap()
{
    for (unsigned int i = 0; i < cols; i++)
        for (unsigned int j = 0; j < rows; j++)
            map[i][j] = BLOCK_NONE;
}

BlockType **GameEngine::getMapWithBlock() const
{
    BlockType **mapWithBlock = new BlockType*[cols];
    for (int i = 0; i < static_cast<int>(cols); i++)
        mapWithBlock[i] = new BlockType[rows];

    int x = static_cast<int>(blockPosX);
    int y = static_cast<int>(blockPosY);

    for (int i = 0; i < static_cast<int>(cols); i++)
    {
        for (int j = 0; j < static_cast<int>(rows); j++)
        {
            if ((i - x >= 0 && i - x <= 3) && (j - y >= 0 && j - y <= 3) && block[i - x][j - y])
                mapWithBlock[i][j] = blockType;
            else
                mapWithBlock[i][j] = map[i][j];
        }
    }

    return mapWithBlock;
}

void GameEngine::clearBlock()
{
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            block[i][j] = false;
}

void GameEngine::createNewBlock()
{
    clearBlock();

    blockPosX = cols / 2 - 2;
    blockPosY = 0;

    getBlockAppearance(nextBlockType, block);
    blockType = nextBlockType;

    nextBlockType = randomBlock();
}

void GameEngine::joinBlockToMap()
{
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            if (block[i][j])
                map[blockPosX + i][blockPosY + j] = blockType;
}

void GameEngine::pullBoardDown(int line)
{
    for (int i = line; i > 0; i--)
        for (int j = 0; j < static_cast<int>(cols); j++)
            map[j][i] = map[j][i - 1];
}

void GameEngine::clearFullLines()
{
    int countClearedLines = 0;

    for (int i = static_cast<int>(rows) - 1; i > 0; i--)
    {
        bool isFull = true;
        for (int j = 0; j < static_cast<int>(cols); j++)
        {
            if (!map[j][i])
            {
                isFull = false;
                break;
            }
        }
        if (isFull)
        {
            pullBoardDown(i);
            countClearedLines++;
            i++;
        }
    }

    addScore(countClearedLines * countClearedLines * level);
    clearedLines += countClearedLines;

    static int linesToNextLevel = 10;
    linesToNextLevel -= countClearedLines;
    if (linesToNextLevel <= 0)
    {
        linesToNextLevel += 10;
        nextLevel();
    }
}

BlockType GameEngine::randomBlock() const
{
    return static_cast<BlockType>(rand() % 7 + 1);
}

bool GameEngine::isBlockOutside(bool block[4][4], Direction direction) const
{
    if (direction != DIR_RIGHT)
        if (blockPosX < 4)
            for (int i = 0; i < 4; i++)
                for (int j = 0; j < 4; j++)
                    if (block[i][j] && blockPosX + i < 0)
                        return true;

    if (direction != DIR_LEFT)
        if (blockPosX > static_cast<int>(cols) - 4)
            for (int i = 0; i < 4; i++)
                for (int j = 0; j < 4; j++)
                    if (block[i][j] && blockPosX + i >= static_cast<int>(cols))
                        return true;

    return false;
}

bool GameEngine::shouldBlockStop(bool block[4][4]) const
{
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            if (block[i][j] && (blockPosY + j >= static_cast<int>(rows) || map[blockPosX + i][blockPosY + j]))
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
        if (isBlockOutside(block, DIR_LEFT) || shouldBlockStop(block))
            blockPosX++;

        break;
    }
    case DIR_RIGHT:
    {
        blockPosX++;
        if (isBlockOutside(block, DIR_RIGHT) || shouldBlockStop(block))
            blockPosX--;

        break;
    }
    default: break;
    }
}

void GameEngine::moveBlockDown()
{
    blockPosY++;

    if (shouldBlockStop(block))
    {
        blockPosY--;
        joinBlockToMap();
        clearFullLines();
        createNewBlock();
    }
}

void GameEngine::rotateBlock()
{
    int blockSize = 3;
    if (blockType == BLOCK_O)
        blockSize = 2;
    else if (blockType == BLOCK_I)
        blockSize = 4;

    bool temp[4][4] = {};

    for (int i = 0; i < blockSize; i++)
        for (int j = 0; j < blockSize; j++)
            temp[i][j] = block[j][blockSize - i - 1];

    if (isBlockOutside(temp) || shouldBlockStop(temp))
    {
        blockPosX++;
        if (isBlockOutside(temp) || shouldBlockStop(temp))
        {
            blockPosX -= 2;
            if (isBlockOutside(temp) || shouldBlockStop(temp))
            {
                blockPosX++;
                return;
            }
        }
    }

    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            block[i][j] = temp[i][j];
}

void GameEngine::getBlockAppearance(BlockType blockType, bool block[4][4]) const
{
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            block[i][j] = false;

    switch(blockType)
    {
    case BLOCK_I:
    {
        block[0][1] = true;
        block[1][1] = true;
        block[2][1] = true;
        block[3][1] = true;
        break;
    }
    case BLOCK_J:
    {
        block[1][0] = true;
        block[1][1] = true;
        block[0][2] = true;
        block[1][2] = true;
        break;
    }
    case BLOCK_L:
    {
        block[0][0] = true;
        block[0][1] = true;
        block[0][2] = true;
        block[1][2] = true;
        break;
    }
    case BLOCK_O:
    {
        block[0][0] = true;
        block[0][1] = true;
        block[1][0] = true;
        block[1][1] = true;
        break;
    }
    case BLOCK_S:
    {
        block[1][1] = true;
        block[2][1] = true;
        block[0][2] = true;
        block[1][2] = true;
        break;
    }
    case BLOCK_T:
    {
        block[0][1] = true;
        block[1][1] = true;
        block[2][1] = true;
        block[1][2] = true;
        break;
    }
    case BLOCK_Z:
    {
        block[0][1] = true;
        block[1][1] = true;
        block[1][2] = true;
        block[2][2] = true;
        break;
    }
    }
}

BlockType **GameEngine::getBoard()
{
    BlockType **mapWithBlock = getMapWithBlock();
    return mapWithBlock;
}

BlockType GameEngine::getBlockType() const
{
    return blockType;
}

BlockType GameEngine::getNextBlockType() const
{
    return nextBlockType;
}

void GameEngine::nextLevel()
{
    if (++level > MAX_LEVEL)
        level = MAX_LEVEL;
}

void GameEngine::setLevel(int level)
{
    this->level = level;
    if (this->level > MAX_LEVEL)
        level = MAX_LEVEL;
    else if (this->level < 1)
        level = 1;
}

int GameEngine::getLevel() const
{
    return level;
}

void GameEngine::addScore(int points)
{
    score += points;
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
