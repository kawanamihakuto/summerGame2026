#include "Transform.h"

Matrix4x4 TransForm::GetWorldMatrix() const
{
    Matrix4x4 scaleMat = Matrix4x4::Scale(scale);

    Matrix4x4 rotateXMat = Matrix4x4::RotationX(rotation.x);
    Matrix4x4 rotateYMat = Matrix4x4::RotationY(rotation.y);
    Matrix4x4 rotateZMat = Matrix4x4::RotationZ(rotation.z);

    Matrix4x4 transMat = Matrix4x4::Translate(position);

    return transMat * rotateZMat * rotateXMat * rotateYMat  * scaleMat;
}

Matrix4x4 TransForm::GetRotationMatrix() const
{
    return Matrix4x4::RotationX(rotation.x) *
           Matrix4x4::RotationX(rotation.y) *
           Matrix4x4::RotationX(rotation.z);
}

Vector3 TransForm::Forward() const
{
    Matrix4x4 rot = GetRotationMatrix();
    return rot.TransformVector({0.0f,0.0f,1.0f}).Normalized();
}

Vector3 TransForm::Right() const
{
    Matrix4x4 rot = GetRotationMatrix();
    return rot.TransformVector({ 1.0f,0.0f,0.0f }).Normalized();
}

Vector3 TransForm::Up() const
{
    Matrix4x4 rot = GetRotationMatrix();
    return rot.TransformVector({ 0.0f,1.0f,0.0f }).Normalized();
}

void TransForm::Translate(const Vector3& move)
{
    position += move;
}

void TransForm::SetScale(const Vector3& s)
{
    scale = s;
}
