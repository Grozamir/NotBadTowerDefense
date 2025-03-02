#include "UIEvents.hpp"

#include "../components/UIComponents.hpp"

#include "../math/Math2D.hpp"

#include "../helpers/UIHelper.hpp"


bool HandleUIMouseEvent( entt::registry& reg, const SDL_Event* event ) {
	bool result = false;
	if ( event->type == SDL_EVENT_MOUSE_BUTTON_DOWN && event->button.button == 1 ) {
		const wlVec2 mousePos{ event->button.x, event->button.y };
		for ( auto&& [ent, btn, uiElement] : reg.view<wlButton, wlUIElement>( entt::exclude<wlHideUIElement> ).each() ) {
			if ( wlMath2D::PointInRect( mousePos, uiElement.dstRect ) && !HasHiddenParent( reg, ent ) ) {
				uiElement.scale *= 0.95f;
				btn.isPressed = true;
				btn.onClick( reg );

				result = true;
			}
		}
	} else if ( event->type == SDL_EVENT_MOUSE_BUTTON_UP && event->button.button == 1 ) {
		for ( auto&& [ent, btn, uiElement] : reg.view<wlButton, wlUIElement>().each() ) {
			if ( btn.isPressed) {
				btn.isPressed = false;
				uiElement.scale *= 1.0f / 0.95f;

				result = true;
			}
		}
	}

	return result;
}