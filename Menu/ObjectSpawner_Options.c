#include "stdafx.h"
#include "Utils.h"
#include "Vehicle_Options.h"
#include "MenuExecutionHandling.h"
#include "Functions.h"
#include "World_Options.h"
#include "Network_Options.h"
#include "ObjectSpawner_Options.h"
#include "ObjectSpawner_AlphabetLists.h"
#include "ObjectSpawner_Categories.h"

int containerObjectSpawnSelect;
char* containerColourStrings[4] = { "Blue", "Orange", "Red", "White" };

int selectedObjectIndex;
int selectedMovementSpeed = 100;
int selectedRotationSpeed = 100;

void(*spawner_page_to_reopen)(int, int); 

//void(*pointAddConsoleToKickStorage)(int, int*);

void SpawnObjectForSpawner(uint objModel, int suppressKeyboardBuffer)
{
	if (IS_MODEL_IN_CDIMAGE(objModel) && !IS_THIS_MODEL_A_PED(objModel))
	{
		float tx, ty, tz, ground, my_distance_from_ground;//, heading;
		vector3 modelMin, modelMax;
		GET_MODEL_DIMENSIONS(objModel, &modelMin, &modelMax);

		//GetOffsetFromCamera(modelMax.y + 5.0f, &tx, &ty, &tz, &heading);
		//tz += modelMax.z;
		GET_OFFSET_FROM_CHAR_IN_WORLD_COORDS(GetPlayerPed(), 0.0f, modelMax.y + 5.0f, modelMax.z, &tx, &ty, &tz);

		
		if (freeMem->m_map_mod.obj_count < 70) //TODO NATH: Adjust this when object spawner is finished.
		{
			Object tmp;
			if (CreateObject(objModel, tx, ty, tz, 0.0f, &tmp, true))
			{
				uint arr_index;
				if (!addObjectToSpawnerList(tmp, &arr_index))
				{
					printf("~r~CTL: ~w~List is full!");
				}
				else
				{
					//printf("Object added to array index %i", arr_index);
					selectedObjectIndex = arr_index;
					select_this_obj(selectedObjectIndex, 0);
					//selectedManageObject = ctlMem->m_spawned_objects[arr_index].o;
				}
			}
		}
		else
			printf("~r~CTL: ~w~Too many objects spawned, delete some objects or your spawned object map.");
			
	}
	else
	{
		if(suppressKeyboardBuffer)
			printf("~r~CTL: ~w~Model 0x%x not found, or is invalid.", objModel);
		else
			printf("~r~CTL: ~w~Model %s (0x%x) not found, or is invalid.", ctlMem->m_char_buffers.keyboard_input_buffer, objModel);
	}
	//use this to keep all our logic with the spawner consistent
	//return 0;
	//return false;
}

void spawnObjectHex(int unused1, int unused2)
{
	if (showAndGetKeyboardInput("Spawn object model (Hex)", true, "0x", 10, 0, 0.5f, 0.5f, 200, 0, 0))
	{
		uint objModel = HexToIntNath(ctlMem->m_char_buffers.keyboard_input_buffer);
		SpawnObjectForSpawner(objModel, 0);
	}
}

void spawnObjectConvertTypedInput(int unused1, int unused2)
{
	if (showAndGetKeyboardInput("Spawn object model (Name)", false, "", 40, 0, 0.5f, 0.5f, 200, 0, 0))
	//if (showAndGetKeyboardInput("Spawn object (Name: \"BM LADDER\" Hex: \"0x5209B31F\" Decimal: \"1376367391\")~n~", false, "", 40, 0, 0.5f, 0.5f, 200, 0, 0))
	{
		uint objModel;
		if (!STRING_TO_INT(ctlMem->m_char_buffers.keyboard_input_buffer, (int*)&objModel)) //not inputted a dec int.
		{
			if (GET_LENGTH_OF_LITERAL_STRING(ctlMem->m_char_buffers.keyboard_input_buffer) > 6) //MODEL_ potentially, we don't want that
			{
				const char* startString = GET_STRING_FROM_STRING(ctlMem->m_char_buffers.keyboard_input_buffer, 0, 6);
				if (COMPARE_STRING(startString, "MODEL_") || COMPARE_STRING(startString, "model_") || COMPARE_STRING(startString, "MODEL ") || COMPARE_STRING(startString, "model "))
				{
					const char* subString = GET_STRING_FROM_STRING(ctlMem->m_char_buffers.keyboard_input_buffer, 6, GET_LENGTH_OF_LITERAL_STRING(ctlMem->m_char_buffers.keyboard_input_buffer));
					strcpy_s(ctlMem->m_char_buffers.keyboard_input_buffer, subString);
				}
			}
			if (GET_LENGTH_OF_LITERAL_STRING(ctlMem->m_char_buffers.keyboard_input_buffer) > 7) //OBJECT_ potentially, we don't want that
			{
				const char* startString = GET_STRING_FROM_STRING(ctlMem->m_char_buffers.keyboard_input_buffer, 0, 7);
				if (COMPARE_STRING(startString, "OBJECT_") || COMPARE_STRING(startString, "object_") || COMPARE_STRING(startString, "OBJECT ") || COMPARE_STRING(startString, "object "))
				{
					const char* subString = GET_STRING_FROM_STRING(ctlMem->m_char_buffers.keyboard_input_buffer, 7, GET_LENGTH_OF_LITERAL_STRING(ctlMem->m_char_buffers.keyboard_input_buffer));
					strcpy_s(ctlMem->m_char_buffers.keyboard_input_buffer, subString);
				}
			}
			int newLength = GET_LENGTH_OF_LITERAL_STRING(ctlMem->m_char_buffers.keyboard_input_buffer);
			for (int i = 0; i < newLength; i++)
			{
				if (ctlMem->m_char_buffers.keyboard_input_buffer[i] == ' ')
					ctlMem->m_char_buffers.keyboard_input_buffer[i] = '_';
			}
			objModel = GET_HASH_KEY(ctlMem->m_char_buffers.keyboard_input_buffer);

			//if(!IS_MODEL_IN_CDIMAGE(objModel))
				//objModel = HexToIntNath(ctlMem->m_char_buffers.keyboard_input_buffer);
		}
		SpawnObjectForSpawner(objModel, 0);
	}
}

void spawnContainerObject(int un1, int un2)
{
	uint containerModels[4] = { OBJECT_BM_CONTNR_01A, OBJECT_BM_CONTNR_02A, OBJECT_BM_CONTNR_04A, OBJECT_BM_CONTNR_05A };
	SpawnObjectForSpawner(containerModels[containerObjectSpawnSelect], true);
}

void spawnContainerObjectDoorsOpen(int un1, int un2)
{
	if (containerObjectSpawnSelect == 0)
	{
		ADD_NEXT_MESSAGE_TO_PREVIOUS_BRIEFS(false);
		printf("~r~CTL: ~w~Not available in this colour/size.");
		return;
	}
	uint containerModels[4] = { 0x13371337, OBJECT_BM_CONTNR_02C, OBJECT_BM_CONTNR_04C, OBJECT_BM_CONTNR_05C };
	SpawnObjectForSpawner(containerModels[containerObjectSpawnSelect], true);
}

void spawnContainerObjectSmaller(int un1, int un2)
{
	if (containerObjectSpawnSelect == 3)
	{
		ADD_NEXT_MESSAGE_TO_PREVIOUS_BRIEFS(false);
		printf("~r~CTL: ~w~Not available in this colour/size.");
		return;
	}
	uint containerModels[4] = { OBJECT_BM_CONT_BLUE_LOW, OBJECT_BM_CONT_YELLOW_LOW, OBJECT_BM_CONT_RED_LOW, 0x5F3372AF };
	SpawnObjectForSpawner(containerModels[containerObjectSpawnSelect], true);
}

void spawnObjectPredefinedSkatePark(int un1, int un2)
{
	spawner_page_to_reopen = spawnObjectPredefinedSkatePark;
	SetHeader("Spawn object (Skate park)");
	AddItemWithParam("Flat ramp", OBJECT_BM_SKATE_FLATRAMP, SpawnObjectForSpawner, "BM_SKATE_FLATRAMP");
	AddItemWithParam("Fun box", OBJECT_BM_SKATE_FUNBOX, SpawnObjectForSpawner, "BM_SKATE_FUNBOX");
	AddItemWithParam("Half pipe", OBJECT_BM_SKATE_HALFPIPE, SpawnObjectForSpawner, "BM_SKATE_HALFPIPE");
	AddItemWithParam("Kickers", OBJECT_BM_SKATE_KICKERS, SpawnObjectForSpawner, "BM_SKATE_KICKERS");
	AddItemWithParam("Quarter pipe", OBJECT_BM_SKATE_QUARTPIPE, SpawnObjectForSpawner, "BM_SKATE_QUARTPIPE");
	AddItemWithParam("Rail", OBJECT_BM_SKATE_RAIL01, SpawnObjectForSpawner, "BM_SKATE_RAIL01");
	AddItemWithParam("Spine ramps", OBJECT_BM_SKATE_SPINERAMPS, SpawnObjectForSpawner, "BM_SKATE_SPINERAMPS");
}

void spawnObjectPredefinedBarges(int un1, int un2)
{
	spawner_page_to_reopen = spawnObjectPredefinedBarges;
	SetHeader("Spawn object (Barges)");
	AddItemWithParam("Container", OBJECT_EC_BARGE_CONTAINER, SpawnObjectForSpawner, "EC_BARGE_CONTAINER");
	AddItemWithParam("Large 1", OBJECT_EC_BARGE_LRG_1, SpawnObjectForSpawner, "EC_BARGE_LRG_1");
	AddItemWithParam("Large 2", OBJECT_EC_BARGE_LRG_2, SpawnObjectForSpawner, "EC_BARGE_LRG_2");
	AddItemWithParam("Medium 1", OBJECT_EC_BARGE_MED_1, SpawnObjectForSpawner, "EC_BARGE_MED_1");
	AddItemWithParam("Medium 2", OBJECT_EC_BARGE_MED_2, SpawnObjectForSpawner, "EC_BARGE_MED_2");
	AddItemWithParam("Medium 3", OBJECT_EC_BARGE_MED_3, SpawnObjectForSpawner, "EC_BARGE_MED_3");
	//yes 4 doesn't exist.
	AddItemWithParam("Medium 5", OBJECT_EC_BARGE_MED_5, SpawnObjectForSpawner, "EC_BARGE_MED_5");
	AddItemWithParam("Medium 6", OBJECT_EC_BARGE_MED_6, SpawnObjectForSpawner, "EC_BARGE_MED_6");
	AddItemWithParam("Oil", OBJECT_EC_BARGE_OIL, SpawnObjectForSpawner, "EC_BARGE_OIL");

}

void spawnObjectPredefined(int un1, int un2)
{
	spawner_page_to_reopen = spawnObjectPredefined;
	SetHeader("Spawn object (Lists)");
	//INDUSTRIAL
	AddItemCharAlt("Shipping container", false, containerColourStrings, 4, &containerObjectSpawnSelect, spawnContainerObject, "~PAD_X~ = With door open, ~PAD_Y~ = Small version.", spawnContainerObjectDoorsOpen, spawnContainerObjectSmaller);
	AddItemMenu("Barges", spawnObjectPredefinedBarges, nullptr);
	//END INDUSTRIAL
	AddItemMenu("Skate park", spawnObjectPredefinedSkatePark, nullptr); 

}

void SpawnerConfig(int un1, int un2)
{
	SetHeader("Object Spawner -> Config");
	//
	AddItemInt("Movement Speed in %", false, 1, 400, &selectedMovementSpeed, 1, nullptr, nullptr);
	AddItemInt("Rotation Speed in %", false, 1, 400, &selectedRotationSpeed, 1, nullptr, nullptr);
	AddItemBool("INVISIBLE", &ctlMem->m_bit_sets.m_menuPreferences, PREF_OBJ_SPAWNER_INVISIBLE, nullptr, nullptr);
	AddItemBool("INVINCIBLE", &ctlMem->m_bit_sets.m_menuPreferences, PREF_OBJ_SPAWNER_INVINCIBLE, nullptr, nullptr);
	AddItemBool("DYNAMIC", &ctlMem->m_bit_sets.m_menuPreferences, PREF_OBJ_SPAWNER_DYNAMIC, nullptr, nullptr);
	AddItemBool("COLLISSION", &ctlMem->m_bit_sets.m_menuPreferences, PREF_OBJ_SPAWNER_COLLISION, nullptr, nullptr);
	AddItemBool("PICKUPABLE", &ctlMem->m_bit_sets.m_menuPreferences, PREF_OBJ_SPAWNER_PICKUP_ABLE, nullptr, nullptr);
	AddItemBool("FROZEN", &ctlMem->m_bit_sets.m_menuPreferences, PREF_OBJ_SPAWNER_FROZEN, nullptr, nullptr);
}

uint getObjectSpawnCount(int un1, int un2) //need to modify map mods array to use this instead :D
{
	uint count = 0;
	for (int i = 0; i < 70; i++) {
		if (DOES_OBJECT_EXIST(ctlMem->m_spawned_objects[i].o)) count++;
	}
	return count;
}

int GetNextObjectIndex(bool upwards)
{
	if (selectedObjectIndex < 0 || selectedObjectIndex > getObjectSpawnCount(0, 0)) return NULL;

	SpawnerObject *validEntities[70];
	int validCount = 0;

	for (int i = 0; i < 70; i++) 
	{
		if (DOES_OBJECT_EXIST(ctlMem->m_spawned_objects[i].o)) 
		{
			validEntities[validCount++] = &ctlMem->m_spawned_objects[i];
		}
	}

	for (int i = 0; i < validCount - 1; i++) {
		for (int j = i + 1; j < validCount; j++) {
			if (validEntities[i]->creationTime > validEntities[j]->creationTime) {
				SpawnerObject *temp = validEntities[i];
				validEntities[i] = validEntities[j];
				validEntities[j] = temp;
			}
		}
	}

	if (selectedObjectIndex < 0 || selectedObjectIndex >= validCount)
	{
		return NULL;
	}

	int nextIndex;
	if (upwards) 
		nextIndex = (selectedObjectIndex + 1) % validCount;
	else 
		nextIndex = (selectedObjectIndex - 1 + validCount) % validCount;
	
	for (int i = 0; i < 70; i++)
	{
		if (ctlMem->m_spawned_objects[i].o == validEntities[nextIndex]->o)
		{
			return i;
		}
		
	}
	return 0;
}


bool addObjectToSpawnerList(Object obj_to_add, uint* array_index)
{
	if (getObjectSpawnCount(0,0) == 70) 
		return false;

	for (int i = 0; i < 70; i++) 
	{
		if (!DOES_OBJECT_EXIST(ctlMem->m_spawned_objects[i].o)) 
		{
#ifdef Developer
			printf("~r~CTL: ~w~Adding object to array index %i", i);
#endif
			ctlMem->m_spawned_objects[i].o = obj_to_add;
			GET_GAME_TIMER((int*)&ctlMem->m_spawned_objects[i].creationTime);
			
			const char* menuItemText = GetCurrentMenuItemText();
			if (COMPARE_STRING(menuItemText, "Spawn object (Name)") || COMPARE_STRING(menuItemText, "Spawn object (Hex)")) //make sure this is correct with time
			{
				strcpy(ctlMem->m_spawned_objects[i].name, ctlMem->m_char_buffers.keyboard_input_buffer, 32);
				ctlMem->m_spawned_objects[i].name[31] = '\0'; //safety.
			}
			else
			{
				if (COMPARE_STRING(GetCurrentMenuItemText(), "Shipping container"))
				{
					sprintf(ctlMem->m_spawned_objects[i].name, "%s (%s)", GetCurrentMenuItemText(), containerColourStrings[containerObjectSpawnSelect]);
				}
				else
				{
					if(IS_STRING_NULL(GetCurrentMenuItemDesc()))
						strcpy(ctlMem->m_spawned_objects[i].name, GetCurrentMenuItemText(), 32);
					else
						strcpy(ctlMem->m_spawned_objects[i].name, GetCurrentMenuItemDesc(), 32);
				}
			}
			*array_index = i;
			SET_OBJECT_INVINCIBLE(obj_to_add, bitTest(ctlMem->m_bit_sets.m_menuPreferences, PREF_OBJ_SPAWNER_INVINCIBLE));
			SET_OBJECT_COLLISION(obj_to_add, bitTest(ctlMem->m_bit_sets.m_menuPreferences, PREF_OBJ_SPAWNER_COLLISION));
			//bitSet(&Container.BitSet, PB_IsInObjectSpawner);
			//bitReset(&Container.BitSet, PB_IsMenuOpen);
			return true;
		}
	}
	return false;
}


void deleteSpawnedObjects()
{
	for (int i = 0; i < 70; i++) {
		if (DOES_OBJECT_EXIST(ctlMem->m_spawned_objects[i].o)) {
			DeleteObject(&ctlMem->m_spawned_objects[i].o);
			ctlMem->m_spawned_objects[i] = (SpawnerObject) { 0 };
		}
	}
}


void teleport_to_spwaned_object(int arr_index, int un1)
{
	if (DOES_OBJECT_EXIST(ctlMem->m_spawned_objects[arr_index].o))
	{
		float x, y, z;
		//TODO NATH write this.
		GET_OBJECT_COORDINATES(ctlMem->m_spawned_objects[arr_index].o, &x, &y, &z);
		SET_CHAR_COORDINATES(GetPlayerPed(), x, y, z);
	}
}

/*submenu is:
Set Object Effect <Speed Boost>
Attach Object to Player <Head>
Attach Object to Vehicle
Detach Object
Clone Object
Delete Object
INVISIBLE
DYNAMIC
PICKUPABLE
FROZEN
SOLID
Change Rotation Speed
Change Movement Speed 
*/

//Call the setup when X is pressed and menu is currently closed.
//Clear the setup when X is pressed and the menu is currently open- And on that page.

void select_this_obj(int index, int un2)
{
	//BONE_EXTRA3
	selectedObjectIndex = index;
	bitSet(&Container.BitSet, PB_IsInObjectSpawner);

	SetHeader(ctlMem->m_spawned_objects[selectedObjectIndex].name);
	//Object effect (speed boost etc) .. look into this later.
	AddItemInt("Movement Speed in %", false, 1, 400, &selectedMovementSpeed, 1, nullptr, nullptr);
	AddItemInt("Rotation Speed in %", false, 1, 400, &selectedRotationSpeed, 1, nullptr, nullptr);
	AddItem("Attach to player", nullptr, nullptr); //PAD_X = Detach, 
	AddItem("Attach to vehicle", nullptr, nullptr); //PAD_X = Detach,


	AddItemBool("INVISIBLE", &ctlMem->m_bit_sets.m_menuPreferences, PREF_OBJ_SPAWNER_INVISIBLE, nullptr, nullptr);
	AddItemBool("INVINCIBLE", &ctlMem->m_bit_sets.m_menuPreferences, PREF_OBJ_SPAWNER_INVINCIBLE, nullptr, nullptr);
	AddItemBool("DYNAMIC", &ctlMem->m_bit_sets.m_menuPreferences, PREF_OBJ_SPAWNER_DYNAMIC, nullptr, nullptr);
	AddItemBool("COLLISSION", &ctlMem->m_bit_sets.m_menuPreferences, PREF_OBJ_SPAWNER_COLLISION, nullptr, nullptr);
	AddItemBool("PICKUPABLE", &ctlMem->m_bit_sets.m_menuPreferences, PREF_OBJ_SPAWNER_PICKUP_ABLE, nullptr, nullptr);
	AddItemBool("FROZEN", &ctlMem->m_bit_sets.m_menuPreferences, PREF_OBJ_SPAWNER_FROZEN, nullptr, nullptr);


	//AddItem("Clone", nullptr, nullptr);
	//AddItem("Delete", nullptr, nullptr);

	bitReset(&Container.BitSet, PB_IsMenuOpen); //might need to block inputs
	//AddItem("Detach", nullptr, nullptr);
//	bitReset(&Container.BitSet, PB_IsMenuOpen); //might need to block inputs
}

void spawner_list_objects_options(int un1, int un2)
{
	spawner_page_to_reopen = spawner_list_objects_options;
	SetHeader("Spawned objects");
	for (int i = 0; i < 70; i++)
	{
		if (DOES_OBJECT_EXIST(ctlMem->m_spawned_objects[i].o))
			AddItemMenuWithParamAlt(ctlMem->m_spawned_objects[i].name, i, select_this_obj, "~PAD_X~ = Teleport, ~PAD_Y~ = TODO/Delete", teleport_to_spwaned_object);
			//AddItemWithParam(ctlMem->m_spawned_objects[i].name, i, select_this_obj, nullptr);
	}
}

void spawnObjectsStartingLetterA(int un1, int un2)
{
	SetHeader("Spawn object (Starting with \"A\")");
	AddItemWithParam("Wall light", OBJECT_AB_WALL_LIGHT1, SpawnObjectForSpawner, nullptr);
}

void spawnObjectOrderedByLetterOptions(int un1, int un2)
{
	spawner_page_to_reopen = spawnObjectOrderedByLetterOptions;
	SetHeader("Spawn object (Ordered by letter)");
	AddItemMenu("A", spawnObjectsStartingLetterA, "Object names beginning with \"A\".");
	AddItemMenu("B", spawnObjectsStartingLetterB, "Object names beginning with \"B\".");
	AddItemMenu("C", spawnObjectsStartingLetterC, "Object names beginning with \"C\".");
	AddItemMenu("D", spawnObjectsStartingLetterD, "Object names beginning with \"D\".");
	AddItemMenu("E", spawnObjectsStartingLetterE, "Object names beginning with \"E\".");
	AddItemMenu("F", spawnObjectsStartingLetterF, "Object names beginning with \"F\".");
	AddItemMenu("G", spawnObjectsStartingLetterG, "Object names beginning with \"G\".");
	AddItemMenu("H", spawnObjectsStartingLetterH, "Object names beginning with \"H\".");
	AddItemMenu("I", spawnObjectsStartingLetterI, "Object names beginning with \"I\".");
	AddItemMenu("J", spawnObjectsStartingLetterJ, "Object names beginning with \"J\".");
	AddItemMenu("K", spawnObjectsStartingLetterK, "Object names beginning with \"K\".");
	AddItemMenu("L", spawnObjectsStartingLetterL, "Object names beginning with \"L\".");
	AddItemMenu("M", spawnObjectsStartingLetterM, "Object names beginning with \"M\".");
	AddItemMenu("N", spawnObjectsStartingLetterN, "Object names beginning with \"N\".");
	AddItemMenu("O", spawnObjectsStartingLetterO, "Object names beginning with \"O\".");
	AddItemMenu("P", spawnObjectsStartingLetterP, "Object names beginning with \"P\".");
	AddItemMenu("Q", spawnObjectsStartingLetterQ, "Object names beginning with \"Q\".");
	AddItemMenu("R", spawnObjectsStartingLetterR, "Object names beginning with \"R\".");
	AddItemMenu("S", spawnObjectsStartingLetterS, "Object names beginning with \"S\".");
	AddItemMenu("T", spawnObjectsStartingLetterT, "Object names beginning with \"T\".");
	AddItemMenu("U", spawnObjectsStartingLetterU, "Object names beginning with \"U\".");
	AddItemMenu("V", spawnObjectsStartingLetterV, "Object names beginning with \"V\".");
	AddItemMenu("W", spawnObjectsStartingLetterW, "Object names beginning with \"W\".");
	AddItemMenu("X", spawnObjectsStartingLetterX, "Object names beginning with \"X\".");
	AddItemMenu("Y", spawnObjectsStartingLetterY, "Object names beginning with \"Y\".");
}

void ObjectSpawnerOptions(int unused1, int unused2)
{
	SetHeader("World -> Object spawner");
	AddItemMenu("Spawn object (Lists)", spawnObjectPredefined, "Popular objects.");
	AddItemMenu("Spawn object (Categories)", showAllObjectSuperCategories, nullptr);
	AddItemMenu("Spawn object (Ordered by letter)", spawnObjectOrderedByLetterOptions, "Popular objects.");
	AddItem("Spawn object (Name or decimal)", spawnObjectConvertTypedInput, "Input model e.g. \"BM LADDER\", \"1376367391\".");
	AddItem("Spawn object (Hex)", spawnObjectHex, "Input model hash in hex form, e.g. 0x5209B31F.");
	
	if (getObjectSpawnCount(nullptr, nullptr) > 0)
		AddItemMenu("Spawned objects", spawner_list_objects_options, nullptr);
	else
		AddItem("Spawned objects", nullptr, "No objects created with spawner.");

	AddItem("Delete all objects", deleteSpawnedObjects, nullptr);
	AddItemMenu("Object Spawner Config", SpawnerConfig, nullptr);
}

quaternion qMultiply(quaternion left, quaternion right)
{
	quaternion calculated;
	calculated.x = (left.x * right.w + right.x * left.w) + (left.y * right.z) - (left.z * right.y);
	calculated.y = (left.y * right.w + right.y * left.w) + (left.z * right.x) - (left.x * right.z);
	calculated.z = (left.z * right.w + right.z * left.w) + (left.x * right.y) - (left.y * right.x);
	calculated.w = (left.w * right.w) - (left.x * right.x + left.y * right.y + left.z * right.z);
	return calculated;
}

quaternion CreateFromAxisAngle(vector3 axis, float angle)
{
	float halfAngle = angle * .5f;
	float s = (float)SIN(halfAngle);
	quaternion create;
	create.x = (axis.x * s);
	create.y = (axis.y * s);
	create.z = (axis.z * s);
	create.w = ((float)COS(halfAngle));
	return create;
}

void rotateObject(Object obj, vector3 rotation, float degree)
{
	quaternion current;
	GET_OBJECT_QUATERNION(obj, &current.x, &current.y, &current.z, &current.w);
	quaternion q = CreateFromAxisAngle(rotation, degree);
	current = qMultiply(current, q);
	SET_OBJECT_QUATERNION(obj, current.x, current.y, current.z, current.w);
}


Object createObjectCopy(OBJECT_INDEX target)
{
	if (!DOES_OBJECT_EXIST(target))
		return nullptr;

	uint objModel;
	GET_OBJECT_MODEL(target, &objModel);
	vector3 pos;
	GET_OBJECT_COORDINATES(target, &pos.x, &pos.y, &pos.z);
	OBJECT_INDEX clone;
	if (CreateObject(objModel, pos.x, pos.y, pos.z, 0.0f, &clone, true))
	{
		quaternion q;
		GET_OBJECT_QUATERNION(target, &q.x, &q.y, &q.z, &q.w);
		SET_OBJECT_QUATERNION(clone, q.x, q.y, q.z, q.w);
		//we'll need to set other flags
		return clone;
	}
	return 0;
}

void Exit_spawner_mode()
{
	//only exit spawner mode if b is pressed && menu_closed OR on the first X page and menu is open 
	
	bitReset(&Container.BitSet, PB_IsInObjectSpawner);
	if (spawner_page_to_reopen == (void*)spawner_list_objects_options)
		UpdateMenuLevel(bitTest(Container.BitSet, PB_IsMenuOpen) ? 0 : -1, spawner_page_to_reopen);
	else
		UpdateMenuLevel(bitTest(Container.BitSet, PB_IsMenuOpen) ? 1 : 0, spawner_page_to_reopen);

	bitSet(&Container.BitSet, PB_IsMenuOpen); //moved to bottom

	/*
	if (flymod == FLYMOD_MODE_PAUSE)
	{
		flymod = FLYMOD_MODE_ON;
	}
	*/
	if (DOES_CAM_EXIST(scriptedCam))
	{
		SET_CAM_ACTIVE(scriptedCam, false);
		SET_CAM_PROPAGATE(scriptedCam, false);
		ACTIVATE_SCRIPTED_CAMS(false, false);
		UNATTACH_CAM(scriptedCam);
		DESTROY_CAM(scriptedCam);
		while (DOES_CAM_EXIST(scriptedCam)) WAIT(0);
		SET_CAM_BEHIND_PED(GetPlayerPed());
		scriptedCam = INVALID_HANDLE;
	}
	SET_CHAR_INVINCIBLE(GetPlayerPed(), bitTest(ctlMem->m_bit_sets.m_player, playerGodMode));
	SET_CHAR_VISIBLE(GetPlayerPed(), !bitTest(ctlMem->m_bit_sets.m_player, Invisibility));
	SET_CHAR_COLLISION(GetPlayerPed(), true);
	FREEZE_CHAR_POSITION(GetPlayerPed(), false);
	SET_PLAYER_CONTROL(GET_PLAYER_ID(), true);
	SET_POLICE_IGNORE_PLAYER(GET_PLAYER_ID(), false);

	if (IS_CHAR_IN_ANY_CAR(GetPlayerPed()))
	{
		Vehicle pVeh;
		pVeh = GetPlayerVeh();
		SET_CAR_VISIBLE(pVeh, !bitTest(ctlMem->m_bit_sets.m_vehicle, vehicleInvisibility));
		SET_CAR_COLLISION(pVeh, true);
		FREEZE_CAR_POSITION(pVeh, false);
	}
}

void Object_spawner_logic_loop() //called in World_Options.c WorldLoopedOptions.
{
	if (!bitTest(Container.BitSet, PB_IsInObjectSpawner))
		return;

	if (DOES_OBJECT_EXIST(ctlMem->m_spawned_objects[selectedObjectIndex].o))
	{
		static float scriptedCamPitch;
		static float scriptedCamYaw;
		static float camDistance;
		if (!DOES_CAM_EXIST(scriptedCam))
		{
			uint selectedEntityModel;
			vector3 selectedEntityMinDims, selectedEntityMaxDims;
			CREATE_CAM(14, &scriptedCam);
			SET_CAM_ACTIVE(scriptedCam, true);
			SET_CAM_PROPAGATE(scriptedCam, true);
			ACTIVATE_SCRIPTED_CAMS(true, true);
			scriptedCamPitch = 0.0f;
			scriptedCamYaw = 0.0f;
			GET_OBJECT_MODEL(ctlMem->m_spawned_objects[selectedObjectIndex].o, &selectedEntityModel);
			GET_MODEL_DIMENSIONS(selectedEntityModel, &selectedEntityMinDims, &selectedEntityMaxDims);
			camDistance = 2.0 * (selectedEntityMaxDims.y - selectedEntityMinDims.y);
		}
		SET_CHAR_INVINCIBLE(GetPlayerPed(), true); //God Mode
		SET_CHAR_VISIBLE(GetPlayerPed(), false);
		FREEZE_CHAR_POSITION(GetPlayerPed(), true);
		SET_CHAR_COLLISION(GetPlayerPed(), false);
		//SET_PLAYER_CONTROL(GET_PLAYER_ID(), false);
		SET_POLICE_IGNORE_PLAYER(GET_PLAYER_ID(), true);
		if (IS_CHAR_IN_ANY_CAR(GetPlayerPed()))
		{
			Vehicle pVeh = GetPlayerVeh();
			SET_CAR_VISIBLE(pVeh, false);
			FREEZE_CAR_POSITION(pVeh, true);
			SET_CAR_COLLISION(pVeh, false);
		}
		
		FREEZE_OBJECT_POSITION(ctlMem->m_spawned_objects[selectedObjectIndex].o, true);
		ATTACH_CAM_TO_OBJECT(scriptedCam, ctlMem->m_spawned_objects[selectedObjectIndex].o);
		POINT_CAM_AT_OBJECT(scriptedCam, ctlMem->m_spawned_objects[selectedObjectIndex].o);

		//TODO NATH: Use this when highlighting the spawned object in the Spanwed objects submenu, then stop when user selects it / enters edit/spawner mode.
		int timer;
		GET_GAME_TIMER(&timer);

		const uint interval = 1500;
		uint t = (timer % interval);

		uint alpha = (t < interval / 2)
			? (100 + (t * 100 / (interval / 2)))
			: (200 - ((t - interval / 2) * 100 / (interval / 2)));

		SET_OBJECT_ALPHA(ctlMem->m_spawned_objects[selectedObjectIndex].o, alpha);

		int leftX, leftY, rightX, rightY;
		GET_POSITION_OF_ANALOGUE_STICKS(0, &leftX, &leftY, &rightX, &rightY);

		if (IS_BUTTON_PRESSED(0, BUTTON_BACK)) //Zoom in/out with stick right while holding back (awesome!)
		{
			float CamNewDist = (camDistance + (rightY / 100.0f)); //pre-calculate the distance before setting it so we know it's within bounds. - Nath
			if (CamNewDist > 1.0f)
				camDistance = CamNewDist;

		}
		else
		{
			scriptedCamYaw = scriptedCamYaw - (rightX * 0.025f);
			if (scriptedCamYaw > 360.0f)    //yaw
				scriptedCamYaw -= 360.0f;
			else if (scriptedCamYaw < 0.0f)
				scriptedCamYaw += 360.0f;


			if (IS_LOOK_INVERTED())            //pitch
				scriptedCamPitch = scriptedCamPitch + (rightY * 0.025f);
			else
				scriptedCamPitch = scriptedCamPitch - (rightY * 0.025f);
			if (scriptedCamPitch < -70.0)
				scriptedCamPitch = -70.0;
			if (scriptedCamPitch > 70.0)
				scriptedCamPitch = 70.0;
		}

		float camOffsetTrig, camOffsetX, camOffsetY, camOffsetZ;

		camOffsetTrig = (camDistance * COS(scriptedCamPitch - 18.0f));
		camOffsetX = camOffsetX + (camOffsetTrig * SIN(scriptedCamYaw));
		camOffsetY = camOffsetY - (camOffsetTrig * COS(scriptedCamYaw));
		camOffsetZ = camOffsetZ - (camDistance * SIN(scriptedCamPitch - 18.0f));

		SET_CAM_ATTACH_OFFSET(scriptedCam, camOffsetX, camOffsetY, camOffsetZ);

		if (IS_BUTTON_JUST_PRESSED(0, BUTTON_X))
		{
			bitFlip(&Container.BitSet, PB_IsMenuOpen);
		}

		bool isAttached = IS_OBJECT_ATTACHED(ctlMem->m_spawned_objects[selectedObjectIndex].o);
		if (!bitTest(Container.BitSet, PB_IsMenuOpen))
		{
			int LT, RT;
			if (USING_STANDARD_CONTROLS())
			{
				LT = GET_CONTROL_VALUE(0, 6);
				RT = GET_CONTROL_VALUE(0, 4);
			}
			else
			{
				LT = GET_CONTROL_VALUE(0, 4);
				RT = GET_CONTROL_VALUE(0, 6);
			}

			//int LEFT_STICK = GET_CONTROL_VALUE(0, I_DONT_KNOW);
			if (IS_BUTTON_PRESSED(0, BUTTON_LB))
			{
				rotateObject(ctlMem->m_spawned_objects[selectedObjectIndex].o, Vector3(0, 0, 1), -(float)selectedRotationSpeed / 100.0f);
			}
			else if (IS_BUTTON_PRESSED(0, BUTTON_RB))
			{
				rotateObject(ctlMem->m_spawned_objects[selectedObjectIndex].o, Vector3(0, 0, 1), (float)selectedRotationSpeed / 100.0f);
			}


			if (IS_BUTTON_PRESSED(0, DPAD_UP))
			{
				rotateObject(ctlMem->m_spawned_objects[selectedObjectIndex].o, Vector3(1, 0, 0), -(float)selectedRotationSpeed / 100.0f);
			}
			else if (IS_BUTTON_PRESSED(0, DPAD_DOWN))
			{
				rotateObject(ctlMem->m_spawned_objects[selectedObjectIndex].o, Vector3(1, 0, 0), (float)selectedRotationSpeed / 100.0f);
			}

			if (LT > 3 || RT > 3) {
				if (!isAttached) {
					vector3 pos;
					GET_OBJECT_COORDINATES(ctlMem->m_spawned_objects[selectedObjectIndex].o, &pos.x, &pos.y, &pos.z);
					pos.z = pos.z - ((LT / 255.0f) * selectedMovementSpeed / 100.0f) + ((RT / 255.0f) * selectedMovementSpeed / 100.0f);
					printf("LT == %i, RT == %i, selectedMovementSpeed == %i", LT, RT, selectedMovementSpeed);
					//pos.z = pos.z - (LT / 1) + (RT / 1);
					SET_OBJECT_COORDINATES(ctlMem->m_spawned_objects[selectedObjectIndex].o, pos.x, pos.y, pos.z);
				}
				else {
					//get attach offset
					//manipulate offset
					//set offset
				}
			}


			if (ABSI(leftX) > 3 || ABSI(leftY) > 3)
			{
				if (!isAttached)
				{
					vector3 pos;
					GET_OBJECT_COORDINATES(ctlMem->m_spawned_objects[selectedObjectIndex].o, &pos.x, &pos.y, &pos.z);

					float moveTrig = (leftY * 0.0133f) * COS(scriptedCamPitch);
					float moveSpeedMultiplier = selectedMovementSpeed / 100.0f;
					pos.x = pos.x + (0.75 * moveSpeedMultiplier * ((moveTrig * SIN(scriptedCamYaw)) + (leftX * 0.0133f) * COS(scriptedCamYaw)));
					pos.y = pos.y - (0.75 * moveSpeedMultiplier * ((moveTrig * COS(scriptedCamYaw)) - (leftX * 0.0133f) * SIN(scriptedCamYaw)));

					SET_OBJECT_COORDINATES(ctlMem->m_spawned_objects[selectedObjectIndex].o, pos.x, pos.y, pos.z);
				}
				else
				{
					//get attach offset
					//manipulate offset
					//set offset
				}
			}

			if (IS_BUTTON_JUST_PRESSED(0, STICK_RIGHT))
			{
				SET_OBJECT_QUATERNION(ctlMem->m_spawned_objects[selectedObjectIndex].o, 0, 0, 0, 0);
				printf("~r~CTL: ~w~Object rotation reset.");
			}

			if (IS_BUTTON_JUST_PRESSED(0, BUTTON_A))  //Clone Object
			{
				if (getObjectSpawnCount(0, 0) == 70)
				{
					printf("you already reached the spawn limit");
					return;
				}
				OBJECT_INDEX clone = createObjectCopy(ctlMem->m_spawned_objects[selectedObjectIndex].o); //clone the object

				uint obj_array_index;
				if (addObjectToSpawnerList(clone, &obj_array_index)) //add the object to the list
				{
					strcpy(ctlMem->m_spawned_objects[obj_array_index].name, ctlMem->m_spawned_objects[selectedObjectIndex].name, 32); //copy original objects name.
					GET_GAME_TIMER((int*)&ctlMem->m_spawned_objects[obj_array_index].creationTime); //set creation time

					//ideally I want to have an activity feed somewhere eventually but for now...
					ADD_NEXT_MESSAGE_TO_PREVIOUS_BRIEFS(false);
					printf("~r~CTL: ~w~Copied ~y~%s.", ctlMem->m_spawned_objects[selectedObjectIndex].name);
					selectedObjectIndex = obj_array_index; //set new selected array index
				}
				return;
			}

			if (IS_BUTTON_JUST_PRESSED(0, BUTTON_Y))
			{
				DeleteObject(&ctlMem->m_spawned_objects[selectedObjectIndex].o);
				if (getObjectSpawnCount(0, 0) > 0)
				{
					selectedObjectIndex = GetNextObjectIndex(false); //this will get the previous based on creationTime
#ifdef Developer
					printf("GetNextObjectIndex() returns %i", selectedObjectIndex);
#endif
				}
				else
					Exit_spawner_mode();
				return;
			}
			else if (IS_BUTTON_JUST_PRESSED(0, DPAD_RIGHT))
			{
				selectedObjectIndex = GetNextObjectIndex(true);
#ifdef Developer
				printf("GetNextObjectIndex() returns %i", selectedObjectIndex);
#endif
				return;
			}
			else if (IS_BUTTON_JUST_PRESSED(0, DPAD_LEFT))
			{
				selectedObjectIndex = GetNextObjectIndex(false);
#ifdef Developer
				printf("GetNextObjectIndex() returns %i", selectedObjectIndex);
#endif
				return;
			}
			else if (IS_BUTTON_JUST_PRESSED(0, STICK_LEFT))
			{
				if (isAttached) {
					//unattach
				}
				else //attach
				{

				}
			}

		}
		if (IS_BUTTON_JUST_PRESSED(0, BUTTON_B))  //need to also run this if y is pressed and no more objects
		{
			Exit_spawner_mode();

			//Set these below once the user is finished messing with the object.
		//	SET_OBJECT_VISIBLE(obj_to_add, !bitTest(ctlMem->m_bit_sets.m_menuPreferences, PREF_OBJ_SPAWNER_INVISIBLE));
			SET_OBJECT_DYNAMIC(ctlMem->m_spawned_objects[selectedObjectIndex].o, bitTest(ctlMem->m_bit_sets.m_menuPreferences, PREF_OBJ_SPAWNER_DYNAMIC));
			//SET_OBJECT_PICK(obj_to_add, bitTest(ctlMem->m_bit_sets.m_menuPreferences, PREF_OBJ_SPAWNER_COLLISION))
			//PICKUP
			FREEZE_OBJECT_POSITION(ctlMem->m_spawned_objects[selectedObjectIndex].o, bitTest(ctlMem->m_bit_sets.m_menuPreferences, PREF_OBJ_SPAWNER_FROZEN));
			return;
		}
	}
}