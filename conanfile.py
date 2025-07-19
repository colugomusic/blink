from conan import ConanFile
from conan.tools.cmake import CMake, cmake_layout
from conan.tools.build import check_min_cppstd
from conan.tools.files import copy
from os.path import join

class AdsConanFile(ConanFile):
	name = "blink"
	version = "1.0.0"
	license = "MIT"
	author = "ColugoMusic"
	url = "https://github.com/colugomusic/blink"
	settings = "os", "compiler", "build_type", "arch"
	generators = "CMakeDeps", "CMakeToolchain"
	exports_sources = "include/*", "lib/*"

	def layout(self):
		cmake_layout(self)

	def requirements(self):
		self.requires("ent/[>=0]")
		self.requires("madronalib/[>=0]")
		self.requires("snd/[>=0]")

	def validate(self):
		if self.settings.get_safe("compiler.cppstd"):
			check_min_cppstd(self, 20)

	def build(self):
		pass

	def package(self):
		copy(self, "*.h", src=join(self.source_folder, "include"), dst=join(self.package_folder, "include"))
		copy(self, "*", src=join(self.source_folder, "lib"), dst=join(self.package_folder, "lib"))
	
	def package_info(self):
		self.cpp_info.includedirs = ["include", "lib/blink"]