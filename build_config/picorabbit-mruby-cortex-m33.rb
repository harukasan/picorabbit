mrubygems_dir = File.expand_path(File.join(File.dirname(__FILE__), "..", "vendor", "mruby", "mrbgems"))
local_gems_dir = File.expand_path(File.join(File.dirname(__FILE__), "..", "mrbgems"))

MRuby::CrossBuild.new("picorabbit-mruby-cortex-m33") do |conf|
  conf.toolchain :gcc
  conf.host_target = "arm-none-eabi"
  conf.cc.command = "#{conf.host_target}-gcc"
  conf.linker.command = "#{conf.host_target}-ld"
  conf.linker.flags << "-static"
  conf.archiver.command = "#{conf.host_target}-gcc-ar"

  conf.cc.flags.flatten!
  conf.cc.flags << "-mcpu=cortex-m33"
  conf.cc.flags << "-mthumb"
  conf.cc.flags << "-ffunction-sections"
  conf.cc.flags << "-fdata-sections"

  conf.cc.flags << "-fno-strict-aliasing"
  conf.cc.flags << "-fno-unroll-loops"
  conf.cc.flags << "-mslow-flash-data"

  conf.cc.flags << "-fshort-enums"

  conf.cc.flags << "-Wall"
  conf.cc.flags << "-Wno-format"
  conf.cc.flags << "-Wno-unused-function"
  conf.cc.flags << "-Wno-maybe-uninitialized"

  conf.gembox "stdlib"

  conf.gem File.join(local_gems_dir, "mruby-pico-print")
  conf.gem File.join(mrubygems_dir, "mruby-compiler")

end
