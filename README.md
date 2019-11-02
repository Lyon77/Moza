# Moza
Moza Engine modeled after on the Hazel Engine from TheCherno

# How to Run
Currently, the engine only supports Windows x64 as Moza is officially untested on other development environments.. If you need to recreate the solution file, run 'GenerateProjects.bat.' Takes advantage of 'Premake'
Visual Studio 2017 or 2019 is recommended.

You can clone the repository to a local destination using git:

git clone --recursive https://github.com/Lyon77/Moza

Make sure that you do a --recursive clone to fetch all of the submodules!


# Architecture
## Sandbox
Main Executable

## Moza
Static Library (was a dll, but eventually switched to static due to optimization reasons)

## Event System
Currently, I am using a blocking event system.
Event System takes in the (x, y) from the window and creates an event that sends data to a callback that sends data to the application.
The Window should have no notion of the application.

## Layers
Ordered list of layers that determine how things are drawn as well as events and update logic. Top == Last for Drawing and First for Events
Use Case - In the core while loop, each layer will be updated based on the ordered list

## Rendeing
Will be an abstraction layer. This is so that the api is flexible enough to use Vulkan or DirectX in the future

Renderer (API/platform graphics) | Render API (API/platform specific)
------------ | -------------
2D & 3D Renderer | Render Context
Scene Graph | Swap Chain
Sorting | Framebuffer
Culling | Vertex Buffer
Materials | Index Buffer
LOD | Texture
Animation | Shader
Camera | States
VFX | Pipelines
PostFX | Render Passes
Relections, Ambient occlusion, etc. | 


# Main features to come:
Fast 2D rendering (UI, particles, sprites, etc.)
High-fidelity Physically-Based 3D rendering (this will be expanded later, 2D to come first)
Support for Mac, Linux, Android and iOS
Native rendering API support (DirectX, Vulkan, Metal)
Fully featured viewer and editor applications
Fully scripted interaction and behavior
Integrated 3rd party 2D and 3D physics engine
Procedural terrain and world generation
Artificial Intelligence
Audio system

# 3rd Party Libraries
## stdlog
https://github.com/gabime/spdlog
## Premake
https://github.com/premake
## GLFW
https://github.com/TheCherno/glfw (forked from GLFW)
## GLAD (not forked)
https://glad.dav1d.de/
## imgui
https://github.com/TheCherno/imgui