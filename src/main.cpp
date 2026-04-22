#include <iostream>
#include <raylib.h>
#include <imgui.h>
#include <rlImGui.h>

int main()
{
	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	InitWindow(800, 600, "Terraria Clone");

	rlImGuiSetup(true);

#pragma region ImGui Setup
	ImGuiIO& io = ImGui::GetIO();
	io.FontGlobalScale = 2;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
#pragma endregion ImGui Setup

	while (!WindowShouldClose())
	{
		BeginDrawing();
		ClearBackground(RAYWHITE);

#pragma region imgui Docking
		rlImGuiBegin();

		ImGui::PushStyleColor(ImGuiCol_WindowBg, {});
		ImGui::PushStyleColor(ImGuiCol_DockingEmptyBg, {});
		ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
		ImGui::PopStyleColor(2);
#pragma endregion imgui Docking

		DrawRectangle(50, 50, 100, 100, {255, 0, 0, 127});
		DrawRectangle(75, 75, 100, 100, {0, 255, 0, 127});

		ImGui::Begin("test");

		ImGui::Text("Hello from ImGui");

		if (ImGui::Button("Test button"))
		{
			std::cout << "Hello from ImGui!" << std::endl;
		}

		ImGui::End();

		ImGui::Begin("Test 2");
		ImGui::Text("Slider Test");
		ImGui::Separator();

		ImGui::NewLine();
		static float a = 0;
		ImGui::SliderFloat("Slider", &a, 0.0f, 1.0f);

		ImGui::End();

		rlImGuiEnd();

		EndDrawing();
	}

	rlImGuiShutdown();

	CloseWindow();

	return 0;
}