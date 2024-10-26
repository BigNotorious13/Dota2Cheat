#pragma once
#include "../Base/Definitions.h"
#include "../Base/VClass.h"
#include "../Base/Vector.h"
#include "../Entities/CBaseEntity.h"

class CDOTAInput : public VClass {
public:
	GETTER(Vector, GetTracedPosition, 0x5d4);
	GETTER(CHandle<CBaseEntity>, GetTracedEntity, 0x5e8);
	
	static CDOTAInput* Get();
};