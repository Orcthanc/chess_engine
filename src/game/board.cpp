#include "game/board.hpp"

#include <cstring>
#include <sstream>
#include <iostream>

using namespace chess;

Board::Board(){
 reset();
}

std::string Board::toFEN(){
	int counter = 0;
	std::stringstream res;

	for( size_t y = 0; y < 8; ++y ){
		Piece* rank = (*this)[y];
		for( size_t x = 0; x < 8; ++x ){
			if( !rank[x] ){
				++counter;
			} else {
				if( counter ){
					res << counter;
					counter = 0;
				}

				res << toLetter( rank[x] );
			}
		}
		if( counter ){
			res << counter;
			counter = 0;
		}

		if( y != 7 )
			res << '/';
	}

	res << ' ' << ( whiteTurn ? 'w' : 'b' ) << ' ';


	if( !castling )
		res << '-';
	else {
		if( castling & SideKingWhite )
			res << 'K';
		if( castling & SideQueenWhite )
			res << 'Q';
		if( castling & SideKingBlack )
			res << 'k';
		if( castling & SideQueenWhite )
			res << 'q';
	}

	res << ' ';

	if( en_passant == Field::nullField()){
		res << '-';
	} else {
		res << en_passant.toAlgebraic();
	}

	res << ' ' << halfmove << ' ' << fullmove;

	return res.str();

}

bool Board::fromFen( const std::string& fen ){
	reset();

	const char* letter = fen.c_str();

	uint8_t rank = 0, file = 0;
	for( ;; ){
		if( !*letter )
			return false;

		if( *letter == ' ' )
			break;
		else if( *letter == '/' ){
			++letter;
			++rank;
			file = 0;
		} else if( isdigit( *letter )){
			file += *letter - '0';
			++letter;
		} else {
			Piece temp = fromLetter( *letter++ );
			pieces[temp].emplace_back( Field().fromNumbers( file, rank + 1 ));
			operator[]( rank )[file++] = temp;
		}
	}

#define chess_expect( a, b ) if( a != b ) return false;

	chess_expect( *letter++, ' ' );

	if( *letter == 'w' ){
		whiteTurn = true;
	} else if( *letter == 'b' ){
		whiteTurn = false;
	} else {
		return false;
	}

	++letter;

	chess_expect( *letter++, ' ' );

	castling = SideNone;

	if( *letter != '-' ){
		while( *letter != ' ' ){
			if( !*letter )
				return false;
			switch( *letter++ ){
				case 'Q':
					castling = static_cast<Sides>( castling | SideQueenWhite );
					break;
				case 'K':
					castling = static_cast<Sides>( castling | SideKingWhite );
					break;
				case 'q':
					castling = static_cast<Sides>( castling | SideQueenBlack );
					break;
				case 'k':
					castling = static_cast<Sides>( castling | SideKingBlack );
			}
		}
	}else {
		++letter;
	}
	chess_expect( *letter++, ' ' );

	if( *letter == '-' ){
		en_passant = Field::nullField();
		++letter;
	} else {
		if( !*letter )
			return false;
		uint8_t rank = *letter++ - 'a';

		if( !*letter )
			return false;
		uint8_t file = *letter++ - '0';

		en_passant = Field().fromNumbers( rank, file );
	}

	chess_expect( *letter++, ' ' );

	char* rest;

	halfmove = strtol( letter, &rest, 10 );

	if( letter == rest )
		return false;

	char* temp;

	fullmove = strtol( rest, &temp, 10 );

	if( rest == temp )
		return false;

#undef chess_expect

	return true;
}

std::string Board::pretty_string(){
	std::stringstream res;

	for( size_t y = 0; y < 8; ++y ){
		for( size_t x = 0; x < 8; ++x ){
			res << toSymbol( operator[]( y )[x] );
		}
		res << "\n";
	}

	return res.str();
}

void Board::reset(){
	std::memset( board.data(), PieceError, 120 );

	for( size_t y = 0; y < 8; ++y ){
		for( size_t x = 0; x < 8; ++x ){
			operator[]( y )[x] = PieceNone;
		}
	}

	en_passant = Field::nullField();
	whiteTurn = true;
	castling = Sides( SideQueenWhite | SideKingWhite | SideQueenBlack | SideKingBlack );
	halfmove = 0;
	fullmove = 1;
	pieces = {};
}
