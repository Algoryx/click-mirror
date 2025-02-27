from conan import ConanFile
from conan.tools.cmake import CMakeToolchain, CMake, cmake_layout, CMakeDeps
from conan.tools.files import copy
from conan.tools.scm import Version

class ClickConan(ConanFile):
    name = "click"
    version = "0.5.6"

    license = "Apache-2.0"
    author = "Algoryx Simulation <contact@algoryx.se>"
    url = "https://github.com/algoryx/click-mirror"
    description = "Click adds low latency communication for controllers communicating with an Algoryx Dynamics simulation."
    topics = ("networking", "robotics", "simulation")
    # To solve revision constency across OS:s (Win CRLF vs *nix LF) we use SCM revision mode
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
        self.requires("cppzmq/4.10.0", visible=True)

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

    def package_id(self):
        if self.info.settings.compiler == "apple-clang" and Version(str(self.info.settings.compiler.version)) >= "7.0":
            self.info.settings.compiler.version = "AppleClang above 7.0"
            del self.info.settings.os.version
