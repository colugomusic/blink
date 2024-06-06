# Blink

Blink is Blockhead's native plugin format. There are three types of plugin: Effect, Sampler and Synth.

Blink plugins work in a way that is specific to the atypical architecture of Blockhead. The Blink API has been designed in a way that enables more host applications to be developed outside of Blockhead, though I don't recommend it.

Blink is NOT an attempt to create a new audio plugin format which competes with VST/CLAP/LV2 etc.

## Terminology

### Plugin
A dynamic link library which implements the C API ([/include/blink.h](/include/blink.h))

### Instance
The host application can create instances of a plugin. An instance is a collection of Units. Instances can be created in various different contexts. For example:
- A workspace block in Blockhead which has a Blink plugin associated with it, will be backed by a single instance.
- A baking operation in Blockhead which involves a Blink plugin, will create a temporary instance on-the-fly.

### Unit
A unit is essentially the audio processor for a plugin. This is where the actual DSP code is. The host application may create more than one unit per instance. For example, Blockhead will create four units per block for the purposes of crossfading between them to avoid clicking artifacts when the project state changes, or the playback loops back to an earlier position.

Most of the time, only one unit per instance is actually doing any processing, but sometimes multiple units per instance may be processing simultaneously, contributing to the final audio output.

For the most part, the plugin developer does not need to worry about any of this. You can just write your DSP code normally, as if there is only one unit, and the host application will mix the unit output correctly. However, for some technical effects, it may be necessary to share data between units to get the correct result during project editing and playback, and the instance/unit architecture allows you to do this. For an example of this see [Blockhead's "Freeze" effect](https://github.com/colugomusic/blockhead_generators/tree/master/effects/freeze).

## This repository

### [/include/blink.h](/include/blink.h):
It's a C API. If you produce a dynamic link library which correctly implements this interface then you have a valid Blink plugin.

- Functions beginning with `blink_effect_` only need to be implemented by effect plugins.
- Functions beginning with `blink_sampler_` only need to be implemented by sampler plugins.
- Functions beginning with `blink_synth_` only need to be implemented by synth plugins.

The host application will deduce the type of the plugin from which of these functions are implemented by the library.

Blockhead searches the `./blink/` directory (`/Frameworks/blink/` on macOS) for plugins matching the following filename format:

`(effect|sampler|synth).(plugin_name).(plugin_version).(linux|macos|win64).blink`

 - `plugin_name` can be anything
 - `plugin_version` is the letter 'v' followed by any number of digits and points e.g. 'v0.1.2.3'
 - `linux|macos|win64` should match the current platform
 - `.blink` replaces the usual dynamic library extension for the current platform (i.e. '.dll', '.so' or '.dylib')

### [/lib](/lib):
This is a C++ library to aid in writing Blink hosts and plugins. There is no requirement to use this library. [Blockhead's stock plugins](https://github.com/colugomusic/blockhead_generators) are all implemented using this library.

## Parameters

There are five types of plugin parameter: Chord, Envelope, Option, SliderInt and SliderReal.

Every parameter has a UUID associated with it. Some standard UUIDs are defined in [/include/blink_std.h](/include/blink_std.h).

UUIDs are unique to particular concepts, not specific parameters. For example "ca2529db-e7bd-4019-9a07-22aee24526d1" is a unique identifier which refers to the general concept of "pitch". If two or more plugins each expose a parameter of the same type, which each use this same identifier, Blockhead will automatically associate them together, enabling features such as cross-plugin manipulation and hotswitching.
