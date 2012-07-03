/**
 *	stageOne.cpp
 *	This program completes the first stage of the research moving the arm and then focusing on it.
 *
 * Date: 11/12/2011
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


#include "stageOne.h"
#include "utilities.h"
#include "armController.cpp"
#include "eyeController.cpp"

using namespace std;
using namespace yarp::os;
using namespace yarp::dev;
using namespace yarp::sig;
using namespace yarp::math;
using namespace iCub::ctrl;
using namespace iCub::iKin;

double currentDepth;
double currentAngle;
int dataArraySize = 100;
Vector eyeAngles,gazePoint,endEffector,q0,currentPos;
bool* mStop=0;

	
int main(int argc, char* argv[])
{
	cout<<"main";
	srand(time(NULL));
	Network yarp;
	armController armCon;
	YARP_REGISTER_DEVICES(icubmod);
	cout<<"Start";
	initDataStruct();
	cout<<"Data";
	initGazeControl();
	cout<<"Gaze";
	rightArmChainDriver = armCon.getRightPoly();
	rightArmChainDriver->view(encs);
	iCubArm* rightiCubArm = new iCubArm("right");
	rightArmChain = rightiCubArm->asChain();
	if(!rightiCubArm->isValid())
			cout<< "error with iKin" << endl;
	cout<<"updatingChain";
	updateChain();
	cout<<"updatedChain";
	int l =0;
	while (l<10)
	{
//		int i = 0;
//		while (i<2)
//		{
			armCon.babble(1);


			//checkMotionStop(&mStop);
			updateChain();
//			endEffector = rightiCubArm->EndEffPose(0);
//			lookAtPoint();
//
//			getValues();
//			armCon.getCurrentPosition(currentPos,1);
//			i++;
//			Time::delay(1);
//		}
		endEffector = rightiCubArm->EndEffPose(0);
		lookAtPoint();
		getValues();
//		bool success;
//		do
//		{
//			success = armCon.getCurrentPosition(currentPos,1);
//		}while(!success);

//		int i =0;
//		cout<< "Current position angles are: ";
//		while(i<4)
//		{
//			cout << " " << currentPos[i];
//			i++;
//		}
//		cout <<endl<<endl;
		addValuesToStruct(eyeAngles,currentPos);
		l++;
	}
	displayStruct();
	return 0;

}

bool updateChain()
{

	Vector fb, encodervalues;
	encodervalues.resize(16);
	encs -> getEncoders(encodervalues.data());
//	cout << encodervalues.toString() <<endl;
//	int i=0;
//	while(i<5)
//	{
//		cout << encodervalues.toString() <<endl;
//		i++;
//	}
	//cout << "Angles values  from encoders: " << encodervalues.toString() <<endl;
	fb = Vector(16, encodervalues.data());
	fb = (M_PI*fb)/180; //Conversion to radians
	//cout << "Angles in radian: " << fb.toString() << endl;
	rightArmChain->setAng(fb); //kinematic
}



bool initGazeControl()
{
	cout<<"init Gaze";
	Property option;
	option.put("device","gazecontrollerclient");
	option.put("remote","/iKinGazeCtrl");
	option.put("local","/gaze_client");
	 
	clientGazeCtrl.open(option);
	
	if (clientGazeCtrl.isValid())
	   clientGazeCtrl.view(iGaze);

	iGaze->blockNeckPitch();
	iGaze->blockNeckRoll();
	iGaze->blockNeckYaw();

	return 0;
}

bool initDataStruct()
{
	rightArm.panValue 				= new double[dataArraySize];
	rightArm.tiltValue  			= new double[dataArraySize];
	rightArm.versionValue 			= new double[dataArraySize];
	rightArm.rollMotorAngleValue 	= new double[dataArraySize];
	rightArm.pitchMotorAngleValue 	= new double[dataArraySize];
	rightArm.yawMotorAngleValue 	= new double[dataArraySize];
	rightArm.elbowMotorAngleValue 	= new double[dataArraySize];
	rightArm.current				= 0;
	return 0;
}

bool incDataStruct()
{
	int tmpInt;
	string tmpString;
	double* tmpArrayPan[dataArraySize];
	double* tmpArrayTilt[dataArraySize];
	double* tmpArrayVersion[dataArraySize];
	double* tmpArrayRollMotor[dataArraySize];
	double* tmpArrayPitchMotor[dataArraySize];
	double* tmpArrayYawMotor[dataArraySize];
	double* tmpArrayElbowMotor[dataArraySize];
	memcpy(tmpArrayPan, 		rightArm.panValue,		   		dataArraySize);
	memcpy(tmpArrayTilt, 		rightArm.tiltValue,		  	dataArraySize);
	memcpy(tmpArrayVersion,		rightArm.versionValue,			dataArraySize);
	memcpy(tmpArrayRollMotor, 	rightArm.rollMotorAngleValue,	dataArraySize);
	memcpy(tmpArrayPitchMotor, 	rightArm.pitchMotorAngleValue,	dataArraySize);
	memcpy(tmpArrayYawMotor, 	rightArm.yawMotorAngleValue,	dataArraySize);
	memcpy(tmpArrayElbowMotor, 	rightArm.elbowMotorAngleValue,	dataArraySize);
	
//	dataArraySize = dataArraySize * 2;
//	tmpString.panValue 			= new double[dataArraySize];
//	tmpString.tiltValue 		= new double[dataArraySize];
//	tmpString.versionValue 		= new double[dataArraySize];
//	tmpString.motorAngleValue 	= new double[dataArraySize];
//	memcpy(tmpString.panValue, 			tmpArrayPan,		halfNumber(dataArraySize) );
//	memcpy(tmpString.tiltValue, 		tmpArrayTilt,	 	halfNumber(dataArraySize) );
//	memcpy(tmpString.versionValue, 		tmpArrayVersion, 	halfNumber(dataArraySize) );
//	memcpy(tmpString.motorAngleValue, 	tmpArrayMotor, 		halfNumber(dataArraySize) );
	return 0;
}

double halfNumber(double num)
{
	double tmpNum = num / 2;
	return tmpNum;
}

bool getValues()
{
	iGaze->getFixationPoint(gazePoint);
	//cout << "Gaze Point: " << gazePoint.toString() <<endl;
	iGaze->getAngles(eyeAngles);
	//cout << "Eye Angles: " << eyeAngles.toString() <<endl <<endl;
	currentPos.resize(16);
	encs -> getEncoders(currentPos.data());
}
bool lookAtPoint()
{
	q0 = endEffector.subVector(0,2);
	iGaze->lookAtFixationPoint(q0);
	return 0;
}
bool addValuesToStruct(Vector eye,Vector motor)
{
	int k = rightArm.current;
	rightArm.panValue[k]				= eye[0];
	rightArm.tiltValue[k]  				= eye[1];
	rightArm.versionValue[k] 			= eye[2];
	rightArm.rollMotorAngleValue[k]		= motor[0];
	rightArm.pitchMotorAngleValue[k]	= motor[1];
	rightArm.yawMotorAngleValue[k]		= motor[2];
	rightArm.elbowMotorAngleValue[k] 	= motor[3];
	k++;
	rightArm.current = k;
}
bool checkMotionStop(bool stop)
{
	bool* tmpbool = &stop;
	iGaze->checkMotionDone(tmpbool);
	while(!stop)
	{
		iGaze->checkMotionDone(tmpbool);
		Time::delay(0.2);
	}
	return tmpbool;
}

bool displayStruct()
{
	int k = rightArm.current;
	double tmpValues[7];
	k--;
	while(k>=0)
	{
		tmpValues[0] = rightArm.panValue[k];
		tmpValues[1] = rightArm.tiltValue[k];
		tmpValues[2] = rightArm.versionValue[k];
		tmpValues[3] = rightArm.rollMotorAngleValue[k];
		tmpValues[4] = rightArm.pitchMotorAngleValue[k];
		tmpValues[5] = rightArm.yawMotorAngleValue[k];
		tmpValues[6] = rightArm.elbowMotorAngleValue[k];
		cout<<"For data " << k << ", values of:	";
		for(int i =0;i<7;i++)
		{
			printf("%3.2f	",tmpValues[i]);
		}
		cout <<endl;
		k--;
	}
	return 0;
}
