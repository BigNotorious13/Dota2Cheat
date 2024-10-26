#pragma once

namespace Hooks {
	struct HookData {
		std::string name;
		void* target;
		bool enabled = true;

		bool SetEnabled(bool val);
	};

	inline std::vector<HookData> installed;

	bool UnhookAll();
	bool Unhook(void* target);
	bool Hook(void* target, void* detour, void* original, const char* name);;

	void InstallHooks();

	void InstallAuxiliaryHooks();

	// Removes any custom, non-MinHook hooks
	void RemoveAuxiliaryHooks();

	// Reloads every hook
	inline void Reload() {
		UnhookAll();
		InstallHooks();
	}
}

#define HOOKFUNC(func) Hooks::Hook(func, &hk##func, &o##func, #func)
#define ORIGCALL(func) ((decltype(&hk##func))(o##func))