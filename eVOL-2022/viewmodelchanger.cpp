#include "client.h"

std::deque<viewmodelchanger_t> ViewModelChanger;
std::deque<viewmodelchanged_t> ViewModelChanged;

char viewmodelname[MAX_MODEL_NAME];
bool viewmodelactive = false;

void file_search_rec(const char* folder)
{
	char path[MAX_PATH];

	strcpy(path, folder);
	strcat(path, "/*");

	//LogToFile(folder);
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind = ::FindFirstFile(path, &FindFileData);

	if (hFind != INVALID_HANDLE_VALUE)
	{
		do
		{
			if (!(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				if (strstr(FindFileData.cFileName, "v_") && strstr(FindFileData.cFileName, ".mdl"))
				{
					char subpath[MAX_PATH];
					strcpy(subpath, folder);
					strcat(subpath, FindFileData.cFileName);
					viewmodelchanger_t view;
					strcpy(view.modelname, strip(subpath).c_str());
					ViewModelChanger.push_back(view);
					//LogToFile(strip(subpath).c_str());
				}
			}
			else
			{
				if (strcmp(FindFileData.cFileName, ".") && strcmp(FindFileData.cFileName, ".."))
				{
					char subpath[MAX_PATH];
					strcpy(subpath, folder);
					strcat(subpath, FindFileData.cFileName);
					strcat(subpath, "/");
					file_search_rec(subpath);
				}
			}
		} while (::FindNextFile(hFind, &FindFileData));
		::FindClose(hFind);
	}
}

void Viewmodelscan()
{
	static bool doscan = true;
	if(doscan)
		file_search_rec(hldir);
	doscan = false;
}

void ViewModels()
{
	if (DrawVisuals && (!cvar.route_auto || cvar.route_draw_visual) && GetTickCount() - HudRedraw <= 100)
	{
		cl_entity_s* vm = g_Engine.GetViewModel();
		if (vm && vm->model)
		{
			strcpy(viewmodelname, vm->model->name);
			viewmodelactive = true;
			for (viewmodelchanged_t View : ViewModelChanged)
			{
				if (!strcmp(vm->model->name, View.modelnameold))
				{
					model_s* model = g_Studio.Mod_ForName(View.modelnamenew, false);
					if (model)vm->model = model;
					break;
				}
			}
		}
		else viewmodelactive = false;
	}
}

void SaveViewChanger()
{
	char filename[256];
	sprintf(filename, "%s%s", hackdir, "viewmodel/viewmodel.changer");
	remove(filename);
	std::ofstream ofs(filename, std::ios::binary | std::ios::app);
	for (viewmodelchanged_t View : ViewModelChanged)
	{
		char text[256];
		sprintf(text, "New: %s Old: %s", View.modelnamenew, View.modelnameold);
		ofs << text << (char)0x0D << (char)0x0A;
	}
	ofs.close();
}

void LoadViewChanger()
{
	char filename[256];
	sprintf(filename, "%s%s", hackdir, "viewmodel/viewmodel.changer");
	std::ifstream ifs(filename);
	while (ifs.good())
	{
		char buf[1024];
		ifs.getline(buf, sizeof(buf));
		char newmodel[256];
		char oldmodel[256];
		if (sscanf(buf, "%*s %s %*s %s", &newmodel, &oldmodel))
		{
			int len = strlen(newmodel);
			int len2 = strlen(oldmodel);
			if (len && len2)
			{
				int havemodel = 0;
				for (viewmodelchanger_t View : ViewModelChanger)
				{
					if (!strcmp(View.modelname, newmodel) || !strcmp(View.modelname, oldmodel))
						havemodel++;
				}
				if (havemodel == 2)
				{
					bool saved = false;
					for (viewmodelchanged_t View : ViewModelChanged)
					{
						if (!strcmp(newmodel, View.modelnamenew) || !strcmp(oldmodel, View.modelnameold))
						{
							saved = true;
							break;
						}

					}
					if (!saved)
					{
						viewmodelchanged_t View;
						strcpy(View.modelnameold, oldmodel);
						strcpy(View.modelnamenew, newmodel);
						ViewModelChanged.push_back(View);
					}
				}
			}
		}
	}
	ifs.close();
}