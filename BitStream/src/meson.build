subdir('bitStream')
subdir('encoder')

compress_exe = executable('compress.cpp.executable',
    sources:['compress.cpp'],
    dependencies : [input_dep, output_dep, encoder_dep, cxxopts_dep],
    install: true)

uncompress_exe = executable('uncompress.cpp.executable',
    sources: ['uncompress.cpp'],
    dependencies : [input_dep, output_dep, encoder_dep, cxxopts_dep],
    install: true)
