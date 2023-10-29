/**
 * @file tictactoe.c
 * @brief A simple game of tic-tac-toe.
 * @author Philipp Schilk, 2023
 * @note Behold the spaghetti monster! Not the cleanest code I have written. Sorry!
 */
#include "tictactoe.h"

#include "stm32l4xx_hal.h"
#include "the_computer.h"
#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>

// ===== Functions =============================================================

#define IDX_TO_X(_idx_) (((_idx_)-1) % 3)
#define IDX_TO_Y(_idx_) (((_idx_)-1) / 3)

static void tictactoe_update_leds(struct TTTState *state, bool show_active_player) {
  uint32_t green = 0;
  uint32_t red = 0;

  for (size_t x = 0; x < 3; x++) {
    for (size_t y = 0; y < 3; y++) {
      switch (state->board[x][y]) {
        case SLOT_P1:
          green |= 0x1 << (x + 4 * y);
          break;
        case SLOT_P2:
          red |= 0x1 << (x + 4 * y);
          break;
        case SLOT_EMPTY:
          break;
      }
    }
  }

  tictactoe_write_display(red, green);

  if (show_active_player) {
    if (state->current_player == P1) {
      tictactoe_write_leds(1, 0);
    } else {
      tictactoe_write_leds(0, 1);
    }
  } else {
    tictactoe_write_leds(0, 0);
  }
}

static uint32_t tictactoe_user_prompt(char *prompt, uint32_t max_valid_option) {
  while (1) {
    printf("%s\r\n", prompt);
    uint32_t inp = tictactoe_get_input();

    if (inp > 9) {
      printf("ERROR! The function tictactoe_get_input returned a value of %" PRIu32 ".\r\n", inp);
      printf("It should return a value between 1 and 9 inclusive if a valid input was received, or 0 if not.\r\n");
      printf("Please fix your tictactoe_get_input function!\r\n\r\n");
    } else if (inp == 0 || inp > max_valid_option) {
      printf("Invalid input!\r\n\r\n");
    } else {
      return inp;
    }
  }
}

#define IDX_STR(_state_, _idx_) ((_state_)->board[IDX_TO_X(_idx_)][IDX_TO_Y(_idx_)] == SLOT_EMPTY ? #_idx_ : " ")

#define BOARD_STR(_state_, _idx_)                                                                                      \
  ((_state_)->board[IDX_TO_X(_idx_)][IDX_TO_Y(_idx_)] == SLOT_P1   ? "X"                                               \
   : (_state_)->board[IDX_TO_X(_idx_)][IDX_TO_Y(_idx_)] == SLOT_P2 ? "O"                                               \
                                                                   : " ")

static uint32_t tictactoe_get_user_move(struct TTTState *state) {
  printf("\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n");
  printf("   Available Moves:     Board:\r\n");
  printf("\r\n");
  printf("     %s | %s | %s", IDX_STR(state, 7), IDX_STR(state, 8), IDX_STR(state, 9));
  printf("        %s | %s | %s\r\n", BOARD_STR(state, 7), BOARD_STR(state, 8), BOARD_STR(state, 9));
  printf("     --+---+--");
  printf("        --+---+--\r\n");
  printf("     %s | %s | %s", IDX_STR(state, 4), IDX_STR(state, 5), IDX_STR(state, 6));
  printf("        %s | %s | %s\r\n", BOARD_STR(state, 4), BOARD_STR(state, 5), BOARD_STR(state, 6));
  printf("     --+---+--");
  printf("        --+---+--\r\n");
  printf("     %s | %s | %s", IDX_STR(state, 1), IDX_STR(state, 2), IDX_STR(state, 3));
  printf("        %s | %s | %s\r\n", BOARD_STR(state, 1), BOARD_STR(state, 2), BOARD_STR(state, 3));
  printf("\r\n");
  printf("Player %i's turn\r\n", state->current_player == P1 ? 1 : 2);
  while (1) {
    uint32_t inp = tictactoe_user_prompt("Move [1-9]:", 9);
    if (state->board[IDX_TO_X(inp)][IDX_TO_Y(inp)] == SLOT_EMPTY) {
      return inp;
    } else {
      printf("Position already occupied. Cheater!\r\n");
    }
  }
}

static bool tictactoe_line_check(struct TTTState *state, uint32_t x1, uint32_t y1, uint32_t x2, uint32_t y2,
                                 uint32_t x3, uint32_t y3) {

  if (state->board[x1][y1] == state->board[x2][y2] && state->board[x2][y2] == state->board[x3][y3]) {
    if (state->board[x1][y1] != SLOT_EMPTY) {
      // Game over!
      enum BoardPos winner = state->board[x1][y1];
      if (state->mode == MODE_P_VS_C) {
        if (winner == SLOT_P1) {
          printf("You won! Congrats!\r\n");
        } else {
          printf("The computer won! Ouch!\r\n");
        }
      } else {
        printf("Player %i won! Congrats!\r\n", winner == SLOT_P1 ? 1 : 2);
      }

      // Short animation:
      for (size_t i = 0; i < 6; i++) {
        state->board[x1][y1] = SLOT_EMPTY;
        state->board[x2][y2] = SLOT_EMPTY;
        state->board[x3][y3] = SLOT_EMPTY;
        tictactoe_update_leds(state, false);
        HAL_Delay(100);
        state->board[x1][y1] = winner;
        state->board[x2][y2] = winner;
        state->board[x3][y3] = winner;
        tictactoe_update_leds(state, false);
        HAL_Delay(100);
      }
      return true;
    }
  }

  return false;
}

static bool tictactoe_check_for_game_over(struct TTTState *state) {
  // Row
  for (uint32_t row = 0; row < 3; row++) {
    if (tictactoe_line_check(state, 0, row, 1, row, 2, row)) {
      return true;
    }
  }
  // Column
  for (uint32_t col = 0; col < 3; col++) {
    if (tictactoe_line_check(state, col, 0, col, 1, col, 2)) {
      return true;
    }
  }
  // Diag
  if (tictactoe_line_check(state, 0, 0, 1, 1, 2, 2)) {
    return true;
  }
  if (tictactoe_line_check(state, 0, 2, 1, 1, 2, 0)) {
    return true;
  }

  // Check for draw:
  bool is_draw = true;
  for (size_t x = 0; x < 3; x++) {
    for (size_t y = 0; y < 3; y++) {
      if (state->board[x][y] == SLOT_EMPTY) {
        is_draw = false;
      }
    }
  }

  if (is_draw) {
    printf("Draw!\r\n");
    // Short animation:
    for (size_t i = 0; i < 6; i++) {
      for (size_t x = 0; x < 3; x++) {
        for (size_t y = 0; y < 3; y++) {
          state->board[x][y] = SLOT_P1;
        }
      }
      tictactoe_update_leds(state, false);
      HAL_Delay(100);
      for (size_t x = 0; x < 3; x++) {
        for (size_t y = 0; y < 3; y++) {
          state->board[x][y] = SLOT_P2;
        }
      }
      tictactoe_update_leds(state, false);
      HAL_Delay(100);
    }
    return true;
  }

  return false;
}

static void tictactoe_reset(struct TTTState *state) {

  printf("\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n");
  printf("--- Welcome to TicTacToe! ---\r\n\r\n");

  // Reset board:
  for (size_t x = 0; x < 3; x++) {
    for (size_t y = 0; y < 3; y++) {
      state->board[x][y] = SLOT_EMPTY;
    }
  }

  tictactoe_update_leds(state, false);

  // Select game mode:
  switch (tictactoe_user_prompt("Play against another player (1) or the computer (2)? [1/2]:", 2)) {
    case 1:
      state->mode = MODE_P_VS_P;
      printf("Selected: Player vs Player\r\n");
      break;
    case 2:
      state->mode = MODE_P_VS_C;
      printf("Selected: Player vs Computer\r\n");
      break;
  }

  if (state->mode == MODE_P_VS_C) {
    // Select computer difficulty:
    switch (tictactoe_user_prompt("Difficulty: easy (1), medium (2), or hard (3)? [1/2/3]:", 3)) {
      case 1:
        state->computer_difficulty = EASY;
        printf("Selected: Easy\r\n");
        break;
      case 2:
        state->computer_difficulty = MEDIUM;
        printf("Selected: Medium\r\n");
        break;
      case 3:
        state->computer_difficulty = HARD;
        printf("Selected: Hard\r\n");
        break;
    }
  }

  // Select starting player:
  char *prompt;
  if (state->mode == MODE_P_VS_C) {
    prompt = "Who starts? Player one (1) or the computer (2)? [1/2]:";
  } else {
    prompt = "Who starts? Player one player two? [1/2]:";
  }
  switch (tictactoe_user_prompt(prompt, 2)) {
    case 1:
      state->current_player = P1;
      break;
    case 2:
      state->current_player = P2;
      break;
  }
  if (state->mode == MODE_P_VS_C) {
    printf("Selected: %s starts\r\n", state->current_player == P1 ? "Player" : "Computer");
  } else {
    printf("Selected: Player %s starts\r\n", state->current_player == P1 ? "1" : "2");
  }

  tictactoe_update_leds(state, true);
}

static bool tictactoe_update(struct TTTState *state) {

  // Get the next move from the user or from the computer:
  uint32_t next_move;
  if (state->mode == MODE_P_VS_C && state->current_player == P2) {
    // Get next move from computer.
    next_move = tictactoe_calculate_computer_move(state);
  } else {
    // Get next move from player.
    next_move = tictactoe_get_user_move(state);
  }

  // Apply the next move to the board:
  if (state->current_player == P1) {
    state->board[IDX_TO_X(next_move)][IDX_TO_Y(next_move)] = SLOT_P1;
  } else {
    state->board[IDX_TO_X(next_move)][IDX_TO_Y(next_move)] = SLOT_P2;
  }

  // Check for a winner:
  if (tictactoe_check_for_game_over(state)) {
    return false; // Stop
  }

  // Move to next player:
  state->current_player = state->current_player == P1 ? P2 : P1;
  return true; // Continue
}

// ===== Game Loop =============================================================

// Game loop. Never returns.
void tictactoe_run(void) {
  struct TTTState state;

  // Main game loop:
  while (1) {
    // Reset and wait for start:
    tictactoe_reset(&state);

    // Run the game until game-over:
    while (tictactoe_update(&state)) { // Returns true while the game is active.
      tictactoe_update_leds(&state, true);
    };
  }
}
