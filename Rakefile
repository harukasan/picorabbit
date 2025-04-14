require 'rake/clean'

CLEAN.include('include/image.h')

desc 'Generate image.h'
task :image_h do
  images = FileList['images/**/*.png']
  output = File.join('include', 'image.h')
  sh "ruby scripts/convert_image.rb #{output} #{images.join(' ')}"
end

BUILD_CONFIG = File.join(File.dirname(__FILE__), "build_config", "picorabbit-mruby-cortext-m33.rb")

task :clean do
  FileUtils.cd("vendor/mruby") do
    sh "rake BUILD_CONFIG=#{BUILD_CONFIG} clean"
  end
end
