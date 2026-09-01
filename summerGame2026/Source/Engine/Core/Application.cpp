#include "Application.h"
#include<DxLib.h>
#include<memory>
#include"Config.h"
#include"Game/Scene/SceneController.h"
#include"Game/Scene/TitleScene.h"
#include"ResourceManager.h"
Application::Application() :
	m_windowSize(Config::kWindowWidth, Config::kWindowHeight)
{

}

Application::~Application()
{

}

Application& Application::GetInstance()
{
	static Application instance;
	return instance;
}

bool Application::Init()
{
#ifdef _DEBUG
	//ウィンドウサイズ設定
	SetGraphMode(m_windowSize.w, m_windowSize.h, Config::kColorBitNum);
	//ウィンドウ
	ChangeWindowMode(true);
#else 
	//フルスクリーン
	ChangeWindowMode(false);
	m_windowSize.w = 1920;
	m_windowSize.h = 1080;
	//ウィンドウサイズ設定
	SetGraphMode(m_windowSize.w, m_windowSize.h, Config::kColorBitNum);
#endif // _DEBUG

	//ウィンドウのタイトル設定
	SetMainWindowText(L"SUPER CHUCHU ODYSSEY");
	//文字コードの設定
	SetUseCharCodeFormat(DX_CHARCODEFORMAT_UTF8);

	if (DxLib_Init() == -1)
	{
		return false;
	}

	// 描画対象をバックバッファに変更
	SetDrawScreen(DX_SCREEN_BACK);
	// カリングの設定
	SetUseBackCulling(true);
	//ライティングの計算を行うかどうかの設定
	SetUseLighting(true);

	//初期化成功
	return true;
}

void Application::Run()
{
	// Zバッファの設定
	SetUseZBuffer3D(true);	// Zバッファを使います
	SetWriteZBuffer3D(true);	// 描画する物体はZバッファにも距離を書き込む

	//リソースのロード
	auto& resouceManager = ResourceManager::GetInstance();
	resouceManager.LoadResources();

	//シーンの作成
	SceneController SceneController;
	SceneController.ChangeScene(std::make_shared<TitleScene>(SceneController));
//	SceneController.ChangeScene(std::make_shared<GameScene>(SceneController));

	while (ProcessMessage() != -1)
	{
		// このフレームの開始時間を取得
		LONGLONG start = GetNowHiPerformanceCount();

		// 前のフレームに描画した内容をクリアする
		ClearDrawScreen();

		//ここにゲームの処理を書く

		SceneController.Update();
		SceneController.Draw();

		//------------------------
		ScreenFlip();

#ifdef _DEBUG

#endif // DEBUG
		//escキーを押したらゲームを強制終了	
		if (CheckHitKey(KEY_INPUT_ESCAPE))
		{
			SceneController.PopScene();
			break;
		}

		//フレームレート固定
		while (GetNowHiPerformanceCount() - start < 16667)
		{

		}
	}
}

void Application::Terminate()
{
	//リソースの解放
	auto& resouceManager = ResourceManager::GetInstance();
	resouceManager.ReleaseResources();
	DxLib_End();
}

const Size& Application::GetWindowSize() const
{
	return m_windowSize;
}


