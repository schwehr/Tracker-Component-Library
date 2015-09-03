**Tracker Component Library Release 1.0, August 2015**

These are the release notes for the first release of the Tracker Component
Library. The Tracker Component Library is a collection of Matlab routines
(some requiring compilation) for simulating and tracking targets in
various scenarios. Due to the complexity of the target tracking problem,
a great many routines can find use in other areas including combinatorics,
astronomy, and statistics.

To use the full library, add the library to your active path in Matlab and
run the CompileCLibraries function. This assumes that a C/ C++ compiler
been set up in Matlab for the mex command. This does not require the
purchase of any Matlab toolboxes. For those who do not have such a
compiler that will work with Matlab, free options are the Windows SDK
from Microsoft and XCode for Mac OS X. Note that the command
line tools must be installed under Mac OS X for Matlab to be able to use
the compiler.

Due to GitHub's limit that files not exceed 100MB, the ephemeris data file
for the astronomical code has been omitted. To use any of the files
requiring planetary ephemerides, such as solarBodyVec.m, please download
the file de430.bsp from
http://naif.jpl.nasa.gov/pub/naif/generic_kernels/spk/planets/
and place it in the folder "Astronomical Code/data".

The subroutines in the folders are part of the Distribution A release of
the tracker component library. The algorithms have been broken into a
number of different categories. Each routine is commented, though when
dealing with compilable code without a native Matlab implementation, the
comments will not show up using the "help" command in Matlab.

Some routines in the Tracker Component Library are available in compiled
as well as native Matlab versions. When both versions are available,
comments on the usage of the file are generally more detailed in the
Matlab implementation file. If the compilation fails, then make sure that
the compiler has been properly setup for Matlab. Additionally, when using
older versions of Matlab under Mac OS X, Matlab has numerous problems as
described below. Compilation has been tested under Mac OS X 10.10 and
Windows 7. Compilation should also work under various versions of Linux
and Unix.

The third party code folder consists of open source code with permissive
licenses (e.g. not GPL) that are used as subroutines in functions in the
Tracker Component Library.


**COMPILING WITH MATLAB 2013b UNDER MAC OS X:**
 
No special work should be necessary to compile the Tracker Component
Library under Matlab 2015a under Mac OS X or Windows. One the other hand,
Matlab 2013b contains a number of bugs in its default compile script that
must be corrected in order to successfully compile the code. Most of these
have been fixed in Matlab 2014a. Additionally, under both versions of
Matlab, one must make sure that the command line versions of the developer
tools have been installed.
 
To compile the code under Mac OS X, make sure that XCode is installed
along with the command-line developer tools. XCode can be obtained for
free through the App Store. The command line developer tools are not
installed by default. To install them, open a terminal window (In
/Applications/Utilities/Terminal ) type the command "cc" on the command
line and hit return. If the command line developer tools are installed,
you will get the message
clang: error: no input files
If the developer tools are not installed, then a window should open up
saying
The "cc" command requires the command line developer tools. Would you like
to install the tools now?
In that case, just click "Install". If the command line tools are not
installed, one will generally get an error like
xcrun: error: unable to find utility "clang++", not a developer tool or in
PATH when trying to compile in Matlab.
 
Whereas the default compile script for Matlab2014a appears to work
properly, the default compile script for Matlab2013b does not work
properly under Mac OS X, because Matlab tries to link to an older library
that might not be installed. Thus, if one is running Mac OS X 10.8 or
later, one will get the error    
xcodebuild: error: SDK "macosx10.7" cannot be located
when trying to compile. The fix for this is to open the terminal and enter
the command
open ~/.matlab/R2013b/mexopts.sh
This should open the compile script file that is used by Matlab in XCode.
The reason that the terminal is used to open the file is because Matlab
hides the file inside of an invisible folder in the user's home directory.
In the file, change the deployment target and the SDK to the version of
Mac OS X being used. For example, if one is using Mac OS X 10.9, then the
following lines should be present in various parts of the file (instead of
using versions less than 10.9)
CC='xcrun  -sdk macosx10.9  clang'
MACOSX_DEPLOYMENT_TARGET='10.9'
CXX='xcrun  -sdk macosx10.9  clang++'
 
Next, Matlab 2013b does not properly define all of the types in its own
headers. Thus, the '-U__STDC_UTF_16__' flag is used in all of the mex
commands in CompileCLibraries to prevent errors.
