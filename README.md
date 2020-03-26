# Gazo-san
![logo](docs/assets/logo.png)

![Version](https://shields-staging.herokuapp.com/github/v/release/lifull-dev/Gazo-san) [![CircleCI](https://circleci.com/gh/lifull-dev/Gazo-san.svg?style=svg)](https://circleci.com/gh/lifull-dev/Gazo-san)

Detect difference between two images.

By separating each image into few parts, Gazo-san only detects difference on matched parts between two images. Optional shows decrease or increase part on two images.

## Quick Start
You can use Gazo-san with docker.

Get image from docker hub
```
docker pull lifullsetg/gazo-san
```

Clone this repository
```
git clone https://github.com/lifull-dev/Gazo-san.git
```

Go into the repository
```
cd Gazo-san
```

Execute Gazo-san by container
```
docker run --rm -it -v `pwd`:/run lifullsetg/gazo-san gazosan tests/images/test_image_new.png tests/images/test_image_old.png output
```

| Input | Input |
| :--: | :--: |
| test_image_old.png | test_image_new.png |
| <img src="tests/images/test_image_old.png" width="50%" /> | <img src="tests/images/test_image_new.png" width="50%" /> |

| Output |
| :--: |
| output_diff.png |
| <img src="https://user-images.githubusercontent.com/27658353/70491691-502c5f00-1b45-11ea-9bdc-8cf1f1189593.png" width="80%" > |


### With option --create-change-image
```
docker run --rm -it -v `pwd`:/run lifullsetg/gazo-san gazosan tests/images/test_image_new.png tests/images/test_image_old.png output --create-change-image
```

| Output | Output |
| :--: | :--: |
| output_delete.png | output_add.png |
| <img src="https://user-images.githubusercontent.com/27658353/70491689-4e629b80-1b45-11ea-98e9-f986bfbaae63.png" width="50%" >| <img src="https://user-images.githubusercontent.com/27658353/70491682-4c004180-1b45-11ea-97a1-bfc77b31e6a0.png" width="50%" > |

| Output |
| :--: |
| output_diff.png |
| <img src="https://user-images.githubusercontent.com/27658353/70491691-502c5f00-1b45-11ea-9bdc-8cf1f1189593.png" width="80%" > |

Output file:
- output_diff.png: Show the difference on matched parts, it's created base on old image.
- output_delete.png: Show the decrease parts to new image, it's created base on old image.
- output_add.png: Show the increase parts to old image, it's created base on new image.

Each color stands for:
- Green rectangle:
  - The parts decrease to new image.
  - The parts increase to old image.
- Red rectangle: Matched parts in both image.
- Color in red : Difference in matched part.

## Built With

* [OpenCV 4.1.2](https://docs.opencv.org/4.1.2/) - Image processing
* [cxxopts](https://github.com/jarro2783/cxxopts) - For option phrase


## How to build Gazo-san

1. Build Gazo-san by CMake
```
mkdir build
cd build
cmake ..
make
cd ..
```
2. Binary file is in bin directory
```bash
cd bin
./gazosan
```

## Execute Gazo-san

Execute image difference calculate.

```bash
Usage: ./gazosan PATH_TO_NEW_FILE PATH_TO_OLD_FILE OUTPUT_NAME [options]

  options

  -v, --verbose              Enable verbose output message
      --create-change-image  Create increase and decrease part image
  -h, --help                 Print help
```

You will get a png file which named "OutputName_diff.png", showing the difference between new and old image.

## Tests

### Download Google Test and Build

```bash
sh .circleci/build_googletest.sh
```

### Run test

#### Unit test

1. Build unit tests by CMake
```
mkdir build
cd build
cmake .. -DGTEST=ON -DTEST_LEVEL=unit
make
cd ..
```
2. Execute unit tests
```bash
./gazosan_unit_test
```

#### Integration test

1. Build integration tests by CMake
```
mkdir build
cd build
cmake .. -DGTEST=ON -DTEST_LEVEL=integration
make
cd ..
```
2. Execute integration tests
```bash
./gazosan_integration_test
```

#### System test

1. Build system tests by CMake
```
mkdir build
cd build
cmake .. -DGTEST=ON -DTEST_LEVEL=system
make
cd ..
```
2. Execute system tests
```bash
./gazosan_system_test
```

## License
[Apache 2.0 license](LICENSE)

## Contributors
(In alphabetical order)
* Akari Ikenoue
* [Jye Ruey](https://github.com/rueyaa332266)
* [Naoto Kishino](https://github.com/naotospace)
* [Rikiya Hikimochi](https://github.com/hikimochi)
