#ifndef _STRAFE_
#define _STRAFE_

extern bool Strafe;
extern bool Fastrun;
extern bool Gstrafe;
extern bool Bhop;
extern bool Jumpbug;

void Kz(float frametime, struct usercmd_s *cmd);
void DrawKzWindows();
void DrawLongJump();
void KzFameCount();
float HeightOrigin();

#endif