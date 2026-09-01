#include <raylib.h>
#include <stdint.h>
#include <string>
#define	IsMousePressed (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)|| IsMouseButtonPressed(MOUSE_BUTTON_LEFT))

constexpr int Pixel = 90;

namespace Window {
	const char* Icon_local = "picecs\\wn.png";
	constexpr int SrcWidth = (Pixel * 8);
	constexpr int SrcHeigth = (Pixel * 8);
	const char* Title = "Chess Game";
}

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
	Player_One_Win,
	Player_Two_Win,
	Stalemate,
	Insufficient_Material,
	Threefold_Repetition,
	Fifty_Move_Rule

}__Game;


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

class Board {
public:
	__RecMin _Board[8][8];
	Board() {
		for (int row = 0; row < 8; row++) {
			for (int col = 0; col < 8; col++) {
				_Board[row][col].x = (int16_t)Pixel * (7 - col);
				_Board[row][col].y = (int16_t)Pixel * (7 - row);
				_Board[row][col]._2D = (int16_t)Pixel;
			}
		}
	}
	void DrawBoard()const {
		for (int row = 0; row < 8; row++) {
			for (int col = 0; col < 8; col++) {
				if ((row + col) % 2 == 0) {
					_Board[row][col].DrawRecMin(RecLightColor);
				}
				else {
					_Board[row][col].DrawRecMin(RecDarkColor);
				}
			}
		}
	}
};

class Piece {
public:
	__Type typ = _Bb;
	__State state = Active;
	__RecMin pos = { 0 };
	bool IsMoved = 0;
	Texture tex = { 0 };
	~Piece() {
		UnloadTexture(tex);
	}
	void __Set(__RecMin rec, __Type ty) {

		this->pos = rec;
		this->typ = ty;

		std::string path = "C:\\Users\\Good\\Downloads\\picecs\\";
		switch (typ) {


		case _Wp:  path += "wp.png";  break;
		case _Wn:  path += "wn.png";  break;
		case _Wb:  path += "wb.png";  break;
		case _Wk:  path += "wk.png";  break;
		case _Wq:  path += "wq.png";  break;
		case _Wr:  path += "wr.png";  break;
		case _Bp:  path += "bp.png";  break;
		case _Bn:  path += "bn.png";  break;
		case _Bb:  path += "bb.png";  break;
		case _Br:  path += "br.png";  break;
		case _Bk:  path += "bk.png";  break;
		case _Bq:  path += "bq.png";  break;


		}

		Image TEX = LoadImage(path.c_str());
			// Add this default ctor to explicitly initialize members
		ImageResize(&TEX, Pixel, Pixel);
		this->tex = LoadTextureFromImage(TEX);
		UnloadImage(TEX);};	
};

#pragma region Piecese

void Init_16Piece(
	Board* Board,
	Piece Setof16Piece[],
	bool TypeofSet
) {
	Setof16Piece[0].__Set((TypeofSet) ? Board->_Board[1][0] : Board->_Board[6][0], (TypeofSet) ? _Wp : _Bp);
	Setof16Piece[1].__Set((TypeofSet) ? Board->_Board[1][1] : Board->_Board[6][1], (TypeofSet) ? _Wp : _Bp);
	Setof16Piece[2].__Set((TypeofSet) ? Board->_Board[1][2] : Board->_Board[6][2], (TypeofSet) ? _Wp : _Bp);
	Setof16Piece[3].__Set((TypeofSet) ? Board->_Board[1][3] : Board->_Board[6][3], (TypeofSet) ? _Wp : _Bp);
	Setof16Piece[4].__Set((TypeofSet) ? Board->_Board[1][4] : Board->_Board[6][4], (TypeofSet) ? _Wp : _Bp);
	Setof16Piece[5].__Set((TypeofSet) ? Board->_Board[1][5] : Board->_Board[6][5], (TypeofSet) ? _Wp : _Bp);
	Setof16Piece[6].__Set((TypeofSet) ? Board->_Board[1][6] : Board->_Board[6][6], (TypeofSet) ? _Wp : _Bp);
	Setof16Piece[7].__Set((TypeofSet) ? Board->_Board[1][7] : Board->_Board[6][7], (TypeofSet) ? _Wp : _Bp);
	Setof16Piece[8].__Set((TypeofSet) ? Board->_Board[0][0] : Board->_Board[7][0], (TypeofSet) ? _Wr : _Br);
	Setof16Piece[9].__Set((TypeofSet) ? Board->_Board[0][1] : Board->_Board[7][1], (TypeofSet) ? _Wn : _Bn);
	Setof16Piece[10].__Set((TypeofSet) ? Board->_Board[0][2] : Board->_Board[7][2], (TypeofSet) ? _Wb : _Bb);
	Setof16Piece[11].__Set((TypeofSet) ? Board->_Board[0][3] : Board->_Board[7][3], (TypeofSet) ? _Wk : _Bk);
	Setof16Piece[12].__Set((TypeofSet) ? Board->_Board[0][4] : Board->_Board[7][4], (TypeofSet) ? _Wq : _Bq);
	Setof16Piece[13].__Set((TypeofSet) ? Board->_Board[0][5] : Board->_Board[7][5], (TypeofSet) ? _Wb : _Bb);
	Setof16Piece[14].__Set((TypeofSet) ? Board->_Board[0][6] : Board->_Board[7][6], (TypeofSet) ? _Wn : _Bn);
	Setof16Piece[15].__Set((TypeofSet) ? Board->_Board[0][7] : Board->_Board[7][7], (TypeofSet) ? _Wr : _Br);
}
void DrawAllPieces(
	Piece Wpieces[],
	Piece Bpieces[]
) {
	for (int i = 0; i < 16;i++) {
		if (Wpieces[i].state == Selected) {
			DrawTexture(Wpieces[i].tex, Wpieces[i].pos.x, Wpieces[i].pos.y, PieceSelectColor);
		}
		else if (Wpieces[i].state == Active) {
			DrawTexture(Wpieces[i].tex, Wpieces[i].pos.x, Wpieces[i].pos.y, WHITE);
		}
	}
	for (int i = 0; i < 16;i++) {
		if (Bpieces[i].state == Selected) {
			DrawTexture(Bpieces[i].tex, Bpieces[i].pos.x, Bpieces[i].pos.y, PieceSelectColor);
		}
		else if (Bpieces[i].state == Active) {
			DrawTexture(Bpieces[i].tex, Bpieces[i].pos.x, Bpieces[i].pos.y, WHITE);
		}
	}
}
#pragma endregion

#pragma region Selectors

Piece* SelectPiece(
	Piece SetofPieces_16_Piece[]
) {
	for (int indexofPiece = 0; indexofPiece < 16; indexofPiece++) {
		if (IsMousePressed && SetofPieces_16_Piece[indexofPiece].pos.CheckRecMinVec(GetMousePosition())) {
			SetofPieces_16_Piece[indexofPiece].state = Selected;
			return &SetofPieces_16_Piece[indexofPiece];
		}
	}
	return nullptr;
};
__RecMin* SelectPosition(
	Board BoardofChess_8X8size
) {
	for (int indexofrow = 0; indexofrow < 8; indexofrow++) {
		for (int indexofcol = 0; indexofcol < 8; indexofcol++) {
			if (IsMousePressed && BoardofChess_8X8size._Board[indexofrow][indexofcol].CheckRecMinVec(GetMousePosition())) {
				return &BoardofChess_8X8size._Board[indexofrow][indexofcol];
			}
		}
	}
	return nullptr;
};

#pragma endregion
#pragma region movement 
void TheRulesofcapture(
	Piece* CurrentPiece,
	Piece SetofPiece[]
) {
	for (int i = 0; i < 16;i++) {
		if (SetofPiece[i].pos == CurrentPiece->pos) {
			SetofPiece[i].state = UnActive;
			return;
		}
	}
	return;
}
bool IsTherePieceInPath(
	Piece* MP,
	__RecMin* MPos,
	Piece SetP[],
	Piece SetP2[]
) {
	if (MP->typ == _Wn) {
		return true;
	}
	int16_t DeltaX = static_cast<int16_t> (MP->pos >> *MPos);
	int16_t DeltaY = static_cast<int16_t> (MP->pos << *MPos);
	__RecMin rc = { 0 };
	int16_t X = MP->pos.x + DeltaX;
	int16_t Y = MP->pos.y + DeltaY;
	for (;((X > MPos->x) || (X < MPos->x) || (MP->pos.x == MPos->x))
		&& ((Y > MPos->y) || (Y < MPos->y) || (MP->pos.y == MPos->y));X += DeltaX, Y += DeltaY)
	{
		rc.x = X;
		rc.y = Y;
		for (int i = 0; i < 16; i++) {
			if (rc == SetP[i].pos && SetP[i].state != UnActive) {
				return false;
			}
		}

	}
	X = MP->pos.x + DeltaX;
	Y = MP->pos.y + DeltaY;
	for (;((X > MPos->x) || (X < MPos->x) || (MP->pos.x == MPos->x))
		&& ((Y > MPos->y) || (Y < MPos->y) || (MP->pos.y == MPos->y));X += DeltaX, Y += DeltaY)
	{
		rc.x = X;
		rc.y = Y;
		for (int i = 0; i < 16; i++) {
			if (rc == SetP2[i].pos && SetP2[i].state != UnActive) {
				return false;
			}
		}
	}

	return true;
}
bool TheRulesOfMovement(
	Piece* CurrentPiece,
	__RecMin* TheSelectedPosition
)
{
	int16_t DeltaX = static_cast<int16_t> (abs(CurrentPiece->pos.x - TheSelectedPosition->x));
	int16_t DeltaY = static_cast<int16_t> (abs(CurrentPiece->pos.y - TheSelectedPosition->y));
	int16_t PDelta = static_cast<int16_t>     (CurrentPiece->pos.y - TheSelectedPosition->y);
	switch (CurrentPiece->typ) {

	case _Wp:
		if (!(CurrentPiece->IsMoved)) {
			if (DeltaX == 0 &&
				((PDelta == Pixel) || (PDelta == 2 * Pixel))) {
				return true;
			}
		}
		else {
			if (DeltaX == 0 && PDelta == Pixel) {
				return true;
			}
		}
		break;
		//BPawn
	case _Bp:
		if (!(CurrentPiece->IsMoved)) {
			if (DeltaX == 0 &&
				((PDelta == -(Pixel)) || (PDelta == (-2) * Pixel))) {
				return true;
			}
		}
		else {
			if (DeltaX == 0 && PDelta == -(Pixel)) {
				return true;
			}
		}
		break;
		//Bioshp
	case _Wb:
	case _Bb:
		if (DeltaX == DeltaY)
		{
			return true;
		}
		break;
		//King
	case _Wk:
	case _Bk:
		if ((DeltaX == Pixel || DeltaX == 0) &&
			(DeltaY == Pixel || DeltaY == 0))
		{
			return true;
		}
		break;
		//Queen
	case _Wq:
	case _Bq:
		if (DeltaX == DeltaY) {
			return true;
		}
		else if ((DeltaX == 0 && DeltaY != 0) ||
			(DeltaY == 0 && DeltaX != 0))
		{
			return true;
		}

		break;
		//King
	case _Wr:
	case _Br:
		if ((DeltaX == 0 && DeltaY != 0) ||
			(DeltaY == 0 && DeltaX != 0))
		{
			return true;
		}

		break;
		//Knight
	case _Wn:
	case _Bn:
		if (DeltaX == 2 * Pixel && DeltaY == Pixel ||
			(DeltaX == Pixel && DeltaY == 2 *Pixel))
		{
			return true;
		}
		break;
	}

	return false;
}
bool TheMovementOfPieces(
	Piece Setof16Piece[],
	Board * BoardofChess_8X8size,
	Piece Setof16Piece2[]
) {

	static Piece* CurrentPiece = nullptr; // Pointer_toPieceHadBeenSelected
	Piece* NewPiece = nullptr; //Pointer_toPieceSelectedNow 
	__RecMin* TheSelectedPosition = nullptr;

	NewPiece = SelectPiece(Setof16Piece);

	if (NewPiece != nullptr) {
		if (CurrentPiece != nullptr) { CurrentPiece->state = Active; }
		CurrentPiece = NewPiece;
	}

	if (CurrentPiece != nullptr) {
		TheSelectedPosition = SelectPosition(*BoardofChess_8X8size);
		if (TheSelectedPosition != nullptr && (CurrentPiece->pos != *TheSelectedPosition) &&
			TheRulesOfMovement(CurrentPiece, TheSelectedPosition) &&
			IsTherePieceInPath(CurrentPiece, TheSelectedPosition, Setof16Piece2, Setof16Piece)) {
			
			CurrentPiece->IsMoved = true;
			CurrentPiece->pos = *TheSelectedPosition;
			CurrentPiece->state = Active;
			TheRulesofcapture(CurrentPiece, Setof16Piece2);
			CurrentPiece = nullptr;
			return true;
		}
	}

	return false;
}

#pragma endregion 


