#pragma once

class SceneController;
class SceneBase
{
public:
	SceneBase(SceneController& controller);

	virtual void Update()abstract;

	virtual void Draw()abstract;

protected:
	SceneController& m_controller;
};

