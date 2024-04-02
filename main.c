#include "main.h"

#include <stdio.h>
#include <stdlib.h>

#include "gba.h"
#include "mainHelper.h"

#include "images/garbage.h"
#include "images/main_screen.h"
#include "images/maze_bg.h"
#include "images/player.h"

enum gba_state {
    START,
    PLAY,
    WIN,
    LOSE,
    START_GAME,
    GAME_OVER,
    START_SCREEN
};

int main(void) {
  // Manipulate REG_DISPCNT here to set Mode 3. //
    REG_DISPCNT = MODE3 | BG2_ENABLE;

  // Save current and previous state of button input.
    u32 previousButtons = BUTTONS;
    u32 currentButtons = BUTTONS;

  // Load initial application state
    enum gba_state state = START_SCREEN;
    struct player game_player;

    while (1) {
        currentButtons = BUTTONS; // Load the current state of the buttons

        switch (state) {
            case START_SCREEN: // Setting up the start screen
                game_player.row = 5;
                game_player.col = 10;
                game_player.avatar = plauer;
                vBlankCounter = 0;
                drawFullScreenImageDMA(OIG3);
                drawString(7,15,"PRESS ENTER", WHITE);
                drawString(17, 15, "TO PLAY", WHITE);
                state = START;
                break;



            case START: // Polling for when to start game
                if (KEY_JUST_PRESSED(BUTTON_START, currentButtons,previousButtons)) {
                    state = START_GAME;
                    break;
                }
                break;



            case START_GAME: // Sets up the actual game screen
                if (KEY_DOWN(BUTTON_SELECT, currentButtons)) {
                    state = START_SCREEN;
                    break;
                }
                drawFullScreenImageDMA(maze);
                state = PLAY;
                drawImageDMA(game_player.row, game_player.col, 3, 3, game_player.avatar);
                drawString(6,130, "TIMER:20",BLACK);
                drawRectDMA(78, 225, 12,4,GREEN);
                break;

            case PLAY: // The playing logic
                if (KEY_DOWN(BUTTON_SELECT, currentButtons)) {
                    state = START_SCREEN;
                    break;
                }

                // Timer logic
                char str[20];
                if (vBlankCounter % 60 == 0 && vBlankCounter != 0) {
                    int seconds = vBlankCounter / 60;
                    if (seconds > 20) {
                        state = LOSE;
                        break;
                    }
                    sprintf(str, "%d",20-seconds);
                    undrawImageDMA(6, 165, 15, 10, maze);
                    drawString(6,165, str,BLACK);
                }

                // Movement logic
                if (KEY_DOWN(BUTTON_DOWN, currentButtons)){
                    waitForVBlank();
                    if (getBackgroundColor(game_player.row+3,game_player.col) != WHITE ||
                            getBackgroundColor(game_player.row+3,game_player.col+1) != WHITE ||
                            getBackgroundColor(game_player.row+3,game_player.col+2) != WHITE) {
                        if (getBackgroundColor(game_player.row+3,game_player.col) != GREEN ||
                            getBackgroundColor(game_player.row+3,game_player.col+1) != GREEN ||
                            getBackgroundColor(game_player.row+3,game_player.col+2) != GREEN) {
                            continue;
                        }
                    }
                    if (getBackgroundColor(game_player.row+3,game_player.col) == GREEN) {
                        state = WIN;
                        break;
                    }
                    undrawImageDMA(game_player.row, game_player.col, 3, 3, maze);
                    game_player.row++;
                    drawImageDMA(game_player.row, game_player.col, 3, 3, game_player.avatar);
                }
                else if (KEY_DOWN(BUTTON_UP, currentButtons)) {
                    waitForVBlank();
                    if (getBackgroundColor(game_player.row-1,game_player.col) != WHITE ||
                            getBackgroundColor(game_player.row-1,game_player.col+1) != WHITE ||
                            getBackgroundColor(game_player.row-1,game_player.col+2) != WHITE) {
                        if (getBackgroundColor(game_player.row-1,game_player.col) != GREEN ||
                            getBackgroundColor(game_player.row-1,game_player.col+1) != GREEN ||
                            getBackgroundColor(game_player.row-1,game_player.col+2) != GREEN) {
                            continue;
                        }
                    }
                    if (getBackgroundColor(game_player.row-1,game_player.col) == GREEN) {
                        state = WIN;
                        break;
                    }
                    undrawImageDMA(game_player.row, game_player.col, 3, 3, maze);
                    game_player.row--;
                    drawImageDMA(game_player.row, game_player.col, 3, 3, game_player.avatar);
                }
                else if (KEY_DOWN(BUTTON_RIGHT, currentButtons)) {
                    waitForVBlank();
                    if (getBackgroundColor(game_player.row,game_player.col+3) != WHITE ||
                            getBackgroundColor(game_player.row+1,game_player.col+3) != WHITE ||
                            getBackgroundColor(game_player.row+2,game_player.col+3) != WHITE) {
                        if (getBackgroundColor(game_player.row,game_player.col+3) != GREEN ||
                            getBackgroundColor(game_player.row+1,game_player.col+3) != GREEN ||
                            getBackgroundColor(game_player.row+2,game_player.col+3) != GREEN) {
                            continue;
                        }
                    }
                    if (getBackgroundColor(game_player.row,game_player.col+3) == GREEN) {
                        state = WIN;
                        break;
                    }
                    undrawImageDMA(game_player.row, game_player.col, 3, 3, maze);
                    game_player.col++;
                    drawImageDMA(game_player.row, game_player.col, 3, 3, game_player.avatar);
                }
                else if (KEY_DOWN(BUTTON_LEFT, currentButtons)) {
                    waitForVBlank();
                    if (getBackgroundColor(game_player.row,game_player.col-1) != WHITE ||
                            getBackgroundColor(game_player.row+1,game_player.col-1) != WHITE ||
                            getBackgroundColor(game_player.row+2,game_player.col-1) != WHITE ) {
                        if (getBackgroundColor(game_player.row,game_player.col+3) != GREEN ||
                            getBackgroundColor(game_player.row+1,game_player.col+3) != GREEN ||
                            getBackgroundColor(game_player.row+2,game_player.col+3) != GREEN) {
                            continue;
                        }
                    }
                    if (getBackgroundColor(game_player.row,game_player.col-1) == GREEN) {
                        state = WIN;
                        break;
                    }
                    undrawImageDMA(game_player.row, game_player.col, 3, 3, maze);
                    game_player.col--;
                    drawImageDMA(game_player.row, game_player.col, 3, 3, game_player.avatar);
                }
                break;



            case LOSE: // Setting up the loss screen
                fillScreenDMA(RED);
                drawString(7,15,"GAME OVER", WHITE);
                drawString(17,15,"YOU LOST", WHITE);
                drawString(27, 15, "PRESS ENTER", WHITE);
                drawString(37, 15, "TO PLAY AGAIN", WHITE);
                state = GAME_OVER;
                break;



            case GAME_OVER: // Polling to start the new game
                if (KEY_DOWN(BUTTON_SELECT, currentButtons) || KEY_JUST_PRESSED(BUTTON_START,currentButtons,previousButtons)) {
                      state = START_SCREEN;
                }
                break;



            case WIN: // Setting up won screen
                fillScreenDMA(GREEN);
                drawString(7,15,"GAME OVER", WHITE);
                drawString(17,15,"YOU WON", WHITE);
                drawString(27, 15, "PRESS ENTER", WHITE);
                drawString(37, 15, "TO PLAY AGAIN", WHITE);
                state = GAME_OVER;
                break;
        }
        previousButtons = currentButtons; // Store the current state of the buttons
    }
    UNUSED(previousButtons); // You can remove this once previousButtons is used
  return 0;
}


