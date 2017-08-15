# Atom-Synth

Atom-Synth is a free and open-source, cross-platform synthesizer built for [modular synthesis](https://en.wikipedia.org/wiki/Modular_synthesizer). This means that it can load different modules (or _atoms_ as called in here) which are the building blocks out of which synthesizers are made. It allows you to build your very own sounds by allowing for modular connection between them.

Atom-Synth can run either in standalone mode or as an audio-plugin on Windows and Linux (MacOS support is planned). Currently, there are 64-bit and VST3 versions available.

## Installing for end users

Just head over to the releases and download the appropriate version for your system.

## Documentation

The documentation on how to use this synthesizer is available [here](https://docs.google.com/document/d/1UNayCf27xGWtYgQsOCOj8C-ochvKuGrm9utuL6AveSM/edit?usp=sharing).

## Building

Building requires the Projucer from the [JUCE Framework](https://github.com/WeAreROLI/JUCE) (it can be found there at ./JUCE/extras/Projucer/Builds) and the [VST3 SDK](https://github.com/steinbergmedia/vst3sdk).

Once the Projucer is installed or built (see below on how to build the open-source version) you should specify the path to the VST-SDK in the settings. Then open `AtomSynth.jucer` _or `AtomSynthStandalone.jucer`, depending on if you want to create the standalone version or the plugin. Ensure that all JUCE-modules and the VST-SDK are found by setting the correct paths under _File → Global Search Paths_. Check again under _Modules_ if they are found. If not specify `../JUCE/modules` for one module and then click on _settings_, scroll down and _Set Paths for all modules..._. Then save the project with _File → Save Project_. Then continue to follow the platform-specific instructions provided below.

### Specific to Windows:

- [TODO]

### Specific to Linux:

The dependencies for JUCE can be installed on any Debian-based Linux-system with the following command:

```
sudo apt-get install freeglut3-dev g++ libasound2-dev libcurl4-openssl-dev libfreetype6-dev libjack-jackd2-dev libx11-dev libxcomposite-dev libxcursor-dev libxinerama-dev libxrandr-dev mesa-common-dev
```

In addition to the dependencies for JUCE you need install this:

```
sudo apt-get install libwebkit2gtk-4.0-dev
```

After the dependencies are installed, the modules are specified in the Projucer and the project is saved, the makefile in `Projucer/Builds/LinuxMakefile` can be invoked by:

```
make CONFIG=Release -j8
```

After the compilation is completed, go to `build/` and start the Projucer with `./Projucer`.

Once you have built the Projucer and saved the project successfully, go to `AtomSynth/Builds/LinuxVst/` and invoke the makefile with:

```
make CONFIG=Release -j8
```

### Specific to MacOS:

- [TODO]

## How to build the open-source version of _Projucer_

Clone the git repository of _JUCE_:

```
git clone https://github.com/WeAreROLI/JUCE.git
```

Next, go to the folder `JUCE/extras/Projucer`. From there, open the file `JuceLibraryCode/AppConfig.h` in a text editor. Find the line `#define JUCER_ENABLE_GPL_MODE 0` and set it the value to 1 like so: `#define JUCER_ENABLE_GPL_MODE 1`

## Contributing

If you find a bug or have a suggestion like a new shiny feature Atom-Synth should have, submit an issue or make a pull request. Both are very much appreciated.
