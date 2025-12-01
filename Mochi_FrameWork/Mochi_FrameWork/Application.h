#pragma once

#include    <Windows.h>
#include    <cstdint>

//-----------------------------------------------------------------------------
// Applicationクラス
//-----------------------------------------------------------------------------
class Application
{
public:
    Application(uint32_t width, uint32_t height);
    ~Application();
    void Run();

    // 幅を取得
    static uint32_t GetWidth() {
        return mWidth;
    }

    // 高さを取得
    static uint32_t GetHeight() {
        return mHeight;
    }

    // ウインドウハンドルを返す
    static HWND GetWindow() {
        return mHWnd;
    }

private:
    static HINSTANCE   mHInst;        // インスタンスハンドル
    static HWND        mHWnd;         // ウィンドウハンドル
    static uint32_t    mWidth;        // ウィンドウの横幅
    static uint32_t    mHeight;       // ウィンドウの縦幅

    static bool InitApp();   //初期化
    static void UninitApp(); //終了処理
    static void MainLoop();  //メインループ

    //プロシージャ
    static LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);
};