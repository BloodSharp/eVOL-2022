#ifndef _VIEWMODELCHANGER_
#define _VIEWMODELCHANGER_

typedef struct
{
	char modelnamenew[MAX_MODEL_NAME];
	char modelnameold[MAX_MODEL_NAME];
} viewmodelchanged_t;
extern std::deque<viewmodelchanged_t> ViewModelChanged;

typedef struct
{
	char modelname[MAX_MODEL_NAME];
} viewmodelchanger_t;
extern std::deque<viewmodelchanger_t> ViewModelChanger;

extern char viewmodelname[MAX_MODEL_NAME];
extern bool viewmodelactive;
void Viewmodelscan();
void ViewModels();
void SaveViewChanger();
void LoadViewChanger();

#endif