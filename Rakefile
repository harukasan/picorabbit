
BUILD_CONFIG = File.join(File.dirname(__FILE__), "build_config", "picorabbit-mruby-cortext-m33.rb")

task :clean do
  FileUtils.cd("vendor/mruby") do
    sh "rake BUILD_CONFIG=#{BUILD_CONFIG} clean"
  end
end

