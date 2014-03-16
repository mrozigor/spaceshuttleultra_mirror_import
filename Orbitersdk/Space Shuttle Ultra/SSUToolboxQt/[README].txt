Currently considering .NET or wxWidgets, this is just as a reference

---------------------------------------------------
Install Qt 5.2.1 or above, for MSVC++ (not MinGW, not GCC)
Installation takes time
Open "SSUToolbox.pro"
Check that Debug and Release path are ok. Should be "(SSU SVN folder)\Orbitersdk\Space Shuttle Ultra\build-SSUToolbox-Desktop_Qt_5_2_1_MSVC2010_32bit-Release" for Release
Click "Configure project" button
Bring up the "Project" page again from the left menu
Select "Run" (Run Settings)
Change the working directory to "(SSU SVN folder)\SSUToolbox"
In the bottom-left of the Qt Creator window change from Debug to Release
Click the "Run" button below to compile and run the project
