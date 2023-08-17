from conan import ConanFile
from conan.tools.cmake import CMakeToolchain, CMake, cmake_layout


class ClickConan(ConanFile):
    name = "click"
    version = "0.1.38"

    # Optional metadata
    license = "Apache License Version 2.0"
    author = "Algoryx Simulation <contact@algoryx.se>"
    url = "https://github.com/algoryx/click-mirror"
    description = "Click adds the low latency communication you need to let your controller control your robots in an Algoryx Dynamics simulation like if they were real robots."
    topics = ("networking", "robotics", "simulation")

    # Binary configuration
    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False], "fPIC": [True, False]}
    default_options = {"shared": False, "fPIC": True}

    generators = "cmake_find_package"

    def export_sources(self):
        # Sources are located in the same place as this recipe, copy them to the recipe
        self.copy("CMakeLists.txt", src="conan", keep_path=False)
        self.copy("ClickConfig.cmake")
        self.copy("src/*")
        self.copy("include/*")
        # For now, we copy the generated protobuf sources from oos directory - should be it's own recipe later
        self.copy("Messaging.pb.h", src="../../oos/protobuf-gen/", dst="include", keep_path=False)
        self.copy("Messaging.pb.cc", src="../../oos/protobuf-gen/", dst="src", keep_path=False)

    def requirements(self):
        self.requires("protobuf/3.21.9", private=False)
        self.requires("zmqpp/4.2.0", private=False)

    def config_options(self):
        if self.settings.os == "Windows":
            del self.options.fPIC

    def layout(self):
        cmake_layout(self)

    def generate(self):
        tc = CMakeToolchain(self, generator="Ninja")
        tc.generate()

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def package(self):
        cmake = CMake(self)
        cmake.install()

    def package_info(self):
        postfix = ""
        if self.settings.build_type == "Debug":
            postfix += "-d"
        self.cpp_info.libs = ["click" + postfix]
