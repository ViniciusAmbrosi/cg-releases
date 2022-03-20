# Setup
1. Right click solution > Configuration/Properties
2. Go to C/C++ > General
3. Change additional include directories to point to include folder under glfw and glad
4. Change additional include directories to point to include folder glm
5.  Right click solution > Configuration/Properties
6.  Go to Linker > General
7.  Change Additional Library Directorries to point to GLFW lib folder for the vs version
8.  Go to Linker > Input
9.  Change Additional Dependencies by adding glfw3.lib and glfw3dll.lib

References:  
https://learnopengl.com/Getting-started/Creating-a-window  
http://www.mcihanozer.com/tips/setting-up-libraries/setting-up-glfw-for-visual-studio/  
https://www.wikihow.com/Set-Up-OpenGL-GLFW-GLEW-GLM-on-a-Project-with-Visual-Studio  

