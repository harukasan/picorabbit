puts "Hello, world!"

include PicoRabbit::Draw

loop do
  background COLOR_BLUE
  draw_text "Hello from Ruby!", 10, 10, COLOR_WHITE
  commit
end
