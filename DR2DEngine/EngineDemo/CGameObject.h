#pragma once

#include <windows.h>
#include <d2d1.h>
#include <vector>
#include "CVECTOR2.h"

class IDREngine;

class COBB
{
public:
	// �̵��� ����� ���� �� (��ġ���� center)
	float m_Width;
	float m_Height;
	float m_Angle;

	// OBB�� �̷�� 4���� ��. ��ġ���̱⵵ �ϰ�, ������ �������� �� �����̱⵵ �ϴ�.
	CVECTOR2 p1;
	CVECTOR2 p2;
	CVECTOR2 p3;
	CVECTOR2 p4;

	CVECTOR2 edge1;
	CVECTOR2 edge2;
	CVECTOR2 edge3;
	CVECTOR2 edge4;

public:
	std::vector<CVECTOR2> Vertices;
	std::vector<CVECTOR2> Edges;

	void MakeEdges();

	void RecalcOBB(CVECTOR2& pos);
};

class CGameObject
{
public:
	enum class eCollisionType
	{
		eCC, eAABB, eOBB,
	};

	CVECTOR2 Velocity() const { return m_Velocity; }
	void SetVelocity(CVECTOR2 val) { m_Velocity = val; }

public:
	CGameObject();
	~CGameObject();

	void SetCC(float radius);
	void SetAABB(float sizeX, float sizeY);
	void SetOBB(float width, float height, float angle);

	void SetPosition(int x, int y);

	void Render(IDREngine* renderer);
	void DrawCollisionData(IDREngine* renderer);

	bool CollisionCheck_CC(CGameObject* pTarget);
	bool CollisionCheck_CC2(CGameObject* pTarget);

	bool CollisionCheck_AABB(CGameObject* pTarget);
	bool CollisionCheck_OBB(CGameObject* pTarget);

	void Accelerate(float dx, float dy);
	void Move_Stop();
	
	void Update();


private:
	float DotProduct(CVECTOR2 v1, CVECTOR2 v2);
	void RecalcOBB();

private:

	// �浹üũ�� �Ǿ��°�?
	bool m_IsCollided;

	// �浹üũ
	eCollisionType m_CollisionType;

	// for CC Collsion Detection
	float m_Radius;
	float m_Magnitude;

	// for AABB Collsion Detection
	D2D1_RECT_F m_AABB;

	// for OBB Collsion Detection
public:
	COBB m_OBB;
	const float PI = 3.1415f;
	void Rotate(float deltaAngle);

private:
	/// ���� ������ ���� ������
	CVECTOR2 m_Position;		// ��ġ
	CVECTOR2 m_Velocity;		// �ӵ�����
	CVECTOR2 m_Acceleration;	// ���ӵ�����. ������� ���� ��

	CVECTOR2 m_Reflection;		// �浹 �� �ݻ纤��


public:
	CGameObject::eCollisionType GetCollisionType() const { return m_CollisionType; }

	int GetPosX() const { return (int)m_Position.x; }
	int GetPosY() const { return (int)m_Position.y; }
	CVECTOR2 GetPosition();

	float GetRadius() const { return m_Radius; }
	float GetMagnitude() const { return m_Magnitude; }

	void SetIsCollided(bool val) { m_IsCollided = val; }

	D2D1_RECT_F GetWorldAABB();

	float DegreeToRad(float degree);

};