#pragma once
#include "Board.hpp"
#include "City.hpp"
#include "Color.hpp"


namespace pandemic{

    class Player {

        protected:
        Board& board;
        City city;
        std::string p_role;

        public:
        Player(pandemic::Board& b, pandemic::City c): board(b), city(c){}

        Player& take_card(City c);
        Player& drive(City c);
        virtual Player& fly_direct(City c);
        Player& fly_charter(City c);
        Player& fly_shuttle(City c);
        virtual Player& build();
        virtual Player& discover_cure(Color c);
        virtual Player& treat(City c);
        std::string role(){return p_role;}
    };
}