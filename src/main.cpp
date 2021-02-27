#include "game/board.hpp"

#include <iostream>
#include <iomanip>

using namespace chess;

int main( int argc, char** argv ){
	Board board;

	//std::cout << board.toFEN() << std::endl;
	
	std::string temp;

	for(;;){
		std::getline( std::cin, temp );
		if( temp.empty() )
			return 0;
		//std::cout << board.fromFen( temp ) << std::endl;
		board.fromFen( temp );

		//std::cout << board.en_passant.field << std::endl;

		std::cout << board.toFEN() << std::endl;
		std::cout << board.pretty_string() << std::endl;

		for( auto& piece: board.pieces ){
			for( auto& f: piece.second )
				std::cout << toLetter( piece.first ) << " : " << f.toAlgebraic() << " " << f.field << std::endl;
		}

		for( size_t y = 0; y < 12; ++y ){
			for( size_t x = 0; x < 10; ++ x ){
				std::cout << std::setw( 2 ) << board.board[y * 10 + x] << " ";
			}
			std::cout << std::endl;
		}

		std::cout << Field().fromAlgebraic( "a1" ).field << std::endl;
		std::cout << Field().fromAlgebraic( "a2" ).field << std::endl;
		std::cout << Field().fromAlgebraic( "b1" ).field << std::endl;
		std::cout << toLetter( board[Field().fromAlgebraic( "a1" )]) << std::endl;
		std::cout << toLetter( board[Field().fromAlgebraic( "a2" )]) << std::endl;
		std::cout << toLetter( board[Field().fromAlgebraic( "b1" )]) << std::endl;

		std::cout << board.en_passant.field << std::endl;
	}
}
