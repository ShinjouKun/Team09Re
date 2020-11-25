#include "ObjectManager.h"


ObjectManager::ObjectManager()
{
}

ObjectManager::~ObjectManager()
{
	Claer();
}

void ObjectManager::Claer()
{
	for (auto addObj:addObjectList)
	{
		if (addObj != nullptr)
			delete addObj;
	}
	addObjectList.clear();
	for (auto obj : objectList)
	{
		if (obj != nullptr)
			delete obj;
	}
	objectList.clear();
}

void ObjectManager::Add(BaseObject * addObj)
{
	addObjectList.emplace_back(addObj);//�ǉ�
}

void ObjectManager::Update()
{
	AddListUpdate();
	ObjectManagerUpdate();
	RemoveListUpdate();
	Draw();
}

void ObjectManager::AddListUpdate()
{
	for (auto addObj : addObjectList)
	{
		//�I�u�W�F�N�g���X�g�Ɉڂ��ď��������������s
		objectList.emplace_back(addObj);
		addObj->Init();
	}
	addObjectList.clear();
}

void ObjectManager::ObjectManagerUpdate()//��ɓ����蔻��Ȃ�
{
	for (auto obj1:objectList)
	{
		if (obj1 == nullptr || obj1->GetDeath())continue;
			obj1->Update();
		for (auto obj2:objectList)
		{
			if (obj2 == nullptr || obj2->GetDeath())continue;
			{
				//���΋�
				if (obj1->StoS_Col(*obj2))
				{
					obj1->Hit(*obj2);
					obj2->Hit(*obj1);
				}
			}
		}
	}
}

void ObjectManager::RemoveListUpdate()
{
	auto itr = objectList.begin();
	while (itr != objectList.end())
	{
		//null�`�F�b�N
		if ((*itr) == nullptr)
		{
			itr = objectList.erase(itr);
		}
		else if((*itr)->GetDeath())
		{
			BaseObject* deleteObj = (*itr);
			itr = objectList.erase(itr);
			delete deleteObj;
		}
		else
		{
			itr++;
		}
	}
	
}

void ObjectManager::Draw()
{
	for (auto obj : objectList)
	{
		if (obj == nullptr)continue;
		if (!obj->GetDeath())
			obj->Rend();
		else
		{
			continue;
		}
	}
}

std::vector<BaseObject*> ObjectManager::getUseList()
{
	return objectList;
}

