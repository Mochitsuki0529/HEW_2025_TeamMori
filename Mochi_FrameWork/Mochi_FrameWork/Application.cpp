#include <chrono>
#include <thread>
#include "Application.h"
#include "Game.h"
#include "Renderer.h"

const auto ClassName = TEXT("2025 framework ひな型");     //ウィンドウクラス名
const auto WindowName = TEXT("2025 framework ひな型");    //ウィンドウ名

HINSTANCE  Application::mHInst;   // インスタンスハンドル
HWND       Application::mHWnd;    // ウィンドウハンドル
uint32_t   Application::mWidth;   // ウィンドウの横幅
uint32_t   Application::mHeight;  // ウィンドウの縦幅

//-----------------------------------------------------------------------------
// コンストラクタ
//-----------------------------------------------------------------------------
Application::Application(uint32_t width, uint32_t height)
{
    mHeight = height;
    mWidth = width;

    timeBeginPeriod(1); //タイマー精度を1ミリ秒に設定
}

//-----------------------------------------------------------------------------
// デストラクタ
//-----------------------------------------------------------------------------
Application::~Application()
{
    timeEndPeriod(1); // タイマー精度を元に戻す
}

//-----------------------------------------------------------------------------
// 実行
//-----------------------------------------------------------------------------
void Application::Run()
{
    //初期化
    bool okfg = InitApp();
    if (okfg) { MainLoop(); }

    UninitApp(); // 終了処理
}

//-----------------------------------------------------------------------------
// 初期化処理
//-----------------------------------------------------------------------------
bool Application::InitApp()
{
    // インスタンスハンドルを取得
    auto hInst = GetModuleHandle(nullptr);
    if (hInst == nullptr)
    {
        return false;
    }

    // ウィンドウの設定
    WNDCLASSEX wc = {};
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WndProc;
    wc.hIcon = LoadIcon(hInst, IDI_APPLICATION);
    wc.hCursor = LoadCursor(hInst, IDC_ARROW);
    wc.hbrBackground = GetSysColorBrush(COLOR_BACKGROUND);
    wc.lpszMenuName = nullptr;
    wc.lpszClassName = ClassName;
    wc.hIconSm = LoadIcon(hInst, IDI_APPLICATION);

    // ウィンドウの登録
    if (!RegisterClassEx(&wc))
    {
        return false;
    }

    // インスタンスハンドル設定
    mHInst = hInst;

    // ウィンドウのサイズを設定
    RECT rc = {};
    rc.right = static_cast<LONG>(mWidth);
    rc.bottom = static_cast<LONG>(mHeight);

    // ウィンドウサイズを調整
    auto style = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU;
    AdjustWindowRect(&rc, style, FALSE);

    // ウィンドウを生成
    mHWnd = CreateWindowEx(
        0,
        //        WS_EX_TOPMOST,
        ClassName,
        WindowName,
        style,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        rc.right - rc.left,
        rc.bottom - rc.top,
        nullptr,
        nullptr,
        mHInst,
        nullptr);

    if (mHWnd == nullptr)
    {
        return false;
    }

    // ウィンドウを表示
    ShowWindow(mHWnd, SW_SHOWNORMAL);

    // ウィンドウを更新
    UpdateWindow(mHWnd);

    // ウィンドウにフォーカスを設定
    SetFocus(mHWnd);

    // 正常終了
    return true;

}

//-----------------------------------------------------------------------------
// 終了処理
//-----------------------------------------------------------------------------
void Application::UninitApp()
{
    // ウィンドウの登録を解除
    if (mHInst != nullptr)
    {
        UnregisterClass(ClassName, mHInst);
    }

    mHInst = nullptr;
    mHWnd = nullptr;
}

//-----------------------------------------------------------------------------
// メインループ
//-----------------------------------------------------------------------------
void Application::MainLoop()
{
    MSG msg = {};

    // ゲームオブジェクト
	MochiFramework::Core::Game game;

    // ゲーム初期化処理
	game.Initialize();

    // FPS計測用変数
    int fpsCounter = 0;
    long long oldTick = GetTickCount64(); // 前回計測時の時間
    long long nowTick = oldTick; // 今回計測時の時間

    // FPS固定用変数
    LARGE_INTEGER liWork; // workがつく変数は作業用変数
    long long frequency;// どれくらい細かく時間をカウントできるか
    QueryPerformanceFrequency(&liWork);
    frequency = liWork.QuadPart;
    // 時間（単位：カウント）取得
    QueryPerformanceCounter(&liWork);
    long long oldCount = liWork.QuadPart;// 前回計測時の時間
    long long nowCount = oldCount;// 今回計測時の時間


    // ゲームループ
    while (1)
    {
        // 新たにメッセージがあれば
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            // ウィンドウプロシージャにメッセージを送る
            TranslateMessage(&msg);
            DispatchMessage(&msg);

            // 「WM_QUIT」メッセージを受け取ったらループを抜ける
            if (msg.message == WM_QUIT) {
                break;
            }
        }
        else
        {
            QueryPerformanceCounter(&liWork);// 現在時間を取得
            nowCount = liWork.QuadPart;
            // 1/60秒が経過したか？
            if (nowCount >= oldCount + frequency / 60) {

                // ゲーム入力処理
				game.ProcessInput();

                // ゲーム更新処理
				game.UpdateGame(1.0f / 60.0f);
                
                // ゲーム描画処理
				game.GenerateOutput();

                fpsCounter++; // ゲーム処理を実行したら＋１する
                oldCount = nowCount;
            }
        }
    }

    // ゲーム終了処理
	game.GenerateOutput();
}

//-----------------------------------------------------------------------------
// ウィンドウプロシージャ
//-----------------------------------------------------------------------------
LRESULT CALLBACK Application::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    static bool isFullscreen = false;
    static bool isMessageBoxShowed = false;
    switch (uMsg)
    {
    case WM_DESTROY:// ウィンドウ破棄のメッセージ
        PostQuitMessage(0);// 「WM_QUIT」メッセージを送る　→　アプリ終了
        break;

    case WM_CLOSE:  // 「x」ボタンが押されたら
    {
        int res = MessageBoxA(NULL, "終了しますか？", "確認", MB_OKCANCEL);
        if (res == IDOK) {
            DestroyWindow(hWnd);  // 「WM_DESTROY」メッセージを送る
        }
    }
    break;

    case WM_KEYDOWN: //キー入力があったメッセージ
        if (LOWORD(wParam) == VK_ESCAPE)
        { //入力されたキーがESCAPEなら
            PostMessage(hWnd, WM_CLOSE, wParam, lParam);//「WM_CLOSE」を送る
        }

        else if (LOWORD(wParam) == VK_F11)
        {
            isFullscreen = !isFullscreen;
            if (isFullscreen) {
                //フルスクリーンに切り替え
                //g_pSwapChain->SetFullscreenState(TRUE, NULL);
                //ShowWindow(hWnd, SW_MAXIMIZE);

                // 疑似フルスクリーンモードに変更
                SetWindowLongPtr(hWnd, GWL_STYLE, WS_POPUP | WS_MINIMIZEBOX); // ウィンドウ枠を削除
                // ディスプレイ解像度を取得
                int screenWidth = GetSystemMetrics(SM_CXSCREEN);
                int screenHeight = GetSystemMetrics(SM_CYSCREEN);
                SetWindowPos(hWnd, HWND_TOP, 0, 0, screenWidth, screenHeight, SWP_FRAMECHANGED | SWP_SHOWWINDOW);
            }
            else {
                //ウィンドウモードに戻す
                //g_pSwapChain->SetFullscreenState(FALSE, NULL);
                //ShowWindow(hWnd, SW_RESTORE);

                // 通常ウィンドウに戻す
                SetWindowLongPtr(hWnd, GWL_STYLE, WS_OVERLAPPEDWINDOW); // ウィンドウ枠を戻す
                SetWindowPos(hWnd, HWND_TOP, 100, 100, mWidth, mHeight, SWP_FRAMECHANGED | SWP_SHOWWINDOW);
            }
        }
        break;

    case WM_ACTIVATE:
        if (wParam == WA_INACTIVE) {
            // フルスクリーン表示かつメッセージボックス非表示なら
            if (isFullscreen && !isMessageBoxShowed)
            {
                // ウインドウを最小化する（タスク切替時に背後に残る問題対策）
                ShowWindow(hWnd, SW_MINIMIZE);
            }
        }
        // 標準挙動を実行
        return DefWindowProc(hWnd, uMsg, wParam, lParam);

    case WM_SIZE: //ウィンドウサイズに変更があったメッセージ

        if (wParam != SIZE_MINIMIZED)
        {
            int width = LOWORD(lParam); //横幅
            int height = HIWORD(lParam); //縦幅
            Renderer::ResizeWindow(width, height);
        }
        break;

    default:
        // 受け取ったメッセージに対してデフォルトの処理を実行
        return DefWindowProc(hWnd, uMsg, wParam, lParam);
        break;
    }

    return 0;
}

