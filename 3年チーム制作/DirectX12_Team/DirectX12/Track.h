#pragma once
#include"Vector3.h"
#include"Easing.h"
#include<vector>
#include<map>

class Track
{
public:

	Track();
	~Track();

	void Initilize();

	void Updata();

	//�@�g���b�N�|�C���g�𑝂₷
	void AddTrackPoint(int tracknumber,
		const Vector3& trackPos);

	Vector3 PointMove(Vector3& start,
		Vector3& end,
		float time ,
		float dis);

	//�@���̃|�C���g
	void NextPointMove();

private:

	Vector3 StartPoint() {
		return _startPoint ;
	}

	Vector3 EndPoint() {
		return _endPoint ;
	}


private:

	//�@�X�^�[�g�n�_�A�ŏI�����n�_
	Vector3 _startPoint,_endPoint;
	
	//Vector3 _trackPointPos[];

	std::vector<Vector3>*_trackPoint; //�@�g���b�N�|�C���g�i�[
	std::map<int, std::vector<Track>> _track;

	Vector3 position;

	static bool WaitActtion;


};







