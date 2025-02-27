import os
import sys
import shutil

from conan import ConanFile, __version__ as conan_version
from conan.tools.scm import Version
from conan.tools.cmake import CMake, cmake_layout, CMakeToolchain, CMakeDeps
from conan.tools.build import cross_building


class ClickTestConan(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False], "fPIC": [True, False]}
    default_options = {"shared": False, "fPIC": True}
    apply_env = False
    test_type = "explicit"
    # VirtualRunEnv is needed for Conan 1:
    generators = "VirtualRunEnv"

    def generate(self):
        tc = CMakeToolchain(self)
        if sys.platform in ('darwin', 'linux'):
            tc.generator="Ninja"
        tc.generate()
        deps = CMakeDeps(self)
        deps.generate()


    def requirements(self):
        self.requires(self.tested_reference_str)

        print(f"self.tested_reference_str: {self.tested_reference_str}")

        # Remove build directory to make sure that subsequent tests are not run with cached configuration
        build_dir = self.recipe_path / "build" # pylint: disable=no-member # recipe_path is a member
        self.output.info(f"Cleaning build directory: {build_dir} ")
        if build_dir and os.path.exists(build_dir):
            shutil.rmtree(build_dir)
            self.output.info(f"Cleaned build directory: {build_dir}")

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def layout(self):
        cmake_layout(self)

    def test(self):
        if not cross_building(self):
            cmd = os.path.join(self.cpp.build.bindirs[0], "example")
            self.run(cmd, env="conanrun")
