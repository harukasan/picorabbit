module PicoRabbit
  module TextBuf
    TEXT_COLS = 360 / 8
    TEXT_ROWS = 240 / 8

    def self.write(x, y, str, color = 7)
      i = 0
      while i < str.size
        set(x + i, y, str.getbyte(i) | (color << 8))
        i += 1
      end
    end

    def self.clear(ch = ' '.ord | (0 << 8))
      TEXT_ROWS.times do |y|
        TEXT_COLS.times do |x|
          set(x, y, ch)
        end
      end
    end
  end
end