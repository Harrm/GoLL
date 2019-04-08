#include "logic.h"
#include <time.h>
#include <assert.h>
#include <random>
#include <iostream>

Logic::Logic() {

}



void Logic::setField(const Field& field) {
    this->field = field;
}



void Logic::randomizeField() {
    std::uniform_int_distribution<uint32_t> dist(0, 100);
    std::default_random_engine rand;

    for(short c = 0; c < kFieldSize; c++) {
        for(short i = 0; i < kFieldSize; i++) {
          uint32_t r = dist(rand);
          field[c][i] = r < 70 ? State::Alive : State::Dead;
        }
    }
}



void Logic::updateField(const std::array<std::array<bool, kFieldSize>, kFieldSize>& substrate) {
    char adjacents_num[kFieldSize][kFieldSize];

    for(short c = 0; c < kFieldSize; c++) {
        for(short i = 0; i < kFieldSize; i++) {
            adjacents_num[c][i] = getAliveAdjacentCellsNum(c , i);
        }
    }

    for(short c = 0; c < kFieldSize; c++) {
        for(short i = 0; i < kFieldSize; i++) {
            if(substrate[c][i]) {
              if (field[c][i] == State::Dead and
                  adjacents_num[c][i] > 2) {
                field[c][i] = State::Alive;
              }
            } else {
              if (field[c][i] == State::Dead and
                  adjacents_num[c][i] > 3) {
                field[c][i] = State::Alive;
              }
            }
        }
    }

    for(short c = 0; c < kFieldSize; c++) {
        for(short i = 0; i < kFieldSize; i++) {
            if(substrate[c][i]) {
              if (field[c][i] == State::Alive and
                  (adjacents_num[c][i] < 2 or
                      adjacents_num[c][i] > 4)) {
                field[c][i] = State::Dead;
              }
            } else {
              if (field[c][i] == State::Alive and
                  (adjacents_num[c][i] < 2 or
                      adjacents_num[c][i] > 3)) {
                field[c][i] = State::Dead;
              }
            }
        }
    }
}


void Logic::killCell(size_t x, size_t y) {
    field[y][x] = State::Dead;
}

void Logic::reviveCell(size_t x, size_t y) {
    field[y][x] = State::Alive;
}

const Logic::Field& Logic::getField() const {
    return field;
}



short Logic::getAliveAdjacentCellsNum(short x, short y) const {
    assert(x < kFieldSize and x >= 0 and y < kFieldSize and y >= 0);
    short alive_cells = 0;

    if(field[x][y] != State::Dead) {
        alive_cells--;
    }

    if(x != 0 and y != 0 and x != kFieldSize-1 and y != kFieldSize-1) {
        for(short c = -1; c <= 1; c++) {
            for(short i = -1; i <= 1; i++) {
                if(field[x+c][y+i] != State::Dead) {
                    alive_cells++;
                }
            }
        }
    } else {
        for(short c = -1; c <= 1; c++) {
            for(short i = -1; i <= 1; i++) {
                auto checking_x = c+x;
                auto checking_y = i+y;
                if(checking_x == -1) {
                    checking_x = kFieldSize-1;
                } else if(checking_x == kFieldSize) {
                    checking_x = 0;
                }
                if(checking_y == -1) {
                    checking_y = kFieldSize-1;
                } else if(checking_y == kFieldSize) {
                    checking_y = 0;
                }
                if(field[checking_x][checking_y] != State::Dead) {
                    alive_cells++;
                }
            }
        }
    }
    return alive_cells;
}
