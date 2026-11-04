project('partial_life', 'c',
  version: '0.0.1',
  default_options: [
    'c_std=c23',
    'warning_level=3',
    'optimization=2',
    'debug=true',
  ],
)

# Force meson to fetch all libs
#run_command('meson', 'subprojects', 'download', check: true)

cmake = import('cmake')

raylib_opts = cmake.subproject_options()
raylib_opts.set_install(false)
raylib_opts.add_cmake_defines({
  'BUILD_EXAMPLES': 'OFF',
  # Add other CMake options for Raylib, if needed
})

if host_machine.system() == 'emscripten'
  raylib_opts.add_cmake_defines({
    'PLATFORM': 'Web',
  })
else
  raylib_opts.add_cmake_defines({
    'PLATFORM': 'Desktop',
  })
endif

raylib_proj = cmake.subproject('raylib', options: raylib_opts)
raylib_dep = raylib_proj.dependency('raylib')

rini_dep = dependency('rini', fallback: 'rini')

sources = files('src/main.c')

exe_name = 'partial_life'

link_args = []
if host_machine.system() == 'emscripten'
  # Emscripten flags must go through link_args; Meson does not
  # have first-class syntax for the -s options.
  link_args += [
    '-sUSE_GLFW=3',
    '-sWASM=1',
    '-sASYNCIFY',
    '-sGL_ENABLE_GET_PROC_ADDRESS=1',
    '-sINITIAL_MEMORY=134217728',
    '-sSTACK_SIZE=8388608',
    '-sALLOW_MEMORY_GROWTH=1',
    #'--shell-file=' + (meson.project_source_root() / 'subprojects/raylib/src/minshell.html'),
    #'--preload-file=' + (meson.project_source_root() / 'resources'),
  ]
elif host_machine.system() == 'darwin'
  # raylib's pkg-config usually carries these, but listing them
  # explicitly makes the macOS link independent of pkg-config.
  link_args += [
    '-framework', 'OpenGL',
    '-framework', 'Cocoa',
    '-framework', 'IOKit',
    '-framework', 'CoreVideo',
    '-framework', 'QuartzCore',
    '-framework', 'Foundation',
  ]
endif

if host_machine.system() == 'emscripten'

  shell_path = meson.project_source_root() / 'subprojects/raylib/src/minshell.html'
  custom_target('partial_life_html',
    output: 'partial_life.html',
    command: [
      'bash', '-c',
      'sed \'s|{{{ SCRIPT }}}|<script src="partial_life.js"></script>|\' "@0@" > "@OUTPUT@"'
        .format(shell_path),
    ],
    build_by_default: true,
  )
endif

executable(exe_name,
  sources,
  dependencies: [rini_dep, raylib_dep],
  link_args: link_args,
  install: true,
)
