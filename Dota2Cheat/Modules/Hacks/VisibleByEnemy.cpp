#include "VisibleByEnemy.h"

void Modules::M_VBE::UpdateEntity(const EntityWrapper& npc) {
	auto visibility = npc->Member<Visibility>(Netvars::C_DOTA_BaseNPC::m_iTaggedAsVisibleByTeam);
	if (visibility == Visibility::Visible) {
		auto& wrap = vbeParticles[npc->GetIndex()] = VBEData{
			.entWrap = npc,
			.particle =
			CParticleMgr::Get()->CreateParticle(
				"materials/ensage_ui/particles/visiblebyenemy.vpcf",
				ParticleAttachment_t::PATTACH_ABSORIGIN_FOLLOW,
				(CNPC*)npc
			)
		};
		wrap.particle->SetControlPoint(0, { 0, 0, 0 });
		wrap.particle->SetControlPoint(1, Config::VBE::ParticleColor * 255); // color
		wrap.particle->SetControlPoint(2, { 255, 0, 0 }); // x = alpha
	}
	else if (visibility == Visibility::NotVisible) {
		RemoveParticleFor(npc);
	}
}

void Modules::M_VBE::OnClientPreSimulate(void* ev) {
	if (!ctx.localHero) return;
	if (!Config::VBE::Enabled) return;

	if (events.toggled) OnToggled();
	if (events.filtersChanged) OnFiltersChanged();
	if (events.colorChanged) OnParticleColorChanged();

	EntityList.ForEach(
		[this](const EntityWrapper& wrap) {
			if(ApplyFilter(wrap))
				UpdateEntity(wrap);
		}
	);
}

void Modules::M_VBE::OnEntityAdded(const EntityWrapper& ent)
{

}

void Modules::M_VBE::OnEntityRemoved(const EntityWrapper& ent)
{
	RemoveParticleFor(ent);
}
