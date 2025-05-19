#ifndef _GAME_H_
#define _GAME_H_
#include "grid.h"
#include "block.h"
#include "graphics.h"
#include "defs.h"
#include <cstdlib>
#include <ctime>

using namespace std;

struct Game
{
    vector<Block> blocks;
    Block currentBlock;
    Block nextBlock;
    int score = 0;
    int highScore = 0;
    bool inMainMenu = true;

    void Menu(Graphics &graphics)
    {
        SDL_Texture *menu = graphics.loadTexture("img/Tetris1.png");
        SDL_Texture *start = graphics.loadTexture("img/Start.png");

        SDL_Event event;
        bool running = true;

        while (running)
        {
            int mouseX, mouseY;
            SDL_GetMouseState(&mouseX, &mouseY);

            graphics.prepareScene(menu);

            if (mouseX >= 115 && mouseX <= 395 && mouseY >= 375 && mouseY <= 480)
            {
                SDL_Rect hoverRect = {100, 360, 310, 135};
                SDL_RenderCopy(graphics.renderer, start, NULL, &hoverRect);
            }

            graphics.presentScene();

            while (SDL_PollEvent(&event))
            {
                if (event.type == SDL_QUIT)
                {
                    running = false;
                    break;
                }
                else if (event.type == SDL_MOUSEBUTTONDOWN)
                {
                    if (mouseX >= 115 && mouseX <= 395 && mouseY >= 375 && mouseY <= 480)
                    {
                        SDL_DestroyTexture(menu);
                        SDL_DestroyTexture(start);
                        return;
                    }
                }
            }
        }
        SDL_DestroyTexture(menu);
        SDL_DestroyTexture(start);
        graphics.quit();
        exit(0);
    }

    void GameOver(Graphics &graphics)
    {
        SDL_Texture *gameOver = graphics.loadTexture("img/GameOver.png");
        SDL_Texture *restart = graphics.loadTexture("img/Restart.png");

        SDL_Event event;
        bool running = true;

        while(running)
        {
            int mouseX, mouseY;
            SDL_GetMouseState(&mouseX, &mouseY);

            graphics.prepareScene(gameOver);

            if (mouseX >= 95 && mouseX <= 420 && mouseY >= 420 && mouseY <= 540)
            {
                SDL_Rect hoverRect = {90, 410, 335, 140};
                SDL_RenderCopy(graphics.renderer, restart, NULL, &hoverRect);
            }

            graphics.presentScene();

            while(SDL_PollEvent(&event))
            {
                if (event.type == SDL_QUIT)
                {
                    running = false;
                    exit(0);
                }
                else if (event.type == SDL_MOUSEBUTTONDOWN)
                {
                    if (mouseX >= 95 && mouseX <= 420 && mouseY >= 420 && mouseY <= 540)
                    {
                        SDL_DestroyTexture(gameOver);
                        SDL_DestroyTexture(restart);
                        return;
                    }
                }
            }
        }
        SDL_DestroyTexture(gameOver);
        SDL_DestroyTexture(restart);
        graphics.quit();
        exit(0);
    }

    void ResetGame()
    {
        blocks.clear();
        score = 0;
        nextBlock = GetRandomBlock();
    }

    void DrawBackground(Graphics &graphics)
    {
        SDL_Texture* background = graphics.loadTexture("img/Tetris2.png");
        SDL_Rect destRect;
        destRect.x = 0;
        destRect.y = 0;
        destRect.w = SCREEN_WIDTH;
        destRect.h = SCREEN_HEIGHT;
        SDL_RenderCopy(graphics.renderer, background, NULL, &destRect);
        SDL_DestroyTexture(background);
        background = nullptr;
    }

    Game()
    {
        srand(time(0));
        nextBlock = GetRandomBlock();
    }

    Block GetRandomBlock()
    {
        if (blocks.empty())
        {
            blocks = GetAllBlocks();

        }
        int randomIndex = rand() % blocks.size();
        Block block  = blocks[randomIndex];
        blocks.erase(blocks.begin() + randomIndex);
        return block;
    }

    Block SpawnNewBlock()
    {
        currentBlock = nextBlock;
        nextBlock = GetRandomBlock();
        return currentBlock;
    }

    vector<Block> GetAllBlocks()
    {
        return {IBlock(), JBlock(),  LBlock(), OBlock(), SBlock(), TBlock(), ZBlock()};
    }

    bool CanMove(Block &block, Grid &grid, int rowOffset, int colOffset)
    {
        for (const auto &cell : block.cells[block.rotationState])
        {
            int newRow = cell.first + rowOffset;
            int newCol = cell.second + colOffset;

            if (newRow < 0 || newRow >= numRows || newCol < 0 || newCol >= numCols)
                return false;

            if (grid.board[newRow][newCol] != 0)
                return false;
        }
        return true;
    }

    bool HitTheWall(Block &block, Grid &grid)
    {
        int originalRotation = block.rotationState;
        int nextRotation = (originalRotation + 1) % 4;

        // Các offset dịch thử (xoay kèm dịch sang trái/phải)
        vector<int> kickOffsets = {0, -1, 1, -2, 2};

        for (int offset : kickOffsets)
        {
            bool valid = true;

            for (const auto &cell : block.cells[nextRotation])
            {
                int row = cell.first;
                int col = cell.second + offset;

                if (row < 0 || row >= numRows || col < 0 || col >= numCols || grid.board[row][col] != 0)
                {
                    valid = false;
                    break;
                }
            }

            if (valid)
            {
                // Áp dụng xoay và dịch
                block.rotationState = nextRotation;
                for (auto &cell : block.cells[nextRotation])
                {
                    cell.second += offset;
                }
                return true;
            }
        }

        return false; // không xoay được
    }

    void HandleInput(Block &block, Grid &grid, SDL_Event &event)
    {
        if (event.type == SDL_KEYDOWN)
        {
            switch (event.key.keysym.sym)
            {
            case SDLK_DOWN:
                if (CanMove(block, grid, 1, 0))
                    block.Move(1, 0);
                break;
            case SDLK_LEFT:
                if (CanMove(block, grid, 0, -1))
                    block.Move(0, -1);
                break;
            case SDLK_RIGHT:
                if (CanMove(block, grid, 0, 1))
                    block.Move(0, 1);
                break;
            case SDLK_UP:
                HitTheWall(block, grid);
                break;
            case SDLK_SPACE:
                HardDrop(block, grid);
                break;
            }
        }
    }

    void LockBlockToGrid(Block &block, Grid &grid)
    {
        for (const auto &cell : block.cells[block.rotationState])
        {
            int row = cell.first;
            int col = cell.second;
            if (row >= 0 && row < numRows && col >= 0 && col < numCols)
            {
                grid.board[row][col] = block.id;
            }
        }
    }

    void ClearFullRows(Grid &grid)
    {
        int rowsCleared = 0;
        for (int row = numRows - 1; row >= 0; --row)
        {
            bool full = true;
            for (int col = 0; col < numCols; ++col)
            {
                if (grid.board[row][col] == 0)
                {
                    full = false;
                    break;
                }
            }

            if (full)
            {
                for (int r = row; r > 0; --r)
                {
                    for (int c = 0; c < numCols; ++c)
                    {
                        grid.board[r][c] = grid.board[r - 1][c];
                    }
                }
                for (int c = 0; c < numCols; ++c)
                {
                    grid.board[0][c] = 0;
                }
                rowsCleared++;
                row++;
            }
        }
        score += rowsCleared * 100;

        switch (rowsCleared)
        {
        case 1:
            score += 100;
            break;
        case 2:
            score += 300;
            break;
        case 3:
            score += 500;
            break;
        case 4:
            score += 800;
            break;
        }
        SaveHighScore();
    }

    void HardDrop(Block &block, Grid &grid)
    {
        while (CanMove(block, grid, 1, 0))
        {
            block.Move(1, 0);
        }
    }

    void DrawNext(Graphics &graphics)
    {
        int X = 320;
        int Y = 250;
        int Width = 6 * CELL_SIZE;
        int Height = 6 * CELL_SIZE;

        SDL_Color Color = {236, 236, 236, 255};  // Trắng

        graphics.drawRectangle(X, Y, Width, Height, Color);

        TTF_Font* font = graphics.loadFont("assets/Purisa-BoldOblique.ttf", 40);
        SDL_Texture* NextText = graphics.renderText("Next", font, {255, 255, 0, 0});
        graphics.renderTexture(NextText, 350, 190);

        SDL_DestroyTexture( NextText );
        TTF_CloseFont( font );

    }

    void DrawScore(Graphics &graphics)
    {
        int X = 320;
        int Y = 540;
        int Width = 6 * CELL_SIZE;
        int Height = 2 * CELL_SIZE;

        SDL_Color Color = {236, 236, 236, 255};

        graphics.drawRectangle(X, Y, Width, Height, Color);

        TTF_Font* font = graphics.loadFont("assets/Purisa-BoldOblique.ttf", 40);
        SDL_Texture* ScoreText = graphics.renderText("Score", font, {255, 255, 0, 0});
        graphics.renderTexture(ScoreText, 345, 480);

        string scoreText = to_string(score);
        SDL_Texture* scoreValue = graphics.renderText(scoreText.c_str(), font, {0, 0, 0, 0});
        graphics.renderTexture(scoreValue, 350, 540);

        SDL_DestroyTexture( ScoreText );
        SDL_DestroyTexture(scoreValue);
        TTF_CloseFont( font );

    }

    void LoadHighScore()
    {
        FILE* file = fopen("highscore.txt", "r");
        if (file)
        {
            fscanf(file, "%d", &highScore);
            fclose(file);
        }
        else
        {
            highScore = 0; // Nếu file chưa tồn tại
        }
    }

    void SaveHighScore()
    {
        if (score > highScore)
        {
            highScore = score;
            FILE* file = fopen("highscore.txt", "w");
            if (file)
            {
                fprintf(file, "%d", highScore);
                fclose(file);
            }
        }
    }

    void DrawHighScore(Graphics &graphics)
    {
        int X = 320;
        int Y = 670;
        int Width = 6 * CELL_SIZE;
        int Height = 2 * CELL_SIZE;

        SDL_Color Color = {236, 236, 236, 255};

        graphics.drawRectangle(X, Y, Width, Height, Color);

        TTF_Font* font = graphics.loadFont("assets/Purisa-BoldOblique.ttf", 25);
        SDL_Texture* HighScoreText = graphics.renderText("High Score", font, {255, 255, 0, 0});
        graphics.renderTexture(HighScoreText, 330, 630);

        string highScoreText = to_string(highScore);
        TTF_Font* fontvalue = graphics.loadFont("assets/Purisa-BoldOblique.ttf", 40);
        SDL_Texture* highScoreValue = graphics.renderText(highScoreText.c_str(), fontvalue, {0, 0, 0, 0});
        graphics.renderTexture(highScoreValue, 350, 670);

        SDL_DestroyTexture( HighScoreText );
        SDL_DestroyTexture(highScoreValue);
        TTF_CloseFont( font );
    }

};

#endif // _GAME_H_
