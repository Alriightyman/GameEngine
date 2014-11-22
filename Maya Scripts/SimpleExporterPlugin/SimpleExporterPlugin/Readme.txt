A simple example of a maya file translator plugin. Note that the mel script should be placed
somewhere in your scripts directory. (mydocuments/maya/scripts or ~/maya/scripts). either place
the compiled plugin in your plugins directory, or set a path to it in your maya.env file.

Make sure you have set the directory paths correctly for the Maya API includes and libraries, 
and then you should be away. This should work with maya 4.5 upwards.

Under Win32 you'll need VC .NET, i use the free VC++ .NET 2003 Toolkit to compile all of the 
plugins. Under linux make sure that mayald is executable (chmod 755 mayald) then typing make
should do everything for you.... 

