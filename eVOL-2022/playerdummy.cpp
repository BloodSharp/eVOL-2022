#include "client.h"

cl_entity_s playerdummy;

std::deque<playermodel_t> PlayerModel;

float vViewanglesFront[3];
float vViewanglesBack[3];
float modelscreenw = 150, modelscreenh = 335;
int model_type;
float espxo, espyo;
float model_pos_x = 100, model_pos_y = 100;
float modelscale = 0.01f;
bool drawdummy = false;
bool drawdummy2 = false;
bool drawgetdummy = false;
bool drawbackdummy = false;
float dummyheight;
float dummywidth;
int espview;

void GetDummyAngleFront(ref_params_s* pparams)
{
	vViewanglesFront[0] = pparams->viewangles[0];
	vViewanglesFront[1] = pparams->viewangles[1] + 180;
	vViewanglesFront[2] = -pparams->viewangles[2];
}

void GetDummyAngleBack(ref_params_s* pparams)
{
	vViewanglesBack[0] = -pparams->viewangles[0];
	vViewanglesBack[1] = pparams->viewangles[1];
	vViewanglesBack[2] = pparams->viewangles[2];
}

void DrawDummyBack()
{
	if (drawbackdummy)
	{
		Vector world1, screen1(model_pos_x, model_pos_y, 0.0f);
		Vector world2, screen2(model_pos_x + modelscreenw, model_pos_y, 0.0f);
		Vector world3, screen3(model_pos_x + modelscreenw, model_pos_y + modelscreenh, 0.0f);
		Vector world4, screen4(model_pos_x, model_pos_y + modelscreenh, 0.0f);
		ScreenToWorld(screen1, world1);
		ScreenToWorld(screen2, world2);
		ScreenToWorld(screen3, world3);
		ScreenToWorld(screen4, world4);
		glDisable(GL_TEXTURE_2D);
		glEnable(GL_BLEND);
		glEnable(GL_LINE_SMOOTH);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glColor3f(0.06f, 0.06f, 0.06f);
		glBegin(GL_QUADS);
		glVertex3fv(world1);
		glVertex3fv(world2);
		glVertex3fv(world3);
		glVertex3fv(world4);
		glEnd();
		glEnable(GL_TEXTURE_2D);
		glDisable(GL_BLEND);
		glDisable(GL_LINE_SMOOTH);
		drawbackdummy = false;
	}
}

void GetDummyModels()
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
	if (drawgetdummy && loadnew)
	{
		PlayerModel.clear();
		for (int i = 0; i < 512; i++)
		{
			model_t* mod = g_Studio.GetModelByIndex(i);
			if (!mod)
				continue;
			if (!strstr(mod->name, ".mdl"))
				continue;
			if (!strstr(mod->name, "/player/"))
				continue;
			studiohdr_t* pStudioHeader = (studiohdr_t*)g_Studio.Mod_Extradata(mod);
			if (!pStudioHeader)
				continue;
			if (pStudioHeader->numhitboxes < 1)
				continue;

			bool saved = false;
			for (playermodel_t Model : PlayerModel)
			{
				char out[256];
				strcpy(out, getfilename(mod->name).c_str());
				int len = strlen(out);
				if (len > 1)out[len - 1] = (char)0;
				if (!strcmp(Model.name, mod->name) || !strcmp(getfilename(Model.name).c_str(), out))
				{
					saved = true;
					break;
				}
			}
			if (saved) continue;

			playermodel_t model;
			strcpy(model.name,mod->name);
			model.player = true;
			PlayerModel.push_back(model);
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
				studiohdr_t* pStudioHeader = (studiohdr_t*)g_Studio.Mod_Extradata(mod);
				if (!pStudioHeader)
					continue;
				if (pStudioHeader->numhitboxes < 1)
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

				bool saved = false;
				for (playermodel_t Model : PlayerModel)
				{
					char out[256];
					strcpy(out, getfilename(mod->name).c_str());
					int len = strlen(out);
					if (len > 1)out[len - 1] = (char)0;
					if (!strcmp(Model.name, mod->name) || !strcmp(getfilename(Model.name).c_str(), out))
					{
						saved = true;
						break;
					}
				}
				if (saved) continue;

				playermodel_t model;
				strcpy(model.name, mod->name);
				model.player = true;
				PlayerModel.push_back(model);
			}
		}
		for (int i = 0; i < 512; i++)
		{
			model_t* mod = g_Studio.GetModelByIndex(i);
			if (!mod)
				continue;
			if (!strstr(mod->name, ".mdl"))
				continue;
			if (strstr(mod->name, "/player/"))
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
			if (isfaker)
				continue;

			if (strstr(mod->name, "/molotov_fire_floor.mdl") || strstr(mod->name, "/w_grenade1.mdl"))
				continue;
			studiohdr_t* pStudioHeader = (studiohdr_t*)g_Studio.Mod_Extradata(mod);
			if (!pStudioHeader)
				continue;
			if (pStudioHeader->numhitboxes < 1)
				continue;

			bool saved = false;
			for (playermodel_t Model : PlayerModel)
			{
				char out[256];
				strcpy(out, getfilename(mod->name).c_str());
				int len = strlen(out);
				if (len > 1)out[len - 1] = (char)0;
				if (!strcmp(Model.name, mod->name) || !strcmp(getfilename(Model.name).c_str(), out))
				{
					saved = true;
					break;
				}
			}
			if (saved) continue;

			playermodel_t model;
			strcpy(model.name, mod->name);
			model.player = false;
			PlayerModel.push_back(model);
		}
		drawgetdummy = false;
		loadnew = false;
	}
}

void Playerdummy()
{
	if (drawdummy2)
	{
		if (PlayerModel.size() && model_type > PlayerModel.size() - 1)
			model_type = PlayerModel.size() - 1;
		if (PlayerModel.size() == 0)
			model_type = 0;

		for (int i = 0; i < PlayerModel.size(); i++)
		{
			struct model_s* mod = g_Studio.Mod_ForName(PlayerModel[i].name, false);
			if (!mod)
			{
				if (model_type > i)
					model_type--;
				PlayerModel.erase(PlayerModel.begin() + i);
				continue;
			}

			if (i != model_type)
				continue;
			if (hasowner)
			{
				bool saved = false;
				for (fakep_t Model : FakePlayer)
				{
					if (!strcmp(Model.name, mod->name))
					{
						saved = true;
						break;
					}
				}
				if (!saved)
				{
					fakep_t Faker;
					strcpy(Faker.name, mod->name);
					FakePlayer.push_back(Faker);
				}
				hasowner = false;
			}
			static int modelindex = i;
			if (modelindex != i)
			{
				dummyheight = 0;
				dummywidth = 0;
				modelscale = 0.01f;
				modelindex = i;
			}
			static int viewer = espview;
			if (viewer != espview)
			{
				dummyheight = 0;
				dummywidth = 0;
				modelscale = 0.01f;
				viewer = espview;
			}
			playerdummy.model = mod;
			playerdummy.curstate.sequence = 0;
			playerdummy.curstate.entityType = 0;
			if (PlayerModel[i].player)
			{
				playerdummy.curstate.sequence = 1;
				playerdummy.curstate.entityType = 1;
			}
			g_Engine.CL_CreateVisibleEntity(ET_PLAYER, &playerdummy);
		}
		drawdummy2 = false;
	}
}