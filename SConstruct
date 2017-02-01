# Create a production environment.
ENV = Environment(
  CCFLAGS=['-O2', '-Wall', '-Werror'],
  CPPFLAGS=['-Wall', '-Werror'],
  LIBPATH=['.']
)

# Build the library.
ENV.Object('src/jitter_buffer.c')
ENV.Library('jitterbuffer', ['src/jitter_buffer.o'])

# Build the unit tests.
ENV.Object('tests/jitter_buffer_tests.cpp')
ENV.Object('tests/test_runner.cpp')
ENV.Program(
  [
    'tests/test_runner.o',
    'tests/jitter_buffer_tests.o',
  ],
  LIBS=['gtest', 'pthread', 'jitterbuffer']
)
