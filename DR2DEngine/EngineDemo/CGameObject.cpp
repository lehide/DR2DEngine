#include "CGameObject.h"
#include "IDREngine.h"
#include <cmath>



void COBB::MakeEdges()
{
	Vertices.clear();
	Edges.clear();

	Vertices.push_back(p1);
	Vertices.push_back(p2);
	Vertices.push_back(p3);
	Vertices.push_back(p4);

	edge1.x = p2.x - p1.x;
	edge1.y = p2.y - p1.y;

	edge2.x = p3.x - p2.x;
	edge2.y = p3.y - p2.y;

	//edge3.x = p4.x - p3.x;
	//edge3.y = p4.y - p3.y;

	//edge4.x = p1.x - p4.x;
	//edge4.y = p1.y - p4.y;

	Edges.push_back(edge1);
	Edges.push_back(edge2);
	//Edges.push_back(edge3);
	//Edges.push_back(edge4);
}

void COBB::RecalcOBB(CVECTOR2& pos)
{
	CVECTOR2 X(cos(m_Angle), sin(m_Angle));
	CVECTOR2 Y(-sin(m_Angle), cos(m_Angle));

	X = X * (m_Width / 2);
	Y = Y * (m_Height / 2);

	p1 = pos - X - Y;
	p2 = pos + X - Y;
	p3 = pos + X + Y;
	p4 = pos - X + Y;

	MakeEdges();
}

CGameObject::CGameObject()
	:m_IsCollided(false), m_CollisionType(eCollisionType::eCC),
	m_Radius(0), m_Magnitude(0), m_AABB(), m_OBB(), m_Position(), m_Reflection()
{

}

CGameObject::~CGameObject()
{

}

void CGameObject::SetCC(float radius)
{
	m_CollisionType = eCollisionType::eCC;
	m_Radius = radius;
}

void CGameObject::SetAABB(float sizeX, float sizeY)
{
	m_CollisionType = eCollisionType::eAABB;
	m_AABB.left = sizeX * -0.5f;
	m_AABB.top = sizeY * -0.5f;
	m_AABB.right = sizeX * 0.5f;
	m_AABB.bottom = sizeY * 0.5f;
}

void CGameObject::SetOBB(float width, float height, float angle)
{
	m_CollisionType = eCollisionType::eOBB;

	m_OBB.m_Width = width;
	m_OBB.m_Height = height;
	m_OBB.m_Angle = DegreeToRad(angle);

	RecalcOBB();
}

void CGameObject::SetPosition(int x, int y)
{
	m_Position.x = (float)x;
	m_Position.y = (float)y;
}

void CGameObject::Render(IDREngine* renderer)
{
	if (renderer != nullptr)
	{
		// 이미지던 뭐던 그린다.
	}
}

void CGameObject::DrawCollisionData(IDREngine* renderer)
{
	if (m_IsCollided == true)
	{
		renderer->SetBrushColor(1.0f, 0, 0, 1.0f);
	}
	else
	{
		renderer->SetBrushColor(0, 1.0f, 0, 1.0f);
	}

	switch (m_CollisionType)
	{
	case eCollisionType::eCC:
	{
		renderer->DrawEllipse((int)m_Position.x, (int)m_Position.y, (int)m_Radius);
	}
	break;

	case eCollisionType::eAABB:
	{
		D2D1_RECT_F _worldAABB = GetWorldAABB();
		renderer->DrawRectangle(_worldAABB.left, _worldAABB.top, _worldAABB.right, _worldAABB.bottom);
	}
	break;

	case  eCollisionType::eOBB:
	{
		renderer->DrawLine(m_OBB.p1.x, m_OBB.p1.y, m_OBB.p2.x, m_OBB.p2.y);
		renderer->DrawLine(m_OBB.p2.x, m_OBB.p2.y, m_OBB.p3.x, m_OBB.p3.y);
		renderer->DrawLine(m_OBB.p3.x, m_OBB.p3.y, m_OBB.p4.x, m_OBB.p4.y);
		renderer->DrawLine(m_OBB.p4.x, m_OBB.p4.y, m_OBB.p1.x, m_OBB.p1.y);
	}
	break;

	default:
	{

	}
	break;

	}
}

bool CGameObject::CollisionCheck_CC(CGameObject* pTarget)
{
	// 1. 두 오브젝트의 circle의 중점의 거리를 구한다.
	float _sqrMagnitude = (float)pow(m_Position.x - (float)pTarget->GetPosX(), 2)
		+ (float)pow(m_Position.y - (float)pTarget->GetPosY(), 2);

	float _magnitude = sqrt(_sqrMagnitude);
	m_Magnitude = _magnitude;

	// 2. 거리가 두 오브젝트의 반지름의 합보다 작거나 같으면 충돌
	if (_magnitude <= m_Radius + pTarget->GetRadius())
	{
		this->SetIsCollided(true);
		pTarget->SetIsCollided(true);

		return true;
	}

	this->SetIsCollided(false);
	pTarget->SetIsCollided(false);

	return true;
}

bool CGameObject::CollisionCheck_CC2(CGameObject* pTarget)
{
	// 1. 두 오브젝트의 위치 차 벡터를 구한다. (타겟에서 내게로 오는 벡터)
	CVECTOR2 _reflVec = m_Position - pTarget->GetPosition();
	
	// 2. 위치 차 벡터의 길이가 (중점의 거리가) 두 오브젝트의 반지름의 합보다
	// 작거나 같으면 충돌
	if (_reflVec.GetMagnitude() <= m_Radius + pTarget->GetRadius())
	{
		this->SetIsCollided(true);
		pTarget->SetIsCollided(true);

		this->m_Velocity = this->m_Velocity + (_reflVec * 0.01f);
		
		/// 타겟 오브젝트의 속도에도 반사 벡터의 역벡터를 넣어 준다.
		pTarget->SetVelocity(pTarget->Velocity() + (_reflVec * -0.01f));
		return true;
	}
	
	this->SetIsCollided(false);
	pTarget->SetIsCollided(false);

	return true;
}

bool CGameObject::CollisionCheck_AABB(CGameObject* pTarget)
{
	D2D1_RECT_F _myWorldAABB = this->GetWorldAABB();
	D2D1_RECT_F _targetWorldAABB = pTarget->GetWorldAABB();

	// 1. 두 오브젝트의 AABB를 이용해서 최소최대를 비교한다.
	// 서로의 좌측이 우측보다 작고, 상단이 하단보다 작으면 충돌
	if (_myWorldAABB.left < _targetWorldAABB.right &&
		_targetWorldAABB.left < _myWorldAABB.right &&
		_myWorldAABB.top < _targetWorldAABB.bottom &&
		_targetWorldAABB.top < _myWorldAABB.bottom)
	{
		this->SetIsCollided(true);
		pTarget->SetIsCollided(true);

		return true;
	}

	this->SetIsCollided(false);
	pTarget->SetIsCollided(false);

	return false;
}

bool CGameObject::CollisionCheck_OBB(CGameObject* pTarget)
{
	bool _IsCollision = true;

	// 알기 쉽게, OBB만 가져온다.
	COBB& _obb1 = this->m_OBB;
	COBB& _obb2 = pTarget->m_OBB;

	this->RecalcOBB();
	pTarget->RecalcOBB();

	/// 최소최대값. 시작값을 무조건 만들어줘야 한다.
	float _minOBB1 = _minOBB1 = +100000000000.f;
	float _maxOBB1 = _maxOBB1 = -100000000000.f;
	float _minOBB2 = _minOBB2 = +100000000000.f;
	float _maxOBB2 = _maxOBB2 = -100000000000.f;

	// OBB1의 모든 edge에 대해서, 첫번째 OBB와 두번째 OBB의 모든 정사영의 최소최대값을 각각 구한다.
	for (unsigned int i = 0; i < _obb1.Edges.size(); i++)
	{
		_minOBB1 = +100000000000.f;
		_maxOBB1 = -100000000000.f;
		_minOBB2 = +100000000000.f;
		_maxOBB2 = -100000000000.f;

		for (unsigned int j = 0; j < _obb1.Vertices.size(); j++)
		{
			_minOBB1 = min(_minOBB1, DotProduct(_obb1.Edges[i], _obb1.Vertices[j]));
			_maxOBB1 = max(_maxOBB1, DotProduct(_obb1.Edges[i], _obb1.Vertices[j]));

			_minOBB2 = min(_minOBB2, DotProduct(_obb1.Edges[i], _obb2.Vertices[j]));
			_maxOBB2 = max(_maxOBB2, DotProduct(_obb1.Edges[i], _obb2.Vertices[j]));
		}

		// 만약, OBB1의 정사영과 OBB2의 정사영이 겹치지 않는다면, 충돌하지 않은 것이다.
		if ((_minOBB1 < _maxOBB2 && _minOBB2 < _maxOBB1) == false)
		{
			_IsCollision = false;
		}
	}

	// OBB2의 모든 edge에 대해서, 첫번째 OBB와 두번째 OBB의 모든 정사영의 최소최대값을 각각 구한다.
	for (unsigned int i = 0; i < _obb2.Edges.size(); i++)
	{
		/// 최소최대의 시작값을 재지정
		_minOBB1 = +100000000000.f;
		_maxOBB1 = -100000000000.f;
		_minOBB2 = +100000000000.f;
		_maxOBB2 = -100000000000.f;

		for (unsigned int j = 0; j < _obb2.Vertices.size(); j++)
		{
			_minOBB1 = min(_minOBB1, DotProduct(_obb2.Edges[i], _obb2.Vertices[j]));
			_maxOBB1 = max(_maxOBB1, DotProduct(_obb2.Edges[i], _obb2.Vertices[j]));

			_minOBB2 = min(_minOBB2, DotProduct(_obb2.Edges[i], _obb1.Vertices[j]));
			_maxOBB2 = max(_maxOBB2, DotProduct(_obb2.Edges[i], _obb1.Vertices[j]));
		}

		// 만약, OBB1의 정사영과 OBB2의 정사영이 겹치지 않는다면, 충돌하지 않은 것이다.
		if ((_minOBB1 < _maxOBB2 && _minOBB2 < _maxOBB1) == false)
		{
			_IsCollision = false;
		}
	}


	/// 두 개의 오브젝트 * 두 번의 축 검사에도 충돌이 없다면, 충돌하지 않은 것이다.


	// 위의 것에 모두 걸리지 않았다면, 충돌하지 않은 것임
	if (_IsCollision == false)
	{
		this->SetIsCollided(false);
		pTarget->SetIsCollided(false);

		return false;
	}

	this->SetIsCollided(true);
	pTarget->SetIsCollided(true);

	return true;
}

float CGameObject::DotProduct(CVECTOR2 v1, CVECTOR2 v2)
{
	return (v1.x * v2.x) + (v1.y * v2.y);
}

void CGameObject::RecalcOBB()
{
	m_OBB.RecalcOBB(m_Position);
}

void CGameObject::Rotate(float deltaAngle)
{
	m_OBB.m_Angle += DegreeToRad(deltaAngle);

	RecalcOBB();
}

void CGameObject::Accelerate(float dx, float dy)
{
	m_Velocity.x += dx;
	m_Velocity.y += dy;
}

void CGameObject::Move_Stop()
{
	m_Velocity.x = 0;
	m_Velocity.y = 0;
}

void CGameObject::Update()
{
	// 상황이야 어쨌든 속도벡터(delta)값을 위치에 더해준다.
	m_Position = m_Position + m_Velocity;
}

CVECTOR2 CGameObject::GetPosition()
{
	return m_Position;
}

D2D1_RECT_F CGameObject::GetWorldAABB()
{
	// 리턴으로 값 전달
	D2D1_RECT_F _worldAABB;
	_worldAABB.left = m_Position.x + m_AABB.left;
	_worldAABB.top = m_Position.y + m_AABB.top;
	_worldAABB.right = m_Position.x + m_AABB.right;
	_worldAABB.bottom = m_Position.y + m_AABB.bottom;

	return _worldAABB;
}

float CGameObject::DegreeToRad(float degree)
{
	return degree * PI / 180.f;
}
