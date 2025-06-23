#include <stdinc.hpp>
#include "loader/component_loader.hpp"

#include <utils/hook.hpp>
#include <utils/binary_resource.hpp>
#include <utils/nt.hpp>

BOOL APIENTRY DllMain(HMODULE module, DWORD ul_reason_for_call, LPVOID /*reserved*/)
{
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
