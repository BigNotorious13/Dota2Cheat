#pragma once
#include "../MListeners.h"
#include "../../CheatSDK/include.h"

#include <nlohmann/json.hpp>

namespace Modules {
	inline class M_VBE : public IEntityListListener {
		enum class Visibility {
			NotVisible = 22,
			Visible = 30,
		};


		struct VBEData {
			EntityWrapper entWrap;
			ParticleWrapper particle;
		};

		std::map<entidx_t, VBEData> vbeParticles;

		void UpdateEntity(const EntityWrapper& npc);
		void RemoveParticleFor(const CNPC* npc) {
			const entidx_t idx = npc->GetIndex();
			if (!vbeParticles.contains(idx)) return;

			CParticleMgr::Get()->DestroyParticle(vbeParticles[idx].particle);
			vbeParticles.erase(idx);
		};

		bool ApplyFilter(const EntityWrapper& wrap) {
			if (wrap->GetTeam() != ctx.localHero->GetTeam()) return false;

			if (wrap.type == EntityType::Hero) return Config::VBE::Filters::NPCs;
			else if (wrap.type == EntityType::Creep) return Config::VBE::Filters::Creeps;

			return false;
		}

		void OnFiltersChanged() {
			events.filtersChanged = false;

			for (auto it = vbeParticles.begin(); it != vbeParticles.end();) {
				if (!ApplyFilter(it->second.entWrap)) {
					CParticleMgr::Get()->DestroyParticle(it->second.particle);
					it = vbeParticles.erase(it);
				}
				else
					it++;
			}
		}

		void OnToggled() {
			events.toggled = false;

			if (!Config::VBE::Enabled) {
				for (auto& [idx, data] : vbeParticles) {
					CParticleMgr::Get()->DestroyParticle(data.particle);
				}

				vbeParticles.clear();
			}
		}

		void OnParticleColorChanged() {
			events.colorChanged = false;

			for (auto& [idx, data] : vbeParticles) {
				data.particle->SetControlPoint(1, Config::VBE::ParticleColor * 255);
			}
		}
		struct Events {
			bool toggled;
			bool filtersChanged;
			bool colorChanged;
		} events;
	public:
		void QueueToggled() {
			events.toggled = true;
		}

		void QueueFiltersChanged() {
			events.filtersChanged = true;
		}

		void QueueColorChanged() {
			events.colorChanged = true;
		}

		void OnClientPreSimulate(void* ev);

		M_VBE() {
			IEntityListListener::Subscribe(this);
		}

		void OnEntityAdded(const EntityWrapper& ent) override;
		void OnEntityRemoved(const EntityWrapper& ent) override;
	} VisibleByEnemy;
}