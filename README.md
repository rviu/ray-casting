# Ray Casting

## Description

A Wolfenstein 3D style raycaster written in C++ that features image textures and movement around the map. QuickCG library is used for drawing graphics on the screen.

The ray caster in action:

![Ray caster](raycast.gif)

## Table of Contents

- [Installation](#installation)
- [Usage](#usage)
- [Credits](#credits)
- [License](#license)

## Installation

Follow the steps below to install the ray caster:

1. Clone the repository

```shell
git clone https://github.com/rviu/ray-casting.git
```

2. Run make inside the project directory

```shell
cd ray-casting
make
```

## Usage

#### Running the ray caster

To run the ray caster, run the following command in the project directory after installation:

```shell
./bin/main
```

This will open the world in a new window.

#### Controls

- `W`: Move forward
- `S`: Move backward
- `A`: Rotate left
- `D`: Rotate right

## Credits

[_Lode's Computer Graphics Tutorial: Raycasting_](https://lodev.org/cgtutor/raycasting.html)

[_QuickCG_](https://lodev.org/quickcg/)

## License

MIT License
