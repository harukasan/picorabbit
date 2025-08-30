puts "main_task.rb"

COLOR_RUBYKAIGI_RED = 0xC0
COLOR_RUBYKAIGI_YELLOW = 0xF0
COLOR_RUBYKAIGI_GREEN = 0x28
COLOR_RUBYKAIGI_INDIGO = 0x01
COLOR_RUBYKAIGI_BROWN = 0x40

include PicoRabbit::Draw

def draw_rotating_line(t, center_x, center_y, length, rotation_speed, color)
  angle = rotation_speed * t

  x2 = center_x + length * Math.cos(angle)
  y2 = center_y + length * Math.sin(angle)

  draw_line(center_x, center_y, x2, y2, color)
end

text = <<~'TEXT'
=

\e[FF]        Successor to PicoRabbit:
\e[FF]      Ruby Programming Environment





\e[FF]            Shunsuke Michii
\e[FF]              @harukasan
\e[FF]             August 30 2025
\e[FF]        RubyKaigi 2025 follow up

= What is PicoRabbit?

A tiny presentation device inspired by
Rabbit, running on Raspberry Pi Pico 2.

Available on GitHub:
github.com/harukasan/picorabbit


I demonstrated this at
the Lightning Talks of RubyKaigi 2025.

= Features

* Running on mruby
* Realtime digital video output
* Drawing: \e[C0]t\e[F0]e\e[28]x\e[01]t\e[reset], image, shapes
* Slide engine

It can run simple game written in Ruby.

= R2P2: Ruby Rapid Portable Platform

"Ruby on Raspberry Pi Pico"
programming platform using PicoRuby.

Features:
  * Shell
  * File system
  * mruby Compiler
  * IRB (Interactive Ruby)
  * Editor

= Missing for Portability

R2P2 needs a terminal emulator on a PC

For true portability,
it needs to connect directly to:
  * a screen for output
  * a keyboard for input

= Motivation

I want to build a standalone platform,
programming Ruby, like IchigoJam.

PicoRabbit already has:
  * screen output via DVI
  * drawaing engine

= R2P2D2 (R2P2 Display Device)

My current project to
add a display and input to R2P2.

Output: DVI for the display.
Input: USB HID support for keyboards.

|------|  |========|---(Display)
|      |--| R2P2D2 |
| R2P2 |  |========|---(Keyboard)
|      |
|------|---------------(IO)

= Single-Chip Approach

Bundle the display driver as an mrbgem

Pros: Single Pico, simple and cheap
Cons: Very tight memory constraints
Cons: Limited number of GPIO pins

= Dual-Chip Approach

Separates R2P2 and the terminal onto
two Pico chips

Pros: Minimal changes to R2P2
Pros: Dedicated CPU for display output
Cons: Requires two chips (costs ~$20)

I tentatively call this R2P2D3
(R2P2D2-Duo)

= Communication via UART

We can use UART for communication
between the two Picos.

+------+            +------+
| R2P2 |---(UART)---|  D2  |
+------+            +------+

PicoSDK can redirect stdio to UART.
So, we just need to change R2P2's
STDIO to UART.

= Next steps

* Implement terminal (WIP)
* Design a new board
* Add USB HID support
* Add audio output

= Internals & Improvements

Today, I'd like to introduce some
technical details and improvements
after RubyKaigi 2025.

Note:

= HSTX

PicoRabbit uses HSTX;
  High-speed
  Serial
  Transmit

It uses DDR registers to communicate
at twice the system clock speed.

= DMA (Direct Memory Access)

HSTX has 32bit FIFO.
It uses DMA to transfer data.
In PicoRabbit, the DMA handler sets the
framebuffer address for each scanline.

= Framebuffer

PicoRabbit uses:

  * double buffered framebuffer
  * half-height resolution
  * RGB332 1bpp color

640 x 240 x 2 x 1 = 307,200B
59% of RP2350 memory

= Changing to half-size framebuffer

Changing to a 320x240px screen size
cuts the memory usage in half.

320 x 240 x 2 x 1 = 153,600B

But, it requires 2x scaling in the
DMA handler.

= Stabilizing DMA

PicoRabbit refers to Raspberry Pi's
example implementation.
It uses two DMA channels in
a "ping-pong" setup.

If the handler couldn't
prepare the next buffer in time,
it caused screen glitches.

Improvement:
Use three DMA channels to transfer

= Use multiple line buffers

Add small line buffer for 2x scaling
in the DMA handler

= WIP: text rendering into line buffer

To save memory, we can render text
directly into the line buffer
instead of framebuffer.

= Next steps

* Implement terminal (WIP)
* Design a new board
* Add USB HID support
* Add audio output

= 

TEXT

margin_x = 4
margin_y = 36

slides = []
elements = []
text_y = 0
text.each_line do |line|
  if line.start_with?("=")
    slides << Slide.new(elements)
    elements = []
    text_y = 0

    line = line[1..].strip
  end

  elements << TextElement.new(line, margin_x, text_y + margin_y, COLOR_BLACK)
  text_y += 12
end
slides << Slide.new(elements)

# Slide switch
button_right = GPIO.new(8, GPIO::PULL_UP)
button_left = GPIO.new(10, GPIO::PULL_UP)
button_up = GPIO.new(11, GPIO::PULL_UP)
button_a = GPIO.new(21, GPIO::PULL_UP)
button_b = GPIO.new(20, GPIO::PULL_UP)

press = {
  up: false,
  down: false,
  left: false,
  right: false,
  a: false,
  b: false,
}

count = 0

start_time = time_usec_32
f = 0

usagi_base_y = 240-24-6
usagi_y = usagi_base_y
usagi_vy = 0.0
usagi_jumping = false
gravity = 0.7
jump_power = -7.6

# GAME
obstacles = []
obstacle_timer = 0
obstacle_interval = 60
obstacle_speed = 3
obstacle_width = 14
obstacle_height = 14
usagi_width = 24
usagi_height = 24
game_over = false
score = 0
prev_press_a = false

loop do
  elapsed = time_usec_32 - start_time

  pressed = false
  if button_right.read == 0
    press[:right] = true
    pressed = true
  end
  if button_left.read == 0
    press[:left] = true
    pressed = true
  end
  if button_up.read == 0
    press[:up] = true
    pressed = true
  end
  if button_a.read == 0
    press[:a] = true
    pressed = true
  end
  if button_b.read == 0
    press[:b] = true
    pressed = true
  end

  unless pressed
    if press[:right]
      if count < slides.size - 1
        count += 1
      end
    elsif press[:left]
      if count > 0
        count -= 1
      end
    end

    press.each do |key, value|
      press[key] = false
    end
  end

  if press[:b]
    start_time = time_usec_32
  end

  case count
  when 0
    draw_image :rubykaigi2025, 0, 0
  when 1
    draw_image :background, 0, 0
  else
    background COLOR_WHITE
    border_y = 6
    draw_rect 0, 0, 320, border_y, COLOR_RUBYKAIGI_RED
    draw_rect 0, 240-border_y, 320, 240-border_y, COLOR_RUBYKAIGI_RED
  end

  draw_slide(slides[count])


  if count == 3
    shape_y = 150
    draw_rotating_line(f, 320/2, shape_y, 40, 0.02, COLOR_RUBYKAIGI_GREEN)
    draw_rect 80-20, shape_y-20, 40, 40, (f%255)
    draw_image :usagi, 320-80, shape_y - 10, -1 * f.to_f
  end

  if count == slides.size - 1
    usagi_x = 40

    unless game_over
      # 障害物生成タイミング
      obstacle_timer += 1
      if obstacle_timer >= obstacle_interval
        obstacle_timer = 0
        obstacles << { x: 320, y: usagi_base_y + usagi_height - obstacle_height, w: obstacle_width, h: obstacle_height, scored: false }
      end

      # 障害物の移動と描画
      obstacles.each do |obs|
        obs[:x] -= obstacle_speed
        draw_rect obs[:x], obs[:y], obs[:w], obs[:h], COLOR_BLACK
        # スコア加算（usagiが飛び越えたら）
        if !obs[:scored] && obs[:x] + obs[:w] < usagi_x
          score += 1
          obs[:scored] = true
        end
      end
      obstacles.reject! { |obs| obs[:x] + obs[:w] < 0 }

      # 衝突判定
      obstacles.each do |obs|
        if obs[:x] < usagi_x + usagi_width &&
           obs[:x] + obs[:w] > usagi_x &&
           obs[:y] < usagi_y + usagi_height &&
           obs[:y] + obs[:h] > usagi_y
          game_over = true
        end
      end
      # スコア表示
      draw_text("SCORE: #{score}", 220, 8, COLOR_BLACK)
    else
      draw_text("GAME OVER", (320 - "GAME OVER".length * 8) / 2, 100, COLOR_BLACK)
      draw_text("SCORE: #{score}", 220, 8, COLOR_BLACK)
    end
  else
    obstacles = []
    obstacle_timer = 0
    game_over = false
    score = 0
  end

  jump_triggered = press[:a] && !prev_press_a

  if jump_triggered && !usagi_jumping && usagi_y >= usagi_base_y
    usagi_jumping = true
    usagi_vy = jump_power
  end

  if usagi_jumping
    usagi_y += usagi_vy
    usagi_vy += gravity
    if usagi_y >= usagi_base_y
      usagi_y = usagi_base_y
      usagi_jumping = false
      usagi_vy = 0.0
    end
  end

  prev_press_a = press[:a]  # ループの最後で更新

  unless count == slides.size - 1
    kame_x = (elapsed / 1000) * (320-24)  / (300 * 1000)
    draw_image :kame, kame_x, 240-24-6

    usagi_x = (count / (slides.size - 1).to_f) * (320-24)
  end
  draw_image :usagi, usagi_x, usagi_y

  f += 1
  commit
end

