﻿#include"pch.h"
#include<bdxlua.h>
#include<lua.hpp>
#include<luafly.h>
#include"homeStorage.h"
int lb_getHomes(lua_State* L) {
	try {
		LuaFly lf{ L };
		string x;
		lf.pop(x);
		xuid_t xid=XIDREG::str2id(x).val();
		auto& homes=getHomeInCache(xid);
		vector<string_view> homenames;
		for (auto& i : homes.data) {
			homenames.emplace_back(i.name);
		}
		lf.push(homenames);
		return 1;
	}CATCH()
}
void InitLUAAPI() {
	lua_register(L, "getHomes", lb_getHomes);
}