#pragma once
#include<cstdint>

using CollisionLayer = uint32_t;

namespace CollisionLayers
{
	constexpr CollisionLayer kNone = 0;
	constexpr CollisionLayer kPlayer = 1 << 0;
	constexpr CollisionLayer kEnemy = 1 << 1;
	constexpr CollisionLayer kHat = 1 << 2;
}
