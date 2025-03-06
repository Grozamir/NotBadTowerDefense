#pragma once

#include <cmath>

struct wlVec2 {
	float			x;
	float			y;

					wlVec2();
					wlVec2( const float t_x, const float t_y );

	wlVec2			operator*( const wlVec2& vec ) const;
	wlVec2			operator/( const wlVec2& vec ) const;
	wlVec2			operator+( const wlVec2& vec ) const;
	wlVec2			operator-( const wlVec2& vec ) const;

	wlVec2			operator*( const float value ) const;

	wlVec2&			operator*=( const float value );
	

	bool			operator==( const wlVec2& vec ) const;

	void			Normalize();
	float			Length() const;
};

inline wlVec2::wlVec2() : x(0.0f), y(0.0f) { }

inline wlVec2::wlVec2( const float t_x, const float t_y ) : x{ t_x }, y{ t_y } {
}

inline wlVec2 wlVec2::operator*( const wlVec2& vec ) const {
	return wlVec2{ x * vec.x, y * vec.y };
}

inline wlVec2 wlVec2::operator/( const wlVec2& vec ) const {
	return wlVec2{ x / vec.x, y / vec.y };
}

inline wlVec2 wlVec2::operator+( const wlVec2& vec ) const {
	return wlVec2{ x + vec.x, y + vec.y };
}

inline wlVec2 wlVec2::operator-( const wlVec2& vec ) const {
	return wlVec2{ x - vec.x, y - vec.y };
}

inline wlVec2 wlVec2::operator*( const float value ) const {
	return wlVec2{ x * value, y * value };
}

inline wlVec2& wlVec2::operator*=( const float value ) {
	x *= value;
	y *= value;
	return *this;
}

inline bool wlVec2::operator==( const wlVec2& vec ) const {
	return std::fabs( x - vec.x ) < 1e-6 && std::fabs( y - vec.y ) < 1e-6;
}

inline void wlVec2::Normalize() {

	const float len = Length();
	if ( len > 0.0001f ) {
		x /= len;
		y /= len;
	}
}

inline float wlVec2::Length() const {
	return std::sqrt( x * x + y * y );
}
