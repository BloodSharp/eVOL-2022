#include "client.h"

char hackdir[256];
char hldir[256];

DWORD WINAPI Hook()
{
	while (!c_Offset.GetRendererInfo())
		Sleep(100);

	g_pClient = (cl_clientfunc_t*)c_Offset.ClientFuncs();

	g_pEngine = (cl_enginefunc_t*)c_Offset.EngineFuncs();

	g_pStudio = (engine_studio_api_t*)c_Offset.StudioFuncs();

	g_pInterface = (r_studio_interface_t*)c_Offset.FindInterface((DWORD)*g_pClient->HUD_GetStudioModelInterface);

	g_pStudioModelRenderer = (StudioModelRenderer_t*)c_Offset.FindStudioModelRenderer((DWORD)g_pInterface->StudioDrawModel);

	PreS_DynamicSound_s = (decltype(PreS_DynamicSound_s))c_Offset.PreS_DynamicSound();
	CL_Move_s = (decltype(CL_Move_s))c_Offset.CL_Move();

	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(&(PVOID&)PreS_DynamicSound_s, PreS_DynamicSound);
	DetourAttach(&(PVOID&)CL_Move_s, CL_Move);
	DetourTransactionCommit();

	c_Offset.GlobalTime();

	c_Offset.dwSpeedPointer = (DWORD)c_Offset.FindSpeed();

	c_Offset.PatchInterpolation();

	LoadCvar();

	LoadHitbox();

	LoadHitboxLegit();

	LoadHitboxFake();

	LoadView();

	Viewmodelscan();

	LoadViewChanger();

	while (!g_Client.V_CalcRefdef)
		RtlCopyMemory(&g_Client, g_pClient, sizeof(cl_clientfunc_t));

	while (!g_Engine.V_CalcShake)
		RtlCopyMemory(&g_Engine, g_pEngine, sizeof(cl_enginefunc_t));

	while (!g_Studio.StudioSetupSkin)
		RtlCopyMemory(&g_Studio, g_pStudio, sizeof(engine_studio_api_t));

	while (!g_Interface.StudioDrawModel)
		RtlCopyMemory(&g_Interface, g_pInterface, sizeof(r_studio_interface_t));

	while (!g_StudioModelRenderer.StudioSlerpBones)
		RtlCopyMemory(&g_StudioModelRenderer, g_pStudioModelRenderer, sizeof(StudioModelRenderer_t));

	while (!pmove)
		pmove = (playermove_t*)c_Offset.FindPlayerMove();

	while (!pUserMsgBase)
		pUserMsgBase = c_Offset.FindUserMsgBase();

	if (c_Offset.HLType != RENDERTYPE_UNDEFINED)
	{
		g_pfnSteam_GSInitiateGameConnection = (decltype(g_pfnSteam_GSInitiateGameConnection))GetDestination(c_Offset.Steam_GSInitiateGameConnection());
		SetDestination(c_Offset.Steam_GSInitiateGameConnection(), (uintptr_t)&Steam_GSInitiateGameConnection_Hooked);
	}

	HookClientFunctions();

	HookStudioFunctions();

	HookStudiModelRendererFunctions();

	HookUserMessages();

	HookOpenGL();

	InitBass();

	return NULL;
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD dwReason, LPVOID lpReserved)
{
	if (dwReason == DLL_PROCESS_ATTACH)
	{
		if (GetLastError() != ERROR_ALREADY_EXISTS)
		{
			DisableThreadLibraryCalls(hinstDLL);
			GetModuleFileName(hinstDLL, hackdir, 255);
			char* pos = hackdir + strlen(hackdir);
			while (pos >= hackdir && *pos != '\\')
				--pos;
			pos[1] = 0;
			GetModuleFileName(GetModuleHandle(NULL), hldir, 255);
			pos = hldir + strlen(hldir);
			while (pos >= hldir && *pos != '\\')
				--pos;
			pos[1] = 0;
			CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)Hook, NULL, NULL, NULL);
		}
	}
	return TRUE;
}