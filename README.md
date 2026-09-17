# Computer Graphics Engine

A 3D graphics engine developed in **C++ and OpenGL** as part of a Computer Graphics course project.

The project includes a **3D primitive generator** and a **rendering engine** capable of loading scenes described in XML, applying hierarchical transformations, animations, lighting, materials and textures.

As a demonstration of the engine, the project includes a procedurally generated **Solar System scene** with planets, moons, Saturn's rings, an asteroid belt and an optional comet.

---

## Features

### 3D Model Generator

The `generator` application creates `.3d` geometry files containing vertices, normals, texture coordinates and indices.

Supported primitives:

* Plane
* Box
* Sphere
* Cone
* Cylinder
* Torus
* Bézier patches

Bézier surfaces can be generated from patch files with configurable tessellation levels.

---

### Rendering Engine

The `engine` application loads and renders XML scene descriptions using OpenGL.

Main features include:

* Hierarchical scene graph
* XML scene parsing
* Static transformations

  * Translation
  * Rotation
  * Scaling
* Time-based rotations
* Catmull-Rom curve translations
* Object alignment along Catmull-Rom curves
* Hierarchical transformations between groups
* Vertex Buffer Objects (VBO)
* Index Buffer Objects (IBO)
* Vertex normals
* Texture coordinates
* Texture mapping
* Material properties
* Lighting
* Back-face culling
* Depth testing
* Perspective camera
* Interactive camera movement

---

## Lighting

The engine supports multiple OpenGL light sources, including:

* Point lights
* Directional lights
* Spotlights

Models can define material properties such as:

* Diffuse
* Ambient
* Specular
* Emissive
* Shininess

---

## Textures

Texture mapping is supported using **stb_image**.

The project includes textures for the Solar System demonstration:

* Sun
* Mercury
* Venus
* Earth
* Moon
* Mars
* Jupiter
* Saturn
* Uranus
* Neptune
* Saturn's rings

---

## Solar System

A complete Solar System scene is included to demonstrate the capabilities of the engine.

The scene contains:

* The Sun
* Eight planets
* Multiple moons
* Saturn's rings
* An asteroid belt with procedurally generated asteroids
* Planetary rotation
* Orbital movement
* Textures
* Hierarchical transformations
* Optional comet trajectory

The XML configuration for the Solar System can be generated automatically through the scripts available in the `scripts/` directory.

---

## Building the Project

### Requirements

The project requires:

* C++17 compatible compiler
* CMake
* OpenGL
* GLUT / FreeGLUT
* GLEW

On Ubuntu, the required dependencies can typically be installed with:

```bash
sudo apt update
sudo apt install build-essential cmake freeglut3-dev libglew-dev libgl1-mesa-dev
```

### Compile

From the project root:

```bash
mkdir -p build
cd build
cmake ..
make -j
```

After compilation, two executables will be created:

```text
build/
├── generator
└── engine
```

---

## Using the Generator

General syntax:

```bash
./generator <primitive> <parameters> <output_file>
```

### Plane

```bash
./generator plane <length> <divisions> <output.3d>
```

Example:

```bash
./generator plane 10 10 plane.3d
```

### Box

```bash
./generator box <length> <divisions> <output.3d>
```

Example:

```bash
./generator box 2 4 box.3d
```

### Sphere

```bash
./generator sphere <radius> <stacks> <slices> <output.3d>
```

Example:

```bash
./generator sphere 1 20 20 sphere.3d
```

### Cone

```bash
./generator cone <radius> <height> <slices> <stacks> <output.3d>
```

Example:

```bash
./generator cone 1 2 20 10 cone.3d
```

### Cylinder

```bash
./generator cylinder <radius> <height> <slices> <output.3d>
```

Example:

```bash
./generator cylinder 1 2 20 cylinder.3d
```

### Torus

```bash
./generator torus <radius> <ring_radius> <stacks> <slices> <output.3d>
```

Example:

```bash
./generator torus 1 0.25 20 20 torus.3d
```

### Bézier Patch

```bash
./generator patch <patch_file> <tessellation_level> <output.3d>
```

Example:

```bash
./generator patch ../patches/teapot.patch 10 teapot.3d
```

---

## Running the Engine

The engine receives an XML scene description as its argument.

```bash
./engine <scene.xml>
```

Example:

```bash
./engine solar_system.xml
```

The XML file defines the scene configuration, including:

* Window size
* Camera
* Projection
* Lights
* Models
* Textures
* Materials
* Transformations
* Scene hierarchy
* Animations

---

## Camera Controls

The engine provides keyboard controls for navigating the 3D scene.

| Key | Action                                |
| --- | ------------------------------------- |
| `W` | Move forward                          |
| `S` | Move backward                         |
| `A` | Move left                             |
| `D` | Move right                            |
| `Q` | Rotate camera left around the target  |
| `E` | Rotate camera right around the target |
| `X` | Zoom in                               |
| `Z` | Zoom out                              |

---

## Animated Transformations

The engine supports transformations based on elapsed time.

### Rotation

Objects can continuously rotate around a specified axis.

Example:

```xml
<rotate time="10" x="0" y="1" z="0"/>
```

### Catmull-Rom Translation

Objects can follow a closed path defined by Catmull-Rom control points.

```xml
<translate time="10" align="true">
    <point x="0" y="0" z="0"/>
    <point x="10" y="0" z="0"/>
    <point x="10" y="0" z="10"/>
    <point x="0" y="0" z="10"/>
</translate>
```

When `align="true"` is enabled, the object orientation is automatically aligned with the direction of the curve.

---

## Generate the Solar System

The project provides a script that generates the required 3D models and creates the Solar System XML scene.

From the `scripts` directory:

```bash
./generate_solar_system.sh \
    <sun_scale> \
    <planet_scale> \
    <distance_scale> \
    <angle_variation> \
    <time_scale> \
    <comet_flag>
```

For example:

```bash
cd scripts

./generate_solar_system.sh 1.0 1.0 1.0 20 1.0 true
```

The script generates the required models and stores the resulting resources in the `build/` directory.

The parameters allow the Solar System visualization to be adjusted without changing the engine itself.

---

## Architecture

The project is divided into two main components.

### Generator

Responsible for generating geometry and exporting it to the custom `.3d` format.

```text
Primitive
├── Plane
├── Box
├── Sphere
├── Cone
├── Cylinder
└── Torus
```

Bézier patches are handled separately through the patch tessellation system.

### Engine

Responsible for:

```text
XML
 │
 ▼
Parser
 │
 ▼
Scene
 │
 ▼
Groups
 ├── Transformations
 ├── Models
 └── Subgroups
 │
 ▼
OpenGL Rendering
```

The hierarchical group structure allows child objects to inherit transformations from their parent groups, making it possible to model systems such as planets and moons naturally.

---

## Authors

Developed by **TP8**:

* Juliana Sofia Vaz da Silva
* Sofia Beatriz Miranda Couto
* Soraia Filipa Ribeiro Pereira

---

## Academic Context

Project developed for the **Computer Graphics** course during the **2025/2026 academic year**.
