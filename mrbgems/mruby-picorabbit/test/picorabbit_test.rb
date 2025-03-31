assert('PicoRabbit::draw_slide') do
  s = PicoRabbit::Slide.new("Hello", [
    PicoRabbit::TextElement.new("* item", 2, 5, 6)
  ])
  PicoRabbit.draw_slide(s)
  
  val = TextBuf.get(2, 5)
  assert_equal '*'.ord | (6 << 8), val
end
