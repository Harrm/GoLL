#pragma once
#include <array>



class Logic {
public:
    static constexpr int kFieldSize = 512;

    enum class State {Dead, Alive};
    using Field = std::array<std::array<State, kFieldSize>, kFieldSize>;

    Logic();

    void setField(const Field& field);
    void randomizeField();
    void updateField(const std::array<std::array<bool, kFieldSize>, kFieldSize>& substrate);
    void killCell(size_t x, size_t y);
    void reviveCell(size_t x, size_t y);
    const Field& getField() const;

private:
    short getAliveAdjacentCellsNum(short pos_x, short pos_y) const;

    Field field;

};
