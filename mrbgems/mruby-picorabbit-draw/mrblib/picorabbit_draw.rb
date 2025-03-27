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

    def draw_text(str)
      _c_draw_text_with_color(str.to_s, 0, 0, COLOR_WHITE)
    end
  end
end
