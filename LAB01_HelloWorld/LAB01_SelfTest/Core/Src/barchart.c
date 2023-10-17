/**
 * @file barchart.c
 * @brief Simple ASCII barchart.
 * @author Philipp Schilk 2023
 */
#include "barchart.h"
#include <stdio.h>

void print_bar_char(uint32_t lvl, uint32_t max) {
  lvl = lvl > max ? max : lvl;
  printf("[");
  for (unsigned int i = 0; i < lvl; i++) {
    printf("#");
  }
  for (unsigned int i = 0; i < (max - lvl); i++) {
    printf(" ");
  }
  printf("]");
}
