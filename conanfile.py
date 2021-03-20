from conans import ConanFile, CMake


class Env(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    requires = "benchmark/1.5.2", "doctest/2.4.4"
    generators = "cmake"
