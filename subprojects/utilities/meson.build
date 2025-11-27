project('bali_utilities', 'c')

include = include_directories('include')

bali_utilities_lib = library(
  'bali_utilities',
  'src/bali_utilities/bump_arena.c',
  include_directories: include,
)

bali_utilities = declare_dependency(link_with: bali_utilities_lib, include_directories: include)
meson.override_dependency('bali_utilities', bali_utilities)
