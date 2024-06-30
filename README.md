# Illumination_swap

## Overview
This project demonstrates a scene using openFrameworks 0.12.0 and OpenGL 3.3, showcasing shader and texture manipulations with a graphical user interface (GUI).

## Addons Used
- **ofxAssimpModelLoader**: Used for loading 3D models.
- **ofxGui**: Provides GUI components for user interaction.

## Shaders Supported
- **Color fill**
- **Lambert**
- **Phong**
- **Gouraud**
- **Goosh**
- **Cel**
- **Blinn_phong**
- **Normal Mapping (with and without texture perturbation)**
- **PBR**

## Features
- **Shader Selection**: Users can switch between different shader types.
- **Texture Settings**: GUI controls allow adjusting texture settings.
- **GUI Controls**: Provides options for changing colors, materials, and activating/deactivating textures.
- **Textures**: Utilizes textures sourced from Polyhaven.
- **Normal Perturbation**: A button to activate or deactivate normal perturbation, which changes the normal mapping shader behavior.

## Screenshots

### Color fill Shader
![color_fill_no_tex](bin/data/screenshots/color_fill_no_tex.png)
![color_fill_tex](bin/data/screenshots/color_fill_tex.png)

### Lambert Shader
![lambert_no_tex](bin/data/screenshots/lambert_no_tex.png)
![lambert_tex](bin/data/screenshots/lambert_tex.png)

### Phong Shader
![phong_no_tex](bin/data/screenshots/phong_no_tex.png)
![phong_tex](bin/data/screenshots/phong_tex.png)

### Gouraud Shader
![gouraud_no_tex](bin/data/screenshots/gouraud_no_tex.png)
![gouraud_tex](bin/data/screenshots/gouraud_tex.png)

### goosh Shader
![goosh_no_tex](bin/data/screenshots/goosh_no_tex.png)
![goosh_tex](bin/data/screenshots/goosh_tex.png)

### cel Shader
![cel_no_tex](bin/data/screenshots/cel_no_tex.png)
![cel_tex](bin/data/screenshots/cel_tex.png)

### Blinn_phong Shader
![blinn_phong_no_tex](bin/data/screenshots/blinn_phong_no_tex.png)
![blinn_phong_tex](bin/data/screenshots/blinn_phong_tex.png)

### Normal Mapping
#### Normal Mapping Without Texture
![Normal No Texture](bin/data/screenshots/normal_no_tex.png)

#### Normal Mapping With Texture
![Normal Texture](bin/data/screenshots/normal_tex.png)

#### Normal Mapping Perturbation
![Normal Perturb No Texture](bin/data/screenshots/normal_perturb_no_tex.png)
![Normal Perturb](bin/data/screenshots/normal_pertub.png)

### PBR Shader
![pbr_no_tex](bin/data/screenshots/pbr_no_tex.png)
![pbr_tex](bin/data/screenshots/pbr_tex.png)

## Installation and Usage
1. Clone the repository.
2. Ensure you have openFrameworks 0.12.0 and OpenGL 3.3 installed.
3. Install the required addons (`ofxAssimpModelLoader` and `ofxGui`).
4. Build and run the project.

