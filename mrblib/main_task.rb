puts "main_task.rb"

COLOR_RUBYKAIGI_RED = 0xC0
COLOR_RUBYKAIGI_YELLOW = 0xF0
COLOR_RUBYKAIGI_GREEN = 0x28
COLOR_RUBYKAIGI_INDIGO = 0x01
COLOR_RUBYKAIGI_BROWN = 0x40

include PicoRabbit::Draw

text = <<~TEXT
= What is PicoRabbit?

A tiny presentation device
inspired by Rabbit
running on Raspberry Pi Pico 2.

* Realtime digital video output
* Inpremented slide engine on mruby VM
* Drawing functions: text,image,shapes

Available on GitHub:
https://github.com/harukasan/picorabbit

= Motivation

At RubyKaigi 2024, I saw 'Dreampresent"
running on Wii and Dreamcast.

And I'm using a keyboard that runs PRK Firmware.
So maybe, PicoRuby can runs a presentation too?
TEXT

margin_x = 10
margin_y = 10
elements = []
y = 0
text.each_line do |line|
  elements << TextElement.new(line, margin_x, y + margin_y, COLOR_BLACK)
  y += 10
end

slide = Slide.new(elements)

start_time = time_usec_32
loop do
  elapsed = time_usec_32 - start_time

  background COLOR_WHITE

  border_y = 6
  draw_rect 0, 0, 320, border_y, COLOR_RUBYKAIGI_RED
  draw_rect 0, 240-border_y, 320, 240-border_y, COLOR_RUBYKAIGI_RED

  draw_slide(slide)

  # draw_text "Hello, world!" + i.to_s, 0, 0, COLOR_WHITE
  kame_x = (elapsed / 1000) * (320-24)  / (300 * 1000)
  draw_rect kame_x, 240-24, 24, 24, COLOR_GREEN

  commit
end
