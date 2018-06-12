
# Simulating Computers

Simulating Computers (SimComp) is a project created as an introduction to the fundamental concepts in the areas of computer architecture and organization, instruction sets, assembler programming,
run time systems and simulation. This repository is intended to accommodate the learning material found [here](#). Most of the documentation is found in the learning material. Currently there exists two user interfaces for the simulator, guidelines for building these is found underneath.

## Build
Both of the UI applications has been built and tested with macOS version 10.12.6 and Windows 10.

### Console
To build the console application you'll need to clone both the `_Console/` and `_Simulator/` directories. If you're using *macOS* or *Linux* we recommend that you use [CMake](https://cmake.org) to build the Console-ui. Grab it [here](https://cmake.org/download/) if you don't have it. If you're using *Windows* we recommend building with [Visual Studio](https://www.visualstudio.com).

For *macOS* and *Linux* users all you need to do is run [CMake](https://cmake.org) from the `_Console/_src/` directory, followed by make.
```
_Console/_src user$ cmake .
_Console/_src user$ make
```   
This will build the source code to an executable named *SimComp*. Finally, to launch the application you'll need to pass the directory with the `*.sams` files as a parameter to the binary. Example using the assembler files included in `_Simulator/_sasm/`:
```
_Console/_src user$ ./SimComp ../../_Simulator/_sasm/
```

### GUI
To build the GUI application you'll need to clone both the `_GUI/` and `_Simulator/` directories. The easiest way to build the... to be continued..
## Structure

- `_Root/`
  - `_Simulator/` Everything related to the core simulator.
    - `_asm/` Example assembler programs.
    - `_src/` C++ source code for simulator.

## Dependencies

Nothing thus far. QT soon.

```
Give examples
```

<!--
### Installing

A step by step series of examples that tell you how to get a development env running

Say what the step will be

```
Give the example
```

And repeat

```
until finished
```

End with an example of getting some data out of the system or using it for a little demo

## Running the tests

Explain how to run the automated tests for this system

### Break down into end to end tests

Explain what these tests test and why

```
Give an example
```

### And coding style tests

Explain what these tests test and why

```
Give an example
```

## Deployment

Add additional notes about how to deploy this on a live system

## Built With

* [Dropwizard](http://www.dropwizard.io/1.0.2/docs/) - The web framework used
* [Maven](https://maven.apache.org/) - Dependency Management
* [ROME](https://rometools.github.io/rome/) - Used to generate RSS Feeds

## Contributing

Please read [CONTRIBUTING.md](https://gist.github.com/PurpleBooth/b24679402957c63ec426) for details on our code of conduct, and the process for submitting pull requests to us.

## Versioning

We use [SemVer](http://semver.org/) for versioning. For the versions available, see the [tags on this repository](https://github.com/your/project/tags).
-->

## Authors

* **Lasse Natvig** - *Initial work* - [NTNU](https://innsida.ntnu.no/person/lasse)
* **Ole Bjørn Eithun Pedersen** - *GUI and general development* - [LinkedIn](http://linkedin.com/in/eithunpedersen/)

See also the list of [contributors](https://github.com/LasseNatvig/SimComp/contributors) who participated in this project.


## Acknowledgments

* Hat tip to anyone whose code was used
