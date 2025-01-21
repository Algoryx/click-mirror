from conan import ConanFile
from conan.tools.cmake import CMakeToolchain, CMake, cmake_layout, CMakeDeps
from conan.tools.files import copy

class ClickConan(ConanFile):
    name = "click"
    version = "0.5.1"

    license = "Apache-2.0"
    author = "Algoryx Simulation <contact@algoryx.se>"
    url = "https://github.com/algoryx/click-mirror"
    description = "Click adds the low latency communication you need to let your controller control your robots in an Algoryx Dynamics simulation like if they were real robots."
    topics = ("networking", "robotics", "simulation")

    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False], "fPIC": [True, False]}
    default_options = {"shared": False, "fPIC": True}

    def export_sources(self):
        copy(self, "CMakeLists.txt", src="conan", dst=self.export_sources_folder)
        copy(self, "ClickConfig.cmake", src=".", dst=self.export_sources_folder)
        copy(self, "shared_conf/*", src=".", dst=self.export_sources_folder)
        copy(self, "src/*", src=".", dst=self.export_sources_folder)
        copy(self, "include/*", src=".", dst=self.export_sources_folder)
        # For now, we copy the generated protobuf sources from oos directory - should be it's own recipe later
        copy(self, "Messaging.pb.h", src="../../oos/protobuf-gen/", dst=f"{self.export_sources_folder}/include", keep_path=False)
        copy(self, "Messaging.pb.cc", src="../../oos/protobuf-gen/", dst=f"{self.export_sources_folder}/src", keep_path=False)

    def requirements(self):
        self.requires("protobuf/5.27.0", visible=True)
        if self.settings.os == "Macos":
            # Hardcode libsodium version used by click->zmq as workaround for clang16 not compiling libsodium as of 2024-09-17
            self.requires("libsodium/1.0.18", override=True)
        self.requires("zmqpp/4.2.0", visible=True)

    def config_options(self):
        if self.settings.os == "Windows":
            del self.options.fPIC

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
        postfix = "_d" if self.settings.build_type == "Debug" else ""
        self.cpp_info.libs = ["click" + postfix]
        self.cpp_info.includedirs = ["include"]
        self.cpp_info.libdirs = ["lib"]
