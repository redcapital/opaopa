Description
===========

Have you ever played a cool game called PaoPao (Kawai 2)? In short, you have
to find two identical cells following simple rules. The rules are best described
in the following screenshot:

![Rules](http://dl.dropbox.com/u/5109084/opaopa/rules.png)

OpaOpa is a program that tries to find those identical cells in order to help
you win the game a lot faster :) Here is a running game:

![Gameboard](http://dl.dropbox.com/u/5109084/opaopa/gameboard.png)

and these are the tips given by OpaOpa:

![Tips](http://dl.dropbox.com/u/5109084/opaopa/hacked.png)

How to use this?
================

Just download [the single executable](https://github.com/galymzhan/opaopa/raw/master/opaopa.exe)
and run it. Ensure that you've launched PaoPao and started a game. Also make 
sure that PaoPao's window is NOT minimized. After that, just click "GO!" button.
Note that I've tested it only on Windows XP.

How does it work?
=================

OpaOpa grabs the screenshot of gameboard and tries to build in-memory 
representation of matrix of cells based on RGB values of particular pixels.
Sometimes it can do some mistakes (which can lead to bad results).  After that,
special algorithm finds identical cells on this matrix.

At the moment, it's far from ideal. Because of ugly cell detection algorithm,
 a single pixel can screw up everything :) Also, after using it for about 
1 minute or so, PaoPao just suddenly raises "Runtime error" popup window and
exits after closing it. Protection against screenshotter programs? Unlikely, but
possible...

Technologies
============

Written in C++ using pure WinAPI. Also uses [CxxTest](http://cxxtest.tigris.org/)
to do unit testing.
