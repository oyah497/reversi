#ifndef MOVE_H
#define MOVE_H

typedef struct intpair
{
  int first;
  int second;
} IntPair;

int is_legal_move(const int side, const IntPair square);
int place_disk(const int side, const IntPair square);
int my_place_disk(const int side, const IntPair square, int board_tmp[8][8]);
int generate_all_legal_moves(const int side, IntPair legal_moves[]);
int my_generate_all_legal_moves(const int side, IntPair legal_moves[], int board_tmp[8][8]);

#endif
