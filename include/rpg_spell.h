#ifndef __RPG_SPELLS_H__
#define __RPG_SPELLS_H__

typedef struct
{
	ItemType type;
	char *name;
	char *description;

	float weight;

}Spell;

typedef enum
{
	potion,
	materials,
	weapon,
	armor,
}ItemType;

#endif
