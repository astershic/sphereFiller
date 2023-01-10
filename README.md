sphereFiller
============

Recreate the sphere-filling algorithm described by:

Ferellec, Jean-Francois, and Glenn R. McDowell. "A method to model realistic particle shape and inertia in DEM." _Granular Matter_ 12.5 (2010): 459-467. https://link.springer.com/article/10.1007/s10035-010-0205-8 

Command Format:
```bash
./sphereFiller.exe inputFile [nspheres] [density] [minDist]
```

Input Arguments:
- ```inputFile```, in Abaqus input file format, separated by particle (required)
- ```nspheres``` Number of Spheres per particle [default = 1]
- ```density``` Density of particle [default = 1.0]
- ```minDist``` Minimum distance between base nodes of generated spheres [default = 0.0]

Output File:	
- Filename: ```inputFile``` - ".inp" + ".out"
- prints: diameter, density, xc, yc, zc

Sample steps to run program:
```bash
# customize Makefile
make
./sphereFiller.exe Two_Grain_Shell.txt 100 2600.0 0.0 1
```
