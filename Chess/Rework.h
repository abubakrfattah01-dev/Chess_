#include <raylib.h>
#include <stdint.h>
#include <cmath>

constexpr int Pixel = 90;
const char* Title = "Chess Game";
constexpr int SrcWidth = (Pixel * 8);
constexpr int SrcHeigth = (Pixel * 8);
const char* Icon_local = "picecs\\wn.png";
#define	IsMousePressed (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)|| IsMouseButtonPressed(MOUSE_BUTTON_LEFT))

// -- Setup all the pieces and the board

constexpr Color PieceSelectColor = { 74,84,45,255 };
constexpr Color RecLightColor = { 74, 112, 61,255 };
constexpr Color RecDarkColor = { 238, 232, 213 ,255 };

typedef enum PieceType
{
	_Wb,
	_Wn,
	_Wk,
	_Wq,
	_Wp,
	_Wr,
	_Bb,
	_Bn,
	_Bk,
	_Bq,
	_Bp,
	_Br

}__Type;

typedef enum PieceState {

	Selected,
	Active,
	UnActive

}__State;

typedef enum GameState {
	Player_One_Turn,
	Player_Two_Turn,
	player_One_In_check,
	player_Two_In_check,
	Player_One_Win,
	Player_Two_Win,
	Stalemate,
	Insufficient_Material,
	Threefold_Repetition,
	Fifty_Move_Rule

}__Game;

typedef enum KingState {
	NotInCheck,
	InCheck,
	InCheckMate
}__KingState;

typedef enum __Pawn {
	blackPawn,
	whitePawn
}__Pawnstate;

class __RecMin {
public:
	int16_t x = 0;
	int16_t y = 0;
	int16_t _2D = (int16_t)(Pixel);

	bool operator == (const __RecMin& rm2) const {

		return (x == rm2.x && y == rm2.y);
	};

	int operator >> (const __RecMin& rm2) const {

		if (rm2.x - x == 0) { return 0; }
		else if (rm2.x > x) { return Pixel; }
		else { return -Pixel; }
	};

	int operator << (const __RecMin& ry) const {

		if (ry.y - y == 0) { return 0; }
		else if (ry.y > y) { return Pixel; }
		else { return -Pixel; }
	};

	void DrawRecMin(Color C) const {
		DrawRectangle(x, y, _2D, _2D, C);
	}

	bool CheckRecMinVec(Vector2 v2) const {
		bool x_ = (x <= v2.x && (x + _2D) >= v2.x);
		bool y_ = (y <= v2.y && (y + _2D) >= v2.y);
		return (x_ && y_);
	}

};
//------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------
class Piece
{
public:
	__State state = Active;
	__RecMin pos = { 0 };
	virtual bool MoveFunction(__RecMin targetPos);
};

class King : public Piece
{
public:
	__KingState kingState = NotInCheck;
	bool IsMoved = false;
	bool MoveFunction( __RecMin targetPos ) override
	{
		uint16_t DeltaX = (abs(pos.x - targetPos.x));
		uint16_t DeltaY = (abs(pos.y - targetPos.y));

		return (DeltaX == Pixel || DeltaX == 0) && (DeltaY == Pixel || DeltaY == 0);
	};
};

class Rook : public Piece
{
public:
	bool IsMoved = false;
	bool MoveFunction(__RecMin targetPos) override {

		uint16_t DeltaX = (abs(pos.x - targetPos.x));
		uint16_t DeltaY = (abs(pos.y - targetPos.y));

		return (DeltaX == 0 && DeltaY != 0) || (DeltaY == 0 && DeltaX != 0);
	};
};

class Bishop : public Piece
{
public:
	bool MoveFunction(__RecMin targetPos) override {

		uint16_t DeltaX = (abs(pos.x - targetPos.x));
		uint16_t DeltaY = (abs(pos.y - targetPos.y));

		return (DeltaX == DeltaY);
	};
};

class Pawn : public Piece
{
	public:
	bool IsMoved = false;
	bool IsEnPassant = false;
	__Pawnstate state;

	bool MoveFunction(__RecMin targetPos) override {
		uint16_t DeltaX = (abs(pos.x - targetPos.x));
		int16_t PDeltaY = (pos.y - targetPos.y);
		if (state == whitePawn) {
			if (!(IsMoved)) {
				if (DeltaX == 0 && ((PDeltaY == Pixel) || (PDeltaY == 2 * Pixel))) {
					return true;
				}
				else if (DeltaX == 0 && PDeltaY == Pixel) {
					return true;
				}
			}
			else {

				if (DeltaX == 0 && PDeltaY == Pixel) {
					return true;
				}
			}
		}
		else if (state == blackPawn) {
			if (!(IsMoved)) {
				if (DeltaX == 0 &&
					((PDeltaY == -(Pixel)) || (PDeltaY == (-2) * Pixel))) {
					return true;
				}

			}
			else {
				if (DeltaX == 0 && PDeltaY == -(Pixel)) {
					return true;
				}
			}
		}
	};
};

class Queen : public Piece
{
	bool MoveFunction(__RecMin targetPos) override {
		uint16_t DeltaX = (abs(pos.x - targetPos.x));
		uint16_t DeltaY = (abs(pos.y - targetPos.y));	

		return (DeltaX == DeltaY) || (DeltaX == 0 && DeltaY != 0) || (DeltaY == 0 && DeltaX != 0);
	}
};

class Knight : public Piece
{
public:
	bool MoveFunction(__RecMin targetPos) override {
		uint16_t DeltaX = (abs(pos.x - targetPos.x));
		uint16_t DeltaY = (abs(pos.y - targetPos.y));
		return (DeltaX == 2 * Pixel && DeltaY == Pixel) || (DeltaX == Pixel && DeltaY == 2 * Pixel);
	}
};

class AsetofPieces {
	Pawn Pawns[8];
	King king;
	Queen queen;
	Knight knights[2];
	Bishop bishops[2];
	Rook rooks[2];
};