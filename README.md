# Moza
Moza Engine modeled after on the Hazel Engine from TheCherno

# How to Run
Currently, the engine only supports Windows x64. To run it, run 'GenerateProjects.bat.' Takes advantage of 'Premake'

# Architecture
## Sandbox
Main Executable

## Moza
dll

## Event System
Currently, I am using a blocking event system.
Event System takes in the (x, y) from the window and creates an event that sends data to a callback that sends data to the application.
The Window should have no notion of the application.

## Layers
Ordered list of layers that determine how things are drawn as well as events and update logic. Top == Last for Drawing and First for Events
Use Case - In the core while loop, each layer will be updated based on the ordered list

# 3rd Party Libraries
## stdlog
https://github.com/gabime/spdlog
## Premake
https://github.com/premake
## GLFW
https://github.com/TheCherno/glfw (forked from GLFW)
