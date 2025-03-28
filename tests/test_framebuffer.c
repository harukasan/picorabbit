#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "hardware_interface.h"
#include "framebuffer.h"

// テスト用のヘルパー関数のプロトタイプ宣言
extern void mock_set_core(uint32_t core_num);

void test_framebuffer_init(void) {
    framebuffer_init();
    
    // 初期化後、表示バッファが0であることを確認
    const uint8_t *display_buffer = framebuffer_get_display();
    assert(display_buffer != NULL);
    
    // 描画バッファが1であることを確認
    uint8_t *draw_buffer = framebuffer_get_draw();
    assert(draw_buffer != NULL);
    assert(draw_buffer != display_buffer);
}

void test_framebuffer_swap(void) {
    framebuffer_init();
    
    // 初期状態を保存
    const uint8_t *initial_display = framebuffer_get_display();
    uint8_t *initial_draw = framebuffer_get_draw();

    framebuffer_swap();
    
    // バッファが入れ替わっていない事を確認
    const uint8_t *new_display = framebuffer_get_display();
    uint8_t *new_draw = framebuffer_get_draw();

    assert(new_display == initial_display);
    assert(new_draw == initial_draw);
}

void test_framebuffer_commit_swap(void) {
    framebuffer_init();
    mock_set_core(0);  // Core 0として実行
    
    // コミット前の状態を保存
    const uint8_t *initial_display = framebuffer_get_display();
    
    // バックバッファに描画
    uint8_t *draw_buffer = framebuffer_get_draw();
    memset(draw_buffer, 0xAA, FRAMEBUFFER_WIDTH * FRAMEBUFFER_HEIGHT);
    
    // コミットを実行
    framebuffer_commit();
    framebuffer_swap();

    const uint8_t *new_display = framebuffer_get_display();
    uint8_t *new_draw = framebuffer_get_draw();
    assert(new_display == draw_buffer);
    assert(new_draw == initial_display);
}

void test_framebuffer_scaled_line(void) {
    framebuffer_init();
    
    // テストパターンを描画
    uint8_t *draw_buffer = framebuffer_get_draw();
    for (int i = 0; i < FRAMEBUFFER_WIDTH; i++) {
        draw_buffer[i] = i & 0xFF;
    }
    
    framebuffer_commit();
    framebuffer_swap();
    
    // スケーリングされた行を取得して確認
    const uint8_t *scaled_line = framebuffer_get_scaled_line(0);
    assert(scaled_line != NULL);
    
    // 水平方向に2倍にスケーリングされていることを確認
    for (int i = 0; i < FRAMEBUFFER_WIDTH; i++) {
        assert(scaled_line[i * 2] == (i & 0xFF));
        assert(scaled_line[i * 2 + 1] == (i & 0xFF));
    }
    
    // 範囲外のラインに対してNULLが返されることを確認
    assert(framebuffer_get_scaled_line(OUTPUT_HEIGHT) == NULL);
}

// メインのテストファイルから呼び出される関数
void run_framebuffer_tests(void) {
    printf("フレームバッファテストを開始します...\n");
    
    test_framebuffer_init();
    printf("初期化テスト ................. OK\n");
    
    test_framebuffer_swap();
    printf("バッファスワップテスト ........ OK\n");
    
    test_framebuffer_commit_swap();
    printf("コミットテスト ................ OK\n");
    
    test_framebuffer_scaled_line();
    printf("スケーリングテスト ............ OK\n");
    
    printf("全てのフレームバッファテストが成功しました！\n");
}