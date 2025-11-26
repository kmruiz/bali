project('bali_vm', 'c')

include = include_directories('include')

bali_vm_lib = library(
  'bali_vm',
  'src/bali_vm.c',
  'src/__bali_vm/value.c',
  include_directories: include,
  dependencies: [
    dependency('bali_utilities')
  ]
)

bali_vm_tests = executable(
  'bali_vm_test',
  'test/bali_vm.test.c',
  include_directories: include,
  dependencies: [
    dependency('check'),
    dependency('bali_utilities')
  ],
  link_with: [
    bali_vm_lib,
  ],
  c_args: [ '-DBALI_USE_DCHECK' ]
)

test('bali_vm', bali_vm_tests)
