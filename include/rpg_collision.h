#ifndef __RPG_COLLISION_H__
#define __RPG_COLLISION_H__

typedef struct {

	float x, y, z;
	float width, height, depth;
	
}BoxCollider;

typedef struct {

	float x, y;
	float radius;

}CircleCollider;

typedef struct {

	float x, y, z;
	float radius;

}SphereCollider;

typedef struct
{
	Vector3 min;
	Vector3 max;
	
}AABB;


#endif
