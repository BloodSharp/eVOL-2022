#include "client.h"

void LoadTextureImageBack(char* image, int index)
{
	char filename[256];
	int width, height;
	sprintf(filename, "%s%s", hackdir, image);
	GLint last_texture;
	glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture);
	if (texture_id[index])glDeleteTextures(1, &texture_id[index]);
	GLuint glindex;
	glGenTextures(1, &glindex);
	texture_id[index] = glindex + 20000;
	glBindTexture(GL_TEXTURE_2D, texture_id[index]);
	unsigned char* soilimage = SOIL_load_image(filename, &width, &height, 0, SOIL_LOAD_RGBA);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, soilimage);
	SOIL_free_image_data(soilimage);
	glBindTexture(GL_TEXTURE_2D, last_texture);
}

bool popoup = true;
void DrawPopupWindow()
{
	static DWORD closewindow = GetTickCount() + 21000;
	if (closewindow < GetTickCount())
		popoup = false;
	
	static bool hidemouse = true;
	if (hidemouse && !popoup)
	{
		if(!bShowMenu && !bInputActive)
			ImGui::GetIO().MouseDrawCursor = false;
		hidemouse = false;
	}
	if (GetTickCount() - HudRedraw <= 100)
		popoup = false;
	if (!popoup)
		return;
	
	static bool showmouse = true;
	if (showmouse)
		ImGui::GetIO().MouseDrawCursor = true,
		LoadTextureImageBack((char*)"texture/popup/back.png", BACKGRND);
	showmouse = false;

	ImGui::SetNextWindowPos(ImVec2(100, 100), ImGuiCond_Once);
	if(ImGui::Begin("##noname", &popoup, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse))
	{
		if (ImGui::IsWindowHovered())
			popoup = false;
		ImGui::GetWindowDrawList()->AddImage((GLuint*)texture_id[BACKGRND], ImVec2(ImGui::GetCurrentWindow()->Pos.x + 6, ImGui::GetCurrentWindow()->Pos.y + 6), ImVec2(ImGui::GetCurrentWindow()->Pos.x + ImGui::GetCurrentWindow()->Size.x - 6, ImGui::GetCurrentWindow()->Pos.y + ImGui::GetCurrentWindow()->Size.y - 6));
		
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), "\n  You are injected!  ");
		if ((closewindow - GetTickCount()) / 100 % 10)
		{
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "\n    Closing in %d", (closewindow - GetTickCount()) / 1000);
		}
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "\n  Multihack by:  \n\n\n\n\n\n\n\n");
		
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), "\n  Special thanks: BloodSharp, oxiKKK <3  ");
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), "\n  ATTENTION! Menu only active in game!  \n");
		if (cvar.gui_key >= 0 && cvar.gui_key < 255)
			ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "  Menu key is [%s]!  \n\n", KeyEventChar((int)cvar.gui_key));
	}
	ImGui::End();
}