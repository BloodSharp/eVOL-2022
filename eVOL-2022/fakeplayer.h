#ifndef _FAKEP_
#define _FAKEP_

typedef struct
{
	char name[MAX_QPATH];
} fakep_t;
extern std::deque<fakep_t> FakePlayer;
void SaveHitboxFake();
void LoadHitboxFake();
extern bool hasowner;

#endif