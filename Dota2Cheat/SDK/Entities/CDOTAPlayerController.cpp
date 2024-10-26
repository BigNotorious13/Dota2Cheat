#include "CDOTAPlayerController.h"

void CDOTAPlayerController::CastNoTarget(CDOTABaseAbility* ability, CBaseEntity* issuer) {
	PrepareOrder(
		PlayerOrder()
		.SetType(DOTA_UNIT_ORDER_CAST_NO_TARGET)
		.SetAbilityIndex(ability->GetIndex())
		.SetIssuer(issuer ? issuer : GetAssignedHero())
		.SetIssuerType(DOTA_ORDER_ISSUER_PASSED_UNIT_ONLY)
	);
}

void CDOTAPlayerController::CastTarget(CDOTABaseAbility* ability, CBaseEntity* target, CBaseEntity* issuer) {
	PrepareOrder(
		PlayerOrder()
		.SetType(DOTA_UNIT_ORDER_CAST_TARGET)
		.SetTargetIndex(target->GetIndex())
		.SetAbilityIndex(ability->GetIndex())
		.SetIssuer(issuer ? issuer : GetAssignedHero())
		.SetIssuerType(DOTA_ORDER_ISSUER_PASSED_UNIT_ONLY)
	);
}

void CDOTAPlayerController::BuyItem(int itemId) {
	PrepareOrder(
		PlayerOrder()
		.SetType(DOTA_UNIT_ORDER_PURCHASE_ITEM)
		.SetTargetIndex(1)
		.SetAbilityIndex(itemId)
		.SetIssuer(GetAssignedHero())
		.SetIssuerType(DOTA_ORDER_ISSUER_PASSED_UNIT_ONLY)
	);
}

void CDOTAPlayerController::OrderMoveTo(const Vector& pos, bool directMovement, CBaseEntity* issuer) {
	PrepareOrder(
		PlayerOrder()
		.SetType(directMovement ? DOTA_UNIT_ORDER_MOVE_TO_DIRECTION : DOTA_UNIT_ORDER_MOVE_TO_POSITION)
		.SetPosition(pos)
		.SetIssuer(issuer)
		.SetIssuerType(DOTA_ORDER_ISSUER_PASSED_UNIT_ONLY)
	);
}

