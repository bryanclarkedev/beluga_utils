# beluga_utils
Utility classes and functions for Arduino ESP32 using PlatformIO. 

As utility tools, they're not particularly exciting; they're meant to enable other, more-exciting-things. The goal is for this repository to be submodule'd inside the repository of the main (hopefully cooler) project.

Many of these are substituting for libraries that won't fit in an ESP32 (e.g. you can't easily include Boost so I have done a bunch of useful string operators) or for my particular requirements.

## What is beluga?
Beluga is a C++ library for Arduino ESP32 using the PlatformIO toolchain/environment/ecosystem. It's designed to make it easy to create a dual-thread application with inter-thread communication buffers, configured from a .ini file. The general emphasis is on standing up a prototype quickly and relatively easily. There's a bunch of classes for various sensors, actuators and communication protocols that can run in this architecture, which are also configured easily from the .ini. It's intended to be easily expandable and configurable, with a fairly straightforward inheritance hierarchy providing convenient "wrappers" around other libraries or classes you might want to include in your project. 

## Why use beluga?
My primary use case is building small remote sensors or robots. I use pretty much the same architecture for all I do on ESP32:
- a thread for slow or blocking processes (serial, TCP server, web server, MQTT)
- a thread for fast or "real-time" processes (reading sensors, control or localisation algorithms, driving actuators)
- buffers to pass data between the two threads
- a config file to allow settings to be changed without having to recompile

Beluga encapsulates this in a couple of handy classes. It's easy to put a sensor object in a thread and have a communication class sitting in the other thread for reporting results.

### Other things I want
- I want a wrapper I can put around a range of sensor/actuator libraries from different vendors, so I can interact with them all in roughly the same way. The static typing in C++ makes this a bit more complex than in, say, Python.
- I want it to be easy to nest different sensor/actuator objects via a sort of list/dictionary/tree structure e.g. a sensor object housing an IMU, which houses a 3-axis accelerometer, which houses the sensor for each axis.
- I want those objects to sit inside a thread or two.
- I want to get data into or out of it all via some communications protocol like MQTT or TCP.
- I want to be able to configure it fairly easily outside of code via a config file.
- I want the code to be reasonably well documented.
- I want the code to be reasonably well tested, with an example project that should work out of the box. 
- I want the whole thing to be easy to build. I'm doing this in my spare time and I battle broken things in my day job, so I am cutting a corner or two for expediency. PlatformIO's config system means I can import libraries without needing to personally battle CMakeLists.txt, for which I am grateful (I love you CMake, just as a friend).

So, the usual spiel; abstraction, modularity, reuseability, that sort of thing. If you have some of the same wants, beluga may be of interest to you.

It's released under the MIT license so you can harvest whatever you want within its terms. You can probably use bits of beluga in other platforms or environments. Most of it is vanilla C++, with Arduino/PlatformIO/embedded dependency in:
- digital and analog I/O
- the ini-file reader via SPIFFS
- threads and inter-thread buffers via freeRTOS
- MQTT via PicoMQTT

Most other classes can be used platform-independently, with the caveat that there are some newer C++ features not supported by the Arduino compiler so they're not used (e.g. I only use static_cast because dynamic_cast isn't available at the time of writing). They should work but might not be the best solution for you.

Some of the bits of code I've scavenged are kind of neat; my curated selection of StackOverflow's and CodeProject's greatest hits. Have a peruse, you might find something worth adding to your own collection:
- exceptions that report the exact file and line number
- converting a type to a string to an enum (by chaining together a few bits of other peoples' great code)
- a wrapper around PicoMQTT to make it a bit more user friendly
- a bunch of string processors.

## Why not use beluga?
Beluga won't be perfect for everything. You should consider alternatives in these cases:
- If you want to use code that is described above as Arduino-dependent, on a non-Arduino system.
- If you aren't using an embedded system (if you're serious about robotics, have a PC the size of a raspberry pi or larger, and aren't interested in stealing fragments of code from here, use ROS2)
- If you have really tight real-time constraints (probably better to have a bunch of interrupts and custom classes; I wouldn't call the thread code here slow or bloated but it's designed for ticking along happily rather than pushing the system to its limits).
- If you have really tight memory requirements (beluga generates objects dynamically so you may run out of memory if you create too many things).
- If you want better support for ESP32-CAM systems (still a WIP).
- If you want fancier tools/UI's for networking the embedded and desktop/cloud (beluga is just MQTT, TCP, serial; bluetooth serial WIP. MQTT explorer works with it fine).
- If you want to have a whole bunch of threads running in parallel (you could probably do it pretty easily with the thread class but I haven't tested it; in my experience untested code is broken code)
- If you don't want to read documentation (no judgement, I've been there).

## Why was beluga written?
### Reason 1: faster, easier ESP32 hackery
The main motivation was to make it easier for me to hack things together with ESP32 and reduce the amount of rewriting required when making changes. I have more grand plans than time to implement them all; if I buy a bunch of sensors, I want to be able to get them up and running without reinventing the wheel every time.

### Reason 2: edutainment
I do most of my work in Python at the moment and wanted to continue to hone my C++ skills. Building stuff for your own satisfaction is great, you should try it. No tech debt, no getting committees on board. It was this or teach myself MicroROS or espressif-IDE and both of those are too close to actual work. Building my own library from first principles gives me the opportunity to try some slightly niftier and more baroque solutions than I would normally try at work.

So, keep in mind that this is the hobby project of some guy* before you go putting it in production; I personally have been burnt by a few well-used and documented libraries with intermittent but catastrophic bugs that cost a lot of development time. Let me know if you find one here, but as with most (all?) open source, it's ultimately caveat emptor.
*some fairly well-trained, fairly experienced, outrageously handsome guy.

### Reason 3: Resume padding
If you're following the link from my resume, you should definitely hire me for the job.

## Why is it called beluga?
There are a bunch of projects I want to complete, 'white whales' that I like to think I am pursuing relentlessly. I scrapped two earlier iterations of this library when I didn't like how they worked; I guess the whale got away on the first couple of hunts. We'll get him this time.