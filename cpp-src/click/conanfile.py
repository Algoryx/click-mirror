from conan import ConanFile
from conan.tools.cmake import CMakeToolchain, CMake, cmake_layout, CMakeDeps
from conan.tools.files import copy

class ClickConan(ConanFile):
    name = "click"
    version = "0.5.4"

    license = "Apache-2.0"
    author = "Algoryx Simulation <contact@algoryx.se>"
    url = "https://github.com/algoryx/click-mirror"
    description = "Click adds low latency communication for controllers communicating with an Algoryx Dynamics simulation."
    topics = ("networking", "robotics", "simulation")
    # For unknown cause, the recipe hash differs on e.g. Linux and MacOS, so use SCM revision mode instead
    revision_mode = "scm"

    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False], "fPIC": [True, False]}
    default_options = {"shared": False, "fPIC": True}

    def export_sources(self):
        copy(self, "CMakeLists.txt", src="conan", dst=self.export_sources_folder)
        copy(self, "ClickConfig.cmake", src=".", dst=self.export_sources_folder)
        copy(self, "shared_conf/*", src=".", dst=self.export_sources_folder)
        copy(self, "src/*", src=".", dst=self.export_sources_folder)
        copy(self, "include/*", src=".", dst=self.export_sources_folder)

    def requirements(self):
        self.requires("protobuf/5.27.0@algoryx/stable", visible=True)
        self.requires("zmqpp/4.2.0@algoryx/stable", visible=True)

    def config_options(self):
        if self.settings.os == "Windows":
            del self.options.fPIC

    def configure(self):
        if self.options.shared:
            self.options.rm_safe("fPIC")

    def layout(self):
        cmake_layout(self)

    def generate(self):
        tc = CMakeToolchain(self)
        if self.settings.os in ("Macos", "Linux"):
            tc.generator = "Ninja"
        tc.generate()

        deps = CMakeDeps(self)
        deps.generate()

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def package(self):
        cmake = CMake(self)
        cmake.install()

    def package_info(self):
        postfix = "d" if self.settings.build_type == "Debug" else ""
        self.cpp_info.libs = ["click" + postfix]
        self.cpp_info.includedirs = ["include"]
        self.cpp_info.libdirs = ["lib"]
