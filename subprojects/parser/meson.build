project('bali_parser', 'c')

include = include_directories('include')

bali_parser_lib = library(
  'bali_parser',
  'src/bali_parser.c',
  include_directories: include,
  dependencies: [
    dependency('bali_utilities')
  ]
)

bali_parser_tests = executable(
  'bali_parser_test',
  'test/bali_parser.test.c',
  include_directories: include,
  dependencies: [
    dependency('check'),
    dependency('bali_utilities')
  ],
  link_with: [
    bali_parser_lib,
  ],
  c_args: [ '-DBALI_USE_DCHECK' ]
)

test('bali_parser', bali_parser_tests)
