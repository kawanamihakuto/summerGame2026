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

Vector3 TransForm::Forward() const
{
    Matrix4x4 rot = Matrix4x4::RotationX(rotation.x) *
                    Matrix4x4::RotationY(rotation.y);

    return rot.TransformVector({ 0.0f,0.0f,1.0f });
}

void TransForm::Translate(const Vector3& move)
{
    position += move;
}

void TransForm::SetScale(const Vector3& s)
{
    scale = s;
}
