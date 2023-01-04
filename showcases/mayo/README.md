<div align="center">
  
[![CI](https://github.com/fougue/mayo/actions/workflows/ci.yml/badge.svg?branch=develop)](https://github.com/fougue/mayo/actions/workflows/ci.yml)
[![Build status](https://ci.appveyor.com/api/projects/status/6d1w0d6gw28npxpf/branch/develop?svg=true)](https://ci.appveyor.com/project/HuguesDelorme/mayo)
[![Codacy Badge](https://app.codacy.com/project/badge/Grade/d51f8ca6fea34886b8308ff0246172ce)](https://www.codacy.com/gh/fougue/mayo/dashboard?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=fougue/mayo&amp;utm_campaign=Badge_Grade)
[![Downloads](https://img.shields.io/github/downloads/fougue/mayo/total.svg)](https://github.com/fougue/mayo/releases)
[![License](https://img.shields.io/badge/license-BSD%202--clause-blue.svg)](https://github.com/fougue/mayo/blob/develop/LICENSE.txt)
[![Version](https://img.shields.io/badge/version-v0.6.0-blue.svg?style=flat)](https://github.com/fougue/mayo/releases)
  
</div>

<div align="center">
  <img src="images/appicon_256.png" alt="Logo" width="128px" align="center" />
  <p></p>
  <p align="center"><strong>Mayo</strong> the opensource 3D CAD viewer and converter</9>
  <p></p>
  <img src="doc/screencast_1.gif"/>
</div>

## :eyeglasses: Overview

- **Convert 3D files** <br/>
  Mayo can read/write 3D files from/to STEP, IGES, STL and many other [CAD formats](https://github.com/fougue/mayo/wiki/Supported-formats)

- **Visualize 3D files** <br/>
  Mayo 3D viewer supports clip planes, exploding of assemblies, measurement of shapes, show/hide parts, ...

- **Cross platform** <br/>
  Mayo runs on Windows, Linux and macOS

- **Solid foundations** <br/>
  Mayo is developed in modern C++ with [Qt](https://www.qt.io) and [OpenCascade](https://dev.opencascade.org)

## :zap: Features

- **3D clip planes** with configurable capping

- **3D exploding of the model tree** allowing better exploration of complex designs

- **3D measure tools** for circles, angles, lengths, areas, ...

- **3D view cube** providing intuitive camera manipulation

- **Quick access to CAD files** recently open thanks to thumbnails in the [Home page](https://github.com/fougue/mayo/blob/develop/doc/screenshot_5.png)

- **Toggle item visibility** within the Model tree(use checkbox)

- **Customizable mesh precision** for BREP shapes, affecting visualization quality and conversion into mesh formats

- **Convert files** to multiple CAD formats from [command-line interface](https://github.com/fougue/mayo/blob/develop/doc/screencast_cli.gif):computer:

## :floppy_disk: Supported formats

| Format | Import             | Export             | Notes              |
| ------ | ------------------ | ------------------ | ------------------ |
| STEP   | :white_check_mark: | :white_check_mark: | AP203, 214, 242    |
| IGES   | :white_check_mark: | :white_check_mark: | v5.3               |
| BREP   | :white_check_mark: | :white_check_mark: | OpenCascade format |
| DXF    | :white_check_mark: | :x:                |
| OBJ    | :white_check_mark: | :white_check_mark: |
| glTF   | :white_check_mark: | :white_check_mark: | 1.0, 2.0 and GLB   |
| VRML   | :x:                | :white_check_mark: | v2.0 UTF8          |
| STL    | :white_check_mark: | :white_check_mark: | ASCII/binary       |
| AMF    | :x:                | :white_check_mark: | v1.2 Text/ZIP      |
| PLY    | :white_check_mark: | :white_check_mark: | ASCII/binary       |
| Image  | :x:                | :white_check_mark: | PNG, JPEG, ...     |

See also this dedicated [wikipage](https://github.com/fougue/mayo/wiki/Supported-formats) for more details

## :mag: 3D viewer operations

| Operation      | Mouse/Keyboard controls       |
| -------------- | ----------------------------- |
| Rotate         | mouseLeft + move              |
| Pan            | mouseRight + move             |
| Zoom           | mouseLeft + mouseRight + move |
| Zoom +/-       | mouseWheel(scroll)            |
| Window zoom    | CTRL + mouseLeft + move       |
| Instant zoom   | spaceBar                      |
| Select Object  | mouseLeft click               |
| Select Objects | SHIFT + mouseLeft clicks      |

Mayo supports also multiple 3D viewer navigation styles to mimic common CAD applications(CATIA, SOLIDWORKS, ...)

## :hammer: How to build Mayo

[Instructions for Windows MSVC](https://github.com/fougue/mayo/wiki/Build-instructions-for-Windows-MSVC)

[Instructions for Debian](https://github.com/fougue/mayo/wiki/Build-instructions-for-Debian)

[Instructions for macOS](https://github.com/fougue/mayo/wiki/Build-instructions-for-macOS)

## :clapper: Gallery

<img src="doc/screencast_cli.gif"/>
  
<img src="doc/screenshot_2.png"/>

<img src="doc/screenshot_3.png"/>

<img src="doc/screenshot_4.png"/>

<img src="doc/screenshot_5.png"/>
