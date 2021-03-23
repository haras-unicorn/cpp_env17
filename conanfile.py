from conans import ConanFile, CMake
import os


class Env(ConanFile):
    name = "Env"
    description = "Environment for all of my C++ projects."
    topics = ("environment", "test", "benchmark")
    settings = "os", "compiler", "build_type", "arch"
    requires = "benchmark/1.5.2", "doctest/2.4.4"
    generators = "cmake"
    license = "MIT"

    def build(self):
        if os.getenv("clang-cl"):
            cmake = CMake(self,
                          generator="Visual Studio 16 2019",
                          toolset="ClangCL")
        else:
            cmake = CMake(self, generator="Ninja")

        cmake.configure()
        cmake.build()
