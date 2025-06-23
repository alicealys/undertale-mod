#include <stdinc.hpp>
#include "console.hpp"
#include "loader/component_loader.hpp"

#include <utils/thread.hpp>
#include <utils/hook.hpp>

namespace console
{
	namespace
	{
		void create_console()
		{
			AllocConsole();
			FILE* handle{};
			freopen_s(&handle, "CONOUT$", "w", stdout);
		}
	}

	class component final : public component_interface
	{
	public:
		void on_startup() override
		{
			//create_console();
			//ShowWindow(GetConsoleWindow(), SW_SHOW);
		}

		void on_shutdown() override
		{

		}
	};
}

REGISTER_COMPONENT(console::component)
