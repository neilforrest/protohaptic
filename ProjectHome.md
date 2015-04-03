# ProtoHaptic: An Interactive Haptic Prototyping Tool #
![http://srvgla.no-ip.org/ProtoHapticGoogleCode/protohaptic_robot.jpg](http://srvgla.no-ip.org/ProtoHapticGoogleCode/protohaptic_robot.jpg)
![http://srvgla.no-ip.org/ProtoHapticGoogleCode/protohaptic.jpg](http://srvgla.no-ip.org/ProtoHapticGoogleCode/protohaptic.jpg)

![http://srvgla.no-ip.org/ProtoHapticGoogleCode/protohaptic_cone.jpg](http://srvgla.no-ip.org/ProtoHapticGoogleCode/protohaptic_cone.jpg)

[More screenshots...](Screenshots.md)

## What is ProtoHaptic? ##

ProtoHaptic is an interactive [haptic](http://en.wikipedia.org/wiki/Haptic) prototyping application which supports [SensAble](http://www.sensable.com/) haptic devices.

The program is built upon the existing [OpenHaptics APIs](http://www.sensable.com/products-openhaptics-toolkit.htm) and facilitates the interactive construction and manipulation of a three dimensional environment that can both be seen on screen and felt via the force feedback haptic device.

Primitive objects may be added and positioned in the scene and various familiar transformations such as scaling and rotation applied to them. In addition to the usual geometric operations the editor provides the ability to change the haptic properties of the scene such as the stiffness of an object, or the lateral friction produced by a surface.

It is intended that the final product will provide a user friendly alternative to programing, for the purposes of rapidly prototyping a haptic environment.

For more information, read on or check out the documentation (Project Report, Short Paper at HAID 2006, User Manual and more...) in the [dowloads section](http://code.google.com/p/protohaptic/downloads/list).

Executables are also available in the [downloads section](http://code.google.com/p/protohaptic/downloads/list), while the source code is available via SVN (details the [source section](http://code.google.com/p/protohaptic/source)).

## Features ##
  * Primitive Objects: Cube, Sphere, Cylinder, Triangle, Torus.

  * Transformations: Translation, Scaling, Rotation and other object specific transformations.

  * Real time editing of haptic properties: Stiffness, Static Friction, Dynamic Friction and Damping.

  * Rigid body dynamics and collision detection for creating dynamic haptic scenes

  * Export code for created scenes: C Code using OpenGL API, C++ Code using AbstractModel API

  * Import code to create scene: C++ Code using AbstractModel API, C++ Code using GHOST API

## Background ##

ProtoHaptic was originally designed and implemented as a project forming part of my undergraduate course at the [University of Glasgow](http://www.dcs.gla.ac.uk/), supervised by Dr Steven A Wall. I now use the application fairly regularly as a tool for haptic research and development in my work at [The Royal Veterinary College](http://www.rvc.ac.uk/). I have continued to work on some features as and when I have required them.

## Applications of ProtoHaptic ##

Although the applications of such an direct manipulation editor are deliberately as diverse as possible, it is envisaged that it would be particularly useful in the following scenarios.

  * The rapid prototyping of haptic interfaces for immediate user testing.

  * The iterative development and refinement of specialist simulations by non-computing scientists. E.g. Medical/Veterinary training simulations.

  * Prototyping experimental procedure, say in psychological research.

  * Designing three dimensional models for use in external applications or games.


Whilst working at The Royal Veterinary College as a software developer / haptic researcher, I have used ProtoHaptic for:

  * Allowing a vet student to develop a cat’s abdomen palpation simulator (without the need to learn how to program).

  * Allowing another vet student to design haptic models for a haptic pulse simulator.

  * Designing haptic models for other haptic applications written using the AbstractModel API.

  * Converting old legacy GHOST code to AbstractModel API code using import/export features