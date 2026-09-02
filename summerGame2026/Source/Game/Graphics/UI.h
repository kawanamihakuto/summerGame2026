#pragma once
class UI
{
public:
	UI(int UIGraphHandle,int maxStarNum,int maxHP);
	~UI();
	void Update();
	void Draw(int starNum,int hp);

private:
	int m_UIGraphHandle;
	int m_maxStarNum;
	int m_maxHP;
};

