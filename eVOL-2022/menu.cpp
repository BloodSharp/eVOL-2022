#include "client.h"

GLuint texture_id[2048];
bool bShowMenu = false;
bool keysmenu[256];
int MenuTab;
bool loadtexturemenu = true;
bool modelmenu;
bool hidemenu;

float EasyIn(float time, float start, float change, float duration)
{
	time /= duration;
	return -change * (sqrt(1 - time * time) - 1) + start;
}

float EasyOut(float time, float start, float change, float duration)
{
	time /= duration;
	time--;
	return change * sqrt(1 - time * time) + start;
}

void EaseMenu(float& menuspeed, float windowsize, int speed, bool showmenu, float& changein, float& changeout, int& mytick, DWORD& Tickcount)
{
	if (GetTickCount() - Tickcount > 0)
	{
		if (!showmenu)
		{
			if (menuspeed < windowsize && EasyOut(mytick, changein, windowsize, speed) < windowsize)
				menuspeed = EasyOut(mytick, changein, windowsize, speed);
			else
				menuspeed = windowsize;

			changeout = menuspeed;
		}
		if (showmenu)
		{
			if (menuspeed > 0 && windowsize - EasyIn(mytick, windowsize - changeout, windowsize, speed / 2) > 0)
				menuspeed = windowsize - EasyIn(mytick, windowsize - changeout, windowsize, speed / 2);
			else
				menuspeed = 0;

			changein = menuspeed;
		}
		mytick++;
		Tickcount = GetTickCount();
	}
}

static bool Items_ArrayGetter(void* data, int idx, const char** out_text)
{
	const char* const* items = (const char* const*)data;
	if (out_text)
		*out_text = items[idx];
	return true;
}

bool ComboBox(const char* label, float* current_item, const char* const items[], int items_count, int height_in_items) 
{
	ImGui::PushItemWidth(-1);
	char str[256];
	sprintf(str, "##%s", label);
	const bool value_changed = ImGui::Combo(str, current_item, Items_ArrayGetter, (void*)items, items_count, height_in_items);
	ImGui::PopItemWidth();
	return value_changed;
}

bool SliderFloat(const char* label, float* v, float v_min, float v_max, const char* format)
{
	ImGui::PushItemWidth(-1);
	char str[256];
	sprintf(str, "##%s", label);
	const bool value_changed = ImGui::SliderFloat(str, v, v_min, v_max, format);
	ImGui::PopItemWidth();
	return value_changed;
}

const char* KeyEventChar(int Key)
{
	switch (Key)
	{
	case K_TAB: return "Tab";
	case K_ENTER: return "Enter";
	case K_ESCAPE: return "Escape";
	case K_SPACE: return "Space";
	case K_0: return "0";
	case K_1: return "1";
	case K_2: return "2";
	case K_3: return "3";
	case K_4: return "4";
	case K_5: return "5";
	case K_6: return "6";
	case K_7: return "7";
	case K_8: return "8";
	case K_9: return "9";
	case K_A: return "A";
	case K_B: return "B";
	case K_C: return "C";
	case K_D: return "D";
	case K_E: return "E";
	case K_F: return "F";
	case K_G: return "G";
	case K_H: return "H";
	case K_I: return "I";
	case K_J: return "J";
	case K_K: return "K";
	case K_L: return "L";
	case K_M: return "M";
	case K_N: return "N";
	case K_O: return "O";
	case K_P: return "P";
	case K_Q: return "Q";
	case K_R: return "R";
	case K_S: return "S";
	case K_T: return "T";
	case K_U: return "U";
	case K_V: return "V";
	case K_W: return "W";
	case K_X: return "X";
	case K_Y: return "Y";
	case K_Z: return "Z";
	case K_BACKSPACE: return "Backspace";
	case K_UPARROW: return "Up Arrow";
	case K_DOWNARROW: return "Down Arrow";
	case K_LEFTARROW: return "Left Arrow";
	case K_RIGHTARROW: return "Right Arrow";
	case K_ALT: return "Alt";
	case K_CTRL: return "Ctrl";
	case K_SHIFT: return "Shift";
	case K_F1: return "F1";
	case K_F2: return "F2";
	case K_F3: return "F3";
	case K_F4: return "F4";
	case K_F5: return "F5";
	case K_F6: return "F6";
	case K_F7: return "F7";
	case K_F8: return "F8";
	case K_F9: return "F9";
	case K_F10: return "F10";
	case K_F11: return "F11";
	case K_F12: return "F12";
	case K_INS: return "Insert";
	case K_DEL: return "Delete";
	case K_PGDN: return "Page Down";
	case K_PGUP: return "Page Up";
	case K_HOME: return "Home";
	case K_END: return "End";
	case K_CAPSLOCK: return "Capslock";
	case K_MWHEELDOWN: return "Wheel Down";
	case K_MWHEELUP: return "Wheel Up";
	case K_MOUSE1: return "Mouse 1";
	case K_MOUSE2: return "Mouse 2";
	case K_MOUSE3: return "Mouse 3";
	case K_MOUSE4: return "Mouse 4";
	case K_MOUSE5: return "Mouse 5";
	}
	return "Unknown Ok";
}

void HudKeyBind(float& key, const char* keyname)
{
	bool clicked = false;
	char str[256];

	if (key == -2)
	{
		for (unsigned int i = 0; i < 255; i++)
		{
			if (keysmenu[i])
			{
				if (i == K_ESCAPE || i == cvar.gui_key)
				{
					key = -1;
					break;
				}
				key = i;
			}
		}
	}
	
	if (key == -1)
	{
		ImGui::Text(keyname);
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
		sprintf(str, "[No key]##%s", keyname);

		if (ImGui::Button(str, ImVec2(ImGui::GetWindowSize().x - 6, 0)))
			clicked = true;
		ImGui::PopStyleColor();
	}
	else if (key == -2)
	{
		ImGui::Text(keyname);
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 0.0f, 1.0f));
		sprintf(str, "[Press key]##%s", keyname);
		if (ImGui::Button(str, ImVec2(ImGui::GetWindowSize().x - 6, 0)))
			clicked = true;
		ImGui::PopStyleColor();
	}
	else
	{
		ImGui::Text(keyname);
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 1.0f, 0.0f, 1.0f));
		sprintf(str, "[%s]##%s", KeyEventChar(key), keyname);
		if (ImGui::Button(str, ImVec2(ImGui::GetWindowSize().x - 6, 0)))
			clicked = true;
		ImGui::PopStyleColor();
	}

	if (clicked)
	{
		if (key == -1)
			key = -2;
		else
			key = -1;
	}
}

void HudMenuBind(float& key)
{
	bool clicked = false;
	static float restorekey;
	if (key != -1)
		restorekey = key;
	if (key == -1)
	{
		for (unsigned int i = 0; i < 255; i++)
		{
			if (keysmenu[i])
			{
				if (i == K_MOUSE1 || i == K_ESCAPE) break;
				key = i;
			}
		}
	}
	
	if (key == -1)
	{
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 0.0f, 1.0f));
		if(ImGui::Button("[Press key]"))
			clicked = true;
		ImGui::PopStyleColor();
	}
	else
	{
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 1.0f, 0.0f, 1.0f));
		char str[256];
		sprintf(str, "[%s]", KeyEventChar(key));
		if (ImGui::Button(str))
			clicked = true;
		ImGui::PopStyleColor();
	}

	if (clicked)
	{
		if (key == -1)
			key = restorekey;
		else
			key = -1;
	}
}

int CheckWeapon(float global, float sub)
{
	if (global == 0 && (sub == 0 || sub == 1 || sub == 2 || sub == 3 || sub == 4 || sub == 5))
	{
		if (sub == 0) return WEAPON_GLOCK18;
		if (sub == 1) return WEAPON_P228;
		if (sub == 2) return WEAPON_DEAGLE;
		if (sub == 3) return WEAPON_ELITE;
		if (sub == 4) return WEAPON_FIVESEVEN;
		if (sub == 5) return WEAPON_USP;
	}
	else if (global == 1 && (sub == 0 || sub == 1 || sub == 2 || sub == 3))
	{
		if (sub == 0) return WEAPON_AWP;
		if (sub == 1) return WEAPON_SCOUT;
		if (sub == 2) return WEAPON_G3SG1;
		if (sub == 3) return WEAPON_SG550;
	}
	else if (global == 2 && (sub == 0 || sub == 1 || sub == 2 || sub == 3 || sub == 4 || sub == 5))
	{
		if (sub == 0) return WEAPON_M4A1;
		if (sub == 1) return WEAPON_GALIL;
		if (sub == 2) return WEAPON_FAMAS;
		if (sub == 3) return WEAPON_AUG;
		if (sub == 4) return WEAPON_AK47;
		if (sub == 5) return WEAPON_SG552;
	}
	else if (global == 3 && (sub == 0 || sub == 1))
	{
		if (sub == 0) return WEAPON_XM1014;
		if (sub == 1) return WEAPON_M3;
	}
	else if (global == 4 && sub == 0)
	{
		if (sub == 0) return WEAPON_M249;
	}
	else if (global == 5 && (sub == 0 || sub == 1 || sub == 2 || sub == 3 || sub == 4))
	{
		if (sub == 0) return WEAPON_TMP;
		if (sub == 1) return WEAPON_P90;
		if (sub == 2) return WEAPON_MP5N;
		if (sub == 3) return WEAPON_MAC10;
		if (sub == 4) return WEAPON_UMP45;
	}
	else return 0;
}

void CheckSubSection(float& global, float& sub)
{
	if (global == 0 && sub > 5) sub = 5;
	if (global == 1 && sub > 3) sub = 3;
	if (global == 2 && sub > 5) sub = 5;
	if (global == 3 && sub > 1) sub = 1;
	if (global == 4 && sub > 0) sub = 0;
	if (global == 5 && sub > 4) sub = 4;
}

void MenuRage1()
{
	ImGui::Text("Rage Aimbot"), ImGui::Separator();

	ImGui::Checkbox("Activate", &cvar.rage_active);
	ImGui::Checkbox("Aim Team", &cvar.rage_team);
	ImGui::Checkbox("Always Fire Manual", &cvar.rage_always_fire);
	if(!cvar.rage_always_fire)ImGui::Checkbox("Auto Fire", &cvar.rage_auto_fire);
	ImGui::Checkbox("Bypass Trace", &cvar.bypass_trace_rage);
	ImGui::Checkbox("Draw Aim", &cvar.rage_draw_aim);
	if (!cvar.rage_always_fire)ImGui::Checkbox("Perfect Silent", &cvar.rage_perfect_silent);
	ImGui::Checkbox("Silent Aim", &cvar.rage_silent);
	ImGui::Checkbox("Shield Attack", &cvar.rage_shield_attack);
	ImGui::Checkbox("Wall", &cvar.rage_wall);
	if (!cvar.rage_always_fire)HudKeyBind(cvar.rage_auto_fire_key, "Auto Fire Key");
	ImGui::Text("Field Of View");
	SliderFloat("Field Of View##1", &cvar.rage_fov, 0.f, 180.f, "%.1f");
	if (!cvar.rage_always_fire)
	{
		ImGui::Text("Fire Delay");
		SliderFloat("Fire Delay##1", &cvar.rage[g_Local.weapon.m_iWeaponID].rage_delay_shot, 0.f, 1000.f, "%.0fms");
		if (!cvar.rage[g_Local.weapon.m_iWeaponID].rage_shot_type)
		{
			ImGui::Text("Count");
			SliderFloat("Count##1", &cvar.rage[g_Local.weapon.m_iWeaponID].rage_shot_count, 1.f, 5.f, "%.0f");
		}
		if (cvar.rage[g_Local.weapon.m_iWeaponID].rage_shot_type)
		{
			ImGui::Text("Random Max");
			SliderFloat("Random Max##1", &cvar.rage[g_Local.weapon.m_iWeaponID].rage_random_max, 2.f, 5.f, "%.0f");
		}
		ImGui::Text("Fire Type");
		const char* listbox_count[] = { "Count", "Random" };
		ComboBox("Fire Type##1", &cvar.rage[g_Local.weapon.m_iWeaponID].rage_shot_type, listbox_count, IM_ARRAYSIZE(listbox_count), 2);
	}
	ImGui::Text("Knife Attack Method");
	const char* listbox_knife[] = { "Slash", "Stab" };
	ComboBox("Knife Attack Method##1", &cvar.rage_knife_attack, listbox_knife, IM_ARRAYSIZE(listbox_knife), 2);
	ImGui::Text("Target Selection");
	const char* listbox_target[] = { "Field Of View", "Distance", "Cycle" };
	ComboBox("Target Selection##1", &cvar.rage_target_selection, listbox_target, IM_ARRAYSIZE(listbox_target), 3);
	ImGui::Text("Spread Method");
	const char* listbox_spread[] = { "Pitch / Yaw", "Roll" };
	ComboBox("Spread Method##1", &cvar.rage_nospread_method, listbox_spread, IM_ARRAYSIZE(listbox_spread), 2);
}

void MenuRage2()
{
	ImGui::Text("Anti Aim"), ImGui::Separator();

	ImGui::Text("Pitch");
	const char* listbox_pitch[] = { "None", "Fakedown", "Random" };
	ComboBox("Pitch##1", &cvar.aa_pitch, listbox_pitch, IM_ARRAYSIZE(listbox_pitch), 3);
	ImGui::Text("Static Yaw");
	SliderFloat("Static Yaw##1", &cvar.aa_yaw_static, -180.f, 180.f, "%.0f");
	ImGui::Text("Yaw");
	const char* listbox_yaw[] = { "None", "180", "180 Jitter", "Spin", "Jitter", "Sideways", "Random", "Static" };
	ComboBox("Yaw##1", &cvar.aa_yaw, listbox_yaw, IM_ARRAYSIZE(listbox_yaw), 8);
	ImGui::Text("Yaw While Running");
	const char* listbox_yawrun[] = { "None", "180", "180 Jitter", "Spin", "Jitter", "Sideways", "Random", "Static" };
	ComboBox("Yaw While Running##1", &cvar.aa_yaw_while_running, listbox_yawrun, IM_ARRAYSIZE(listbox_yawrun), 8);
	ImGui::Text("Edge");
	const char* listbox_edge[] = { "None", "Standing", "In Move", "All", "All + Fake" };
	ComboBox("Edge##1", &cvar.aa_edge, listbox_edge, IM_ARRAYSIZE(listbox_edge), 5);

	ImGui::Text("Fakelag"), ImGui::Separator();

	ImGui::Checkbox("Activate", &cvar.fakelag_active);
	ImGui::Checkbox("Fakelag While Shooting", &cvar.fakelag_while_shooting);
	ImGui::Text("Fakelag Limit");
	SliderFloat("Limit##1", &cvar.fakelag_limit, 0.f, 30.f, "%.0f");
	ImGui::Text("Fakelag Variance");
	SliderFloat("Variance##1", &cvar.fakelag_variance, 0.f, 100.f, "%.0f");
	ImGui::Text("Fakelag Move");
	const char* listbox_move[] = { "On Land", "On Move", "In Air", "All" };
	ComboBox("Fakelag Move##1", &cvar.fakelag_move, listbox_move, IM_ARRAYSIZE(listbox_move), 4);
	ImGui::Text("Fakelag Type");
	const char* listbox_type[] = { "Dynamic", "Maximum", "Flucture", "Break Lag Compensation" };
	ComboBox("Fakelag Type##1", &cvar.fakelag_type, listbox_type, IM_ARRAYSIZE(listbox_type), 4);
}

void MenuLegit1()
{
	ImGui::Text("Select Weapon Section"), ImGui::Separator();
	const char* listbox_section[] = { "Pistol", "Sniper", "Rifle", "Shotgun", "Machine Gun", "Submachine Gun" };
	ComboBox("Section##1", &cvar.menu_legit_global_section, listbox_section, IM_ARRAYSIZE(listbox_section), 6);

	if (CheckWeapon(cvar.menu_legit_global_section, cvar.menu_legit_sub_section))
	{
		ImGui::Text("Legit Aimbot"), ImGui::Separator();

		ImGui::Checkbox("Activate", &cvar.legit[CheckWeapon(cvar.menu_legit_global_section, cvar.menu_legit_sub_section)].active);
		ImGui::Checkbox("Auto Scope", &cvar.legit_autoscope);
		ImGui::Checkbox("Aim Team", &cvar.legit_team);
		ImGui::Checkbox("Bypass Trace", &cvar.bypass_trace_legit);
		ImGui::Checkbox("Draw Aim", &cvar.legit_draw_aim);
		ImGui::Checkbox("Draw Fov", &cvar.legit[CheckWeapon(cvar.menu_legit_global_section, cvar.menu_legit_sub_section)].drawfov);
		ImGui::Checkbox("Humanize", &cvar.legit[CheckWeapon(cvar.menu_legit_global_section, cvar.menu_legit_sub_section)].humanize);
		
		HudKeyBind(cvar.legit_key, "Fire Key");

		ImGui::Text("Auto Aim Lock Speed");
		SliderFloat("Auto Aim Lock Speed##1", &cvar.legit[CheckWeapon(cvar.menu_legit_global_section, cvar.menu_legit_sub_section)].speed, 0.f, 100.f, "%.0f");
		
		ImGui::Text("Attack Aim Lock Speed");
		SliderFloat("Attack Aim Lock Speed##1", &cvar.legit[CheckWeapon(cvar.menu_legit_global_section, cvar.menu_legit_sub_section)].speed_in_attack, 0.f, 100.f, "%.0f");
		
		ImGui::Text("Block Attack After Kill");
		SliderFloat("Block Attack After Kill##1", &cvar.legit[CheckWeapon(cvar.menu_legit_global_section, cvar.menu_legit_sub_section)].block_attack_after_kill, 0.f, 1000.f, "%.0fms");
		
		ImGui::Text("Maximum FOV");
		SliderFloat("Maximum FOV##1", &cvar.legit[CheckWeapon(cvar.menu_legit_global_section, cvar.menu_legit_sub_section)].fov, 0.f, 180.f, "%.1f");
		
		ImGui::Text("Perfect Silent Attack Speed");
		SliderFloat("Perfect Silent Attack Speed##1", &cvar.legit[CheckWeapon(cvar.menu_legit_global_section, cvar.menu_legit_sub_section)].perfect_silent, 0.f, 100.f, "%.0f");
		
		ImGui::Text("Reaction Time");
		SliderFloat("Reaction Time##1", &cvar.legit[CheckWeapon(cvar.menu_legit_global_section, cvar.menu_legit_sub_section)].reaction_time, 0.f, 1000.f, "%.0fms");
		
		ImGui::Text("Recoil Compensation Pitch");
		SliderFloat("Recoil Compensation Pitch##1", &cvar.legit[CheckWeapon(cvar.menu_legit_global_section, cvar.menu_legit_sub_section)].recoil_compensation_pitch, 0.f, 100.f, "%.0f%%");
		
		ImGui::Text("Recoil Compensation Yaw");
		SliderFloat("Recoil Compensation Yaw##1", &cvar.legit[CheckWeapon(cvar.menu_legit_global_section, cvar.menu_legit_sub_section)].recoil_compensation_yaw, 0.f, 100.f, "%.0f%%");
		
		ImGui::Text("Recoil Compensation After Fire");
		SliderFloat("Recoil Compensation After Fire##1", &cvar.legit[CheckWeapon(cvar.menu_legit_global_section, cvar.menu_legit_sub_section)].recoil_compensation_after_shots_fired, 0.f, 10.f, "%.0f");
		
		ImGui::Text("Speed Scale FOV");
		SliderFloat("Speed Scale FOV##1", &cvar.legit[CheckWeapon(cvar.menu_legit_global_section, cvar.menu_legit_sub_section)].speed_scale_fov, 0.f, 100.f, "%.0f%%");
		
		ImGui::Text("Accuracy Boost");
		const char* listbox_accuracy[] = { "None", "Aiming", "Recoil", "Recoil / Spread" };
		ComboBox("Accuracy Boost##1", &cvar.legit[CheckWeapon(cvar.menu_legit_global_section, cvar.menu_legit_sub_section)].accuracy, listbox_accuracy, IM_ARRAYSIZE(listbox_accuracy), 4);
	}
}

void MenuLegit2()
{
	ImGui::Text("Select Weapon Type"), ImGui::Separator();
	CheckSubSection(cvar.menu_legit_global_section, cvar.menu_legit_sub_section);

	if (cvar.menu_legit_global_section == 0)
	{
		const char* listbox_sub1[] = { "GLOCK18", "P228", "DEAGLE", "ELITE", "FIVESEVEN", "USP" };
		ComboBox("Weapon##1", &cvar.menu_legit_sub_section, listbox_sub1, IM_ARRAYSIZE(listbox_sub1), 6);
	}
	if (cvar.menu_legit_global_section == 1)
	{
		const char* listbox_sub2[] = { "AWP", "SCOUT", "G3SG1", "SG550" };
		ComboBox("Weapon##2", &cvar.menu_legit_sub_section, listbox_sub2, IM_ARRAYSIZE(listbox_sub2), 4);
	}
	if (cvar.menu_legit_global_section == 2)
	{
		const char* listbox_sub3[] = { "M4A1", "GALIL", "FAMAS", "AUG", "AK47", "SG552" };
		ComboBox("Weapon##3", &cvar.menu_legit_sub_section, listbox_sub3, IM_ARRAYSIZE(listbox_sub3), 6);
	}
	if (cvar.menu_legit_global_section == 3)
	{
		const char* listbox_sub4[] = { "XM1014", "M3" };
		ComboBox("Weapon##4", &cvar.menu_legit_sub_section, listbox_sub4, IM_ARRAYSIZE(listbox_sub4), 2);
	}
	if (cvar.menu_legit_global_section == 4)
	{
		const char* listbox_sub5[] = { "M249" };
		ComboBox("Weapon##5", &cvar.menu_legit_sub_section, listbox_sub5, IM_ARRAYSIZE(listbox_sub5), 1);
	}
	if (cvar.menu_legit_global_section == 5)
	{
		const char* listbox_sub6[] = { "TMP", "P90", "MP5N", "MAC10", "UMP45" };
		ComboBox("Weapon##6", &cvar.menu_legit_sub_section, listbox_sub6, IM_ARRAYSIZE(listbox_sub6), 5);
	}

	if (CheckWeapon(cvar.menu_legit_global_section, cvar.menu_legit_sub_section))
	{
		ImGui::Text("Legit Trigger Aimbot"), ImGui::Separator();

		ImGui::Checkbox("Activate", &cvar.legit[CheckWeapon(cvar.menu_legit_global_section, cvar.menu_legit_sub_section)].trigger_active);
		ImGui::Checkbox("Aim Team", &cvar.legit_trigger_team);
		ImGui::Checkbox("Bypass Trace", &cvar.bypass_trace_trigger);
		ImGui::Checkbox("Draw Aim", &cvar.legit_trigger_draw_aim);
		ImGui::Checkbox("Only Zoom", &cvar.legit_trigger_only_zoom);
		ImGui::Checkbox("Wall", &cvar.legit[CheckWeapon(cvar.menu_legit_global_section, cvar.menu_legit_sub_section)].trigger_wall);
		HudKeyBind(cvar.legit_trigger_key, "Trigger Key");
		
		if (!cvar.legit[CheckWeapon(cvar.menu_legit_global_section, cvar.menu_legit_sub_section)].trigger_shot_type)
		{
			ImGui::Text("Count");
			SliderFloat("Count##1", &cvar.legit[CheckWeapon(cvar.menu_legit_global_section, cvar.menu_legit_sub_section)].trigger_shot_count, 1.f, 5.f, "%.0f");
		}
		ImGui::Text("Fire Delay");
		SliderFloat("Fire Delay##1", &cvar.legit[CheckWeapon(cvar.menu_legit_global_section, cvar.menu_legit_sub_section)].trigger_delay_shot, 0.f, 1000.f, "%.0fms");

		if (cvar.legit[CheckWeapon(cvar.menu_legit_global_section, cvar.menu_legit_sub_section)].trigger_shot_type)
		{
			ImGui::Text("Random Max");
			SliderFloat("Random Max##1", &cvar.legit[CheckWeapon(cvar.menu_legit_global_section, cvar.menu_legit_sub_section)].trigger_random_max, 2.f, 5.f, "%.0f");
		}
		ImGui::Text("Fire Type");
		const char* listbox_count[] = { "Count", "Random" };
		ComboBox("Fire Type##1", &cvar.legit[CheckWeapon(cvar.menu_legit_global_section, cvar.menu_legit_sub_section)].trigger_shot_type, listbox_count, IM_ARRAYSIZE(listbox_count), 2);
		ImGui::Text("Accuracy Boost");
		const char* listbox_accuracy[] = { "None", "Recoil", "Recoil / Spread" };
		ComboBox("Accuracy Boost##1", &cvar.legit[CheckWeapon(cvar.menu_legit_global_section, cvar.menu_legit_sub_section)].trigger_accuracy, listbox_accuracy, IM_ARRAYSIZE(listbox_accuracy), 3);
		
		ImGui::Text("Legit Knife Aimbot"), ImGui::Separator();

		ImGui::Checkbox("Activate##2", &cvar.knifebot_active);
		ImGui::Checkbox("Aim Team##2", &cvar.knifebot_team);
		ImGui::Checkbox("Bypass Trace##2", &cvar.bypass_trace_knife);
		ImGui::Checkbox("Draw Aim##2", &cvar.knifebot_draw_aim);
		ImGui::Checkbox("Silent", &cvar.knifebot_silent);
		ImGui::Checkbox("Perfect Silent", &cvar.knifebot_perfect_silent);

		ImGui::Text("Field Of View");
		SliderFloat("Field Of View##1", &cvar.knifebot_fov, 0.f, 180.f, "%.1f");
		if (cvar.knifebot_attack == 0)
		{
			ImGui::Text("Slash Distance");
			SliderFloat("Slash Distance##1", &cvar.knifebot_attack_distance, 0.f, 100.f, "%.0f");
		}
		if (cvar.knifebot_attack == 1)
		{
			ImGui::Text("Stab Distance");
			SliderFloat("Stab Distance##1", &cvar.knifebot_attack2_distance, 0.f, 100.f, "%.0f");
		}
		ImGui::Text("Knife Attack Method");
		const char* listbox_attack[] = { "Slash", "Stab" };
		ComboBox("Knife Attack Method##1", &cvar.knifebot_attack, listbox_attack, IM_ARRAYSIZE(listbox_attack), 2);
	}
}

void MenuLegit3()
{
	ImGui::Text("Hitboxes"), ImGui::Separator();

	if (ImGui::Button("Save Hitbox"))SaveHitboxLegit();
	ImGui::SameLine();
	if (ImGui::Button("Load Hitbox"))LoadHitboxLegit();
	ImGui::SameLine();
	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
	if (ImGui::Button("Clear Current"))
	{
		std::deque<int> Hitboxes;
		for (unsigned int i = 0; i < PlayerAimLegit.size(); i++)
		{
			if (PlayerAimLegit[i].m_iWeaponID != CheckWeapon(cvar.menu_legit_global_section, cvar.menu_legit_sub_section))
				continue;
			Hitboxes.push_back(i);
		}
		for (int x = Hitboxes.size() - 1; x >= 0; x--)
			PlayerAimLegit.erase(PlayerAimLegit.begin() + Hitboxes[x]);
	}
	ImGui::PopStyleColor();
	
	for (unsigned int x = 0; x < Model_Aim_Select.size(); x++)
	{
		float checksave = 1;
		for (playeraimlegit_t AimLegit : PlayerAimLegit)
		{
			if (strcmp(AimLegit.checkmodel, Model_Aim_Select[x].checkmodel))
				continue;
			if (AimLegit.numhitbox != Model_Aim_Select[x].numhitbox)
				continue;
			if (AimLegit.m_iWeaponID != CheckWeapon(cvar.menu_legit_global_section, cvar.menu_legit_sub_section))
				continue;
			
			checksave = 0;
		}

		char str[256];
		float button = checksave;
		sprintf(str, "Model:##%d", x);
		ImGui::Checkbox(str, &button);
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), Model_Aim_Select[x].displaymodel);
		ImGui::SameLine();
		ImGui::Text("Hitbox: %d", Model_Aim_Select[x].numhitbox);

		if (checksave && !button)
		{
			playeraimlegit_t AimLegit;
			AimLegit.numhitbox = Model_Aim_Select[x].numhitbox;
			sprintf(AimLegit.displaymodel, Model_Aim_Select[x].displaymodel);
			sprintf(AimLegit.checkmodel, Model_Aim_Select[x].checkmodel);
			AimLegit.m_iWeaponID = CheckWeapon(cvar.menu_legit_global_section, cvar.menu_legit_sub_section);
			PlayerAimLegit.push_back(AimLegit);
		}
		if (!checksave && button)
		{
			for (unsigned int i = 0; i < PlayerAimLegit.size(); i++)
			{
				if (strcmp(PlayerAimLegit[i].checkmodel, Model_Aim_Select[x].checkmodel))
					continue;
				if (PlayerAimLegit[i].numhitbox != Model_Aim_Select[x].numhitbox)
					continue;
				PlayerAimLegit.erase(PlayerAimLegit.begin() + i);
			}
		}
	}
}

void MenuModelAim1()
{
	ImGui::Text("Model Hitboxes"), ImGui::Separator();

	ImGui::Checkbox("Show Player Model", &cvar.visual_model);
	ImGui::SameLine();
	ImGui::Checkbox("Show Player Hitbox", &cvar.visual_model_hitbox);

	static int modelselect = 0;
	for (unsigned int i = 0; i < Model_Aim.size(); i++)
	{
		if (modelselect > Model_Aim.size() - 1)
			modelselect = Model_Aim.size() - 1;
		if (i == 0) ImGui::Separator();
		char str[256];
		sprintf(str, "%s", Model_Aim[i].displaymodel);
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 1.0f, 0.0f, 1.0f));
		if (ImGui::Button(str)) modelselect = i;
		ImGui::PopStyleColor();
		
		if (i == modelselect)
		{
			static int Hitbox = 0;
			if (Hitbox > Model_Aim[i].numhitboxes - 1)
				Hitbox = Model_Aim[i].numhitboxes - 1;
			ImGui::SameLine();
			if (ImGui::ArrowButton("##left", ImGuiDir_Left))
			{
				if (Hitbox > 0)Hitbox--;
				else
					Hitbox = Model_Aim[i].numhitboxes - 1;
			}
			ImGui::SameLine();
			if (ImGui::ArrowButton("##right", ImGuiDir_Right))
			{
				if (Hitbox < Model_Aim[i].numhitboxes - 1)Hitbox++;
				else
					Hitbox = 0;
			}
			ImGui::SameLine();
			ImGui::Text("Hitbox: %d", Hitbox);
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 1.0f, 0.0f, 1.0f));
			ImGui::SameLine();
			if (ImGui::Button("Push One"))
			{
				bool saved = false;
				for (model_aim_select_t Model_Selected : Model_Aim_Select)
				{
					if (!strcmp(Model_Selected.checkmodel, Model_Aim[i].checkmodel) && Model_Selected.numhitbox == Hitbox)
					{
						saved = true;
						break;
					}
				}
				if (!saved)
				{
					model_aim_select_t Model_Select;
					sprintf(Model_Select.displaymodel, Model_Aim[i].displaymodel);
					sprintf(Model_Select.checkmodel, Model_Aim[i].checkmodel);
					Model_Select.numhitbox = Hitbox;
					Model_Aim_Select.push_front(Model_Select);
				}
			}
			ImGui::SameLine();
			if (ImGui::Button("Push All"))
			{
				for (int x = Model_Aim[i].numhitboxes - 1; x >= 0; x--)
				{
					bool saved = false;
					for (model_aim_select_t Model_Selected : Model_Aim_Select)
					{
						if (!strcmp(Model_Selected.checkmodel, Model_Aim[i].checkmodel) && Model_Selected.numhitbox == x)
						{
							saved = true;
							break;
						}
					}
					if (!saved)
					{
						model_aim_select_t Model_Select;
						sprintf(Model_Select.displaymodel, Model_Aim[i].displaymodel);
						sprintf(Model_Select.checkmodel, Model_Aim[i].checkmodel);
						Model_Select.numhitbox = x;
						Model_Aim_Select.push_front(Model_Select);
					}
				}
			}
			ImGui::PopStyleColor();
		}
	}
}

void MenuModelAim2()
{
	ImGui::Text("Selected Model Hitboxes"), ImGui::Separator();

	if (ImGui::Button("Save Hitbox"))SaveHitbox();
	ImGui::SameLine();
	if (ImGui::Button("Load Hitbox"))LoadHitbox();
	ImGui::SameLine();
	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
	if (ImGui::Button("Clear All"))Model_Aim_Select.clear();
	ImGui::PopStyleColor();
	
	for (unsigned int i = 0; i < Model_Aim_Select.size(); i++)
	{
		if (i == 0) ImGui::Separator();

		ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "%s", Model_Aim_Select[i].displaymodel);

		ImGui::SameLine();

		char str[256];

		sprintf(str, "+##%d", i);
		if (ImGui::ArrowButton(str, ImGuiDir_Up))
		{
			if (i > 0)
			{
				if (!strcmp(Model_Aim_Select[i].checkmodel, Model_Aim_Select[i - 1].checkmodel))
					std::swap(Model_Aim_Select[i], Model_Aim_Select[i - 1]);
				else
				{
					if (i - 2 > 0)
					{
						for (int x = i - 2; x >= 0; x--)
						{
							if (!strcmp(Model_Aim_Select[i].checkmodel, Model_Aim_Select[x].checkmodel))
							{
								for (int z = i; z > x + 1; z--)
								{
									std::swap(Model_Aim_Select[z], Model_Aim_Select[z - 1]);
								}
								break;
							}
						}
					}
				}
			}
		}
		if (ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();
			ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
			ImGui::TextUnformatted("Push Priority Front");
			ImGui::PopTextWrapPos();
			ImGui::EndTooltip();
		}

		ImGui::SameLine();

		sprintf(str, "-##%d", i);
		if (ImGui::ArrowButton(str, ImGuiDir_Down))
		{
			if (i + 1 < Model_Aim_Select.size())
			{
				if (!strcmp(Model_Aim_Select[i].checkmodel, Model_Aim_Select[i + 1].checkmodel))
					std::swap(Model_Aim_Select[i], Model_Aim_Select[i + 1]);
				else
				{
					if (i + 2 < Model_Aim_Select.size())
					{
						for (unsigned int x = i + 2; x < Model_Aim_Select.size(); x++)
						{
							if (!strcmp(Model_Aim_Select[i].checkmodel, Model_Aim_Select[x].checkmodel))
							{
								for (unsigned int z = i; z < x - 1; z++)
								{
									std::swap(Model_Aim_Select[z], Model_Aim_Select[z + 1]);
								}
								break;
							}
						}
					}
				}
			}
		}

		if (ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();
			ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
			ImGui::TextUnformatted("Push Priority Back");
			ImGui::PopTextWrapPos();
			ImGui::EndTooltip();
		}
		ImGui::SameLine();
		ImGui::Text("Hitbox: %d", Model_Aim_Select[i].numhitbox);

		ImGui::SameLine();
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
		sprintf(str, "Clear##%d", i);
		if (ImGui::Button(str))
			Model_Aim_Select.erase(Model_Aim_Select.begin() + i);
		ImGui::PopStyleColor();
	}
}

void MenuSpawn()
{
	ImGui::Text("Spawn Point"), ImGui::Separator();
	ImGui::Checkbox("Draw", &cvar.visual_spawn_points);
	if (cvar.visual_spawn_points)
	{
		ImGui::SameLine();
		ImGui::Checkbox("Scan", &cvar.visual_spawn_scan);
		ImGui::SameLine();
		ImGui::Checkbox("Show Point Number", &cvar.visual_spawn_num);

		if (ImGui::Button("Save"))
			SaveSpawn();
		ImGui::SameLine();
		if (ImGui::Button("Load"))
			LoadSpawn();
		ImGui::SameLine();
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
		if (ImGui::Button("Clear All"))
			SpawnOrigin.clear();
		ImGui::PopStyleColor();

		char str[256];
		for (unsigned int i = 0; i < SpawnOrigin.size(); i++)
		{
			if(i == 0)ImGui::Separator();
			sprintf(str, "Point: %d", i);
			ImGui::Text(str);
			ImGui::SameLine();

			sprintf(str, "Clear##%d", i);
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
			if (ImGui::Button(str))
				SpawnOrigin.erase(SpawnOrigin.begin() + i);
			ImGui::PopStyleColor();
		}
	}
}

void MenuIdHook1()
{
	ImGui::Text("Player List"), ImGui::Separator();
	bool terror = false;
	for (unsigned int id = 1; id <= g_Engine.GetMaxClients(); id++)
	{
		player_info_s* player = g_Studio.PlayerInfo(id - 1);
		if (!player || !(lstrlenA(player->name) > 0))
			continue;
		if (id == pmove->player_index + 1)
			continue;
		if (g_Player[id].iTeam != 1)
			continue;
		if (!terror)ImGui::Text("Terror:"), terror = true;

		if (idhook.FirstKillPlayer[id] == 0)
		{
			ImVec4 col = ImColor(1.f, 0.f, 0.f, 1.0f);
			ImGui::PushStyleColor(ImGuiCol_Text, col);
			char str[256];
			sprintf(str, "%s", player->name);
			if (ImGui::Button(str))
				idhook.AddPlayer(id);
			if (g_Player[id].bVip) ImGui::SameLine(), ImGui::Image((GLuint*)texture_id[VIP], ImVec2(25, 17));
			ImGui::PopStyleColor();
		}
		else if (idhook.FirstKillPlayer[id] == 1)
		{
			ImVec4 col = ImColor(0.f, 1.f, 0.f, 1.0f);
			ImGui::PushStyleColor(ImGuiCol_Text, col);
			char str[256];
			sprintf(str, "%s [On]", player->name);
			if (ImGui::Button(str))
				idhook.AddPlayer(id);
			if (g_Player[id].bVip) ImGui::SameLine(), ImGui::Image((GLuint*)texture_id[VIP], ImVec2(23, 17));
			ImGui::PopStyleColor();
		}
		else
		{
			ImVec4 col = ImColor(1.f, 1.f, 0.f, 1.0f);
			ImGui::PushStyleColor(ImGuiCol_Text, col);
			char str[256];
			sprintf(str, "%s [Off]", player->name);
			if (ImGui::Button(str))
				idhook.AddPlayer(id);
			if (g_Player[id].bVip) ImGui::SameLine(), ImGui::Image((GLuint*)texture_id[VIP], ImVec2(23, 17));
			ImGui::PopStyleColor();
		}
	}
	bool counterterror = false;
	for (unsigned int id = 1; id <= g_Engine.GetMaxClients(); id++)
	{
		player_info_s* player = g_Studio.PlayerInfo(id - 1);
		if (!player || !(lstrlenA(player->name) > 0))
			continue;
		if (id == pmove->player_index + 1)
			continue;
		if (g_Player[id].iTeam != 2)
			continue;
		if (!counterterror)ImGui::Text("Counter Terror:"), counterterror = true;

		if (idhook.FirstKillPlayer[id] == 0)
		{
			ImVec4 col = ImColor(0.25f, 0.41f, 1.f, 1.0f);
			ImGui::PushStyleColor(ImGuiCol_Text, col);
			char str[256];
			sprintf(str, "%s", player->name);
			if (ImGui::Button(str))
				idhook.AddPlayer(id);
			if (g_Player[id].bVip) ImGui::SameLine(), ImGui::Image((GLuint*)texture_id[VIP], ImVec2(23, 17));
			ImGui::PopStyleColor();
		}
		else if (idhook.FirstKillPlayer[id] == 1)
		{
			ImVec4 col = ImColor(0.f, 1.f, 0.f, 1.0f);
			ImGui::PushStyleColor(ImGuiCol_Text, col);
			char str[256];
			sprintf(str, "%s [On]", player->name);
			if (ImGui::Button(str))
				idhook.AddPlayer(id);
			if (g_Player[id].bVip) ImGui::SameLine(), ImGui::Image((GLuint*)texture_id[VIP], ImVec2(23, 17));
			ImGui::PopStyleColor();
		}
		else
		{
			ImVec4 col = ImColor(1.f, 1.f, 0.f, 1.0f);
			ImGui::PushStyleColor(ImGuiCol_Text, col);
			char str[256];
			sprintf(str, "%s [Off]", player->name);
			if (ImGui::Button(str))
				idhook.AddPlayer(id);
			if (g_Player[id].bVip) ImGui::SameLine(), ImGui::Image((GLuint*)texture_id[VIP], ImVec2(23, 17));
			ImGui::PopStyleColor();
		}
	}
	if (!counterterror && !terror)
		ImGui::Text("No Players!");
}

void MenuIdHook2()
{
	ImGui::Text("Player Settings"), ImGui::Separator();

	ImGui::Checkbox("Esp Only On", &cvar.visual_idhook_only);
	if (cvar.aim_id_mode == 0)
	{
		if (ImGui::Button("Attack All"))
			idhook.First_Kill_Mode();
	}
	if (cvar.aim_id_mode == 1)
	{
		if (ImGui::Button("Attack On First, Dont Attack Off"))
			idhook.First_Kill_Mode();
	}
	if (cvar.aim_id_mode == 2)
	{
		if (ImGui::Button("Attack Only On"))
			idhook.First_Kill_Mode();
	}
	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
	if (ImGui::Button("Clear All Player"))
		idhook.ClearPlayer();
	ImGui::PopStyleColor();
}

void MenuVisual1Window1()
{
	ImGui::Text("Visual1"), ImGui::Separator();

	ImGui::Checkbox("Player Box", &cvar.visual_box);
	ImGui::Checkbox("Player Health", &cvar.visual_health);
	ImGui::Checkbox("Player Lambert", &cvar.visual_lambert);
	ImGui::Checkbox("Player Name", &cvar.visual_name);
	ImGui::Checkbox("Player Reload", &cvar.visual_reload_bar);
	ImGui::Checkbox("Player Sound Index", &cvar.visual_sound_index);
	ImGui::Checkbox("Player Sound No Index", &cvar.visual_sound_no_index);
	ImGui::Checkbox("Player Sound Steps", &cvar.visual_sound_steps);
	ImGui::Checkbox("Player Vip", &cvar.visual_vip);
	ImGui::Checkbox("Player Visual Team", &cvar.visual_visual_team);
	ImGui::Checkbox("Player Weapon Name", &cvar.visual_weapon);
	ImGui::Checkbox("World Box", &cvar.visual_box_world);
	ImGui::Checkbox("World Model", &cvar.visual_model_world);
	ImGui::Checkbox("World Owner Name", &cvar.visual_name_world);
	ImGui::Separator();
	ImGui::Text("Third Person");
	ImGui::Checkbox("Chase", &cvar.visual_chase_cam);
	ImGui::Separator();
	ImGui::Text("Other");
	ImGui::Checkbox("Crosshair", &cvar.visual_crosshair);
	ImGui::Checkbox("Esp Preview", &cvar.model_preview);
	ImGui::Checkbox("Grenade Trajectory", &cvar.visual_grenade_trajectory);
	ImGui::Checkbox("Kill Sound", &cvar.radio_kill_sound);
	ImGui::Checkbox("Light Map", &cvar.visual_lightmap);
	ImGui::Checkbox("My Weapon Name", &cvar.visual_weapon_local);
	ImGui::Checkbox("No Flash", &cvar.visual_rem_flash);
	ImGui::Checkbox("Radar", &cvar.radar);
}

void MenuVisual1Window2()
{
	ImGui::Text("Visual2"), ImGui::Separator();

	ImGui::Text("Chase Back");
	SliderFloat("Chase Back##1", &cvar.visual_chase_back, 100.f, 1000.f, "%.0f");
	ImGui::Text("Chase Up");
	SliderFloat("Chase Up##1", &cvar.visual_chase_up, 16.f, 500.f, "%.0f");
	ImGui::Text("Kill Sound Volume");
	SliderFloat("Kill Sound Volume##1", &cvar.radio_kill_volume, 0.f, 100.f, "%.0f");
	ImGui::Text("Radar Zoom");
	SliderFloat("Radar Zoom##1", &cvar.radar_zoom, 1, 10, "%.1f");
	ImGui::Text("Radar Point Size");
	SliderFloat("Radar Point Size##1", &cvar.radar_point_size, 5, 25, "%.1f");
	ImGui::Text("View Model FOV");
	SliderFloat("View Model FOV##1", &cvar.visual_viewmodel_fov, 0, 50, "%.0f");
	
	ImGui::Text("Skeleton"), ImGui::Separator();

	ImGui::Checkbox("Player Bone", &cvar.skeleton_player_bone);
	ImGui::Checkbox("Player HitBox", &cvar.skeleton_player_hitbox);
	ImGui::Checkbox("Player Weapon Bone", &cvar.skeleton_player_weapon_bone);
	ImGui::Checkbox("Player Weapon HitBox", &cvar.skeleton_player_weapon_hitbox);
	ImGui::Checkbox("View Model Bone", &cvar.skeleton_view_model_bone);
	ImGui::Checkbox("View Model HitBox", &cvar.skeleton_view_model_hitbox);
	ImGui::Checkbox("World Bone", &cvar.skeleton_world_bone);
	ImGui::Checkbox("World Hitbox", &cvar.skeleton_world_hitbox);

	ImGui::Text("Chams"), ImGui::Separator();

	ImGui::Checkbox("Player Chams Wall", &cvar.chams_player_wall);
	ImGui::Checkbox("World Chams Wall", &cvar.chams_world_wall);
	const char* listbox_player[] = { "Player Chams None", "Player Chams Material", "Player Chams Texture", "Player Chams Flat" };
	ComboBox("Player Chams", &cvar.chams_player, listbox_player, IM_ARRAYSIZE(listbox_player), 4);
	const char* listbox_weapon[] = { "View Model Chams None", "View Model Chams Material", "View Model Chams Texture", "View Model Chams Flat" };
	ComboBox("View Model Chams", &cvar.chams_view_model, listbox_weapon, IM_ARRAYSIZE(listbox_weapon), 4);
	const char* listbox_world[] = { "World Chams None", "World Chams Material", "World Chams Texture", "World Chams Flat" };
	ComboBox("World Chams", &cvar.chams_world, listbox_world, IM_ARRAYSIZE(listbox_world), 4);
}

void MenuVisual2Window1()
{
	ImGui::Text("Player/ViewModel/World Texture"), ImGui::Separator();
	if (ImGui::Button("Reload Textures From Folder"))
		loadtexturemodel = true;
	ImGui::Checkbox("Player", &cvar.visual_skins_player);
	ImGui::Checkbox("Player Weapon", &cvar.visual_skins_player_weapon);
	ImGui::Checkbox("Player Backweapon", &cvar.visual_skins_player_backweapon);
	ImGui::Checkbox("Player Backpack", &cvar.visual_skins_backpack);
	ImGui::Checkbox("Player Thighpack", &cvar.visual_skins_thighpack);
	ImGui::Checkbox("View Model", &cvar.visual_skins_viewmodel);
	ImGui::Checkbox("View Model Hands", &cvar.visual_skins_viewmodel_hands);
	ImGui::Checkbox("Bullet Shell", &cvar.visual_skins_bullet_shell);
	ImGui::Checkbox("World", &cvar.visual_skins_world);
	ImGui::Checkbox("Chicken", &cvar.visual_skins_chicken);
}

void MenuVisual2Window2()
{
	ImGui::Text("Sky Texture"), ImGui::Separator();
	if (ImGui::Button("Reload Textures From Folder"))
		loadtexturesky = true;
	ImGui::Checkbox("Sky", &cvar.visual_sky);
}

void MenuVisual3Window1()
{
	if (cvar.visual_skins_viewmodel_nohands)
	{
		ImGui::Text("Select Texture To Remove"), ImGui::Separator();
		ImGui::Checkbox("Enable", &cvar.visual_skins_viewmodel_nohands);

		cl_entity_s* vm = g_Engine.GetViewModel();
		if (vm && vm->model)
		{
			studiohdr_t* pStudioHeader = (studiohdr_t*)g_Studio.Mod_Extradata(vm->model);
			mstudiotexture_t* ptexture = (mstudiotexture_t*)((byte*)pStudioHeader + pStudioHeader->textureindex);

			int textures = 0;
			for (unsigned int i = 0; i < pStudioHeader->numtextures; i++)
			{
				if (ptexture[i].name && ptexture[i].index)
				{
					if (textures != 0 && (textures % 4))
						ImGui::SameLine();

					if (ImGui::ImageButtonID(textures + 1, (GLuint*)ptexture[i].index, ImVec2(60, 60)))
					{
						bool saved = false;
						for (viewmodeltexture_t Tex : ViewModelTexture)
						{
							if (!strcmp(Tex.name, ptexture[i].name))
							{
								saved = true;
								break;
							}
						}
						if (!saved)
						{
							viewmodeltexture_t Tex;
							strcpy(Tex.name, ptexture[i].name);
							ViewModelTexture.push_back(Tex);
						}
					}
					if (ImGui::IsItemHovered())
					{
						ImGui::BeginTooltip();
						ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
						ImGui::Text("Name: %s", ptexture[i].name);
						ImGui::Text("Width: %d", ptexture[i].width);
						ImGui::Text("Height: %d", ptexture[i].height);
						ImGui::Image((GLuint*)ptexture[i].index, ImVec2(256, 256));
						ImGui::PopTextWrapPos();
						ImGui::EndTooltip();
					}
					textures++;
				}
			}
		}
		else
			ImGui::Text("No View Model");
	}
	else
	{
		ImGui::Text("No Hands"), ImGui::Separator();
		ImGui::Checkbox("Enable", &cvar.visual_skins_viewmodel_nohands);
	}
	if (viewmodelactive)
	{
		ImGui::Text("Select New ViewModel"), ImGui::Separator();
		std::deque<viewmodelchanger_t> ViewModels;
		for (int i = 0; i < ViewModelChanger.size(); i++)
		{
			if (strstr(ViewModelChanger[i].modelname, getfilename(viewmodelname).c_str()))
			{
				bool saved = false;
				for (viewmodelchanger_t View : ViewModels)
				{
					if (!strcmp(View.modelname, ViewModelChanger[i].modelname))
					{
						saved = true;
						break;
					}
				}
				if (!saved)
				{
					viewmodelchanger_t View;
					strcpy(View.modelname, ViewModelChanger[i].modelname);
					ViewModels.push_back(View);
				}
			}
		}
		bool moveback = false, movefront = false;
		if (ImGui::ArrowButton("##left", ImGuiDir_Left))
		{
			moveback = true;
		}
		ImGui::SameLine();
		if (ImGui::ArrowButton("##right", ImGuiDir_Right))
		{
			movefront = true;
		}
		
		int selected = -1;

		for (int i = 0; i < ViewModels.size(); i++)
		{
			cl_entity_s* vm = g_Engine.GetViewModel();
			if (vm && !strcmp(ViewModels[i].modelname, vm->model->name))
				selected = i;
		}
		ImGui::SameLine();
		if (selected != -1)
			ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "%s", ViewModels[selected].modelname);
		if ((movefront || moveback) && selected != -1)
		{
			if (moveback)
			{
				int index;
				if (selected - 1 >= 0)
				{
					index = selected - 1;
				}
				else
					index = ViewModels.size() - 1;

				for (int i = 0; i < ViewModelChanged.size(); i++)
				{
					if (!strcmp(viewmodelname, ViewModelChanged[i].modelnameold))
						ViewModelChanged.erase(ViewModelChanged.begin() + i);
				}
				if (strcmp(viewmodelname, ViewModels[index].modelname))
				{
					viewmodelchanged_t View;
					strcpy(View.modelnameold, viewmodelname);
					strcpy(View.modelnamenew, ViewModels[index].modelname);
					ViewModelChanged.push_back(View);
				}
			}
			if (movefront)
			{
				int index;
				if (selected + 1 < ViewModels.size())
				{
					index = selected + 1;
				}
				else
					index = 0;

				for (int i = 0; i < ViewModelChanged.size(); i++)
				{
					if (!strcmp(viewmodelname, ViewModelChanged[i].modelnameold))
						ViewModelChanged.erase(ViewModelChanged.begin() + i);
				}
				if (strcmp(viewmodelname, ViewModels[index].modelname))
				{
					viewmodelchanged_t View;
					strcpy(View.modelnameold, viewmodelname);
					strcpy(View.modelnamenew, ViewModels[index].modelname);
					ViewModelChanged.push_back(View);
				}
			}
		}
		for (int i = 0; i < ViewModels.size(); i++)
		{
			ImVec4 prevColor = ImGui::GetStyle().Colors[ImGuiCol_Text];
			cl_entity_s* vm = g_Engine.GetViewModel();
			if (vm && !strcmp(ViewModels[i].modelname, vm->model->name))
				ImGui::GetStyle().Colors[ImGuiCol_Text] = ImVec4(0.0f, 1.0f, 0.0f, 1.0f);
			char str[256];
			sprintf(str, "Use Model##%d", i);
			if (ImGui::Button(str))
			{
				for (int i = 0; i < ViewModelChanged.size(); i++)
				{
					if (!strcmp(viewmodelname, ViewModelChanged[i].modelnameold))
						ViewModelChanged.erase(ViewModelChanged.begin() + i);
				}
				if (strcmp(viewmodelname, ViewModels[i].modelname))
				{
					viewmodelchanged_t View;
					strcpy(View.modelnameold, viewmodelname);
					strcpy(View.modelnamenew, ViewModels[i].modelname);
					ViewModelChanged.push_back(View);
				}
			}
			ImGui::GetStyle().Colors[ImGuiCol_Text] = prevColor;

			ImGui::SameLine();
			ImGui::Text(ViewModels[i].modelname);
		}
	}
}

void MenuVisual3Window2()
{
	if (cvar.visual_skins_viewmodel_nohands)
	{
		ImGui::Text("Selected Texture"), ImGui::Separator();
		if (ImGui::Button("Save"))
			SaveView();
		ImGui::SameLine();
		if (ImGui::Button("Load"))
			LoadView();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
		if (ImGui::Button("Clear All"))
		{
			ResetViewModelSkin();
			ViewModelTexture.clear();
		}
		ImGui::PopStyleColor();
		
		cl_entity_s* vm = g_Engine.GetViewModel();
		if (vm && vm->model)
		{
			studiohdr_t* pStudioHeader = (studiohdr_t*)g_Studio.Mod_Extradata(vm->model);
			mstudiotexture_t* ptexture = (mstudiotexture_t*)((byte*)pStudioHeader + pStudioHeader->textureindex);

			int textures = 0;
			for (unsigned int i = 0; i < pStudioHeader->numtextures; i++)
			{
				if (ptexture[i].name && ptexture[i].index)
				{
					for (int x = 0; x < ViewModelTexture.size(); x++)
					{
						if (!strcmp(ptexture[i].name, ViewModelTexture[x].name))
						{
							if (textures != 0 && (textures % 4))
								ImGui::SameLine();

							ImGui::Image((GLuint*)ptexture[i].index, ImVec2(60, 60));
							if (ImGui::IsItemHovered())
							{
								ImGui::BeginTooltip();
								ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
								ImGui::Text("Name: %s", ptexture[i].name);
								ImGui::Text("Width: %d", ptexture[i].width);
								ImGui::Text("Height: %d", ptexture[i].height);
								ImGui::Image((GLuint*)ptexture[i].index, ImVec2(256, 256));
								ImGui::PopTextWrapPos();
								ImGui::EndTooltip();
							}
							textures++;
						}
					}
				}
			}
		}
		else
			ImGui::Text("No View Model");
	}

	if (viewmodelactive)
	{
		ImGui::Text("Selected ViewModels"), ImGui::Separator();
		if (ImGui::Button("Save##2"))
			SaveViewChanger();
		ImGui::SameLine();
		if (ImGui::Button("Load##2"))
			LoadViewChanger();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
		if (ImGui::Button("Clear All##2"))
			ViewModelChanged.clear();
		ImGui::PopStyleColor();
		for (viewmodelchanged_t View : ViewModelChanged)
			ImGui::Text(View.modelnamenew);
	}
}

void MenuVisual4Window1()
{
	if (cvar.visual_skins_wall)
	{
		ImGui::Text("Select Texture To Replace"), ImGui::Separator();
		ImGui::Checkbox("Enable", &cvar.visual_skins_wall);
		ImGui::SameLine();
		if (ImGui::Button("Save"))
			savewall = true;
		ImGui::SameLine();
		if (ImGui::Button("Load"))
			loadwall = true;
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
		if (ImGui::Button("Clear All Textures"))
			resetall = true;
		ImGui::PopStyleColor();
		if (ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();
			ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
			ImGui::TextUnformatted("Reset to original");
			ImGui::PopTextWrapPos();
			ImGui::EndTooltip();
		}
		cl_entity_s* ent = g_Engine.GetEntityByIndex(0);
		if (!ent || !ent->model)return;
		int textures = 0;
		for (unsigned int i = 0; i < ent->model->numtextures; i++)
		{
			if (ent->model->textures[i]->name && ent->model->textures[i]->gl_texturenum)
			{
				if (textures != 0 && (textures % 4))
					ImGui::SameLine();
				ImVec4 prevColoract = ImGui::GetStyle().Colors[ImGuiCol_ButtonActive];
				ImVec4 prevColorhov = ImGui::GetStyle().Colors[ImGuiCol_ButtonHovered];
				ImVec4 prevColor = ImGui::GetStyle().Colors[ImGuiCol_Button];
				if (selected[i])
				{
					ImGui::GetStyle().Colors[ImGuiCol_ButtonActive] = Green();
					ImGui::GetStyle().Colors[ImGuiCol_ButtonHovered] = Green();
					ImGui::GetStyle().Colors[ImGuiCol_Button] = Green();
				}

				if (ImGui::ImageButtonID(textures + 1, (GLuint*)ent->model->textures[i]->gl_texturenum, ImVec2(60, 60)))
				{
					if (resetindex[i])
						resetone[i] = true;
					else
					{
						if (selected[i])
							resetone[i] = true;
						else
							selected[i] = true;
					}
					newimage = false;
				}
				ImGui::GetStyle().Colors[ImGuiCol_ButtonActive] = prevColoract;
				ImGui::GetStyle().Colors[ImGuiCol_ButtonHovered] = prevColorhov;
				ImGui::GetStyle().Colors[ImGuiCol_Button] = prevColor;
				if (ImGui::IsItemHovered())
				{
					ImGui::BeginTooltip();
					ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
					ImGui::Text("Name: %s", ent->model->textures[i]->name);
					ImGui::Text("Width: %d", ent->model->textures[i]->width);
					ImGui::Text("Height: %d", ent->model->textures[i]->height);
					ImGui::Image((GLuint*)ent->model->textures[i]->gl_texturenum, ImVec2(256, 256));
					ImGui::PopTextWrapPos();
					ImGui::EndTooltip();
				}
				textures++;
			}
		}
	}
	else
	{
		ImGui::Text("Wall Texture"), ImGui::Separator();
		ImGui::Checkbox("Enable", &cvar.visual_skins_wall);
	}
}

void MenuVisual4Window2()
{
	if (cvar.visual_skins_wall)
	{
		ImGui::Text("Select Texture To Replace With"), ImGui::Separator();
		int textures = 0;
		if (ImGui::Button("Reload Textures From Folder"))
			loadtexturewall = true;

		if (ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();
			ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
			ImGui::TextUnformatted("You can put textures in /wall folder and reload upto 512 images");
			ImGui::PopTextWrapPos();
			ImGui::EndTooltip();
		}
		for (unsigned int i = 500; i < 1024; i++)
		{
			if (texture_id[i])
			{
				if (textures != 0 && (textures % 4))
					ImGui::SameLine();

				if (ImGui::ImageButtonID(textures + 1, (GLuint*)texture_id[i], ImVec2(60, 60)))
				{
					newimage = true;
					arraynum = i;
				}
				if (ImGui::IsItemHovered())
				{
					ImGui::BeginTooltip();
					ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);

					ImGui::Text("Name: %s", imagenamenew[i]);
					ImGui::Text("Width: %d", imagewidthnew[i]);
					ImGui::Text("Height: %d", imageheightnew[i]);
					ImGui::Image((GLuint*)texture_id[i], ImVec2(256, 256));
					ImGui::PopTextWrapPos();
					ImGui::EndTooltip();
				}
				textures++;
			}
		}
	}
	else
		ImGui::Text("Enable To Open"), ImGui::Separator();
}

void MenuKz1()
{
	ImGui::Text("Kz"), ImGui::Separator();

	ImGui::Checkbox("Bhop", &cvar.kz_bhop);
	ImGui::Checkbox("Bhop Double", &cvar.kz_bhop_double);
	ImGui::Checkbox("Bhop Triple", &cvar.kz_bhop_triple);
	ImGui::Checkbox("Fast Run", &cvar.kz_fast_run);
	ImGui::Checkbox("GStrafe", &cvar.kz_ground_strafe);
	ImGui::Checkbox("Jump Bug", &cvar.kz_jump_bug);
	ImGui::Checkbox("Jump Bug Auto", &cvar.kz_jump_bug_auto);
	ImGui::Checkbox("Strafe", &cvar.kz_strafe);
	ImGui::Checkbox("Strafe Perfect Silent", &cvar.kz_strafe_psilent);
	ImGui::Checkbox("Window", &cvar.kz_show_kz);
	ImGui::Text("Display Time");
	SliderFloat("Display Time##1", &cvar.kz_display_time, 1, 20);
}

void MenuKz2()
{
	ImGui::Text("Kz Keys"), ImGui::Separator();

	HudKeyBind(cvar.kz_bhop_key, "Bhop Key");
	HudKeyBind(cvar.kz_fastrun_key, "Fastrun Key");
	HudKeyBind(cvar.kz_ground_strafe_key, "Gstrafe Key");
	HudKeyBind(cvar.kz_jumpbug_key, "Jumpbug Key");
	HudKeyBind(cvar.kz_strafe_key, "Strafe Key");
}

void MenuRoute1()
{
	ImGui::Text("Route1"), ImGui::Separator();

	ImGui::Checkbox("Activate", &cvar.route_activate);
	ImGui::Checkbox("Auto Direction", &cvar.route_direction);
	ImGui::Checkbox("Auto Jump", &cvar.route_jump);
	ImGui::Checkbox("Draw Route", &cvar.route_draw);
	ImGui::Checkbox("Visual While Rushing", &cvar.route_draw_visual);
	ImGui::Text("Direction Steps");
	SliderFloat("Direction Steps##1", &cvar.route_direction_step, 1.f, 10.f, "%.0f");
	ImGui::Text("Jump Steps");
	SliderFloat("Jump Steps##1", &cvar.route_jump_step, 1.f, 10.f, "%.0f");
	ImGui::Text("Route Mode");
	const char* listbox_route[] = { "Stay In Route", "Go After Target" };
	ComboBox("Route Mode##1", &cvar.route_mode, listbox_route, IM_ARRAYSIZE(listbox_route), 2);
}

void MenuRoute2()
{
	ImGui::Text("Route2"), ImGui::Separator();

	ImVec4 prevColor = ImGui::GetStyle().Colors[ImGuiCol_Text];
	ImGui::GetStyle().Colors[ImGuiCol_Text] = ImVec4(0.0f, 1.0f, 0.0f, 1.0f);
	if (ImGui::Button("Start"))autoroute.RecordAnyRouteStart();
	ImGui::GetStyle().Colors[ImGuiCol_Text] = prevColor;
	if (autoroute.Record)ImGui::SameLine(), ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "Recording!");
	prevColor = ImGui::GetStyle().Colors[ImGuiCol_Text];
	ImGui::GetStyle().Colors[ImGuiCol_Text] = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
	if (ImGui::Button("Stop"))autoroute.RecordRouteStop();
	ImGui::GetStyle().Colors[ImGuiCol_Text] = prevColor;
	if (!autoroute.Record)ImGui::SameLine(), ImGui::Text("Recording Stopped!");
	if (ImGui::Button("Add Point"))autoroute.AddPoint();
	prevColor = ImGui::GetStyle().Colors[ImGuiCol_Text];
	ImGui::GetStyle().Colors[ImGuiCol_Text] = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
	if (ImGui::Button("Clear"))autoroute.ClearAllRoute();
	ImGui::GetStyle().Colors[ImGuiCol_Text] = prevColor;
	if (ImGui::Button("Save"))autoroute.SaveRoute();
	if (ImGui::Button("Load"))autoroute.LoadRoute("");
	HudKeyBind(cvar.route_rush_key, "Speed Key");
}

void MenuSnapshot()
{
	ImGui::Text("Clean Snapshot"), ImGui::Separator();

	ImGui::Checkbox("Save Snapshot To Memory", &cvar.snapshot_memory);
	ImGui::Text("Snapshot Time");
	SliderFloat("Snapshot Time##1", &cvar.snapshot_time, 1.f, 60.f, "%.0f sec");
}

void MenuSteam1()
{
	if (c_Offset.HLType != RENDERTYPE_UNDEFINED)
	{
		ImGui::Text("Steam ID"), ImGui::Separator();
		
		ImGui::Text("Steam ID Method");
		const char* listbox[] = { "Off", "Auto", "Manual" };
		ComboBox("Steam ID", &cvar.steamid, listbox, IM_ARRAYSIZE(listbox), 3);
	}
	else
		ImGui::Text("Steam ID Not Available"), ImGui::Separator();
}

void MenuSteam2()
{
	if (c_Offset.HLType != RENDERTYPE_UNDEFINED)
	{
		if (cvar.steamid == 2)
		{
			ImGui::Text("Steam ID Manual"), ImGui::Separator();

			int steamid[7] = { int(cvar.id1), int(cvar.id2), int(cvar.id3), int(cvar.id4), int(cvar.id5), int(cvar.id6), int(cvar.id7) };
			ImGui::PushItemWidth(-1);
			ImGui::DragInt7("##1", steamid, 1, 0, 35);
			ImGui::PopItemWidth();

			cvar.id1 = steamid[0];
			cvar.id2 = steamid[1];
			cvar.id3 = steamid[2];
			cvar.id4 = steamid[3];
			cvar.id5 = steamid[4];
			cvar.id6 = steamid[5];
			cvar.id7 = steamid[6];
			if (cvar.id1 > 35 || cvar.id1 < 0)cvar.id1 = 1;
			if (cvar.id2 > 35 || cvar.id2 < 0)cvar.id2 = 2;
			if (cvar.id3 > 35 || cvar.id3 < 0)cvar.id3 = 3;
			if (cvar.id4 > 35 || cvar.id4 < 0)cvar.id4 = 4;
			if (cvar.id5 > 35 || cvar.id5 < 0)cvar.id5 = 5;
			if (cvar.id6 > 35 || cvar.id6 < 0)cvar.id6 = 6;
			if (cvar.id7 > 35 || cvar.id7 < 0)cvar.id7 = 7;
		}
		else
			ImGui::Text("Steam ID Manual"), ImGui::Separator();
	}
	else
		ImGui::Text("Not Available"), ImGui::Separator();
}

void MenuSettings()
{
	ImGui::Text("Settings"), ImGui::Separator();

	if (ImGui::Button("Reload cvar.ini")) LoadCvar();
	if (ImGui::Button("Reload Menu Texture")) loadtexturemenu = true;
}

void MenuKey()
{
	ImGui::Text("Gui Keys"), ImGui::Separator();

	ImGui::Checkbox("Gui Chat", &cvar.gui_chat);
	HudKeyBind(cvar.gui_chat_key, "Gui Chat Key");
	HudKeyBind(cvar.gui_chat_key_team, "Chat Chat Key Team");
	ImGui::Checkbox("Quick Change", &cvar.misc_quick_change);
	HudKeyBind(cvar.misc_quick_change_key, "Quick Change Key");
}

int buttonanglesmenu[15] = { 180, 156, 132, 108, 84, 60, 36, 12, 348, 324, 300, 276, 252, 228, 204 };
int buttonanglesstatic[15] = { 180, 156, 132, 108, 84, 60, 36, 12, 348, 324, 300, 276, 252, 228, 204 };
int buttonrotate[15] = { 180, 156, 132, 108, 84, 60, 36, 12, 348, 324, 300, 276, 252, 228, 204 };
int checkangles[15] = { 180, 156, 132, 108, 84, 60, 36, 12, 348, 324, 300, 276, 252, 228, 204 };
int buttonxend[15] = { 80, 74, 68, 62, 56, 50, 44, 38, 38, 44, 50, 56, 62, 68, 74 };
int buttonyend[15] = { 80, 74, 68, 62, 56, 50, 44, 38, 38, 44, 50, 56, 62, 68, 74 };
int buttonxstart[15] = { 80, 74, 68, 62, 56, 50, 44, 38, 38, 44, 50, 56, 62, 68, 74 };
int buttonystart[15] = { 80, 74, 68, 62, 56, 50, 44, 38, 38, 44, 50, 56, 62, 68, 74 };
int direction;
bool changewindowfocus = true;
float GifDelay[2048];
float radiusx;
float radiusy;
DWORD TickcountMenu;

void LoadTextureImageMenu(const char* image, int index)
{
	char filename[256];
	int width, height;
	sprintf(filename, "%s%s", hackdir, image);
	GLint last_texture;
	glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture);
	if (texture_id[index])glDeleteTextures(1, &texture_id[index]);
	GLuint glindex;
	glGenTextures(1, &glindex);
	texture_id[index] = glindex + 20000;
	glBindTexture(GL_TEXTURE_2D, texture_id[index]);
	unsigned char* soilimage = SOIL_load_image(filename, &width, &height, 0, SOIL_LOAD_RGBA);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, soilimage);
	SOIL_free_image_data(soilimage);
	glBindTexture(GL_TEXTURE_2D, last_texture);
}

void GetTextureMenu(const char* itemname, int& index)
{
	int maxindex = index + 50;
	char filedir[256];
	sprintf(filedir, "%stexture/menu/%s\\*", hackdir, itemname);
	WIN32_FIND_DATA fd;
	HANDLE hFind = ::FindFirstFile(filedir, &fd);
	if (hFind != INVALID_HANDLE_VALUE)
	{
		do
		{
			if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				if (index < maxindex)
				{
					char filename[256];
					sprintf(filename, "texture/menu/%s/%s", itemname, fd.cFileName);
					LoadTextureImageMenu(filename, index);

					float delay;
					if (sscanf(fd.cFileName, "%*s %*d %*s %f", &delay))
						GifDelay[index] = delay;
					index++;
				}
			}
		} while (::FindNextFile(hFind, &fd));
		::FindClose(hFind);
	}
}

void DrawkeyBind(int buttonsize, bool reset)
{
	static float showspeed = 1;
	static float changein = showspeed;
	static float changeout; 
	static bool menu;
	static int mytick;
	if (reset)mytick = 0;
	static DWORD Tickcount;
	EaseMenu(showspeed, 1, 20, bShowMenu, changein, changeout, mytick, Tickcount);
	if (showspeed < 1)
	{
		int y = ImGui::GetIO().DisplaySize.y -
			radiusy * cos(M_PI * 2 * 0 / 360) - 
			buttonsize / 2;

		ImGui::SetNextWindowPos(ImVec2(0, y), ImGuiCond_Always, ImVec2(showspeed, 0.5));
		ImGui::SetNextWindowSize(ImVec2(0, 0));
		ImGui::Begin("keywindow", reinterpret_cast<bool*>(true), ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar);
		{
			HudMenuBind(cvar.gui_key);
		}
		ImGui::End();
	}
}

void DrawMenuChild(int total, bool reset)
{
	int maxheight = ImGui::GetIO().DisplaySize.y -
		total;

	static float showspeed = 1;
	static float changein = showspeed;
	static float changeout;
	static bool menu;
	static int mytick;
	if (reset)mytick = 0;
	static DWORD Tickcount;
	EaseMenu(showspeed, 1, 20, bShowMenu, changein, changeout, mytick, Tickcount);
	if (showspeed < 1)
	{
		if (!hidemenu)
		{
			float pos = 1;
			if (MenuTab == 4 || MenuTab == 11 || MenuTab == 13 || MenuTab == 14)
				pos = 0.5f;
			ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x / 2, 0), ImGuiCond_Always, ImVec2(pos, showspeed));
			ImGui::PushStyleVar(ImGuiStyleVar_WindowMaxSize, ImVec2(ImGui::GetIO().DisplaySize.x, maxheight));
			char str[256];
			sprintf(str, "child1%d", MenuTab);
			ImGui::Begin(str, reinterpret_cast<bool*>(true), ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar);
			{
				if (MenuTab == 0)
					MenuRage1();
				if (MenuTab == 1)
					MenuLegit1();
				if (MenuTab == 2)
					MenuModelAim1();
				if (MenuTab == 3)
					MenuIdHook1();
				if (MenuTab == 4)
					MenuSpawn();
				if (MenuTab == 5)
					MenuVisual1Window1();
				if (MenuTab == 6)
					MenuVisual2Window1();
				if (MenuTab == 7)
					MenuVisual3Window1();
				if (MenuTab == 8)
					MenuVisual4Window1();
				if (MenuTab == 9)
					MenuKz1();
				if (MenuTab == 10)
					MenuRoute1();
				if (MenuTab == 11)
					MenuSnapshot();
				if (MenuTab == 12)
					MenuSteam1();
				if (MenuTab == 13)
					MenuSettings();
				if (MenuTab == 14)
					MenuKey();
			}
			ImGui::End();
			ImGui::PopStyleVar();
		}

		ImVec2 windowsizes;
		if (MenuTab != 4 && MenuTab != 11 && MenuTab != 13 && MenuTab != 14 && !hidemenu)
		{
			float width = 0;
			if (MenuTab == 12)
				width = 200;
			ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x / 2, 0), ImGuiCond_Always, ImVec2(0, showspeed));
			ImGui::PushStyleVar(ImGuiStyleVar_WindowMaxSize, ImVec2(ImGui::GetIO().DisplaySize.x, maxheight));
			char str[256];
			sprintf(str, "child2%d", MenuTab);
			ImGui::Begin(str, reinterpret_cast<bool*>(true), ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar);
			{
				if (MenuTab == 0)
					MenuRage2();
				if (MenuTab == 1)
					MenuLegit2();
				if (MenuTab == 2)
					MenuModelAim2();
				if (MenuTab == 3)
					MenuIdHook2();
				if (MenuTab == 5)
					MenuVisual1Window2();
				if (MenuTab == 6)
					MenuVisual2Window2();
				if (MenuTab == 7)
					MenuVisual3Window2();
				if (MenuTab == 8)
					MenuVisual4Window2();
				if (MenuTab == 9)
					MenuKz2();
				if (MenuTab == 10)
					MenuRoute2();
				if (MenuTab == 12)
					MenuSteam2();
				windowsizes = ImGui::GetWindowSize();
			}
			ImGui::End();
			ImGui::PopStyleVar();
		}

		if (MenuTab == 1 && Model_Aim_Select.size() && !hidemenu)
		{
			ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x / 2 + windowsizes.x, 0), ImGuiCond_Always, ImVec2(0, showspeed));
			ImGui::PushStyleVar(ImGuiStyleVar_WindowMaxSize, ImVec2(ImGui::GetIO().DisplaySize.x, maxheight));
			char str[256];
			sprintf(str, "child3%d", MenuTab);
			ImGui::Begin(str, reinterpret_cast<bool*>(true), ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar);
			{
				MenuLegit3();
			}
			ImGui::End();
			ImGui::PopStyleVar();
		}

		if (cvar.model_preview && modelmenu && pmove->iuser1 != OBS_MAP_FREE && pmove->iuser1 != OBS_MAP_CHASE)
		{
			drawdummy = true;
			drawdummy2 = true;
			drawgetdummy = true;
			drawbackdummy = true;
			if (hidemenu)
				g_Engine.pfnGetCvarPointer("r_drawviewmodel")->value = 0;

			static bool fakemenu = false;
			ImVec2 windowpos;
			float WindowBorderSize = ImGui::GetStyle().WindowBorderSize;
			ImGui::GetStyle().WindowBorderSize = 1.0f;
			ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(150, 335));
			ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
			ImGui::SetNextWindowPos(ImVec2(model_pos_x, model_pos_y), ImGuiCond_Once);
			ImGui::SetNextWindowSize(ImVec2(modelscreenw, modelscreenh), ImGuiCond_Once);
			ImGui::Begin("esppreview", reinterpret_cast<bool*>(true), ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar);
			{
				ImGui::GetCurrentWindow()->DrawList->AddRectFilled({ ImGui::GetWindowPos().x, ImGui::GetWindowPos().y }, { ImGui::GetWindowPos().x + ImGui::GetWindowSize().x, ImGui::GetWindowPos().y + 86 }, Black());
				ImGui::Text("Esp Preview"), ImGui::Separator();
				if (ImGui::ArrowButton("##left", ImGuiDir_Left))
				{
					if (model_type > 0)
						model_type--;
					else
						model_type = PlayerModel.size() - 1;
				}
				ImGui::SameLine();
				if (ImGui::ArrowButton("##right", ImGuiDir_Right))
				{
					if (model_type < PlayerModel.size() - 1)
						model_type++;
					else
						model_type = 0;
				}
				ImGui::SameLine();
				if (PlayerModel.size() && model_type < PlayerModel.size() && model_type >= 0)
					ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "%s", PlayerModel[int(model_type)].name);

				if (ImGui::ArrowButton("##left2", ImGuiDir_Left))
				{
					if (espview > 0)
						espview--;
					else
						espview = 1;
				}
				ImGui::SameLine();
				if (ImGui::ArrowButton("##right2", ImGuiDir_Right))
				{
					if (espview < 1)
						espview++;
					else
						espview = 0;
				}
				ImGui::SameLine();
				if(!espview)
					ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "Front");
				else
					ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "Back");

				ImGui::Checkbox("Hide Menu", &hidemenu);
				ImGui::SameLine();
				ImGui::Checkbox("Fake Menu", &fakemenu);

				model_pos_x = IM_ROUND(ImGui::GetCursorScreenPos().x);
				model_pos_y = IM_ROUND(ImGui::GetCursorScreenPos().y);
				modelscreenw = IM_ROUND(ImGui::GetContentRegionAvail().x);
				modelscreenh = IM_ROUND(ImGui::GetContentRegionAvail().y);


				windowpos.x = ImGui::GetWindowPos().x + ImGui::GetWindowSize().x + 1;
				windowpos.y = ImGui::GetWindowPos().y;
			}
			ImGui::End();
			ImGui::PopStyleColor(); 
			ImGui::PopStyleVar(1);
			ImGui::GetStyle().WindowBorderSize = WindowBorderSize;

			if (fakemenu)
			{
				float WindowBorderSize = ImGui::GetStyle().WindowBorderSize;
				ImGui::GetStyle().WindowBorderSize = 1.0f;
				ImGui::SetNextWindowPos(ImVec2(windowpos.x, windowpos.y), ImGuiCond_Always);
				ImGui::Begin("esppreview2", reinterpret_cast<bool*>(true), ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize);
				{
					ImGui::Text("Fake Players"), ImGui::Separator();

					if (!playerdummy.curstate.entityType)
					{
						if (ImGui::Button("Player"))
						{
							hasowner = true;
							dummyheight = 0;
							dummywidth = 0;
							modelscale = 0.01f;
						}
						if (ImGui::IsItemHovered())
						{
							ImGui::BeginTooltip();
							ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
							ImGui::TextUnformatted("Caution! This will make any world entity with curstate.owner index matching player index into player effecting everything including aimbot");
							ImGui::PopTextWrapPos();
							ImGui::EndTooltip();
						}
						ImGui::SameLine();
					}
					if (playerdummy.curstate.entityType && !strstr(playerdummy.model->name, "/player/"))
					{
						if (ImGui::Button("World"))
						{
							for (int i = 0; i < FakePlayer.size(); i++)
							{
								if (!strcmp(FakePlayer[i].name, playerdummy.model->name))
									FakePlayer.erase(FakePlayer.begin() + i);
							}
							dummyheight = 0;
							dummywidth = 0;
							modelscale = 0.01f;
						}
						if (ImGui::IsItemHovered())
						{
							ImGui::BeginTooltip();
							ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
							ImGui::TextUnformatted("This will reset any entity selected as player");
							ImGui::PopTextWrapPos();
							ImGui::EndTooltip();
						}
						ImGui::SameLine();
					}

					if (ImGui::Button("Save"))
						SaveHitboxFake();
					ImGui::SameLine();
					if (ImGui::Button("Load"))
						LoadHitboxFake();
					ImGui::SameLine();
					ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
					if (ImGui::Button("Clear All"))
						FakePlayer.clear();
					ImGui::PopStyleColor();

					for (int i = 0; i < FakePlayer.size(); i++)
					{
						if(i == 0)ImGui::Separator();
						ImGui::Text("%d %s", i, FakePlayer[i].name);
					}
				}
				ImGui::End(); 
				ImGui::GetStyle().WindowBorderSize = WindowBorderSize;
			}
		}
	}
}

void ChangeAngle(int windowx, int windowy, int buttonx, int buttony, int& direction, int move, int* buttonangles, int angle)
{
	if (windowx == buttonx && windowy == buttony)
	{
		direction = move;
		for (unsigned int k = 0; k < 15; k++)
		{
			if(move == 1)
				buttonangles[k] += angle;
			if (move == 2)
				buttonangles[k] -= angle;
		}
	}
}

float xScreen(int angle)
{
	return ImGui::GetIO().DisplaySize.x / 2 - radiusx * sin(M_PI * 2 * angle / 360);
}

float yScreen(int angle, int sizebot)
{
	return ImGui::GetIO().DisplaySize.y -
		radiusy * cos(M_PI * 2 * angle / 360) -
		radiusy * cos(M_PI * 2 * 0 / 360) -
		sizebot / 2;
}

void Button(float posx, float posy, int buttonsize, int sizex, int sizey, int& textures, int texturestart, int index, bool* focused, int maxindex)
{
	if (focused[index] || index == MenuTab)
	{
		static DWORD Tickcount[2048];
		if (GetTickCount() - Tickcount[index] >= GifDelay[textures] * 1000)
		{
			if (textures < maxindex - 1)
				textures++;
			else
				textures = texturestart;
			Tickcount[index] = GetTickCount();
		}
	}
	else
		textures = texturestart;
	
	if (ImGui::ImageButtonID(index, (GLuint*)texture_id[textures], ImVec2(sizex, sizey)))
	{
		ChangeAngle(posx, posy, xScreen(buttonanglesstatic[1]), yScreen(buttonanglesstatic[1], buttonsize), direction, 1, buttonanglesmenu, 24);
		ChangeAngle(posx, posy, xScreen(buttonanglesstatic[2]), yScreen(buttonanglesstatic[2], buttonsize), direction, 1, buttonanglesmenu, 48);
		ChangeAngle(posx, posy, xScreen(buttonanglesstatic[3]), yScreen(buttonanglesstatic[3], buttonsize), direction, 1, buttonanglesmenu, 72);
		ChangeAngle(posx, posy, xScreen(buttonanglesstatic[4]), yScreen(buttonanglesstatic[4], buttonsize), direction, 1, buttonanglesmenu, 96);
		ChangeAngle(posx, posy, xScreen(buttonanglesstatic[5]), yScreen(buttonanglesstatic[5], buttonsize), direction, 1, buttonanglesmenu, 120);
		ChangeAngle(posx, posy, xScreen(buttonanglesstatic[6]), yScreen(buttonanglesstatic[6], buttonsize), direction, 1, buttonanglesmenu, 144);
		ChangeAngle(posx, posy, xScreen(buttonanglesstatic[7]), yScreen(buttonanglesstatic[7], buttonsize), direction, 1, buttonanglesmenu, 168);
		ChangeAngle(posx, posy, xScreen(buttonanglesstatic[8]), yScreen(buttonanglesstatic[8], buttonsize), direction, 2, buttonanglesmenu, 168);
		ChangeAngle(posx, posy, xScreen(buttonanglesstatic[9]), yScreen(buttonanglesstatic[9], buttonsize), direction, 2, buttonanglesmenu, 144);
		ChangeAngle(posx, posy, xScreen(buttonanglesstatic[10]), yScreen(buttonanglesstatic[10], buttonsize), direction, 2, buttonanglesmenu, 120);
		ChangeAngle(posx, posy, xScreen(buttonanglesstatic[11]), yScreen(buttonanglesstatic[11], buttonsize), direction, 2, buttonanglesmenu, 96);
		ChangeAngle(posx, posy, xScreen(buttonanglesstatic[12]), yScreen(buttonanglesstatic[12], buttonsize), direction, 2, buttonanglesmenu, 72);
		ChangeAngle(posx, posy, xScreen(buttonanglesstatic[13]), yScreen(buttonanglesstatic[13], buttonsize), direction, 2, buttonanglesmenu, 48);
		ChangeAngle(posx, posy, xScreen(buttonanglesstatic[14]), yScreen(buttonanglesstatic[14], buttonsize), direction, 2, buttonanglesmenu, 24);
	}
}

int ragemaxindex;
int legitmaxindex;
int modelmaxindex;
int playerlistmaxindex;
int spawnmaxindex;
int visual1maxindex;
int visual2maxindex;
int visual3maxindex;
int visual4maxindex;
int kzmaxindex;
int routemaxindex;
int snapshotmaxindex;
int steammaxindex;
int settingsmaxindex;
int keymaxindex;

int rageindex = 1050;
int legitindex = 1100;
int modelindex = 1150;
int playerlistindex = 1200;
int spawnindex = 1250;
int visual1index = 1300;
int visual2index = 1350;
int visual3index = 1400;
int visual4index = 1450;
int kzindex = 1500;
int routeindex = 1550;
int snapshotindex = 1600;
int steamindex = 1650;
int settingsindex = 1750;
int keyindex = 1700;

void DrawMenuButton(int posx, int posy, int sizex, int sizey, int index, int buttonsize)
{
	int paddingx = sizex / 20, paddingy = sizey / 20;
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(paddingx, paddingy));
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
	ImGui::SetNextWindowPos(ImVec2(posx, posy), ImGuiCond_Always, ImVec2(0.5f, 0.5f));
	ImGui::SetNextWindowSize(ImVec2(sizex + paddingx * 2, sizey + paddingy * 2));
	static bool focused[15];
	if (focused[index])
		ImGui::SetNextWindowFocus(); 
	char str[10];
	sprintf(str, "menu%d", index);

	ImGui::Begin(str, reinterpret_cast<bool*>(true), ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar);
	{
		if (index == 0)Button(posx, posy, buttonsize, sizex, sizey, rageindex, 1050, index, focused, ragemaxindex);
		if (index == 1)Button(posx, posy, buttonsize, sizex, sizey, legitindex, 1100, index, focused, legitmaxindex);
		if (index == 2)Button(posx, posy, buttonsize, sizex, sizey, modelindex, 1150, index, focused, modelmaxindex);
		if (index == 3)Button(posx, posy, buttonsize, sizex, sizey, playerlistindex, 1200, index, focused, playerlistmaxindex);
		if (index == 4)Button(posx, posy, buttonsize, sizex, sizey, spawnindex, 1250, index, focused, spawnmaxindex);
		if (index == 5)Button(posx, posy, buttonsize, sizex, sizey, visual1index, 1300, index, focused, visual1maxindex);
		if (index == 6)Button(posx, posy, buttonsize, sizex, sizey, visual2index, 1350, index, focused, visual2maxindex);
		if (index == 7)Button(posx, posy, buttonsize, sizex, sizey, visual3index, 1400, index, focused, visual3maxindex);
		if (index == 8)Button(posx, posy, buttonsize, sizex, sizey, visual4index, 1450, index, focused, visual4maxindex);
		if (index == 9)Button(posx, posy, buttonsize, sizex, sizey, kzindex, 1500, index, focused, kzmaxindex);
		if (index == 10)Button(posx, posy, buttonsize, sizex, sizey, routeindex, 1550, index, focused, routemaxindex);
		if (index == 11)Button(posx, posy, buttonsize, sizex, sizey, snapshotindex, 1600, index, focused, snapshotmaxindex);
		if (index == 12)Button(posx, posy, buttonsize, sizex, sizey, steamindex, 1650, index, focused, steammaxindex);
		if (index == 13)Button(posx, posy, buttonsize, sizex, sizey, settingsindex, 1700, index, focused, settingsmaxindex);
		if (index == 14)Button(posx, posy, buttonsize, sizex, sizey, keyindex, 1750, index, focused, keymaxindex);

		if (ImGui::IsItemHovered() && !focused[index])
			focused[index] = true;
		if (!ImGui::IsItemHovered() && focused[index])
		{
			changewindowfocus = true;
			focused[index] = false;
		}
	}
	ImGui::End();
	ImGui::PopStyleColor();
	ImGui::PopStyleVar(2);

	if (focused[index])
	{
		const char* tip[] = { "Rage", "Legit", "Model", "Player List", "Spawn", "Visual1", "Visual2", "Visual3", "Visual4", "KZ", "Route", "Snapshot", "Steam ID", "Settings", "Keys" };
		ImGui::SetNextWindowFocus(); 
		ImGui::SetNextWindowPos(ImVec2(posx, posy - sizey / 2 - paddingy), ImGuiCond_Always, ImVec2(0.5f, 1.0f));
		ImGui::SetNextWindowSize(ImVec2(0, 0));
		ImGui::Begin("tooltip", reinterpret_cast<bool*>(true), ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoInputs);
		{
			ImGui::Text(tip[index]);
		}
		ImGui::End();
	}
}

void GetTextureMenu()
{
	ragemaxindex = 1050;
	legitmaxindex = 1100;
	modelmaxindex = 1150;
	playerlistmaxindex = 1200;
	spawnmaxindex = 1250;
	visual1maxindex = 1300;
	visual2maxindex = 1350;
	visual3maxindex = 1400;
	visual4maxindex = 1450;
	kzmaxindex = 1500;
	routemaxindex = 1550;
	snapshotmaxindex = 1600;
	steammaxindex = 1650;
	settingsmaxindex = 1700;
	keymaxindex = 1750;
	GetTextureMenu("rage", ragemaxindex);
	GetTextureMenu("legit", legitmaxindex);
	GetTextureMenu("model", modelmaxindex);
	GetTextureMenu("playerlist", playerlistmaxindex);
	GetTextureMenu("spawn", spawnmaxindex);
	GetTextureMenu("visual1", visual1maxindex);
	GetTextureMenu("visual2", visual2maxindex);
	GetTextureMenu("visual3", visual3maxindex);
	GetTextureMenu("visual4", visual4maxindex);
	GetTextureMenu("kz", kzmaxindex);
	GetTextureMenu("route", routemaxindex);
	GetTextureMenu("snapshot", snapshotmaxindex);
	GetTextureMenu("steam", steammaxindex);
	GetTextureMenu("settings", settingsmaxindex);
	GetTextureMenu("key", keymaxindex);
	LoadTextureImageMenu("texture/menu/vip/vip.png", VIP);
	LoadTextureImageMenu("texture/radar/redarrow.png", REDARROW);
	LoadTextureImageMenu("texture/radar/bluearrow.png", BLUEARROW);
	LoadTextureImageMenu("texture/radar/redsound.png", REDSOUND);
	LoadTextureImageMenu("texture/radar/bluesound.png", BLUESOUND);
	LoadTextureImageMenu("texture/radar/greensound.png", GREENSOUND);
}

bool Checkpos(int windowx, int windowy, int buttonx, int buttony, char* windowname, int buttonindex, int& buttonsizex, int& buttonsizey, float buttonsize)
{
	if (windowx == buttonx && windowy == buttony)
	{
		sprintf(windowname, "menu%d", buttonindex);
		buttonsizex = buttonsize * ImGui::GetIO().DisplaySize.x / 1024;
		buttonsizey = buttonsize * ImGui::GetIO().DisplaySize.y / 768;
		return true;
	}
	return false;
}

void CalcButtonSize(int& sizetop, int& sizebot, int buttonsizetop, int buttonsizebot)
{
	for (unsigned int i = 0; i < 2; i++)
	{
		if (sizetop < buttonsizetop + buttonsizetop / 20 * 2)
			sizetop += 1;
		if (sizetop > buttonsizetop + buttonsizetop / 20 * 2)
			sizetop -= 1;

		if (sizebot < buttonsizebot + buttonsizebot / 20 * 2)
			sizebot += 1;
		if (sizebot > buttonsizebot + buttonsizebot / 20 * 2)
			sizebot -= 1;
	}
}

void CheckPosition(int sizebot, char windowname[15][10])
{
	for (unsigned int i = 0; i < 15; i++)
	{
		if (Checkpos(xScreen(checkangles[i]), yScreen(checkangles[i], sizebot), xScreen(buttonanglesstatic[0]), yScreen(buttonanglesstatic[0], sizebot), windowname[14], i, buttonxstart[i], buttonystart[i], 80) && MenuTab != i) MenuTab = i;
		Checkpos(xScreen(checkangles[i]), yScreen(checkangles[i], sizebot), xScreen(buttonanglesstatic[1]), yScreen(buttonanglesstatic[1], sizebot), windowname[12], i, buttonxstart[i], buttonystart[i], 74);
		Checkpos(xScreen(checkangles[i]), yScreen(checkangles[i], sizebot), xScreen(buttonanglesstatic[2]), yScreen(buttonanglesstatic[2], sizebot), windowname[10], i, buttonxstart[i], buttonystart[i], 68);
		Checkpos(xScreen(checkangles[i]), yScreen(checkangles[i], sizebot), xScreen(buttonanglesstatic[3]), yScreen(buttonanglesstatic[3], sizebot), windowname[8], i, buttonxstart[i], buttonystart[i], 62);
		Checkpos(xScreen(checkangles[i]), yScreen(checkangles[i], sizebot), xScreen(buttonanglesstatic[4]), yScreen(buttonanglesstatic[4], sizebot), windowname[6], i, buttonxstart[i], buttonystart[i], 56);
		Checkpos(xScreen(checkangles[i]), yScreen(checkangles[i], sizebot), xScreen(buttonanglesstatic[5]), yScreen(buttonanglesstatic[5], sizebot), windowname[4], i, buttonxstart[i], buttonystart[i], 50);
		Checkpos(xScreen(checkangles[i]), yScreen(checkangles[i], sizebot), xScreen(buttonanglesstatic[6]), yScreen(buttonanglesstatic[6], sizebot), windowname[2], i, buttonxstart[i], buttonystart[i], 44);
		Checkpos(xScreen(checkangles[i]), yScreen(checkangles[i], sizebot), xScreen(buttonanglesstatic[7]), yScreen(buttonanglesstatic[7], sizebot), windowname[0], i, buttonxstart[i], buttonystart[i], 38);
		Checkpos(xScreen(checkangles[i]), yScreen(checkangles[i], sizebot), xScreen(buttonanglesstatic[8]), yScreen(buttonanglesstatic[8], sizebot), windowname[1], i, buttonxstart[i], buttonystart[i], 38);
		Checkpos(xScreen(checkangles[i]), yScreen(checkangles[i], sizebot), xScreen(buttonanglesstatic[9]), yScreen(buttonanglesstatic[9], sizebot), windowname[3], i, buttonxstart[i], buttonystart[i], 44);
		Checkpos(xScreen(checkangles[i]), yScreen(checkangles[i], sizebot), xScreen(buttonanglesstatic[10]), yScreen(buttonanglesstatic[10], sizebot), windowname[5], i, buttonxstart[i], buttonystart[i], 50);
		Checkpos(xScreen(checkangles[i]), yScreen(checkangles[i], sizebot), xScreen(buttonanglesstatic[11]), yScreen(buttonanglesstatic[11], sizebot), windowname[7], i, buttonxstart[i], buttonystart[i], 56);
		Checkpos(xScreen(checkangles[i]), yScreen(checkangles[i], sizebot), xScreen(buttonanglesstatic[12]), yScreen(buttonanglesstatic[12], sizebot), windowname[9], i, buttonxstart[i], buttonystart[i], 62);
		Checkpos(xScreen(checkangles[i]), yScreen(checkangles[i], sizebot), xScreen(buttonanglesstatic[13]), yScreen(buttonanglesstatic[13], sizebot), windowname[11], i, buttonxstart[i], buttonystart[i], 68);
		Checkpos(xScreen(checkangles[i]), yScreen(checkangles[i], sizebot), xScreen(buttonanglesstatic[14]), yScreen(buttonanglesstatic[14], sizebot), windowname[13], i, buttonxstart[i], buttonystart[i], 74);
	}
}

void RotateMenu()
{
	if (GetTickCount() - TickcountMenu > 0)
	{
		for (unsigned int i = 0; i < 15; i++)
		{
			for (unsigned int k = 0; k < 10; k++)
			{
				if (buttonrotate[i] < buttonanglesmenu[i])
					buttonrotate[i] += 1;

				if (buttonrotate[i] > buttonanglesmenu[i])
					buttonrotate[i] -= 1;
			}
			for (unsigned int x = 0; x < 3; x++)
			{
				if (buttonxend[i] < buttonxstart[i])
					buttonxend[i] += 2;
				if (buttonxend[i] > buttonxstart[i])
					buttonxend[i] -= 2;

				if (buttonyend[i] < buttonystart[i])
					buttonyend[i] += 2;
				if (buttonyend[i] > buttonystart[i])
					buttonyend[i] -= 2;
			}
		}
		TickcountMenu = GetTickCount();
	}

	for (unsigned int i = 0; i < 15; i++)
	{
		if (direction == 1)
		{
			if (checkangles[i] > buttonrotate[i])
			{
				changewindowfocus = true;
				checkangles[i] -= 24;
			}
			if (checkangles[i] < buttonrotate[i])
			{
				changewindowfocus = true;
				checkangles[i] += 24;
			}
		}
		if (direction == 2)
		{
			if (checkangles[i] < buttonrotate[i])
			{
				changewindowfocus = true;
				checkangles[i] += 24;
			}
			if (checkangles[i] > buttonrotate[i])
			{
				changewindowfocus = true;
				checkangles[i] -= 24;
			}
		}
	}
}

int TopButton(int sizebot, int sizetop)
{
	return ImGui::GetIO().DisplaySize.y -
		radiusy * cos(M_PI * 2 * 12 / 360) -
		radiusy * cos(M_PI * 2 * 0 / 360) -
		sizebot / 2 -
		sizetop / 2;
}

int BotButton()
{
	return ImGui::GetIO().DisplaySize.y;
}

void DrawMenuWindow()
{
	modelmenu = false;
	if (loadtexturemenu)
		GetTextureMenu(), loadtexturemenu = false;

	static bool checkhide = hidemenu;
	if (checkhide != hidemenu)
	{
		if (!hidemenu)
			changewindowfocus = true;
		checkhide = hidemenu;
	}

	char windowname[15][10];

	radiusx = ImGui::GetIO().DisplaySize.x / 4;
	radiusy = ImGui::GetIO().DisplaySize.y / 15;
	
	int buttonsizetop = 38 * ImGui::GetIO().DisplaySize.y / 768;
	int buttonsizebot = 80 * ImGui::GetIO().DisplaySize.y / 768;
	static int sizetop = buttonsizetop;
	static int sizebot = buttonsizetop;
	CalcButtonSize(sizetop, sizebot, buttonsizetop, buttonsizebot);
	CheckPosition(sizebot, windowname);
	RotateMenu();
	
	bool reset = false;
	static bool menu;
	if (menu != bShowMenu)
	{
		reset = true;
		menu = bShowMenu;
	}

	int total = BotButton() - TopButton(sizebot, sizetop);
	static float showspeed = total;
	static float changein = showspeed;
	static float changeout;
	static int mytick;
	if (reset)mytick = 0;
	static DWORD Tickcount;
	EaseMenu(showspeed, total, 20, bShowMenu, changein, changeout, mytick, Tickcount);
	if (showspeed < total && !hidemenu)
	{
		for (unsigned int i = 0; i < 15; i++)
		{
			int x = xScreen(buttonrotate[i]);
			int y = yScreen(buttonrotate[i], sizebot);
			DrawMenuButton(x, y + showspeed, buttonxend[i], buttonyend[i], i, sizebot);
		}
	}
	if (!showspeed)
		modelmenu = true;
	DrawMenuChild(total, reset);
	DrawkeyBind(sizebot, reset);
	DrawChatInputWindow(sizebot);
	if (changewindowfocus && bShowMenu && showspeed < total)
	{
		for (unsigned int i = 0; i < 15; i++)
			ImGui::SetWindowFocus(windowname[i]);
		ImGui::SetWindowFocus("keywindow");
		changewindowfocus = false;
	}
}