#include "ObjectSpawnerCore.h"
#include "ObjectSpawnerMenu.h"
#include "ObjectSpawnerGlobals.h"
#include "ObjectCatalog\GameCategories.h"
#include "../../World_Options.h"

/*void listFilesInDirTest(int un1, int un2)
{
	SetHeader("Ye test");
	int ini_count;
	if (CTL_GET_FILES_IN_DIRECTORY("GAME:\\Cartel\\object_maps", ".ini", &ini_count))
	{
		printf("CTL_GET_FILES_IN_DIRECTORY.. ini_count == %i", ini_count);
		for (int i = 0; i < ini_count; i++)
		{
			char* entry;
			if (CTL_GET_FILES_IN_DIRECTORY_ENTRY(i, &entry))
			{
				AddItem(entry, nullptr, "Ye entry");
			}
		}
	}
	else
	{
		printf("Failed to get files in dir");
		AddItem("Failed", nullptr, "Failed");
	}
}*/

void loadHighlightedLocalMap(int entry_index, int un2)
{
    char* entry_string_pointer;
    char* path = BUFFER64;
    CTL_GET_FILES_IN_DIRECTORY_ENTRY(entry_index, &entry_string_pointer);
    strcpy(path, "object_maps\\", 128); //compiler bitches about small sprintf so we will just do this instead
    stradd(path, entry_string_pointer, 128);
    loadMapFileGivenPath(path);
}

void uploadMyLocalMapFile(int entry_index, int uin2)
{
    char* entry_string_pointer;
    char* path = BUFFER128;
    CTL_GET_FILES_IN_DIRECTORY_ENTRY(entry_index, &entry_string_pointer);
    sprintf(path, "GAME:\\Cartel\\object_maps\\%s", entry_string_pointer);
    if (CTL_UPLOAD_MAP_INI(path))
    {
        return;
    }
}

void deleteLocalMapFile(int entry_index, int un2)
{
    char* entry_string_pointer;
    char* path = BUFFER128;
    CTL_GET_FILES_IN_DIRECTORY_ENTRY(entry_index, &entry_string_pointer);
    sprintf(path, "GAME:\\Cartel\\object_maps\\%s", entry_string_pointer);
    if (CTL_DELETE_FILE(path))
        printf("~r~CTL: ~w~Deleted %s", path);
    else
        printf("~r~CTL: ~w~Failed to delete file.");
}

void listMyLocalMapFiles(int un1, int un2)
{
    SetHeader("My object ini files");
    int ini_count;
    if (CTL_GET_FILES_IN_DIRECTORY(false, "GAME:\\Cartel\\object_maps", ".ini", &ini_count))
    {
        for (int i = 0; i < ini_count; i++)
        {
            char* entry;
            if (CTL_GET_FILES_IN_DIRECTORY_ENTRY(i, &entry))
                AddItemWithParamAlt(entry, i, loadHighlightedLocalMap, "~PAD_A~ = Spawn, ~PAD_X~ = Upload to Cartel Server, ~PAD_Y~ = Delete", uploadMyLocalMapFile, deleteLocalMapFile);
        }
    }
    else
    {
        printf("Failed to get files in dir");
        AddItem("Failed", nullptr, "Do you have any maps on your hdd?");
    }
}

void load_downloaded_map(int entry_index, int un2)
{
    char* entry_string_pointer;
    char* path = BUFFER128;
    CTL_GET_FILES_IN_DIRECTORY_ENTRY(entry_index, &entry_string_pointer);
    sprintf(path, "object_maps\\downloaded\\%s\\%s", Container.Ui.HeaderText[Container.CurrentMenuLevel], entry_string_pointer);
    loadMapFileGivenPath(path);
}

void delete_downloaded_map(int entry_index, int un2)
{
    char* entry_string_pointer;
    char* path = BUFFER128;
    CTL_GET_FILES_IN_DIRECTORY_ENTRY(entry_index, &entry_string_pointer);
    sprintf(path, "GAME:\\Cartel\\object_maps\\downloaded\\%s\\%s", Container.Ui.HeaderText[Container.CurrentMenuLevel], entry_string_pointer);
    if (CTL_DELETE_FILE(path))
        printf("~r~CTL: ~w~Deleted %s", path);
    else
        printf("~r~CTL: ~w~Failed to delete file.");
}

char* user_we_are_checking; //allocated a BUFFER16 inside listMyDownloadedMapFolders
void listDownloadedMapsByUser(int user_entry_index, int un2) //issue here is purely with us losing user_we_are_checking for some reason.
{
    if(user_we_are_checking[0] == '\0') //lets store the users tag to check before it gets overwritten.
    {
        char* tmp;
        if (CTL_GET_FILES_IN_DIRECTORY_ENTRY(user_entry_index, &tmp)) //this just returns an address that get overwritten sharpish below so lets change it
            strcpy(user_we_are_checking, tmp, 16);
    }
  
    SetHeader(user_we_are_checking); 
    int ini_count;
    char* usersPath = BUFFER128;
    sprintf(usersPath, "GAME:\\Cartel\\object_maps\\downloaded\\%s", user_we_are_checking); //this is passing to native correctly //CTL_GET_FILES_IN_DIRECTORY: path == GAME:\Cartel\object_maps\downloaded\TBPING
    if (CTL_GET_FILES_IN_DIRECTORY(false, usersPath, ".ini", &ini_count))
    {
        for (int i = 0; i < ini_count + 1; i++)
        {
            char* entry_string_pointer;
            if (CTL_GET_FILES_IN_DIRECTORY_ENTRY(i, &entry_string_pointer))
                AddItemWithParamAlt(entry_string_pointer, i, load_downloaded_map, "~PAD_A~ = Spawn, ~PAD_Y~ = Delete", nullptr, delete_downloaded_map);
                //AddItemWithParam(entry_string_pointer, i, load_downloaded_map, "~PAD_A~ = Spawn");
        }
    }
    else
        AddItem("Failed", nullptr, "Do you have any maps on your hdd?");
}

void listMyDownloadedMapFolders(int un1, int un2) 
{
    SetHeader("Downloaded ini maps");

    user_we_are_checking = BUFFER16; //that's right, point riiiight here in codespace //could move this to ctlMem eventually..
    user_we_are_checking[0] = '\0'; //yes this is required, i know how fucked up this looks
    int folder_count;
    if (CTL_GET_FILES_IN_DIRECTORY(true, "GAME:\\Cartel\\object_maps\\downloaded", "", &folder_count))
    {
        for (int i = 0; i < folder_count; i++)
        {
            char* entry_string_pointer;
            if (CTL_GET_FILES_IN_DIRECTORY_ENTRY(i, &entry_string_pointer))
                AddItemMenuWithParam(entry_string_pointer, i, listDownloadedMapsByUser, "List downloaded maps by this user."); //lets pass the index so we can strcpy this entry for use in the other function before it gets overwritten
        }
    }
    else
    {
        printf("Failed to get files in dir");
        AddItem("Failed", nullptr, "Do you have any maps on your hdd?");
    }
}

void ObjectSpawnerMainPage(int unk1, int unk2)
{
    SetHeader("Object Spawner by xStormsnoutx  ~BLIP_71~");
    bitReset(&ctlMem->m_bit_sets.m_debug, DEBUG_IN_SPAWNER_EDIT_SUBMENUS);
    int objCount = GetSpawnedObjectCount();
    if (selectedItemsToDelete > objCount)
        selectedItemsToDelete = objCount;
    else
        selectedItemsToDelete = 1;

    AddItemMenu("Spawn object", ObjectSpawnerSpawnPage, nullptr);
    if (objCount > 0) 
    {
        AddItemMenu("Edit Objects", ObjectSpawnerEditObjectsPage, nullptr);
        AddItemInt("Delete last Objects", false, 1, objCount, &selectedItemsToDelete, 1, Action_DeleteLastSpawnedObjects, nullptr);
        AddItemAlt("Save Objects to ini", saveMapFile, "~BLIP_24~ Nathan, ~PAD_X~ = Store players location for teleport.", saveMapFileTP, nullptr);
        //AddItem("Save Objects to ini", nullptr, "~BLIP_24~ Nathan");
    }
    else
    {
        AddItem("Edit Objects", nullptr, "Come back when you have spawned Objects");
        AddItem("Delete last Objects", nullptr, "Come back when you have spawned Objects");
        AddItem("Save Objects to ini", nullptr, "Come back when you have spawned Objects");
    } 
    AddItemMenu("Spawner Configuration", ObjectSpawnerSettingsPage, nullptr);
    AddItemMenu("ini file list", listMyLocalMapFiles, "You got yourself a Dank Function.");
    AddItemMenu("Downloaded ini file list", listMyDownloadedMapFolders, "You got yourself a Dank Function.");
    AddItem("Load Objects from ini (keyboard)", loadLocalMapFile, "~BLIP_24~ Nathan");
}

void ObjectSpawnerSpawnPage(int unk1, int unk2)
{
    SetHeader("Spawn Object");

   // AddItemMenu("Spawn object (Categories)", showAllObjectSuperCategories, nullptr);
    //AddItemMenu("Spawn object (Ordered by letter)", spawnObjectOrderedByLetterOptions, "Popular objects.");
    AddItemMenu("Cartel Selections", Menu_CartelSelections, nullptr);
    AddItemMenu("Game Categories", GameCategories_Root, nullptr);
    AddItem("Spawn object (Name or decimal)", SpawnObjectFromNameOrDecimal, "Input model e.g. \"BM LADDER\", \"1376367391\".");
    AddItem("Spawn object (Hex)", SpawnObjectFromHex, "Input model hash in hex form, e.g. 0x5209B31F.");
}

void ObjectSpawnerConfigPage(void* index_ptr, int unk2) {
    int index = (int)index_ptr;
    SetHeader("Object Config");
    AddItemWithParam("Set FROZEN", index, Action_SetSelectedObjectFrozen, nullptr);
    AddItemWithParam("Set DYNAMIC", index, Action_SetSelectedObjectDynamic, nullptr);
    AddItemWithParam("Set COLLISION", index, Action_SetSelectedObjectCollision, "Only works when object naturally has collision");
    AddItemWithParam("Set VISIBLE", index, Action_SetSelectedObjectVisible, nullptr);
    AddItemWithParam("Set INVINCIBLE", index, Action_SetSelectedObjectInvincible, nullptr);
    AddItemWithParam("Set PICKUPABLE", index, Action_SetSelectedObjectPickupable, nullptr);
    AddItemWithParam("Give to Player", index, Action_GiveSelectedObjectToPlayer, nullptr);
    AddItemWithParam("Set NON FROZEN", index, Action_SetSelectedObjectNonFrozen, nullptr);
    AddItemWithParam("Set NON DYNAMIC", index, Action_SetSelectedObjectNonDynamic, nullptr);
    AddItemWithParam("Set NON COLLISION", index, Action_SetSelectedObjectNonCollision, nullptr);
    AddItemWithParam("Set NON VISIBLE", index, Action_SetSelectedObjectNonVisible, nullptr);
    AddItemWithParam("Set NON INVINCIBLE", index, Action_SetSelectedObjectNonInvincible, nullptr);
}

void ObjectSpawnerSettingsPage(int unk1, int unk2) {
	SetHeader("Spawner Configuration");

	AddItemInt("Movement Speed in %", false, 1, 400, &selectedMovementSpeed, 1, nullptr, nullptr);
	AddItemInt("Rotation Speed in %", false, 1, 400, &selectedRotationSpeed, 1, nullptr, nullptr);
	//AddItemBool("Speed related to dimensions", &ctlMem->m_bit_sets.m_obj_spawner, PREF_OBJ_SPAWNER_SPEED_IS_DIMENSIONS, nullptr, nullptr);
    //AddItemBool("Sphere around edit object", &ctlMem->m_bit_sets.m_obj_spawner, PREF_OBJ_SPAWNER_SPHERE_AROUND_EDIT_OBJECT, nullptr, nullptr);
    //AddItemBool("Draw origin for edit object", &ctlMem->m_bit_sets.m_obj_spawner, PREF_OBJ_SPAWNER_DRAW_ORIGIN_FOR_EDIT_OBJECT, nullptr, nullptr);
	AddItemBool("Spawn default: INVISIBLE", &ctlMem->m_bit_sets.m_obj_spawner, PREF_OBJ_SPAWNER_INVISIBLE, nullptr, nullptr);
	AddItemBool("Spawn default: INVINCIBLE", &ctlMem->m_bit_sets.m_obj_spawner, PREF_OBJ_SPAWNER_INVINCIBLE, nullptr, nullptr);
	AddItemBool("Spawn default: DYNAMIC", &ctlMem->m_bit_sets.m_obj_spawner, PREF_OBJ_SPAWNER_DYNAMIC, nullptr, nullptr);
	AddItemBool("Spawn default: COLLISSION", &ctlMem->m_bit_sets.m_obj_spawner, PREF_OBJ_SPAWNER_COLLISION, nullptr, nullptr);
	AddItemBool("Spawn default: PICKUPABLE", &ctlMem->m_bit_sets.m_obj_spawner, PREF_OBJ_SPAWNER_PICKUP_ABLE, nullptr, nullptr);
	AddItemBool("Spawn default: FROZEN", &ctlMem->m_bit_sets.m_obj_spawner, PREF_OBJ_SPAWNER_FROZEN, nullptr, nullptr);
}


void ObjectSpawnerPlacementToolPage(void* index_ptr, int unk2)
{
    int index = (int)index_ptr;
    static char* alignments[6] = { "Front", "Back", "Left", "Right", "Top", "Bottom" };
    SetHeader("Placement Tool");
    AddItemWithParamAlt("- Spawn -", index, Action_PlacementTool, "~PAD_A~ to spawn | ~PAD_X~ for 5 second preview", Action_PlacementToolPreview, nullptr);
    AddItemInt("Copies to create", false, 1, MAX_SPAWNER_OBJECTS, &placementToolConfig.copies, 1, nullptr, nullptr);
    AddItemFloatAlt("Spacing", false, 0.0f, 1000.0f, &placementToolConfig.spacing, 0.01f, 3, Reset_spacing, "1.0 = 100% of object size | ~PAD_A~ = Reset,  ~PAD_X~ + 1,   ~PAD_Y~ - 1", Decrease_spacing, Increase_spacing);
    AddItemChar("Alignment", false, alignments, 6, &placementToolConfig.alignment, nullptr, "W.I.P.");
    AddItemFloatAlt("Pitch", false, -180.0f, 180.0f, &placementToolConfig.pitch, 0.01f, 3, Reset_pitch, "Pitch in degree | ~PAD_A~ = Reset,  ~PAD_X~ + 1,   ~PAD_Y~ - 1", Decrease_pitch, Increase_pitch);
    AddItemFloatAlt("Roll", false, -180.0f, 180.0f, &placementToolConfig.roll, 0.01f, 3, Reset_roll, "Roll in degree | ~PAD_A~ = Reset,  ~PAD_X~ + 1,   ~PAD_Y~ - 1", Decrease_roll, Increase_roll);
    AddItemFloatAlt("Yaw", false, -180.0f, 180.0f, &placementToolConfig.yaw, 0.01f, 3, Reset_yaw, "Yaw in degree | ~PAD_A~ = Reset,  ~PAD_X~ + 1,   ~PAD_Y~ - 1", Decrease_yaw, Increase_yaw);
    AddItemFloatAlt("Offset X", false, -1000.0f, 1000.0f, &placementToolConfig.offsetX, 0.01f, 3, Reset_offsetX, "1.0 = 100% of object size | ~PAD_A~ = Reset,  ~PAD_X~ + 1,   ~PAD_Y~ - 1", Decrease_offsetX, Increase_offsetX);
    AddItemFloatAlt("Offset Y", false, -1000.0f, 1000.0f, &placementToolConfig.offsetY, 0.01f, 3, Reset_offsetY, "1.0 = 100% of object size | ~PAD_A~ = Reset,  ~PAD_X~ + 1,   ~PAD_Y~ - 1", Decrease_offsetY, Increase_offsetY);
    AddItemFloatAlt("Offset Z", false, -1000.0f, 1000.0f, &placementToolConfig.offsetZ, 0.01f, 3, Reset_offsetZ, "1.0 = 100% of object size | ~PAD_A~ = Reset,  ~PAD_X~ + 1,   ~PAD_Y~ - 1", Decrease_offsetZ, Increase_offsetZ);
}

void ObjectSpawnerEditObjectsPage(int unk1, int unk2) 
{
    SetHeader("Spawned objects");
    bitSet(&ctlMem->m_bit_sets.m_debug, DEBUG_IN_SPAWNER_EDIT_SUBMENUS);
    int count = 0;
    SpawnerObject* objectsArray = GetSpawnerObjectsArray(); 
    //int sortedIndices[MAX_SPAWNER_OBJECTS];  //STACK OVERFLOW with 300
    short sortedIndices[300]; 
    CreateSortedSpawnerObjectView(objectsArray, sortedIndices, &count); 
   // printf("Count == %i", count);
    if (count == 0) {
        AddItem("~r~No objects spawned", nullptr, nullptr);
        return;
    }


    for (int i = count - 1; i >= 0; i--) // reversed
    {
        if (DOES_OBJECT_EXIST(spawnerMem[sortedIndices[i]].o))
        {
        	AddItemMenusWithParamAlt(spawnerMem[sortedIndices[i]].name, (uint)sortedIndices[i], Action_EditSelectedObject, "~PAD_A~  Move Object  ~PAD_X~  Placement Tool  ~PAD_Y~  Config", ObjectSpawnerPlacementToolPage, ObjectSpawnerConfigPage);
		}
        else
        {
            AddItem("~r~Item does no more exist", nullptr, nullptr);
        }
    }
    
}