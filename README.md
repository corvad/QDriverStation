![Header](etc/header/small.png)

[![Build Status](https://img.shields.io/travis/FRC-Utilities/QDriverStation.svg?style=flat-square)](https://travis-ci.org/FRC-Utilities/QDriverStation) 
[![Github All Releases](https://img.shields.io/github/downloads/frc-utilities/qdriverstation/total.svg?style=flat-square)](http://github.com/frc-utilities/qdriverstation/releases/latest)

The QDriverStation is a cross-platform and open-source alternative to the FRC Driver Station. It allows you to operate FRC robots with the major operating systems (Windows, Mac OSX and GNU/Linux). The QDriverStation is able to operate both 2009-2014 robots and 2015-2017 robots.

The actual code that operates a FRC robot is found in a [separate repository](https://github.com/FRC-Utilities/LibDS), which is written in C and can be used for your own projects or change it to support more communication protocols (such as [ROS](https://github.com/FRC-Utilities/QDriverStation/issues/21)).

You can find the online documentation of the QDriverStation and its sub-projects [here](http://frc-utilities.github.io/documentation/).

### Project Status [2019]

Unfortunately, I do not participate in any FRC/FIRST team anymore, so I cannot test any changes of the QDriverStation/LibDS with FRC hardware.

The project has not received any significant updates since 2017, thus the QDriverStation does not work with 2018 robots. However, due to the design of the LibDS, it should be possible to support newer communication protocols without too much effort.

If you are interested in continuing development of this project, please contact me so I can add you to the "FRC Utilities" organization, be sure to have a copy of [Wireshark](https://www.wireshark.org/) installed and a pencil and notebook to reverse-engineer the FRC Comm. Protocols :wink:

### Install notes

You can download the QDriverStation from [GitHub](http://github.com/FRC-Utilities/QDriverStation/releases).

Once you finish installing the software, you can launch it and begin driving your robot. Just be sure to input your team number and to verify that the joysticks are working correctly.

Mac users will be prompted to download an additional driver for Xbox 360 controllers to work.

###### Note for Linux users

For convenience, Linux releases are now handled with AppImages, which are generated by Travis CI while building/testing the project.

To run the AppImage, simply download the latest release, make it executable and run it. Terminal commands below:

    cd Downloads
    chmod +x QDriverStation*.AppImage
    ./QDriverStation*.AppImage

More info can be found here: [https://appimage.org/](https://appimage.org/).

###### Warnings

If you are on Linux, the QDriverStation may detect some devices as a joystick ([more info...](https://gist.github.com/denilsonsa/978f1d842cf5430f57f6#file-51-these-are-not-joysticks-rules)). If that happens, just disable the faulty device by clicking on the power button next to its name.

### Build instructions

###### Cloning the repository

To clone the project, fire up a Terminal and run the following command:

`git clone https://github.com/FRC-Utilities/QDriverStation`

Since this project makes use of [git submodules](), you need to run the following command to download the latest LibDS version:

`cd QDriverStation`
`git submodule update --init --recursive`

###### Requirements

The only requirement to compile the application is to have [Qt](http://www.qt.io/download-open-source/) installed in your system. The desktop application will compile with Qt 5.2 or greater.

- If you are using Linux, make sure that you have installed the following packages:
    - libsdl2-dev

The project already contains the compiled SDL libraries for Windows and Mac.

###### Compiling the application

Once you have Qt installed, open *QDriverStation.pro* in Qt Creator and click the "Run" button.

Alternatively, you can also use the following commands:
- qmake
- make
- **Optional:** sudo make install

You can check this [tutorial by Dakota Keeler](https://www.youtube.com/watch?v=G9DywTB9_cY) to compile the QDriverStation.

### Credits

This application was created by [Alex Spataru](http://github.com/alex-spataru).

Of course, many people contributed in different ways to this project, you can find more details in the [contributors list](CONTRIBUTORS.md). Finally, we want to thank you for trying this little project, we sincerely hope that you enjoy our application and we would love some of your feedback.
