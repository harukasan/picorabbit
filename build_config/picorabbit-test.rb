local_gems_dir = File.expand_path(File.join(File.dirname(__FILE__), "..", "mrbgems"))

MRuby::Build.new do |conf|
  toolchain :gcc

  conf.gembox "stdlib"
  conf.gembox "stdlib-ext"

  conf.gem :core => "mruby-bin-mrbc"
  conf.gem :core => "mruby-bin-debugger"
  conf.gem :core => "mruby-bin-mirb"
  conf.gem :core => "mruby-bin-mruby"
  conf.gem :core => "mruby-bin-strip"
  conf.gem :core => "mruby-bin-config"

  conf.gem File.join(local_gems_dir, "mruby-picorabbit-textbuf")

  conf.enable_test
end