#include <raylib.h>
#include <stdint.h>
#include <string>
#include <math.h>
#include <algorithm>
#include <iostream>
//square

#define WorkMode 0
#define Left 
#define Right 
#define long_C
#define short_C

namespace Window {
	const char* Icon_local = "picecs\\wn.png";
	constexpr int Pixal = 90;
	constexpr int SrcWidth = (Pixal * 8);
	constexpr int SrcHeigth = (Pixal * 8);
	const char* Title = "Chess Game";
}

constexpr float Pixel = 90;

constexpr Color PieceSelectColor = { 74,84,45,255 };

constexpr Color RecLightColor = { 74, 112, 61,255 };

constexpr Color RecDarkColor = { 238, 232, 213 ,255 };

#define	IsMousePressed (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)|| IsMouseButtonPressed(MOUSE_BUTTON_LEFT))

typedef Vector2 V2;
typedef Texture Tex;

V2 VecSum(V2 v1, V2 v2) {
	return{ v1.x + v2.x, v1.y + v2.y };
}

class Ray2 {
public:

	float Distance = 0;
	V2  bPos = { 0 };
	V2  direction = { 0 };

	Ray2(V2 Bpos, V2 Epos)
	{
		// center the bPos
		bPos = { Bpos.x + Pixel / 2 ,Bpos.y + Pixel / 2 };
		// Get The directionVector
		float dy = Epos.y - Bpos.y;
		float dx = Epos.x - Bpos.x;
		direction.x = Pixel * (dx / std::max(abs(dx), 1.0f));
		direction.y = Pixel * (dy / std::max(abs(dy), 1.0f));
		// Calc The Distance In Pixel Uint
		Distance = sqrtf((dx * dx) + (dy * dy));
		Distance = !(dy && dx) ? Distance /= Pixel : Distance /= sqrtf(2) * Pixel;
		Distance = std::round(Distance);
	};
};

class sqar_t {
public:

	V2 Pos;
	uint16_t d2;

	bool operator == (const sqar_t& other) const {
		return (this->Pos.x == other.Pos.x && this->Pos.y == other.Pos.y);
	};
	bool IsClicked() const {
		Vector2 v2 = GetMousePosition();
		bool x_ = (this->Pos.x <= v2.x && (this->Pos.x + d2) >= v2.x);
		bool y_ = (this->Pos.y <= v2.y && (this->Pos.y + d2) >= v2.y);
		return (x_ && y_) && IsMousePressed;
	}
	bool CheckV2(V2 v2)const {
		bool x_ = (this->Pos.x <= v2.x && (this->Pos.x + d2) >= v2.x);
		bool y_ = (this->Pos.y <= v2.y && (this->Pos.y + d2) >= v2.y);
		return (x_ && y_);
	};
	void Draw(Color color)const {
		DrawRectangle((int)Pos.x, (int)Pos.y, (int)d2, (int)d2, color);
	}

};

class Board {
public:
	sqar_t _Board[8][8];
	Board() {
		for (int row = 0; row < 8; row++) {
			for (int col = 0; col < 8; col++) {
				_Board[row][col].Pos.x = (float)Pixel * (7 - col);
				_Board[row][col].Pos.y = (float)Pixel * (7 - row);
				_Board[row][col].d2 = (int16_t)Pixel;
			}
		}
	}
	void DrawBoard()const {
		for (int row = 0; row < 8; row++) {
			for (int col = 0; col < 8; col++) {
				if ((row + col) % 2 == 0) {
					_Board[row][col].Draw(RecLightColor);
				}
				else {
					_Board[row][col].Draw(RecDarkColor);
				}
			}
		}
	}
};

typedef enum PieceType
{
	_Wp,
	_Wn,
	_Wb,
	_Wk,
	_Wq,
	_Wr,

	_Bp,
	_Bn,
	_Bb,
	_Bk,
	_Bq,
	_Br

}_Ty;

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

typedef enum PieceState {

	Selected,
	Active,
	UnActive

}_Sta;

Texture tex[12];

std::string Path[12] = {
	"C:\\Users\\Good\\Downloads\\picecs\\wp.png",
	"C:\\Users\\Good\\Downloads\\picecs\\wn.png",
	"C:\\Users\\Good\\Downloads\\picecs\\wb.png",
	"C:\\Users\\Good\\Downloads\\picecs\\wk.png",
	"C:\\Users\\Good\\Downloads\\picecs\\wq.png",
	"C:\\Users\\Good\\Downloads\\picecs\\wr.png",
	"C:\\Users\\Good\\Downloads\\picecs\\bp.png",
	"C:\\Users\\Good\\Downloads\\picecs\\bn.png",
	"C:\\Users\\Good\\Downloads\\picecs\\bb.png",
	"C:\\Users\\Good\\Downloads\\picecs\\bk.png",
	"C:\\Users\\Good\\Downloads\\picecs\\bq.png",
	"C:\\Users\\Good\\Downloads\\picecs\\br.png"
};

void InitTexture() {
	Image IMG = { 0 };
	for (int i = 0; i < 12; i++) {
		IMG = LoadImage(Path[i].c_str());
		ImageResize(&IMG, (int)Pixel, (int)Pixel);
		tex[i] = LoadTextureFromImage(IMG);
	}
	UnloadImage(IMG);
}

void deInitTexture() {
	for (int i = 0; i < 12; i++) {
		UnloadTexture(tex[i]);
	}
}

class Piece {
public:
	sqar_t pos = { 0 };
	_Ty Ty;
	_Sta state = Active;
	bool IsMoved = 0;


	bool RayCasting(sqar_t E, Piece arr[16], Piece arr2[16]) {

		if (this->Ty == _Wn || this->Ty == _Bn) {
			return true;
		}

		Ray2 Path(this->pos.Pos, E.Pos);

		V2 Thechecker = Path.bPos;

		for (int Move = 1; Move < Path.Distance; Move++)
		{
			Thechecker = VecSum(Thechecker, Path.direction);

			for (int PieceIndex = 0; PieceIndex < 16; PieceIndex++) {

				if (((arr[PieceIndex].pos.CheckV2(Thechecker) && arr[PieceIndex].state != UnActive))) {
					return false;
				}
				if ((arr2[PieceIndex].pos.CheckV2(Thechecker) && arr2[PieceIndex].state != UnActive)) {
					return false;
				}
			}
		}

		return true;

	};

	void Draw(Color color) const {
		DrawTexture(tex[((int)(Ty))], (int)(pos.Pos.x), (int)(pos.Pos.y), color);
	}

	void set(sqar_t rec, _Ty ty) {
		this->pos = rec;
		this->Ty = ty;
	};
};

void InitSetPiece(Board* Board, Piece Setof16Piece[], bool TypeofSet) {

	Setof16Piece[0].set((TypeofSet) ? Board->_Board[1][0] : Board->_Board[6][0], (TypeofSet) ? _Wp : _Bp);
	Setof16Piece[1].set((TypeofSet) ? Board->_Board[1][1] : Board->_Board[6][1], (TypeofSet) ? _Wp : _Bp);
	Setof16Piece[2].set((TypeofSet) ? Board->_Board[1][2] : Board->_Board[6][2], (TypeofSet) ? _Wp : _Bp);
	Setof16Piece[3].set((TypeofSet) ? Board->_Board[1][3] : Board->_Board[6][3], (TypeofSet) ? _Wp : _Bp);
	Setof16Piece[4].set((TypeofSet) ? Board->_Board[1][4] : Board->_Board[6][4], (TypeofSet) ? _Wp : _Bp);
	Setof16Piece[5].set((TypeofSet) ? Board->_Board[1][5] : Board->_Board[6][5], (TypeofSet) ? _Wp : _Bp);
	Setof16Piece[6].set((TypeofSet) ? Board->_Board[1][6] : Board->_Board[6][6], (TypeofSet) ? _Wp : _Bp);
	Setof16Piece[7].set((TypeofSet) ? Board->_Board[1][7] : Board->_Board[6][7], (TypeofSet) ? _Wp : _Bp);

	Setof16Piece[8].set((TypeofSet) ? Board->_Board[0][0] : Board->_Board[7][0], (TypeofSet) ? _Wr : _Br);
	Setof16Piece[9].set((TypeofSet) ? Board->_Board[0][1] : Board->_Board[7][1], (TypeofSet) ? _Wn : _Bn);
	Setof16Piece[10].set((TypeofSet) ? Board->_Board[0][2] : Board->_Board[7][2], (TypeofSet) ? _Wb : _Bb);
	Setof16Piece[11].set((TypeofSet) ? Board->_Board[0][3] : Board->_Board[7][3], (TypeofSet) ? _Wk : _Bk);
	Setof16Piece[12].set((TypeofSet) ? Board->_Board[0][4] : Board->_Board[7][4], (TypeofSet) ? _Wq : _Bq);
	Setof16Piece[13].set((TypeofSet) ? Board->_Board[0][5] : Board->_Board[7][5], (TypeofSet) ? _Wb : _Bb);
	Setof16Piece[14].set((TypeofSet) ? Board->_Board[0][6] : Board->_Board[7][6], (TypeofSet) ? _Wn : _Bn);
	Setof16Piece[15].set((TypeofSet) ? Board->_Board[0][7] : Board->_Board[7][7], (TypeofSet) ? _Wr : _Br);

}

void DrawAllPieces(Piece Wpieces[], Piece Bpieces[]) {
	for (int i = 0; i < 16;i++) {
		if (Wpieces[i].state == Selected) {
			Wpieces[i].Draw(PieceSelectColor);
		}
		else if (Wpieces[i].state == Active) {
			Wpieces[i].Draw(WHITE);
		}
	}
	for (int i = 0; i < 16;i++) {
		if (Bpieces[i].state == Selected) {
			Bpieces[i].Draw(PieceSelectColor);
		}
		else if (Bpieces[i].state == Active) {
			Bpieces[i].Draw(WHITE);
		}
	}
}

Piece* SelectPiece(Piece SetofPieces_16_Piece[]) {
	for (int indexofPiece = 0; indexofPiece < 16; indexofPiece++) {
		if (IsMousePressed && SetofPieces_16_Piece[indexofPiece].pos.IsClicked() && SetofPieces_16_Piece[indexofPiece].state != UnActive) {
			SetofPieces_16_Piece[indexofPiece].state = Selected;
			return &SetofPieces_16_Piece[indexofPiece];
		}
	}
	return nullptr;
};

sqar_t* SelectPosition(Board BoardofChess_8X8size) {
	for (int indexofrow = 0; indexofrow < 8; indexofrow++) {
		for (int indexofcol = 0; indexofcol < 8; indexofcol++) {
			if (IsMousePressed && BoardofChess_8X8size._Board[indexofrow][indexofcol].IsClicked()) {
				return &BoardofChess_8X8size._Board[indexofrow][indexofcol];
			}
		}
	}
	return nullptr;
};

bool IstherePieceHere(sqar_t* pos, Piece SetofPiece2[]) {
	for (int i = 0; i < 16;i++) {
		if (SetofPiece2[i].pos == *pos && SetofPiece2[i].state != UnActive) {
			return true;
		}
	}
	return false;
}

Piece* GetPiece(sqar_t* pos, Piece Set[]) {
	for (int indexofPiece = 0; indexofPiece < 16; indexofPiece++) {
		if (Set[indexofPiece].pos == *pos && Set[indexofPiece].state != UnActive) {
			return &Set[indexofPiece];
		}
	}
	return nullptr;
};

bool KingRookMove(bool type, Piece* king, Piece Set[]) {
	sqar_t Pos = { 0 };
	if (king->Ty == _Bk) {

		if (type) {
			Pos.Pos.x = 0;
			Pos.Pos.y = Window::SrcHeigth;
			Piece* p = GetPiece(&Pos, Set);
			if (p != nullptr) {
				return (!king->IsMoved) && (!p->IsMoved);
			}
		}
		else {
			Pos.Pos.x = Window::SrcWidth;
			Pos.Pos.y = Window::SrcHeigth;
			Piece* p = GetPiece(&Pos, Set);
			if (p != nullptr) {
				return (!king->IsMoved) && (!p->IsMoved);
			}
		}
	}
	else
	{
		if (type == false) {
			Pos.Pos.x = 0;
			Pos.Pos.y = 0;
			Piece* p = GetPiece(&Pos, Set);
			if (p != nullptr) {
				return (!king->IsMoved) && (!p->IsMoved);
			}
		}
		else {
			Pos.Pos.x = Window::SrcWidth;
			Pos.Pos.y = 0;
			Piece* p = GetPiece(&Pos, Set);
			if (p != nullptr) {
				return (!king->IsMoved) && (!p->IsMoved);
			}
		}
	}
	return false;
}

void CapturePiece(sqar_t* pos, Piece SetofPiece[]) {
	for (int i = 0; i < 16;i++) {
		if (SetofPiece[i].pos == *pos) {
			SetofPiece[i].state = UnActive;
			return;
		}
	}
	return;
}

bool IsTheMoveValid(Piece* CurrentPiece, sqar_t* TheSelectedPosition, Piece TheAotherSet[], Piece CurrentPieceSet[])
{
	int16_t DeltaX = (int16_t)(abs(CurrentPiece->pos.Pos.x - TheSelectedPosition->Pos.x));
	int16_t DeltaY = (int16_t)(abs(CurrentPiece->pos.Pos.y - TheSelectedPosition->Pos.y));
	int16_t PDeltaY = (int16_t)(CurrentPiece->pos.Pos.y - TheSelectedPosition->Pos.y);
	int16_t PDeltaX = (int16_t)(CurrentPiece->pos.Pos.x - TheSelectedPosition->Pos.x);

	switch (CurrentPiece->Ty) {
#pragma region WP
	case _Wp:
		if (!(CurrentPiece->IsMoved)) {
			if ((DeltaX == 0 && ((PDeltaY == Pixel) || (PDeltaY == 2 * Pixel))) &&
				!(IstherePieceHere(TheSelectedPosition, TheAotherSet) || IstherePieceHere(TheSelectedPosition, CurrentPieceSet))) {
				return true;
			}
			else if ((DeltaX == Pixel && PDeltaY == Pixel) && IstherePieceHere(TheSelectedPosition, TheAotherSet)) {
				return true;
			}
		}
		else {
			if ((DeltaX == 0 && PDeltaY == Pixel) &&
				!(IstherePieceHere(TheSelectedPosition, TheAotherSet) || IstherePieceHere(TheSelectedPosition, CurrentPieceSet))) {
				return true;
			}
			else if ((DeltaX == Pixel && PDeltaY == Pixel) && IstherePieceHere(TheSelectedPosition, TheAotherSet)) {
				return true;
			}
		}
		break;
#pragma endregion
#pragma region BP
	case _Bp:
		if (!(CurrentPiece->IsMoved)) {
			if ((DeltaX == 0 && ((PDeltaY == -(Pixel)) || (PDeltaY == (-2) * Pixel))) &&
				!(IstherePieceHere(TheSelectedPosition, TheAotherSet) || IstherePieceHere(TheSelectedPosition, CurrentPieceSet))) {
				return true;
			}
			else if ((DeltaX == (Pixel) && PDeltaY == (-Pixel)) && IstherePieceHere(TheSelectedPosition, TheAotherSet)) {
				return true;
			}
		}
		else {
			if ((DeltaX == 0 && PDeltaY == -(Pixel))
				&& !(IstherePieceHere(TheSelectedPosition, TheAotherSet) || IstherePieceHere(TheSelectedPosition, CurrentPieceSet)))
			{
				return true;
			}
			else if ((DeltaX == (Pixel) && PDeltaY == (-Pixel)) && IstherePieceHere(TheSelectedPosition, TheAotherSet)) {
				return true;
			}
		}
		break;
#pragma endregion 

	case _Wk:case _Bk:

		if ((DeltaX == Pixel || DeltaX == 0) && (DeltaY == Pixel || DeltaY == 0)) {
			return true;
		}

		else if (DeltaY == 0 && PDeltaX == -2 * Pixel) short_C{
			/*
			* x = max , y = max
			*/
			sqar_t P2 {};
		P2.Pos.x = Window::SrcWidth;
		P2.Pos.y = Window::SrcHeigth;
		P2.d2 = Pixel;
		Piece* p = GetPiece(&P2, CurrentPieceSet);
		  if ((p != nullptr) && (p->Ty == _Wr || p->Ty == _Br)) {
			return (!p->IsMoved || !CurrentPiece->IsMoved);
		  }
			return false;
		}

		else if (DeltaY == 0 && PDeltaX == 2 * Pixel)long_C
		{
			/*
			* x = max , y = 0
			*/
			return true;
		}

		break;
#pragma region Queen
	case _Wq:case _Bq:
		if (DeltaX == DeltaY)
		{
			return true;
		}
		else if ((DeltaX == 0 && DeltaY != 0) ||
			(DeltaY == 0 && DeltaX != 0))
		{
			return true;
		}
		break;
#pragma endregion
#pragma region Rook
	case _Wr:case _Br:
		if ((DeltaX == 0 && DeltaY != 0) ||
			(DeltaY == 0 && DeltaX != 0))
		{
			return true;
		}
		break;
#pragma endregion
#pragma region Knight
	case _Wn:case _Bn:
		if ((DeltaX == 2 * Pixel && DeltaY == Pixel) ||
			(DeltaX == Pixel && DeltaY == 2 * Pixel))
		{
			return true;
		}
		break;
#pragma endregion
#pragma region Bioshp
	case _Wb:case _Bb:
		if (DeltaX == DeltaY)
		{
			return true;
		}
		break;
	}
#pragma endregion
	return false;
}

bool TheMovement(Piece Setof16Piece[], Board* BoardofChess_8X8size, Piece Setof16Piece2[], __Game p) {

	static Piece* CurrentPiece = nullptr; // Pointer_toPieceHadBeenSelected
	Piece* NewPiece = nullptr; //Pointer_toPieceSelectedNow 
	sqar_t* TheSelectedPosition = nullptr;

	NewPiece = SelectPiece(Setof16Piece);
	if (NewPiece != nullptr) {

		if (CurrentPiece != nullptr) {
			CurrentPiece->state = Active;
		}

		CurrentPiece = NewPiece;
		CurrentPiece->state = Selected;
	}

	if (CurrentPiece != nullptr) {
		TheSelectedPosition = SelectPosition(*BoardofChess_8X8size);
		if (TheSelectedPosition != nullptr && (CurrentPiece->pos != *TheSelectedPosition) &&
			IsTheMoveValid(CurrentPiece, TheSelectedPosition, Setof16Piece2, Setof16Piece) &&
			CurrentPiece->RayCasting(*TheSelectedPosition, Setof16Piece2, Setof16Piece)) {

			CapturePiece(TheSelectedPosition, Setof16Piece2);
			CurrentPiece->IsMoved = true;
			CurrentPiece->pos = *TheSelectedPosition;
			CurrentPiece->state = Active;

			CurrentPiece = nullptr;
			return true;
		}
	}

	return false;
}
