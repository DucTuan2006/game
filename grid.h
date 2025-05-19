#ifndef GRID_H
#define GRID_H

#include <map>
#include <vector>
#include "defs.h"
#include "graphics.h"

using namespace std;

struct Grid
{
    SDL_Texture *cellEmpty, *cellI, *cellJ, *cellL, *cellO, *cellS, *cellT, *cellZ;
    int board[numRows][numCols];

    void cell(Graphics &graphics)
    {
        cellEmpty = graphics.loadTexture("img//cell_empty.png");
        cellI = graphics.loadTexture("img//cell_I.png");
        cellJ = graphics.loadTexture("img//cell_J.png");
        cellL = graphics.loadTexture("img//cell_L.png");
        cellO = graphics.loadTexture("img//cell_O.png");
        cellS = graphics.loadTexture("img//cell_S.png");
        cellT = graphics.loadTexture("img//cell_T.png");
        cellZ = graphics.loadTexture("img//cell_Z.png");
    }

    void Initialize()
    {
        for(int row = 0; row < numRows; row++)
        {
            for(int col = 0; col < numCols; col++)
            {
                board[row][col] = 0;
            }
        }
    }

    void SetValue(int row, int col, int value)
    {
        if (row >= 0 && row < numRows && col >= 0 && col < numCols)
            board[row][col] = value;
    }

    void Print()
    {
        for(int row = 0; row < numRows; row++)
        {
            for(int col = 0; col < numCols; col++)
            {
                std::cout << board[row][col] << " ";
            }
            std::cout << std::endl;
        }
    }

    void Draw(Graphics &graphics)
    {

        for (int row = 0; row < numRows; row++)
            for (int col = 0; col < numCols; col++)
            {
                int x = 10 + col * CELL_SIZE;
                int y = 170 + row * CELL_SIZE;
                graphics.renderTexture(cellEmpty, x, y);
                switch(board[row][col])
                {
                case L:
                    graphics.renderTexture(cellL, x, y);
                    break;
                case J:
                    graphics.renderTexture(cellJ, x, y);
                    break;
                case I:
                    graphics.renderTexture(cellI, x, y);
                    break;
                case O:
                    graphics.renderTexture(cellO, x, y);
                    break;
                case S:
                    graphics.renderTexture(cellS, x, y);
                    break;
                case T:
                    graphics.renderTexture(cellT, x, y);
                    break;
                case Z:
                    graphics.renderTexture(cellZ, x, y);
                    break;
                }

            };

        graphics.presentScene();
    }

    void quit_cell()
    {
        SDL_DestroyTexture(cellEmpty);
        cellEmpty = nullptr;
        SDL_DestroyTexture(cellI);
        cellI = nullptr;
        SDL_DestroyTexture(cellJ);
        cellJ = nullptr;
        SDL_DestroyTexture(cellL);
        cellL = nullptr;
        SDL_DestroyTexture(cellO);
        cellO = nullptr;
        SDL_DestroyTexture(cellS);
        cellS = nullptr;
        SDL_DestroyTexture(cellT);
        cellT = nullptr;
        SDL_DestroyTexture(cellZ);
        cellZ = nullptr;
    }
} ;

#endif
