#include "client.h"

typedef BOOL (APIENTRY* wglSwapBuffers_t)(HDC  hdc);
typedef void (APIENTRY* glClear_t)(GLbitfield mask);
typedef void (APIENTRY* glColor4f_t)(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
typedef void (__stdcall* glReadPixels_t)(GLint, GLint, GLsizei, GLsizei, GLenum, GLenum, GLvoid*); 
typedef void (APIENTRY* glVertex3fv_t)(GLfloat *v);

wglSwapBuffers_t pwglSwapBuffers = NULL;
glClear_t pglClear = NULL;
glColor4f_t pglColor4f = NULL;
glReadPixels_t pglReadPixels = NULL;
glVertex3fv_t pglVertex3fv = NULL;

void APIENTRY Hooked_glVertex3fv(GLfloat* v)
{
	if (drawdummy)
	{
		Vector world, screen(model_pos_x + IM_ROUND(modelscreenw / 2) + dummywidth, model_pos_y + IM_ROUND(modelscreenh / 2) + dummyheight, 0.0f);
		ScreenToWorld(screen, world);

		playerdummy.curstate.angles = vViewanglesFront;
		if (espview == 1) playerdummy.curstate.angles = vViewanglesBack;
		playerdummy.origin = world;
		drawdummy = false;
	}
	pglVertex3fv(v);
}

BOOL APIENTRY Hooked_wglSwapBuffers(HDC hdc)
{
	if(hdc)HookImGui(hdc);
	return pwglSwapBuffers(hdc);
}

void APIENTRY Hooked_glClear(GLbitfield mask)
{
	if (mask == GL_DEPTH_BUFFER_BIT)
		pglClear(GL_COLOR_BUFFER_BIT), glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	pglClear(mask);
}

void __stdcall Hooked_glReadPixels(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLvoid* pixels)
{
	if (ScreenFirst || !cvar.snapshot_memory)
	{
		dwSize = (width * height) * 3;
		BufferScreen = (PBYTE)malloc(dwSize);
		pglReadPixels(x, y, width, height, format, type, pixels);
		memcpy(BufferScreen, pixels, dwSize);
		DrawVisuals = true;
		ScreenFirst = false;
		return;
	}
	memcpy(pixels, BufferScreen, dwSize);
}

void APIENTRY Hooked_glColor4f(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
{
	if (chams_world)
	{
		if (cvar.chams_world == 1 || cvar.chams_world == 3)
			red = chams_world_r, green = chams_world_g, blue = chams_world_b;
		if (cvar.chams_world == 2)
			red = chams_world_r * red, green = chams_world_g * green, blue = chams_world_b * blue;
	}
	if (chams_viewmodel)
	{
		if (cvar.chams_view_model == 1 || cvar.chams_view_model == 3)
			red = chams_viewmodel_r, green = chams_viewmodel_g, blue = chams_viewmodel_b;
		if (cvar.chams_view_model == 2)
			red = chams_viewmodel_r * red, green = chams_viewmodel_g * green, blue = chams_viewmodel_b * blue;
	}
	if (chams_player)
	{
		if (cvar.chams_player == 1 || cvar.chams_player == 3)
			red = chams_player_r, green = chams_player_g, blue = chams_player_b;
		if (cvar.chams_player == 2)
			red = chams_player_r * red, green = chams_player_g * green, blue = chams_player_b * blue;
	}
	pglColor4f(red, green, blue, alpha);
}

void HookOpenGL()
{
	if (g_Studio.IsHardware() != 1)
		c_Offset.Error("Please run game in OpenGL renderer mode");

	const char* szOpenGLModuleName = "opengl32.dll";
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	pwglSwapBuffers = (decltype(pwglSwapBuffers))DetourFindFunction(szOpenGLModuleName, "wglSwapBuffers");
	pglClear = (decltype(pglClear))DetourFindFunction(szOpenGLModuleName, "glClear");
	pglColor4f = (decltype(pglColor4f))DetourFindFunction(szOpenGLModuleName, "glColor4f");
	pglReadPixels = (decltype(pglReadPixels))DetourFindFunction(szOpenGLModuleName, "glReadPixels");
	pglVertex3fv = (decltype(pglVertex3fv))DetourFindFunction(szOpenGLModuleName, "glVertex3fv");
	DetourAttach(&(PVOID&)pwglSwapBuffers, Hooked_wglSwapBuffers);
	DetourAttach(&(PVOID&)pglClear, Hooked_glClear);
	DetourAttach(&(PVOID&)pglColor4f, Hooked_glColor4f);
	DetourAttach(&(PVOID&)pglReadPixels, Hooked_glReadPixels);
	DetourAttach(&(PVOID&)pglVertex3fv, Hooked_glVertex3fv);
	DetourTransactionCommit();
}
