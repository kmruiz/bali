doxygen = find_program('doxygen', required: false)

if doxygen.found()
  doxyfile = configure_file(
    input: 'Doxyfile.in',
    output: 'Doxyfile',
    configuration: {
      'PROJECT_NAME': 'bali',
      'OUTPUT_DIR': meson.current_build_dir(),
      'INPUT_DIRS': (
        (meson.global_source_root() / 'subprojects' / 'utilities' / 'include ') +
        (meson.global_source_root() / 'subprojects' / 'tokenizer' / 'include ') 
      )
    }
  )

  run_target(
    'docs',
    command: [doxygen, doxyfile]
  )
else
  message('Doxygen not found, "docs" target will not be available')
endif
