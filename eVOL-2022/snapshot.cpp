#include "client.h"

bool ScreenFirst = true, DrawVisuals;
PBYTE BufferScreen;
int temp;
DWORD dwSize, time_scr;

void Snapshot()
{
	if (!bInitializeImGui)
		return;

	if (cvar.snapshot_memory)
	{
		static bool bAntiSSTemp = true;
		if (bAntiSSTemp)
		{
			time_scr = GetTickCount();
			temp = 0;

			bAntiSSTemp = false;
		}

		if (GetTickCount() - time_scr > cvar.snapshot_time * 1000)
		{
			DrawVisuals = false;
			temp++;

			if (temp > 4)
			{
				bAntiSSTemp = true;
				ScreenFirst = true;
				DWORD sz = ImGui::GetIO().DisplaySize.x * ImGui::GetIO().DisplaySize.y * 3;
				free((PBYTE)BufferScreen);
				PBYTE buf = (PBYTE)malloc(sz);
				glReadPixels(0, 0, ImGui::GetIO().DisplaySize.x, ImGui::GetIO().DisplaySize.y, GL_RGB, GL_UNSIGNED_BYTE, buf);
				free((PBYTE)buf);
			}
		}
	}
	static bool FirstFrame = true;
	if (FirstFrame)
	{
		DWORD sz = ImGui::GetIO().DisplaySize.x * ImGui::GetIO().DisplaySize.y * 3;
		PBYTE buf = (PBYTE)malloc(sz);
		glReadPixels(0, 0, ImGui::GetIO().DisplaySize.x, ImGui::GetIO().DisplaySize.y, GL_RGB, GL_UNSIGNED_BYTE, buf);
		free((PBYTE)buf);

		FirstFrame = false;
	}
}