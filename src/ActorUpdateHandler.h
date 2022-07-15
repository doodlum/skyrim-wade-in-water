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

	void Update(RE::Actor* a_actor, float a_delta);


protected:
	struct Hooks
	{
		struct PlayerCharacter_Update
		{
			static void thunk(RE::PlayerCharacter* a_player, float a_delta)
			{
				func(a_player, a_delta);
				GetSingleton()->Update(a_player, a_delta);
			}
			static inline REL::Relocation<decltype(thunk)> func;
		};

		struct Actor_Update
		{
			static void thunk(RE::Actor* a_actor, float a_delta)
			{
				func(a_actor, a_delta);
				GetSingleton()->Update(a_actor, a_delta);
			}
			static inline REL::Relocation<decltype(thunk)> func;
		};

		static void Install()
		{
			stl::write_vfunc<RE::PlayerCharacter, 0xAD, PlayerCharacter_Update>();
			stl::write_vfunc<RE::Character, 0xAD, Actor_Update>();
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
