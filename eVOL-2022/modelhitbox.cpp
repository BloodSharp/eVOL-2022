#include "client.h"

bool getmodelhitbox = false;

void SaveHitbox()
{
	char filename[256];
	sprintf(filename, "%s%s", hackdir, "model/model.hitbox");
	remove(filename);
	std::ofstream ofs(filename, std::ios::binary | std::ios::app);
	for (model_aim_select_t Model_Selected : Model_Aim_Select)
	{
		char text[256];
		sprintf(text, "Display: %s Path: %s Hitbox: %d", Model_Selected.displaymodel, Model_Selected.checkmodel, Model_Selected.numhitbox);
		ofs << text << (char)0x0D << (char)0x0A;
	}
	ofs.close();
}

void LoadHitbox()
{
	char filename[256];
	sprintf(filename, "%s%s", hackdir, "model/model.hitbox");
	std::ifstream ifs(filename);
	while (ifs.good())
	{
		char buf[1024];
		ifs.getline(buf, sizeof(buf));
		int hitbox = -1;
		char display[256];
		char path[256];
		if (sscanf(buf, "%*s %s %*s %s %*s %d", &display, &path, &hitbox))
		{
			int len = strlen(display);
			int len2 = strlen(path);
			if (len && len2 && hitbox != -1)
			{
				bool saved = false;
				for (model_aim_select_t Model_Selected : Model_Aim_Select)
				{
					if (!strcmp(Model_Selected.checkmodel, path) && Model_Selected.numhitbox == hitbox)
						saved = true;
				}
				if (!saved)
				{
					model_aim_select_t Model_Select;
					sprintf(Model_Select.displaymodel, display);
					sprintf(Model_Select.checkmodel, path);
					Model_Select.numhitbox = hitbox;
					Model_Aim_Select.push_back(Model_Select);
				}
			}
		}
	}
	ifs.close();
}

void LoadHitboxLegit()
{
	char filename[256];
	sprintf(filename, "%s%s", hackdir, "model/modellegit.hitbox");
	std::ifstream ifs(filename);
	while (ifs.good())
	{
		char buf[1024];
		ifs.getline(buf, sizeof(buf));
		int hitbox = -1;
		int weaponid = -1;
		char display[256];
		char path[256];
		if (sscanf(buf, "%*s %s %*s %s %*s %d %*s %d", &display, &path, &hitbox, &weaponid))
		{
			int len = strlen(display);
			int len2 = strlen(path);
			if (len && len2 && hitbox != -1 && weaponid != -1)
			{
				for (model_aim_select_t Model_Selected : Model_Aim_Select)
				{
					if (strcmp(Model_Selected.checkmodel, path))
						continue;
					if (Model_Selected.numhitbox != hitbox)
						continue;

					bool saved = false;
					for (playeraimlegit_t AimLegit : PlayerAimLegit)
					{
						if (!strcmp(AimLegit.checkmodel, path) && AimLegit.numhitbox == hitbox && AimLegit.m_iWeaponID == weaponid)
							saved = true;
					}
					if (!saved)
					{
						playeraimlegit_t AimLegitsave;
						sprintf(AimLegitsave.displaymodel, display);
						sprintf(AimLegitsave.checkmodel, path);
						AimLegitsave.numhitbox = hitbox;
						AimLegitsave.m_iWeaponID = weaponid;
						PlayerAimLegit.push_back(AimLegitsave);
					}
				}
			}
		}
	}
	ifs.close();
}

void SaveHitboxLegit()
{
	char filename[256];
	sprintf(filename, "%s%s", hackdir, "model/modellegit.hitbox");
	remove(filename);
	std::ofstream ofs(filename, std::ios::binary | std::ios::app);
	for (playeraimlegit_t AimLegit : PlayerAimLegit)
	{
		char text[256];
		sprintf(text, "Display: %s Path: %s Hitbox: %d Weaponid: %d", AimLegit.displaymodel, AimLegit.checkmodel, AimLegit.numhitbox, AimLegit.m_iWeaponID);
		ofs << text << (char)0x0D << (char)0x0A;
	}
	ofs.close();
}

void GetModelAndHitbox()
{
	static bool loadnew = false;

	int checkplayer = FakePlayer.size();
	static int getplayer = checkplayer;
	if (getplayer != checkplayer)
	{
		getplayer = checkplayer;
		loadnew = true;
	}
	bool checkmodel = bShowMenu && DrawVisuals && GetTickCount() - HudRedraw <= 100;
	static bool getmodel = checkmodel;
	if (getmodel != checkmodel)
	{
		getmodel = checkmodel;
		if (getmodel)
			loadnew = true;
	}
	if (MenuTab == 2 && loadnew)
	{
		Model_Aim.clear();
		for (int i = 0; i < 512; i++)
		{
			model_t* mod = g_Studio.GetModelByIndex(i);
			if (!mod)
				continue;
			if (!strstr(mod->name, ".mdl"))
				continue;
			if(!strstr(mod->name, "/player/"))
				continue;
				studiohdr_t* pStudioHeader = (studiohdr_t*)g_Studio.Mod_Extradata(mod);
			if (!pStudioHeader)
				continue;
			mstudiobbox_t* pHitbox = (mstudiobbox_t*)((byte*)pStudioHeader + pStudioHeader->hitboxindex);
			if (!pHitbox)
				return;
			BoneMatrix_t* pBoneMatrix = (BoneMatrix_t*)g_Studio.StudioGetBoneTransform();
			if (!pBoneMatrix)
				return;
			int numhitboxes = 0;
			for (unsigned int i = 0; i < pStudioHeader->numhitboxes; i++)
			{
				Vector vBBMax, vBBMin, vCubePointsTrans[8], vCubePoints[8];
				Vector vEye = pmove->origin + pmove->view_ofs;
				VectorTransform(pHitbox[i].bbmin, (*pBoneMatrix)[pHitbox[i].bone], vBBMin);
				VectorTransform(pHitbox[i].bbmax, (*pBoneMatrix)[pHitbox[i].bone], vBBMax);

				vCubePoints[0] = Vector(pHitbox[i].bbmin.x, pHitbox[i].bbmin.y, pHitbox[i].bbmin.z);
				vCubePoints[1] = Vector(pHitbox[i].bbmin.x, pHitbox[i].bbmax.y, pHitbox[i].bbmin.z);
				vCubePoints[2] = Vector(pHitbox[i].bbmax.x, pHitbox[i].bbmax.y, pHitbox[i].bbmin.z);
				vCubePoints[3] = Vector(pHitbox[i].bbmax.x, pHitbox[i].bbmin.y, pHitbox[i].bbmin.z);
				vCubePoints[4] = Vector(pHitbox[i].bbmax.x, pHitbox[i].bbmax.y, pHitbox[i].bbmax.z);
				vCubePoints[5] = Vector(pHitbox[i].bbmin.x, pHitbox[i].bbmax.y, pHitbox[i].bbmax.z);
				vCubePoints[6] = Vector(pHitbox[i].bbmin.x, pHitbox[i].bbmin.y, pHitbox[i].bbmax.z);
				vCubePoints[7] = Vector(pHitbox[i].bbmax.x, pHitbox[i].bbmin.y, pHitbox[i].bbmax.z);
				for (unsigned int x = 0; x < 8; x++)
					VectorTransform(vCubePoints[x], (*pBoneMatrix)[pHitbox[i].bone], vCubePointsTrans[x]);

				if (!IsSHield(vCubePointsTrans))
					numhitboxes++;
			}
			bool saved = false;
			for (model_aim_t Models : Model_Aim)
			{
				char out[256];
				strcpy(out, getfilename(mod->name).c_str());
				int len = strlen(out);
				if (len > 1)out[len - 1] = (char)0;
				if (!strcmp(Models.checkmodel, mod->name) || !strcmp(Models.displaymodel, out))
				{
					saved = true;
					break;
				}
			}
			if (saved) continue;

			model_aim_t Model;
			Model.numhitboxes = numhitboxes;
			strcpy(Model.displaymodel, getfilename(mod->name).c_str());
			strcpy(Model.checkmodel, mod->name);
			Model_Aim.push_back(Model);
		}
		if (FakePlayer.size())
		{
			for (int i = 0; i < 512; i++)
			{
				model_t* mod = g_Studio.GetModelByIndex(i);
				if (!mod)
					continue;
				if (!strstr(mod->name, ".mdl"))
					continue;

				bool isfaker = false;
				for (fakep_t Model : FakePlayer)
				{
					if (!strcmp(Model.name, mod->name))
					{
						isfaker = true;
						break;
					}
				}
				if (!isfaker)
					continue;

				studiohdr_t* pStudioHeader = (studiohdr_t*)g_Studio.Mod_Extradata(mod);
				if (!pStudioHeader)
					continue;
				mstudiobbox_t* pHitbox = (mstudiobbox_t*)((byte*)pStudioHeader + pStudioHeader->hitboxindex);
				if (!pHitbox)
					return;
				BoneMatrix_t* pBoneMatrix = (BoneMatrix_t*)g_Studio.StudioGetBoneTransform();
				if (!pBoneMatrix)
					return;
				int numhitboxes = 0;
				for (unsigned int i = 0; i < pStudioHeader->numhitboxes; i++)
				{
					Vector vBBMax, vBBMin, vCubePointsTrans[8], vCubePoints[8];
					Vector vEye = pmove->origin + pmove->view_ofs;
					VectorTransform(pHitbox[i].bbmin, (*pBoneMatrix)[pHitbox[i].bone], vBBMin);
					VectorTransform(pHitbox[i].bbmax, (*pBoneMatrix)[pHitbox[i].bone], vBBMax);

					vCubePoints[0] = Vector(pHitbox[i].bbmin.x, pHitbox[i].bbmin.y, pHitbox[i].bbmin.z);
					vCubePoints[1] = Vector(pHitbox[i].bbmin.x, pHitbox[i].bbmax.y, pHitbox[i].bbmin.z);
					vCubePoints[2] = Vector(pHitbox[i].bbmax.x, pHitbox[i].bbmax.y, pHitbox[i].bbmin.z);
					vCubePoints[3] = Vector(pHitbox[i].bbmax.x, pHitbox[i].bbmin.y, pHitbox[i].bbmin.z);
					vCubePoints[4] = Vector(pHitbox[i].bbmax.x, pHitbox[i].bbmax.y, pHitbox[i].bbmax.z);
					vCubePoints[5] = Vector(pHitbox[i].bbmin.x, pHitbox[i].bbmax.y, pHitbox[i].bbmax.z);
					vCubePoints[6] = Vector(pHitbox[i].bbmin.x, pHitbox[i].bbmin.y, pHitbox[i].bbmax.z);
					vCubePoints[7] = Vector(pHitbox[i].bbmax.x, pHitbox[i].bbmin.y, pHitbox[i].bbmax.z);
					for (unsigned int x = 0; x < 8; x++)
						VectorTransform(vCubePoints[x], (*pBoneMatrix)[pHitbox[i].bone], vCubePointsTrans[x]);

					if (!IsSHield(vCubePointsTrans))
						numhitboxes++;
				}
				bool saved = false;
				for (model_aim_t Models : Model_Aim)
				{
					char out[256];
					strcpy(out, getfilename(mod->name).c_str());
					int len = strlen(out);
					if (len > 1)out[len - 1] = (char)0;
					if (!strcmp(Models.checkmodel, mod->name) || !strcmp(Models.displaymodel, out))
					{
						saved = true;
						break;
					}
				}
				if (saved) continue;

				model_aim_t Model;
				Model.numhitboxes = numhitboxes;
				strcpy(Model.displaymodel, getfilename(mod->name).c_str());
				strcpy(Model.checkmodel, mod->name);
				Model_Aim.push_back(Model);
			}
		}

		loadnew = false;
	}
}