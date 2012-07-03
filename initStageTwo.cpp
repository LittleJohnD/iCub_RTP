/**
 *	initStageTwo.cpp
 *	This program is a refactors stageOne into a more object oreitened design.
 *
 * Date: 23/3/2011
 * Author: John Dyer
 *
 */
#include <yarp/sig/Vector.h>
#include <yarp/os/all.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "stageTwo.cpp"
#include "positionObject.cpp"
using namespace std;
using namespace yarp::os;
using namespace yarp::dev;
using namespace yarp::sig;


class initStageTwo
{
public:
	initStageTwo();
	bool startTest(int reps);
	int sessionIdentifier();
	bool prepLog(int id);
	bool closeLog();
	bool setUpVect(int reps);
	bool checkDataSet(yarp::sig::Vector posVect);
	bool insertData(yarp::sig::Vector posVect,int currentPlace);
	bool searchData(yarp::sig::Vector posVect,int currentPlace);
};

int sesID,objectID,reps;
double tolerance;
ofstream logFile;
Vector currentPosDataVect;
vector<positionObject*> positionsVect;
string tmpLine;

positionObject* tempPositionObject;
stageTwo* stageTwoTest;
initStageTwo* testFrame;

int main(int argc,const char* argv[])
{
	cout<<"It Begins"<<endl;
	stageTwoTest = new stageTwo();
	testFrame = new initStageTwo();
	int initialReps = 100;
	testFrame->startTest(initialReps);
	return 0;
}

initStageTwo::initStageTwo()
{
    sesID = testFrame->sessionIdentifier();
    cout<<"SessionID: "<<sesID<<endl;
    tolerance = 5;
	reps = 100;
}
int initStageTwo::sessionIdentifier()
{
	fstream sessionID;
	sessionID.open("/home/little-john/Documents/iCub/Programs/stageTwo/logs/sessionID.conf");
	if(sessionID.is_open())
		getline(sessionID,tmpLine);
	int i=atoi(tmpLine.c_str());
	i++;
	sessionID<<i;
	sessionID.close();
	return i;
}

bool initStageTwo::prepLog(int id)
{
	string fileName = "/home/little-john/Documents/iCub/Programs/stageTwo/logs/log" + static_cast<ostringstream*>( &(ostringstream() << id) )->str() + ".txt";
	logFile.open(fileName.c_str());
	if (logFile.is_open())
		logFile<<"This is a log of size: "<< reps<< "\n";
	else
		cout<<"Error opening File"<<endl;

	return 0;
}

bool initStageTwo::closeLog()
{
	logFile.close();
	return 0;
}

bool initStageTwo::startTest(int tmpReps)
{
	cout<<"Starting Test"<<endl;
	testFrame->prepLog(sesID);
	//testFrame->setUpVect(tmpReps);
	int j = 0;
	for(int i=tmpReps;i>=0;i--)
	{
		cout<<"Repetition: "<< j <<endl;
		currentPosDataVect = stageTwoTest->movement();
		testFrame->checkDataSet(currentPosDataVect);
		j++;
	}
	cout<<"End of Test"<<endl;
	closeLog();
	stageTwoTest->closeGaze();
	return 0;
}

bool initStageTwo::setUpVect(int size)
{
	int i = size + 1;
	objectID=1;
	positionVect(i);
	return 0;
}

bool initStageTwo::checkDataSet(yarp::sig::Vector posVector)
{
	//cout<<"Check Data"<<endl;
	int k = positionsVect.size() - 1;
	//cout<<"k is : "<<k<<endl;
	if(k==-1)
	{
		cout<<"Straight Insert Data"<<endl;
		testFrame->insertData(posVector,0);
	}
	else
	{
		for(int l=0;l<=k;l++)
		{
				//cout<<"L= "<<l<<endl;
				if(testFrame->searchData(posVector,l))
				{
					cout<<"Insert Data"<<endl;
					testFrame->insertData(posVector,l);
					break;
				}
		}
	}


}

bool initStageTwo::searchData(yarp::sig::Vector checkVector,int currentPlace)
{
	//cout<<"Search Data"<<endl;
	yarp::sig::Vector tempVector(11);
	positionsVect.at(currentPlace)->getData(tempVector);
//	cout<<"vector [0]: "<<checkVector[0]-tempVector[0];
//	cout<<" vector [1]: "<<checkVector[1]-tempVector[1];
//	cout<<" vector [2]: "<<checkVector[2]-tempVector[2]<<endl;

	if((checkVector[0]-tempVector[0])<tolerance)
		return 0;
	else if((checkVector[1]-tempVector[1])<tolerance)
		return 0;
	else if((checkVector[2]-tempVector[2])<tolerance)
		return 0;
	else
		return 1;
}

bool initStageTwo::insertData(yarp::sig::Vector insertVect,int currentPlace)
{
	positionObject* newPositionDataPoint =  new positionObject();
	newPositionDataPoint->addData(insertVect,currentPlace);
	positionsVect.push_back(newPositionDataPoint);
	logFile<<insertVect.toString()<<endl;
	return 0;
}
