
#define WorkMode 1
#define Out0 return 0;
#define Out1 return 1;

#include <raylib.h>
#include <stdint.h>
#include <string>
#include <math.h>
#include <algorithm>
#include <iostream>

#pragma region const
float Pixel = 90;
Color SelectColor = { 74, 84, 45, 255 };
Color LightColor = { 74, 112, 61, 255 };
Color DarkColor = { 238, 232, 213 ,255 };
namespace Window {
	const char* Icon_local = "picecs\\wn.png";
	constexpr int Pixal = 90;
	constexpr int SrcWidth = (Pixal * 8);
	constexpr int SrcHeigth = (Pixal * 8);
	const char* Title = "Chess Game";
}
#define	IsMousePressed \
(IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)||\
 IsMouseButtonPressed(MOUSE_BUTTON_LEFT)||\
IsMouseButtonPressed(MOUSE_BUTTON_MIDDLE))
#pragma endregion

#pragma region  MainStructions
typedef Vector2 V2;
typedef Texture _Tx;
typedef enum Piece_Type
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
typedef enum Piece_State {

	Selected,
	Active,
	UnActive

}_Sta;
typedef enum Move_Type
{
	// Odd Moves
	ShortCasling = 0b0,
	LongCasling = 0b1,
	Promote = 0b10000,

	// Normal Moves
	PieceMovements = 8, // No capture
	Capture = 2,
	Check = 4,
	CheckMate = 32,

	//  a Sum of Moves
	CaptureAndCheck = 2 | 4,
	CaptureAndCheckMate = 2 | 32,
	CaptureAndPromote = 2 | 16


}_Mty;
typedef enum GameState {

	Player_One_Turn,
	Player_Two_Turn,

	Player_One_Win,
	Player_Two_Win,

	Stalemate,
	Insufficient_Material,
	Threefold_Repetition,
	Fifty_Move_Rule

}Game;
class RAY {
public:

	float Distance = 0;
	V2  bPos = { 0 };
	V2  direction = { 0 };

	RAY(V2 Bpos, V2 Epos)
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
#pragma endregion

#pragma region Texture 
_Tx tex[12];
std::string Path[12] = {

	"picecs\\wp.png",
	"picecs\\wn.png",
	"picecs\\wb.png",
	"picecs\\wk.png",
	"picecs\\wq.png",
	"picecs\\wr.png",

	"picecs\\bp.png",
	"picecs\\bn.png",
	"picecs\\bb.png",
	"picecs\\bk.png",
	"picecs\\bq.png",
	"picecs\\br.png"

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
#pragma endregion

#pragma region Board_PPos
class sqar_t {
public:

	float x = 0.0f;
	float y = 0.0f;
	uint16_t d2 = (uint16_t)Pixel;

	bool operator == (const sqar_t& other) const {
		return (this->x == other.x && this->y == other.y);
	};
	bool IsClicked() const {
		Vector2 v2 = GetMousePosition();
		bool x_ = (this->x <= v2.x && (this->x + d2) >= v2.x);
		bool y_ = (this->y <= v2.y && (this->y + d2) >= v2.y);
		return (x_ && y_) && IsMousePressed;
	}
	bool CheckV2(V2 v2)const {
		bool x_ = (this->x <= v2.x && (this->x + d2) >= v2.x);
		bool y_ = (this->y <= v2.y && (this->y + d2) >= v2.y);
		return (x_ && y_);
	};
	void Draw(Color color)const {
		DrawRectangle((int)x, (int)y, (int)d2, (int)d2, color);
	}

};
typedef sqar_t* PPos, coord;
class Board {
public:
	sqar_t _Board[8][8];
	Board() {
		for (int row = 0; row < 8; row++) {
			for (int col = 0; col < 8; col++) {
				_Board[row][col].x = (float)Pixel * (7 - col);
				_Board[row][col].y = (float)Pixel * (7 - row);
				_Board[row][col].d2 = (int16_t)Pixel;
			}
		}
	}
	void DrawBoard()const {
		for (int row = 0; row < 8; row++) {
			for (int col = 0; col < 8; col++) {
				if ((row + col) % 2 == 0) {
					_Board[row][col].Draw(LightColor);
				}
				else {
					_Board[row][col].Draw(DarkColor);
				}
			}
		}
	}
};
#pragma endregion

#pragma region Piecese
class Piece {
public:
	sqar_t Pos = { 0 };
	_Ty Ty{};
	_Sta state = Active;
	bool IsMoved = 0;
	bool TheRayPath(sqar_t Epos, Piece thePieceSet[], Piece theOtherSet[]) const {

		if (this->Ty == _Wn || this->Ty == _Bn) {
			int16_t DeltaX = (int16_t)abs((Epos.x - Pos.x));
			int16_t DeltaY = (int16_t)abs((Epos.x - Pos.x));

			if ((DeltaX == 2 * Pixel && DeltaY == Pixel) ||
				(DeltaX == Pixel && DeltaY == 2 * Pixel))
			{
				return true;
			}
			Out0
		}
		V2 BPos = {
			.x = this->Pos.x,
			.y = this->Pos.y
		};
		V2 EPos = {
			.x = Epos.x,
			.y = Epos.y
		};

		RAY Path(BPos, EPos);

		V2 Thechecker = Path.bPos;

		for (int Move = 1; Move < Path.Distance; Move++)
		{
			Thechecker.x += Path.direction.x;
			Thechecker.y += Path.direction.y;

			for (int PieceIndex = 0; PieceIndex < 16; PieceIndex++) {

				if ((thePieceSet[PieceIndex].Pos.CheckV2(Thechecker) && thePieceSet[PieceIndex].state != UnActive)) {
					return false;
				}
				if ((theOtherSet[PieceIndex].Pos.CheckV2(Thechecker) && theOtherSet[PieceIndex].state != UnActive)) {
					return false;
				}
			}
		}

		return true;
	}
	void Draw(Color color) const {
		DrawTexture(tex[((int)(Ty))], (int)(Pos.x), (int)(Pos.y), color);
	};
	void set(sqar_t rec, _Ty ty) {
		this->Pos = rec;
		this->Ty = ty;
	};
};
typedef Piece* PTyP, TyP;
PTyP GetPiece(TyP Set[], PPos pos) {
	for (int i = 0; i < 16;i++) {
		if (Set[i].Pos == *pos && (Set[i].state != UnActive)) {
			return &Set[i];
		}
	}
	return nullptr;
}
void DrawAllPiecese(TyP White[], TyP Black[]) {
	for (int i = 0; i < 16;i++) {
		if (White[i].state == Selected) {
			White[i].Draw(SelectColor);
		}
		else if (White[i].state == Active) {
			White[i].Draw(WHITE);
		}
	}
	for (int i = 0; i < 16;i++) {
		if (Black[i].state == Selected) {
			Black[i].Draw(SelectColor);
		}
		else if (Black[i].state == Active) {
			Black[i].Draw(WHITE);
		}
	}
}
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
#pragma endregion

#pragma region Selectors
Piece* SelectPiece(Piece SetofPieces_16_Piece[]) {
	for (int indexofPiece = 0; indexofPiece < 16; indexofPiece++) {
		if (IsMousePressed && SetofPieces_16_Piece[indexofPiece].Pos.IsClicked() &&
			SetofPieces_16_Piece[indexofPiece].state != UnActive) {
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
#pragma endregion

#pragma region Movement
PTyP GetTheking(TyP Set[])
{
	for (int i = 0; i < 16; i++) {
		if (Set[i].Ty == _Wk || Set[i].Ty == _Bk) {
			return &Set[i];
		}
	}
	return nullptr;
};
bool IsPieceHere(sqar_t* Pos, Piece SetofPiecese[]) {
	for (int i = 0; i < 16;i++) {
		if (SetofPiecese[i].Pos == *Pos && SetofPiecese[i].state != UnActive) {
			return true;
		}
	}
	return false;
}
bool IstheKingsafe(Piece* King, Piece KingSet[], Piece OtherSet[])
{
	for (int index = 0; index < 16; index++) {
		if (OtherSet[index].TheRayPath(King->Pos, KingSet, OtherSet)) {
			return false;
		}
	}
	return true;
};
bool IsTheMoveVaild(Piece* CurrentPiece, Piece CurrentPieceSet[], sqar_t* TheSelectedPosition, Board* Board_, Piece TheOtherSet[], _Mty* Mt) {
	int16_t DeltaX = (int16_t)(abs(CurrentPiece->Pos.x - TheSelectedPosition->x));
	int16_t DeltaY = (int16_t)(abs(CurrentPiece->Pos.y - TheSelectedPosition->y));
	int16_t PDeltaY = (int16_t)(CurrentPiece->Pos.y - TheSelectedPosition->y);
	int16_t PDeltaX = (int16_t)(TheSelectedPosition->x - CurrentPiece->Pos.x);

	switch (CurrentPiece->Ty) {
#pragma region WP
	case _Wp:
		if (!(CurrentPiece->IsMoved)) {
			if ((DeltaX == 0 && ((PDeltaY == Pixel) || (PDeltaY == 2 * Pixel)))) {
				*Mt = PieceMovements;
				return true;
			}
			else if ((DeltaX == Pixel && PDeltaY == Pixel)) {
				if (IsPieceHere(TheSelectedPosition, TheOtherSet)) {
					*Mt = Capture;
					return true;
				}
			}
		}
		else {
			if ((DeltaX == 0 && PDeltaY == Pixel)) {

				*Mt = PieceMovements;
				return true;
			}
			else if ((DeltaX == Pixel && PDeltaY == Pixel)) {
				if (IsPieceHere(TheSelectedPosition, TheOtherSet)) {
					*Mt = Capture;
					return true;
				}
			}
		}
		break;
#pragma endregion
#pragma region BP
	case _Bp:
		if (!(CurrentPiece->IsMoved)) {
			if ((DeltaX == 0 && ((PDeltaY == -(Pixel)) || (PDeltaY == (-2) * Pixel)))) {
				*Mt = PieceMovements;
				return true;
			}
			else if ((DeltaX == (Pixel) && PDeltaY == (-Pixel))) {
				if (IsPieceHere(TheSelectedPosition, TheOtherSet)) {
					*Mt = Capture;
					return true;
				}
			}
		}
		else {
			if ((DeltaX == 0 && PDeltaY == -(Pixel)))
			{
				*Mt = PieceMovements;
				return true;
			}
			else if ((DeltaX == (Pixel) && PDeltaY == (-Pixel))) {
				if (IsPieceHere(TheSelectedPosition, TheOtherSet)) {
					*Mt = Capture;
					return true;
				}
			}
		}
		break;
#pragma endregion 
#pragma region King
	case _Wk:case _Bk:
		if ((DeltaX == Pixel || DeltaX == 0) && (DeltaY == Pixel || DeltaY == 0)) {
			if (IsPieceHere(TheSelectedPosition, TheOtherSet)) {
				*Mt = Capture;
				return true;
			}
			*Mt = PieceMovements;
			return true;
		}

		if (DeltaY == 0 && PDeltaX == (2 * Pixel))
		{
			*Mt = ShortCasling;
			return true;
		}

		if (DeltaY == 0 && PDeltaX == (-2 * Pixel))
		{
			*Mt = LongCasling;
			return true;
		}

		break;
#pragma endregion
#pragma region Queen
	case _Wq:case _Bq:
		if (DeltaX == DeltaY)
		{
			if (IsPieceHere(TheSelectedPosition, TheOtherSet)) {
				*Mt = Capture;
				return true;
			}
			*Mt = PieceMovements;
			return true;
		}
		else if ((DeltaX == 0 && DeltaY != 0) || (DeltaY == 0 && DeltaX != 0))
		{
			if (IsPieceHere(TheSelectedPosition, TheOtherSet)) {
				*Mt = Capture;
				return true;
			}
			*Mt = PieceMovements;
			return true;
		}
		break;
#pragma endregion
#pragma region Rook
	case _Wr:case _Br:
		if ((DeltaX == 0 && DeltaY != 0) ||
			(DeltaY == 0 && DeltaX != 0))
		{
			if (IsPieceHere(TheSelectedPosition, TheOtherSet)) {
				*Mt = Capture;
				return true;
			}
			*Mt = PieceMovements;
			return true;
		}
		break;
#pragma endregion
#pragma region Knight
	case _Wn:case _Bn:
		if ((DeltaX == 2 * Pixel && DeltaY == Pixel) ||
			(DeltaX == Pixel && DeltaY == 2 * Pixel))
		{
			if (IsPieceHere(TheSelectedPosition, TheOtherSet)) {
				*Mt = Capture;
				return true;
			}
			*Mt = PieceMovements;
			return true;
		}
		break;
#pragma endregion
#pragma region Bioshp
	case _Wb:case _Bb:
		if (DeltaX == DeltaY)
		{
			if (IsPieceHere(TheSelectedPosition, TheOtherSet)) {
				*Mt = Capture;
				return true;
			}
			*Mt = PieceMovements;
			return true;
		}
		break;
	}
#pragma endregion
	return false;
}
bool TheMovement(Board* BoardofChess_8X8size, Piece CurrentPieceSet[], Piece theOtherSet[]) {

	static Piece* CurrentPiece = nullptr;
	Piece* NewPiece = nullptr;
	sqar_t* TheSelectedPosition = nullptr;


#pragma region SelectLogic

	NewPiece = SelectPiece(CurrentPieceSet);
	if (NewPiece != nullptr) {
		if (CurrentPiece != nullptr) {
			CurrentPiece->state = Active;
		}
		CurrentPiece = NewPiece;
		CurrentPiece->state = Selected;
	}
#pragma endregion

	if (CurrentPiece != nullptr) {
		TheSelectedPosition = SelectPosition(*BoardofChess_8X8size);
		if (TheSelectedPosition != nullptr && (CurrentPiece->Pos != *TheSelectedPosition)) {
			_Mty Mt;
			bool TheMoveIsValid = IsTheMoveVaild(CurrentPiece, CurrentPieceSet, TheSelectedPosition, BoardofChess_8X8size, theOtherSet, &Mt);
			if (TheMoveIsValid) {
#pragma region MovementLogic
				switch (Mt)
				{


				case PieceMovements:
				{
					CurrentPiece->Pos = *TheSelectedPosition;
				} break;
				case Capture:
				{
					CurrentPiece->Pos = *TheSelectedPosition;
					for (int i = 0; i < 16; i++) {
						if (theOtherSet[i].Pos == *TheSelectedPosition) {
							theOtherSet[i].state = UnActive;
						}
					}
				} break;
				case LongCasling:
				{
					if (CurrentPiece->IsMoved) {
						break;
					}

					sqar_t Rookpos;

					Rookpos.x = 0;
					Rookpos.y = CurrentPiece->Pos.y;

					PTyP Rook = GetPiece(CurrentPieceSet, &Rookpos);
					if (Rook != nullptr) {
						if ((!(Rook->IsMoved)) && Rook->TheRayPath(CurrentPiece->Pos, CurrentPieceSet, theOtherSet)) {
							CurrentPiece->Pos = *TheSelectedPosition;
							Rookpos.x = CurrentPiece->Pos.x + Pixel;
							Rook->Pos = Rookpos;
						}
					}

				}break;
				case ShortCasling:
				{
					if (CurrentPiece->IsMoved) {
						break;
					}
					sqar_t Rookpos;
					Rookpos.x = Window::SrcWidth - Pixel;
					Rookpos.y = CurrentPiece->Pos.y;
					PTyP Rook = GetPiece(CurrentPieceSet, &Rookpos);
					if (Rook != nullptr) {
						if ((!(Rook->IsMoved)) && Rook->TheRayPath(CurrentPiece->Pos, CurrentPieceSet, theOtherSet)) {
							CurrentPiece->Pos = *TheSelectedPosition;
							Rookpos.x = CurrentPiece->Pos.x - Pixel;
							Rook->Pos = Rookpos;
						}
					}
				}break;
				
				case Promote:
				{
				

				}break;
				case Check:
				{

				}break;
				case CheckMate:
				{

				}break;

				}

#pragma endregion
				CurrentPiece->IsMoved = true;
				CurrentPiece->state = Active;
				CurrentPiece = nullptr;
				return true;
			}
		}
	}
	return false;
}
#pragma endregion


