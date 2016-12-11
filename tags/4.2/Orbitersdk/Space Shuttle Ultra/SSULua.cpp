#include "Atlantis.h"
#include <discsignals.h>

extern "C" {
#include "lua\lua.h"
#include "lua\lualib.h"
#include "lua\lauxlib.h"
}

// ==========================================================================
// API function prototypes

Atlantis *lua_toSSU (lua_State *L, int idx = 1);
int ssuGear (lua_State *L);
int ssuODSRing (lua_State *L);
int ssuReadDiscrete (lua_State *L);
int ssuListBundles (lua_State *L);

// ==========================================================================
// API initialisation

int Atlantis::Lua_InitInterpreter (void *context)
{
	lua_State *L = (lua_State*)context;

	// load atmospheric autopilot
	//luaL_dofile (L, "Script\\dg\\aap.lua");

	return 0;
}

int Atlantis::Lua_InitInstance (void *context)
{
	lua_State *L = (lua_State*)context;

	// check if interpreter has DG table loaded already
	luaL_getmetatable (L, "VESSEL.SSU");

	if (lua_isnil (L, -1)) { // register new functions
		lua_pop (L, 1);
		static const struct luaL_reg ssuLib[] = {
			{"gear", ssuGear},
			{"ODS_ring", ssuODSRing},
			{"read_discrete", ssuReadDiscrete},
			{"list_bundles", ssuListBundles},
			{NULL, NULL}
		};

		// create metatable for vessel userdata
		luaL_newmetatable (L, "SSU.vtable");

		// create a table for the overloaded methods
		luaL_openlib (L, "SSU.method", ssuLib, 0);

		// create metatable for accessing inherited methods from VESSEL
		luaL_newmetatable (L, "SSU.base");
		lua_pushstring (L, "__index");
		luaL_getmetatable (L, "VESSEL.vtable");
		lua_settable (L, -3);

		// set SSU.base as metatable for SSU.method
		lua_setmetatable (L, -2);

		// point vessel userdata to SSU.method
		lua_pushstring (L, "__index");
		lua_pushvalue (L, -2); // push SSU.method
		lua_settable (L, -4);

		// pop SSU.method from the stack
		lua_pop (L, 1);
	}

	lua_setmetatable (L, -2);

	return 0;
}

// ==========================================================================
// Script API functions

Atlantis* lua_toSSU (lua_State *L, int idx)
{
	VESSEL **pv = (VESSEL**)lua_touserdata (L, idx);
	Atlantis *ssu = (Atlantis*)*pv;
	return ssu;
}

/*
static DeltaGlider::DoorStatus DGaction[4] = {
	DeltaGlider::DOOR_CLOSING,
	DeltaGlider::DOOR_OPENING,
	DeltaGlider::DOOR_CLOSING,
	DeltaGlider::DOOR_OPENING
}; */

static int ssuGear (lua_State *L)
{
	Atlantis *sts = lua_toSSU (L, 1);
	int action = lua_tointeger (L, 2);
	return 0;
}

static int ssuODSRing (lua_State *L)
{
	Atlantis *sts = lua_toSSU (L, 1);
	double pos = lua_tonumber (L, 2);
	return 0;
}

static int ssuReadDiscrete (lua_State *L)
{
	int args = lua_gettop(L) - 2;
	if(args < 0)
		return 0;

	Atlantis *sts = lua_toSSU (L, 1);
	const char* bundle = lua_tostring (L, 2);
	
	discsignals::DiscreteBundle* db = sts->BundleManager()->FindBundle(bundle);
	if(args == 0)	//If no additional arguments given, return all discretes of a bundle
	{
		args = db->CountLines();
	}

	for(int i = 0; i<args; i++) 
	{
		lua_pushnumber(L, db->GetVoltage(i));
	}

	return args;
}

static int ssuListBundles (lua_State *L)
{
	Atlantis *sts = lua_toSSU (L, 1);
	
	if(sts) {
		lua_newtable(L);
		unsigned int n = sts->BundleManager()->CountBundles();
		for(unsigned int i = 0; i<n; i++) {
			lua_pushstring(L, sts->BundleManager()->GetBundleByIndex(i)->GetIdentity().c_str());
			lua_rawseti(L, -2, i+1);
		}
		return 1;
	} else
		return 0;
}
