#pragma once

namespace CK
{

struct Transform2D
{
public:
	FORCEINLINE constexpr Transform2D() = default;
	FORCEINLINE constexpr Transform2D(const Vector2& InPosition) : Position(InPosition) { }
	FORCEINLINE constexpr Transform2D(const Vector2& InPosition, float InRotation) : Position(InPosition), Rotation(InRotation) { }
	FORCEINLINE constexpr Transform2D(const Vector2& InPosition, float InRotation, const Vector2& InScale) : Position(InPosition), Rotation(InRotation), Scale(InScale) { }

public: // Ʈ������ �����Լ�
	constexpr void SetPosition(const Vector2& InPosition) { Position = InPosition; }
	constexpr void AddPosition(const Vector2& InDeltaPosition) { Position += InDeltaPosition; }
	void AddRotation(float InDegree) { Rotation += InDegree; Update(); }

	void SetRotation(float InDegree) { Rotation = InDegree; Update(); }
	constexpr void SetScale(const Vector2& InScale) { Scale = InScale; }

	FORCEINLINE constexpr Vector2 GetXAxis() const { return Right; }
	FORCEINLINE constexpr Vector2 GetYAxis() const { return Up; }
	constexpr Matrix3x3 GetMatrix() const;

	FORCEINLINE constexpr Vector2 GetPosition() const { return Position; }
	FORCEINLINE constexpr float GetRotation() const { return Rotation; }
	FORCEINLINE constexpr Vector2 GetScale() const { return Scale; }

	// Ʈ������ ��ȯ
	FORCEINLINE Transform2D Inverse() const;
	FORCEINLINE Transform2D LocalToWorld(const Transform2D& InParentWorldTransform) const;
	FORCEINLINE Transform2D WorldToLocal(const Transform2D& InParentWorldTransform) const;

	void Update()
	{
		ClampAngle();
		float sin = 0.f, cos = 0.f;
		Math::GetSinCos(sin, cos, Rotation);
		Right = Vector2(cos, sin);
		Up = Vector2(-sin, cos);
	}

private:
	void ClampAngle()
	{
		Rotation = Math::FMod(Rotation, 360.f);
		if (Rotation < 0.f)
		{
			Rotation += 360.f;
		}
	}

private: // Ʈ�������� ���õ� ����
	Vector2 Position;
	float Rotation = 0.f;
	Vector2 Scale = Vector2::One;

	Vector2 Up = Vector2::UnitY;
	Vector2 Right = Vector2::UnitX;
};

FORCEINLINE constexpr Matrix3x3 Transform2D::GetMatrix() const
{
	return Matrix3x3(
		Vector3(GetXAxis() * Scale.X, false),
		Vector3(GetYAxis() * Scale.Y, false),
		Vector3(Position, true)
	);
}

FORCEINLINE Transform2D Transform2D::Inverse() const
{
	// ���� ������ ����� ���� Ʈ������ ( ����� )
	Vector2 reciprocalScale = Vector2::Zero;
	if (!Math::EqualsInTolerance(Scale.X, 0.f)) reciprocalScale.X = 1.f / Scale.X;
	if (!Math::EqualsInTolerance(Scale.Y, 0.f)) reciprocalScale.Y = 1.f / Scale.Y;

	Transform2D result;
	result.Scale = reciprocalScale;
	result.Position = -Position;
	result.Rotation = -Rotation;
	result.Update();

	return result;
}


FORCEINLINE Transform2D Transform2D::LocalToWorld(const Transform2D& InParentWorldTransform) const
{
	// ���� Ʈ������ ������ ������ ���
	Matrix2x2 rot(InParentWorldTransform.Right, InParentWorldTransform.Up);

	Transform2D result;
	result.Scale = InParentWorldTransform.Scale * Scale;
	result.Rotation = InParentWorldTransform.Rotation + Rotation;
	result.Position = rot * (Position * InParentWorldTransform.Scale) + InParentWorldTransform.Position;
	result.Update();

	return result;
}

FORCEINLINE Transform2D Transform2D::WorldToLocal(const Transform2D& InParentWorldTransform) const
{
	Transform2D invParent = InParentWorldTransform.Inverse();

	// ���� Ʈ������ ������ ������ ���
	Transform2D result;
	result.Scale = invParent.Scale * Scale;
	result.Rotation = invParent.Rotation + Rotation;
	result.Position = (invParent.Position + Position) * invParent.Scale;
	result.Update();

	return result;
}

}
