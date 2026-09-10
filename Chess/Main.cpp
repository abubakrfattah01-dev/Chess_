#if 1
#include "Rework.h"
int main() {
	InitWindow(Window::SrcWidth, Window::SrcHeigth, Window::Title);
	SetTargetFPS(60);
	Image Icon = { 0 };
	Icon = LoadImage(Window::Icon_local);
	SetWindowIcon(Icon);
	UnloadImage(Icon);
	Board BoardG;
	__Game CurrentGameState = Player_One_Turn;
	Piece Bpieces[16], Wpieces[16];
	InitSetPiece(&BoardG, Wpieces, 1);
	InitSetPiece(&BoardG, Bpieces, 0);
    InitTexture();
	while (!WindowShouldClose())
	{
		BeginDrawing();
		ClearBackground(RAYWHITE);
		BoardG.DrawBoard();
		DrawAllPieces(Wpieces, Bpieces);


		if (CurrentGameState == Player_One_Turn) {
			if (TheMovement(Wpieces, &BoardG, Bpieces, CurrentGameState)) {
				CurrentGameState = Player_Two_Turn;
			}
		}

		else if (CurrentGameState == Player_Two_Turn) {
			if (TheMovement(Bpieces, &BoardG, Wpieces, CurrentGameState)) {
				CurrentGameState = Player_One_Turn;
			}
		}

		else if (CurrentGameState == player_One_In_check) {

		}
		else if (CurrentGameState == player_Two_In_check) {

		}
		EndDrawing();
	}
	deInitTexture();
	CloseWindow();
	return 0;
}
#endif 
