#pragma once
#include <raylib.h>
#include <string>
#include <cstdint>


/*The Rules of movement
* 1- no piece can jump over another piece except the knight***
* 2- the special move castling is not implemented
* 3- the special move en passant is not implemented
* 4- the special move promotion is not implemented
* 5- the capture of pieces is not implemented
* 6- the check and checkmate rules are not implemented
* 7- the stalemate rule is not implemented
* 8- the draw by repetition rule is not implemented
* 9- the fifty-move rule is not implemented
* 10- the threefold repetition rule is not implemented
* 11- the insufficient material rule is not implemented
* 12- the time control rule is not implemented
*/

#ifndef S1
#define S1

//-- Window 
namespace Window {
	const char* Icon_local = "picecs\\wn.png";
	constexpr int Pixal = 90; 
	constexpr int SrcWidth = (Pixal * 8);
	constexpr int SrcHeigth = (Pixal * 8);
	const char* Title = "Chess Game";
}
//-- TO save some time
#define	IsMousePressed (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)|| IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                    
typedef struct RecMin {
	int16_t x = 0;
	int16_t y = 0;
	int16_t _2D = (int16_t)(Window::Pixal);
	bool operator == (const RecMin& rm2) const {
		return (x == rm2.x && y == rm2.y);
	};
	int operator >> (const RecMin& rm2) const {
		if (rm2.x - x == 0) {
			return 0;
		}
		else if (rm2.x > x ) {
			return Window::Pixal;
		}
		else{
			return - Window::Pixal;
		}
	};
	int operator << (const RecMin& rm2) const {
		if (rm2.y - y == 0) {
			return 0;
		}
		else if (rm2.y > y ) {
			return Window::Pixal;
		}
		else {
			return -Window::Pixal;
		}
	};
    
	
} RecMin;
void DrawRecMin(RecMin rm, Color C) {
	DrawRectangle(rm.x, rm.y, rm._2D, rm._2D, C);
}
bool CheckRecMinVec(RecMin rm, Vector2 v2) {
	bool x = (rm.x <= v2.x && (rm.x + rm._2D) >= v2.x);
	bool y = (rm.y <= v2.y && (rm.y + rm._2D) >= v2.y);
	return (x && y);
}
//-- Game consts block
using turn_t = bool;
using state_t = unsigned char;
using type_t = unsigned char;
//-- Game consts
namespace Game {
	constexpr bool MOVED = true, UNMOVED = false;
	constexpr turn_t PLAYER_1 = true;
	constexpr turn_t PLAYER_2 = false;
	constexpr state_t Selected = 1;
	constexpr state_t UnSelected = 0;
	constexpr Color PieceSelectColor = { 74,84,45,255 };
	constexpr Color RecLightColor = { 74, 112, 61,255 };
	constexpr Color RecDarkColor = { 238, 232, 213 ,255 };
	constexpr type_t _Wb = 0, _Wn = 1, _Wk = 2, _Wq = 3, _Wp = 4, _Wr = 5;
	constexpr type_t _Bb = 6, _Bn = 7, _Bk = 8, _Bq = 9, _Bp = 10, _Br = 11;
}
//-- Board class
class Board {
public:
	RecMin _Board[8][8];
	Board() {
		for (int row = 0; row < 8; row++) {
			for (int col = 0; col < 8; col++) {
				_Board[row][col].x = (int16_t)Window::Pixal * (7 - col);
				_Board[row][col].y = (int16_t)Window::Pixal * (7 - row);
				_Board[row][col]._2D = (int16_t)Window::Pixal;
			}
		}
	}
	void DrawBoard()const {
		for (int row = 0; row < 8; row++) {
			for (int col = 0; col < 8; col++) {
				if ((row + col) % 2 == 0) {
					DrawRecMin(_Board[row][col], Game::RecLightColor);
				}
				else {
					DrawRecMin(_Board[row][col], Game::RecDarkColor);
				}
			}
		}
	}
};
//-- Piece class
class Piece {
public:
	bool IsMoved = Game::UNMOVED;
	state_t state = Game::UnSelected;
	type_t typ = { 0 };
	RecMin pos = { 0 };
	Texture tex = { 0 };

	void setPiece(RecMin rec, type_t ty) {


		this->pos = rec;
		this->typ = ty;

		std::string path = "C:\\Users\\Good\\Downloads\\picecs\\";

		if (typ == Game::_Wp) { path += "wp.png"; }
		else if (typ == Game::_Wn) { path += "wn.png"; }
		else if (typ == Game::_Wb) { path += "wb.png"; }
		else if (typ == Game::_Wk) { path += "wk.png"; }
		else if (typ == Game::_Wq) { path += "wq.png"; }
		else if (typ == Game::_Wr) { path += "wr.png"; }
		else if (typ == Game::_Bp) { path += "bp.png"; }
		else if (typ == Game::_Bn) { path += "bn.png"; }
		else if (typ == Game::_Bb) { path += "bb.png"; }
		else if (typ == Game::_Br) { path += "br.png"; }
		else if (typ == Game::_Bk) { path += "bk.png"; }
		else if (typ == Game::_Bq) { path += "bq.png"; }

		Image tex = LoadImage(path.c_str());
		ImageResize(&tex, Window::Pixal, Window::Pixal);
		this->tex = LoadTextureFromImage(tex);
		UnloadImage(tex);
	}
};
//-- functions 
//-- Initialization - Drawing
void Init_16Piece(
	Board* Board,
	Piece Setof16Piece[],
	bool TypeofSet
) {
	Setof16Piece[0].setPiece((TypeofSet) ? Board->_Board[1][0] : Board->_Board[6][0], (TypeofSet) ? Game::_Wp : Game::_Bp);
	Setof16Piece[1].setPiece((TypeofSet) ? Board->_Board[1][1] : Board->_Board[6][1], (TypeofSet) ? Game::_Wp : Game::_Bp);
	Setof16Piece[2].setPiece((TypeofSet) ? Board->_Board[1][2] : Board->_Board[6][2], (TypeofSet) ? Game::_Wp : Game::_Bp);
	Setof16Piece[3].setPiece((TypeofSet) ? Board->_Board[1][3] : Board->_Board[6][3], (TypeofSet) ? Game::_Wp : Game::_Bp);
	Setof16Piece[4].setPiece((TypeofSet) ? Board->_Board[1][4] : Board->_Board[6][4], (TypeofSet) ? Game::_Wp : Game::_Bp);
	Setof16Piece[5].setPiece((TypeofSet) ? Board->_Board[1][5] : Board->_Board[6][5], (TypeofSet) ? Game::_Wp : Game::_Bp);
	Setof16Piece[6].setPiece((TypeofSet) ? Board->_Board[1][6] : Board->_Board[6][6], (TypeofSet) ? Game::_Wp : Game::_Bp);
	Setof16Piece[7].setPiece((TypeofSet) ? Board->_Board[1][7] : Board->_Board[6][7], (TypeofSet) ? Game::_Wp : Game::_Bp);
	Setof16Piece[8].setPiece((TypeofSet) ? Board->_Board[0][0] : Board->_Board[7][0], (TypeofSet) ? Game::_Wr : Game::_Br);
	Setof16Piece[9].setPiece((TypeofSet) ? Board->_Board[0][1] : Board->_Board[7][1], (TypeofSet) ? Game::_Wn : Game::_Bn);
	Setof16Piece[10].setPiece((TypeofSet) ? Board->_Board[0][2] : Board->_Board[7][2], (TypeofSet) ? Game::_Wb : Game::_Bb);
	Setof16Piece[11].setPiece((TypeofSet) ? Board->_Board[0][3] : Board->_Board[7][3], (TypeofSet) ? Game::_Wk : Game::_Bk);
	Setof16Piece[12].setPiece((TypeofSet) ? Board->_Board[0][4] : Board->_Board[7][4], (TypeofSet) ? Game::_Wq : Game::_Bq);
	Setof16Piece[13].setPiece((TypeofSet) ? Board->_Board[0][5] : Board->_Board[7][5], (TypeofSet) ? Game::_Wb : Game::_Bb);
	Setof16Piece[14].setPiece((TypeofSet) ? Board->_Board[0][6] : Board->_Board[7][6], (TypeofSet) ? Game::_Wn : Game::_Bn);
	Setof16Piece[15].setPiece((TypeofSet) ? Board->_Board[0][7] : Board->_Board[7][7], (TypeofSet) ? Game::_Wr : Game::_Br);
}
void DrawAllPieces(
	Piece Wpieces[],
	Piece Bpieces[]
) {
	for (int i = 0; i < 16;i++) {
		if (Wpieces[i].state) {
			DrawTexture(Wpieces[i].tex, Wpieces[i].pos.x, Wpieces[i].pos.y, Game::PieceSelectColor);
		}
		else {
			DrawTexture(Wpieces[i].tex, Wpieces[i].pos.x, Wpieces[i].pos.y, WHITE);
		}
	}
	for (int i = 0; i < 16;i++) {
		if (Bpieces[i].state) {
			DrawTexture(Bpieces[i].tex, Bpieces[i].pos.x, Bpieces[i].pos.y, Game::PieceSelectColor);
		}
		else {
			DrawTexture(Bpieces[i].tex, Bpieces[i].pos.x, Bpieces[i].pos.y, WHITE);
		}
	}
}
//Selection
Piece* SelectPiece(
	Piece SetofPieces_16_Piece[]
) {
	for (int indexofPiece = 0; indexofPiece < 16; indexofPiece++) {
		if (IsMousePressed && CheckRecMinVec(SetofPieces_16_Piece[indexofPiece].pos, GetMousePosition())) {
			SetofPieces_16_Piece[indexofPiece].state = Game::Selected;
			return &SetofPieces_16_Piece[indexofPiece];
		}
	}
	return nullptr;
};
RecMin* SelectPosition(
	Board BoardofChess_8X8size
) {
	for (int indexofrow = 0; indexofrow < 8; indexofrow++) {
		for (int indexofcol = 0; indexofcol < 8; indexofcol++) {
			if (IsMousePressed && CheckRecMinVec(BoardofChess_8X8size._Board[indexofrow][indexofcol], GetMousePosition())) {
				return &BoardofChess_8X8size._Board[indexofrow][indexofcol];
			}
		}
	}
	return nullptr;
};
//-- Movement
bool TheRulesofcapture(
	Piece* CurrentPiece,
	RecMin* TheSelectedPosition
) {
	return 0;
}
bool IsTherePieceInPath(
	Piece* MP,
	RecMin* MPos,
	Piece SetP[],
	Piece SetP2[]
) {
	if (MP->typ == Game::_Wn) {
		return true;
	}
	int16_t DeltaX = static_cast<int16_t> (MP->pos >> *MPos);
	int16_t DeltaY = static_cast<int16_t> (MP->pos << *MPos);
	RecMin rc = { 0 };
	int64_t X = (int64_t)MP->pos.x + DeltaX;
	int64_t Y = (int64_t)MP->pos.y + DeltaY;
	for (;((X > MPos->x) || (X < MPos->x) || (MP->pos.x == MPos->x )) && ((Y > MPos->y) || (Y < MPos->y) || (MP->pos.y == MPos->y));X += DeltaX, Y += DeltaY)
	{
		rc.x = X;
		rc.y = Y;
		for (int i = 0; i < 16; i++) {
			if (rc == SetP[i].pos) {
				return false;
			}
		}
	
	}
	X = (int64_t)MP->pos.x + DeltaX;
	Y = (int64_t)MP->pos.y + DeltaY;
	for (;((X > MPos->x) || (X < MPos->x) || (MP->pos.x == MPos->x)) && ((Y > MPos->y) || (Y < MPos->y) || (MP->pos.y == MPos->y));X += DeltaX, Y += DeltaY)
	{
		rc.x = X;
		rc.y = Y;
		for (int i = 0; i < 16; i++) {
			if (rc == SetP2[i].pos) {
				return false;
			}
		}
	}

	return true;
}
bool TheRulesOfMovement(
	Piece* CurrentPiece,
	RecMin* TheSelectedPosition
)
{
	int16_t DeltaX = static_cast<int16_t> (abs(CurrentPiece->pos.x - TheSelectedPosition->x));
	int16_t DeltaY = static_cast<int16_t> (abs(CurrentPiece->pos.y - TheSelectedPosition->y));
	int16_t PDelta = static_cast<int16_t>     (CurrentPiece->pos.y - TheSelectedPosition->y);

	//WPawn
	if (CurrentPiece->typ == Game::_Wp) {
		if (!(CurrentPiece->IsMoved)) {
			if (DeltaX == 0 &&
				((PDelta == Window::Pixal) ||(PDelta == 2 * Window::Pixal))) {
				return true;
			}
		}
		else {
			if (DeltaX == 0 && PDelta == Window::Pixal) {
				return true;
			}
		}
	}
	//BPawn
	else if (CurrentPiece->typ == Game::_Bp) {
		if (!(CurrentPiece->IsMoved)) {
			if (DeltaX == 0 &&
				((PDelta == -Window::Pixal) || (PDelta == -2 * Window::Pixal))) {
				return true;
			}
		}
		else {
			if (DeltaX == 0 && PDelta == -Window::Pixal) {
				return true;
			}
		}
	}
	//Bioshp
	else if (CurrentPiece->typ == Game::_Wb || CurrentPiece->typ == Game::_Bb) {
		if (DeltaX == DeltaY) {
			return true;
		}
	}
	//King
	else if (CurrentPiece->typ == Game::_Wk || CurrentPiece->typ == Game::_Bk) {
		if ((DeltaX == Window::Pixal || DeltaX == 0) && (DeltaY == Window::Pixal || DeltaY == 0)) {
			return true;
		}
	}
	//Queen
	else if (CurrentPiece->typ == Game::_Wq || CurrentPiece->typ == Game::_Bq) {
	    if (DeltaX == DeltaY) {
			return true;
		}
		else if ((DeltaX == 0 && DeltaY != 0) ||
			     (DeltaY == 0 && DeltaX != 0)) {
			return true;
		}
		
	}
	//King
	else if (CurrentPiece->typ == Game::_Wr || CurrentPiece->typ == Game::_Br) {
		if ((DeltaX == 0 && DeltaY != 0) ||
			(DeltaY == 0 && DeltaX != 0))
		{
			return true;
		}

	}
	//Knight
	else if (CurrentPiece->typ == Game::_Wn || CurrentPiece->typ == Game::_Bn) {
		if (DeltaX == 2 * Window::Pixal && DeltaY == 1 * Window::Pixal ||
		   (DeltaX == 1 * Window::Pixal && DeltaY == 2 * Window::Pixal)) {
			return true;
		}
	}

	return false;
}
bool TheMovementOfPieces(
	Piece Setof16Piece[],
	Board BoardofChess_8X8size,
	Piece Setof16Piece2[]
) {

	static Piece* CurrentPiece = nullptr; // Pointer_toPieceHadBeenSelected
	Piece* NewPiece = nullptr; //Pointer_toPieceSelectedNow 
	RecMin* TheSelectedPosition = nullptr;

	NewPiece = SelectPiece(Setof16Piece);

	if (NewPiece != nullptr) {
		if (CurrentPiece != nullptr) { CurrentPiece->state = Game::UnSelected; }
		CurrentPiece = NewPiece;
	}

	if (CurrentPiece != nullptr) {
		TheSelectedPosition = SelectPosition(BoardofChess_8X8size);
		if (TheSelectedPosition != nullptr && (CurrentPiece->pos != *TheSelectedPosition) &&
			TheRulesOfMovement(CurrentPiece, TheSelectedPosition)&&
		    IsTherePieceInPath(CurrentPiece, TheSelectedPosition, Setof16Piece2, Setof16Piece)) {
			CurrentPiece->IsMoved = Game::MOVED;
			CurrentPiece->pos = *TheSelectedPosition;
			CurrentPiece->state = Game::UnSelected;
			CurrentPiece = nullptr;
			return true;
		}
	}

	return false;
}
#endif