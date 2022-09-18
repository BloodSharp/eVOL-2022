#include "client.h"

bool WorldToScreen(float* pflOrigin, float* pflVecScreen)
{
	int iResult = g_Engine.pTriAPI->WorldToScreen(pflOrigin, pflVecScreen);
	if (pflVecScreen[0] < 1.5f && pflVecScreen[1] < 1.5f && pflVecScreen[0] > -1.5f && pflVecScreen[1] > -1.5f && !iResult)
	{
		pflVecScreen[0] = pflVecScreen[0] * (ImGui::GetIO().DisplaySize.x / 2) + (ImGui::GetIO().DisplaySize.x / 2);
		pflVecScreen[1] = -pflVecScreen[1] * (ImGui::GetIO().DisplaySize.y / 2) + (ImGui::GetIO().DisplaySize.y / 2);
		return true;
	}
	return false;
}

void ScreenToWorld(float* screen, float* world)
{
	screen[0] = (screen[0] - (ImGui::GetIO().DisplaySize.x / 2)) / (ImGui::GetIO().DisplaySize.x / 2);
	screen[1] = -(screen[1] - (ImGui::GetIO().DisplaySize.y / 2)) / (ImGui::GetIO().DisplaySize.y / 2);
	g_Engine.pTriAPI->ScreenToWorld(screen, world);
}

void DrawFullScreenWindow()
{
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
	ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
	ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize, ImGuiCond_Always);
	ImGui::Begin("FullScreen", reinterpret_cast<bool*>(true), ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoInputs);
	{
		DrawSpawn();
		DrawRouteLine();
		DrawLongJump();
		DrawSkeletonWorld();
		DrawWorldEsp();
		DrawPlayerSoundNoIndexEsp();
		DrawPlayerSoundIndexEsp();
		DrawSkeletonPlayer();
		DrawPlayerEsp();
		DrawAimbot();
		DrawTraceGrenade();
		DrawCrossHair();
		DrawWeaponName();
		DrawModeChange();
		DrawAimBotFOV();
	}
	ImGui::End();
	ImGui::PopStyleColor(); 
}