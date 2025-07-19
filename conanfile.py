from conan import ConanFile
from conan.tools.cmake import CMake, cmake_layout
from conan.tools.build import check_min_cppstd

class AdsConanFile(ConanFile):
	name = "snd"
	version = "1.0.0"
	license = "MIT"
	author = "ColugoMusic"
	url = "https://github.com/colugomusic/blink"
	settings = "os", "compiler", "build_type", "arch"
	generators = "CMakeDeps", "CMakeToolchain"
	exports_sources = "CMakeLists.txt", "include/*", "lib/*"

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
		pass
	
	def package_info(self):
		self.cpp_info.includedirs = ["include", "lib/blink"]