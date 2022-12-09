# OpenGL-PBR-Renderer

A physically-based rendering engine written in C++ using OpenGL.

Real-time video: https://www.youtube.com/watch?v=UCdlUd6DJs4

<img src="https://github.com/tristancalderbank/NanoEngine/blob/master/screenshots/pbr_first_implementation.png">

### Features
* PBR lighting with indirect image-based lighting (IBL)
* Cook-Torrance BRDF
* glTF 2.0 PBR model loading support
* Texture support: albedo, metallic, roughness, tangent-space normal, ao, emissive
* Bloom (Gaussian blur, blended from 6 mip levels for maximum spread)
* HDRI skyboxes
* Separate post-processing render step with tonemapping/gamma correction
* ImGui for debugging/tweaking parameters
* Free flight camera (WASD + mouse)
* Cross platform: Windows/MacOS

Metallic vs. Roughness

<img src="https://github.com/tristancalderbank/NanoEngine/blob/master/screenshots/metallicroughnesslabelled.png">

Bloom (Gaussian blur, blended from 6 mip levels for maximum spread)

<img src="https://github.com/tristancalderbank/NanoEngine/blob/master/screenshots/bloom.png">

### Dependencies

* glad, OpenGL loader for loading function pointers at runtime
* glfw, for creating OpenGL context/window, handling IO
* glm, OpenGL math utility library
* assimp, for asset loading
* stb_image, for loading images
* imgui, for the debug/tweaking UI

### References

* Models: https://github.com/KhronosGroup/glTF-Sample-Models
* HDRIs: http://www.hdrlabs.com/sibl/archive.html
* Learn OpenGL: https://learnopengl.com
* Image-based lighting: https://chetanjags.wordpress.com/2015/08/26/image-based-lighting/
* Generating low discrepancy points on a hemisphere: http://holger.dammertz.org/stuff/notes_HammersleyOnHemisphere.html#sec-SourceCode
