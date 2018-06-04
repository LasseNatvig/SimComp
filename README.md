
# Simulating Computers

Simulating Computers (SimComp) is a project created as an introduction to the fundamental concepts in the areas of computer architecture and organization, instruction sets, assembler programming,
run time systems and simulation. This repository is intended to accommodate the learning material found [here](#). Most of the documentation is found in the learning material, although following the "Getting Started" underneath should get the simulator running.

## Getting Started

First, you should start off by cloning this repository, or at least the \_simulator part. Then you´ll need to compile the code. Example using clang-900.0.39.2:
```
$ clang++ -std=c++11 -o SimComp clang++ -std=c++11 main.cpp compSim.cpp config.cpp isa.cpp loader.cpp logger.cpp memory.cpp Program.cpp utils.cpp
```   
Running this command will compile all the necessary source files to a executable named "SimComp".
The generated binary will load all "\*.sasm" file from a directory which must be passed as an argument on execution.

<!--These instructions will get you a copy of the project up and running on your local machine for development and testing purposes. See deployment for notes on how to deploy the project on a live system. -->

### Prerequisites

Nothing this far. QT soon.

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
