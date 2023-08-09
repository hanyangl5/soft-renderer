# Soft Renderer

[![ci](https://github.com/hanyangl5/soft-renderer/actions/workflows/ci.yml/badge.svg?branch=master)](https://github.com/hanyangl5/soft-renderer/actions/workflows/ci.yml)

## Project Overview

Soft Renderer is a basic implementation of a rendering pipeline, including model loading, geometric transformations, rasterization based on barycentric coordinates, Phong shading, and texture mapping.

![](figs/output_224.png)

## Feature Overview

- Model Loading: Supports various formats of 3D model files.
- Geometric Transformations: Implements translation, rotation, and scaling of models.
- Rasterization: Uses a method based on barycentric coordinates for rasterization.
- Phong Shading: Implements the Phong reflection model, including ambient light, diffuse reflection, and specular reflection.
- Texture Mapping: Supports 2D texture mapping.

## Build From Source

| Platform      |       Clang        |        GCC         |        MSVC        |
| ------------- | :----------------: | :----------------: | :----------------: |
| Windows       | :heavy_check_mark: |        N/A         | :heavy_check_mark: |
| Linux(Ubuntu) | :heavy_check_mark: | :heavy_check_mark: |        N/A         |

1. Clone the repository to your local machine.
2. Open the terminal in the root directory of the project.
3. Run the compilation command.
