/* ****************************************************************** **
**    OpenSees - Open System for Earthquake Engineering Simulation    **
**          Pacific Earthquake Engineering Research Center            **
**                                                                    **
**                                                                    **
** (C) Copyright 1999, The Regents of the University of California    **
** All Rights Reserved.                                               **
**                                                                    **
** Commercial use of this program without express permission of the   **
** University of California, Berkeley, is strictly prohibited.  See   **
** file 'COPYRIGHT'  in main directory for information on usage and   **
** redistribution,  and for a DISCLAIMER OF ALL WARRANTIES.           **
**                                                                    **
** Developed by:                                                      **
**   Frank McKenna (fmckenna@ce.berkeley.edu)                         **
**   Gregory L. Fenves (fenves@ce.berkeley.edu)                       **
**   Filip C. Filippou (filippou@ce.berkeley.edu)                     **
**                                                                    **
** ****************************************************************** */

// $Revision: 6223 $
// $Date: 2016-03-01 19:32:50 -0500 (Tue, 01 Mar 2016) $
// $URL: svn://peera.berkeley.edu/usr/local/svn/OpenSees/trunk/SRC/analysis/integrator/HHTHSIncrReduct.h $

#ifndef HHTHSIncrReduct_h
#define HHTHSIncrReduct_h

// Written: Andreas Schellenberg (andreas.schellenberg@gmail.com)
// Created: 10/05
// Revision: A
//
// Description: This file contains the class definition for HHTHSIncrReduct.
// HHTHSIncrReduct is an algorithmic class for performing a transient analysis
// using the HHT integration scheme. This is a special integrator for hybrid
// simulation where a reduction factor, that modifies the response increments,
// is introduced to enhance the smoothness of the convergence path and to reduce
// the possibility of spurious loading/unloading cycles during iteration.

#include <TransientIntegrator.h>

class DOF_Group;
class FE_Element;
class Vector;

class HHTHSIncrReduct : public TransientIntegrator
{
public:
    // constructors
    HHTHSIncrReduct();
    HHTHSIncrReduct(double rhoInf, double reduct);
    HHTHSIncrReduct(double alphaI, double alphaF,
        double beta, double gamma, double reduct);
    
    // destructor
    ~HHTHSIncrReduct();
    
    // methods which define what the FE_Element and DOF_Groups add
    // to the system of equation object.
    int formEleTangent(FE_Element *theEle);
    int formNodTangent(DOF_Group *theDof);
    
    int domainChanged(void);
    int newStep(double deltaT);
    int revertToLastStep(void);
    int update(const Vector &deltaU);
    int commit(void);
    
    virtual int sendSelf(int commitTag, Channel &theChannel);
    virtual int recvSelf(int commitTag, Channel &theChannel, FEM_ObjectBroker &theBroker);
    
    void Print(OPS_Stream &s, int flag = 0);
    
protected:
    
private:
    double alphaI;
    double alphaF;
    double beta;
    double gamma;
    double reduct;   // disp increment reduction factor 0<reduct<=1
    double deltaT;
    
    double c1, c2, c3;                          // some constants we need to keep
    Vector *Ut, *Utdot, *Utdotdot;              // response quantities at time t
    Vector *U, *Udot, *Udotdot;                 // response quantities at time t + deltaT
    Vector *Ualpha, *Ualphadot, *Ualphadotdot;  // response quantities at time t+alpha*deltaT
    Vector *scaledDeltaU;                       // scaled displacement increment
};

#endif
