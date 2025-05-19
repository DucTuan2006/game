#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include "graphics.h"
#include "defs.h"
#include "game.h"
#include "grid.h"
#include "block.h"

using namespace std;

int main(int argc, char *argv[])
{
    Graphics graphics;
    graphics.init();
    graphics.prepareScene();

    Grid grid;
    grid.cell(graphics);

    Game game;
    game.Menu(graphics);

    Mix_Music *gMusic = graphics.loadMusic("assets\\Music.mp3");
    graphics.play(gMusic);

    bool quit = false;
    Uint32 lastFallTime = 0;
    const Uint32 fallInterval = 400;

    while (!quit)
    {
        grid.Initialize();

        Block block = game.SpawnNewBlock();
        game.DrawBackground(graphics);

        bool inGame = true;

        while (inGame)
        {
            SDL_Event event;
            while (SDL_PollEvent(&event))
            {
                if (event.type == SDL_QUIT)
                {
                    quit = true;
                    inGame = false;
                }
                else
                {
                    game.HandleInput(block, grid, event);
                }
            }

            Uint32 currentTime = SDL_GetTicks();
            if (currentTime - lastFallTime > fallInterval)
            {
                if (game.CanMove(block, grid, 1, 0))
                {
                    block.Move(1, 0);
                }
                else
                {
                    game.HardDrop(block, grid);
                    game.LockBlockToGrid(block, grid);
                    game.ClearFullRows(grid);
                    block = game.SpawnNewBlock();

                    if (!game.CanMove(block, grid, 0, 0))
                    {
                        game.GameOver(graphics);
                        inGame = false;
                    }
                }
                lastFallTime = currentTime;
            }

            grid.Draw(graphics);
            block.Draw(grid, graphics);
            game.DrawNext(graphics);
            game.DrawScore(graphics);
            game.DrawHighScore(graphics);
            game.LoadHighScore();
            game.nextBlock.DrawNextBlock(graphics, grid);
            graphics.presentScene();
        }
    }
    grid.quit_cell();
    if (gMusic != nullptr) Mix_FreeMusic(gMusic);
    graphics.quit();
    return 0;
}
