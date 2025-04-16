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

\e[FF]              PicoRabbit:
\e[FF]      a Tiny Presentation Device
\e[FF]            Powered by Ruby





\e[FF]            Shunsuke Michii
\e[FF]              @harukasan
\e[FF]      April 17 2025, RubyKaigi 2025

= What is PicoRabbit?

A tiny presentation device inspired by
Rabbit, running on Raspberry Pi Pico 2.

* Realtime digital video output
* Slide engine built on mruby VM
* Drawing: \e[C0]t\e[F0]e\e[28]x\e[01]t\e[reset], image, shapes

Available on GitHub:
github.com/harukasan/picorabbit

= Motivation

At RubyKaigi2024, I saw "Dreampresent"
running on both Wii and Dreamcast.

My keyboard runs with PRK Firmware,
powered by PicoRuby.

So I wondered:
Can Pico run presentation too?

= Difficulties

+ DVI output handling
  Requires extremely precise timing

+ Limited memory
  640x480 screen needs 307kB of memory
  but RP2040 has only 264kB SRAM

+ Instability of PicoRuby
  PicoRuby is still under development
  Hard to get a stable build

So, I gave up using PicoRuby on RP2040.


= Raspberry Pi Pico 2 made it possible

Raspberry Pi Pico 2
features more powerful chip RP2350
Great for real-time graphics
and running the full mruby VM

* 520kB of SRAM (2x the previous Pico!)
  Enough for a 640x480 screen
  and the full mruby VM,
  not just mruby/c

* HSTX interface for DVI output
  Enables direct DVI signal generation

= Running mruby on RP2350

Most steps can be based on PicoRuby.

1. Write build_config.rb

2. Build the mruby with mrbgems

3. Compile Ruby script with mrbc

4. Load and run in main.c

= Write build_config.rb

It can use the same way as PicoRuby

MRuby::CrossBuild.new("mruby-cortex-m33") do |conf|
  conf.toolchain :gcc
  conf.host_target = "arm-none-eabi"

  (snip)  

  conf.cc.flags.flatten!
  conf.cc.flags << "-O3"
  conf.cc.flags << "-mcpu=cortex-m33"
  ...

= mrbgems for PicoRabbit

* mruby-pico-gpio
* mruby-pico-print
* mruby-pico-time
* mruby-pico-memory-usage
* mruby-picorabbit-draw

= mrbgems difference of mruby/c and mruby

PicoRuby supports mrbgems.
But method/class definition APIs
are different between mruby and mruby/c.

In mruby/c:
mrbc_class *c = mrbc_define_class(vm, "GPIO", mrbc_class_object);
mrbc_define_method(vm, c, "write", c_write);

In mruby:
mrb_value c = mrb_define_class(mrb, "GPIO", mrb->object_class);
MRB_SET_INSTANCE_TT(c, MRB_TT_DATA);
mrb_define_method(mrb, c, "write", c_write, MRB_ARGS_REQ(1));

= Future

* Remove output noise
* USB mass storage support
* PicoRuby integration
* Keyboard input support

Not just a presentation device -
I want PicoRabbit to become
a standalone programming tool
that runs mruby on its own.

= Thank you!

I have a few boards to give away.
Talk to me after the session!

github.com/harukasan/picorabbit

= BONUS TIME

Have time left?  
Next slide starts the game!

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


  if count == 2
    shape_y = 190
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
      draw_text("GAME OVER", 100, 100, COLOR_BLACK)
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

