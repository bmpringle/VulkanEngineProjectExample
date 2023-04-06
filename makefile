compile:
	clang++ -std=c++17 src/main.cpp -o test -I./VulkanEngine/include/Engine/ -I./VulkanEngine/glm/ -I./VulkanEngine/StringToText/freetype/include -I./VulkanEngine/StringToText/include -I./VulkanEngine/ -I./VulkanEngine/VulkanSDKMacOS/macOS/include/ -I ./VulkanEngine/include/ -L ./VulkanEngine/bin/rel/ -lVulkanEngineLib
clean:
	rm -rf deploy/*
	rm test