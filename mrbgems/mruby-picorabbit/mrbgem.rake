MRuby::Gem::Specification.new('mruby-picorabbit') do |spec|
    spec.license = 'MIT'
    spec.authors = 'Shunsuke Michii'
    spec.summary = 'PicoRabbit is a tiny presentation tool'
    spec.version = '0.0.1'

    spec.add_dependency 'mrb-picorabbit-textbuf'
    spec.add_dependency 'mrb-picorabbit-draw'
end
