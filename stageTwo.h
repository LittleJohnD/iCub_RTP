/**
 *	stageTwo.h
 *	This program is a refactors stageOne into a more object oreitened design.
 *
 * Date: 20/2/2011
 * Author: John Dyer
 *
 */
#include <yarp/os/all.h>
#include <yarp/dev/all.h>
#include <yarp/sig/all.h>
#include <algorithm>
#include <iostream>
#include <math.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>
#include <iomanip>
#include <GazeControl.h>
#include <iCub/iKin/iKinFwd.h>
#include <iCub/iKin/iKinIpOpt.h>
#include <iCub/iKin/iKinHlp.h>
#include <iCub/iKin/iKinSlv.h>
#include <gsl/gsl_math.h>

#include "utilities.h"
#include "armController.cpp"


double randomPos[] = 	{5, 10,  0, 15,  0,-30,  0, 60,  6, 55, 30,  0,  0,  0,  0,  0};
YARP_DECLARE_DEVICES(icubmod);

class stageTwo
{
public:
	stageTwo();
	bool updateValues();
	double halfNumber(double);
	bool getValues();
	bool lookAtPoint();
	yarp::sig::Vector movement();
	bool closeGaze();
	bool updateChain();

private:

	bool setUp();
	bool setUpVect();
	bool initGaze();
	bool initChain();

	yarp::dev::PolyDriver	*rightArmChainDriver;
	yarp::dev::IEncoders	*encs;
	yarp::dev::PolyDriver	clientGazeCtrl;
	yarp::dev::IGazeControl	*iGaze;
	iCub::iKin::iKinChain 	*rightArmChain;
};
