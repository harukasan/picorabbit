puts "Hello, world!"

include PicoRabbit::Draw

i = 0
loop do
	line_y = 0
	while line_y < 480
		background COLOR_BLUE
		draw_text "Hello from Ruby!"
  	commit_line line_y
  	line_y += 1
	end
end
