#pragma once
#include<vector>
#include<memory>
#include"BaseObject.h"

class ObjectManager
{
public:
	ObjectManager();
	~ObjectManager();
	void Claer();//各リストの初期化
	void Add(BaseObject* addObj);//リストへの追加
	void Update();//総更新
	void AddListUpdate();//追加したリストの更新
	void ObjectManagerUpdate();//リスト内のオブジェクトの更新
	void RemoveListUpdate();//死んだものなどの更新処理

	void Draw();//描画処理
	std::vector<BaseObject*>getUseList();
private:
	std::vector<BaseObject*>objectList;//使うもの
	std::vector<BaseObject*>addObjectList;//追加したものの一時保存用
};
