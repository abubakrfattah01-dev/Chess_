#if 1
#include "Main2.h"
int main() {
    InitWindow(Window::SrcWidth, Window::SrcHeigth, Window::Title);
    SetTargetFPS(60);
    Image Icon = { 0 };
    Icon = LoadImage(Window::Icon_local);
    SetWindowIcon(Icon);
    UnloadImage(Icon);
    Board BoardG;
   // __Game turnG = Player_One_Turn;
    Piece Bpieces[16], Wpieces[16];
    Init_16Piece(&BoardG, Wpieces, 1);
    Init_16Piece(&BoardG, Bpieces, 0);
    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        BoardG.DrawBoard();
        DrawAllPieces(Wpieces,Bpieces);

  
       
       // TheMovementOfPieces(Bpieces, BoardG, Wpieces);
        TheMovementOfPieces(Wpieces, &BoardG, Bpieces);
        

        EndDrawing();
    }
    CloseWindow();
    return 0;
}
#endif 
       