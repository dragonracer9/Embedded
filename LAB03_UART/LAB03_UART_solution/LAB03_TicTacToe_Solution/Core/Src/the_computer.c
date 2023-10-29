#include "the_computer.h"
#include "stm32l476xx.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

static uint32_t sum(uint32_t *array, size_t len) {
  uint32_t acc = 0;
  for (size_t i = 0; i < len; i++) {
    acc += array[i];
  }
  return acc;
}

static size_t argmax(uint32_t *array, size_t len) {
  uint32_t max_arg = 0;
  for (size_t i = 0; i < len; i++) {
    if (array[i] > array[max_arg]) {
      max_arg = i;
    }
  }
  return max_arg;
}

static void score_win(enum BoardPos b1, enum BoardPos b2, enum BoardPos b3, uint32_t *score1, uint32_t *score2,
                      uint32_t *score3) {
  if (b1 == SLOT_EMPTY && b2 == SLOT_P2 && b3 == SLOT_P2) {
    *score1 += 100000;
  }
  if (b1 == SLOT_P2 && b2 == SLOT_EMPTY && b3 == SLOT_P2) {
    *score2 += 100000;
  }
  if (b1 == SLOT_P2 && b2 == SLOT_P2 && b3 == SLOT_EMPTY) {
    *score3 += 100000;
  }
}

static void score_block(enum BoardPos b1, enum BoardPos b2, enum BoardPos b3, uint32_t *score1, uint32_t *score2,
                        uint32_t *score3) {
  if (b1 == SLOT_EMPTY && b2 == SLOT_P1 && b3 == SLOT_P1) {
    *score1 += 10000;
  }
  if (b1 == SLOT_P1 && b2 == SLOT_EMPTY && b3 == SLOT_P1) {
    *score2 += 10000;
  }
  if (b1 == SLOT_P1 && b2 == SLOT_P1 && b3 == SLOT_EMPTY) {
    *score3 += 10000;
  }
}

static void score_if_free(enum BoardPos b, uint32_t *score, uint32_t additional_score) {
  if (b == SLOT_EMPTY) {
    *score += additional_score;
  }
}

static void score_moves(struct TTTState *state, uint32_t *scores) {
  enum BoardPos(*b)[3][3] = &state->board;
  // scores moves if they guratane a win:
  score_win((*b)[0][0], (*b)[1][0], (*b)[2][0], &scores[0], &scores[1], &scores[2]); // rows
  score_win((*b)[0][1], (*b)[1][1], (*b)[2][1], &scores[3], &scores[4], &scores[5]);
  score_win((*b)[0][2], (*b)[1][2], (*b)[2][2], &scores[6], &scores[7], &scores[8]);
  score_win((*b)[0][0], (*b)[0][1], (*b)[0][2], &scores[0], &scores[3], &scores[6]); // cols
  score_win((*b)[1][0], (*b)[1][1], (*b)[1][2], &scores[1], &scores[4], &scores[7]);
  score_win((*b)[2][0], (*b)[2][1], (*b)[2][2], &scores[2], &scores[5], &scores[8]);
  score_win((*b)[0][0], (*b)[1][1], (*b)[2][2], &scores[0], &scores[4], &scores[8]); // diags
  score_win((*b)[0][2], (*b)[1][1], (*b)[2][0], &scores[6], &scores[4], &scores[2]);
  // scores moves if they block a player 1 win:
  score_block((*b)[0][0], (*b)[1][0], (*b)[2][0], &scores[0], &scores[1], &scores[2]); // rows
  score_block((*b)[0][1], (*b)[1][1], (*b)[2][1], &scores[3], &scores[4], &scores[5]);
  score_block((*b)[0][2], (*b)[1][2], (*b)[2][2], &scores[6], &scores[7], &scores[8]);
  score_block((*b)[0][0], (*b)[0][1], (*b)[0][2], &scores[0], &scores[3], &scores[6]); // cols
  score_block((*b)[1][0], (*b)[1][1], (*b)[1][2], &scores[1], &scores[4], &scores[7]);
  score_block((*b)[2][0], (*b)[2][1], (*b)[2][2], &scores[2], &scores[5], &scores[8]);
  score_block((*b)[0][0], (*b)[1][1], (*b)[2][2], &scores[0], &scores[4], &scores[8]); // diags
  score_block((*b)[0][2], (*b)[1][1], (*b)[2][0], &scores[6], &scores[4], &scores[2]);
  // Prefer the middle:
  score_if_free((*b)[1][1], &scores[4], 1000);
  // Prefer the sides:
  score_if_free((*b)[1][0], &scores[1], 100);
  score_if_free((*b)[0][1], &scores[3], 100);
  score_if_free((*b)[2][1], &scores[5], 100);
  score_if_free((*b)[1][2], &scores[7], 100);
  // Lastly play the corners:
  score_if_free((*b)[0][0], &scores[0], 10);
  score_if_free((*b)[2][0], &scores[2], 10);
  score_if_free((*b)[0][2], &scores[6], 10);
  score_if_free((*b)[2][2], &scores[8], 10);
}

uint32_t tictactoe_calculate_computer_move(struct TTTState *state) {
  // Tic Tac Toe is a solved game (But this implementation is not 100% perfect
  // and can be beaten - even on hard!)
  // To implement a computer with different difficulties we do the following:
  // 1) Give all the possible moves a score depending on how optimal they are
  // 2) Sort the moves based on their score
  // 3) Pick a move:
  //   - Hard: Pick the best move.
  //   - Medium: 80% chance to pick the best move, 20% chance to pick the second best move.
  //   - Easy: 60% chance to pick the best move, 30% chance to pick the second-best move,
  //           10% chance to pick the third-best move.

  // --- First, score each available move: ---
  uint32_t scores[9] = {0};
  score_moves(state, scores);

  // --- Next find the best, second-best and third-best move: ---
  uint32_t move_best, move_secondbest, move_thirdbest = 0;
  bool have_secondbest, have_thirdbest = false;

  // Make flat array:

  move_best = argmax(scores, 9);
  scores[move_best] = 0;

  if (sum(scores, 9) > 0) {
    // Second move available.
    have_secondbest = true;
    move_secondbest = argmax(scores, 9);
    scores[move_secondbest] = 0;
  }

  if (sum(scores, 9) > 0) {
    // Third move available.
    have_thirdbest = true;
    move_thirdbest = argmax(scores, 9);
  }

  // --- Pick a move based on the current difficulty: ---

  uint32_t randnum = tictactoe_get_random_number() % 99; // random number [0,100)

  switch (state->computer_difficulty) {
    default:
    case HARD:
      return move_best + 1;
    case MEDIUM:
      if (randnum >= 80 && have_secondbest) {
        return move_secondbest + 1;
      } else {
        return move_best + 1;
      }
    case EASY:
      if (randnum <= 10 && have_thirdbest) {
        return move_thirdbest + 1;
      } else if (randnum <= 40 && have_secondbest) {
        return move_secondbest + 1;
      } else {
        return move_best + 1;
      }
  }
}
