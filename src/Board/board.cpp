#include "Board/board.h"
#include <iostream>

namespace {
  constexpr char kDefaultChar = '0';
}

// Constructor with one input
Board::Board(int n) : Board(n,n) {}

// Constructor with two inputs
Board::Board(int n, int m) {
  data_ = std::vector<std::vector<char>>(n, std::vector<char>(m, ' '));
  num_cols_ = m;
  num_rows_ = n;
}

bool Board::IsValid(int r, int c) const {
  return !(r < 0 || r>= num_rows_ || c<0 || c>= num_cols_);
}

char Board::GetCell(int r, int c) const {
  return IsValid(r,c) ? data_[r][c] : kDefaultChar;
}

void Board::SetCell(int r, int c, char new_val) {
  if(IsValid(r,c)) {
    data_[r][c] = new_val;
  }
}

void Board::Print() const {
  const auto print_line = [&]() {
    for(int j = 0; j < num_cols_; ++j) {
      std::cout << "+-";
    }
    std::cout << "+" << std::endl;  
  };
  print_line();
  for(int i = 0; i < num_rows_; ++i) {
    for(int j = 0; j < num_cols_; ++j) {
      std::cout << '|' << data_[i][j];
    }
    std::cout << '|' << std::endl;
    print_line();
  }
}
