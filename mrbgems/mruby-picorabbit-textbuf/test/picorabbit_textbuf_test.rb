
assert('PicoRabbit::TextBuf.set/get') do
  PicoRabbit::TextBuf.set(0, 0, 'A'.ord | (7 << 8))
  val = PicoRabbit::TextBuf.get(0, 0)
  
  char = val & 0xFF
  color = (val >> 8) & 0xFF
  
  assert_equal 'A'.ord, char
  assert_equal 7, color
end

assert('TextBuf.write helper') do
  PicoRabbit::TextBuf.write(1, 1, "Hi", 6)

  val0 = PicoRabbit::TextBuf.get(1, 1)
  val1 = PicoRabbit::TextBuf.get(2, 1)

  assert_equal 'H'.ord | (6 << 8), val0
  assert_equal 'i'.ord | (6 << 8), val1
end