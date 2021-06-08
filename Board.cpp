#include "Board.hpp"
#include "City.hpp"

#include <iostream>

using namespace std;
using namespace pandemic;


int& Board::operator[](const City& c){
    return disease_cubes[c]; //the level
}

bool Board::is_clean() {
    return true;
}

void Board::remove_cures(){}

std::ostream& pandemic::operator<<(std::ostream& out, const Board& b) {
    return out << "" << std::endl;
}