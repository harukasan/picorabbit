module PicoRabbit
  Slide = Struct.new(:title, :elements)
  TextElement = Struct.new(:text, :x, :y, :color)

  def self.draw_slide(slide)
    TextBuf.clear
    x = (45 - slide.title.size) / 2
    TextBuf.write(x, 1, slide.title, 7)
    slide.elements.each { |e| TextBuf.write(e.x, e.y, e.text, e.color) }
  end

end