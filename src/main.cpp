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
		board.fromFen( temp );

		std::cout << board.toFEN() << std::endl;
		std::cout << board.pretty_string() << std::endl;
	}
}
