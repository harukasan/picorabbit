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
= What is PicoRabbit?

A tiny presentation device inspired by
Rabbit, running on Raspberry Pi Pico 2.

* Realtime digital video output
* Slide engine built on mruby VM
* Drawing: text, image, shapes

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

Pico 2 features more powerful chip RP2350
Great for real-time graphics
and running the full mruby VM

* 520kB of SRAM (2x the previous Pico!)
  Enough for a 640x480 screen
  and the full mruby VM, not just mruby/c

* HSTX interface for DVI output
  Enables direct DVI signal generation

= Running mruby on RP2350

Most steps can be based on PicoRuby.

1. Write build_config.rb

2. Build the mruby with mrbgems

3. Compile Ruby script with mrbc

4. Load and run in main.c

= Write build_config.rb

It can use the same setup as PicoRuby

MRuby::CrossBuild.new("mruby-cortex-m33") do |conf|
  conf.toolchain :gcc
  conf.host_target = "arm-none-eabi"
  conf.cc.command = "#{conf.host_target}-gcc"
  conf.linker.command = "#{conf.host_target}-ld"
  conf.linker.flags << "-static"
  conf.archiver.command = "#{conf.host_target}-gcc-ar"

  conf.cc.flags.flatten!
  conf.cc.flags << "-O3"
  conf.cc.flags << "-mcpu=cortex-m33"
  conf.cc.flags << "-mthumb"
  conf.cc.flags << "-ffunction-sections"
  conf.cc.flags << "-fdata-sections"

  conf.cc.flags << "-fno-strict-aliasing"
  conf.cc.flags << "-fno-unroll-loops"

= Mrbgems




= Universal Mrbgems?


=


TEXT

margin_x = 4
margin_y = 20

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
button = GPIO.new(8, GPIO::PULL_UP)
press = false
count = 0

start_time = time_usec_32
f = 0
loop do
  elapsed = time_usec_32 - start_time

  if button.read == 0
    press = true
  else
    if press
      count += 1
      press = false
    end
  end

  background COLOR_WHITE
  border_y = 6
  draw_rect 0, 0, 320, border_y, COLOR_RUBYKAIGI_RED
  draw_rect 0, 240-border_y, 320, 240-border_y, COLOR_RUBYKAIGI_RED

  draw_slide(slides[count])

  if count == 0
    draw_image :rubykaigi2025, 0, 0
  end

  if count == 1
    shape_y = 190
    draw_rotating_line(f, 320/2, shape_y, 40, 0.02, COLOR_RUBYKAIGI_GREEN)
    draw_rect 80-20, shape_y-20, 40, 40, (f%255)
    draw_image :usagi, 320-80, shape_y - 10, -1 * f.to_f
  end

  kame_x = (elapsed / 1000) * (320-24)  / (300 * 1000)
  draw_image :kame, kame_x, 240-24-6
  usagi_x = (count / (slides.size - 1).to_f) * (320-24)
  draw_image :usagi, usagi_x, 240-24-6

  f += 1
  commit
end
