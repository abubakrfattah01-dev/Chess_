#if 1
#include "Rework2.h"
int main() {
	InitWindow(Window::SrcWidth, Window::SrcHeigth, Window::Title);
	SetTargetFPS(60);
	Image Icon = { 0 };
	Icon = LoadImage(Window::Icon_local);
	SetWindowIcon(Icon);
	UnloadImage(Icon);
	Board BoardG;

	Piece Bpieces[16], Wpieces[16];
	InitSetPiece(&BoardG, Wpieces, 1);
	InitSetPiece(&BoardG, Bpieces, 0);
	Game CurrentGameState = Player_One_Turn;
	InitTexture();
	while (!WindowShouldClose())
	{
		BeginDrawing();
		ClearBackground(RAYWHITE);
		BoardG.DrawBoard();
		DrawAllPiecese(Wpieces, Bpieces);

#if WorkMode
		if (CurrentGameState == Player_One_Turn) {
			if (TheMovement(&BoardG, Wpieces,  Bpieces)) {
				CurrentGameState = Player_Two_Turn;
			}
		}

		else if (CurrentGameState == Player_Two_Turn) {
			if (TheMovement(&BoardG, Bpieces,  Wpieces)) {
				CurrentGameState = Player_One_Turn;
			}
		}
#else
		
		TheMovement(&BoardG, Bpieces, Wpieces);
#endif
		EndDrawing();
	}
	deInitTexture();
	CloseWindow();
	return 0;
}
#endif // WorkMode
