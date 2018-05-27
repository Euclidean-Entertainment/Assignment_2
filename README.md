# Assignment_1
COSC1187 Assignment 1

Copyright 2018 Tim Davis and Jesse Buhagiar

## Build Instructions
### Linux

Install _cmake_ and _Doxygen_

Make sure you have GLUT, glu and OpenGL installed on your system (most likely mesa-libgl etc)

Type _cmake CMakeLists.txt_ from the root project directory and hit enter

Type _make_ and hit enter

_cd_ to the _/bin/_ directory and then run the binary.

### Windows

_Note_: You really should be using MinGW w64 on a Windows based system. Make sure it's in your path. 

Run build.bat

Enter the directory where your library/include directories are (e.g C:/C++Libs)

Hit enter

Type _mingw32-make_

Go to the binary directory and you should be able to run the executable

_Note_: You may and probably _will_ get linker errors on the first try; building on Windows is a tad bit complicated.

## Making a pull request

First make sure your master branch is up to date (_git pull upstream master_)

Create your branch (_git branch mybranch && git checkout mybranch_) and make your changes

Once you're done making changes, _git add_ all the files you have changed, then type _git commit_ and type your commit message

Once that has been completed, type _git push -u origin mybranch_, where _mybranch_ is the name of your branch

Your changes will now be on github and you can open a pull request from your repo

## I made a booboo in my PR!!!! What now!?!?!?!? 

Firstly, don't panic.

Make the changes you need to make, and then make a normal push. The commit message doesn't matter.

Type _git stash_ to stash any local changes

Then type _git rebase -i HEAD~2_. 

Change the bottom most commit from _pick_ to _squash_

Then edit out the _SECOND_ commit message in your editor

Then type _git push -u origin +mybranch_

You're mistake has now been fixed :^)
