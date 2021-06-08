/**
 * 
 * AUTHORS: Noy Osi
 * 
 */

#include "doctest.h"
#include "Board.hpp"
#include "City.hpp"
#include "Color.hpp"
#include "Player.hpp"
#include "Researcher.hpp"
#include "Scientist.hpp"
#include "FieldDoctor.hpp"
#include "GeneSplicer.hpp"
#include "OperationsExpert.hpp"
#include "Dispatcher.hpp"
#include "Medic.hpp"
#include "Virologist.hpp"

#include <iostream>
#include <stdexcept>
#include <string>

using namespace pandemic;
using namespace std;


TEST_CASE("check is_clean & operator[]") {
	Board board;
	CHECK(board.is_clean()==true);

	//blue
	board[City::Milan]=1;
    CHECK(board[City::Milan]==1);

	//yellow
	board[City::Miami]=1;
    CHECK(board[City::Miami]==1);

	//red
	board[City::Tokyo]=1;
    CHECK(board[City::Tokyo]==1);

	//black
	board[City::Tehran]=1;
    CHECK(board[City::Tehran]==1);

	//change the level
	board[City::Tehran]=2;
    CHECK(board[City::Tehran]==2);

	//Negative
	CHECK_THROWS(board[City::Chicago]=-1);

	CHECK(board.is_clean()==false);

	board[City::Milan]=0;
	board[City::Miami]=0;
	board[City::Tokyo]=0;
	board[City::Tehran]=0;
	CHECK(board.is_clean()==true);
}
/**************************************************************************************************************
 * Can only drive to connected city 
 */
TEST_CASE("check drive") {
Board board;
OperationsExpert oe {board, City::NewYork};

CHECK_NOTHROW(oe.drive(City::Madrid)); //drive to connected city with the same color
CHECK_NOTHROW(oe.drive(City::SaoPaulo)); //drive to connected city with diff color
CHECK_THROWS(oe.drive(City::MexicoCity)); //try to drive to an unconnected city with the same color
CHECK_THROWS(oe.drive(City::Bangkok)); //try to drive to an unconnected city with diff color
}
/**************************************************************************************************************
 * Only operationsExpert can build without discard any card
 */
TEST_CASE("check build") {
Board board;

OperationsExpert oe {board, City::NewYork};
oe.take_card(City::NewYork);
CHECK_NOTHROW(oe.build()); //research station & no need to discard NY card

//try to build another station
OperationsExpert oe1 {board, City::NewYork};
CHECK_NOTHROW(oe1.build()); //there is already a research station & no need to discard NY card

//build without a card
OperationsExpert oe2 {board, City::Paris};
CHECK_NOTHROW(oe2.build()); 


//others
Dispatcher d {board, City::Miami};
CHECK_THROWS(d.build()); //doesn't have the Miami card
d.take_card(City::Miami);
CHECK_NOTHROW(d.build()); //research station & discard Miami card

//try to build another station
Researcher r {board, City::Miami};
r.take_card(City::Miami);
CHECK_NOTHROW(r.build()); //there is already a research station & no need to discard Miami card
}
/**************************************************************************************************************
 * Only when Dispatcher is in research station, he can fly_direct without discard any card
 */
TEST_CASE("check fly_direct") {
Board board;

OperationsExpert oe {board, City::NewYork};
oe.take_card(City::LosAngeles)
	.take_card(City::London);

CHECK_NOTHROW(oe.fly_direct(City::LosAngeles)); //discard the LosAngeles card
CHECK_THROWS(oe.fly_direct(City::Bogota)); //don't have the card of Bogota


//
Dispatcher d {board, City::NewYork};
CHECK_THROWS(d.fly_direct(City::Bogota)); //there is no research station in NY

OperationsExpert b {board, City::NewYork};
b.build(); //build research station in NY without throw any card

CHECK_NOTHROW(d.fly_direct(City::Moscow)); //can fly direct without the Moscow card
CHECK_THROWS(d.fly_direct(City::NewYork)); //can't fly to NY cause no research station at Moscow & no card


d.take_card(City::NewYork);
d.fly_direct(City::NewYork); //Moscow to NY & discard the NY card

CHECK_NOTHROW(oe.fly_direct(City::London)); //fly & discard the london card
CHECK_NOTHROW(d.fly_direct(City::London)); //not discard the london card cause he's coming from NY station
}
/**************************************************************************************************************/
TEST_CASE("check fly_charter") {
Board board;

OperationsExpert oe {board, City::NewYork};
CHECK_THROWS(oe.fly_charter(City::London)); //don't have the card of NY

oe.take_card(City::NewYork)
  .take_card(City::London);

CHECK_NOTHROW(oe.fly_charter(City::London)); //discard NY card
CHECK_NOTHROW(oe.fly_charter(City::LosAngeles)); //discard london card
CHECK_THROWS(oe.fly_charter(City::London)); //don't have the card of LA

oe.take_card(City::Paris);
CHECK_THROWS(oe.fly_charter(City::Paris)); //don't have the card of LA
}
/**************************************************************************************************************
 * Fly from one research station to another
 */
TEST_CASE("check fly_shuttle") {
Board board;

OperationsExpert b1 {board, City::NewYork};
b1.take_card(City::NewYork);
b1.build(); //research station in NY

OperationsExpert b2 {board, City::LosAngeles};
b2.take_card(City::LosAngeles);
b2.build(); //research station in LA

CHECK_NOTHROW(b1.fly_shuttle(City::LosAngeles)); //from one research station to another
CHECK_NOTHROW(b2.fly_shuttle(City::NewYork)); 

CHECK_THROWS(b1.fly_shuttle(City::HongKong)); //from one research station to a regular city

OperationsExpert b {board, City::HongKong};
CHECK_THROWS(b.fly_shuttle(City::NewYork)); //from a regular city to a research station 
CHECK_THROWS(b.fly_shuttle(City::Baghdad)); //both not a research station
}
/**************************************************************************************************************
 * need to be in research station to discover_cure (specific color)
 */ 
TEST_CASE("check discover_cure") {
Board board;
board[City::NewYork]=5;

OperationsExpert oe {board, City::NewYork};
CHECK_THROWS(oe.discover_cure(Color::Blue)); //NY not a research station
oe.build(); //build research station in NY without throw any card

oe.take_card(City::Chicago)
	.take_card(City::Washington)
	.take_card(City::Milan);

CHECK_THROWS(oe.discover_cure(Color::Blue)); //no 5 cards

oe.take_card(City::SanFrancisco)
	.take_card(City::Madrid);

CHECK_NOTHROW(oe.discover_cure(Color::Blue)); //discard 5 blue cards 


CHECK_THROWS(oe.fly_direct(City::Chicago)); //doesn't have Chicago card
Dispatcher d {board, City::NewYork};
CHECK_NOTHROW(d.fly_direct(City::Chicago)); //can fly without discard a card


//again
oe.take_card(City::Chicago)
	.take_card(City::Washington)
	.take_card(City::Milan)
	.take_card(City::SanFrancisco)
	.take_card(City::Madrid);

CHECK_NOTHROW(oe.discover_cure(Color::Blue)); //already found a cure so the cards stays


CHECK_NOTHROW(oe.fly_direct(City::Chicago)); //discard the chicago card
oe.take_card(City::Chicago);
CHECK_NOTHROW(oe.fly_charter(City::NewYork)); //discard the chicago card
}
/**************************************************************************************************************
 * GeneSplicer can discover_cure with 5 cards - but diff colors
 */
TEST_CASE("check genesplicer_discover_cure") {
Board board;
board[City::NewYork] = 5;

GeneSplicer g {board, City::NewYork};
CHECK_THROWS(g.discover_cure(Color::Blue)); //no research station & no cards
OperationsExpert oe {board, City::NewYork};
oe.build(); //build research station in NY without throw any card

g.take_card(City::Chicago) //blue
	.take_card(City::Taipei) //red
	.take_card(City::Delhi) //black
	.take_card(City::SanFrancisco) //blue
	.take_card(City::Bogota); //yellow

CHECK_NOTHROW(g.discover_cure(Color::Blue)); //should work without 5 blue cards & discard 5 cards

//again
g.take_card(City::Chicago) 
	.take_card(City::Taipei) 
	.take_card(City::Delhi) 
	.take_card(City::SanFrancisco) 
	.take_card(City::Bogota); 

CHECK_NOTHROW(g.discover_cure(Color::Blue)); //already found a cure so the cards stays
}
/**************************************************************************************************************
 * Researcher can discover_cure everywhere & not only in research station
 */
TEST_CASE("check researcher_discover_cure") {
Board board;
board[City::London]=5;
Researcher r {board, City::London}; //no research station

r.take_card(City::Chicago)
	.take_card(City::Washington)
	.take_card(City::Milan);

CHECK_THROWS(r.discover_cure(Color::Blue)); //no 5 cards

r.take_card(City::SanFrancisco)
	.take_card(City::Madrid);

CHECK_NOTHROW(r.discover_cure(Color::Blue)); //no need resarch station & discard 5 cards

//again
r.take_card(City::Chicago)
	.take_card(City::Washington)
	.take_card(City::Milan)
	.take_card(City::SanFrancisco)
	.take_card(City::Madrid);

CHECK_NOTHROW(r.discover_cure(Color::Blue)); //already found a cure so the cards stays
}
/**************************************************************************************************************
 * Scientist can discover_cure with n cards
 */
TEST_CASE("check scientist_discover_cure") {
Board board;
board[City::NewYork]=4;

Scientist s {board, City::NewYork, 4};
CHECK_THROWS(s.discover_cure(Color::Blue)); //NY not a research station & no cards

OperationsExpert oe {board, City::NewYork};
oe.build(); //build research station in NY without throw any card

s.take_card(City::Chicago)
	.take_card(City::Washington)
	.take_card(City::Milan);

CHECK_THROWS(s.discover_cure(Color::Blue)); //no 4 cards

s.take_card(City::SanFrancisco);

CHECK_NOTHROW(s.discover_cure(Color::Blue)); //discard 4 blue cards 

//again
s.take_card(City::Chicago)
	.take_card(City::Washington)
	.take_card(City::Milan)
	.take_card(City::SanFrancisco);

CHECK_NOTHROW(s.discover_cure(Color::Blue)); //already found a cure so the cards stays
}
/**************************************************************************************************************
* If discover_cure -> When medic enter (drive/fly) a city, with the same color ->
* the level reset to 0 - without any treat
 */
TEST_CASE("check medic_with_discover_cure") {
Board board;
board[City::NewYork]=5;

OperationsExpert oe {board, City::NewYork};
CHECK_THROWS(oe.discover_cure(Color::Blue)); //NY not a research station
oe.build(); //build research station in NY without throw any card

oe.take_card(City::Chicago)
	.take_card(City::Washington)
	.take_card(City::Milan);

CHECK_THROWS(oe.discover_cure(Color::Blue)); //no 5 cards

oe.take_card(City::SanFrancisco)
	.take_card(City::Madrid);

CHECK_NOTHROW(oe.discover_cure(Color::Blue)); //discard 5 blue cards 


//medic enter 
board[City::Montreal]=5;
board[City::London]=5;

Medic m1 {board, City::NewYork};
m1.take_card(City::Montreal);
CHECK_NOTHROW(m1.fly_direct(City::Montreal)); //discard Montreal card
CHECK(board[City::Montreal]==0);

Medic m2 {board, City::NewYork};
CHECK_NOTHROW(m2.drive(City::London)); //connected city
CHECK(board[City::London]==0);
}
/**************************************************************************************************************
 * Level--
 */
TEST_CASE("check treat_without_discover_cure") {
Board board;

board[City::NewYork]=5;
OperationsExpert oe {board, City::NewYork};
CHECK_NOTHROW(oe.treat(City::NewYork));
CHECK(board[City::NewYork]==4);

//treat only his city
OperationsExpert oe1 {board, City::Miami};
CHECK_THROWS(oe1.treat(City::NewYork)); //Can't treat other city
CHECK(board[City::NewYork]==4); //not change

board[City::Miami]=1;
Player p {board, City::Miami};
CHECK_NOTHROW(p.treat(City::Miami));
CHECK(board[City::Miami]==0);
}
/**************************************************************************************************************
 * If the level is 0 & still try to treat -> throw
 */
TEST_CASE("check treat_throws") {
Board board;

board[City::NewYork]=0;
OperationsExpert oe {board, City::NewYork};
CHECK_THROWS(oe.treat(City::NewYork)); 

board[City::Miami]=0;
Player p {board, City::Miami};
CHECK_THROWS(p.treat(City::Miami)); 
}
/**************************************************************************************************************
 * Only virologist can treat his city & everywhere - but need to throw a card
 */
TEST_CASE("check treat_virologist") {
Board board;

Virologist v {board, City::HoChiMinhCity};
board[City::Chicago]=3;

//treat other city
CHECK_THROWS(v.treat(City::Chicago)); //doesn't have the chicago card
v.take_card(City::Chicago);
CHECK_NOTHROW(v.treat(City::Chicago)); //discard chicago card
CHECK(board[City::Chicago]==2);

//treat his city
board[City::HoChiMinhCity]=2;
CHECK_THROWS(v.treat(City::HoChiMinhCity)); //doesn't have the HoChiMinhCity card
v.take_card(City::HoChiMinhCity);
CHECK_NOTHROW(v.treat(City::HoChiMinhCity)); //discard HoChiMinhCity card
CHECK(board[City::HoChiMinhCity]==1);


//others
OperationsExpert oe {board, City::Chicago};
CHECK_THROWS(oe.treat(City::HoChiMinhCity)); //Can only treat his city
CHECK(board[City::HoChiMinhCity]==1); //not change
}
/**************************************************************************************************************
 * Medic reduce the level to 0 in his city
 */
TEST_CASE("check treat_medic") {
Board board;

board[City::Miami]=4;
Medic m {board, City::Miami};
CHECK_NOTHROW(m.treat(City::Miami));
CHECK(board[City::Miami]==0);
}
/**************************************************************************************************************
 * FieldDoctor treat his city & connected cities - without discard any card
 */
TEST_CASE("check treat_FieldDoctor") {
Board board;

board[City::Miami]=4;
FieldDoctor fd {board, City::Miami};
CHECK_NOTHROW(fd.treat(City::Miami)); //treat his city & not discard any card
CHECK(board[City::Miami]==3);

board[City::MexicoCity]=2;
CHECK_NOTHROW(fd.treat(City::MexicoCity)); //MexicoCity connected to miami
CHECK(board[City::MexicoCity]==1);

board[City::Madrid]=2;
CHECK_THROWS(fd.treat(City::Madrid)); //not connected to miami
CHECK(board[City::Madrid]==2);


//others
OperationsExpert oe {board, City::Miami};
CHECK_THROWS(oe.treat(City::MexicoCity)); //only FD can treat in connected city 
CHECK(board[City::MexicoCity]==1); //not change
}
/**************************************************************************************************************
 * If discover_cure -> reset the city level to 0
 * discover_cure & then treat
 */
TEST_CASE("check treat_with_discover_cure") {
Board board;
board[City::NewYork]=5;

OperationsExpert oe {board, City::NewYork};
CHECK_THROWS(oe.discover_cure(Color::Blue)); //NY not a research station
oe.build(); //build research station in NY without throw any card

oe.take_card(City::Chicago)
	.take_card(City::Washington)
	.take_card(City::Milan)
	.take_card(City::SanFrancisco)
	.take_card(City::Madrid);

CHECK_NOTHROW(oe.discover_cure(Color::Blue)); //discard 5 blue cards 


CHECK_NOTHROW(oe.treat(City::NewYork));
CHECK(board[City::NewYork]==0);
}