project('rini', 'c', version: '0.1')

rini_inc = include_directories('src')

rini_dep = declare_dependency(
  include_directories: rini_inc,
)

meson.override_dependency('rini', rini_dep)
