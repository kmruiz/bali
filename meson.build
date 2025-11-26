project(
  'bali', 'c',
  default_options: [
    'c_std=c23',
    'warning_level=3',
    'werror=true'
  ],
)

bali_dcheck_opt = get_option('bali_dcheck')
buildtype = get_option('buildtype')

# For debug-y builds, we want dchecks unless explicitly disabled
is_debug_like = buildtype == 'debug' or buildtype == 'debugoptimized'

bali_dcheck_enabled = (
  bali_dcheck_opt.enabled() or
  (bali_dcheck_opt.auto() and is_debug_like)
)

if bali_dcheck_enabled
  add_project_arguments('-DBALI_USE_DCHECK', language: 'c')
endif

subdir('docs')

bali_utilities = subproject('utilities')
bali_tokenizer = subproject('tokenizer')
bali_vm        = subproject('vm')
bali_parser    = subproject('parser')
