#include "client.h"

int StudioCheckBBox(void)
{
	if ((cvar.rage_active && cvar.rage_fov > 45) ||
		(cvar.legit[g_Local.weapon.m_iWeaponID].active && cvar.legit[g_Local.weapon.m_iWeaponID].fov > 45) ||
		(cvar.knifebot_active && cvar.knifebot_fov > 45))
		return 1;
	return g_Studio.StudioCheckBBox();
}

void StudioSetRemapColors(int top, int bottom)
{
	GetHitboxes();
	g_Studio.StudioSetRemapColors(top, bottom);
}

void StudioDrawPoints()
{
	ViewModelSkin();
	g_Studio.StudioDrawPoints();
}

void StudioEntityLight(struct alight_s* plight)
{
	cl_entity_s* ent = g_Studio.GetCurrentEntity();
	if (ent && ent == &playerdummy)
	{
		plight->ambientlight = 128;
		plight->shadelight = 192;
		plight->color.x = 1.f;
		plight->color.y = 1.f;
		plight->color.z = 1.f;
		Vector angles = vViewanglesFront;
		if (espview == 1)
			angles = vViewanglesBack;
		Vector forward, right;
		g_Engine.pfnAngleVectors(angles, forward, right, NULL);
		Vector origin = plight->plightvec;
		
		float dir3 = -100;
		if (model_pos_x + (modelscreenw / 2) + dummywidth > ImGui::GetIO().DisplaySize.x / 2)
			dir3 = 100;

		static int move2 = 0;
		static float dir2 = 0;
		static int move = 0;
		static float dir = -100;
		static DWORD tickcount = GetTickCount();
		if (GetTickCount() - tickcount > 0)
		{
			if (!move2 && dir2 < 100)
				dir2++;
			if (dir2 == 100)
				move2 = 1;
			if (move2 && dir2 > -100)
				dir2--;
			if (dir2 == -100)
				move2 = 0;

			if (!move && dir < 100)
				dir++;
			if (dir == 100)
				move = 1;
			if (move && dir > -100)
				dir--;
			if (dir == -100)
				move = 0;
			tickcount = GetTickCount();
		}
		if(cvar.chams_player == 2 && strstr(ent->model->name, "/player/") ||
			cvar.chams_world == 2 && !strstr(ent->model->name, "/player/"))
			origin = origin + forward * dir2 + right * dir;
		else
			origin = origin + right * dir3;

		plight->plightvec = origin;
	}
	if (ent && ent->player && cvar.visual_lambert)
	{
		plight->ambientlight = 128;
		plight->shadelight = 192;
		plight->color.x = 1.f;
		plight->color.y = 1.f;
		plight->color.z = 1.f;
	}
	g_Studio.StudioEntityLight(plight);
}

void HookStudioFunctions()
{
	g_pStudio->StudioEntityLight = StudioEntityLight;
	g_pStudio->StudioDrawPoints = StudioDrawPoints;
	g_pStudio->StudioSetRemapColors = StudioSetRemapColors;
	g_pStudio->StudioCheckBBox = StudioCheckBBox;
}