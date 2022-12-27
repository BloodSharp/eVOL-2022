#include "client.h"

std::vector<byte> BufferScreen;
bool DrawVisuals = true;
DWORD time_scr = 0;

enum
{
	EVOL_ANTISCREENSHOT_IS_NOT_TAKING_SCREENSHOT,
	EVOL_ANTISCREENSHOT_IS_CLEAN_FRAME,
	EVOL_ANTISCREENSHOT_IS_RECORDING_SCREEN,
};

int g_iScreenShotStatus = EVOL_ANTISCREENSHOT_IS_NOT_TAKING_SCREENSHOT;

void Snapshot()
{
	if (!bInitializeImGui)
	{
		g_iScreenShotStatus = EVOL_ANTISCREENSHOT_IS_NOT_TAKING_SCREENSHOT;
		DrawVisuals = true;
		return;
	}

	static bool bDoOnce = true;
	if (bDoOnce)
	{
		time_scr = GetTickCount();

		bDoOnce = false;
	}

	if (cvar.snapshot_memory)
	{
		if (g_iScreenShotStatus == EVOL_ANTISCREENSHOT_IS_NOT_TAKING_SCREENSHOT)
		{
			if ((GetTickCount() - time_scr) > (cvar.snapshot_time * 1000))
			{
				ImVec2& iv2DisplaySize = ImGui::GetIO().DisplaySize;
				auto bytesToRecord = iv2DisplaySize.x * iv2DisplaySize.y * 3;
				//if (!BufferScreen.size())
					/*BufferScreen(bytesToRecord);
				else*/
					BufferScreen.resize(bytesToRecord);
				DrawVisuals = false;
				g_iScreenShotStatus = EVOL_ANTISCREENSHOT_IS_CLEAN_FRAME;
			}
		}
		else if (g_iScreenShotStatus == EVOL_ANTISCREENSHOT_IS_CLEAN_FRAME)
		{
			// Do nothing because we need a clean screen...
			g_iScreenShotStatus = EVOL_ANTISCREENSHOT_IS_RECORDING_SCREEN;
		}
		else if (g_iScreenShotStatus == EVOL_ANTISCREENSHOT_IS_RECORDING_SCREEN)
		{
			ImVec2& iv2DisplaySize = ImGui::GetIO().DisplaySize;
			auto bytesToRecord = iv2DisplaySize.x * iv2DisplaySize.y * 3;
			if (BufferScreen.size() <= bytesToRecord)
			{
				time_scr = GetTickCount();
				if(pglReadPixels)
					pglReadPixels(0, 0, iv2DisplaySize.x, iv2DisplaySize.y, GL_RGB, GL_UNSIGNED_BYTE, BufferScreen.data());
				DrawVisuals = true;
				g_iScreenShotStatus = EVOL_ANTISCREENSHOT_IS_NOT_TAKING_SCREENSHOT;
			}
		}
	}
	else
		g_iScreenShotStatus = EVOL_ANTISCREENSHOT_IS_NOT_TAKING_SCREENSHOT;
}