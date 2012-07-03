/**
 *	stageTwo.cpp
 *	This program is a refactors stageOne into a more object oreitened design.
 *
 * Date: 20/2/2011
 * Author: John Dyer
 *
 */

#include "stageTwo.h"


using namespace std;
using namespace yarp::os;
using namespace yarp::dev;
using namespace yarp::sig;
using namespace yarp::math;
using namespace iCub::ctrl;
using namespace iCub::iKin;

Vector eyeAnglesVect,endEffectorVect,fixPointVect,encoderValuesVect,positionVect;
armController* armCon;
iCubArm* rightiCubArm;

stageTwo::stageTwo()
{
	Network yarp;
	YARP_REGISTER_DEVICES(icubmod);
	srand(time(NULL));
	armCon = new armController();
	setUp();
	updateValues();
}

bool stageTwo::setUp()
{
	armCon->setUp();
	initGaze();
	setUpVect();
	initChain();
	cout<<"Set up complete"<<endl;
	return 0;
}

bool stageTwo::initGaze()
{
	bool check= true;
	Property gazeOption;
		gazeOption.put("device","gazecontrollerclient");
		gazeOption.put("remote","/iKinGazeCtrl");
		gazeOption.put("local","/gaze_client");
	clientGazeCtrl.open(gazeOption);
	iGaze=NULL;
	//cout<<"opened gaze"<<endl;
	if (clientGazeCtrl.isValid())
	   clientGazeCtrl.view(iGaze);
	//cout<<"checked gaze"<<endl;
	iGaze->setTrackingMode(!check);
	//iGaze->blockNeckPitch(-20);
	//iGaze->blockNeckRoll();
	//iGaze->blockNeckYaw(-15);
	//cout<<"locked gaze"<<endl;
	return 0;
}

bool stageTwo::closeGaze()
{
	clientGazeCtrl.close();
}

bool stageTwo::initChain()
{

//	//cout<<"opened parrot"<<endl;
//	rightArmChainDriver->view(encs);
	rightiCubArm = new iCubArm("right");
	//cout<<"opened rightArmChain"<<endl;
	rightArmChain = rightiCubArm->asChain();
	//cout<<"chain->chain"<<endl;
	if(!rightiCubArm->isValid())
		cout<< "error with iKin" << endl;
	encs = armCon->getRightEnc();
	//cout<<"checked chain"<<endl;
	updateChain();
	//cout<<"finished chain"<<endl;
	return 0;
}

bool stageTwo::updateChain()
{

	//cout<<"opened updateChain()"<<endl;
//	rightArmChainDriver->getEncoders(encoderValuesVect.data());
	encoderValuesVect.zero();
	encs->getEncoders(encoderValuesVect.data());
	rightArmChain->setAng(encoderValuesVect); //kinematic
	//cout<<"finished updateChain()"<<endl;
	return 0;
}

bool stageTwo::lookAtPoint()
{
	Vector tempVect(3);
	bool check = false;
	fixPointVect = endEffectorVect.subVector(0,2);
	cout<<"See fixation Point Vect: "<<fixPointVect.toString()<<endl;
	iGaze->lookAtFixationPoint(fixPointVect);
	iGaze->waitMotionDone();
	iGaze->getFixationPoint(tempVect);
	cout<<"Fixation point check: "<<fixPointVect.toString()<<endl;
	cout<<"Fixation point from iGaze: "<<tempVect.toString()<<endl;
	if(fixPointVect==tempVect)
		cout<<"Equals"<<endl;
	return 0;
}

bool stageTwo::updateValues()
{
	//cout<<"Updating values ... ";
	updateChain();
	//cout<<"updated Chain, ";
	endEffectorVect = rightArmChain->EndEffPose(0);
	//cout<<"Vector copied, ";
	lookAtPoint();
	//cout<<"Looked at Point, ";
	iGaze->getAngles(eyeAnglesVect);
	//cout<<"Finished updating ... ";
	return 0;
}

bool stageTwo::setUpVect()
{
	encoderValuesVect.resize(16);
	endEffectorVect.resize(6);
	positionVect.resize(8);
	eyeAnglesVect.resize(15);
	return 0;
}

Vector stageTwo::movement()
{
	//cout<<"Moving .... ";
	armCon->babble(1);
	while(!armCon->armsStationary())
	{
		sleep(5);
//		if(armCon->armsStationary())
//			break;
	}
	sleep(5);
	//cout<<"Babbled, ";
	updateValues();
	//cout<<"Updated Values, ";
	int j = 0;

	for(int i=0;i<7;i++)
	{

		if(i<4)
			positionVect[i]=eyeAnglesVect[i];
		else
		{
			positionVect[i]=encoderValuesVect[j];
			j++;
		}
	}
	//cout<<"Finished Moving"<<endl;
	return positionVect;
}
