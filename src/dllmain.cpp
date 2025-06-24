#include <stdinc.hpp>
#include "loader/component_loader.hpp"

#include <utils/hook.hpp>
#include <utils/binary_resource.hpp>
#include <utils/nt.hpp>

namespace
{
	utils::nt::library load_d3d9_library()
	{
		char dir[MAX_PATH]{};
		GetSystemDirectoryA(dir, sizeof(dir));
		return utils::nt::library::load(dir + "/d3d9.dll"s);
	}

	utils::nt::library& get_d3d9_library()
	{
		static auto lib = load_d3d9_library();
		return lib;
	}
}

FARPROC WINAPI get_proc_address(const HMODULE module, const LPCSTR lp_proc_name)
{
	if (lp_proc_name == "Direct3DCreate9"s || lp_proc_name == "Direct3DCreate9Ex"s)
	{
		return get_d3d9_library().get_proc<FARPROC>(lp_proc_name);
	}

	return GetProcAddress(module, lp_proc_name);
}

BOOL APIENTRY DllMain(HMODULE module, DWORD ul_reason_for_call, LPVOID /*reserved*/)
{
	utils::hook::nop(0x694DE1, 6);
	utils::hook::call(0x694DE1, get_proc_address);

	if (ul_reason_for_call == DLL_PROCESS_ATTACH)
	{
		component_loader::on_startup();
		utils::nt::library::set_current_handle(module);
	}

	if (ul_reason_for_call == DLL_PROCESS_DETACH)
	{
		component_loader::on_shutdown();
	}

	return TRUE;
}
