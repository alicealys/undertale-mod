#include <stdinc.hpp>
#include "loader/component_loader.hpp"

#include <utils/hook.hpp>
#include <utils/string.hpp>

namespace patches
{
	namespace
	{
		utils::hook::detour wnd_proc_hook;

		struct key_rebind_t
		{
			std::uint32_t native;
			std::uint32_t custom;
		};

		std::unordered_map<std::uint32_t, std::uint32_t> binds;		// native to custom
		std::unordered_map<std::uint32_t, std::uint32_t> binds_rev; // custom to native

		LRESULT __stdcall wnd_proc_stub(HWND hwnd, UINT msg, WPARAM w_param, LPARAM l_param)
		{
			if (msg == WM_KEYDOWN || msg == WM_KEYUP)
			{
				const auto iter = binds_rev.find(w_param);
				if (iter != binds_rev.end())
				{
					w_param = iter->second;
				}
			}

			return wnd_proc_hook.invoke_pascal<LRESULT>(hwnd, msg, w_param, l_param);
		}

		SHORT __stdcall get_async_get_state(const std::uint32_t key)
		{
			auto result = GetAsyncKeyState(key);

			const auto iter = binds.find(key);
			if (iter != binds.end())
			{
				result |= GetAsyncKeyState(iter->second);
			}

			return result;
		}

		void add_keybind(const std::uint32_t native, const std::uint32_t custom)
		{
			binds.insert(std::make_pair(native, custom));
			binds_rev.insert(std::make_pair(custom, native));
		}
	}

	class component final : public component_interface
	{
	public:
		void on_startup() override
		{
			patch();
		}

		void patch()
		{
			add_keybind(VK_UP, 'W');
			add_keybind(VK_DOWN, 'S');
			add_keybind(VK_LEFT, 'A');
			add_keybind(VK_RIGHT, 'D');

			utils::hook::nop(0x52970C, 6);
			utils::hook::call(0x52970C, get_async_get_state);
			wnd_proc_hook.create(0x413D80, wnd_proc_stub);
		}
	};
}

REGISTER_COMPONENT(patches::component)
