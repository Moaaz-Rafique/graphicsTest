#pragma once

#include <string>
#include <GLFW/glfw3.h>

namespace customUtils {
	class FileDialogs
	{
	public:
		static std::string OpenFile(const char* filter, GLFWwindow* window);
		static std::string SaveFile(const char* filter, GLFWwindow* window);
	};
}
