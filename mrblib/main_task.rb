puts "main_task.rb"

include PicoRabbit::Draw

textbuf = TextBuf.new

title = "Hello, world!"
textbuf.write(0, 1, title, COLOR_BLACK)


start_time = time_usec_32
loop do
  elapsed = time_usec_32 - start_time

  background COLOR_WHITE
  draw_rect 0, 0, 320, 10, COLOR_RED

  draw_rect 0, 240-10, 320, 240-10, COLOR_RED

  # draw_text "Hello, world!" + i.to_s, 0, 0, COLOR_WHITE
  kame_x = (elapsed / 1000) * (320-24)  / (300 * 1000)
  draw_rect kame_x, 240-24, 24, 24, COLOR_GREEN

  textbuf.render()
  commit
end
