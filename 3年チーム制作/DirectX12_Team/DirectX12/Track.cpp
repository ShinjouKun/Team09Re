#include "Track.h"

Track::Track(Vector3 pos)	
{
}

Track::~Track()
{
}

void Track::Initilize()
{
	

}

void Track::Updata()
{
	
}

void Track::AddTrackPoint(int tracknumber,
	const Track& trackPos)
{
	_trackPoint->push_back(trackPos);

}

Vector3 Track::PointMove(Vector3& start,
	Vector3& end, float time, float dis)
{
	_startPoint = start;
	_endPoint = end;
	   
	double x = time / dis;
	double v = x;
	Vector3 ret = (_endPoint - _startPoint) * v + _startPoint;


	return ret;
}
