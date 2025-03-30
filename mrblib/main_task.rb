puts "Hello, world!"

include PicoRabbit::Draw

i = 0
loop do
  puts i.to_s
  i += 1

  background COLOR_MAGENTA
  draw_text "Hello, world!" + i.to_s, 0, 0, COLOR_WHITE
  commit
end
