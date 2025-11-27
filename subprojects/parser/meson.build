project('bali_parser', 'c')

include = include_directories('include')

bali_parser_lib = library(
  'bali_parser',
  'src/bali_parser.c',
  include_directories: include,
  dependencies: [
    dependency('bali_tokenizer'),
    dependency('bali_utilities'),
    dependency('bali_vm'),
  ]
)

bali_parser = declare_dependency(link_with: bali_parser_lib, include_directories: include)
meson.override_dependency('bali_parser', bali_parser)

bali_parser_tests = executable(
  'bali_parser_test',
  'test/bali_parser.test.c',
  dependencies: [
    bali_parser,
    dependency('bali_tokenizer'),
    dependency('bali_utilities'),
    dependency('bali_vm'),
    dependency('check'),
  ],
  c_args: [ '-DBALI_USE_DCHECK' ]
)

test('bali_parser', bali_parser_tests)
