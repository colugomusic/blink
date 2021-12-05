# Blink (not an audio plugin format)

Blink is Blockhead's native plugin format. There are three types of plugin: Effect, Sampler and Synth

## Why did you create another audio plugin format?

I didn't. I made a "Blockhead plugin format" which is a plugin format which happens to deal with audio signals but otherwise has little in common with other "audio plugin" formats. 

## Oh no

Blockhead is not architected in the traditional way where things are organised into "devices" which are dropped into a signal chain and then sit there reacting to input signals. Instead we have this "block" concept which defines a finite-time audio generation or transformation which exists at a certain point in a timeline.

When the playhead is not inside a block, it is not being processed (there are some technical reasons why this is not actually true, but it is true most of the time).

Blink started as an effort to modularize some parts of Blockhead's codebase and gradually turned into a plugin format.

## This repository

Consists of two directories:

### [/include](/include):
It's a C API. There are 4 headers, [blink.h](/include/blink.h) which defines the base API which is common among all plugin types, and three extension headers defining the rest of the interface which must be implemented depending on whether the plugin is an [effect](/include/blink_effect.h), a [sampler](/include/blink_sampler.h) or a [synth](/include/blink_synth.h).

If you produce a dynamic link library which correctly implements one of these three interfaces then you have a valid Blink plugin.

### [/plugin](/plugin):
This is a C++ library to aid in writing Blink plugins. There is no requirement to use this library. [Blockhead's stock plugins](https://github.com/colugomusic/blockhead_generators) are all implemented using this library.

## This README is not done yet
If you stumble upon this repository and are interested in developing Blockhead plugins then please contact me!

Random notes:
```
Sampler plugins are responsible for defining how the sample waveform should be drawn (Blockhead will pass in a horizontal list of block positions for pixels that it wants to draw and the plugin must return a corresponding list of sample positions.)
```
