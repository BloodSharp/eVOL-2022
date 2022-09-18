#include "client.h"

float chams_viewmodel_r;
float chams_viewmodel_g;
float chams_viewmodel_b;
float chams_viewmodel;

float chams_player_r;
float chams_player_g;
float chams_player_b;
float chams_player;

float chams_world_r;
float chams_world_g;
float chams_world_b;
float chams_world;

bool Chams(cl_entity_s* ent, bool valident, float chams, float chamswall, float chams_r, float chams_g, float chams_b, float chamswall_r, float chamswall_g, float chamswall_b, float& coloring, float& coloring_r, float& coloring_g, float& coloring_b)
{
	bool result = false;
	if (valident && chams && DrawVisuals && (!cvar.route_auto || cvar.route_draw_visual) && GetTickCount() - HudRedraw <= 100)
	{
		coloring = true;

		ent->curstate.rendermode = 0;
		ent->curstate.renderfx = 0;
		ent->curstate.renderamt = 0;
		g_Studio.SetForceFaceFlags(0);

		if (chams > 1) glDisable(GL_TEXTURE_2D);
		if (chams > 1) glBindTexture(GL_TEXTURE_2D, 0);

		ent->curstate.rendermode = 0;
		ent->curstate.renderfx = 0;
		ent->curstate.renderamt = 0;
		g_Studio.SetForceFaceFlags(0);

		if (chamswall)
		{
			glDepthFunc(GL_GREATER);
			glDisable(GL_DEPTH_TEST);
			coloring_r = chamswall_r;
			coloring_g = chamswall_g;
			coloring_b = chamswall_b;
			oStudioRenderFinal();
		}

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
		coloring_r = chams_r;
		coloring_g = chams_g;
		coloring_b = chams_b;
		oStudioRenderFinal();

		if (chams > 1) glEnable(GL_TEXTURE_2D);
		result = true;
	}
	coloring = false;
	return result;
}

// This pointer to CStudioModelRenderer class !
StudioModelRenderer_d pThis;

//=========================
// StudioDrawModel
//=========================
int (*pStudioDrawModel)(int flags);
int	StudioDrawModel(int flags)
{
	int studioret = pStudioDrawModel(flags);
	return studioret;
}
//=========================
// StudioDrawPlayer
//=========================
void PlayerWeapon(int flags, entity_state_s* pplayer)
{
	if (flags & STUDIO_RENDER && pplayer->weaponmodel)
	{
		model_t* model = g_Studio.GetModelByIndex(pplayer->weaponmodel);
		if (!model)
			return;
		studiohdr_t* pStudioHeader = (studiohdr_t*)g_Studio.Mod_Extradata(model);
		if (!pStudioHeader)
			return;
		mstudiobbox_t* pHitbox = (mstudiobbox_t*)((byte*)pStudioHeader + pStudioHeader->hitboxindex);
		if (!pHitbox)
			return;
		mstudiobone_t* pbones = (mstudiobone_t*)((byte*)pStudioHeader + pStudioHeader->boneindex);
		if (!pbones)
			return;
		BoneMatrix_t* pBoneMatrix = (BoneMatrix_t*)g_Studio.StudioGetBoneTransform();
		if (!pBoneMatrix)
			return;
		if (!g_Studio.StudioCheckBBox())
			return;

		if (cvar.skeleton_player_weapon_bone)
		{
			for (unsigned int i = 0; i < pStudioHeader->numbones; i++)
			{
				if (pbones[i].parent >= 0)
				{
					playerbone_t Bones;
					Bones.vBone[0] = (*pBoneMatrix)[i][0][3];
					Bones.vBone[1] = (*pBoneMatrix)[i][1][3];
					Bones.vBone[2] = (*pBoneMatrix)[i][2][3];
					Bones.vBoneParent[0] = (*pBoneMatrix)[pbones[i].parent][0][3];
					Bones.vBoneParent[1] = (*pBoneMatrix)[pbones[i].parent][1][3];
					Bones.vBoneParent[2] = (*pBoneMatrix)[pbones[i].parent][2][3];
					Bones.index = pplayer->number;
					Bones.dummy = false;
					PlayerBone.push_back(Bones);
				}
			}
		}
		if (cvar.skeleton_player_weapon_hitbox)
		{
			for (unsigned int i = 0; i < pStudioHeader->numhitboxes; i++)
			{
				Vector vCubePointsTrans[8], vCubePoints[8];

				vCubePoints[0] = Vector(pHitbox[i].bbmin.x, pHitbox[i].bbmin.y, pHitbox[i].bbmin.z);
				vCubePoints[1] = Vector(pHitbox[i].bbmin.x, pHitbox[i].bbmax.y, pHitbox[i].bbmin.z);
				vCubePoints[2] = Vector(pHitbox[i].bbmax.x, pHitbox[i].bbmax.y, pHitbox[i].bbmin.z);
				vCubePoints[3] = Vector(pHitbox[i].bbmax.x, pHitbox[i].bbmin.y, pHitbox[i].bbmin.z);
				vCubePoints[4] = Vector(pHitbox[i].bbmax.x, pHitbox[i].bbmax.y, pHitbox[i].bbmax.z);
				vCubePoints[5] = Vector(pHitbox[i].bbmin.x, pHitbox[i].bbmax.y, pHitbox[i].bbmax.z);
				vCubePoints[6] = Vector(pHitbox[i].bbmin.x, pHitbox[i].bbmin.y, pHitbox[i].bbmax.z);
				vCubePoints[7] = Vector(pHitbox[i].bbmax.x, pHitbox[i].bbmin.y, pHitbox[i].bbmax.z);

				playerhitbox_t Hitboxes;
				Hitboxes.index = pplayer->number;
				Hitboxes.dummy = false;
				for (unsigned int x = 0; x < 8; x++)
				{
					VectorTransform(vCubePoints[x], (*pBoneMatrix)[pHitbox[i].bone], vCubePointsTrans[x]);
					Hitboxes.vCubePointsTrans[x] = vCubePointsTrans[x];
				}
				PlayerHitbox.push_back(Hitboxes);
			}
		}
	}
}

int (*pStudioDrawPlayer)(int flags, entity_state_s* pplayer);
int StudioDrawPlayer(int flags, entity_state_s* pplayer)
{
	int ret = pStudioDrawPlayer(flags, pplayer);
	if (ret && pplayer->number > 0 && pplayer->number <= g_Engine.GetMaxClients())
		PlayerWeapon(flags, pplayer);
	return ret;
}
//=========================
// StudioGetAnim
// Look up animation data for sequence
//=========================
mstudioanim_t StudioGetAnim(model_t* m_pSubModel, mstudioseqdesc_t* pseqdesc)
{
	return oStudioGetAnim(m_pSubModel, pseqdesc);
}

//=========================
// StudioSetUpTransform
// Interpolate model position and angles and set up matrices
//=========================
void StudioSetUpTransform(int trivial_accept)
{
	oStudioSetUpTransform(trivial_accept);
}

//=========================
// StudioSetupBones
// Set up model bone positions
//=========================
void StudioSetupBones(void)
{
	cl_entity_s* ent = g_Studio.GetCurrentEntity();
	if (ent && ent->model && ent == &playerdummy)
	{
		pThis->m_protationmatrix = (float(*)[3][4])g_Studio.StudioGetRotationMatrix();
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
				(*pThis->m_protationmatrix)[i][j] *= modelscale;
		}
	}
	oStudioSetupBones();
}

//=========================
// StudioCalcAttachments
// Find final attachment points
//=========================
void StudioCalcAttachments(void)
{
	oStudioCalcAttachments();
}

//=========================
// StudioSaveBones
// Save bone matrices and names
//=========================
void StudioSaveBones(void)
{
	oStudioSaveBones();
}

//=========================
// StudioMergeBones
// Merge cached bones with current bones for model
//=========================
void StudioMergeBones(model_t* m_pSubModel)
{
	oStudioMergeBones(m_pSubModel);
}

//=========================
// StudioEstimateInterpolant
// Determine interpolation fraction
//=========================
float StudioEstimateInterpolant(void)
{
	return oStudioEstimateInterpolant();
}

//=========================
// StudioEstimateFrame
// Determine current frame for rendering
//=========================
float StudioEstimateFrame(mstudioseqdesc_t* pseqdesc)
{
	return oStudioEstimateFrame(pseqdesc);
}

//=========================
// StudioFxTransform
// Apply special effects to transform matrix
//=========================
void StudioFxTransform(cl_entity_t* ent, float transform[3][4])
{
	oStudioFxTransform(ent, transform);
}

//=========================
// StudioSlerpBones
// Spherical interpolation of bones
//=========================
void StudioSlerpBones(vec4_t q1[], float pos1[][3], vec4_t q2[], float pos2[][3], float s)
{
	oStudioSlerpBones(q1, pos1, q2, pos2, s);
}

//=========================
// StudioCalcBoneAdj
// Compute bone adjustments ( bone controllers )
//=========================
void StudioCalcBoneAdj(float dadt, float* adj, const byte* pcontroller1, const byte* pcontroller2, byte mouthopen)
{
	oStudioCalcBoneAdj(dadt, adj, pcontroller1, pcontroller2, mouthopen);
}

//=========================
// StudioCalcBoneQuaterion
// Get bone quaternions
//=========================
void StudioCalcBoneQuaterion(int frame, float s, mstudiobone_t* pbone, mstudioanim_t* panim, float* adj, float* q)
{
	oStudioCalcBoneQuaterion(frame, s, pbone, panim, adj, q);
}

//=========================
// StudioCalcBonePosition
// Get bone positions
//=========================
void StudioCalcBonePosition(int frame, float s, mstudiobone_t* pbone, mstudioanim_t* panim, float* adj, float* pos)
{
	oStudioCalcBonePosition(frame, s, pbone, panim, adj, pos);
}

//=========================
// StudioCalcRotations
// Compute rotations
//=========================
void StudioCalcRotations(float pos[][3], vec4_t* q, mstudioseqdesc_t* pseqdesc, mstudioanim_t* panim, float f)
{
	oStudioCalcRotations(pos, q, pseqdesc, panim, f);
}

//=========================
// StudioRenderModel
// Send bones and verts to renderer
//=========================

void StudioRenderModel(void)
{
	cl_entity_s* ent = g_Studio.GetCurrentEntity();

	bool DummyPlayer = ent && ent == &playerdummy && strstr(ent->model->name, "/player/");
	bool DummyWorld = ent && ent == &playerdummy && !strstr(ent->model->name, "/player/");
	bool Player = ent && ent->player && (g_Player[ent->index].iTeam != g_Local.iTeam || cvar.visual_visual_team);
	bool ViewModel = ent && ent == g_Engine.GetViewModel();
	bool World = ent && ent->model && !strstr(ent->model->name, "/player.mdl") && !strstr(ent->model->name, "/player/") && !strstr(ent->model->name, "/p_") && ent != g_Engine.GetViewModel();

	if (DummyPlayer || DummyWorld)
	{
		ent->curstate.rendermode = 0;
		ent->curstate.renderfx = 0;
		ent->curstate.renderamt = 0;
		g_Studio.SetForceFaceFlags(0);
		glDisable(GL_DEPTH_TEST);
		oStudioRenderFinal();
		glEnable(GL_DEPTH_TEST);
	}
	float r[2], g[2], b[2];
	r[0] = 1, g[0] = 1, b[0] = 1, r[1] = 1, g[1] = 1, b[1] = 1;
	if (Player)
	{
		if (g_Player[ent->index].iTeam == 1) r[0] = 1, g[0] = 0, b[0] = 0, r[1] = 1, g[1] = 0, b[1] = 1;
		if (g_Player[ent->index].iTeam == 2) r[0] = 0, g[0] = 0, b[0] = 1, r[1] = 0, g[1] = 1, b[1] = 1;
	}
	if (Chams(ent, World, cvar.chams_world, cvar.chams_world_wall, color_blue, color_red, color_green, color_red, color_green, color_blue, chams_world, chams_world_r, chams_world_g, chams_world_b))
		return;
	if (Chams(ent, DummyWorld, cvar.chams_world, cvar.chams_world_wall, color_blue, color_red, color_green, color_red, color_green, color_blue, chams_world, chams_world_r, chams_world_g, chams_world_b))
		return;
	if (Chams(ent, ViewModel, cvar.chams_view_model, 0, color_blue, color_red, color_green, 0, 0, 0, chams_viewmodel, chams_viewmodel_r, chams_viewmodel_g, chams_viewmodel_b))
		return;
	if (Chams(ent, Player, cvar.chams_player, cvar.chams_player_wall, r[0], g[0], b[0], r[1], g[1], b[1], chams_player, chams_player_r, chams_player_g, chams_player_b))
		return;
	if (Chams(ent, DummyPlayer, cvar.chams_player, cvar.chams_player_wall, color_green, color_blue, color_red, color_blue, color_red, color_green, chams_player, chams_player_r, chams_player_g, chams_player_b))
		return;
	oStudioRenderModel();
}

//=========================
// StudioRenderFinal
// Finalize rendering
//=========================
void StudioRenderFinal(void)
{
	oStudioRenderFinal();
}

//=========================
// StudioRenderFinal_Software
// Software renderer finishing function
//=========================

void StudioRenderFinal_Software(void)
{
	oStudioRenderFinal_Software();
}

//=========================
// StudioRenderFinal_Hardware
// GL&D3D renderer finishing function
//=========================

void StudioRenderFinal_Hardware(void)
{
	oStudioRenderFinal_Hardware();
}

//=========================
// StudioPlayerBlend
// Determine pitch and blending amounts for players
//=========================
void StudioPlayerBlend(mstudioseqdesc_t* pseqdesc, int* pBlend, float* pPitch)
{
	oStudioPlayerBlend(pseqdesc, pBlend, pPitch);
}

//=========================
// StudioEstimateGait
// Estimate gait frame for player
//=========================
void StudioEstimateGait(entity_state_t* pplayer)
{
	oStudioEstimateGait(pplayer);
}

//=========================
// StudioProcessGait
// Process movement of player
//=========================
void StudioProcessGait(entity_state_t* pplayer)
{
	oStudioProcessGait(pplayer);
}

//===================================================================================
// Callgates & Patchgates
//===================================================================================
void __declspec(naked) oStudioRenderModel(void)
{
	_asm
	{
		mov ecx, pThis
		jmp g_StudioModelRenderer.StudioRenderModel;
	}
}
void __declspec(naked)StudioRenderModel_Gate(void)
{
	_asm
	{
		mov pThis, ecx
		jmp StudioRenderModel
	}
}

int __declspec(naked) oStudioDrawModel(int flags)
{
	_asm
	{
		mov ecx, pThis
		jmp g_StudioModelRenderer.StudioDrawModel;
	}
}
void __declspec(naked) StudioDrawModel_Gate(void)
{
	_asm
	{
		mov pThis, ecx
		jmp StudioDrawModel
	}
}

int __declspec(naked) oStudioDrawPlayer(int flags, struct entity_state_s* pplayer)
{
	_asm
	{
		mov ecx, pThis
		jmp g_StudioModelRenderer.StudioDrawPlayer;
	}
}
void __declspec(naked) StudioDrawPlayer_Gate(void)
{
	_asm
	{
		mov pThis, ecx
		jmp StudioDrawPlayer
	}
}

mstudioanim_t __declspec(naked) oStudioGetAnim(model_t* m_pSubModel, mstudioseqdesc_t* pseqdesc)
{
	_asm
	{
		mov ecx, pThis
		jmp g_StudioModelRenderer.StudioGetAnim;
	}
}
void __declspec(naked) StudioGetAnim_Gate(void)
{
	_asm
	{
		mov pThis, ecx
		jmp StudioGetAnim
	}
}

void __declspec(naked) oStudioSetUpTransform(int trivial_accept)
{
	_asm
	{
		mov ecx, pThis
		jmp g_StudioModelRenderer.StudioSetUpTransform;
	}
}
void __declspec(naked) StudioSetUpTransform_Gate(void)
{
	_asm
	{
		mov pThis, ecx
		jmp StudioSetUpTransform
	}
}

void __declspec(naked) oStudioSetupBones(void)
{
	_asm
	{
		mov ecx, pThis
		jmp g_StudioModelRenderer.StudioSetupBones;
	}
}
void __declspec(naked) StudioSetupBones_Gate(void)
{
	_asm
	{
		mov pThis, ecx
		jmp StudioSetupBones
	}
}

void __declspec(naked) oStudioCalcAttachments(void)
{
	_asm
	{
		mov ecx, pThis
		jmp g_StudioModelRenderer.StudioCalcAttachments;
	}
}
void __declspec(naked) StudioCalcAttachments_Gate(void)
{
	_asm
	{
		mov pThis, ecx
		jmp StudioCalcAttachments
	}
}

void __declspec(naked) oStudioSaveBones(void)
{
	_asm
	{
		mov ecx, pThis
		jmp g_StudioModelRenderer.StudioSaveBones;
	}
}
void __declspec(naked) StudioSaveBones_Gate(void)
{
	_asm
	{
		mov pThis, ecx
		jmp StudioSaveBones
	}
}

void __declspec(naked) oStudioMergeBones(model_t* m_pSubModel)
{
	_asm
	{
		mov ecx, pThis
		jmp g_StudioModelRenderer.StudioMergeBones;
	}
}
void __declspec(naked) StudioMergeBones_Gate(void)
{
	_asm
	{
		mov pThis, ecx
		jmp StudioMergeBones
	}
}

float __declspec(naked) oStudioEstimateInterpolant(void)
{
	_asm
	{
		mov ecx, pThis
		jmp g_StudioModelRenderer.StudioEstimateInterpolant;
	}
}
void __declspec(naked) StudioEstimateInterpolant_Gate(void)
{
	_asm
	{
		mov pThis, ecx
		jmp StudioEstimateInterpolant
	}
}

float __declspec(naked) oStudioEstimateFrame(mstudioseqdesc_t* pseqdesc)
{
	_asm
	{
		mov ecx, pThis
		jmp g_StudioModelRenderer.StudioEstimateFrame;
	}
}
void __declspec(naked) StudioEstimateFrame_Gate(void)
{
	_asm
	{
		mov pThis, ecx
		jmp StudioEstimateFrame
	}
}

void __declspec(naked) oStudioFxTransform(cl_entity_t* ent, float transform[3][4])
{
	_asm
	{
		mov ecx, pThis
		jmp g_StudioModelRenderer.StudioFxTransform;
	}
}
void __declspec(naked) StudioFxTransforme_Gate(void)
{
	_asm
	{
		mov pThis, ecx
		jmp StudioFxTransform
	}
}

void __declspec(naked) oStudioSlerpBones(vec4_t q1[], float pos1[][3], vec4_t q2[], float pos2[][3], float s)
{
	_asm
	{
		mov ecx, pThis
		jmp g_StudioModelRenderer.StudioSlerpBones;
	}
}
void __declspec(naked) StudioSlerpBones_Gate(void)
{
	_asm
	{
		mov pThis, ecx
		jmp StudioSlerpBones
	}
}

void __declspec(naked) oStudioCalcBoneAdj(float dadt, float* adj, const byte* pcontroller1, const byte* pcontroller2, byte mouthopen)
{
	_asm
	{
		mov ecx, pThis
		jmp g_StudioModelRenderer.StudioCalcBoneAdj;
	}
}
void __declspec(naked) StudioCalcBoneAdj_Gate(void)
{
	_asm
	{
		mov pThis, ecx
		jmp StudioCalcBoneAdj
	}
}

void __declspec(naked) oStudioCalcBoneQuaterion(int frame, float s, mstudiobone_t* pbone, mstudioanim_t* panim, float* adj, float* q)
{
	_asm
	{
		mov ecx, pThis
		jmp g_StudioModelRenderer.StudioCalcBoneQuaterion;
	}
}
void __declspec(naked) StudioCalcBoneQuaterion_Gate(void)
{
	_asm
	{
		mov pThis, ecx
		jmp StudioCalcBoneQuaterion
	}
}

void __declspec(naked) oStudioCalcBonePosition(int frame, float s, mstudiobone_t* pbone, mstudioanim_t* panim, float* adj, float* pos)
{
	_asm
	{
		mov ecx, pThis
		jmp g_StudioModelRenderer.StudioCalcBonePosition;
	}
}
void __declspec(naked) StudioCalcBonePosition_Gate(void)
{
	_asm
	{
		mov pThis, ecx
		jmp StudioCalcBonePosition
	}
}

void __declspec(naked) oStudioCalcRotations(float pos[][3], vec4_t* q, mstudioseqdesc_t* pseqdesc, mstudioanim_t* panim, float f)
{
	_asm
	{
		mov ecx, pThis
		jmp g_StudioModelRenderer.StudioCalcRotations;
	}
}
void __declspec(naked) StudioCalcRotations_Gate(void)
{
	_asm
	{
		mov pThis, ecx
		jmp StudioCalcRotations
	}
}

void __declspec(naked) oStudioRenderFinal(void)
{
	_asm
	{
		mov ecx, pThis
		jmp g_StudioModelRenderer.StudioRenderFinal;
	}
}
void __declspec(naked) StudioRenderFinal_Gate(void)
{
	_asm
	{
		mov pThis, ecx
		jmp StudioRenderFinal
	}
}

void __declspec(naked) oStudioRenderFinal_Software(void)
{
	_asm
	{
		mov ecx, pThis
		jmp g_StudioModelRenderer.StudioRenderFinal_Software;
	}
}
void __declspec(naked) StudioRenderFinal_Software_Gate(void)
{
	_asm
	{
		mov pThis, ecx
		jmp StudioRenderFinal_Software
	}
}

void __declspec(naked) oStudioRenderFinal_Hardware(void)
{
	_asm
	{
		mov ecx, pThis
		jmp g_StudioModelRenderer.StudioRenderFinal_Hardware;
	}
}
void __declspec(naked) StudioRenderFinal_Hardware_Gate(void)
{
	_asm
	{
		mov pThis, ecx
		jmp StudioRenderFinal_Hardware
	}
}

void __declspec(naked) oStudioPlayerBlend(mstudioseqdesc_t* pseqdesc, int* pBlend, float* pPitch)
{
	_asm
	{
		mov ecx, pThis
		jmp g_StudioModelRenderer.StudioPlayerBlend;
	}
}
void __declspec(naked) StudioPlayerBlend_Gate(void)
{
	_asm
	{
		mov pThis, ecx
		jmp StudioPlayerBlend
	}
}

void __declspec(naked) oStudioEstimateGait(entity_state_t* pplayer)
{
	_asm
	{
		mov ecx, pThis
		jmp g_StudioModelRenderer.StudioEstimateGait;
	}
}
void __declspec(naked) StudioEstimateGait_Gate(void)
{
	_asm
	{
		mov pThis, ecx
		jmp StudioEstimateGait
	}
}

void __declspec(naked) oStudioProcessGait(entity_state_t* pplayer)
{
	_asm
	{
		mov ecx, pThis
		jmp g_StudioModelRenderer.StudioProcessGait;
	}
}
void __declspec(naked) StudioProcessGait_Gate(void)
{
	_asm
	{
		mov pThis, ecx
		jmp StudioProcessGait
	}
}

void HookStudiModelRendererFunctions()
{
	c_Offset.EnablePageWrite((DWORD)g_pStudioModelRenderer, sizeof(StudioModelRenderer_t));
	g_pStudioModelRenderer->StudioRenderModel = StudioRenderModel_Gate;
	g_pStudioModelRenderer->StudioSetupBones = StudioSetupBones_Gate;
	g_pStudioModelRenderer->StudioRenderFinal_Hardware = StudioRenderFinal_Hardware_Gate;
	c_Offset.RestorePageProtection((DWORD)g_pStudioModelRenderer, sizeof(StudioModelRenderer_t));
	pStudioDrawPlayer = g_pInterface->StudioDrawPlayer;
	g_pInterface->StudioDrawPlayer = StudioDrawPlayer;
	pStudioDrawModel = g_pInterface->StudioDrawModel;
	g_pInterface->StudioDrawModel = StudioDrawModel;
}
