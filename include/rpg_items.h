#ifndef __RPG_ITEMS_H__
#define __RPG_ITEMS_H__

typedef enum
{
	potion,
	materials,
	weapon,
	armor,
}ItemType;

typedef struct
{
	ItemType type;
	char *name;
	char *description;

	float weight;

}Item;

typedef struct
{
	ItemType type;
	char *name;
	char *description;

	Uint8 armor;

	float weight;
}Armor;

typedef struct
{
	ItemType type;
	char *name;
	char *description;

	Uint8 damage;

	float weight;
}Weapon;


#endif
