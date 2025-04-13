#!/usr/bin/env ruby

def rgb_to_rg332(r, g, b)
  r3 = (r >> 5) & 0x07
  g3 = (g >> 5) & 0x07
  b2 = (b >> 6) & 0x03
  (r3 << 5) | (g3 << 2) | b2
end

ARGF.each_line do |line|
  r, g, b = line.split.map(&:to_i)
  rg332_value = rgb_to_rg332(r, g, b)
  puts "%02X" % rg332_value
end
