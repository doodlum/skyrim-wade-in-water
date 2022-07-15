#include "ActorUpdateHandler.h"

void ActorUpdateHandler::Update(RE::Actor* a_actor, [[maybe_unused]] float a_delta)
{
	static RE::SpellItem*      WaterSlowdownSmall;
	static RE::SpellItem*      WaterSlowdownLarge;
	static RE::SpellItem*      WaterSlowdownSwim;
	static RE::TESDataHandler* dataHandler;
	if (!dataHandler) {
		dataHandler = RE::TESDataHandler::GetSingleton();
		if (dataHandler) {
			WaterSlowdownSmall = dataHandler->LookupForm<RE::SpellItem>(0xD64, "WadeInWater.esp");
			WaterSlowdownLarge = dataHandler->LookupForm<RE::SpellItem>(0xD65, "WadeInWater.esp");
			WaterSlowdownSwim = dataHandler->LookupForm<RE::SpellItem>(0xD67, "WadeInWater.esp");
		}
	};

	auto submergedLevel = GetSubmergedLevel(a_actor);

	if (submergedLevel >= 0.69) {
		if (a_actor->HasSpell(WaterSlowdownSwim))
			return;
		a_actor->RemoveSpell(WaterSlowdownLarge);
		a_actor->RemoveSpell(WaterSlowdownSmall);
		a_actor->AddSpell(WaterSlowdownSwim);
	} else if (submergedLevel >= 0.4) {
		if (a_actor->HasSpell(WaterSlowdownLarge))
			return;
		a_actor->RemoveSpell(WaterSlowdownSwim);
		a_actor->RemoveSpell(WaterSlowdownSmall);
		a_actor->AddSpell(WaterSlowdownLarge);
	} else if (submergedLevel >= 0.2) {
		if (a_actor->HasSpell(WaterSlowdownSmall))
			return;
		a_actor->RemoveSpell(WaterSlowdownSwim);
		a_actor->RemoveSpell(WaterSlowdownLarge);
		a_actor->AddSpell(WaterSlowdownSmall);
	} else {
		a_actor->RemoveSpell(WaterSlowdownSwim);
		a_actor->RemoveSpell(WaterSlowdownLarge);
		a_actor->RemoveSpell(WaterSlowdownSmall);
	}
}
