bool initEyes();
bool updateChain();
//bool moveEyes();
bool initGazeControl();
bool initDataStruct();
bool incDataStruct();
double halfNumber(double);
bool getValues();
bool lookAtPoint();
bool addValuesToStruct(yarp::sig::Vector,yarp::sig::Vector);
bool checkMotionStop(bool);
bool displayStruct();

yarp::dev::PolyDriver 	*headDriver;
yarp::dev::PolyDriver 	*rightArmChainDriver;
yarp::dev::IEncoders 	*encs;
yarp::dev::PolyDriver	clientGazeCtrl;
yarp::dev::IGazeControl *iGaze;
iCub::iKin::iKinChain* rightArmChain;

struct motorGazePoint
{
	double* panValue;
	double* tiltValue;
	double* versionValue;
	double* rollMotorAngleValue;
	double* pitchMotorAngleValue;
	double* yawMotorAngleValue;
	double* elbowMotorAngleValue;
	int		current;
}rightArm;



YARP_DECLARE_DEVICES(icubmod);
