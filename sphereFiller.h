/*******************************************************************************

  Class <sphereFiller> 

  Author: Andrew Stershic
          Duke Computational Mechanics Lab (DCML)
          Duke University - Durham, NC (USA)
  E-mail: ajs84@duke.edu

  Copyright (c) 2013 Andrew Stershic. All rights reserved. No warranty. No
  liability.

*******************************************************************************/
#include <vector>
#include <string>
#include <algorithm>

#ifndef __SPHEREFILLER_H__
#define __SPHEREFILLER_H__

class SphereFiller {

public:
    //! Constructor
    /*! 
        \brief Loads values for distribution parameters and the number of elements
        \param string : distribution name
        \param parameter1 : first parameter
        \param parameter2 : second parameter
        \param nx : number of nodal points
    */
    //! CTL Empty constructor
    SphereFiller ();

    //! Standard destructor
    ~SphereFiller (); 

    //! Assign
    /*! 
        \brief Switches to the probability distribution methods and writes them to cohPar
        \param cohPar : cohesive law parameter list, either sigma or delta
    */


private:

};

#endif//__SPHEREFILLER_H__
