#define M_PI 3.14159265358979323846

/*
struct Player {
	char unknown1[4]; //Skip 4 bytes since we dont know what this is most likely useless or for me unusable informations
	float x; //0x4
	float y; //0x8
	float z; //0xC
	char unknown2[0x30]; //Skip hex: 0x30 must convert from hex to dec to know the byte value
						 //(0x30 will be added to the past bytes in this case: 0x30 + 0x4 + 0x4 + 0x4 + 0x4 = 0x30 + 0x4 * 4 = 0x40)
						 //as you can see 0x40 is our yaw value that we can save in a float (since a float has 4 bytes) after we added the float
						 //we also added 4 bytes thats where our pitch is located but we have to save the pitch which we will by simply
						 //adding another float
	float yaw; //0x40
	float pitch; //0x44
	char unknown3[0x2E4];
	int team; //0x32C
	char unknown4[0x8];
	int IsAlive; //0x338
	// well lemme explain for my future journey:
	//the IsAlive value is located in the localPlayer pointer as the offset 0x338
	//I can now subtract this value with the last value I had in the list
	//which in this case is (or was when ive added something in the future)
	//pith with the offset 0x44 ok nice the result is: 0x338 - 0x44 = 2F4
	//??? but why do I skip 0X2F0 (unknown4) then when the value is 2F4???
	//well this is because it actually IS the IsAlive value that means
	//that we would have to skip 0x2F0 bytes after adding the "int" value
	//the bytes will be: 0x2F0 + 4 = 0x2F4 which would result as our IsAlive Value
};
*/
struct Player {
	char unknown1[4];
	float x; //0x4
	float y; //0x8
	float z; //0xC
	char unknown2[0x30];
	float yaw; //0x40
	float pitch; //0x44
	char unknown3[0x2E4];
	int team; //0x32C
	char unknown4[0x8];
	int IsAlive; //0x338
};

HMODULE hmBase = GetModuleHandle("ac_client.exe");

DWORD offEntityList = (DWORD)(hmBase)+0x10F4F8;

DWORD offPlayerEntity = DWORD(hmBase) + 0x10F4F4;
Player* lPlr = (Player*)*(DWORD*)offPlayerEntity;



float euclidean_distance(float x, float y) {
	return sqrtf((x * x) + (y * y));
}
void Aimbot()
{
	int* current_players = (int*)(0x50F500);

	float closest_player = -1.0f;
	float closest_yaw = 0.0f;
	float closest_pitch = 0.0f;

	// Iterate through all active enemies
	for (int i = 0; i < *current_players; i++) {
		DWORD* enemy_list = (DWORD*)(0x50F4F8);
		DWORD* enemy_offset = (DWORD*)(*enemy_list + (i * 4));
		Player* enemy = (Player*)(*enemy_offset);

		// Make sure the enemy is valid and alive
		if (lPlr != NULL && enemy != NULL and enemy->IsAlive == 0) { //(lPlr != NULL && lPlr->team != enemy->team && enemy != NULL and enemy->IsAlive == 0) {

			// Calculate the absolute position of the enemy away from us to ensure that our future calculations are correct and based
			// around the origin
			float abspos_x = enemy->x - lPlr->x;
			float abspos_y = enemy->y - lPlr->y;
			float abspos_z = enemy->z - lPlr->z;

			// Calculate our distance from the enemy
			float temp_distance = euclidean_distance(abspos_x, abspos_y);
			// If this is the closest enemy so far, calculate the yaw and pitch to aim at them
			if (closest_player == -1.0f || temp_distance < closest_player) {
				closest_player = temp_distance;

				// Calculate the yaw
				float azimuth_xy = atan2f(abspos_y, abspos_x);
				// Convert to degrees
				float yaw = (float)(azimuth_xy * (180.0 / M_PI));
				// Add 90 since the game assumes direct north is 90 degrees
				closest_yaw = yaw + 90;

				// Calculate the pitch
				// Since Z values are so limited, pick the larger between x and y to ensure that I 
				// don't look straight at the air when close to an enemy
				if (abspos_y < 0) {
					abspos_y *= -1;
				}
				if (abspos_y < 5) {
					if (abspos_x < 0) {
						abspos_x *= -1;
					}
					abspos_y = abspos_x;
				}
				float azimuth_z = atan2f(abspos_z, abspos_y);
				// Covert the value to degrees
				closest_pitch = (float)(azimuth_z * (180.0 / M_PI));
			}
		}
	}

	lPlr->yaw = closest_yaw;
	lPlr->pitch = closest_pitch;
}





/*
void Aimbot(PlayerEntity* enemy)
{

	//if (enemy == NULL) {return;} if (localPlayer == NULL) {return;} // returns if enemy or locaplayer is NULL
	float abspos_x = enemy->HeadPosition.x - localPlayer->HeadPosition.x;
	float abspos_y = enemy->HeadPosition.y - localPlayer->HeadPosition.y;
	float abspos_z = enemy->HeadPosition.z - localPlayer->HeadPosition.z;

	float azimuth_xy = atan2f(abspos_y, abspos_x);
	float yaw_X = (float)(azimuth_xy * (180.0 / M_PI));

	float azimuth_yz = atan2f(abspos_z, abspos_y);
	float yaw_Y = (float)(azimuth_yz * (180.0 / M_PI));

	float azimuth_zx = atan2f(abspos_x, abspos_z);
	float yaw_Z = (float)(azimuth_zx * (180.0 / M_PI));

	//set values
	//localPlayer->Angles.x = abspos_x;
	//localPlayer->Angles.y = abspos_y;
	//localPlayer->Angles.z = abspos_z;

	//set values
	localPlayer->Angles.x = yaw_X;
	localPlayer->Angles.y = yaw_Y;
	localPlayer->Angles.z = yaw_Z;
}
*/

/*
void Aimbot(PlayerEntity* enemy)
{

	//if (enemy == NULL) {return;} if (localPlayer == NULL) {return;} // returns if enemy or locaplayer is NULL

	//calculate yaw
	float abspos_x = enemy->HeadPosition.x - localPlayer->HeadPosition.x;
	float abspos_y = enemy->HeadPosition.y - localPlayer->HeadPosition.y;
	float azimuth_xy = atan2f(abspos_y, abspos_x);
	float yaw_X = (float)(azimuth_xy * (180.0 / M_PI));
	//


	//calculate pich
	float abspos_z = enemy->HeadPosition.z - localPlayer->HeadPosition.z;
	 If you inject this code, it will appear to initially work. However,
	 when you get within arm's distance of an enemy, your player will suddenly
	 look straight up or straight down. This is due to the game having very
	 limited Z values. For example, most maps in the game have Z values between
	 0 and 6. When the value of Y gets too small, the resulting equation ends up
	 being skewed. Imagine the case where the difference in Z values was 3 but the
	 Y value difference was 1, or arctan(3 / 1). This resolves to 75°, which is
	 effectively straight up in the air when it comes to pitch.

	To account for this behavior, we will look at the value of Y and ensure that it is reasonably large.
	If it’s not, we will use X. This is not perfect, but it will help alleviate some of the issues.
	We will also ensure that the value is positive, regardless:
if (abspos_y < 0) {
	abspos_y *= -1;
}
if (abspos_y < 5) {
	if (abspos_x < 0) {
		abspos_x *= -1;
	}
	abspos_y = abspos_x;
}
float azimuth_z = atan2f(abspos_z, abspos_y);
float yaw_Y = (float)(azimuth_xy * (180.0 / M_PI));
//


//set values
//localPlayer->Angles.x = yaw_X + 90; // change x value
localPlayer->Angles.y = yaw_Y / 2; // change y value
localPlayer->Angles.z = yaw_Y / 2; // change y value
//localPlayer->Angles.y = yaw_X / yaw_Y; // here I calculate the Z value because Im using vectors instead of "Angels" (Angels in vectors/degrees)
}
*/