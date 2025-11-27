project('bali_cli', 'c')

include = include_directories('include')

bali_cli_bin = executable(
  'bali_cli',
  'src/bali_cli.c',
  include_directories: include,
  dependencies: [
    dependency('bali_tokenizer'),
    dependency('bali_utilities'),
    dependency('bali_parser'),
    dependency('bali_vm'),
  ]
)
