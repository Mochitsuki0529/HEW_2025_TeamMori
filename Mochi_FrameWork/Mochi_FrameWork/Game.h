#pragma once

#include <memory>
#include "SceneStack.h"
#include "EventQueue.h"
#include "SceneEventProcessor.h"
#include "InputManager.h"
#include "FrameworkContext.h"
#include "FrameworkFacade.h"


// === 名前空間 === //
namespace MochiFramework::Core
{
	class Game
	{

	public:
		// ===  コンストラクタ・デストラクタ  === //
		Game();
		~Game() = default;

		Game(const Game&) = delete;
		Game& operator=(const Game&) = delete;
		Game(Game&&) = delete;
		Game& operator=(Game&&) = delete;

		// ===  公開関数（外部インターフェース）  === //

		bool Initialize();	// 初期化
		void RunLoop();		// ゲームループ
		void Shutdown();	// 終了時処理

		bool ShouldQuit() const; // 終了フラグの取得


	private:

		// ===  補助関数（内部ロジック）  === //

		void ProcessInput();						// 入力
		void UpdateGame(const float deltaTime);		// 更新
		void GenerateOutput();						// 描画

		// ===  メンバ変数（内部変数）  === //

		 // 所有しているシステム
		MochiFramework::SceneSystem::EventQueue              mEventQueue;      // イベントキュー
		std::unique_ptr<MochiFramework::InputSystem::InputManager> mInput;     // 共有入力デバイス

		// それらへの参照を束ねたコンテキスト & Facade
		MochiFramework::Core::FrameworkContext mContext;
		MochiFramework::Core::FrameworkFacade  mFramework;

		// Facade ベースのシーン管理 & イベント処理機
		MochiFramework::SceneSystem::SceneStack             mSceneStack;       // シーン管理
		MochiFramework::SceneSystem::SceneEventProcessor    mEventProcessor;   // イベント処理機
	};

} // namespace MochiFramework::Core

