#pragma once

#include <type_traits>
#include <array>
#include <stdint.h>
#include <string>
#include <unordered_map>
#include <vector>

#include <compare>

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

	struct Field {
		int field;

		std::string toAlgebraic(){
			std::string temp = "a";
			temp.at( 0 ) = static_cast<char>( field % 10 - 1 + 'a' );
			return temp
				+ std::to_string( field / 10 - 1 );
		}

		Field& fromAlgebraic( const std::string& s ){
			const char* temp = s.c_str();

			field = ( *temp++ - 'a' ) * 10;
			field += ( *temp - '0' ) + 1;

			return *this;
		}

		Field& fromNumbers( uint8_t x, uint8_t y ){
			field = 11 + 10 * y + x;
			return *this;
		}

		auto operator<=>( const Field& other ) const = default;

		static constexpr Field nullField(){
			return Field{ std::numeric_limits<int>::max() };
		}
	};

	struct Board {
		std::array<Piece, 120> board;
		std::unordered_map<Piece, std::vector<Field>> pieces;
		Field en_passant;
		bool whiteTurn = true;
		Sides castling;
		size_t halfmove = 0;
		size_t fullmove = 1;

		Board();

		inline Piece* operator[]( uint8_t rank ){
			return &board[20 + rank * 10];
		}

		inline Piece operator[]( const Field& f ){
			return board[f.field];
		}

		std::string toFEN();
		bool fromFen( const std::string& fen );

		std::string pretty_string();

		void reset();
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
			case '0':
				return PieceNone;
			default:
				return PieceError;
		}
	}
}
