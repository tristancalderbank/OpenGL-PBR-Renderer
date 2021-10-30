# NanoEngine

A toy rendering engine written in C++ using OpenGL.

<img src="https://github.com/tristancalderbank/NanoEngine/blob/master/screenshots/pbr_first_implementation.png">

### Features
* PBR lighting with indirect image-based lighting (IBL)
* Cook-Torrance BRDF (based on Disney/Unreal 4 formulation)
* glTF 2.0 PBR model loading support
* HDRI skyboxes
* Separate post-processing render step with tonemapping/gamma correction
* ImGui for debugging/tweaking parameters
* Free flight camera (WASD + mouse)

### Dependencies

* glad, OpenGL loader for loading function pointers at runtime
* glfw, for creating OpenGL context/window, handling IO
* glm, OpenGL math utility library
* assimp, for asset loading
* stb_image, for loading images
* imgui, for the debug/tweaking UI

### References

* Learn OpenGL: https://learnopengl.com
* Generating low discrepancy points on a hemisphere: http://holger.dammertz.org/stuff/notes_HammersleyOnHemisphere.html#sec-SourceCode
