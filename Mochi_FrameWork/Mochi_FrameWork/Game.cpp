#include "Game.h"
#include "SceneStack.h"
#include "EventQueue.h"
#include "SceneEventProcessor.h"
#include "InputManager.h"
#include "Renderer.h"

// 課題用　後で消す
#include "KadaiScene.h"

#include <memory>

// === 名前空間使用宣言 === //
using namespace MochiFramework::SceneSystem;
using namespace MochiFramework::InputSystem;

// === 名前空間 === //
namespace MochiFramework::Core
{
	Game::Game()
		: mEventQueue()
		, mInput(std::make_unique<InputManager>(0))
		, mContext()
		, mFramework(mContext)
		, mSceneStack(mFramework)
		, mEventProcessor(mSceneStack, mEventQueue)
	{
		mContext.eventQueue = &mEventQueue;
		mContext.input = mInput.get();
	}
	bool Game::Initialize()
	{
		//-- ゲームの初期化処理 --//	
		// 
		// 描画初期化処理
		Renderer::Init();

		//-- シーン登録 --//
		mSceneStack.RegisterScene("KadaiScene", [](FrameworkFacade& f) { return std::make_unique<KadaiScene>(f); });

		mSceneStack.ChangeMainScene("KadaiScene");

		return true; // 初期化に成功すればtrueを返す
	}

	void Game::Shutdown()
	{
		//-- ゲームの終了処理 --//

		// 描画終了処理
		Renderer::Uninit();
	}

	void Game::ProcessInput()
	{
		mInput->Update(); // デバイス状態を1フレーム分更新
		mSceneStack.Input(); // SceneStackにデバイスを渡して InputComponent を更新させる
	}

	void Game::UpdateGame(const float deltaTime)
	{
		mEventProcessor.Process();		// イベント処理（遷移など）
		mSceneStack.Update(deltaTime);	// 現在のシーンの更新
	}

	void Game::GenerateOutput()
	{
		mSceneStack.Draw();
	}

	bool Game::ShouldQuit() const {
		return mEventProcessor.ShouldQuit();
	}
}
