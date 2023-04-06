# Vulkan Engine Example Project
### This project is a barebones example of how to use [this](https://github.com/bmpringle/VulkanEngine) graphics engine I wrote.
Essentialy, this is how it works:
1. Create a git repository and add [this](https://github.com/bmpringle/VulkanEngine) to your submodules
2. Write some code using the engine and write some kind of build script for your code. You can look at the provided makefile for an example of a very simple way to do that.
3. run the VulkanEngine/build.py python script from your project directory and figure out what inputs are needed to each of the prompts to properly build and deploy your code.
    - This means that when it asks you for executable path, executable name, app name, etc., you should figure out what these values should be.
4. Put them all one after another in a text file like in build_settings.txt, with a newline at the end.
5. Use a script like compile_all.sh to pipe your saved inputs to build.py into the python script, so that all you have to do to compile your code is run sh compile_all.sh
6. On subsequent runs if you don't want to recompile all VulkanEngine dependencies, you can run the compile.sh script which is exactly the same, but passes CBSP instead of A to VulkanEngine/build.py in order to skip the dependency compilation step.