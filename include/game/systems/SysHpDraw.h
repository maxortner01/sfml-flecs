#pragma once

#include <engine/Core.h>

namespace game
{
	struct SysHpDraw : public s2de::System
	{
		SysHpDraw(sf::Texture* healthbar);

		void onRender(s2de::Scene& scene, flecs::world& world) override;
		void onUpdate(double dt, flecs::world& world) override;

	private:
		sf::RenderTexture inside_health;
		sf::RenderTexture healthbar;
		sf::Texture* health_texture;
	};
}