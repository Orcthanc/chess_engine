#include "game/board.hpp"

#include <iostream>

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
				std::cout << toLetter( piece.first ) << " : " << f.toAlgebraic() << std::endl;
		}
	}
}
