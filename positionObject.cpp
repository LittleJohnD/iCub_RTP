/**
 *	positionObject.cpp
 *	This program is a refactors stageOne into a more object oreitened design.
 *
 * Date: 23/3/2011
 * Author: John Dyer
 *
 */



using namespace std;
using namespace yarp::sig;


class positionObject
{
	double checkAng(double current, double newInt,bool less);
	double pan,tilt,verg,mtr0l,mtr0h,mtr1l,mtr1h,mtr2l,mtr2h,mtr3l,mtr3h;
	yarp::sig::Vector positionDataVect;
	int id;

public:
	positionObject();
	bool addData(Vector dataVect,int parameterID);
	Vector getData(Vector outputVect);
};

positionObject::positionObject()
{

}
bool positionObject::addData(yarp::sig::Vector inputDataVect,int parameterID)
{
	int i = parameterID;
	Vector dataVect =inputDataVect;
	id = (int)i;
	pan = dataVect[0];
	tilt = dataVect[1];
	verg = dataVect[2];
	mtr0l = checkAng(mtr0l,dataVect[0],1);
	mtr0h = checkAng(mtr0h,dataVect[0],0);
	mtr1l = checkAng(mtr1l,dataVect[1],1);
	mtr1h = checkAng(mtr1h,dataVect[1],0);
	mtr2l = checkAng(mtr2l,dataVect[2],1);
	mtr2h = checkAng(mtr2h,dataVect[2],0);
	mtr3l = checkAng(mtr3l,dataVect[3],1);
	mtr3h = checkAng(mtr3h,dataVect[3],0);
	return 0;
}

double positionObject::checkAng(double cur,double nw,bool less)// if true less than else greater than
{
	if(less)
	{
		if(nw<cur)
			return nw;
	}
	else if(!less)
	{
		if(nw>cur)
			return nw;
	}
	else
		return cur;

}

Vector positionObject::getData(yarp::sig::Vector outputVect)
{
	outputVect.resize(11);
	positionDataVect.resize(11);
	positionDataVect[0]=pan;
	positionDataVect[1]=tilt;
	positionDataVect[2]=verg;
	positionDataVect[3]=mtr0l;
	positionDataVect[4]=mtr0h;
	positionDataVect[5]=mtr1l;
	positionDataVect[6]=mtr1h;
	positionDataVect[7]=mtr2l;
	positionDataVect[8]=mtr2h;
	positionDataVect[9]=mtr3l;
	positionDataVect[10]=mtr3h;
	outputVect = positionDataVect;
	return outputVect;
}


