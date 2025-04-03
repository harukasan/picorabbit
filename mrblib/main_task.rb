puts "Hello, world! from main tasks"

include PicoRabbit::Draw

textbuf = TextBuf.new
textbuf.write(0, 0, "Hello, world! on the text buffer")

i = 0
loop do
  i += 1

  background COLOR_MAGENTA
  # draw_text "Hello, world!" + i.to_s, 0, 0, COLOR_WHITE
  draw_rect (i/10)%(320-24), 240-24, 24, 24, COLOR_GREEN


  textbuf.render()
  commit
end
