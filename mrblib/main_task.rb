puts "Hello, world!"


linebuffer = PicoRabbit::LineBuffer.new

# red
red = "\xe0" * linebuffer.width
blue = "\x03" * linebuffer.width

puts "Total heap: #{PicoMemoryUsage.total_heap}"
puts "Used heap: #{PicoMemoryUsage.used_heap}"
puts "Free heap: #{PicoMemoryUsage.free_heap}"

loop do
  linebuffer.add_line(red)
  linebuffer.add_line(blue)
end
