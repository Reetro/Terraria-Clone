#include <iostream>
#include <raylib.h>
#include <imgui.h>
#include <rlImGui.h>
#include "gameMain.h"

int main()
{
#if PRODUCTION_BUILD
	SetTraceLogCallback(LOG_NONE);
#endif

	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	InitWindow(800, 600, "Terraria Clone");
	SetExitKey(KEY_NULL); // Disable esc key from closing window
	SetTargetFPS(240);

	rlImGuiSetup(true);

	if (!initGame())
	{
		return 0;
	}

#pragma region ImGui Setup
	ImGuiIO& io = ImGui::GetIO();
	io.FontGlobalScale = 2;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
#pragma endregion ImGui Setup

	while (!WindowShouldClose())
	{
		BeginDrawing();
		ClearBackground(BLACK);

		rlImGuiBegin();

#pragma region imgui Docking
		ImGui::PushStyleColor(ImGuiCol_WindowBg, {});
		ImGui::PushStyleColor(ImGuiCol_DockingEmptyBg, {});
		ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
		ImGui::PopStyleColor(2);
#pragma endregion imgui Docking

		if (!updateGame())
		{
			CloseWindow();
		}

		rlImGuiEnd();

		EndDrawing();
	}

	CloseWindow();

	closeGame();

	rlImGuiShutdown();

	return 0;
}