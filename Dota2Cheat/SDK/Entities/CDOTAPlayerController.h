#pragma once
#include "../Base/CUtlVector.h"
#include "../Enums.h"
#include <dota_commonmessages.pb.h>

#include "CDOTABaseNPCHero.h"

// client.dll!CUnitOrders
struct CUnitOrders {
	CUtlVector< entidx_t > m_nUnits;// 0x0;
	Vector m_vPosition; // 0x18;
	uint32_t m_nIssuerPlayerIndex; // 0x24;
	int32_t m_nOrderSequenceNumber; // 0x28;
	dotaunitorder_t m_nOrderType; // 0x2c;
	entidx_t m_nTargetIndex; // 0x30;
	entidx_t m_nAbilityIndex; // 0x34;
	uint32_t m_nFlags;  // 0x38;
};

static_assert(sizeof(CUnitOrders) == 0x40);

struct PlayerOrder {
	dotaunitorder_t Type{};
	uint32_t
		TargetIndex{},
		AbilityIndex{};
	Vector Position{};
	bool
		Queue = false,
		ShowEffects = true;
	PlayerOrderIssuer_t IssuerType = DOTA_ORDER_ISSUER_PASSED_UNIT_ONLY;
	CBaseEntity* Issuer{};

#define BUILDER_SETTER(var) \
	PlayerOrder& Set##var(decltype(var) var){ \
		this->var = var; return *this; \
	}

	BUILDER_SETTER(Type);
	BUILDER_SETTER(TargetIndex);
	BUILDER_SETTER(AbilityIndex);
	BUILDER_SETTER(Position);
	BUILDER_SETTER(Queue);
	BUILDER_SETTER(ShowEffects);
	BUILDER_SETTER(Issuer);
	BUILDER_SETTER(IssuerType);

#undef BUILDER_SETTER

	CUnitOrders ToUnitOrders() const;
};

class CDOTAPlayerController : public CBaseEntity {
public:
	GETTER(CHandle<CDOTABaseNPC_Hero>, GetAssignedHeroHandle, Netvars::C_DOTAPlayerController::m_hAssignedHero);

	auto GetAssignedHero() const {
		return GetAssignedHeroHandle().Entity();
	}

	FIELD(CUtlVector<CUnitOrders>, UnitOrders, Netvars::C_DOTAPlayerController::m_unitorders);
	GETTER(CUtlVector<uint32_t>, GetSelectedUnits, Netvars::C_DOTAPlayerController::m_nSelectedUnits);
	GETTER(int32_t, GetSequenceNum, Netvars::C_DOTAPlayerController::m_nOutgoingOrderSequenceNumber);
	GETTER(uint32_t, GetPlayerID, Netvars::C_DOTAPlayerController::m_nPlayerID);
	GETTER(uint64_t, GetSteamID, Netvars::C_DOTAPlayerController::m_steamID);

	void CastNoTarget(CDOTABaseAbility* ability, CBaseEntity* issuer = nullptr);
	void CastTarget(CDOTABaseAbility* ability, CBaseEntity* target, CBaseEntity* issuer = nullptr);
	void BuyItem(int itemId);
	void OrderMoveTo(const Vector& pos, bool directMovement = false, CBaseEntity* issuer = nullptr);

	void PrepareOrder(const PlayerOrder& order) {
		this->UnitOrders().push_back(order.ToUnitOrders());
		//PrepareOrder(order.Type, order.TargetIndex, order.Position, order.AbilityIndex, order.IssuerType, order.Issuer, order.Queue, order.ShowEffects);
	}
};
