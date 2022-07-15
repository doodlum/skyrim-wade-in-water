#include "ActorUpdateHandler.h"

float ActorUpdateHandler::GetWaterMultiplier(RE::Actor* a_actor)
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
	auto waterMultiplier = 1.0f;

	if (a_actor->IsSwimming()) {
		if (!a_actor->HasSpell(WaterSlowdownSwim)) {
			a_actor->RemoveSpell(WaterSlowdownLarge);
			a_actor->RemoveSpell(WaterSlowdownSmall);
			a_actor->AddSpell(WaterSlowdownSwim);
		}
		if (submergedLevel == 1.0f) {
			return 1.0f;
		}
	}

	if (a_actor->IsSwimming()) {
		if (!a_actor->HasSpell(WaterSlowdownSwim)) {
			a_actor->RemoveSpell(WaterSlowdownLarge);
			a_actor->RemoveSpell(WaterSlowdownSmall);
			a_actor->AddSpell(WaterSlowdownSwim);
		}
		return 1.0f;
	} else if (submergedLevel >= 0.4f) {
		if (!a_actor->HasSpell(WaterSlowdownLarge)) {
			a_actor->RemoveSpell(WaterSlowdownSwim);
			a_actor->RemoveSpell(WaterSlowdownSmall);
			a_actor->AddSpell(WaterSlowdownLarge);
			waterMultiplier = 1.0f - (std::lerp(0, WaterSlowdownLarge->effects[0]->effectItem.magnitude, submergedLevel + 0.6f) / 100);
		}
	} else if (submergedLevel >= 0.2f) {
		if (!a_actor->HasSpell(WaterSlowdownSmall)) {
			a_actor->RemoveSpell(WaterSlowdownSwim);
			a_actor->RemoveSpell(WaterSlowdownLarge);
			a_actor->AddSpell(WaterSlowdownSmall);
		}
		waterMultiplier = 1.0f - (std::lerp(WaterSlowdownSmall->effects[0]->effectItem.magnitude, WaterSlowdownLarge->effects[0]->effectItem.magnitude, submergedLevel - 0.2f * 5) / 100);
	} else {
		a_actor->RemoveSpell(WaterSlowdownSwim);
		a_actor->RemoveSpell(WaterSlowdownLarge);
		a_actor->RemoveSpell(WaterSlowdownSmall);
		waterMultiplier = 1.0f - (std::lerp(0, WaterSlowdownSmall->effects[0]->effectItem.magnitude, submergedLevel * 5) / 100);
	}

	return waterMultiplier;
}
