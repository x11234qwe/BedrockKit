﻿#include"pch.h"
#include"bdxlua.h"
bool LUA_SHELL_ENABLED;
string LAST_INPUT_BUFFER;
void ExecuteLuaInBuffer() {
	lua_getglobal(L, "EXCEPTION");
	auto EHIDX = lua_gettop(L);
	luaL_loadstring(L, LAST_INPUT_BUFFER.c_str());
	if (lua_pcall(L, 0, -1, EHIDX) != 0) {
		auto str = lua_tostring(L, -1);
		printf("[LUA] error: %s\n", str);
		lua_settop(L, EHIDX - 1);
		return;
	}
	lua_settop(L, EHIDX - 1);
}
THook(int, "??$try_dequeue@V?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@@?$SPSCQueue@V?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@$0CAA@@@QEAA_NAEAV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@@Z", void* que, string& content) {
	//onConsoleInput
	int HaveInput = original(que, content)&1;
	if (HaveInput) {
		if (content == "!lua") {
			LUA_SHELL_ENABLED = true;
			printf("lua shell enabled\n");
			content.clear();
			return 0;
		}
		if (LUA_SHELL_ENABLED) {
			if (content.size() == 0) {
				//EOF,execute it
				ExecuteLuaInBuffer();
				LAST_INPUT_BUFFER.clear();
			}
			else {
				if (content == "bye") {
					LUA_SHELL_ENABLED = false;
					LAST_INPUT_BUFFER.clear();
					content.clear();
					printf("exited lua shell\n");
					return 0;
				}
				LAST_INPUT_BUFFER.push_back('\n');
				LAST_INPUT_BUFFER.append(content);
			}
			content.clear();
			return 0;
		}
	}
	return HaveInput;
}