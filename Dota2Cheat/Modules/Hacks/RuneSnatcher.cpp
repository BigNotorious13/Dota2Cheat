#include "RuneSnatcher.h"
#include <dota_usercmd.pb.h>

void Modules::M_RuneSnatcher::OnTracedItem(entidx_t idx) {
	if (!ctx.localHero) return;
	if (!runes.contains(idx)) return;

	auto rune = CEntSys::Get()->GetEntity(idx);
	if (!IsWithinRadius(rune->GetPos(), ctx.localHero->GetPos(), 160)) return;

	ctx.localPlayer->PrepareOrder(
		PlayerOrder()
		.SetTargetIndex(idx)
		.SetType(DOTA_UNIT_ORDER_PICKUP_RUNE)
		.SetIssuer(ctx.localHero)
	);
	runes.erase(idx);
}

void Modules::M_RuneSnatcher::OnEntityAdded(const EntityWrapper& ent)
{
	if (ent.type == EntityType::Rune)
		runes.insert(ent->GetIndex());
}

void Modules::M_RuneSnatcher::OnEntityRemoved(const EntityWrapper& ent)
{
	runes.erase(ent->GetIndex());
}

void Modules::M_RuneSnatcher::OnFrame()
{
	if (!Config::AutoPickUpRunes) return;

	static NormalClass** cmdBufs = Address(CDOTAInput::Get()->GetVFunc(5)).Offset(0x8c).GetAbsoluteAddress(3).Dereference();
	static NormalClass* cmdBuf = cmdBufs[0];

	uint32_t lastTick = cmdBuf->Member<uint32_t>(0x5748);
	auto* cmd = cmdBuf->MemberInline<NormalClass>(0x90 * (lastTick % 150))->MemberInline<CDota2UserCmdPB>(0x10);
	auto tracedItem = CDOTAInput::Get()->Member<CHandle<CEnt>>(0x5d4 + 0x8c);
	if (tracedItem.IsValid()) OnTracedItem(tracedItem.Index());
}
