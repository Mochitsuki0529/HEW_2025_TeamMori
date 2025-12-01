///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// メモリリーク検知用のデバッグマクロ
// メモリリーク時に場所を特定

#define _CRTDBG_MAP_ALLOC
#include <cstdlib>
#include <crtdbg.h>

#ifdef _DEBUG
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif

// 注意
// 影響範囲が現在mainのみになっているのでこのマクロをnewする場所にインクルードもしくは書き込まないと場所は出てこない

//////////////////////////////////////////////////////////////////////////////////////

#include "Game.h"

int main() 
{
    // メモリリーク検出を有効化
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    MochiFramework::Core::Game game;
    bool success = game.Initialize();
    if (success) {
        game.RunLoop();
    }

    game.Shutdown();

    return 0;
}
