#include <iostream>
#include <raylib.h>

int main()
{
	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	InitWindow(800, 600, "Terraria Clone");

	while (!WindowShouldClose())
	{
		BeginDrawing();
		ClearBackground(RAYWHITE);

		DrawRectangle(50, 50, 100, 100, {255, 0, 0, 127});
		DrawRectangle(75, 75, 100, 100, {0, 255, 0, 127});

		EndDrawing();
	}

	return 0;
}