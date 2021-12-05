# Blink (not an audio plugin format)

Blink is Blockhead's native plugin format. There are three types of plugin: Effect, Sampler and Synth

## Why did you create another audio plugin format?

I didn't. I made a "Blockhead plugin format" which is a plugin format which happens to deal with audio signals but otherwise has little in common with other "audio plugin" formats. 

Blink started as an effort to modularize some parts of Blockhead's codebase and gradually turned into a plugin format.

## Oh no

Blockhead is not architected in the traditional way where things are organised into "devices" which are dropped into a signal chain and then sit there reacting to input signals. Instead we have this "block" concept which defines a finite-time audio generation or transformation which exists at a certain point in a timeline.

When the playhead is not inside a block, it is not being processed (there are some technical reasons why this is not actually true, but it is true most of the time).

Blink plugin instances are very lightweight entities which can be sliced and copied around on the workspace without any lag.

A blink plugin has full access to a block's modulation data (it knows what an envelope is going to do in the future, for example). This is because the host just passes the plugin a pointer to the entire block data every audio buffer, instead of continuously sending MIDI events (or whatever) to emulate parameter changes. The pointer that the host passes to the plugin is guaranteed to be valid for the duration of the audio buffer, and the pointed-to data is guaranteed to not change.

Blockhead tries to eliminate clicking artifacts that occur during loops and block data changes by crossfading between different versions of the same blocks. A "block data change" is anything that changes the way the block sounds (including moving the block backwards and forwards in time).
 - Any time the block data changes, a crossfade occurs (if the block is currently being played)
 - Any time the song playback loops back to an earlier point in time, a crossfade occurs (if the block is currently being played)
 - In blockhead a loop region can be shorter than the length of an audio buffer! (64 frames)

If the block data changes and a loop occurs at the same time then a block may have to simulataneously crossfade between 4 versions of itself! This sounds wasteful but 99% of the time only one version of the block is actually being processed, and the others are completely turned off. To facilitate this nonsense, Blink plugins have a hierarchical structure:

### Plugin
A dynamic link library which implements the blink C API (see below.)

### Instance
An instance of the plugin. Blockhead will create one instance per block. Data that needs to be shared between units should be stored against the instance, which is uncommon but sometimes necessary ([the freeze effect does this](https://github.com/colugomusic/blockhead_generators/tree/master/effects/freeze).)

### Unit
A unit is the thing that does the actual audio processing. Blockhead will request 4 units for each instance. For most synths and audio effects, units don't need to know anything about each other and can just be written as if they are the only unit.

Units usually don't need to do anything special in response to loop events because Blockhead automatically performs a crossfade between two units. Again, [the freeze effect](https://github.com/colugomusic/blockhead_generators/tree/master/effects/freeze) is an example where things are more complicated and units need to share data with one another.

Units don't need to do anything to smooth out parameter changes because this is also dealt with by the host using the same crossfading mechanism.

During a crossfade, the two (or four) units involved are just processed normally (forwards, as if nothing unusual is happening). Their final amplitude is adjusted by the host.

While a unit is being processed, the host passes the unit a sequence of 64 block positions for each audio buffer. The positions are relative to the position of the block in the timeline (with the left edge of the block being zero). In most cases the block will begin processing slightly before the edge of the block, so positions can be negative. If the unit needs to do something special when the playhead hits position zero then it needs to handle this itself.

Block positions are not aligned to the audio buffer i.e. they do not have to (and probably don't) start at 0, 64, 128, 192, etc... This is because Blockhead supports loop regions of arbitrary frame length (sorry!)

If a unit goes more than one audio buffer without being processed, the host requests a reset.

## This repository

Consists of two directories:

### [/include](/include):
It's a C API. There are 4 headers, [blink.h](/include/blink.h) which defines the base API which is common among all plugin types, and three extension headers defining the rest of the interface which must be implemented depending on whether the plugin is an [effect](/include/blink_effect.h), a [sampler](/include/blink_sampler.h) or a [synth](/include/blink_synth.h).

If you produce a dynamic link library which correctly implements one of these three interfaces then you have a valid Blink plugin.

Blockhead searches the `./blink/` directory (`/Frameworks/blink/` on macOS) for plugins matching the following filename format:

`(effect|sampler|synth).(plugin_name).(plugin_version).(linux|macos|win64).blink`

 - `plugin_name` can be anything
 - `plugin_version` is the letter 'v' followed by any number of digits and points e.g. 'v0.1.2.3'
 - `linux|macos|win64` should match the current platform
 - `.blink` replaces the usual dynamic library extension for the current platform (i.e. '.dll', '.so' or '.dylib')

### [/plugin](/plugin):
This is a C++ library to aid in writing Blink plugins. There is no requirement to use this library. [Blockhead's stock plugins](https://github.com/colugomusic/blockhead_generators) are all implemented using this library.

## This README is not done yet
If you stumble upon this repository and are interested in developing Blockhead plugins then please contact me!


-------
Dump random unorganized notes here:

Sampler plugins are responsible for defining how the sample waveform should be drawn (Blockhead will pass in a horizontal list of block positions for pixels that it wants to draw and the plugin must return a corresponding list of sample positions.)
