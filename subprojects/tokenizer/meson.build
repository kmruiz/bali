project('bali_tokenizer', 'c')

include = include_directories('include')

bali_tokenizer_lib = library(
  'bali_tokenizer',
  'src/bali_tokenizer.c',
  include_directories: include,
  dependencies: [
    dependency('bali_utilities')
  ]
)

bali_tokenizer_tests = executable(
  'bali_tokenizer_test',
  'test/bali_tokenizer.test.c',
  include_directories: include,
  dependencies: [
    dependency('check'),
    dependency('bali_utilities')
  ],
  link_with: [
    bali_tokenizer_lib,
  ],
  c_args: [ '-DBALI_USE_DCHECK' ]
)

test('bali_tokenizer', bali_tokenizer_tests)
