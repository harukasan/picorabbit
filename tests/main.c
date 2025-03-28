#include <stdio.h>
#include <assert.h>

#include "hardware_interface.h"

// テスト関数のプロトタイプ宣言
void test_draw_background(void);
void test_draw_rect(void);
void test_draw_text(void);
void test_draw_text_fast(void);
void run_framebuffer_tests(void);

int main() {
    printf("テストを開始します...\n");

    // ハードウェア初期化
    hw_init();

    test_draw_background();
    test_draw_rect();
    test_draw_text();
    test_draw_text_fast();
    run_framebuffer_tests();

    printf("全てのテストが成功しました！\n");
    return 0;
}