#pragma once

#pragma warning(disable:4005)
#pragma warning(disable:4018)
#pragma warning(disable:4244)
#pragma warning(disable:4715)
#pragma warning(disable:4819)
#pragma warning(disable:4996)
#pragma warning(disable:4700)
#pragma warning(disable:4305)
#pragma warning(disable:4410)
#pragma warning(disable:4409)
#pragma warning(disable:4002)

#define _USE_MATH_DEFINES
#define IMGUI_DEFINE_MATH_OPERATORS

#include <ctime>
#include <map>
#include <vector>
#include <fstream>
#include <windows.h>
#include <iostream>
#include <array>
#include <deque>
#include <math.h>
#include <list>

//using namespace std;
extern char hackdir[256];
extern char hldir[256];

#include "bass/dll.h"
#include "bass/bass.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_opengl2.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_internal.h"
#include "imgui/soil.h"
#include "imgui/gl.h"
#include "imgui/glu.h"

//#include "vector/vector.h"
#include "vector/qangle.h"

// Half-Life SDK files
#include "../halflife/cl_dll/wrect.h"
#include "../halflife/cl_dll/cl_dll.h"
#include "../halflife/engine/cdll_int.h"
#include "../halflife/common/const.h"
#include "../halflife/engine/progdefs.h"
#include "../halflife/engine/eiface.h"
#include "../halflife/engine/edict.h"
#include "../halflife/common/studio_event.h"
#include "../halflife/common/entity_types.h"
#include "../halflife/common/r_efx.h"
#include "../halflife/common/pmtrace.h"
#include "../halflife/public/keydefs.h"
#include "../halflife/pm_shared/pm_movevars.h"
#include "../halflife/common/ref_params.h"
#include "../halflife/common/screenfade.h"
#include "../halflife/common/event_api.h"
#include "../halflife/common/com_model.h"
#include "../halflife/common/net_api.h"

#include "../halflife/common/parsemsg.h"
//#include "../ValveSDK/sprites.h" ?????
#include "../halflife/common/r_studioint.h"
#include "../halflife/common/triangleapi.h"
#include "../halflife/pm_shared/pm_defs.h"
#include "../halflife/engine/studio.h"
#include "../halflife/common/weaponinfo.h"


#define	K_0 48
#define	K_1 49
#define	K_2 50
#define	K_3 51
#define	K_4 52
#define	K_5 53
#define	K_6 54
#define	K_7 55
#define	K_8 56
#define	K_9 57

#define	K_A	97
#define	K_B	98
#define	K_C	99
#define	K_D	100
#define	K_E	101
#define	K_F	102
#define	K_G	103
#define	K_H	104
#define	K_I	105
#define	K_J	106
#define	K_K	107
#define	K_L	108
#define	K_M	109
#define	K_N	110
#define	K_O	111
#define	K_P	112
#define	K_Q	113
#define	K_R	114
#define	K_S	115
#define	K_T	116
#define	K_U	117
#define	K_V	118
#define	K_W	119
#define	K_X	120
#define	K_Y	121
#define	K_Z	122

#include "../halflife/public/keydefs.h"

//#include "../halflife/cl_dll/util_vector.h"
#ifndef VectorAdd
#define VectorAdd(a,b,c) {(c)[0]=(a)[0]+(b)[0];(c)[1]=(a)[1]+(b)[1];(c)[2]=(a)[2]+(b)[2];}
#endif
#ifndef VectorCopy
#define VectorCopy(a,b) {(b)[0]=(a)[0];(b)[1]=(a)[1];(b)[2]=(a)[2];}
#endif
#ifndef VectorClear
#define VectorClear(a) { a[0]=0.0;a[1]=0.0;a[2]=0.0;}
#endif
#ifndef VectorSubtract
#define VectorSubtract(a,b,c) {(c)[0]=(a)[0]-(b)[0];(c)[1]=(a)[1]-(b)[1];(c)[2]=(a)[2]-(b)[2];}
#endif
#ifndef VectorIsZero
#define VectorIsZero(a) (a[0] == 0.0f && a[1] == 0.0f && a[2] == 0.0f)
#endif
#ifndef VectorIsZero2D
#define VectorIsZero2D(a) (a[0] == 0.0f && a[1] == 0.0f)
#endif

float AngleBetween(Vector& v1, Vector& v2);
Vector& CrossProduct(Vector& vThis, const Vector& a, const Vector& b);
QAngle ToEulerAngles(Vector vThis);
QAngle ToEulerAngles(Vector vThis, Vector* PseudoUp);
void AngleMatrix(QAngle& Rotation, float(*matrix)[3]);
void VectorRotate(Vector& vThis, Vector& In, QAngle& Rotation);

#include "../Detours/src/detours.h"
#include "resources/defs.h"
#include "resources/stringfinder.h"
#include "resources/trace.h"
#include "resources/textures.h"
#include "resources/parsemsg.h"
#include "resources/keydefs.h"
#include "resources/gl3winit.h"

#include "opengl.h"
#include "usermsg.h"
#include "offset.h"
#include "soundesp.h"
#include "weapons.h"
#include "player.h"
#include "cvar.h"
#include "color.h"
#include "strafe.h"
#include "idhook.h"
#include "autoroute.h"
#include "gui.h"
#include "esp.h"
#include "menu.h"
#include "snapshot.h"
#include "aimbot.h"
#include "norecoil.h"
#include "nospread.h"
#include "studiomodelrender.h"
#include "skins.h"
#include "sound.h"
#include "hitboxes.h"
#include "crosshair.h"
#include "tracegrenade.h"
#include "flash.h"
#include "studio.h"
#include "modechange.h"
#include "guiwindow.h"
#include "antiaim.h"
#include "fakelag.h"
#include "chatinput.h"
#include "killsound.h"
#include "steamid.h"
#include "lightmap.h"
#include "wallskin.h"
#include "sky.h"
#include "spawn.h"
#include "modelhitbox.h"
#include "radar.h"
#include "strings.h"
#include "playerdummy.h"
#include "viewmodel.h"
#include "viewmodelchanger.h"
#include "guiwindowpop.h"
#include "thirdperson.h"
#include "fakeplayer.h"

void HookClientFunctions();
void CL_Move();
void bSendpacket(bool status);
void AdjustSpeed(double speed);

typedef struct cl_clientfuncs_s
{
	int(*Initialize) (cl_enginefunc_t *pEnginefuncs, int iVersion);
	int(*HUD_Init) (void);
	int(*HUD_VidInit) (void);
	void(*HUD_Redraw) (float time, int intermission);
	int(*HUD_UpdateClientData) (client_data_t *pcldata, float flTime);
	int(*HUD_Reset) (void);
	void(*HUD_PlayerMove) (struct playermove_s *ppmove, int server);
	void(*HUD_PlayerMoveInit) (struct playermove_s *ppmove);
	char(*HUD_PlayerMoveTexture) (char *name);
	void(*IN_ActivateMouse) (void);
	void(*IN_DeactivateMouse) (void);
	void(*IN_MouseEvent) (int mstate);
	void(*IN_ClearStates) (void);
	void(*IN_Accumulate) (void);
	void(*CL_CreateMove) (float frametime, struct usercmd_s *cmd, int active);
	int(*CL_IsThirdPerson) (void);
	void(*CL_CameraOffset) (float *ofs);
	struct kbutton_s *(*KB_Find) (const char *name);
	void(*CAM_Think) (void);
	void(*V_CalcRefdef) (struct ref_params_s *pparams);
	int(*HUD_AddEntity) (int type, struct cl_entity_s *ent, const char *modelname);
	void(*HUD_CreateEntities) (void);
	void(*HUD_DrawNormalTriangles) (void);
	void(*HUD_DrawTransparentTriangles) (void);
	void(*HUD_StudioEvent) (const struct mstudioevent_s *event, const struct cl_entity_s *entity);
	void(*HUD_PostRunCmd) (struct local_state_s *from, struct local_state_s *to, struct usercmd_s *cmd, int runfuncs, double time, unsigned int random_seed);
	void(*HUD_Shutdown) (void);
	void(*HUD_TxferLocalOverrides) (struct entity_state_s *state, const struct clientdata_s *client);
	void(*HUD_ProcessPlayerState) (struct entity_state_s *dst, const struct entity_state_s *src);
	void(*HUD_TxferPredictionData) (struct entity_state_s *ps, const struct entity_state_s *pps, struct clientdata_s *pcd, const struct clientdata_s *ppcd, struct weapon_data_s *wd, const struct weapon_data_s *pwd);
	void(*Demo_ReadBuffer) (int size, unsigned char *buffer);
	int(*HUD_ConnectionlessPacket) (struct netadr_s *net_from, const char *args, char *response_buffer, int *response_buffer_size);
	int(*HUD_GetHullBounds) (int hullnumber, float *mins, float *maxs);
	void(*HUD_Frame) (double time);
	int(*HUD_Key_Event) (int down, int keynum, const char *pszCurrentBinding);
	void(*HUD_TempEntUpdate) (double frametime, double client_time, double cl_gravity, struct tempent_s **ppTempEntFree, struct tempent_s **ppTempEntActive, int(*Callback_AddVisibleEntity)(struct cl_entity_s *pEntity), void(*Callback_TempEntPlaySound)(struct tempent_s *pTemp, float damp));
	struct cl_entity_s *(*HUD_GetUserEntity) (int index);
	int(*HUD_VoiceStatus) (int entindex, qboolean bTalking);
	int(*HUD_DirectorMessage) (unsigned char command, unsigned int firstObject, unsigned int secondObject, unsigned int flags);
	int(*HUD_GetStudioModelInterface) (int version, struct r_studio_interface_s **ppinterface, struct engine_studio_api_s *pstudio);
	void(*HUD_CHATINPUTPOSITION_FUNCTION) (int *x, int *y);
	int(*HUD_GETPLAYERTEAM_FUNCTION) (int iplayer);
	void(*CLIENTFACTORY) (void);
} cl_clientfunc_t; 

extern cl_clientfunc_t *g_pClient;
extern cl_enginefunc_t *g_pEngine;
extern engine_studio_api_t *g_pStudio;
extern r_studio_interface_t* g_pInterface;
extern StudioModelRenderer_t* g_pStudioModelRenderer;
extern playermove_t* pmove;
typedef void(*CL_Move_t)();

extern CL_Move_t CL_Move_s;
extern PreS_DynamicSound_t PreS_DynamicSound_s;
extern cl_clientfunc_t g_Client;
extern cl_enginefunc_t g_Engine;
extern engine_studio_api_t g_Studio;
extern r_studio_interface_t g_Interface;
extern StudioModelRenderer_t g_StudioModelRenderer;

extern DWORD HudRedraw;