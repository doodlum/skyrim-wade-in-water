#pragma once

class ActorUpdateHandler 
{
public:
	static void InstallHooks()
	{
		Hooks::Install();
	}

	static ActorUpdateHandler* GetSingleton()
	{
		static ActorUpdateHandler handler;
		return &handler;
	}

	static float GetSubmergedLevel_Impl(RE::Actor* a_actor, float a_zPos, RE::TESObjectCELL* a_cell)
	{
		using func_t = decltype(&GetSubmergedLevel_Impl);
		REL::Relocation<func_t> func{ REL::RelocationID(36452, 37448) };  // 1.5.97 1405E1510
		return func(a_actor, a_zPos, a_cell);
	}

	static float GetSubmergedLevel(RE::Actor* a_actor)
	{
		return GetSubmergedLevel_Impl(a_actor, a_actor->GetPositionZ(), a_actor->GetParentCell());
	}

	static float GetWaterMultiplier(RE::Actor* a_actor);


protected:
	struct Hooks
	{
		struct Actor_SetMaximumMovementSpeed
		{
			static float thunk(RE::Actor* a_actor)
			{
				float original = func(a_actor);
				return original * GetWaterMultiplier(a_actor);
			}
			static inline REL::Relocation<decltype(thunk)> func;
		};


		static void Install()
		{
			stl::write_thunk_call<Actor_SetMaximumMovementSpeed>(REL::RelocationID(37013, 37943).address() + REL::Relocate(0x1A, 0x51));
		}
	};
private:
	constexpr ActorUpdateHandler() noexcept = default;
	ActorUpdateHandler(const ActorUpdateHandler&) = delete;
	ActorUpdateHandler(ActorUpdateHandler&&) = delete;

	~ActorUpdateHandler() = default;

	ActorUpdateHandler& operator=(const ActorUpdateHandler&) = delete;
	ActorUpdateHandler& operator=(ActorUpdateHandler&&) = delete;
};
