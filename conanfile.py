from conans import ConanFile, CMake


class Env(ConanFile):
    name = "Env"
    description = "Environment for all of my C++ projects."
    topics = ("environment", "test", "benchmark")
    settings = "os", "compiler", "build_type", "arch"
    requires = "benchmark/1.5.2", "doctest/2.4.4"
    generators = "cmake"
    license = "MIT"

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()
