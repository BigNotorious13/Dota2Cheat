#include "PrepareUnitOrders.h"

CUnitOrders PlayerOrder::ToUnitOrders() const {
	CUnitOrders orders;
	orders.m_vPosition = Position;
	if (Queue)
		orders.m_nFlags |= 1;
	orders.m_nIssuerPlayerIndex = ctx.localPlayer->GetIndex();
	orders.m_nTargetIndex = TargetIndex;
	orders.m_nOrderSequenceNumber = ctx.localPlayer->Member<uint32_t>(Netvars::C_DOTAPlayerController::m_nLastSentOutgoingOrderSequenceNumber);
	orders.m_nOrderType = Type;

	orders.m_nUnits.m_pElements = CMemAlloc::Get()->Alloc<entidx_t>();
	orders.m_nUnits.m_pElements[0] = Issuer->GetIndex();
	orders.m_nUnits.m_Size = 1;
	orders.m_nUnits.m_Capacity = 1;

	return orders;
}

static bool OrderCallback(dotaunitorder_t orderType, uint32_t& targetIndex, Vector& position, UINT32& abilityIndex, CBaseEntity* issuer) {
	bool giveOrder = true;

	if (!Modules::ManaHPAbuse.IsInterruptible())
		return true;

	if (!issuer || !issuer->IsAlive())
		return true;

	switch (orderType) {
	case DOTA_UNIT_ORDER_CAST_TARGET:
	{
		Modules::CastRedirection.RedirectIfIllusionCast(targetIndex, issuer, abilityIndex, giveOrder);
		break;
	}
	case DOTA_UNIT_ORDER_CAST_POSITION: {
		Modules::PerfectBlink.AdjustIfBlink(&position, abilityIndex, issuer);
		break;
	}
	case DOTA_UNIT_ORDER_CAST_NO_TARGET: {
		Modules::ManaHPAbuse.PerformAbuse((CDOTABaseNPC*)issuer, CEntSys::Get()->GetEntity<CDOTAItem>(abilityIndex));
		break;
	}
	}

	if (orderType == DOTA_UNIT_ORDER_CAST_NO_TARGET ||
		orderType == DOTA_UNIT_ORDER_CAST_POSITION ||
		orderType == DOTA_UNIT_ORDER_CAST_TARGET)
		if (Modules::BadCastPrevention.IsBadCast(orderType, targetIndex, &position, abilityIndex, issuer)) {
			ShowHUDError("Bad cast: no targets!", "General.CastFail_NoTarget");
			return false;
		}

	return giveOrder;
}

static void OnSendMove(const char* source) {
	if (!ctx.localPlayer) return;
	auto& orders = ctx.localPlayer->Field<CUtlVector<CUnitOrders>>(Netvars::C_DOTAPlayerController::m_unitorders);
	if (orders.empty()) return;

	// Checks rebuilt from C_DOTAPlayerController::SendMove (called via CDOTAInput::SendMove)

	auto v2 = Plat_FloatTime();
	float v4 = 0;
	if (
		ctx.localPlayer->Member<uint32_t>(Netvars::C_DOTAPlayerController::m_nOutgoingOrderSequenceNumber)
		==
		ctx.localPlayer->Member<uint32_t>(Netvars::C_DOTAPlayerController::m_nLastSentOutgoingOrderSequenceNumber)
		)
	{
		static const auto& dota_unit_orders_resend_rate = CCVar::CVars["dota_unit_orders_resend_rate"];
		v4 = fmaxf(dota_unit_orders_resend_rate.m_pVar->value.flt, 1.0);
	}
	else
	{
		static const auto& dota_unit_orders_rate = CCVar::CVars["dota_unit_orders_rate"];
		v4 = fmaxf(dota_unit_orders_rate.m_pVar->value.flt, 10.0);
	}

	if (v2 < ((1.0 / v4) + ctx.localPlayer->Member<float>(Netvars::C_DOTAPlayerController::m_flLastUnitOrdersSendTime))) return;

	for (int i = 0; i < orders.size(); ) {
		auto& order = orders[i];
		bool pass = OrderCallback(order.m_nOrderType, order.m_nTargetIndex, order.m_vPosition, order.m_nAbilityIndex, CEntSys::Get()->GetEntity(order.m_nUnits[0]));
		if (!pass) orders.remove_at(i);

		else i++;
	}

	//for (auto& order : orders) {
	//	enum class OrderFlags {
	//		Queue = 1,
	//		FromClick = 2,
	//		AltCast = 4
	//	};

	//	std::lock_guard lock(mLogging);
	//	std::cout << std::format("Order {} =====", order.m_nOrderSequenceNumber) << std::endl;
	//	std::cout << std::format("Pos: {:.2f} {:.2f} {:.2f}", order.m_vPosition.x, order.m_vPosition.y, order.m_vPosition.z) << std::endl;
	//	std::cout << std::format("Type: {}", dotaunitorder_t_Name(order.m_nOrderType)) << std::endl;
	//	std::cout << std::format("Target: {}", order.m_nTargetIndex);
	//	if (order.m_nTargetIndex != 0) std::cout << " " << CEntSys::Get()->GetEntity(order.m_nTargetIndex)->GetIdentity()->GetName();
	//	std::cout << std::endl;

	//	std::cout << std::format("Ability: {}", order.m_nAbilityIndex);
	//	if (order.m_nAbilityIndex != 0) std::cout << " " << CEntSys::Get()->GetEntity(order.m_nAbilityIndex)->GetIdentity()->GetName();
	//	std::cout << std::endl;

	//	std::cout << std::format("Flags: {:03b}", order.m_nFlags) << std::endl;
	//}

}

void Hooks::hkCInput__SendMove(CDOTAInput* rcx, int a2, float a3, bool a4) {
	OnSendMove("SendMove");
	return ORIGCALL(CInput__SendMove)(rcx, a2, a3, a4);
}

bool Hooks::hkPrepareUnitOrders(CDOTAPlayerController* player, dotaunitorder_t orderType, UINT32 targetIndex, Vector* position, UINT32 abilityIndex, PlayerOrderIssuer_t orderIssuer, CBaseEntity* issuer, bool queue, bool showEffects) {
	if (OrderCallback(orderType, targetIndex, *position, abilityIndex, issuer))
		return oPrepareUnitOrders(player, orderType, targetIndex, position, abilityIndex, orderIssuer, issuer, queue, showEffects);

	return false;
}

