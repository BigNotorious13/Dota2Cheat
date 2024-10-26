#pragma once
#include <string_view>
#include <Windows.h>

template<typename Ret, typename...Args>
struct ExportFunction;

template<typename Ret, typename... Args>
struct ExportFunction<Ret(Args...)> {
	std::string_view dll;
	std::string_view name;
	Ret(*func)(Args...) = nullptr;

	ExportFunction(std::string_view dll, std::string_view name) : dll(dll), name(name) {}

	Ret Call(Args... args) {
		if (!func)
			func = (Ret(*)(Args...))GetProcAddress(GetModuleHandleA(dll.data()), name.data());

		return func(args...);
	}

	Ret operator()(Args... args) {
		return Call(args...);
	}
};

template<typename Ret, typename... Args>
struct ExportFunction<Ret(Args..., ...)> {
	std::string_view dll;
	std::string_view name;
	Ret(*func)(Args..., ...) = nullptr;

	ExportFunction(std::string_view dll, std::string_view name) : dll(dll), name(name) {}

	template<typename... VarArgs>
	Ret Call(Args... args, VarArgs... vargs) {
		if (!func)
			func = reinterpret_cast<decltype(func)>(GetProcAddress(GetModuleHandleA(dll.data()), name.data()));

		return func(args..., vargs...);
	}

	template<typename... VarArgs>
	Ret operator()(Args... args, VarArgs... vargs) {
		return Call(args..., vargs...);
	}
};

inline ExportFunction<double()> Plat_FloatTime("tier0.dll", "Plat_FloatTime");
inline ExportFunction<void(const char* format, ...)> Msg("tier0.dll", "Msg");
inline ExportFunction<void(const Color& color, const char* format, ...)> ConColorMsg("tier0.dll", "?ConColorMsg@@YAXAEBVColor@@PEBDZZ");