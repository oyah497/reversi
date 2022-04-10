#include <stdio.h>
#include <stdlib.h>
#include "move.h"

#define INF 10000


int board[8][8];  // 8x8 squares: 0 -> empty; 1 -> black; -1 -> white
int past_board[8][8];
int eval_board[8][8] = {{45, -11, 4, -1, -1, 4, -11, 45}, 
                        {-11, -16, -1, -3, -3, -1, -16, -11}, 
                        {4, -1, 2, -1, -1, 2, -1, 4}, 
                        {-1, -3, -1, 0, 0, -1, -3, -1},
                        {-1, -3, -1, 0, 0, -1, -3, -1},
                        {4, -1, 2, -1, -1, 2, -1, 4},
                        {-11, -16, -1, -3, -3, -1, -16, -11},
                        {45, -11, 4, -1, -1, 4, -11, 45},
                        };

int max_node(int d, int a, int b, int turn, int board_tmp[8][8], int flag_pass);
int min_node(int d, int a, int b, int turn, int board_tmp[8][8], int flag_pass);



int count_disks(){
  int tmp = 0;
  for (int x = 0; x < 8; x++){
    for (int y = 0; y < 8; y++){
      tmp += board[x][y];
    }
  }
  return tmp;
}

static void init_board()
{
  int x, y;
  for (y = 0; y < 8; y++) 
    for (x = 0; x < 8; x++)
      board[x][y] = 0;

  board[4][3] = board[3][4] =  1;  // black disks
  board[3][3] = board[4][4] = -1;  // white disks
}

static void print_board()
{
  int x, y;
  puts("\n  a b c d e f g h");
  for (y = 0; y < 8; y++) {
    printf("%d ", y + 1);
    for (x = 0; x < 8; x++) {
      const int d = board[x][y];
      char c = '-';
      if (d ==  1) c = 'B';
      if (d == -1) c = 'W';
      printf("%c ", c);
    }
    putchar('\n');
  }
  putchar('\n');
}

static void matta(){
  for (int i = 0; i < 8; i++){
    for (int j = 0; j < 8; j++){
      board[i][j] = past_board[i][j];
    }
  }
}

static void reserve_board(){
  for (int i = 0; i < 8; i++){
    for (int j = 0; j < 8; j++){
      past_board[i][j] = board[i][j];
    }
  }
}

static void copy_board(int board_origin[8][8], int board_copy[8][8]){
  for (int i = 0; i < 8; i++){
    for (int j = 0; j < 8; j++){
      board_copy[i][j] = board_origin[i][j];
    }
  }
}

int eval_func(int board[8][8]){
  int sum = 0;
  for (int x = 0; x < 8; x++){
    for (int y = 0; y < 8; y++){
      sum += board[x][y] * eval_board[x][y];
    }
  }

  return sum;
}

int max_node(int d, int a, int b, int turn, int board_tmp[8][8], int flag_pass){
  if (d == 0){
    return eval_func(board_tmp);
  }
  IntPair legal_moves[60];
  const int nmoves = my_generate_all_legal_moves(-turn, legal_moves, board_tmp);
  if (nmoves == -1){
    int count = count_disks(board_tmp);
    if (count > 0) return INF;
    else if (count == 0) return 0;
    else return -INF;
  }
  if (nmoves == 0){
    if (flag_pass){
      int count = count_disks(board_tmp);
      if (count > 0) return INF;
      else if (count == 0) return 0;
      else return -INF;
    }
    return min_node(d, a, b, -turn, board_tmp, 1);
  }

  for (int i = 0; i < nmoves; i++){
    int board_next[8][8];
    copy_board(board_tmp, board_next);
    my_place_disk(-turn, legal_moves[i], board_next);
    int v = min_node(d-1, a, b, -turn, board_next, 0);
    if (v > a){
      a = v;
      if (a > b) break;
    }
  }
  return a;

}


int min_node(int d, int a, int b, int turn, int board_tmp[8][8], int flag_pass){
  if (d == 0){
    return eval_func(board_tmp);
  }
  IntPair legal_moves[60];
  const int nmoves = my_generate_all_legal_moves(-turn, legal_moves, board_tmp);
  if (nmoves == -1){
    int count = count_disks(board_tmp);
    if (count > 0) return INF;
    else if (count == 0) return 0;
    else return -INF;
  }
  if (nmoves == 0){
    if (flag_pass){
      int count = count_disks(board_tmp);
      if (count > 0) return INF;
      else if (count == 0) return 0;
      else return -INF;
    }
    return max_node(d, a, b, -turn, board_tmp, 1);
  }

  for (int i = 0; i < nmoves; i++){
    int board_next[8][8];
    copy_board(board_tmp, board_next);
    my_place_disk(-turn, legal_moves[i], board_next);
    int v = max_node(d-1, a, b, -turn, board_next, 0);
    if (v < b){
      b = v;
      if (a > b) break;
    }
  }
  return b;
}


int main(int argc, char **argv)
{
  const int human_side = (argc >= 2) ? atoi(argv[1]) : 0; // 1 -> 先手, -1 -> 後手, 0 -> AI vs AI

  init_board();

  int turn;
  int flag_matta = 0;
  int flag_pass = 0;
  int d = 6;

  for (turn = 1;; turn *= -1) {
    print_board();

    IntPair legal_moves[60];
    const int nmoves = generate_all_legal_moves(turn, legal_moves);
    if (nmoves == -1) break;     // no empty square
    if (nmoves ==  0) { // pass
      printf("turn = %d, move = Pass\n", turn);
      if (flag_pass){
        break;
      }
      flag_pass = 1;
      continue;  
    }
    flag_pass = 0;

    IntPair move;
    if (turn == human_side) {
      while (1) {
	      printf("Where? ");
	      char buf[1000];
	      scanf("%s", buf);
        if (buf[0] == 'm'){
          matta();
          flag_matta = 1;
          turn *= -1;
          printf("matta!!\n\n");
          break;
        }
	      move.first  = buf[0] - 'a';
	      move.second = buf[1] - '1';
	      if (is_legal_move(turn, move)) {
          reserve_board();
          break;
        }
	      printf("%s is not a legal move!\n\n", buf);
      }
      if (flag_matta){
        flag_matta = 0;
        continue;
      }
    } 
    else if (turn == 1){
      //move = legal_moves[0];  // choose the first legal move
      int best = -INF + 1;
      for (int i = 0; i < nmoves; i++){
        int tmp_board[8][8];
        copy_board(board, tmp_board);
        my_place_disk(turn, legal_moves[i], tmp_board);
        //int d = 5;
        int a = max_node(d, -INF-1, INF+1, turn, tmp_board, 0);
        //copy_board(tmp_board, board);
        if (a > best){
          move = legal_moves[i];
          best = a;
        }
      }
      printf("turn = %d, move = %c%c, evaluation = %d\n", turn, 'a' + move.first, '1' + move.second, best);
    }
    else{
      //move = legal_moves[0];  // choose the first legal move
      int best = INF + 1;
      for (int i = 0; i < nmoves; i++){
        int tmp_board[8][8];
        copy_board(board, tmp_board);
        my_place_disk(turn, legal_moves[i], tmp_board);
        //int d = 5;
        int b = min_node(d, -INF-1, INF+1, turn, tmp_board, 0);
        //copy_board(tmp_board, board);
        if (b < best){
          move = legal_moves[i];
          best = b;
        }
      }
      printf("turn = %d, move = %c%c, evaluation = %d\n", turn, 'a' + move.first, '1' + move.second, best);
    }
    place_disk(turn, move);
  }

  int judge = count_disks();
  if (judge > 0){
    printf("black win\n");
  }
  else if (judge < 0){
    printf("white win\n");
  }
  else {
    printf("draw\n");
  }

  return 0;
}
