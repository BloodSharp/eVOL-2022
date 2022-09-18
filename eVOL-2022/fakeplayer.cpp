#include "client.h"

std::deque<fakep_t> FakePlayer;

bool hasowner = false;

void SaveHitboxFake()
{
	char filename[256];
	sprintf(filename, "%s%s", hackdir, "fakeplayer/name.model");
	remove(filename);
	std::ofstream ofs(filename, std::ios::binary | std::ios::app);
	for (fakep_t Model_Selected : FakePlayer)
	{
		char text[256];
		sprintf(text, "Name: %s", Model_Selected.name);
		ofs << text << (char)0x0D << (char)0x0A;
	}
	ofs.close();
}

void LoadHitboxFake()
{
	char filename[256];
	sprintf(filename, "%s%s", hackdir, "fakeplayer/name.model");
	std::ifstream ifs(filename);
	while (ifs.good())
	{
		char buf[1024];
		ifs.getline(buf, sizeof(buf));
		char name[256];
		if (sscanf(buf, "%*s %s", &name))
		{
			int len = strlen(name);
			if (len)
			{
				bool saved = false;
				for (fakep_t Model_Selected : FakePlayer)
				{
					if (!strcmp(Model_Selected.name, name))
						saved = true;
				}
				if (!saved)
				{
					fakep_t Model_Select;
					sprintf(Model_Select.name, name);
					FakePlayer.push_back(Model_Select);
				}
			}
		}
	}
	ifs.close();
}