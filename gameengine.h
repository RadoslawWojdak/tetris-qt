#ifndef GAMEENGINE_H
#define GAMEENGINE_H

enum Direction
{
    DIR_NONE,
    DIR_LEFT,
    DIR_RIGHT
};

enum BlockType
{
    BLOCK_I = 0,
    BLOCK_J,
    BLOCK_L,
    BLOCK_O,
    BLOCK_S,
    BLOCK_T,
    BLOCK_Z
};

class GameEngine
{
public:
    static const int MAX_LEVEL;

private:
    bool **map;
    bool block[4][4];

    unsigned int rows, cols;
    int blockPosX, blockPosY;
    int level, score;
    BlockType blockType, nextBlockType;

    void clearMap();

    bool **getMapWithBlock() const;

    void clearBlock();
    void createNewBlock();
    void joinBlockToMap();

    BlockType randomBlock() const;

    bool isBlockOutside(bool block[4][4], Direction direction = DIR_NONE) const;
    bool shouldBlockStop(bool block[4][4]) const;

public:
    GameEngine(int rows, int columns, int level = 1);
    ~GameEngine();

    void moveBlockToTheSide(Direction dir);
    void moveBlockDown();
    void rotateBlock();

    void getBlockAppearance(BlockType blockType, bool block[4][4]) const;
    BlockType getNextBlock() const;
    int getLevel() const;
    int getScore() const;
    unsigned int getRows() const;
    unsigned int getColumns() const;
    bool **getBoard();
};

#endif // GAMEENGINE_H
