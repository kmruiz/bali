project('bali_vm', 'c')

include = include_directories('include')

bali_vm_lib = library(
  'bali_vm',
  'src/bali_vm.c',
  'src/__bali_vm/value.c',
  'src/__bali_vm/instruction.c',
  'src/__bali_vm/context.c',
  'src/__bali_vm/shard.c',
  include_directories: include,
  dependencies: [
    dependency('bali_utilities')
  ]
)

bali_vm = declare_dependency(link_with: bali_vm_lib, include_directories: include)
meson.override_dependency('bali_vm', bali_vm)

bali_vm_tests = executable(
  'bali_vm_test',
  'test/bali_vm.test.c',
  dependencies: [
    bali_vm,
    dependency('bali_utilities'),
    dependency('check'),
  ],
  c_args: [ '-DBALI_USE_DCHECK' ]
)

test('bali_vm', bali_vm_tests)
