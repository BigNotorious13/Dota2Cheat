#pragma once
#include "../../CheatSDK/include.h"
#include "../MListeners.h"

namespace Modules {
	inline class M_RuneSnatcher : public IEntityListListener, public IFrameListener {
		std::set<entidx_t> runes;
	public:
		void OnTracedItem(entidx_t idx);

		void OnEntityAdded(const EntityWrapper& ent) override;
		void OnEntityRemoved(const EntityWrapper& ent) override;

		void OnFrame() override;

		M_RuneSnatcher() {
			IEntityListListener::Subscribe(this);
		}
	} RuneSnatcher;
}