#include "Application.h"
#include<DxLib.h>
#include<memory>
#include"Config.h"
#include"../SceneMain.h"
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
	//ウィンドウサイズ設定
	SetGraphMode(m_windowSize.w, m_windowSize.h, Config::kColorBitNum);
#ifdef _DEBUG
	//ウィンドウ
	ChangeWindowMode(true);
#else
	//フルスクリーン
	ChangeWindowMode(false);
#endif // _DEBUG

	//ウィンドウのタイトル設定
	SetMainWindowText(L"マリオオデッセイみたいなやつ");
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

	//シーンの作成
	std::shared_ptr<SceneMain>pScene = std::make_shared<SceneMain>();
	pScene->Init();

	while (ProcessMessage() != -1)
	{
		// このフレームの開始時間を取得
		LONGLONG start = GetNowHiPerformanceCount();

		// 前のフレームに描画した内容をクリアする
		ClearDrawScreen();

		//ここにゲームの処理を書く

		pScene->Update();

		pScene->Draw();

		//------------------------
		ScreenFlip();

#ifdef _DEBUG

#endif // DEBUG
		//escキーを押したらゲームを強制終了	
		if (CheckHitKey(KEY_INPUT_ESCAPE))
		{
			pScene->End();
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
	DxLib_End();
}

const Size& Application::GetWindowSize() const
{
	return m_windowSize;
}


