# TrainEngine

TrainEngine is a flexible open source 2D game engine using SFML.

## Getting Started

### Prerequisites

You must have a copy of the game engine on your local computer.

#### On Windows

* Make sure you have mingw SEH 6.1.0 64 bit installed and in your system PATH.
The version is important since SFML's binairies are shipped with the engine.
If you want a different compiler, you must compile SFML 2.4.2 and replace it
in the engine under libs/SFML-2.4.2.

* You must have GNU-make installed.

* You must have a way to execute bash commands. One way of doing that is using Git Bash as your terminal.

#### On Linux

TODO

#### On MacOS

TODO

### Compiling

Simply CD in the root directory of the game engine (where the Makefile is) and
type one of the following commands.

Note: To do any of the following commands in release mode, simply add release=1 at the end of the command.

To compile:
```
make all
```

To run the engine:
```
make copyassets run
```

To clean the build and bin files:
```
make clean
```

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

## Authors

* **Billie Thompson** - *Initial work* - [PurpleBooth](https://github.com/PurpleBooth)

See also the list of [contributors](https://github.com/your/project/contributors) who participated in this project.

## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details

## Acknowledgments

* Hat tip to anyone whose code was used
* Inspiration
* etc
