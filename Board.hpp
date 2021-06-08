#pragma once
#include "City.hpp"
#include "Color.hpp"
#include <map>

namespace pandemic{

    class Board {

        std::map<City, int> disease_cubes; 

        public:
        Board(){}

        int& operator[](const City&);
        static bool is_clean();
        void remove_cures();
        friend std::ostream& operator<<(std::ostream& out, const Board& b);
    };
}