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
public:
    static const int MAX_LEVEL;

private:
    bool **map;
    bool **mapWithBlock;
    bool block[4][4];

    unsigned int rows, cols;
    int blockPosX, blockPosY;
    int level, score;

    void clearMap();

    void clearMapWithBlock();
    void refreshMapWithBlock();

    void clearBlock();
    void createNewBlock();
    void joinBlockToMap();

    bool isBlockOutside() const;
    bool shouldBlockStop() const;

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
