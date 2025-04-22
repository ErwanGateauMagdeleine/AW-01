# Plugin

This folder gathers the sources to build a plugin. It is a vst3 plugin that is based on the JUCE framework.

## Setup

You can generate an Visual Studio 2022 Project using the command:
```
cmake -B Builds -G "Visual Studio 17 2022"
```

Then you can trigger a build directly from the command line running the following command:

```
cmake --build Builds --config Release
```

Once the build is finished, the built plugin is automatically moved to the OUT folder.

## References

In order to build the vst project, the following references were used:
* [pamplejuce](https://github.com/sudara/pamplejuce)
* [JUCE CMake API](https://github.com/juce-framework/JUCE/blob/master/docs/CMake%20API.md)
* [JUCECmakeRepoPrototype](https://github.com/eyalamirmusic/JUCECmakeRepoPrototype)
