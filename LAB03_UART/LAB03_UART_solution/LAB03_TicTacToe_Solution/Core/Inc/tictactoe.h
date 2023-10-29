/**
 * @file tictactoe.h
 * @brief A simple game of Tic-Tac-Toe
 * @author Philipp Schilk, 2023
 */
#ifndef TICTACTOE_H_
#define TICTACTOE_H_

#include <stdint.h>

// ===== Type Definitions ======================================================

enum GameMode {
  MODE_P_VS_P,
  MODE_P_VS_C,
};

enum Difficulty {
  EASY,
  MEDIUM,
  HARD,
};

enum BoardPos {
  SLOT_EMPTY,
  SLOT_P1,
  SLOT_P2,
};

enum Player {
  P1,
  P2
};

struct TTTState {
  // Game state:
  enum BoardPos board[3][3];
  enum Player current_player;

  // Game settings:
  enum GameMode mode;
  enum Difficulty computer_difficulty;
};

// ===== Function Prototypes ===================================================

void tictactoe_write_display(uint32_t red, uint32_t green);
void tictactoe_write_leds(uint32_t led1, uint32_t led2);
uint32_t tictactoe_get_input(void);
uint32_t tictactoe_get_random_number(void);
void tictactoe_run(void);

#endif /* TICTACTOE_H_ */
