#pragma once
#include"Vector3.h"
#include"Easing.h"
#include<vector>
#include<map>

class Track
{
public:

	Track(Vector3 pos);
	~Track();

	void Initilize();

	void Updata();

	//　トラックポイントを増やす
	void AddTrackPoint(int tracknumber,
		const Track& trackPos);

	Vector3 PointMove(Vector3& start,
		Vector3& end,
		float time ,
		float dis);

	//　次のポイント
	void NextPointMove();

private:

	Vector3 StartPoint() {
		return _startPoint ;
	}

	Vector3 EndPoint() {
		return _endPoint ;
	}


private:

	//　スタート地点、最終到着地点
	Vector3 _startPoint,_endPoint;
	
	//Vector3 _trackPointPos[];

	std::vector<Track>*_trackPoint; //　トラックポイント格納
	std::map<int, std::vector<Track>> _track;

	Vector3 position;

	static bool WaitActtion;


};







