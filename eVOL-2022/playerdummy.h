#ifndef _PLAYERDUMMY_
#define _PLAYERDUMMY_

extern cl_entity_s playerdummy;

typedef struct
{
	char name[MAX_MODEL_NAME];
	bool player;
} playermodel_t;

extern std::deque<playermodel_t> PlayerModel;

extern float vViewanglesFront[3];
extern float vViewanglesBack[3];
extern float modelscreenw, modelscreenh;
extern int model_type;
extern float espxo, espyo;
extern float model_pos_x, model_pos_y;
extern float modelscale;
extern bool drawdummy;
extern bool drawdummy2;
extern bool drawgetdummy;
extern bool drawbackdummy;
extern float dummyheight;
extern float dummywidth;
extern int espview;

void GetDummyAngleFront(ref_params_s* pparams);
void GetDummyAngleBack(ref_params_s* pparams);
void GetDummyModels(); 
void DrawDummyBack();
void Playerdummy();

#endif