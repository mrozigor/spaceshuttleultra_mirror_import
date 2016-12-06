This release, created from Rev. 25xx of the Space Shuttle Ultra code, is for use in Orbiter 2016.

Installation:
1. Install Orbiter and other required addons:
	OrbiterSound 4.0 (http://orbiter.dansteph.com/index.php?disp=d)
	KSC hires tiles (http://www.orbithangar.com/searchid.php?ID=5583)
	Vandenberg AFB (http://www.orbithangar.com/searchid.php?ID=2380)
	Antelope Valley scenery pack (http://orbit.medphys.ucl.ac.uk/mirrors/orbiter_radio/tex_mirror.html)
2. Extract files into Orbiter folder, overwriting any existing files.

3. Install the "SSU_Font_A" and "SSU_Font_B" fonts, located in the Orbiter base directory, by opening it and selecting install. After installation the files can be deleted.

4. The following lines need to be added to the list of textures of the "<orbiter_installation>\Config\Base.cfg" file:
SSU\NOR1735tex
SSU\NOR2305tex

5. If using the D3D9 graphics client, the geometry instancing option should be off (Orbiter Launchpad > Video > Advanced > uncheck "Enable geometry instancing").

6. If you encounter the error "msvcp120.dll is missing" you need to download the Microsoft Visual C++ Redistributable for Visual Studio 2013.

7. The displays in SSU require the MFD resolution of 512 x 512 (Orbiter Launchpad > Extra > Instruments and panels > MFD parameter configuration > MFD texture size).


DISCLAIMER: The SSU team is not responsible for any crashes or other problems caused by this addon. Use at your own risk.
