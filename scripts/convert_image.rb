require 'chunky_png'
require 'fileutils'

def rgb_to_rg332(r, g, b)
  r3 = (r >> 5) & 0x07
  g3 = (g >> 5) & 0x07
  b2 = (b >> 6) & 0x03
  (r3 << 5) | (g3 << 2) | b2
end

# Pack 8 alpha values into a single byte mask
def pack_alpha_mask(alpha_values)
  mask = 0
  alpha_values.each_with_index do |alpha, i|
    mask |= (alpha >= 128 ? 1 : 0) << i
  end
  mask
end

# Check if image has any transparent pixels
def has_transparency?(image)
  image.height.times do |y|
    image.width.times do |x|
      rgba = image[x, y]
      alpha = ChunkyPNG::Color.a(rgba)
      return true if alpha < 1
    end
  end
  false
end

if ARGV.size < 2
  puts "Usage: #{$0} output.h input1.png [input2.png ...]"
  exit 1
end

output_file = ARGV.shift
input_files = ARGV

File.open(output_file, 'w') do |f|
  f.puts "#ifndef IMAGE_DATA_H"
  f.puts "#define IMAGE_DATA_H"
  f.puts

  input_files.each do |input_file|
    base_name = File.basename(input_file, ".*")
    safe_name = base_name.gsub(/[^0-9A-Za-z_]/, '_')

    image = ChunkyPNG::Image.from_file(input_file)
    width  = image.width
    height = image.height
    has_alpha = has_transparency?(image)

    f.puts "// #{input_file} -> RGB332"
    f.puts "static const unsigned char image_data_#{safe_name}[] = {"

    height.times do |y|
      row_data = []
      width.times do |x|
        rgba = image[x, y]
        r = ChunkyPNG::Color.r(rgba)
        g = ChunkyPNG::Color.g(rgba)
        b = ChunkyPNG::Color.b(rgba)
        rgb332 = rgb_to_rg332(r, g, b)
        row_data << sprintf("0x%02X", rgb332)
      end
      f.puts "    " + row_data.join(", ") + ","
    end

    f.puts "};"
    f.puts

    # Generate 1-bit transparency mask only if image has transparency
    if has_alpha
      f.puts "// #{input_file} -> Transparency mask (1 bit per pixel)"
      f.puts "static const unsigned char mask_data_#{safe_name}[] = {"

      height.times do |y|
        row_data = []
        (0...width).step(8) do |x|
          alpha_values = []
          8.times do |i|
            if x + i < width
              rgba = image[x + i, y]
              alpha = ChunkyPNG::Color.a(rgba)
              alpha_values << alpha
            else
              alpha_values << 0  # Padding
            end
          end
          mask_byte = pack_alpha_mask(alpha_values)
          row_data << sprintf("0x%02X", mask_byte)
        end
        f.puts "    " + row_data.join(", ") + ","
      end

      f.puts "};"
    end

    f.puts "#define IMAGE_WIDTH_#{safe_name.upcase} #{width}"
    f.puts "#define IMAGE_HEIGHT_#{safe_name.upcase} #{height}"
    f.puts
  end

  f.puts "#endif"
end
