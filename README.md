sphereFiller
============

Recreate the sphere-filling algorithm described by Jean-Francois Ferellec

INPUT ARGUMENTS:
+Input File - in Abaqus input file format, separated by particle
-Number of Spheres per particle [1]
-Density of particle [1.0]
-Minimum distance between base nodes of generated spheres [0.0]

(+) Required
(-) Optional [default value]

OUTPUT FILE:	InputFile - ".inp" + ".out"
prints: diameter, density, xc, yc, zc





TODO:
-figure out why some very large particles are made
?-change output format to input for liggghts
?-parallelize
