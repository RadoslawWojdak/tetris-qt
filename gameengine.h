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
    BLOCK_NONE = 0,
    BLOCK_I,
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
    BlockType **map; //for colors
    bool block[4][4];

    bool gameOver;
    unsigned int rows, cols;
    int blockPosX, blockPosY;
    int level, score, clearedLines;
    BlockType blockType, nextBlockType;


    void clearMap();

    void clearBlock();
    void createNewBlock();
    void joinBlockToMap();

    void pullBoardDown(int line);
    void clearFullLines();

    BlockType randomBlock() const;
    BlockType **getMapWithBlock() const;
    bool isBlockOutside(bool block[4][4], Direction direction = DIR_NONE) const;
    bool shouldBlockStop(bool block[4][4]) const;

public:
    GameEngine(int rows, int columns, int level = 1);
    ~GameEngine();


    void moveBlockToTheSide(Direction dir);
    void moveBlockDown();
    void rotateBlock();


    void getBlockAppearance(BlockType blockType, bool block[4][4]) const;

    bool isGameOver();

    BlockType **getBoard();

    BlockType getBlockType() const;
    BlockType getNextBlockType() const;

    void nextLevel();
    void setLevel(int level);
    int getLevel() const;

    void addScore(int points);
    int getScore() const;

    unsigned int getRows() const;
    unsigned int getColumns() const;
};

#endif // GAMEENGINE_H
