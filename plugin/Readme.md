# Plugin

This folder gathers the sources to build a plugin. It is a vst3 plugin that is based on the JUCE framework.

## Generating the IDE project

To generate a project for your IDE, one can run the following command:
```
cmake -B Builds -G <your_IDE_generator>
```
You can find the list of available IDE generators [here](https://cmake.org/cmake/help/latest/manual/cmake-generators.7.html).

## Build the plugin
Once you have generated your IDE project, you can from the command line build the plugin running the following command:

```
cmake --build Builds --target AD-01_VST3 --config Release
```

Once the build is finished, the built plugin `.vst3` is automatically moved to the OUT folder.

## Building the tests

The software comes along with Catch2 tests. To build the tests, on can run the following command:
```
cmake --build Builds --target Catch2Tests --config Debug
```

Once the test target is built, one can run them using the following command:
```
.\build\Debug\Catch2Tests.exe
```

## References

In order to build the vst project, the following references were used:
* [pamplejuce](https://github.com/sudara/pamplejuce)
* [JUCE CMake API](https://github.com/juce-framework/JUCE/blob/master/docs/CMake%20API.md)
* [JUCECmakeRepoPrototype](https://github.com/eyalamirmusic/JUCECmakeRepoPrototype)
