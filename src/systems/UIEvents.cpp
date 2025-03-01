#include "UIEvents.hpp"

#include "../math/Math2D.hpp"

#include "../components/UIComponents.hpp"

void HandleUIMouseEvent( entt::registry& reg, const SDL_Event* event ) {
	if ( event->type == SDL_EVENT_MOUSE_BUTTON_DOWN && event->button.button == 1 ) {
		const wlVec2 mousePos{ event->button.x, event->button.y };
		for ( auto&& [ent, btn, uiElement] : reg.view<wlButton, wlUIElement>().each() ) {
			if (wlMath2D::PointInRect( mousePos, uiElement.dstRect ) ) {
				uiElement.scale *= 0.95f;
				btn.isPressed = true;
				btn.onClick( reg );
			}
		}
	} else if ( event->type == SDL_EVENT_MOUSE_BUTTON_UP && event->button.button == 1 ) {
		const wlVec2 mousePos{ event->button.x, event->button.y };
		for ( auto&& [ent, btn, uiElement] : reg.view<wlButton, wlUIElement>().each() ) {
			if (wlMath2D::PointInRect( mousePos, uiElement.dstRect ) ) {
				btn.isPressed = false;
				uiElement.scale *= 1.0f / 0.95f;
			}
		}
	}
}