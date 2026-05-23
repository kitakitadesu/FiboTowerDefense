#pragma once
#include <vector>

class Board {
public:
  Board(int n);
  Board(int n, int m);

  char GetCell(int r, int c) const;
  void SetCell(int r, int c, char new_val);

  void Print() const;

protected:
  bool IsValid(int r, int c) const;
  int num_rows_ = 0;
  int num_cols_ = 0;
  std::vector<std::vector<char>> data_;
};
