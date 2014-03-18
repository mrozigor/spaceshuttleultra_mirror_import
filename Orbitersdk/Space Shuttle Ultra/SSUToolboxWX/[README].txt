Official instructions:
http://wxwidgets.blogspot.com/2012/08/how-to-use-294-wxmsw-binaries.html

Download the following files:
	wxMSW-3.0.0_vc120_Dev.7z
	wxMSW-3.0.0_vc120_ReleaseDLL.7z
	wxWidgets-3.0.0_headers.7z

If you use MSVC2010 download "vc100" instead of "vc120".

Put these files on "C:\wx".
Extract *directly* all of them.
Inside "C:\wx" you should now have "include" and "lib" folders full of contents.

Run the solutions.
If you use MSVC2010 create new solutions following the first link and add OrbiterSDK includes and libs or ask me to setup a solution for you.

To run both the standalone program and the DLL you have to put the required wxWidgets libraries in the working directory:
wxbase30ud_vc120.dll
wxmsw30ud_core_vc120.dll

or

wxbase30u_vc120.dll
wxmsw30u_core_vc120.dll
