/**
 * @file snake.c
 * @brief
 * @author Philipp Schilk 2023
 * Super basic implementation of the classic 'snake' game.
 * Relies on interface functions in main.c, see main.h.
 */
#include "snake.h"
#include "main.h"
#include <string.h>
// ===== Type Definitions ======================================================

#define SNAKE_HEIGHT 3
#define SNAKE_WIDTH  4

#define SNAKE_UP    0
#define SNAKE_RIGHT 1
#define SNAKE_DOWN  2
#define SNAKE_LEFT  3

// Uncomment to have the snake wrap around when hitting the edge of the display,
// instead of ending the game.
// #define SNAKE_ALLOW_WRAPAROUND

struct SnakeState {
  // Current snake state:
  uint32_t direction; // Direction of travel, one of SNAKE_UP,
                      // SNAKE_RIGHT, SNAKE_DOWN, SNAKE LEFT.
  uint32_t snake_x;   // Snake head position.
  uint32_t snake_y;
  uint32_t length; // Snake length.

  // Current apple position:
  uint32_t apple_x;
  uint32_t apple_y;

  // Board:
  // Each uint32_t represents one tile on the display. If it is equal to zero,
  // there is no snake body present on that tile. If it is non-zero, a snake
  // body is present and will remain present for that many update cycles.
  uint32_t board[SNAKE_WIDTH][SNAKE_HEIGHT];
};

// ===== Functions =============================================================

static void snake_reset(struct SnakeState *state);
static void snake_random_apple_position(struct SnakeState *state);
static void snake_render(struct SnakeState *state, bool render_apple);
static void snake_gather_input(bool *left, bool *right);
static void snake_game_over_animation(void);
static int snake_update(struct SnakeState *state);

// Reset the game, and wait for the user to press enter.
static void snake_reset(struct SnakeState *state) {
  // Reset board:
  memset(state->board, 0, sizeof(state->board));

  // Place snake at starting position, at length 1:
  state->snake_x = 1;
  state->snake_y = 1;
  state->direction = SNAKE_RIGHT;
  state->length = 1;
  state->board[1][1] = 1;

  // Render this initial state to the display:
  snake_render(state, false);

  // Wait for 'ENTER' button to be pressed & released:
  bool btn_enter = false;
  do {
    btn_enter = snake_read_enter_button();
  } while (btn_enter == false);
  do {
    btn_enter = snake_read_enter_button();
  } while (btn_enter == true);

  // Determine random (initial) apple position:
  snake_random_apple_position(state);

  snake_render(state, true);
}

// Find a new random position for the apple.
static void snake_random_apple_position(struct SnakeState *state) {
  // We use the LSBs of the current systick counter as a
  // source of randmoness to determine an initial (x,y) apple position.
  // If this position is already occupied by the snake,
  // we iterate forward from this position until we find one that
  // is empty.

  uint32_t tick = HAL_GetTick();
  uint32_t x = (tick % (SNAKE_WIDTH * SNAKE_HEIGHT)) % SNAKE_WIDTH;
  uint32_t y = (tick % (SNAKE_WIDTH * SNAKE_HEIGHT)) / SNAKE_WIDTH;

  size_t cnt = 0;
  while (state->board[x][y] > 0) {
    cnt++;

    x = (x + 1) % SNAKE_WIDTH;

    if (x == 0) {
      y = (y + 1) % SNAKE_HEIGHT;
    }

    if (cnt > SNAKE_WIDTH * SNAKE_HEIGHT) {
      // No more empty space! Don't place an apple.
      return;
    }
  }

  state->apple_x = x;
  state->apple_y = y;
}

// Convert an (x,y) coordinate to a shift register position for
// rendering.
#define DISPLAY_OFFSET(_x_, _y_) ((_x_) + SNAKE_WIDTH * (_y_))

// Render the current board, and call the display update function.
static void snake_render(struct SnakeState *state, bool render_apple) {
  uint32_t red = 0;
  uint32_t green = 0;

  // Red dot at apple position:
  if (render_apple) {
    red |= 0x1 << DISPLAY_OFFSET(state->apple_x, state->apple_y);
  }

  // Green snake:
  for (size_t x = 0; x < SNAKE_WIDTH; x++) {
    for (size_t y = 0; y < SNAKE_HEIGHT; y++) {
      if (state->board[x][y] > 0) {
        green |= 0x1 << DISPLAY_OFFSET(x, y);
      }
    }
  }

  // Send to display:
  snake_write_display(red, green);
}

// Receive control inputs for the user.
static void snake_gather_input(bool *left, bool *right) {
  // The amount of time we spend gathering input is based on
  // the current poti position, allowing the poti position
  // to control the speed of the game.
  uint32_t adc_reading = snake_read_adc();

  // Map the ADC positions to a range of 15 to 50 sampling periods:
  uint32_t sampling_periods = 15 + (35 * adc_reading) / (4096);

  *left = false;
  *right = false;

  // Read initial button state:
  bool btn_l_prev = snake_read_left_button();
  bool btn_r_prev = snake_read_right_button();

  for (size_t i = 0; i < sampling_periods; i++) {
    // Read buttons:
    bool btn_l = snake_read_left_button();
    bool btn_r = snake_read_right_button();

    // Rising-edge detection:
    if (btn_l && !btn_l_prev) {
      *left = true;
    }
    if (btn_r && !btn_r_prev) {
      *right = true;
    }

    btn_r_prev = btn_r;
    btn_l_prev = btn_l;
    HAL_Delay(10);
  }

  // Prevent simultaneous left and right inputs:
  if (*left && *right) {
    *left = false;
    *right = false;
  }
}

// A quick game-over animation:
static void snake_game_over_animation(void) {
  snake_write_display(0xF00, 0x000);
  HAL_Delay(200);
  snake_write_display(0xFF0, 0x000);
  HAL_Delay(200);
  snake_write_display(0xFFF, 0x000);
  HAL_Delay(200);
  snake_write_display(0x969, 0x000);
  HAL_Delay(1000);
}

// Update the game:
static int snake_update(struct SnakeState *state) {

  // Gather input:
  bool left, right;
  snake_gather_input(&left, &right);

  // Determine the next snake rotation:
  if (left) {
    state->direction = state->direction == SNAKE_UP ? SNAKE_LEFT : state->direction - 1;
  } else if (right) {
    state->direction = state->direction == SNAKE_LEFT ? SNAKE_UP : state->direction + 1;
  }

  // Determine next snake position:
  uint32_t next_x, next_y;
  switch (state->direction) {
    case SNAKE_UP:
#ifndef SNAKE_ALLOW_WRAPAROUND
      if (state->snake_y == (SNAKE_HEIGHT - 1)) {
        return false;
      }
#endif
      next_x = state->snake_x;
      next_y = (state->snake_y + 1) % SNAKE_HEIGHT;
      break;
    case SNAKE_DOWN:
#ifndef SNAKE_ALLOW_WRAPAROUND
      if (state->snake_y == 0) {
        return false;
      }
#endif
      next_x = state->snake_x;
      next_y = state->snake_y == 0 ? (SNAKE_HEIGHT - 1) : state->snake_y - 1;
      break;
    case SNAKE_LEFT:
#ifndef SNAKE_ALLOW_WRAPAROUND
      if (state->snake_x == 0) {
        return false;
      }
#endif
      next_x = state->snake_x == 0 ? (SNAKE_WIDTH - 1) : state->snake_x - 1;
      next_y = state->snake_y;
      break;
    case SNAKE_RIGHT:
#ifndef SNAKE_ALLOW_WRAPAROUND
      if (state->snake_x == (SNAKE_WIDTH - 1)) {
        return false;
      }
#endif
      next_x = (state->snake_x + 1) % SNAKE_WIDTH;
      next_y = state->snake_y;
      break;
    default:
      // How did we get there?
      while (1) {
      }
  }

  // Check for collisions:
  if (state->board[next_x][next_y] != 0) {
    // Collison!!
    return 0;
  }

  // Check for apple consumption:
  int apple_consumed = 0;
  if (next_x == state->apple_x && next_y == state->apple_y) {
    // Apple consumed.
    state->length += 1;
    apple_consumed = 1;
  }

  // 'Decay' snake:
  // This is skipped if we consumed an apple (in addition to increasing
  // the length of the snake), as all the snake body tiles already on
  // the board would still decay after the previous length.
  if (!apple_consumed) {
    for (size_t x = 0; x < SNAKE_WIDTH; x++) {
      for (size_t y = 0; y < SNAKE_HEIGHT; y++) {
        state->board[x][y] -= state->board[x][y] == 0 ? 0 : 1;
      }
    }
  }

  // Actually advance the snake:
  state->board[next_x][next_y] = state->length;
  state->snake_x = next_x;
  state->snake_y = next_y;

  // Determine the new apple position of the previous one was consumed.
  // This is done at the very end once the board has been fully updated
  // to ensure it gets placed in a legal position.
  if (apple_consumed) {
    snake_random_apple_position(state);
  }

  return 1; // Not game over.
}

// ===== Game Loop =============================================================

// Game loop. Never returns.
void snake_run(void) {
  struct SnakeState state;

  // Main game loop:
  while (1) {
    // Reset and wait for start:
    snake_reset(&state);

    // Run the game until game-over:
    while (snake_update(&state)) { // Returns true while the game is active.
      snake_render(&state, true);
    };

    // Render the game-over anmiation:
    snake_game_over_animation();
  }
}
