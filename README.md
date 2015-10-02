VIBes
=====

[![Build Status](https://travis-ci.org/ENSTABretagneRobotics/VIBES.png)](https://travis-ci.org/ENSTABretagneRobotics/VIBES)

**V**isualizer for **I**ntervals and **B**oxes: http://ENSTABretagneRobotics.github.io/VIBES

Get the last version from https://github.com/ENSTABretagneRobotics/VIBES/releases

VIBes is a visualization system that aims at providing people working with interval methods a way to display results (boxes, pavings), without worrying with GUI programming. It provides drawing functions accessible from a lot of programming languages, without complex installation and library dependencies.
The main design goal of VIBes is to be cross-platform, available from different programming languages, simple to set-up, easy to port to a new language.

VIBes consists in two parts:
- the VIBes application that features viewing, annotating and exporting figures
- the VIBes API that enables your program to communicate with the viewer in order to draw figures from C, C++, Python, Matlab...

The use of a separate viewer application enables an easy set-up on every system. First, pre-built binaries of the VIBes viewer are provided for Windows, MacOS and Linux platforms (but you are free to build the viewer from sources). Then, the API consists only of a few files that have to be included in your program.
