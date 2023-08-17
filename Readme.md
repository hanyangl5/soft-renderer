# Soft Renderer

Soft Renderer is a cross-platform implementation of the GPU rendering pipeline on CPU.

![](figs/output_224.png)

## Feature List

- Support multiple platforms, Windows, Linux, and macOS.
- Model Loading: Supports load `.obj` model file and `.jpg`, `.png` texture resources.
- Vertex Processing: Vertex Shader, Clipping,  Perspective Division, Backface Culling.
- Rasterization: Rasterize triangles based on barycentric coordinates with perspective correction.
- Pixel Shading: Implements the Phong reflection model, including ambient light, diffuse reflection, and specular reflection.
- Texture Filtering: Supports Bi-Linear texture filtering.

## Platform Support

| Platform      |       Clang        |        GCC         |        MSVC        |
| ------------- | :----------------: | :----------------: | :----------------: |
| Windows       | :heavy_check_mark: |        N/A         | :heavy_check_mark: |
| Linux(Ubuntu) | :heavy_check_mark: | :heavy_check_mark: |        N/A         |
| macOS | :heavy_check_mark: | N/A |        N/A         |

## Build From Source

before build the project, you need to install

- C++17 capable compiler
- CMake 3.10(or later)

then 

- Clone the project.
`git clone https://github.com/hanyangl5/soft-renderer.git`
- Generate project using cmake
`cmake . -B build -G <generator>`
- build `Rasterizer` and run

## License

[MIT](LICENSE) © hanyangl5