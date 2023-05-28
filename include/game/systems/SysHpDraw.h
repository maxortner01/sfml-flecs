#pragma once

#include <engine/Core.h>

namespace game
{
	struct SysHpDraw : public s2de::System
	{
		void onRender(s2de::Scene& scene, flecs::world& world) override;
		void onUpdate(double dt, flecs::world& world) override;
	};
}