#ifndef _BLOCK_H_
#define _BLOCK_H_

#include "defs.h"
#include <map>
#include <vector>
#include "graphics.h"

using namespace std;

struct Block
{
    int id;
    map<int, vector<pair<int, int>>> cells;
    int rotationState = 0;

    void Draw(Grid &grid, Graphics &graphics)
    {
        for(auto cell : cells[rotationState])
        {
            int row = cell.first;
            int col = cell.second;
            int x = 10 + col * CELL_SIZE;
            int y = 170 + row * CELL_SIZE;
            switch(id)
            {
            case L:
                graphics.renderTexture(grid.cellL, x, y);
                break;
            case J:
                graphics.renderTexture(grid.cellJ, x, y);
                break;
            case I:
                graphics.renderTexture(grid.cellI, x, y);
                break;
            case O:
                graphics.renderTexture(grid.cellO, x, y);
                break;
            case S:
                graphics.renderTexture(grid.cellS, x, y);
                break;
            case T:
                graphics.renderTexture(grid.cellT, x, y);
                break;
            case Z:
                graphics.renderTexture(grid.cellZ, x, y);
                break;
            }
        }
        graphics.presentScene();
    }

    void DrawNextBlock(Graphics &graphics, Grid &grid)
    {
        for (auto cell : cells[rotationState])
        {
            int row = cell.first;
            int col = cell.second;
            int x = 275 + col * CELL_SIZE;
            int y = 310 + row * CELL_SIZE;

            switch(id)
            {
            case L:
                graphics.renderTexture(grid.cellL, x, y);
                break;
            case J:
                graphics.renderTexture(grid.cellJ, x, y);
                break;
            case I:
                graphics.renderTexture(grid.cellI, x - 15, y + 15);
                break;
            case O:
                graphics.renderTexture(grid.cellO, x - 17, y);
                break;
            case S:
                graphics.renderTexture(grid.cellS, x, y);
                break;
            case T:
                graphics.renderTexture(grid.cellT, x, y);
                break;
            case Z:
                graphics.renderTexture(grid.cellZ, x, y);
                break;
            }
        }
    }

    void Move(int rowOffset, int colOffset)
    {
        for (auto &rotation : cells)
        {
            for (auto &cell : rotation.second)
            {
                cell.first += rowOffset;
                cell.second += colOffset;
            }
        }
    }

};

struct LBlock : public Block
{
    LBlock()
    {
        id = L;
        cells[0] = {{0, 2}, {1, 0}, {1, 1}, {1, 2}};
        cells[1] = {{0, 1}, {1, 1}, {2, 1}, {2, 2}};
        cells[2] = {{1, 0}, {1, 1}, {1, 2}, {2, 0}};
        cells[3] = {{0, 0}, {0, 1}, {1, 1}, {2, 1}};
        Move(0, 3);
    }
};

struct JBlock : public Block
{
    JBlock()
    {
        id = J;
        cells[0] = {{0, 0}, {1, 0}, {1, 1}, {1, 2}};
        cells[1] = {{0, 1}, {0, 2}, {1, 1}, {2, 1}};
        cells[2] = {{1, 0}, {1, 1}, {1, 2}, {2, 2}};
        cells[3] = {{0, 1}, {1, 1}, {2, 0}, {2, 1}};
        Move(0, 3);
    }
};

struct IBlock : public Block
{
    IBlock()
    {
        id = I;
        cells[0] = {{1, 0}, {1, 1}, {1, 2}, {1, 3}};
        cells[1] = {{0, 2}, {1, 2}, {2, 2}, {3, 2}};
        cells[2] = {{2, 0}, {2, 1}, {2, 2}, {2, 3}};
        cells[3] = {{0, 1}, {1, 1}, {2, 1}, {3, 1}};
        Move(-1, 3);
    }
};

struct OBlock : public Block
{
    OBlock()
    {
        id = O;
        cells[0] = {{0, 0}, {0, 1}, {1, 0}, {1, 1}};
        Move(0, 4);
    }
};

struct SBlock : public Block
{
    SBlock()
    {
        id = S;
        cells[0] = {{0, 1}, {0, 2}, {1, 0}, {1, 1}};
        cells[1] = {{0, 1}, {1, 1}, {1, 2}, {2, 2}};
        cells[2] = {{1, 1}, {1, 2}, {2, 0}, {2, 1}};
        cells[3] = {{0, 0}, {1, 0}, {1, 1}, {2, 1}};
        Move(0, 3);
    }
};

struct TBlock : public Block
{
    TBlock()
    {
        id = T;
        cells[0] = {{0, 1}, {1, 0}, {1, 1}, {1, 2}};
        cells[1] = {{0, 1}, {1, 1}, {1, 2}, {2, 1}};
        cells[2] = {{1, 0}, {1, 1}, {1, 2}, {2, 1}};
        cells[3] = {{0, 1}, {1, 0}, {1, 1}, {2, 1}};
        Move(0, 3);
    }
};

struct ZBlock : public Block
{
    ZBlock()
    {
        id = Z;
        cells[0] = {{0, 0}, {0, 1}, {1, 1}, {1, 2}};
        cells[1] = {{0, 2}, {1, 1}, {1, 2}, {2, 1}};
        cells[2] = {{1, 0}, {1, 1}, {2, 1}, {2, 2}};
        cells[3] = {{0, 1}, {1, 0}, {1, 1}, {2, 0}};
        Move(0, 3);
    }
};

#endif // _BLOCK_H_
