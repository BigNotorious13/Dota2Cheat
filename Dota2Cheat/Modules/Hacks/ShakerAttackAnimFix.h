#pragma once
#include <Dota2SDK.h>
#include <dota_usermessages.pb.h>

#include "../MListeners.h"

namespace Modules {
inline 
	// Changes Shaker's default attack animation to Enchant Totem's one
	// rofl feature from memelonity :)
	class ShakerAttackAnimFix : public INetChanListener {
	private:
		CDOTABaseNPC* shaker = nullptr;
	public:
		void Reset() {
			shaker = nullptr;
		}

		void SubscribeEntity(CDOTABaseNPC* hero) {
			if (auto unitName = hero->GetUnitName()) {
				if (unitName == "npc_dota_hero_earthshaker"sv)
					shaker = hero;
				else
					Reset();
			}
		}

		void OnReceivedMsg(NetMessageHandle_t* msgHandle, google::protobuf::Message* msg) override {
			if (msgHandle->messageID != DOTA_UM_TE_UnitAnimation || !shaker)
				return;

			auto animMsg = reinterpret_cast<CDOTAUserMsg_TE_UnitAnimation*>(msg);
			if (animMsg->activity() == 1503 &&
				NH2IDX(animMsg->entity()) == shaker->GetIndex())
				animMsg->set_sequence_variant(3);

		}
	} ShakerAttackAnimFix;
}