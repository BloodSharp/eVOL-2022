#ifndef _IMGUIMENU_
#define _IMGUIMENU_

extern int MenuTab;
extern float radiusy;
extern bool changewindowfocus;
extern GLuint texture_id[2048];
extern bool keysmenu[256];
extern bool modelmenu;
extern bool bShowMenu;
const char* KeyEventChar(int Key);
void DrawMenuWindow();
bool SliderFloat(const char* label, float* v, float v_min, float v_max, const char* format = "%.1f");

#endif