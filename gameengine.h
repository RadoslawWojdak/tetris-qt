#ifndef GAMEENGINE_H
#define GAMEENGINE_H

enum Direction
{
    DIR_NONE,
    DIR_LEFT,
    DIR_RIGHT
};

class GameEngine
{
    bool **map;
    bool **mapWithBlock;
    bool block[4][4];

    unsigned int rows, cols;
    unsigned int blockPosX, blockPosY;
    int level, score;

    void clearMap();
    void clearMapWithBlock();
    void refreshMapWithBlock();
    void clearBlock();
    void newBlock();

public:
    GameEngine(int rows, int columns, int level = 1);
    ~GameEngine();

    void moveBlockToTheSide(Direction dir);
    void moveBlockDown();

    int getLevel() const;
    int getScore() const;
    unsigned int getRows() const;
    unsigned int getColumns() const;
    bool **getBoard();
};

#endif // GAMEENGINE_H
