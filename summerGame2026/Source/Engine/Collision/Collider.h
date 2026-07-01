#pragma once

/// <summary>
/// コライダーの種類
/// </summary>
enum ColliderType
{
	capsule,
	sphere,
};

class Collider
{
public:
	virtual ~Collider() = default;

	/// <summary>
	/// コライダーの種類を返す
	/// </summary>
	/// <returns></returns>
	virtual ColliderType GetType()const = 0;
};

