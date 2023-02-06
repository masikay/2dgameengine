#ifndef RENDER_GUI_SYSTEM_H
#define RENDER_GUI_SYSTEM_H

#include "../ECS/ECS.h"
#include "../AssetStore/AssetStore.h"
#include "../Components/TransformComponent.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/BoxColliderComponent.h"
#include "../Components/ProjectileEmitterComponent.h"
#include "../Components/HealthComponent.h"
#include <glm/glm.hpp>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_sdl.h>
#include <imgui/imgui_impl_sdlrenderer.h>

#include <vector>
#include <string>
#include <cmath>


#if defined(WIN32) || defined(_WIN32) || defined(__WIN32)
#include <SDL.h>
#else
#include <SDL2/SDL.h>
#endif

// Helper to display a little (?) mark which shows a tooltip when hovered.
// In your own code you may want to display an actual icon if you are using a merged icon fonts (see docs/FONTS.md)
static void HelpMarker(const char* desc)
{
	ImGui::TextDisabled("(?)");
	if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayShort))
	{
		ImGui::BeginTooltip();
		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
		ImGui::TextUnformatted(desc);
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}
}

class RenderGuiSystem : public System
{
public:
	RenderGuiSystem() = default;

	void Update(const std::unique_ptr<Registry>& registry, SDL_Rect& camera)
	{
		ImGui_ImplSDLRenderer_NewFrame();
		ImGui_ImplSDL2_NewFrame();
		ImGui::NewFrame();
		
		if (ImGui::Begin("Spawn Enemies"))
		{
			// Static variables for TransformComponent
			static int positionX = 0;
			static int positionY = 0;
			static int scaleX = 1;
			static int scaleY = 1;
			static float rotation = 0.0;
			
			// Static variables for RigidBodyComponent
			static int bodySpeed = 0;
			static float bodyAngle = 0.0;
			static bool syncBodyAngle = true;
			
			// Static variables for ProjectileEmitterComponent
			static int projectileSpeed = 100;
			static float projectileAngle = 0.0;
			static int projectileHitPercentDamage = 10;
			static int projectileRepeatFrequency = 10;
			static int projectileDuration = 10;
			static bool syncProjectileAngle = true;

			// Variables for SpriteComponent
			const char* sprites[] = { "tank-image", "truck-image" };
			static int spriteIdx = 0;

			// Static variables for HealthComponents
			static int health = 100;

			// SpriteComponent input section
			if (ImGui::CollapsingHeader("Sprite", ImGuiTreeNodeFlags_DefaultOpen))
			{
				ImGui::Combo("texture id", &spriteIdx, sprites, IM_ARRAYSIZE(sprites));
			}
			ImGui::Spacing();

			// TransformComponent input section
			if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
			{
				ImGui::InputInt("position x", &positionX);
				ImGui::InputInt("position y", &positionY);
				ImGui::SliderInt("scale x", &scaleX, 1, 10);
				ImGui::SliderInt("scale y", &scaleY, 1, 10);
				ImGui::SliderAngle("rotation (deg)", &rotation, 0, 360);
			}
			ImGui::Spacing();

			// RigidBodyComponent input section
			if (ImGui::CollapsingHeader("Rigid Body", ImGuiTreeNodeFlags_DefaultOpen))
			{
				ImGui::SliderInt("speed (px/sec)##body", &bodySpeed, 0, 500);
				ImGui::SliderAngle("angle (deg)##body", &bodyAngle, 0, 360); ImGui::SameLine();
				ImGui::Checkbox("sync##body", &syncBodyAngle); ImGui::SameLine(); HelpMarker("Synchronize with enemy's rotation angle");
			}
			ImGui::Spacing();

			if (syncBodyAngle)
			{
				bodyAngle = rotation;
			}

			// ProjectileEmitterComponent input section
			if (ImGui::CollapsingHeader("Projectile Emitter", ImGuiTreeNodeFlags_DefaultOpen))
			{
				ImGui::SliderInt("speed (px/sec)##projectile", &projectileSpeed, 0, 500);
				ImGui::SliderAngle("angle (deg)##projectile", &projectileAngle, 0, 360); ImGui::SameLine();
				ImGui::Checkbox("sync##projectile", &syncProjectileAngle); ImGui::SameLine(); HelpMarker("Synchronize with enemy's rotation angle");
				ImGui::InputInt("repeat (sec)", &projectileRepeatFrequency);
				ImGui::InputInt("duration (sec)", &projectileDuration);
				ImGui::SliderInt("damage %", &projectileHitPercentDamage, 0, 100);
			}
			ImGui::Spacing();


			if (syncProjectileAngle)
			{
				projectileAngle = rotation;
			}

			// HealthComponent input section
			if (ImGui::CollapsingHeader("Health", ImGuiTreeNodeFlags_DefaultOpen))
			{
	
				ImGui::SliderInt("%", &health, 0, 100);
			}

			ImGui::Spacing();
			ImGui::Separator();
			ImGui::Spacing();


			if (ImGui::Button("Create new enemy"))
			{
				Entity enemy = registry->CreateEntity();
				enemy.Group("enemies");
				enemy.AddComponent<TransformComponent>(glm::vec2(positionX, positionY), glm::vec2(scaleX, scaleY), glm::degrees(rotation));
				
				double bodyVelocityX = bodySpeed * cos(bodyAngle);
				double bodyVelocityY = bodySpeed * sin(bodyAngle);				
				enemy.AddComponent<RigidBodyComponent>(glm::vec2(bodyVelocityX, bodyVelocityY));

				enemy.AddComponent<SpriteComponent>(sprites[spriteIdx], 32, 32, 2);
				enemy.AddComponent<BoxColliderComponent>(25, 20, glm::vec2(5, 5));
				
				double projectileVelocityX = projectileSpeed * cos(projectileAngle);
				double projectileVelocityY = projectileSpeed * sin(projectileAngle);
				enemy.AddComponent<ProjectileEmitterComponent>(glm::vec2(projectileVelocityX, projectileVelocityY), projectileRepeatFrequency * 1000, projectileDuration * 1000, projectileHitPercentDamage, false);
				
				enemy.AddComponent<HealthComponent>(health);

				// Reset all the input values
				positionX = positionY = bodySpeed = spriteIdx = 0;
				scaleX = scaleY = 1;
				rotation = bodyAngle = projectileAngle = 0.0;
				projectileRepeatFrequency = projectileDuration = projectileHitPercentDamage = 0;
				projectileSpeed = health = 100;

			}
		}
		ImGui::End();

		// DIsplay a small overlay window to display the map position using the mouse
		ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoNav;
		ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_Always, ImVec2(0, 0));
		ImGui::SetNextWindowBgAlpha(0.9f);
		if (ImGui::Begin("Map Coordinates", NULL, windowFlags))
		{
			ImGui::Text(
				"Map coordinates (x=%.1f, y=%.1f)",
				ImGui::GetIO().MousePos.x + camera.x,
				ImGui::GetIO().MousePos.y + camera.y
			);
		}
		ImGui::End();

		ImGui::Render();
		ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());
	}
};

#endif
