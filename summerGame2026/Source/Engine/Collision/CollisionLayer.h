#pragma once
#include<cstdint>

using CollisionLayer = uint32_t;

/// <summary>
/// コリジョンのレイヤーをビットで行う
/// </summary>
namespace CollisionLayers
{
	constexpr CollisionLayer kNone = 0;
	constexpr CollisionLayer kPlayer = 1 << 0;
	constexpr CollisionLayer kEnemy = 1 << 1;
	constexpr CollisionLayer kHat = 1 << 2;
	constexpr CollisionLayer kControllEnemy = 1 << 3;
	constexpr CollisionLayer kStar = 1 << 4;
}
