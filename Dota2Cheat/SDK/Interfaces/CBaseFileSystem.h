#pragma once
#include "../Base/VClass.h"

typedef void* CFileHandle;

// filesystem_stdio.dll
// Most of these functions are directly xref'd like "CBaseFileSystem::Read"
// You can find them in TF2 sources to see exact implementations
class CBaseFileSystem : public VClass
{
public:
	/// <summary>
	/// Opens a VPK resource file
	/// </summary>
	/// <param name="filePath">resource path in "scripts/npc/npc_heroes.txt" format</param>
	/// <param name="options">C-style read-write permissions like "r"/"w+" etc</param>
	/// <param name="unk">I don't exactly know what that is, doesn't get checked if reading</param>
	/// <param name="pathId">Narrows the scope for search or something, LWSS knows better</param>
	/// <returns>CFileHandle pointer if successful</returns>
	CFileHandle OpenFile(const char* filePath, const char* options, int flags = 0, void* pathId = nullptr) {
		return CallVFunc<78>(filePath, options, flags, pathId);
	}
	
	char* Read(char* buffer, int size, int capacity, CFileHandle handle) {
		return CallVFunc<79, char*>(buffer, size, capacity, handle);
	}

	uint32_t Size(CFileHandle handle) {
		return CallVFunc<18, uint32_t>(handle);
	}

	uint32_t Close(CFileHandle handle) {
		return CallVFunc<14, uint32_t>(handle);
	}

	// Reads a line from a CFileHandle to a buffer. Returns the buffer if successful
	char* ReadLine(char* buffer, int size, CFileHandle handle) {
		return CallVFunc<56, char*>(buffer, size, handle);
	}

	enum SearchPathAdd_t : uint32_t {
		PATH_ADD_TO_HEAD, // First path searched
		PATH_ADD_TO_TAIL, // Last path searched
	};

	using SearchPathPriority_t = uint32_t;

	void AddSearchPath(const char* path, const char* scope, SearchPathAdd_t order, SearchPathPriority_t priority = 0) {
		CallVFunc<31>(path, scope, order, priority);
	}

	static CBaseFileSystem* Get();
};