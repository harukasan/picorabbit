module PicoRabbit
  module Draw

    # Font size
    FONT_WIDTH = 8
    FONT_HEIGHT = 8

    # Color constants
    COLOR_BLACK = 0x00
    COLOR_WHITE = 0xFF
    COLOR_RED   = 0xE0
    COLOR_GREEN = 0x1C
    COLOR_BLUE  = 0x03
    COLOR_YELLOW = 0xFC
    COLOR_CYAN   = 0x1F
    COLOR_MAGENTA = 0xE3

    class TextBuf
      def write(x, y, str, color = COLOR_WHITE)
        str.each_char.with_index do |ch, i|
          set(x + i, y, ch.ord | (color << 8))
        end
      end

      def clear
        30.times do |y|
          45.times do |x|
            set(x, y, 0)
          end
        end
      end
    end

    Slide = Struct.new(:elements)
    TextElement = Struct.new(:text, :x, :y, :color)

    def draw_slide(slide)
      slide.elements.each do |e|
        case e
        when TextElement
          draw_text(e.text, e.x, e.y, e.color)
        end
      end
    end
  end
end
