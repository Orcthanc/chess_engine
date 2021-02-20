#pragma once

#include <type_traits>
#include <array>
#include <stdint.h>
#include <string>
#include <sstream>
#include <cstring>

#include <iostream>

namespace chess {
	enum Piece: int {
		PieceNone = 0,
		PiecePawn = 1,
		PieceKnight = 2,
		PieceBishop = 3,
		PieceRook = 4,
		PieceQueen = 5,
		PieceKing = 6,

		PieceWhite = 8,
		PieceBlack = 16,
		PieceError = 32,
	};

	enum Sides: int {
		SideNone = 0,
		SideQueenWhite = 1,
		SideKingWhite = 2,
		SideQueenBlack = 4,
		SideKingBlack = 8,
	};

	inline char toLetter( Piece p ){
		switch( (int)p ){
			case PiecePawn | PieceWhite:
				return 'P';
			case PieceKnight | PieceWhite:
				return 'N';
			case PieceBishop | PieceWhite:
				return 'B';
			case PieceRook | PieceWhite:
				return 'R';
			case PieceQueen | PieceWhite:
				return 'Q';
			case PieceKing | PieceWhite:
				return 'K';
			case PiecePawn | PieceBlack:
				return 'p';
			case PieceKnight | PieceBlack:
				return 'n';
			case PieceBishop | PieceBlack:
				return 'b';
			case PieceRook | PieceBlack:
				return 'r';
			case PieceQueen | PieceBlack:
				return 'q';
			case PieceKing | PieceBlack:
				return 'k';
			case PieceError:
				return 'e';
			case PieceNone:
				return '0';
			default:
				return 'E';
		}
	}

	inline std::string toSymbol( Piece p ){
		switch( (int)p ){
			case PiecePawn | PieceWhite:
				return "♟︎";
			case PieceKnight | PieceWhite:
				return "♞";
			case PieceBishop | PieceWhite:
				return "♝";
			case PieceRook | PieceWhite:
				return "♜";
			case PieceQueen | PieceWhite:
				return "♛";
			case PieceKing | PieceWhite:
				return "♚";
			case PiecePawn | PieceBlack:
				return "♙";
			case PieceKnight | PieceBlack:
				return "♘";
			case PieceBishop | PieceBlack:
				return "♗";
			case PieceRook | PieceBlack:
				return "♖";
			case PieceQueen | PieceBlack:
				return "♕";
			case PieceKing | PieceBlack:
				return "♔";
			case PieceError:
				return "e";
			case PieceNone:
				return "　";
			default:
				return "E";
		}
	}

	inline Piece fromLetter( char c ){
		switch( c ){
			case 'P':
				return static_cast<Piece>( PiecePawn | PieceWhite );
			case 'N':
				return static_cast<Piece>( PieceKnight | PieceWhite );
			case 'B':
				return static_cast<Piece>( PieceBishop | PieceWhite );
			case 'R':
				return static_cast<Piece>( PieceRook | PieceWhite );
			case 'Q':
				return static_cast<Piece>( PieceQueen | PieceWhite );
			case 'K':
				return static_cast<Piece>( PieceKing | PieceWhite );
			case 'p':
				return static_cast<Piece>( PiecePawn | PieceBlack );
			case 'n':
				return static_cast<Piece>( PieceKnight | PieceBlack );
			case 'b':
				return static_cast<Piece>( PieceBishop | PieceBlack );
			case 'r':
				return static_cast<Piece>( PieceRook | PieceBlack );
			case 'q':
				return static_cast<Piece>( PieceQueen | PieceBlack );
			case 'k':
				return static_cast<Piece>( PieceKing | PieceBlack );
			case 'e':
				return static_cast<Piece>( PieceError );
			case '0':
				return static_cast<Piece>( PieceNone );
			default:
				return static_cast<Piece>( PieceError );
		}
	}

	struct Board {
		std::array<Piece, 120> board;
		std::pair<uint8_t, uint8_t> en_passant;
		bool whiteTurn = true;
		Sides castling;
		size_t halfmove = 0;
		size_t fullmove = 1;

		Board(){
			std::memset( board.data(), PieceError, 120 );

			for( size_t y = 0; y < 8; ++y ){
				for( size_t x = 0; x < 8; ++x ){
					operator[]( y )[x] = PieceNone;
				}
			}
			en_passant = std::make_pair<uint8_t, uint8_t>( 255, 255 );
			castling = Sides( SideQueenWhite | SideKingWhite | SideQueenBlack | SideKingBlack );
		}

		inline Piece* operator[]( uint8_t rank ){
			return &board[20 + rank * 10];
		}

		std::string toFEN(){
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

			if( en_passant == std::make_pair<uint8_t, uint8_t>( 255, 255 )){
				res << '-';
			} else {
				res << static_cast<char>( 'a' + en_passant.first ) << static_cast<int>( en_passant.second );
			}

			res << ' ' << halfmove << ' ' << fullmove;

			return res.str();
		}


		bool fromFen( const std::string& fen ){

			std::memset( board.data(), PieceError, 120 );

			for( size_t y = 0; y < 8; ++y ){
				for( size_t x = 0; x < 8; ++x ){
					operator[]( y )[x] = PieceNone;
				}
			}
			const char* letter = fen.c_str();

			uint8_t rank = 0, file = 0;
			for( ;; ){
				if( !letter )
					return false;

				if( *letter == ' ' )
					break;
				if( *letter == '/' ){
					++letter;
					++rank;
					file = 0;
					continue;
				}

				if( isdigit( *letter )){
					file += *letter - '0';
					++letter;
				} else {
					operator[]( rank )[file++] = fromLetter( *letter++ );
				}
			}

#define chess_expect( a, b ) if( a != b ) return false;
//#define chess_expect( a, b ) if( a != b ) throw std::runtime_error( "asdf" );

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
				en_passant = std::make_pair<uint8_t, uint8_t>( 255, 255 );
				++letter;
			} else {
				uint8_t rank = *letter++ - 'a';
				uint8_t file = *letter++ - '0';

				en_passant = std::make_pair( rank, file );
			}

			chess_expect( *letter++, ' ' );

			char* rest;

			halfmove = strtol( letter, &rest, 10 );

			fullmove = strtol( rest, nullptr, 10 );

			return true;
		}

		std::string pretty_string(){
			std::stringstream res;

			for( size_t y = 0; y < 8; ++y ){
				for( size_t x = 0; x < 8; ++x ){
					res << toSymbol( operator[]( y )[x] );
				}
				res << "\n";
			}

			return res.str();
		}
	};
}
