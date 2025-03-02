#pragma once

#include "entt.hpp"
#include "../math/Vector.hpp"

enum class yAnchor_t : uint8_t;
enum class xAnchor_t : uint8_t;

bool HasHiddenParent( entt::registry& reg, entt::entity ent );

namespace UIFactory {
entt::entity CreateButton( entt::registry& reg, const std::string& spriteName,
						   const wlVec2& scale, xAnchor_t xAnchor, yAnchor_t yAnchor, const wlVec2& position,
						   std::function<void( entt::registry& )> onClick, entt::entity parent = entt::null,
						   int positionZ = 0);

entt::entity CreateText( entt::registry& reg, const std::string& text, entt::entity parent,
						 xAnchor_t xAnchor, yAnchor_t yAnchor, int positionZ = 0 );

entt::entity CreatePanel( entt::registry& reg, const std::string& spriteName,
						  const wlVec2& scale, xAnchor_t xAnchor, yAnchor_t yAnchor );
}
