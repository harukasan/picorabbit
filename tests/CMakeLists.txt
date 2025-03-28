# 共通のソースファイル
set(TEST_SOURCES
    main.c
    test_draw.c
    test_framebuffer.c
)

if(BUILD_DESKTOP_TESTS)
    # デスクトップ向けビルド設定
    project(desktop_tests C)
    
    add_executable(desktop_tests
        ${TEST_SOURCES}
        mocks/hardware_mock.c
        ../src/draw.c
        ../src/framebuffer.c
    )
    
    target_include_directories(desktop_tests PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/../include
    )
    
    # デスクトップテスト用の定義
    target_compile_definitions(desktop_tests PRIVATE
        PICO_DESKTOP_TEST=1
    )
    
else()
    # Pico SDKの設定を読み込む
    set(PICO_SDK_PATH $ENV{PICO_SDK_PATH})
    include(${PICO_SDK_PATH}/external/pico_sdk_import.cmake)
    
    # Pico向けビルド設定
    project(pico_tests C CXX ASM)
    pico_sdk_init()
    
    add_executable(pico_tests
        ${TEST_SOURCES}
        ../src/draw.c
        ../src/framebuffer.c
    )
    
    target_include_directories(pico_tests PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/../include
    )
    
    target_link_libraries(pico_tests
        pico_stdlib
        pico_multicore
    )
    
    # Picoのビルド設定
    pico_add_extra_outputs(pico_tests)
    pico_enable_stdio_usb(pico_tests 1)
endif()