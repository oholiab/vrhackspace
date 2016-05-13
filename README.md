#Grimmware VR Hackspace Mk I
## What
The VR Hackspace is a proof-of-concept attempt to render computer terminals and other collaborative hacking tools in a VR environment. Currently it is only single user.

## Why
The VR Hackspace is the product of both idle musings about how you don't need space for a lot of monitors if your computer terminal is rendered in VR, and also prompted by the n-o-d-e.net post about a VR hackspace.

After reading it, I took it upon myself to produce my interpretation using technologies I was familiar with (containerisation, weird X11 desktop hacks) and ones I wasn't (C++, Irrlicht, X11 keysyms).

## How
The VR Hackspace utilises containerisation (namely Docker at the moment) to produce an isolated Linux environment running a virtual X framebuffer (via Xvfb). A program within this environment continually takes screenshots of the virtual X session and writes these out to a volume mount on the host. The Unix domain socket for controlling the X session is also mounted on the host, where the VR hackspace program runs. This program runs a 3D environment where the screenshots are continually applied as a texture to a "screen" object, and keypresses are translated and forwarded to the X11 socket.

So basically it's an enormous shonky hack.

### Running
####Dependencies
As far as I can figure, you need two^Wthree main dependencies:
- libirrlicht
- docker
- g++

I'll be creating a Dockerfile for building this at some point

####Making it go
DISCLAIMER: This runs from an ubuntu base image which I have never audited, which runs as root and uses volume mounts. If you get owned doing this, it's your own stupid fault (in the same way it would be mine if I got owned). You have been warned.

In one terminal, to run the container:
    make runc

In another, to build and run the application:
    make run

The default flags for running the container include the /workspace volume mount which will enable you to work on the code from within the hackspace... Fun fact: that's how I'm writing this right now! (It's also why it now supports commas).

####Contributing
I prefer POC code over suggestions, so if you've got an idea but you're not sure about implementation, just submit a PR and say so!

###Roadmap
####Philosophy
In the spirit of portability, there's no reason that this code should be tied to Docker or even X11 - X can equally run in a BSD jail (or even Xen, KVM or something of that ilk), but so could Wayland. With that in mind, all X11 or Docker specific code should be kept outside of the main 3D application.

It is important that the VR Hackspace is usable, so it is an equally important goal that it is suitable for dogfooding - that is to say, it is a goal that you can hack on the Hackspace (and hopefully run it) from within the Hackspace! That's why I'm writing the docs from here now!

Finally, not so much a core tenet - you should feel free to adapt the containerised environment to your own needs! Every hacker has their own setup and that's half the fun! Go nuts!
####Existing Bugs
- Uses basic irrlicht camera controls, so when you type "j" you jump at the same time which is an enormous pain when navigating in vim. Equally, I've disabled cursor keys which is awful for bash. This is the price of MVP ;) It's next on the list
- Screen updating is slow. This could very well be a limitation of irrlicht, hence why this is both proof-of-concept and "Mark I"... The engine may not be suitable. That said, if you can work with transatlantic latency, you should be fine :)
- There are certain programs (xterm, the program I was attempting to develop against, being one of them... how we laughed) don't respect the "fake keypress" method used to forward keypresses. This may result in a big rewrite.
- I use XCape to map a single tap of LControl to Escape - in VRHackspace it always translates to Escape regardless, so there are some nuances there which aren't entirely obvious
- It's rough as all hell. Sorry about that... I don't normally C++.

####Planned features
In rough order of priority:

- Makefile support for overriding default docker image
- Seperate input/movement modes
- Mouse support
- Multiple, placeable and selectable computer environments
- Multiple users
- Oculus Rift/Google Cardboard style rendering and controls
- Rendering of interactive filesystem/process/device style constructs a la Jurassic Park (THIS IS A UNIX SYSTEM... I KNOW THIS)
