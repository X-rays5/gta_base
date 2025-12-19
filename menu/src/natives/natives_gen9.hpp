#pragma once

#pragma warning(push)
#pragma warning(disable: 4100)

// Auto-generated file - DO NOT EDIT
// Generated from GTA5 NativeDB data

#include "../rage/native_types.hpp"
#include "invoker.hpp"

namespace base::menu::natives {

	namespace BUILTIN {

		/**
		 * Pauses execution of the current script, please note this behavior is only seen when called from one of the game script files(ysc). In order to wait an asi script use "static void WAIT(DWORD time);" found in main.h
		 */
		void WAIT(int ms);
		/**
		 * Examples:
		 * g_384A = SYSTEM::START_NEW_SCRIPT("cellphone_flashhand", 1424);
		 * l_10D = SYSTEM::START_NEW_SCRIPT("taxiService", 1828);
		 * SYSTEM::START_NEW_SCRIPT("AM_MP_YACHT", 5000);
		 * SYSTEM::START_NEW_SCRIPT("emergencycall", 512);
		 * SYSTEM::START_NEW_SCRIPT("emergencycall", 512);
		 * SYSTEM::START_NEW_SCRIPT("FM_maintain_cloud_header_data", 1424);
		 * SYSTEM::START_NEW_SCRIPT("FM_Mission_Controller", 31000);
		 * SYSTEM::START_NEW_SCRIPT("tennis_family", 3650);
		 * SYSTEM::START_NEW_SCRIPT("Celebrations", 3650);
		 * 
		 * Decompiled examples of usage when starting a script:
		 * 
		 * SCRIPT::REQUEST_SCRIPT(a_0);
		 * if (SCRIPT::HAS_SCRIPT_LOADED(a_0)) {
		 * SYSTEM::START_NEW_SCRIPT(a_0, v_3);
		 * SCRIPT::SET_SCRIPT_AS_NO_LONGER_NEEDED(a_0);
		 * return 1;
		 * }
		 * 
		 * or:
		 * 
		 * v_2 = "MrsPhilips2";
		 * SCRIPT::REQUEST_SCRIPT(v_2);
		 * while (!SCRIPT::HAS_SCRIPT_LOADED(v_2)) {
		 * SCRIPT::REQUEST_SCRIPT(v_2);
		 * SYSTEM::WAIT(0);
		 * }
		 * sub_8792(36);
		 * SYSTEM::START_NEW_SCRIPT(v_2, 17000);
		 * SCRIPT::SET_SCRIPT_AS_NO_LONGER_NEEDED(v_2);
		 */
		int START_NEW_SCRIPT(const char* scriptName, int stackSize);
		/**
		 * return : script thread id, 0 if failed
		 * Pass pointer to struct of args in p1, size of struct goes into p2
		 */
		int START_NEW_SCRIPT_WITH_ARGS(const char* scriptName, Any* args, int argCount, int stackSize);
		int START_NEW_SCRIPT_WITH_NAME_HASH(Hash scriptHash, int stackSize);
		int START_NEW_SCRIPT_WITH_NAME_HASH_AND_ARGS(Hash scriptHash, Any* args, int argCount, int stackSize);
		/**
		 * Counts up. Every 1000 is 1 real-time second. Use SETTIMERA(int value) to set the timer (e.g.: SETTIMERA(0)).
		 */
		int TIMERA();
		int TIMERB();
		void SETTIMERA(int value);
		void SETTIMERB(int value);
		/**
		 * Gets the current frame time.
		 */
		float TIMESTEP();
		float SIN(float value);
		float COS(float value);
		float SQRT(float value);
		float POW(float base, float exponent);
		float LOG10(float value);
		/**
		 * Calculates the magnitude of a vector.
		 */
		float VMAG(float x, float y, float z);
		/**
		 * Calculates the magnitude of a vector but does not perform Sqrt operations. (Its way faster)
		 */
		float VMAG2(float x, float y, float z);
		/**
		 * Calculates distance between vectors.
		 */
		float VDIST(float x1, float y1, float z1, float x2, float y2, float z2);
		/**
		 * Calculates distance between vectors but does not perform Sqrt operations. (Its way faster)
		 */
		float VDIST2(float x1, float y1, float z1, float x2, float y2, float z2);
		int SHIFT_LEFT(int value, int bitShift);
		int SHIFT_RIGHT(int value, int bitShift);
		int FLOOR(float value);
		/**
		 * I'm guessing this rounds a float value up to the next whole number, and FLOOR rounds it down
		 */
		int CEIL(float value);
		int ROUND(float value);
		float TO_FLOAT(int value);
		/**
		 * THREAD_PRIO_HIGHEST = 0
		 * THREAD_PRIO_NORMAL = 1
		 * THREAD_PRIO_LOWEST = 2
		 * THREAD_PRIO_MANUAL_UPDATE = 100
		 */
		void SET_THIS_THREAD_PRIORITY(int priority);

	} // namespace BUILTIN

	namespace APP {

		BOOL APP_DATA_VALID();
		int APP_GET_INT(const char* property);
		float APP_GET_FLOAT(const char* property);
		const char* APP_GET_STRING(const char* property);
		void APP_SET_INT(const char* property, int value);
		void APP_SET_FLOAT(const char* property, float value);
		void APP_SET_STRING(const char* property, const char* value);
		/**
		 * Called in the gamescripts like:
		 * APP::APP_SET_APP("car");
		 * APP::APP_SET_APP("dog");
		 */
		void APP_SET_APP(const char* appName);
		void APP_SET_BLOCK(const char* blockName);
		void APP_CLEAR_BLOCK();
		void APP_CLOSE_APP();
		void APP_CLOSE_BLOCK();
		BOOL APP_HAS_LINKED_SOCIAL_CLUB_ACCOUNT();
		BOOL APP_HAS_SYNCED_DATA(const char* appName);
		void APP_SAVE_DATA();
		int APP_GET_DELETED_FILE_STATUS();
		BOOL APP_DELETE_APP_DATA(const char* appName);

	} // namespace APP

	namespace AUDIO {

		/**
		 * All found occurrences in b617d, sorted alphabetically and identical lines removed: https://pastebin.com/RFb4GTny
		 * 
		 * AUDIO::PLAY_PED_RINGTONE("Remote_Ring", PLAYER::PLAYER_PED_ID(), 1);
		 * AUDIO::PLAY_PED_RINGTONE("Dial_and_Remote_Ring", PLAYER::PLAYER_PED_ID(), 1);
		 * 
		 */
		void PLAY_PED_RINGTONE(const char* ringtoneName, Ped ped, BOOL p2);
		BOOL IS_PED_RINGTONE_PLAYING(Ped ped);
		void STOP_PED_RINGTONE(Ped ped);
		BOOL IS_MOBILE_PHONE_CALL_ONGOING();
		BOOL IS_MOBILE_INTERFERENCE_ACTIVE();
		int GET_CURRENT_TV_SHOW_PLAY_TIME();
		void CREATE_NEW_SCRIPTED_CONVERSATION();
		/**
		 * NOTE: ones that are -1, 0 - 35 are determined by a function where it gets a TextLabel from a global then runs,
		 * GET_CHARACTER_FROM_AUDIO_CONVERSATION_FILENAME and depending on what the result is it goes in check order of 0 - 9 then A - Z then z (lowercase). So it will then return 0 - 35 or -1 if it's 'z'. The func to handle that ^^ is func_67 in dialog_handler.c atleast in TU27 Xbox360 scripts.
		 * 
		 * p0 is -1, 0 - 35
		 * p1 is a char or string (whatever you wanna call it)
		 * p2 is Global 10597 + i * 6. 'i' is a while(i < 70) loop
		 * p3 is again -1, 0 - 35
		 * p4 is again -1, 0 - 35
		 * p5 is either 0 or 1 (bool ?)
		 * p6 is either 0 or 1 (The func to determine this is bool)
		 * p7 is either 0 or 1 (The func to determine this is bool)
		 * p8 is either 0 or 1 (The func to determine this is bool)
		 * p9 is 0 - 3 (Determined by func_60 in dialogue_handler.c)
		 * p10 is either 0 or 1 (The func to determine this is bool)
		 * p11 is either 0 or 1 (The func to determine this is bool)
		 * p12 is unknown as in TU27 X360 scripts it only goes to p11.
		 */
		void ADD_LINE_TO_CONVERSATION(int index, const char* p1, const char* p2, int p3, int p4, BOOL p5, BOOL p6, BOOL p7, BOOL p8, int p9, BOOL p10, BOOL p11, BOOL p12);
		/**
		 * 4 calls in the b617d scripts. The only one with p0 and p2 in clear text:
		 * 
		 * AUDIO::ADD_PED_TO_CONVERSATION(5, l_AF, "DINAPOLI");
		 * 
		 * =================================================
		 * One of the 2 calls in dialogue_handler.c p0 is in a while-loop, and so is determined to also possibly be 0 - 15.
		 */
		void ADD_PED_TO_CONVERSATION(int index, Ped ped, const char* p2);
		void SET_POSITION_FOR_NULL_CONV_PED(Any p0, float p1, float p2, float p3);
		void SET_ENTITY_FOR_NULL_CONV_PED(int p0, Entity entity);
		/**
		 * This native controls where the game plays audio from. By default the microphone is positioned on the player.
		 * When p0 is true the game will play audio from the 3 positions inputted.
		 * It is recommended to set all 3 positions to the same value as mixing different positions doesn't seem to work well.
		 * The scripts mostly use it with only one position such as in fbi3.c:
		 * AUDIO::SET_MICROPHONE_POSITION(true, ENTITY::GET_ENTITY_COORDS(iLocal_3091, true), ENTITY::GET_ENTITY_COORDS(iLocal_3091, true), ENTITY::GET_ENTITY_COORDS(iLocal_3091, true));
		 */
		void SET_MICROPHONE_POSITION(BOOL toggle, float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3);
		void SET_CONVERSATION_AUDIO_CONTROLLED_BY_ANIM(BOOL p0);
		void SET_CONVERSATION_AUDIO_PLACEHOLDER(BOOL p0);
		void START_SCRIPT_PHONE_CONVERSATION(BOOL p0, BOOL p1);
		void PRELOAD_SCRIPT_PHONE_CONVERSATION(BOOL p0, BOOL p1);
		void START_SCRIPT_CONVERSATION(BOOL p0, BOOL p1, BOOL p2, BOOL p3);
		void PRELOAD_SCRIPT_CONVERSATION(BOOL p0, BOOL p1, BOOL p2, BOOL p3);
		void START_PRELOADED_CONVERSATION();
		BOOL GET_IS_PRELOADED_CONVERSATION_READY();
		BOOL IS_SCRIPTED_CONVERSATION_ONGOING();
		BOOL IS_SCRIPTED_CONVERSATION_LOADED();
		int GET_CURRENT_SCRIPTED_CONVERSATION_LINE();
		void PAUSE_SCRIPTED_CONVERSATION(BOOL p0);
		void RESTART_SCRIPTED_CONVERSATION();
		int STOP_SCRIPTED_CONVERSATION(BOOL p0);
		void SKIP_TO_NEXT_SCRIPTED_CONVERSATION_LINE();
		/**
		 * Example from carsteal3.c: AUDIO::INTERRUPT_CONVERSATION(PLAYER::PLAYER_PED_ID(), "CST4_CFAA", "FRANKLIN");
		 * Voicelines can be found in GTAV\x64\audio\sfx in files starting with "SS_" which seems to mean scripted speech.
		 */
		void INTERRUPT_CONVERSATION(Ped ped, const char* voiceline, const char* speaker);
		/**
		 * One call found in the b617d scripts:
		 * 
		 * AUDIO::INTERRUPT_CONVERSATION_AND_PAUSE(NETWORK::NET_TO_PED(l_3989._f26F[0/*1* /]), "CONV_INTERRUPT_QUIT_IT", "LESTER");
		 */
		void INTERRUPT_CONVERSATION_AND_PAUSE(Ped ped, const char* p1, const char* speaker);
		int GET_VARIATION_CHOSEN_FOR_SCRIPTED_LINE(Any* p0);
		void SET_NO_DUCKING_FOR_CONVERSATION(BOOL p0);
		/**
		 * This native does absolutely nothing, just a nullsub
		 */
		void REGISTER_SCRIPT_WITH_AUDIO(int p0);
		/**
		 * This native does absolutely nothing, just a nullsub
		 */
		void UNREGISTER_SCRIPT_WITH_AUDIO();
		/**
		 * All occurrences and usages found in b617d: https://pastebin.com/NzZZ2Tmm
		 * Full list of mission audio bank names by DurtyFree https://github.com/DurtyFree/gta-v-data-dumps/blob/master/missionAudioBankNames.json
		 * p2 is always -1
		 */
		BOOL REQUEST_MISSION_AUDIO_BANK(const char* audioBank, BOOL p1, Any p2);
		/**
		 * All occurrences and usages found in b617d, sorted alphabetically and identical lines removed: https://pastebin.com/XZ1tmGEz
		 * Full list of ambient audio bank names by DurtyFree https://github.com/DurtyFree/gta-v-data-dumps/blob/master/ambientAudioBankNames.json
		 * p2 is always -1
		 */
		BOOL REQUEST_AMBIENT_AUDIO_BANK(const char* audioBank, BOOL p1, Any p2);
		/**
		 * All occurrences and usages found in b617d, sorted alphabetically and identical lines removed: https://pastebin.com/AkmDAVn6
		 * Full list of script audio bank names by DurtyFree https://github.com/DurtyFree/gta-v-data-dumps/blob/master/scriptAudioBankNames.json
		 * p2 is always -1
		 */
		BOOL REQUEST_SCRIPT_AUDIO_BANK(const char* audioBank, BOOL p1, Any p2);
		/**
		 * p2 is always -1
		 */
		BOOL HINT_MISSION_AUDIO_BANK(const char* audioBank, BOOL p1, Any p2);
		/**
		 * p2 is always -1
		 */
		BOOL HINT_AMBIENT_AUDIO_BANK(const char* audioBank, BOOL p1, Any p2);
		/**
		 * p2 is always -1
		 */
		BOOL HINT_SCRIPT_AUDIO_BANK(const char* audioBank, BOOL p1, Any p2);
		void RELEASE_MISSION_AUDIO_BANK();
		void RELEASE_AMBIENT_AUDIO_BANK();
		/**
		 * Full list of script audio bank names by DurtyFree https://github.com/DurtyFree/gta-v-data-dumps/blob/master/scriptAudioBankNames.json
		 */
		void RELEASE_NAMED_SCRIPT_AUDIO_BANK(const char* audioBank);
		void RELEASE_SCRIPT_AUDIO_BANK();
		void UNHINT_AMBIENT_AUDIO_BANK();
		void UNHINT_SCRIPT_AUDIO_BANK();
		void UNHINT_NAMED_SCRIPT_AUDIO_BANK(const char* audioBank);
		int GET_SOUND_ID();
		void RELEASE_SOUND_ID(int soundId);
		/**
		 * All found occurrences in b617d, sorted alphabetically and identical lines removed: https://pastebin.com/A8Ny8AHZ
		 * 
		 * Full list of audio / sound names by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/soundNames.json
		 */
		void PLAY_SOUND(int soundId, const char* audioName, const char* audioRef, BOOL p3, Any p4, BOOL p5);
		/**
		 * List: https://pastebin.com/DCeRiaLJ
		 * 
		 * All occurrences as of Cayo Perico Heist DLC (b2189), sorted alphabetically and identical lines removed: https://git.io/JtLxM
		 * 
		 * Full list of audio / sound names by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/soundNames.json
		 */
		void PLAY_SOUND_FRONTEND(int soundId, const char* audioName, const char* audioRef, BOOL p3);
		/**
		 * Only call found in the b617d scripts:
		 * 
		 * AUDIO::PLAY_DEFERRED_SOUND_FRONTEND("BACK", "HUD_FREEMODE_SOUNDSET");
		 * 
		 * Full list of audio / sound names by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/soundNames.json
		 */
		void PLAY_DEFERRED_SOUND_FRONTEND(const char* soundName, const char* soundsetName);
		/**
		 * All found occurrences in b617d, sorted alphabetically and identical lines removed: https://pastebin.com/f2A7vTj0
		 * No changes made in b678d.
		 * 
		 * gtaforums.com/topic/795622-audio-for-mods
		 * 
		 * Full list of audio / sound names by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/soundNames.json
		 */
		void PLAY_SOUND_FROM_ENTITY(int soundId, const char* audioName, Entity entity, const char* audioRef, BOOL isNetwork, Any p5);
		/**
		 * Only used with "formation_flying_blips_soundset" and "biker_formation_blips_soundset".
		 * p1 is always the model of p2
		 */
		void PLAY_SOUND_FROM_ENTITY_HASH(int soundId, Hash model, Entity entity, Hash soundSetHash, Any p4, Any p5);
		/**
		 * All found occurrences in b617d, sorted alphabetically and identical lines removed: https://pastebin.com/eeFc5DiW
		 * 
		 * gtaforums.com/topic/795622-audio-for-mods
		 * 
		 * Full list of audio / sound names by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/soundNames.json
		 */
		void PLAY_SOUND_FROM_COORD(int soundId, const char* audioName, float x, float y, float z, const char* audioRef, BOOL isNetwork, int range, BOOL p8);
		void UPDATE_SOUND_COORD(int soundId, float x, float y, float z);
		void STOP_SOUND(int soundId);
		/**
		 * Could this be used alongside either,
		 * SET_NETWORK_ID_EXISTS_ON_ALL_MACHINES or _SET_NETWORK_ID_SYNC_TO_PLAYER to make it so other players can hear the sound while online? It'd be a bit troll-fun to be able to play the Zancudo UFO creepy sounds globally.
		 */
		int GET_NETWORK_ID_FROM_SOUND_ID(int soundId);
		int GET_SOUND_ID_FROM_NETWORK_ID(int netId);
		void SET_VARIABLE_ON_SOUND(int soundId, const char* variable, float p2);
		/**
		 * From the scripts, p0:
		 * 
		 * "ArmWrestlingIntensity",
		 * "INOUT",
		 * "Monkey_Stream",
		 * "ZoomLevel"
		 */
		void SET_VARIABLE_ON_STREAM(const char* variable, float p1);
		void OVERRIDE_UNDERWATER_STREAM(const char* p0, BOOL p1);
		/**
		 * AUDIO::SET_VARIABLE_ON_UNDER_WATER_STREAM("inTunnel", 1.0);
		 * AUDIO::SET_VARIABLE_ON_UNDER_WATER_STREAM("inTunnel", 0.0);
		 */
		void SET_VARIABLE_ON_UNDER_WATER_STREAM(const char* variableName, float value);
		BOOL HAS_SOUND_FINISHED(int soundId);
		/**
		 * Plays ambient speech. See also _0x444180DB.
		 * 
		 * ped: The ped to play the ambient speech.
		 * speechName: Name of the speech to play, eg. "GENERIC_HI".
		 * speechParam: Can be one of the following:
		 * SPEECH_PARAMS_STANDARD
		 * SPEECH_PARAMS_ALLOW_REPEAT
		 * SPEECH_PARAMS_BEAT
		 * SPEECH_PARAMS_FORCE
		 * SPEECH_PARAMS_FORCE_FRONTEND
		 * SPEECH_PARAMS_FORCE_NO_REPEAT_FRONTEND
		 * SPEECH_PARAMS_FORCE_NORMAL
		 * SPEECH_PARAMS_FORCE_NORMAL_CLEAR
		 * SPEECH_PARAMS_FORCE_NORMAL_CRITICAL
		 * SPEECH_PARAMS_FORCE_SHOUTED
		 * SPEECH_PARAMS_FORCE_SHOUTED_CLEAR
		 * SPEECH_PARAMS_FORCE_SHOUTED_CRITICAL
		 * SPEECH_PARAMS_FORCE_PRELOAD_ONLY
		 * SPEECH_PARAMS_MEGAPHONE
		 * SPEECH_PARAMS_HELI
		 * SPEECH_PARAMS_FORCE_MEGAPHONE
		 * SPEECH_PARAMS_FORCE_HELI
		 * SPEECH_PARAMS_INTERRUPT
		 * SPEECH_PARAMS_INTERRUPT_SHOUTED
		 * SPEECH_PARAMS_INTERRUPT_SHOUTED_CLEAR
		 * SPEECH_PARAMS_INTERRUPT_SHOUTED_CRITICAL
		 * SPEECH_PARAMS_INTERRUPT_NO_FORCE
		 * SPEECH_PARAMS_INTERRUPT_FRONTEND
		 * SPEECH_PARAMS_INTERRUPT_NO_FORCE_FRONTEND
		 * SPEECH_PARAMS_ADD_BLIP
		 * SPEECH_PARAMS_ADD_BLIP_ALLOW_REPEAT
		 * SPEECH_PARAMS_ADD_BLIP_FORCE
		 * SPEECH_PARAMS_ADD_BLIP_SHOUTED
		 * SPEECH_PARAMS_ADD_BLIP_SHOUTED_FORCE
		 * SPEECH_PARAMS_ADD_BLIP_INTERRUPT
		 * SPEECH_PARAMS_ADD_BLIP_INTERRUPT_FORCE
		 * SPEECH_PARAMS_FORCE_PRELOAD_ONLY_SHOUTED
		 * SPEECH_PARAMS_FORCE_PRELOAD_ONLY_SHOUTED_CLEAR
		 * SPEECH_PARAMS_FORCE_PRELOAD_ONLY_SHOUTED_CRITICAL
		 * SPEECH_PARAMS_SHOUTED
		 * SPEECH_PARAMS_SHOUTED_CLEAR
		 * SPEECH_PARAMS_SHOUTED_CRITICAL
		 * 
		 * Note: A list of Name and Parameters can be found here https://pastebin.com/1GZS5dCL
		 * 
		 * Full list of speeches and voices names by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/speeches.json
		 */
		void PLAY_PED_AMBIENT_SPEECH_NATIVE(Ped ped, const char* speechName, const char* speechParam, Any p3);
		/**
		 * Plays ambient speech. See also _0x5C57B85D.
		 * 
		 * See PLAY_PED_AMBIENT_SPEECH_NATIVE for parameter specifications.
		 * 
		 * Full list of speeches and voices names by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/speeches.json
		 */
		void PLAY_PED_AMBIENT_SPEECH_AND_CLONE_NATIVE(Ped ped, const char* speechName, const char* speechParam, Any p3);
		/**
		 * This is the same as PLAY_PED_AMBIENT_SPEECH_NATIVE and PLAY_PED_AMBIENT_SPEECH_AND_CLONE_NATIVE but it will allow you to play a speech file from a specific voice file. It works on players and all peds, even animals.
		 * 
		 * EX (C#):
		 * GTA.Native.Function.Call(Hash.PLAY_PED_AMBIENT_SPEECH_WITH_VOICE_NATIVE, Game.Player.Character, "GENERIC_INSULT_HIGH", "s_m_y_sheriff_01_white_full_01", "SPEECH_PARAMS_FORCE_SHOUTED", 0);
		 * 
		 * The first param is the ped you want to play it on, the second is the speech name, the third is the voice name, the fourth is the speech param, and the last param is usually always 0.
		 * 
		 * Full list of speeches and voices names by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/speeches.json
		 */
		void PLAY_PED_AMBIENT_SPEECH_WITH_VOICE_NATIVE(Ped ped, const char* speechName, const char* voiceName, const char* speechParam, BOOL p4);
		/**
		 * Full list of speeches and voices names by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/speeches.json
		 */
		void PLAY_AMBIENT_SPEECH_FROM_POSITION_NATIVE(const char* speechName, const char* voiceName, float x, float y, float z, const char* speechParam);
		/**
		 * This native enables the audio flag "TrevorRageIsOverridden" and sets the voice effect to `voiceEffect`
		 */
		void OVERRIDE_TREVOR_RAGE(const char* voiceEffect);
		void RESET_TREVOR_RAGE();
		void SET_PLAYER_ANGRY(Ped ped, BOOL toggle);
		/**
		 * Needs another parameter [int p2]. The signature is PED::PLAY_PAIN(Ped ped, int painID, int p1, int p2);
		 * 
		 * Last 2 parameters always seem to be 0.
		 * 
		 * EX: Function.Call(Hash.PLAY_PAIN, TestPed, 6, 0, 0);
		 * 
		 * Known Pain IDs
		 * ________________________
		 * 
		 * 1 - Doesn't seem to do anything. Does NOT crash the game like previously said. (Latest patch)
		 * 6 - Scream (Short)
		 * 7 - Scared Scream (Kinda Long)
		 * 8 - On Fire
		 * 
		 */
		void PLAY_PAIN(Ped ped, int painID, int p1, Any p3);
		void RELEASE_WEAPON_AUDIO();
		/**
		 * mode can be any of these:
		 * SLOWMO_T1_TRAILER_SMASH
		 * SLOWMO_T1_RAYFIRE_EXPLOSION
		 * SLOWMO_PROLOGUE_VAULT
		 * NIGEL_02_SLOWMO_SETTING
		 * JSH_EXIT_TUNNEL_SLOWMO
		 * SLOWMO_BIG_SCORE_JUMP
		 * SLOWMO_FIB4_TRUCK_SMASH
		 * SLOWMO_EXTREME_04
		 * SLOW_MO_METH_HOUSE_RAYFIRE
		 * BARRY_02_SLOWMO
		 * BARRY_01_SLOWMO
		 */
		void ACTIVATE_AUDIO_SLOWMO_MODE(const char* mode);
		/**
		 * see ACTIVATE_AUDIO_SLOWMO_MODE for modes
		 */
		void DEACTIVATE_AUDIO_SLOWMO_MODE(const char* mode);
		/**
		 * Audio List
		 * gtaforums.com/topic/795622-audio-for-mods/
		 * 
		 * All found occurrences in b617d, sorted alphabetically and identical lines removed: https://pastebin.com/FTeAj4yZ
		 */
		void SET_AMBIENT_VOICE_NAME(Ped ped, const char* name);
		void SET_AMBIENT_VOICE_NAME_HASH(Ped ped, Hash hash);
		Hash GET_AMBIENT_VOICE_NAME_HASH(Ped ped);
		/**
		 * Assigns some ambient voice to the ped.
		 */
		void SET_PED_VOICE_FULL(Ped ped);
		void SET_PED_RACE_AND_VOICE_GROUP(Ped ped, int p1, Hash voiceGroup);
		/**
		 * From the scripts:
		 * 
		 * AUDIO::SET_PED_VOICE_GROUP(PLAYER::PLAYER_PED_ID(), MISC::GET_HASH_KEY("PAIGE_PVG"));
		 * AUDIO::SET_PED_VOICE_GROUP(PLAYER::PLAYER_PED_ID(), MISC::GET_HASH_KEY("TALINA_PVG"));
		 * AUDIO::SET_PED_VOICE_GROUP(PLAYER::PLAYER_PED_ID(), MISC::GET_HASH_KEY("FEMALE_LOST_BLACK_PVG"));
		 * AUDIO::SET_PED_VOICE_GROUP(PLAYER::PLAYER_PED_ID(), MISC::GET_HASH_KEY("FEMALE_LOST_WHITE_PVG"));
		 */
		void SET_PED_VOICE_GROUP(Ped ped, Hash voiceGroupHash);
		/**
		 * Dat151RelType == 29
		 */
		void SET_PED_VOICE_GROUP_FROM_RACE_TO_PVG(Ped ped, Hash voiceGroupHash);
		/**
		 * BOOL p1: 0 = Female; 1 = Male
		 */
		void SET_PED_GENDER(Ped ped, BOOL p1);
		void STOP_CURRENT_PLAYING_SPEECH(Ped ped);
		void STOP_CURRENT_PLAYING_AMBIENT_SPEECH(Ped ped);
		BOOL IS_AMBIENT_SPEECH_PLAYING(Ped ped);
		BOOL IS_SCRIPTED_SPEECH_PLAYING(Ped p0);
		BOOL IS_ANY_SPEECH_PLAYING(Ped ped);
		BOOL IS_ANY_POSITIONAL_SPEECH_PLAYING();
		/**
		 * Checks if the ped can play the speech or has the speech file, p2 is usually false.
		 */
		BOOL DOES_CONTEXT_EXIST_FOR_THIS_PED(Ped ped, const char* speechName, BOOL p2);
		BOOL IS_PED_IN_CURRENT_CONVERSATION(Ped ped);
		/**
		 * Sets the ped drunk sounds.  Only works with PLAYER_PED_ID
		 * 
		 * ====================================================
		 * 
		 * As mentioned above, this only sets the drunk sound to ped/player.
		 * 
		 * To give the Ped a drunk effect with drunk walking animation try using SET_PED_MOVEMENT_CLIPSET
		 * 
		 * Below is an example
		 * 
		 * if (!Function.Call<bool>(Hash.HAS_ANIM_SET_LOADED, "move_m@drunk@verydrunk"))
		 * {
		 * Function.Call(Hash.REQUEST_ANIM_SET, "move_m@drunk@verydrunk");
		 * }
		 * Function.Call(Hash.SET_PED_MOVEMENT_CLIPSET, Ped.Handle, "move_m@drunk@verydrunk", 0x3E800000);
		 * 
		 * 
		 * 
		 * And to stop the effect use
		 * RESET_PED_MOVEMENT_CLIPSET
		 */
		void SET_PED_IS_DRUNK(Ped ped, BOOL toggle);
		/**
		 * Plays sounds from a ped with chop model. For example it used to play bark or sniff sounds. p1 is always 3 or 4294967295 in decompiled scripts. By a quick disassembling I can assume that this arg is unused.
		 * This native is works only when you call it on the ped with right model (ac_chop only ?)
		 * Speech Name can be: CHOP_SNIFF_SEQ CHOP_WHINE CHOP_LICKS_MOUTH CHOP_PANT bark GROWL SNARL BARK_SEQ
		 */
		void PLAY_ANIMAL_VOCALIZATION(Ped pedHandle, int p1, const char* speechName);
		BOOL IS_ANIMAL_VOCALIZATION_PLAYING(Ped pedHandle);
		/**
		 * mood can be 0 or 1 (it's not a boolean value!). Effects audio of the animal.
		 */
		void SET_ANIMAL_MOOD(Ped animal, int mood);
		void PLAY_PED_AUDIO_EVENT_ANIM(Ped pedHandle, const char* audioEvent); // Missing in crossmap
		BOOL IS_MOBILE_PHONE_RADIO_ACTIVE();
		void SET_MOBILE_PHONE_RADIO_STATE(BOOL state);
		/**
		 * Returns 255 (radio off index) if the function fails.
		 */
		int GET_PLAYER_RADIO_STATION_INDEX();
		/**
		 * Returns active radio station name
		 */
		const char* GET_PLAYER_RADIO_STATION_NAME();
		/**
		 * Converts radio station index to string. Use HUD::GET_FILENAME_FOR_AUDIO_CONVERSATION to get the user-readable text.
		 */
		const char* GET_RADIO_STATION_NAME(int radioStation);
		int GET_PLAYER_RADIO_STATION_GENRE();
		BOOL IS_RADIO_RETUNING();
		BOOL IS_RADIO_FADED_OUT();
		/**
		 * Tune Forward...
		 */
		void SET_RADIO_RETUNE_UP();
		/**
		 * Tune Backwards...
		 */
		void SET_RADIO_RETUNE_DOWN();
		/**
		 * List of radio stations that are in the wheel, in clockwise order, as of LS Tuners DLC: https://git.io/J8a3k
		 * An older list including hidden radio stations: https://pastebin.com/Kj9t38KF
		 */
		void SET_RADIO_TO_STATION_NAME(const char* stationName);
		/**
		 * List of radio stations that are in the wheel, in clockwise order, as of LS Tuners DLC: https://git.io/J8a3k
		 * An older list including hidden radio stations: https://pastebin.com/Kj9t38KF
		 */
		void SET_VEH_RADIO_STATION(Vehicle vehicle, const char* radioStation);
		void SET_VEH_HAS_NORMAL_RADIO(Vehicle vehicle);
		BOOL IS_VEHICLE_RADIO_ON(Vehicle vehicle);
		void SET_VEH_FORCED_RADIO_THIS_FRAME(Vehicle vehicle);
		/**
		 * Full list of static emitters by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/staticEmitters.json
		 */
		void SET_EMITTER_RADIO_STATION(const char* emitterName, const char* radioStation, Any p2);
		/**
		 * Example:
		 * AUDIO::SET_STATIC_EMITTER_ENABLED((Any*)"LOS_SANTOS_VANILLA_UNICORN_01_STAGE", false);    AUDIO::SET_STATIC_EMITTER_ENABLED((Any*)"LOS_SANTOS_VANILLA_UNICORN_02_MAIN_ROOM", false);    AUDIO::SET_STATIC_EMITTER_ENABLED((Any*)"LOS_SANTOS_VANILLA_UNICORN_03_BACK_ROOM", false);
		 * 
		 * This turns off surrounding sounds not connected directly to peds.
		 * 
		 * Full list of static emitters by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/staticEmitters.json
		 */
		void SET_STATIC_EMITTER_ENABLED(const char* emitterName, BOOL toggle);
		/**
		 * Full list of static emitters by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/staticEmitters.json
		 */
		void LINK_STATIC_EMITTER_TO_ENTITY(const char* emitterName, Entity entity);
		/**
		 * Sets radio station by index.
		 */
		void SET_RADIO_TO_STATION_INDEX(int radioStation);
		void SET_FRONTEND_RADIO_ACTIVE(BOOL active);
		/**
		 * "news" that play on the radio after you've done something in story mode(?)
		 */
		void UNLOCK_MISSION_NEWS_STORY(int newsStory);
		BOOL IS_MISSION_NEWS_STORY_UNLOCKED(int newsStory);
		int GET_AUDIBLE_MUSIC_TRACK_TEXT_ID();
		void PLAY_END_CREDITS_MUSIC(BOOL play);
		void SKIP_RADIO_FORWARD();
		void FREEZE_RADIO_STATION(const char* radioStation);
		void UNFREEZE_RADIO_STATION(const char* radioStation);
		void SET_RADIO_AUTO_UNFREEZE(BOOL toggle);
		void SET_INITIAL_PLAYER_STATION(const char* radioStation);
		void SET_USER_RADIO_CONTROL_ENABLED(BOOL toggle);
		/**
		 * Only found this one in the decompiled scripts:
		 * 
		 * AUDIO::SET_RADIO_TRACK("RADIO_03_HIPHOP_NEW", "ARM1_RADIO_STARTS");
		 * 
		 */
		void SET_RADIO_TRACK(const char* radioStation, const char* radioTrack);
		void SET_RADIO_TRACK_WITH_START_OFFSET(const char* radioStationName, const char* mixName, int p2);
		void SET_NEXT_RADIO_TRACK(const char* radioName, const char* radioTrack, const char* p2, const char* p3);
		void SET_VEHICLE_RADIO_LOUD(Vehicle vehicle, BOOL toggle);
		BOOL CAN_VEHICLE_RECEIVE_CB_RADIO(Vehicle vehicle);
		void SET_MOBILE_RADIO_ENABLED_DURING_GAMEPLAY(BOOL toggle);
		BOOL DOES_PLAYER_VEH_HAVE_RADIO();
		BOOL IS_PLAYER_VEH_RADIO_ENABLE();
		/**
		 * can't seem to enable radio on cop cars etc
		 */
		void SET_VEHICLE_RADIO_ENABLED(Vehicle vehicle, BOOL toggle);
		void SET_POSITIONED_PLAYER_VEHICLE_RADIO_EMITTER_ENABLED(Any p0);
		/**
		 * Examples:
		 * 
		 * AUDIO::SET_CUSTOM_RADIO_TRACK_LIST("RADIO_01_CLASS_ROCK", "END_CREDITS_KILL_MICHAEL", 1);
		 * AUDIO::SET_CUSTOM_RADIO_TRACK_LIST("RADIO_01_CLASS_ROCK", "END_CREDITS_KILL_MICHAEL", 1);
		 * AUDIO::SET_CUSTOM_RADIO_TRACK_LIST("RADIO_01_CLASS_ROCK", "END_CREDITS_KILL_TREVOR", 1);
		 * AUDIO::SET_CUSTOM_RADIO_TRACK_LIST("RADIO_01_CLASS_ROCK", "END_CREDITS_SAVE_MICHAEL_TREVOR", 1);
		 * AUDIO::SET_CUSTOM_RADIO_TRACK_LIST("RADIO_01_CLASS_ROCK", "OFF_ROAD_RADIO_ROCK_LIST", 1);
		 * AUDIO::SET_CUSTOM_RADIO_TRACK_LIST("RADIO_06_COUNTRY", "MAGDEMO2_RADIO_DINGHY", 1);
		 * AUDIO::SET_CUSTOM_RADIO_TRACK_LIST("RADIO_16_SILVERLAKE", "SEA_RACE_RADIO_PLAYLIST", 1);
		 * AUDIO::SET_CUSTOM_RADIO_TRACK_LIST("RADIO_01_CLASS_ROCK", "OFF_ROAD_RADIO_ROCK_LIST", 1);
		 */
		void SET_CUSTOM_RADIO_TRACK_LIST(const char* radioStation, const char* trackListName, BOOL p2);
		/**
		 * 3 calls in the b617d scripts, removed duplicate.
		 * 
		 * AUDIO::CLEAR_CUSTOM_RADIO_TRACK_LIST("RADIO_16_SILVERLAKE");
		 * AUDIO::CLEAR_CUSTOM_RADIO_TRACK_LIST("RADIO_01_CLASS_ROCK");
		 */
		void CLEAR_CUSTOM_RADIO_TRACK_LIST(const char* radioStation);
		int GET_NUM_UNLOCKED_RADIO_STATIONS();
		int FIND_RADIO_STATION_INDEX(Hash stationNameHash);
		/**
		 * 6 calls in the b617d scripts, removed identical lines:
		 * 
		 * AUDIO::SET_RADIO_STATION_MUSIC_ONLY("RADIO_01_CLASS_ROCK", 1);
		 * AUDIO::SET_RADIO_STATION_MUSIC_ONLY(AUDIO::GET_RADIO_STATION_NAME(10), 0);
		 * AUDIO::SET_RADIO_STATION_MUSIC_ONLY(AUDIO::GET_RADIO_STATION_NAME(10), 1);
		 */
		void SET_RADIO_STATION_MUSIC_ONLY(const char* radioStation, BOOL toggle);
		void SET_RADIO_FRONTEND_FADE_TIME(float fadeTime);
		/**
		 * AUDIO::UNLOCK_RADIO_STATION_TRACK_LIST("RADIO_16_SILVERLAKE", "MIRRORPARK_LOCKED");
		 */
		void UNLOCK_RADIO_STATION_TRACK_LIST(const char* radioStation, const char* trackListName);
		void LOCK_RADIO_STATION_TRACK_LIST(const char* radioStation, const char* trackListName);
		/**
		 * Just a nullsub (i.e. does absolutely nothing) since build 1604.
		 */
		void UPDATE_UNLOCKABLE_DJ_RADIO_TRACKS(BOOL enableMixes);
		/**
		 * Disables the radio station (hides it from the radio wheel).
		 */
		void LOCK_RADIO_STATION(const char* radioStationName, BOOL toggle);
		/**
		 * Doesn't have an effect in Story Mode.
		 */
		void SET_RADIO_STATION_AS_FAVOURITE(const char* radioStation, BOOL toggle);
		BOOL IS_RADIO_STATION_FAVOURITED(const char* radioStation);
		BOOL GET_NEXT_AUDIBLE_BEAT(float* out1, float* out2, int* out3);
		/**
		 * Changes start time of a tracklist (milliseconds)
		 * R* uses a random int: MISC::GET_RANDOM_INT_IN_RANGE(0, 13) * 60000)
		 */
		void FORCE_MUSIC_TRACK_LIST(const char* radioStation, const char* trackListName, int milliseconds);
		int GET_CURRENT_TRACK_PLAY_TIME(const char* radioStationName);
		Hash GET_CURRENT_TRACK_SOUND_NAME(const char* radioStationName);
		void SET_VEHICLE_MISSILE_WARNING_ENABLED(Vehicle vehicle, BOOL toggle);
		/**
		 * Full list of ambient zones by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/ambientZones.json
		 */
		void SET_AMBIENT_ZONE_STATE(const char* zoneName, BOOL p1, BOOL p2);
		/**
		 * This function also has a p2, unknown. Signature AUDIO::CLEAR_AMBIENT_ZONE_STATE(const char* zoneName, bool p1, Any p2);
		 * 
		 * Still needs more research.
		 * 
		 * Full list of ambient zones by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/ambientZones.json
		 */
		void CLEAR_AMBIENT_ZONE_STATE(const char* zoneName, BOOL p1);
		void SET_AMBIENT_ZONE_LIST_STATE(const char* ambientZone, BOOL p1, BOOL p2);
		void CLEAR_AMBIENT_ZONE_LIST_STATE(const char* ambientZone, BOOL p1);
		/**
		 * Full list of ambient zones by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/ambientZones.json
		 */
		void SET_AMBIENT_ZONE_STATE_PERSISTENT(const char* ambientZone, BOOL p1, BOOL p2);
		/**
		 * Full list of ambient zones by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/ambientZones.json
		 */
		void SET_AMBIENT_ZONE_LIST_STATE_PERSISTENT(const char* ambientZone, BOOL p1, BOOL p2);
		/**
		 * Full list of ambient zones by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/ambientZones.json
		 */
		BOOL IS_AMBIENT_ZONE_ENABLED(const char* ambientZone);
		void REFRESH_CLOSEST_OCEAN_SHORELINE();
		/**
		 * All occurrences found in b617d, sorted alphabetically and identical lines removed:
		 * 
		 * AUDIO::SET_CUTSCENE_AUDIO_OVERRIDE("_AK");
		 * AUDIO::SET_CUTSCENE_AUDIO_OVERRIDE("_CUSTOM");
		 * AUDIO::SET_CUTSCENE_AUDIO_OVERRIDE("_TOOTHLESS");
		 * Full list of cutscene names by DurtyFree https://github.com/DurtyFree/gta-v-data-dumps/blob/master/cutsceneNames.json
		 */
		void SET_CUTSCENE_AUDIO_OVERRIDE(const char* name);
		void SET_VARIABLE_ON_SYNCH_SCENE_AUDIO(const char* variableName, float value);
		/**
		 * Plays the given police radio message.
		 * 
		 * All found occurrences in b617d, sorted alphabetically and identical lines removed: https://pastebin.com/GBnsQ5hr
		 * Full list of police report names by DurtyFree https://github.com/DurtyFree/gta-v-data-dumps/blob/master/policeReportNames.json
		 */
		int PLAY_POLICE_REPORT(const char* name, float p1);
		void CANCEL_ALL_POLICE_REPORTS();
		/**
		 * Plays the siren sound of a vehicle which is otherwise activated when fastly double-pressing the horn key.
		 * Only works on vehicles with a police siren.
		 */
		void BLIP_SIREN(Vehicle vehicle);
		/**
		 * Overrides the vehicle's horn hash.
		 * When changing this hash on a vehicle, it will not return the 'overwritten' hash. It will still always return the default horn hash (same as GET_VEHICLE_DEFAULT_HORN)
		 * 
		 * vehicle - the vehicle whose horn should be overwritten
		 */
		void OVERRIDE_VEH_HORN(Vehicle vehicle, BOOL override, int hornHash);
		/**
		 * Checks whether the horn of a vehicle is currently played.
		 */
		BOOL IS_HORN_ACTIVE(Vehicle vehicle);
		/**
		 * Makes pedestrians sound their horn longer, faster and more agressive when they use their horn.
		 */
		void SET_AGGRESSIVE_HORNS(BOOL toggle);
		/**
		 * Does nothing (it's a nullsub).
		 */
		void SET_RADIO_POSITION_AUDIO_MUTE(BOOL p0);
		void SET_VEHICLE_CONVERSATIONS_PERSIST(BOOL p0, BOOL p1);
		void SET_VEHICLE_CONVERSATIONS_PERSIST_NEW(BOOL p0, BOOL p1, BOOL p2);
		BOOL IS_STREAM_PLAYING();
		int GET_STREAM_PLAY_TIME();
		/**
		 * Example:
		 * AUDIO::LOAD_STREAM("CAR_STEAL_1_PASSBY", "CAR_STEAL_1_SOUNDSET");
		 * 
		 * All found occurrences in the b678d decompiled scripts: https://pastebin.com/3rma6w5w
		 * 
		 * Stream names often ends with "_MASTER", "_SMALL" or "_STREAM". Also "_IN", "_OUT" and numbers.
		 * 
		 * soundSet is often set to 0 in the scripts. These are common to end the soundSets: "_SOUNDS", "_SOUNDSET" and numbers.
		 * 
		 * Full list of audio / sound names by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/soundNames.json
		 */
		BOOL LOAD_STREAM(const char* streamName, const char* soundSet);
		/**
		 * Example:
		 * AUDIO::LOAD_STREAM_WITH_START_OFFSET("STASH_TOXIN_STREAM", 2400, "FBI_05_SOUNDS");
		 * 
		 * Only called a few times in the scripts.
		 * 
		 * Full list of audio / sound names by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/soundNames.json
		 */
		BOOL LOAD_STREAM_WITH_START_OFFSET(const char* streamName, int startOffset, const char* soundSet);
		void PLAY_STREAM_FROM_PED(Ped ped);
		void PLAY_STREAM_FROM_VEHICLE(Vehicle vehicle);
		/**
		 * Used with AUDIO::LOAD_STREAM
		 * 
		 * Example from finale_heist2b.c4:
		 * TASK::TASK_SYNCHRONIZED_SCENE(l_4C8[2/*14* /], l_4C8[2/*14* /]._f7, l_30A, "push_out_vault_l", 4.0, -1.5, 5, 713, 4.0, 0);
		 * PED::SET_SYNCHRONIZED_SCENE_PHASE(l_4C8[2/*14* /]._f7, 0.0);
		 * PED::FORCE_PED_AI_AND_ANIMATION_UPDATE(l_4C8[2/*14* /], 0, 0);
		 * PED::SET_PED_COMBAT_ATTRIBUTES(l_4C8[2/*14* /], 38, 1);
		 * PED::SET_BLOCKING_OF_NON_TEMPORARY_EVENTS(l_4C8[2/*14* /], 1);
		 * if (AUDIO::LOAD_STREAM("Gold_Cart_Push_Anim_01", "BIG_SCORE_3B_SOUNDS")) {
		 * AUDIO::PLAY_STREAM_FROM_OBJECT(l_36F[0/*1* /]);
		 * }
		 */
		void PLAY_STREAM_FROM_OBJECT(Object object);
		void PLAY_STREAM_FRONTEND();
		void PLAY_STREAM_FROM_POSITION(float x, float y, float z);
		void STOP_STREAM();
		void STOP_PED_SPEAKING(Ped ped, BOOL shaking);
		void BLOCK_ALL_SPEECH_FROM_PED(Ped ped, BOOL p1, BOOL p2);
		void STOP_PED_SPEAKING_SYNCED(Ped ped, BOOL p1);
		void DISABLE_PED_PAIN_AUDIO(Ped ped, BOOL toggle);
		/**
		 * Common in the scripts:
		 * AUDIO::IS_AMBIENT_SPEECH_DISABLED(PLAYER::PLAYER_PED_ID());
		 */
		BOOL IS_AMBIENT_SPEECH_DISABLED(Ped ped);
		void BLOCK_SPEECH_CONTEXT_GROUP(const char* p0, int p1);
		void UNBLOCK_SPEECH_CONTEXT_GROUP(const char* p0);
		void SET_SIREN_WITH_NO_DRIVER(Vehicle vehicle, BOOL toggle);
		void SET_SIREN_BYPASS_MP_DRIVER_CHECK(Vehicle vehicle, BOOL toggle);
		void TRIGGER_SIREN_AUDIO(Vehicle vehicle);
		void SET_HORN_PERMANENTLY_ON(Vehicle vehicle);
		void SET_HORN_ENABLED(Vehicle vehicle, BOOL toggle);
		void SET_AUDIO_VEHICLE_PRIORITY(Vehicle vehicle, Any p1);
		void SET_HORN_PERMANENTLY_ON_TIME(Vehicle vehicle, float time);
		void USE_SIREN_AS_HORN(Vehicle vehicle, BOOL toggle);
		/**
		 * This native sets the audio of the specified vehicle to the audioName (p1).
		 * 
		 * Use the audioNameHash found in vehicles.meta
		 * 
		 * Example:
		 * _SET_VEHICLE_AUDIO(veh, "ADDER");
		 * The selected vehicle will now have the audio of the Adder.
		 */
		void FORCE_USE_AUDIO_GAME_OBJECT(Vehicle vehicle, const char* audioName);
		void PRELOAD_VEHICLE_AUDIO_BANK(Hash vehicleModel);
		void SET_VEHICLE_STARTUP_REV_SOUND(Vehicle vehicle, const char* p1, const char* p2);
		void RESET_VEHICLE_STARTUP_REV_SOUND(Vehicle vehicle);
		void SET_VEHICLE_FORCE_REVERSE_WARNING(Any p0, Any p1);
		BOOL IS_VEHICLE_AUDIBLY_DAMAGED(Vehicle vehicle);
		void SET_VEHICLE_AUDIO_ENGINE_DAMAGE_FACTOR(Vehicle vehicle, float damageFactor);
		/**
		 * intensity: 0.0f - 1.0f, only used once with 1.0f in R* Scripts (nigel2)
		 * Makes an engine rattling noise when you decelerate, you need to be going faster to hear lower values
		 */
		void SET_VEHICLE_AUDIO_BODY_DAMAGE_FACTOR(Vehicle vehicle, float intensity);
		/**
		 * Called together with SET_VEHICLE_TYRES_CAN_BURST
		 */
		void _FORCE_VEHICLE_ENGINE_SYNTH(Vehicle vehicle, BOOL force);
		void ENABLE_VEHICLE_FANBELT_DAMAGE(Vehicle vehicle, BOOL toggle);
		void ENABLE_VEHICLE_EXHAUST_POPS(Vehicle vehicle, BOOL toggle);
		/**
		 * SET_VEHICLE_BOOST_ACTIVE(vehicle, 1, 0);
		 * SET_VEHICLE_BOOST_ACTIVE(vehicle, 0, 0);
		 * 
		 * Will give a boost-soundeffect.
		 */
		void SET_VEHICLE_BOOST_ACTIVE(Vehicle vehicle, BOOL toggle);
		void SET_PLAYER_VEHICLE_ALARM_AUDIO_ACTIVE(Vehicle vehicle, BOOL toggle);
		void SET_SCRIPT_UPDATE_DOOR_AUDIO(Hash doorHash, BOOL toggle);
		/**
		 * doorId: see SET_VEHICLE_DOOR_SHUT
		 */
		void PLAY_VEHICLE_DOOR_OPEN_SOUND(Vehicle vehicle, int doorId);
		/**
		 * doorId: see SET_VEHICLE_DOOR_SHUT
		 */
		void PLAY_VEHICLE_DOOR_CLOSE_SOUND(Vehicle vehicle, int doorId);
		/**
		 * Works for planes only.
		 */
		void ENABLE_STALL_WARNING_SOUNDS(Vehicle vehicle, BOOL toggle);
		void _ENABLE_DRAG_RACE_STATIONARY_WARNING_SOUNDS(Vehicle vehicle, BOOL enable);
		/**
		 * Hardcoded to return 1
		 */
		BOOL IS_GAME_IN_CONTROL_OF_MUSIC();
		void SET_GPS_ACTIVE(BOOL active);
		/**
		 * Called 38 times in the scripts. There are 5 different audioNames used.
		 * One unknown removed below.
		 * 
		 * AUDIO::PLAY_MISSION_COMPLETE_AUDIO("DEAD");
		 * AUDIO::PLAY_MISSION_COMPLETE_AUDIO("FRANKLIN_BIG_01");
		 * AUDIO::PLAY_MISSION_COMPLETE_AUDIO("GENERIC_FAILED");
		 * AUDIO::PLAY_MISSION_COMPLETE_AUDIO("TREVOR_SMALL_01");
		 */
		void PLAY_MISSION_COMPLETE_AUDIO(const char* audioName);
		BOOL IS_MISSION_COMPLETE_PLAYING();
		BOOL IS_MISSION_COMPLETE_READY_FOR_UI();
		void BLOCK_DEATH_JINGLE(BOOL toggle);
		/**
		 * Used to prepare a scene where the surrounding sound is muted or a bit changed. This does not play any sound.
		 * 
		 * List of all usable scene names found in b617d. Sorted alphabetically and identical names removed: https://pastebin.com/MtM9N9CC
		 * Full list of audio scene names by DurtyFree https://github.com/DurtyFree/gta-v-data-dumps/blob/master/audioSceneNames.json
		 */
		BOOL START_AUDIO_SCENE(const char* scene);
		/**
		 * Full list of audio scene names by DurtyFree https://github.com/DurtyFree/gta-v-data-dumps/blob/master/audioSceneNames.json
		 */
		void STOP_AUDIO_SCENE(const char* scene);
		void STOP_AUDIO_SCENES();
		/**
		 * Full list of audio scene names by DurtyFree https://github.com/DurtyFree/gta-v-data-dumps/blob/master/audioSceneNames.json
		 */
		BOOL IS_AUDIO_SCENE_ACTIVE(const char* scene);
		/**
		 * Full list of audio scene names by DurtyFree https://github.com/DurtyFree/gta-v-data-dumps/blob/master/audioSceneNames.json
		 */
		void SET_AUDIO_SCENE_VARIABLE(const char* scene, const char* variable, float value);
		void SET_AUDIO_SCRIPT_CLEANUP_TIME(int time);
		/**
		 * All found occurrences in b678d:
		 * https://pastebin.com/ceu67jz8
		 */
		void ADD_ENTITY_TO_AUDIO_MIX_GROUP(Entity entity, const char* groupName, float p2);
		void REMOVE_ENTITY_FROM_AUDIO_MIX_GROUP(Entity entity, float p1);
		BOOL AUDIO_IS_MUSIC_PLAYING();
		/**
		 * This is an alias of AUDIO_IS_MUSIC_PLAYING.
		 */
		BOOL AUDIO_IS_SCRIPTED_MUSIC_PLAYING();
		/**
		 * All music event names found in the b617d scripts: https://pastebin.com/GnYt0R3P
		 * Full list of music event names by DurtyFree https://github.com/DurtyFree/gta-v-data-dumps/blob/master/musicEventNames.json
		 */
		BOOL PREPARE_MUSIC_EVENT(const char* eventName);
		/**
		 * All music event names found in the b617d scripts: https://pastebin.com/GnYt0R3P
		 * Full list of music event names by DurtyFree https://github.com/DurtyFree/gta-v-data-dumps/blob/master/musicEventNames.json
		 */
		BOOL CANCEL_MUSIC_EVENT(const char* eventName);
		/**
		 * List of all usable event names found in b617d used with this native. Sorted alphabetically and identical names removed: https://pastebin.com/RzDFmB1W
		 * 
		 * All music event names found in the b617d scripts: https://pastebin.com/GnYt0R3P
		 * Full list of music event names by DurtyFree https://github.com/DurtyFree/gta-v-data-dumps/blob/master/musicEventNames.json
		 */
		BOOL TRIGGER_MUSIC_EVENT(const char* eventName);
		BOOL IS_MUSIC_ONESHOT_PLAYING();
		int GET_MUSIC_PLAYTIME();
		void SET_GLOBAL_RADIO_SIGNAL_LEVEL(Any p0);
		void RECORD_BROKEN_GLASS(float x, float y, float z, float radius);
		void CLEAR_ALL_BROKEN_GLASS();
		void SCRIPT_OVERRIDES_WIND_ELEVATION(BOOL p0, Any p1);
		void SET_PED_WALLA_DENSITY(float p0, float p1);
		void SET_PED_INTERIOR_WALLA_DENSITY(float p0, float p1);
		void FORCE_PED_PANIC_WALLA();
		/**
		 * Example:
		 * 
		 * bool prepareAlarm = AUDIO::PREPARE_ALARM("PORT_OF_LS_HEIST_FORT_ZANCUDO_ALARMS");
		 * Full list of alarm names by DurtyFree https://github.com/DurtyFree/gta-v-data-dumps/blob/master/alarmSounds.json
		 */
		BOOL PREPARE_ALARM(const char* alarmName);
		/**
		 * Example:
		 * 
		 * This will start the alarm at Fort Zancudo.
		 * 
		 * AUDIO::START_ALARM("PORT_OF_LS_HEIST_FORT_ZANCUDO_ALARMS", 1);
		 * 
		 * First parameter (char) is the name of the alarm.
		 * Second parameter (bool) is unknown, it does not seem to make a difference if this one is 0 or 1.
		 * 
		 * ----------
		 * 
		 * It DOES make a difference but it has to do with the duration or something I dunno yet
		 * 
		 * ----------
		 * 
		 * Found in the b617d scripts:
		 * 
		 * AUDIO::START_ALARM("AGENCY_HEIST_FIB_TOWER_ALARMS", 0);
		 * AUDIO::START_ALARM("AGENCY_HEIST_FIB_TOWER_ALARMS_UPPER", 1);
		 * AUDIO::START_ALARM("AGENCY_HEIST_FIB_TOWER_ALARMS_UPPER_B", 0);
		 * AUDIO::START_ALARM("BIG_SCORE_HEIST_VAULT_ALARMS", a_0);
		 * AUDIO::START_ALARM("FBI_01_MORGUE_ALARMS", 1);
		 * AUDIO::START_ALARM("FIB_05_BIOTECH_LAB_ALARMS", 0);
		 * AUDIO::START_ALARM("JEWEL_STORE_HEIST_ALARMS", 0);
		 * AUDIO::START_ALARM("PALETO_BAY_SCORE_ALARM", 1);
		 * AUDIO::START_ALARM("PALETO_BAY_SCORE_CHICKEN_FACTORY_ALARM", 0);
		 * AUDIO::START_ALARM("PORT_OF_LS_HEIST_FORT_ZANCUDO_ALARMS", 1);
		 * AUDIO::START_ALARM("PORT_OF_LS_HEIST_SHIP_ALARMS", 0);
		 * AUDIO::START_ALARM("PRISON_ALARMS", 0);
		 * AUDIO::START_ALARM("PROLOGUE_VAULT_ALARMS", 0);
		 * Full list of alarm names by DurtyFree https://github.com/DurtyFree/gta-v-data-dumps/blob/master/alarmSounds.json
		 */
		void START_ALARM(const char* alarmName, BOOL p2);
		/**
		 * Example:
		 * 
		 * This will stop the alarm at Fort Zancudo.
		 * 
		 * AUDIO::STOP_ALARM("PORT_OF_LS_HEIST_FORT_ZANCUDO_ALARMS", 1);
		 * 
		 * First parameter (char) is the name of the alarm.
		 * Second parameter (bool) has to be true (1) to have any effect.
		 * Full list of alarm names by DurtyFree https://github.com/DurtyFree/gta-v-data-dumps/blob/master/alarmSounds.json
		 */
		void STOP_ALARM(const char* alarmName, BOOL toggle);
		void STOP_ALL_ALARMS(BOOL stop);
		/**
		 * Example:
		 * 
		 * bool playing = AUDIO::IS_ALARM_PLAYING("PORT_OF_LS_HEIST_FORT_ZANCUDO_ALARMS");
		 * Full list of alarm names by DurtyFree https://github.com/DurtyFree/gta-v-data-dumps/blob/master/alarmSounds.json
		 */
		BOOL IS_ALARM_PLAYING(const char* alarmName);
		/**
		 * Returns hash of default vehicle horn
		 * 
		 * Hash is stored in audVehicleAudioEntity
		 */
		Hash GET_VEHICLE_DEFAULT_HORN(Vehicle vehicle);
		Hash GET_VEHICLE_DEFAULT_HORN_IGNORE_MODS(Vehicle vehicle);
		void RESET_PED_AUDIO_FLAGS(Ped ped);
		/**
		 * Enables/disables ped's "loud" footstep sound.
		 */
		void SET_PED_FOOTSTEPS_EVENTS_ENABLED(Ped ped, BOOL toggle);
		/**
		 * Enables/disables ped's "quiet" footstep sound.
		 */
		void SET_PED_CLOTH_EVENTS_ENABLED(Ped ped, BOOL toggle);
		/**
		 * Sets audio flag "OverridePlayerGroundMaterial"
		 */
		void OVERRIDE_PLAYER_GROUND_MATERIAL(Hash hash, BOOL toggle);
		void USE_FOOTSTEP_SCRIPT_SWEETENERS(Ped ped, BOOL p1, Hash hash);
		/**
		 * Sets audio flag "OverrideMicrophoneSettings"
		 */
		void OVERRIDE_MICROPHONE_SETTINGS(Hash hash, BOOL toggle);
		void FREEZE_MICROPHONE();
		/**
		 * If value is set to true, and ambient siren sound will be played.
		 * Appears to enable/disable an audio flag.
		 */
		void DISTANT_COP_CAR_SIRENS(BOOL value);
		void SET_SIREN_CAN_BE_CONTROLLED_BY_AUDIO(Vehicle vehicle, BOOL p1);
		void ENABLE_STUNT_JUMP_AUDIO();
		/**
		 * Possible flag names:
		 * "ActivateSwitchWheelAudio"
		 * "AllowAmbientSpeechInSlowMo"
		 * "AllowCutsceneOverScreenFade"
		 * "AllowForceRadioAfterRetune"
		 * "AllowPainAndAmbientSpeechToPlayDuringCutscene"
		 * "AllowPlayerAIOnMission"
		 * "AllowPoliceScannerWhenPlayerHasNoControl"
		 * "AllowRadioDuringSwitch"
		 * "AllowRadioOverScreenFade"
		 * "AllowScoreAndRadio"
		 * "AllowScriptedSpeechInSlowMo"
		 * "AvoidMissionCompleteDelay"
		 * "DisableAbortConversationForDeathAndInjury"
		 * "DisableAbortConversationForRagdoll"
		 * "DisableBarks"
		 * "DisableFlightMusic"
		 * "DisableReplayScriptStreamRecording"
		 * "EnableHeadsetBeep"
		 * "ForceConversationInterrupt"
		 * "ForceSeamlessRadioSwitch"
		 * "ForceSniperAudio"
		 * "FrontendRadioDisabled"
		 * "HoldMissionCompleteWhenPrepared"
		 * "IsDirectorModeActive"
		 * "IsPlayerOnMissionForSpeech"
		 * "ListenerReverbDisabled"
		 * "LoadMPData"
		 * "MobileRadioInGame"
		 * "OnlyAllowScriptTriggerPoliceScanner"
		 * "PlayMenuMusic"
		 * "PoliceScannerDisabled"
		 * "ScriptedConvListenerMaySpeak"
		 * "SpeechDucksScore"
		 * "SuppressPlayerScubaBreathing"
		 * "WantedMusicDisabled"
		 * "WantedMusicOnMission"
		 * 
		 * -------------------------------
		 * No added flag names between b393d and b573d, including b573d.
		 * 
		 * #######################################################################
		 * 
		 * "IsDirectorModeActive" is an audio flag which will allow you to play speech infinitely without any pauses like in Director Mode.
		 * 
		 * -----------------------------------------------------------------------
		 * 
		 * All flag IDs and hashes:
		 * 
		 * ID: 00 | Hash: 0x0FED7A7F
		 * ID: 01 | Hash: 0x20A7858F
		 * ID: 02 | Hash: 0xA11C2259
		 * ID: 03 | Hash: 0x08DE4700
		 * ID: 04 | Hash: 0x989F652F
		 * ID: 05 | Hash: 0x3C9E76BA
		 * ID: 06 | Hash: 0xA805FEB0
		 * ID: 07 | Hash: 0x4B94EA26
		 * ID: 08 | Hash: 0x803ACD34
		 * ID: 09 | Hash: 0x7C741226
		 * ID: 10 | Hash: 0x31DB9EBD
		 * ID: 11 | Hash: 0xDF386F18
		 * ID: 12 | Hash: 0x669CED42
		 * ID: 13 | Hash: 0x51F22743
		 * ID: 14 | Hash: 0x2052B35C
		 * ID: 15 | Hash: 0x071472DC
		 * ID: 16 | Hash: 0xF9928BCC
		 * ID: 17 | Hash: 0x7ADBDD48
		 * ID: 18 | Hash: 0xA959BA1A
		 * ID: 19 | Hash: 0xBBE89B60
		 * ID: 20 | Hash: 0x87A08871
		 * ID: 21 | Hash: 0xED1057CE
		 * ID: 22 | Hash: 0x1584AD7A
		 * ID: 23 | Hash: 0x8582CFCB
		 * ID: 24 | Hash: 0x7E5E2FB0
		 * ID: 25 | Hash: 0xAE4F72DB
		 * ID: 26 | Hash: 0x5D16D1FA
		 * ID: 27 | Hash: 0x06B2F4B8
		 * ID: 28 | Hash: 0x5D4CDC96
		 * ID: 29 | Hash: 0x8B5A48BA
		 * ID: 30 | Hash: 0x98FBD539
		 * ID: 31 | Hash: 0xD8CB0473
		 * ID: 32 | Hash: 0x5CBB4874
		 * ID: 33 | Hash: 0x2E9F93A9
		 * ID: 34 | Hash: 0xD93BEA86
		 * ID: 35 | Hash: 0x92109B7D
		 * ID: 36 | Hash: 0xB7EC9E4D
		 * ID: 37 | Hash: 0xCABDBB1D
		 * ID: 38 | Hash: 0xB3FD4A52
		 * ID: 39 | Hash: 0x370D94E5
		 * ID: 40 | Hash: 0xA0F7938F
		 * ID: 41 | Hash: 0xCBE1CE81
		 * ID: 42 | Hash: 0xC27F1271
		 * ID: 43 | Hash: 0x9E3258EB
		 * ID: 44 | Hash: 0x551CDA5B
		 * ID: 45 | Hash: 0xCB6D663C
		 * ID: 46 | Hash: 0x7DACE87F
		 * ID: 47 | Hash: 0xF9DE416F
		 * ID: 48 | Hash: 0x882E6E9E
		 * ID: 49 | Hash: 0x16B447E7
		 * ID: 50 | Hash: 0xBD867739
		 * ID: 51 | Hash: 0xA3A58604
		 * ID: 52 | Hash: 0x7E046BBC
		 * ID: 53 | Hash: 0xD95FDB98
		 * ID: 54 | Hash: 0x5842C0ED
		 * ID: 55 | Hash: 0x285FECC6
		 * ID: 56 | Hash: 0x9351AC43
		 * ID: 57 | Hash: 0x50032E75
		 * ID: 58 | Hash: 0xAE6D0D59
		 * ID: 59 | Hash: 0xD6351785
		 * ID: 60 | Hash: 0xD25D71BC
		 * ID: 61 | Hash: 0x1F7F6423
		 * ID: 62 | Hash: 0xE24C3AA6
		 * ID: 63 | Hash: 0xBFFDD2B7
		 */
		void SET_AUDIO_FLAG(const char* flagName, BOOL toggle);
		/**
		 * p1 is always 0 in the scripts
		 */
		BOOL PREPARE_SYNCHRONIZED_AUDIO_EVENT(const char* audioEvent, Any p1);
		BOOL PREPARE_SYNCHRONIZED_AUDIO_EVENT_FOR_SCENE(int sceneID, const char* audioEvent);
		BOOL PLAY_SYNCHRONIZED_AUDIO_EVENT(int sceneID);
		BOOL STOP_SYNCHRONIZED_AUDIO_EVENT(int sceneID);
		void INIT_SYNCH_SCENE_AUDIO_WITH_POSITION(const char* audioEvent, float x, float y, float z);
		void INIT_SYNCH_SCENE_AUDIO_WITH_ENTITY(const char* audioEvent, Entity entity);
		/**
		 * Needs to be called every frame.
		 * Audio mode to apply this frame: https://alloc8or.re/gta5/doc/enums/audSpecialEffectMode.txt
		 */
		void SET_AUDIO_SPECIAL_EFFECT_MODE(int mode);
		/**
		 * Found in the b617d scripts, duplicates removed:
		 * 
		 * AUDIO::SET_PORTAL_SETTINGS_OVERRIDE("V_CARSHOWROOM_PS_WINDOW_UNBROKEN", "V_CARSHOWROOM_PS_WINDOW_BROKEN");
		 * 
		 * AUDIO::SET_PORTAL_SETTINGS_OVERRIDE("V_CIA_PS_WINDOW_UNBROKEN", "V_CIA_PS_WINDOW_BROKEN");
		 * 
		 * AUDIO::SET_PORTAL_SETTINGS_OVERRIDE("V_DLC_HEIST_APARTMENT_DOOR_CLOSED", "V_DLC_HEIST_APARTMENT_DOOR_OPEN");
		 * 
		 * AUDIO::SET_PORTAL_SETTINGS_OVERRIDE("V_FINALEBANK_PS_VAULT_INTACT", "V_FINALEBANK_PS_VAULT_BLOWN");
		 * 
		 * AUDIO::SET_PORTAL_SETTINGS_OVERRIDE("V_MICHAEL_PS_BATHROOM_WITH_WINDOW", "V_MICHAEL_PS_BATHROOM_WITHOUT_WINDOW");
		 */
		void SET_PORTAL_SETTINGS_OVERRIDE(const char* p0, const char* p1);
		void SET_INDIVIDUAL_PORTAL_SETTINGS_OVERRIDE(Hash interiorNameHash, int roomIndex, int doorIndex, const char* newPortalSettingsName); // Missing in crossmap
		/**
		 * Found in the b617d scripts, duplicates removed:
		 * 
		 * AUDIO::REMOVE_PORTAL_SETTINGS_OVERRIDE("V_CARSHOWROOM_PS_WINDOW_UNBROKEN");
		 * AUDIO::REMOVE_PORTAL_SETTINGS_OVERRIDE("V_CIA_PS_WINDOW_UNBROKEN");
		 * AUDIO::REMOVE_PORTAL_SETTINGS_OVERRIDE("V_DLC_HEIST_APARTMENT_DOOR_CLOSED");
		 * AUDIO::REMOVE_PORTAL_SETTINGS_OVERRIDE("V_FINALEBANK_PS_VAULT_INTACT");
		 * AUDIO::REMOVE_PORTAL_SETTINGS_OVERRIDE("V_MICHAEL_PS_BATHROOM_WITH_WINDOW");
		 */
		void REMOVE_PORTAL_SETTINGS_OVERRIDE(const char* p0);
		void REMOVE_INDIVIDUAL_PORTAL_SETTINGS_OVERRIDE(Hash interiorNameHash, int roomIndex, int doorIndex); // Missing in crossmap
		void STOP_SMOKE_GRENADE_EXPLOSION_SOUNDS();
		int GET_MUSIC_VOL_SLIDER();
		void REQUEST_TENNIS_BANKS(Ped ped);
		void UNREQUEST_TENNIS_BANKS();
		void SET_SKIP_MINIGUN_SPIN_UP_AUDIO(BOOL p0);
		void STOP_CUTSCENE_AUDIO();
		BOOL HAS_LOADED_MP_DATA_SET();
		BOOL HAS_LOADED_SP_DATA_SET();
		int GET_VEHICLE_HORN_SOUND_INDEX(Vehicle vehicle);
		void SET_VEHICLE_HORN_SOUND_INDEX(Vehicle vehicle, int value);

	} // namespace AUDIO

	namespace BRAIN {

		/**
		 * BRAIN::ADD_SCRIPT_TO_RANDOM_PED("pb_prostitute", ${s_f_y_hooker_01}, 100, 0);
		 * 
		 * - Nacorpio
		 * 
		 * -----
		 * 
		 * Hardcoded to not work in Multiplayer.
		 */
		void ADD_SCRIPT_TO_RANDOM_PED(const char* name, Hash model, float p2, float p3);
		/**
		 * Registers a script for any object with a specific model hash.
		 * 
		 * BRAIN::REGISTER_OBJECT_SCRIPT_BRAIN("ob_telescope", ${prop_telescope_01}, 100, 4.0, -1, 9);
		 * 
		 * - Nacorpio
		 */
		void REGISTER_OBJECT_SCRIPT_BRAIN(const char* scriptName, Hash modelHash, int p2, float activationRange, int p4, int p5);
		BOOL IS_OBJECT_WITHIN_BRAIN_ACTIVATION_RANGE(Object object);
		void REGISTER_WORLD_POINT_SCRIPT_BRAIN(const char* scriptName, float activationRange, int p2);
		/**
		 * Gets whether the world point the calling script is registered to is within desired range of the player.
		 */
		BOOL IS_WORLD_POINT_WITHIN_BRAIN_ACTIVATION_RANGE();
		void ENABLE_SCRIPT_BRAIN_SET(int brainSet);
		void DISABLE_SCRIPT_BRAIN_SET(int brainSet);
		void REACTIVATE_ALL_WORLD_BRAINS_THAT_ARE_WAITING_TILL_OUT_OF_RANGE();
		void REACTIVATE_ALL_OBJECT_BRAINS_THAT_ARE_WAITING_TILL_OUT_OF_RANGE();
		/**
		 * Possible values:
		 * 
		 * act_cinema
		 * am_mp_carwash_launch
		 * am_mp_carwash_control
		 * am_mp_property_ext
		 * chop
		 * fairgroundHub
		 * launcher_BasejumpHeli
		 * launcher_BasejumpPack
		 * launcher_CarWash
		 * launcher_golf
		 * launcher_Hunting_Ambient
		 * launcher_MrsPhilips
		 * launcher_OffroadRacing
		 * launcher_pilotschool
		 * launcher_Racing
		 * launcher_rampage
		 * launcher_rampage
		 * launcher_range
		 * launcher_stunts
		 * launcher_stunts
		 * launcher_tennis
		 * launcher_Tonya
		 * launcher_Triathlon
		 * launcher_Yoga
		 * ob_mp_bed_low
		 * ob_mp_bed_med
		 */
		void REACTIVATE_NAMED_WORLD_BRAINS_WAITING_TILL_OUT_OF_RANGE(const char* scriptName);
		/**
		 * Looks like a cousin of above function _6D6840CEE8845831 as it was found among them. Must be similar
		 * 
		 * Here are possible values of argument -
		 * 
		 * "ob_tv"
		 * "launcher_Darts"
		 */
		void REACTIVATE_NAMED_OBJECT_BRAINS_WAITING_TILL_OUT_OF_RANGE(const char* scriptName);

	} // namespace BRAIN

	namespace CAM {

		/**
		 * ease - smooth transition between the camera's positions
		 * easeTime - Time in milliseconds for the transition to happen
		 * 
		 * If you have created a script (rendering) camera, and want to go back to the
		 * character (gameplay) camera, call this native with render set to 0.
		 * Setting ease to 1 will smooth the transition.
		 */
		void RENDER_SCRIPT_CAMS(BOOL render, BOOL ease, int easeTime, BOOL p3, BOOL p4, Any p5);
		/**
		 * This native makes the gameplay camera zoom into first person/third person with a special effect.
		 */
		void STOP_RENDERING_SCRIPT_CAMS_USING_CATCH_UP(BOOL render, float p1, int p2, Any p3);
		/**
		 * "DEFAULT_SCRIPTED_CAMERA"
		 * "DEFAULT_ANIMATED_CAMERA"
		 * "DEFAULT_SPLINE_CAMERA"
		 * "DEFAULT_SCRIPTED_FLY_CAMERA"
		 * "TIMED_SPLINE_CAMERA"
		 */
		Cam CREATE_CAM(const char* camName, BOOL p1);
		/**
		 * camName is always set to "DEFAULT_SCRIPTED_CAMERA" in Rockstar's scripts.
		 * ------------
		 * Camera names found in the b617d scripts:
		 * "DEFAULT_ANIMATED_CAMERA"
		 * "DEFAULT_SCRIPTED_CAMERA"
		 * "DEFAULT_SCRIPTED_FLY_CAMERA"
		 * "DEFAULT_SPLINE_CAMERA"
		 * ------------
		 * Side Note: It seems p8 is basically to represent what would be the bool p1 within CREATE_CAM native. As well as the p9 since it's always 2 in scripts seems to represent what would be the last param within SET_CAM_ROT native which normally would be 2.
		 */
		Cam CREATE_CAM_WITH_PARAMS(const char* camName, float posX, float posY, float posZ, float rotX, float rotY, float rotZ, float fov, BOOL p8, int p9);
		Cam CREATE_CAMERA(Hash camHash, BOOL p1);
		/**
		 * p9 uses 2 by default
		 */
		Cam CREATE_CAMERA_WITH_PARAMS(Hash camHash, float posX, float posY, float posZ, float rotX, float rotY, float rotZ, float fov, BOOL p8, Any p9);
		/**
		 * BOOL param indicates whether the cam should be destroyed if it belongs to the calling script.
		 */
		void DESTROY_CAM(Cam cam, BOOL bScriptHostCam);
		/**
		 * BOOL param indicates whether the cam should be destroyed if it belongs to the calling script.
		 */
		void DESTROY_ALL_CAMS(BOOL bScriptHostCam);
		/**
		 * Returns whether or not the passed camera handle exists.
		 */
		BOOL DOES_CAM_EXIST(Cam cam);
		/**
		 * Set camera as active/inactive.
		 */
		void SET_CAM_ACTIVE(Cam cam, BOOL active);
		/**
		 * Returns whether or not the passed camera handle is active.
		 */
		BOOL IS_CAM_ACTIVE(Cam cam);
		BOOL IS_CAM_RENDERING(Cam cam);
		Cam GET_RENDERING_CAM();
		Vector3 GET_CAM_COORD(Cam cam);
		/**
		 * The last parameter, as in other "ROT" methods, is usually 2.
		 */
		Vector3 GET_CAM_ROT(Cam cam, int rotationOrder);
		float GET_CAM_FOV(Cam cam);
		float GET_CAM_NEAR_CLIP(Cam cam);
		float GET_CAM_FAR_CLIP(Cam cam);
		float GET_CAM_NEAR_DOF(Cam cam);
		float GET_CAM_FAR_DOF(Cam cam);
		float GET_CAM_DOF_STRENGTH(Cam cam);
		void SET_CAM_PARAMS(Cam cam, float posX, float posY, float posZ, float rotX, float rotY, float rotZ, float fieldOfView, Any p8, int p9, int p10, int p11);
		/**
		 * Sets the position of the cam.
		 */
		void SET_CAM_COORD(Cam cam, float posX, float posY, float posZ);
		/**
		 * Sets the rotation of the cam.
		 * Last parameter unknown.
		 * 
		 * Last parameter seems to always be set to 2.
		 */
		void SET_CAM_ROT(Cam cam, float rotX, float rotY, float rotZ, int rotationOrder);
		/**
		 * Sets the field of view of the cam.
		 * ---------------------------------------------
		 * Min: 1.0f
		 * Max: 130.0f
		 */
		void SET_CAM_FOV(Cam cam, float fieldOfView);
		void SET_CAM_NEAR_CLIP(Cam cam, float nearClip);
		void SET_CAM_FAR_CLIP(Cam cam, float farClip);
		void FORCE_CAM_FAR_CLIP(Cam cam, float p1);
		void SET_CAM_MOTION_BLUR_STRENGTH(Cam cam, float strength);
		void SET_CAM_NEAR_DOF(Cam cam, float nearDOF);
		void SET_CAM_FAR_DOF(Cam cam, float farDOF);
		void SET_CAM_DOF_STRENGTH(Cam cam, float dofStrength);
		void SET_CAM_DOF_PLANES(Cam cam, float p1, float p2, float p3, float p4);
		void SET_CAM_USE_SHALLOW_DOF_MODE(Cam cam, BOOL toggle);
		void SET_USE_HI_DOF();
		/**
		 * Only used in R* Script fm_mission_controller_2020
		 */
		void SET_USE_HI_DOF_ON_SYNCED_SCENE_THIS_UPDATE();
		void SET_CAM_DOF_OVERRIDDEN_FOCUS_DISTANCE(Cam camera, float p1);
		void SET_CAM_DOF_OVERRIDDEN_FOCUS_DISTANCE_BLEND_LEVEL(Any p0, float p1);
		/**
		 * This native has its name defined inside its codE
		 * 
		 */
		void SET_CAM_DOF_FNUMBER_OF_LENS(Cam camera, float p1);
		/**
		 * Native name labeled within its code
		 */
		void SET_CAM_DOF_FOCAL_LENGTH_MULTIPLIER(Cam camera, float multiplier);
		/**
		 * This native has a name defined inside its code
		 */
		void SET_CAM_DOF_FOCUS_DISTANCE_BIAS(Cam camera, float p1);
		/**
		 * This native has a name defined inside its code
		 */
		void SET_CAM_DOF_MAX_NEAR_IN_FOCUS_DISTANCE(Cam camera, float p1);
		/**
		 * This native has a name defined inside its code
		 */
		void SET_CAM_DOF_MAX_NEAR_IN_FOCUS_DISTANCE_BLEND_LEVEL(Cam camera, float p1);
		/**
		 * This native has a name defined inside its code
		 */
		void SET_CAM_DOF_SHOULD_KEEP_LOOK_AT_TARGET_IN_FOCUS(Cam camera, BOOL state);
		/**
		 * Last param determines if its relative to the Entity
		 */
		void ATTACH_CAM_TO_ENTITY(Cam cam, Entity entity, float xOffset, float yOffset, float zOffset, BOOL isRelative);
		void ATTACH_CAM_TO_PED_BONE(Cam cam, Ped ped, int boneIndex, float x, float y, float z, BOOL heading);
		void HARD_ATTACH_CAM_TO_PED_BONE(Cam cam, Ped ped, int boneIndex, float p3, float p4, float p5, float p6, float p7, float p8, BOOL p9);
		/**
		 * Example from am_mp_drone script:
		 * 
		 * CAM::HARD_ATTACH_CAM_TO_ENTITY(Local_190.f_169, NETWORK::NET_TO_OBJ(Local_190.f_159), 0f, 0f, 180f, Var0, 1);
		 */
		void HARD_ATTACH_CAM_TO_ENTITY(Cam cam, Entity entity, float xRot, float yRot, float zRot, float xOffset, float yOffset, float zOffset, BOOL isRelative);
		/**
		 * This native works with vehicles only. Bone indexes are usually given by this native GET_ENTITY_BONE_INDEX_BY_NAME.
		 */
		void ATTACH_CAM_TO_VEHICLE_BONE(Cam cam, Vehicle vehicle, int boneIndex, BOOL relativeRotation, float rotX, float rotY, float rotZ, float offsetX, float offsetY, float offsetZ, BOOL fixedDirection);
		void DETACH_CAM(Cam cam);
		/**
		 * The native seems to only be called once.
		 * 
		 * The native is used as so,
		 * CAM::SET_CAM_INHERIT_ROLL_VEHICLE(l_544, getElem(2, &l_525, 4));
		 * In the exile1 script.
		 */
		void SET_CAM_INHERIT_ROLL_VEHICLE(Cam cam, BOOL p1);
		void POINT_CAM_AT_COORD(Cam cam, float x, float y, float z);
		/**
		 * p5 always seems to be 1 i.e TRUE
		 */
		void POINT_CAM_AT_ENTITY(Cam cam, Entity entity, float p2, float p3, float p4, BOOL p5);
		/**
		 * Parameters p0-p5 seems correct. The bool p6 is unknown, but through every X360 script it's always 1. Please correct p0-p5 if any prove to be wrong.
		 */
		void POINT_CAM_AT_PED_BONE(Cam cam, Ped ped, int boneIndex, float x, float y, float z, BOOL p6);
		void STOP_CAM_POINTING(Cam cam);
		/**
		 * Allows you to aim and shoot at the direction the camera is facing.
		 */
		void SET_CAM_AFFECTS_AIMING(Cam cam, BOOL toggle);
		/**
		 * Rotates the radar to match the camera's Z rotation
		 */
		void SET_CAM_CONTROLS_MINI_MAP_HEADING(Cam cam, BOOL toggle);
		/**
		 * When set to true shadows appear more smooth but less detailed.
		 * Set to false by default.
		 */
		void SET_CAM_IS_INSIDE_VEHICLE(Cam cam, BOOL toggle);
		void ALLOW_MOTION_BLUR_DECAY(Any p0, BOOL p1);
		/**
		 * NOTE: Debugging functions are not present in the retail version of the game.
		 */
		void SET_CAM_DEBUG_NAME(Cam camera, const char* name);
		Cam GET_DEBUG_CAM();
		/**
		 * I filled p1-p6 (the floats) as they are as other natives with 6 floats in a row are similar and I see no other method. So if a test from anyone proves them wrong please correct.
		 * 
		 * p7 (length) determines the length of the spline, affects camera path and duration of transition between previous node and this one
		 * 
		 * p8 big values ~100 will slow down the camera movement before reaching this node
		 * 
		 * p9 != 0 seems to override the rotation/pitch (bool?)
		 */
		void ADD_CAM_SPLINE_NODE(Cam camera, float x, float y, float z, float xRot, float yRot, float zRot, int length, int smoothingStyle, int rotationOrder);
		/**
		 * p0 is the spline camera to which the node is being added.
		 * p1 is the camera used to create the node.
		 * p3 is always 3 in scripts. It might be smoothing style or rotation order.
		 */
		void ADD_CAM_SPLINE_NODE_USING_CAMERA_FRAME(Cam cam, Cam cam2, int length, int p3);
		/**
		 * p0 is the spline camera to which the node is being added.
		 * p1 is the camera used to create the node.
		 * p3 is always 3 in scripts. It might be smoothing style or rotation order.
		 */
		void ADD_CAM_SPLINE_NODE_USING_CAMERA(Cam cam, Cam cam2, int length, int p3);
		/**
		 * p2 is always 2 in scripts. It might be smoothing style or rotation order.
		 */
		void ADD_CAM_SPLINE_NODE_USING_GAMEPLAY_FRAME(Cam cam, int length, int p2);
		void SET_CAM_SPLINE_PHASE(Cam cam, float p1);
		/**
		 * Can use this with SET_CAM_SPLINE_PHASE to set the float it this native returns.
		 * 
		 * (returns 1.0f when no nodes has been added, reached end of non existing spline)
		 */
		float GET_CAM_SPLINE_PHASE(Cam cam);
		/**
		 * I'm pretty sure the parameter is the camera as usual, but I am not certain so I'm going to leave it as is.
		 */
		float GET_CAM_SPLINE_NODE_PHASE(Cam cam);
		/**
		 * I named p1 as timeDuration as it is obvious. I'm assuming tho it is ran in ms(Milliseconds) as usual.
		 */
		void SET_CAM_SPLINE_DURATION(Cam cam, int timeDuration);
		void SET_CAM_SPLINE_SMOOTHING_STYLE(Cam cam, int smoothingStyle);
		int GET_CAM_SPLINE_NODE_INDEX(Cam cam);
		void SET_CAM_SPLINE_NODE_EASE(Cam cam, int easingFunction, int p2, float p3);
		void SET_CAM_SPLINE_NODE_VELOCITY_SCALE(Cam cam, int p1, float scale);
		void OVERRIDE_CAM_SPLINE_VELOCITY(Cam cam, int p1, float p2, float p3);
		/**
		 * Max value for p1 is 15.
		 */
		void OVERRIDE_CAM_SPLINE_MOTION_BLUR(Cam cam, int p1, float p2, float p3);
		void SET_CAM_SPLINE_NODE_EXTRA_FLAGS(Cam cam, int p1, int flags);
		BOOL IS_CAM_SPLINE_PAUSED(Cam cam);
		void _INTERPOLATE_CAM_WITH_PARAMS(Cam camera, float camPosX, float camPosY, float camPosZ, float camRotX, float camRotY, float camRotZ, float fov, int duration, int posCurveType, int rotCurveType, int rotOrder, int fovCurveType);
		void _ACTIVATE_CAM_WITH_INTERP_AND_FOV_CURVE(Cam camTo, Cam camFrom, int duration, int easeLocation, int easeRotation, int easeFov);
		/**
		 * Previous declaration void SET_CAM_ACTIVE_WITH_INTERP(Cam camTo, Cam camFrom, int duration, BOOL easeLocation, BOOL easeRotation) is completely wrong. The last two params are integers not BOOLs...
		 * 
		 */
		void SET_CAM_ACTIVE_WITH_INTERP(Cam camTo, Cam camFrom, int duration, int easeLocation, int easeRotation);
		BOOL IS_CAM_INTERPOLATING(Cam cam);
		/**
		 * Possible shake types (updated b617d):
		 * 
		 * DEATH_FAIL_IN_EFFECT_SHAKE
		 * DRUNK_SHAKE
		 * FAMILY5_DRUG_TRIP_SHAKE
		 * HAND_SHAKE
		 * JOLT_SHAKE
		 * LARGE_EXPLOSION_SHAKE
		 * MEDIUM_EXPLOSION_SHAKE
		 * SMALL_EXPLOSION_SHAKE
		 * ROAD_VIBRATION_SHAKE
		 * SKY_DIVING_SHAKE
		 * VIBRATE_SHAKE
		 * 
		 * Full list of cam shake types by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/camShakeTypesCompact.json
		 */
		void SHAKE_CAM(Cam cam, const char* type, float amplitude);
		/**
		 * Example from michael2 script.
		 * 
		 * CAM::ANIMATED_SHAKE_CAM(l_5069, "shake_cam_all@", "light", "", 1f);
		 */
		void ANIMATED_SHAKE_CAM(Cam cam, const char* p1, const char* p2, const char* p3, float amplitude);
		BOOL IS_CAM_SHAKING(Cam cam);
		void SET_CAM_SHAKE_AMPLITUDE(Cam cam, float amplitude);
		void STOP_CAM_SHAKING(Cam cam, BOOL p1);
		/**
		 * CAM::SHAKE_SCRIPT_GLOBAL("HAND_SHAKE", 0.2);
		 * 
		 * Full list of cam shake types by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/camShakeTypesCompact.json
		 */
		void SHAKE_SCRIPT_GLOBAL(const char* p0, float p1);
		/**
		 * CAM::ANIMATED_SHAKE_SCRIPT_GLOBAL("SHAKE_CAM_medium", "medium", "", 0.5f);
		 * 
		 * Full list of cam shake types by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/camShakeTypesCompact.json
		 */
		void ANIMATED_SHAKE_SCRIPT_GLOBAL(const char* p0, const char* p1, const char* p2, float p3);
		/**
		 * In drunk_controller.c4, sub_309
		 * if (CAM::IS_SCRIPT_GLOBAL_SHAKING()) {
		 * CAM::STOP_SCRIPT_GLOBAL_SHAKING(0);
		 * }
		 */
		BOOL IS_SCRIPT_GLOBAL_SHAKING();
		/**
		 * In drunk_controller.c4, sub_309
		 * if (CAM::IS_SCRIPT_GLOBAL_SHAKING()) {
		 * CAM::STOP_SCRIPT_GLOBAL_SHAKING(0);
		 * }
		 */
		void STOP_SCRIPT_GLOBAL_SHAKING(BOOL p0);
		/**
		 * p1: 0..16
		 */
		void TRIGGER_VEHICLE_PART_BROKEN_CAMERA_SHAKE(Vehicle vehicle, int p1, float p2);
		/**
		 * Atleast one time in a script for the zRot Rockstar uses GET_ENTITY_HEADING to help fill the parameter.
		 * 
		 * p9 is unknown at this time.
		 * p10 throughout all the X360 Scripts is always 2.
		 * 
		 * Full list of animation dictionaries and anims by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/animDictsCompact.json
		 */
		BOOL PLAY_CAM_ANIM(Cam cam, const char* animName, const char* animDictionary, float x, float y, float z, float xRot, float yRot, float zRot, BOOL p9, int p10);
		BOOL IS_CAM_PLAYING_ANIM(Cam cam, const char* animName, const char* animDictionary);
		void SET_CAM_ANIM_CURRENT_PHASE(Cam cam, float phase);
		float GET_CAM_ANIM_CURRENT_PHASE(Cam cam);
		/**
		 * Examples:
		 * 
		 * CAM::PLAY_SYNCHRONIZED_CAM_ANIM(l_2734, NETWORK::NETWORK_GET_LOCAL_SCENE_FROM_NETWORK_ID(l_2739), "PLAYER_EXIT_L_CAM", "mp_doorbell");
		 * 
		 * CAM::PLAY_SYNCHRONIZED_CAM_ANIM(l_F0D[7/*1* /], l_F4D[15/*1* /], "ah3b_attackheli_cam2", "missheistfbi3b_helicrash");
		 */
		BOOL PLAY_SYNCHRONIZED_CAM_ANIM(Any p0, Any p1, const char* animName, const char* animDictionary);
		void SET_FLY_CAM_HORIZONTAL_RESPONSE(Cam cam, float p1, float p2, float p3);
		void SET_FLY_CAM_VERTICAL_RESPONSE(Cam cam, float p1, float p2, float p3);
		void SET_FLY_CAM_MAX_HEIGHT(Cam cam, float height);
		void SET_FLY_CAM_COORD_AND_CONSTRAIN(Cam cam, float x, float y, float z);
		void SET_FLY_CAM_VERTICAL_CONTROLS_THIS_UPDATE(Cam cam);
		BOOL WAS_FLY_CAM_CONSTRAINED_ON_PREVIOUS_UDPATE(Cam cam);
		BOOL IS_SCREEN_FADED_OUT();
		BOOL IS_SCREEN_FADED_IN();
		BOOL IS_SCREEN_FADING_OUT();
		BOOL IS_SCREEN_FADING_IN();
		/**
		 * Fades the screen in.
		 * 
		 * duration: The time the fade should take, in milliseconds.
		 */
		void DO_SCREEN_FADE_IN(int duration);
		/**
		 * Fades the screen out.
		 * 
		 * duration: The time the fade should take, in milliseconds.
		 */
		void DO_SCREEN_FADE_OUT(int duration);
		void SET_WIDESCREEN_BORDERS(BOOL p0, int p1);
		BOOL ARE_WIDESCREEN_BORDERS_ACTIVE();
		Vector3 GET_GAMEPLAY_CAM_COORD();
		/**
		 * p0 dosen't seem to change much, I tried it with 0, 1, 2:
		 * 0-Pitch(X): -70.000092
		 * 0-Roll(Y): -0.000001
		 * 0-Yaw(Z): -43.886459
		 * 1-Pitch(X): -70.000092
		 * 1-Roll(Y): -0.000001
		 * 1-Yaw(Z): -43.886463
		 * 2-Pitch(X): -70.000092
		 * 2-Roll(Y): -0.000002
		 * 2-Yaw(Z): -43.886467
		 */
		Vector3 GET_GAMEPLAY_CAM_ROT(int rotationOrder);
		float GET_GAMEPLAY_CAM_FOV();
		/**
		 * some camera effect that is used in the drunk-cheat, and turned off (by setting it to 0.0) along with the shaking effects once the drunk cheat is disabled.
		 */
		void SET_GAMEPLAY_CAM_MOTION_BLUR_SCALING_THIS_UPDATE(float p0);
		/**
		 * some camera effect that is (also) used in the drunk-cheat, and turned off (by setting it to 0.0) along with the shaking effects once the drunk cheat is disabled.
		 */
		void SET_GAMEPLAY_CAM_MAX_MOTION_BLUR_STRENGTH_THIS_UPDATE(float p0);
		float GET_GAMEPLAY_CAM_RELATIVE_HEADING();
		/**
		 * Sets the camera position relative to heading in float from -360 to +360.
		 * 
		 * Heading is alwyas 0 in aiming camera.
		 */
		void SET_GAMEPLAY_CAM_RELATIVE_HEADING(float heading);
		float GET_GAMEPLAY_CAM_RELATIVE_PITCH();
		/**
		 * This native sets the camera's pitch (rotation on the x-axis).
		 */
		void SET_GAMEPLAY_CAM_RELATIVE_PITCH(float angle, float scalingFactor);
		void RESET_GAMEPLAY_CAM_FULL_ATTACH_PARENT_TRANSFORM_TIMER();
		void FORCE_CAMERA_RELATIVE_HEADING_AND_PITCH(float roll, float pitch, float yaw);
		void FORCE_BONNET_CAMERA_RELATIVE_HEADING_AND_PITCH(float p0, float p1);
		/**
		 * Does nothing
		 */
		void SET_FIRST_PERSON_SHOOTER_CAMERA_HEADING(float yaw);
		void SET_FIRST_PERSON_SHOOTER_CAMERA_PITCH(float pitch);
		void SET_SCRIPTED_CAMERA_IS_FIRST_PERSON_THIS_FRAME(BOOL p0);
		/**
		 * Possible shake types (updated b617d):
		 * 
		 * DEATH_FAIL_IN_EFFECT_SHAKE
		 * DRUNK_SHAKE
		 * FAMILY5_DRUG_TRIP_SHAKE
		 * HAND_SHAKE
		 * JOLT_SHAKE
		 * LARGE_EXPLOSION_SHAKE
		 * MEDIUM_EXPLOSION_SHAKE
		 * SMALL_EXPLOSION_SHAKE
		 * ROAD_VIBRATION_SHAKE
		 * SKY_DIVING_SHAKE
		 * VIBRATE_SHAKE
		 * 
		 * Full list of cam shake types by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/camShakeTypesCompact.json
		 */
		void SHAKE_GAMEPLAY_CAM(const char* shakeName, float intensity);
		BOOL IS_GAMEPLAY_CAM_SHAKING();
		/**
		 * Sets the amplitude for the gameplay (i.e. 3rd or 1st) camera to shake. Used in script "drunk_controller.ysc.c4" to simulate making the player drunk.
		 */
		void SET_GAMEPLAY_CAM_SHAKE_AMPLITUDE(float amplitude);
		void STOP_GAMEPLAY_CAM_SHAKING(BOOL p0);
		/**
		 * Forces gameplay cam to specified ped as if you were the ped or spectating it
		 */
		void SET_GAMEPLAY_CAM_FOLLOW_PED_THIS_UPDATE(Ped ped);
		/**
		 * Examples when this function will return 0 are:
		 * - During busted screen.
		 * - When player is coming out from a hospital.
		 * - When player is coming out from a police station.
		 * - When player is buying gun from AmmuNation.
		 */
		BOOL IS_GAMEPLAY_CAM_RENDERING();
		BOOL IS_INTERPOLATING_FROM_SCRIPT_CAMS();
		BOOL IS_INTERPOLATING_TO_SCRIPT_CAMS();
		void SET_GAMEPLAY_CAM_ALTITUDE_FOV_SCALING_STATE(BOOL p0);
		/**
		 * Shows the crosshair even if it wouldn't show normally. Only works for one frame, so make sure to call it repeatedly.
		 */
		void DISABLE_GAMEPLAY_CAM_ALTITUDE_FOV_SCALING_THIS_UPDATE();
		BOOL IS_GAMEPLAY_CAM_LOOKING_BEHIND();
		void SET_GAMEPLAY_CAM_IGNORE_ENTITY_COLLISION_THIS_UPDATE(Entity entity);
		void DISABLE_CAM_COLLISION_FOR_OBJECT(Entity entity);
		void BYPASS_CAMERA_COLLISION_BUOYANCY_TEST_THIS_UPDATE();
		void SET_GAMEPLAY_CAM_ENTITY_TO_LIMIT_FOCUS_OVER_BOUNDING_SPHERE_THIS_UPDATE(Entity entity);
		/**
		 * Sets some flag on cinematic camera
		 */
		void DISABLE_FIRST_PERSON_CAMERA_WATER_CLIPPING_TEST_THIS_UPDATE();
		void SET_FOLLOW_CAM_IGNORE_ATTACH_PARENT_MOVEMENT_THIS_UPDATE();
		BOOL IS_SPHERE_VISIBLE(float x, float y, float z, float radius);
		BOOL IS_FOLLOW_PED_CAM_ACTIVE();
		/**
		 * From the scripts:
		 * 
		 * CAM::SET_FOLLOW_PED_CAM_THIS_UPDATE("FOLLOW_PED_ATTACHED_TO_ROPE_CAMERA", 0);
		 * CAM::SET_FOLLOW_PED_CAM_THIS_UPDATE("FOLLOW_PED_ON_EXILE1_LADDER_CAMERA", 1500);
		 * CAM::SET_FOLLOW_PED_CAM_THIS_UPDATE("FOLLOW_PED_SKY_DIVING_CAMERA", 0);
		 * CAM::SET_FOLLOW_PED_CAM_THIS_UPDATE("FOLLOW_PED_SKY_DIVING_CAMERA", 3000);
		 * CAM::SET_FOLLOW_PED_CAM_THIS_UPDATE("FOLLOW_PED_SKY_DIVING_FAMILY5_CAMERA", 0);
		 * CAM::SET_FOLLOW_PED_CAM_THIS_UPDATE("FOLLOW_PED_SKY_DIVING_CAMERA", 0);
		 */
		BOOL SET_FOLLOW_PED_CAM_THIS_UPDATE(const char* camName, int p1);
		void USE_SCRIPT_CAM_FOR_AMBIENT_POPULATION_ORIGIN_THIS_FRAME(BOOL p0, BOOL p1);
		void SET_FOLLOW_PED_CAM_LADDER_ALIGN_THIS_UPDATE();
		/**
		 * minimum: Degrees between -180f and 180f.
		 * maximum: Degrees between -180f and 180f.
		 * 
		 * Clamps the gameplay camera's current yaw.
		 * 
		 * Eg. SET_THIRD_PERSON_CAM_RELATIVE_HEADING_LIMITS_THIS_UPDATE(0.0f, 0.0f) will set the horizontal angle directly behind the player.
		 */
		void SET_THIRD_PERSON_CAM_RELATIVE_HEADING_LIMITS_THIS_UPDATE(float minimum, float maximum);
		/**
		 * minimum: Degrees between -90f and 90f.
		 * maximum: Degrees between -90f and 90f.
		 * 
		 * Clamps the gameplay camera's current pitch.
		 * 
		 * Eg. SET_THIRD_PERSON_CAM_RELATIVE_PITCH_LIMITS_THIS_UPDATE(0.0f, 0.0f) will set the vertical angle directly behind the player.
		 */
		void SET_THIRD_PERSON_CAM_RELATIVE_PITCH_LIMITS_THIS_UPDATE(float minimum, float maximum);
		/**
		 * Seems to animate the gameplay camera zoom.
		 * 
		 * Eg. SET_THIRD_PERSON_CAM_ORBIT_DISTANCE_LIMITS_THIS_UPDATE(1f, 1000f);
		 * will animate the camera zooming in from 1000 meters away.
		 * 
		 * Game scripts use it like this:
		 * 
		 * // Setting this to 1 prevents V key from changing zoom
		 * PLAYER::SET_PLAYER_FORCED_ZOOM(PLAYER::PLAYER_ID(), 1);
		 * 
		 * // These restrict how far you can move cam up/down left/right
		 * CAM::SET_THIRD_PERSON_CAM_RELATIVE_HEADING_LIMITS_THIS_UPDATE(-20f, 50f);
		 * CAM::SET_THIRD_PERSON_CAM_RELATIVE_PITCH_LIMITS_THIS_UPDATE(-60f, 0f);
		 * 
		 * CAM::SET_THIRD_PERSON_CAM_ORBIT_DISTANCE_LIMITS_THIS_UPDATE(1f, 1f);
		 */
		void SET_THIRD_PERSON_CAM_ORBIT_DISTANCE_LIMITS_THIS_UPDATE(float p0, float distance);
		float _GET_THIRD_PERSON_CAM_MIN_ORBIT_DISTANCE_SPRING();
		float _GET_THIRD_PERSON_CAM_MAX_ORBIT_DISTANCE_SPRING();
		/**
		 * Forces gameplay cam to specified vehicle as if you were in it
		 */
		void SET_IN_VEHICLE_CAM_STATE_THIS_UPDATE(Vehicle p0, int p1);
		/**
		 * Disables first person camera for the current frame.
		 * 
		 * Found in decompiled scripts:
		 * GRAPHICS::DRAW_DEBUG_TEXT_2D("Disabling First Person Cam", 0.5, 0.8, 0.0, 0, 0, 255, 255);
		 * CAM::DISABLE_ON_FOOT_FIRST_PERSON_VIEW_THIS_UPDATE();
		 */
		void DISABLE_ON_FOOT_FIRST_PERSON_VIEW_THIS_UPDATE();
		void DISABLE_FIRST_PERSON_FLASH_EFFECT_THIS_UPDATE();
		void BLOCK_FIRST_PERSON_ORIENTATION_RESET_THIS_UPDATE();
		int GET_FOLLOW_PED_CAM_ZOOM_LEVEL();
		/**
		 * See viewmode enum in CAM.GET_FOLLOW_VEHICLE_CAM_VIEW_MODE for return value
		 */
		int GET_FOLLOW_PED_CAM_VIEW_MODE();
		/**
		 * Sets the type of Player camera:
		 * 
		 * 0 - Third Person Close
		 * 1 - Third Person Mid
		 * 2 - Third Person Far
		 * 4 - First Person
		 */
		void SET_FOLLOW_PED_CAM_VIEW_MODE(int viewMode);
		BOOL IS_FOLLOW_VEHICLE_CAM_ACTIVE();
		void SET_FOLLOW_VEHICLE_CAM_HIGH_ANGLE_MODE_THIS_UPDATE(BOOL p0);
		void SET_FOLLOW_VEHICLE_CAM_HIGH_ANGLE_MODE_EVERY_UPDATE(BOOL p0, BOOL p1);
		BOOL SET_TABLE_GAMES_CAMERA_THIS_UPDATE(Hash hash);
		int GET_FOLLOW_VEHICLE_CAM_ZOOM_LEVEL();
		void SET_FOLLOW_VEHICLE_CAM_ZOOM_LEVEL(int zoomLevel);
		/**
		 * Returns the type of camera:
		 * 
		 * enum _viewmode //0xA11D7CA8
		 * {
		 * THIRD_PERSON_NEAR = 0,
		 * THIRD_PERSON_MEDIUM = 1,
		 * THIRD_PERSON_FAR = 2,
		 * CINEMATIC = 3,
		 * FIRST_PERSON = 4
		 * };
		 */
		int GET_FOLLOW_VEHICLE_CAM_VIEW_MODE();
		/**
		 * Sets the type of Player camera in vehicles:
		 * viewmode: see CAM.GET_FOLLOW_VEHICLE_CAM_VIEW_MODE
		 */
		void SET_FOLLOW_VEHICLE_CAM_VIEW_MODE(int viewMode);
		/**
		 * context: see _GET_CAM_ACTIVE_VIEW_MODE_CONTEXT
		 */
		int GET_CAM_VIEW_MODE_FOR_CONTEXT(int context);
		/**
		 * context: see _GET_CAM_ACTIVE_VIEW_MODE_CONTEXT, viewmode: see CAM.GET_FOLLOW_VEHICLE_CAM_VIEW_MODE
		 */
		void SET_CAM_VIEW_MODE_FOR_CONTEXT(int context, int viewMode);
		/**
		 * enum Context
		 * {
		 * ON_FOOT,
		 * IN_VEHICLE,
		 * ON_BIKE,
		 * IN_BOAT,
		 * IN_AIRCRAFT,
		 * IN_SUBMARINE,
		 * IN_HELI,
		 * IN_TURRET
		 * };
		 */
		int GET_CAM_ACTIVE_VIEW_MODE_CONTEXT();
		void USE_VEHICLE_CAM_STUNT_SETTINGS_THIS_UPDATE();
		/**
		 * Sets gameplay camera to hash
		 */
		void USE_DEDICATED_STUNT_CAMERA_THIS_UPDATE(const char* camName);
		void FORCE_VEHICLE_CAM_STUNT_SETTINGS_THIS_UPDATE();
		void SET_FOLLOW_VEHICLE_CAM_SEAT_THIS_UPDATE(int seatIndex);
		BOOL IS_AIM_CAM_ACTIVE();
		BOOL IS_AIM_CAM_ACTIVE_IN_ACCURATE_MODE();
		BOOL IS_FIRST_PERSON_AIM_CAM_ACTIVE();
		void DISABLE_AIM_CAM_THIS_UPDATE();
		float GET_FIRST_PERSON_AIM_CAM_ZOOM_FACTOR();
		void SET_FIRST_PERSON_AIM_CAM_ZOOM_FACTOR(float zoomFactor);
		void SET_FIRST_PERSON_AIM_CAM_ZOOM_FACTOR_LIMITS_THIS_UPDATE(float p0, float p1);
		void SET_FIRST_PERSON_AIM_CAM_RELATIVE_HEADING_LIMITS_THIS_UPDATE(float p0, float p1);
		void SET_FIRST_PERSON_AIM_CAM_RELATIVE_PITCH_LIMITS_THIS_UPDATE(float p0, float p1);
		void SET_FIRST_PERSON_AIM_CAM_NEAR_CLIP_THIS_UPDATE(float p0);
		void SET_THIRD_PERSON_AIM_CAM_NEAR_CLIP_THIS_UPDATE(float p0);
		void SET_ALLOW_CUSTOM_VEHICLE_DRIVE_BY_CAM_THIS_UPDATE(BOOL p0);
		void FORCE_TIGHTSPACE_CUSTOM_FRAMING_THIS_UPDATE();
		Vector3 GET_FINAL_RENDERED_CAM_COORD();
		/**
		 * p0 seems to consistently be 2 across scripts
		 * 
		 * Function is called faily often by CAM::CREATE_CAM_WITH_PARAMS
		 */
		Vector3 GET_FINAL_RENDERED_CAM_ROT(int rotationOrder);
		Vector3 GET_FINAL_RENDERED_REMOTE_PLAYER_CAM_ROT(Player player, int rotationOrder);
		/**
		 * Gets some camera fov
		 */
		float GET_FINAL_RENDERED_CAM_FOV();
		float GET_FINAL_RENDERED_REMOTE_PLAYER_CAM_FOV(Player player);
		float GET_FINAL_RENDERED_CAM_NEAR_CLIP();
		float GET_FINAL_RENDERED_CAM_FAR_CLIP();
		float GET_FINAL_RENDERED_CAM_NEAR_DOF();
		float GET_FINAL_RENDERED_CAM_FAR_DOF();
		float GET_FINAL_RENDERED_CAM_MOTION_BLUR_STRENGTH();
		void SET_GAMEPLAY_COORD_HINT(float x, float y, float z, int duration, int blendOutDuration, int blendInDuration, int p6);
		void SET_GAMEPLAY_PED_HINT(Ped ped, float x1, float y1, float z1, BOOL p4, int duration, int blendOutDuration, int blendInDuration);
		/**
		 * Focuses the camera on the specified vehicle.
		 */
		void SET_GAMEPLAY_VEHICLE_HINT(Vehicle vehicle, float offsetX, float offsetY, float offsetZ, BOOL p4, int time, int easeInTime, int easeOutTime);
		void SET_GAMEPLAY_OBJECT_HINT(Object object, float xOffset, float yOffset, float zOffset, BOOL p4, int time, int easeInTime, int easeOutTime);
		/**
		 * p8 could be some sort of flag. Scripts use:
		 * -244429742
		 * 0
		 * 1726668277
		 * 1844968929
		 */
		void SET_GAMEPLAY_ENTITY_HINT(Entity entity, float xOffset, float yOffset, float zOffset, BOOL p4, int time, int easeInTime, int easeOutTime, int p8);
		BOOL IS_GAMEPLAY_HINT_ACTIVE();
		void STOP_GAMEPLAY_HINT(BOOL p0);
		/**
		 * This native does absolutely nothing, just a nullsub
		 */
		void STOP_GAMEPLAY_HINT_BEING_CANCELLED_THIS_UPDATE(BOOL p0);
		void STOP_CODE_GAMEPLAY_HINT(BOOL p0);
		BOOL IS_CODE_GAMEPLAY_HINT_ACTIVE();
		void SET_GAMEPLAY_HINT_FOV(float FOV);
		void SET_GAMEPLAY_HINT_FOLLOW_DISTANCE_SCALAR(float value);
		void SET_GAMEPLAY_HINT_BASE_ORBIT_PITCH_OFFSET(float value);
		void SET_GAMEPLAY_HINT_CAMERA_RELATIVE_SIDE_OFFSET(float xOffset);
		void SET_GAMEPLAY_HINT_CAMERA_RELATIVE_VERTICAL_OFFSET(float yOffset);
		void SET_GAMEPLAY_HINT_CAMERA_BLEND_TO_FOLLOW_PED_MEDIUM_VIEW_MODE(BOOL toggle);
		void SET_CINEMATIC_BUTTON_ACTIVE(BOOL p0);
		BOOL IS_CINEMATIC_CAM_RENDERING();
		/**
		 * p0 argument found in the b617d scripts: "DRUNK_SHAKE"
		 * 
		 * Full list of cam shake types by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/camShakeTypesCompact.json
		 */
		void SHAKE_CINEMATIC_CAM(const char* shakeType, float amount);
		BOOL IS_CINEMATIC_CAM_SHAKING();
		void SET_CINEMATIC_CAM_SHAKE_AMPLITUDE(float p0);
		void STOP_CINEMATIC_CAM_SHAKING(BOOL p0);
		void DISABLE_CINEMATIC_BONNET_CAMERA_THIS_UPDATE();
		void DISABLE_CINEMATIC_VEHICLE_IDLE_MODE_THIS_UPDATE();
		/**
		 * Resets the vehicle idle camera timer. Calling this in a loop will disable the idle camera.
		 */
		void INVALIDATE_CINEMATIC_VEHICLE_IDLE_MODE();
		/**
		 * Resets the idle camera timer. Calling that in a loop once every few seconds is enough to disable the idle cinematic camera.
		 */
		void INVALIDATE_IDLE_CAM();
		BOOL IS_CINEMATIC_IDLE_CAM_RENDERING();
		BOOL IS_CINEMATIC_FIRST_PERSON_VEHICLE_INTERIOR_CAM_RENDERING();
		/**
		 * hash is always JOAAT("CAMERA_MAN_SHOT") in decompiled scripts
		 */
		void CREATE_CINEMATIC_SHOT(Hash p0, int time, BOOL p2, Entity entity);
		/**
		 * Hash is always JOAAT("CAMERA_MAN_SHOT") in decompiled scripts
		 */
		BOOL IS_CINEMATIC_SHOT_ACTIVE(Hash p0);
		/**
		 * Only used once in carsteal3 with p0 set to -1096069633 (CAMERA_MAN_SHOT)
		 */
		void STOP_CINEMATIC_SHOT(Hash p0);
		void FORCE_CINEMATIC_RENDERING_THIS_UPDATE(BOOL toggle);
		void SET_CINEMATIC_NEWS_CHANNEL_ACTIVE_THIS_UPDATE();
		/**
		 * Toggles the vehicle cinematic cam; requires the player ped to be in a vehicle to work.
		 */
		void SET_CINEMATIC_MODE_ACTIVE(BOOL toggle);
		BOOL IS_IN_VEHICLE_MOBILE_PHONE_CAMERA_RENDERING();
		BOOL DISABLE_CINEMATIC_SLOW_MO_THIS_UPDATE();
		BOOL IS_BONNET_CINEMATIC_CAM_RENDERING();
		/**
		 * Tests some cinematic camera flags
		 */
		BOOL IS_CINEMATIC_CAM_INPUT_ACTIVE();
		void IGNORE_MENU_PREFERENCE_FOR_BONNET_CAMERA_THIS_UPDATE();
		void BYPASS_CUTSCENE_CAM_RENDERING_THIS_UPDATE();
		void STOP_CUTSCENE_CAM_SHAKING(Any p0);
		/**
		 * Hardcoded to only work in multiplayer.
		 */
		void SET_CUTSCENE_CAM_FAR_CLIP_THIS_UPDATE(float p0);
		Ped GET_FOCUS_PED_ON_SCREEN(float p0, int p1, float p2, float p3, float p4, float p5, float p6, int p7, int p8);
		void DISABLE_NEAR_CLIP_SCAN_THIS_UPDATE();
		/**
		 * if p0 is 0, effect is cancelled
		 * 
		 * if p0 is 1, effect zooms in, gradually tilts cam clockwise apx 30 degrees, wobbles slowly. Motion blur is active until cancelled.
		 * 
		 * if p0 is 2, effect immediately tilts cam clockwise apx 30 degrees, begins to wobble slowly, then gradually tilts cam back to normal. The wobbling will continue until the effect is cancelled.
		 */
		void SET_CAM_DEATH_FAIL_EFFECT_STATE(int p0);
		void SET_FIRST_PERSON_FLASH_EFFECT_TYPE(Any p0);
		/**
		 * From b617 scripts:
		 * 
		 * CAM::SET_FIRST_PERSON_FLASH_EFFECT_VEHICLE_MODEL_NAME("DINGHY");
		 * CAM::SET_FIRST_PERSON_FLASH_EFFECT_VEHICLE_MODEL_NAME("ISSI2");
		 * CAM::SET_FIRST_PERSON_FLASH_EFFECT_VEHICLE_MODEL_NAME("SPEEDO");
		 */
		void SET_FIRST_PERSON_FLASH_EFFECT_VEHICLE_MODEL_NAME(const char* vehicleName);
		void SET_FIRST_PERSON_FLASH_EFFECT_VEHICLE_MODEL_HASH(Hash vehicleModel);
		BOOL IS_ALLOWED_INDEPENDENT_CAMERA_MODES();
		void CAMERA_PREVENT_COLLISION_SETTINGS_FOR_TRIPLEHEAD_IN_INTERIORS_THIS_UPDATE();
		float REPLAY_GET_MAX_DISTANCE_ALLOWED_FROM_PLAYER();

	} // namespace CAM

	namespace CLOCK {

		/**
		 * SET_CLOCK_TIME(12, 34, 56);
		 */
		void SET_CLOCK_TIME(int hour, int minute, int second);
		void PAUSE_CLOCK(BOOL toggle);
		void ADVANCE_CLOCK_TIME_TO(int hour, int minute, int second);
		void ADD_TO_CLOCK_TIME(int hours, int minutes, int seconds);
		/**
		 * Gets the current ingame hour, expressed without zeros. (09:34 will be represented as 9)
		 */
		int GET_CLOCK_HOURS();
		/**
		 * Gets the current ingame clock minute.
		 */
		int GET_CLOCK_MINUTES();
		/**
		 * Gets the current ingame clock second. Note that ingame clock seconds change really fast since a day in GTA is only 48 minutes in real life.
		 */
		int GET_CLOCK_SECONDS();
		void SET_CLOCK_DATE(int day, int month, int year);
		/**
		 * Gets the current day of the week.
		 * 
		 * 0: Sunday
		 * 1: Monday
		 * 2: Tuesday
		 * 3: Wednesday
		 * 4: Thursday
		 * 5: Friday
		 * 6: Saturday
		 */
		int GET_CLOCK_DAY_OF_WEEK();
		int GET_CLOCK_DAY_OF_MONTH();
		int GET_CLOCK_MONTH();
		int GET_CLOCK_YEAR();
		int GET_MILLISECONDS_PER_GAME_MINUTE();
		/**
		 * Gets system time as year, month, day, hour, minute and second.
		 * 
		 * Example usage:
		 * 
		 * int year;
		 * int month;
		 * int day;
		 * int hour;
		 * int minute;
		 * int second;
		 * 
		 * TIME::GET_POSIX_TIME(&year, &month, &day, &hour, &minute, &second);
		 * 
		 */
		void GET_POSIX_TIME(int* year, int* month, int* day, int* hour, int* minute, int* second);
		/**
		 * Gets current UTC time
		 */
		void GET_UTC_TIME(int* year, int* month, int* day, int* hour, int* minute, int* second);
		/**
		 * Gets local system time as year, month, day, hour, minute and second.
		 * 
		 * Example usage:
		 * 
		 * int year;
		 * int month;
		 * int day;
		 * int hour;
		 * int minute;
		 * int second;
		 * or use std::tm struct
		 * 
		 * TIME::GET_LOCAL_TIME(&year, &month, &day, &hour, &minute, &second);
		 * 
		 */
		void GET_LOCAL_TIME(int* year, int* month, int* day, int* hour, int* minute, int* second);

	} // namespace CLOCK

	namespace CUTSCENE {

		/**
		 * flags: Usually 8
		 * Full list of cutscene names by DurtyFree https://github.com/DurtyFree/gta-v-data-dumps/blob/master/cutsceneNames.json
		 */
		void REQUEST_CUTSCENE(const char* cutsceneName, int flags);
		/**
		 * flags: Usually 8
		 * 
		 * playbackFlags: Which scenes should be played.
		 * Example: 0x105 (bit 0, 2 and 8 set) will enable scene 1, 3 and 9.
		 * Full list of cutscene names by DurtyFree https://github.com/DurtyFree/gta-v-data-dumps/blob/master/cutsceneNames.json
		 */
		void REQUEST_CUTSCENE_WITH_PLAYBACK_LIST(const char* cutsceneName, int playbackFlags, int flags);
		void REMOVE_CUTSCENE();
		BOOL HAS_CUTSCENE_LOADED();
		/**
		 * Full list of cutscene names by DurtyFree https://github.com/DurtyFree/gta-v-data-dumps/blob/master/cutsceneNames.json
		 */
		BOOL HAS_THIS_CUTSCENE_LOADED(const char* cutsceneName);
		/**
		 * Sets the cutscene's owning thread ID.
		 */
		void SET_SCRIPT_CAN_START_CUTSCENE(int threadId);
		BOOL CAN_REQUEST_ASSETS_FOR_CUTSCENE_ENTITY();
		BOOL IS_CUTSCENE_PLAYBACK_FLAG_SET(int flag);
		void SET_CUTSCENE_ENTITY_STREAMING_FLAGS(const char* cutsceneEntName, int p1, int p2);
		/**
		 * Simply loads the cutscene and doesn't do extra stuff that REQUEST_CUTSCENE does.
		 * Full list of cutscene names by DurtyFree https://github.com/DurtyFree/gta-v-data-dumps/blob/master/cutsceneNames.json
		 */
		void REQUEST_CUT_FILE(const char* cutsceneName);
		/**
		 * Simply checks if the cutscene has loaded and doesn't check via CutSceneManager as opposed to HAS_[THIS]_CUTSCENE_LOADED.
		 * Full list of cutscene names by DurtyFree https://github.com/DurtyFree/gta-v-data-dumps/blob/master/cutsceneNames.json
		 */
		BOOL HAS_CUT_FILE_LOADED(const char* cutsceneName);
		/**
		 * Simply unloads the cutscene and doesn't do extra stuff that REMOVE_CUTSCENE does.
		 * Full list of cutscene names by DurtyFree https://github.com/DurtyFree/gta-v-data-dumps/blob/master/cutsceneNames.json
		 */
		void REMOVE_CUT_FILE(const char* cutsceneName);
		void GET_CUT_FILE_OFFSET(const char* cutsceneName, int index); // Missing in crossmap
		/**
		 * Full list of cutscene names by DurtyFree https://github.com/DurtyFree/gta-v-data-dumps/blob/master/cutsceneNames.json
		 */
		int GET_CUT_FILE_CONCAT_COUNT(const char* cutsceneName);
		/**
		 * flags: Usually 0.
		 */
		void START_CUTSCENE(int flags);
		/**
		 * flags: Usually 0.
		 */
		void START_CUTSCENE_AT_COORDS(float x, float y, float z, int flags);
		void STOP_CUTSCENE(BOOL p0);
		void STOP_CUTSCENE_IMMEDIATELY();
		/**
		 * p3 could be heading. Needs more research.
		 */
		void SET_CUTSCENE_ORIGIN(float x, float y, float z, float p3, int p4);
		void SET_CUTSCENE_ORIGIN_AND_ORIENTATION(float x1, float y1, float z1, float x2, float y2, float z2, int p6);
		int GET_CUTSCENE_TIME();
		int GET_CUTSCENE_PLAY_TIME();
		int GET_CUTSCENE_TOTAL_DURATION();
		int GET_CUTSCENE_END_TIME();
		int GET_CUTSCENE_PLAY_DURATION();
		BOOL WAS_CUTSCENE_SKIPPED();
		BOOL HAS_CUTSCENE_FINISHED();
		BOOL IS_CUTSCENE_ACTIVE();
		BOOL IS_CUTSCENE_PLAYING();
		int GET_CUTSCENE_SECTION_PLAYING();
		Entity GET_ENTITY_INDEX_OF_CUTSCENE_ENTITY(const char* cutsceneEntName, Hash modelHash);
		int GET_CUTSCENE_CONCAT_SECTION_PLAYING();
		/**
		 * This function is hard-coded to always return 1.
		 */
		BOOL IS_CUTSCENE_AUTHORIZED(const char* cutsceneName);
		int DOES_CUTSCENE_HANDLE_EXIST(int cutsceneHandle);
		void REGISTER_ENTITY_FOR_CUTSCENE(Ped cutscenePed, const char* cutsceneEntName, int p2, Hash modelHash, int p4);
		Entity GET_ENTITY_INDEX_OF_REGISTERED_ENTITY(const char* cutsceneEntName, Hash modelHash);
		/**
		 * Full list of vehicles by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/vehicles.json
		 */
		void SET_VEHICLE_MODEL_PLAYER_WILL_EXIT_SCENE(Hash modelHash);
		/**
		 * Only used twice in R* scripts
		 */
		void SET_CUTSCENE_TRIGGER_AREA(float x1, float y1, float z1, float x2, float y2, float z2);
		/**
		 * modelHash (p1) was always 0 in R* scripts
		 */
		BOOL CAN_SET_ENTER_STATE_FOR_REGISTERED_ENTITY(const char* cutsceneEntName, Hash modelHash);
		BOOL CAN_SET_EXIT_STATE_FOR_REGISTERED_ENTITY(const char* cutsceneEntName, Hash modelHash);
		BOOL CAN_SET_EXIT_STATE_FOR_CAMERA(BOOL p0);
		/**
		 * Toggles a value (bool) for cutscenes.
		 */
		void SET_PAD_CAN_SHAKE_DURING_CUTSCENE(BOOL toggle);
		void SET_CUTSCENE_FADE_VALUES(BOOL p0, BOOL p1, BOOL p2, BOOL p3);
		void SET_CUTSCENE_MULTIHEAD_FADE(BOOL p0, BOOL p1, BOOL p2, BOOL p3);
		void SET_CUTSCENE_MULTIHEAD_FADE_MANUAL(BOOL p0);
		BOOL IS_MULTIHEAD_FADE_UP();
		void NETWORK_SET_MOCAP_CUTSCENE_CAN_BE_SKIPPED(BOOL toggle);
		void SET_CAR_GENERATORS_CAN_UPDATE_DURING_CUTSCENE(BOOL p0);
		BOOL CAN_USE_MOBILE_PHONE_DURING_CUTSCENE();
		void SET_CUTSCENE_CAN_BE_SKIPPED(BOOL p0);
		void SET_CAN_DISPLAY_MINIMAP_DURING_CUTSCENE_THIS_UPDATE();
		/**
		 * Full list of ped components by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/pedComponentVariations.json
		 */
		void SET_CUTSCENE_PED_COMPONENT_VARIATION(const char* cutsceneEntName, int componentId, int drawableId, int textureId, Hash modelHash);
		void SET_CUTSCENE_PED_COMPONENT_VARIATION_FROM_PED(const char* cutsceneEntName, Ped ped, Hash modelHash);
		BOOL DOES_CUTSCENE_ENTITY_EXIST(const char* cutsceneEntName, Hash modelHash);
		/**
		 * Thanks R*! ;)
		 * 
		 * if ((l_161 == 0) || (l_161 == 2)) {
		 * sub_2ea27("Trying to set Jimmy prop variation");
		 * CUTSCENE::SET_CUTSCENE_PED_PROP_VARIATION("Jimmy_Boston", 1, 0, 0, 0);
		 * }
		 * 
		 * Full list of ped components by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/pedComponentVariations.json
		 */
		void SET_CUTSCENE_PED_PROP_VARIATION(const char* cutsceneEntName, int componentId, int drawableId, int textureId, Hash modelHash);
		/**
		 * Possibly HAS_CUTSCENE_CUT_THIS_FRAME, needs more research.
		 */
		BOOL HAS_CUTSCENE_CUT_THIS_FRAME();

	} // namespace CUTSCENE

	namespace DATAFILE {

		/**
		 * Adds the given requestID to the watch list.
		 */
		void DATAFILE_WATCH_REQUEST_ID(int requestId);
		void DATAFILE_CLEAR_WATCH_LIST();
		BOOL DATAFILE_IS_VALID_REQUEST_ID(int index);
		BOOL DATAFILE_HAS_LOADED_FILE_DATA(int requestId);
		BOOL DATAFILE_HAS_VALID_FILE_DATA(int requestId);
		BOOL DATAFILE_SELECT_ACTIVE_FILE(int requestId, Any p1);
		BOOL DATAFILE_DELETE_REQUESTED_FILE(int requestId);
		BOOL UGC_CREATE_CONTENT(Any* data, int dataCount, const char* contentName, const char* description, const char* tagsCsv, const char* contentTypeName, BOOL publish, Any p7);
		BOOL UGC_CREATE_MISSION(const char* contentName, const char* description, const char* tagsCsv, const char* contentTypeName, BOOL publish, Any p5);
		BOOL UGC_UPDATE_CONTENT(const char* contentId, Any* data, int dataCount, const char* contentName, const char* description, const char* tagsCsv, const char* contentTypeName, Any p7);
		BOOL UGC_UPDATE_MISSION(const char* contentId, const char* contentName, const char* description, const char* tagsCsv, const char* contentTypeName, Any p5);
		BOOL UGC_SET_PLAYER_DATA(const char* contentId, float rating, const char* contentTypeName, Any p3);
		BOOL DATAFILE_SELECT_UGC_DATA(int p0, Any p1);
		BOOL DATAFILE_SELECT_UGC_STATS(int p0, BOOL p1, Any p2);
		BOOL DATAFILE_SELECT_UGC_PLAYER_DATA(int p0, Any p1);
		BOOL DATAFILE_SELECT_CREATOR_STATS(int p0, Any p1);
		/**
		 * Loads a User-Generated Content (UGC) file. These files can be found in "[GTA5]\data\ugc" and "[GTA5]\common\patch\ugc". They seem to follow a naming convention, most likely of "[name]_[part].ugc". See example below for usage.
		 * 
		 * Returns whether or not the file was successfully loaded.
		 * 
		 * Example:
		 * DATAFILE::DATAFILE_LOAD_OFFLINE_UGC("RockstarPlaylists") // loads "rockstarplaylists_00.ugc"
		 */
		BOOL DATAFILE_LOAD_OFFLINE_UGC(const char* filename, Any p1);
		void DATAFILE_CREATE(int p0);
		void DATAFILE_DELETE(int p0);
		void DATAFILE_STORE_MISSION_HEADER(int p0);
		void DATAFILE_FLUSH_MISSION_HEADER();
		Any* DATAFILE_GET_FILE_DICT(int p0);
		BOOL DATAFILE_START_SAVE_TO_CLOUD(const char* filename, Any p1);
		BOOL DATAFILE_UPDATE_SAVE_TO_CLOUD(BOOL* p0);
		BOOL DATAFILE_IS_SAVE_PENDING();
		BOOL DATAFILE_LOAD_OFFLINE_UGC_FOR_ADDITIONAL_DATA_FILE(Any p0, Any p1);
		void DATAFILE_DELETE_FOR_ADDITIONAL_DATA_FILE(Any p0);
		Any* DATAFILE_GET_FILE_DICT_FOR_ADDITIONAL_DATA_FILE(Any p0);
		void DATADICT_SET_BOOL(Any* objectData, const char* key, BOOL value);
		void DATADICT_SET_INT(Any* objectData, const char* key, int value);
		void DATADICT_SET_FLOAT(Any* objectData, const char* key, float value);
		void DATADICT_SET_STRING(Any* objectData, const char* key, const char* value);
		void DATADICT_SET_VECTOR(Any* objectData, const char* key, float valueX, float valueY, float valueZ);
		Any* DATADICT_CREATE_DICT(Any* objectData, const char* key);
		Any* DATADICT_CREATE_ARRAY(Any* objectData, const char* key);
		BOOL DATADICT_GET_BOOL(Any* objectData, const char* key);
		int DATADICT_GET_INT(Any* objectData, const char* key);
		float DATADICT_GET_FLOAT(Any* objectData, const char* key);
		const char* DATADICT_GET_STRING(Any* objectData, const char* key);
		Vector3 DATADICT_GET_VECTOR(Any* objectData, const char* key);
		Any* DATADICT_GET_DICT(Any* objectData, const char* key);
		Any* DATADICT_GET_ARRAY(Any* objectData, const char* key);
		/**
		 * Types:
		 * 1 = Boolean
		 * 2 = Integer
		 * 3 = Float
		 * 4 = String
		 * 5 = Vector3
		 * 6 = Object
		 * 7 = Array
		 */
		int DATADICT_GET_TYPE(Any* objectData, const char* key);
		void DATAARRAY_ADD_BOOL(Any* arrayData, BOOL value);
		void DATAARRAY_ADD_INT(Any* arrayData, int value);
		void DATAARRAY_ADD_FLOAT(Any* arrayData, float value);
		void DATAARRAY_ADD_STRING(Any* arrayData, const char* value);
		void DATAARRAY_ADD_VECTOR(Any* arrayData, float valueX, float valueY, float valueZ);
		Any* DATAARRAY_ADD_DICT(Any* arrayData);
		BOOL DATAARRAY_GET_BOOL(Any* arrayData, int arrayIndex);
		int DATAARRAY_GET_INT(Any* arrayData, int arrayIndex);
		float DATAARRAY_GET_FLOAT(Any* arrayData, int arrayIndex);
		const char* DATAARRAY_GET_STRING(Any* arrayData, int arrayIndex);
		Vector3 DATAARRAY_GET_VECTOR(Any* arrayData, int arrayIndex);
		Any* DATAARRAY_GET_DICT(Any* arrayData, int arrayIndex);
		int DATAARRAY_GET_COUNT(Any* arrayData);
		/**
		 * Types:
		 * 1 = Boolean
		 * 2 = Integer
		 * 3 = Float
		 * 4 = String
		 * 5 = Vector3
		 * 6 = Object
		 * 7 = Array
		 */
		int DATAARRAY_GET_TYPE(Any* arrayData, int arrayIndex);

	} // namespace DATAFILE

	namespace DECORATOR {

		BOOL DECOR_SET_TIME(Entity entity, const char* propertyName, int timestamp);
		/**
		 * This function sets metadata of type bool to specified entity.
		 * 
		 */
		BOOL DECOR_SET_BOOL(Entity entity, const char* propertyName, BOOL value);
		BOOL DECOR_SET_FLOAT(Entity entity, const char* propertyName, float value);
		/**
		 * Sets property to int.
		 */
		BOOL DECOR_SET_INT(Entity entity, const char* propertyName, int value);
		BOOL DECOR_GET_BOOL(Entity entity, const char* propertyName);
		float DECOR_GET_FLOAT(Entity entity, const char* propertyName);
		int DECOR_GET_INT(Entity entity, const char* propertyName);
		/**
		 * Returns whether or not the specified property is set for the entity.
		 */
		BOOL DECOR_EXIST_ON(Entity entity, const char* propertyName);
		BOOL DECOR_REMOVE(Entity entity, const char* propertyName);
		/**
		 * https://alloc8or.re/gta5/doc/enums/eDecorType.txt
		 */
		void DECOR_REGISTER(const char* propertyName, int type);
		/**
		 * type: see DECOR_REGISTER
		 */
		BOOL DECOR_IS_REGISTERED_AS_TYPE(const char* propertyName, int type);
		/**
		 * Called after all decorator type initializations.
		 */
		void DECOR_REGISTER_LOCK();

	} // namespace DECORATOR

	namespace DLC {

		BOOL ARE_ANY_CCS_PENDING();
		/**
		 * Returns true if the given DLC pack is present.
		 */
		BOOL IS_DLC_PRESENT(Hash dlcHash);
		/**
		 * This function is hard-coded to always return 1.
		 */
		BOOL DLC_CHECK_CLOUD_DATA_CORRECT();
		/**
		 * This function is hard-coded to always return 0.
		 */
		int GET_EXTRACONTENT_CLOUD_RESULT();
		/**
		 * This function is hard-coded to always return 1.
		 */
		BOOL DLC_CHECK_COMPAT_PACK_CONFIGURATION();
		BOOL GET_EVER_HAD_BAD_PACK_ORDER();
		BOOL GET_IS_LOADING_SCREEN_ACTIVE();
		BOOL GET_IS_INITIAL_LOADING_SCREEN_ACTIVE();
		/**
		 * Sets the value of the specified variable to 0.
		 * Always returns true.
		 */
		BOOL HAS_CLOUD_REQUESTS_FINISHED(BOOL* p0, int unused);
		/**
		 * Unloads GROUP_MAP (GTAO/MP) DLC data and loads GROUP_MAP_SP DLC. Neither are loaded by default, ON_ENTER_MP is a cognate to this function and loads MP DLC (and unloads SP DLC by extension).
		 * Works in singleplayer.
		 */
		void ON_ENTER_SP();
		/**
		 * This loads the GTA:O dlc map parts (high end garages, apartments).
		 * Works in singleplayer.
		 * In order to use GTA:O heist IPL's you have to call this native with the following params: SET_INSTANCE_PRIORITY_MODE(1);
		 */
		void ON_ENTER_MP();
		void ARE_USER_ENTITLEMENTS_UP_TO_DATE(); // Missing in crossmap
		void TRY_GET_USER_ENTITLEMENTS(Any* entitlements); // Missing in crossmap
		void DECLARE_IN_MULTIPLAYER_THIS_FRAME(); // Missing in crossmap

	} // namespace DLC

	namespace ENTITY {

		/**
		 * Checks whether an entity exists in the game world.
		 */
		BOOL DOES_ENTITY_EXIST(Entity entity);
		BOOL DOES_ENTITY_BELONG_TO_THIS_SCRIPT(Entity entity, BOOL p1);
		BOOL DOES_ENTITY_HAVE_DRAWABLE(Entity entity);
		BOOL DOES_ENTITY_HAVE_PHYSICS(Entity entity);
		BOOL DOES_ENTITY_HAVE_SKELETON(Entity entity);
		BOOL DOES_ENTITY_HAVE_ANIM_DIRECTOR(Entity entity);
		/**
		 * P3 is always 3 as far as i cant tell
		 * 
		 * Full list of animation dictionaries and anims by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/animDictsCompact.json
		 */
		BOOL HAS_ENTITY_ANIM_FINISHED(Entity entity, const char* animDict, const char* animName, int p3);
		BOOL HAS_ENTITY_BEEN_DAMAGED_BY_ANY_OBJECT(Entity entity);
		BOOL HAS_ENTITY_BEEN_DAMAGED_BY_ANY_PED(Entity entity);
		BOOL HAS_ENTITY_BEEN_DAMAGED_BY_ANY_VEHICLE(Entity entity);
		/**
		 * Entity 1 = Victim
		 * Entity 2 = Attacker
		 * 
		 * p2 seems to always be 1
		 */
		BOOL HAS_ENTITY_BEEN_DAMAGED_BY_ENTITY(Entity entity1, Entity entity2, BOOL p2);
		/**
		 * traceType is always 17 in the scripts.
		 * 
		 * There is other codes used for traceType:
		 * 19 - in jewelry_prep1a
		 * 126 - in am_hunt_the_beast
		 * 256 & 287 - in fm_mission_controller
		 */
		BOOL HAS_ENTITY_CLEAR_LOS_TO_ENTITY(Entity entity1, Entity entity2, int traceType);
		BOOL HAS_ENTITY_CLEAR_LOS_TO_ENTITY_ADJUST_FOR_COVER(Entity entity1, Entity entity2, int traceType);
		/**
		 * Has the entity1 got a clear line of sight to the other entity2 from the direction entity1 is facing.
		 * This is one of the most CPU demanding BOOL natives in the game; avoid calling this in things like nested for-loops
		 */
		BOOL HAS_ENTITY_CLEAR_LOS_TO_ENTITY_IN_FRONT(Entity entity1, Entity entity2);
		/**
		 * Called on tick.
		 * Tested with vehicles, returns true whenever the vehicle is touching any entity.
		 * 
		 * Note: for vehicles, the wheels can touch the ground and it will still return false, but if the body of the vehicle touches the ground, it will return true.
		 */
		BOOL HAS_ENTITY_COLLIDED_WITH_ANYTHING(Entity entity);
		Entity _GET_LAST_ENTITY_HIT_BY_ENTITY(Entity entity);
		Hash GET_LAST_MATERIAL_HIT_BY_ENTITY(Entity entity);
		Vector3 GET_COLLISION_NORMAL_OF_LAST_HIT_FOR_ENTITY(Entity entity);
		/**
		 * Based on carmod_shop script decompile this takes a vehicle parameter. It is called when repair is done on initial enter.
		 */
		void FORCE_ENTITY_AI_AND_ANIMATION_UPDATE(Entity entity);
		/**
		 * Returns a float value representing animation's current playtime with respect to its total playtime. This value increasing in a range from [0 to 1] and wrap back to 0 when it reach 1.
		 * 
		 * Example:
		 * 0.000000 - mark the starting of animation.
		 * 0.500000 - mark the midpoint of the animation.
		 * 1.000000 - mark the end of animation.
		 * 
		 * Full list of animation dictionaries and anims by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/animDictsCompact.json
		 */
		float GET_ENTITY_ANIM_CURRENT_TIME(Entity entity, const char* animDict, const char* animName);
		/**
		 * Returns a float value representing animation's total playtime in milliseconds.
		 * 
		 * Example:
		 * GET_ENTITY_ANIM_TOTAL_TIME(PLAYER_ID(),"amb@world_human_yoga@female@base","base_b")
		 * return 20800.000000
		 * 
		 * Full list of animation dictionaries and anims by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/animDictsCompact.json
		 */
		float GET_ENTITY_ANIM_TOTAL_TIME(Entity entity, const char* animDict, const char* animName);
		/**
		 * Full list of animation dictionaries and anims by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/animDictsCompact.json
		 */
		float GET_ANIM_DURATION(const char* animDict, const char* animName);
		Entity GET_ENTITY_ATTACHED_TO(Entity entity);
		/**
		 * Gets the current coordinates for a specified entity.
		 * `entity` = The entity to get the coordinates from.
		 * `alive` = Unused by the game, potentially used by debug builds of GTA in order to assert whether or not an entity was alive.
		 */
		Vector3 GET_ENTITY_COORDS(Entity entity, BOOL alive);
		/**
		 * Gets the entity's forward vector.
		 */
		Vector3 GET_ENTITY_FORWARD_VECTOR(Entity entity);
		/**
		 * Gets the X-component of the entity's forward vector.
		 */
		float GET_ENTITY_FORWARD_X(Entity entity);
		/**
		 * Gets the Y-component of the entity's forward vector.
		 */
		float GET_ENTITY_FORWARD_Y(Entity entity);
		/**
		 * Returns the heading of the entity in degrees. Also know as the "Yaw" of an entity.
		 */
		float GET_ENTITY_HEADING(Entity entity);
		/**
		 * Gets the heading of the entity physics in degrees, which tends to be more accurate than just "GET_ENTITY_HEADING". This can be clearly seen while, for example, ragdolling a ped/player.
		 * 
		 * NOTE: The name and description of this native are based on independent research. If you find this native to be more suitable under a different name and/or described differently, please feel free to do so.
		 */
		float GET_ENTITY_HEADING_FROM_EULERS(Entity entity);
		/**
		 * Returns an integer value of entity's current health.
		 * 
		 * Example of range for ped:
		 * - Player [0 to 200]
		 * - Ped [100 to 200]
		 * - Vehicle [0 to 1000]
		 * - Object [0 to 1000]
		 * 
		 * Health is actually a float value but this native casts it to int.
		 * In order to get the actual value, do:
		 * float health = *(float *)(entityAddress + 0x280);
		 */
		int GET_ENTITY_HEALTH(Entity entity);
		/**
		 * Return an integer value of entity's maximum health.
		 * 
		 * Example:
		 * - Player = 200
		 * - Ped = 150
		 */
		int GET_ENTITY_MAX_HEALTH(Entity entity);
		/**
		 * For instance: ENTITY::SET_ENTITY_MAX_HEALTH(PLAYER::PLAYER_PED_ID(), 200); // director_mode.c4: 67849
		 */
		void SET_ENTITY_MAX_HEALTH(Entity entity, int value);
		float GET_ENTITY_HEIGHT(Entity entity, float X, float Y, float Z, BOOL atTop, BOOL inWorldCoords);
		/**
		 * Return height (z-dimension) above ground.
		 * Example: The pilot in a titan plane is 1.844176 above ground.
		 * 
		 * How can i convert it to meters?
		 * Everything seems to be in meters, probably this too.
		 */
		float GET_ENTITY_HEIGHT_ABOVE_GROUND(Entity entity);
		void GET_ENTITY_MATRIX(Entity entity, Vector3* forwardVector, Vector3* rightVector, Vector3* upVector, Vector3* position);
		/**
		 * Returns the model hash from the entity
		 */
		Hash GET_ENTITY_MODEL(Entity entity);
		/**
		 * Converts world coords (posX - Z) to coords relative to the entity
		 * 
		 * Example:
		 * posX is given as 50
		 * entity's x coord is 40
		 * the returned x coord will then be 10 or -10, not sure haven't used this in a while (think it is 10 though).
		 */
		Vector3 GET_OFFSET_FROM_ENTITY_GIVEN_WORLD_COORDS(Entity entity, float posX, float posY, float posZ);
		/**
		 * Offset values are relative to the entity.
		 * 
		 * x = left/right
		 * y = forward/backward
		 * z = up/down
		 */
		Vector3 GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(Entity entity, float offsetX, float offsetY, float offsetZ);
		float GET_ENTITY_PITCH(Entity entity);
		/**
		 * w is the correct parameter name!
		 */
		void GET_ENTITY_QUATERNION(Entity entity, float* x, float* y, float* z, float* w);
		/**
		 * Displays the current ROLL axis of the entity [-180.0000/180.0000+]
		 * (Sideways Roll) such as a vehicle tipped on its side
		 */
		float GET_ENTITY_ROLL(Entity entity);
		/**
		 * rotationOrder is the order yaw, pitch and roll is applied. Usually 2. Returns a vector where the Z coordinate is the yaw.
		 * 
		 * rotationOrder refers to the order yaw pitch roll is applied; value ranges from 0 to 5 and is usually *2* in scripts.
		 * What you use for rotationOrder when getting must be the same as rotationOrder when setting the rotation.
		 * 
		 * What it returns is the yaw on the z part of the vector, which makes sense considering R* considers z as vertical. Here's a picture for those of you who don't understand pitch, yaw, and roll: www.allstar.fiu.edu/aero/images/pic5-1.gif
		 * 
		 * Rotation Orders:
		 * 0: ZYX - Rotate around the z-axis, then the y-axis and finally the x-axis.
		 * 1: YZX - Rotate around the y-axis, then the z-axis and finally the x-axis.
		 * 2: ZXY - Rotate around the z-axis, then the x-axis and finally the y-axis.
		 * 3: XZY - Rotate around the x-axis, then the z-axis and finally the y-axis.
		 * 4: YXZ - Rotate around the y-axis, then the x-axis and finally the z-axis.
		 * 5: XYZ - Rotate around the x-axis, then the y-axis and finally the z-axis.
		 */
		Vector3 GET_ENTITY_ROTATION(Entity entity, int rotationOrder);
		Vector3 GET_ENTITY_ROTATION_VELOCITY(Entity entity);
		/**
		 * Returns the name of the script that owns/created the entity or nullptr. Second parameter is unused, can just be a nullptr.
		 */
		const char* GET_ENTITY_SCRIPT(Entity entity, ScrHandle* script);
		/**
		 * result is in meters per second
		 * 
		 * ------------------------------------------------------------
		 * So would the conversion to mph and km/h, be along the lines of this.
		 * 
		 * float speed = GET_ENTITY_SPEED(veh);
		 * float kmh = (speed * 3.6);
		 * float mph = (speed * 2.236936);
		 * ------------------------------------------------------------
		 */
		float GET_ENTITY_SPEED(Entity entity);
		/**
		 * Relative can be used for getting speed relative to the frame of the vehicle, to determine for example, if you are going in reverse (-y speed) or not (+y speed).
		 */
		Vector3 GET_ENTITY_SPEED_VECTOR(Entity entity, BOOL relative);
		float GET_ENTITY_UPRIGHT_VALUE(Entity entity);
		Vector3 GET_ENTITY_VELOCITY(Entity entity);
		/**
		 * Simply returns whatever is passed to it (Regardless of whether the handle is valid or not).
		 */
		Object GET_OBJECT_INDEX_FROM_ENTITY_INDEX(Entity entity);
		/**
		 * Simply returns whatever is passed to it (Regardless of whether the handle is valid or not).
		 */
		Ped GET_PED_INDEX_FROM_ENTITY_INDEX(Entity entity);
		/**
		 * Simply returns whatever is passed to it (Regardless of whether the handle is valid or not).
		 */
		Vehicle GET_VEHICLE_INDEX_FROM_ENTITY_INDEX(Entity entity);
		/**
		 * Returns the coordinates of an entity-bone.
		 */
		Vector3 GET_WORLD_POSITION_OF_ENTITY_BONE(Entity entity, int boneIndex);
		Player GET_NEAREST_PLAYER_TO_ENTITY(Entity entity);
		Player GET_NEAREST_PLAYER_TO_ENTITY_ON_TEAM(Entity entity, int team);
		int GET_NEAREST_PARTICIPANT_TO_ENTITY(Entity entity);
		/**
		 * Returns:
		 * 0 = no entity
		 * 1 = ped
		 * 2 = vehicle
		 * 3 = object
		 */
		int GET_ENTITY_TYPE(Entity entity);
		/**
		 * A population type, from the following enum: https://alloc8or.re/gta5/doc/enums/ePopulationType.txt
		 */
		int GET_ENTITY_POPULATION_TYPE(Entity entity);
		BOOL IS_AN_ENTITY(ScrHandle handle);
		BOOL IS_ENTITY_A_PED(Entity entity);
		BOOL IS_ENTITY_A_MISSION_ENTITY(Entity entity);
		BOOL IS_ENTITY_A_VEHICLE(Entity entity);
		BOOL IS_ENTITY_AN_OBJECT(Entity entity);
		/**
		 * Checks if entity is within x/y/zSize distance of x/y/z.
		 * 
		 * Last three are unknown ints, almost always p7 = 0, p8 = 1, p9 = 0
		 */
		BOOL IS_ENTITY_AT_COORD(Entity entity, float xPos, float yPos, float zPos, float xSize, float ySize, float zSize, BOOL p7, BOOL p8, int p9);
		/**
		 * Checks if entity1 is within the box defined by x/y/zSize of entity2.
		 * 
		 * Last three parameters are almost alwasy p5 = 0, p6 = 1, p7 = 0
		 */
		BOOL IS_ENTITY_AT_ENTITY(Entity entity1, Entity entity2, float xSize, float ySize, float zSize, BOOL p5, BOOL p6, int p7);
		/**
		 * Whether the entity is attached to any other entity.
		 */
		BOOL IS_ENTITY_ATTACHED(Entity entity);
		BOOL IS_ENTITY_ATTACHED_TO_ANY_OBJECT(Entity entity);
		BOOL IS_ENTITY_ATTACHED_TO_ANY_PED(Entity entity);
		BOOL IS_ENTITY_ATTACHED_TO_ANY_VEHICLE(Entity entity);
		BOOL IS_ENTITY_ATTACHED_TO_ENTITY(Entity from, Entity to);
		BOOL IS_ENTITY_DEAD(Entity entity, BOOL p1);
		BOOL IS_ENTITY_IN_AIR(Entity entity);
		/**
		 * `p8` is a debug flag invoking functions in the same path as ``DRAW_MARKER``
		 * `p10` is some entity flag check, also used in `IS_ENTITY_AT_ENTITY`, `IS_ENTITY_IN_AREA`, and `IS_ENTITY_AT_COORD`.
		 * See IS_POINT_IN_ANGLED_AREA for the definition of an angled area.
		 */
		BOOL IS_ENTITY_IN_ANGLED_AREA(Entity entity, float x1, float y1, float z1, float x2, float y2, float z2, float width, BOOL debug, BOOL includeZ, Any p10);
		BOOL IS_ENTITY_IN_AREA(Entity entity, float x1, float y1, float z1, float x2, float y2, float z2, BOOL p7, BOOL p8, Any p9);
		/**
		 * Full list of zones by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/zones.json
		 */
		BOOL IS_ENTITY_IN_ZONE(Entity entity, const char* zone);
		BOOL IS_ENTITY_IN_WATER(Entity entity);
		/**
		 * Get how much of the entity is submerged.  1.0f is whole entity.
		 */
		float GET_ENTITY_SUBMERGED_LEVEL(Entity entity);
		void SET_ENTITY_REQUIRES_MORE_EXPENSIVE_RIVER_CHECK(Entity entity, BOOL toggle);
		/**
		 * Returns true if the entity is in between the minimum and maximum values for the 2d screen coords.
		 * This means that it will return true even if the entity is behind a wall for example, as long as you're looking at their location.
		 * Chipping
		 */
		BOOL IS_ENTITY_ON_SCREEN(Entity entity);
		/**
		 * See also PED::IS_SCRIPTED_SCENARIO_PED_USING_CONDITIONAL_ANIM 0x6EC47A344923E1ED 0x3C30B447
		 * 
		 * Taken from ENTITY::IS_ENTITY_PLAYING_ANIM(PLAYER::PLAYER_PED_ID(), "creatures@shark@move", "attack_player", 3)
		 * 
		 * p4 is always 3 in the scripts.
		 * 
		 * Full list of animation dictionaries and anims by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/animDictsCompact.json
		 */
		BOOL IS_ENTITY_PLAYING_ANIM(Entity entity, const char* animDict, const char* animName, int taskFlag);
		/**
		 * a static ped will not react to natives like "APPLY_FORCE_TO_ENTITY" or "SET_ENTITY_VELOCITY" and oftentimes will not react to task-natives like "TASK::TASK_COMBAT_PED". The only way I know of to make one of these peds react is to ragdoll them (or sometimes to use CLEAR_PED_TASKS_IMMEDIATELY(). Static peds include almost all far-away peds, beach-combers, peds in certain scenarios, peds crossing a crosswalk, peds walking to get back into their cars, and others. If anyone knows how to make a ped non-static without ragdolling them, please edit this with the solution.
		 */
		BOOL IS_ENTITY_STATIC(Entity entity);
		BOOL IS_ENTITY_TOUCHING_ENTITY(Entity entity, Entity targetEntity);
		BOOL IS_ENTITY_TOUCHING_MODEL(Entity entity, Hash modelHash);
		BOOL IS_ENTITY_UPRIGHT(Entity entity, float angle);
		BOOL IS_ENTITY_UPSIDEDOWN(Entity entity);
		BOOL IS_ENTITY_VISIBLE(Entity entity);
		BOOL IS_ENTITY_VISIBLE_TO_SCRIPT(Entity entity);
		BOOL IS_ENTITY_OCCLUDED(Entity entity);
		BOOL WOULD_ENTITY_BE_OCCLUDED(Hash entityModelHash, float x, float y, float z, BOOL p4);
		BOOL IS_ENTITY_WAITING_FOR_WORLD_COLLISION(Entity entity);
		/**
		 * Applies a force to the specified entity.
		 * 
		 * **List of force types (p1)**:
		 * public enum ForceType
		 * {
		 * MinForce = 0,
		 * MaxForceRot = 1,
		 * MinForce2 = 2,
		 * MaxForceRot2 = 3,
		 * ForceNoRot = 4,
		 * ForceRotPlusForce = 5
		 * }
		 * Research/documentation on the gtaforums can be found here https://gtaforums.com/topic/885669-precisely-define-object-physics/) and here https://gtaforums.com/topic/887362-apply-forces-and-momentums-to-entityobject/.
		 * 
		 * p6/relative - makes the xyz force not relative to world coords, but to something else
		 * p7/highForce - setting false will make the force really low
		 */
		void APPLY_FORCE_TO_ENTITY_CENTER_OF_MASS(Entity entity, int forceType, float x, float y, float z, BOOL p5, BOOL isDirectionRel, BOOL isForceRel, BOOL p8);
		/**
		 * Documented here:
		 * gtaforums.com/topic/885669-precisely-define-object-physics/
		 * gtaforums.com/topic/887362-apply-forces-and-momentums-to-entityobject/
		 * 
		 * forceFlags:
		 * First bit (lowest): Strong force flag, factor 100
		 * Second bit: Unkown flag
		 * Third bit: Momentum flag=1 (vector (x,y,z) is a momentum, more research needed)
		 * If higher bits are unequal 0 the function doesn't applay any forces at all.
		 * (As integer possible values are 0-7)
		 * 
		 * 0: weak force
		 * 1: strong force
		 * 2: same as 0 (2nd bit?)
		 * 3: same as 1
		 * 4: weak momentum
		 * 5: strong momentum
		 * 6: same as 4
		 * 7: same as 5
		 * 
		 * isLocal: vector defined in local (body-fixed) coordinate frame
		 * isMassRel: if true the force gets multiplied with the objects mass (this is why it was known as highForce) and different objects will have the same acceleration.
		 * 
		 * p8 !!! Whenever I set this !=0, my script stopped.
		 */
		void APPLY_FORCE_TO_ENTITY(Entity entity, int forceFlags, float x, float y, float z, float offX, float offY, float offZ, int boneIndex, BOOL isDirectionRel, BOOL ignoreUpVec, BOOL isForceRel, BOOL p12, BOOL p13);
		/**
		 * Attaches entity1 to bone (boneIndex) of entity2.
		 * 
		 * boneIndex - this is different to boneID, use GET_PED_BONE_INDEX to get the index from the ID. use the index for attaching to specific bones. entity1 will be attached to entity2's centre if bone index given doesn't correspond to bone indexes for that entity type.
		 * 
		 * useSoftPinning - if set to false attached entity will not detach when fixed
		 * collision - controls collision between the two entities (FALSE disables collision).
		 * isPed - pitch doesnt work when false and roll will only work on negative numbers (only peds)
		 * vertexIndex - position of vertex
		 * fixedRot - if false it ignores entity vector
		 * 
		 */
		void ATTACH_ENTITY_TO_ENTITY(Entity entity1, Entity entity2, int boneIndex, float xPos, float yPos, float zPos, float xRot, float yRot, float zRot, BOOL p9, BOOL useSoftPinning, BOOL collision, BOOL isPed, int vertexIndex, BOOL fixedRot, Any p15);
		void ATTACH_ENTITY_BONE_TO_ENTITY_BONE(Entity entity1, Entity entity2, int boneIndex1, int boneIndex2, BOOL p4, BOOL p5);
		void ATTACH_ENTITY_BONE_TO_ENTITY_BONE_Y_FORWARD(Entity entity1, Entity entity2, int boneIndex1, int boneIndex2, BOOL p4, BOOL p5);
		/**
		 * breakForce is the amount of force required to break the bond.
		 * p14 - is always 1 in scripts
		 * p15 - is 1 or 0 in scripts - unknoun what it does
		 * p16 - controls collision between the two entities (FALSE disables collision).
		 * p17 - do not teleport entity to be attached to the position of the bone Index of the target entity (if 1, entity will not be teleported to target bone)
		 * p18 - is always 2 in scripts.
		 * 
		 * 
		 */
		void ATTACH_ENTITY_TO_ENTITY_PHYSICALLY(Entity entity1, Entity entity2, int boneIndex1, int boneIndex2, float xPos1, float yPos1, float zPos1, float xPos2, float yPos2, float zPos2, float xRot, float yRot, float zRot, float breakForce, BOOL fixedRot, BOOL p15, BOOL collision, BOOL p17, int p18);
		void ATTACH_ENTITY_TO_ENTITY_PHYSICALLY_OVERRIDE_INVERSE_MASS(Entity firstEntityIndex, Entity secondEntityIndex, int firstEntityBoneIndex, int secondEntityBoneIndex, float secondEntityOffsetX, float secondEntityOffsetY, float secondEntityOffsetZ, float firstEntityOffsetX, float firstEntityOffsetY, float firstEntityOffsetZ, float vecRotationX, float vecRotationY, float vecRotationZ, float physicalStrength, BOOL constrainRotation, BOOL doInitialWarp, BOOL collideWithEntity, BOOL addInitialSeperation, int rotOrder, float invMassScaleA, float invMassScaleB);
		/**
		 * Called to update entity attachments.
		 */
		void PROCESS_ENTITY_ATTACHMENTS(Entity entity);
		/**
		 * Returns the index of the bone. If the bone was not found, -1 will be returned.
		 * 
		 * list:
		 * https://pastebin.com/D7JMnX1g
		 * 
		 * BoneNames:
		 * chassis,
		 * windscreen,
		 * seat_pside_r,
		 * seat_dside_r,
		 * bodyshell,
		 * suspension_lm,
		 * suspension_lr,
		 * platelight,
		 * attach_female,
		 * attach_male,
		 * bonnet,
		 * boot,
		 * chassis_dummy,  //Center of the dummy
		 * chassis_Control,    //Not found yet
		 * door_dside_f,   //Door left, front
		 * door_dside_r,   //Door left, back
		 * door_pside_f,   //Door right, front
		 * door_pside_r,   //Door right, back
		 * Gun_GripR,
		 * windscreen_f,
		 * platelight, //Position where the light above the numberplate is located
		 * VFX_Emitter,
		 * window_lf,  //Window left, front
		 * window_lr,  //Window left, back
		 * window_rf,  //Window right, front
		 * window_rr,  //Window right, back
		 * engine, //Position of the engine
		 * gun_ammo,
		 * ROPE_ATTATCH,   //Not misspelled. In script "finale_heist2b.c4".
		 * wheel_lf,   //Wheel left, front
		 * wheel_lr,   //Wheel left, back
		 * wheel_rf,   //Wheel right, front
		 * wheel_rr,   //Wheel right, back
		 * exhaust,    //Exhaust. shows only the position of the stock-exhaust
		 * overheat,   //A position on the engine(not exactly sure, how to name it)
		 * misc_e, //Not a car-bone.
		 * seat_dside_f,   //Driver-seat
		 * seat_pside_f,   //Seat next to driver
		 * Gun_Nuzzle,
		 * seat_r
		 * 
		 * I doubt that the function is case-sensitive, since I found a "Chassis" and a "chassis". - Just tested: Definitely not case-sensitive.
		 * 
		 * 
		 */
		int GET_ENTITY_BONE_INDEX_BY_NAME(Entity entity, const char* boneName);
		void CLEAR_ENTITY_LAST_DAMAGE_ENTITY(Entity entity);
		/**
		 * Deletes the specified entity, then sets the handle pointed to by the pointer to NULL.
		 */
		void DELETE_ENTITY(Entity* entity);
		/**
		 * If `collision` is set to true, both entities won't collide with the other until the distance between them is above 4 meters.
		 * Set `dynamic` to true to keep velocity after dettaching
		 */
		void DETACH_ENTITY(Entity entity, BOOL dynamic, BOOL collision);
		/**
		 * Freezes or unfreezes an entity preventing its coordinates to change by the player if set to `true`. You can still change the entity position using SET_ENTITY_COORDS.
		 */
		void FREEZE_ENTITY_POSITION(Entity entity, BOOL toggle);
		/**
		 * True means it can be deleted by the engine when switching lobbies/missions/etc, false means the script is expected to clean it up.
		 * 
		 * "Allow Freeze If No Collision"
		 */
		void SET_ENTITY_SHOULD_FREEZE_WAITING_ON_COLLISION(Entity entity, BOOL toggle);
		/**
		 * delta and bitset are guessed fields. They are based on the fact that most of the calls have 0 or nil field types passed in.
		 * 
		 * The only time bitset has a value is 0x4000 and the only time delta has a value is during stealth with usually <1.0f values.
		 * 
		 * Full list of animation dictionaries and anims by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/animDictsCompact.json
		 */
		BOOL PLAY_ENTITY_ANIM(Entity entity, const char* animName, const char* animDict, float p3, BOOL loop, BOOL stayInAnim, BOOL p6, float delta, Any bitset);
		/**
		 * p4 and p7 are usually 1000.0f.
		 * 
		 * Full list of animation dictionaries and anims by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/animDictsCompact.json
		 */
		BOOL PLAY_SYNCHRONIZED_ENTITY_ANIM(Entity entity, int syncedScene, const char* animation, const char* propName, float p4, float p5, Any p6, float p7);
		/**
		 * p6,p7 probably animname and animdict
		 * 
		 * Full list of animation dictionaries and anims by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/animDictsCompact.json
		 */
		BOOL PLAY_SYNCHRONIZED_MAP_ENTITY_ANIM(float x1, float y1, float z1, float x2, Any y2, float z2, const char* p6, const char* p7, float p8, float p9, Any p10, float p11);
		BOOL STOP_SYNCHRONIZED_MAP_ENTITY_ANIM(float x1, float y1, float z1, float x2, Any y2, float z2);
		/**
		 * Full list of animation dictionaries and anims by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/animDictsCompact.json
		 * 
		 * RAGEPluginHook list: docs.ragepluginhook.net/html/62951c37-a440-478c-b389-c471230ddfc5.htm
		 */
		BOOL STOP_ENTITY_ANIM(Entity entity, const char* animation, const char* animGroup, float p3);
		BOOL STOP_SYNCHRONIZED_ENTITY_ANIM(Entity entity, float p1, BOOL p2);
		/**
		 * if (ENTITY::HAS_ANIM_EVENT_FIRED(PLAYER::PLAYER_PED_ID(), MISC::GET_HASH_KEY("CreateObject")))
		 */
		BOOL HAS_ANIM_EVENT_FIRED(Entity entity, Hash actionHash);
		/**
		 * In the script "player_scene_t_bbfight.c4":
		 * "if (ENTITY::FIND_ANIM_EVENT_PHASE(&l_16E, &l_19F[v_4/*16* /], v_9, &v_A, &v_B))"
		 * -- &l_16E (p0) is requested as an anim dictionary earlier in the script.
		 * -- &l_19F[v_4/*16* /] (p1) is used in other natives in the script as the "animation" param.
		 * -- v_9 (p2) is instantiated as "victim_fall"; I'm guessing that's another anim
		 * --v_A and v_B (p3 & p4) are both set as -1.0, but v_A is used immediately after this native for:
		 * "if (v_A < ENTITY::GET_ENTITY_ANIM_CURRENT_TIME(...))"
		 * Both v_A and v_B are seemingly used to contain both Vector3's and floats, so I can't say what either really is other than that they are both output parameters. p4 looks more like a *Vector3 though
		 * 
		 * Full list of animation dictionaries and anims by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/animDictsCompact.json
		 */
		BOOL FIND_ANIM_EVENT_PHASE(const char* animDictionary, const char* animName, const char* p2, Any* p3, Any* p4);
		/**
		 * Full list of animation dictionaries and anims by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/animDictsCompact.json
		 */
		void SET_ENTITY_ANIM_CURRENT_TIME(Entity entity, const char* animDictionary, const char* animName, float time);
		/**
		 * Full list of animation dictionaries and anims by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/animDictsCompact.json
		 */
		void SET_ENTITY_ANIM_SPEED(Entity entity, const char* animDictionary, const char* animName, float speedMultiplier);
		/**
		 * Makes the specified entity (ped, vehicle or object) persistent. Persistent entities will not automatically be removed by the engine.
		 * 
		 * p1 has no effect when either its on or off
		 * maybe a quick disassembly will tell us what it does
		 * 
		 * p2 has no effect when either its on or off
		 * maybe a quick disassembly will tell us what it does
		 */
		void SET_ENTITY_AS_MISSION_ENTITY(Entity entity, BOOL p1, BOOL p2);
		/**
		 * Marks the specified entity (ped, vehicle or object) as no longer needed if its population type is set to the mission type.
		 * If the entity is ped, it will also clear their tasks immediately just like when CLEAR_PED_TASKS_IMMEDIATELY is called.
		 * Entities marked as no longer needed, will be deleted as the engine sees fit.
		 * Use this if you just want to just let the game delete the ped:
		 * void MarkPedAsAmbientPed(Ped ped) {
		 * auto addr = getScriptHandleBaseAddress(ped);
		 * 
		 * if (!addr) {
		 * return;
		 * }
		 * 
		 * //the game uses only lower 4 bits as entity population type
		 * BYTE origValue = *(BYTE *)(addr + 0xDA);
		 * *(BYTE *)(addr + 0xDA) = ((origValue & 0xF0) | ePopulationType::POPTYPE_RANDOM_AMBIENT);
		 * }
		 */
		void SET_ENTITY_AS_NO_LONGER_NEEDED(Entity* entity);
		/**
		 * This is an alias of SET_ENTITY_AS_NO_LONGER_NEEDED.
		 */
		void SET_PED_AS_NO_LONGER_NEEDED(Ped* ped);
		/**
		 * This is an alias of SET_ENTITY_AS_NO_LONGER_NEEDED.
		 */
		void SET_VEHICLE_AS_NO_LONGER_NEEDED(Vehicle* vehicle);
		/**
		 * This is an alias of SET_ENTITY_AS_NO_LONGER_NEEDED.
		 */
		void SET_OBJECT_AS_NO_LONGER_NEEDED(Object* object);
		void SET_ENTITY_CAN_BE_DAMAGED(Entity entity, BOOL toggle);
		BOOL GET_ENTITY_CAN_BE_DAMAGED(Entity entity);
		void SET_ENTITY_CAN_BE_DAMAGED_BY_RELATIONSHIP_GROUP(Entity entity, BOOL bCanBeDamaged, int relGroup);
		void SET_ENTITY_CAN_ONLY_BE_DAMAGED_BY_SCRIPT_PARTICIPANTS(Entity entity, BOOL toggle);
		/**
		 * Sets whether the entity can be targeted without being in line-of-sight.
		 */
		void SET_ENTITY_CAN_BE_TARGETED_WITHOUT_LOS(Entity entity, BOOL toggle);
		void SET_ENTITY_COLLISION(Entity entity, BOOL toggle, BOOL keepPhysics);
		BOOL GET_ENTITY_COLLISION_DISABLED(Entity entity);
		void SET_ENTITY_COMPLETELY_DISABLE_COLLISION(Entity entity, BOOL toggle, BOOL keepPhysics);
		/**
		 * p7 is always 1 in the scripts. Set to 1, an area around the destination coords for the moved entity is cleared from other entities.
		 * 
		 * Often ends with 1, 0, 0, 1); in the scripts. It works.
		 * 
		 * Axis - Invert Axis Flags
		 */
		void SET_ENTITY_COORDS(Entity entity, float xPos, float yPos, float zPos, BOOL xAxis, BOOL yAxis, BOOL zAxis, BOOL clearArea);
		void SET_ENTITY_COORDS_WITHOUT_PLANTS_RESET(Entity entity, float xPos, float yPos, float zPos, BOOL alive, BOOL deadFlag, BOOL ragdollFlag, BOOL clearArea);
		/**
		 * Axis - Invert Axis Flags
		 */
		void SET_ENTITY_COORDS_NO_OFFSET(Entity entity, float xPos, float yPos, float zPos, BOOL xAxis, BOOL yAxis, BOOL zAxis);
		void SET_ENTITY_DYNAMIC(Entity entity, BOOL toggle);
		/**
		 * Set the heading of an entity in degrees also known as "Yaw".
		 */
		void SET_ENTITY_HEADING(Entity entity, float heading);
		/**
		 * health >= 0
		 * male ped ~= 100 - 200
		 * female ped ~= 0 - 100
		 */
		void SET_ENTITY_HEALTH(Entity entity, int health, Entity instigator, Hash weaponType);
		/**
		 * Sets a ped or an object totally invincible. It doesn't take any kind of damage. Peds will not ragdoll on explosions and the tazer animation won't apply either.
		 * 
		 * If you use this for a ped and you want Ragdoll to stay enabled, then do:
		 * *(DWORD *)(pedAddress + 0x188) |= (1 << 9);
		 * 
		 * Use this if you want to get the invincibility status:
		 * bool IsPedInvincible(Ped ped)
		 * {
		 * auto addr = getScriptHandleBaseAddress(ped);
		 * 
		 * if (addr)
		 * {
		 * DWORD flag = *(DWORD *)(addr + 0x188);
		 * return ((flag & (1 << 8)) != 0) || ((flag & (1 << 9)) != 0);
		 * }
		 * 
		 * return false;
		 * }
		 */
		void SET_ENTITY_INVINCIBLE(Entity entity, BOOL toggle, BOOL dontResetOnCleanup);
		void SET_ENTITY_IS_TARGET_PRIORITY(Entity entity, BOOL p1, float p2);
		void SET_ENTITY_LIGHTS(Entity entity, BOOL toggle);
		/**
		 * Loads collision grid for an entity spawned outside of a player's loaded area. This allows peds to execute tasks rather than sit dormant because of a lack of a physics grid.
		 * Certainly not the main usage of this native but when set to true for a Vehicle, it will prevent the vehicle to explode if it is spawned far away from the player.
		 */
		void SET_ENTITY_LOAD_COLLISION_FLAG(Entity entity, BOOL toggle, Any p2);
		BOOL HAS_COLLISION_LOADED_AROUND_ENTITY(Entity entity);
		void SET_ENTITY_MAX_SPEED(Entity entity, float speed);
		void SET_ENTITY_ONLY_DAMAGED_BY_PLAYER(Entity entity, BOOL toggle);
		void SET_ENTITY_ONLY_DAMAGED_BY_RELATIONSHIP_GROUP(Entity entity, BOOL p1, Any p2);
		/**
		 * Enable / disable each type of damage.
		 * 
		 * waterProof is damage related to water not drowning
		 * --------------
		 * p7 is to to '1' in am_mp_property_ext/int: ENTITY::SET_ENTITY_PROOFS(uParam0->f_19, true, true, true, true, true, true, 1, true);
		 * 
		 */
		void SET_ENTITY_PROOFS(Entity entity, BOOL bulletProof, BOOL fireProof, BOOL explosionProof, BOOL collisionProof, BOOL meleeProof, BOOL steamProof, BOOL dontResetOnCleanup, BOOL waterProof);
		BOOL GET_ENTITY_PROOFS(Entity entity, BOOL* bulletProof, BOOL* fireProof, BOOL* explosionProof, BOOL* collisionProof, BOOL* meleeProof, BOOL* steamProof, BOOL* p7, BOOL* drownProof);
		/**
		 * w is the correct parameter name!
		 */
		void SET_ENTITY_QUATERNION(Entity entity, float x, float y, float z, float w);
		void SET_ENTITY_RECORDS_COLLISIONS(Entity entity, BOOL toggle);
		/**
		 * rotationOrder refers to the order yaw pitch roll is applied
		 * value ranges from 0 to 5. What you use for rotationOrder when setting must be the same as rotationOrder when getting the rotation.
		 * Unsure what value corresponds to what rotation order, more testing will be needed for that.
		 * For the most part R* uses 1 or 2 as the order.
		 * p5 is usually set as true
		 * 
		 */
		void SET_ENTITY_ROTATION(Entity entity, float pitch, float roll, float yaw, int rotationOrder, BOOL p5);
		/**
		 * p2 is always 0.
		 */
		void SET_ENTITY_VISIBLE(Entity entity, BOOL toggle, BOOL p2);
		void SET_ENTITY_WATER_REFLECTION_FLAG(Entity entity, BOOL toggle);
		void SET_ENTITY_MIRROR_REFLECTION_FLAG(Entity entity, BOOL p1);
		/**
		 * Note that the third parameter(denoted as z) is "up and down" with positive numbers encouraging upwards movement.
		 */
		void SET_ENTITY_VELOCITY(Entity entity, float x, float y, float z);
		void SET_ENTITY_ANGULAR_VELOCITY(Entity entity, float x, float y, float z);
		void SET_ENTITY_HAS_GRAVITY(Entity entity, BOOL toggle);
		/**
		 * LOD distance can be 0 to 0xFFFF (higher values will result in 0xFFFF) as it is actually stored as a 16-bit value (aka uint16_t).
		 */
		void SET_ENTITY_LOD_DIST(Entity entity, int value);
		/**
		 * Returns the LOD distance of an entity.
		 */
		int GET_ENTITY_LOD_DIST(Entity entity);
		/**
		 * skin - everything alpha except skin
		 * Set entity alpha level. Ranging from 0 to 255 but chnages occur after every 20 percent (after every 51).
		 */
		void SET_ENTITY_ALPHA(Entity entity, int alphaLevel, BOOL skin);
		int GET_ENTITY_ALPHA(Entity entity);
		void RESET_ENTITY_ALPHA(Entity entity);
		/**
		 * Similar to RESET_ENTITY_ALPHA
		 */
		void RESET_PICKUP_ENTITY_GLOW(Entity entity);
		void SET_PICKUP_COLLIDES_WITH_PROJECTILES(Any p0, Any p1);
		/**
		 * Only called once in the scripts.
		 * 
		 * Related to weapon objects.
		 * 
		 */
		void SET_ENTITY_SORT_BIAS(Entity entity, float p1);
		void SET_ENTITY_ALWAYS_PRERENDER(Entity entity, BOOL toggle);
		void SET_ENTITY_RENDER_SCORCHED(Entity entity, BOOL toggle);
		/**
		 * Example here: www.gtaforums.com/topic/830463-help-with-turning-lights-green-and-causing-peds-to-crash-into-each-other/#entry1068211340
		 * 
		 * 0 = green
		 * 1 = red
		 * 2 = yellow
		 * 3 = reset changes
		 * changing lights may not change the behavior of vehicles
		 */
		void SET_ENTITY_TRAFFICLIGHT_OVERRIDE(Entity entity, int state);
		void SET_ENTITY_IS_IN_VEHICLE(Entity entity);
		/**
		 * Only works with objects!
		 */
		void CREATE_MODEL_SWAP(float x, float y, float z, float radius, Hash originalModel, Hash newModel, BOOL p6);
		void REMOVE_MODEL_SWAP(float x, float y, float z, float radius, Hash originalModel, Hash newModel, BOOL p6);
		/**
		 * p5 = sets as true in scripts
		 * Same as the comment for CREATE_MODEL_SWAP unless for some reason p5 affects it this only works with objects as well.
		 * 
		 * Network players do not see changes done with this.
		 */
		void CREATE_MODEL_HIDE(float x, float y, float z, float radius, Hash modelHash, BOOL p5);
		void CREATE_MODEL_HIDE_EXCLUDING_SCRIPT_OBJECTS(float x, float y, float z, float radius, Hash modelHash, BOOL p5);
		/**
		 * This native makes entities visible that are hidden by the native CREATE_MODEL_HIDE.
		 * p5 should be false, true does nothing
		 */
		void REMOVE_MODEL_HIDE(float x, float y, float z, float radius, Hash modelHash, BOOL p5);
		void CREATE_FORCED_OBJECT(float x, float y, float z, Any p3, Hash modelHash, BOOL p5);
		void REMOVE_FORCED_OBJECT(float x, float y, float z, float p3, Hash modelHash);
		/**
		 * Calling this function disables collision between two entities.
		 * The importance of the order for entity1 and entity2 is unclear.
		 * The third parameter, `thisFrame`, decides whether the collision is to be disabled until it is turned back on, or if it's just this frame.
		 */
		void SET_ENTITY_NO_COLLISION_ENTITY(Entity entity1, Entity entity2, BOOL thisFrameOnly);
		void _SET_ENTITY_NO_COLLISION_WITH_NETWORKED_ENTITY(Entity entity1, Entity entity2); // Missing in crossmap
		void SET_ENTITY_MOTION_BLUR(Entity entity, BOOL toggle);
		/**
		 * p1 always false.
		 */
		void SET_CAN_AUTO_VAULT_ON_ENTITY(Entity entity, BOOL toggle);
		/**
		 * p1 always false.
		 */
		void SET_CAN_CLIMB_ON_ENTITY(Entity entity, BOOL toggle);
		/**
		 * Only called within 1 script for x360. 'fm_mission_controller' and it used on an object.
		 * 
		 * Ran after these 2 natives,
		 * set_object_targettable(uParam0, 0);
		 * set_entity_invincible(uParam0, 1);
		 */
		void SET_WAIT_FOR_COLLISIONS_BEFORE_PROBE(Entity entity, BOOL toggle);
		void SET_ENTITY_NOWEAPONDECALS(Entity entity, BOOL p1);
		void SET_ENTITY_USE_MAX_DISTANCE_FOR_WATER_REFLECTION(Entity entity, BOOL p1);
		/**
		 * Gets the world rotation of the specified bone of the specified entity.
		 */
		Vector3 GET_ENTITY_BONE_ROTATION(Entity entity, int boneIndex);
		/**
		 * Gets the world position of the specified bone of the specified entity.
		 */
		Vector3 GET_ENTITY_BONE_POSTION(Entity entity, int boneIndex);
		/**
		 * Gets the local rotation of the specified bone of the specified entity.
		 */
		Vector3 GET_ENTITY_BONE_OBJECT_ROTATION(Entity entity, int boneIndex);
		Vector3 GET_ENTITY_BONE_OBJECT_POSTION(Entity entity, int boneIndex);
		int GET_ENTITY_BONE_COUNT(Entity entity);
		void ENABLE_ENTITY_BULLET_COLLISION(Entity entity);
		void SET_ENTITY_CAN_ONLY_BE_DAMAGED_BY_ENTITY(Entity entity1, Entity entity2);
		void SET_ENTITY_CANT_CAUSE_COLLISION_DAMAGED_ENTITY(Entity entity1, Entity entity2);
		/**
		 * p1 is always set to 1
		 */
		void SET_ALLOW_MIGRATE_TO_SPECTATOR(Entity entity, Any p1);
		/**
		 * Gets the handle of an entity with a specific model hash attached to another entity, such as an object attached to a ped.
		 * This native does not appear to have anything to do with pickups as in scripts it is used with objects.
		 * 
		 * Example from fm_mission_controller_2020.c:
		 * 
		 * iVar8 = ENTITY::GET_ENTITY_OF_TYPE_ATTACHED_TO_ENTITY(bParam0->f_9, joaat("p_cs_clipboard"));
		 */
		Entity GET_ENTITY_OF_TYPE_ATTACHED_TO_ENTITY(Entity entity, Hash modelHash);
		void SET_PICK_UP_BY_CARGOBOB_DISABLED(Entity entity, BOOL toggle);

	} // namespace ENTITY

	namespace EVENT {

		void SET_DECISION_MAKER(Ped ped, Hash name);
		/**
		 * eventType: https://alloc8or.re/gta5/doc/enums/eEventType.txt
		 */
		void CLEAR_DECISION_MAKER_EVENT_RESPONSE(Hash name, int eventType);
		/**
		 * eventType: https://alloc8or.re/gta5/doc/enums/eEventType.txt
		 * 
		 * This is limited to 4 blocked events at a time.
		 */
		void BLOCK_DECISION_MAKER_EVENT(Hash name, int eventType);
		/**
		 * eventType: https://alloc8or.re/gta5/doc/enums/eEventType.txt
		 */
		void UNBLOCK_DECISION_MAKER_EVENT(Hash name, int eventType);
		/**
		 * eventType: https://alloc8or.re/gta5/doc/enums/eEventType.txt
		 */
		int ADD_SHOCKING_EVENT_AT_POSITION(int eventType, float x, float y, float z, float duration);
		/**
		 * eventType: https://alloc8or.re/gta5/doc/enums/eEventType.txt
		 */
		int ADD_SHOCKING_EVENT_FOR_ENTITY(int eventType, Entity entity, float duration);
		/**
		 * eventType: https://alloc8or.re/gta5/doc/enums/eEventType.txt
		 */
		BOOL IS_SHOCKING_EVENT_IN_SPHERE(int eventType, float x, float y, float z, float radius);
		BOOL REMOVE_SHOCKING_EVENT(ScrHandle event);
		void REMOVE_ALL_SHOCKING_EVENTS(BOOL p0);
		void REMOVE_SHOCKING_EVENT_SPAWN_BLOCKING_AREAS();
		void SUPPRESS_SHOCKING_EVENTS_NEXT_FRAME();
		/**
		 * eventType: https://alloc8or.re/gta5/doc/enums/eEventType.txt
		 */
		void SUPPRESS_SHOCKING_EVENT_TYPE_NEXT_FRAME(int eventType);
		void SUPPRESS_AGITATION_EVENTS_NEXT_FRAME();

	} // namespace EVENT

	namespace EXTRAMETADATA {

		/**
		 * Character types:
		 * 0 = Michael,
		 * 1 = Franklin,
		 * 2 = Trevor,
		 * 3 = MPMale,
		 * 4 = MPFemale
		 */
		int GET_NUM_TATTOO_SHOP_DLC_ITEMS(int character);
		/**
		 * Character types:
		 * 0 = Michael,
		 * 1 = Franklin,
		 * 2 = Trevor,
		 * 3 = MPMale,
		 * 4 = MPFemale
		 * 
		 * 
		 * enum TattooZoneData
		 * {
		 * ZONE_TORSO = 0,
		 * ZONE_HEAD = 1,
		 * ZONE_LEFT_ARM = 2,
		 * ZONE_RIGHT_ARM = 3,
		 * ZONE_LEFT_LEG = 4,
		 * ZONE_RIGHT_LEG = 5,
		 * ZONE_UNKNOWN = 6,
		 * ZONE_NONE = 7,
		 * };
		 * struct outComponent
		 * {
		 * // these vars are suffixed with 4 bytes of padding each.
		 * uint unk;
		 * int unk2;
		 * uint tattooCollectionHash;
		 * uint tattooNameHash;
		 * int unk3;
		 * TattooZoneData zoneId;
		 * uint unk4;
		 * uint unk5;
		 * // maybe more, not sure exactly, decompiled scripts are very vague around this part.
		 * }
		 */
		BOOL GET_TATTOO_SHOP_DLC_ITEM_DATA(int characterType, int decorationIndex, Any* outComponent);
		/**
		 * Returns some sort of index/offset for overlays/decorations.
		 * 
		 * Character types:
		 * 0 = Michael,
		 * 1 = Franklin,
		 * 2 = Trevor,
		 * 3 = MPMale,
		 * 4 = MPFemale
		 */
		int GET_TATTOO_SHOP_DLC_ITEM_INDEX(Hash overlayHash, Any p1, int character);
		void INIT_SHOP_PED_COMPONENT(Any* outComponent);
		void INIT_SHOP_PED_PROP(Any* outProp);
		int SETUP_SHOP_PED_APPAREL_QUERY(int p0, int p1, int p2, int p3);
		/**
		 * character is 0 for Michael, 1 for Franklin, 2 for Trevor, 3 for freemode male, and 4 for freemode female.
		 * 
		 * componentId is between 0 and 11 and corresponds to the usual component slots.
		 * 
		 * p1 could be the outfit number; unsure.
		 * 
		 * p2 is usually -1; unknown function.
		 * 
		 * p3 appears to be for selecting between clothes and props; false is used with components/clothes, true is used with props.
		 * 
		 * p4 is usually -1; unknown function.
		 * 
		 * componentId is -1 when p3 is true in decompiled scripts.
		 */
		int SETUP_SHOP_PED_APPAREL_QUERY_TU(int character, int p1, int p2, BOOL p3, int p4, int componentId);
		/**
		 * See https://git.io/JtcRf for example and structs.
		 */
		void GET_SHOP_PED_QUERY_COMPONENT(int componentId, Any* outComponent);
		/**
		 * Returns some sort of index/offset for components.
		 * Needs _GET_NUM_PROPS_FROM_OUTFIT to be called with p3 = false and componentId with the drawable's component slot first, returns -1 otherwise.
		 */
		int GET_SHOP_PED_QUERY_COMPONENT_INDEX(Hash componentHash);
		/**
		 * More info here: https://gist.github.com/root-cause/3b80234367b0c856d60bf5cb4b826f86
		 */
		void GET_SHOP_PED_COMPONENT(Hash componentHash, Any* outComponent);
		/**
		 * See https://git.io/JtcRf for example and structs.
		 */
		void GET_SHOP_PED_QUERY_PROP(int componentId, Any* outProp);
		/**
		 * Returns some sort of index/offset for props.
		 * Needs _GET_NUM_PROPS_FROM_OUTFIT to be called with p3 = true and componentId = -1 first, returns -1 otherwise.
		 */
		int GET_SHOP_PED_QUERY_PROP_INDEX(Hash componentHash);
		/**
		 * More info here: https://gist.github.com/root-cause/3b80234367b0c856d60bf5cb4b826f86
		 */
		void GET_SHOP_PED_PROP(Hash componentHash, Any* outProp);
		Hash GET_HASH_NAME_FOR_COMPONENT(Entity entity, int componentId, int drawableVariant, int textureVariant);
		Hash GET_HASH_NAME_FOR_PROP(Entity entity, int componentId, int propIndex, int propTextureIndex);
		int GET_SHOP_PED_APPAREL_VARIANT_COMPONENT_COUNT(Hash componentHash);
		/**
		 * `propHash`: Ped helmet prop hash?
		 * This native returns 1 when the player helmet has a visor (there is another prop index for the same helmet with closed/opened visor variant) that can be toggled. 0 if there's no alternative version with a visor for this helmet prop.
		 */
		int GET_SHOP_PED_APPAREL_VARIANT_PROP_COUNT(Hash propHash);
		void GET_VARIANT_COMPONENT(Hash componentHash, int variantComponentIndex, Hash* nameHash, int* enumValue, int* componentType);
		void GET_VARIANT_PROP(Hash componentHash, int variantPropIndex, Hash* nameHash, int* enumValue, int* anchorPoint);
		/**
		 * Returns number of possible values of the forcedComponentIndex argument of GET_FORCED_COMPONENT.
		 */
		int GET_SHOP_PED_APPAREL_FORCED_COMPONENT_COUNT(Hash componentHash);
		/**
		 * Returns number of possible values of the forcedPropIndex argument of GET_FORCED_PROP.
		 */
		int GET_SHOP_PED_APPAREL_FORCED_PROP_COUNT(Hash componentHash);
		void GET_FORCED_COMPONENT(Hash componentHash, int forcedComponentIndex, Hash* nameHash, int* enumValue, int* componentType);
		void GET_FORCED_PROP(Hash componentHash, int forcedPropIndex, Hash* nameHash, int* enumValue, int* anchorPoint);
		/**
		 * Full list of restriction tags by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/pedApparelRestrictionTags.json
		 * 
		 * componentId/last parameter seems to be unused.
		 */
		BOOL DOES_SHOP_PED_APPAREL_HAVE_RESTRICTION_TAG(Hash componentHash, Hash restrictionTagHash, int componentId);
		BOOL DOES_CURRENT_PED_COMPONENT_HAVE_RESTRICTION_TAG(Ped ped, int componentId, Hash restrictionTagHash);
		BOOL DOES_CURRENT_PED_PROP_HAVE_RESTRICTION_TAG(Ped ped, int componentId, Hash restrictionTagHash);
		/**
		 * characters
		 * 
		 * 0: Michael
		 * 1: Franklin
		 * 2: Trevor
		 * 3: MPMale
		 * 4: MPFemale
		 */
		int SETUP_SHOP_PED_OUTFIT_QUERY(int character, BOOL p1);
		/**
		 * outfitIndex: from 0 to SETUP_SHOP_PED_OUTFIT_QUERY(characterIndex, false) - 1.
		 * See https://git.io/JtcB8 for example and outfit struct.
		 */
		void GET_SHOP_PED_QUERY_OUTFIT(int outfitIndex, Any* outfit);
		void GET_SHOP_PED_OUTFIT(Any p0, Any* p1);
		int GET_SHOP_PED_OUTFIT_LOCATE(Any p0);
		/**
		 * See https://git.io/JtcBH for example and structs.
		 */
		BOOL GET_SHOP_PED_OUTFIT_PROP_VARIANT(Hash outfitHash, int variantIndex, Any* outPropVariant);
		/**
		 * See https://git.io/JtcBH for example and structs.
		 */
		BOOL GET_SHOP_PED_OUTFIT_COMPONENT_VARIANT(Hash outfitHash, int variantIndex, Any* outComponentVariant);
		int GET_NUM_DLC_VEHICLES();
		/**
		 * dlcVehicleIndex is 0 to GET_NUM_DLC_VEHICLS() - 1
		 */
		Hash GET_DLC_VEHICLE_MODEL(int dlcVehicleIndex);
		/**
		 * dlcVehicleIndex takes a number from 0 - GET_NUM_DLC_VEHICLES() - 1.
		 * outData is a struct of 3 8-byte items.
		 * The Second item in the struct *(Hash *)(outData + 1) is the vehicle hash.
		 */
		BOOL GET_DLC_VEHICLE_DATA(int dlcVehicleIndex, Any* outData);
		int GET_DLC_VEHICLE_FLAGS(int dlcVehicleIndex);
		/**
		 * Returns the total number of DLC weapons.
		 */
		int GET_NUM_DLC_WEAPONS();
		/**
		 * Returns the total number of DLC weapons that are available in SP (availableInSP field in shop_weapon.meta).
		 */
		int GET_NUM_DLC_WEAPONS_SP();
		/**
		 * 
		 * dlcWeaponIndex takes a number from 0 - GET_NUM_DLC_WEAPONS() - 1.
		 * struct DlcWeaponData
		 * {
		 * int emptyCheck; //use DLC1::IS_CONTENT_ITEM_LOCKED on this
		 * int padding1;
		 * int weaponHash;
		 * int padding2;
		 * int unk;
		 * int padding3;
		 * int weaponCost;
		 * int padding4;
		 * int ammoCost;
		 * int padding5;
		 * int ammoType;
		 * int padding6;
		 * int defaultClipSize;
		 * int padding7;
		 * char nameLabel[64];
		 * char descLabel[64];
		 * char desc2Label[64]; // usually "the" + name
		 * char upperCaseNameLabel[64];
		 * };
		 */
		BOOL GET_DLC_WEAPON_DATA(int dlcWeaponIndex, Any* outData);
		/**
		 * Same as GET_DLC_WEAPON_DATA but only works for DLC weapons that are available in SP.
		 */
		BOOL GET_DLC_WEAPON_DATA_SP(int dlcWeaponIndex, Any* outData);
		/**
		 * Returns the total number of DLC weapon components.
		 */
		int GET_NUM_DLC_WEAPON_COMPONENTS(int dlcWeaponIndex);
		/**
		 * Returns the total number of DLC weapon components that are available in SP.
		 */
		int GET_NUM_DLC_WEAPON_COMPONENTS_SP(int dlcWeaponIndex);
		/**
		 * p0 seems to be the weapon index
		 * p1 seems to be the weapon component index
		 * struct DlcComponentData{
		 * int attachBone;
		 * int padding1;
		 * int bActiveByDefault;
		 * int padding2;
		 * int unk;
		 * int padding3;
		 * int componentHash;
		 * int padding4;
		 * int unk2;
		 * int padding5;
		 * int componentCost;
		 * int padding6;
		 * char nameLabel[64];
		 * char descLabel[64];
		 * };
		 * 
		 */
		BOOL GET_DLC_WEAPON_COMPONENT_DATA(int dlcWeaponIndex, int dlcWeapCompIndex, Any* ComponentDataPtr);
		/**
		 * Same as GET_DLC_WEAPON_COMPONENT_DATA but only works for DLC components that are available in SP.
		 */
		BOOL GET_DLC_WEAPON_COMPONENT_DATA_SP(int dlcWeaponIndex, int dlcWeapCompIndex, Any* ComponentDataPtr);
		BOOL IS_CONTENT_ITEM_LOCKED(Hash itemHash);
		BOOL IS_DLC_VEHICLE_MOD(Hash hash);
		Hash GET_DLC_VEHICLE_MOD_LOCK_HASH(Hash hash);
		/**
		 * From fm_deathmatch_creator and fm_race_creator:
		 * 
		 * FILES::REVERT_CONTENT_CHANGESET_GROUP_FOR_ALL(joaat("GROUP_MAP_SP"));
		 * FILES::EXECUTE_CONTENT_CHANGESET_GROUP_FOR_ALL(joaat("GROUP_MAP"));
		 */
		void EXECUTE_CONTENT_CHANGESET_GROUP_FOR_ALL(Hash hash);
		/**
		 * From fm_deathmatch_creator and fm_race_creator:
		 * 
		 * FILES::REVERT_CONTENT_CHANGESET_GROUP_FOR_ALL(joaat("GROUP_MAP_SP"));
		 * FILES::EXECUTE_CONTENT_CHANGESET_GROUP_FOR_ALL(joaat("GROUP_MAP"));
		 */
		void REVERT_CONTENT_CHANGESET_GROUP_FOR_ALL(Hash hash);

	} // namespace EXTRAMETADATA

	namespace FIRE {

		/**
		 * Starts a fire:
		 * 
		 * xyz: Location of fire
		 * maxChildren: The max amount of times a fire can spread to other objects. Must be 25 or less, or the function will do nothing.
		 * isGasFire: Whether or not the fire is powered by gasoline.
		 */
		FireId START_SCRIPT_FIRE(float X, float Y, float Z, int maxChildren, BOOL isGasFire);
		void REMOVE_SCRIPT_FIRE(FireId fireHandle);
		FireId START_ENTITY_FIRE(Entity entity);
		void STOP_ENTITY_FIRE(Entity entity);
		BOOL IS_ENTITY_ON_FIRE(Entity entity);
		int GET_NUMBER_OF_FIRES_IN_RANGE(float x, float y, float z, float radius);
		void SET_FLAMMABILITY_MULTIPLIER(float p0);
		void STOP_FIRE_IN_RANGE(float x, float y, float z, float radius);
		/**
		 * Returns TRUE if it found something. FALSE if not.
		 */
		BOOL GET_CLOSEST_FIRE_POS(Vector3* outPosition, float x, float y, float z);
		/**
		 * BOOL isAudible = If explosion makes a sound.
		 * BOOL isInvisible = If the explosion is invisible or not.
		 * 
		 * explosionType: https://alloc8or.re/gta5/doc/enums/eExplosionTag.txt
		 */
		void ADD_EXPLOSION(float x, float y, float z, int explosionType, float damageScale, BOOL isAudible, BOOL isInvisible, float cameraShake, BOOL noDamage);
		/**
		 * isAudible: If explosion makes a sound.
		 * isInvisible: If the explosion is invisible or not.
		 * explosionType: See ADD_EXPLOSION.
		 */
		void ADD_OWNED_EXPLOSION(Ped ped, float x, float y, float z, int explosionType, float damageScale, BOOL isAudible, BOOL isInvisible, float cameraShake);
		/**
		 * isAudible: If explosion makes a sound.
		 * isInvisible: If the explosion is invisible or not.
		 * explosionType: See ADD_EXPLOSION.
		 */
		void ADD_EXPLOSION_WITH_USER_VFX(float x, float y, float z, int explosionType, Hash explosionFx, float damageScale, BOOL isAudible, BOOL isInvisible, float cameraShake);
		/**
		 * explosionType: See ADD_EXPLOSION.
		 */
		BOOL IS_EXPLOSION_IN_AREA(int explosionType, float x1, float y1, float z1, float x2, float y2, float z2);
		/**
		 * explosionType: See ADD_EXPLOSION.
		 */
		BOOL IS_EXPLOSION_ACTIVE_IN_AREA(int explosionType, float x1, float y1, float z1, float x2, float y2, float z2);
		/**
		 * explosionType: See ADD_EXPLOSION.
		 */
		BOOL IS_EXPLOSION_IN_SPHERE(int explosionType, float x, float y, float z, float radius);
		/**
		 * explosionType: See ADD_EXPLOSION.
		 */
		Entity GET_OWNER_OF_EXPLOSION_IN_SPHERE(int explosionType, float x, float y, float z, float radius);
		/**
		 * explosionType: See ADD_EXPLOSION, -1 for any explosion type
		 * 
		 */
		BOOL IS_EXPLOSION_IN_ANGLED_AREA(int explosionType, float x1, float y1, float z1, float x2, float y2, float z2, float width);
		/**
		 * Returns a handle to the first entity within the a circle spawned inside the 2 points from a radius.
		 * 
		 * explosionType: See ADD_EXPLOSION.
		 */
		Entity GET_OWNER_OF_EXPLOSION_IN_ANGLED_AREA(int explosionType, float x1, float y1, float z1, float x2, float y2, float z2, float radius);
		void _NETWORK_EXPECT_EXPLOSION_EVENTS_FOR_PLAYER(BOOL expect, Player player); // Missing in crossmap
		void _GET_MAXIMUM_NUMBER_OF_WATER_CANNONS(); // Missing in crossmap
		void _GET_WATER_CANNON_COORDS(int index); // Missing in crossmap

	} // namespace FIRE

	namespace GRAPHICS {

		/**
		 * NOTE: Debugging functions are not present in the retail version of the game.
		 */
		void SET_DEBUG_LINES_AND_SPHERES_DRAWING_ACTIVE(BOOL enabled);
		void DRAW_DEBUG_LINE(float x1, float y1, float z1, float x2, float y2, float z2, int r, int g, int b, int alpha);
		/**
		 * NOTE: Debugging functions are not present in the retail version of the game.
		 */
		void DRAW_DEBUG_LINE_WITH_TWO_COLOURS(float x1, float y1, float z1, float x2, float y2, float z2, int r1, int g1, int b1, int r2, int g2, int b2, int alpha1, int alpha2);
		/**
		 * NOTE: Debugging functions are not present in the retail version of the game.
		 */
		void DRAW_DEBUG_SPHERE(float x, float y, float z, float radius, int red, int green, int blue, int alpha);
		void DRAW_DEBUG_BOX(float x1, float y1, float z1, float x2, float y2, float z2, int r, int g, int b, int alpha);
		/**
		 * NOTE: Debugging functions are not present in the retail version of the game.
		 */
		void DRAW_DEBUG_CROSS(float x, float y, float z, float size, int red, int green, int blue, int alpha);
		/**
		 * NOTE: Debugging functions are not present in the retail version of the game.
		 */
		void DRAW_DEBUG_TEXT(const char* text, float x, float y, float z, int red, int green, int blue, int alpha);
		/**
		 * NOTE: Debugging functions are not present in the retail version of the game.
		 */
		void DRAW_DEBUG_TEXT_2D(const char* text, float x, float y, float z, int red, int green, int blue, int alpha);
		/**
		 * Draws a depth-tested line from one point to another.
		 * ----------------
		 * x1, y1, z1 : Coordinates for the first point
		 * x2, y2, z2 : Coordinates for the second point
		 * r, g, b, alpha : Color with RGBA-Values
		 * I recommend using a predefined function to call this.
		 * [VB.NET]
		 * Public Sub DrawLine(from As Vector3, [to] As Vector3, col As Color)
		 * [Function].Call(Hash.DRAW_LINE, from.X, from.Y, from.Z, [to].X, [to].Y, [to].Z, col.R, col.G, col.B, col.A)
		 * End Sub
		 * 
		 * [C#]
		 * public void DrawLine(Vector3 from, Vector3 to, Color col)
		 * {
		 * Function.Call(Hash.DRAW_LINE, from.X, from.Y, from.Z, to.X, to.Y, to.Z, col.R, col.G, col.B, col.A);
		 * }
		 */
		void DRAW_LINE(float x1, float y1, float z1, float x2, float y2, float z2, int red, int green, int blue, int alpha);
		/**
		 * x/y/z - Location of a vertex (in world coords), presumably.
		 * ----------------
		 * x1, y1, z1     : Coordinates for the first point
		 * x2, y2, z2     : Coordinates for the second point
		 * x3, y3, z3     : Coordinates for the third point
		 * r, g, b, alpha : Color with RGBA-Values
		 * 
		 * Keep in mind that only one side of the drawn triangle is visible: It's the side, in which the vector-product of the vectors heads to: (b-a)x(c-a) Or (b-a)x(c-b).
		 * But be aware: The function seems to work somehow differently. I have trouble having them drawn in rotated orientation. Try it yourself and if you somehow succeed, please edit this and post your solution.
		 * I recommend using a predefined function to call this.
		 * [VB.NET]
		 * Public Sub DrawPoly(a As Vector3, b As Vector3, c As Vector3, col As Color)
		 * [Function].Call(Hash.DRAW_POLY, a.X, a.Y, a.Z, b.X, b.Y, b.Z, c.X, c.Y, c.Z, col.R, col.G, col.B, col.A)
		 * End Sub
		 * 
		 * [C#]
		 * public void DrawPoly(Vector3 a, Vector3 b, Vector3 c, Color col)
		 * {
		 * Function.Call(Hash.DRAW_POLY, a.X, a.Y, a.Z, b.X, b.Y, b.Z, c.X, c.Y, c.Z, col.R, col.G, col.B, col.A);
		 * }
		 * BTW: Intersecting triangles are not supported: They overlap in the order they were called.
		 */
		void DRAW_POLY(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3, int red, int green, int blue, int alpha);
		/**
		 * Used for drawling Deadline trailing lights, see deadline.ytd
		 * 
		 * p15 through p23 are values that appear to be related to illiumation, scaling, and rotation; more testing required.
		 * For UVW mapping (u,v,w parameters), reference your favourite internet resource for more details.
		 */
		void DRAW_TEXTURED_POLY(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3, int red, int green, int blue, int alpha, const char* textureDict, const char* textureName, float u1, float v1, float w1, float u2, float v2, float w2, float u3, float v3, float w3);
		/**
		 * Used for drawling Deadline trailing lights, see deadline.ytd
		 * 
		 * Each vertex has its own colour that is blended/illuminated on the texture. Additionally, the R, G, and B components are floats that are int-casted internally.
		 * For UVW mapping (u,v,w parameters), reference your favourite internet resource for more details.
		 */
		void DRAW_TEXTURED_POLY_WITH_THREE_COLOURS(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3, float red1, float green1, float blue1, int alpha1, float red2, float green2, float blue2, int alpha2, float red3, float green3, float blue3, int alpha3, const char* textureDict, const char* textureName, float u1, float v1, float w1, float u2, float v2, float w2, float u3, float v3, float w3);
		/**
		 * x,y,z = start pos
		 * x2,y2,z2 = end pos
		 * 
		 * Draw's a 3D Box between the two x,y,z coords.
		 * --------------
		 * Keep in mind that the edges of the box do only align to the worlds base-vectors. Therefore something like rotation cannot be applied. That means this function is pretty much useless, unless you want a static unicolor box somewhere.
		 * I recommend using a predefined function to call this.
		 * [VB.NET]
		 * Public Sub DrawBox(a As Vector3, b As Vector3, col As Color)
		 * [Function].Call(Hash.DRAW_BOX,a.X, a.Y, a.Z,b.X, b.Y, b.Z,col.R, col.G, col.B, col.A)
		 * End Sub
		 * 
		 * [C#]
		 * public void DrawBox(Vector3 a, Vector3 b, Color col)
		 * {
		 * Function.Call(Hash.DRAW_BOX,a.X, a.Y, a.Z,b.X, b.Y, b.Z,col.R, col.G, col.B, col.A);
		 * }
		 */
		void DRAW_BOX(float x1, float y1, float z1, float x2, float y2, float z2, int red, int green, int blue, int alpha);
		void SET_BACKFACECULLING(BOOL toggle);
		void SET_DEPTHWRITING(BOOL toggle);
		BOOL BEGIN_TAKE_MISSION_CREATOR_PHOTO();
		int GET_STATUS_OF_TAKE_MISSION_CREATOR_PHOTO();
		void FREE_MEMORY_FOR_MISSION_CREATOR_PHOTO();
		BOOL LOAD_MISSION_CREATOR_PHOTO(Any* p0, Any p1, Any p2, Any p3);
		int GET_STATUS_OF_LOAD_MISSION_CREATOR_PHOTO(Any* p0);
		BOOL BEGIN_CREATE_MISSION_CREATOR_PHOTO_PREVIEW();
		int GET_STATUS_OF_CREATE_MISSION_CREATOR_PHOTO_PREVIEW();
		void FREE_MEMORY_FOR_MISSION_CREATOR_PHOTO_PREVIEW();
		BOOL BEGIN_TAKE_HIGH_QUALITY_PHOTO();
		int GET_STATUS_OF_TAKE_HIGH_QUALITY_PHOTO();
		void FREE_MEMORY_FOR_HIGH_QUALITY_PHOTO();
		void SET_TAKEN_PHOTO_IS_MUGSHOT(BOOL toggle);
		void SET_ARENA_THEME_AND_VARIATION_FOR_TAKEN_PHOTO(Any p0, int p1);
		void SET_ON_ISLAND_X_FOR_TAKEN_PHOTO(Any p0);
		/**
		 * 1 match in 1 script. cellphone_controller.
		 * p0 is -1 in scripts.
		 */
		BOOL SAVE_HIGH_QUALITY_PHOTO(int unused);
		int GET_STATUS_OF_SAVE_HIGH_QUALITY_PHOTO();
		BOOL BEGIN_CREATE_LOW_QUALITY_COPY_OF_PHOTO(Any p0);
		int GET_STATUS_OF_CREATE_LOW_QUALITY_COPY_OF_PHOTO(int p0);
		void FREE_MEMORY_FOR_LOW_QUALITY_PHOTO();
		void DRAW_LOW_QUALITY_PHOTO_TO_PHONE(BOOL p0, BOOL p1);
		/**
		 * This function is hard-coded to always return 0.
		 */
		int GET_MAXIMUM_NUMBER_OF_PHOTOS();
		/**
		 * This function is hard-coded to always return 96.
		 */
		int GET_MAXIMUM_NUMBER_OF_CLOUD_PHOTOS();
		int GET_CURRENT_NUMBER_OF_CLOUD_PHOTOS();
		/**
		 * 2 matches across 2 scripts. Only showed in appcamera & appmedia. Both were 0.
		 */
		BOOL QUEUE_OPERATION_TO_CREATE_SORTED_LIST_OF_PHOTOS(Any p0);
		/**
		 * 3 matches across 3 scripts. First 2 were 0, 3rd was 1. Possibly a bool.
		 * appcamera, appmedia, and cellphone_controller.
		 */
		int GET_STATUS_OF_SORTED_LIST_OPERATION(Any p0);
		void CLEAR_STATUS_OF_SORTED_LIST_OPERATION();
		/**
		 * This function is hard-coded to always return 0.
		 */
		BOOL DOES_THIS_PHOTO_SLOT_CONTAIN_A_VALID_PHOTO(Any p0);
		/**
		 * This function is hard-coded to always return 0.
		 */
		BOOL LOAD_HIGH_QUALITY_PHOTO(Any p0);
		/**
		 * Hardcoded to always return 2.
		 */
		int GET_LOAD_HIGH_QUALITY_PHOTO_STATUS(int p0);
		void DRAW_LIGHT_WITH_RANGEEX(float x, float y, float z, int r, int g, int b, float range, float intensity, float shadow);
		void DRAW_LIGHT_WITH_RANGE(float posX, float posY, float posZ, int colorR, int colorG, int colorB, float range, float intensity);
		/**
		 * Parameters:
		 * * pos - coordinate where the spotlight is located
		 * * dir - the direction vector the spotlight should aim at from its current position
		 * * r,g,b - color of the spotlight
		 * * distance - the maximum distance the light can reach
		 * * brightness - the brightness of the light
		 * * roundness - "smoothness" of the circle edge
		 * * radius - the radius size of the spotlight
		 * * falloff - the falloff size of the light's edge (example: www.i.imgur.com/DemAWeO.jpg)
		 * 
		 * Example in C# (spotlight aims at the closest vehicle):
		 * Vector3 myPos = Game.Player.Character.Position;
		 * Vehicle nearest = World.GetClosestVehicle(myPos , 1000f);
		 * Vector3 destinationCoords = nearest.Position;
		 * Vector3 dirVector = destinationCoords - myPos;
		 * dirVector.Normalize();
		 * Function.Call(Hash.DRAW_SPOT_LIGHT, pos.X, pos.Y, pos.Z, dirVector.X, dirVector.Y, dirVector.Z, 255, 255, 255, 100.0f, 1f, 0.0f, 13.0f, 1f);
		 */
		void DRAW_SPOT_LIGHT(float posX, float posY, float posZ, float dirX, float dirY, float dirZ, int colorR, int colorG, int colorB, float distance, float brightness, float hardness, float radius, float falloff);
		void DRAW_SHADOWED_SPOT_LIGHT(float posX, float posY, float posZ, float dirX, float dirY, float dirZ, int colorR, int colorG, int colorB, float distance, float brightness, float roundness, float radius, float falloff, int shadowId);
		void FADE_UP_PED_LIGHT(float p0);
		void UPDATE_LIGHTS_ON_ENTITY(Entity entity);
		void SET_LIGHT_OVERRIDE_MAX_INTENSITY_SCALE(Any p0);
		float GET_LIGHT_OVERRIDE_MAX_INTENSITY_SCALE();
		/**
		 * enum MarkerTypes
		 * {
		 * MarkerTypeUpsideDownCone = 0,
		 * MarkerTypeVerticalCylinder = 1,
		 * MarkerTypeThickChevronUp = 2,
		 * MarkerTypeThinChevronUp = 3,
		 * MarkerTypeCheckeredFlagRect = 4,
		 * MarkerTypeCheckeredFlagCircle = 5,
		 * MarkerTypeVerticleCircle = 6,
		 * MarkerTypePlaneModel = 7,
		 * MarkerTypeLostMCDark = 8,
		 * MarkerTypeLostMCLight = 9,
		 * MarkerTypeNumber0 = 10,
		 * MarkerTypeNumber1 = 11,
		 * MarkerTypeNumber2 = 12,
		 * MarkerTypeNumber3 = 13,
		 * MarkerTypeNumber4 = 14,
		 * MarkerTypeNumber5 = 15,
		 * MarkerTypeNumber6 = 16,
		 * MarkerTypeNumber7 = 17,
		 * MarkerTypeNumber8 = 18,
		 * MarkerTypeNumber9 = 19,
		 * MarkerTypeChevronUpx1 = 20,
		 * MarkerTypeChevronUpx2 = 21,
		 * MarkerTypeChevronUpx3 = 22,
		 * MarkerTypeHorizontalCircleFat = 23,
		 * MarkerTypeReplayIcon = 24,
		 * MarkerTypeHorizontalCircleSkinny = 25,
		 * MarkerTypeHorizontalCircleSkinny_Arrow = 26,
		 * MarkerTypeHorizontalSplitArrowCircle = 27,
		 * MarkerTypeDebugSphere = 28,
		 * MarkerTypeDallorSign = 29,
		 * MarkerTypeHorizontalBars = 30,
		 * MarkerTypeWolfHead = 31
		 * };
		 * 
		 * dirX/Y/Z represent a heading on each axis in which the marker should face, alternatively you can rotate each axis independently with rotX/Y/Z (and set dirX/Y/Z all to 0).
		 * 
		 * faceCamera - Rotates only the y-axis (the heading) towards the camera
		 * 
		 * p19 - no effect, default value in script is 2
		 * 
		 * rotate - Rotates only on the y-axis (the heading)
		 * 
		 * textureDict - Name of texture dictionary to load texture from (e.g. "GolfPutting")
		 * 
		 * textureName - Name of texture inside dictionary to load (e.g. "PuttingMarker")
		 * 
		 * drawOnEnts - Draws the marker onto any entities that intersect it
		 * 
		 * basically what he said, except textureDict and textureName are totally not const char*, or if so, then they are always set to 0/NULL/nullptr in every script I checked, eg:
		 * 
		 * bj.c: graphics::draw_marker(6, vParam0, 0f, 0f, 1f, 0f, 0f, 0f, 4f, 4f, 4f, 240, 200, 80, iVar1, 0, 0, 2, 0, 0, 0, false);
		 * 
		 * his is what I used to draw an amber downward pointing chevron "V", has to be redrawn every frame.  The 180 is for 180 degrees rotation around the Y axis, the 50 is alpha, assuming max is 100, but it will accept 255.
		 * 
		 * GRAPHICS::DRAW_MARKER(2, v.x, v.y, v.z + 2, 0, 0, 0, 0, 180, 0, 2, 2, 2, 255, 128, 0, 50, 0, 1, 1, 0, 0, 0, 0);
		 * 
		 * 
		 */
		void DRAW_MARKER(int type, float posX, float posY, float posZ, float dirX, float dirY, float dirZ, float rotX, float rotY, float rotZ, float scaleX, float scaleY, float scaleZ, int red, int green, int blue, int alpha, BOOL bobUpAndDown, BOOL faceCamera, int p19, BOOL rotate, const char* textureDict, const char* textureName, BOOL drawOnEnts);
		void DRAW_MARKER_EX(int type, float posX, float posY, float posZ, float dirX, float dirY, float dirZ, float rotX, float rotY, float rotZ, float scaleX, float scaleY, float scaleZ, int red, int green, int blue, int alpha, BOOL bobUpAndDown, BOOL faceCamera, Any p19, BOOL rotate, const char* textureDict, const char* textureName, BOOL drawOnEnts, BOOL p24, BOOL p25);
		/**
		 * Draws a 3D sphere, typically seen in the GTA:O freemode event "Penned In".
		 * Example https://imgur.com/nCbtS4H
		 * 
		 * alpha - The alpha for the sphere. Goes from 0.0 to 1.0.
		 */
		void DRAW_MARKER_SPHERE(float x, float y, float z, float radius, int red, int green, int blue, float alpha);
		/**
		 * Creates a checkpoint. Returns the handle of the checkpoint.
		 * 
		 * 20/03/17 : Attention, checkpoints are already handled by the game itself, so you must not loop it like markers.
		 * 
		 * Parameters:
		 * * type - The type of checkpoint to create. See below for a list of checkpoint types.
		 * * pos1 - The position of the checkpoint.
		 * * pos2 - The position of the next checkpoint to point to.
		 * * radius - The radius of the checkpoint.
		 * * color - The color of the checkpoint.
		 * * reserved - Special parameter, see below for details. Usually set to 0 in the scripts.
		 * 
		 * Checkpoint types:
		 * 0-4---------Cylinder: 1 arrow, 2 arrow, 3 arrows, CycleArrow, Checker
		 * 5-9---------Cylinder: 1 arrow, 2 arrow, 3 arrows, CycleArrow, Checker
		 * 10-14-------Ring: 1 arrow, 2 arrow, 3 arrows, CycleArrow, Checker
		 * 15-19-------1 arrow, 2 arrow, 3 arrows, CycleArrow, Checker
		 * 20-24-------Cylinder: 1 arrow, 2 arrow, 3 arrows, CycleArrow, Checker
		 * 25-29-------Cylinder: 1 arrow, 2 arrow, 3 arrows, CycleArrow, Checker
		 * 30-34-------Cylinder: 1 arrow, 2 arrow, 3 arrows, CycleArrow, Checker
		 * 35-38-------Ring: Airplane Up, Left, Right, UpsideDown
		 * 39----------?
		 * 40----------Ring: just a ring
		 * 41----------?
		 * 42-44-------Cylinder w/ number (uses 'reserved' parameter)
		 * 45-47-------Cylinder no arrow or number
		 * 
		 * If using type 42-44, reserved sets number / number and shape to display
		 * 
		 * 0-99------------Just numbers (0-99)
		 * 100-109-----------------Arrow (0-9)
		 * 110-119------------Two arrows (0-9)
		 * 120-129----------Three arrows (0-9)
		 * 130-139----------------Circle (0-9)
		 * 140-149------------CycleArrow (0-9)
		 * 150-159----------------Circle (0-9)
		 * 160-169----Circle  w/ pointer (0-9)
		 * 170-179-------Perforated ring (0-9)
		 * 180-189----------------Sphere (0-9)
		 */
		int CREATE_CHECKPOINT(int type, float posX1, float posY1, float posZ1, float posX2, float posY2, float posZ2, float diameter, int red, int green, int blue, int alpha, int reserved);
		void SET_CHECKPOINT_INSIDE_CYLINDER_HEIGHT_SCALE(int checkpoint, float scale);
		void SET_CHECKPOINT_INSIDE_CYLINDER_SCALE(int checkpoint, float scale);
		/**
		 * Sets the cylinder height of the checkpoint.
		 * 
		 * Parameters:
		 * * nearHeight - The height of the checkpoint when inside of the radius.
		 * * farHeight - The height of the checkpoint when outside of the radius.
		 * * radius - The radius of the checkpoint.
		 */
		void SET_CHECKPOINT_CYLINDER_HEIGHT(int checkpoint, float nearHeight, float farHeight, float radius);
		/**
		 * Sets the checkpoint color.
		 */
		void SET_CHECKPOINT_RGBA(int checkpoint, int red, int green, int blue, int alpha);
		/**
		 * Sets the checkpoint icon color.
		 */
		void SET_CHECKPOINT_RGBA2(int checkpoint, int red, int green, int blue, int alpha);
		/**
		 * This does not move an existing checkpoint... so wtf.
		 */
		void SET_CHECKPOINT_CLIPPLANE_WITH_POS_NORM(int checkpoint, float posX, float posY, float posZ, float unkX, float unkY, float unkZ);
		void SET_CHECKPOINT_FORCE_OLD_ARROW_POINTING(int checkpoint);
		/**
		 * Unknown. Called after creating a checkpoint (type: 51) in the creators.
		 */
		void SET_CHECKPOINT_DECAL_ROT_ALIGNED_TO_CAMERA_ROT(int checkpoint);
		void SET_CHECKPOINT_FORCE_DIRECTION(int checkpoint);
		void SET_CHECKPOINT_DIRECTION(int checkpoint, float posX, float posY, float posZ);
		void DELETE_CHECKPOINT(int checkpoint);
		void DONT_RENDER_IN_GAME_UI(BOOL p0);
		void FORCE_RENDER_IN_GAME_UI(BOOL toggle);
		/**
		 * This function can requests texture dictonaries from following RPFs:
		 * scaleform_generic.rpf
		 * scaleform_minigames.rpf
		 * scaleform_minimap.rpf
		 * scaleform_web.rpf
		 * 
		 * last param isnt a toggle
		 */
		void REQUEST_STREAMED_TEXTURE_DICT(const char* textureDict, BOOL p1);
		BOOL HAS_STREAMED_TEXTURE_DICT_LOADED(const char* textureDict);
		void SET_STREAMED_TEXTURE_DICT_AS_NO_LONGER_NEEDED(const char* textureDict);
		/**
		 * Draws a rectangle on the screen.
		 * 
		 * -x: The relative X point of the center of the rectangle. (0.0-1.0, 0.0 is the left edge of the screen, 1.0 is the right edge of the screen)
		 * 
		 * -y: The relative Y point of the center of the rectangle. (0.0-1.0, 0.0 is the top edge of the screen, 1.0 is the bottom edge of the screen)
		 * 
		 * -width: The relative width of the rectangle. (0.0-1.0, 1.0 means the whole screen width)
		 * 
		 * -height: The relative height of the rectangle. (0.0-1.0, 1.0 means the whole screen height)
		 * 
		 * -R: Red part of the color. (0-255)
		 * 
		 * -G: Green part of the color. (0-255)
		 * 
		 * -B: Blue part of the color. (0-255)
		 * 
		 * -A: Alpha part of the color. (0-255, 0 means totally transparent, 255 means totally opaque)
		 * 
		 * The total number of rectangles to be drawn in one frame is apparently limited to 399.
		 * 
		 */
		void DRAW_RECT(float x, float y, float width, float height, int r, int g, int b, int a, BOOL p8);
		/**
		 * Sets a flag defining whether or not script draw commands should continue being drawn behind the pause menu. This is usually used for TV channels and other draw commands that are used with a world render target.
		 */
		void SET_SCRIPT_GFX_DRAW_BEHIND_PAUSEMENU(BOOL toggle);
		/**
		 * Sets the draw order for script draw commands.
		 * 
		 * Examples from decompiled scripts:
		 * GRAPHICS::SET_SCRIPT_GFX_DRAW_ORDER(7);
		 * GRAPHICS::DRAW_RECT(0.5, 0.5, 3.0, 3.0, v_4, v_5, v_6, a_0._f172, 0);
		 * 
		 * GRAPHICS::SET_SCRIPT_GFX_DRAW_ORDER(1);
		 * GRAPHICS::DRAW_RECT(0.5, 0.5, 1.5, 1.5, 0, 0, 0, 255, 0);
		 */
		void SET_SCRIPT_GFX_DRAW_ORDER(int drawOrder);
		/**
		 * horizontalAlign: The horizontal alignment. This can be 67 ('C'), 76 ('L'), or 82 ('R').
		 * verticalAlign: The vertical alignment. This can be 67 ('C'), 66 ('B'), or 84 ('T').
		 * 
		 * This function anchors script draws to a side of the safe zone. This needs to be called to make the interface independent of the player's safe zone configuration.
		 * 
		 * These values are equivalent to alignX and alignY in common:/data/ui/frontend.xml, which can be used as a baseline for default alignment.
		 * 
		 * Using any other value (including 0) will result in the safe zone not being taken into account for this draw. The canonical value for this is 'I' (73).
		 * 
		 * For example, you can use SET_SCRIPT_GFX_ALIGN(0, 84) to only scale on the Y axis (to the top), but not change the X axis.
		 * 
		 * To reset the value, use RESET_SCRIPT_GFX_ALIGN.
		 */
		void SET_SCRIPT_GFX_ALIGN(int horizontalAlign, int verticalAlign);
		/**
		 * This function resets the alignment set using SET_SCRIPT_GFX_ALIGN and SET_SCRIPT_GFX_ALIGN_PARAMS to the default values ('I', 'I'; 0, 0, 0, 0).
		 * This should be used after having used the aforementioned functions in order to not affect any other scripts attempting to draw.
		 */
		void RESET_SCRIPT_GFX_ALIGN();
		/**
		 * Sets the draw offset/calculated size for SET_SCRIPT_GFX_ALIGN. If using any alignment other than left/top, the game expects the width/height to be configured using this native in order to get a proper starting position for the draw command.
		 */
		void SET_SCRIPT_GFX_ALIGN_PARAMS(float x, float y, float w, float h);
		/**
		 * Calculates the effective X/Y fractions when applying the values set by SET_SCRIPT_GFX_ALIGN and SET_SCRIPT_GFX_ALIGN_PARAMS
		 */
		void GET_SCRIPT_GFX_ALIGN_POSITION(float x, float y, float* calculatedX, float* calculatedY);
		/**
		 * Gets the scale of safe zone. if the safe zone size scale is max, it will return 1.0.
		 */
		float GET_SAFE_ZONE_SIZE();
		/**
		 * Draws a 2D sprite on the screen.
		 * 
		 * Parameters:
		 * textureDict - Name of texture dictionary to load texture from (e.g. "CommonMenu", "MPWeaponsCommon", etc.)
		 * 
		 * textureName - Name of texture to load from texture dictionary (e.g. "last_team_standing_icon", "tennis_icon", etc.)
		 * 
		 * screenX/Y - Screen offset (0.5 = center)
		 * scaleX/Y - Texture scaling. Negative values can be used to flip the texture on that axis. (0.5 = half)
		 * 
		 * heading - Texture rotation in degrees (default = 0.0) positive is clockwise, measured in degrees
		 * 
		 * red,green,blue - Sprite color (default = 255/255/255)
		 * 
		 * alpha - opacity level
		 */
		void DRAW_SPRITE(const char* textureDict, const char* textureName, float screenX, float screenY, float width, float height, float heading, int red, int green, int blue, int alpha, BOOL p11, Any p12);
		/**
		 * Used in arcade games and Beam hack minigame in Doomsday Heist. I will most certainly dive into this to try replicate arcade games.
		 * x position must be between 0.0 and 1.0 (1.0 being the most right side of the screen)
		 * y position must be between 0.0 and 1.0 (1.0 being the most bottom side of the screen)
		 * width 0.0 - 1.0 is the reasonable amount generally
		 * height 0.0 - 1.0 is the reasonable amount generally
		 * p6 almost always 0.0
		 * p11 seems to be unknown but almost always 0 int
		 */
		void DRAW_SPRITE_ARX(const char* textureDict, const char* textureName, float x, float y, float width, float height, float p6, int red, int green, int blue, int alpha, Any p11, Any p12);
		/**
		 * Similar to _DRAW_SPRITE, but seems to be some kind of "interactive" sprite, at least used by render targets.
		 * These seem to be the only dicts ever requested by this native:
		 * 
		 * prop_screen_biker_laptop
		 * Prop_Screen_GR_Disruption
		 * Prop_Screen_TaleOfUs
		 * prop_screen_nightclub
		 * Prop_Screen_IE_Adhawk
		 * prop_screen_sm_free_trade_shipping
		 * prop_screen_hacker_truck
		 * MPDesktop
		 * Prop_Screen_Nightclub
		 * And a few others
		 * 
		 */
		void DRAW_SPRITE_NAMED_RENDERTARGET(const char* textureDict, const char* textureName, float screenX, float screenY, float width, float height, float heading, int red, int green, int blue, int alpha, Any p11);
		/**
		 * Similar to DRAW_SPRITE, but allows to specify the texture coordinates used to draw the sprite.
		 * 
		 * u1, v1 - texture coordinates for the top-left corner
		 * u2, v2 - texture coordinates for the bottom-right corner
		 */
		void DRAW_SPRITE_ARX_WITH_UV(const char* textureDict, const char* textureName, float x, float y, float width, float height, float u1, float v1, float u2, float v2, float heading, int red, int green, int blue, int alpha, Any p15);
		/**
		 * Example:
		 * GRAPHICS::ADD_ENTITY_ICON(a_0, "MP_Arrow");
		 * 
		 * I tried this and nothing happened...
		 */
		int ADD_ENTITY_ICON(Entity entity, const char* icon);
		void SET_ENTITY_ICON_VISIBILITY(Entity entity, BOOL toggle);
		void SET_ENTITY_ICON_COLOR(Entity entity, int red, int green, int blue, int alpha);
		/**
		 * Sets the on-screen drawing origin for draw-functions (which is normally x=0,y=0 in the upper left corner of the screen) to a world coordinate.
		 * From now on, the screen coordinate which displays the given world coordinate on the screen is seen as x=0,y=0.
		 * 
		 * Example in C#:
		 * Vector3 boneCoord = somePed.GetBoneCoord(Bone.SKEL_Head);
		 * Function.Call(Hash.SET_DRAW_ORIGIN, boneCoord.X, boneCoord.Y, boneCoord.Z, 0);
		 * Function.Call(Hash.DRAW_SPRITE, "helicopterhud", "hud_corner", -0.01, -0.015, 0.013, 0.013, 0.0, 255, 0, 0, 200);
		 * Function.Call(Hash.DRAW_SPRITE, "helicopterhud", "hud_corner", 0.01, -0.015, 0.013, 0.013, 90.0, 255, 0, 0, 200);
		 * Function.Call(Hash.DRAW_SPRITE, "helicopterhud", "hud_corner", -0.01, 0.015, 0.013, 0.013, 270.0, 255, 0, 0, 200);
		 * Function.Call(Hash.DRAW_SPRITE, "helicopterhud", "hud_corner", 0.01, 0.015, 0.013, 0.013, 180.0, 255, 0, 0, 200);
		 * Function.Call(Hash.CLEAR_DRAW_ORIGIN);
		 * 
		 * Result: www11.pic-upload.de/19.06.15/bkqohvil2uao.jpg
		 * If the pedestrian starts walking around now, the sprites are always around her head, no matter where the head is displayed on the screen.
		 * 
		 * This function also effects the drawing of texts and other UI-elements.
		 * The effect can be reset by calling GRAPHICS::CLEAR_DRAW_ORIGIN().
		 */
		void SET_DRAW_ORIGIN(float x, float y, float z, BOOL p3);
		/**
		 * Resets the screen's draw-origin which was changed by the function GRAPHICS::SET_DRAW_ORIGIN(...) back to x=0,y=0.
		 * 
		 * See GRAPHICS::SET_DRAW_ORIGIN(...) for further information.
		 */
		void CLEAR_DRAW_ORIGIN();
		int SET_BINK_MOVIE(const char* name);
		void PLAY_BINK_MOVIE(int binkMovie);
		void STOP_BINK_MOVIE(int binkMovie);
		void RELEASE_BINK_MOVIE(int binkMovie);
		void DRAW_BINK_MOVIE(int binkMovie, float p1, float p2, float p3, float p4, float p5, int r, int g, int b, int a);
		/**
		 * In percentage: 0.0 - 100.0
		 */
		void SET_BINK_MOVIE_TIME(int binkMovie, float progress);
		/**
		 * In percentage: 0.0 - 100.0
		 */
		float GET_BINK_MOVIE_TIME(int binkMovie);
		/**
		 * binkMovie: Is return value from _SET_BINK_MOVIE. Has something to do with bink volume? (audRequestedSettings::SetVolumeCurveScale)
		 */
		void SET_BINK_MOVIE_VOLUME(int binkMovie, float value);
		void ATTACH_TV_AUDIO_TO_ENTITY(Entity entity);
		void SET_BINK_MOVIE_AUDIO_FRONTEND(int binkMovie, BOOL p1);
		/**
		 * Probably changes tvs from being a 3d audio to being "global" audio
		 */
		void SET_TV_AUDIO_FRONTEND(BOOL toggle);
		void SET_BINK_SHOULD_SKIP(int binkMovie, BOOL bShouldSkip);
		int LOAD_MOVIE_MESH_SET(const char* movieMeshSetName);
		void RELEASE_MOVIE_MESH_SET(int movieMeshSet);
		int QUERY_MOVIE_MESH_SET_STATE(Any p0);
		/**
		 * int screenresx,screenresy;
		 * GET_SCREEN_RESOLUTION(&screenresx,&screenresy);
		 */
		void GET_SCREEN_RESOLUTION(int* x, int* y);
		/**
		 * Returns current screen resolution.
		 */
		void GET_ACTUAL_SCREEN_RESOLUTION(int* x, int* y);
		float GET_ASPECT_RATIO(BOOL b);
		float GET_SCREEN_ASPECT_RATIO();
		/**
		 * Setting Aspect Ratio Manually in game will return:
		 * 
		 * false - for Narrow format Aspect Ratios (3:2, 4:3, 5:4, etc. )
		 * true - for Wide format Aspect Ratios (5:3, 16:9, 16:10, etc. )
		 * 
		 * Setting Aspect Ratio to "Auto" in game will return "false" or "true" based on the actual set Resolution Ratio.
		 */
		BOOL GET_IS_WIDESCREEN();
		/**
		 * false = Any resolution < 1280x720
		 * true = Any resolution >= 1280x720
		 */
		BOOL GET_IS_HIDEF();
		void ADJUST_NEXT_POS_SIZE_AS_NORMALIZED_16_9();
		void DISPLAY_LOADING_SCREEN_NOW(BOOL onOff); // Missing in crossmap
		/**
		 * Enables Night Vision.
		 * 
		 * Example:
		 * C#: Function.Call(Hash.SET_NIGHTVISION, true);
		 * C++: GRAPHICS::SET_NIGHTVISION(true);
		 * 
		 * BOOL toggle:
		 * true = turns night vision on for your player.
		 * false = turns night vision off for your player.
		 */
		void SET_NIGHTVISION(BOOL toggle);
		BOOL GET_REQUESTINGNIGHTVISION();
		BOOL GET_USINGNIGHTVISION();
		void SET_EXPOSURETWEAK(BOOL toggle);
		void FORCE_EXPOSURE_READBACK(BOOL toggle);
		void OVERRIDE_NIGHTVISION_LIGHT_RANGE(float p0);
		void SET_NOISEOVERIDE(BOOL toggle);
		void SET_NOISINESSOVERIDE(float value);
		/**
		 * Convert a world coordinate into its relative screen coordinate.  (WorldToScreen)
		 * 
		 * Returns a boolean; whether or not the operation was successful. It will return false if the coordinates given are not visible to the rendering camera.
		 * 
		 * 
		 * For .NET users...
		 * 
		 * VB:
		 * Public Shared Function World3DToScreen2d(pos as vector3) As Vector2
		 * 
		 * Dim x2dp, y2dp As New Native.OutputArgument
		 * 
		 * Native.Function.Call(Of Boolean)(Native.Hash.GET_SCREEN_COORD_FROM_WORLD_COORD , pos.x, pos.y, pos.z, x2dp, y2dp)
		 * Return New Vector2(x2dp.GetResult(Of Single), y2dp.GetResult(Of Single))
		 * 
		 * End Function
		 * 
		 * C#:
		 * Vector2 World3DToScreen2d(Vector3 pos)
		 * {
		 * var x2dp = new OutputArgument();
		 * var y2dp = new OutputArgument();
		 * 
		 * Function.Call<bool>(Hash.GET_SCREEN_COORD_FROM_WORLD_COORD , pos.X, pos.Y, pos.Z, x2dp, y2dp);
		 * return new Vector2(x2dp.GetResult<float>(), y2dp.GetResult<float>());
		 * }
		 * //USE VERY SMALL VALUES FOR THE SCALE OF RECTS/TEXT because it is dramatically larger on screen than in 3D, e.g '0.05' small.
		 * 
		 * Used to be called _WORLD3D_TO_SCREEN2D
		 * 
		 * I thought we lost you from the scene forever. It does seem however that calling SET_DRAW_ORIGIN then your natives, then ending it. Seems to work better for certain things such as keeping boxes around people for a predator missile e.g.
		 */
		BOOL GET_SCREEN_COORD_FROM_WORLD_COORD(float worldX, float worldY, float worldZ, float* screenX, float* screenY);
		/**
		 * Returns the texture resolution of the passed texture dict+name.
		 * 
		 * Note: Most texture resolutions are doubled compared to the console version of the game.
		 */
		Vector3 GET_TEXTURE_RESOLUTION(const char* textureDict, const char* textureName);
		/**
		 * Overriding ped badge texture to a passed texture. It's synced between players (even custom textures!), don't forget to request used dict on *all* clients to make it sync properly. Can be removed by passing empty strings.
		 */
		BOOL OVERRIDE_PED_CREW_LOGO_TEXTURE(Ped ped, const char* txd, const char* txn);
		void SET_DISTANCE_BLUR_STRENGTH_OVERRIDE(float p0);
		/**
		 * Purpose of p0 and p1 unknown.
		 */
		void SET_FLASH(float p0, float p1, float fadeIn, float duration, float fadeOut);
		void DISABLE_OCCLUSION_THIS_FRAME();
		/**
		 * Does not affect weapons, particles, fire/explosions, flashlights or the sun.
		 * When set to true, all emissive textures (including ped components that have light effects), street lights, building lights, vehicle lights, etc will all be turned off.
		 * 
		 * Used in Humane Labs Heist for EMP.
		 * 
		 * state: True turns off all artificial light sources in the map: buildings, street lights, car lights, etc. False turns them back on.
		 */
		void SET_ARTIFICIAL_LIGHTS_STATE(BOOL state);
		/**
		 * If "blackout" is enabled, this native allows you to ignore "blackout" for vehicles.
		 */
		void SET_ARTIFICIAL_VEHICLE_LIGHTS_STATE(BOOL toggle);
		void DISABLE_HDTEX_THIS_FRAME();
		/**
		 * Creates a tracked point, useful for checking the visibility of a 3D point on screen.
		 */
		int CREATE_TRACKED_POINT();
		void SET_TRACKED_POINT_INFO(int point, float x, float y, float z, float radius);
		BOOL IS_TRACKED_POINT_VISIBLE(int point);
		void DESTROY_TRACKED_POINT(int point);
		/**
		 * This function is hard-coded to always return 0.
		 */
		int SET_GRASS_CULL_SPHERE(float p0, float p1, float p2, float p3);
		/**
		 * This native does absolutely nothing, just a nullsub
		 */
		void REMOVE_GRASS_CULL_SPHERE(int handle);
		void PROCGRASS_ENABLE_CULLSPHERE(int handle, float x, float y, float z, float scale);
		void PROCGRASS_DISABLE_CULLSPHERE(int handle);
		BOOL PROCGRASS_IS_CULLSPHERE_ENABLED(int handle);
		void PROCGRASS_ENABLE_AMBSCALESCAN();
		void PROCGRASS_DISABLE_AMBSCALESCAN();
		void DISABLE_PROCOBJ_CREATION();
		void ENABLE_PROCOBJ_CREATION();
		void GRASSBATCH_ENABLE_FLATTENING_EXT_IN_SPHERE(float x, float y, float z, Any p3, float p4, float p5, float p6, float scale);
		/**
		 * Wraps 0xAAE9BE70EC7C69AB with FLT_MAX as p7
		 */
		void GRASSBATCH_ENABLE_FLATTENING_IN_SPHERE(float x, float y, float z, float radius, float p4, float p5, float p6);
		void GRASSBATCH_DISABLE_FLATTENING();
		void CASCADE_SHADOWS_INIT_SESSION();
		void CASCADE_SHADOWS_SET_CASCADE_BOUNDS(Any p0, BOOL p1, float p2, float p3, float p4, float p5, BOOL p6, float p7);
		void CASCADE_SHADOWS_SET_CASCADE_BOUNDS_SCALE(float p0);
		void CASCADE_SHADOWS_SET_ENTITY_TRACKER_SCALE(float p0);
		void CASCADE_SHADOWS_SET_SPLIT_Z_EXP_WEIGHT(float p0);
		void CASCADE_SHADOWS_SET_BOUND_POSITION(Any p0);
		/**
		 * When this is set to ON, shadows only draw as you get nearer.
		 * 
		 * When OFF, they draw from a further distance.
		 */
		void CASCADE_SHADOWS_ENABLE_ENTITY_TRACKER(BOOL toggle);
		void CASCADE_SHADOWS_SET_SCREEN_SIZE_CHECK_ENABLED(BOOL p0);
		/**
		 * Possible values:
		 * "CSM_ST_POINT"
		 * "CSM_ST_LINEAR"
		 * "CSM_ST_TWOTAP"
		 * "CSM_ST_BOX3x3"
		 * "CSM_ST_BOX4x4"
		 * "CSM_ST_DITHER2_LINEAR"
		 * "CSM_ST_CUBIC"
		 * "CSM_ST_DITHER4"
		 * "CSM_ST_DITHER16"
		 * "CSM_ST_SOFT16"
		 * "CSM_ST_DITHER16_RPDB"
		 * "CSM_ST_POISSON16_RPDB_GNORM"
		 * "CSM_ST_HIGHRES_BOX4x4"
		 * "CSM_ST_CLOUDS_SIMPLE"
		 * "CSM_ST_CLOUDS_LINEAR"
		 * "CSM_ST_CLOUDS_TWOTAP"
		 * "CSM_ST_CLOUDS_BOX3x3"
		 * "CSM_ST_CLOUDS_BOX4x4"
		 * "CSM_ST_CLOUDS_DITHER2_LINEAR"
		 * "CSM_ST_CLOUDS_SOFT16"
		 * "CSM_ST_CLOUDS_DITHER16_RPDB"
		 * "CSM_ST_CLOUDS_POISSON16_RPDB_GNORM"
		 */
		void CASCADE_SHADOWS_SET_SHADOW_SAMPLE_TYPE(const char* type);
		void CASCADE_SHADOWS_CLEAR_SHADOW_SAMPLE_TYPE();
		void CASCADE_SHADOWS_SET_AIRCRAFT_MODE(BOOL p0);
		void CASCADE_SHADOWS_SET_DYNAMIC_DEPTH_MODE(BOOL p0);
		void CASCADE_SHADOWS_SET_DYNAMIC_DEPTH_VALUE(float p0);
		void CASCADE_SHADOWS_ENABLE_FREEZER(BOOL p0);
		void WATER_REFLECTION_SET_SCRIPT_OBJECT_VISIBILITY(Any p0);
		void GOLF_TRAIL_SET_ENABLED(BOOL toggle);
		/**
		 * p8 seems to always be false.
		 */
		void GOLF_TRAIL_SET_PATH(float p0, float p1, float p2, float p3, float p4, float p5, float p6, float p7, BOOL p8);
		void GOLF_TRAIL_SET_RADIUS(float p0, float p1, float p2);
		void GOLF_TRAIL_SET_COLOUR(int p0, int p1, int p2, int p3, int p4, int p5, int p6, int p7, int p8, int p9, int p10, int p11);
		void GOLF_TRAIL_SET_TESSELLATION(int p0, int p1);
		void GOLF_TRAIL_SET_FIXED_CONTROL_POINT_ENABLE(BOOL p0);
		/**
		 * 12 matches across 4 scripts. All 4 scripts were job creators.
		 * 
		 * type ranged from 0 - 2.
		 * p4 was always 0.2f. Likely scale.
		 * assuming p5 - p8 is RGBA, the graphic is always yellow (255, 255, 0, 255).
		 * 
		 * Tested but noticed nothing.
		 */
		void GOLF_TRAIL_SET_FIXED_CONTROL_POINT(int type, float xPos, float yPos, float zPos, float p4, int red, int green, int blue, int alpha);
		/**
		 * Only appeared in Golf & Golf_mp. Parameters were all ptrs
		 */
		void GOLF_TRAIL_SET_SHADER_PARAMS(float p0, float p1, float p2, float p3, float p4);
		void GOLF_TRAIL_SET_FACING(BOOL p0);
		float GOLF_TRAIL_GET_MAX_HEIGHT();
		Vector3 GOLF_TRAIL_GET_VISUAL_CONTROL_POINT(int p0);
		/**
		 * Toggles Heatvision on/off.
		 */
		void SET_SEETHROUGH(BOOL toggle);
		BOOL GET_USINGSEETHROUGH();
		void SEETHROUGH_RESET();
		void SEETHROUGH_SET_FADE_STARTDISTANCE(float distance);
		void SEETHROUGH_SET_FADE_ENDDISTANCE(float distance);
		float SEETHROUGH_GET_MAX_THICKNESS();
		/**
		 * 0.0 = you will not be able to see people behind the walls. 50.0 and more = you will see everyone through the walls. More value is "better" view. See https://gfycat.com/FirmFlippantGourami
		 * min: 1.0
		 * max: 10000.0
		 */
		void SEETHROUGH_SET_MAX_THICKNESS(float thickness);
		void SEETHROUGH_SET_NOISE_MIN(float amount);
		void SEETHROUGH_SET_NOISE_MAX(float amount);
		void SEETHROUGH_SET_HILIGHT_INTENSITY(float intensity);
		void SEETHROUGH_SET_HIGHLIGHT_NOISE(float noise);
		/**
		 * min: 0.0
		 * max: 0.75
		 */
		void SEETHROUGH_SET_HEATSCALE(int index, float heatScale);
		void SEETHROUGH_SET_COLOR_NEAR(int red, int green, int blue);
		/**
		 * Setter for GET_MOTIONBLUR_MAX_VEL_SCALER
		 */
		void SET_MOTIONBLUR_MAX_VEL_SCALER(float p0);
		/**
		 * Getter for SET_MOTIONBLUR_MAX_VEL_SCALER
		 */
		float GET_MOTIONBLUR_MAX_VEL_SCALER();
		void SET_FORCE_MOTIONBLUR(BOOL toggle);
		void TOGGLE_PLAYER_DAMAGE_OVERLAY(BOOL toggle);
		/**
		 * Sets an value related to timecycles.
		 */
		void RESET_ADAPTATION(int p0);
		/**
		 * time in ms to transition to fully blurred screen
		 */
		BOOL TRIGGER_SCREENBLUR_FADE_IN(float transitionTime);
		/**
		 * time in ms to transition from fully blurred to normal
		 */
		BOOL TRIGGER_SCREENBLUR_FADE_OUT(float transitionTime);
		void DISABLE_SCREENBLUR_FADE();
		float GET_SCREENBLUR_FADE_CURRENT_TIME();
		/**
		 * Returns whether screen transition to blur/from blur is running.
		 */
		BOOL IS_SCREENBLUR_FADE_RUNNING();
		void TOGGLE_PAUSED_RENDERPHASES(BOOL toggle);
		BOOL GET_TOGGLE_PAUSED_RENDERPHASES_STATUS();
		void RESET_PAUSED_RENDERPHASES();
		void GRAB_PAUSEMENU_OWNERSHIP();
		void SET_HIDOF_OVERRIDE(BOOL p0, BOOL p1, float nearplaneOut, float nearplaneIn, float farplaneOut, float farplaneIn);
		void SET_LOCK_ADAPTIVE_DOF_DISTANCE(BOOL p0);
		BOOL PHONEPHOTOEDITOR_TOGGLE(BOOL p0);
		BOOL PHONEPHOTOEDITOR_IS_ACTIVE();
		BOOL PHONEPHOTOEDITOR_SET_FRAME_TXD(const char* textureDict, BOOL p1);
		/**
		 * GRAPHICS::START_PARTICLE_FX_NON_LOOPED_AT_COORD("scr_paleto_roof_impact", -140.8576f, 6420.789f, 41.1391f, 0f, 0f, 267.3957f, 0x3F800000, 0, 0, 0);
		 * 
		 * Axis - Invert Axis Flags
		 * 
		 * Full list of particle effect dictionaries and effects by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/particleEffectsCompact.json
		 * 
		 * 
		 * -------------------------------------------------------------------
		 * C#
		 * 
		 * Function.Call<int>(Hash.START_PARTICLE_FX_NON_LOOPED_AT_COORD, = you are calling this function.
		 * 
		 * char *effectname = This is an in-game effect name, for e.g. "scr_fbi4_trucks_crash" is used to give the effects when truck crashes etc
		 * 
		 * float x, y, z pos = this one is Simple, you just have to declare, where do you want this effect to take place at, so declare the ordinates
		 * 
		 * float xrot, yrot, zrot = Again simple? just mention the value in case if you want the effect to rotate.
		 * 
		 * float scale = is declare the scale of the effect, this may vary as per the effects for e.g 1.0f
		 * 
		 * bool xaxis, yaxis, zaxis = To bool the axis values.
		 * 
		 * example:
		 * Function.Call<int>(Hash.START_PARTICLE_FX_NON_LOOPED_AT_COORD, "scr_fbi4_trucks_crash", GTA.Game.Player.Character.Position.X, GTA.Game.Player.Character.Position.Y, GTA.Game.Player.Character.Position.Z + 4f, 0, 0, 0, 5.5f, 0, 0, 0);
		 */
		BOOL START_PARTICLE_FX_NON_LOOPED_AT_COORD(const char* effectName, float xPos, float yPos, float zPos, float xRot, float yRot, float zRot, float scale, BOOL xAxis, BOOL yAxis, BOOL zAxis);
		/**
		 * Full list of particle effect dictionaries and effects by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/particleEffectsCompact.json
		 */
		BOOL START_NETWORKED_PARTICLE_FX_NON_LOOPED_AT_COORD(const char* effectName, float xPos, float yPos, float zPos, float xRot, float yRot, float zRot, float scale, BOOL xAxis, BOOL yAxis, BOOL zAxis, BOOL p11);
		/**
		 * GRAPHICS::START_PARTICLE_FX_NON_LOOPED_ON_PED_BONE("scr_sh_bong_smoke", PLAYER::PLAYER_PED_ID(), -0.025f, 0.13f, 0f, 0f, 0f, 0f, 31086, 0x3F800000, 0, 0, 0);
		 * 
		 * Axis - Invert Axis Flags
		 * 
		 * Full list of particle effect dictionaries and effects by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/particleEffectsCompact.json
		 */
		BOOL START_PARTICLE_FX_NON_LOOPED_ON_PED_BONE(const char* effectName, Ped ped, float offsetX, float offsetY, float offsetZ, float rotX, float rotY, float rotZ, int boneIndex, float scale, BOOL axisX, BOOL axisY, BOOL axisZ);
		/**
		 * Full list of particle effect dictionaries and effects by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/particleEffectsCompact.json
		 */
		BOOL START_NETWORKED_PARTICLE_FX_NON_LOOPED_ON_PED_BONE(const char* effectName, Ped ped, float offsetX, float offsetY, float offsetZ, float rotX, float rotY, float rotZ, int boneIndex, float scale, BOOL axisX, BOOL axisY, BOOL axisZ);
		/**
		 * Starts a particle effect on an entity for example your player.
		 * 
		 * Full list of particle effect dictionaries and effects by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/particleEffectsCompact.json
		 * 
		 * Example:
		 * C#:
		 * Function.Call(Hash.REQUEST_NAMED_PTFX_ASSET, "scr_rcbarry2");                     Function.Call(Hash.USE_PARTICLE_FX_ASSET, "scr_rcbarry2");                             Function.Call(Hash.START_PARTICLE_FX_NON_LOOPED_ON_ENTITY, "scr_clown_appears", Game.Player.Character, 0.0, 0.0, -0.5, 0.0, 0.0, 0.0, 1.0, false, false, false);
		 * 
		 * Internally this calls the same function as GRAPHICS::START_PARTICLE_FX_NON_LOOPED_ON_PED_BONE
		 * however it uses -1 for the specified bone index, so it should be possible to start a non looped fx on an entity bone using that native
		 * 
		 * -can confirm START_PARTICLE_FX_NON_LOOPED_ON_PED_BONE does NOT work on vehicle bones.
		 */
		BOOL START_PARTICLE_FX_NON_LOOPED_ON_ENTITY(const char* effectName, Entity entity, float offsetX, float offsetY, float offsetZ, float rotX, float rotY, float rotZ, float scale, BOOL axisX, BOOL axisY, BOOL axisZ);
		/**
		 * Full list of particle effect dictionaries and effects by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/particleEffectsCompact.json
		 */
		BOOL START_NETWORKED_PARTICLE_FX_NON_LOOPED_ON_ENTITY(const char* effectName, Entity entity, float offsetX, float offsetY, float offsetZ, float rotX, float rotY, float rotZ, float scale, BOOL axisX, BOOL axisY, BOOL axisZ);
		/**
		 * Full list of particle effect dictionaries and effects by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/particleEffectsCompact.json
		 */
		BOOL START_PARTICLE_FX_NON_LOOPED_ON_ENTITY_BONE(const char* effectName, Entity entity, float offsetX, float offsetY, float offsetZ, float rotX, float rotY, float rotZ, int boneIndex, float scale, BOOL axisX, BOOL axisY, BOOL axisZ);
		/**
		 * only works on some fx's, not networked
		 */
		void SET_PARTICLE_FX_NON_LOOPED_COLOUR(float r, float g, float b);
		/**
		 * Usage example for C#:
		 * 
		 * Function.Call(Hash.SET_PARTICLE_FX_NON_LOOPED_ALPHA, new InputArgument[] { 0.1f });
		 * 
		 * Note: the argument alpha ranges from 0.0f-1.0f !
		 */
		void SET_PARTICLE_FX_NON_LOOPED_ALPHA(float alpha);
		void SET_PARTICLE_FX_NON_LOOPED_SCALE(float scale);
		void SET_PARTICLE_FX_NON_LOOPED_EMITTER_SIZE(float p0, float p1, float scale);
		/**
		 * Used only once in the scripts (taxi_clowncar)
		 */
		void SET_PARTICLE_FX_FORCE_VEHICLE_INTERIOR(BOOL toggle);
		/**
		 * GRAPHICS::START_PARTICLE_FX_LOOPED_AT_COORD("scr_fbi_falling_debris", 93.7743f, -749.4572f, 70.86904f, 0f, 0f, 0f, 0x3F800000, 0, 0, 0, 0)
		 * 
		 * 
		 * p11 seems to be always 0
		 * 
		 * Full list of particle effect dictionaries and effects by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/particleEffectsCompact.json
		 */
		int START_PARTICLE_FX_LOOPED_AT_COORD(const char* effectName, float x, float y, float z, float xRot, float yRot, float zRot, float scale, BOOL xAxis, BOOL yAxis, BOOL zAxis, BOOL p11);
		/**
		 * Full list of particle effect dictionaries and effects by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/particleEffectsCompact.json
		 */
		int START_PARTICLE_FX_LOOPED_ON_PED_BONE(const char* effectName, Ped ped, float xOffset, float yOffset, float zOffset, float xRot, float yRot, float zRot, int boneIndex, float scale, BOOL xAxis, BOOL yAxis, BOOL zAxis);
		/**
		 * Full list of particle effect dictionaries and effects by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/particleEffectsCompact.json
		 */
		int START_PARTICLE_FX_LOOPED_ON_ENTITY(const char* effectName, Entity entity, float xOffset, float yOffset, float zOffset, float xRot, float yRot, float zRot, float scale, BOOL xAxis, BOOL yAxis, BOOL zAxis);
		/**
		 * Full list of particle effect dictionaries and effects by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/particleEffectsCompact.json
		 */
		int START_PARTICLE_FX_LOOPED_ON_ENTITY_BONE(const char* effectName, Entity entity, float xOffset, float yOffset, float zOffset, float xRot, float yRot, float zRot, int boneIndex, float scale, BOOL xAxis, BOOL yAxis, BOOL zAxis);
		/**
		 * Full list of particle effect dictionaries and effects by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/particleEffectsCompact.json
		 */
		int START_NETWORKED_PARTICLE_FX_LOOPED_ON_ENTITY(const char* effectName, Entity entity, float xOffset, float yOffset, float zOffset, float xRot, float yRot, float zRot, float scale, BOOL xAxis, BOOL yAxis, BOOL zAxis, float r, float g, float b, float a);
		/**
		 * Full list of particle effect dictionaries and effects by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/particleEffectsCompact.json
		 */
		int START_NETWORKED_PARTICLE_FX_LOOPED_ON_ENTITY_BONE(const char* effectName, Entity entity, float xOffset, float yOffset, float zOffset, float xRot, float yRot, float zRot, int boneIndex, float scale, BOOL xAxis, BOOL yAxis, BOOL zAxis, float r, float g, float b, float a);
		/**
		 * p1 is always 0 in the native scripts
		 */
		void STOP_PARTICLE_FX_LOOPED(int ptfxHandle, BOOL p1);
		void REMOVE_PARTICLE_FX(int ptfxHandle, BOOL p1);
		void REMOVE_PARTICLE_FX_FROM_ENTITY(Entity entity);
		void REMOVE_PARTICLE_FX_IN_RANGE(float X, float Y, float Z, float radius);
		void FORCE_PARTICLE_FX_IN_VEHICLE_INTERIOR(Any p0, Any p1);
		BOOL DOES_PARTICLE_FX_LOOPED_EXIST(int ptfxHandle);
		void SET_PARTICLE_FX_LOOPED_OFFSETS(int ptfxHandle, float x, float y, float z, float rotX, float rotY, float rotZ);
		void SET_PARTICLE_FX_LOOPED_EVOLUTION(int ptfxHandle, const char* propertyName, float amount, BOOL noNetwork);
		/**
		 * only works on some fx's
		 * 
		 * p4 = 0
		 */
		void SET_PARTICLE_FX_LOOPED_COLOUR(int ptfxHandle, float r, float g, float b, BOOL p4);
		void SET_PARTICLE_FX_LOOPED_ALPHA(int ptfxHandle, float alpha);
		void SET_PARTICLE_FX_LOOPED_SCALE(int ptfxHandle, float scale);
		void SET_PARTICLE_FX_LOOPED_FAR_CLIP_DIST(int ptfxHandle, float range);
		void _SET_PARTICLE_FX_LOOPED_CAMERA_BIAS(int ptfxHandle, float p1);
		void SET_PARTICLE_FX_CAM_INSIDE_VEHICLE(BOOL p0);
		void SET_PARTICLE_FX_CAM_INSIDE_NONPLAYER_VEHICLE(Vehicle vehicle, BOOL p1);
		void SET_PARTICLE_FX_SHOOTOUT_BOAT(Any p0);
		void CLEAR_PARTICLE_FX_SHOOTOUT_BOAT();
		void SET_PARTICLE_FX_BLOOD_SCALE(Any p0);
		void DISABLE_IN_WATER_PTFX(BOOL toggle);
		void DISABLE_DOWNWASH_PTFX(BOOL toggle);
		void SET_PARTICLE_FX_SLIPSTREAM_LODRANGE_SCALE(float scale);
		/**
		 * Creates cartoon effect when Michel smokes the weed
		 */
		void ENABLE_CLOWN_BLOOD_VFX(BOOL toggle);
		/**
		 * Creates a motion-blur sort of effect, this native does not seem to work, however by using the `START_SCREEN_EFFECT` native with `DrugsMichaelAliensFight` as the effect parameter, you should be able to get the effect.
		 */
		void ENABLE_ALIEN_BLOOD_VFX(BOOL toggle);
		void SET_PARTICLE_FX_BULLET_IMPACT_SCALE(float scale);
		void SET_PARTICLE_FX_BULLET_IMPACT_LODRANGE_SCALE(float p0);
		void SET_PARTICLE_FX_BULLET_TRACE_NO_ANGLE_REJECT(BOOL p0);
		void SET_PARTICLE_FX_BANG_SCRAPE_LODRANGE_SCALE(float p0);
		void SET_PARTICLE_FX_FOOT_LODRANGE_SCALE(float p0);
		/**
		 * Full list of particle effect dictionaries and effects by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/particleEffectsCompact.json
		 */
		void SET_PARTICLE_FX_FOOT_OVERRIDE_NAME(const char* p0);
		void SET_SKIDMARK_RANGE_SCALE(float scale);
		void SET_PTFX_FORCE_VEHICLE_INTERIOR_FLAG(Any p0);
		void _REGISTER_PTFX_FOG_VOLUME(float posX, float posY, float posZ, float range, float density); // Missing in crossmap
		void REGISTER_POSTFX_BULLET_IMPACT(float weaponWorldPosX, float weaponWorldPosY, float weaponWorldPosZ, float intensity);
		void FORCE_POSTFX_BULLET_IMPACTS_AFTER_HUD(BOOL p0);
		/**
		 * From the b678d decompiled scripts:
		 * 
		 * GRAPHICS::USE_PARTICLE_FX_ASSET("FM_Mission_Controler");
		 * GRAPHICS::USE_PARTICLE_FX_ASSET("scr_apartment_mp");
		 * GRAPHICS::USE_PARTICLE_FX_ASSET("scr_indep_fireworks");
		 * GRAPHICS::USE_PARTICLE_FX_ASSET("scr_mp_cig_plane");
		 * GRAPHICS::USE_PARTICLE_FX_ASSET("scr_mp_creator");
		 * GRAPHICS::USE_PARTICLE_FX_ASSET("scr_ornate_heist");
		 * GRAPHICS::USE_PARTICLE_FX_ASSET("scr_prison_break_heist_station");
		 * 
		 * Full list of particle effect dictionaries and effects by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/particleEffectsCompact.json
		 */
		void USE_PARTICLE_FX_ASSET(const char* name);
		/**
		 * Full list of particle effect dictionaries and effects by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/particleEffectsCompact.json
		 */
		void SET_PARTICLE_FX_OVERRIDE(const char* oldAsset, const char* newAsset);
		/**
		 * Resets the effect of SET_PARTICLE_FX_OVERRIDE
		 * 
		 * Full list of particle effect dictionaries and effects by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/particleEffectsCompact.json
		 */
		void RESET_PARTICLE_FX_OVERRIDE(const char* name);
		/**
		 * Returns ptfxHandle
		 * effectName: scr_sv_drag_burnout
		 */
		int _START_VEHICLE_PARTICLE_FX_LOOPED(Vehicle vehicle, const char* effectName, BOOL frontBack, BOOL leftRight, BOOL localOnly);
		void SET_WEATHER_PTFX_USE_OVERRIDE_SETTINGS(BOOL p0);
		void SET_WEATHER_PTFX_OVERRIDE_CURR_LEVEL(float p0);
		void WASH_DECALS_IN_RANGE(float x, float y, float z, float range, float p4);
		void WASH_DECALS_FROM_VEHICLE(Vehicle vehicle, float p1);
		/**
		 * Fades nearby decals within the range specified
		 */
		void FADE_DECALS_IN_RANGE(float x, float y, float z, float p3, float p4);
		/**
		 * Removes all decals in range from a position, it includes the bullet holes, blood pools, petrol...
		 */
		void REMOVE_DECALS_IN_RANGE(float x, float y, float z, float range);
		void REMOVE_DECALS_FROM_OBJECT(Object obj);
		void REMOVE_DECALS_FROM_OBJECT_FACING(Object obj, float x, float y, float z);
		void REMOVE_DECALS_FROM_VEHICLE(Vehicle vehicle);
		/**
		 * decal types:
		 * 
		 * public enum DecalTypes
		 * {
		 * splatters_blood = 1010,
		 * splatters_blood_dir = 1015,
		 * splatters_blood_mist = 1017,
		 * splatters_mud = 1020,
		 * splatters_paint = 1030,
		 * splatters_water = 1040,
		 * splatters_water_hydrant = 1050,
		 * splatters_blood2 = 1110,
		 * weapImpact_metal = 4010,
		 * weapImpact_concrete = 4020,
		 * weapImpact_mattress = 4030,
		 * weapImpact_mud = 4032,
		 * weapImpact_wood = 4050,
		 * weapImpact_sand = 4053,
		 * weapImpact_cardboard = 4040,
		 * weapImpact_melee_glass = 4100,
		 * weapImpact_glass_blood = 4102,
		 * weapImpact_glass_blood2 = 4104,
		 * weapImpact_shotgun_paper = 4200,
		 * weapImpact_shotgun_mattress,
		 * weapImpact_shotgun_metal,
		 * weapImpact_shotgun_wood,
		 * weapImpact_shotgun_dirt,
		 * weapImpact_shotgun_tvscreen,
		 * weapImpact_shotgun_tvscreen2,
		 * weapImpact_shotgun_tvscreen3,
		 * weapImpact_melee_concrete = 4310,
		 * weapImpact_melee_wood = 4312,
		 * weapImpact_melee_metal = 4314,
		 * burn1 = 4421,
		 * burn2,
		 * burn3,
		 * burn4,
		 * burn5,
		 * bang_concrete_bang = 5000,
		 * bang_concrete_bang2,
		 * bang_bullet_bang,
		 * bang_bullet_bang2 = 5004,
		 * bang_glass = 5031,
		 * bang_glass2,
		 * solidPool_water = 9000,
		 * solidPool_blood,
		 * solidPool_oil,
		 * solidPool_petrol,
		 * solidPool_mud,
		 * porousPool_water,
		 * porousPool_blood,
		 * porousPool_oil,
		 * porousPool_petrol,
		 * porousPool_mud,
		 * porousPool_water_ped_drip,
		 * liquidTrail_water = 9050
		 * }
		 */
		int ADD_DECAL(int decalType, float posX, float posY, float posZ, float p4, float p5, float p6, float p7, float p8, float p9, float width, float height, float rCoef, float gCoef, float bCoef, float opacity, float timeout, BOOL p17, BOOL p18, BOOL p19);
		int ADD_PETROL_DECAL(float x, float y, float z, float groundLvl, float width, float transparency);
		int ADD_OIL_DECAL(float x, float y, float z, float groundLvl, float width, float transparency);
		void START_PETROL_TRAIL_DECALS(float p0);
		void ADD_PETROL_TRAIL_DECAL_INFO(float x, float y, float z, float p3);
		void END_PETROL_TRAIL_DECALS();
		void REMOVE_DECAL(int decal);
		BOOL IS_DECAL_ALIVE(int decal);
		float GET_DECAL_WASH_LEVEL(int decal);
		void SET_DISABLE_PETROL_DECALS_IGNITING_THIS_FRAME();
		void SET_DISABLE_PETROL_DECALS_RECYCLING_THIS_FRAME();
		void SET_DISABLE_DECAL_RENDERING_THIS_FRAME();
		BOOL GET_IS_PETROL_DECAL_IN_RANGE(float xCoord, float yCoord, float zCoord, float radius);
		void PATCH_DECAL_DIFFUSE_MAP(int decalType, const char* textureDict, const char* textureName);
		void UNPATCH_DECAL_DIFFUSE_MAP(int decalType);
		void MOVE_VEHICLE_DECALS(Any p0, Any p1);
		/**
		 * boneIndex is always chassis_dummy in the scripts. The x/y/z params are location relative to the chassis bone.
		 */
		BOOL ADD_VEHICLE_CREW_EMBLEM(Vehicle vehicle, Ped ped, int boneIndex, float x1, float x2, float x3, float y1, float y2, float y3, float z1, float z2, float z3, float scale, Any p13, int alpha);
		BOOL ABORT_VEHICLE_CREW_EMBLEM_REQUEST(int* p0);
		void REMOVE_VEHICLE_CREW_EMBLEM(Vehicle vehicle, int p1);
		int GET_VEHICLE_CREW_EMBLEM_REQUEST_STATE(Vehicle vehicle, int p1);
		BOOL DOES_VEHICLE_HAVE_CREW_EMBLEM(Vehicle vehicle, int p1);
		void DISABLE_COMPOSITE_SHOTGUN_DECALS(BOOL toggle);
		void DISABLE_SCUFF_DECALS(BOOL toggle);
		void SET_DECAL_BULLET_IMPACT_RANGE_SCALE(float p0);
		void OVERRIDE_INTERIOR_SMOKE_NAME(const char* name);
		void OVERRIDE_INTERIOR_SMOKE_LEVEL(float level);
		void OVERRIDE_INTERIOR_SMOKE_END();
		/**
		 * Used with 'NG_filmnoir_BW{01,02}' timecycles and the "NOIR_FILTER_SOUNDS" audioref.
		 */
		void REGISTER_NOIR_LENS_EFFECT();
		void DISABLE_VEHICLE_DISTANTLIGHTS(BOOL toggle);
		void RENDER_SHADOWED_LIGHTS_WITH_NO_SHADOWS(BOOL p0);
		void REQUEST_EARLY_LIGHT_CHECK();
		/**
		 * Forces footstep tracks on all surfaces.
		 */
		void USE_SNOW_FOOT_VFX_WHEN_UNSHELTERED(BOOL toggle);
		void _FORCE_ALLOW_SNOW_FOOT_VFX_ON_ICE(BOOL toggle);
		/**
		 * Forces vehicle trails on all surfaces.
		 */
		void USE_SNOW_WHEEL_VFX_WHEN_UNSHELTERED(BOOL toggle);
		void DISABLE_REGION_VFX(Any p0);
		void _FORCE_GROUND_SNOW_PASS(BOOL toggle);
		/**
		 * Only one match in the scripts:
		 * 
		 * GRAPHICS::PRESET_INTERIOR_AMBIENT_CACHE("int_carrier_hanger");
		 */
		void PRESET_INTERIOR_AMBIENT_CACHE(const char* timecycleModifierName);
		/**
		 * Loads the specified timecycle modifier. Modifiers are defined separately in another file (e.g. "timecycle_mods_1.xml")
		 * 
		 * Parameters:
		 * modifierName - The modifier to load (e.g. "V_FIB_IT3", "scanline_cam", etc.)
		 * 
		 * Full list of timecycle modifiers by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/timecycleModifiers.json
		 */
		void SET_TIMECYCLE_MODIFIER(const char* modifierName);
		void SET_TIMECYCLE_MODIFIER_STRENGTH(float strength);
		/**
		 * Full list of timecycle modifiers by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/timecycleModifiers.json
		 */
		void SET_TRANSITION_TIMECYCLE_MODIFIER(const char* modifierName, float transition);
		void SET_TRANSITION_OUT_OF_TIMECYCLE_MODIFIER(float strength);
		void CLEAR_TIMECYCLE_MODIFIER();
		/**
		 * Only use for this in the PC scripts is:
		 * 
		 * if (GRAPHICS::GET_TIMECYCLE_MODIFIER_INDEX() != -1)
		 */
		int GET_TIMECYCLE_MODIFIER_INDEX();
		int GET_TIMECYCLE_TRANSITION_MODIFIER_INDEX();
		BOOL GET_IS_TIMECYCLE_TRANSITIONING_OUT();
		void PUSH_TIMECYCLE_MODIFIER();
		void POP_TIMECYCLE_MODIFIER();
		void SET_CURRENT_PLAYER_TCMODIFIER(const char* modifierName);
		void SET_PLAYER_TCMODIFIER_TRANSITION(float value);
		void SET_NEXT_PLAYER_TCMODIFIER(const char* modifierName);
		void ADD_TCMODIFIER_OVERRIDE(const char* modifierName1, const char* modifierName2);
		void CLEAR_ALL_TCMODIFIER_OVERRIDES(const char* p0);
		/**
		 * Full list of timecycle modifiers by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/timecycleModifiers.json
		 */
		void SET_EXTRA_TCMODIFIER(const char* modifierName);
		/**
		 * Clears the secondary timecycle modifier usually set with _SET_EXTRA_TIMECYCLE_MODIFIER
		 */
		void CLEAR_EXTRA_TCMODIFIER();
		/**
		 * See GET_TIMECYCLE_MODIFIER_INDEX for use, works the same just for the secondary timecycle modifier.
		 * Returns an integer representing the Timecycle modifier
		 */
		int GET_EXTRA_TCMODIFIER();
		/**
		 * The same as SET_TIMECYCLE_MODIFIER_STRENGTH but for the secondary timecycle modifier.
		 */
		void ENABLE_MOON_CYCLE_OVERRIDE(float strength);
		/**
		 * Resets the extra timecycle modifier strength normally set with 0x2C328AF17210F009
		 */
		void DISABLE_MOON_CYCLE_OVERRIDE();
		int REQUEST_SCALEFORM_MOVIE(const char* scaleformName);
		/**
		 * Another REQUEST_SCALEFORM_MOVIE equivalent.
		 */
		int REQUEST_SCALEFORM_MOVIE_WITH_IGNORE_SUPER_WIDESCREEN(const char* scaleformName);
		int REQUEST_SCALEFORM_MOVIE_INSTANCE(const char* scaleformName);
		/**
		 * Similar to REQUEST_SCALEFORM_MOVIE, but seems to be some kind of "interactive" scaleform movie?
		 * 
		 * These seem to be the only scaleforms ever requested by this native:
		 * "breaking_news"
		 * "desktop_pc"
		 * "ECG_MONITOR"
		 * "Hacking_PC"
		 * "TEETH_PULLING"
		 * 
		 * Note: Unless this hash is out-of-order, this native is next-gen only.
		 * 
		 */
		int REQUEST_SCALEFORM_MOVIE_SKIP_RENDER_WHILE_PAUSED(const char* scaleformName);
		BOOL HAS_SCALEFORM_MOVIE_LOADED(int scaleformHandle);
		void _HAS_SCALEFORM_MOVIE_NAMED_LOADED(int* scaleformHandle, const char* scaleformName); // Missing in crossmap
		/**
		 * val is 1-20 (0 will return false)
		 */
		BOOL IS_ACTIVE_SCALEFORM_MOVIE_DELETING(int val);
		/**
		 * val is 1-20. Return is related to INSTRUCTIONAL_BUTTONS, COLOUR_SWITCHER_02, etc?
		 */
		BOOL IS_SCALEFORM_MOVIE_DELETING(int val);
		/**
		 * Only values used in the scripts are:
		 * 
		 * "heist_mp"
		 * "heistmap_mp"
		 * "instructional_buttons"
		 * "heist_pre"
		 */
		BOOL HAS_SCALEFORM_MOVIE_FILENAME_LOADED(const char* scaleformName);
		BOOL HAS_SCALEFORM_CONTAINER_MOVIE_LOADED_INTO_PARENT(int scaleformHandle);
		void SET_SCALEFORM_MOVIE_AS_NO_LONGER_NEEDED(int* scaleformHandle);
		void _SET_SCALEFORM_MOVIE_NAMED_AS_NO_LONGER_NEEDED(int scaleformHandle, const char* scaleformName); // Missing in crossmap
		void SET_SCALEFORM_MOVIE_TO_USE_SYSTEM_TIME(int scaleform, BOOL toggle);
		void SET_SCALEFORM_MOVIE_TO_USE_LARGE_RT(int scaleformHandle, BOOL toggle);
		/**
		 * This native is used in some casino scripts to fit the scaleform in the rendertarget.
		 */
		void SET_SCALEFORM_MOVIE_TO_USE_SUPER_LARGE_RT(int scaleformHandle, BOOL toggle);
		void DRAW_SCALEFORM_MOVIE(int scaleformHandle, float x, float y, float width, float height, int red, int green, int blue, int alpha, int p9);
		/**
		 * unk is not used so no need
		 */
		void DRAW_SCALEFORM_MOVIE_FULLSCREEN(int scaleform, int red, int green, int blue, int alpha, int p5);
		void DRAW_SCALEFORM_MOVIE_FULLSCREEN_MASKED(int scaleform1, int scaleform2, int red, int green, int blue, int alpha);
		void DRAW_SCALEFORM_MOVIE_3D(int scaleform, float posX, float posY, float posZ, float rotX, float rotY, float rotZ, float p7, float p8, float p9, float scaleX, float scaleY, float scaleZ, int rotationOrder);
		void DRAW_SCALEFORM_MOVIE_3D_SOLID(int scaleform, float posX, float posY, float posZ, float rotX, float rotY, float rotZ, float p7, float p8, float p9, float scaleX, float scaleY, float scaleZ, int rotationOrder);
		/**
		 * Calls the Scaleform function.
		 */
		void CALL_SCALEFORM_MOVIE_METHOD(int scaleform, const char* method);
		/**
		 * Calls the Scaleform function and passes the parameters as floats.
		 * 
		 * The number of parameters passed to the function varies, so the end of the parameter list is represented by -1.0.
		 */
		void CALL_SCALEFORM_MOVIE_METHOD_WITH_NUMBER(int scaleform, const char* methodName, float param1, float param2, float param3, float param4, float param5);
		/**
		 * Calls the Scaleform function and passes the parameters as strings.
		 * 
		 * The number of parameters passed to the function varies, so the end of the parameter list is represented by 0 (NULL).
		 */
		void CALL_SCALEFORM_MOVIE_METHOD_WITH_STRING(int scaleform, const char* methodName, const char* param1, const char* param2, const char* param3, const char* param4, const char* param5);
		/**
		 * Calls the Scaleform function and passes both float and string parameters (in their respective order).
		 * 
		 * The number of parameters passed to the function varies, so the end of the float parameters is represented by -1.0, and the end of the string parameters is represented by 0 (NULL).
		 * 
		 * NOTE: The order of parameters in the function prototype is important! All float parameters must come first, followed by the string parameters.
		 * 
		 * Examples:
		 * // function MY_FUNCTION(floatParam1, floatParam2, stringParam)
		 * GRAPHICS::CALL_SCALEFORM_MOVIE_METHOD_WITH_NUMBER_AND_STRING(scaleform, "MY_FUNCTION", 10.0, 20.0, -1.0, -1.0, -1.0, "String param", 0, 0, 0, 0);
		 * 
		 * // function MY_FUNCTION_2(floatParam, stringParam1, stringParam2)
		 * GRAPHICS::CALL_SCALEFORM_MOVIE_METHOD_WITH_NUMBER_AND_STRING(scaleform, "MY_FUNCTION_2", 10.0, -1.0, -1.0, -1.0, -1.0, "String param #1", "String param #2", 0, 0, 0);
		 */
		void CALL_SCALEFORM_MOVIE_METHOD_WITH_NUMBER_AND_STRING(int scaleform, const char* methodName, float floatParam1, float floatParam2, float floatParam3, float floatParam4, float floatParam5, const char* stringParam1, const char* stringParam2, const char* stringParam3, const char* stringParam4, const char* stringParam5);
		/**
		 * Pushes a function from the Hud component Scaleform onto the stack. Same behavior as GRAPHICS::BEGIN_SCALEFORM_MOVIE_METHOD, just a hud component id instead of a Scaleform.
		 * 
		 * Known components:
		 * 19 - MP_RANK_BAR
		 * 20 - HUD_DIRECTOR_MODE
		 * 
		 * This native requires more research - all information can be found inside of 'hud.gfx'. Using a decompiler, the different components are located under "scripts\__Packages\com\rockstargames\gtav\hud\hudComponents" and "scripts\__Packages\com\rockstargames\gtav\Multiplayer".
		 */
		BOOL BEGIN_SCALEFORM_SCRIPT_HUD_MOVIE_METHOD(int hudComponent, const char* methodName);
		/**
		 * Push a function from the Scaleform onto the stack
		 * 
		 */
		BOOL BEGIN_SCALEFORM_MOVIE_METHOD(int scaleform, const char* methodName);
		/**
		 * Starts frontend (pause menu) scaleform movie methods.
		 * This can be used when you want to make custom frontend menus, and customize things like images or text in the menus etc.
		 * Use `BEGIN_SCALEFORM_MOVIE_METHOD_ON_FRONTEND_HEADER` for header scaleform functions.
		 */
		BOOL BEGIN_SCALEFORM_MOVIE_METHOD_ON_FRONTEND(const char* methodName);
		/**
		 * Starts frontend (pause menu) scaleform movie methods for header options.
		 * Use `BEGIN_SCALEFORM_MOVIE_METHOD_ON_FRONTEND` to customize the content inside the frontend menus.
		 */
		BOOL BEGIN_SCALEFORM_MOVIE_METHOD_ON_FRONTEND_HEADER(const char* methodName);
		/**
		 * Pops and calls the Scaleform function on the stack
		 */
		void END_SCALEFORM_MOVIE_METHOD();
		int END_SCALEFORM_MOVIE_METHOD_RETURN_VALUE();
		/**
		 * methodReturn: The return value of this native: END_SCALEFORM_MOVIE_METHOD_RETURN_VALUE
		 * Returns true if the return value of a scaleform function is ready to be collected (using GET_SCALEFORM_MOVIE_METHOD_RETURN_VALUE_STRING or GET_SCALEFORM_MOVIE_METHOD_RETURN_VALUE_INT).
		 */
		BOOL IS_SCALEFORM_MOVIE_METHOD_RETURN_VALUE_READY(int methodReturn);
		/**
		 * methodReturn: The return value of this native: END_SCALEFORM_MOVIE_METHOD_RETURN_VALUE
		 * Used to get a return value from a scaleform function. Returns an int in the same way GET_SCALEFORM_MOVIE_METHOD_RETURN_VALUE_STRING returns a string.
		 */
		int GET_SCALEFORM_MOVIE_METHOD_RETURN_VALUE_INT(int methodReturn);
		/**
		 * methodReturn: The return value of this native: END_SCALEFORM_MOVIE_METHOD_RETURN_VALUE
		 */
		BOOL GET_SCALEFORM_MOVIE_METHOD_RETURN_VALUE_BOOL(int methodReturn);
		/**
		 * methodReturn: The return value of this native: END_SCALEFORM_MOVIE_METHOD_RETURN_VALUE
		 * Used to get a return value from a scaleform function. Returns a string in the same way GET_SCALEFORM_MOVIE_METHOD_RETURN_VALUE_INT returns an int.
		 */
		const char* GET_SCALEFORM_MOVIE_METHOD_RETURN_VALUE_STRING(int methodReturn);
		/**
		 * Pushes an integer for the Scaleform function onto the stack.
		 */
		void SCALEFORM_MOVIE_METHOD_ADD_PARAM_INT(int value);
		/**
		 * Pushes a float for the Scaleform function onto the stack.
		 */
		void SCALEFORM_MOVIE_METHOD_ADD_PARAM_FLOAT(float value);
		/**
		 * Pushes a boolean for the Scaleform function onto the stack.
		 */
		void SCALEFORM_MOVIE_METHOD_ADD_PARAM_BOOL(BOOL value);
		/**
		 * Called prior to adding a text component to the UI. After doing so, GRAPHICS::END_TEXT_COMMAND_SCALEFORM_STRING is called.
		 * 
		 * Examples:
		 * GRAPHICS::BEGIN_TEXT_COMMAND_SCALEFORM_STRING("NUMBER");
		 * HUD::ADD_TEXT_COMPONENT_INTEGER(MISC::ABSI(a_1));
		 * GRAPHICS::END_TEXT_COMMAND_SCALEFORM_STRING();
		 * 
		 * GRAPHICS::BEGIN_TEXT_COMMAND_SCALEFORM_STRING("STRING");
		 * HUD::ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME(a_2);
		 * GRAPHICS::END_TEXT_COMMAND_SCALEFORM_STRING();
		 * 
		 * GRAPHICS::BEGIN_TEXT_COMMAND_SCALEFORM_STRING("STRTNM2");
		 * HUD::ADD_TEXT_COMPONENT_SUBSTRING_TEXT_LABEL_HASH_KEY(v_3);
		 * HUD::ADD_TEXT_COMPONENT_SUBSTRING_TEXT_LABEL_HASH_KEY(v_4);
		 * GRAPHICS::END_TEXT_COMMAND_SCALEFORM_STRING();
		 * 
		 * GRAPHICS::BEGIN_TEXT_COMMAND_SCALEFORM_STRING("STRTNM1");
		 * HUD::ADD_TEXT_COMPONENT_SUBSTRING_TEXT_LABEL_HASH_KEY(v_3);
		 * GRAPHICS::END_TEXT_COMMAND_SCALEFORM_STRING();
		 */
		void BEGIN_TEXT_COMMAND_SCALEFORM_STRING(const char* componentType);
		void END_TEXT_COMMAND_SCALEFORM_STRING();
		/**
		 * Same as END_TEXT_COMMAND_SCALEFORM_STRING but does not perform HTML conversion for text tokens.
		 */
		void END_TEXT_COMMAND_UNPARSED_SCALEFORM_STRING();
		/**
		 * Same as SCALEFORM_MOVIE_METHOD_ADD_PARAM_TEXTURE_NAME_STRING
		 * Both SCALEFORM_MOVIE_METHOD_ADD_PARAM_TEXTURE_NAME_STRING / _SCALEFORM_MOVIE_METHOD_ADD_PARAM_TEXTURE_NAME_STRING_2 works, but _SCALEFORM_MOVIE_METHOD_ADD_PARAM_TEXTURE_NAME_STRING_2 is usually used for "name" (organisation, players..).
		 */
		void SCALEFORM_MOVIE_METHOD_ADD_PARAM_LITERAL_STRING(const char* string);
		void SCALEFORM_MOVIE_METHOD_ADD_PARAM_TEXTURE_NAME_STRING(const char* string);
		void SCALEFORM_MOVIE_METHOD_ADD_PARAM_PLAYER_NAME_STRING(const char* string);
		BOOL DOES_LATEST_BRIEF_STRING_EXIST(int p0);
		void SCALEFORM_MOVIE_METHOD_ADD_PARAM_LATEST_BRIEF_STRING(int value);
		void REQUEST_SCALEFORM_SCRIPT_HUD_MOVIE(int hudComponent);
		BOOL HAS_SCALEFORM_SCRIPT_HUD_MOVIE_LOADED(int hudComponent);
		void REMOVE_SCALEFORM_SCRIPT_HUD_MOVIE(int hudComponent);
		BOOL PASS_KEYBOARD_INPUT_TO_SCALEFORM(int scaleformHandle);
		void SET_TV_CHANNEL(int channel);
		int GET_TV_CHANNEL();
		void SET_TV_VOLUME(float volume);
		float GET_TV_VOLUME();
		/**
		 * All calls to this native are preceded by calls to GRAPHICS::SET_SCRIPT_GFX_DRAW_ORDER and GRAPHICS::SET_SCRIPT_GFX_DRAW_BEHIND_PAUSEMENU, respectively.
		 * 
		 * "act_cinema.ysc", line 1483:
		 * HUD::SET_HUD_COMPONENT_POSITION(15, 0.0, -0.0375);
		 * HUD::SET_TEXT_RENDER_ID(l_AE);
		 * GRAPHICS::SET_SCRIPT_GFX_DRAW_ORDER(4);
		 * GRAPHICS::SET_SCRIPT_GFX_DRAW_BEHIND_PAUSEMENU(1);
		 * if (GRAPHICS::IS_TVSHOW_CURRENTLY_PLAYING(${movie_arthouse})) {
		 * GRAPHICS::DRAW_TV_CHANNEL(0.5, 0.5, 0.7375, 1.0, 0.0, 255, 255, 255, 255);
		 * } else {
		 * GRAPHICS::DRAW_TV_CHANNEL(0.5, 0.5, 1.0, 1.0, 0.0, 255, 255, 255, 255);
		 * }
		 * 
		 * "am_mp_property_int.ysc", line 102545:
		 * if (ENTITY::DOES_ENTITY_EXIST(a_2._f3)) {
		 * if (HUD::IS_NAMED_RENDERTARGET_LINKED(ENTITY::GET_ENTITY_MODEL(a_2._f3))) {
		 * HUD::SET_TEXT_RENDER_ID(a_2._f1);
		 * GRAPHICS::SET_SCRIPT_GFX_DRAW_ORDER(4);
		 * GRAPHICS::SET_SCRIPT_GFX_DRAW_BEHIND_PAUSEMENU(1);
		 * GRAPHICS::DRAW_TV_CHANNEL(0.5, 0.5, 1.0, 1.0, 0.0, 255, 255, 255, 255);
		 * if (GRAPHICS::GET_TV_CHANNEL() == -1) {
		 * sub_a8fa5(a_2, 1);
		 * } else {
		 * sub_a8fa5(a_2, 1);
		 * GRAPHICS::ATTACH_TV_AUDIO_TO_ENTITY(a_2._f3);
		 * }
		 * HUD::SET_TEXT_RENDER_ID(HUD::GET_DEFAULT_SCRIPT_RENDERTARGET_RENDER_ID());
		 * }
		 * }
		 * 
		 */
		void DRAW_TV_CHANNEL(float xPos, float yPos, float xScale, float yScale, float rotation, int red, int green, int blue, int alpha);
		/**
		 * Loads specified video sequence into the TV Channel
		 * TV_Channel ranges from 0-2
		 * VideoSequence can be any of the following:
		 * "PL_STD_CNT" CNT Standard Channel
		 * "PL_STD_WZL" Weazel Standard Channel
		 * "PL_LO_CNT"
		 * "PL_LO_WZL"
		 * "PL_SP_WORKOUT"
		 * "PL_SP_INV" - Jay Norris Assassination Mission Fail
		 * "PL_SP_INV_EXP" - Jay Norris Assassination Mission Success
		 * "PL_LO_RS" - Righteous Slaughter Ad
		 * "PL_LO_RS_CUTSCENE" - Righteous Slaughter Cut-scene
		 * "PL_SP_PLSH1_INTRO"
		 * "PL_LES1_FAME_OR_SHAME"
		 * "PL_STD_WZL_FOS_EP2"
		 * "PL_MP_WEAZEL" - Weazel Logo on loop
		 * "PL_MP_CCTV" - Generic CCTV loop
		 * 
		 * Restart:
		 * 0=video sequence continues as normal
		 * 1=sequence restarts from beginning every time that channel is selected
		 * 
		 * 
		 * The above playlists work as intended, and are commonly used, but there are many more playlists, as seen in `tvplaylists.xml`. A pastebin below outlines all playlists, they will be surronded by the name tag I.E. (<Name>PL_STD_CNT</Name> = PL_STD_CNT).
		 * https://pastebin.com/zUzGB6h7
		 */
		void SET_TV_CHANNEL_PLAYLIST(int tvChannel, const char* playlistName, BOOL restart);
		void SET_TV_CHANNEL_PLAYLIST_AT_HOUR(int tvChannel, const char* playlistName, int hour);
		/**
		 * Does not actually return anything.
		 */
		Any _SET_TV_CHANNEL_PLAYLIST_DIRTY(int tvChannel, BOOL p1);
		void CLEAR_TV_CHANNEL_PLAYLIST(int tvChannel);
		BOOL IS_PLAYLIST_ON_CHANNEL(int tvChannel, Any p1);
		BOOL IS_TVSHOW_CURRENTLY_PLAYING(Hash videoCliphash);
		void ENABLE_MOVIE_KEYFRAME_WAIT(BOOL toggle);
		void SET_TV_PLAYER_WATCHING_THIS_FRAME(Any p0);
		Hash GET_CURRENT_TV_CLIP_NAMEHASH();
		void ENABLE_MOVIE_SUBTITLES(BOOL toggle);
		BOOL UI3DSCENE_IS_AVAILABLE();
		/**
		 * All presets can be found in common\data\ui\uiscenes.meta
		 */
		BOOL UI3DSCENE_PUSH_PRESET(const char* presetName);
		/**
		 * It's called after UI3DSCENE_IS_AVAILABLE and UI3DSCENE_PUSH_PRESET
		 * 
		 * presetName was always "CELEBRATION_WINNER"
		 * All presets can be found in common\data\ui\uiscenes.meta
		 */
		BOOL UI3DSCENE_ASSIGN_PED_TO_SLOT(const char* presetName, Ped ped, int slot, float posX, float posY, float posZ);
		void UI3DSCENE_CLEAR_PATCHED_DATA();
		void UI3DSCENE_MAKE_PUSHED_PRESET_PERSISTENT(BOOL toggle);
		/**
		 * This native enables/disables the gold putting grid display (https://i.imgur.com/TC6cku6.png).
		 * This requires these two natives to be called as well to configure the grid: `TERRAINGRID_SET_PARAMS` and `TERRAINGRID_SET_COLOURS`.
		 */
		void TERRAINGRID_ACTIVATE(BOOL toggle);
		/**
		 * This native is used along with these two natives: `TERRAINGRID_ACTIVATE` and `TERRAINGRID_SET_COLOURS`.
		 * This native configures the location, size, rotation, normal height, and the difference ratio between min, normal and max.
		 * 
		 * All those natives combined they will output something like this: https://i.imgur.com/TC6cku6.png
		 * 
		 * This native renders a box at the given position, with a special shader that renders a grid on world geometry behind it. This box does not have backface culling.
		 * The forward args here are a direction vector, something similar to what's returned by GET_ENTITY_FORWARD_VECTOR.
		 * normalHeight and heightDiff are used for positioning the color gradient of the grid, colors specified via TERRAINGRID_SET_COLOURS.
		 * 
		 * Example with box superimposed on the image to demonstrate: https://i.imgur.com/wdqskxd.jpg
		 */
		void TERRAINGRID_SET_PARAMS(float x, float y, float z, float forwardX, float forwardY, float forwardZ, float sizeX, float sizeY, float sizeZ, float gridScale, float glowIntensity, float normalHeight, float heightDiff);
		/**
		 * This native is used along with these two natives: `TERRAINGRID_ACTIVATE` and `TERRAINGRID_SET_PARAMS`.
		 * This native sets the colors for the golf putting grid. the 'min...' values are for the lower areas that the grid covers, the 'max...' values are for the higher areas that the grid covers, all remaining values are for the 'normal' ground height.
		 * All those natives combined they will output something like this: https://i.imgur.com/TC6cku6.png
		 */
		void TERRAINGRID_SET_COLOURS(int lowR, int lowG, int lowB, int lowAlpha, int r, int g, int b, int alpha, int highR, int highG, int highB, int highAlpha);
		/**
		 * duration - is how long to play the effect for in milliseconds. If 0, it plays the default length
		 * if loop is true, the effect won't stop until you call ANIMPOSTFX_STOP on it. (only loopable effects)
		 * 
		 * Full list of animpostFX / screen effects by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/animPostFxNamesCompact.json
		 */
		void ANIMPOSTFX_PLAY(const char* effectName, int duration, BOOL looped);
		/**
		 * See ANIMPOSTFX_PLAY
		 * 
		 * Full list of animpostFX / screen effects by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/animPostFxNamesCompact.json
		 */
		void ANIMPOSTFX_STOP(const char* effectName);
		/**
		 * See ANIMPOSTFX_PLAY
		 * 
		 * Full list of animpostFX / screen effects by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/animPostFxNamesCompact.json
		 */
		float ANIMPOSTFX_GET_CURRENT_TIME(const char* effectName);
		/**
		 * Returns whether the specified effect is active.
		 * See ANIMPOSTFX_PLAY
		 * 
		 * Full list of animpostFX / screen effects by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/animPostFxNamesCompact.json
		 */
		BOOL ANIMPOSTFX_IS_RUNNING(const char* effectName);
		/**
		 * Stops ALL currently playing effects.
		 */
		void ANIMPOSTFX_STOP_ALL();
		/**
		 * Stops the effect and sets a value (bool) in its data (+0x199) to false.
		 * See ANIMPOSTFX_PLAY
		 * 
		 * Full list of animpostFX / screen effects by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/animPostFxNamesCompact.json
		 */
		void ANIMPOSTFX_STOP_AND_FLUSH_REQUESTS(const char* effectName);

	} // namespace GRAPHICS

	namespace HUD {

		/**
		 * Initializes the text entry for the the text next to a loading prompt. All natives for building UI texts can be used here
		 * 
		 * 
		 * e.g
		 * void StartLoadingMessage(char *text, int spinnerType = 3)
		 * {
		 * BEGIN_TEXT_COMMAND_BUSYSPINNER_ON("STRING");
		 * ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME(text);
		 * END_TEXT_COMMAND_BUSYSPINNER_ON(spinnerType);
		 * }
		 * /*OR* /
		 * void ShowLoadingMessage(char *text, int spinnerType = 3, int timeMs = 10000)
		 * {
		 * BEGIN_TEXT_COMMAND_BUSYSPINNER_ON("STRING");
		 * ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME(text);
		 * END_TEXT_COMMAND_BUSYSPINNER_ON(spinnerType);
		 * WAIT(timeMs);
		 * BUSYSPINNER_OFF();
		 * }
		 * 
		 * 
		 * These are some localized strings used in the loading spinner.
		 * "PM_WAIT"                   = Please Wait
		 * "CELEB_WPLYRS"              = Waiting For Players.
		 * "CELL_SPINNER2"             = Scanning storage.
		 * "ERROR_CHECKYACHTNAME" = Registering your yacht's name. Please wait.
		 * "ERROR_CHECKPROFANITY"   = Checking your text for profanity. Please wait.
		 * "FM_COR_AUTOD"                        = Just spinner no text
		 * "FM_IHELP_WAT2"                        = Waiting for other players
		 * "FM_JIP_WAITO"                            = Game options are being set
		 * "FMMC_DOWNLOAD"                    = Downloading
		 * "FMMC_PLYLOAD"                         = Loading
		 * "FMMC_STARTTRAN"                    = Launching session
		 * "HUD_QUITTING"                           =  Quiting session
		 * "KILL_STRIP_IDM"                         = Waiting for to accept
		 * "MP_SPINLOADING"                      = Loading
		 */
		void BEGIN_TEXT_COMMAND_BUSYSPINNER_ON(const char* string);
		/**
		 * enum eBusySpinnerType
		 * {
		 * BUSY_SPINNER_LEFT,
		 * BUSY_SPINNER_LEFT_2,
		 * BUSY_SPINNER_LEFT_3,
		 * BUSY_SPINNER_SAVE,
		 * BUSY_SPINNER_RIGHT,
		 * };
		 */
		void END_TEXT_COMMAND_BUSYSPINNER_ON(int busySpinnerType);
		/**
		 * Removes the loading prompt at the bottom right of the screen.
		 */
		void BUSYSPINNER_OFF();
		void PRELOAD_BUSYSPINNER();
		BOOL BUSYSPINNER_IS_ON();
		BOOL BUSYSPINNER_IS_DISPLAYING();
		void DISABLE_PAUSEMENU_SPINNER(BOOL p0);
		/**
		 * Shows the cursor on screen for one frame.
		 */
		void SET_MOUSE_CURSOR_THIS_FRAME();
		/**
		 * Changes the mouse cursor's sprite.
		 * 1 = Normal
		 * 6 = Left Arrow
		 * 7 = Right Arrow
		 */
		void SET_MOUSE_CURSOR_STYLE(int spriteId);
		/**
		 * Shows/hides the frontend cursor on the pause menu or similar menus.
		 * Clicking off and then on the game window will show it again.
		 */
		void SET_MOUSE_CURSOR_VISIBLE(BOOL toggle);
		/**
		 * Returns TRUE if mouse is hovering above instructional buttons. Works with all buttons gfx, such as popup_warning, pause_menu_instructional_buttons, instructional_buttons, etc. Note: You have to call TOGGLE_MOUSE_BUTTONS on the scaleform if you want this native to work.
		 */
		BOOL IS_MOUSE_ROLLED_OVER_INSTRUCTIONAL_BUTTONS();
		BOOL GET_MOUSE_EVENT(int scaleformHandle, Any* p1, Any* p2, Any* p3);
		void THEFEED_ONLY_SHOW_TOOLTIPS(BOOL toggle);
		void THEFEED_SET_SCRIPTED_MENU_HEIGHT(float pos);
		/**
		 * Stops loading screen tips shown by invoking `THEFEED_SHOW`
		 */
		void THEFEED_HIDE();
		/**
		 * Once called each frame hides all above radar notifications.
		 */
		void THEFEED_HIDE_THIS_FRAME();
		/**
		 * Displays loading screen tips, requires `THEFEED_AUTO_POST_GAMETIPS_ON` to be called beforehand.
		 */
		void THEFEED_SHOW();
		void THEFEED_FLUSH_QUEUE();
		/**
		 * Removes a notification instantly instead of waiting for it to disappear
		 */
		void THEFEED_REMOVE_ITEM(int notificationId);
		void THEFEED_FORCE_RENDER_ON();
		/**
		 * Enables loading screen tips to be be shown (`THEFEED_SHOW`), blocks other kinds of notifications from being displayed (at least from current script). Call `0xADED7F5748ACAFE6` to display those again.
		 */
		void THEFEED_FORCE_RENDER_OFF();
		void THEFEED_PAUSE();
		void THEFEED_RESUME();
		BOOL THEFEED_IS_PAUSED();
		void THEFEED_REPORT_LOGO_ON();
		void THEFEED_REPORT_LOGO_OFF();
		/**
		 * Returns the handle for the notification currently displayed on the screen. Name may be a hash collision, but describes the function accurately.
		 */
		int THEFEED_GET_LAST_SHOWN_PHONE_ACTIVATABLE_FEED_ID();
		/**
		 * Enables loading screen tips to be be shown (`THEFEED_SHOW`), blocks other kinds of notifications from being displayed (at least from current script). Call `THEFEED_AUTO_POST_GAMETIPS_OFF` to display those again.
		 */
		void THEFEED_AUTO_POST_GAMETIPS_ON();
		/**
		 * Displays "normal" notifications again after calling `THEFEED_AUTO_POST_GAMETIPS_ON` (those that were drawn before calling this native too), though those will have a weird offset and stay on screen forever (tested with notifications created from same script).
		 */
		void THEFEED_AUTO_POST_GAMETIPS_OFF();
		/**
		 * From the decompiled scripts:
		 * HUD::THEFEED_SET_BACKGROUND_COLOR_FOR_NEXT_POST(6);
		 * HUD::THEFEED_SET_BACKGROUND_COLOR_FOR_NEXT_POST(184);
		 * HUD::THEFEED_SET_BACKGROUND_COLOR_FOR_NEXT_POST(190);
		 * 
		 * sets background color for the next notification
		 * 6 = red
		 * 184 = green
		 * 190 = yellow
		 * 
		 * Here is a list of some colors that can be used: https://gyazo.com/68bd384455fceb0a85a8729e48216e15
		 */
		void THEFEED_SET_BACKGROUND_COLOR_FOR_NEXT_POST(int hudColorIndex);
		void THEFEED_SET_RGBA_PARAMETER_FOR_NEXT_MESSAGE(int red, int green, int blue, int alpha);
		/**
		 * Related to notification color flashing, setting count to 0 invalidates a `THEFEED_SET_RGBA_PARAMETER_FOR_NEXT_MESSAGE` call for the target notification.
		 */
		void THEFEED_SET_FLASH_DURATION_PARAMETER_FOR_NEXT_MESSAGE(int count);
		void THEFEED_SET_VIBRATE_PARAMETER_FOR_NEXT_MESSAGE(BOOL toggle);
		void THEFEED_RESET_ALL_PARAMETERS();
		/**
		 * Requires manual management of game stream handles (i.e., 0xBE4390CB40B3E627).
		 */
		void THEFEED_FREEZE_NEXT_POST();
		void THEFEED_CLEAR_FROZEN_POST();
		void THEFEED_SET_SNAP_FEED_ITEM_POSITIONS(BOOL p0);
		/**
		 * Used in the native scripts to reference "GET_PEDHEADSHOT_TXD_STRING" and "CHAR_DEFAULT".
		 */
		void THEFEED_UPDATE_ITEM_TEXTURE(const char* txdString1, const char* txnString1, const char* txdString2, const char* txnString2);
		/**
		 * Declares the entry type of a notification, for example "STRING".
		 * 
		 * int ShowNotification(char *text)
		 * {
		 * BEGIN_TEXT_COMMAND_THEFEED_POST("STRING");
		 * ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME(text);
		 * return END_TEXT_COMMAND_THEFEED_POST_TICKER(1, 1);
		 * }
		 */
		void BEGIN_TEXT_COMMAND_THEFEED_POST(const char* text);
		/**
		 * List of picture names: https://pastebin.com/XdpJVbHz
		 * Example result: https://i.imgur.com/SdEZ22m.png
		 */
		int END_TEXT_COMMAND_THEFEED_POST_STATS(const char* statTitle, int iconEnum, BOOL stepVal, int barValue, BOOL isImportant, const char* pictureTextureDict, const char* pictureTextureName);
		/**
		 * This function can show pictures of every texture that can be requested by REQUEST_STREAMED_TEXTURE_DICT.
		 * 
		 * List of picNames: https://pastebin.com/XdpJVbHz
		 * 
		 * 
		 * flash is a bool for fading in.
		 * iconTypes:
		 * 1 : Chat Box
		 * 2 : Email
		 * 3 : Add Friend Request
		 * 4 : Nothing
		 * 5 : Nothing
		 * 6 : Nothing
		 * 7 : Right Jumping Arrow
		 * 8 : RP Icon
		 * 9 : $ Icon
		 * 
		 * "sender" is the very top header. This can be any old string.
		 * "subject" is the header under the sender.
		 */
		int END_TEXT_COMMAND_THEFEED_POST_MESSAGETEXT(const char* txdName, const char* textureName, BOOL flash, int iconType, const char* sender, const char* subject);
		/**
		 * This function can show pictures of every texture that can be requested by REQUEST_STREAMED_TEXTURE_DICT.
		 * 
		 * Needs more research.
		 * 
		 * Only one type of usage in the scripts:
		 * 
		 * HUD::END_TEXT_COMMAND_THEFEED_POST_MESSAGETEXT_SUBTITLE_LABEL("CHAR_ACTING_UP", "CHAR_ACTING_UP", 0, 0, "DI_FEED_CHAR", a_0);
		 */
		int END_TEXT_COMMAND_THEFEED_POST_MESSAGETEXT_SUBTITLE_LABEL(const char* txdName, const char* textureName, BOOL flash, int iconType, const char* sender, const char* subject);
		/**
		 * This function can show pictures of every texture that can be requested by REQUEST_STREAMED_TEXTURE_DICT.
		 * 
		 * NOTE: 'duration' is a multiplier, so 1.0 is normal, 2.0 is twice as long (very slow), and 0.5 is half as long.
		 * 
		 * Example, only occurrence in the scripts:
		 * v_8 = HUD::END_TEXT_COMMAND_THEFEED_POST_MESSAGETEXT_TU("CHAR_SOCIAL_CLUB", "CHAR_SOCIAL_CLUB", 0, 0, &v_9, "", a_5);
		 */
		int END_TEXT_COMMAND_THEFEED_POST_MESSAGETEXT_TU(const char* txdName, const char* textureName, BOOL flash, int iconType, const char* sender, const char* subject, float duration);
		/**
		 * This function can show pictures of every texture that can be requested by REQUEST_STREAMED_TEXTURE_DICT.
		 * 
		 * List of picNames https://pastebin.com/XdpJVbHz
		 * 
		 * flash is a bool for fading in.
		 * iconTypes:
		 * 1 : Chat Box
		 * 2 : Email
		 * 3 : Add Friend Request
		 * 4 : Nothing
		 * 5 : Nothing
		 * 6 : Nothing
		 * 7 : Right Jumping Arrow
		 * 8 : RP Icon
		 * 9 : $ Icon
		 * 
		 * "sender" is the very top header. This can be any old string.
		 * "subject" is the header under the sender.
		 * "duration" is a multiplier, so 1.0 is normal, 2.0 is twice as long (very slow), and 0.5 is half as long.
		 * "clanTag" shows a crew tag in the "sender" header, after the text. You need to use 3 underscores as padding. Maximum length of this field seems to be 7. (e.g. "MK" becomes "___MK", "ACE" becomes "___ACE", etc.)
		 */
		int END_TEXT_COMMAND_THEFEED_POST_MESSAGETEXT_WITH_CREW_TAG(const char* txdName, const char* textureName, BOOL flash, int iconType, const char* sender, const char* subject, float duration, const char* clanTag);
		/**
		 * This function can show pictures of every texture that can be requested by REQUEST_STREAMED_TEXTURE_DICT.
		 * 
		 * List of picNames:  https://pastebin.com/XdpJVbHz
		 * 
		 * flash is a bool for fading in.
		 * iconTypes:
		 * 1 : Chat Box
		 * 2 : Email
		 * 3 : Add Friend Request
		 * 4 : Nothing
		 * 5 : Nothing
		 * 6 : Nothing
		 * 7 : Right Jumping Arrow
		 * 8 : RP Icon
		 * 9 : $ Icon
		 * 
		 * "sender" is the very top header. This can be any old string.
		 * "subject" is the header under the sender.
		 * "duration" is a multiplier, so 1.0 is normal, 2.0 is twice as long (very slow), and 0.5 is half as long.
		 * "clanTag" shows a crew tag in the "sender" header, after the text. You need to use 3 underscores as padding. Maximum length of this field seems to be 7. (e.g. "MK" becomes "___MK", "ACE" becomes "___ACE", etc.)
		 * iconType2 is a mirror of iconType. It shows in the "subject" line, right under the original iconType.
		 * 
		 * 
		 * int IconNotification(char *text, char *text2, char *Subject)
		 * {
		 * BEGIN_TEXT_COMMAND_THEFEED_POST("STRING");
		 * ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME(text);
		 * _SET_NOTIFICATION_MESSAGE_CLAN_TAG_2("CHAR_SOCIAL_CLUB", "CHAR_SOCIAL_CLUB", 1, 7, text2, Subject, 1.0f, "__EXAMPLE", 7);
		 * return END_TEXT_COMMAND_THEFEED_POST_TICKER(1, 1);
		 * }
		 */
		int END_TEXT_COMMAND_THEFEED_POST_MESSAGETEXT_WITH_CREW_TAG_AND_ADDITIONAL_ICON(const char* txdName, const char* textureName, BOOL flash, int iconType1, const char* sender, const char* subject, float duration, const char* clanTag, int iconType2, int p9);
		int END_TEXT_COMMAND_THEFEED_POST_TICKER(BOOL blink, BOOL p1);
		int END_TEXT_COMMAND_THEFEED_POST_TICKER_FORCED(BOOL blink, BOOL p1);
		int END_TEXT_COMMAND_THEFEED_POST_TICKER_WITH_TOKENS(BOOL blink, BOOL p1);
		/**
		 * Shows an "award" notification above the minimap, example: https://i.imgur.com/e2DNaKX.png
		 * Example:
		 * 
		 * HUD::BEGIN_TEXT_COMMAND_THEFEED_POST("HUNT");
		 * HUD::END_TEXT_COMMAND_THEFEED_POST_AWARD("Hunting", "Hunting_Gold_128", 0, 109, "HUD_MED_UNLKED");
		 */
		int END_TEXT_COMMAND_THEFEED_POST_AWARD(const char* textureDict, const char* textureName, int rpBonus, int colorOverlay, const char* titleLabel);
		int END_TEXT_COMMAND_THEFEED_POST_CREWTAG(BOOL p0, BOOL p1, int* p2, int p3, BOOL isLeader, BOOL unk0, int clanDesc, int R, int G, int B);
		int END_TEXT_COMMAND_THEFEED_POST_CREWTAG_WITH_GAME_NAME(BOOL p0, BOOL p1, int* p2, int p3, BOOL isLeader, BOOL unk0, int clanDesc, const char* playerName, int R, int G, int B);
		int END_TEXT_COMMAND_THEFEED_POST_UNLOCK(const char* gxtLabel1, int p1, const char* gxtLabel2);
		int END_TEXT_COMMAND_THEFEED_POST_UNLOCK_TU(const char* gxtLabel1, int p1, const char* gxtLabel2, int p3);
		int END_TEXT_COMMAND_THEFEED_POST_UNLOCK_TU_WITH_COLOR(Any p0, Any p1, Any p2, Any p3, Any p4, Any p5);
		int END_TEXT_COMMAND_THEFEED_POST_MPTICKER(BOOL blink, BOOL p1);
		int END_TEXT_COMMAND_THEFEED_POST_CREW_RANKUP_WITH_LITERAL_FLAG(const char* p0, const char* p1, const char* p2, BOOL p3, BOOL p4);
		/**
		 * This function can show pictures of every texture that can be requested by REQUEST_STREAMED_TEXTURE_DICT.
		 * 
		 * List of picNames: https://pastebin.com/XdpJVbHz
		 * HUD colors and their values: https://pastebin.com/d9aHPbXN
		 * 
		 * Shows a deathmatch score above the minimap, example: https://i.imgur.com/YmoMklG.png
		 */
		int END_TEXT_COMMAND_THEFEED_POST_VERSUS_TU(const char* txdName1, const char* textureName1, int count1, const char* txdName2, const char* textureName2, int count2, int hudColor1, int hudColor2);
		/**
		 * returns a notification handle, prints out a notification like below:
		 * type range: 0 - 2
		 * if you set type to 1, image goes from 0 - 39 - Xbox you can add text to
		 * 
		 * example:
		 * HUD::END_TEXT_COMMAND_THEFEED_POST_REPLAY_INPUT(1, 20, "Who you trynna get crazy with, ese? Don't you know I'm LOCO?!");
		 * - https://imgur.com/lGBPCz3
		 */
		int END_TEXT_COMMAND_THEFEED_POST_REPLAY(int type, int image, const char* text);
		/**
		 * returns a notification handle, prints out a notification like below:
		 * type range: 0 - 2
		 * if you set type to 1, button accepts "~INPUT_SOMETHING~"
		 * 
		 * example:
		 * HUD::END_TEXT_COMMAND_THEFEED_POST_REPLAY_INPUT(1, "~INPUT_TALK~", "Who you trynna get crazy with, ese? Don't you know I'm LOCO?!");
		 * - https://imgur.com/UPy0Ial
		 * 
		 * 
		 * Examples from the scripts:
		 * l_D1[1/*1* /]=HUD::END_TEXT_COMMAND_THEFEED_POST_REPLAY_INPUT(1,"~INPUT_REPLAY_START_STOP_RECORDING~","");
		 * l_D1[2/*1* /]=HUD::END_TEXT_COMMAND_THEFEED_POST_REPLAY_INPUT(1,"~INPUT_SAVE_REPLAY_CLIP~","");
		 * l_D1[1/*1* /]=HUD::END_TEXT_COMMAND_THEFEED_POST_REPLAY_INPUT(1,"~INPUT_REPLAY_START_STOP_RECORDING~","");
		 * l_D1[2/*1* /]=HUD::END_TEXT_COMMAND_THEFEED_POST_REPLAY_INPUT(1,"~INPUT_REPLAY_START_STOP_RECORDING_SECONDARY~","");
		 * 
		 */
		int END_TEXT_COMMAND_THEFEED_POST_REPLAY_INPUT(int type, const char* button, const char* text);
		/**
		 * void ShowSubtitle(const char *text)
		 * {
		 * BEGIN_TEXT_COMMAND_PRINT("STRING");
		 * ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME(text);
		 * END_TEXT_COMMAND_PRINT(2000, true);
		 * }
		 */
		void BEGIN_TEXT_COMMAND_PRINT(const char* GxtEntry);
		/**
		 * Draws the subtitle at middle center of the screen.
		 * 
		 * int duration = time in milliseconds to show text on screen before disappearing
		 * 
		 * drawImmediately = If true, the text will be drawn immediately, if false, the text will be drawn after the previous subtitle has finished
		 * 
		 * Used to be known as _DRAW_SUBTITLE_TIMED
		 */
		void END_TEXT_COMMAND_PRINT(int duration, BOOL drawImmediately);
		/**
		 * nothin doin.
		 * 
		 * BOOL Message(const char* text)
		 * {
		 * BEGIN_TEXT_COMMAND_IS_MESSAGE_DISPLAYED("STRING");
		 * ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME(text);
		 * return END_TEXT_COMMAND_IS_MESSAGE_DISPLAYED();
		 * }
		 */
		void BEGIN_TEXT_COMMAND_IS_MESSAGE_DISPLAYED(const char* text);
		BOOL END_TEXT_COMMAND_IS_MESSAGE_DISPLAYED();
		/**
		 * The following were found in the decompiled script files:
		 * STRING, TWOSTRINGS, NUMBER, PERCENTAGE, FO_TWO_NUM, ESMINDOLLA, ESDOLLA, MTPHPER_XPNO, AHD_DIST, CMOD_STAT_0, CMOD_STAT_1, CMOD_STAT_2, CMOD_STAT_3, DFLT_MNU_OPT, F3A_TRAFDEST, ES_HELP_SOC3
		 * 
		 * ESDOLLA - cash
		 * ESMINDOLLA - cash (negative)
		 * 
		 * Used to be known as _SET_TEXT_ENTRY
		 */
		void BEGIN_TEXT_COMMAND_DISPLAY_TEXT(const char* text);
		/**
		 * After applying the properties to the text (See HUD::SET_TEXT_), this will draw the text in the applied position. Also 0.0f < x, y < 1.0f, percentage of the axis.
		 * 
		 * Used to be known as _DRAW_TEXT
		 */
		void END_TEXT_COMMAND_DISPLAY_TEXT(float x, float y, int p2);
		void BEGIN_TEXT_COMMAND_GET_SCREEN_WIDTH_OF_DISPLAY_TEXT(const char* text);
		float END_TEXT_COMMAND_GET_SCREEN_WIDTH_OF_DISPLAY_TEXT(BOOL p0);
		/**
		 * int GetLineCount(char *text, float x, float y)
		 * {
		 * BEGIN_TEXT_COMMAND_GET_NUMBER_OF_LINES_FOR_STRING("STRING");
		 * ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME(text);
		 * return BEGIN_TEXT_COMMAND_GET_NUMBER_OF_LINES_FOR_STRING(x, y);
		 * }
		 */
		void BEGIN_TEXT_COMMAND_GET_NUMBER_OF_LINES_FOR_STRING(const char* entry);
		/**
		 * Determines how many lines the text string will use when drawn on screen.
		 * Must use 0x521FB041D93DD0E4 for setting up
		 */
		int END_TEXT_COMMAND_GET_NUMBER_OF_LINES_FOR_STRING(float x, float y);
		/**
		 * Used to be known as _SET_TEXT_COMPONENT_FORMAT
		 */
		void BEGIN_TEXT_COMMAND_DISPLAY_HELP(const char* inputType);
		/**
		 * shape goes from -1 to 50 (may be more).
		 * p0 is always 0.
		 * 
		 * Example:
		 * void FloatingHelpText(const char* text)
		 * {
		 * BEGIN_TEXT_COMMAND_DISPLAY_HELP("STRING");
		 * ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME(text);
		 * END_TEXT_COMMAND_DISPLAY_HELP (0, 0, 1, -1);
		 * }
		 * 
		 * Image:
		 * - imgbin.org/images/26209.jpg
		 * 
		 * more inputs/icons:
		 * - https://pastebin.com/nqNYWMSB
		 * 
		 * Used to be known as _DISPLAY_HELP_TEXT_FROM_STRING_LABEL
		 */
		void END_TEXT_COMMAND_DISPLAY_HELP(int p0, BOOL loop, BOOL beep, int shape);
		/**
		 * BOOL IsContextActive(char *ctx)
		 * {
		 * BEGIN_TEXT_COMMAND_IS_THIS_HELP_MESSAGE_BEING_DISPLAYED(ctx);
		 * return END_TEXT_COMMAND_IS_THIS_HELP_MESSAGE_BEING_DISPLAYED(0);
		 * }
		 */
		void BEGIN_TEXT_COMMAND_IS_THIS_HELP_MESSAGE_BEING_DISPLAYED(const char* labelName);
		BOOL END_TEXT_COMMAND_IS_THIS_HELP_MESSAGE_BEING_DISPLAYED(int p0);
		/**
		 * Starts a text command to change the name of a blip displayed in the pause menu.
		 * This should be paired with `END_TEXT_COMMAND_SET_BLIP_NAME`, once adding all required text components.
		 * Example:
		 * 
		 * HUD::BEGIN_TEXT_COMMAND_SET_BLIP_NAME("STRING");
		 * HUD::ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME("Name");
		 * HUD::END_TEXT_COMMAND_SET_BLIP_NAME(blip);
		 */
		void BEGIN_TEXT_COMMAND_SET_BLIP_NAME(const char* textLabel);
		/**
		 * Finalizes a text command started with BEGIN_TEXT_COMMAND_SET_BLIP_NAME, setting the name of the specified blip.
		 */
		void END_TEXT_COMMAND_SET_BLIP_NAME(Blip blip);
		void BEGIN_TEXT_COMMAND_ADD_DIRECTLY_TO_PREVIOUS_BRIEFS(const char* p0);
		void END_TEXT_COMMAND_ADD_DIRECTLY_TO_PREVIOUS_BRIEFS(BOOL p0);
		/**
		 * clears a print text command with this text
		 */
		void BEGIN_TEXT_COMMAND_CLEAR_PRINT(const char* text);
		void END_TEXT_COMMAND_CLEAR_PRINT();
		void BEGIN_TEXT_COMMAND_OVERRIDE_BUTTON_TEXT(const char* gxtEntry);
		void END_TEXT_COMMAND_OVERRIDE_BUTTON_TEXT(int p0);
		void ADD_TEXT_COMPONENT_INTEGER(int value);
		void ADD_TEXT_COMPONENT_FLOAT(float value, int decimalPlaces);
		void ADD_TEXT_COMPONENT_SUBSTRING_TEXT_LABEL(const char* labelName);
		/**
		 * It adds the localized text of the specified GXT entry name. Eg. if the argument is GET_HASH_KEY("ES_HELP"), adds "Continue". Just uses a text labels hash key
		 */
		void ADD_TEXT_COMPONENT_SUBSTRING_TEXT_LABEL_HASH_KEY(Hash gxtEntryHash);
		void ADD_TEXT_COMPONENT_SUBSTRING_BLIP_NAME(Blip blip);
		void ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME(const char* text);
		/**
		 * Adds a timer (e.g. "00:00:00:000"). The appearance of the timer depends on the flags, which needs more research.
		 */
		void ADD_TEXT_COMPONENT_SUBSTRING_TIME(int timestamp, int flags);
		void ADD_TEXT_COMPONENT_FORMATTED_INTEGER(int value, BOOL commaSeparated);
		/**
		 * p1 was always -1
		 */
		void ADD_TEXT_COMPONENT_SUBSTRING_PHONE_NUMBER(const char* p0, int p1);
		/**
		 * This native (along with ADD_TEXT_COMPONENT_SUBSTRING_KEYBOARD_DISPLAY and ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME) do not actually filter anything. They simply add the provided text (as of 944)
		 */
		void ADD_TEXT_COMPONENT_SUBSTRING_WEBSITE(const char* website);
		void ADD_TEXT_COMPONENT_SUBSTRING_KEYBOARD_DISPLAY(const char* string);
		void SET_COLOUR_OF_NEXT_TEXT_COMPONENT(int hudColor);
		/**
		 * Returns a substring of a specified length starting at a specified position.
		 * 
		 * Example:
		 * // Get "STRING" text from "MY_STRING"
		 * subStr = HUD::GET_CHARACTER_FROM_AUDIO_CONVERSATION_FILENAME("MY_STRING", 3, 6);
		 */
		const char* GET_CHARACTER_FROM_AUDIO_CONVERSATION_FILENAME(const char* text, int position, int length);
		/**
		 * Returns a substring of a specified length starting at a specified position. The result is guaranteed not to exceed the specified max length.
		 * 
		 * NOTE: The 'maxLength' parameter might actually be the size of the buffer that is returned. More research is needed. -CL69
		 * 
		 * Example:
		 * // Condensed example of how Rockstar uses this function
		 * strLen = HUD::GET_LENGTH_OF_LITERAL_STRING(MISC::GET_ONSCREEN_KEYBOARD_RESULT());
		 * subStr = HUD::GET_CHARACTER_FROM_AUDIO_CONVERSATION_FILENAME_WITH_BYTE_LIMIT(MISC::GET_ONSCREEN_KEYBOARD_RESULT(), 0, strLen, 63);
		 * 
		 * --
		 * 
		 * "fm_race_creator.ysc", line 85115:
		 * // parameters modified for clarity
		 * BOOL sub_8e5aa(char *text, int length) {
		 * for (i = 0; i <= (length - 2); i += 1) {
		 * if (!MISC::ARE_STRINGS_EQUAL(HUD::GET_CHARACTER_FROM_AUDIO_CONVERSATION_FILENAME_WITH_BYTE_LIMIT(text, i, i + 1, 1), " ")) {
		 * return FALSE;
		 * }
		 * }
		 * return TRUE;
		 * }
		 */
		const char* GET_CHARACTER_FROM_AUDIO_CONVERSATION_FILENAME_WITH_BYTE_LIMIT(const char* text, int position, int length, int maxLength);
		/**
		 * Returns a substring that is between two specified positions. The length of the string will be calculated using (endPosition - startPosition).
		 * 
		 * Example:
		 * // Get "STRING" text from "MY_STRING"
		 * subStr = HUD::GET_CHARACTER_FROM_AUDIO_CONVERSATION_FILENAME_BYTES("MY_STRING", 3, 9);
		 * // Overflows are possibly replaced with underscores (needs verification)
		 * subStr = HUD::GET_CHARACTER_FROM_AUDIO_CONVERSATION_FILENAME_BYTES("MY_STRING", 3, 10); // "STRING_"?
		 */
		const char* GET_CHARACTER_FROM_AUDIO_CONVERSATION_FILENAME_BYTES(const char* text, int startPosition, int endPosition);
		/**
		 * Gets a localized string literal from a label name. Can be used for output of e.g. VEHICLE::GET_LIVERY_NAME. To check if a GXT label can be localized with this, HUD::DOES_TEXT_LABEL_EXIST can be used.
		 */
		const char* GET_FILENAME_FOR_AUDIO_CONVERSATION(const char* labelName);
		void CLEAR_PRINTS();
		void CLEAR_BRIEF();
		void CLEAR_ALL_HELP_MESSAGES();
		/**
		 * p0: found arguments in the b617d scripts: https://pastebin.com/X5akCN7z
		 */
		void CLEAR_THIS_PRINT(const char* p0);
		void CLEAR_SMALL_PRINTS();
		BOOL DOES_TEXT_BLOCK_EXIST(const char* gxt);
		/**
		 * Request a gxt into the passed slot.
		 */
		void REQUEST_ADDITIONAL_TEXT(const char* gxt, int slot);
		void REQUEST_ADDITIONAL_TEXT_FOR_DLC(const char* gxt, int slot);
		BOOL HAS_ADDITIONAL_TEXT_LOADED(int slot);
		void CLEAR_ADDITIONAL_TEXT(int p0, BOOL p1);
		BOOL IS_STREAMING_ADDITIONAL_TEXT(int p0);
		/**
		 * Checks if the specified gxt has loaded into the passed slot.
		 */
		BOOL HAS_THIS_ADDITIONAL_TEXT_LOADED(const char* gxt, int slot);
		BOOL IS_MESSAGE_BEING_DISPLAYED();
		/**
		 * Checks if the passed gxt name exists in the game files.
		 */
		BOOL DOES_TEXT_LABEL_EXIST(const char* gxt);
		const char* GET_FIRST_N_CHARACTERS_OF_LITERAL_STRING(const char* string, int length);
		/**
		 * Returns the string length of the string from the gxt string .
		 */
		int GET_LENGTH_OF_STRING_WITH_THIS_TEXT_LABEL(const char* gxt);
		/**
		 * Returns the length of the string passed (much like strlen).
		 */
		int GET_LENGTH_OF_LITERAL_STRING(const char* string);
		int GET_LENGTH_OF_LITERAL_STRING_IN_BYTES(const char* string);
		/**
		 * This functions converts the hash of a street name into a readable string.
		 * 
		 * For how to get the hashes, see PATHFIND::GET_STREET_NAME_AT_COORD.
		 */
		const char* GET_STREET_NAME_FROM_HASH_KEY(Hash hash);
		BOOL IS_HUD_PREFERENCE_SWITCHED_ON();
		BOOL IS_RADAR_PREFERENCE_SWITCHED_ON();
		BOOL IS_SUBTITLE_PREFERENCE_SWITCHED_ON();
		/**
		 * If Hud should be displayed
		 */
		void DISPLAY_HUD(BOOL toggle);
		/**
		 * Enables drawing some hud components, such as help labels, this frame, when the player is dead.
		 */
		void DISPLAY_HUD_WHEN_NOT_IN_STATE_OF_PLAY_THIS_FRAME();
		void DISPLAY_HUD_WHEN_PAUSED_THIS_FRAME();
		/**
		 * If Minimap / Radar should be displayed.
		 */
		void DISPLAY_RADAR(BOOL toggle);
		/**
		 * Setter for GET_FAKE_SPECTATOR_MODE
		 */
		void SET_FAKE_SPECTATOR_MODE(BOOL toggle);
		/**
		 * Getter for SET_FAKE_SPECTATOR_MODE
		 */
		BOOL GET_FAKE_SPECTATOR_MODE();
		BOOL IS_HUD_HIDDEN();
		BOOL IS_RADAR_HIDDEN();
		BOOL IS_MINIMAP_RENDERING();
		void USE_VEHICLE_TARGETING_RETICULE(Any p0);
		void _USE_VEHICLE_TARGETING_RETICULE_ON_VEHICLES(BOOL enable);
		void ADD_VALID_VEHICLE_HIT_HASH(Any p0);
		void CLEAR_VALID_VEHICLE_HIT_HASHES();
		/**
		 * Enable / disable showing route for the Blip-object.
		 */
		void SET_BLIP_ROUTE(Blip blip, BOOL enabled);
		void CLEAR_ALL_BLIP_ROUTES();
		void SET_BLIP_ROUTE_COLOUR(Blip blip, int colour);
		void SET_FORCE_SHOW_GPS(BOOL toggle);
		void SET_USE_SET_DESTINATION_IN_PAUSE_MAP(BOOL toggle);
		void SET_BLOCK_WANTED_FLASH(BOOL disabled);
		void ADD_NEXT_MESSAGE_TO_PREVIOUS_BRIEFS(BOOL p0);
		void FORCE_NEXT_MESSAGE_TO_PREVIOUS_BRIEFS_LIST(int p0);
		/**
		 * zoom ranges from 0 to 90f in R* Scripts
		 */
		void SET_RADAR_ZOOM_PRECISE(float zoom);
		/**
		 * zoomLevel ranges from 0 to 1400 in R* Scripts
		 */
		void SET_RADAR_ZOOM(int zoomLevel);
		void SET_RADAR_ZOOM_TO_BLIP(Blip blip, float zoom);
		void SET_RADAR_ZOOM_TO_DISTANCE(float zoom);
		/**
		 * Does nothing (it's a nullsub).
		 */
		void UPDATE_RADAR_ZOOM_TO_BLIP();
		void GET_HUD_COLOUR(int hudColorIndex, int* r, int* g, int* b, int* a);
		/**
		 * Sets the color of HUD_COLOUR_SCRIPT_VARIABLE
		 */
		void SET_SCRIPT_VARIABLE_HUD_COLOUR(int r, int g, int b, int a);
		/**
		 * Sets the color of HUD_COLOUR_SCRIPT_VARIABLE_2
		 */
		void SET_SECOND_SCRIPT_VARIABLE_HUD_COLOUR(int r, int g, int b, int a);
		/**
		 * makes hudColorIndex2 color into hudColorIndex color
		 */
		void REPLACE_HUD_COLOUR(int hudColorIndex, int hudColorIndex2);
		void REPLACE_HUD_COLOUR_WITH_RGBA(int hudColorIndex, int r, int g, int b, int a);
		void SET_ABILITY_BAR_VISIBILITY(BOOL visible);
		void SET_ALLOW_ABILITY_BAR(BOOL toggle);
		void FLASH_ABILITY_BAR(int millisecondsToFlash);
		void SET_ABILITY_BAR_VALUE(float p0, float p1);
		void FLASH_WANTED_DISPLAY(BOOL p0);
		void FORCE_OFF_WANTED_STAR_FLASH(BOOL toggle);
		void SET_CUSTOM_MP_HUD_COLOR(int hudColorId);
		/**
		 * This gets the height of the FONT and not the total text. You need to get the number of lines your text uses, and get the height of a newline (I'm using a smaller value) to get the total text height.
		 */
		float GET_RENDERED_CHARACTER_HEIGHT(float size, int font);
		/**
		 * Size range : 0F to 1.0F
		 * p0 is unknown and doesn't seem to have an effect, yet in the game scripts it changes to 1.0F sometimes.
		 */
		void SET_TEXT_SCALE(float scale, float size);
		/**
		 * colors you input not same as you think?
		 * A: for some reason its R B G A
		 */
		void SET_TEXT_COLOUR(int red, int green, int blue, int alpha);
		void SET_TEXT_CENTRE(BOOL align);
		void SET_TEXT_RIGHT_JUSTIFY(BOOL toggle);
		/**
		 * Types -
		 * 0: Center-Justify
		 * 1: Left-Justify
		 * 2: Right-Justify
		 * 
		 * Right-Justify requires SET_TEXT_WRAP, otherwise it will draw to the far right of the screen
		 */
		void SET_TEXT_JUSTIFICATION(int justifyType);
		void SET_TEXT_LINE_HEIGHT_MULT(float lineHeightMult);
		/**
		 * It sets the text in a specified box and wraps the text if it exceeds the boundries. Both values are for X axis. Useful when positioning text set to center or aligned to the right.
		 * 
		 * start - left boundry on screen position (0.0 - 1.0)
		 * end - right boundry on screen position (0.0 - 1.0)
		 */
		void SET_TEXT_WRAP(float start, float end);
		void SET_TEXT_LEADING(int p0);
		/**
		 * This native does absolutely nothing, just a nullsub
		 */
		void SET_TEXT_PROPORTIONAL(BOOL p0);
		/**
		 * fonts that mess up your text where made for number values/misc stuff
		 */
		void SET_TEXT_FONT(int fontType);
		void SET_TEXT_DROP_SHADOW();
		/**
		 * distance - shadow distance in pixels, both horizontal and vertical
		 * r, g, b, a - color
		 */
		void SET_TEXT_DROPSHADOW(int distance, int r, int g, int b, int a);
		void SET_TEXT_OUTLINE();
		/**
		 * This native does absolutely nothing, just a nullsub
		 */
		void SET_TEXT_EDGE(int p0, int r, int g, int b, int a);
		void SET_TEXT_RENDER_ID(int renderId);
		/**
		 * This function is hard-coded to always return 1.
		 */
		int GET_DEFAULT_SCRIPT_RENDERTARGET_RENDER_ID();
		BOOL REGISTER_NAMED_RENDERTARGET(const char* name, BOOL p1);
		BOOL IS_NAMED_RENDERTARGET_REGISTERED(const char* name);
		BOOL RELEASE_NAMED_RENDERTARGET(const char* name);
		void LINK_NAMED_RENDERTARGET(Hash modelHash);
		int GET_NAMED_RENDERTARGET_RENDER_ID(const char* name);
		BOOL IS_NAMED_RENDERTARGET_LINKED(Hash modelHash);
		void CLEAR_HELP(BOOL toggle);
		BOOL IS_HELP_MESSAGE_ON_SCREEN();
		BOOL HAS_SCRIPT_HIDDEN_HELP_THIS_FRAME();
		BOOL IS_HELP_MESSAGE_BEING_DISPLAYED();
		BOOL IS_HELP_MESSAGE_FADING_OUT();
		void SET_HELP_MESSAGE_STYLE(int style, int hudColor, int alpha, int p3, int p4);
		int GET_STANDARD_BLIP_ENUM_ID();
		int GET_WAYPOINT_BLIP_ENUM_ID();
		int GET_NUMBER_OF_ACTIVE_BLIPS();
		Blip GET_NEXT_BLIP_INFO_ID(int blipSprite);
		Blip GET_FIRST_BLIP_INFO_ID(int blipSprite);
		Blip GET_CLOSEST_BLIP_INFO_ID(int blipSprite);
		Vector3 GET_BLIP_INFO_ID_COORD(Blip blip);
		int GET_BLIP_INFO_ID_DISPLAY(Blip blip);
		/**
		 * Returns a value based on what the blip is attached to
		 * 1 - Vehicle
		 * 2 - Ped
		 * 3 - Object
		 * 4 - Coord
		 * 5 - unk
		 * 6 - Pickup
		 * 7 - Radius
		 */
		int GET_BLIP_INFO_ID_TYPE(Blip blip);
		Entity GET_BLIP_INFO_ID_ENTITY_INDEX(Blip blip);
		/**
		 * This function is hard-coded to always return 0.
		 */
		Pickup GET_BLIP_INFO_ID_PICKUP_INDEX(Blip blip);
		/**
		 * Returns the Blip handle of given Entity.
		 */
		Blip GET_BLIP_FROM_ENTITY(Entity entity);
		Blip ADD_BLIP_FOR_RADIUS(float posX, float posY, float posZ, float radius);
		/**
		 * Adds a rectangular blip for the specified coordinates/area.
		 * 
		 * It is recommended to use SET_BLIP_ROTATION and SET_BLIP_COLOUR to make the blip not rotate along with the camera.
		 * 
		 * By default, the blip will show as a _regular_ blip with the specified color/sprite if it is outside of the minimap view.
		 * 
		 * Example image:
		 * minimap https://w.wew.wtf/pdcjig.png
		 * big map https://w.wew.wtf/zgcjcm.png
		 * 
		 * (Native name is _likely_ to actually be ADD_BLIP_FOR_AREA, but due to the usual reasons this can't be confirmed)
		 */
		Blip ADD_BLIP_FOR_AREA(float x, float y, float z, float width, float height);
		/**
		 * Returns red ( default ) blip attached to entity.
		 * 
		 * Example:
		 * Blip blip; //Put this outside your case or option
		 * blip = HUD::ADD_BLIP_FOR_ENTITY(YourPedOrBodyguardName);
		 * HUD::SET_BLIP_AS_FRIENDLY(blip, true);
		 */
		Blip ADD_BLIP_FOR_ENTITY(Entity entity);
		Blip ADD_BLIP_FOR_PICKUP(Pickup pickup);
		/**
		 * Creates an orange ( default ) Blip-object. Returns a Blip-object which can then be modified.
		 */
		Blip ADD_BLIP_FOR_COORD(float x, float y, float z);
		void TRIGGER_SONAR_BLIP(float posX, float posY, float posZ, float radius, int p4);
		void ALLOW_SONAR_BLIPS(BOOL toggle);
		void SET_BLIP_COORDS(Blip blip, float posX, float posY, float posZ);
		Vector3 GET_BLIP_COORDS(Blip blip);
		/**
		 * Sets the displayed sprite for a specific blip..
		 * 
		 * You may have your own list, but since dev-c didn't show it I was bored and started looking through scripts and functions to get a presumable almost positive list of a majority of blip IDs
		 * https://pastebin.com/Bpj9Sfft
		 * 
		 * Blips Images + IDs:
		 * https://gtaxscripting.blogspot.com/2016/05/gta-v-blips-id-and-image.html
		 */
		void SET_BLIP_SPRITE(Blip blip, int spriteId);
		/**
		 * Blips Images + IDs:
		 * gtaxscripting.blogspot.com/2016/05/gta-v-blips-id-and-image.html
		 */
		int GET_BLIP_SPRITE(Blip blip);
		void SET_COP_BLIP_SPRITE(int p0, float p1);
		void SET_COP_BLIP_SPRITE_AS_STANDARD();
		/**
		 * Doesn't work if the label text of gxtEntry is >= 80.
		 */
		void SET_BLIP_NAME_FROM_TEXT_FILE(Blip blip, const char* gxtEntry);
		void SET_BLIP_NAME_TO_PLAYER_NAME(Blip blip, Player player);
		/**
		 * Sets alpha-channel for blip color.
		 * 
		 * Example:
		 * 
		 * Blip blip = HUD::ADD_BLIP_FOR_ENTITY(entity);
		 * HUD::SET_BLIP_COLOUR(blip , 3);
		 * HUD::SET_BLIP_ALPHA(blip , 64);
		 * 
		 */
		void SET_BLIP_ALPHA(Blip blip, int alpha);
		int GET_BLIP_ALPHA(Blip blip);
		void SET_BLIP_FADE(Blip blip, int opacity, int duration);
		/**
		 * Returns -1, 0, +1, depending on if the blip is fading out, doing nothing, or fading in respectively.
		 */
		int GET_BLIP_FADE_DIRECTION(Blip blip);
		/**
		 * After some testing, looks like you need to use CEIL() on the rotation (vehicle/ped heading) before using it there.
		 */
		void SET_BLIP_ROTATION(Blip blip, int rotation);
		/**
		 * Does not require whole number/integer rotations.
		 */
		void SET_BLIP_ROTATION_WITH_FLOAT(Blip blip, float heading);
		int GET_BLIP_ROTATION(Blip blip);
		/**
		 * Adds up after viewing multiple R* scripts. I believe that the duration is in miliseconds.
		 */
		void SET_BLIP_FLASH_TIMER(Blip blip, int duration);
		void SET_BLIP_FLASH_INTERVAL(Blip blip, Any p1);
		/**
		 * https://gtaforums.com/topic/864881-all-blip-color-ids-pictured/
		 */
		void SET_BLIP_COLOUR(Blip blip, int color);
		/**
		 * Can be used to give blips any RGB colour with SET_BLIP_COLOUR(blip, 84).
		 */
		void SET_BLIP_SECONDARY_COLOUR(Blip blip, int r, int g, int b);
		int GET_BLIP_COLOUR(Blip blip);
		int GET_BLIP_HUD_COLOUR(Blip blip);
		BOOL IS_BLIP_SHORT_RANGE(Blip blip);
		BOOL IS_BLIP_ON_MINIMAP(Blip blip);
		BOOL DOES_BLIP_HAVE_GPS_ROUTE(Blip blip);
		void SET_BLIP_HIDDEN_ON_LEGEND(Blip blip, BOOL toggle);
		void SET_BLIP_HIGH_DETAIL(Blip blip, BOOL toggle);
		void SET_BLIP_AS_MISSION_CREATOR_BLIP(Blip blip, BOOL toggle);
		BOOL IS_MISSION_CREATOR_BLIP(Blip blip);
		Blip GET_NEW_SELECTED_MISSION_CREATOR_BLIP();
		BOOL IS_HOVERING_OVER_MISSION_CREATOR_BLIP();
		void SHOW_START_MISSION_INSTRUCTIONAL_BUTTON(BOOL toggle);
		void SHOW_CONTACT_INSTRUCTIONAL_BUTTON(BOOL toggle);
		void _SHOW_PURCHASE_INSTRUCTIONAL_BUTTON(BOOL toggle); // Missing in crossmap
		void RELOAD_MAP_MENU();
		void SET_BLIP_MARKER_LONG_DISTANCE(Any p0, Any p1);
		void SET_BLIP_FLASHES(Blip blip, BOOL toggle);
		void SET_BLIP_FLASHES_ALTERNATE(Blip blip, BOOL toggle);
		BOOL IS_BLIP_FLASHING(Blip blip);
		/**
		 * Sets whether or not the specified blip should only be displayed when nearby, or on the minimap.
		 */
		void SET_BLIP_AS_SHORT_RANGE(Blip blip, BOOL toggle);
		void SET_BLIP_SCALE(Blip blip, float scale);
		/**
		 * See https://imgur.com/a/lLkEsMN
		 */
		void SET_BLIP_SCALE_2D(Blip blip, float xScale, float yScale);
		/**
		 * See this topic for more details : gtaforums.com/topic/717612-v-scriptnative-documentation-and-research/page-35?p=1069477935
		 */
		void SET_BLIP_PRIORITY(Blip blip, int priority);
		/**
		 * Display Id behaviours:
		 * 0 = Doesn't show up, ever, anywhere.
		 * 1 = Doesn't show up, ever, anywhere.
		 * 2 = Shows on both main map and minimap. (Selectable on map)
		 * 3 = Shows on main map only. (Selectable on map)
		 * 4 = Shows on main map only. (Selectable on map)
		 * 5 = Shows on minimap only.
		 * 6 = Shows on both main map and minimap. (Selectable on map)
		 * 7 = Doesn't show up, ever, anywhere.
		 * 8 = Shows on both main map and minimap. (Not selectable on map)
		 * 9 = Shows on minimap only.
		 * 10 = Shows on both main map and minimap. (Not selectable on map)
		 * 
		 * Anything higher than 10 seems to be exactly the same as 10.
		 */
		void SET_BLIP_DISPLAY(Blip blip, int displayId);
		/**
		 * Example: https://i.imgur.com/skY6vAJ.png
		 * 
		 * Index:
		 * 1 = No distance shown in legend
		 * 2 = Distance shown in legend
		 * 7 = "Other Players" category, also shows distance in legend
		 * 10 = "Property" category
		 * 11 = "Owned Property" category
		 * 
		 * Any other value behaves like index = 1, index wraps around after 255
		 * Blips with categories 7, 10 or 11 will all show under the specific categories listing in the map legend, regardless of sprite or name.
		 * Legend entries:
		 * 7 = Other Players (BLIP_OTHPLYR)
		 * 10 = Property (BLIP_PROPCAT)
		 * 11 = Owned Property (BLIP_APARTCAT)
		 * 
		 * Category needs to be `7` in order for blip names to show on the expanded minimap when using DISPLAY_PLAYER_NAME_TAGS_ON_BLIPS.
		 */
		void SET_BLIP_CATEGORY(Blip blip, int index);
		/**
		 * In the C++ SDK, this seems not to work-- the blip isn't removed immediately. I use it for saving cars.
		 * 
		 * E.g.:
		 * 
		 * Ped pped = PLAYER::PLAYER_PED_ID();
		 * Vehicle v = PED::GET_VEHICLE_PED_IS_USING(pped);
		 * Blip b = HUD::ADD_BLIP_FOR_ENTITY(v);
		 * 
		 * works fine.
		 * But later attempting to delete it with:
		 * 
		 * Blip b = HUD::GET_BLIP_FROM_ENTITY(v);
		 * if (HUD::DOES_BLIP_EXIST(b)) HUD::REMOVE_BLIP(&b);
		 * 
		 * doesn't work. And yes, doesn't work without the DOES_BLIP_EXIST check either. Also, if you attach multiple blips to the same thing (say, a vehicle), and that thing disappears, the blips randomly attach to other things (in my case, a vehicle).
		 * 
		 * Thus for me, HUD::REMOVE_BLIP(&b) only works if there's one blip, (in my case) the vehicle is marked as no longer needed, you drive away from it and it eventually despawns, AND there is only one blip attached to it. I never intentionally attach multiple blips but if the user saves the car, this adds a blip. Then if they delete it, it is supposed to remove the blip, but it doesn't. Then they can immediately save it again, causing another blip to re-appear.
		 * -------------
		 * 
		 * Passing the address of the variable instead of the value works for me.
		 * e.g.
		 * int blip = HUD::ADD_BLIP_FOR_ENTITY(ped);
		 * HUD::REMOVE_BLIP(&blip);
		 * 
		 * 
		 * Remove blip will currently crash your game, just artificially remove the blip by setting the sprite to a id that is 'invisible'.
		 */
		void REMOVE_BLIP(Blip* blip);
		/**
		 * false for enemy
		 * true for friendly
		 */
		void SET_BLIP_AS_FRIENDLY(Blip blip, BOOL toggle);
		void PULSE_BLIP(Blip blip);
		void SHOW_NUMBER_ON_BLIP(Blip blip, int number);
		void HIDE_NUMBER_ON_BLIP(Blip blip);
		void SHOW_HEIGHT_ON_BLIP(Blip blip, BOOL toggle);
		/**
		 * Adds a green checkmark on top of a blip.
		 */
		void SHOW_TICK_ON_BLIP(Blip blip, BOOL toggle);
		/**
		 * Adds a orange checkmark on top of a given blip handle: https://imgur.com/a/aw5OTMF
		 * _SHOW_FRIEND_INDICATOR_ON_BLIP* - _SHOW_HEADING_INDICATOR_ON_BLIP*
		 */
		void SHOW_GOLD_TICK_ON_BLIP(Blip blip, BOOL toggle);
		void SHOW_FOR_SALE_ICON_ON_BLIP(Blip blip, BOOL toggle);
		/**
		 * Adds the GTA: Online player heading indicator to a blip.
		 */
		void SHOW_HEADING_INDICATOR_ON_BLIP(Blip blip, BOOL toggle);
		/**
		 * Highlights a blip by a cyan color circle.
		 * 
		 * Color can be changed with SET_BLIP_SECONDARY_COLOUR
		 */
		void SHOW_OUTLINE_INDICATOR_ON_BLIP(Blip blip, BOOL toggle);
		/**
		 * Highlights a blip by a half cyan circle on the right side of the blip. https://i.imgur.com/FrV9M4e.png
		 * .Indicating that that player is a friend (in GTA:O). This color can not be changed.
		 * To toggle the left side (crew member indicator) of the half circle around the blip, use: `SHOW_CREW_INDICATOR_ON_BLIP`
		 */
		void SHOW_FRIEND_INDICATOR_ON_BLIP(Blip blip, BOOL toggle);
		/**
		 * Enables or disables the blue half circle https://i.imgur.com/iZes9Ec.png around the specified blip on the left side of the blip. This is used to indicate that the player is in your crew in GTA:O. Color is changeable by using `SET_BLIP_SECONDARY_COLOUR`.
		 */
		void SHOW_CREW_INDICATOR_ON_BLIP(Blip blip, BOOL toggle);
		/**
		 * Must be toggled before being queued for animation
		 */
		void SET_BLIP_EXTENDED_HEIGHT_THRESHOLD(Blip blip, BOOL toggle);
		void SET_BLIP_SHORT_HEIGHT_THRESHOLD(Any p0, Any p1);
		void SET_BLIP_USE_HEIGHT_INDICATOR_ON_EDGE(Blip blip, Any p1);
		/**
		 * Makes a blip go small when off the minimap.
		 */
		void SET_BLIP_AS_MINIMAL_ON_EDGE(Blip blip, BOOL toggle);
		/**
		 * Enabling this on a radius blip will make it outline only. See https://cdn.discordapp.com/attachments/553235301632573459/575132227935928330/unknown.png
		 */
		void SET_RADIUS_BLIP_EDGE(Blip blip, BOOL toggle);
		BOOL DOES_BLIP_EXIST(Blip blip);
		/**
		 * This native removes the current waypoint from the map.
		 * 
		 * Example:
		 * C#:
		 * Function.Call(Hash.SET_WAYPOINT_OFF);
		 * 
		 * C++:
		 * HUD::SET_WAYPOINT_OFF();
		 */
		void SET_WAYPOINT_OFF();
		void DELETE_WAYPOINTS_FROM_THIS_PLAYER();
		void REFRESH_WAYPOINT();
		BOOL IS_WAYPOINT_ACTIVE();
		void SET_NEW_WAYPOINT(float x, float y);
		void GET_WAYPOINT_CLEAR_ON_ARRIVAL_MODE(); // Missing in crossmap
		void SET_WAYPOINT_CLEAR_ON_ARRIVAL_MODE(int mode); // Missing in crossmap
		void SET_BLIP_BRIGHT(Blip blip, BOOL toggle);
		/**
		 * As of b2189, the third parameter sets the color of the cone (before b2189 it was ignored). Note that it uses HUD colors, not blip colors.
		 */
		void SET_BLIP_SHOW_CONE(Blip blip, BOOL toggle, int hudColorIndex);
		/**
		 * Interesting fact: A hash collision for this is RESET_JETPACK_MODEL_SETTINGS
		 */
		void REMOVE_COP_BLIP_FROM_PED(Ped ped);
		void SETUP_FAKE_CONE_DATA(Blip blip, float p1, float p2, float p3, float p4, float p5, float p6, Any p7, int p8);
		void REMOVE_FAKE_CONE_DATA(Blip blip);
		void CLEAR_FAKE_CONE_ARRAY();
		/**
		 * Applies to new eBlipParams _BLIP_CHANGE_46* and _BLIP_CHANGE_47*
		 */
		void _SET_BLIP_GPS_ROUTE_DISPLAY_DISTANCE(Blip blip, int blipChangeParam46, BOOL blipChangeParam47);
		/**
		 * This native is used to colorize certain map components like the army base at the top of the map.
		 * p2 appears to be always -1. If p2 is -1 then native wouldn't change the color. See https://gfycat.com/SkinnyPinkChupacabra
		 */
		BOOL SET_MINIMAP_COMPONENT(int componentId, BOOL toggle, int overrideColor);
		void SET_MINIMAP_SONAR_SWEEP(BOOL toggle);
		void SHOW_ACCOUNT_PICKER();
		Blip GET_MAIN_PLAYER_BLIP_ID();
		void SET_PM_WARNINGSCREEN_ACTIVE(BOOL p0);
		void HIDE_LOADING_ON_FADE_THIS_FRAME();
		/**
		 * List of interior hashes: https://pastebin.com/1FUyXNqY
		 * Not for every interior zoom > 0 available.
		 */
		void SET_RADAR_AS_INTERIOR_THIS_FRAME(Hash interior, float x, float y, int z, int zoom);
		void SET_INSIDE_VERY_SMALL_INTERIOR(BOOL toggle);
		void SET_INSIDE_VERY_LARGE_INTERIOR(BOOL toggle);
		void SET_RADAR_AS_EXTERIOR_THIS_FRAME();
		/**
		 * Sets the position of the arrow icon representing the player on both the minimap and world map.
		 * 
		 * Too bad this wouldn't work over the network (obviously not). Could spoof where we would be.
		 */
		void SET_FAKE_PAUSEMAP_PLAYER_POSITION_THIS_FRAME(float x, float y);
		void SET_FAKE_GPS_PLAYER_POSITION_THIS_FRAME(float x, float y, float z);
		BOOL IS_PAUSEMAP_IN_INTERIOR_MODE();
		void HIDE_MINIMAP_EXTERIOR_MAP_THIS_FRAME();
		void HIDE_MINIMAP_INTERIOR_MAP_THIS_FRAME();
		/**
		 * Toggles the Cayo Perico map.
		 */
		void SET_USE_ISLAND_MAP(BOOL toggle);
		void _SET_PAUSE_EXTERIOR_RENDERING_WHILE_IN_INTERIOR();
		/**
		 * When calling this, the current frame will have the players "arrow icon" be focused on the dead center of the radar.
		 */
		void DONT_TILT_MINIMAP_THIS_FRAME();
		void DONT_ZOOM_MINIMAP_WHEN_RUNNING_THIS_FRAME();
		void DONT_ZOOM_MINIMAP_WHEN_SNIPING_THIS_FRAME();
		void SET_WIDESCREEN_FORMAT(Any p0);
		void DISPLAY_AREA_NAME(BOOL toggle);
		/**
		 * "DISPLAY_CASH(false);" makes the cash amount render on the screen when appropriate
		 * "DISPLAY_CASH(true);" disables cash amount rendering
		 */
		void DISPLAY_CASH(BOOL toggle);
		/**
		 * Related to displaying cash on the HUD
		 * Always called before HUD::CHANGE_FAKE_MP_CASH in decompiled scripts
		 */
		void USE_FAKE_MP_CASH(BOOL toggle);
		/**
		 * Displays cash change notifications on HUD.
		 */
		void CHANGE_FAKE_MP_CASH(int cash, int bank);
		void DISPLAY_AMMO_THIS_FRAME(BOOL display);
		/**
		 * Displays the crosshair for this frame.
		 */
		void DISPLAY_SNIPER_SCOPE_THIS_FRAME();
		/**
		 * Hides HUD and radar this frame and prohibits switching to other weapons (or accessing the weapon wheel)
		 */
		void HIDE_HUD_AND_RADAR_THIS_FRAME();
		/**
		 * Controls whether to display 'Cash'/'Bank' next to the money balance HUD in Multiplayer (https://i.imgur.com/MiYUtNl.png)
		 */
		void ALLOW_DISPLAY_OF_MULTIPLAYER_CASH_TEXT(BOOL allow);
		void SET_MULTIPLAYER_WALLET_CASH();
		void REMOVE_MULTIPLAYER_WALLET_CASH();
		void SET_MULTIPLAYER_BANK_CASH();
		void REMOVE_MULTIPLAYER_BANK_CASH();
		/**
		 * This native does absolutely nothing, just a nullsub
		 */
		void SET_MULTIPLAYER_HUD_CASH(int p0, BOOL p1);
		/**
		 * Removes multiplayer cash hud each frame
		 */
		void REMOVE_MULTIPLAYER_HUD_CASH();
		void HIDE_HELP_TEXT_THIS_FRAME();
		BOOL IS_IME_IN_PROGRESS();
		/**
		 * The messages are localized strings.
		 * Examples:
		 * "No_bus_money"
		 * "Enter_bus"
		 * "Tour_help"
		 * "LETTERS_HELP2"
		 * "Dummy"
		 * 
		 * **The bool appears to always be false (if it even is a bool, as it's represented by a zero)**
		 * --------
		 * p1 doesn't seem to make a difference, regardless of the state it's in.
		 * 
		 * 
		 * picture of where on the screen this is displayed?
		 */
		void DISPLAY_HELP_TEXT_THIS_FRAME(const char* message, BOOL p1);
		/**
		 * Forces the weapon wheel to show/hide.
		 */
		void HUD_FORCE_WEAPON_WHEEL(BOOL show);
		/**
		 * Displays "blazer_wheels_up" and "blazer_wheels_down" "weapon" icons when switching between jetski and quadbike modes. Works only on vehicles using "VEHICLE_TYPE_AMPHIBIOUS_QUADBIKE" vehicle type. Needs to be called every time prior to switching modes, otherwise the icon will only appear when switching modes once.
		 */
		void HUD_FORCE_SPECIAL_VEHICLE_WEAPON_WHEEL();
		/**
		 * Calling this each frame, stops the player from receiving a weapon via the weapon wheel.
		 */
		void HUD_SUPPRESS_WEAPON_WHEEL_RESULTS_THIS_FRAME();
		/**
		 * Returns the weapon hash to the selected/highlighted weapon in the wheel
		 */
		Hash HUD_GET_WEAPON_WHEEL_CURRENTLY_HIGHLIGHTED();
		/**
		 * Set the active slotIndex in the wheel weapon to the slot associated with the provided Weapon hash
		 */
		void HUD_SET_WEAPON_WHEEL_TOP_SLOT(Hash weaponHash);
		/**
		 * Returns the weapon hash active in a specific weapon wheel slotList
		 */
		Hash HUD_GET_WEAPON_WHEEL_TOP_SLOT(int weaponTypeIndex);
		/**
		 * Sets a global that disables many weapon input tasks (shooting, aiming, etc.). Does not work with vehicle weapons, only used in selector.ysc
		 */
		void HUD_SHOWING_CHARACTER_SWITCH_SELECTION(BOOL toggle);
		/**
		 * Only the script that originally called SET_GPS_FLAGS can set them again. Another script cannot set the flags, until the first script that called it has called CLEAR_GPS_FLAGS.
		 * 
		 * Doesn't seem like the flags are actually read by the game at all.
		 */
		void SET_GPS_FLAGS(int p0, float p1);
		/**
		 * Clears the GPS flags. Only the script that originally called SET_GPS_FLAGS can clear them.
		 * 
		 * Doesn't seem like the flags are actually read by the game at all.
		 */
		void CLEAR_GPS_FLAGS();
		void SET_RACE_TRACK_RENDER(BOOL toggle);
		/**
		 * Does the same as SET_RACE_TRACK_RENDER(false);
		 */
		void CLEAR_GPS_RACE_TRACK();
		/**
		 * Starts a new GPS custom-route, allowing you to plot lines on the map.
		 * Lines are drawn directly between points.
		 * The GPS custom route works like the GPS multi route, except it does not follow roads.
		 * Example result: https://i.imgur.com/BDm5pzt.png
		 * hudColor: The HUD color of the GPS path.
		 * displayOnFoot: Draws the path regardless if the player is in a vehicle or not.
		 * followPlayer: Draw the path partially between the previous and next point based on the players position between them. When false, the GPS appears to not disappear after the last leg is completed.
		 */
		void START_GPS_CUSTOM_ROUTE(int hudColor, BOOL displayOnFoot, BOOL followPlayer);
		void ADD_POINT_TO_GPS_CUSTOM_ROUTE(float x, float y, float z);
		/**
		 * radarThickness: The width of the GPS route on the radar
		 * mapThickness: The width of the GPS route on the map
		 */
		void SET_GPS_CUSTOM_ROUTE_RENDER(BOOL toggle, int radarThickness, int mapThickness);
		void CLEAR_GPS_CUSTOM_ROUTE();
		/**
		 * Starts a new GPS multi-route, allowing you to create custom GPS paths.
		 * GPS functions like the waypoint, except it can contain multiple points it's forced to go through.
		 * Once the player has passed a point, the GPS will no longer force its path through it.
		 * 
		 * Works independently from the player-placed waypoint and blip routes.
		 * Example result: https://i.imgur.com/ZZHQatX.png
		 * hudColor: The HUD color of the GPS path.
		 * routeFromPlayer: Makes the GPS draw a path from the player to the next point, rather than the original path from the previous point.
		 * displayOnFoot: Draws the GPS path regardless if the player is in a vehicle or not.
		 */
		void START_GPS_MULTI_ROUTE(int hudColor, BOOL routeFromPlayer, BOOL displayOnFoot);
		void ADD_POINT_TO_GPS_MULTI_ROUTE(float x, float y, float z);
		void SET_GPS_MULTI_ROUTE_RENDER(BOOL toggle);
		/**
		 * Does the same as SET_GPS_MULTI_ROUTE_RENDER(false);
		 */
		void CLEAR_GPS_MULTI_ROUTE();
		void CLEAR_GPS_PLAYER_WAYPOINT();
		void SET_GPS_FLASHES(BOOL toggle);
		void SET_PLAYER_ICON_COLOUR(int color);
		/**
		 * adds a short flash to the Radar/Minimap
		 * Usage: UI.FLASH_MINIMAP_DISPLAY
		 */
		void FLASH_MINIMAP_DISPLAY();
		void FLASH_MINIMAP_DISPLAY_WITH_COLOR(int hudColorIndex);
		void TOGGLE_STEALTH_RADAR(BOOL toggle);
		void SET_MINIMAP_IN_SPECTATOR_MODE(BOOL toggle, Ped ped);
		void SET_MISSION_NAME(BOOL p0, const char* name);
		void SET_MISSION_NAME_FOR_UGC_MISSION(BOOL p0, const char* name);
		void SET_DESCRIPTION_FOR_UGC_MISSION_EIGHT_STRINGS(BOOL p0, const char* p1, const char* p2, const char* p3, const char* p4, const char* p5, const char* p6, const char* p7, const char* p8);
		void SET_MINIMAP_BLOCK_WAYPOINT(BOOL toggle);
		/**
		 * Toggles the North Yankton map
		 */
		void SET_MINIMAP_IN_PROLOGUE(BOOL toggle);
		/**
		 * This native does absolutely nothing on PC master builds, just a nullsub.
		 */
		void SET_MINIMAP_BACKGROUND_HIDDEN(BOOL toggle);
		/**
		 * If true, the entire map will be revealed.
		 * 
		 * FOW = Fog of War
		 */
		void SET_MINIMAP_HIDE_FOW(BOOL toggle);
		float GET_MINIMAP_FOW_DISCOVERY_RATIO();
		BOOL GET_MINIMAP_FOW_COORDINATE_IS_REVEALED(float x, float y, float z);
		void SET_MINIMAP_FOW_DO_NOT_UPDATE(BOOL p0);
		/**
		 * Up to eight coordinates may be revealed per frame
		 */
		void SET_MINIMAP_FOW_REVEAL_COORDINATE(float x, float y, float z);
		/**
		 * Not much is known so far on what it does _exactly_.
		 * All I know for sure is that it draws the specified hole ID on the pause menu map as well as on the mini-map/radar. This native also seems to change some other things related to the pause menu map's behaviour, for example: you can no longer set waypoints, the pause menu map starts up in a 'zoomed in' state. This native does not need to be executed every tick.
		 * You need to center the minimap manually as well as change/lock it's zoom and angle in order for it to appear correctly on the minimap.
		 * You'll also need to use the `GOLF` scaleform in order to get the correct minmap border to show up.
		 * Use `0x35edd5b2e3ff01c0` to reset the map when you no longer want to display any golf holes (you still need to unlock zoom, position and angle of the radar manually after calling this).
		 */
		void SET_MINIMAP_GOLF_COURSE(int hole);
		void SET_MINIMAP_GOLF_COURSE_OFF();
		/**
		 * Locks the minimap to the specified angle in integer degrees.
		 * 
		 * angle: The angle in whole degrees. If less than 0 or greater than 360, unlocks the angle.
		 */
		void LOCK_MINIMAP_ANGLE(int angle);
		void UNLOCK_MINIMAP_ANGLE();
		/**
		 * Locks the minimap to the specified world position.
		 */
		void LOCK_MINIMAP_POSITION(float x, float y);
		void UNLOCK_MINIMAP_POSITION();
		/**
		 * Argument must be 0.0f or above 38.0f, or it will be ignored.
		 */
		void SET_FAKE_MINIMAP_MAX_ALTIMETER_HEIGHT(float altitude, BOOL p1, Any p2);
		void SET_HEALTH_HUD_DISPLAY_VALUES(int health, int capacity, BOOL wasAdded);
		void SET_MAX_HEALTH_HUD_DISPLAY(int maximumValue);
		void SET_MAX_ARMOUR_HUD_DISPLAY(int maximumValue);
		/**
		 * Toggles the big minimap state like in GTA:Online.
		 */
		void SET_BIGMAP_ACTIVE(BOOL toggleBigMap, BOOL showFullMap);
		/**
		 * Full list of components below
		 * 
		 * HUD = 0;
		 * HUD_WANTED_STARS = 1;
		 * HUD_WEAPON_ICON = 2;
		 * HUD_CASH = 3;
		 * HUD_MP_CASH = 4;
		 * HUD_MP_MESSAGE = 5;
		 * HUD_VEHICLE_NAME = 6;
		 * HUD_AREA_NAME = 7;
		 * HUD_VEHICLE_CLASS = 8;
		 * HUD_STREET_NAME = 9;
		 * HUD_HELP_TEXT = 10;
		 * HUD_FLOATING_HELP_TEXT_1 = 11;
		 * HUD_FLOATING_HELP_TEXT_2 = 12;
		 * HUD_CASH_CHANGE = 13;
		 * HUD_RETICLE = 14;
		 * HUD_SUBTITLE_TEXT = 15;
		 * HUD_RADIO_STATIONS = 16;
		 * HUD_SAVING_GAME = 17;
		 * HUD_GAME_STREAM = 18;
		 * HUD_WEAPON_WHEEL = 19;
		 * HUD_WEAPON_WHEEL_STATS = 20;
		 * MAX_HUD_COMPONENTS = 21;
		 * MAX_HUD_WEAPONS = 22;
		 * MAX_SCRIPTED_HUD_COMPONENTS = 141;
		 */
		BOOL IS_HUD_COMPONENT_ACTIVE(int id);
		BOOL IS_SCRIPTED_HUD_COMPONENT_ACTIVE(int id);
		void HIDE_SCRIPTED_HUD_COMPONENT_THIS_FRAME(int id);
		void SHOW_SCRIPTED_HUD_COMPONENT_THIS_FRAME(int id);
		BOOL IS_SCRIPTED_HUD_COMPONENT_HIDDEN_THIS_FRAME(int id);
		/**
		 * This function hides various HUD (Heads-up Display) components.
		 * Listed below are the integers and the corresponding HUD component.
		 * - 1 : WANTED_STARS
		 * - 2 : WEAPON_ICON
		 * - 3 : CASH
		 * - 4 : MP_CASH
		 * - 5 : MP_MESSAGE
		 * - 6 : VEHICLE_NAME
		 * - 7 : AREA_NAME
		 * - 8 : VEHICLE_CLASS
		 * - 9 : STREET_NAME
		 * - 10 : HELP_TEXT
		 * - 11 : FLOATING_HELP_TEXT_1
		 * - 12 : FLOATING_HELP_TEXT_2
		 * - 13 : CASH_CHANGE
		 * - 14 : RETICLE
		 * - 15 : SUBTITLE_TEXT
		 * - 16 : RADIO_STATIONS
		 * - 17 : SAVING_GAME
		 * - 18 : GAME_STREAM
		 * - 19 : WEAPON_WHEEL
		 * - 20 : WEAPON_WHEEL_STATS
		 * - 21 : HUD_COMPONENTS
		 * - 22 : HUD_WEAPONS
		 * 
		 * These integers also work for the `SHOW_HUD_COMPONENT_THIS_FRAME` native, but instead shows the HUD Component.
		 */
		void HIDE_HUD_COMPONENT_THIS_FRAME(int id);
		void IS_HUD_COMPONENT_HIDDEN_THIS_FRAME(int id); // Missing in crossmap
		/**
		 * This function hides various HUD (Heads-up Display) components.
		 * Listed below are the integers and the corresponding HUD component.
		 * - 1 : WANTED_STARS
		 * - 2 : WEAPON_ICON
		 * - 3 : CASH
		 * - 4 : MP_CASH
		 * - 5 : MP_MESSAGE
		 * - 6 : VEHICLE_NAME
		 * - 7 : AREA_NAME
		 * - 8 : VEHICLE_CLASS
		 * - 9 : STREET_NAME
		 * - 10 : HELP_TEXT
		 * - 11 : FLOATING_HELP_TEXT_1
		 * - 12 : FLOATING_HELP_TEXT_2
		 * - 13 : CASH_CHANGE
		 * - 14 : RETICLE
		 * - 15 : SUBTITLE_TEXT
		 * - 16 : RADIO_STATIONS
		 * - 17 : SAVING_GAME
		 * - 18 : GAME_STREAM
		 * - 19 : WEAPON_WHEEL
		 * - 20 : WEAPON_WHEEL_STATS
		 * - 21 : HUD_COMPONENTS
		 * - 22 : HUD_WEAPONS
		 * 
		 * These integers also work for the `HIDE_HUD_COMPONENT_THIS_FRAME` native, but instead hides the HUD Component.
		 */
		void SHOW_HUD_COMPONENT_THIS_FRAME(int id);
		/**
		 * Hides area and vehicle name HUD components for one frame.
		 */
		void HIDE_STREET_AND_CAR_NAMES_THIS_FRAME();
		void RESET_RETICULE_VALUES();
		void RESET_HUD_COMPONENT_VALUES(int id);
		void SET_HUD_COMPONENT_POSITION(int id, float x, float y);
		Vector3 GET_HUD_COMPONENT_POSITION(int id);
		/**
		 * This native does absolutely nothing, just a nullsub
		 */
		void CLEAR_REMINDER_MESSAGE();
		/**
		 * World to relative screen coords, this world to screen will keep the text on screen. Was named _GET_SCREEN_COORD_FROM_WORLD_COORD, but this conflicts with 0x34E82F05DF2974F5. As that hash actually matches GET_SCREEN_COORD_FROM_WORLD_COORD that one supercedes and this one was renamed to _GET_2D_COORD_FROM_3D_COORD
		 */
		int GET_HUD_SCREEN_POSITION_FROM_WORLD_POSITION(float worldX, float worldY, float worldZ, float* screenX, float* screenY);
		/**
		 * Shows a menu for reporting UGC content.
		 */
		void OPEN_REPORTUGC_MENU();
		void FORCE_CLOSE_REPORTUGC_MENU();
		BOOL IS_REPORTUGC_MENU_OPEN();
		BOOL IS_FLOATING_HELP_TEXT_ON_SCREEN(int hudIndex);
		void SET_FLOATING_HELP_TEXT_SCREEN_POSITION(int hudIndex, float x, float y);
		void SET_FLOATING_HELP_TEXT_WORLD_POSITION(int hudIndex, float x, float y, float z);
		void SET_FLOATING_HELP_TEXT_TO_ENTITY(int hudIndex, Entity entity, float offsetX, float offsetY);
		void SET_FLOATING_HELP_TEXT_STYLE(int hudIndex, int p1, int p2, int p3, int p4, int p5);
		void CLEAR_FLOATING_HELP(int hudIndex, BOOL p1);
		/**
		 * clanFlag: takes a number 0-5
		 */
		void CREATE_MP_GAMER_TAG_WITH_CREW_COLOR(Player player, const char* username, BOOL pointedClanTag, BOOL isRockstarClan, const char* clanTag, int clanFlag, int r, int g, int b);
		BOOL IS_MP_GAMER_TAG_MOVIE_ACTIVE();
		/**
		 * clanFlag: takes a number 0-5
		 */
		int CREATE_FAKE_MP_GAMER_TAG(Ped ped, const char* username, BOOL pointedClanTag, BOOL isRockstarClan, const char* clanTag, int clanFlag);
		void REMOVE_MP_GAMER_TAG(int gamerTagId);
		BOOL IS_MP_GAMER_TAG_ACTIVE(int gamerTagId);
		BOOL IS_MP_GAMER_TAG_FREE(int gamerTagId);
		/**
		 * enum eMpGamerTagComponent
		 * {
		 * MP_TAG_GAMER_NAME,
		 * MP_TAG_CREW_TAG,
		 * MP_TAG_HEALTH_ARMOUR,
		 * MP_TAG_BIG_TEXT,
		 * MP_TAG_AUDIO_ICON,
		 * MP_TAG_USING_MENU,
		 * MP_TAG_PASSIVE_MODE,
		 * MP_TAG_WANTED_STARS,
		 * MP_TAG_DRIVER,
		 * MP_TAG_CO_DRIVER,
		 * MP_TAG_TAGGED,
		 * MP_TAG_GAMER_NAME_NEARBY,
		 * MP_TAG_ARROW,
		 * MP_TAG_PACKAGES,
		 * MP_TAG_INV_IF_PED_FOLLOWING,
		 * MP_TAG_RANK_TEXT,
		 * MP_TAG_TYPING,
		 * MP_TAG_BAG_LARGE,
		 * MP_TAG_ARROW,
		 * MP_TAG_GANG_CEO,
		 * MP_TAG_GANG_BIKER,
		 * MP_TAG_BIKER_ARROW,
		 * MP_TAG_MC_ROLE_PRESIDENT,
		 * MP_TAG_MC_ROLE_VICE_PRESIDENT,
		 * MP_TAG_MC_ROLE_ROAD_CAPTAIN,
		 * MP_TAG_MC_ROLE_SARGEANT,
		 * MP_TAG_MC_ROLE_ENFORCER,
		 * MP_TAG_MC_ROLE_PROSPECT,
		 * MP_TAG_TRANSMITTER,
		 * MP_TAG_BOMB
		 * };
		 */
		void SET_MP_GAMER_TAG_VISIBILITY(int gamerTagId, int component, BOOL toggle, Any p3);
		void SET_ALL_MP_GAMER_TAGS_VISIBILITY(int gamerTagId, BOOL toggle);
		/**
		 * Displays a bunch of icons above the players name, and level, and their name twice
		 */
		void SET_MP_GAMER_TAGS_SHOULD_USE_VEHICLE_HEALTH(int gamerTagId, BOOL toggle);
		void SET_MP_GAMER_TAGS_SHOULD_USE_POINTS_HEALTH(int gamerTagId, BOOL toggle);
		void SET_MP_GAMER_TAGS_POINT_HEALTH(int gamerTagId, int value, int maximumValue);
		/**
		 * Sets a gamer tag's component colour
		 * 
		 * gamerTagId is obtained using for example CREATE_FAKE_MP_GAMER_TAG
		 * Ranges from 0 to 255. 0 is grey health bar, ~50 yellow, 200 purple.
		 */
		void SET_MP_GAMER_TAG_COLOUR(int gamerTagId, int component, int hudColorIndex);
		/**
		 * Ranges from 0 to 255. 0 is grey health bar, ~50 yellow, 200 purple.
		 * Should be enabled as flag (2). Has 0 opacity by default.
		 * 
		 * - This was _SET_MP_GAMER_TAG_HEALTH_BAR_COLOR,
		 * -> Rockstar use the EU spelling of 'color' so I hashed the same name with COLOUR and it came back as the correct hash, so it has been corrected above.
		 */
		void SET_MP_GAMER_TAG_HEALTH_BAR_COLOUR(int gamerTagId, int hudColorIndex);
		/**
		 * Sets flag's sprite transparency. 0-255.
		 */
		void SET_MP_GAMER_TAG_ALPHA(int gamerTagId, int component, int alpha);
		/**
		 * displays wanted star above head
		 */
		void SET_MP_GAMER_TAG_WANTED_LEVEL(int gamerTagId, int wantedlvl);
		void SET_MP_GAMER_TAG_NUM_PACKAGES(int gamerTagId, int p1);
		void SET_MP_GAMER_TAG_NAME(int gamerTagId, const char* string);
		BOOL IS_UPDATING_MP_GAMER_TAG_NAME_AND_CREW_DETAILS(int gamerTagId);
		void SET_MP_GAMER_TAG_BIG_TEXT(int gamerTagId, const char* string);
		int GET_CURRENT_WEBPAGE_ID();
		int GET_CURRENT_WEBSITE_ID();
		/**
		 * Returns the ActionScript flagValue.
		 * ActionScript flags are global flags that scaleforms use
		 * Flags found during testing
		 * 0: Returns 1 if the web_browser keyboard is open, otherwise 0
		 * 1: Returns 1 if the player has clicked back twice on the opening page, otherwise 0 (web_browser)
		 * 2: Returns how many links the player has clicked in the web_browser scaleform, returns 0 when the browser gets closed
		 * 9: Returns the current selection on the mobile phone scaleform
		 * 
		 * There are 20 flags in total.
		 */
		int GET_GLOBAL_ACTIONSCRIPT_FLAG(int flagIndex);
		void RESET_GLOBAL_ACTIONSCRIPT_FLAG(int flagIndex);
		BOOL IS_WARNING_MESSAGE_READY_FOR_CONTROL();
		/**
		 * You can only use text entries. No custom text.
		 * 
		 * Example: SET_WARNING_MESSAGE("t20", 3, "adder", false, -1, 0, 0, true);
		 * errorCode: shows an error code at the bottom left if nonzero
		 */
		void SET_WARNING_MESSAGE(const char* titleMsg, int flags, const char* promptMsg, BOOL p3, int p4, const char* p5, const char* p6, BOOL showBackground, int errorCode);
		/**
		 * Shows a warning message on screen with a header.
		 * Note: You can only use text entries. No custom text. You can recreate this easily with scaleforms.
		 * Example: https://i.imgur.com/ITJt8bJ.png
		 */
		void SET_WARNING_MESSAGE_WITH_HEADER(const char* entryHeader, const char* entryLine1, int instructionalKey, const char* entryLine2, BOOL p4, Any p5, Any* showBackground, Any* p7, BOOL p8, Any p9);
		/**
		 * You can use this native for custom input, without having to use any scaleform-related natives.
		 * The native must be called on tick.
		 * The entryHeader must be a valid label.
		 * For Single lines use JL_INVITE_N as entryLine1, JL_INVITE_ND for multiple.
		 * Notes:
		 * - additionalIntInfo: replaces first occurrence of ~1~ in provided label with an integer
		 * - additionalTextInfoLine1: replaces first occurrence of ~a~ in provided label, with your custom text
		 * - additionalTextInfoLine2: replaces second occurrence of ~a~ in provided label, with your custom text
		 * - showBackground: shows black background of the warning screen
		 * - errorCode: shows an error code at the bottom left if nonzero
		 * Example of usage:
		 * SET_WARNING_MESSAGE_WITH_HEADER_AND_SUBSTRING_FLAGS("ALERT", "JL_INVITE_ND", 66, "", true, -1, -1, "Testing line 1", "Testing line 2", true, 0);
		 * Screenshot:
		 * https://imgur.com/a/IYA7vJ8
		 */
		void SET_WARNING_MESSAGE_WITH_HEADER_AND_SUBSTRING_FLAGS(const char* entryHeader, const char* entryLine1, int instructionalKey, const char* entryLine2, BOOL p4, Any p5, Any additionalIntInfo, const char* additionalTextInfoLine1, const char* additionalTextInfoLine2, BOOL showBackground, int errorCode);
		void SET_WARNING_MESSAGE_WITH_HEADER_EXTENDED(const char* entryHeader, const char* entryLine1, int flags, const char* entryLine2, BOOL p4, Any p5, Any* p6, Any* p7, BOOL showBg, Any p9, Any p10);
		/**
		 * labelTitle: Label of the alert's title.
		 * labelMsg: Label of the alert's message.
		 * p2: This is an enum, check the description for a list.
		 * p3: This is an enum, check the description for a list.
		 * labelMsg2: Label of another message line
		 * p5: usually 0
		 * p6: usually -1
		 * p7: usually 0
		 * p8: unknown label
		 * p9: unknown label
		 * background: Set to anything other than 0 or false (even any string) and it will draw a background. Setting it to 0 or false will draw no background.
		 * errorCode: Error code, shown at the bottom left if set to value other than 0.
		 * 
		 * instructionalKey enum list:
		 * Buttons = {
		 * Empty = 0,
		 * Select = 1, -- (RETURN)
		 * Ok = 2, -- (RETURN)
		 * Yes = 4, -- (RETURN)
		 * Back = 8, -- (ESC)
		 * Cancel = 16, -- (ESC)
		 * No = 32, -- (ESC)
		 * RetrySpace = 64, -- (SPACE)
		 * Restart = 128, -- (SPACE)
		 * Skip = 256, -- (SPACE)
		 * Quit = 512, -- (ESC)
		 * Adjust = 1024, -- (ARROWS)
		 * SpaceKey = 2048, -- (SPACE)
		 * Share = 4096, -- (SPACE)
		 * SignIn = 8192, -- (SPACE)
		 * Continue = 16384, -- (RETURN)
		 * AdjustLeftRight = 32768, -- (SCROLL L/R)
		 * AdjustUpDown = 65536, -- (SCROLL U/D)
		 * Overwrite = 131072, -- (SPACE)
		 * SocialClubSignup = 262144, -- (RETURN)
		 * Confirm = 524288, -- (RETURN)
		 * Queue = 1048576, -- (RETURN)
		 * RetryReturn = 2097152, -- (RETURN)
		 * BackEsc = 4194304, -- (ESC)
		 * SocialClub = 8388608, -- (RETURN)
		 * Spectate = 16777216, -- (SPACE)
		 * OkEsc = 33554432, -- (ESC)
		 * CancelTransfer = 67108864, -- (ESC)
		 * LoadingSpinner = 134217728,
		 * NoReturnToGTA = 268435456, -- (ESC)
		 * CancelEsc = 536870912, -- (ESC)
		 * }
		 * 
		 * Alt = {
		 * Empty = 0,
		 * No = 1, -- (SPACE)
		 * Host = 2, -- (ESC)
		 * SearchForJob = 4, -- (RETURN)
		 * ReturnKey = 8, -- (TURN)
		 * Freemode = 16, -- (ESC)
		 * }
		 * 
		 * Example: https://i.imgur.com/TvmNF4k.png
		 */
		void SET_WARNING_MESSAGE_WITH_HEADER_AND_SUBSTRING_FLAGS_EXTENDED(const char* labelTitle, const char* labelMessage, int p2, int p3, const char* labelMessage2, BOOL p5, int p6, int p7, const char* p8, const char* p9, BOOL background, int errorCode);
		/**
		 * Has to do with the confirmation overlay (E.g. confirm exit)
		 */
		Hash GET_WARNING_SCREEN_MESSAGE_HASH();
		/**
		 * Some sort of list displayed in a warning message. Yet unknown how to prevent repeating.
		 * Param names copied from the corresponding scaleform function "SET_LIST_ROW".
		 * Example: https://i.imgur.com/arKvOYx.png
		 */
		BOOL SET_WARNING_MESSAGE_OPTION_ITEMS(int index, const char* name, int cash, int rp, int lvl, int colour);
		BOOL SET_WARNING_MESSAGE_OPTION_HIGHLIGHT(Any p0);
		void REMOVE_WARNING_MESSAGE_OPTION_ITEMS();
		BOOL IS_WARNING_MESSAGE_ACTIVE();
		void CLEAR_DYNAMIC_PAUSE_MENU_ERROR_MESSAGE();
		/**
		 * If toggle is true, the map is shown in full screen
		 * If toggle is false, the map is shown in normal mode
		 */
		void CUSTOM_MINIMAP_SET_ACTIVE(BOOL toggle);
		/**
		 * Sets the sprite of the next BLIP_GALLERY blip, values used in the native scripts: 143 (ObjectiveBlue), 144 (ObjectiveGreen), 145 (ObjectiveRed), 146 (ObjectiveYellow).
		 */
		void CUSTOM_MINIMAP_SET_BLIP_OBJECT(int spriteId);
		/**
		 * Add a BLIP_GALLERY at the specific coordinate. Used in fm_maintain_transition_players to display race track points.
		 */
		int CUSTOM_MINIMAP_CREATE_BLIP(float x, float y, float z);
		void CUSTOM_MINIMAP_CLEAR_BLIPS();
		/**
		 * Doesn't actually return anything.
		 */
		BOOL FORCE_SONAR_BLIPS_THIS_FRAME();
		Blip GET_NORTH_BLID_INDEX();
		/**
		 * Toggles whether or not name labels are shown on the expanded minimap next to player blips, like in GTA:O.
		 * Doesn't need to be called every frame.
		 * Preview: https://i.imgur.com/DfqKWfJ.png
		 * 
		 * Make sure to call SET_BLIP_CATEGORY with index 7 for this to work on the desired blip.
		 */
		void DISPLAY_PLAYER_NAME_TAGS_ON_BLIPS(BOOL toggle);
		/**
		 * This native does absolutely nothing, just a nullsub
		 */
		void DRAW_FRONTEND_BACKGROUND_THIS_FRAME();
		void DRAW_HUD_OVER_FADE_THIS_FRAME();
		/**
		 * Does stuff like this:
		 * gyazo.com/7fcb78ea3520e3dbc5b2c0c0f3712617
		 * 
		 * Example:
		 * int GetHash = GET_HASH_KEY("fe_menu_version_corona_lobby");
		 * ACTIVATE_FRONTEND_MENU(GetHash, 0, -1);
		 * 
		 * BOOL p1 is a toggle to define the game in pause.
		 * int p2 is unknown but -1 always works, not sure why though.
		 * 
		 * [30/03/2017] ins1de :
		 * 
		 * the int p2 is actually a component variable. When the pause menu is visible, it opens the tab related to it.
		 * 
		 * Example : Function.Call(Hash.ACTIVATE_FRONTEND_MENU,-1171018317, 0, 42);
		 * Result : Opens the "Online" tab without pausing the menu, with -1 it opens the map.Below is a list of all known Frontend Menu Hashes.
		 * - FE_MENU_VERSION_SP_PAUSE
		 * - FE_MENU_VERSION_MP_PAUSE
		 * - FE_MENU_VERSION_CREATOR_PAUSE
		 * - FE_MENU_VERSION_CUTSCENE_PAUSE
		 * - FE_MENU_VERSION_SAVEGAME
		 * - FE_MENU_VERSION_PRE_LOBBY
		 * - FE_MENU_VERSION_LOBBY
		 * - FE_MENU_VERSION_MP_CHARACTER_SELECT
		 * - FE_MENU_VERSION_MP_CHARACTER_CREATION
		 * - FE_MENU_VERSION_EMPTY
		 * - FE_MENU_VERSION_EMPTY_NO_BACKGROUND
		 * - FE_MENU_VERSION_TEXT_SELECTION
		 * - FE_MENU_VERSION_CORONA
		 * - FE_MENU_VERSION_CORONA_LOBBY
		 * - FE_MENU_VERSION_CORONA_JOINED_PLAYERS
		 * - FE_MENU_VERSION_CORONA_INVITE_PLAYERS
		 * - FE_MENU_VERSION_CORONA_INVITE_FRIENDS
		 * - FE_MENU_VERSION_CORONA_INVITE_CREWS
		 * - FE_MENU_VERSION_CORONA_INVITE_MATCHED_PLAYERS
		 * - FE_MENU_VERSION_CORONA_INVITE_LAST_JOB_PLAYERS
		 * - FE_MENU_VERSION_CORONA_RACE
		 * - FE_MENU_VERSION_CORONA_BETTING
		 * - FE_MENU_VERSION_JOINING_SCREEN
		 * - FE_MENU_VERSION_LANDING_MENU
		 * - FE_MENU_VERSION_LANDING_KEYMAPPING_MENU
		 */
		void ACTIVATE_FRONTEND_MENU(Hash menuhash, BOOL togglePause, int component);
		/**
		 * Before using this native click the native above and look at the decription.
		 * 
		 * Example:
		 * int GetHash = Function.Call<int>(Hash.GET_HASH_KEY, "fe_menu_version_corona_lobby");
		 * Function.Call(Hash.ACTIVATE_FRONTEND_MENU, GetHash, 0, -1);
		 * Function.Call(Hash.RESTART_FRONTEND_MENU(GetHash, -1);
		 * 
		 * This native refreshes the frontend menu.
		 * 
		 * p1 = Hash of Menu
		 * p2 = Unknown but always works with -1.
		 */
		void RESTART_FRONTEND_MENU(Hash menuHash, int p1);
		/**
		 * if (HUD::GET_CURRENT_FRONTEND_MENU_VERSION() == joaat("fe_menu_version_empty_no_background"))
		 */
		Hash GET_CURRENT_FRONTEND_MENU_VERSION();
		void SET_PAUSE_MENU_ACTIVE(BOOL toggle);
		void DISABLE_FRONTEND_THIS_FRAME();
		void SUPPRESS_FRONTEND_RENDERING_THIS_FRAME();
		/**
		 * Allows opening the pause menu this frame, when the player is dead.
		 */
		void ALLOW_PAUSE_WHEN_NOT_IN_STATE_OF_PLAY_THIS_FRAME();
		void SET_FRONTEND_ACTIVE(BOOL active);
		BOOL IS_PAUSE_MENU_ACTIVE();
		BOOL IS_STORE_PENDING_NETWORK_SHUTDOWN_TO_OPEN();
		/**
		 * Returns:
		 * 
		 * 0
		 * 5
		 * 10
		 * 15
		 * 20
		 * 25
		 * 30
		 * 35
		 * 
		 */
		int GET_PAUSE_MENU_STATE();
		Vector3 GET_PAUSE_MENU_POSITION();
		BOOL IS_PAUSE_MENU_RESTARTING();
		/**
		 * Not present in retail version of the game, actual definiton seems to be
		 * _LOG_DEBUG_INFO(const char* category, const char* debugText);
		 */
		void FORCE_SCRIPTED_GFX_WHEN_FRONTEND_ACTIVE(const char* p0);
		void PAUSE_MENUCEPTION_GO_DEEPER(int page);
		void PAUSE_MENUCEPTION_THE_KICK();
		void PAUSE_TOGGLE_FULLSCREEN_MAP(Any p0);
		/**
		 * Activates the specified frontend menu context.
		 * pausemenu.xml defines some specific menu options using 'context'. Context is basically a 'condition'.
		 * The `*ALL*` part of the context means that whatever is being defined, will be active when any or all of those conditions after `*ALL*` are met.
		 * The `*NONE*` part of the context section means that whatever is being defined, will NOT be active if any or all of the conditions after `*NONE*` are met.
		 * This basically allows you to hide certain menu sections, or things like instructional buttons.
		 */
		void PAUSE_MENU_ACTIVATE_CONTEXT(Hash contextHash);
		void PAUSE_MENU_DEACTIVATE_CONTEXT(Hash contextHash);
		BOOL PAUSE_MENU_IS_CONTEXT_ACTIVE(Hash contextHash);
		BOOL PAUSE_MENU_IS_CONTEXT_MENU_ACTIVE();
		int PAUSE_MENU_GET_HAIR_COLOUR_INDEX();
		int PAUSE_MENU_GET_MOUSE_HOVER_INDEX();
		int PAUSE_MENU_GET_MOUSE_HOVER_UNIQUE_ID();
		BOOL PAUSE_MENU_GET_MOUSE_CLICK_EVENT(Any* p0, Any* p1, Any* p2);
		void PAUSE_MENU_REDRAW_INSTRUCTIONAL_BUTTONS(int p0);
		void PAUSE_MENU_SET_BUSY_SPINNER(BOOL p0, int position, int spinnerIndex);
		void PAUSE_MENU_SET_WARN_ON_TAB_CHANGE(BOOL p0);
		void PAUSE_MENU_SET_CLOUD_BUSY_SPINNER(const char* TextLabel, BOOL bNoMenu, BOOL bBlackBackground); // Missing in crossmap
		void PAUSE_MENU_CLEAR_CLOUD_BUSY_SPINNER(); // Missing in crossmap
		BOOL IS_FRONTEND_READY_FOR_CONTROL();
		/**
		 * Disables frontend (works in custom frontends, not sure about regular pause menu) navigation keys on keyboard. Not sure about controller. Does not disable mouse controls. No need to call this every tick.
		 * 
		 * To enable the keys again, use `0x14621BB1DF14E2B2`.
		 */
		void TAKE_CONTROL_OF_FRONTEND();
		/**
		 * Enables frontend (works in custom frontends, not sure about regular pause menu) navigation keys on keyboard if they were disabled using the native below.
		 * To disable the keys, use `0xEC9264727EEC0F28`
		 */
		void RELEASE_CONTROL_OF_FRONTEND();
		BOOL CODE_WANTS_SCRIPT_TO_TAKE_CONTROL();
		int GET_SCREEN_CODE_WANTS_SCRIPT_TO_CONTROL();
		BOOL IS_NAVIGATING_MENU_CONTENT();
		BOOL HAS_MENU_TRIGGER_EVENT_OCCURRED();
		BOOL HAS_MENU_LAYOUT_CHANGED_EVENT_OCCURRED();
		void SET_SAVEGAME_LIST_UNIQUE_ID(Any p0);
		void GET_MENU_TRIGGER_EVENT_DETAILS(int* lastItemMenuId, int* selectedItemUniqueId);
		/**
		 * lastItemMenuId: this is the menuID of the last selected item minus 1000 (lastItem.menuID - 1000)
		 * selectedItemMenuId: same as lastItemMenuId except for the currently selected menu item
		 * selectedItemUniqueId: this is uniqueID of the currently selected menu item
		 * 
		 * when the pausemenu is closed:
		 * lastItemMenuId = -1
		 * selectedItemMenuId = -1
		 * selectedItemUniqueId = 0
		 * 
		 * when the header gains focus:
		 * lastItemMenuId updates as normal or 0 if the pausemenu was just opened
		 * selectedItemMenuId becomes a unique id for the pausemenu page that focus was taken from (?) or 0 if the pausemenu was just opened
		 * selectedItemUniqueId = -1
		 * 
		 * when focus is moved from the header to a pausemenu page:
		 * lastItemMenuId becomes a unique id for the pausemenu page that focus was moved to (?)
		 * selectedItemMenuId = -1
		 * selectedItemUniqueId updates as normal
		 */
		void GET_MENU_LAYOUT_CHANGED_EVENT_DETAILS(int* lastItemMenuId, int* selectedItemMenuId, int* selectedItemUniqueId);
		BOOL GET_PM_PLAYER_CREW_COLOR(int* r, int* g, int* b);
		BOOL GET_MENU_PED_INT_STAT(Any p0, Any* p1);
		BOOL GET_CHARACTER_MENU_PED_INT_STAT(Any p0, Any* p1, Any p2);
		BOOL GET_MENU_PED_MASKED_INT_STAT(Hash statHash, int* outValue, int mask, BOOL p3);
		BOOL GET_CHARACTER_MENU_PED_MASKED_INT_STAT(Hash statHash, Any* outValue, int p2, int mask, BOOL p4);
		BOOL GET_MENU_PED_FLOAT_STAT(Hash statHash, float* outValue);
		BOOL GET_CHARACTER_MENU_PED_FLOAT_STAT(float statHash, float* outValue, BOOL p2);
		/**
		 * p0 was always 0xAE2602A3.
		 */
		BOOL GET_MENU_PED_BOOL_STAT(Hash statHash, BOOL* outValue);
		void CLEAR_PED_IN_PAUSE_MENU();
		/**
		 * p1 is either 1 or 2 in the PC scripts.
		 */
		void GIVE_PED_TO_PAUSE_MENU(Ped ped, int p1);
		/**
		 * Toggles the light state for the pause menu ped in frontend menus.
		 * 
		 * This is used by R* in combination with `SET_PAUSE_MENU_PED_SLEEP_STATE` to toggle the "offline" or "online" state in the "friends" tab of the pause menu in GTA Online.
		 * 
		 * 
		 * Example:
		 * Lights On: https://vespura.com/hi/i/2019-04-01_16-09_540ee_1015.png
		 * Lights Off: https://vespura.com/hi/i/2019-04-01_16-10_8b5e7_1016.png
		 */
		void SET_PAUSE_MENU_PED_LIGHTING(BOOL state);
		/**
		 * Toggles the pause menu ped sleep state for frontend menus.
		 * 
		 * Example: https://vespura.com/hi/i/2019-04-01_15-51_8ed38_1014.gif
		 * 
		 * `state` 0 will make the ped slowly fall asleep, 1 will slowly wake the ped up.
		 */
		void SET_PAUSE_MENU_PED_SLEEP_STATE(BOOL state);
		void OPEN_ONLINE_POLICIES_MENU();
		BOOL ARE_ONLINE_POLICIES_UP_TO_DATE();
		/**
		 * Returns the same as IS_SOCIAL_CLUB_ACTIVE
		 */
		BOOL IS_ONLINE_POLICIES_MENU_ACTIVE();
		/**
		 * Uses the `SOCIAL_CLUB2` scaleform.
		 * menu: GALLERY, MISSIONS, CREWS, MIGRATE, PLAYLISTS, JOBS
		 */
		void OPEN_SOCIAL_CLUB_MENU(Hash menu);
		void CLOSE_SOCIAL_CLUB_MENU();
		/**
		 * HUD::SET_SOCIAL_CLUB_TOUR("Gallery");
		 * HUD::SET_SOCIAL_CLUB_TOUR("Missions");
		 * HUD::SET_SOCIAL_CLUB_TOUR("General");
		 * HUD::SET_SOCIAL_CLUB_TOUR("Playlists");
		 */
		void SET_SOCIAL_CLUB_TOUR(const char* name);
		BOOL IS_SOCIAL_CLUB_ACTIVE();
		void SET_TEXT_INPUT_BOX_ENABLED(BOOL p0);
		void FORCE_CLOSE_TEXT_INPUT_BOX();
		void SET_ALLOW_COMMA_ON_TEXT_INPUT(Any p0);
		void OVERRIDE_MP_TEXT_CHAT_TEAM_STRING(Hash gxtEntryHash);
		/**
		 * Returns whether or not the text chat (MULTIPLAYER_CHAT Scaleform component) is active.
		 */
		BOOL IS_MP_TEXT_CHAT_TYPING();
		void CLOSE_MP_TEXT_CHAT();
		void MP_TEXT_CHAT_IS_TEAM_JOB(Any p0);
		void OVERRIDE_MP_TEXT_CHAT_COLOR(int p0, int hudColor);
		/**
		 * Hides the chat history, closes the input box and makes it unable to be opened unless called again with FALSE.
		 */
		void MP_TEXT_CHAT_DISABLE(BOOL toggle);
		void FLAG_PLAYER_CONTEXT_IN_TOURNAMENT(BOOL toggle);
		/**
		 * This native turns on the AI blip on the specified ped. It also disappears automatically when the ped is too far or if the ped is dead. You don't need to control it with other natives.
		 * 
		 * See gtaforums.com/topic/884370-native-research-ai-blips for further information.
		 */
		void SET_PED_HAS_AI_BLIP(Ped ped, BOOL hasCone);
		/**
		 * color: see SET_BLIP_COLOUR
		 */
		void SET_PED_HAS_AI_BLIP_WITH_COLOUR(Ped ped, BOOL hasCone, int color);
		BOOL DOES_PED_HAVE_AI_BLIP(Ped ped);
		void SET_PED_AI_BLIP_GANG_ID(Ped ped, int gangId);
		void SET_PED_AI_BLIP_HAS_CONE(Ped ped, BOOL toggle);
		void SET_PED_AI_BLIP_FORCED_ON(Ped ped, BOOL toggle);
		void SET_PED_AI_BLIP_NOTICE_RANGE(Ped ped, float range);
		void SET_PED_AI_BLIP_SPRITE(Ped ped, int spriteId);
		Blip GET_AI_PED_PED_BLIP_INDEX(Ped ped);
		/**
		 * Returns the current AI BLIP for the specified ped
		 */
		Blip GET_AI_PED_VEHICLE_BLIP_INDEX(Ped ped);
		BOOL HAS_DIRECTOR_MODE_BEEN_LAUNCHED_BY_CODE();
		void SET_DIRECTOR_MODE_LAUNCHED_BY_SCRIPT();
		/**
		 * If toggle is true, hides special ability bar / character name in the pause menu
		 * If toggle is false, shows special ability bar / character name in the pause menu
		 */
		void SET_PLAYER_IS_IN_DIRECTOR_MODE(BOOL toggle);
		void SET_DIRECTOR_MODE_AVAILABLE(BOOL toggle);
		void HIDE_HUDMARKERS_THIS_FRAME();

	} // namespace HUD

	namespace IIS {

		/**
		 * Launches the Initial Interactive Screen
		 */
		void SET_SHOULD_LAUNCH_IIS(); // Missing in crossmap

	} // namespace IIS

	namespace INTERIOR {

		float GET_INTERIOR_HEADING(Interior interior);
		void GET_INTERIOR_LOCATION_AND_NAMEHASH(Interior interior, Vector3* position, Hash* nameHash);
		/**
		 * Returns the group ID of the specified interior.
		 * 0 = default
		 * 1 = subway station, subway tracks, sewers
		 * 3 = train tunnel under mirror park
		 * 5 = tunnel near del perro
		 * 6 = train tunnel near chilliad
		 * 7 = train tunnel near josiah
		 * 8 = train tunnel in sandy shores
		 * 9 = braddock tunnel (near chilliad)
		 * 12 = tunnel under fort zancudo
		 * 14 = train tunnel under cypress flats
		 * 18 = rockford plaza parking garage
		 * 19 = arcadius parking garage
		 * 20 = union depository parking garage
		 * 21 = fib parking garage
		 */
		int GET_INTERIOR_GROUP_ID(Interior interior);
		Vector3 GET_OFFSET_FROM_INTERIOR_IN_WORLD_COORDS(Interior interior, float x, float y, float z);
		BOOL IS_INTERIOR_SCENE();
		BOOL IS_VALID_INTERIOR(Interior interior);
		void CLEAR_ROOM_FOR_ENTITY(Entity entity);
		void FORCE_ROOM_FOR_ENTITY(Entity entity, Interior interior, Hash roomHashKey);
		/**
		 * Gets the room hash key from the room that the specified entity is in. Each room in every interior has a unique key. Returns 0 if the entity is outside.
		 */
		Hash GET_ROOM_KEY_FROM_ENTITY(Entity entity);
		/**
		 * Seems to do the exact same as INTERIOR::GET_ROOM_KEY_FROM_ENTITY
		 */
		Hash GET_KEY_FOR_ENTITY_IN_ROOM(Entity entity);
		/**
		 * Returns the handle of the interior that the entity is in. Returns 0 if outside.
		 */
		Interior GET_INTERIOR_FROM_ENTITY(Entity entity);
		void RETAIN_ENTITY_IN_INTERIOR(Entity entity, Interior interior);
		/**
		 * Immediately removes entity from an interior. Like sets entity to `limbo` room.
		 */
		void CLEAR_INTERIOR_STATE_OF_ENTITY(Entity entity);
		void FORCE_ACTIVATING_TRACKING_ON_ENTITY(Any p0, Any p1);
		void FORCE_ROOM_FOR_GAME_VIEWPORT(int interiorID, Hash roomHashKey);
		/**
		 * Example of use (carmod_shop)
		 * INTERIOR::SET_ROOM_FOR_GAME_VIEWPORT_BY_NAME("V_CarModRoom");
		 */
		void SET_ROOM_FOR_GAME_VIEWPORT_BY_NAME(const char* roomName);
		/**
		 * Usage: INTERIOR::SET_ROOM_FOR_GAME_VIEWPORT_BY_KEY(INTERIOR::GET_KEY_FOR_ENTITY_IN_ROOM(PLAYER::PLAYER_PED_ID()));
		 */
		void SET_ROOM_FOR_GAME_VIEWPORT_BY_KEY(Hash roomHashKey);
		Hash GET_ROOM_KEY_FOR_GAME_VIEWPORT();
		void CLEAR_ROOM_FOR_GAME_VIEWPORT();
		/**
		 * Returns the current interior id from gameplay camera
		 */
		Interior GET_INTERIOR_FROM_PRIMARY_VIEW();
		/**
		 * Returns interior ID from specified coordinates. If coordinates are outside, then it returns 0.
		 * 
		 * Example for VB.NET
		 * Dim interiorID As Integer = Native.Function.Call(Of Integer)(Hash.GET_INTERIOR_AT_COORDS, X, Y, Z)
		 */
		Interior GET_INTERIOR_AT_COORDS(float x, float y, float z);
		void ADD_PICKUP_TO_INTERIOR_ROOM_BY_NAME(Pickup pickup, const char* roomName);
		void PIN_INTERIOR_IN_MEMORY(Interior interior);
		/**
		 * Does something similar to INTERIOR::DISABLE_INTERIOR.
		 * 
		 * You don't fall through the floor but everything is invisible inside and looks the same as when INTERIOR::DISABLE_INTERIOR is used. Peds behaves normally inside.
		 */
		void UNPIN_INTERIOR(Interior interior);
		BOOL IS_INTERIOR_READY(Interior interior);
		/**
		 * Only used once in the entire game scripts.
		 * Does not actually return anything.
		 */
		BOOL SET_INTERIOR_IN_USE(Interior interior);
		/**
		 * Returns the interior ID representing the requested interior at that location (if found?). The supplied interior string is not the same as the one used to load the interior.
		 * 
		 * Use: INTERIOR::UNPIN_INTERIOR(INTERIOR::GET_INTERIOR_AT_COORDS_WITH_TYPE(x, y, z, interior))
		 * 
		 * Interior types include: "V_Michael", "V_Franklins", "V_Franklinshouse", etc.. you can find them in the scripts.
		 * 
		 * Not a very useful native as you could just use GET_INTERIOR_AT_COORDS instead and get the same result, without even having to specify the interior type.
		 */
		Interior GET_INTERIOR_AT_COORDS_WITH_TYPE(float x, float y, float z, const char* interiorType);
		/**
		 * Hashed version of GET_INTERIOR_AT_COORDS_WITH_TYPE
		 */
		Interior GET_INTERIOR_AT_COORDS_WITH_TYPEHASH(float x, float y, float z, Hash typeHash);
		void ACTIVATE_INTERIOR_GROUPS_USING_CAMERA();
		/**
		 * Returns true if the collision at the specified coords is marked as being outside (false if there's an interior)
		 */
		BOOL IS_COLLISION_MARKED_OUTSIDE(float x, float y, float z);
		Interior GET_INTERIOR_FROM_COLLISION(float x, float y, float z);
		void ENABLE_STADIUM_PROBES_THIS_FRAME(BOOL toggle);
		/**
		 * More info: http://gtaforums.com/topic/836367-adding-props-to-interiors/
		 * 
		 * Full list of IPLs and interior entity sets by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/ipls.json
		 */
		void ACTIVATE_INTERIOR_ENTITY_SET(Interior interior, const char* entitySetName);
		/**
		 * Full list of IPLs and interior entity sets by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/ipls.json
		 */
		void DEACTIVATE_INTERIOR_ENTITY_SET(Interior interior, const char* entitySetName);
		/**
		 * Full list of IPLs and interior entity sets by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/ipls.json
		 */
		BOOL IS_INTERIOR_ENTITY_SET_ACTIVE(Interior interior, const char* entitySetName);
		/**
		 * Full list of IPLs and interior entity sets by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/ipls.json
		 */
		void SET_INTERIOR_ENTITY_SET_TINT_INDEX(Interior interior, const char* entitySetName, int color);
		void REFRESH_INTERIOR(Interior interior);
		/**
		 * This is the native that is used to hide the exterior of GTA Online apartment buildings when you are inside an apartment.
		 * 
		 * More info: http://gtaforums.com/topic/836301-hiding-gta-online-apartment-exteriors/
		 */
		void ENABLE_EXTERIOR_CULL_MODEL_THIS_FRAME(Hash mapObjectHash);
		void ENABLE_SHADOW_CULL_MODEL_THIS_FRAME(Hash mapObjectHash);
		/**
		 * Example:
		 * This removes the interior from the strip club and when trying to walk inside the player just falls:
		 * 
		 * INTERIOR::DISABLE_INTERIOR(118018, true);
		 */
		void DISABLE_INTERIOR(Interior interior, BOOL toggle);
		BOOL IS_INTERIOR_DISABLED(Interior interior);
		/**
		 * Does something similar to INTERIOR::DISABLE_INTERIOR
		 */
		void CAP_INTERIOR(Interior interior, BOOL toggle);
		BOOL IS_INTERIOR_CAPPED(Interior interior);
		void DISABLE_METRO_SYSTEM(BOOL toggle);
		/**
		 * Jenkins hash _might_ be 0xFC227584.
		 */
		void SET_IS_EXTERIOR_ONLY(Entity entity, BOOL toggle);

	} // namespace INTERIOR

	namespace ITEMSET {

		ScrHandle CREATE_ITEMSET(BOOL p0);
		void DESTROY_ITEMSET(ScrHandle itemset);
		BOOL IS_ITEMSET_VALID(ScrHandle itemset);
		BOOL ADD_TO_ITEMSET(ScrHandle item, ScrHandle itemset);
		void REMOVE_FROM_ITEMSET(ScrHandle item, ScrHandle itemset);
		int GET_ITEMSET_SIZE(ScrHandle itemset);
		ScrHandle GET_INDEXED_ITEM_IN_ITEMSET(int index, ScrHandle itemset);
		BOOL IS_IN_ITEMSET(ScrHandle item, ScrHandle itemset);
		void CLEAN_ITEMSET(ScrHandle itemset);

	} // namespace ITEMSET

	namespace LANDINGPAGE {

		void IS_LANDING_PAGE_ACTIVE(); // Missing in crossmap
		void SET_SHOULD_LAUNCH_LANDING_PAGE(int entrypointId); // Missing in crossmap
		void SET_SHOULD_DISMISS_LANDING_PAGE(); // Missing in crossmap
		void GET_LANDING_PAGE_SELECTED_CHARACTER_SLOT(); // Missing in crossmap

	} // namespace LANDINGPAGE

	namespace LOBBY {

		/**
		 * This function is hard-coded to always return 0.
		 */
		BOOL LOBBY_AUTO_MULTIPLAYER_MENU();
		BOOL LOBBY_AUTO_MULTIPLAYER_FREEMODE();
		void LOBBY_SET_AUTO_MULTIPLAYER(BOOL toggle);
		BOOL LOBBY_AUTO_MULTIPLAYER_EVENT();
		void LOBBY_SET_AUTO_MULTIPLAYER_EVENT(BOOL toggle);
		BOOL LOBBY_AUTO_MULTIPLAYER_RANDOM_JOB();
		void LOBBY_SET_AUTO_MP_RANDOM_JOB(BOOL toggle);
		void SHUTDOWN_SESSION_CLEARS_AUTO_MULTIPLAYER(BOOL toggle);

	} // namespace LOBBY

	namespace LOCALIZATION {

		/**
		 * Same return values as GET_CURRENT_LANGUAGE
		 */
		int LOCALIZATION_GET_SYSTEM_LANGUAGE();
		/**
		 * 0 = american (en-US)
		 * 1 = french (fr-FR)
		 * 2 = german (de-DE)
		 * 3 = italian (it-IT)
		 * 4 = spanish (es-ES)
		 * 5 = brazilian (pt-BR)
		 * 6 = polish (pl-PL)
		 * 7 = russian (ru-RU)
		 * 8 = korean (ko-KR)
		 * 9 = chinesetrad (zh-TW)
		 * 10 = japanese (ja-JP)
		 * 11 = mexican (es-MX)
		 * 12 = chinesesimp (zh-CN)
		 */
		int GET_CURRENT_LANGUAGE();
		/**
		 * Possible return values: 0, 1, 2
		 */
		int LOCALIZATION_GET_SYSTEM_DATE_TYPE();

	} // namespace LOCALIZATION

	namespace MISC {

		int GET_ALLOCATED_STACK_SIZE();
		int GET_NUMBER_OF_FREE_STACKS_OF_THIS_SIZE(int stackSize);
		void SET_RANDOM_SEED(int seed);
		/**
		 * Maximum value is 1.
		 * At a value of 0 the game will still run at a minimum time scale.
		 * 
		 * Slow Motion 1: 0.6
		 * Slow Motion 2: 0.4
		 * Slow Motion 3: 0.2
		 */
		void SET_TIME_SCALE(float timeScale);
		/**
		 * If true, the player can't save the game.
		 * 
		 * 
		 * If the parameter is true, sets the mission flag to true, if the parameter is false, the function does nothing at all.
		 * 
		 * ^ also, if the mission flag is already set, the function does nothing at all
		 */
		void SET_MISSION_FLAG(BOOL toggle);
		BOOL GET_MISSION_FLAG();
		/**
		 * If the parameter is true, sets the random event flag to true, if the parameter is false, the function does nothing at all.
		 * Does nothing if the mission flag is set.
		 */
		void SET_RANDOM_EVENT_FLAG(BOOL toggle);
		BOOL GET_RANDOM_EVENT_FLAG();
		/**
		 * Returns pointer to an empty string.
		 */
		const char* GET_CONTENT_TO_LOAD();
		/**
		 * Does nothing (it's a nullsub). Seems to be PS4 specific.
		 */
		void ACTIVITY_FEED_CREATE(const char* p0, const char* p1);
		/**
		 * Does nothing (it's a nullsub). Seems to be PS4 specific.
		 */
		void ACTIVITY_FEED_ADD_SUBSTRING_TO_CAPTION(const char* p0);
		/**
		 * Does nothing (it's a nullsub). Seems to be PS4 specific.
		 */
		void ACTIVITY_FEED_ADD_LITERAL_SUBSTRING_TO_CAPTION(const char* p0);
		/**
		 * Does nothing (it's a nullsub). Seems to be PS4 specific.
		 */
		void ACTIVITY_FEED_ADD_INT_TO_CAPTION(Any p0);
		/**
		 * Does nothing (it's a nullsub). Seems to be PS4 specific.
		 */
		void ACTIVITY_FEED_LARGE_IMAGE_URL(const char* p0);
		/**
		 * Does nothing (it's a nullsub). Seems to be PS4 specific.
		 */
		void ACTIVITY_FEED_ACTION_START_WITH_COMMAND_LINE(const char* p0, const char* p1);
		/**
		 * Does nothing (it's a nullsub). Seems to be PS4 specific.
		 */
		void ACTIVITY_FEED_ACTION_START_WITH_COMMAND_LINE_ADD(const char* p0);
		/**
		 * Does nothing (it's a nullsub). Seems to be PS4 specific.
		 */
		void ACTIVITY_FEED_POST();
		/**
		 * Does nothing (it's a nullsub). Seems to be PS4 specific.
		 * 
		 * Used only once in the scripts (ingamehud) with p0 = "AF_GAMEMODE"
		 */
		void ACTIVITY_FEED_ONLINE_PLAYED_WITH_POST(const char* p0);
		/**
		 * Hardcoded to return false.
		 */
		BOOL HAS_RESUMED_FROM_SUSPEND();
		/**
		 * Sets GtaThread+0x14A
		 */
		void SET_SCRIPT_HIGH_PRIO(BOOL toggle);
		/**
		 * Sets bit 3 in GtaThread+0x150
		 */
		void SET_THIS_IS_A_TRIGGER_SCRIPT(BOOL toggle);
		void INFORM_CODE_OF_CONTENT_ID_OF_CURRENT_UGC_MISSION(const char* p0);
		BOOL GET_BASE_ELEMENT_LOCATION_FROM_METADATA_BLOCK(Any* p0, Any* p1, Any p2, BOOL p3);
		/**
		 * Returns current weather name hash
		 */
		Hash GET_PREV_WEATHER_TYPE_HASH_NAME();
		/**
		 * Returns weather name hash
		 */
		Hash GET_NEXT_WEATHER_TYPE_HASH_NAME();
		BOOL IS_PREV_WEATHER_TYPE(const char* weatherType);
		BOOL IS_NEXT_WEATHER_TYPE(const char* weatherType);
		/**
		 * The following weatherTypes are used in the scripts:
		 * "CLEAR"
		 * "EXTRASUNNY"
		 * "CLOUDS"
		 * "OVERCAST"
		 * "RAIN"
		 * "CLEARING"
		 * "THUNDER"
		 * "SMOG"
		 * "FOGGY"
		 * "XMAS"
		 * "SNOW"
		 * "SNOWLIGHT"
		 * "BLIZZARD"
		 * "HALLOWEEN"
		 * "NEUTRAL"
		 */
		void SET_WEATHER_TYPE_PERSIST(const char* weatherType);
		/**
		 * The following weatherTypes are used in the scripts:
		 * "CLEAR"
		 * "EXTRASUNNY"
		 * "CLOUDS"
		 * "OVERCAST"
		 * "RAIN"
		 * "CLEARING"
		 * "THUNDER"
		 * "SMOG"
		 * "FOGGY"
		 * "XMAS"
		 * "SNOW"
		 * "SNOWLIGHT"
		 * "BLIZZARD"
		 * "HALLOWEEN"
		 * "NEUTRAL"
		 */
		void SET_WEATHER_TYPE_NOW_PERSIST(const char* weatherType);
		/**
		 * The following weatherTypes are used in the scripts:
		 * "CLEAR"
		 * "EXTRASUNNY"
		 * "CLOUDS"
		 * "OVERCAST"
		 * "RAIN"
		 * "CLEARING"
		 * "THUNDER"
		 * "SMOG"
		 * "FOGGY"
		 * "XMAS"
		 * "SNOW"
		 * "SNOWLIGHT"
		 * "BLIZZARD"
		 * "HALLOWEEN"
		 * "NEUTRAL"
		 */
		void SET_WEATHER_TYPE_NOW(const char* weatherType);
		void SET_WEATHER_TYPE_OVERTIME_PERSIST(const char* weatherType, float time);
		void SET_RANDOM_WEATHER_TYPE();
		void CLEAR_WEATHER_TYPE_PERSIST();
		void CLEAR_WEATHER_TYPE_NOW_PERSIST_NETWORK(int milliseconds);
		void GET_CURR_WEATHER_STATE(Hash* weatherType1, Hash* weatherType2, float* percentWeather2);
		/**
		 * Mixes two weather types. If percentWeather2 is set to 0.0f, then the weather will be entirely of weatherType1, if it is set to 1.0f it will be entirely of weatherType2. If it's set somewhere in between, there will be a mixture of weather behaviors. To test, try this in the RPH console, and change the float to different values between 0 and 1:
		 * 
		 * execute "NativeFunction.Natives.x578C752848ECFA0C(Game.GetHashKey(""RAIN""), Game.GetHashKey(""SMOG""), 0.50f);
		 * 
		 * Note that unlike most of the other weather natives, this native takes the hash of the weather name, not the plain string. These are the weather names and their hashes:
		 * 
		 * CLEAR  0x36A83D84
		 * EXTRASUNNY  0x97AA0A79
		 * CLOUDS  0x30FDAF5C
		 * OVERCAST    0xBB898D2D
		 * RAIN    0x54A69840
		 * CLEARING    0x6DB1A50D
		 * THUNDER 0xB677829F
		 * SMOG    0x10DCF4B5
		 * FOGGY   0xAE737644
		 * XMAS    0xAAC9C895
		 * SNOWLIGHT   0x23FB812B
		 * BLIZZARD    0x27EA2814
		 * 
		 * 
		 * 
		 * 
		 * 
		 * /* OLD INVALID INFO BELOW * /
		 * Not tested. Based purely on disassembly. Instantly sets the weather to sourceWeather, then transitions to targetWeather over the specified transitionTime in seconds.
		 * 
		 * If an invalid hash is specified for sourceWeather, the current weather type will be used.
		 * If an invalid hash is specified for targetWeather, the next weather type will be used.
		 * If an invalid hash is specified for both sourceWeather and targetWeather, the function just changes the transition time of the current transition.
		 */
		void SET_CURR_WEATHER_STATE(Hash weatherType1, Hash weatherType2, float percentWeather2);
		/**
		 * Appears to have an optional bool parameter that is unused in the scripts.
		 * 
		 * If you pass true, something will be set to zero.
		 */
		void SET_OVERRIDE_WEATHER(const char* weatherType);
		/**
		 * Identical to SET_OVERRIDE_WEATHER but has an additional BOOL param that sets some weather var to 0 if true
		 */
		void SET_OVERRIDE_WEATHEREX(const char* weatherType, BOOL p1);
		void CLEAR_OVERRIDE_WEATHER();
		void WATER_OVERRIDE_SET_SHOREWAVEAMPLITUDE(float amplitude);
		void WATER_OVERRIDE_SET_SHOREWAVEMINAMPLITUDE(float minAmplitude);
		void WATER_OVERRIDE_SET_SHOREWAVEMAXAMPLITUDE(float maxAmplitude);
		void WATER_OVERRIDE_SET_OCEANNOISEMINAMPLITUDE(float minAmplitude);
		void WATER_OVERRIDE_SET_OCEANWAVEAMPLITUDE(float amplitude);
		void WATER_OVERRIDE_SET_OCEANWAVEMINAMPLITUDE(float minAmplitude);
		void WATER_OVERRIDE_SET_OCEANWAVEMAXAMPLITUDE(float maxAmplitude);
		void WATER_OVERRIDE_SET_RIPPLEBUMPINESS(float bumpiness);
		void WATER_OVERRIDE_SET_RIPPLEMINBUMPINESS(float minBumpiness);
		void WATER_OVERRIDE_SET_RIPPLEMAXBUMPINESS(float maxBumpiness);
		void WATER_OVERRIDE_SET_RIPPLEDISTURB(float disturb);
		/**
		 * This seems to edit the water wave, intensity around your current location.
		 * 
		 * 0.0f = Normal
		 * 1.0f = So Calm and Smooth, a boat will stay still.
		 * 3.0f = Really Intense.
		 */
		void WATER_OVERRIDE_SET_STRENGTH(float strength);
		void WATER_OVERRIDE_FADE_IN(float p0);
		void WATER_OVERRIDE_FADE_OUT(float p0);
		/**
		 * Sets the the normalized wind speed value. The wind speed clamps always at 12.0, SET_WIND sets the wind in a percentage, 0.0 is 0 and 1.0 is 12.0. Setting this value to a negative number resumes the random wind speed changes provided by the game.
		 */
		void SET_WIND(float speed);
		/**
		 * Using this native will set the absolute wind speed value. The wind speed clamps to a range of 0.0- 12.0. Setting this value to a negative number resumes the random wind speed changes provided by the game.
		 */
		void SET_WIND_SPEED(float speed);
		float GET_WIND_SPEED();
		/**
		 * The wind direction in radians
		 * 180 degrees (PI), wind will blow from the south. Setting this value to a negative number resumes the random wind direction changes provided by the game.
		 */
		void SET_WIND_DIRECTION(float direction);
		Vector3 GET_WIND_DIRECTION();
		/**
		 * With an `intensity` higher than `0.5f`, only the creation of puddles gets faster, rain and rain sound won't increase after that.
		 * With an `intensity` of `0.0f` rain and rain sounds are disabled and there won't be any new puddles.
		 * To use the rain intensity of the current weather, call this native with `-1f` as `intensity`.
		 */
		void SET_RAIN(float intensity);
		float GET_RAIN_LEVEL();
		void SET_SNOW(float level);
		float GET_SNOW_LEVEL();
		/**
		 * creates single lightning+thunder at random position
		 */
		void FORCE_LIGHTNING_FLASH();
		void SET_CLOUD_SETTINGS_OVERRIDE(const char* p0);
		void PRELOAD_CLOUD_HAT(const char* name);
		/**
		 * The following cloudhats are useable:
		 * altostratus
		 * Cirrus
		 * cirrocumulus
		 * Clear 01
		 * Cloudy 01
		 * Contrails
		 * Horizon
		 * horizonband1
		 * horizonband2
		 * horizonband3
		 * horsey
		 * Nimbus
		 * Puffs
		 * RAIN
		 * Snowy 01
		 * Stormy 01
		 * stratoscumulus
		 * Stripey
		 * shower
		 * Wispy
		 * 
		 */
		void LOAD_CLOUD_HAT(const char* name, float transitionTime);
		void UNLOAD_CLOUD_HAT(const char* name, float p1);
		void UNLOAD_ALL_CLOUD_HATS();
		void SET_CLOUDS_ALPHA(float opacity);
		float GET_CLOUDS_ALPHA();
		int GET_GAME_TIMER();
		float GET_FRAME_TIME();
		float GET_SYSTEM_TIME_STEP();
		int GET_FRAME_COUNT();
		float GET_RANDOM_FLOAT_IN_RANGE(float startRange, float endRange);
		int GET_RANDOM_INT_IN_RANGE(int startRange, int endRange);
		int GET_RANDOM_MWC_INT_IN_RANGE(int startRange, int endRange);
		/**
		 * Gets the ground elevation at the specified position. Note that if the specified position is below ground level, the function will output zero!
		 * 
		 * x: Position on the X-axis to get ground elevation at.
		 * y: Position on the Y-axis to get ground elevation at.
		 * z: Position on the Z-axis to get ground elevation at.
		 * groundZ: The ground elevation at the specified position.
		 * ignoreWater: Nearly always 0, very rarely 1 in the scripts: https://gfycat.com/NiftyTatteredCricket
		 * 
		 * Bear in mind this native can only calculate the elevation when the coordinates are within the client's render distance.
		 */
		BOOL GET_GROUND_Z_FOR_3D_COORD(float x, float y, float z, float* groundZ, BOOL ignoreWater, BOOL p5);
		BOOL GET_GROUND_Z_AND_NORMAL_FOR_3D_COORD(float x, float y, float z, float* groundZ, Vector3* normal);
		BOOL GET_GROUND_Z_EXCLUDING_OBJECTS_FOR_3D_COORD(float x, float y, float z, float* groundZ, BOOL p4, BOOL p5);
		float ASIN(float p0);
		float ACOS(float p0);
		float TAN(float p0);
		float ATAN(float p0);
		float ATAN2(float p0, float p1);
		/**
		 * Returns the distance between two three-dimensional points, optionally ignoring the Z values.
		 * If useZ is false, only the 2D plane (X-Y) will be considered for calculating the distance.
		 * 
		 * Consider using this faster native instead: SYSTEM::VDIST - DVIST always takes in consideration the 3D coordinates.
		 */
		float GET_DISTANCE_BETWEEN_COORDS(float x1, float y1, float z1, float x2, float y2, float z2, BOOL useZ);
		float GET_ANGLE_BETWEEN_2D_VECTORS(float x1, float y1, float x2, float y2);
		/**
		 * dx = x1 - x2
		 * dy = y1 - y2
		 */
		float GET_HEADING_FROM_VECTOR_2D(float dx, float dy);
		/**
		 * returns a float between 0.0 and 1.0, clamp: sets whether the product should be clamped between the given coordinates
		 */
		float GET_RATIO_OF_CLOSEST_POINT_ON_LINE(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3, BOOL clamp);
		/**
		 * clamp: sets whether the product should be clamped between the given coordinates
		 */
		Vector3 GET_CLOSEST_POINT_ON_LINE(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3, BOOL clamp);
		BOOL GET_LINE_PLANE_INTERSECTION(float p0, float p1, float p2, float p3, float p4, float p5, float p6, float p7, float p8, float p9, float p10, float p11, float* p12);
		BOOL GET_POINT_AREA_OVERLAP(Any p0, Any p1, Any p2, Any p3, Any p4, Any p5, Any p6, Any p7, Any p8, Any p9, Any p10, Any p11, Any p12, Any p13);
		/**
		 * This sets bit [offset] of [address] to on.
		 * 
		 * The offsets used are different bits to be toggled on and off, typically there is only one address used in a script.
		 * 
		 * Example:
		 * MISC::SET_BIT(&bitAddress, 1);
		 * 
		 * To check if this bit has been enabled:
		 * MISC::IS_BIT_SET(bitAddress, 1); // will return 1 afterwards
		 * 
		 * Please note, this method may assign a value to [address] when used.
		 */
		void SET_BIT(int* address, int offset);
		/**
		 * This sets bit [offset] of [address] to off.
		 * 
		 * Example:
		 * MISC::CLEAR_BIT(&bitAddress, 1);
		 * 
		 * To check if this bit has been enabled:
		 * MISC::IS_BIT_SET(bitAddress, 1); // will return 0 afterwards
		 */
		void CLEAR_BIT(int* address, int offset);
		/**
		 * This native converts its past string to hash. It is hashed using jenkins one at a time method.
		 */
		Hash GET_HASH_KEY(const char* string);
		/**
		 * This native always come right before SET_ENTITY_QUATERNION where its final 4 parameters are SLERP_NEAR_QUATERNION p9 to p12
		 */
		void SLERP_NEAR_QUATERNION(float t, float x, float y, float z, float w, float x1, float y1, float z1, float w1, float* outX, float* outY, float* outZ, float* outW);
		BOOL IS_AREA_OCCUPIED(float p0, float p1, float p2, float p3, float p4, float p5, BOOL p6, BOOL p7, BOOL p8, BOOL p9, BOOL p10, Any p11, BOOL p12);
		BOOL IS_AREA_OCCUPIED_SLOW(Any p0, Any p1, Any p2, Any p3, Any p4, Any p5, Any p6, Any p7, Any p8, Any p9, Any p10, Any p11, Any p12);
		/**
		 * `range`: The range, seems to not be very accurate during testing.
		 * `p4`: Unknown, when set to true it seems to always return true no matter what I try.
		 * `checkVehicle`: Check for any vehicles in that area.
		 * `checkPeds`: Check for any peds in that area.
		 * `ignoreEntity`: This entity will be ignored if it's in the area. Set to 0 if you don't want to exclude any entities.
		 * The BOOL parameters that are documented have not been confirmed. They are just documented from what I've found during testing. They may not work as expected in all cases.
		 * 
		 * Returns true if there is anything in that location matching the provided parameters.
		 */
		BOOL IS_POSITION_OCCUPIED(float x, float y, float z, float range, BOOL p4, BOOL checkVehicles, BOOL checkPeds, BOOL p7, BOOL p8, Entity ignoreEntity, BOOL p10);
		BOOL IS_POINT_OBSCURED_BY_A_MISSION_ENTITY(float p0, float p1, float p2, float p3, float p4, float p5, Any p6);
		/**
		 * Example: CLEAR_AREA(0, 0, 0, 30, true, false, false, false);
		 */
		void CLEAR_AREA(float X, float Y, float Z, float radius, BOOL p4, BOOL ignoreCopCars, BOOL ignoreObjects, BOOL p7);
		/**
		 * MISC::CLEAR_AREA_LEAVE_VEHICLE_HEALTH(x, y, z, radius, false, false, false, false); seem to make all objects go away, peds, vehicles etc. All booleans set to true doesn't seem to change anything.
		 */
		void CLEAR_AREA_LEAVE_VEHICLE_HEALTH(float x, float y, float z, float radius, BOOL p4, BOOL p5, BOOL p6, BOOL p7);
		/**
		 * Example:
		 * CLEAR_AREA_OF_VEHICLES(0.0f, 0.0f, 0.0f, 10000.0f, false, false, false, false, false, false);
		 */
		void CLEAR_AREA_OF_VEHICLES(float x, float y, float z, float radius, BOOL p4, BOOL p5, BOOL p6, BOOL p7, BOOL p8, BOOL p9, Any p10);
		void CLEAR_ANGLED_AREA_OF_VEHICLES(float x1, float y1, float z1, float x2, float y2, float z2, float width, BOOL p7, BOOL p8, BOOL p9, BOOL p10, BOOL p11, Any p12, Any p13);
		/**
		 * I looked through the PC scripts that this site provides you with a link to find. It shows the last param mainly uses, (0, 2, 6, 16, and 17) so I am going to assume it is a type of flag.
		 */
		void CLEAR_AREA_OF_OBJECTS(float x, float y, float z, float radius, int flags);
		/**
		 * Example:       CLEAR_AREA_OF_PEDS(0, 0, 0, 10000, 1);
		 */
		void CLEAR_AREA_OF_PEDS(float x, float y, float z, float radius, int flags);
		/**
		 * flags appears to always be 0
		 */
		void CLEAR_AREA_OF_COPS(float x, float y, float z, float radius, int flags);
		/**
		 * flags is usually 0 in the scripts.
		 */
		void CLEAR_AREA_OF_PROJECTILES(float x, float y, float z, float radius, int flags);
		/**
		 * Possibly used to clear scenario points.
		 */
		void CLEAR_SCENARIO_SPAWN_HISTORY();
		/**
		 * ignoreVehicle - bypasses vehicle check of the local player (it will not open if you are in a vehicle and this is set to false)
		 */
		void SET_SAVE_MENU_ACTIVE(BOOL ignoreVehicle);
		int GET_STATUS_OF_MANUAL_SAVE();
		void SET_CREDITS_ACTIVE(BOOL toggle);
		void SET_CREDITS_FADE_OUT_WITH_SCREEN(BOOL toggle);
		BOOL HAVE_CREDITS_REACHED_END();
		BOOL ARE_CREDITS_RUNNING();
		void TERMINATE_ALL_SCRIPTS_WITH_THIS_NAME(const char* scriptName);
		void NETWORK_SET_SCRIPT_IS_SAFE_FOR_NETWORK_GAME();
		/**
		 * Returns the index of the newly created hospital spawn point.
		 * 
		 * p3 might be radius?
		 */
		int ADD_HOSPITAL_RESTART(float x, float y, float z, float p3, Any p4);
		/**
		 * The game by default has 5 hospital respawn points. Disabling them all will cause the player to respawn at the last position they were.
		 */
		void DISABLE_HOSPITAL_RESTART(int hospitalIndex, BOOL toggle);
		int ADD_POLICE_RESTART(float p0, float p1, float p2, float p3, Any p4);
		/**
		 * Disables the spawn point at the police house on the specified index.
		 * 
		 * policeIndex: The police house index.
		 * toggle: true to enable the spawn point, false to disable.
		 * 
		 * - Nacorpio
		 */
		void DISABLE_POLICE_RESTART(int policeIndex, BOOL toggle);
		void SET_RESTART_COORD_OVERRIDE(float x, float y, float z, float heading);
		void CLEAR_RESTART_COORD_OVERRIDE();
		void PAUSE_DEATH_ARREST_RESTART(BOOL toggle);
		void IGNORE_NEXT_RESTART(BOOL toggle);
		/**
		 * Sets whether the game should fade out after the player dies.
		 */
		void SET_FADE_OUT_AFTER_DEATH(BOOL toggle);
		/**
		 * Sets whether the game should fade out after the player is arrested.
		 */
		void SET_FADE_OUT_AFTER_ARREST(BOOL toggle);
		/**
		 * Sets whether the game should fade in after the player dies or is arrested.
		 */
		void SET_FADE_IN_AFTER_DEATH_ARREST(BOOL toggle);
		void SET_FADE_IN_AFTER_LOAD(BOOL toggle);
		/**
		 * returns savehouseHandle
		 */
		int REGISTER_SAVE_HOUSE(float x, float y, float z, float p3, const char* p4, Any p5, Any p6);
		void SET_SAVE_HOUSE(int savehouseHandle, BOOL p1, BOOL p2);
		BOOL OVERRIDE_SAVE_HOUSE(BOOL p0, float p1, float p2, float p3, float p4, BOOL p5, float p6, float p7);
		BOOL GET_SAVE_HOUSE_DETAILS_AFTER_SUCCESSFUL_LOAD(Vector3* p0, float* p1, BOOL* fadeInAfterLoad, BOOL* p3);
		void DO_AUTO_SAVE();
		/**
		 * Returns true if profile setting 208 is equal to 0.
		 */
		BOOL GET_IS_AUTO_SAVE_OFF();
		BOOL IS_AUTO_SAVE_IN_PROGRESS();
		BOOL HAS_CODE_REQUESTED_AUTOSAVE();
		void CLEAR_CODE_REQUESTED_AUTOSAVE();
		void BEGIN_REPLAY_STATS(Any p0, Any p1);
		void ADD_REPLAY_STAT_VALUE(Any value);
		void END_REPLAY_STATS();
		BOOL HAVE_REPLAY_STATS_BEEN_STORED();
		int GET_REPLAY_STAT_MISSION_ID();
		int GET_REPLAY_STAT_MISSION_TYPE();
		int GET_REPLAY_STAT_COUNT();
		int GET_REPLAY_STAT_AT_INDEX(int index);
		void CLEAR_REPLAY_STATS();
		BOOL QUEUE_MISSION_REPEAT_LOAD();
		/**
		 * Shows the screen which is visible before you redo a mission? The game will make a restoration point where you will cameback when the mission is over.
		 * Returns 1 if the message isn't currently on screen
		 */
		BOOL QUEUE_MISSION_REPEAT_SAVE();
		BOOL QUEUE_MISSION_REPEAT_SAVE_FOR_BENCHMARK_TEST();
		int GET_STATUS_OF_MISSION_REPEAT_SAVE();
		BOOL IS_MEMORY_CARD_IN_USE();
		void SHOOT_SINGLE_BULLET_BETWEEN_COORDS(float x1, float y1, float z1, float x2, float y2, float z2, int damage, BOOL p7, Hash weaponHash, Ped ownerPed, BOOL isAudible, BOOL isInvisible, float speed);
		/**
		 * entity - entity to ignore
		 */
		void SHOOT_SINGLE_BULLET_BETWEEN_COORDS_IGNORE_ENTITY(float x1, float y1, float z1, float x2, float y2, float z2, int damage, BOOL p7, Hash weaponHash, Ped ownerPed, BOOL isAudible, BOOL isInvisible, float speed, Entity entity, Any p14);
		/**
		 * entity - entity to ignore
		 * targetEntity - entity to home in on, if the weapon hash provided supports homing
		 */
		void SHOOT_SINGLE_BULLET_BETWEEN_COORDS_IGNORE_ENTITY_NEW(float x1, float y1, float z1, float x2, float y2, float z2, int damage, BOOL p7, Hash weaponHash, Ped ownerPed, BOOL isAudible, BOOL isInvisible, float speed, Entity entity, BOOL p14, BOOL p15, Entity targetEntity, BOOL p17, Any p18, Any p19, Any p20);
		/**
		 * Gets the dimensions of a model.
		 * 
		 * Calculate (maximum - minimum) to get the size, in which case, Y will be how long the model is.
		 * 
		 * Example from the scripts: MISC::GET_MODEL_DIMENSIONS(ENTITY::GET_ENTITY_MODEL(PLAYER::PLAYER_PED_ID()), &v_1A, &v_17);
		 */
		void GET_MODEL_DIMENSIONS(Hash modelHash, Vector3* minimum, Vector3* maximum);
		/**
		 * Sets a visually fake wanted level on the user interface. Used by Rockstar's scripts to "override" regular wanted levels and make custom ones while the real wanted level and multipliers are still in effect.
		 * 
		 * Max is 6, anything above this makes it just 6. Also the mini-map gets the red & blue flashing effect.
		 */
		void SET_FAKE_WANTED_LEVEL(int fakeWantedLevel);
		int GET_FAKE_WANTED_LEVEL();
		void USING_MISSION_CREATOR(BOOL toggle);
		void ALLOW_MISSION_CREATOR_WARP(BOOL toggle);
		void SET_MINIGAME_IN_PROGRESS(BOOL toggle);
		BOOL IS_MINIGAME_IN_PROGRESS();
		BOOL IS_THIS_A_MINIGAME_SCRIPT();
		/**
		 * This function is hard-coded to always return 0.
		 */
		BOOL IS_SNIPER_INVERTED();
		/**
		 * Returns true if the game is using the metric measurement system (profile setting 227), false if imperial is used.
		 */
		BOOL SHOULD_USE_METRIC_MEASUREMENTS();
		int GET_PROFILE_SETTING(int profileSetting);
		BOOL ARE_STRINGS_EQUAL(const char* string1, const char* string2);
		/**
		 * Compares two strings up to a specified number of characters.
		 * 
		 * Parameters:
		 * str1 - String to be compared.
		 * str2 - String to be compared.
		 * matchCase - Comparison will be case-sensitive.
		 * maxLength - Maximum number of characters to compare. A value of -1 indicates an infinite length.
		 * 
		 * Returns:
		 * A value indicating the relationship between the strings:
		 * <0 - The first non-matching character in 'str1' is less than the one in 'str2'. (e.g. 'A' < 'B', so result = -1)
		 * 0 - The contents of both strings are equal.
		 * >0 - The first non-matching character in 'str1' is less than the one in 'str2'. (e.g. 'B' > 'A', so result = 1)
		 * 
		 * Examples:
		 * MISC::COMPARE_STRINGS("STRING", "string", false, -1); // 0; equal
		 * MISC::COMPARE_STRINGS("TESTING", "test", false, 4); // 0; equal
		 * MISC::COMPARE_STRINGS("R2D2", "R2xx", false, 2); // 0; equal
		 * MISC::COMPARE_STRINGS("foo", "bar", false, -1); // 4; 'f' > 'b'
		 * MISC::COMPARE_STRINGS("A", "A", true, 1); // 0; equal
		 * 
		 * When comparing case-sensitive strings, lower-case characters are greater than upper-case characters:
		 * MISC::COMPARE_STRINGS("A", "a", true, 1); // -1; 'A' < 'a'
		 * MISC::COMPARE_STRINGS("a", "A", true, 1); // 1; 'a' > 'A'
		 */
		int COMPARE_STRINGS(const char* str1, const char* str2, BOOL matchCase, int maxLength);
		int ABSI(int value);
		float ABSF(float value);
		/**
		 * Determines whether there is a sniper bullet within the specified coordinates. The coordinates form an axis-aligned bounding box.
		 */
		BOOL IS_SNIPER_BULLET_IN_AREA(float x1, float y1, float z1, float x2, float y2, float z2);
		/**
		 * Determines whether there is a projectile within the specified coordinates. The coordinates form a rectangle.
		 * 
		 * - Nacorpio
		 * 
		 * 
		 * ownedByPlayer = only projectiles fired by the player will be detected.
		 */
		BOOL IS_PROJECTILE_IN_AREA(float x1, float y1, float z1, float x2, float y2, float z2, BOOL ownedByPlayer);
		/**
		 * Determines whether there is a projectile of a specific type within the specified coordinates. The coordinates form a axis-aligned bounding box.
		 */
		BOOL IS_PROJECTILE_TYPE_IN_AREA(float x1, float y1, float z1, float x2, float y2, float z2, int type, BOOL ownedByPlayer);
		/**
		 * See IS_POINT_IN_ANGLED_AREA for the definition of an angled area.
		 */
		BOOL IS_PROJECTILE_TYPE_IN_ANGLED_AREA(float x1, float y1, float z1, float x2, float y2, float z2, float width, Any p7, BOOL ownedByPlayer);
		BOOL IS_PROJECTILE_TYPE_WITHIN_DISTANCE(float x, float y, float z, Hash projectileHash, float radius, BOOL ownedByPlayer);
		BOOL GET_COORDS_OF_PROJECTILE_TYPE_IN_AREA(float x1, float y1, float z1, float x2, float y2, float z2, Hash projectileHash, Vector3* projectilePos, BOOL ownedByPlayer);
		BOOL GET_COORDS_OF_PROJECTILE_TYPE_IN_ANGLED_AREA(float vecAngledAreaPoint1X, float vecAngledAreaPoint1Y, float vecAngledAreaPoint1Z, float vecAngledAreaPoint2X, float vecAngledAreaPoint2Y, float vecAngledAreaPoint2Z, float distanceOfOppositeFace, Hash weaponType, Vector3* positionOut, BOOL bIsPlayer);
		BOOL GET_COORDS_OF_PROJECTILE_TYPE_WITHIN_DISTANCE(Ped ped, Hash weaponHash, float distance, Vector3* outCoords, BOOL p4);
		BOOL GET_PROJECTILE_OF_PROJECTILE_TYPE_WITHIN_DISTANCE(Ped ped, Hash weaponHash, float distance, Vector3* outCoords, Object* outProjectile, BOOL p5);
		/**
		 * For projectiles, see: IS_PROJECTILE_TYPE_IN_ANGLED_AREA
		 * See IS_POINT_IN_ANGLED_AREA for the definition of an angled area.
		 * Returns True if a bullet, as maintained by a pool within CWeaponManager, has been fired into the defined angled area.
		 */
		BOOL IS_BULLET_IN_ANGLED_AREA(float x1, float y1, float z1, float x2, float y2, float z2, float width, BOOL ownedByPlayer);
		BOOL IS_BULLET_IN_AREA(float x, float y, float z, float radius, BOOL ownedByPlayer);
		BOOL IS_BULLET_IN_BOX(float x1, float y1, float z1, float x2, float y2, float z2, BOOL ownedByPlayer);
		/**
		 * p3 - possibly radius?
		 */
		BOOL HAS_BULLET_IMPACTED_IN_AREA(float x, float y, float z, float p3, BOOL p4, BOOL p5);
		BOOL HAS_BULLET_IMPACTED_IN_BOX(float p0, float p1, float p2, float p3, float p4, float p5, BOOL p6, BOOL p7);
		/**
		 * PS4
		 */
		BOOL IS_ORBIS_VERSION();
		/**
		 * XBOX ONE
		 */
		BOOL IS_DURANGO_VERSION();
		BOOL IS_XBOX360_VERSION();
		BOOL IS_PS3_VERSION();
		BOOL IS_PC_VERSION();
		BOOL IS_STEAM_VERSION();
		void _IS_XBOXPC_VERSION(); // Missing in crossmap
		/**
		 * Used to block some of the prostitute stuff due to laws in Australia.
		 */
		BOOL IS_AUSSIE_VERSION();
		BOOL IS_JAPANESE_VERSION();
		BOOL IS_XBOX_PLATFORM();
		/**
		 * Xbox Series (Scarlett) version...
		 */
		BOOL IS_SCARLETT_VERSION();
		BOOL IS_SCE_PLATFORM();
		/**
		 * PS5 (Prospero) version...
		 */
		BOOL IS_PROSPERO_VERSION();
		BOOL IS_STRING_NULL(const char* string);
		BOOL IS_STRING_NULL_OR_EMPTY(const char* string);
		/**
		 * Returns false if it's a null or empty string or if the string is too long. outInteger will be set to -999 in that case.
		 * 
		 * If all checks have passed successfully, the return value will be set to whatever strtol(string, 0i64, 10); returns.
		 */
		BOOL STRING_TO_INT(const char* string, int* outInteger);
		void SET_BITS_IN_RANGE(int* var, int rangeStart, int rangeEnd, int p3);
		int GET_BITS_IN_RANGE(int var, int rangeStart, int rangeEnd);
		/**
		 * See description of `ADD_STUNT_JUMP_ANGLED` for detailed info. The only difference really is this one does not have the radius (or angle, not sure) floats parameters for entry and landing zones.
		 */
		int ADD_STUNT_JUMP(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3, float x4, float y4, float z4, float camX, float camY, float camZ, int p15, int p16, int p17);
		/**
		 * Creates a new stunt jump.
		 * 
		 * The radius1 and radius2 might actually not be a radius at all, but that's what it seems to me testing them in-game. But they may be 'angle' floats instead, considering this native is named ADD_STUNT_JUMP_**ANGLED**.
		 * 
		 * Info about the specific 'parameter sections':
		 * 
		 * 
		 * **x1, y1, z1, x2, y2, z2 and radius1:**
		 * 
		 * First coordinates are for the jump entry area, and the radius that will be checked around that area. So if you're not exactly within the coordinates, but you are within the outter radius limit then it will still register as entering the stunt jump. Note as mentioned above, the radius is just a guess, I'm not really sure about it's exact purpose.
		 * 
		 * 
		 * **x3, y3, z3, x4, y4, z4 and radius2:**
		 * 
		 * Next part is the landing area, again starting with the left bottom (nearest to the stunt jump entry zone) coordinate, and the second one being the top right furthest away part of the landing area. Followed by another (most likely) radius float, this is usually slightly larger than the entry zone 'radius' float value, just because you have quite a lot of places where you can land (I'm guessing).
		 * 
		 * 
		 * **camX, camY and camZ:**
		 * 
		 * The final coordinate in this native is the Camera position. Rotation and zoom/FOV is managed by the game itself, you just need to provide the camera location.
		 * 
		 * 
		 * **unk1, unk2 and unk3:**
		 * 
		 * Not sure what these are for, but they're always `150, 0, 0` in decompiled scripts.
		 * 
		 * Here is a list of almost all of the stunt jumps from GTA V (taken from decompiled scripts): https://pastebin.com/EW1jBPkY
		 */
		int ADD_STUNT_JUMP_ANGLED(float x1, float y1, float z1, float x2, float y2, float z2, float radius1, float x3, float y3, float z3, float x4, float y4, float z4, float radius2, float camX, float camY, float camZ, int p17, int p18, int p19);
		/**
		 * Toggles some stunt jump stuff.
		 */
		void TOGGLE_SHOW_OPTIONAL_STUNT_JUMP_CAMERA(BOOL toggle);
		void DELETE_STUNT_JUMP(int p0);
		void ENABLE_STUNT_JUMP_SET(int p0);
		void DISABLE_STUNT_JUMP_SET(int p0);
		void SET_STUNT_JUMPS_CAN_TRIGGER(BOOL toggle);
		BOOL IS_STUNT_JUMP_IN_PROGRESS();
		BOOL IS_STUNT_JUMP_MESSAGE_SHOWING();
		int GET_NUM_SUCCESSFUL_STUNT_JUMPS();
		int GET_TOTAL_SUCCESSFUL_STUNT_JUMPS();
		void CANCEL_STUNT_JUMP();
		/**
		 * Make sure to call this from the correct thread if you're using multiple threads because all other threads except the one which is calling SET_GAME_PAUSED will be paused which means you will lose control and the game remains in paused mode until you exit GTA5.exe
		 */
		void SET_GAME_PAUSED(BOOL toggle);
		void SET_THIS_SCRIPT_CAN_BE_PAUSED(BOOL toggle);
		void SET_THIS_SCRIPT_CAN_REMOVE_BLIPS_CREATED_BY_ANY_SCRIPT(BOOL toggle);
		/**
		 * This native appears on the cheat_controller script and tracks a combination of buttons, which may be used to toggle cheats in-game. Credits to ThreeSocks for the info. The hash contains the combination, while the "amount" represents the amount of buttons used in a combination. The following page can be used to make a button combination: gta5offset.com/ts/hash/
		 * 
		 * INT_SCORES_SCORTED was a hash collision
		 */
		BOOL HAS_CHEAT_WITH_HASH_BEEN_ACTIVATED(Hash hash, int amount);
		/**
		 * Get inputted "Cheat code", for example:
		 * 
		 * while (TRUE)
		 * {
		 * if (MISC::HAS_PC_CHEAT_WITH_HASH_BEEN_ACTIVATED(${fugitive}))
		 * {
		 * // Do something.
		 * }
		 * SYSTEM::WAIT(0);
		 * }
		 * 
		 * Calling this will also set the last saved string hash to zero.
		 * 
		 */
		BOOL HAS_PC_CHEAT_WITH_HASH_BEEN_ACTIVATED(Hash hash);
		void OVERRIDE_FREEZE_FLAGS(BOOL p0);
		/**
		 * Formerly known as _LOWER_MAP_PROP_DENSITY and wrongly due to idiots as _ENABLE_MP_DLC_MAPS.
		 * Sets the maximum prop density and changes a loading screen flag from 'loading story mode' to 'loading GTA Online'. Does not touch DLC map data at all.
		 * 
		 * In fact, I doubt this changes the flag whatsoever, that's the OTHER native idiots use together with this that does so, this one only causes a loading screen to show as it reloads map data.
		 */
		void SET_INSTANCE_PRIORITY_MODE(int p0);
		/**
		 * Sets an unknown flag used by CScene in determining which entities from CMapData scene nodes to draw, similar to SET_INSTANCE_PRIORITY_MODE.
		 */
		void SET_INSTANCE_PRIORITY_HINT(int flag);
		/**
		 * This function is hard-coded to always return 0.
		 */
		BOOL IS_FRONTEND_FADING();
		/**
		 * spawns a few distant/out-of-sight peds, vehicles, animals etc each time it is called
		 */
		void POPULATE_NOW();
		int GET_INDEX_OF_CURRENT_LEVEL();
		/**
		 * level can be from 0 to 3
		 * 0: 9.8 - normal
		 * 1: 2.4 - low
		 * 2: 0.1 - very low
		 * 3: 0.0 - off
		 */
		void SET_GRAVITY_LEVEL(int level);
		void START_SAVE_DATA(Any* p0, Any p1, BOOL p2);
		void STOP_SAVE_DATA();
		int GET_SIZE_OF_SAVE_DATA(BOOL p0);
		void REGISTER_INT_TO_SAVE(Any* p0, const char* name);
		void REGISTER_INT64_TO_SAVE(Any* p0, const char* name);
		void REGISTER_ENUM_TO_SAVE(Any* p0, const char* name);
		void REGISTER_FLOAT_TO_SAVE(Any* p0, const char* name);
		void REGISTER_BOOL_TO_SAVE(Any* p0, const char* name);
		void REGISTER_TEXT_LABEL_TO_SAVE(Any* p0, const char* name);
		/**
		 * MISC::REGISTER_TEXT_LABEL_15_TO_SAVE(&a_0._f1, "tlPlateText");
		 * MISC::REGISTER_TEXT_LABEL_15_TO_SAVE(&a_0._f1C, "tlPlateText_pending");
		 * MISC::REGISTER_TEXT_LABEL_15_TO_SAVE(&a_0._f10B, "tlCarAppPlateText");
		 */
		void REGISTER_TEXT_LABEL_15_TO_SAVE(Any* p0, const char* name);
		/**
		 * Only found 3 times in decompiled scripts.
		 * 
		 * MISC::REGISTER_TEXT_LABEL_23_TO_SAVE(a_0, "Movie_Name_For_This_Player");
		 * MISC::REGISTER_TEXT_LABEL_23_TO_SAVE(&a_0._fB, "Ringtone_For_This_Player");
		 * MISC::REGISTER_TEXT_LABEL_23_TO_SAVE(&a_0._f1EC4._f12[v_A/*6* /], &v_13); // where v_13 is "MPATMLOGSCRS0" thru "MPATMLOGSCRS15"
		 */
		void REGISTER_TEXT_LABEL_23_TO_SAVE(Any* p0, const char* name);
		/**
		 * Only found 2 times in decompiled scripts.
		 * 
		 * MISC::REGISTER_TEXT_LABEL_31_TO_SAVE(&a_0._f1F5A._f6[0/*8* /], "TEMPSTAT_LABEL"); // gets saved in a struct called "g_SaveData_STRING_ScriptSaves"
		 * MISC::REGISTER_TEXT_LABEL_31_TO_SAVE(&a_0._f4B4[v_1A/*8* /], &v_5); // where v_5 is "Name0" thru "Name9", gets saved in a struct called "OUTFIT_Name"
		 */
		void REGISTER_TEXT_LABEL_31_TO_SAVE(Any* p0, const char* name);
		/**
		 * MISC::REGISTER_TEXT_LABEL_63_TO_SAVE(a_0, "Thumb_label");
		 * MISC::REGISTER_TEXT_LABEL_63_TO_SAVE(&a_0._f10, "Photo_label");
		 * MISC::REGISTER_TEXT_LABEL_63_TO_SAVE(a_0, "GXTlabel");
		 * MISC::REGISTER_TEXT_LABEL_63_TO_SAVE(&a_0._f21, "StringComp");
		 * MISC::REGISTER_TEXT_LABEL_63_TO_SAVE(&a_0._f43, "SecondStringComp");
		 * MISC::REGISTER_TEXT_LABEL_63_TO_SAVE(&a_0._f53, "ThirdStringComp");
		 * MISC::REGISTER_TEXT_LABEL_63_TO_SAVE(&a_0._f32, "SenderStringComp");
		 * MISC::REGISTER_TEXT_LABEL_63_TO_SAVE(&a_0._f726[v_1A/*16* /], &v_20); // where v_20 is "LastJobTL_0_1" thru "LastJobTL_2_1", gets saved in a struct called "LAST_JobGamer_TL"
		 * MISC::REGISTER_TEXT_LABEL_63_TO_SAVE(&a_0._f4B, "PAID_PLAYER");
		 * MISC::REGISTER_TEXT_LABEL_63_TO_SAVE(&a_0._f5B, "RADIO_STATION");
		 */
		void REGISTER_TEXT_LABEL_63_TO_SAVE(Any* p0, const char* name);
		void START_SAVE_STRUCT_WITH_SIZE(Any* p0, int size, const char* structName);
		void STOP_SAVE_STRUCT();
		void START_SAVE_ARRAY_WITH_SIZE(Any* p0, int size, const char* arrayName);
		void STOP_SAVE_ARRAY();
		void COPY_SCRIPT_STRUCT(Any* dst, Any* src, int size);
		/**
		 * https://alloc8or.re/gta5/doc/enums/DispatchType.txt
		 */
		void ENABLE_DISPATCH_SERVICE(int dispatchService, BOOL toggle);
		void BLOCK_DISPATCH_SERVICE_RESOURCE_CREATION(int dispatchService, BOOL toggle);
		int GET_NUMBER_RESOURCES_ALLOCATED_TO_WANTED_LEVEL(int dispatchService);
		/**
		 * As for the 'police' incident, it will call police cars to you, but unlike PedsInCavalcades & Merryweather they won't start shooting at you unless you shoot first or shoot at them. The top 2 however seem to cancel theirselves if there is noone dead around you or a fire. I only figured them out as I found out the 3rd param is definately the amountOfPeople and they called incident 3 in scripts with 4 people (which the firetruck has) and incident 5 with 2 people (which the ambulence has). The 4 param I cant say is radius, but for the pedsInCavalcades and Merryweather R* uses 0.0f and for the top 3 (Emergency Services) they use 3.0f.
		 * 
		 * Side Note: It seems calling the pedsInCavalcades or Merryweather then removing it seems to break you from calling the EmergencyEvents and I also believe pedsInCavalcades. (The V cavalcades of course not IV).
		 * 
		 * Side Note 2: I say it breaks as if you call this proper,
		 * if(CREATE_INCIDENT) etc it will return false if you do as I said above.
		 * =====================================================
		 */
		BOOL CREATE_INCIDENT(int dispatchService, float x, float y, float z, int numUnits, float radius, int* outIncidentID, Any p7, Any p8);
		/**
		 * As for the 'police' incident, it will call police cars to you, but unlike PedsInCavalcades & Merryweather they won't start shooting at you unless you shoot first or shoot at them. The top 2 however seem to cancel theirselves if there is noone dead around you or a fire. I only figured them out as I found out the 3rd param is definately the amountOfPeople and they called incident 3 in scripts with 4 people (which the firetruck has) and incident 5 with 2 people (which the ambulence has). The 4 param I cant say is radius, but for the pedsInCavalcades and Merryweather R* uses 0.0f and for the top 3 (Emergency Services) they use 3.0f.
		 * 
		 * Side Note: It seems calling the pedsInCavalcades or Merryweather then removing it seems to break you from calling the EmergencyEvents and I also believe pedsInCavalcades. (The V cavalcades of course not IV).
		 * 
		 * Side Note 2: I say it breaks as if you call this proper,
		 * if(CREATE_INCIDENT) etc it will return false if you do as I said above.
		 * =====================================================
		 */
		BOOL CREATE_INCIDENT_WITH_ENTITY(int dispatchService, Ped ped, int numUnits, float radius, int* outIncidentID, Any p5, Any p6);
		/**
		 * Delete an incident with a given id.
		 * 
		 * =======================================================
		 * Correction, I have change this to int, instead of int*
		 * as it doesn't use a pointer to the createdIncident.
		 * If you try it you will crash (or) freeze.
		 * =======================================================
		 */
		void DELETE_INCIDENT(int incidentId);
		/**
		 * =======================================================
		 * Correction, I have change this to int, instead of int*
		 * as it doesn't use a pointer to the createdIncident.
		 * If you try it you will crash (or) freeze.
		 * =======================================================
		 */
		BOOL IS_INCIDENT_VALID(int incidentId);
		void SET_INCIDENT_REQUESTED_UNITS(int incidentId, int dispatchService, int numUnits);
		void SET_IDEAL_SPAWN_DISTANCE_FOR_INCIDENT(int incidentId, float p1);
		/**
		 * Finds a position ahead of the player by predicting the players next actions.
		 * The positions match path finding node positions.
		 * When roads diverge, the position may rapidly change between two or more positions. This is due to the engine not being certain of which path the player will take.
		 */
		BOOL FIND_SPAWN_POINT_IN_DIRECTION(float posX, float posY, float posZ, float fwdVecX, float fwdVecY, float fwdVecZ, float distance, Vector3* spawnPoint);
		int ADD_POP_MULTIPLIER_AREA(float x1, float y1, float z1, float x2, float y2, float z2, float p6, float p7, BOOL p8, BOOL p9);
		BOOL DOES_POP_MULTIPLIER_AREA_EXIST(int id);
		void REMOVE_POP_MULTIPLIER_AREA(int id, BOOL p1);
		BOOL IS_POP_MULTIPLIER_AREA_NETWORKED(int id);
		/**
		 * This native is adding a zone, where you can change density settings. For example, you can add a zone on 0.0, 0.0, 0.0 with radius 900.0 and vehicleMultiplier 0.0, and you will not see any new population vehicle spawned in a radius of 900.0 from 0.0, 0.0, 0.0. Returns the id. You can have only 15 zones at the same time. You can remove zone using REMOVE_POP_MULTIPLIER_SPHERE
		 */
		int ADD_POP_MULTIPLIER_SPHERE(float x, float y, float z, float radius, float pedMultiplier, float vehicleMultiplier, BOOL p6, BOOL p7);
		BOOL DOES_POP_MULTIPLIER_SPHERE_EXIST(int id);
		/**
		 * Removes population multiplier sphere
		 */
		void REMOVE_POP_MULTIPLIER_SPHERE(int id, BOOL p1);
		/**
		 * Makes the ped jump around like they're in a tennis match
		 */
		void ENABLE_TENNIS_MODE(Ped ped, BOOL toggle, BOOL p2);
		BOOL IS_TENNIS_MODE(Ped ped);
		/**
		 * Full list of animation dictionaries and anims by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/animDictsCompact.json
		 */
		void PLAY_TENNIS_SWING_ANIM(Ped ped, const char* animDict, const char* animName, float p3, float p4, BOOL p5);
		BOOL GET_TENNIS_SWING_ANIM_COMPLETE(Ped ped);
		BOOL GET_TENNIS_SWING_ANIM_CAN_BE_INTERRUPTED(Ped ped);
		BOOL GET_TENNIS_SWING_ANIM_SWUNG(Ped ped);
		void PLAY_TENNIS_DIVE_ANIM(Ped ped, int p1, float p2, float p3, float p4, BOOL p5);
		/**
		 * From the scripts:
		 * 
		 * MISC::SET_TENNIS_MOVE_NETWORK_SIGNAL_FLOAT(sub_aa49(a_0), "ForcedStopDirection", v_E);
		 * 
		 * Related to tennis mode.
		 */
		void SET_TENNIS_MOVE_NETWORK_SIGNAL_FLOAT(Ped ped, const char* p1, float p2);
		void RESET_DISPATCH_SPAWN_LOCATION();
		void SET_DISPATCH_SPAWN_LOCATION(float x, float y, float z);
		void RESET_DISPATCH_IDEAL_SPAWN_DISTANCE();
		void SET_DISPATCH_IDEAL_SPAWN_DISTANCE(float distance);
		void RESET_DISPATCH_TIME_BETWEEN_SPAWN_ATTEMPTS(Any p0);
		void SET_DISPATCH_TIME_BETWEEN_SPAWN_ATTEMPTS(Any p0, float p1);
		void SET_DISPATCH_TIME_BETWEEN_SPAWN_ATTEMPTS_MULTIPLIER(Any p0, float p1);
		/**
		 * To remove, see: REMOVE_DISPATCH_SPAWN_BLOCKING_AREA
		 * See IS_POINT_IN_ANGLED_AREA for the definition of an angled area.
		 */
		int ADD_DISPATCH_SPAWN_ANGLED_BLOCKING_AREA(float x1, float y1, float z1, float x2, float y2, float z2, float width);
		int ADD_DISPATCH_SPAWN_SPHERE_BLOCKING_AREA(float x1, float y1, float x2, float y2);
		void REMOVE_DISPATCH_SPAWN_BLOCKING_AREA(int p0);
		void RESET_DISPATCH_SPAWN_BLOCKING_AREAS();
		void RESET_WANTED_RESPONSE_NUM_PEDS_TO_SPAWN();
		void SET_WANTED_RESPONSE_NUM_PEDS_TO_SPAWN(int p0, int p1);
		void ADD_TACTICAL_NAV_MESH_POINT(float x, float y, float z);
		void CLEAR_TACTICAL_NAV_MESH_POINTS();
		/**
		 * Activates (usused?) riot mode. All NPCs are being hostile to each other (including player). Also the game will give weapons (pistols, smgs) to random NPCs.
		 */
		void SET_RIOT_MODE_ENABLED(BOOL toggle);
		void DISPLAY_ONSCREEN_KEYBOARD_WITH_LONGER_INITIAL_STRING(int p0, const char* windowTitle, Any* p2, const char* defaultText, const char* defaultConcat1, const char* defaultConcat2, const char* defaultConcat3, const char* defaultConcat4, const char* defaultConcat5, const char* defaultConcat6, const char* defaultConcat7, int maxInputLength);
		/**
		 * sfink: note, p0 is set to 6 for PC platform in at least 1 script, or to `unk::_get_ui_language_id() == 0` otherwise.
		 * 
		 * NOTE: windowTitle uses text labels, and an invalid value will display nothing.
		 * 
		 * www.gtaforums.com/topic/788343-vrel-script-hook-v/?p=1067380474
		 * 
		 * windowTitle's
		 * -----------------
		 * CELL_EMAIL_BOD  =   "Enter your Eyefind message"
		 * CELL_EMAIL_BODE =   "Message too long. Try again"
		 * CELL_EMAIL_BODF    =   "Forbidden message. Try again"
		 * CELL_EMAIL_SOD    =   "Enter your Eyefind subject"
		 * CELL_EMAIL_SODE =   "Subject too long. Try again"
		 * CELL_EMAIL_SODF    =   "Forbidden text. Try again"
		 * CELL_EMASH_BOD   =   "Enter your Eyefind message"
		 * CELL_EMASH_BODE =   "Message too long. Try again"
		 * CELL_EMASH_BODF    =   "Forbidden message. Try again"
		 * CELL_EMASH_SOD    =   "Enter your Eyefind subject"
		 * CELL_EMASH_SODE =   "Subject too long. Try again"
		 * CELL_EMASH_SODF    =   "Forbidden Text. Try again"
		 * FMMC_KEY_TIP10   =   "Enter Synopsis"
		 * FMMC_KEY_TIP12  =   "Enter Custom Team Name"
		 * FMMC_KEY_TIP12F =   "Forbidden Text. Try again"
		 * FMMC_KEY_TIP12N  =   "Custom Team Name"
		 * FMMC_KEY_TIP8 =   "Enter Message"
		 * FMMC_KEY_TIP8F   =   "Forbidden Text. Try again"
		 * FMMC_KEY_TIP8FS  =   "Invalid Message. Try again"
		 * FMMC_KEY_TIP8S  =   "Enter Message"
		 * FMMC_KEY_TIP9    =   "Enter Outfit Name"
		 * FMMC_KEY_TIP9F   =   "Invalid Outfit Name. Try again"
		 * FMMC_KEY_TIP9N  =   "Outfit Name"
		 * PM_NAME_CHALL  =   "Enter Challenge Name"
		 */
		void DISPLAY_ONSCREEN_KEYBOARD(int p0, const char* windowTitle, const char* p2, const char* defaultText, const char* defaultConcat1, const char* defaultConcat2, const char* defaultConcat3, int maxInputLength);
		/**
		 * Returns the current status of the onscreen keyboard, and updates the output.
		 * 
		 * Status Codes:
		 * 
		 * -1: Keyboard isn't active
		 * 0: User still editing
		 * 1: User has finished editing
		 * 2: User has canceled editing
		 */
		int UPDATE_ONSCREEN_KEYBOARD();
		/**
		 * Returns NULL unless UPDATE_ONSCREEN_KEYBOARD() returns 1 in the same tick.
		 */
		const char* GET_ONSCREEN_KEYBOARD_RESULT();
		/**
		 * DO NOT use this as it doesn't clean up the text input box properly and your script will get stuck in the UPDATE_ONSCREEN_KEYBOARD() loop.
		 * Use FORCE_CLOSE_TEXT_INPUT_BOX instead.
		 */
		void CANCEL_ONSCREEN_KEYBOARD();
		/**
		 * p0 was always 2 in R* scripts.
		 * Called before calling DISPLAY_ONSCREEN_KEYBOARD if the input needs to be saved.
		 */
		void NEXT_ONSCREEN_KEYBOARD_RESULT_WILL_DISPLAY_USING_THESE_FONTS(int p0);
		/**
		 * Appears to remove stealth kill action from memory
		 */
		void ACTION_MANAGER_ENABLE_ACTION(Hash hash, BOOL enable);
		/**
		 * GET_GAME_TIMER() / 1000
		 */
		int GET_REAL_WORLD_TIME();
		void SUPRESS_RANDOM_EVENT_THIS_FRAME(int eventType, BOOL suppress);
		void SET_EXPLOSIVE_AMMO_THIS_FRAME(Player player);
		void SET_FIRE_AMMO_THIS_FRAME(Player player);
		void SET_EXPLOSIVE_MELEE_THIS_FRAME(Player player);
		void SET_SUPER_JUMP_THIS_FRAME(Player player);
		void SET_BEAST_JUMP_THIS_FRAME(Player player);
		void SET_FORCED_JUMP_THIS_FRAME(Player player);
		BOOL HAS_GAME_INSTALLED_THIS_SESSION();
		void SET_TICKER_JOHNMARSTON_IS_DONE();
		BOOL ARE_PROFILE_SETTINGS_VALID();
		void PREVENT_ARREST_STATE_THIS_FRAME();
		/**
		 * Sets the localplayer playerinfo state back to playing (State 0)
		 * 
		 * States are:
		 * -1: "Invalid"
		 * 0: "Playing"
		 * 1: "Died"
		 * 2: "Arrested"
		 * 3: "Failed Mission"
		 * 4: "Left Game"
		 * 5: "Respawn"
		 * 6: "In MP Cutscene"
		 */
		void FORCE_GAME_STATE_PLAYING();
		void SCRIPT_RACE_INIT(int p0, int p1, Any p2, Any p3);
		void SCRIPT_RACE_SHUTDOWN();
		void SCRIPT_RACE_PLAYER_HIT_CHECKPOINT(Player player, Any p1, Any p2, Any p3);
		BOOL SCRIPT_RACE_GET_PLAYER_SPLIT_TIME(Player player, int* p1, int* p2);
		void START_END_USER_BENCHMARK();
		void STOP_END_USER_BENCHMARK();
		void RESET_END_USER_BENCHMARK();
		/**
		 * Saves the benchmark recording to %USERPROFILE%\Documents\Rockstar Games\GTA V\Benchmarks and submits some metrics.
		 */
		void SAVE_END_USER_BENCHMARK();
		/**
		 * Returns true if the current frontend menu is FE_MENU_VERSION_SP_PAUSE
		 */
		BOOL UI_STARTED_END_USER_BENCHMARK();
		/**
		 * Returns true if the current frontend menu is FE_MENU_VERSION_LANDING_MENU
		 */
		BOOL LANDING_SCREEN_STARTED_END_USER_BENCHMARK();
		/**
		 * Returns true if command line option '-benchmark' is set.
		 */
		BOOL IS_COMMANDLINE_END_USER_BENCHMARK();
		/**
		 * Returns value of the '-benchmarkIterations' command line option.
		 */
		int GET_BENCHMARK_ITERATIONS();
		/**
		 * Returns value of the '-benchmarkPass' command line option.
		 */
		int GET_BENCHMARK_PASS();
		/**
		 * In singleplayer it does exactly what the name implies. In FiveM / GTA:Online it shows `Disconnecting from GTA Online` HUD and then quits the game.
		 */
		void RESTART_GAME();
		/**
		 * Exits the game and downloads a fresh social club update on next restart.
		 */
		void QUIT_GAME();
		/**
		 * Hardcoded to always return true.
		 */
		BOOL HAS_ASYNC_INSTALL_FINISHED();
		void CLEANUP_ASYNC_INSTALL();
		/**
		 * aka "constrained"
		 */
		BOOL PLM_IS_IN_CONSTRAINED_MODE();
		/**
		 * Returns duration of how long the game has been in power-saving mode (aka "constrained") in milliseconds.
		 */
		int PLM_GET_CONSTRAINED_DURATION_MS();
		/**
		 * If toggle is true, the ped's head is shown in the pause menu
		 * If toggle is false, the ped's head is not shown in the pause menu
		 */
		void SET_PLAYER_IS_IN_ANIMAL_FORM(BOOL toggle);
		/**
		 * Although we don't have a jenkins hash for this one, the name is 100% confirmed.
		 */
		BOOL GET_IS_PLAYER_IN_ANIMAL_FORM();
		void SET_PLAYER_IS_REPEATING_A_MISSION(BOOL toggle);
		/**
		 * Does nothing (it's a nullsub).
		 */
		void DISABLE_SCREEN_DIMMING_THIS_FRAME();
		float GET_CITY_DENSITY();
		void USE_ACTIVE_CAMERA_FOR_TIMESLICING_CENTRE();
		void GET_SCRIPT_ROUTER_CONTEXT(Any* contextData); // Missing in crossmap
		void SET_SCRIPT_ROUTER_LINK(Any* contextData); // Missing in crossmap
		void HAS_PENDING_SCRIPT_ROUTER_LINK(); // Missing in crossmap
		void CLEAR_SCRIPT_ROUTER_LINK(); // Missing in crossmap
		/**
		 * Does nothing (it's a nullsub).
		 */
		void REPORT_INVALID_SCRIPT_ROUTER_ARGUMENT(const char* argument); // Missing in crossmap
		/**
		 * Does nothing (it's a nullsub).
		 */
		void SET_ACTIVITY_SCRIPT_ROUTING_ENABLED(BOOL enabled); // Missing in crossmap
		void IS_SESSION_INITIALIZED(); // Missing in crossmap
		void GET_CHOSEN_CRIMINAL_CAREER(); // Missing in crossmap
		void HAS_FINALIZED_CHOSEN_CRIMINAL_CAREER(); // Missing in crossmap
		void GET_CHOSEN_MP_CHARACTER_SLOT(); // Missing in crossmap
		void RESET_CHOSEN_MP_CHARACTER_SLOT(); // Missing in crossmap
		void SET_CONTENT_ID_INDEX(Hash contentId, int index);
		int GET_CONTENT_ID_INDEX(Hash contentId);
		void _SET_CONTENT_PROP_TYPE(Hash model, int type);
		/**
		 * Returns prop type for given model hash
		 */
		int _GET_CONTENT_PROP_TYPE(Hash model);

	} // namespace MISC

	namespace MOBILE {

		/**
		 * Creates a mobile phone of the specified type.
		 * 
		 * Possible phone types:
		 * 
		 * 0 - Default phone / Michael's phone
		 * 1 - Trevor's phone
		 * 2 - Franklin's phone
		 * 3 - Unused police phone
		 * 4 - Prologue phone
		 * 
		 * Higher values may crash your game.
		 */
		void CREATE_MOBILE_PHONE(int phoneType);
		/**
		 * Destroys the currently active mobile phone.
		 */
		void DESTROY_MOBILE_PHONE();
		/**
		 * The minimum/default is 500.0f. If you plan to make it bigger set it's position as well. Also this seems to need to be called in a loop as when you close the phone the scale is reset. If not in a loop you'd need to call it everytime before you re-open the phone.
		 */
		void SET_MOBILE_PHONE_SCALE(float scale);
		/**
		 * Last parameter is unknown and always zero.
		 */
		void SET_MOBILE_PHONE_ROTATION(float rotX, float rotY, float rotZ, Any p3);
		void GET_MOBILE_PHONE_ROTATION(Vector3* rotation, Vehicle p1);
		void SET_MOBILE_PHONE_POSITION(float posX, float posY, float posZ);
		void GET_MOBILE_PHONE_POSITION(Vector3* position);
		/**
		 * If bool Toggle = true so the mobile is hide to screen.
		 * If bool Toggle = false so the mobile is show to screen.
		 */
		void SCRIPT_IS_MOVING_MOBILE_PHONE_OFFSCREEN(BOOL toggle);
		/**
		 * This one is weird and seems to return a TRUE state regardless of whether the phone is visible on screen or tucked away.
		 * 
		 * 
		 * I can confirm the above. This function is hard-coded to always return 1.
		 */
		BOOL CAN_PHONE_BE_SEEN_ON_SCREEN();
		void SET_MOBILE_PHONE_DOF_STATE(BOOL toggle);
		/**
		 * For move the finger of player, the value of int goes 1 at 5.
		 */
		void CELL_SET_INPUT(int direction);
		/**
		 * if the bool "Toggle" is "true" so the phone is lean.
		 * if the bool "Toggle" is "false" so the phone is not lean.
		 */
		void CELL_HORIZONTAL_MODE_TOGGLE(BOOL toggle);
		void CELL_CAM_ACTIVATE(BOOL p0, BOOL p1);
		void CELL_CAM_ACTIVATE_SELFIE_MODE(BOOL toggle);
		void CELL_CAM_ACTIVATE_SHALLOW_DOF_MODE(BOOL toggle);
		void CELL_CAM_SET_SELFIE_MODE_SIDE_OFFSET_SCALING(float p0);
		void CELL_CAM_SET_SELFIE_MODE_HORZ_PAN_OFFSET(float horizontalPan);
		void CELL_CAM_SET_SELFIE_MODE_VERT_PAN_OFFSET(float vertPan);
		void CELL_CAM_SET_SELFIE_MODE_ROLL_OFFSET(float roll);
		void CELL_CAM_SET_SELFIE_MODE_DISTANCE_SCALING(float distanceScaling);
		void CELL_CAM_SET_SELFIE_MODE_HEAD_YAW_OFFSET(float yaw);
		void CELL_CAM_SET_SELFIE_MODE_HEAD_ROLL_OFFSET(float roll);
		void CELL_CAM_SET_SELFIE_MODE_HEAD_PITCH_OFFSET(float pitch);
		BOOL CELL_CAM_IS_CHAR_VISIBLE_NO_FACE_CHECK(Entity entity);
		void GET_MOBILE_PHONE_RENDER_ID(int* renderId);

	} // namespace MOBILE

	namespace MONEY {

		void NETWORK_INITIALIZE_CASH(int wallet, int bank);
		/**
		 * Note the 2nd parameters are always 1, 0. I have a feeling it deals with your money, wallet, bank. So when you delete the character it of course wipes the wallet cash at that time. So if that was the case, it would be eg, NETWORK_DELETE_CHARACTER(characterIndex, deleteWalletCash, deleteBankCash);
		 */
		void NETWORK_DELETE_CHARACTER(int characterSlot, BOOL p1, BOOL p2);
		void NETWORK_MANUAL_DELETE_CHARACTER(int characterSlot);
		BOOL NETWORK_GET_PLAYER_IS_HIGH_EARNER();
		void NETWORK_CLEAR_CHARACTER_WALLET(int characterSlot);
		void NETWORK_GIVE_PLAYER_JOBSHARE_CASH(int amount, Any* gamerHandle);
		void NETWORK_RECEIVE_PLAYER_JOBSHARE_CASH(int value, Any* gamerHandle);
		BOOL NETWORK_CAN_SHARE_JOB_CASH();
		/**
		 * index
		 * -------
		 * See function sub_1005 in am_boat_taxi.ysc
		 * 
		 * context
		 * ----------
		 * "BACKUP_VAGOS"
		 * "BACKUP_LOST"
		 * "BACKUP_FAMILIES"
		 * "HIRE_MUGGER"
		 * "HIRE_MERCENARY"
		 * "BUY_CARDROPOFF"
		 * "HELI_PICKUP"
		 * "BOAT_PICKUP"
		 * "CLEAR_WANTED"
		 * "HEAD_2_HEAD"
		 * "CHALLENGE"
		 * "SHARE_LAST_JOB"
		 * "DEFAULT"
		 * 
		 * reason
		 * ---------
		 * "NOTREACHTARGET"
		 * "TARGET_ESCAPE"
		 * "DELIVERY_FAIL"
		 * "NOT_USED"
		 * "TEAM_QUIT"
		 * "SERVER_ERROR"
		 * "RECEIVE_LJ_L"
		 * "CHALLENGE_PLAYER_LEFT"
		 * "DEFAULT"
		 * 
		 * unk
		 * -----
		 * Unknown bool value
		 * 
		 * 
		 */
		void NETWORK_REFUND_CASH(int index, const char* context, const char* reason, BOOL p3);
		void NETWORK_DEDUCT_CASH(int amount, const char* p1, const char* p2, BOOL p3, BOOL p4, BOOL p5);
		BOOL NETWORK_MONEY_CAN_BET(int amount, BOOL p1, BOOL p2);
		BOOL NETWORK_CAN_BET(int amount);
		/**
		 * GTAO_CASINO_HOUSE
		 * GTAO_CASINO_INSIDETRACK
		 * GTAO_CASINO_LUCKYWHEEL
		 * GTAO_CASINO_BLACKJACK
		 * GTAO_CASINO_ROULETTE
		 * GTAO_CASINO_SLOTS
		 * GTAO_CASINO_PURCHASE_CHIPS
		 * 
		 * NETWORK_C*
		 */
		BOOL NETWORK_CASINO_CAN_BET(Hash hash);
		BOOL NETWORK_CASINO_CAN_BET_PVC();
		BOOL NETWORK_CASINO_CAN_BET_AMOUNT(Any p0);
		BOOL NETWORK_CASINO_CAN_BUY_CHIPS_PVC();
		BOOL NETWORK_CASINO_BUY_CHIPS(int p0, int p1);
		BOOL NETWORK_CASINO_SELL_CHIPS(int p0, int p1);
		/**
		 * Does nothing (it's a nullsub).
		 */
		void NETWORK_DEFER_CASH_TRANSACTIONS_UNTIL_SHOP_SAVE();
		BOOL CAN_PAY_AMOUNT_TO_BOSS(int p0, int p1, int amount, int* p3);
		void NETWORK_EARN_FROM_PICKUP(int amount);
		void NETWORK_EARN_FROM_CASHING_OUT(int amount);
		void NETWORK_EARN_FROM_GANGATTACK_PICKUP(int amount);
		void NETWORK_EARN_ASSASSINATE_TARGET_KILLED(int amount);
		/**
		 * For the money bags that drop a max of $40,000. Often called 40k bags.
		 * 
		 * Most likely NETWORK_EARN_FROM_ROB***
		 */
		void NETWORK_EARN_FROM_ROB_ARMORED_CARS(int amount);
		void NETWORK_EARN_FROM_CRATE_DROP(int amount);
		void NETWORK_EARN_FROM_BETTING(int amount, const char* p1);
		void NETWORK_EARN_FROM_JOB(int amount, const char* p1);
		void NETWORK_EARN_FROM_JOBX2(int amount, const char* p1);
		void NETWORK_EARN_FROM_PREMIUM_JOB(int amount, const char* p1);
		void NETWORK_EARN_FROM_BEND_JOB(int amount, const char* heistHash);
		void NETWORK_EARN_FROM_CHALLENGE_WIN(Any p0, Any* p1, BOOL p2);
		void NETWORK_EARN_FROM_BOUNTY(int amount, Any* gamerHandle, Any* p2, Any p3);
		void NETWORK_EARN_FROM_IMPORT_EXPORT(int amount, Hash modelHash);
		void NETWORK_EARN_FROM_HOLDUPS(int amount);
		void NETWORK_EARN_FROM_PROPERTY(int amount, Hash propertyName);
		/**
		 * DSPORT
		 */
		void NETWORK_EARN_FROM_AI_TARGET_KILL(Any p0, Any p1);
		void NETWORK_EARN_FROM_NOT_BADSPORT(int amount);
		void NETWORK_EARN_FROM_VEHICLE(Any p0, Any p1, Any p2, Any p3, Any p4, Any p5, Any p6, Any p7);
		void NETWORK_EARN_FROM_PERSONAL_VEHICLE(Any p0, Any p1, Any p2, Any p3, Any p4, Any p5, Any p6, Any p7, Any p8);
		/**
		 * type either Monthly,Weekly,Daily
		 */
		void NETWORK_EARN_FROM_DAILY_OBJECTIVES(int amount, const char* type, int characterSlot);
		/**
		 * Example for p1: "AM_DISTRACT_COPS"
		 */
		void NETWORK_EARN_FROM_AMBIENT_JOB(int p0, const char* p1, Any* p2);
		void NETWORK_EARN_FROM_JOB_BONUS(Any p0, Any* p1, Any* p2);
		void NETWORK_EARN_FROM_CRIMINAL_MASTERMIND(Any p0, Any p1, Any p2);
		void NETWORK_EARN_HEIST_AWARD(Any p0, Any p1, Any p2);
		void NETWORK_EARN_FIRST_TIME_BONUS(Any p0, Any p1, Any p2);
		void NETWORK_EARN_GOON(Any p0, Any p1, Any p2);
		void NETWORK_EARN_BOSS(Any p0, Any p1, Any p2);
		void NETWORK_EARN_AGENCY(Any p0, Any p1, Any p2, Any p3);
		void NETWORK_EARN_FROM_WAREHOUSE(int amount, int id);
		void NETWORK_EARN_FROM_CONTRABAND(int amount, Any p1);
		void NETWORK_EARN_FROM_DESTROYING_CONTRABAND(Any p0, Any p1, Any p2);
		void NETWORK_EARN_FROM_SMUGGLER_WORK(Any p0, Any p1, Any p2, Any p3, Any p4, Any p5);
		void NETWORK_EARN_FROM_HANGAR_TRADE(Any p0, Any p1);
		void NETWORK_EARN_PURCHASE_CLUB_HOUSE(Any p0, Any p1);
		void NETWORK_EARN_FROM_BUSINESS_PRODUCT(int amount, Any p1, Any p2, Any p3);
		void NETWORK_EARN_FROM_VEHICLE_EXPORT(int amount, Any p1, Any p2);
		void NETWORK_EARN_SMUGGLER_AGENCY(int amount, Any p1, Any p2, Any p3);
		void NETWORK_EARN_BOUNTY_HUNTER_REWARD(Any p0);
		void NETWORK_EARN_FROM_BUSINESS_BATTLE(Any p0);
		void NETWORK_EARN_FROM_CLUB_MANAGEMENT_PARTICIPATION(Any p0, int p1);
		void NETWORK_EARN_FROM_FMBB_PHONECALL_MISSION(Any p0);
		void NETWORK_EARN_FROM_BUSINESS_HUB_SELL(Any p0, Any p1, Any p2);
		void NETWORK_EARN_FROM_FMBB_BOSS_WORK(Any p0);
		void NETWORK_EARN_FMBB_WAGE_BONUS(Any p0);
		BOOL NETWORK_CAN_SPEND_MONEY(Any p0, BOOL p1, BOOL p2, BOOL p3, Any p4, Any p5);
		BOOL NETWORK_CAN_SPEND_MONEY2(Any p0, BOOL p1, BOOL p2, BOOL p3, Any* p4, Any p5, Any p6);
		void NETWORK_BUY_ITEM(int amount, Hash item, Any p2, Any p3, BOOL p4, const char* item_name, Any p6, Any p7, Any p8, BOOL p9);
		void NETWORK_SPENT_TAXI(int amount, BOOL p1, BOOL p2, Any p3, Any p4);
		void NETWORK_PAY_EMPLOYEE_WAGE(Any p0, BOOL p1, BOOL p2);
		void NETWORK_PAY_MATCH_ENTRY_FEE(int amount, const char* matchId, BOOL p2, BOOL p3);
		void NETWORK_SPENT_BETTING(int amount, int p1, const char* matchId, BOOL p3, BOOL p4);
		void NETWORK_SPENT_WAGER(Any p0, Any p1, int amount);
		void NETWORK_SPENT_IN_STRIPCLUB(Any p0, BOOL p1, Any p2, BOOL p3);
		void NETWORK_BUY_HEALTHCARE(int cost, BOOL p1, BOOL p2);
		/**
		 * p1 = 0 (always)
		 * p2 = 1 (always)
		 */
		void NETWORK_BUY_AIRSTRIKE(int cost, BOOL p1, BOOL p2, Any p3);
		void NETWORK_BUY_BACKUP_GANG(int p0, int p1, BOOL p2, BOOL p3, int npcProvider);
		/**
		 * p1 = 0 (always)
		 * p2 = 1 (always)
		 */
		void NETWORK_BUY_HELI_STRIKE(int cost, BOOL p1, BOOL p2, Any p3);
		void NETWORK_SPENT_AMMO_DROP(Any p0, BOOL p1, BOOL p2, Any p3);
		/**
		 * p1 is just an assumption. p2 was false and p3 was true.
		 */
		void NETWORK_BUY_BOUNTY(int amount, Player victim, BOOL p2, BOOL p3, Any p4);
		void NETWORK_BUY_PROPERTY(int cost, Hash propertyName, BOOL p2, BOOL p3);
		void NETWORK_BUY_SMOKES(int p0, BOOL p1, BOOL p2);
		void NETWORK_SPENT_HELI_PICKUP(Any p0, BOOL p1, BOOL p2, Any p3);
		void NETWORK_SPENT_BOAT_PICKUP(Any p0, BOOL p1, BOOL p2, Any p3);
		void NETWORK_SPENT_BULL_SHARK(Any p0, BOOL p1, BOOL p2, Any p3);
		void NETWORK_SPENT_CASH_DROP(int amount, BOOL p1, BOOL p2);
		/**
		 * Only used once in a script (am_contact_requests)
		 * p1 = 0
		 * p2 = 1
		 */
		void NETWORK_SPENT_HIRE_MUGGER(Any p0, BOOL p1, BOOL p2, Any p3);
		void NETWORK_SPENT_ROBBED_BY_MUGGER(int amount, BOOL p1, BOOL p2, Any p3);
		void NETWORK_SPENT_HIRE_MERCENARY(Any p0, BOOL p1, BOOL p2, Any p3);
		void NETWORK_SPENT_BUY_WANTEDLEVEL(Any p0, Any* p1, BOOL p2, BOOL p3, Any p4);
		void NETWORK_SPENT_BUY_OFFTHERADAR(Any p0, BOOL p1, BOOL p2, Any p3);
		void NETWORK_SPENT_BUY_REVEAL_PLAYERS(Any p0, BOOL p1, BOOL p2, Any p3);
		void NETWORK_SPENT_CARWASH(Any p0, Any p1, Any p2, BOOL p3, BOOL p4);
		void NETWORK_SPENT_CINEMA(Any p0, Any p1, BOOL p2, BOOL p3);
		void NETWORK_SPENT_TELESCOPE(Any p0, BOOL p1, BOOL p2);
		void NETWORK_SPENT_HOLDUPS(Any p0, BOOL p1, BOOL p2);
		void NETWORK_SPENT_BUY_PASSIVE_MODE(Any p0, BOOL p1, BOOL p2, Any p3);
		void NETWORK_SPENT_BANK_INTEREST(int p0, BOOL p1, BOOL p2);
		void NETWORK_SPENT_PROSTITUTES(Any p0, BOOL p1, BOOL p2);
		void NETWORK_SPENT_ARREST_BAIL(Any p0, BOOL p1, BOOL p2);
		/**
		 * According to how I understood this in the freemode script alone,
		 * The first parameter is determined by a function named, func_5749 within the freemode script which has a list of all the vehicles and a set price to return which some vehicles deals with globals as well. So the first parameter is basically the set in stone insurance cost it's gonna charge you for that specific vehicle model.
		 * 
		 * The second parameter whoever put it was right, they call GET_ENTITY_MODEL with the vehicle as the paremeter.
		 * 
		 * The third parameter is the network handle as they call their little struct<13> func or atleast how the script decompiled it to look which in lamens terms just returns the network handle of the previous owner based on DECOR_GET_INT(vehicle, "Previous_Owner").
		 * 
		 * The fourth parameter is a bool that returns true/false depending on if your bank balance is greater then 0.
		 * 
		 * The fifth and last parameter is a bool that returns true/false depending on if you have the money for the car based on the cost returned by func_5749. In the freemode script eg,
		 * bool hasTheMoney = MONEY::_GET_BANK_BALANCE() < carCost.
		 */
		void NETWORK_SPENT_PAY_VEHICLE_INSURANCE_PREMIUM(int amount, Hash vehicleModel, Any* gamerHandle, BOOL notBankrupt, BOOL hasTheMoney);
		void NETWORK_SPENT_CALL_PLAYER(Any p0, Any* p1, BOOL p2, BOOL p3);
		void NETWORK_SPENT_BOUNTY(Any p0, BOOL p1, BOOL p2);
		void NETWORK_SPENT_FROM_ROCKSTAR(int p0, BOOL p1, BOOL p2);
		/**
		 * Hardcoded to return 0.
		 */
		int NETWORK_SPEND_EARNED_FROM_BANK_AND_WALLETS(int amount);
		/**
		 * This isn't a hash collision.
		 */
		const char* PROCESS_CASH_GIFT(int* p0, int* p1, const char* p2);
		void NETWORK_SPENT_MOVE_SUBMARINE(Any p0, Any p1, Any p2);
		void NETWORK_SPENT_PLAYER_HEALTHCARE(int p0, int p1, BOOL p2, BOOL p3);
		void NETWORK_SPENT_NO_COPS(Any p0, BOOL p1, BOOL p2, Any p3);
		void NETWORK_SPENT_CARGO_SOURCING(Any p0, Any p1, Any p2, Any p3, Any p4, Any p5, Any p6);
		void NETWORK_SPENT_REQUEST_JOB(Any p0, BOOL p1, BOOL p2, Any p3);
		void NETWORK_SPENT_REQUEST_HEIST(Any p0, BOOL p1, BOOL p2, Any p3);
		/**
		 * The first parameter is the amount spent which is store in a global when this native is called. The global returns 10. Which is the price for both rides.
		 * 
		 * The last 3 parameters are,
		 * 2,0,1 in the am_ferriswheel.c
		 * 1,0,1 in the am_rollercoaster.c
		 * 
		 */
		void NETWORK_BUY_FAIRGROUND_RIDE(int amount, Any p1, BOOL p2, BOOL p3, Any p4);
		BOOL NETWORK_ECONOMY_HAS_FIXED_CRAZY_NUMBERS();
		void NETWORK_SPENT_JOB_SKIP(int amount, const char* matchId, BOOL p2, BOOL p3);
		BOOL NETWORK_SPENT_BOSS_GOON(int amount, BOOL p1, BOOL p2);
		void NETWORK_SPEND_GOON(int p0, int p1, int amount);
		void NETWORK_SPEND_BOSS(Any p0, Any p1, Any p2);
		void NETWORK_SPENT_MOVE_YACHT(int amount, BOOL p1, BOOL p2);
		void NETWORK_SPENT_RENAME_ORGANIZATION(Any p0, Any p1, Any p2, Any p3);
		void NETWORK_BUY_CONTRABAND_MISSION(int p0, int p1, Hash p2, BOOL p3, BOOL p4);
		void NETWORK_SPENT_PA_SERVICE_HELI(Any p0, Any p1, Any p2, Any p3);
		void NETWORK_SPENT_PA_SERVICE_VEHICLE(Any p0, Any p1, Any p2, Any p3);
		void NETWORK_SPENT_PA_SERVICE_SNACK(Any p0, Any p1, Any p2, Any p3);
		void NETWORK_SPENT_PA_SERVICE_DANCER(Any p0, Any p1, Any p2, Any p3);
		void NETWORK_SPENT_PA_SERVICE_IMPOUND(Any p0, Any p1, Any p2);
		void NETWORK_SPENT_PA_HELI_PICKUP(Any p0, Any p1, Any p2, Any p3);
		void NETWORK_SPENT_PURCHASE_OFFICE_PROPERTY(Any p0, Any p1, Any p2, Any p3, Any p4);
		void NETWORK_SPENT_UPGRADE_OFFICE_PROPERTY(Any p0, Any p1, Any p2, Any p3, Any p4);
		void NETWORK_SPENT_PURCHASE_WAREHOUSE_PROPERTY(Any p0, Any p1, Any p2, Any p3);
		void NETWORK_SPENT_UPGRADE_WAREHOUSE_PROPERTY(Any p0, Any p1, Any p2, Any p3);
		void NETWORK_SPENT_PURCHASE_IMPEXP_WAREHOUSE_PROPERTY(int amount, Any* data, BOOL p2, BOOL p3);
		void NETWORK_SPENT_UPGRADE_IMPEXP_WAREHOUSE_PROPERTY(Any p0, Any p1, Any p2, Any p3);
		void NETWORK_SPENT_TRADE_IMPEXP_WAREHOUSE_PROPERTY(Any p0, Any p1, Any p2, Any p3);
		void NETWORK_SPENT_ORDER_WAREHOUSE_VEHICLE(Any p0, Any p1, Any p2, Any p3);
		void NETWORK_SPENT_ORDER_BODYGUARD_VEHICLE(Any p0, Any p1, Any p2, Any p3);
		void NETWORK_SPENT_JUKEBOX(Any p0, Any p1, Any p2, Any p3);
		void NETWORK_SPENT_PURCHASE_CLUB_HOUSE(Any p0, Any p1, Any p2, Any p3);
		void NETWORK_SPENT_UPGRADE_CLUB_HOUSE(Any p0, Any p1, Any p2, Any p3);
		void NETWORK_SPENT_PURCHASE_BUSINESS_PROPERTY(Any p0, Any p1, Any p2, Any p3);
		void NETWORK_SPENT_UPGRADE_BUSINESS_PROPERTY(Any p0, Any p1, Any p2, Any p3);
		void NETWORK_SPENT_TRADE_BUSINESS_PROPERTY(Any p0, Any p1, Any p2, Any p3);
		void NETWORK_SPENT_MC_ABILITY(Any p0, Any p1, Any p2, Any p3, Any p4);
		void NETWORK_SPENT_PAY_BUSINESS_SUPPLIES(Any p0, Any p1, Any p2, Any p3);
		void NETWORK_SPENT_CHANGE_APPEARANCE(Any p0, Any p1, Any p2);
		void NETWORK_SPENT_VEHICLE_EXPORT_MODS(Any p0, Any p1, Any p2, Any p3, Any p4, Any p5, Any p6, Any p7, Any p8, Any p9);
		void NETWORK_SPENT_PURCHASE_OFFICE_GARAGE(Any p0, Any p1, Any p2, Any p3);
		void NETWORK_SPENT_UPGRADE_OFFICE_GARAGE(Any p0, Any p1, Any p2, Any p3);
		void NETWORK_SPENT_IMPORT_EXPORT_REPAIR(Any p0, Any p1, Any p2);
		void NETWORK_SPENT_PURCHASE_HANGAR(Any p0, Any p1, Any p2, Any p3);
		void NETWORK_SPENT_UPGRADE_HANGAR(Any p0, Any p1, Any p2, Any p3);
		void NETWORK_SPENT_HANGAR_UTILITY_CHARGES(int amount, BOOL p1, BOOL p2);
		void NETWORK_SPENT_HANGAR_STAFF_CHARGES(int amount, BOOL p1, BOOL p2);
		void NETWORK_SPENT_BUY_TRUCK(Any p0, Any p1, Any p2, Any p3);
		void NETWORK_SPENT_UPGRADE_TRUCK(Any p0, Any p1, Any p2, Any p3);
		void NETWORK_SPENT_BUY_BUNKER(Any p0, Any p1, Any p2, Any p3);
		void NETWORK_SPENT_UPRADE_BUNKER(Any p0, Any p1, Any p2, Any p3);
		void NETWORK_EARN_FROM_SELL_BUNKER(int amount, Hash bunkerHash);
		void NETWORK_SPENT_BALLISTIC_EQUIPMENT(int amount, BOOL p1, BOOL p2);
		void NETWORK_EARN_RDR_BONUS(int amount, Any p1);
		void NETWORK_EARN_WAGE_PAYMENT(int amount, Any p1);
		void NETWORK_EARN_WAGE_PAYMENT_BONUS(int amount);
		void NETWORK_SPENT_BUY_BASE(Any p0, Any p1, Any p2, Any p3);
		void NETWORK_SPENT_UPGRADE_BASE(Any p0, Any p1, Any p2, Any p3);
		void NETWORK_SPENT_BUY_TILTROTOR(Any p0, Any p1, Any p2, Any p3);
		void NETWORK_SPENT_UPGRADE_TILTROTOR(Any p0, Any p1, Any p2, Any p3);
		void NETWORK_SPENT_EMPLOY_ASSASSINS(Any p0, Any p1, Any p2, Any p3);
		void NETWORK_SPEND_GANGOPS_CANNON(Any p0, Any p1, Any p2, Any p3);
		void NETWORK_SPEND_GANGOPS_SKIP_MISSION(Any p0, Any p1, Any p2, Any p3);
		void NETWORK_SPEND_CASINO_HEIST_SKIP_MISSION(Any p0, Any p1, Any p2, Any p3);
		void NETWORK_EARN_SELL_BASE(int amount, Hash baseNameHash);
		void NETWORK_EARN_TARGET_REFUND(int amount, int p1);
		void NETWORK_EARN_GANGOPS_WAGES(int amount, int p1);
		void NETWORK_EARN_GANGOPS_WAGES_BONUS(int amount, int p1);
		void NETWORK_EARN_DAR_CHALLENGE(int amount, Any p1);
		void NETWORK_EARN_DOOMSDAY_FINALE_BONUS(int amount, Hash vehicleHash);
		void NETWORK_EARN_GANGOPS_AWARD(int amount, const char* p1, Any p2);
		void NETWORK_EARN_GANGOPS_ELITE(int amount, const char* p1, int actIndex);
		void NETWORK_SERVICE_EARN_GANGOPS_RIVAL_DELIVERY(int earnedMoney);
		void NETWORK_SPEND_GANGOPS_START_STRAND(int type, int amount, BOOL p2, BOOL p3);
		void NETWORK_SPEND_GANGOPS_TRIP_SKIP(int amount, BOOL p1, BOOL p2);
		void NETWORK_EARN_GANGOPS_PREP_PARTICIPATION(int amount);
		void NETWORK_EARN_GANGOPS_SETUP(int amount, const char* p1);
		void NETWORK_EARN_GANGOPS_FINALE(int amount, const char* p1);
		void NETWORK_SPEND_GANGOPS_REPAIR_COST(Any p0, Any p1, Any p2);
		void NETWORK_EARN_NIGHTCLUB(Any p0, Any p1);
		void NETWORK_EARN_NIGHTCLUB_DANCING(Any p0);
		void NETWORK_EARN_BB_EVENT_BONUS(int amount);
		void NETWORK_SPENT_PURCHASE_HACKER_TRUCK(Any p0, Any p1, Any p2, Any p3);
		void NETWORK_SPENT_UPGRADE_HACKER_TRUCK(Any p0, Any p1, Any p2, Any p3);
		void NETWORK_EARN_HACKER_TRUCK(Any p0, int amount, Any p2, Any p3);
		void NETWORK_SPENT_PURCHASE_NIGHTCLUB_AND_WAREHOUSE(Any p0, Any p1, Any p2, Any p3);
		void NETWORK_SPENT_UPGRADE_NIGHTCLUB_AND_WAREHOUSE(Any p0, Any p1, Any p2, Any p3);
		void NETWORK_EARN_NIGHTCLUB_AND_WAREHOUSE(Any p0, Any p1, Any p2, Any p3, Any p4, Any p5, Any p6);
		void NETWORK_SPEND_NIGHTCLUB_AND_WAREHOUSE(Any p0, Any p1, Any p2, Any p3);
		void NETWORK_SPENT_RDR_HATCHET_BONUS(int amount, BOOL p1, BOOL p2);
		void NETWORK_SPENT_NIGHTCLUB_ENTRY_FEE(Player player, int amount, Any p1, BOOL p2, BOOL p3);
		void NETWORK_SPEND_NIGHTCLUB_BAR_DRINK(int amount, Any p1, BOOL p2, BOOL p3);
		void NETWORK_SPEND_BOUNTY_HUNTER_MISSION(int amount, BOOL p1, BOOL p2);
		void NETWORK_SPENT_REHIRE_DJ(int amount, Any p1, BOOL p2, BOOL p3);
		void NETWORK_SPENT_ARENA_JOIN_SPECTATOR(int amount, Any p1, BOOL p2, BOOL p3);
		void NETWORK_EARN_ARENA_SKILL_LEVEL_PROGRESSION(int amount, Any p1);
		void NETWORK_EARN_ARENA_CAREER_PROGRESSION(int amount, Any p1);
		void NETWORK_SPEND_MAKE_IT_RAIN(int amount, BOOL p1, BOOL p2);
		void NETWORK_SPEND_BUY_ARENA(int amount, BOOL p1, BOOL p2, const char* p3);
		void NETWORK_SPEND_UPGRADE_ARENA(int amount, BOOL p1, BOOL p2, const char* p3);
		/**
		 * type either, 1 for cam spectate, 2 for drone
		 */
		void NETWORK_SPEND_ARENA_SPECTATOR_BOX(int amount, int type, BOOL p2, BOOL p3);
		void NETWORK_SPEND_SPIN_THE_WHEEL_PAYMENT(int amount, Any p1, BOOL p2);
		void NETWORK_EARN_SPIN_THE_WHEEL_CASH(int amount);
		void NETWORK_SPEND_ARENA_PREMIUM(int amount, BOOL p1, BOOL p2);
		void NETWORK_EARN_ARENA_WAR(int amount, Any p1, Any p2, Any p3);
		void NETWORK_EARN_ARENA_WAR_ASSASSINATE_TARGET(int amount);
		void NETWORK_EARN_ARENA_WAR_EVENT_CARGO(int amount);
		void NETWORK_EARN_RC_TIME_TRIAL(int amount);
		void NETWORK_EARN_DAILY_OBJECTIVE_EVENT(int amount);
		void NETWORK_SPEND_CASINO_MEMBERSHIP(int amount, BOOL p1, BOOL p2, int p3);
		void NETWORK_SPEND_BUY_CASINO(int amount, BOOL p1, BOOL p2, Any* data);
		void NETWORK_SPEND_UPGRADE_CASINO(int amount, BOOL p1, BOOL p2, Any* data);
		void NETWORK_SPEND_CASINO_GENERIC(int amount, Any p1, Any p2, Any p3, Any p4);
		void NETWORK_EARN_CASINO_TIME_TRIAL_WIN(int amount);
		void NETWORK_EARN_COLLECTABLES_ACTION_FIGURES(int amount);
		void NETWORK_EARN_CASINO_COLLECTABLE_COMPLETED_COLLECTION(int amount);
		void NETWORK_EARN_SELL_PRIZE_VEHICLE(int amount, Any p1, Any p2);
		void NETWORK_EARN_CASINO_MISSION_REWARD(int amount);
		void NETWORK_EARN_CASINO_STORY_MISSION_REWARD(int amount);
		void NETWORK_EARN_CASINO_MISSION_PARTICIPATION(int amount);
		void NETWORK_EARN_CASINO_AWARD(int amount, Hash hash);
		void NETWORK_SPEND_BUY_ARCADE(Any p0, Any p1, Any p2, Any p3);
		void NETWORK_SPEND_UPGRADE_ARCADE(Any p0, Any p1, Any p2, Any p3);
		void NETWORK_SPEND_CASINO_HEIST(Any p0, Any p1, Any p2, Any p3, Any p4, Any p5, Any p6, Any p7, Any p8, Any p9, Any p10);
		void NETWORK_SPEND_ARCADE_MGMT(Any p0, Any p1, Any p2, Any p3, Any p4);
		void NETWORK_SPEND_PLAY_ARCADE(Any p0, Any p1, Any p2, Any p3, Any p4);
		void NETWORK_SPEND_ARCADE(Any p0, Any p1, Any p2, Any p3, Any p4);
		void NETWORK_EARN_CASINO_HEIST(Any p0, Any p1, Any p2, Any p3, Any p4, Any p5, Any p6);
		void NETWORK_EARN_UPGRADE_ARCADE(Any p0, Any p1, Any p2);
		void NETWORK_EARN_ARCADE(Any p0, Any p1, Any p2, Any p3, Any p4, Any p5);
		void NETWORK_EARN_COLLECTABLES(Any p0, Any p1, Any p2);
		void NETWORK_EARN_CHALLENGE(int amount, Any p1, Any p2);
		void NETWORK_EARN_CASINO_HEIST_AWARDS(Any p0, Any p1, Any p2, Any p3, Any p4);
		void NETWORK_EARN_COLLECTABLE_ITEM(int amount, Any p1);
		void NETWORK_EARN_COLLECTABLE_COMPLETED_COLLECTION(int amount, Any p1);
		void NETWORK_EARN_YATCH_MISSION(int amount, Any p1);
		void NETWORK_EARN_DISPATCH_CALL(int amount, Any p1);
		void NETWORK_SPEND_BEACH_PARTY(Any p0);
		void NETWORK_SPEND_SUBMARINE(Any p0, Any p1, Any p2, Any p3, Any p4, Any p5);
		void NETWORK_SPEND_CASINO_CLUB(int amount1, Any p1, BOOL p2, Any p3, int p4, int p5, int p6, int amount2, Any p8);
		void NETWORK_SPEND_BUY_SUB(Any p0, Any p1, Any p2, Any p3);
		void NETWORK_SPEND_UPGRADE_SUB(Any p0, Any p1, Any p2, Any p3);
		void NETWORK_SPEND_ISLAND_HEIST(Any p0, Any p1, Any p2, Any p3);
		void NETWORK_EARN_ISLAND_HEIST(int amount1, Any p1, Any p2, Any p3, int amount2, int p5);
		void NETWORK_EARN_BEACH_PARTY_LOST_FOUND(Any p0, Any p1, Any p2);
		void NETWORK_EARN_FROM_ISLAND_HEIST_DJ_MISSION(Any p0, Any p1);
		void NETWORK_SPEND_CAR_CLUB_MEMBERSHIP(int amount1, Any p1, Any p2, int amount2, Any p4);
		void NETWORK_SPEND_CAR_CLUB_BAR(Any p0, BOOL p1, BOOL p2, Any p3);
		void NETWORK_SPEND_AUTOSHOP_MODIFY(Any p0, Any p1, Any p2, Any p3, Any p4);
		void NETWORK_SPEND_CAR_CLUB_TAKEOVER(Any p0, Any p1, Any p2, Any p3);
		void NETWORK_SPEND_BUY_AUTOSHOP(Any p0, Any p1, Any p2, Any p3);
		void NETWORK_SPEND_UPGRADE_AUTOSHOP(Any p0, Any p1, Any p2, Any p3);
		void NETWORK_EARN_AUTOSHOP_BUSINESS(Any p0, Any p1, Any p2);
		void NETWORK_EARN_AUTOSHOP_INCOME(Any p0, int p1);
		void NETWORK_EARN_CARCLUB_MEMBERSHIP(Any p0);
		void NETWORK_EARN_DAILY_VEHICLE(Any p0, Any p1);
		void NETWORK_EARN_DAILY_VEHICLE_BONUS(Any p0);
		void NETWORK_EARN_TUNER_AWARD(Any p0, Any p1, Any p2);
		void NETWORK_EARN_TUNER_ROBBERY(Any p0, Any p1, Any p2, Any p3, Any p4);
		void NETWORK_EARN_UPGRADE_AUTOSHOP(Any p0, Any p1);
		void NETWORK_SPEND_INTERACTION_MENU_ABILITY(Any p0, Any p1, Any p2, Any p3);
		void NETWORK_SPEND_SET_COMMON_FIELDS(Any p0, Any p1, Any p2, BOOL p3);
		void NETWORK_SPEND_SET_DISCOUNT(BOOL p0);
		void NETWORK_SPEND_BUY_AGENCY(Any p0, Any p1, Any p2, Any p3);
		void NETWORK_SPEND_UPGRADE_AGENCY(Any p0, Any p1, Any p2, Any p3);
		void NETWORK_SPEND_AGENCY(Any p0, Any p1, Any p2, Any p3, Any p4);
		void NETWORK_SPEND_HIDDEN(Any p0, Any p1, Any p2, Any p3);
		void NETWORK_SPEND_SOURCE_BIKE(Any p0, Any p1, Any p2, Any p3);
		void NETWORK_SPEND_COMP_SUV(Any p0, Any p1, Any p2, Any p3);
		void NETWORK_SPEND_SUV_FST_TRVL(int p0, BOOL p1, BOOL p2, Any p3);
		void NETWORK_SPEND_SUPPLY(Any p0, Any p1, Any p2, Any p3);
		void NETWORK_SPEND_BIKE_SHOP(Any p0, Any p1, Any p2, Any p3);
		void NETWORK_SPEND_VEHICLE_REQUESTED(Any p0, Any p1, Any p2, Any p3, Any p4);
		void NETWORK_SPEND_GUNRUNNING(Any p0, Any p1, Any p2, Any p3);
		void NETWORK_EARN_AGENCY_SAFE(Any p0, Any p1);
		void NETWORK_EARN_AWARD_CONTRACT(Any p0, Any p1);
		void NETWORK_EARN_AGENCY_CONTRACT(Any p0, Any p1);
		void NETWORK_EARN_AWARD_PHONE(Any p0, Any p1);
		void NETWORK_EARN_AGENCY_PHONE(Any p0, Any p1, Any p2);
		void NETWORK_EARN_AWARD_FIXER_MISSION(Any p0, Any p1);
		void NETWORK_EARN_FIXER_PREP(Any p0, Any p1);
		void NETWORK_EARN_FIXER_FINALE(Any p0, Any p1);
		void NETWORK_EARN_FIXER_AGENCY_SHORT_TRIP(Any p0, Any p1);
		void NETWORK_EARN_AWARD_SHORT_TRIP(Any p0, Any p1);
		void NETWORK_EARN_FIXER_RIVAL_DELIVERY(Any p0, Any p1);
		void NETWORK_EARN_UPGRADE_AGENCY(Any p0, Any p1);
		void NETWORK_SPEND_APARTMENT_UTILITIES(int amount, BOOL p1, BOOL p2, Any* data);
		void NETWORK_SPEND_BUSINESS_PROPERTY_FEES(Any p0, Any p1, Any p2, Any p3);
		void NETWORK_EARN_SIGHTSEEING_REWARD(Any p0, Any p1, Any p2, Any p3);
		void NETWORK_EARN_BIKER_SHOP(Any p0, Any p1);
		void NETWORK_EARN_BIKER(Any p0);
		void NETWORK_YOHAN_SOURCE_GOODS(Any p0, Any p1, Any p2, Any p3);
		void _NETWORK_SPEND_BUY_MFGARAGE(Any p0, Any p1, Any p2, Any p3);
		void _NETWORK_SPEND_UPGRADE_MFGARAGE(Any p0, Any p1, Any p2, Any p3);
		void _NETWORK_SPEND_BUY_SUPPLIES(int p0, BOOL p1, BOOL p2, int p3);
		void _NETWORK_SPEND_BUY_ACID_LAB(Any p0, Any p1, Any p2, Any p3);
		void _NETWORK_SPEND_UPGRADE_ACID_LAB_EQUIPMENT(Any p0, Any p1, Any p2, Any p3);
		void _NETWORK_SPEND_UPGRADE_ACID_LAB_ARMOR(int p0, BOOL p1, BOOL p2, int p3);
		void _NETWORK_SPEND_UPGRADE_ACID_LAB_SCOOP(int p0, BOOL p1, BOOL p2, int p3);
		void _NETWORK_SPEND_UPGRADE_ACID_LAB_MINES(int p0, BOOL p1, BOOL p2, int p3);
		void _NETWORK_SPEND_RENAME_ACID_LAB(Any p0, Any p1, Any p2, Any p3);
		void _NETWORK_SPEND_RENAME_ACID_PRODUCT(Any p0, Any p1, Any p2, Any p3);
		void _NETWORK_EARN_AWARD_JUGGALO_MISSION(Any p0, Any p1);
		void _NETWORK_EARN_AWARD_ACID_LAB(Any p0, Any p1);
		void _NETWORK_EARN_AWARD_DAILY_STASH(Any p0, Any p1);
		void _NETWORK_EARN_AWARD_DEAD_DROP(Any p0, Any p1);
		void _NETWORK_EARN_AWARD_RANDOM_EVENT(Any p0, Any p1);
		void _NETWORK_EARN_AWARD_TAXI(Any p0, Any p1);
		void _NETWORK_EARN_STREET_DEALER(Any p0, Any p1);
		void _NETWORK_EARN_SELL_ACID(Any p0, Any p1);
		void _NETWORK_EARN_SETUP_PARTICIPATION_ACID_LAB(Any p0, Any p1);
		void _NETWORK_EARN_SOURCE_PARTICIPATION_ACID_LAB(Any p0, Any p1);
		void _NETWORK_EARN_SELL_PARTICIPATION_ACID_LAB(Any p0, Any p1);
		void _NETWORK_EARN_JUGGALO_STORY_MISSION(Any p0, Any p1);
		void _NETWORK_EARN_JUGGALO_STORY_MISSION_PARTICIPATION(Any p0, Any p1);
		/**
		 * JUGGALO_PHONE_MISSION...
		 */
		void _NETWORK_EARN_FOOLIGAN_JOB(Any p0, Any p1);
		/**
		 * JUGGALO_PHONE_MISSION_PARTICIPATION...
		 */
		void _NETWORK_EARN_FOOLIGAN_JOB_PARTICIPATION(Any p0, Any p1);
		void _NETWORK_EARN_TAXI_JOB(Any p0, Any p1);
		void _NETWORK_EARN_DAILY_STASH_HOUSE_COMPLETED(Any p0, Any p1);
		void _NETWORK_EARN_DAILY_STASH_HOUSE_PARTICIPATION(Any p0, Any p1);
		/**
		 * Used for SERVICE_EARN_AVENGER_OPERATIONS & SERVICE_EARN_AVENGER_OPS_BONUS
		 */
		void _NETWORK_EARN_AVENGER(int amount, int p1);
		void _NETWORK_EARN_SMUGGLER_OPS(Any p0, Any p1, Any p2);
		void _NETWORK_EARN_BONUS_OBJECTIVE(int amount, Any p1, Any p2);
		void _NETWORK_EARN_PROGRESS_HUB(Any p0, Any p1);
		void _NETWORK_SPENT_AIR_FREIGHT(int hangarCargoSourcingPrice, BOOL fromBank, BOOL fromBankAndWallet, int cost, int warehouseId, int warehouseSlot, Any p6);
		void _NETWORK_SPENT_SKIP_CARGO_SOURCE_SETUP(int amount, BOOL fromBank, BOOL fromBankAndWallet, int cost);
		/**
		 * Hash p3 = STEALTH_MODULE
		 */
		void _NETWORK_SPENT_STEALTH_MODULE(int amount, BOOL fromBank, BOOL fromBankAndWallet, Hash p3);
		/**
		 * Hash p3 = MISSILE_JAMMER
		 */
		void _NETWORK_SPENT_MISSILE_JAMMER(int amount, BOOL fromBank, BOOL fromBankAndWallet, Hash p3);
		void _NETWORK_SPENT_GENERIC(int price, BOOL p1, BOOL p2, Hash stat, Hash spent, const char* p5, const char* p6, Any* data);
		/**
		 * _NETWORK_EARN_G*
		 */
		void _NETWORK_EARN_GENERIC(int amount, Hash earn, const char* p2, const char* p3, Any* data);
		void _NETWORK_CLEAR_TRANSACTION_TELEMETRY_NONCE();
		int NETWORK_GET_VC_BANK_BALANCE();
		int NETWORK_GET_VC_WALLET_BALANCE(int characterSlot);
		int NETWORK_GET_VC_BALANCE();
		int NETWORK_GET_EVC_BALANCE();
		int NETWORK_GET_PVC_BALANCE();
		const char* NETWORK_GET_STRING_WALLET_BALANCE(int characterSlot);
		const char* NETWORK_GET_STRING_BANK_BALANCE();
		const char* NETWORK_GET_STRING_BANK_WALLET_BALANCE(int character);
		/**
		 * Returns true if wallet balance >= amount.
		 */
		BOOL NETWORK_GET_CAN_SPEND_FROM_WALLET(int amount, int characterSlot);
		/**
		 * Returns true if bank balance >= amount.
		 */
		BOOL NETWORK_GET_CAN_SPEND_FROM_BANK(int amount);
		/**
		 * Returns true if bank balance + wallet balance >= amount.
		 */
		BOOL NETWORK_GET_CAN_SPEND_FROM_BANK_AND_WALLET(int amount, int characterSlot);
		/**
		 * Retturns the same value as NETWORK_GET_REMAINING_TRANSFER_BALANCE.
		 */
		int NETWORK_GET_PVC_TRANSFER_BALANCE();
		/**
		 * Returns false if amount > wallet balance or daily transfer limit has been hit.
		 */
		BOOL NETWORK_GET_CAN_TRANSFER_CASH(int amount);
		BOOL NETWORK_CAN_RECEIVE_PLAYER_CASH(Any p0, Any p1, Any p2, Any p3);
		/**
		 * Returns the same value as NETWORK_GET_PVC_TRANSFER_BALANCE.
		 */
		int NETWORK_GET_REMAINING_TRANSFER_BALANCE();
		/**
		 * Does nothing and always returns 0.
		 */
		int WITHDRAW_VC(int amount);
		/**
		 * Does nothing and always returns false.
		 */
		BOOL DEPOSIT_VC(int amount);
		/**
		 * This function is hard-coded to always return 1.
		 */
		BOOL HAS_VC_WITHDRAWAL_COMPLETED(Any p0);
		/**
		 * This function is hard-coded to always return 1.
		 */
		BOOL WAS_VC_WITHDRAWAL_SUCCESSFUL(Any p0);
		void NETWORK_GET_MP_WINDFALL_AVAILABLE(); // Missing in crossmap

	} // namespace MONEY

	namespace NETSHOPPING {

		BOOL NET_GAMESERVER_USE_SERVER_TRANSACTIONS();
		BOOL NET_GAMESERVER_CATALOG_ITEM_IS_VALID(const char* name);
		BOOL NET_GAMESERVER_CATALOG_ITEM_KEY_IS_VALID(Hash hash);
		/**
		 * bool is always true in game scripts
		 */
		int NET_GAMESERVER_GET_PRICE(Hash itemHash, Hash categoryHash, BOOL p2);
		BOOL NET_GAMESERVER_CATALOG_IS_VALID();
		BOOL NET_GAMESERVER_IS_CATALOG_CURRENT();
		Hash NET_GAMESERVER_GET_CATALOG_CLOUD_CRC();
		BOOL NET_GAMESERVER_REFRESH_SERVER_CATALOG();
		BOOL NET_GAMESERVER_RETRIEVE_CATALOG_REFRESH_STATUS(int* state);
		BOOL NET_GAMESERVER_INIT_SESSION();
		BOOL NET_GAMESERVER_RETRIEVE_INIT_SESSION_STATUS(int* p0);
		BOOL NET_GAMESERVER_START_SESSION(int charSlot);
		BOOL NET_GAMESERVER_START_SESSION_PENDING();
		BOOL NET_GAMESERVER_RETRIEVE_START_SESSION_STATUS(int* p0);
		BOOL NET_GAMESERVER_RETRIEVE_SESSION_ERROR_CODE(int* p0);
		BOOL NET_GAMESERVER_IS_SESSION_VALID(int charSlot);
		BOOL NET_GAMESERVER_CLEAR_SESSION(int p0);
		BOOL NET_GAMESERVER_SESSION_APPLY_RECEIVED_DATA(int charSlot);
		BOOL NET_GAMESERVER_IS_SESSION_REFRESH_PENDING();
		/**
		 * Instructs the transaction session manager to perform a session restart as soon as possible.
		 * This is different from NET_GAMESERVER_START_SESSION_RESTART which, despite its name, does not actually restart the session (it only requests fresh inventory and/or balance data from the server).
		 * Returns true if it was possible to set the flag.
		 */
		void _NET_GAMESERVER_FLAG_FOR_SESSION_RESTART(); // Missing in crossmap
		/**
		 * Note: only one of the arguments can be set to true at a time
		 */
		BOOL NET_GAMESERVER_START_SESSION_RESTART(BOOL inventory, BOOL playerbalance);
		BOOL NET_GAMESERVER_TRANSACTION_IN_PROGRESS();
		BOOL NET_GAMESERVER_GET_SESSION_STATE_AND_STATUS(int* p0, BOOL* p1);
		BOOL NET_GAMESERVER_BASKET_START(int* transactionId, Hash categoryHash, Hash actionHash, int flags);
		BOOL NET_GAMESERVER_BASKET_END();
		BOOL NET_GAMESERVER_BASKET_IS_ACTIVE();
		BOOL NET_GAMESERVER_BASKET_ADD_ITEM(Any* itemData, int quantity);
		BOOL NET_GAMESERVER_BASKET_IS_FULL();
		BOOL NET_GAMESERVER_BASKET_APPLY_SERVER_DATA(Any p0, Any* p1);
		BOOL NET_GAMESERVER_CHECKOUT_START(int transactionId);
		void NET_GAMESERVER_CHECKOUT_PENDING(int transactionId); // Missing in crossmap
		BOOL NET_GAMESERVER_BEGIN_SERVICE(int* transactionId, Hash categoryHash, Hash itemHash, Hash actionTypeHash, int value, int flags);
		BOOL NET_GAMESERVER_END_SERVICE(int transactionId);
		BOOL NET_GAMESERVER_DELETE_CHARACTER(int slot, BOOL transfer, Hash reason);
		int NET_GAMESERVER_DELETE_CHARACTER_GET_STATUS();
		BOOL NET_GAMESERVER_DELETE_SET_TELEMETRY_NONCE_SEED();
		BOOL NET_GAMESERVER_TRANSFER_BANK_TO_WALLET(int charSlot, int amount);
		BOOL NET_GAMESERVER_TRANSFER_WALLET_TO_BANK(int charSlot, int amount);
		/**
		 * Same as 0x350AA5EBC03D3BD2
		 */
		int NET_GAMESERVER_TRANSFER_BANK_TO_WALLET_GET_STATUS();
		/**
		 * Same as 0x23789E777D14CE44
		 */
		int NET_GAMESERVER_TRANSFER_WALLET_TO_BANK_GET_STATUS();
		/**
		 * Used to be NETWORK_SHOP_CASH_TRANSFER_SET_TELEMETRY_NONCE_SEED
		 */
		BOOL NET_GAMESERVER_TRANSFER_CASH_SET_TELEMETRY_NONCE_SEED();
		BOOL NET_GAMESERVER_SET_TELEMETRY_NONCE_SEED(int p0);

	} // namespace NETSHOPPING

	namespace NETWORK {

		/**
		 * Online version is defined here: update\update.rpf\common\data\version.txt
		 * 
		 * Example:
		 * 
		 * [ONLINE_VERSION_NUMBER]
		 * 1.33
		 * 
		 * _GET_ONLINE_VERSION() will return "1.33"
		 */
		const char* GET_ONLINE_VERSION();
		/**
		 * Returns whether the player is signed into Social Club.
		 */
		BOOL NETWORK_IS_SIGNED_IN();
		/**
		 * Returns whether the game is not in offline mode.
		 * 
		 * seemed not to work for some ppl
		 */
		BOOL NETWORK_IS_SIGNED_ONLINE();
		/**
		 * This function is hard-coded to always return 1.
		 */
		BOOL NETWORK_IS_NP_AVAILABLE();
		/**
		 * This function is hard-coded to always return 1.
		 */
		BOOL NETWORK_IS_NP_PENDING();
		/**
		 * Hardcoded to return zero.
		 * 
		 * ==== PS4 specific info ====
		 * 
		 * Returns some sort of unavailable reason:
		 * -1 = REASON_INVALID
		 * 0 = REASON_OTHER
		 * 1 = REASON_SYSTEM_UPDATE
		 * 2 = REASON_GAME_UPDATE
		 * 3 = REASON_SIGNED_OUT
		 * 4 = REASON_AGE
		 * 5 = REASON_CONNECTION
		 * 
		 * =================================
		 */
		int NETWORK_GET_NP_UNAVAILABLE_REASON();
		/**
		 * This function is hard-coded to always return 1.
		 */
		BOOL NETWORK_IS_CONNETED_TO_NP_PRESENCE();
		/**
		 * This function is hard-coded to always return 0.
		 */
		BOOL NETWORK_IS_LOGGED_IN_TO_PSN();
		/**
		 * Returns whether the signed-in user has valid Rockstar Online Services (ROS) credentials.
		 */
		BOOL NETWORK_HAS_VALID_ROS_CREDENTIALS();
		BOOL NETWORK_IS_REFRESHING_ROS_CREDENTIALS();
		BOOL NETWORK_IS_CLOUD_AVAILABLE();
		BOOL NETWORK_HAS_SOCIAL_CLUB_ACCOUNT();
		BOOL NETWORK_ARE_SOCIAL_CLUB_POLICIES_CURRENT();
		/**
		 * If you are host, returns true else returns false.
		 */
		BOOL NETWORK_IS_HOST();
		Player NETWORK_GET_HOST_PLAYER_INDEX();
		BOOL NETWORK_WAS_GAME_SUSPENDED();
		BOOL NETWORK_HAVE_ONLINE_PRIVILEGES();
		/**
		 * For the XboxPC version this returns true if XPRIVILEGE_MULTIPLAYER_SESSIONS is granted but XPRIVILEGE_CROSS_PLAY is NOT granted.
		 * For more information, see https://learn.microsoft.com/en-us/gaming/gdk/_content/gc/live/features/identity/privileges/concepts/live-user-privileges-client#privilege-ids-to-check
		 * 
		 * Always returns false for non-XboxPC versions.
		 */
		void _NETWORK_MULTIPLAYER_CROSSPLAY_NOT_ALLOWED(); // Missing in crossmap
		BOOL NETWORK_HAS_AGE_RESTRICTIONS();
		BOOL NETWORK_HAVE_USER_CONTENT_PRIVILEGES(int p0);
		BOOL NETWORK_HAVE_COMMUNICATION_PRIVILEGES(int p0, Player player);
		/**
		 * Appears to be PlayStation-specific. Always returns true on other platforms if signed in with the primary user profile
		 */
		BOOL _NETWORK_HAVE_PLATFORM_COMMUNICATION_PRIVILEGES();
		BOOL NETWORK_CHECK_ONLINE_PRIVILEGES(Any p0, BOOL p1);
		BOOL NETWORK_CHECK_USER_CONTENT_PRIVILEGES(int p0, int p1, BOOL p2);
		BOOL NETWORK_CHECK_COMMUNICATION_PRIVILEGES(int p0, int p1, BOOL p2);
		BOOL NETWORK_CHECK_TEXT_COMMUNICATION_PRIVILEGES(Any p0, Any p1, Any p2);
		BOOL NETWORK_IS_USING_ONLINE_PROMOTION();
		BOOL NETWORK_SHOULD_SHOW_PROMOTION_ALERT_SCREEN();
		BOOL NETWORK_HAS_SOCIAL_NETWORKING_SHARING_PRIV();
		int NETWORK_GET_AGE_GROUP();
		BOOL NETWORK_CHECK_PRIVILEGES(Any p0, Any p1, Any p2);
		/**
		 * Hardcoded to return false.
		 */
		BOOL NETWORK_IS_PRIVILEGE_CHECK_IN_PROGRESS();
		void NETWORK_SET_PRIVILEGE_CHECK_RESULT_NOT_NEEDED();
		/**
		 * Hardcoded to return true.
		 */
		BOOL NETWORK_RESOLVE_PRIVILEGE_USER_CONTENT();
		BOOL NETWORK_HAVE_PLATFORM_SUBSCRIPTION();
		BOOL NETWORK_IS_PLATFORM_SUBSCRIPTION_CHECK_PENDING();
		void NETWORK_SHOW_ACCOUNT_UPGRADE_UI();
		BOOL NETWORK_IS_SHOWING_SYSTEM_UI_OR_RECENTLY_REQUESTED_UPSELL();
		BOOL NETWORK_NEED_TO_START_NEW_GAME_BUT_BLOCKED();
		BOOL NETWORK_CAN_BAIL();
		void NETWORK_BAIL(int p0, int p1, int p2);
		void NETWORK_ON_RETURN_TO_SINGLE_PLAYER();
		BOOL NETWORK_TRANSITION_START(int p0, Any p1, Any p2, Any p3);
		BOOL NETWORK_TRANSITION_ADD_STAGE(Hash hash, int p1, int p2, int state, int p4);
		BOOL NETWORK_TRANSITION_FINISH(Any p0, Any p1, Any p2);
		/**
		 * 11 - Need to download tunables.
		 * 12 - Need to download background script.
		 * 
		 * Returns 1 if the multiplayer is loaded, otherwhise 0.
		 */
		BOOL NETWORK_CAN_ACCESS_MULTIPLAYER(int* loadingState);
		void NETWORK_CHECK_CAN_ACCESS_AND_ALERT(); // Missing in crossmap
		void _NETWORK_GET_ACCESS_CODE_LABEL_HEADING(int accessCode); // Missing in crossmap
		void _NETWORK_GET_ACCESS_CODE_LABEL_BODY(int accessCode); // Missing in crossmap
		BOOL NETWORK_IS_MULTIPLAYER_DISABLED();
		BOOL NETWORK_CAN_ENTER_MULTIPLAYER();
		BOOL NETWORK_SESSION_DO_FREEROAM_QUICKMATCH(Any p0, Any p1, Any p2);
		BOOL NETWORK_SESSION_DO_FRIEND_MATCHMAKING(int p0, int p1, int p2);
		/**
		 * p4 seems to be unused in 1.60/build 2628
		 */
		BOOL NETWORK_SESSION_DO_CREW_MATCHMAKING(int crewId, int p1, int p2, int maxPlayers);
		BOOL NETWORK_SESSION_DO_ACTIVITY_QUICKMATCH(Any p0, Any p1, Any p2, Any p3, Any p4);
		/**
		 * Does nothing in online but in offline it will cause the screen to fade to black. Nothing happens past then, the screen will sit at black until you restart GTA. Other stuff must be needed to actually host a session.
		 */
		BOOL NETWORK_SESSION_HOST(int p0, int maxPlayers, BOOL p2);
		BOOL NETWORK_SESSION_HOST_CLOSED(int p0, int maxPlayers);
		/**
		 * Does nothing in online but in offline it will cause the screen to fade to black. Nothing happens past then, the screen will sit at black until you restart GTA. Other stuff must be needed to actually host a session.
		 */
		BOOL NETWORK_SESSION_HOST_FRIENDS_ONLY(int p0, int maxPlayers);
		BOOL NETWORK_SESSION_IS_CLOSED_FRIENDS();
		BOOL NETWORK_SESSION_IS_CLOSED_CREW();
		BOOL NETWORK_SESSION_IS_SOLO();
		BOOL NETWORK_SESSION_IS_PRIVATE();
		BOOL _NETWORK_SESSION_LEAVE_INCLUDING_REASON(int leaveFlags, int leaveReason);
		BOOL NETWORK_SESSION_LEAVE(Any p0);
		/**
		 * Only works as host.
		 */
		void NETWORK_SESSION_KICK_PLAYER(Player player);
		BOOL NETWORK_SESSION_GET_KICK_VOTE(Player player);
		BOOL NETWORK_SESSION_RESERVE_SLOTS_TRANSITION(Any p0, Any p1, Any p2);
		BOOL NETWORK_JOIN_PREVIOUSLY_FAILED_SESSION();
		BOOL NETWORK_JOIN_PREVIOUSLY_FAILED_TRANSITION();
		void NETWORK_SESSION_SET_MATCHMAKING_GROUP(int matchmakingGroup);
		/**
		 * playerType is an unsigned int from 0 to 4
		 * 0 = regular joiner
		 * 4 = spectator
		 */
		void NETWORK_SESSION_SET_MATCHMAKING_GROUP_MAX(int playerType, int playerCount);
		int NETWORK_SESSION_GET_MATCHMAKING_GROUP_FREE(int p0);
		/**
		 * groupId range: [0, 4]
		 */
		void NETWORK_SESSION_ADD_ACTIVE_MATCHMAKING_GROUP(int groupId);
		void NETWORK_SESSION_SET_UNIQUE_CREW_LIMIT(Any p0);
		int NETWORK_SESSION_GET_UNIQUE_CREW_LIMIT();
		void NETWORK_SESSION_SET_UNIQUE_CREW_LIMIT_TRANSITION(Any p0);
		void NETWORK_SESSION_SET_UNIQUE_CREW_ONLY_CREWS_TRANSITION(BOOL p0);
		void NETWORK_SESSION_SET_CREW_LIMIT_MAX_MEMBERS_TRANSITION(Any p0);
		void NETWORK_SESSION_SET_MATCHMAKING_PROPERTY_ID(BOOL p0);
		/**
		 * p0 in the decompiled scripts is always the stat mesh_texblend * 0.07 to int
		 */
		void NETWORK_SESSION_SET_MATCHMAKING_MENTAL_STATE(int p0);
		void NETWORK_SESSION_SET_NUM_BOSSES(int num);
		void NETWORK_SESSION_SET_SCRIPT_VALIDATE_JOIN();
		void NETWORK_SESSION_VALIDATE_JOIN(BOOL p0);
		/**
		 * ..
		 */
		void NETWORK_ADD_FOLLOWERS(int* p0, int p1);
		void NETWORK_CLEAR_FOLLOWERS();
		void NETWORK_GET_GLOBAL_MULTIPLAYER_CLOCK(int* hours, int* minutes, int* seconds);
		void NETWORK_SESSION_SET_GAMEMODE(Any p0);
		int NETWORK_SESSION_GET_HOST_AIM_PREFERENCE();
		BOOL NETWORK_FIND_GAMERS_IN_CREW(int crewId);
		/**
		 * Uses attributes to find players with similar stats. Upper/Lower limit must be above zero or the fallback limit +/-0.1 is used.
		 * There can be up to 15 attributes, they are as follows:
		 * 
		 * 0 = Races
		 * 1 = Parachuting
		 * 2 = Horde
		 * 3 = Darts
		 * 4 = Arm Wrestling
		 * 5 = Tennis
		 * 6 = Golf
		 * 7 = Shooting Range
		 * 8 = Deathmatch
		 * 9 = MPPLY_MCMWIN/MPPLY_CRMISSION
		 */
		BOOL NETWORK_FIND_MATCHED_GAMERS(int attribute, float fallbackLimit, float lowerLimit, float upperLimit);
		BOOL NETWORK_IS_FINDING_GAMERS();
		BOOL NETWORK_DID_FIND_GAMERS_SUCCEED();
		int NETWORK_GET_NUM_FOUND_GAMERS();
		BOOL NETWORK_GET_FOUND_GAMER(Any* p0, Any p1);
		void NETWORK_CLEAR_FOUND_GAMERS();
		BOOL NETWORK_QUEUE_GAMER_FOR_STATUS(Any* p0);
		BOOL NETWORK_GET_GAMER_STATUS_FROM_QUEUE();
		BOOL NETWORK_IS_GETTING_GAMER_STATUS();
		BOOL NETWORK_DID_GET_GAMER_STATUS_SUCCEED();
		BOOL NETWORK_GET_GAMER_STATUS_RESULT(Any* p0, Any p1);
		void NETWORK_CLEAR_GET_GAMER_STATUS();
		void NETWORK_SESSION_JOIN_INVITE();
		void NETWORK_SESSION_CANCEL_INVITE();
		void NETWORK_SESSION_FORCE_CANCEL_INVITE();
		BOOL NETWORK_HAS_PENDING_INVITE();
		BOOL NETWORK_HAS_CONFIRMED_INVITE();
		/**
		 * Triggers a CEventNetworkInviteConfirmed event
		 */
		BOOL NETWORK_REQUEST_INVITE_CONFIRMED_EVENT();
		BOOL NETWORK_SESSION_WAS_INVITED();
		void NETWORK_SESSION_GET_INVITER(Any* gamerHandle);
		/**
		 * Seems to be true while "Getting GTA Online session details" shows up.
		 */
		BOOL NETWORK_SESSION_IS_AWAITING_INVITE_RESPONSE();
		BOOL NETWORK_SESSION_IS_DISPLAYING_INVITE_CONFIRMATION();
		void NETWORK_SUPPRESS_INVITE(BOOL toggle);
		void NETWORK_BLOCK_INVITES(BOOL toggle);
		void NETWORK_BLOCK_JOIN_QUEUE_INVITES(BOOL toggle);
		void NETWORK_SET_CAN_RECEIVE_RS_INVITES(BOOL p0);
		void NETWORK_STORE_INVITE_THROUGH_RESTART();
		void NETWORK_ALLOW_INVITE_PROCESS_IN_PLAYER_SWITCH(BOOL p0);
		void NETWORK_SET_SCRIPT_READY_FOR_EVENTS(BOOL toggle);
		BOOL NETWORK_IS_OFFLINE_INVITE_PENDING();
		void NETWORK_CLEAR_OFFLINE_INVITE_PENDING();
		/**
		 * Retrieves the failed invite join alert reason
		 */
		void _NETWORK_INVITE_GET_JOIN_FAIL_REASON(); // Missing in crossmap
		/**
		 * Clears the failed invite join alert reason
		 */
		void _NETWORK_INVITE_CLEAR_JOIN_FAIL_REASON(); // Missing in crossmap
		/**
		 * Loads up the map that is loaded when beeing in mission creator
		 * Player gets placed in a mix between online/offline mode
		 * p0 is always 2 in R* scripts.
		 * 
		 * Appears to be patched in gtav b757 (game gets terminated) alonside with most other network natives to prevent online modding ~ghost30812
		 */
		void NETWORK_SESSION_HOST_SINGLE_PLAYER(int p0);
		void NETWORK_SESSION_LEAVE_SINGLE_PLAYER();
		BOOL NETWORK_IS_GAME_IN_PROGRESS();
		BOOL NETWORK_IS_SESSION_ACTIVE();
		BOOL NETWORK_IS_IN_SESSION();
		/**
		 * Hardcoded to return 0.
		 */
		BOOL _NETWORK_IS_AMERICAS_VERSION();
		/**
		 * This checks if player is playing on gta online or not.
		 * Please add an if and block your mod if this is "true".
		 */
		BOOL NETWORK_IS_SESSION_STARTED();
		BOOL NETWORK_IS_SESSION_BUSY();
		BOOL NETWORK_CAN_SESSION_END();
		int NETWORK_GET_GAME_MODE();
		void NETWORK_SESSION_MARK_VISIBLE(BOOL toggle);
		BOOL NETWORK_SESSION_IS_VISIBLE();
		void NETWORK_SESSION_BLOCK_JOIN_REQUESTS(BOOL toggle);
		/**
		 * num player slots allowed in session, seems to work? 32 max
		 */
		void NETWORK_SESSION_CHANGE_SLOTS(int slots, BOOL p1);
		int NETWORK_SESSION_GET_PRIVATE_SLOTS();
		BOOL NETWORK_SESSION_VOICE_HOST();
		BOOL NETWORK_SESSION_VOICE_LEAVE();
		void NETWORK_SESSION_VOICE_CONNECT_TO_PLAYER(Any* gamerHandle);
		void NETWORK_SESSION_VOICE_RESPOND_TO_REQUEST(BOOL p0, int p1);
		void NETWORK_SESSION_VOICE_SET_TIMEOUT(int timeout);
		BOOL NETWORK_SESSION_IS_IN_VOICE_SESSION();
		BOOL NETWORK_SESSION_IS_VOICE_SESSION_ACTIVE();
		BOOL NETWORK_SESSION_IS_VOICE_SESSION_BUSY();
		/**
		 * Message is limited to 64 characters.
		 */
		BOOL NETWORK_SEND_TEXT_MESSAGE(const char* message, Any* gamerHandle);
		void NETWORK_SET_ACTIVITY_SPECTATOR(BOOL toggle);
		BOOL NETWORK_IS_ACTIVITY_SPECTATOR();
		void NETWORK_SET_ACTIVITY_PLAYER_MAX(Any p0);
		void NETWORK_SET_ACTIVITY_SPECTATOR_MAX(int maxSpectators);
		int NETWORK_GET_ACTIVITY_PLAYER_NUM(BOOL p0);
		BOOL NETWORK_IS_ACTIVITY_SPECTATOR_FROM_HANDLE(Any* gamerHandle);
		/**
		 * p0: Unknown int
		 * p1: Unknown int
		 * p2: Unknown int
		 * p3: Unknown int
		 * p4: Unknown always 0 in decompiled scripts
		 * p5: BOOL purpose unknown, both 0 and 1 are used in decompiled scripts.
		 * p6: BOOL purpose unknown, both 0 and 1 are used in decompiled scripts.
		 * p7: Unknown int, it's an int according to decompiled scripts, however the value is always 0 or 1.
		 * p8: Unknown int, it's an int according to decompiled scripts, however the value is always 0 or 1.
		 * p9: Unknown int, sometimes 0, but also 32768 or 16384 appear in decompiled scripst, maybe a flag of some sort?
		 * 
		 * From what I can tell it looks like it does the following:
		 * Creates/hosts a new transition to another online session, using this in FiveM will result in other players being disconencted from the server/preventing them from joining. This is most likely because I entered the wrong session parameters since they're pretty much all unknown right now.
		 * You also need to use `NetworkJoinTransition(Player player)` and `NetworkLaunchTransition()`.
		 */
		BOOL NETWORK_HOST_TRANSITION(int p0, int p1, int p2, int p3, Any p4, BOOL p5, BOOL p6, int p7, Any p8, int p9);
		BOOL NETWORK_DO_TRANSITION_QUICKMATCH(Any p0, Any p1, Any p2, Any p3, Any p4, Any p5);
		BOOL NETWORK_DO_TRANSITION_QUICKMATCH_ASYNC(Any p0, Any p1, Any p2, Any p3, Any p4, Any p5);
		BOOL NETWORK_DO_TRANSITION_QUICKMATCH_WITH_GROUP(Any p0, Any p1, Any p2, Any p3, Any* p4, Any p5, Any p6, Any p7);
		BOOL NETWORK_JOIN_GROUP_ACTIVITY();
		void NETWORK_CLEAR_GROUP_ACTIVITY();
		void NETWORK_RETAIN_ACTIVITY_GROUP();
		BOOL NETWORK_IS_TRANSITION_CLOSED_FRIENDS();
		BOOL NETWORK_IS_TRANSITION_CLOSED_CREW();
		BOOL NETWORK_IS_TRANSITION_SOLO();
		BOOL NETWORK_IS_TRANSITION_PRIVATE();
		int NETWORK_GET_NUM_TRANSITION_NON_ASYNC_GAMERS();
		void NETWORK_MARK_AS_PREFERRED_ACTIVITY(BOOL p0);
		void NETWORK_MARK_AS_WAITING_ASYNC(BOOL p0);
		void NETWORK_SET_IN_PROGRESS_FINISH_TIME(Any p0);
		void NETWORK_SET_TRANSITION_CREATOR_HANDLE(Any* p0);
		void NETWORK_CLEAR_TRANSITION_CREATOR_HANDLE();
		BOOL NETWORK_INVITE_GAMERS_TO_TRANSITION(Any* p0, Any p1);
		void NETWORK_SET_GAMER_INVITED_TO_TRANSITION(Any* gamerHandle);
		BOOL NETWORK_LEAVE_TRANSITION();
		BOOL NETWORK_LAUNCH_TRANSITION();
		/**
		 * Appears to set whether a transition should be started when the session is migrating.
		 */
		void NETWORK_SET_DO_NOT_LAUNCH_FROM_JOIN_AS_MIGRATED_HOST(BOOL toggle);
		void NETWORK_CANCEL_TRANSITION_MATCHMAKING();
		void NETWORK_BAIL_TRANSITION(int p0, int p1, int p2);
		BOOL NETWORK_DO_TRANSITION_TO_GAME(BOOL p0, int maxPlayers);
		BOOL NETWORK_DO_TRANSITION_TO_NEW_GAME(BOOL p0, int maxPlayers, BOOL p2);
		/**
		 * p2 is true 3/4 of the occurrences I found.
		 * 'players' is the number of players for a session. On PS3/360 it's always 18. On PC it's 32.
		 */
		BOOL NETWORK_DO_TRANSITION_TO_FREEMODE(Any* p0, Any p1, BOOL p2, int players, BOOL p4);
		BOOL NETWORK_DO_TRANSITION_TO_NEW_FREEMODE(Any* p0, Any p1, int players, BOOL p3, BOOL p4, BOOL p5);
		BOOL NETWORK_IS_TRANSITION_TO_GAME();
		/**
		 * Returns count.
		 */
		int NETWORK_GET_TRANSITION_MEMBERS(Any* data, int dataCount);
		void NETWORK_APPLY_TRANSITION_PARAMETER(int p0, int p1);
		void NETWORK_APPLY_TRANSITION_PARAMETER_STRING(int p0, const char* string, BOOL p2);
		BOOL NETWORK_SEND_TRANSITION_GAMER_INSTRUCTION(Any* gamerHandle, const char* p1, int p2, int p3, BOOL p4);
		BOOL NETWORK_MARK_TRANSITION_GAMER_AS_FULLY_JOINED(Any* p0);
		BOOL NETWORK_IS_TRANSITION_HOST();
		BOOL NETWORK_IS_TRANSITION_HOST_FROM_HANDLE(Any* gamerHandle);
		BOOL NETWORK_GET_TRANSITION_HOST(Any* gamerHandle);
		BOOL NETWORK_IS_IN_TRANSITION();
		BOOL NETWORK_IS_TRANSITION_STARTED();
		BOOL NETWORK_IS_TRANSITION_BUSY();
		BOOL NETWORK_IS_TRANSITION_MATCHMAKING();
		BOOL NETWORK_IS_TRANSITION_LEAVE_POSTPONED();
		void NETWORK_TRANSITION_SET_IN_PROGRESS(Any p0);
		void NETWORK_TRANSITION_SET_CONTENT_CREATOR(Any p0);
		void NETWORK_TRANSITION_SET_ACTIVITY_ISLAND(Any p0);
		void NETWORK_OPEN_TRANSITION_MATCHMAKING();
		void NETWORK_CLOSE_TRANSITION_MATCHMAKING();
		BOOL NETWORK_IS_TRANSITION_OPEN_TO_MATCHMAKING();
		void NETWORK_SET_TRANSITION_VISIBILITY_LOCK(BOOL p0, BOOL p1);
		BOOL NETWORK_IS_TRANSITION_VISIBILITY_LOCKED();
		void NETWORK_SET_TRANSITION_ACTIVITY_ID(Any p0);
		void NETWORK_CHANGE_TRANSITION_SLOTS(Any p0, BOOL p1);
		void NETWORK_TRANSITION_BLOCK_JOIN_REQUESTS(BOOL p0);
		BOOL NETWORK_HAS_PLAYER_STARTED_TRANSITION(Player player);
		BOOL NETWORK_ARE_TRANSITION_DETAILS_VALID(Any p0);
		/**
		 * int handle[76];
		 * NETWORK_HANDLE_FROM_FRIEND(iSelectedPlayer, &handle[0], 13);
		 * Player uVar2 = NETWORK_GET_PLAYER_FROM_GAMER_HANDLE(&handle[0]);
		 * NETWORK_JOIN_TRANSITION(uVar2);
		 * nothing doin.
		 */
		BOOL NETWORK_JOIN_TRANSITION(Player player);
		BOOL NETWORK_HAS_INVITED_GAMER_TO_TRANSITION(Any* p0);
		BOOL NETWORK_HAS_TRANSITION_INVITE_BEEN_ACKED(Any* p0);
		BOOL NETWORK_IS_ACTIVITY_SESSION();
		void NETWORK_DISABLE_REALTIME_MULTIPLAYER();
		void NETWORK_OVERRIDE_REALTIME_MULTIPLAYER_CONTROL_DISABLE(); // Missing in crossmap
		void NETWORK_DISABLE_REALTIME_MULTIPLAYER_SPECTATOR(); // Missing in crossmap
		/**
		 * Does nothing. It's just a nullsub.
		 */
		void NETWORK_SET_PRESENCE_SESSION_INVITES_BLOCKED(BOOL toggle);
		BOOL NETWORK_SEND_INVITE_VIA_PRESENCE(Any* gamerHandle, const char* p1, int dataCount, int p3);
		BOOL NETWORK_SEND_TRANSITION_INVITE_VIA_PRESENCE(Any* gamerHandle, const char* p1, int dataCount, int p3);
		/**
		 * Contains the string "NETWORK_SEND_PRESENCE_TRANSITION_INVITE" but so does 0xC116FF9B4D488291; seems to fit alphabetically here, tho.
		 */
		BOOL NETWORK_SEND_IMPORTANT_TRANSITION_INVITE_VIA_PRESENCE(Any* gamerHandle, const char* p1, int dataCount, int p3);
		int NETWORK_GET_PRESENCE_INVITE_INDEX_BY_ID(int p0);
		int NETWORK_GET_NUM_PRESENCE_INVITES();
		BOOL NETWORK_ACCEPT_PRESENCE_INVITE(int p0);
		BOOL NETWORK_REMOVE_PRESENCE_INVITE(int p0);
		int NETWORK_GET_PRESENCE_INVITE_ID(int p0);
		const char* NETWORK_GET_PRESENCE_INVITE_INVITER(int p0);
		BOOL NETWORK_GET_PRESENCE_INVITE_HANDLE(Any p0, Any* p1);
		int NETWORK_GET_PRESENCE_INVITE_SESSION_ID(Any p0);
		const char* NETWORK_GET_PRESENCE_INVITE_CONTENT_ID(int p0);
		int NETWORK_GET_PRESENCE_INVITE_PLAYLIST_LENGTH(int p0);
		int NETWORK_GET_PRESENCE_INVITE_PLAYLIST_CURRENT(int p0);
		BOOL NETWORK_GET_PRESENCE_INVITE_FROM_ADMIN(int p0);
		BOOL NETWORK_GET_PRESENCE_INVITE_IS_TOURNAMENT(Any p0);
		BOOL NETWORK_HAS_FOLLOW_INVITE();
		BOOL NETWORK_ACTION_FOLLOW_INVITE();
		BOOL NETWORK_CLEAR_FOLLOW_INVITE();
		void NETWORK_REMOVE_AND_CANCEL_ALL_INVITES();
		void NETWORK_REMOVE_TRANSITION_INVITE(Any* p0);
		void NETWORK_REMOVE_ALL_TRANSITION_INVITE();
		void NETWORK_REMOVE_AND_CANCEL_ALL_TRANSITION_INVITES();
		BOOL NETWORK_INVITE_GAMERS(Any* p0, Any p1, Any* p2, Any p3);
		BOOL NETWORK_HAS_INVITED_GAMER(Any* p0);
		BOOL NETWORK_HAS_MADE_INVITE_DECISION(Any* gamerHandle);
		int NETWORK_GET_INVITE_REPLY_STATUS(Any p0);
		BOOL NETWORK_GET_CURRENTLY_SELECTED_GAMER_HANDLE_FROM_INVITE_MENU(Any* p0);
		BOOL NETWORK_SET_CURRENTLY_SELECTED_GAMER_HANDLE_FROM_INVITE_MENU(Any* p0);
		void NETWORK_SET_INVITE_ON_CALL_FOR_INVITE_MENU(Any* p0);
		BOOL NETWORK_CHECK_DATA_MANAGER_SUCCEEDED_FOR_HANDLE(int p0, Any* gamerHandle);
		BOOL NETWORK_CHECK_DATA_MANAGER_FOR_HANDLE(Any p0, Any* gamerHandle);
		void NETWORK_SET_INVITE_FAILED_MESSAGE_FOR_INVITE_MENU(Any* p0, Any* p1);
		BOOL FILLOUT_PM_PLAYER_LIST(Any* gamerHandle, Any p1, Any p2);
		BOOL FILLOUT_PM_PLAYER_LIST_WITH_NAMES(Any* p0, Any* p1, Any p2, Any p3);
		BOOL REFRESH_PLAYER_LIST_STATS(int p0);
		BOOL NETWORK_SET_CURRENT_DATA_MANAGER_HANDLE(Any* p0);
		/**
		 * Hardcoded to return false.
		 */
		BOOL NETWORK_IS_IN_PLATFORM_PARTY();
		int NETWORK_GET_PLATFORM_PARTY_MEMBER_COUNT();
		int NETWORK_GET_PLATFORM_PARTY_MEMBERS(Any* data, int dataSize);
		/**
		 * Hardcoded to return false.
		 */
		BOOL NETWORK_IS_IN_PLATFORM_PARTY_CHAT();
		/**
		 * This would be nice to see if someone is in party chat, but 2 sad notes.
		 * 1) It only becomes true if said person is speaking in that party at the time.
		 * 2) It will never, become true unless you are in that party with said person.
		 */
		BOOL NETWORK_IS_CHATTING_IN_PLATFORM_PARTY(Any* gamerHandle);
		BOOL NETWORK_CAN_QUEUE_FOR_PREVIOUS_SESSION_JOIN();
		BOOL NETWORK_IS_QUEUING_FOR_SESSION_JOIN();
		void NETWORK_CLEAR_QUEUED_JOIN_REQUEST();
		void NETWORK_SEND_QUEUED_JOIN_REQUEST();
		void NETWORK_REMOVE_ALL_QUEUED_JOIN_REQUESTS();
		void NETWORK_SEED_RANDOM_NUMBER_GENERATOR(int seed);
		int NETWORK_GET_RANDOM_INT();
		/**
		 * Same as GET_RANDOM_INT_IN_RANGE
		 */
		int NETWORK_GET_RANDOM_INT_RANGED(int rangeStart, int rangeEnd);
		float _NETWORK_GET_RANDOM_FLOAT_RANGED(float rangeStart, float rangeEnd);
		BOOL NETWORK_PLAYER_IS_CHEATER();
		int NETWORK_PLAYER_GET_CHEATER_REASON();
		BOOL NETWORK_PLAYER_IS_BADSPORT();
		BOOL REMOTE_CHEATER_PLAYER_DETECTED(Player player, int a, int b);
		BOOL BAD_SPORT_PLAYER_LEFT_DETECTED(Any* gamerHandle, int event, int amountReceived);
		void NETWORK_ADD_INVALID_OBJECT_MODEL(Hash modelHash, Any p1);
		void NETWORK_REMOVE_INVALID_OBJECT_MODEL(Hash modelHash);
		void NETWORK_CLEAR_INVALID_OBJECT_MODELS();
		void NETWORK_APPLY_PED_SCAR_DATA(Ped ped, int p1);
		void NETWORK_SET_THIS_SCRIPT_IS_NETWORK_SCRIPT(int maxNumMissionParticipants, BOOL p1, int instanceId);
		BOOL NETWORK_TRY_TO_SET_THIS_SCRIPT_IS_NETWORK_SCRIPT(Any p0, BOOL p1, Any p2);
		BOOL NETWORK_GET_THIS_SCRIPT_IS_NETWORK_SCRIPT();
		int NETWORK_GET_MAX_NUM_PARTICIPANTS();
		int NETWORK_GET_NUM_PARTICIPANTS();
		int NETWORK_GET_SCRIPT_STATUS();
		void NETWORK_REGISTER_HOST_BROADCAST_VARIABLES(int* vars, int numVars, const char* debugName);
		void NETWORK_REGISTER_PLAYER_BROADCAST_VARIABLES(int* vars, int numVars, const char* debugName);
		void NETWORK_REGISTER_HIGH_FREQUENCY_HOST_BROADCAST_VARIABLES(Any p0, Any p1, Any p2);
		void NETWORK_REGISTER_HIGH_FREQUENCY_PLAYER_BROADCAST_VARIABLES(Any p0, Any p1, Any p2);
		void NETWORK_FINISH_BROADCASTING_DATA();
		/**
		 * Note: this native was added in build 889.19
		 */
		void _NETWORK_GET_HOST_BROADCAST_DATA_SIZE_UNSYNCED(Hash scriptNameHash, int instance, Hash positionHash, int handlerNum); // Missing in crossmap
		/**
		 * Note: this native was added in build 889.19
		 */
		void _NETWORK_GET_PLAYER_BROADCAST_DATA_SIZE_UNSYNCED(Hash scriptNameHash, int instance, Hash positionHash, int handlerNum); // Missing in crossmap
		/**
		 * Note: this native was added in build 889.19
		 */
		void _NETWORK_GET_BROADCAST_DATA_HOST_UPDATE_SIZE(Hash scriptNameHash, int instance, Hash positionHash, int handlerNum); // Missing in crossmap
		/**
		 * Note: this native was added in build 889.19
		 */
		void _NETWORK_GET_BROADCAST_DATA_PLAYER_UPDATE_SIZE(Hash scriptNameHash, int instance, Hash positionHash, int handlerNum); // Missing in crossmap
		BOOL NETWORK_HAS_RECEIVED_HOST_BROADCAST_DATA();
		Player NETWORK_GET_PLAYER_INDEX(Player player);
		int NETWORK_GET_PARTICIPANT_INDEX(int index);
		/**
		 * Returns the Player associated to a given Ped when in an online session.
		 */
		Player NETWORK_GET_PLAYER_INDEX_FROM_PED(Ped ped);
		/**
		 * Returns the amount of players connected in the current session. Only works when connected to a session/server.
		 */
		int NETWORK_GET_NUM_CONNECTED_PLAYERS();
		BOOL NETWORK_IS_PLAYER_CONNECTED(Player player);
		int NETWORK_GET_TOTAL_NUM_PLAYERS();
		BOOL NETWORK_IS_PARTICIPANT_ACTIVE(int p0);
		BOOL NETWORK_IS_PLAYER_ACTIVE(Player player);
		BOOL NETWORK_IS_PLAYER_A_PARTICIPANT(Player player);
		BOOL NETWORK_IS_HOST_OF_THIS_SCRIPT();
		Player NETWORK_GET_HOST_OF_THIS_SCRIPT();
		/**
		 * scriptName examples:
		 * "freemode", "AM_CR_SecurityVan", ...
		 * 
		 * Most of the time, these values are used:
		 * instance_id = -1
		 * position_hash = 0
		 */
		Player NETWORK_GET_HOST_OF_SCRIPT(const char* scriptName, int instance_id, int position_hash);
		void NETWORK_GET_HOST_OF_THREAD(int threadId); // Missing in crossmap
		void NETWORK_SET_MISSION_FINISHED();
		BOOL NETWORK_IS_SCRIPT_ACTIVE(const char* scriptName, int instance_id, BOOL p2, int position_hash);
		BOOL NETWORK_IS_SCRIPT_ACTIVE_BY_HASH(Hash scriptHash, int p1, BOOL p2, int p3);
		BOOL NETWORK_IS_THREAD_A_NETWORK_SCRIPT(int threadId);
		int NETWORK_GET_NUM_SCRIPT_PARTICIPANTS(const char* scriptName, int instance_id, int position_hash);
		int NETWORK_GET_INSTANCE_ID_OF_THIS_SCRIPT();
		Hash NETWORK_GET_POSITION_HASH_OF_THIS_SCRIPT();
		BOOL NETWORK_IS_PLAYER_A_PARTICIPANT_ON_SCRIPT(Player player, const char* script, int instance_id);
		void NETWORK_PREVENT_SCRIPT_HOST_MIGRATION();
		void NETWORK_REQUEST_TO_BE_HOST_OF_THIS_SCRIPT();
		/**
		 * Return the local Participant ID
		 */
		Player PARTICIPANT_ID();
		/**
		 * Return the local Participant ID.
		 * 
		 * This native is exactly the same as 'PARTICIPANT_ID' native.
		 */
		int PARTICIPANT_ID_TO_INT();
		Player NETWORK_GET_KILLER_OF_PLAYER(Player player, Hash* weaponHash);
		Player NETWORK_GET_DESTROYER_OF_NETWORK_ID(int netId, Hash* weaponHash);
		Player NETWORK_GET_DESTROYER_OF_ENTITY(Entity entity, Hash* weaponHash);
		/**
		 * NETWORK_GET_ASSISTED_DAMAGE_OF_ENTITY that ensures the entity is dead (IS_ENTITY_DEAD)
		 */
		BOOL NETWORK_GET_ASSISTED_KILL_OF_ENTITY(Player player, Entity entity, int* p2);
		BOOL NETWORK_GET_ASSISTED_DAMAGE_OF_ENTITY(Player player, Entity entity, int* p2);
		Entity NETWORK_GET_ENTITY_KILLER_OF_PLAYER(Player player, Hash* weaponHash);
		void NETWORK_SET_CURRENT_PUBLIC_CONTENT_ID(const char* missionId);
		void NETWORK_SET_CURRENT_CHAT_OPTION(int newChatOption);
		/**
		 * mpSettingSpawn:
		 * 
		 * enum eMpSettingSpawn
		 * {
		 * MP_SETTING_SPAWN_NULL,
		 * MP_SETTING_SPAWN_PROPERTY,
		 * MP_SETTING_SPAWN_LAST_POSITION,
		 * MP_SETTING_SPAWN_GARAGE,
		 * MP_SETTING_SPAWN_RANDOM,
		 * MP_SETTING_SPAWN_PRIVATE_YACHT,
		 * MP_SETTING_SPAWN_OFFICE,
		 * MP_SETTING_SPAWN_CLUBHOUSE,
		 * MP_SETTING_SPAWN_IE_WAREHOUSE,
		 * MP_SETTING_SPAWN_BUNKER,
		 * MP_SETTING_SPAWN_HANGAR,
		 * MP_SETTING_SPAWN_DEFUNCT_BASE,
		 * MP_SETTING_SPAWN_NIGHTCLUB,
		 * MP_SETTING_SPAWN_ARENA_GARAGE,
		 * MP_SETTING_SPAWN_CASINO_APARTMENT,
		 * MP_SETTING_SPAWN_ARCADE,
		 * MP_SETTING_SPAWN_SUBMARINE,
		 * MP_SETTING_SPAWN_CAR_MEET,
		 * MP_SETTING_SPAWN_AUTO_SHOP,
		 * MP_SETTING_SPAWN_FIXER_HQ,
		 * MP_SETTING_SPAWN_MAX,
		 * };
		 */
		void NETWORK_SET_CURRENT_SPAWN_LOCATION_OPTION(Hash mpSettingSpawn);
		/**
		 * Used by MetricVEHICLE_DIST_DRIVEN
		 */
		void NETWORK_SET_VEHICLE_DRIVEN_IN_TEST_DRIVE(BOOL toggle);
		/**
		 * Sets 'loc' variable used in MetricVEHICLE_DIST_DRIVEN
		 */
		void NETWORK_SET_VEHICLE_DRIVEN_LOCATION(Hash location);
		void NETWORK_RESURRECT_LOCAL_PLAYER(float x, float y, float z, float heading, BOOL p4, BOOL changetime, BOOL p6, int p7, int p8);
		void NETWORK_SET_LOCAL_PLAYER_INVINCIBLE_TIME(int time);
		BOOL NETWORK_IS_LOCAL_PLAYER_INVINCIBLE();
		void NETWORK_DISABLE_INVINCIBLE_FLASHING(Player player, BOOL toggle);
		void NETWORK_PATCH_POST_CUTSCENE_HS4F_TUN_ENT(Ped ped);
		void NETWORK_SET_LOCAL_PLAYER_SYNC_LOOK_AT(BOOL toggle);
		BOOL NETWORK_HAS_ENTITY_BEEN_REGISTERED_WITH_THIS_THREAD(Entity entity);
		int NETWORK_GET_NETWORK_ID_FROM_ENTITY(Entity entity);
		Entity NETWORK_GET_ENTITY_FROM_NETWORK_ID(int netId);
		BOOL NETWORK_GET_ENTITY_IS_NETWORKED(Entity entity);
		BOOL NETWORK_GET_ENTITY_IS_LOCAL(Entity entity);
		void NETWORK_REGISTER_ENTITY_AS_NETWORKED(Entity entity);
		void NETWORK_UNREGISTER_NETWORKED_ENTITY(Entity entity);
		BOOL NETWORK_DOES_NETWORK_ID_EXIST(int netId);
		BOOL NETWORK_DOES_ENTITY_EXIST_WITH_NETWORK_ID(int netId);
		BOOL NETWORK_REQUEST_CONTROL_OF_NETWORK_ID(int netId);
		BOOL NETWORK_HAS_CONTROL_OF_NETWORK_ID(int netId);
		/**
		 * Returns true if the specified network id is controlled by someone else.
		 */
		BOOL NETWORK_IS_NETWORK_ID_REMOTELY_CONTROLLED(int netId);
		BOOL NETWORK_REQUEST_CONTROL_OF_ENTITY(Entity entity);
		BOOL NETWORK_REQUEST_CONTROL_OF_DOOR(int doorID);
		BOOL NETWORK_HAS_CONTROL_OF_ENTITY(Entity entity);
		BOOL NETWORK_HAS_CONTROL_OF_PICKUP(Pickup pickup);
		BOOL NETWORK_HAS_CONTROL_OF_DOOR(Hash doorHash);
		BOOL NETWORK_IS_DOOR_NETWORKED(Hash doorHash);
		/**
		 * calls from vehicle to net.
		 * 
		 */
		int VEH_TO_NET(Vehicle vehicle);
		/**
		 * gets the network id of a ped
		 */
		int PED_TO_NET(Ped ped);
		/**
		 * Lets objects spawn online simply do it like this:
		 * 
		 * int createdObject = OBJ_TO_NET(CREATE_OBJECT_NO_OFFSET(oball, pCoords.x, pCoords.y, pCoords.z, 1, 0, 0));
		 */
		int OBJ_TO_NET(Object object);
		Vehicle NET_TO_VEH(int netHandle);
		/**
		 * gets the ped id of a network id
		 */
		Ped NET_TO_PED(int netHandle);
		/**
		 * gets the object id of a network id
		 */
		Object NET_TO_OBJ(int netHandle);
		/**
		 * gets the entity id of a network id
		 */
		Entity NET_TO_ENT(int netHandle);
		void NETWORK_GET_LOCAL_HANDLE(Any* gamerHandle, int gamerHandleSize);
		void NETWORK_HANDLE_FROM_USER_ID(const char* userId, Any* gamerHandle, int gamerHandleSize);
		void NETWORK_HANDLE_FROM_MEMBER_ID(const char* memberId, Any* gamerHandle, int gamerHandleSize);
		void NETWORK_HANDLE_FROM_PLAYER(Player player, Any* gamerHandle, int gamerHandleSize);
		Hash NETWORK_HASH_FROM_PLAYER_HANDLE(Player player);
		Hash NETWORK_HASH_FROM_GAMER_HANDLE(Any* gamerHandle);
		void NETWORK_HANDLE_FROM_FRIEND(int friendIndex, Any* gamerHandle, int gamerHandleSize);
		BOOL NETWORK_GAMERTAG_FROM_HANDLE_START(Any* gamerHandle);
		BOOL NETWORK_GAMERTAG_FROM_HANDLE_PENDING();
		BOOL NETWORK_GAMERTAG_FROM_HANDLE_SUCCEEDED();
		const char* NETWORK_GET_GAMERTAG_FROM_HANDLE(Any* gamerHandle);
		/**
		 * Hardcoded to return -1.
		 */
		int NETWORK_DISPLAYNAMES_FROM_HANDLES_START(Any* p0, Any p1);
		/**
		 * This function is hard-coded to always return 0.
		 */
		int NETWORK_GET_DISPLAYNAMES_FROM_HANDLES(Any p0, Any p1, Any p2);
		BOOL NETWORK_ARE_HANDLES_THE_SAME(Any* gamerHandle1, Any* gamerHandle2);
		BOOL NETWORK_IS_HANDLE_VALID(Any* gamerHandle, int gamerHandleSize);
		Player NETWORK_GET_PLAYER_FROM_GAMER_HANDLE(Any* gamerHandle);
		const char* NETWORK_MEMBER_ID_FROM_GAMER_HANDLE(Any* gamerHandle);
		BOOL NETWORK_IS_GAMER_IN_MY_SESSION(Any* gamerHandle);
		void NETWORK_SHOW_PROFILE_UI(Any* gamerHandle);
		/**
		 * Returns the name of a given player. Returns "**Invalid**" if rlGamerInfo of the given player cannot be retrieved or the player doesn't exist.
		 */
		const char* NETWORK_PLAYER_GET_NAME(Player player);
		/**
		 * Returns a string of the player's Rockstar Id.
		 * Takes a 24 char buffer. Returns the buffer or "**Invalid**" if rlGamerInfo of the given player cannot be retrieved or the player doesn't exist.
		 */
		const char* NETWORK_PLAYER_GET_USERID(Player player, int* userID);
		/**
		 * Checks if a specific value (BYTE) in CNetGamePlayer is nonzero.
		 * Returns always false in Singleplayer.
		 * 
		 * No longer used for dev checks since first mods were released on PS3 & 360.
		 * R* now checks with the IS_DLC_PRESENT native for the dlc hash 2532323046,
		 * if that is present it will unlock dev stuff.
		 */
		BOOL NETWORK_PLAYER_IS_ROCKSTAR_DEV(Player player);
		BOOL NETWORK_PLAYER_INDEX_IS_CHEATER(Player player);
		int NETWORK_ENTITY_GET_OBJECT_ID(Entity entity);
		/**
		 * I've had this return the player's ped handle sometimes, but also other random entities.
		 * Whatever p0 is, it's at least not synced to other players.
		 * At least not all the time, some p0 values actually output the same entity, (different handle of course, but same entity).
		 * But another p0 value may return an entity for player x, but not for player y (it'll just return -1 even if the entity exists on both clients).
		 * 
		 * Returns an entity handle or -1, value changes based on p0's value.
		 */
		Entity NETWORK_GET_ENTITY_FROM_OBJECT_ID(Any p0);
		BOOL NETWORK_IS_INACTIVE_PROFILE(Any* p0);
		/**
		 * Returns the name from given friend gamer handle.
		 */
		const char* _NETWORK_LOAD_GAMER_DISPLAY_NAME(Any* gamerHandle);
		int NETWORK_GET_MAX_FRIENDS();
		int NETWORK_GET_FRIEND_COUNT();
		const char* NETWORK_GET_FRIEND_NAME(int friendIndex);
		const char* NETWORK_GET_FRIEND_DISPLAY_NAME(int friendIndex);
		BOOL NETWORK_IS_FRIEND_ONLINE(const char* name);
		BOOL NETWORK_IS_FRIEND_HANDLE_ONLINE(Any* gamerHandle);
		/**
		 * In scripts R* calls 'NETWORK_GET_FRIEND_NAME' in this param.
		 */
		BOOL NETWORK_IS_FRIEND_IN_SAME_TITLE(const char* friendName);
		BOOL NETWORK_IS_FRIEND_IN_MULTIPLAYER(const char* friendName);
		BOOL NETWORK_IS_FRIEND(Any* gamerHandle);
		/**
		 * This function is hard-coded to always return 0.
		 */
		BOOL NETWORK_IS_PENDING_FRIEND(Any p0);
		BOOL NETWORK_IS_ADDING_FRIEND();
		BOOL NETWORK_ADD_FRIEND(Any* gamerHandle, const char* message);
		BOOL NETWORK_IS_FRIEND_INDEX_ONLINE(int friendIndex);
		void NETWORK_SET_PLAYER_IS_PASSIVE(BOOL toggle);
		BOOL NETWORK_GET_PLAYER_OWNS_WAYPOINT(Player player);
		BOOL NETWORK_CAN_SET_WAYPOINT();
		void NETWORK_IGNORE_REMOTE_WAYPOINTS();
		/**
		 * communicationType: 0 = VOICE; 1 = TEXT_CHAT; 2 = TEXT_MESSAGE; 3 = EMAIL; 4 = USER_CONTENT; 5 = USER_TEXT
		 */
		BOOL _NETWORK_DOES_COMMUNICATION_GROUP_HAVE_PERMISSION(int communicationType);
		void _NETWORK_DOES_COMMUNICATION_GROUP_HAVE_SETTINGS_ENABLED(int communicationType); // Missing in crossmap
		/**
		 * Returns communicationGroupFlag
		 * communicationType: see 0xDBDF80673BBA3D65
		 * 
		 * enum eCommunicationGroupFlag
		 * {
		 * COMMUNICATION_GROUP_LOCAL_PLAYER = 1 << 0,
		 * COMMUNICATION_GROUP_FRIENDS = 1 << 1,
		 * COMMUNICATION_GROUP_SMALL_CREW = 1 << 2,
		 * COMMUNICATION_GROUP_LARGE_CREW = 1 << 3,
		 * COMMUNICATION_GROUP_RECENT_PLAYER = 1 << 4,
		 * COMMUNICATION_GROUP_SAME_SESSION = 1 << 5,
		 * COMMUNICATION_GROUP_SAME_TEAM = 1 << 6,
		 * COMMUNICATION_GROUP_INVALID = 1 << 7,
		 * };
		 */
		int _NETWORK_GET_COMMUNICATION_GROUP_FLAGS(int communicationType);
		void _NETWORK_GET_COMMUNICATION_GROUP_DEFAULT_FLAGS(int communicationType); // Missing in crossmap
		void _NETWORK_GET_COMMUNICATION_GROUP_VALUE(int communicationType); // Missing in crossmap
		/**
		 * communicationType: see 0xDBDF80673BBA3D65
		 * communicationGroupFlag: see 0x40DF02F371F40883
		 */
		void _NETWORK_SET_COMMUNICATION_GROUP_FLAGS(int communicationType, int communicationGroupFlag);
		/**
		 * _NETWORK_HAS_P*
		 * checkType 3 is similar as using 0x9F633448E4C73207
		 */
		BOOL _NETWORK_HAS_PLAYER_PASSED_CHECK_TYPE(int checkType, Any* gamerHandle);
		BOOL NETWORK_IS_PLAYER_ON_BLOCKLIST(Any* gamerHandle);
		BOOL NETWORK_SET_SCRIPT_AUTOMUTED(Any p0);
		BOOL NETWORK_HAS_AUTOMUTE_OVERRIDE();
		BOOL NETWORK_HAS_HEADSET();
		void NETWORK_SET_LOOK_AT_TALKERS(BOOL p0);
		BOOL NETWORK_IS_PUSH_TO_TALK_ACTIVE();
		BOOL NETWORK_GAMER_HAS_HEADSET(Any* gamerHandle);
		BOOL NETWORK_IS_GAMER_TALKING(Any* gamerHandle);
		BOOL NETWORK_PERMISSIONS_HAS_GAMER_RECORD(Any* gamerHandle);
		BOOL NETWORK_CAN_COMMUNICATE_WITH_GAMER(Any* gamerHandle);
		BOOL NETWORK_CAN_TEXT_CHAT_WITH_GAMER(Any* gamerHandle);
		BOOL NETWORK_IS_GAMER_MUTED_BY_ME(Any* gamerHandle);
		BOOL NETWORK_AM_I_MUTED_BY_GAMER(Any* gamerHandle);
		BOOL NETWORK_IS_GAMER_BLOCKED_BY_ME(Any* gamerHandle);
		BOOL NETWORK_AM_I_BLOCKED_BY_GAMER(Any* gamerHandle);
		BOOL NETWORK_CAN_VIEW_GAMER_USER_CONTENT(Any* gamerHandle);
		BOOL NETWORK_HAS_VIEW_GAMER_USER_CONTENT_RESULT(Any* gamerHandle);
		/**
		 * NETWORK_CAN_*
		 */
		BOOL _NETWORK_CAN_TEXT_FROM_GAMER_BE_VIEWED(Any* gamerHandle);
		BOOL NETWORK_CAN_PLAY_MULTIPLAYER_WITH_GAMER(Any* gamerHandle);
		BOOL NETWORK_CAN_GAMER_PLAY_MULTIPLAYER_WITH_ME(Any* gamerHandle);
		BOOL NETWORK_CAN_SEND_LOCAL_INVITE(Any* gamerHandle);
		BOOL NETWORK_CAN_RECEIVE_LOCAL_INVITE(Any* gamerHandle);
		/**
		 * returns true if someone is screaming or talking in a microphone
		 */
		BOOL NETWORK_IS_PLAYER_TALKING(Player player);
		BOOL NETWORK_PLAYER_HAS_HEADSET(Player player);
		BOOL NETWORK_IS_PLAYER_MUTED_BY_ME(Player player);
		BOOL NETWORK_AM_I_MUTED_BY_PLAYER(Player player);
		BOOL NETWORK_IS_PLAYER_BLOCKED_BY_ME(Player player);
		BOOL NETWORK_AM_I_BLOCKED_BY_PLAYER(Player player);
		float NETWORK_GET_PLAYER_LOUDNESS(Player player);
		void NETWORK_SET_TALKER_PROXIMITY(float value);
		float NETWORK_GET_TALKER_PROXIMITY();
		void NETWORK_SET_VOICE_ACTIVE(BOOL toggle);
		void NETWORK_REMAIN_IN_GAME_CHAT(BOOL p0);
		void NETWORK_OVERRIDE_TRANSITION_CHAT(BOOL p0);
		void NETWORK_SET_TEAM_ONLY_CHAT(BOOL toggle);
		void NETWORK_SET_SCRIPT_CONTROLLING_TEAMS(Any p0);
		BOOL NETWORK_SET_SAME_TEAM_AS_LOCAL_PLAYER(Any p0, Any p1);
		void NETWORK_OVERRIDE_TEAM_RESTRICTIONS(int team, BOOL toggle);
		void NETWORK_SET_OVERRIDE_SPECTATOR_MODE(BOOL toggle);
		void NETWORK_SET_OVERRIDE_TUTORIAL_SESSION_CHAT(BOOL toggle);
		void NETWORK_SET_PROXIMITY_AFFECTS_TEAM(BOOL toggle);
		void NETWORK_SET_NO_SPECTATOR_CHAT(BOOL toggle);
		void NETWORK_SET_IGNORE_SPECTATOR_CHAT_LIMITS_SAME_TEAM(BOOL toggle);
		/**
		 * Could possibly bypass being muted or automatically muted
		 */
		void NETWORK_OVERRIDE_CHAT_RESTRICTIONS(Player player, BOOL toggle);
		/**
		 * This is used alongside the native,
		 * 'NETWORK_OVERRIDE_RECEIVE_RESTRICTIONS'. Read its description for more info.
		 */
		void NETWORK_OVERRIDE_SEND_RESTRICTIONS(Player player, BOOL toggle);
		void NETWORK_OVERRIDE_SEND_RESTRICTIONS_ALL(BOOL toggle);
		/**
		 * R* uses this to hear all player when spectating.
		 * It allows you to hear other online players when their chat is on none, crew and or friends
		 */
		void NETWORK_OVERRIDE_RECEIVE_RESTRICTIONS(Player player, BOOL toggle);
		/**
		 * p0 is always false in scripts.
		 */
		void NETWORK_OVERRIDE_RECEIVE_RESTRICTIONS_ALL(BOOL toggle);
		void NETWORK_SET_VOICE_CHANNEL(int channel);
		void NETWORK_CLEAR_VOICE_CHANNEL();
		void NETWORK_APPLY_VOICE_PROXIMITY_OVERRIDE(float x, float y, float z);
		void NETWORK_CLEAR_VOICE_PROXIMITY_OVERRIDE();
		void NETWORK_ENABLE_VOICE_BANDWIDTH_RESTRICTION(Player player);
		void NETWORK_DISABLE_VOICE_BANDWIDTH_RESTRICTION(Player player);
		/**
		 * NETWORK_GET_M[A-U]
		 */
		void NETWORK_GET_MUTE_COUNT_FOR_PLAYER(Player p0, float* p1, float* p2);
		void NETWORK_SET_SPECTATOR_TO_NON_SPECTATOR_TEXT_CHAT(BOOL toggle);
		/**
		 * Same as _IS_TEXT_CHAT_ACTIVE, except it does not check if the text chat HUD component is initialized, and therefore may crash.
		 */
		BOOL NETWORK_TEXT_CHAT_IS_TYPING();
		/**
		 * Starts a new singleplayer game (at the prologue).
		 */
		void SHUTDOWN_AND_LAUNCH_SINGLE_PLAYER_GAME();
		/**
		 * In singleplayer this will re-load your game.
		 * 
		 * In FiveM / GTA:Online this disconnects you from the session, and starts loading single player, however you still remain connected to the server (only if you're the host, if you're not then you also (most likely) get disconnected from the server) and other players will not be able to join until you exit the game.
		 * 
		 * You might need to DoScreenFadeIn and ShutdownLoadingScreen otherwise you probably won't end up loading into SP at all.
		 * 
		 * Somewhat related note: opening the pause menu after loading into this 'singleplayer' mode crashes the game.
		 */
		BOOL SHUTDOWN_AND_LOAD_MOST_RECENT_SAVE();
		void NETWORK_SET_FRIENDLY_FIRE_OPTION(BOOL toggle);
		/**
		 * This native does absolutely nothing, just a nullsub
		 */
		void NETWORK_SET_RICH_PRESENCE(int p0, int p1, Any p2, Any p3);
		/**
		 * This native does absolutely nothing, just a nullsub
		 */
		void NETWORK_SET_RICH_PRESENCE_STRING(int p0, const char* textLabel);
		int NETWORK_GET_TIMEOUT_TIME();
		/**
		 * p4 and p5 are always 0 in scripts
		 */
		void NETWORK_LEAVE_PED_BEHIND_BEFORE_WARP(Player player, float x, float y, float z, BOOL p4, BOOL p5);
		void NETWORK_LEAVE_PED_BEHIND_BEFORE_CUTSCENE(Player player, BOOL p1);
		/**
		 * entity must be a valid entity; ped can be NULL
		 */
		void REMOVE_ALL_STICKY_BOMBS_FROM_ENTITY(Entity entity, Ped ped);
		void NETWORK_KEEP_ENTITY_COLLISION_DISABLED_AFTER_ANIM_SCENE(Any p0, Any p1);
		BOOL NETWORK_IS_ANY_PLAYER_NEAR(Any p0, Any p1, Any p2, Any p3, Any p4, Any p5, Any p6);
		BOOL NETWORK_CLAN_SERVICE_IS_VALID();
		BOOL NETWORK_CLAN_PLAYER_IS_ACTIVE(Any* gamerHandle);
		/**
		 * bufferSize is 35 in the scripts.
		 * 
		 * bufferSize is the elementCount of p0(desc), sizeof(p0) == 280 == p1*8 == 35 * 8, p2(netHandle) is obtained from NETWORK::NETWORK_HANDLE_FROM_PLAYER.  And no, I can't explain why 35 * sizeof(int) == 280 and not 140, but I'll get back to you on that.
		 * 
		 * the answer is: because p0 an int64_t* / int64_t[35].  and FYI p2 is an int64_t[13]
		 * 
		 * https://pastebin.com/cSZniHak
		 */
		BOOL NETWORK_CLAN_PLAYER_GET_DESC(Any* clanDesc, int bufferSize, Any* gamerHandle);
		/**
		 * bufferSize is 35 in the scripts.
		 */
		BOOL NETWORK_CLAN_IS_ROCKSTAR_CLAN(Any* clanDesc, int bufferSize);
		/**
		 * bufferSize is 35 in the scripts.
		 */
		void NETWORK_CLAN_GET_UI_FORMATTED_TAG(Any* clanDesc, int bufferSize, char* formattedTag);
		int NETWORK_CLAN_GET_LOCAL_MEMBERSHIPS_COUNT();
		BOOL NETWORK_CLAN_GET_MEMBERSHIP_DESC(Any* memberDesc, int p1);
		BOOL NETWORK_CLAN_DOWNLOAD_MEMBERSHIP(Any* gamerHandle);
		BOOL NETWORK_CLAN_DOWNLOAD_MEMBERSHIP_PENDING(Any* p0);
		BOOL NETWORK_CLAN_ANY_DOWNLOAD_MEMBERSHIP_PENDING();
		BOOL NETWORK_CLAN_REMOTE_MEMBERSHIPS_ARE_IN_CACHE(int* p0);
		int NETWORK_CLAN_GET_MEMBERSHIP_COUNT(int* p0);
		BOOL NETWORK_CLAN_GET_MEMBERSHIP_VALID(int* p0, Any p1);
		BOOL NETWORK_CLAN_GET_MEMBERSHIP(int* p0, Any* clanMembership, int p2);
		BOOL NETWORK_CLAN_JOIN(int clanDesc);
		/**
		 * Only documented...
		 * 
		 * Full list of animation dictionaries and anims by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/animDictsCompact.json
		 */
		BOOL NETWORK_CLAN_CREWINFO_GET_STRING_VALUE(const char* animDict, const char* animName);
		BOOL NETWORK_CLAN_CREWINFO_GET_CREWRANKTITLE(int p0, const char* p1);
		BOOL NETWORK_CLAN_HAS_CREWINFO_METADATA_BEEN_RECEIVED();
		BOOL NETWORK_CLAN_GET_EMBLEM_TXD_NAME(Any* netHandle, char* txdName);
		BOOL NETWORK_CLAN_REQUEST_EMBLEM(Any p0);
		BOOL NETWORK_CLAN_IS_EMBLEM_READY(Any p0, Any* p1);
		void NETWORK_CLAN_RELEASE_EMBLEM(Any p0);
		BOOL NETWORK_GET_PRIMARY_CLAN_DATA_CLEAR();
		void NETWORK_GET_PRIMARY_CLAN_DATA_CANCEL();
		BOOL NETWORK_GET_PRIMARY_CLAN_DATA_START(Any* p0, Any p1);
		BOOL NETWORK_GET_PRIMARY_CLAN_DATA_PENDING();
		BOOL NETWORK_GET_PRIMARY_CLAN_DATA_SUCCESS();
		BOOL NETWORK_GET_PRIMARY_CLAN_DATA_NEW(Any* p0, Any* p1);
		/**
		 * Whether or not another player is allowed to take control of the entity
		 */
		void SET_NETWORK_ID_CAN_MIGRATE(int netId, BOOL toggle);
		void SET_NETWORK_ID_EXISTS_ON_ALL_MACHINES(int netId, BOOL toggle);
		void SET_NETWORK_ID_ALWAYS_EXISTS_FOR_PLAYER(int netId, Player player, BOOL toggle);
		/**
		 * "No Reassign" in CPhysicalScriptGameStateDataNode
		 */
		void SET_NETWORK_ID_CAN_BE_REASSIGNED(int netId, BOOL toggle);
		void NETWORK_SET_ENTITY_CAN_BLEND(Entity entity, BOOL toggle);
		void NETWORK_SET_OBJECT_CAN_BLEND_WHEN_FIXED(Object object, BOOL toggle);
		/**
		 * if set to true other network players can't see it
		 * if set to false other network player can see it
		 * =========================================
		 * ^^ I attempted this by grabbing an object with GET_ENTITY_PLAYER_IS_FREE_AIMING_AT and setting this naive no matter the toggle he could still see it.
		 * 
		 * pc or last gen?
		 * 
		 * ^^ last-gen
		 */
		void NETWORK_SET_ENTITY_ONLY_EXISTS_FOR_PARTICIPANTS(Entity entity, BOOL toggle);
		void SET_NETWORK_ID_VISIBLE_IN_CUTSCENE(int netId, BOOL p1, BOOL p2);
		void SET_NETWORK_ID_VISIBLE_IN_CUTSCENE_HACK(int netId, BOOL p1, BOOL p2);
		void SET_NETWORK_ID_VISIBLE_IN_CUTSCENE_REMAIN_HACK(Any p0, Any p1, Any p2);
		void SET_NETWORK_CUTSCENE_ENTITIES(BOOL toggle);
		/**
		 * Getter for SET_NETWORK_CUTSCENE_ENTITIES.
		 */
		BOOL ARE_CUTSCENE_ENTITIES_NETWORKED();
		void SET_NETWORK_ID_PASS_CONTROL_IN_TUTORIAL(int netId, BOOL state);
		void _NETWORK_SET_TUTORIAL_SPECIAL_SESSION(BOOL toggle); // Missing in crossmap
		void _NETWORK_IS_SPECIAL_TUTORIAL_SESSION(); // Missing in crossmap
		BOOL IS_NETWORK_ID_OWNED_BY_PARTICIPANT(int netId);
		void SET_REMOTE_PLAYER_VISIBLE_IN_CUTSCENE(Player player, BOOL locallyVisible);
		void SET_LOCAL_PLAYER_VISIBLE_IN_CUTSCENE(BOOL p0, BOOL p1);
		void SET_LOCAL_PLAYER_INVISIBLE_LOCALLY(BOOL bIncludePlayersVehicle);
		void SET_LOCAL_PLAYER_VISIBLE_LOCALLY(BOOL bIncludePlayersVehicle);
		void SET_PLAYER_INVISIBLE_LOCALLY(Player player, BOOL bIncludePlayersVehicle);
		void SET_PLAYER_VISIBLE_LOCALLY(Player player, BOOL bIncludePlayersVehicle);
		/**
		 * Hardcoded to not work in SP.
		 */
		void FADE_OUT_LOCAL_PLAYER(BOOL p0);
		/**
		 * normal - transition like when your coming out of LSC
		 * slow - transition like when you walk into a mission
		 * 
		 */
		void NETWORK_FADE_OUT_ENTITY(Entity entity, BOOL normal, BOOL slow);
		/**
		 * state - 0 does 5 fades
		 * state - 1 does 6 fades
		 * 
		 * p3: setting to 1 made vehicle fade in slower, probably "slow" as per NETWORK_FADE_OUT_ENTITY
		 */
		void NETWORK_FADE_IN_ENTITY(Entity entity, BOOL state, Any p2);
		BOOL NETWORK_IS_PLAYER_FADING(Player player);
		BOOL NETWORK_IS_ENTITY_FADING(Entity entity);
		BOOL IS_PLAYER_IN_CUTSCENE(Player player);
		void SET_ENTITY_VISIBLE_IN_CUTSCENE(Any p0, BOOL p1, BOOL p2);
		/**
		 * Makes the provided entity visible for yourself for the current frame.
		 */
		void SET_ENTITY_LOCALLY_INVISIBLE(Entity entity);
		void SET_ENTITY_LOCALLY_VISIBLE(Entity entity);
		BOOL IS_DAMAGE_TRACKER_ACTIVE_ON_NETWORK_ID(int netID);
		void ACTIVATE_DAMAGE_TRACKER_ON_NETWORK_ID(int netID, BOOL toggle);
		BOOL IS_DAMAGE_TRACKER_ACTIVE_ON_PLAYER(Player player);
		void ACTIVATE_DAMAGE_TRACKER_ON_PLAYER(Player player, BOOL toggle);
		BOOL IS_SPHERE_VISIBLE_TO_ANOTHER_MACHINE(float p0, float p1, float p2, float p3);
		BOOL IS_SPHERE_VISIBLE_TO_PLAYER(Any p0, float p1, float p2, float p3, float p4);
		void RESERVE_NETWORK_MISSION_OBJECTS(int amount);
		void RESERVE_NETWORK_MISSION_PEDS(int amount);
		void RESERVE_NETWORK_MISSION_VEHICLES(int amount);
		void RESERVE_LOCAL_NETWORK_MISSION_OBJECTS(int amount);
		void RESERVE_LOCAL_NETWORK_MISSION_PEDS(int amount);
		void RESERVE_LOCAL_NETWORK_MISSION_VEHICLES(int amount);
		BOOL CAN_REGISTER_MISSION_OBJECTS(int amount);
		BOOL CAN_REGISTER_MISSION_PEDS(int amount);
		BOOL CAN_REGISTER_MISSION_VEHICLES(int amount);
		BOOL CAN_REGISTER_MISSION_PICKUPS(int amount);
		BOOL CAN_REGISTER_MISSION_DOORS(Any p0);
		BOOL CAN_REGISTER_MISSION_ENTITIES(int ped_amt, int vehicle_amt, int object_amt, int pickup_amt);
		/**
		 * p0 appears to be for MP
		 */
		int GET_NUM_RESERVED_MISSION_OBJECTS(BOOL p0, Any p1);
		/**
		 * p0 appears to be for MP
		 */
		int GET_NUM_RESERVED_MISSION_PEDS(BOOL p0, Any p1);
		/**
		 * p0 appears to be for MP
		 */
		int GET_NUM_RESERVED_MISSION_VEHICLES(BOOL p0, Any p1);
		int GET_NUM_CREATED_MISSION_OBJECTS(BOOL p0);
		int GET_NUM_CREATED_MISSION_PEDS(BOOL p0);
		int GET_NUM_CREATED_MISSION_VEHICLES(BOOL p0);
		void GET_RESERVED_MISSION_ENTITIES_IN_AREA(float x, float y, float z, Any p3, Any* out1, Any* out2, Any* out3);
		int GET_MAX_NUM_NETWORK_OBJECTS();
		int GET_MAX_NUM_NETWORK_PEDS();
		int GET_MAX_NUM_NETWORK_VEHICLES();
		int GET_MAX_NUM_NETWORK_PICKUPS();
		void NETWORK_SET_OBJECT_SCOPE_DISTANCE(Object object, float range);
		void NETWORK_ALLOW_CLONING_WHILE_IN_TUTORIAL(Any p0, Any p1);
		/**
		 * A value between 1.0 and 5.0
		 */
		void NETWORK_SET_TASK_CUTSCENE_INSCOPE_MULTIPLER(float multiplier);
		void _NETWORK_HIDE_ENTITY_IN_TUTORIAL_SESSION(int netHandle, BOOL hide);
		int GET_NETWORK_TIME();
		/**
		 * Returns the same value as GET_NETWORK_TIME in freemode, but as opposed to `GET_NETWORK_TIME` it always gets the most recent time, instead of once per tick.
		 * Could be used for benchmarking since it can return times in ticks.
		 */
		int GET_NETWORK_TIME_ACCURATE();
		BOOL HAS_NETWORK_TIME_STARTED();
		/**
		 * Adds the first argument to the second.
		 */
		int GET_TIME_OFFSET(int timeA, int timeB);
		/**
		 * Subtracts the second argument from the first, then returns whether the result is negative.
		 */
		BOOL IS_TIME_LESS_THAN(int timeA, int timeB);
		/**
		 * Subtracts the first argument from the second, then returns whether the result is negative.
		 */
		BOOL IS_TIME_MORE_THAN(int timeA, int timeB);
		/**
		 * Returns true if the two times are equal; otherwise returns false.
		 */
		BOOL IS_TIME_EQUAL_TO(int timeA, int timeB);
		/**
		 * Subtracts the second argument from the first.
		 */
		int GET_TIME_DIFFERENCE(int timeA, int timeB);
		const char* GET_TIME_AS_STRING(int time);
		/**
		 * Same as GET_CLOUD_TIME_AS_INT but returns the value as a hex string (%I64X).
		 */
		const char* GET_CLOUD_TIME_AS_STRING();
		/**
		 * Returns POSIX timestamp, an int representing the cloud time.
		 */
		int GET_CLOUD_TIME_AS_INT();
		/**
		 * Takes the specified time and writes it to the structure specified in the second argument.
		 * 
		 * struct date_time
		 * {
		 * int year;
		 * int PADDING1;
		 * int month;
		 * int PADDING2;
		 * int day;
		 * int PADDING3;
		 * int hour;
		 * int PADDING4;
		 * int minute;
		 * int PADDING5;
		 * int second;
		 * int PADDING6;
		 * };
		 */
		void CONVERT_POSIX_TIME(int posixTime, Any* timeStructure);
		void NETWORK_SET_IN_SPECTATOR_MODE(BOOL toggle, Ped playerPed);
		void NETWORK_SET_IN_SPECTATOR_MODE_EXTENDED(BOOL toggle, Ped playerPed, BOOL p2);
		void NETWORK_SET_IN_FREE_CAM_MODE(BOOL toggle);
		void NETWORK_SET_ANTAGONISTIC_TO_PLAYER(BOOL toggle, Player player);
		BOOL NETWORK_IS_IN_SPECTATOR_MODE();
		void NETWORK_SET_IN_MP_CUTSCENE(BOOL p0, BOOL p1);
		BOOL NETWORK_IS_IN_MP_CUTSCENE();
		BOOL NETWORK_IS_PLAYER_IN_MP_CUTSCENE(Player player);
		void NETWORK_HIDE_PROJECTILE_IN_CUTSCENE();
		void SET_NETWORK_VEHICLE_RESPOT_TIMER(int netId, int time, Any p2, Any p3);
		BOOL IS_NETWORK_VEHICLE_RUNNING_RESPOT_TIMER(int networkID);
		void SET_NETWORK_VEHICLE_AS_GHOST(Vehicle vehicle, BOOL toggle);
		/**
		 * rage::netBlenderLinInterp::GetPositionMaxForUpdateLevel
		 */
		void SET_NETWORK_VEHICLE_MAX_POSITION_DELTA_MULTIPLIER(Vehicle vehicle, float multiplier);
		/**
		 * Enables a periodic ShapeTest within the NetBlender and invokes rage::netBlenderLinInterp::GoStraightToTarget (or some functional wrapper).
		 */
		void SET_NETWORK_ENABLE_HIGH_SPEED_EDGE_FALL_DETECTION(Vehicle vehicle, BOOL toggle);
		void SET_LOCAL_PLAYER_AS_GHOST(BOOL toggle, BOOL p1);
		BOOL IS_ENTITY_A_GHOST(Entity entity);
		void SET_NON_PARTICIPANTS_OF_THIS_SCRIPT_AS_GHOSTS(BOOL p0);
		/**
		 * Enables ghosting between specific players
		 */
		void SET_REMOTE_PLAYER_AS_GHOST(Player player, BOOL p1);
		/**
		 * Must be a value between 1 and 254
		 */
		void SET_GHOST_ALPHA(int alpha);
		/**
		 * Resets the entity ghost alpha to the default value (128)
		 */
		void RESET_GHOST_ALPHA();
		void SET_ENTITY_GHOSTED_FOR_GHOST_PLAYERS(Entity entity, BOOL toggle);
		void SET_INVERT_GHOSTING(BOOL p0);
		BOOL IS_ENTITY_IN_GHOST_COLLISION(Entity entity);
		/**
		 * reportData includes mc, ceo, yacht and licenceplate names
		 */
		void _SET_FREEMODE_REPORT_DATA(Any* gamerHandle, Any* reportData);
		void IS_OBJECT_REASSIGNMENT_IN_PROGRESS(); // Missing in crossmap
		void USE_PLAYER_COLOUR_INSTEAD_OF_TEAM_COLOUR(BOOL toggle);
		int NETWORK_CREATE_SYNCHRONISED_SCENE(float x, float y, float z, float xRot, float yRot, float zRot, int rotationOrder, BOOL useOcclusionPortal, BOOL looped, float p9, float animTime, float p11);
		void NETWORK_ADD_PED_TO_SYNCHRONISED_SCENE(Ped ped, int netScene, const char* animDict, const char* animnName, float speed, float speedMultiplier, int duration, int flag, float playbackRate, Any p9);
		void NETWORK_ADD_PED_TO_SYNCHRONISED_SCENE_WITH_IK(Any p0, Any p1, Any p2, Any p3, Any p4, Any p5, Any p6, Any p7, Any p8, Any p9);
		void NETWORK_ADD_ENTITY_TO_SYNCHRONISED_SCENE(Entity entity, int netScene, const char* animDict, const char* animName, float speed, float speedMulitiplier, int flag);
		/**
		 * Similar structure as NETWORK_ADD_ENTITY_TO_SYNCHRONISED_SCENE but it includes this time a hash.
		 * In casino_slots it is used one time in a synced scene involving a ped and the slot machine?
		 */
		void NETWORK_ADD_MAP_ENTITY_TO_SYNCHRONISED_SCENE(int netScene, Hash modelHash, float x, float y, float z, float p5, const char* p6, float p7, float p8, int flags);
		void NETWORK_ADD_SYNCHRONISED_SCENE_CAMERA(int netScene, const char* animDict, const char* animName);
		void NETWORK_ATTACH_SYNCHRONISED_SCENE_TO_ENTITY(int netScene, Entity entity, int bone);
		void NETWORK_START_SYNCHRONISED_SCENE(int netScene);
		void NETWORK_STOP_SYNCHRONISED_SCENE(int netScene);
		int NETWORK_GET_LOCAL_SCENE_FROM_NETWORK_ID(int netId);
		void NETWORK_FORCE_LOCAL_USE_OF_SYNCED_SCENE_CAMERA(int netScene);
		void NETWORK_ALLOW_REMOTE_SYNCED_SCENE_LOCAL_PLAYER_REQUESTS(Any p0);
		/**
		 * p0 is always 0. p1 is pointing to a global.
		 */
		BOOL NETWORK_FIND_LARGEST_BUNCH_OF_PLAYERS(int p0, Any p1);
		/**
		 * One of the first things it does is get the players ped.
		 * Then it calls a function that is used in some tasks and ped based functions.
		 * p5, p6, p7 is another coordinate (or zero), often related to `GET_BLIP_COORDS, in the decompiled scripts.
		 */
		BOOL NETWORK_START_RESPAWN_SEARCH_FOR_PLAYER(Player player, float x, float y, float z, float radius, float p5, float p6, float p7, int flags);
		/**
		 * p8, p9, p10 is another coordinate, or zero, often related to `GET_BLIP_COORDS in the decompiled scripts.
		 */
		BOOL NETWORK_START_RESPAWN_SEARCH_IN_ANGLED_AREA_FOR_PLAYER(Player player, float x1, float y1, float z1, float x2, float y2, float z2, float width, float p8, float p9, float p10, int flags);
		int NETWORK_QUERY_RESPAWN_RESULTS(Any* p0);
		void NETWORK_CANCEL_RESPAWN_SEARCH();
		/**
		 * Based on scripts such as in freemode.c how they call their vars vVar and fVar the 2nd and 3rd param it a Vector3 and Float, but the first is based on get_random_int_in_range..
		 */
		void NETWORK_GET_RESPAWN_RESULT(int randomInt, Vector3* coordinates, float* heading);
		int NETWORK_GET_RESPAWN_RESULT_FLAGS(int p0);
		void NETWORK_START_SOLO_TUTORIAL_SESSION();
		/**
		 * teamId must be < 3, instanceId must be < 64
		 */
		void NETWORK_ALLOW_GANG_TO_JOIN_TUTORIAL_SESSION(int teamId, int instanceId);
		void NETWORK_END_TUTORIAL_SESSION();
		BOOL NETWORK_IS_IN_TUTORIAL_SESSION();
		BOOL NETWORK_WAITING_POP_CLEAR_TUTORIAL_SESSION();
		BOOL NETWORK_IS_TUTORIAL_SESSION_CHANGE_PENDING();
		int NETWORK_GET_PLAYER_TUTORIAL_SESSION_INSTANCE(Player player);
		BOOL NETWORK_ARE_PLAYERS_IN_SAME_TUTORIAL_SESSION(Player player, int index);
		void NETWORK_BLOCK_PROXY_MIGRATION_BETWEEN_TUTORIAL_SESSIONS(Any p0);
		void NETWORK_CONCEAL_PLAYER(Player player, BOOL toggle, BOOL p2);
		BOOL NETWORK_IS_PLAYER_CONCEALED(Player player);
		void NETWORK_CONCEAL_ENTITY(Entity entity, BOOL toggle);
		/**
		 * Note: This only works for vehicles, which appears to be a bug (since the setter _does_ work for every entity type and the name is 99% correct).
		 */
		BOOL NETWORK_IS_ENTITY_CONCEALED(Entity entity);
		/**
		 * Works in Singleplayer too.
		 * Passing wrong data (e.g. hours above 23) will cause the game to crash.
		 */
		void NETWORK_OVERRIDE_CLOCK_TIME(int hours, int minutes, int seconds);
		void NETWORK_OVERRIDE_CLOCK_RATE(int ms);
		void NETWORK_CLEAR_CLOCK_TIME_OVERRIDE();
		void _NETWORK_CLEAR_CLOCK_SYNC_TIME_OVERRIDE(BOOL startGlobalTransition, int transitionTime); // Missing in crossmap
		/**
		 * Does nothing in final builds.
		 */
		void NETWORK_SYNC_CLOCK_TIME_OVERRIDE(); // Missing in crossmap
		BOOL NETWORK_IS_CLOCK_TIME_OVERRIDDEN();
		int NETWORK_ADD_ENTITY_AREA(float x1, float y1, float z1, float x2, float y2, float z2);
		/**
		 * To remove, see: NETWORK_REMOVE_ENTITY_AREA
		 * See IS_POINT_IN_ANGLED_AREA for the definition of an angled area.
		 */
		int NETWORK_ADD_ENTITY_ANGLED_AREA(float x1, float y1, float z1, float x2, float y2, float z2, float width);
		int NETWORK_ADD_CLIENT_ENTITY_AREA(float x1, float y1, float z1, float x2, float y2, float z2);
		int NETWORK_ADD_CLIENT_ENTITY_ANGLED_AREA(float x1, float y1, float z1, float x2, float y2, float z2, float radius);
		BOOL NETWORK_REMOVE_ENTITY_AREA(int areaHandle);
		BOOL NETWORK_ENTITY_AREA_DOES_EXIST(int areaHandle);
		BOOL NETWORK_ENTITY_AREA_HAVE_ALL_REPLIED(int areaHandle);
		BOOL NETWORK_ENTITY_AREA_IS_OCCUPIED(int areaHandle);
		void NETWORK_USE_HIGH_PRECISION_BLENDING(int netID, BOOL toggle);
		void NETWORK_SET_CUSTOM_ARENA_BALL_PARAMS(int netId);
		void NETWORK_ENTITY_USE_HIGH_PRECISION_ROTATION(int netId, BOOL toggle);
		BOOL NETWORK_REQUEST_CLOUD_BACKGROUND_SCRIPTS();
		BOOL NETWORK_IS_CLOUD_BACKGROUND_SCRIPT_REQUEST_PENDING();
		void NETWORK_REQUEST_CLOUD_TUNABLES();
		BOOL NETWORK_IS_TUNABLE_CLOUD_REQUEST_PENDING();
		/**
		 * Actually returns the version (TUNABLE_VERSION)
		 */
		int NETWORK_GET_TUNABLE_CLOUD_CRC();
		/**
		 * Sets up tunable contexts for _NETWORK_GET_TUNABLES_REGISTRATION_{BOOL|INT|FLOAT}
		 */
		void _NETWORK_SET_TUNABLES_REGISTRATION_CONTEXTS(Any* tunableContextData);
		/**
		 * Clears tunable contexts that were set up using _NETWORK_SET_TUNABLES_REGISTRATION_CONTEXTS
		 */
		void _NETWORK_CLEAR_TUNABLES_REGISTRATION_CONTEXTS();
		/**
		 * Checks if the given tunable exists and returns its value. Otherwise returns defaultValue.
		 * Possible tunable contexts must first be set up using _NETWORK_SET_TUNABLES_REGISTRATION_CONTEXTS.
		 */
		BOOL _NETWORK_GET_TUNABLES_REGISTRATION_BOOL(Hash tunableName, BOOL defaultValue);
		/**
		 * Checks if the given tunable exists and returns its value. Otherwise returns defaultValue.
		 * Possible tunable contexts must first be set up using _NETWORK_SET_TUNABLES_REGISTRATION_CONTEXTS.
		 */
		int _NETWORK_GET_TUNABLES_REGISTRATION_INT(Hash tunableName, int defaultValue);
		/**
		 * Checks if the given tunable exists and returns its value. Otherwise returns defaultValue.
		 * Possible tunable contexts must first be set up using _NETWORK_SET_TUNABLES_REGISTRATION_CONTEXTS.
		 */
		float _NETWORK_GET_TUNABLES_REGISTRATION_FLOAT(Hash tunableName, float defaultValue);
		BOOL NETWORK_DOES_TUNABLE_EXIST(const char* tunableContext, const char* tunableName);
		BOOL NETWORK_ACCESS_TUNABLE_INT(const char* tunableContext, const char* tunableName, int* value);
		BOOL NETWORK_ACCESS_TUNABLE_FLOAT(const char* tunableContext, const char* tunableName, float* value);
		BOOL NETWORK_ACCESS_TUNABLE_BOOL(const char* tunableContext, const char* tunableName);
		BOOL NETWORK_DOES_TUNABLE_EXIST_HASH(Hash tunableContext, Hash tunableName);
		BOOL NETWORK_ACCESS_TUNABLE_MODIFICATION_DETECTION_CLEAR();
		BOOL NETWORK_ACCESS_TUNABLE_INT_HASH(Hash tunableContext, Hash tunableName, int* value);
		BOOL NETWORK_ACCESS_TUNABLE_INT_MODIFICATION_DETECTION_REGISTRATION_HASH(Hash contextHash, Hash nameHash, int* value);
		BOOL NETWORK_ACCESS_TUNABLE_FLOAT_HASH(Hash tunableContext, Hash tunableName, float* value);
		BOOL NETWORK_ACCESS_TUNABLE_FLOAT_MODIFICATION_DETECTION_REGISTRATION_HASH(Hash contextHash, Hash nameHash, float* value);
		BOOL NETWORK_ACCESS_TUNABLE_BOOL_HASH(Hash tunableContext, Hash tunableName);
		BOOL NETWORK_ACCESS_TUNABLE_BOOL_MODIFICATION_DETECTION_REGISTRATION_HASH(Hash contextHash, Hash nameHash, BOOL* value);
		/**
		 * Returns defaultValue if the tunable doesn't exist.
		 */
		BOOL NETWORK_TRY_ACCESS_TUNABLE_BOOL_HASH(Hash tunableContext, Hash tunableName, BOOL defaultValue);
		/**
		 * Return the content modifier id (the tunables context if you want) of a specific content.
		 * 
		 * It takes the content hash (which is the mission id hash), and return the content modifier id, used as the tunables context.
		 * 
		 * The mission id can be found on the Social club, for example, 'socialclub.rockstargames.com/games/gtav/jobs/job/A8M6Bz8MLEC5xngvDCzGwA'
		 * 
		 * 'A8M6Bz8MLEC5xngvDCzGwA' is the mission id, so the game hash this and use it as the parameter for this native.
		 * 
		 */
		int NETWORK_GET_CONTENT_MODIFIER_LIST_ID(Hash contentHash);
		int NETWORK_GET_BONE_ID_OF_FATAL_HIT();
		void NETWORK_RESET_BODY_TRACKER();
		int NETWORK_GET_NUMBER_BODY_TRACKER_HITS();
		BOOL NETWORK_HAS_BONE_BEEN_HIT_BY_KILLER(int boneIndex);
		BOOL NETWORK_SET_ATTRIBUTE_DAMAGE_TO_PLAYER(Ped ped, Player player);
		/**
		 * Allows vehicle wheels to be destructible even when the vehicle entity is invincible.
		 */
		void NETWORK_TRIGGER_DAMAGE_EVENT_FOR_ZERO_DAMAGE(Entity entity, BOOL toggle);
		void _NETWORK_TRIGGER_DAMAGE_EVENT_FOR_ZERO_PED_DAMAGE(Entity entity, BOOL trigger);
		void NETWORK_TRIGGER_DAMAGE_EVENT_FOR_ZERO_WEAPON_HASH(Entity entity, BOOL toggle);
		void NETWORK_SET_NO_LONGER_NEEDED(Entity entity, BOOL toggle);
		/**
		 * In the console script dumps, this is only referenced once.
		 * NETWORK::NETWORK_EXPLODE_VEHICLE(vehicle, 1, 0, 0);
		 * 
		 * ^^^^^ That must be PC script dumps? In X360 Script Dumps it is reference a few times with 2 differences in the parameters.
		 * Which as you see below is 1, 0, 0 + 1, 1, 0 + 1, 0, and a *param?
		 * 
		 * am_plane_takedown.c
		 * network_explode_vehicle(net_to_veh(Local_40.imm_2), 1, 1, 0);
		 * 
		 * armenian2.c
		 * network_explode_vehicle(Local_80[6 <2>], 1, 0, 0);
		 * 
		 * fm_horde_controler.c
		 * network_explode_vehicle(net_to_veh(*uParam0), 1, 0, *uParam0);
		 * 
		 * fm_mission_controller.c, has 6 hits so not going to list them.
		 * 
		 * Side note, setting the first parameter to 0 seems to mute sound or so?
		 * 
		 * Seems it's like ADD_EXPLOSION, etc. the first 2 params. The 3rd atm no need to worry since it always seems to be 0.
		 * 
		 */
		BOOL NETWORK_EXPLODE_VEHICLE(Vehicle vehicle, BOOL isAudible, BOOL isInvisible, int netId);
		void NETWORK_EXPLODE_HELI(Vehicle vehicle, BOOL isAudible, BOOL isInvisible, int netId);
		void NETWORK_USE_LOGARITHMIC_BLENDING_THIS_FRAME(Entity entity);
		void NETWORK_OVERRIDE_COORDS_AND_HEADING(Entity entity, float x, float y, float z, float heading);
		void NETWORK_ENABLE_EXTRA_VEHICLE_ORIENTATION_BLEND_CHECKS(int netId, BOOL toggle);
		void NETWORK_DISABLE_PROXIMITY_MIGRATION(int netID);
		/**
		 * p0 in the decompiled scripts is always the stat mesh_texblend * 0.07 to int
		 */
		void NETWORK_SET_PLAYER_MENTAL_STATE(int p0);
		void NETWORK_SET_MINIMUM_RANK_FOR_MISSION(BOOL p0);
		void NETWORK_CACHE_LOCAL_PLAYER_HEAD_BLEND_DATA();
		BOOL NETWORK_HAS_CACHED_PLAYER_HEAD_BLEND_DATA(Player player);
		BOOL NETWORK_APPLY_CACHED_PLAYER_HEAD_BLEND_DATA(Ped ped, Player player);
		void _NETWORK_SET_IGNORE_VEHICLE_RAMMED_BY_NON_VEHICLE(BOOL toggle); // Missing in crossmap
		int GET_NUM_COMMERCE_ITEMS();
		BOOL IS_COMMERCE_DATA_VALID();
		/**
		 * Does nothing (it's a nullsub).
		 */
		void TRIGGER_COMMERCE_DATA_FETCH(Any p0);
		BOOL IS_COMMERCE_DATA_FETCH_IN_PROGRESS();
		const char* GET_COMMERCE_ITEM_ID(int index);
		const char* GET_COMMERCE_ITEM_NAME(int index);
		const char* GET_COMMERCE_PRODUCT_PRICE(int index);
		int GET_COMMERCE_ITEM_NUM_CATS(int index);
		/**
		 * index2 is unused
		 */
		const char* GET_COMMERCE_ITEM_CAT(int index, int index2);
		void RESERVE_COMMERCE_STORE_PURCHASE_LOCATION(int location); // Missing in crossmap
		void OPEN_COMMERCE_STORE(const char* productID, const char* category, int location, BOOL launchLandingPageOnClose);
		void CHECKOUT_COMMERCE_PRODUCT(const char* productID, int location, BOOL launchLandingPageOnClose); // Missing in crossmap
		BOOL IS_COMMERCE_STORE_OPEN();
		/**
		 * Access to the store for shark cards etc...
		 */
		void SET_STORE_ENABLED(BOOL toggle);
		BOOL REQUEST_COMMERCE_ITEM_IMAGE(int index);
		void RELEASE_ALL_COMMERCE_ITEM_IMAGES();
		const char* GET_COMMERCE_ITEM_TEXTURENAME(int index);
		BOOL IS_STORE_AVAILABLE_TO_USER();
		void DELAY_MP_STORE_OPEN();
		void RESET_STORE_NETWORK_GAME_TRACKING();
		BOOL IS_USER_OLD_ENOUGH_TO_ACCESS_STORE();
		void SET_LAST_VIEWED_SHOP_ITEM(Hash p0, int p1, Hash p2);
		/**
		 * Checks some commerce stuff
		 */
		int GET_USER_PREMIUM_ACCESS();
		/**
		 * Checks some commerce stuff
		 */
		int GET_USER_STARTER_ACCESS();
		int CLOUD_DELETE_MEMBER_FILE(const char* p0);
		BOOL CLOUD_HAS_REQUEST_COMPLETED(int requestId);
		BOOL CLOUD_DID_REQUEST_SUCCEED(int requestId);
		/**
		 * Downloads prod.cloud.rockstargames.com/titles/gta5/[platform]/check.json
		 */
		void CLOUD_CHECK_AVAILABILITY();
		BOOL CLOUD_IS_CHECKING_AVAILABILITY();
		BOOL CLOUD_GET_AVAILABILITY_CHECK_RESULT();
		/**
		 * This function is hard-coded to always return 0.
		 */
		int GET_CONTENT_TO_LOAD_TYPE();
		/**
		 * This function is hard-coded to always return 0.
		 */
		BOOL GET_IS_LAUNCH_FROM_LIVE_AREA();
		/**
		 * This function is hard-coded to always return 0.
		 */
		BOOL GET_IS_LIVE_AREA_LAUNCH_WITH_CONTENT();
		/**
		 * This native does absolutely nothing, just a nullsub
		 */
		void CLEAR_SERVICE_EVENT_ARGUMENTS();
		BOOL UGC_COPY_CONTENT(Any* p0, Any* p1);
		BOOL UGC_IS_CREATING();
		BOOL UGC_HAS_CREATE_FINISHED();
		BOOL UGC_DID_CREATE_SUCCEED();
		int UGC_GET_CREATE_RESULT();
		const char* UGC_GET_CREATE_CONTENT_ID();
		void UGC_CLEAR_CREATE_RESULT();
		BOOL UGC_QUERY_MY_CONTENT(Any p0, Any p1, Any* p2, Any p3, Any p4, Any p5);
		BOOL UGC_QUERY_BY_CATEGORY(Any p0, Any p1, Any p2, const char* p3, Any p4, BOOL p5);
		BOOL UGC_QUERY_BY_CONTENT_ID(const char* contentId, BOOL latestVersion, const char* contentTypeName);
		BOOL UGC_QUERY_BY_CONTENT_IDS(Any* data, int count, BOOL latestVersion, const char* contentTypeName);
		BOOL UGC_QUERY_MOST_RECENTLY_CREATED_CONTENT(int offset, int count, const char* contentTypeName, int p3);
		BOOL UGC_GET_BOOKMARKED_CONTENT(Any p0, Any p1, const char* p2, Any* p3);
		BOOL UGC_GET_MY_CONTENT(Any p0, Any p1, const char* p2, Any* p3);
		BOOL UGC_GET_FRIEND_CONTENT(Any p0, Any p1, const char* p2, Any* p3);
		BOOL UGC_GET_CREW_CONTENT(Any p0, Any p1, Any p2, const char* p3, Any* p4);
		BOOL UGC_GET_GET_BY_CATEGORY(Any p0, Any p1, Any p2, const char* p3, Any* p4);
		BOOL UGC_GET_GET_BY_CONTENT_ID(const char* contentId, const char* contentTypeName);
		BOOL UGC_GET_GET_BY_CONTENT_IDS(Any* data, int dataCount, const char* contentTypeName);
		BOOL UGC_GET_MOST_RECENTLY_CREATED_CONTENT(Any p0, Any p1, Any* p2, Any* p3);
		BOOL UGC_GET_MOST_RECENTLY_PLAYED_CONTENT(Any p0, Any p1, Any* p2, Any* p3);
		BOOL UGC_GET_TOP_RATED_CONTENT(Any p0, Any p1, Any* p2, Any* p3);
		void UGC_CANCEL_QUERY();
		BOOL UGC_IS_GETTING();
		BOOL UGC_HAS_GET_FINISHED();
		BOOL UGC_DID_GET_SUCCEED();
		BOOL UGC_WAS_QUERY_FORCE_CANCELLED();
		int UGC_GET_QUERY_RESULT();
		int UGC_GET_CONTENT_NUM();
		int UGC_GET_CONTENT_TOTAL();
		Hash UGC_GET_CONTENT_HASH();
		void UGC_CLEAR_QUERY_RESULTS();
		const char* UGC_GET_CONTENT_USER_ID(int p0);
		BOOL UGC_GET_CONTENT_CREATOR_GAMER_HANDLE(int p0, Any* p1);
		BOOL UGC_GET_CONTENT_CREATED_BY_LOCAL_PLAYER(Any p0);
		const char* UGC_GET_CONTENT_USER_NAME(Any p0);
		BOOL UGC_GET_CONTENT_IS_USING_SC_NICKNAME(Any p0);
		int UGC_GET_CONTENT_CATEGORY(int p0);
		/**
		 * Return the mission id of a job.
		 */
		const char* UGC_GET_CONTENT_ID(int p0);
		/**
		 * Return the root content id of a job.
		 */
		const char* UGC_GET_ROOT_CONTENT_ID(int p0);
		const char* UGC_GET_CONTENT_NAME(Any p0);
		void UGC_GET_CONTENT_DESCRIPTION(int index); // Missing in crossmap
		int UGC_GET_CONTENT_DESCRIPTION_HASH(Any p0);
		const char* UGC_GET_CONTENT_PATH(int p0, int p1);
		void UGC_GET_CONTENT_UPDATED_DATE(Any p0, Any* p1);
		int UGC_GET_CONTENT_FILE_VERSION(Any p0, Any p1);
		BOOL UGC_GET_CONTENT_HAS_LO_RES_PHOTO(int p0);
		BOOL UGC_GET_CONTENT_HAS_HI_RES_PHOTO(int p0);
		int UGC_GET_CONTENT_LANGUAGE(Any p0);
		BOOL UGC_GET_CONTENT_IS_PUBLISHED(Any p0);
		BOOL UGC_GET_CONTENT_IS_VERIFIED(Any p0);
		float UGC_GET_CONTENT_RATING(Any p0, Any p1);
		int UGC_GET_CONTENT_RATING_COUNT(Any p0, Any p1);
		int UGC_GET_CONTENT_RATING_POSITIVE_COUNT(Any p0, Any p1);
		int UGC_GET_CONTENT_RATING_NEGATIVE_COUNT(Any p0, Any p1);
		BOOL UGC_GET_CONTENT_HAS_PLAYER_RECORD(Any p0);
		BOOL UGC_GET_CONTENT_HAS_PLAYER_BOOKMARKED(Any p0);
		int UGC_REQUEST_CONTENT_DATA_FROM_INDEX(int p0, int p1);
		int UGC_REQUEST_CONTENT_DATA_FROM_PARAMS(const char* contentTypeName, const char* contentId, int p2, int p3, int p4);
		int UGC_REQUEST_CACHED_DESCRIPTION(int p0);
		BOOL UGC_IS_DESCRIPTION_REQUEST_IN_PROGRESS(Any p0);
		BOOL UGC_HAS_DESCRIPTION_REQUEST_FINISHED(Any p0);
		BOOL UGC_DID_DESCRIPTION_REQUEST_SUCCEED(Any p0);
		const char* UGC_GET_CACHED_DESCRIPTION(Any p0, Any p1);
		BOOL UGC_RELEASE_CACHED_DESCRIPTION(Any p0);
		void UGC_RELEASE_ALL_CACHED_DESCRIPTIONS();
		BOOL UGC_HAS_PERMISSION_TO_WRITE();
		BOOL UGC_PUBLISH(const char* contentId, const char* baseContentId, const char* contentTypeName);
		BOOL UGC_SET_BOOKMARKED(const char* contentId, BOOL bookmarked, const char* contentTypeName);
		BOOL UGC_SET_DELETED(Any* p0, BOOL p1, const char* p2);
		BOOL UGC_IS_MODIFYING();
		BOOL UGC_HAS_MODIFY_FINISHED();
		BOOL UGC_DID_MODIFY_SUCCEED();
		int UGC_GET_MODIFY_RESULT();
		void UGC_CLEAR_MODIFY_RESULT();
		BOOL UGC_GET_CREATORS_BY_USER_ID(Any* p0, Any* p1);
		BOOL UGC_HAS_QUERY_CREATORS_FINISHED();
		BOOL UGC_DID_QUERY_CREATORS_SUCCEED();
		int UGC_GET_CREATOR_NUM();
		BOOL UGC_LOAD_OFFLINE_QUERY(Any p0);
		void UGC_CLEAR_OFFLINE_QUERY();
		void UGC_SET_QUERY_DATA_FROM_OFFLINE(BOOL p0);
		void UGC_SET_USING_OFFLINE_CONTENT(BOOL p0);
		BOOL UGC_IS_LANGUAGE_SUPPORTED(Any p0);
		BOOL FACEBOOK_POST_COMPLETED_HEIST(const char* heistName, int cashEarned, int xpEarned);
		BOOL FACEBOOK_POST_CREATE_CHARACTER();
		BOOL FACEBOOK_POST_COMPLETED_MILESTONE(int milestoneId);
		BOOL FACEBOOK_HAS_POST_COMPLETED();
		BOOL FACEBOOK_DID_POST_SUCCEED();
		BOOL FACEBOOK_CAN_POST_TO_FACEBOOK();
		int TEXTURE_DOWNLOAD_REQUEST(Any* gamerHandle, const char* filePath, const char* name, BOOL p3);
		int TITLE_TEXTURE_DOWNLOAD_REQUEST(const char* filePath, const char* name, BOOL p2);
		int UGC_TEXTURE_DOWNLOAD_REQUEST(const char* p0, int p1, int p2, int p3, const char* p4, BOOL p5);
		void TEXTURE_DOWNLOAD_RELEASE(int p0);
		BOOL TEXTURE_DOWNLOAD_HAS_FAILED(int p0);
		const char* TEXTURE_DOWNLOAD_GET_NAME(int p0);
		/**
		 * 0 = succeeded
		 * 1 = pending
		 * 2 = failed
		 */
		int GET_STATUS_OF_TEXTURE_DOWNLOAD(int p0);
		/**
		 * Returns true if profile setting 901 is set to true and sets it to false.
		 */
		BOOL NETWORK_CHECK_ROS_LINK_WENTDOWN_NOT_NET();
		/**
		 * Returns true if the NAT type is Strict (3) and a certain number of connections have failed.
		 */
		BOOL NETWORK_SHOULD_SHOW_STRICT_NAT_WARNING();
		BOOL NETWORK_IS_CABLE_CONNECTED();
		BOOL NETWORK_HAVE_SCS_PRIVATE_MSG_PRIV();
		BOOL NETWORK_HAVE_ROS_SOCIAL_CLUB_PRIV();
		BOOL NETWORK_HAVE_ROS_BANNED_PRIV();
		BOOL NETWORK_HAVE_ROS_CREATE_TICKET_PRIV();
		BOOL NETWORK_HAVE_ROS_MULTIPLAYER_PRIV();
		BOOL NETWORK_HAVE_ROS_LEADERBOARD_WRITE_PRIV();
		/**
		 * index is always 18 in scripts
		 */
		BOOL NETWORK_HAS_ROS_PRIVILEGE(int index);
		BOOL NETWORK_HAS_ROS_PRIVILEGE_END_DATE(int privilege, int* banType, Any* timeData);
		BOOL NETWORK_HAS_ROS_PRIVILEGE_PLAYED_LAST_GEN();
		BOOL NETWORK_HAS_ROS_PRIVILEGE_SPECIAL_EDITION_CONTENT();
		/**
		 * Checks for privilege 29
		 */
		BOOL _NETWORK_HAS_ROS_PRIVILEGE_MP_TEXT_COMMUNICATION();
		/**
		 * Checks for privilege 30
		 */
		BOOL _NETWORK_HAS_ROS_PRIVILEGE_MP_VOICE_COMMUNICATION();
		/**
		 * Checks for privilege 31
		 */
		BOOL _NETWORK_HAS_ROS_PRIVILEGE_REPORTING();
		/**
		 * Always returns -1. Seems to be XB1 specific.
		 */
		int NETWORK_START_USER_CONTENT_PERMISSIONS_CHECK(Any* netHandle);
		void NETWORK_SKIP_RADIO_RESET_NEXT_CLOSE();
		void NETWORK_SKIP_RADIO_RESET_NEXT_OPEN();
		/**
		 * Returns true if dinput8.dll is present in the game directory.
		 * You will get following error message if that is true: "You are attempting to access GTA Online servers with an altered version of the game."
		 */
		BOOL NETWORK_SKIP_RADIO_WARNING();
		/**
		 * NETWORK_F[I-O]
		 */
		void NETWORK_FORCE_LOCAL_PLAYER_SCAR_SYNC();
		void NETWORK_DISABLE_LEAVE_REMOTE_PED_BEHIND(BOOL toggle);
		void NETWORK_ALLOW_REMOTE_ATTACHMENT_MODIFICATION(Entity entity, BOOL toggle);
		/**
		 * Does nothing (it's a nullsub).
		 */
		void NETWORK_SHOW_CHAT_RESTRICTION_MSC(Player player);
		/**
		 * This native does absolutely nothing, just a nullsub
		 */
		void NETWORK_SHOW_PSN_UGC_RESTRICTION();
		/**
		 * This function is hard-coded to always return 0.
		 */
		BOOL NETWORK_IS_TITLE_UPDATE_REQUIRED();
		void NETWORK_QUIT_MP_TO_DESKTOP();
		BOOL NETWORK_IS_CONNECTED_VIA_RELAY(Player player);
		float NETWORK_GET_AVERAGE_LATENCY(Player player);
		/**
		 * Same as NETWORK_GET_AVERAGE_LATENCY
		 */
		float NETWORK_GET_AVERAGE_PING(Player player);
		float NETWORK_GET_AVERAGE_PACKET_LOSS(Player player);
		int NETWORK_GET_NUM_UNACKED_RELIABLES(Player player);
		int NETWORK_GET_UNRELIABLE_RESEND_COUNT(Player player);
		int NETWORK_GET_HIGHEST_RELIABLE_RESEND_COUNT(Player player);
		void NETWORK_REPORT_CODE_TAMPER();
		void _NETWORK_GET_DUMP_OF_ASSET_VERIFIER(Any* p0); // Missing in crossmap
		Vector3 NETWORK_GET_LAST_ENTITY_POS_RECEIVED_OVER_NETWORK(Entity entity);
		/**
		 * Returns the coordinates of another player.
		 * Does not work if you enter your own player id as p0 (will return `(0.0, 0.0, 0.0)` in that case).
		 */
		Vector3 NETWORK_GET_LAST_PLAYER_POS_RECEIVED_OVER_NETWORK(Player player);
		/**
		 * Used by NetBlender
		 */
		Vector3 NETWORK_GET_LAST_VEL_RECEIVED_OVER_NETWORK(Entity entity);
		Vector3 NETWORK_GET_PREDICTED_VELOCITY(Entity entity, float maxSpeedToPredict);
		/**
		 * Does nothing (it's a nullsub).
		 */
		void NETWORK_DUMP_NET_IF_CONFIG();
		/**
		 * Does nothing (it's a nullsub).
		 */
		void NETWORK_GET_SIGNALLING_INFO(Any* p0);
		/**
		 * Does nothing (it's a nullsub).
		 */
		void NETWORK_GET_NET_STATISTICS_INFO(Any* p0);
		int NETWORK_GET_PLAYER_ACCOUNT_ID(Player player);
		void NETWORK_POST_UDS_ACTIVITY_START(const char* activityId); // Missing in crossmap
		void NETWORK_POST_UDS_ACTIVITY_END(const char* activityId, int iOutcome, int iScore); // Missing in crossmap
		void NETWORK_POST_UDS_ACTIVITY_RESUME(const char* activityId); // Missing in crossmap
		void NETWORK_POST_UDS_ACTIVITY_RESUME_WITH_TASKS(const char* activityId, Any* taskStatus); // Missing in crossmap
		void NETWORK_POST_UDS_ACTIVITY_AVAILABILITY_CHANGE(Any* availablityData); // Missing in crossmap
		/**
		 * Does nothing.
		 */
		void NETWORK_POST_UDS_ACTIVITY_TERMINATE(); // Missing in crossmap
		void NETWORK_HAS_SC_MEMBERSHIP_INFO(); // Missing in crossmap
		void NETWORK_HAS_SC_MEMBERSHIP(); // Missing in crossmap
		void NETWORK_GET_SC_MEMBERSHIP_INFO(Any* info); // Missing in crossmap
		void NETWORK_UGC_NAV(Any p0, Any p1);
		/**
		 * enum eNetworkGameRestartReason
		 * {
		 * BE_RESTART_SERVICE_NOT_RUNNING = 1, 	// BattlEye service not running
		 * BE_RESTART_SERVICE_NEEDS_UPDATE, 		// BattlEye service needs an update
		 * BE_RESTART_UNKNOWN_REASON,				// Unknown BattlEye reason
		 * BE_RESTART_EF_DISABLED_WITH_BE,			// BattlEye Enforcement Flag is disabled, but BattlEye is active
		 * BE_RESTART_EF_ENABLED_NO_BE,			// BattlEye Enforcement Flag is enabled, but BattlEye is not active
		 * BE_RESTART_BE_MODULES_FAILED_TO_LOAD,	// BattlEye modules failed to load
		 * NUM_NETWORK_GAME_RESTART_REASONS
		 * };
		 */
		int _NETWORK_GET_GAME_RESTART_REASON();
		/**
		 * If the restart reason is 4 (BE_RESTART_EF_DISABLED_WITH_BE), will cause the game to exit with exit code 622011. Otherwise exits with code 622010. Has no effect when a restart hasn't been requested (reason == 0 or reason > 6).
		 * 
		 * Note that this 'confirmation' itself does not restart the game, a call to QUIT_GAME or similar is required afterwards.
		 */
		void _NETWORK_CONFIRM_GAME_RESTART(); // Missing in crossmap
		/**
		 * Returns the restart reason as a string, see _NETWORK_GET_GAME_RESTART_REASON. Returns a null pointer when a restart hasn't been requested (reason == 0 or reason > 6).
		 */
		void _NETWORK_GET_GAME_RESTART_REASON_MESSAGE_LABEL(); // Missing in crossmap
		/**
		 * 0x20211000 = HUD_BE_ERROR_FAILED_UNKNOWN
		 * 0x20212000 = HUD_BE_ERROR_FAILED_CLIENT_INIT
		 * 0x20213000 = HUD_BE_ERROR_FAILED_SERVER_INIT
		 * 0x20214000 = HUD_BE_ERROR_FAILED_AUTH_ATTEMPTS
		 * 0x20215000 = HUD_BE_ERROR_UNTRUSTED_HOST
		 * 0x20216000 = HUD_BE_ERROR_KICKED
		 * 0x20217000 = HUD_BE_ERROR_BANNED
		 * 0x50140000 = HUD_BE_BLOCK_BANNED
		 * 
		 * Note: an invalid error code returns a null pointer!
		 */
		void _NETWORK_GET_BATTLEYE_ERROR_MESSAGE_LABEL(int errorCode); // Missing in crossmap

	} // namespace NETWORK

	namespace OBJECT {

		/**
		 * List of object models that can be created without any additional effort like making sure ytyp is loaded etc: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/ObjectList.ini
		 */
		Object CREATE_OBJECT(Hash modelHash, float x, float y, float z, BOOL isNetwork, BOOL bScriptHostObj, BOOL dynamic);
		/**
		 * List of object models that can be created without any additional effort like making sure ytyp is loaded etc: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/ObjectList.ini
		 */
		Object CREATE_OBJECT_NO_OFFSET(Hash modelHash, float x, float y, float z, BOOL isNetwork, BOOL bScriptHostObj, BOOL dynamic, Any p7);
		/**
		 * Deletes the specified object, then sets the handle pointed to by the pointer to NULL.
		 */
		void DELETE_OBJECT(Object* object);
		BOOL PLACE_OBJECT_ON_GROUND_PROPERLY(Object object);
		BOOL PLACE_OBJECT_ON_GROUND_OR_OBJECT_PROPERLY(Object object);
		BOOL ROTATE_OBJECT(Object object, float p1, float p2, BOOL p3);
		/**
		 * Returns true if the object has finished moving.
		 * 
		 * If false, moves the object towards the specified X, Y and Z coordinates with the specified X, Y and Z speed.
		 * 
		 * See also: https://gtagmodding.com/opcode-database/opcode/034E/
		 * Has to be looped until it returns true.
		 */
		BOOL SLIDE_OBJECT(Object object, float toX, float toY, float toZ, float speedX, float speedY, float speedZ, BOOL collision);
		void SET_OBJECT_TARGETTABLE(Object object, BOOL targettable, Any p2);
		/**
		 * Sets the 34th and 35th object flags related to player peds.
		 */
		void _SET_OBJECT_TARGETTABLE_BY_PLAYER(Object object, BOOL setFlag34, BOOL setFlag35);
		/**
		 * Overrides a flag on the object which determines if the object should be avoided by a vehicle in task CTaskVehicleGoToPointWithAvoidanceAutomobile.
		 */
		void SET_OBJECT_FORCE_VEHICLES_TO_AVOID(Object object, BOOL toggle);
		/**
		 * Has 8 params in the latest patches.
		 * 
		 * isMission - if true doesn't return mission objects
		 */
		Object GET_CLOSEST_OBJECT_OF_TYPE(float x, float y, float z, float radius, Hash modelHash, BOOL isMission, BOOL p6, BOOL p7);
		BOOL HAS_OBJECT_BEEN_BROKEN(Object object, Any p1);
		BOOL HAS_CLOSEST_OBJECT_OF_TYPE_BEEN_BROKEN(float p0, float p1, float p2, float p3, Hash modelHash, Any p5);
		BOOL HAS_CLOSEST_OBJECT_OF_TYPE_BEEN_COMPLETELY_DESTROYED(float x, float y, float z, float radius, Hash modelHash, BOOL p5);
		BOOL GET_HAS_OBJECT_BEEN_COMPLETELY_DESTROYED(Any p0);
		Vector3 GET_OFFSET_FROM_COORD_AND_HEADING_IN_WORLD_COORDS(float xPos, float yPos, float zPos, float heading, float xOffset, float yOffset, float zOffset);
		BOOL GET_COORDS_AND_ROTATION_OF_CLOSEST_OBJECT_OF_TYPE(float x, float y, float z, float radius, Hash modelHash, Vector3* outPosition, Vector3* outRotation, int rotationOrder);
		/**
		 * Hardcoded to not work in multiplayer.
		 * 
		 * 
		 * Used to lock/unlock doors to interior areas of the game.
		 * 
		 * (Possible) Door Types:
		 * 
		 * https://pastebin.com/9S2m3qA4
		 * 
		 * Heading is either 1, 0 or -1 in the scripts. Means default closed(0) or opened either into(1) or out(-1) of the interior.
		 * Locked means that the heading is locked.
		 * p6 is always 0.
		 * 
		 * 225 door types, model names and coords found in stripclub.c4:
		 * https://pastebin.com/gywnbzsH
		 * 
		 * get door info: https://pastebin.com/i14rbekD
		 */
		void SET_STATE_OF_CLOSEST_DOOR_OF_TYPE(Hash type, float x, float y, float z, BOOL locked, float heading, BOOL p6);
		/**
		 * locked is 0 if no door is found
		 * locked is 0 if door is unlocked
		 * locked is 1 if door is found and unlocked.
		 * 
		 * -------------
		 * the locked bool is either 0(unlocked)(false) or 1(locked)(true)
		 */
		void GET_STATE_OF_CLOSEST_DOOR_OF_TYPE(Hash type, float x, float y, float z, BOOL* locked, float* heading);
		/**
		 * Hardcoded not to work in multiplayer environments.
		 * When you set locked to 0 the door open and to 1 the door close
		 * OBJECT::SET_LOCKED_UNSTREAMED_IN_DOOR_OF_TYPE(${prop_gate_prison_01}, 1845.0, 2605.0, 45.0, 0, 0.0, 50.0, 0);  //door open
		 * 
		 * OBJECT::SET_LOCKED_UNSTREAMED_IN_DOOR_OF_TYPE(${prop_gate_prison_01}, 1845.0, 2605.0, 45.0, 1, 0.0, 50.0, 0);  //door close
		 */
		void SET_LOCKED_UNSTREAMED_IN_DOOR_OF_TYPE(Hash modelHash, float x, float y, float z, BOOL locked, float xRotMult, float yRotMult, float zRotMult);
		void PLAY_OBJECT_AUTO_START_ANIM(Any p0);
		/**
		 * doorHash has to be unique. scriptDoor false; relies upon getNetworkGameScriptHandler. isLocal On true disables the creation CRequestDoorEvent's in DOOR_SYSTEM_SET_DOOR_STATE.
		 * p5 only set to true in single player native scripts.
		 * If scriptDoor is true, register the door on the script handler host (note: there's a hardcap on the number of script IDs that can be added to the system at a given time). If scriptDoor and isLocal are both false, the door is considered to be in a "Persists w/o netobj" state.
		 * 
		 * door hashes normally look like PROP_[int]_DOOR_[int] for interior doors and PROP_BUILDING_[int]_DOOR_[int] exterior doors but you can just make up your own hash if you want
		 * All doors need to be registered with ADD_DOOR_TO_SYSTEM before they can be manipulated with the door natives and the easiest way to get door models is just find the door in codewalker.
		 * 
		 * Example: AddDoorToSystem("PROP_43_DOOR_0", "hei_v_ilev_fh_heistdoor2", -1456.818, -520.5037, 69.67043, 0, 0, 0)
		 */
		void ADD_DOOR_TO_SYSTEM(Hash doorHash, Hash modelHash, float x, float y, float z, BOOL p5, BOOL scriptDoor, BOOL isLocal, Any p8);
		/**
		 * CDoor and CDoorSystemData still internally allocated (and their associations between doorHash, modelHash, and coordinates).
		 * Only its NetObj removed and flag ``*(v2 + 192) |= 8u`` (1604 retail) toggled.
		 */
		void REMOVE_DOOR_FROM_SYSTEM(Hash doorHash, Any p1);
		/**
		 * Lockstates not applied and CNetObjDoor's not created until DOOR_SYSTEM_GET_IS_PHYSICS_LOADED returns true.
		 * `requestDoor` on true, and when door system is configured to, i.e., "persists w/o netobj", generate a CRequestDoorEvent.
		 * `forceUpdate` on true, forces an update on the door system (same path as netObjDoor_applyDoorStuff)
		 * Door lock states:
		 * 0: UNLOCKED
		 * 1: LOCKED
		 * 2: DOORSTATE_FORCE_LOCKED_UNTIL_OUT_OF_AREA
		 * 3: DOORSTATE_FORCE_UNLOCKED_THIS_FRAME
		 * 4: DOORSTATE_FORCE_LOCKED_THIS_FRAME
		 * 5: DOORSTATE_FORCE_OPEN_THIS_FRAME
		 * 6: DOORSTATE_FORCE_CLOSED_THIS_FRAME
		 */
		void DOOR_SYSTEM_SET_DOOR_STATE(Hash doorHash, int state, BOOL requestDoor, BOOL forceUpdate);
		int DOOR_SYSTEM_GET_DOOR_STATE(Hash doorHash);
		int DOOR_SYSTEM_GET_DOOR_PENDING_STATE(Hash doorHash);
		/**
		 * Includes networking check: ownership vs. or the door itself **isn't** networked.
		 * `forceUpdate` on true invokes DOOR_SYSTEM_SET_DOOR_STATE otherwise requestDoor is unused.
		 */
		void DOOR_SYSTEM_SET_AUTOMATIC_RATE(Hash doorHash, float rate, BOOL requestDoor, BOOL forceUpdate);
		/**
		 * `forceUpdate` on true invokes DOOR_SYSTEM_SET_DOOR_STATE otherwise requestDoor is unused.
		 */
		void DOOR_SYSTEM_SET_AUTOMATIC_DISTANCE(Hash doorHash, float distance, BOOL requestDoor, BOOL forceUpdate);
		/**
		 * Sets the ajar angle of a door.
		 * Ranges from -1.0 to 1.0, and 0.0 is closed / default.
		 * `forceUpdate` on true invokes DOOR_SYSTEM_SET_DOOR_STATE otherwise requestDoor is unused.
		 */
		void DOOR_SYSTEM_SET_OPEN_RATIO(Hash doorHash, float ajar, BOOL requestDoor, BOOL forceUpdate);
		float DOOR_SYSTEM_GET_AUTOMATIC_DISTANCE(Hash doorHash);
		float DOOR_SYSTEM_GET_OPEN_RATIO(Hash doorHash);
		void DOOR_SYSTEM_GET_IS_SPRING_REMOVED(Hash doorHash); // Missing in crossmap
		/**
		 * Includes networking check: ownership vs. or the door itself **isn't** networked.
		 * `forceUpdate` on true invokes DOOR_SYSTEM_SET_DOOR_STATE otherwise requestDoor is unused.
		 */
		void DOOR_SYSTEM_SET_SPRING_REMOVED(Hash doorHash, BOOL removed, BOOL requestDoor, BOOL forceUpdate);
		/**
		 * Includes networking check: ownership vs. or the door itself **isn't** networked.
		 */
		void DOOR_SYSTEM_SET_HOLD_OPEN(Hash doorHash, BOOL toggle);
		/**
		 * Some property related to gates. Native name between ``DOOR_SYSTEM_SET_AUTOMATIC_RATE`` and ``DOOR_SYSTEM_SET_DOOR_STATE``.
		 */
		void DOOR_SYSTEM_SET_DOOR_OPEN_FOR_RACES(Hash doorHash, BOOL p1);
		/**
		 * if (OBJECT::IS_DOOR_REGISTERED_WITH_SYSTEM(doorHash))
		 * {
		 * OBJECT::REMOVE_DOOR_FROM_SYSTEM(doorHash);
		 * }
		 */
		BOOL IS_DOOR_REGISTERED_WITH_SYSTEM(Hash doorHash);
		BOOL IS_DOOR_CLOSED(Hash doorHash);
		void OPEN_ALL_BARRIERS_FOR_RACE(BOOL p0);
		/**
		 * Clears the fields sets by 0xC7F29CA00F46350E (1604 retail: 0x1424A7A10, 0x1424A7A11) and iterates over the global CDoor's bucket-list.
		 * Related to its "Pre-networked state"?
		 */
		void CLOSE_ALL_BARRIERS_FOR_RACE();
		BOOL DOOR_SYSTEM_GET_IS_PHYSICS_LOADED(Any p0);
		/**
		 * Search radius: 0.5
		 */
		BOOL DOOR_SYSTEM_FIND_EXISTING_DOOR(float x, float y, float z, Hash modelHash, Hash* outDoorHash);
		BOOL IS_GARAGE_EMPTY(Hash garageHash, BOOL p1, int p2);
		BOOL IS_PLAYER_ENTIRELY_INSIDE_GARAGE(Hash garageHash, Player player, float p2, int p3);
		BOOL IS_PLAYER_PARTIALLY_INSIDE_GARAGE(Hash garageHash, Player player, int p2);
		BOOL ARE_ENTITIES_ENTIRELY_INSIDE_GARAGE(Hash garageHash, BOOL p1, BOOL p2, BOOL p3, Any p4);
		BOOL IS_ANY_ENTITY_ENTIRELY_INSIDE_GARAGE(Hash garageHash, BOOL p1, BOOL p2, BOOL p3, Any p4);
		/**
		 * Despite the name, it does work for any entity type.
		 */
		BOOL IS_OBJECT_ENTIRELY_INSIDE_GARAGE(Hash garageHash, Entity entity, float p2, int p3);
		/**
		 * Despite the name, it does work for any entity type.
		 */
		BOOL IS_OBJECT_PARTIALLY_INSIDE_GARAGE(Hash garageHash, Entity entity, int p2);
		void CLEAR_GARAGE(Hash garageHash, BOOL isNetwork);
		void CLEAR_OBJECTS_INSIDE_GARAGE(Hash garageHash, BOOL vehicles, BOOL peds, BOOL objects, BOOL isNetwork);
		/**
		 * Sets a flag. A valid id is 0x157DC10D
		 */
		void DISABLE_TIDYING_UP_IN_GARAGE(int id, BOOL toggle);
		void ENABLE_SAVING_IN_GARAGE(Hash garageHash, BOOL toggle);
		void CLOSE_SAFEHOUSE_GARAGES();
		/**
		 * p5 is usually 0.
		 */
		BOOL DOES_OBJECT_OF_TYPE_EXIST_AT_COORDS(float x, float y, float z, float radius, Hash hash, BOOL p5);
		/**
		 * An angled area is an X-Z oriented rectangle with three parameters:
		 * 1. origin: the mid-point along a base edge of the rectangle;
		 * 2. extent: the mid-point of opposite base edge on the other Z;
		 * 3. width: the length of the base edge; (named derived from logging strings ``CNetworkRoadNodeWorldStateData``).
		 * 
		 * The oriented rectangle can then be derived from the direction of the two points (``norm(origin - extent)``), its orthonormal, and the width, e.g:
		 * 1. golf_mp https://i.imgur.com/JhsQAK9.png
		 * 2. am_taxi https://i.imgur.com/TJWCZaT.jpg
		 */
		BOOL IS_POINT_IN_ANGLED_AREA(float xPos, float yPos, float zPos, float x1, float y1, float z1, float x2, float y2, float z2, float width, BOOL debug, BOOL includeZ);
		/**
		 * Overrides the climbing/blocking flags of the object, used in the native scripts mostly for "prop_dock_bouy_*"
		 */
		void SET_OBJECT_ALLOW_LOW_LOD_BUOYANCY(Object object, BOOL toggle);
		/**
		 * Adjust the physics parameters of a prop, or otherwise known as "object". This is useful for simulated gravity.
		 * 
		 * Other parameters seem to be unknown.
		 * 
		 * p2: seems to be weight and gravity related. Higher value makes the obj fall faster. Very sensitive?
		 * p3: seems similar to p2
		 * p4: makes obj fall slower the higher the value
		 * p5: similar to p4
		 */
		void SET_OBJECT_PHYSICS_PARAMS(Object object, float weight, float p2, float p3, float p4, float p5, float gravity, float p7, float p8, float p9, float p10, float buoyancy);
		float GET_OBJECT_FRAGMENT_DAMAGE_HEALTH(Any p0, BOOL p1);
		void SET_ACTIVATE_OBJECT_PHYSICS_AS_SOON_AS_IT_IS_UNFROZEN(Object object, BOOL toggle);
		BOOL IS_ANY_OBJECT_NEAR_POINT(float x, float y, float z, float range, BOOL p4);
		BOOL IS_OBJECT_NEAR_POINT(Hash objectHash, float x, float y, float z, float range);
		void REMOVE_OBJECT_HIGH_DETAIL_MODEL(Object object);
		void BREAK_OBJECT_FRAGMENT_CHILD(Object p0, Any p1, BOOL p2);
		void DAMAGE_OBJECT_FRAGMENT_CHILD(Any p0, Any p1, Any p2);
		void FIX_OBJECT_FRAGMENT(Object object);
		void TRACK_OBJECT_VISIBILITY(Object object);
		BOOL IS_OBJECT_VISIBLE(Object object);
		void SET_OBJECT_IS_SPECIAL_GOLFBALL(Object object, BOOL toggle);
		void SET_OBJECT_TAKES_DAMAGE_FROM_COLLIDING_WITH_BUILDINGS(Any p0, BOOL p1);
		void ALLOW_DAMAGE_EVENTS_FOR_NON_NETWORKED_OBJECTS(BOOL value);
		/**
		 * Requires a component_at_*_flsh to be attached to the weapon object
		 */
		void SET_CUTSCENES_WEAPON_FLASHLIGHT_ON_THIS_FRAME(Object object, BOOL toggle);
		/**
		 * Example:
		 * OBJECT::GET_RAYFIRE_MAP_OBJECT(-809.9619750976562, 170.919, 75.7406997680664, 3.0, "des_tvsmash");
		 */
		Object GET_RAYFIRE_MAP_OBJECT(float x, float y, float z, float radius, const char* name);
		/**
		 * Defines the state of a destructible object.
		 * Use the GET_RAYFIRE_MAP_OBJECT native to find an object's handle with its name / coords.
		 * State 2 == object just spawned
		 * State 4 == Beginning of the animation
		 * State 6 == Start animation
		 * State 9 == End of the animation
		 */
		void SET_STATE_OF_RAYFIRE_MAP_OBJECT(Object object, int state);
		/**
		 * Get a destructible object's state.
		 * Substract 1 to get the real state.
		 * See SET_STATE_OF_RAYFIRE_MAP_OBJECT to see the different states
		 * For example, if the object just spawned (state 2), the native will return 3.
		 */
		int GET_STATE_OF_RAYFIRE_MAP_OBJECT(Object object);
		/**
		 * Returns true if a destructible object with this handle exists, false otherwise.
		 */
		BOOL DOES_RAYFIRE_MAP_OBJECT_EXIST(Object object);
		/**
		 * `object`: The des-object handle to get the animation progress from.
		 * Return value is a float between 0.0 and 1.0, 0.0 is the beginning of the animation, 1.0 is the end. Value resets to 0.0 instantly after reaching 1.0.
		 */
		float GET_RAYFIRE_MAP_OBJECT_ANIM_PHASE(Object object);
		/**
		 * Full list of pickup types by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/pickupTypes.json
		 */
		Pickup CREATE_PICKUP(Hash pickupHash, float posX, float posY, float posZ, int p4, int value, BOOL p6, Hash modelHash);
		/**
		 * flags:
		 * 8 (1 << 3): place on ground
		 * 512 (1 << 9): spin around
		 * 
		 * Full list of pickup types by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/pickupTypes.json
		 */
		Pickup CREATE_PICKUP_ROTATE(Hash pickupHash, float posX, float posY, float posZ, float rotX, float rotY, float rotZ, int flag, int amount, Any p9, BOOL p10, Hash modelHash);
		void FORCE_PICKUP_ROTATE_FACE_UP();
		void SET_CUSTOM_PICKUP_WEAPON_HASH(Hash pickupHash, Pickup pickup);
		/**
		 * Full list of pickup types by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/pickupTypes.json
		 */
		Object CREATE_AMBIENT_PICKUP(Hash pickupHash, float posX, float posY, float posZ, int flags, int value, Hash modelHash, BOOL p7, BOOL p8);
		Object CREATE_NON_NETWORKED_AMBIENT_PICKUP(Hash pickupHash, float posX, float posY, float posZ, int flags, int value, Hash modelHash, BOOL p7, BOOL p8);
		void BLOCK_PLAYERS_FOR_AMBIENT_PICKUP(Any p0, Any p1);
		/**
		 * Full list of pickup types by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/pickupTypes.json
		 */
		Object CREATE_PORTABLE_PICKUP(Hash pickupHash, float x, float y, float z, BOOL placeOnGround, Hash modelHash);
		/**
		 * Full list of pickup types by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/pickupTypes.json
		 */
		Object CREATE_NON_NETWORKED_PORTABLE_PICKUP(Hash pickupHash, float x, float y, float z, BOOL placeOnGround, Hash modelHash);
		void ATTACH_PORTABLE_PICKUP_TO_PED(Object pickupObject, Ped ped);
		void DETACH_PORTABLE_PICKUP_FROM_PED(Object pickupObject);
		void FORCE_PORTABLE_PICKUP_LAST_ACCESSIBLE_POSITION_SETTING(Object object);
		void HIDE_PORTABLE_PICKUP_WHEN_DETACHED(Object pickupObject, BOOL toggle);
		void SET_MAX_NUM_PORTABLE_PICKUPS_CARRIED_BY_PLAYER(Hash modelHash, int number);
		void SET_LOCAL_PLAYER_CAN_COLLECT_PORTABLE_PICKUPS(BOOL toggle);
		Vector3 GET_SAFE_PICKUP_COORDS(float x, float y, float z, float p3, float p4);
		/**
		 * Adds an area that seems to be related to pickup physics behavior.
		 * Max amount of areas is 10. Only works in multiplayer.
		 */
		void ADD_EXTENDED_PICKUP_PROBE_AREA(float x, float y, float z, float radius);
		/**
		 * Clears all areas created by ADD_EXTENDED_PICKUP_PROBE_AREA
		 */
		void CLEAR_EXTENDED_PICKUP_PROBE_AREAS();
		Vector3 GET_PICKUP_COORDS(Pickup pickup);
		void SUPPRESS_PICKUP_SOUND_FOR_PICKUP(Any p0, Any p1);
		/**
		 * Full list of pickup types by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/pickupTypes.json
		 */
		void REMOVE_ALL_PICKUPS_OF_TYPE(Hash pickupHash);
		BOOL HAS_PICKUP_BEEN_COLLECTED(Pickup pickup);
		void REMOVE_PICKUP(Pickup pickup);
		/**
		 * Spawns one or more money pickups.
		 * 
		 * x: The X-component of the world position to spawn the money pickups at.
		 * y: The Y-component of the world position to spawn the money pickups at.
		 * z: The Z-component of the world position to spawn the money pickups at.
		 * value: The combined value of the pickups (in dollars).
		 * amount: The number of pickups to spawn.
		 * model: The model to use, or 0 for default money model.
		 * 
		 * Example:
		 * CREATE_MONEY_PICKUPS(x, y, z, 1000, 3, 0x684a97ae);
		 * 
		 * Spawns 3 spray cans that'll collectively give $1000 when picked up. (Three spray cans, each giving $334, $334, $332 = $1000).
		 * 
		 * ==============================================
		 * 
		 * Max is 2000 in MP. So if you put the amount to 20, but the value to $400,000 eg. They will only be able to pickup 20 - $2,000 bags. So, $40,000
		 */
		void CREATE_MONEY_PICKUPS(float x, float y, float z, int value, int amount, Hash model);
		BOOL DOES_PICKUP_EXIST(Pickup pickup);
		BOOL DOES_PICKUP_OBJECT_EXIST(Object pickupObject);
		Object GET_PICKUP_OBJECT(Pickup pickup);
		BOOL IS_OBJECT_A_PICKUP(Object object);
		BOOL IS_OBJECT_A_PORTABLE_PICKUP(Object object);
		/**
		 * Full list of pickup types by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/pickupTypes.json
		 */
		BOOL DOES_PICKUP_OF_TYPE_EXIST_IN_AREA(Hash pickupHash, float x, float y, float z, float radius);
		void SET_PICKUP_REGENERATION_TIME(Pickup pickup, int duration);
		void FORCE_PICKUP_REGENERATE(Any p0);
		/**
		 * Disabling/enabling a player from getting pickups. From the scripts:
		 * 
		 * OBJECT::SET_PLAYER_PERMITTED_TO_COLLECT_PICKUPS_OF_TYPE(PLAYER::PLAYER_ID(), ${pickup_portable_package}, 0);
		 * OBJECT::SET_PLAYER_PERMITTED_TO_COLLECT_PICKUPS_OF_TYPE(PLAYER::PLAYER_ID(), ${pickup_portable_package}, 0);
		 * OBJECT::SET_PLAYER_PERMITTED_TO_COLLECT_PICKUPS_OF_TYPE(PLAYER::PLAYER_ID(), ${pickup_portable_package}, 1);
		 * OBJECT::SET_PLAYER_PERMITTED_TO_COLLECT_PICKUPS_OF_TYPE(PLAYER::PLAYER_ID(), ${pickup_portable_package}, 0);
		 * OBJECT::SET_PLAYER_PERMITTED_TO_COLLECT_PICKUPS_OF_TYPE(PLAYER::PLAYER_ID(), ${pickup_armour_standard}, 0);
		 * OBJECT::SET_PLAYER_PERMITTED_TO_COLLECT_PICKUPS_OF_TYPE(PLAYER::PLAYER_ID(), ${pickup_armour_standard}, 1);
		 * 
		 * Full list of pickup types by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/pickupTypes.json
		 */
		void SET_PLAYER_PERMITTED_TO_COLLECT_PICKUPS_OF_TYPE(Player player, Hash pickupHash, BOOL toggle);
		/**
		 * Maximum amount of pickup models that can be disallowed is 30.
		 */
		void SET_LOCAL_PLAYER_PERMITTED_TO_COLLECT_PICKUPS_WITH_MODEL(Hash modelHash, BOOL toggle);
		/**
		 * Full list of pickup types by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/pickupTypes.json
		 */
		void ALLOW_ALL_PLAYERS_TO_COLLECT_PICKUPS_OF_TYPE(Hash pickupHash);
		void SET_TEAM_PICKUP_OBJECT(Object object, Any p1, BOOL p2);
		void PREVENT_COLLECTION_OF_PORTABLE_PICKUP(Object object, BOOL p1, BOOL p2);
		void SET_PICKUP_OBJECT_GLOW_WHEN_UNCOLLECTABLE(Pickup pickup, BOOL toggle);
		void _SET_PICKUP_GLOW_DISABLED(Pickup pickup, BOOL toggle); // Missing in crossmap
		/**
		 * p1 is always 0.51. This native is called before SET_PICKUP_REGENERATION_TIME in all occurances.
		 */
		void SET_PICKUP_GLOW_OFFSET(Pickup pickup, float p1);
		/**
		 * p1 is always -0.2 in scripts and p2 is always true in scripts.
		 */
		void SET_PICKUP_OBJECT_GLOW_OFFSET(Pickup pickup, float p1, BOOL p2);
		void SET_OBJECT_GLOW_IN_SAME_TEAM(Pickup pickup);
		void SET_PICKUP_OBJECT_ARROW_MARKER(Pickup pickup, BOOL toggle);
		void ALLOW_PICKUP_ARROW_MARKER_WHEN_UNCOLLECTABLE(Pickup pickup, BOOL toggle);
		int GET_DEFAULT_AMMO_FOR_WEAPON_PICKUP(Hash pickupHash);
		void SET_PICKUP_GENERATION_RANGE_MULTIPLIER(float multiplier);
		float GET_PICKUP_GENERATION_RANGE_MULTIPLIER();
		void SET_ONLY_ALLOW_AMMO_COLLECTION_WHEN_LOW(BOOL p0);
		void SET_PICKUP_UNCOLLECTABLE(Pickup pickup, BOOL toggle);
		void SET_PICKUP_TRANSPARENT_WHEN_UNCOLLECTABLE(Pickup pickup, BOOL toggle);
		void SET_PICKUP_HIDDEN_WHEN_UNCOLLECTABLE(Pickup pickup, BOOL toggle);
		void SET_PICKUP_OBJECT_TRANSPARENT_WHEN_UNCOLLECTABLE(Pickup pickup, BOOL toggle);
		/**
		 * p0 is either 0 or 50 in scripts.
		 */
		void SET_PICKUP_OBJECT_ALPHA_WHEN_TRANSPARENT(int p0);
		void SET_PORTABLE_PICKUP_PERSIST(Pickup pickup, BOOL toggle);
		void ALLOW_PORTABLE_PICKUP_TO_MIGRATE_TO_NON_PARTICIPANTS(Pickup pickup, BOOL toggle);
		void FORCE_ACTIVATE_PHYSICS_ON_UNFIXED_PICKUP(Pickup pickup, BOOL toggle);
		void ALLOW_PICKUP_BY_NONE_PARTICIPANT(Pickup pickup, BOOL toggle);
		/**
		 * enum ePickupRewardType
		 * {
		 * PICKUP_REWARD_TYPE_AMMO = (1 << 0),
		 * PICKUP_REWARD_TYPE_BULLET_MP = (1 << 1),
		 * PICKUP_REWARD_TYPE_MISSILE_MP = (1 << 2),
		 * PICKUP_REWARD_TYPE_GRENADE_LAUNCHER_MP = (1 << 3),
		 * PICKUP_REWARD_TYPE_ARMOUR = (1 << 4),
		 * PICKUP_REWARD_TYPE_HEALTH = (1 << 5),
		 * PICKUP_REWARD_TYPE_HEALTH_VARIABLE = PICKUP_REWARD_TYPE_HEALTH,
		 * PICKUP_REWARD_TYPE_MONEY_FIXED = (1 << 6),
		 * PICKUP_REWARD_TYPE_MONEY_VARIABLE = PICKUP_REWARD_TYPE_MONEY_FIXED,
		 * PICKUP_REWARD_TYPE_WEAPON = (1 << 7),
		 * PICKUP_REWARD_TYPE_STAT = (1 << 8),
		 * PICKUP_REWARD_TYPE_STAT_VARIABLE = PICKUP_REWARD_TYPE_STAT,
		 * PICKUP_REWARD_TYPE_VEHICLE_FIX = (1 << 9),
		 * PICKUP_REWARD_TYPE_FIREWORK_MP = (1 << 10),
		 * };
		 */
		void SUPPRESS_PICKUP_REWARD_TYPE(int rewardType, BOOL suppress);
		void CLEAR_ALL_PICKUP_REWARD_TYPE_SUPPRESSION();
		void CLEAR_PICKUP_REWARD_TYPE_SUPPRESSION(int rewardType);
		/**
		 * draws circular marker at pos
		 * -1 = none
		 * 0 = red
		 * 1 = green
		 * 2 = blue
		 * 3 = green larger
		 * 4 = nothing
		 * 5 = green small
		 */
		void RENDER_FAKE_PICKUP_GLOW(float x, float y, float z, int colorIndex);
		void SET_PICKUP_OBJECT_COLLECTABLE_IN_VEHICLE(Pickup pickup);
		void SET_PICKUP_TRACK_DAMAGE_EVENTS(Pickup pickup, BOOL toggle);
		/**
		 * Sets entity+38 to C (when false) or 0xFF3f (when true)
		 */
		void SET_ENTITY_FLAG_SUPPRESS_SHADOW(Entity entity, BOOL toggle);
		void SET_ENTITY_FLAG_RENDER_SMALL_SHADOW(Object object, BOOL toggle);
		/**
		 * Full list of pickup types by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/pickupTypes.json
		 */
		Hash GET_WEAPON_TYPE_FROM_PICKUP_TYPE(Hash pickupHash);
		/**
		 * Returns the pickup hash for the given weapon hash
		 */
		Hash GET_PICKUP_TYPE_FROM_WEAPON_HASH(Hash weaponHash);
		BOOL IS_PICKUP_WEAPON_OBJECT_VALID(Object object);
		int GET_OBJECT_TINT_INDEX(Object object);
		/**
		 * enum ObjectPaintVariants
		 * {
		 * Pacific = 0,
		 * Azure = 1,
		 * Nautical = 2,
		 * Continental = 3,
		 * Battleship = 4,
		 * Intrepid = 5,
		 * Uniform = 6,
		 * Classico = 7,
		 * Mediterranean = 8,
		 * Command = 9,
		 * Mariner = 10,
		 * Ruby = 11,
		 * Vintage = 12,
		 * Pristine = 13,
		 * Merchant = 14,
		 * Voyager = 15
		 * };
		 */
		void SET_OBJECT_TINT_INDEX(Object object, int textureVariation);
		BOOL SET_TINT_INDEX_CLOSEST_BUILDING_OF_TYPE(float x, float y, float z, float radius, Hash modelHash, int textureVariation);
		void SET_PROP_TINT_INDEX(Any p0, Any p1);
		BOOL SET_PROP_LIGHT_COLOR(Object object, BOOL p1, int r, int g, int b);
		BOOL IS_PROP_LIGHT_OVERRIDEN(Object object);
		void SET_OBJECT_IS_VISIBLE_IN_MIRRORS(Object object, BOOL toggle);
		void SET_OBJECT_SPEED_BOOST_AMOUNT(Object object, Any p1);
		void SET_OBJECT_SPEED_BOOST_DURATION(Object object, float duration);
		/**
		 * returns pickup hash.
		 * 
		 * Full list of pickup types by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/pickupTypes.json
		 */
		Hash CONVERT_OLD_PICKUP_TYPE_TO_NEW(Hash pickupHash);
		void SET_FORCE_OBJECT_THIS_FRAME(float x, float y, float z, float p3);
		void ONLY_CLEAN_UP_OBJECT_WHEN_OUT_OF_RANGE(Object object);
		void SET_DISABLE_COLLISIONS_BETWEEN_CARS_AND_CAR_PARACHUTE(Any p0);
		void SET_PROJECTILES_SHOULD_EXPLODE_ON_CONTACT(Entity entity, Any p1);
		/**
		 * Activate the physics to: "xs_prop_arena_{flipper,wall,bollard,turntable,pit}"
		 */
		void SET_DRIVE_ARTICULATED_JOINT(Object object, BOOL toggle, int p2);
		void SET_DRIVE_ARTICULATED_JOINT_WITH_INFLICTOR(Object object, BOOL toggle, int p2, Ped ped);
		void SET_OBJECT_IS_A_PRESSURE_PLATE(Object object, BOOL toggle);
		void SET_WEAPON_IMPACTS_APPLY_GREATER_FORCE(Object object, BOOL p1);
		BOOL GET_IS_ARTICULATED_JOINT_AT_MIN_ANGLE(Object object, Any p1);
		BOOL GET_IS_ARTICULATED_JOINT_AT_MAX_ANGLE(Any p0, Any p1);
		void SET_IS_OBJECT_ARTICULATED(Object object, BOOL toggle);
		void SET_IS_OBJECT_BALL(Object object, BOOL toggle);

	} // namespace OBJECT

	namespace PAD {

		/**
		 * control: 0: PLAYER_CONTROL, 1: CAMERA_CONTROL, 2: FRONTEND_CONTROL
		 * For more info, see https://docs.fivem.net/docs/game-references/controls/
		 */
		BOOL IS_CONTROL_ENABLED(int control, int action);
		/**
		 * Returns whether a control is currently pressed.
		 * control: see IS_CONTROL_ENABLED
		 */
		BOOL IS_CONTROL_PRESSED(int control, int action);
		/**
		 * Returns whether a control is currently _not_ pressed.
		 * control: see IS_CONTROL_ENABLED
		 */
		BOOL IS_CONTROL_RELEASED(int control, int action);
		/**
		 * Returns whether a control was newly pressed since the last check.
		 * control: see IS_CONTROL_ENABLED
		 */
		BOOL IS_CONTROL_JUST_PRESSED(int control, int action);
		/**
		 * Returns whether a control was newly released since the last check.
		 * control: see IS_CONTROL_ENABLED
		 */
		BOOL IS_CONTROL_JUST_RELEASED(int control, int action);
		void _IS_CONTROL_HELD_DOWN(int control, int action, int duration); // Missing in crossmap
		/**
		 * control: see IS_CONTROL_ENABLED
		 */
		int GET_CONTROL_VALUE(int control, int action);
		/**
		 * Returns the value of GET_CONTROL_VALUE normalized (i.e. a real number value between -1 and 1)
		 * 
		 * control: see IS_CONTROL_ENABLED
		 */
		float GET_CONTROL_NORMAL(int control, int action);
		void SET_USE_ADJUSTED_MOUSE_COORDS(BOOL toggle);
		/**
		 * Seems to return values between -1 and 1 for controls like gas and steering.
		 * 
		 * control: see IS_CONTROL_ENABLED
		 */
		float GET_CONTROL_UNBOUND_NORMAL(int control, int action);
		/**
		 * This is for simulating player input.
		 * value is a float value from 0 - 1
		 * 
		 * control: see IS_CONTROL_ENABLED
		 */
		BOOL SET_CONTROL_VALUE_NEXT_FRAME(int control, int action, float value);
		/**
		 * control: see IS_CONTROL_ENABLED
		 */
		BOOL IS_DISABLED_CONTROL_PRESSED(int control, int action);
		/**
		 * control: see IS_CONTROL_ENABLED
		 */
		BOOL IS_DISABLED_CONTROL_RELEASED(int control, int action);
		/**
		 * control: see IS_CONTROL_ENABLED
		 */
		BOOL IS_DISABLED_CONTROL_JUST_PRESSED(int control, int action);
		/**
		 * control: see IS_CONTROL_ENABLED
		 */
		BOOL IS_DISABLED_CONTROL_JUST_RELEASED(int control, int action);
		/**
		 * control: see IS_CONTROL_ENABLED
		 */
		float GET_DISABLED_CONTROL_NORMAL(int control, int action);
		/**
		 * The "disabled" variant of GET_CONTROL_UNBOUND_NORMAL.
		 * 
		 * control: see IS_CONTROL_ENABLED
		 */
		float GET_DISABLED_CONTROL_UNBOUND_NORMAL(int control, int action);
		/**
		 * Returns time in ms since last input.
		 * 
		 * control: see IS_CONTROL_ENABLED
		 */
		int GET_CONTROL_HOW_LONG_AGO(int control);
		/**
		 * control: unused parameter
		 */
		BOOL IS_USING_KEYBOARD_AND_MOUSE(int control);
		/**
		 * control: see IS_CONTROL_ENABLED
		 */
		BOOL IS_USING_CURSOR(int control);
		BOOL SET_CURSOR_POSITION(float x, float y);
		/**
		 * control: see IS_CONTROL_ENABLED
		 * 
		 * Hardcoded to return false.
		 */
		BOOL IS_USING_REMOTE_PLAY(int control);
		/**
		 * control: unused parameter
		 */
		BOOL HAVE_CONTROLS_CHANGED(int control);
		/**
		 * allowXOSwap appears to always be true.
		 * 
		 * EG:
		 * GET_CONTROL_INSTRUCTIONAL_BUTTON (2, 201, 1) /*INPUT_FRONTEND_ACCEPT (e.g. Enter button)* /
		 * GET_CONTROL_INSTRUCTIONAL_BUTTON (2, 202, 1) /*INPUT_FRONTEND_CANCEL (e.g. ESC button)* /
		 * GET_CONTROL_INSTRUCTIONAL_BUTTON (2, 51, 1) /*INPUT_CONTEXT (e.g. E button)* /
		 * 
		 * gtaforums.com/topic/819070-c-draw-instructional-buttons-scaleform-movie/#entry1068197378
		 * 
		 * control: unused parameter
		 */
		const char* GET_CONTROL_INSTRUCTIONAL_BUTTONS_STRING(int control, int action, BOOL allowXOSwap);
		/**
		 * control: unused parameter
		 */
		const char* GET_CONTROL_GROUP_INSTRUCTIONAL_BUTTONS_STRING(int control, int controlGroup, BOOL allowXOSwap);
		/**
		 * control: see IS_CONTROL_ENABLED
		 */
		void SET_CONTROL_LIGHT_EFFECT_COLOR(int control, int red, int green, int blue);
		/**
		 * control: see IS_CONTROL_ENABLED
		 */
		void CLEAR_CONTROL_LIGHT_EFFECT(int control);
		/**
		 * control: see IS_CONTROL_ENABLED
		 * duration in milliseconds
		 * frequency should range from about 10 (slow vibration) to 255 (very fast)
		 * 
		 * example:
		 * SET_CONTROL_SHAKE(PLAYER_CONTROL, 100, 200);
		 */
		void SET_CONTROL_SHAKE(int control, int duration, int frequency);
		/**
		 * Does nothing (it's a nullsub).
		 */
		void SET_CONTROL_TRIGGER_SHAKE(int control, int leftDuration, int leftFrequency, int rightDuration, int rightFrequency);
		/**
		 * control: see IS_CONTROL_ENABLED
		 */
		void STOP_CONTROL_SHAKE(int control);
		/**
		 * control: see IS_CONTROL_ENABLED
		 */
		void SET_CONTROL_SHAKE_SUPPRESSED_ID(int control, int uniqueId);
		/**
		 * control: see IS_CONTROL_ENABLED
		 */
		void CLEAR_CONTROL_SHAKE_SUPPRESSED_ID(int control);
		BOOL IS_LOOK_INVERTED();
		/**
		 * Used with IS_LOOK_INVERTED() and negates its affect.
		 * 
		 * --
		 * 
		 * Not sure how the person above got that description, but here's an actual example:
		 * 
		 * if (PAD::IS_USING_KEYBOARD_AND_MOUSE(2)) {
		 * if (a_5) {
		 * if (PAD::IS_LOOK_INVERTED()) {
		 * a_3 *= -1;
		 * }
		 * if (PAD::IS_MOUSE_LOOK_INVERTED()) {
		 * a_3 *= -1;
		 * }
		 * }
		 * }
		 */
		BOOL IS_MOUSE_LOOK_INVERTED();
		/**
		 * Hard-coded to return 3 if using KBM, otherwise same behavior as GET_LOCAL_PLAYER_GAMEPAD_AIM_STATE.
		 */
		int GET_LOCAL_PLAYER_AIM_STATE();
		/**
		 * Returns the local player's targeting mode. See PLAYER::SET_PLAYER_TARGETING_MODE.
		 */
		int GET_LOCAL_PLAYER_GAMEPAD_AIM_STATE();
		BOOL GET_IS_USING_ALTERNATE_HANDBRAKE();
		/**
		 * Returns profile setting 225.
		 */
		BOOL GET_IS_USING_ALTERNATE_DRIVEBY();
		/**
		 * Returns profile setting 17.
		 */
		BOOL GET_ALLOW_MOVEMENT_WHILE_ZOOMED();
		void SET_PLAYERPAD_SHAKES_WHEN_CONTROLLER_DISABLED(BOOL toggle);
		/**
		 * control: see IS_CONTROL_ENABLED
		 */
		void SET_INPUT_EXCLUSIVE(int control, int action);
		/**
		 * control: see IS_CONTROL_ENABLED
		 */
		void DISABLE_CONTROL_ACTION(int control, int action, BOOL disableRelatedActions);
		/**
		 * control: see IS_CONTROL_ENABLED
		 */
		void ENABLE_CONTROL_ACTION(int control, int action, BOOL enableRelatedActions);
		/**
		 * control: see IS_CONTROL_ENABLED
		 */
		void DISABLE_ALL_CONTROL_ACTIONS(int control);
		/**
		 * control: see IS_CONTROL_ENABLED
		 */
		void ENABLE_ALL_CONTROL_ACTIONS(int control);
		/**
		 * Used in carsteal3 script with schemeName = "Carsteal4_spycar".
		 */
		BOOL INIT_PC_SCRIPTED_CONTROLS(const char* schemeName);
		/**
		 * Same as INIT_PC_SCRIPTED_CONTROLS
		 */
		BOOL SWITCH_PC_SCRIPTED_CONTROLS(const char* schemeName);
		void SHUTDOWN_PC_SCRIPTED_CONTROLS();
		/**
		 * control: see IS_CONTROL_ENABLED
		 */
		void ALLOW_ALTERNATIVE_SCRIPT_CONTROLS_LAYOUT(int control);
		/**
		 * Returns 0 (default) or 5 (alternate/DualSense).
		 */
		void _GET_GAMEPAD_TYPE(); // Missing in crossmap

	} // namespace PAD

	namespace PATHFIND {

		/**
		 * When nodeEnabled is set to false, all nodes in the area get disabled.
		 * `GET_VEHICLE_NODE_IS_SWITCHED_OFF` returns true afterwards.
		 * If it's true, `GET_VEHICLE_NODE_IS_SWITCHED_OFF` returns false.
		 */
		void SET_ROADS_IN_AREA(float x1, float y1, float z1, float x2, float y2, float z2, BOOL nodeEnabled, BOOL unknown2);
		/**
		 * unknown3 is related to `SEND_SCRIPT_WORLD_STATE_EVENT > CNetworkRoadNodeWorldStateData` in networked environments.
		 * See IS_POINT_IN_ANGLED_AREA for the definition of an angled area.
		 */
		void SET_ROADS_IN_ANGLED_AREA(float x1, float y1, float z1, float x2, float y2, float z2, float width, BOOL unknown1, BOOL unknown2, BOOL unknown3);
		void SET_PED_PATHS_IN_AREA(float x1, float y1, float z1, float x2, float y2, float z2, BOOL p6, Any p7);
		/**
		 * Flags are:
		 * 1 = 1 = B02_IsFootpath
		 * 2 = 4 = !B15_InteractionUnk
		 * 4 = 0x20 = !B14_IsInterior
		 * 8 = 0x40 = !B07_IsWater
		 * 16 = 0x200 = B17_IsFlatGround
		 * When onGround == true outPosition is a position located on the nearest pavement.
		 * 
		 * When a safe coord could not be found the result of a function is false and outPosition == Vector3.Zero.
		 * 
		 * In the scripts these flags are used: 0, 14, 12, 16, 20, 21, 28. 0 is most commonly used, then 16.
		 * 
		 * 16 works for me, 0 crashed the script.
		 */
		BOOL GET_SAFE_COORD_FOR_PED(float x, float y, float z, BOOL onGround, Vector3* outPosition, int flags);
		/**
		 * https://gtaforums.com/topic/843561-pathfind-node-types
		 */
		BOOL GET_CLOSEST_VEHICLE_NODE(float x, float y, float z, Vector3* outPosition, int nodeFlags, float p5, float p6);
		/**
		 * Get the closest vehicle node to a given position.
		 */
		BOOL GET_CLOSEST_MAJOR_VEHICLE_NODE(float x, float y, float z, Vector3* outPosition, float unknown1, float unknown2);
		/**
		 * p5, p6 and p7 seems to be about the same as p4, p5 and p6 for GET_CLOSEST_VEHICLE_NODE. p6 and/or p7 has something to do with finding a node on the same path/road and same direction(at least for this native, something to do with the heading maybe). Edit this when you find out more.
		 * 
		 * nodeType: 0 = main roads, 1 = any dry path, 3 = water
		 * p6 is always 3.0
		 * p7 is always 0
		 * 
		 * gtaforums.com/topic/843561-pathfind-node-types
		 * 
		 * Example of usage, moving vehicle to closest path/road:
		 * Vector3 coords = ENTITY::GET_ENTITY_COORDS(playerVeh, true);
		 * Vector3 closestVehicleNodeCoords;
		 * float roadHeading;
		 * PATHFIND::GET_CLOSEST_VEHICLE_NODE_WITH_HEADING(coords.x, coords.y, coords.z, &closestVehicleNodeCoords, &roadHeading, 1, 3, 0);
		 * ENTITY::SET_ENTITY_HEADING(playerVeh, roadHeading);
		 * ENTITY::SET_ENTITY_COORDS(playerVeh, closestVehicleNodeCoords.x, closestVehicleNodeCoords.y, closestVehicleNodeCoords.z, 1, 0, 0, 1);
		 * VEHICLE::SET_VEHICLE_ON_GROUND_PROPERLY(playerVeh);
		 * 
		 * ------------------------------------------------------------------
		 * C# Example (ins1de) : https://pastebin.com/fxtMWAHD
		 */
		BOOL GET_CLOSEST_VEHICLE_NODE_WITH_HEADING(float x, float y, float z, Vector3* outPosition, float* outHeading, int nodeType, float p6, float p7);
		BOOL GET_NTH_CLOSEST_VEHICLE_NODE(float x, float y, float z, int nthClosest, Vector3* outPosition, int nodeFlags, float unknown1, float unknown2);
		/**
		 * Returns the id.
		 */
		int GET_NTH_CLOSEST_VEHICLE_NODE_ID(float x, float y, float z, int nth, int nodeFlags, float p5, float p6);
		/**
		 * Get the nth closest vehicle node and its heading.
		 */
		BOOL GET_NTH_CLOSEST_VEHICLE_NODE_WITH_HEADING(float x, float y, float z, int nthClosest, Vector3* outPosition, float* outHeading, int* outNumLanes, int nodeFlags, float unknown3, float unknown4);
		int GET_NTH_CLOSEST_VEHICLE_NODE_ID_WITH_HEADING(float x, float y, float z, int nthClosest, Vector3* outPosition, float* outHeading, int nodeFlags, float p7, float p8);
		/**
		 * See gtaforums.com/topic/843561-pathfind-node-types for node type info. 0 = paved road only, 1 = any road, 3 = water
		 * 
		 * p10 always equals 3.0
		 * p11 always equals 0
		 */
		BOOL GET_NTH_CLOSEST_VEHICLE_NODE_FAVOUR_DIRECTION(float x, float y, float z, float desiredX, float desiredY, float desiredZ, int nthClosest, Vector3* outPosition, float* outHeading, int nodeFlags, float p10, float p11);
		/**
		 * Gets the density and flags of the closest node to the specified position.
		 * Density is a value between 0 and 15, indicating how busy the road is.
		 * Flags is a bit field.
		 */
		BOOL GET_VEHICLE_NODE_PROPERTIES(float x, float y, float z, int* density, int* flags);
		/**
		 * Returns true if the id is non zero.
		 */
		BOOL IS_VEHICLE_NODE_ID_VALID(int vehicleNodeId);
		/**
		 * Calling this with an invalid node id, will crash the game.
		 * Note that IS_VEHICLE_NODE_ID_VALID simply checks if nodeId is not zero. It does not actually ensure that the id is valid.
		 * Eg. IS_VEHICLE_NODE_ID_VALID(1) will return true, but will crash when calling GET_VEHICLE_NODE_POSITION().
		 */
		void GET_VEHICLE_NODE_POSITION(int nodeId, Vector3* outPosition);
		/**
		 * Returns false for nodes that aren't used for GPS routes.
		 * Example:
		 * Nodes in Fort Zancudo and LSIA are false
		 */
		BOOL GET_VEHICLE_NODE_IS_GPS_ALLOWED(int nodeID);
		/**
		 * Returns true when the node is Offroad. Alleys, some dirt roads, and carparks return true.
		 * Normal roads where plenty of Peds spawn will return false
		 */
		BOOL GET_VEHICLE_NODE_IS_SWITCHED_OFF(int nodeID);
		/**
		 * p1 seems to be always 1.0f in the scripts
		 */
		BOOL GET_CLOSEST_ROAD(float x, float y, float z, float p3, int p4, Vector3* p5, Vector3* p6, Any* p7, Any* p8, float* p9, BOOL p10);
		BOOL LOAD_ALL_PATH_NODES(BOOL set);
		void SET_ALLOW_STREAM_PROLOGUE_NODES(BOOL toggle);
		/**
		 * Activates Cayo Perico path nodes if passed `1`. GPS navigation will start working, maybe more stuff will change, not sure. It seems if you try to unload (pass `0`) when close to the island, your game might crash.
		 */
		void SET_ALLOW_STREAM_HEIST_ISLAND_NODES(int type);
		BOOL ARE_NODES_LOADED_FOR_AREA(float x1, float y1, float x2, float y2);
		/**
		 * Used internally for long range tasks
		 */
		BOOL REQUEST_PATH_NODES_IN_AREA_THIS_FRAME(float x1, float y1, float x2, float y2);
		void SET_ROADS_BACK_TO_ORIGINAL(float p0, float p1, float p2, float p3, float p4, float p5, Any p6);
		/**
		 * See IS_POINT_IN_ANGLED_AREA for the definition of an angled area.
		 * bool p7 - always 1
		 */
		void SET_ROADS_BACK_TO_ORIGINAL_IN_ANGLED_AREA(float x1, float y1, float z1, float x2, float y2, float z2, float width, Any p7);
		void SET_AMBIENT_PED_RANGE_MULTIPLIER_THIS_FRAME(float multiplier);
		void ADJUST_AMBIENT_PED_SPAWN_DENSITIES_THIS_FRAME(Any p0, Any p1, Any p2, Any p3, Any p4, Any p5, Any p6);
		/**
		 * p6 is always 0
		 */
		void SET_PED_PATHS_BACK_TO_ORIGINAL(float x1, float y1, float z1, float x2, float y2, float z2, Any p6);
		BOOL GET_RANDOM_VEHICLE_NODE(float x, float y, float z, float radius, BOOL p4, BOOL p5, BOOL p6, Vector3* outPosition, int* nodeId);
		void GET_SPAWN_COORDS_FOR_VEHICLE_NODE(int nodeAddress, float towardsCoorsX, float towardsCoorsY, float towardsCoorsZ, Vector3* centrePoint, float* heading);
		/**
		 * Determines the name of the street which is the closest to the given coordinates.
		 * 
		 * x,y,z - the coordinates of the street
		 * streetName - returns a hash to the name of the street the coords are on
		 * crossingRoad - if the coordinates are on an intersection, a hash to the name of the crossing road
		 * 
		 * Note: the names are returned as hashes, the strings can be returned using the function HUD::GET_STREET_NAME_FROM_HASH_KEY.
		 */
		void GET_STREET_NAME_AT_COORD(float x, float y, float z, Hash* streetName, Hash* crossingRoad);
		/**
		 * p3 is 0 in the only game script occurrence (trevor3) but 1 doesn't seem to make a difference
		 * 
		 * distToNxJunction seems to be the distance in metres * 10.0f
		 * 
		 * direction:
		 * 0 = This happens randomly during the drive for seemingly no reason but if you consider that this native is only used in trevor3, it seems to mean "Next frame, stop whatever's being said and tell the player the direction."
		 * 1 = Route is being calculated or the player is going in the wrong direction
		 * 2 = Please Proceed the Highlighted Route
		 * 3 = In (distToNxJunction) Turn Left
		 * 4 = In (distToNxJunction) Turn Right
		 * 5 = In (distToNxJunction) Keep Straight
		 * 6 = In (distToNxJunction) Turn Sharply To The Left
		 * 7 = In (distToNxJunction) Turn Sharply To The Right
		 * 8 = Route is being recalculated or the navmesh is confusing. This happens randomly during the drive but consistently at {2044.0358, 2996.6116, 44.9717} if you face towards the bar and the route needs you to turn right. In that particular case, it could be a bug with how the turn appears to be 270 deg. CCW instead of "right." Either way, this seems to be the engine saying "I don't know the route right now."
		 * 
		 * return value set to 0 always
		 */
		int GENERATE_DIRECTIONS_TO_COORD(float x, float y, float z, BOOL p3, int* direction, float* p5, float* distToNxJunction);
		void SET_IGNORE_NO_GPS_FLAG(BOOL toggle);
		/**
		 * See: SET_BLIP_ROUTE
		 */
		void SET_IGNORE_NO_GPS_FLAG_UNTIL_FIRST_NORMAL_NODE(BOOL toggle);
		void SET_GPS_DISABLED_ZONE(float x1, float y1, float z1, float x2, float y2, float z3);
		int GET_GPS_BLIP_ROUTE_LENGTH();
		/**
		 * p3 can be 0, 1 or 2.
		 */
		BOOL GET_POS_ALONG_GPS_TYPE_ROUTE(Vector3* result, BOOL p1, float p2, int p3);
		BOOL GET_GPS_BLIP_ROUTE_FOUND();
		BOOL GET_ROAD_BOUNDARY_USING_HEADING(float x, float y, float z, float heading, Vector3* outPosition);
		BOOL GET_POSITION_BY_SIDE_OF_ROAD(float x, float y, float z, int p3, Vector3* outPosition);
		/**
		 * Gets a value indicating whether the specified position is on a road.
		 * The vehicle parameter is not implemented (ignored).
		 */
		BOOL IS_POINT_ON_ROAD(float x, float y, float z, Vehicle vehicle);
		/**
		 * Gets the next zone that has been disabled using SET_GPS_DISABLED_ZONE_AT_INDEX.
		 */
		int GET_NEXT_GPS_DISABLED_ZONE_INDEX();
		/**
		 * Disables the GPS route displayed on the minimap while within a certain zone (area). When in a disabled zone and creating a waypoint, the GPS route is not shown on the minimap until you are outside of the zone. When disabled, the direct distance is shown on minimap opposed to distance to travel. Seems to only work before setting a waypoint.
		 * You can clear the disabled zone with CLEAR_GPS_DISABLED_ZONE_AT_INDEX.
		 * 
		 * **Setting a waypoint at the same coordinate:**
		 * Disabled Zone: https://i.imgur.com/P9VUuxM.png
		 * Enabled Zone (normal): https://i.imgur.com/BPi24aw.png
		 */
		void SET_GPS_DISABLED_ZONE_AT_INDEX(float x1, float y1, float z1, float x2, float y2, float z2, int index);
		/**
		 * Clears a disabled GPS route area from a certain index previously set using `SET_GPS_DISABLED_ZONE_AT_INDEX`.
		 */
		void CLEAR_GPS_DISABLED_ZONE_AT_INDEX(int index);
		void ADD_NAVMESH_REQUIRED_REGION(float x, float y, float radius);
		void REMOVE_NAVMESH_REQUIRED_REGIONS();
		BOOL IS_NAVMESH_REQUIRED_REGION_IN_USE();
		/**
		 * Set toggle true to disable navmesh.
		 * Set toggle false to enable navmesh.
		 */
		void DISABLE_NAVMESH_IN_AREA(float x1, float y1, float z1, float x2, float y2, float z2, BOOL toggle);
		BOOL ARE_ALL_NAVMESH_REGIONS_LOADED();
		/**
		 * Returns whether navmesh for the region is loaded. The region is a rectangular prism defined by it's top left deepest corner to it's bottom right shallowest corner.
		 * 
		 * If you can re-word this so it makes more sense, please do. I'm horrible with words sometimes...
		 */
		BOOL IS_NAVMESH_LOADED_IN_AREA(float x1, float y1, float z1, float x2, float y2, float z2);
		int GET_NUM_NAVMESHES_EXISTING_IN_AREA(float x1, float y1, float z1, float x2, float y2, float z2);
		int ADD_NAVMESH_BLOCKING_OBJECT(float p0, float p1, float p2, float p3, float p4, float p5, float p6, BOOL p7, Any p8);
		void UPDATE_NAVMESH_BLOCKING_OBJECT(Any p0, float p1, float p2, float p3, float p4, float p5, float p6, float p7, Any p8);
		void REMOVE_NAVMESH_BLOCKING_OBJECT(Any p0);
		BOOL DOES_NAVMESH_BLOCKING_OBJECT_EXIST(Any p0);
		/**
		 * Returns CGameWorldHeightMap's maximum Z value at specified point (grid node).
		 */
		float GET_APPROX_HEIGHT_FOR_POINT(float x, float y);
		/**
		 * Returns CGameWorldHeightMap's maximum Z among all grid nodes that intersect with the specified rectangle.
		 */
		float GET_APPROX_HEIGHT_FOR_AREA(float x1, float y1, float x2, float y2);
		/**
		 * Returns CGameWorldHeightMap's minimum Z value at specified point (grid node).
		 */
		float GET_APPROX_FLOOR_FOR_POINT(float x, float y);
		/**
		 * Returns CGameWorldHeightMap's minimum Z among all grid nodes that intersect with the specified rectangle.
		 */
		float GET_APPROX_FLOOR_FOR_AREA(float x1, float y1, float x2, float y2);
		/**
		 * Calculates the travel distance between a set of points.
		 * 
		 * Doesn't seem to correlate with distance on gps sometimes.
		 * This function returns the value 100000.0 over long distances, seems to be a failure mode result, potentially occurring when not all path nodes are loaded into pathfind.
		 */
		float CALCULATE_TRAVEL_DISTANCE_BETWEEN_POINTS(float x1, float y1, float z1, float x2, float y2, float z2);

	} // namespace PATHFIND

	namespace PED {

		/**
		 * https://alloc8or.re/gta5/doc/enums/ePedType.txt
		 * 
		 * Full list of peds by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/peds.json
		 */
		Ped CREATE_PED(int pedType, Hash modelHash, float x, float y, float z, float heading, BOOL isNetwork, BOOL bScriptHostPed);
		/**
		 * Deletes the specified ped, then sets the handle pointed to by the pointer to NULL.
		 */
		void DELETE_PED(Ped* ped);
		Ped CLONE_PED(Ped ped, BOOL isNetwork, BOOL bScriptHostPed, BOOL copyHeadBlendFlag);
		Ped CLONE_PED_ALT(Ped ped, BOOL isNetwork, BOOL bScriptHostPed, BOOL copyHeadBlendFlag, BOOL p4);
		/**
		 * Copies ped's components and props to targetPed.
		 */
		void CLONE_PED_TO_TARGET(Ped ped, Ped targetPed);
		void CLONE_PED_TO_TARGET_ALT(Ped ped, Ped targetPed, BOOL p2);
		/**
		 * Gets a value indicating whether the specified ped is in the specified vehicle.
		 * 
		 * If 'atGetIn' is false, the function will not return true until the ped is sitting in the vehicle and is about to close the door. If it's true, the function returns true the moment the ped starts to get onto the seat (after opening the door). Eg. if false, and the ped is getting into a submersible, the function will not return true until the ped has descended down into the submersible and gotten into the seat, while if it's true, it'll return true the moment the hatch has been opened and the ped is about to descend into the submersible.
		 */
		BOOL IS_PED_IN_VEHICLE(Ped ped, Vehicle vehicle, BOOL atGetIn);
		BOOL IS_PED_IN_MODEL(Ped ped, Hash modelHash);
		/**
		 * Gets a value indicating whether the specified ped is in any vehicle.
		 * 
		 * If 'atGetIn' is false, the function will not return true until the ped is sitting in the vehicle and is about to close the door. If it's true, the function returns true the moment the ped starts to get onto the seat (after opening the door). Eg. if false, and the ped is getting into a submersible, the function will not return true until the ped has descended down into the submersible and gotten into the seat, while if it's true, it'll return true the moment the hatch has been opened and the ped is about to descend into the submersible.
		 */
		BOOL IS_PED_IN_ANY_VEHICLE(Ped ped, BOOL atGetIn);
		/**
		 * xyz - relative to the world origin.
		 */
		BOOL IS_COP_PED_IN_AREA_3D(float x1, float y1, float z1, float x2, float y2, float z2);
		/**
		 * Gets a value indicating whether this ped's health is below its injured threshold.
		 * 
		 * The default threshold is 100.
		 */
		BOOL IS_PED_INJURED(Ped ped);
		/**
		 * Returns whether the specified ped is hurt.
		 */
		BOOL IS_PED_HURT(Ped ped);
		/**
		 * Gets a value indicating whether this ped's health is below its fatally injured threshold. The default threshold is 100.
		 * If the handle is invalid, the function returns true.
		 */
		BOOL IS_PED_FATALLY_INJURED(Ped ped);
		/**
		 * Seems to consistently return true if the ped is dead.
		 * 
		 * p1 is always passed 1 in the scripts.
		 * 
		 * I suggest to remove "OR_DYING" part, because it does not detect dying phase.
		 * 
		 * That's what the devs call it, cry about it.
		 * 
		 * lol
		 */
		BOOL IS_PED_DEAD_OR_DYING(Ped ped, BOOL p1);
		BOOL IS_CONVERSATION_PED_DEAD(Ped ped);
		BOOL IS_PED_AIMING_FROM_COVER(Ped ped);
		/**
		 * Returns whether the specified ped is reloading.
		 */
		BOOL IS_PED_RELOADING(Ped ped);
		/**
		 * Returns true if the given ped has a valid pointer to CPlayerInfo in its CPed class. That's all.
		 */
		BOOL IS_PED_A_PLAYER(Ped ped);
		/**
		 * pedType: see CREATE_PED
		 * 
		 * Full list of peds by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/peds.json
		 */
		Ped CREATE_PED_INSIDE_VEHICLE(Vehicle vehicle, int pedType, Hash modelHash, int seat, BOOL isNetwork, BOOL bScriptHostPed);
		void SET_PED_DESIRED_HEADING(Ped ped, float heading);
		void FORCE_ALL_HEADING_VALUES_TO_ALIGN(Ped ped);
		/**
		 * angle is ped's view cone
		 */
		BOOL IS_PED_FACING_PED(Ped ped, Ped otherPed, float angle);
		/**
		 * Notes: The function only returns true while the ped is:
		 * A.) Swinging a random melee attack (including pistol-whipping)
		 * 
		 * B.) Reacting to being hit by a melee attack (including pistol-whipping)
		 * 
		 * C.) Is locked-on to an enemy (arms up, strafing/skipping in the default fighting-stance, ready to dodge+counter).
		 * 
		 * You don't have to be holding the melee-targetting button to be in this stance; you stay in it by default for a few seconds after swinging at someone. If you do a sprinting punch, it returns true for the duration of the punch animation and then returns false again, even if you've punched and made-angry many peds
		 */
		BOOL IS_PED_IN_MELEE_COMBAT(Ped ped);
		/**
		 * Returns true if the ped doesn't do any movement. If the ped is being pushed forwards by using APPLY_FORCE_TO_ENTITY for example, the function returns false.
		 */
		BOOL IS_PED_STOPPED(Ped ped);
		BOOL IS_PED_SHOOTING_IN_AREA(Ped ped, float x1, float y1, float z1, float x2, float y2, float z2, BOOL p7, BOOL p8);
		BOOL IS_ANY_PED_SHOOTING_IN_AREA(float x1, float y1, float z1, float x2, float y2, float z2, BOOL p6, BOOL p7);
		/**
		 * Returns whether the specified ped is shooting.
		 */
		BOOL IS_PED_SHOOTING(Ped ped);
		/**
		 * accuracy = 0-100, 100 being perfectly accurate
		 */
		void SET_PED_ACCURACY(Ped ped, int accuracy);
		int GET_PED_ACCURACY(Ped ped);
		void SET_AMBIENT_LAW_PED_ACCURACY_MODIFIER(float multiplier);
		BOOL IS_PED_MODEL(Ped ped, Hash modelHash);
		/**
		 * Forces the ped to fall back and kills it.
		 * 
		 * It doesn't really explode the ped's head but it kills the ped
		 */
		void EXPLODE_PED_HEAD(Ped ped, Hash weaponHash);
		/**
		 * Judging purely from a quick disassembly, if the ped is in a vehicle, the ped will be deleted immediately. If not, it'll be marked as no longer needed. - very elegant..
		 */
		void REMOVE_PED_ELEGANTLY(Ped* ped);
		/**
		 * Same as SET_PED_ARMOUR, but ADDS 'amount' to the armor the Ped already has.
		 */
		void ADD_ARMOUR_TO_PED(Ped ped, int amount);
		/**
		 * Sets the armor of the specified ped.
		 * 
		 * ped: The Ped to set the armor of.
		 * amount: A value between 0 and 100 indicating the value to set the Ped's armor to.
		 */
		void SET_PED_ARMOUR(Ped ped, int amount);
		/**
		 * Ped: The ped to warp.
		 * vehicle: The vehicle to warp the ped into.
		 * Seat_Index: [-1 is driver seat, -2 first free passenger seat]
		 * 
		 * Moreinfo of Seat Index
		 * DriverSeat = -1
		 * Passenger = 0
		 * Left Rear = 1
		 * RightRear = 2
		 */
		void SET_PED_INTO_VEHICLE(Ped ped, Vehicle vehicle, int seatIndex);
		void SET_PED_ALLOW_VEHICLES_OVERRIDE(Ped ped, BOOL toggle);
		BOOL CAN_CREATE_RANDOM_PED(BOOL p0);
		/**
		 * vb.net
		 * Dim ped_handle As Integer
		 * With Game.Player.Character
		 * Dim pos As Vector3 = .Position + .ForwardVector * 3
		 * ped_handle = Native.Function.Call(Of Integer)(Hash.CREATE_RANDOM_PED, pos.X, pos.Y, pos.Z)
		 * End With
		 * 
		 * Creates a Ped at the specified location, returns the Ped Handle.
		 * Ped will not act until SET_PED_AS_NO_LONGER_NEEDED is called.
		 */
		Ped CREATE_RANDOM_PED(float posX, float posY, float posZ);
		Ped CREATE_RANDOM_PED_AS_DRIVER(Vehicle vehicle, BOOL returnHandle);
		BOOL CAN_CREATE_RANDOM_DRIVER();
		BOOL CAN_CREATE_RANDOM_BIKE_RIDER();
		void SET_PED_MOVE_ANIMS_BLEND_OUT(Ped ped);
		void SET_PED_CAN_BE_DRAGGED_OUT(Ped ped, BOOL toggle);
		/**
		 * ntoggle was always false except in one instance (b678).
		 * 
		 * The one time this is set to true seems to do with when you fail the mission.
		 */
		void SET_PED_ALLOW_HURT_COMBAT_FOR_ALL_MISSION_PEDS(BOOL toggle);
		/**
		 * Returns true/false if the ped is/isn't male.
		 */
		BOOL IS_PED_MALE(Ped ped);
		/**
		 * Returns true/false if the ped is/isn't humanoid.
		 */
		BOOL IS_PED_HUMAN(Ped ped);
		/**
		 * Gets the vehicle the specified Ped is in. Returns 0 if the ped is/was not in a vehicle.
		 */
		Vehicle GET_VEHICLE_PED_IS_IN(Ped ped, BOOL includeEntering);
		/**
		 * Resets the value for the last vehicle driven by the Ped.
		 */
		void RESET_PED_LAST_VEHICLE(Ped ped);
		void SET_PED_DENSITY_MULTIPLIER_THIS_FRAME(float multiplier);
		void SET_SCENARIO_PED_DENSITY_MULTIPLIER_THIS_FRAME(float p0, float p1);
		void SUPPRESS_AMBIENT_PED_AGGRESSIVE_CLEANUP_THIS_FRAME();
		void SET_SCRIPTED_CONVERSION_COORD_THIS_FRAME(float x, float y, float z);
		/**
		 * The distance between these points, is the diagonal of a box (remember it's 3D).
		 */
		void SET_PED_NON_CREATION_AREA(float x1, float y1, float z1, float x2, float y2, float z2);
		void CLEAR_PED_NON_CREATION_AREA();
		void INSTANTLY_FILL_PED_POPULATION();
		/**
		 * Same function call as PED::GET_MOUNT, aka just returns 0
		 */
		BOOL IS_PED_ON_MOUNT(Ped ped);
		/**
		 * 
		 * Function just returns 0
		 * void __fastcall ped__get_mount(NativeContext *a1)
		 * {
		 * NativeContext *v1; // rbx@1
		 * 
		 * v1 = a1;
		 * GetAddressOfPedFromScriptHandle(a1->Args->Arg1);
		 * v1->Returns->Item1= 0;
		 * }
		 */
		Ped GET_MOUNT(Ped ped);
		/**
		 * Gets a value indicating whether the specified ped is on top of any vehicle.
		 * 
		 * Return 1 when ped is on vehicle.
		 * Return 0 when ped is not on a vehicle.
		 * 
		 */
		BOOL IS_PED_ON_VEHICLE(Ped ped);
		BOOL IS_PED_ON_SPECIFIC_VEHICLE(Ped ped, Vehicle vehicle);
		/**
		 * Maximum possible amount of money on MP is 2000. ~JX
		 * 
		 * -----------------------------------------------------------------------------
		 * 
		 * Maximum amount that a ped can theoretically have is 65535 (0xFFFF) since the amount is stored as an unsigned short (uint16_t) value.
		 */
		void SET_PED_MONEY(Ped ped, int amount);
		int GET_PED_MONEY(Ped ped);
		/**
		 * Related to Peds dropping pickup_health_snack; p0 is a value between [0.0, 1.0] that corresponds to drop rate
		 */
		void SET_HEALTH_SNACKS_CARRIED_BY_ALL_NEW_PEDS(float p0, Any p1);
		void SET_AMBIENT_PEDS_DROP_MONEY(BOOL p0);
		void _SET_BLOCK_AMBIENT_PEDS_FROM_DROPPING_WEAPONS_THIS_FRAME();
		void SET_BLOCKING_OF_NON_TEMPORARY_EVENTS_FOR_AMBIENT_PEDS_THIS_FRAME(BOOL p0);
		/**
		 * Ped no longer takes critical damage modifiers if set to FALSE.
		 * Example: Headshotting a player no longer one shots them. Instead they will take the same damage as a torso shot.
		 */
		void SET_PED_SUFFERS_CRITICAL_HITS(Ped ped, BOOL toggle);
		void SET_PED_UPPER_BODY_DAMAGE_ONLY(Ped ped, BOOL toggle);
		/**
		 * Detect if ped is sitting in the specified vehicle
		 * [True/False]
		 */
		BOOL IS_PED_SITTING_IN_VEHICLE(Ped ped, Vehicle vehicle);
		/**
		 * Detect if ped is in any vehicle
		 * [True/False]
		 */
		BOOL IS_PED_SITTING_IN_ANY_VEHICLE(Ped ped);
		BOOL IS_PED_ON_FOOT(Ped ped);
		BOOL IS_PED_ON_ANY_BIKE(Ped ped);
		BOOL IS_PED_PLANTING_BOMB(Ped ped);
		Vector3 GET_DEAD_PED_PICKUP_COORDS(Ped ped, float p1, float p2);
		BOOL IS_PED_IN_ANY_BOAT(Ped ped);
		BOOL IS_PED_IN_ANY_SUB(Ped ped);
		BOOL IS_PED_IN_ANY_HELI(Ped ped);
		BOOL IS_PED_IN_ANY_PLANE(Ped ped);
		BOOL IS_PED_IN_FLYING_VEHICLE(Ped ped);
		void SET_PED_DIES_IN_WATER(Ped ped, BOOL toggle);
		BOOL GET_PED_DIES_IN_WATER(Ped ped);
		void SET_PED_DIES_IN_SINKING_VEHICLE(Ped ped, BOOL toggle);
		int GET_PED_ARMOUR(Ped ped);
		void SET_PED_STAY_IN_VEHICLE_WHEN_JACKED(Ped ped, BOOL toggle);
		void SET_PED_CAN_BE_SHOT_IN_VEHICLE(Ped ped, BOOL toggle);
		BOOL GET_PED_LAST_DAMAGE_BONE(Ped ped, int* outBone);
		void CLEAR_PED_LAST_DAMAGE_BONE(Ped ped);
		void SET_AI_WEAPON_DAMAGE_MODIFIER(float value);
		void RESET_AI_WEAPON_DAMAGE_MODIFIER();
		void SET_AI_MELEE_WEAPON_DAMAGE_MODIFIER(float modifier);
		void RESET_AI_MELEE_WEAPON_DAMAGE_MODIFIER();
		void SET_TREAT_AS_AMBIENT_PED_FOR_DRIVER_LOCKON(Ped ped, BOOL p1);
		void SET_PED_CAN_BE_TARGETTED(Ped ped, BOOL toggle);
		void SET_PED_CAN_BE_TARGETTED_BY_TEAM(Ped ped, int team, BOOL toggle);
		void SET_PED_CAN_BE_TARGETTED_BY_PLAYER(Ped ped, Player player, BOOL toggle);
		void SET_ALLOW_LOCKON_TO_PED_IF_FRIENDLY(Ped ped, BOOL toggle);
		void SET_USE_CAMERA_HEADING_FOR_DESIRED_DIRECTION_LOCK_ON_TEST(Ped ped, BOOL toggle);
		BOOL IS_PED_IN_ANY_POLICE_VEHICLE(Ped ped);
		void FORCE_PED_TO_OPEN_PARACHUTE(Ped ped);
		BOOL IS_PED_IN_PARACHUTE_FREE_FALL(Ped ped);
		BOOL IS_PED_FALLING(Ped ped);
		BOOL IS_PED_JUMPING(Ped ped);
		BOOL IS_PED_LANDING(Any p0);
		BOOL IS_PED_DOING_A_BEAST_JUMP(Any p0);
		BOOL IS_PED_CLIMBING(Ped ped);
		BOOL IS_PED_VAULTING(Ped ped);
		BOOL IS_PED_DIVING(Ped ped);
		BOOL IS_PED_JUMPING_OUT_OF_VEHICLE(Ped ped);
		/**
		 * Returns true if the ped is currently opening a door (CTaskOpenDoor).
		 */
		BOOL IS_PED_OPENING_DOOR(Ped ped);
		/**
		 * Returns:
		 * 
		 * -1: Normal
		 * 0: Wearing parachute on back
		 * 1: Parachute opening
		 * 2: Parachute open
		 * 3: Falling to doom (e.g. after exiting parachute)
		 * 
		 * Normal means no parachute?
		 */
		int GET_PED_PARACHUTE_STATE(Ped ped);
		/**
		 * -1: no landing
		 * 0: landing on both feet
		 * 1: stumbling
		 * 2: rolling
		 * 3: ragdoll
		 */
		int GET_PED_PARACHUTE_LANDING_TYPE(Ped ped);
		void SET_PED_PARACHUTE_TINT_INDEX(Ped ped, int tintIndex);
		void GET_PED_PARACHUTE_TINT_INDEX(Ped ped, int* outTintIndex);
		void SET_PED_RESERVE_PARACHUTE_TINT_INDEX(Ped ped, Any p1);
		Object CREATE_PARACHUTE_BAG_OBJECT(Ped ped, BOOL p1, BOOL p2);
		/**
		 * This is the SET_CHAR_DUCKING from GTA IV, that makes Peds duck. This function does nothing in GTA V. It cannot set the ped as ducking in vehicles, and IS_PED_DUCKING will always return false.
		 */
		void SET_PED_DUCKING(Ped ped, BOOL toggle);
		BOOL IS_PED_DUCKING(Ped ped);
		BOOL IS_PED_IN_ANY_TAXI(Ped ped);
		void SET_PED_ID_RANGE(Ped ped, float value);
		void SET_PED_HIGHLY_PERCEPTIVE(Ped ped, BOOL toggle);
		void SET_COP_PERCEPTION_OVERRIDES(float seeingRange, float seeingRangePeripheral, float hearingRange, float visualFieldMinAzimuthAngle, float visualFieldMaxAzimuthAngle, float fieldOfGazeMaxAngle, float p6);
		void SET_PED_INJURED_ON_GROUND_BEHAVIOUR(Ped ped, float p1);
		void DISABLE_PED_INJURED_ON_GROUND_BEHAVIOUR(Ped ped);
		void SET_PED_SEEING_RANGE(Ped ped, float value);
		void SET_PED_HEARING_RANGE(Ped ped, float value);
		void SET_PED_VISUAL_FIELD_MIN_ANGLE(Ped ped, float value);
		void SET_PED_VISUAL_FIELD_MAX_ANGLE(Ped ped, float value);
		/**
		 * This native refers to the field of vision the ped has below them, starting at 0 degrees. The angle value should be negative.
		 * -90f should let the ped see 90 degrees below them, for example.
		 */
		void SET_PED_VISUAL_FIELD_MIN_ELEVATION_ANGLE(Ped ped, float angle);
		/**
		 * This native refers to the field of vision the ped has above them, starting at 0 degrees. 90f would let the ped see enemies directly above of them.
		 */
		void SET_PED_VISUAL_FIELD_MAX_ELEVATION_ANGLE(Ped ped, float angle);
		void SET_PED_VISUAL_FIELD_PERIPHERAL_RANGE(Ped ped, float range);
		void SET_PED_VISUAL_FIELD_CENTER_ANGLE(Ped ped, float angle);
		float GET_PED_VISUAL_FIELD_CENTER_ANGLE(Ped ped);
		/**
		 * p1 is usually 0 in the scripts. action is either 0 or a pointer to "DEFAULT_ACTION".
		 */
		void SET_PED_STEALTH_MOVEMENT(Ped ped, BOOL p1, const char* action);
		/**
		 * Returns whether the entity is in stealth mode
		 */
		BOOL GET_PED_STEALTH_MOVEMENT(Ped ped);
		/**
		 * Creates a new ped group.
		 * Groups can contain up to 8 peds.
		 * 
		 * The parameter is unused.
		 * 
		 * Returns a handle to the created group, or 0 if a group couldn't be created.
		 */
		int CREATE_GROUP(int unused);
		void SET_PED_AS_GROUP_LEADER(Ped ped, int groupId);
		void SET_PED_AS_GROUP_MEMBER(Ped ped, int groupId);
		/**
		 * This only will teleport the ped to the group leader if the group leader teleports (sets coords).
		 * 
		 * Only works in singleplayer
		 */
		void SET_PED_CAN_TELEPORT_TO_GROUP_LEADER(Ped pedHandle, int groupHandle, BOOL toggle);
		void REMOVE_GROUP(int groupId);
		void REMOVE_PED_FROM_GROUP(Ped ped);
		BOOL IS_PED_GROUP_MEMBER(Ped ped, int groupId);
		BOOL IS_PED_HANGING_ON_TO_VEHICLE(Ped ped);
		/**
		 * Sets the range at which members will automatically leave the group.
		 */
		void SET_GROUP_SEPARATION_RANGE(int groupHandle, float separationRange);
		/**
		 * Ped will stay on the ground after being stunned for at lest ms time. (in milliseconds)
		 */
		void SET_PED_MIN_GROUND_TIME_FOR_STUNGUN(Ped ped, int ms);
		BOOL IS_PED_PRONE(Ped ped);
		/**
		 * Checks to see if ped and target are in combat with eachother. Only goes one-way: if target is engaged in combat with ped but ped has not yet reacted, the function will return false until ped starts fighting back.
		 * 
		 * p1 is usually 0 in the scripts because it gets the ped id during the task sequence. For instance: PED::IS_PED_IN_COMBAT(l_42E[4/*14* /], PLAYER::PLAYER_PED_ID()) // armenian2.ct4: 43794
		 */
		BOOL IS_PED_IN_COMBAT(Ped ped, Ped target);
		Entity GET_PED_TARGET_FROM_COMBAT_PED(Ped ped, Any p1);
		BOOL CAN_PED_IN_COMBAT_SEE_TARGET(Ped ped, Ped target);
		BOOL IS_PED_DOING_DRIVEBY(Ped ped);
		BOOL IS_PED_JACKING(Ped ped);
		BOOL IS_PED_BEING_JACKED(Ped ped);
		/**
		 * p1 is always 0
		 */
		BOOL IS_PED_BEING_STUNNED(Ped ped, int p1);
		Ped GET_PEDS_JACKER(Ped ped);
		Ped GET_JACK_TARGET(Ped ped);
		BOOL IS_PED_FLEEING(Ped ped);
		/**
		 * p1 is nearly always 0 in the scripts.
		 */
		BOOL IS_PED_IN_COVER(Ped ped, BOOL exceptUseWeapon);
		BOOL IS_PED_IN_COVER_FACING_LEFT(Ped ped);
		BOOL IS_PED_IN_HIGH_COVER(Ped ped);
		BOOL IS_PED_GOING_INTO_COVER(Ped ped);
		/**
		 * i could be time. Only example in the decompiled scripts uses it as -1.
		 */
		BOOL SET_PED_PINNED_DOWN(Ped ped, BOOL pinned, int i);
		BOOL _HAS_PED_CLEAR_LOS_TO_ENTITY(Ped ped, Entity entity, float x, float y, float z, int p5, BOOL p6, BOOL p7);
		int GET_SEAT_PED_IS_TRYING_TO_ENTER(Ped ped);
		Vehicle GET_VEHICLE_PED_IS_TRYING_TO_ENTER(Ped ped);
		/**
		 * Returns the Entity (Ped, Vehicle, or ?Object?) that killed the 'ped'
		 * 
		 * Is best to check if the Ped is dead before asking for its killer.
		 */
		Entity GET_PED_SOURCE_OF_DEATH(Ped ped);
		/**
		 * Returns the hash of the weapon/model/object that killed the ped.
		 */
		Hash GET_PED_CAUSE_OF_DEATH(Ped ped);
		int GET_PED_TIME_OF_DEATH(Ped ped);
		int COUNT_PEDS_IN_COMBAT_WITH_TARGET(Ped ped);
		int COUNT_PEDS_IN_COMBAT_WITH_TARGET_WITHIN_RADIUS(Ped ped, float x, float y, float z, float radius);
		void SET_PED_RELATIONSHIP_GROUP_DEFAULT_HASH(Ped ped, Hash hash);
		void SET_PED_RELATIONSHIP_GROUP_HASH(Ped ped, Hash hash);
		/**
		 * Sets the relationship between two groups. This should be called twice (once for each group).
		 * 
		 * Relationship types:
		 * 0 = Companion
		 * 1 = Respect
		 * 2 = Like
		 * 3 = Neutral
		 * 4 = Dislike
		 * 5 = Hate
		 * 255 = Pedestrians
		 * 
		 * Example:
		 * PED::SET_RELATIONSHIP_BETWEEN_GROUPS(2, l_1017, 0xA49E591C);
		 * PED::SET_RELATIONSHIP_BETWEEN_GROUPS(2, 0xA49E591C, l_1017);
		 */
		void SET_RELATIONSHIP_BETWEEN_GROUPS(int relationship, Hash group1, Hash group2);
		/**
		 * Clears the relationship between two groups. This should be called twice (once for each group).
		 * 
		 * Relationship types:
		 * 0 = Companion
		 * 1 = Respect
		 * 2 = Like
		 * 3 = Neutral
		 * 4 = Dislike
		 * 5 = Hate
		 * 255 = Pedestrians
		 * (Credits: Inco)
		 * 
		 * Example:
		 * PED::CLEAR_RELATIONSHIP_BETWEEN_GROUPS(2, l_1017, 0xA49E591C);
		 * PED::CLEAR_RELATIONSHIP_BETWEEN_GROUPS(2, 0xA49E591C, l_1017);
		 */
		void CLEAR_RELATIONSHIP_BETWEEN_GROUPS(int relationship, Hash group1, Hash group2);
		/**
		 * Can't select void. This function returns nothing. The hash of the created relationship group is output in the second parameter.
		 */
		BOOL ADD_RELATIONSHIP_GROUP(const char* name, Hash* groupHash);
		void REMOVE_RELATIONSHIP_GROUP(Hash groupHash);
		BOOL DOES_RELATIONSHIP_GROUP_EXIST(Hash groupHash);
		/**
		 * Gets the relationship between two peds. This should be called twice (once for each ped).
		 * 
		 * Relationship types:
		 * 0 = Companion
		 * 1 = Respect
		 * 2 = Like
		 * 3 = Neutral
		 * 4 = Dislike
		 * 5 = Hate
		 * 255 = Pedestrians
		 * (Credits: Inco)
		 * 
		 * Example:
		 * PED::GET_RELATIONSHIP_BETWEEN_PEDS(2, l_1017, 0xA49E591C);
		 * PED::GET_RELATIONSHIP_BETWEEN_PEDS(2, 0xA49E591C, l_1017);
		 */
		int GET_RELATIONSHIP_BETWEEN_PEDS(Ped ped1, Ped ped2);
		Hash GET_PED_RELATIONSHIP_GROUP_DEFAULT_HASH(Ped ped);
		Hash GET_PED_RELATIONSHIP_GROUP_HASH(Ped ped);
		/**
		 * Gets the relationship between two groups. This should be called twice (once for each group).
		 * 
		 * Relationship types:
		 * 0 = Companion
		 * 1 = Respect
		 * 2 = Like
		 * 3 = Neutral
		 * 4 = Dislike
		 * 5 = Hate
		 * 255 = Pedestrians
		 * 
		 * Example:
		 * PED::GET_RELATIONSHIP_BETWEEN_GROUPS(l_1017, 0xA49E591C);
		 * PED::GET_RELATIONSHIP_BETWEEN_GROUPS(0xA49E591C, l_1017);
		 */
		int GET_RELATIONSHIP_BETWEEN_GROUPS(Hash group1, Hash group2);
		void SET_RELATIONSHIP_GROUP_AFFECTS_WANTED_LEVEL(Hash group, BOOL p1);
		void TELL_GROUP_PEDS_IN_AREA_TO_ATTACK(Ped ped, Any p1, float p2, Hash hash, Any p4, Any p5);
		void SET_PED_CAN_BE_TARGETED_WITHOUT_LOS(Ped ped, BOOL toggle);
		void SET_PED_TO_INFORM_RESPECTED_FRIENDS(Ped ped, float radius, int maxFriends);
		BOOL IS_PED_RESPONDING_TO_EVENT(Ped ped, Any event);
		BOOL GET_POS_FROM_FIRED_EVENT(Ped ped, int eventType, Any* outData);
		/**
		 * FIRING_PATTERN_BURST_FIRE = 0xD6FF6D61 ( 1073727030 )
		 * FIRING_PATTERN_BURST_FIRE_IN_COVER = 0x026321F1 ( 40051185 )
		 * FIRING_PATTERN_BURST_FIRE_DRIVEBY = 0xD31265F2 ( -753768974 )
		 * FIRING_PATTERN_FROM_GROUND = 0x2264E5D6 ( 577037782 )
		 * FIRING_PATTERN_DELAY_FIRE_BY_ONE_SEC = 0x7A845691 ( 2055493265 )
		 * FIRING_PATTERN_FULL_AUTO = 0xC6EE6B4C ( -957453492 )
		 * FIRING_PATTERN_SINGLE_SHOT = 0x5D60E4E0 ( 1566631136 )
		 * FIRING_PATTERN_BURST_FIRE_PISTOL = 0xA018DB8A ( -1608983670 )
		 * FIRING_PATTERN_BURST_FIRE_SMG = 0xD10DADEE ( 1863348768 )
		 * FIRING_PATTERN_BURST_FIRE_RIFLE = 0x9C74B406 ( -1670073338 )
		 * FIRING_PATTERN_BURST_FIRE_MG = 0xB573C5B4 ( -1250703948 )
		 * FIRING_PATTERN_BURST_FIRE_PUMPSHOTGUN = 0x00BAC39B ( 12239771 )
		 * FIRING_PATTERN_BURST_FIRE_HELI = 0x914E786F ( -1857128337 )
		 * FIRING_PATTERN_BURST_FIRE_MICRO = 0x42EF03FD ( 1122960381 )
		 * FIRING_PATTERN_SHORT_BURSTS = 0x1A92D7DF ( 445831135 )
		 * FIRING_PATTERN_SLOW_FIRE_TANK = 0xE2CA3A71 ( -490063247 )
		 * 
		 * Firing pattern info: https://pastebin.com/Px036isB
		 */
		void SET_PED_FIRING_PATTERN(Ped ped, Hash patternHash);
		/**
		 * shootRate 0-1000
		 */
		void SET_PED_SHOOT_RATE(Ped ped, int shootRate);
		/**
		 * combatType can be between 0-14. See GET_COMBAT_FLOAT below for a list of possible parameters.
		 */
		void SET_COMBAT_FLOAT(Ped ped, int combatType, float p2);
		/**
		 * p0: Ped Handle
		 * p1: int i | 0 <= i <= 27
		 * 
		 * p1 probably refers to the attributes configured in combatbehavior.meta. There are 13. Example:
		 * 
		 * <BlindFireChance value="0.1"/>
		 * <WeaponShootRateModifier value="1.0"/>
		 * <TimeBetweenBurstsInCover value="1.25"/>
		 * <BurstDurationInCover value="2.0"/>
		 * <TimeBetweenPeeks value="10.0"/>
		 * <WeaponAccuracy value="0.18"/>
		 * <FightProficiency value="0.8"/>
		 * <StrafeWhenMovingChance value="1.0"/>
		 * <WalkWhenStrafingChance value="0.0"/>
		 * <AttackWindowDistanceForCover value="55.0"/>
		 * <TimeToInvalidateInjuredTarget value="9.0"/>
		 * <TriggerChargeTime_Near value="4.0"/>
		 * <TriggerChargeTime_Far value="10.0"/>
		 * 
		 * -------------Confirmed by editing combatbehavior.meta:
		 * p1:
		 * 0=BlindFireChance
		 * 1=BurstDurationInCover
		 * 3=TimeBetweenBurstsInCover
		 * 4=TimeBetweenPeeks
		 * 5=StrafeWhenMovingChance
		 * 8=WalkWhenStrafingChance
		 * 11=AttackWindowDistanceForCover
		 * 12=TimeToInvalidateInjuredTarget
		 * 16=OptimalCoverDistance
		 * 
		 */
		float GET_COMBAT_FLOAT(Ped ped, int p1);
		/**
		 * p1 may be a BOOL representing whether or not the group even exists
		 */
		void GET_GROUP_SIZE(int groupID, Any* p1, int* sizeInMembers);
		BOOL DOES_GROUP_EXIST(int groupId);
		/**
		 * Returns the group id of which the specified ped is a member of.
		 */
		int GET_PED_GROUP_INDEX(Ped ped);
		BOOL IS_PED_IN_GROUP(Ped ped);
		Player GET_PLAYER_PED_IS_FOLLOWING(Ped ped);
		/**
		 * 0: Default
		 * 1: Circle Around Leader
		 * 2: Alternative Circle Around Leader
		 * 3: Line, with Leader at center
		 */
		void SET_GROUP_FORMATION(int groupId, int formationType);
		void SET_GROUP_FORMATION_SPACING(int groupId, float x, float y, float z);
		void RESET_GROUP_FORMATION_DEFAULT_SPACING(int groupHandle);
		/**
		 * Gets ID of vehicle player using. It means it can get ID at any interaction with vehicle. Enter\exit for example. And that means it is faster than GET_VEHICLE_PED_IS_IN but less safe.
		 */
		Vehicle GET_VEHICLE_PED_IS_USING(Ped ped);
		Vehicle GET_VEHICLE_PED_IS_ENTERING(Ped ped);
		/**
		 * enable or disable the gravity of a ped
		 * 
		 * Examples:
		 * PED::SET_PED_GRAVITY(PLAYER::PLAYER_PED_ID(), 0x00000001);
		 * PED::SET_PED_GRAVITY(Local_289[iVar0 /*20* /], 0x00000001);
		 */
		void SET_PED_GRAVITY(Ped ped, BOOL toggle);
		void _SET_PED_SURVIVES_BEING_OUT_OF_WATER(Ped ped, BOOL toggle); // Missing in crossmap
		/**
		 * damages a ped with the given amount
		 */
		void APPLY_DAMAGE_TO_PED(Ped ped, int damageAmount, BOOL p2, Any p3, Hash weaponType);
		int GET_TIME_PED_DAMAGED_BY_WEAPON(Ped ped, Hash weaponHash);
		void SET_PED_ALLOWED_TO_DUCK(Ped ped, BOOL toggle);
		void SET_PED_NEVER_LEAVES_GROUP(Ped ped, BOOL toggle);
		/**
		 * https://alloc8or.re/gta5/doc/enums/ePedType.txt
		 */
		int GET_PED_TYPE(Ped ped);
		/**
		 * Turns the desired ped into a cop. If you use this on the player ped, you will become almost invisible to cops dispatched for you. You will also report your own crimes, get a generic cop voice, get a cop-vision-cone on the radar, and you will be unable to shoot at other cops. SWAT and Army will still shoot at you. Toggling ped as "false" has no effect; you must change p0's ped model to disable the effect.
		 */
		void SET_PED_AS_COP(Ped ped, BOOL toggle);
		void SET_PED_HEALTH_PENDING_LAST_DAMAGE_EVENT_OVERRIDE_FLAG(BOOL toggle);
		/**
		 * Sets the maximum health of a ped.
		 */
		void SET_PED_MAX_HEALTH(Ped ped, int value);
		int GET_PED_MAX_HEALTH(Ped ped);
		void SET_PED_MAX_TIME_IN_WATER(Ped ped, float value);
		void SET_PED_MAX_TIME_UNDERWATER(Ped ped, float value);
		void SET_CORPSE_RAGDOLL_FRICTION(Ped ped, float p1);
		/**
		 * seatIndex must be <= 2
		 */
		void SET_PED_VEHICLE_FORCED_SEAT_USAGE(Ped ped, Vehicle vehicle, int seatIndex, int flags, Any p4);
		void CLEAR_ALL_PED_VEHICLE_FORCED_SEAT_USAGE(Ped ped);
		/**
		 * This native does absolutely nothing, just a nullsub
		 */
		void SET_PED_CAN_BE_KNOCKED_OFF_BIKE(Any p0, Any p1);
		/**
		 * state: https://alloc8or.re/gta5/doc/enums/eKnockOffVehicle.txt
		 */
		void SET_PED_CAN_BE_KNOCKED_OFF_VEHICLE(Ped ped, int state);
		BOOL CAN_KNOCK_PED_OFF_VEHICLE(Ped ped);
		void KNOCK_PED_OFF_VEHICLE(Ped ped);
		void SET_PED_COORDS_NO_GANG(Ped ped, float posX, float posY, float posZ);
		/**
		 * from fm_mission_controller.c4 (variable names changed for clarity):
		 * 
		 * int groupID = PLAYER::GET_PLAYER_GROUP(PLAYER::PLAYER_ID());
		 * PED::GET_GROUP_SIZE(group, &unused, &groupSize);
		 * if (groupSize >= 1) {
		 * . . . . for (int memberNumber = 0; memberNumber < groupSize; memberNumber++) {
		 * . . . . . . . . Ped ped1 = PED::GET_PED_AS_GROUP_MEMBER(groupID, memberNumber);
		 * . . . . . . . . //and so on
		 */
		Ped GET_PED_AS_GROUP_MEMBER(int groupID, int memberNumber);
		Ped GET_PED_AS_GROUP_LEADER(int groupID);
		void SET_PED_KEEP_TASK(Ped ped, BOOL toggle);
		void SET_PED_ALLOW_MINOR_REACTIONS_AS_MISSION_PED(Ped ped, BOOL toggle);
		BOOL IS_PED_SWIMMING(Ped ped);
		BOOL IS_PED_SWIMMING_UNDER_WATER(Ped ped);
		/**
		 * teleports ped to coords along with the vehicle ped is in
		 */
		void SET_PED_COORDS_KEEP_VEHICLE(Ped ped, float posX, float posY, float posZ);
		void SET_PED_DIES_IN_VEHICLE(Ped ped, BOOL toggle);
		void SET_CREATE_RANDOM_COPS(BOOL toggle);
		void SET_CREATE_RANDOM_COPS_NOT_ON_SCENARIOS(BOOL toggle);
		void SET_CREATE_RANDOM_COPS_ON_SCENARIOS(BOOL toggle);
		BOOL CAN_CREATE_RANDOM_COPS();
		void SET_PED_AS_ENEMY(Ped ped, BOOL toggle);
		void SET_PED_CAN_SMASH_GLASS(Ped ped, BOOL p1, BOOL p2);
		BOOL IS_PED_IN_ANY_TRAIN(Ped ped);
		BOOL IS_PED_GETTING_INTO_A_VEHICLE(Ped ped);
		BOOL IS_PED_TRYING_TO_ENTER_A_LOCKED_VEHICLE(Ped ped);
		/**
		 * ped can not pull out a weapon when true
		 */
		void SET_ENABLE_HANDCUFFS(Ped ped, BOOL toggle);
		/**
		 * Used with SET_ENABLE_HANDCUFFS in decompiled scripts. From my observations, I have noticed that while being ragdolled you are not able to get up but you can still run. Your legs can also bend.
		 */
		void SET_ENABLE_BOUND_ANKLES(Ped ped, BOOL toggle);
		/**
		 * Enables diving motion when underwater.
		 */
		void SET_ENABLE_SCUBA(Ped ped, BOOL toggle);
		/**
		 * Setting ped to true allows the ped to shoot "friendlies".
		 * 
		 * p2 set to true when toggle is also true seams to make peds permanently unable to aim at, even if you set p2 back to false.
		 * 
		 * p1 = false & p2 = false for unable to aim at.
		 * p1 = true & p2 = false for able to aim at.
		 */
		void SET_CAN_ATTACK_FRIENDLY(Ped ped, BOOL toggle, BOOL p2);
		/**
		 * Returns the ped's alertness (0-3).
		 * 
		 * Values :
		 * 
		 * 0 : Neutral
		 * 1 : Heard something (gun shot, hit, etc)
		 * 2 : Knows (the origin of the event)
		 * 3 : Fully alerted (is facing the event?)
		 * 
		 * If the Ped does not exist, returns -1.
		 */
		int GET_PED_ALERTNESS(Ped ped);
		/**
		 * value ranges from 0 to 3.
		 */
		void SET_PED_ALERTNESS(Ped ped, int value);
		void SET_PED_GET_OUT_UPSIDE_DOWN_VEHICLE(Ped ped, BOOL toggle);
		/**
		 * transitionSpeed is the time in seconds it takes to transition from one movement clipset to another.	ransitionSpeed is usually 1.0f
		 * 
		 * List of movement clipsets:
		 * Thanks to elsewhat for list.
		 * 
		 * "ANIM_GROUP_MOVE_BALLISTIC"
		 * "ANIM_GROUP_MOVE_LEMAR_ALLEY"
		 * "clipset@move@trash_fast_turn"
		 * "FEMALE_FAST_RUNNER"
		 * "missfbi4prepp1_garbageman"
		 * "move_characters@franklin@fire"
		 * "move_characters@Jimmy@slow@"
		 * "move_characters@michael@fire"
		 * "move_f@flee@a"
		 * "move_f@scared"
		 * "move_f@sexy@a"
		 * "move_heist_lester"
		 * "move_injured_generic"
		 * "move_lester_CaneUp"
		 * "move_m@bag"
		 * "MOVE_M@BAIL_BOND_NOT_TAZERED"
		 * "MOVE_M@BAIL_BOND_TAZERED"
		 * "move_m@brave"
		 * "move_m@casual@d"
		 * "move_m@drunk@moderatedrunk"
		 * "MOVE_M@DRUNK@MODERATEDRUNK"
		 * "MOVE_M@DRUNK@MODERATEDRUNK_HEAD_UP"
		 * "MOVE_M@DRUNK@SLIGHTLYDRUNK"
		 * "MOVE_M@DRUNK@VERYDRUNK"
		 * "move_m@fire"
		 * "move_m@gangster@var_e"
		 * "move_m@gangster@var_f"
		 * "move_m@gangster@var_i"
		 * "move_m@JOG@"
		 * "MOVE_M@PRISON_GAURD"
		 * "MOVE_P_M_ONE"
		 * "MOVE_P_M_ONE_BRIEFCASE"
		 * "move_p_m_zero_janitor"
		 * "move_p_m_zero_slow"
		 * "move_ped_bucket"
		 * "move_ped_crouched"
		 * "move_ped_mop"
		 * "MOVE_M@FEMME@"
		 * "MOVE_F@FEMME@"
		 * "MOVE_M@GANGSTER@NG"
		 * "MOVE_F@GANGSTER@NG"
		 * "MOVE_M@POSH@"
		 * "MOVE_F@POSH@"
		 * "MOVE_M@TOUGH_GUY@"
		 * "MOVE_F@TOUGH_GUY@"
		 * 
		 * ~ NotCrunchyTaco
		 * 
		 * Full list of movement clipsets by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/movementClipsetsCompact.json
		 */
		void SET_PED_MOVEMENT_CLIPSET(Ped ped, const char* clipSet, float transitionSpeed);
		/**
		 * If p1 is 0.0, I believe you are back to normal.
		 * If p1 is 1.0, it looks like you can only rotate the ped, not walk.
		 * 
		 * Using the following code to reset back to normal
		 * PED::RESET_PED_MOVEMENT_CLIPSET(PLAYER::PLAYER_PED_ID(), 0.0);
		 */
		void RESET_PED_MOVEMENT_CLIPSET(Ped ped, float p1);
		/**
		 * Full list of movement clipsets by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/movementClipsetsCompact.json
		 */
		void SET_PED_STRAFE_CLIPSET(Ped ped, const char* clipSet);
		void RESET_PED_STRAFE_CLIPSET(Ped ped);
		void SET_PED_WEAPON_MOVEMENT_CLIPSET(Ped ped, const char* clipSet);
		void RESET_PED_WEAPON_MOVEMENT_CLIPSET(Ped ped);
		void SET_PED_DRIVE_BY_CLIPSET_OVERRIDE(Ped ped, const char* clipset);
		void CLEAR_PED_DRIVE_BY_CLIPSET_OVERRIDE(Ped ped);
		/**
		 * Found in the b617d scripts:
		 * PED::SET_PED_MOTION_IN_COVER_CLIPSET_OVERRIDE(v_7, "trevor_heist_cover_2h");
		 */
		void SET_PED_MOTION_IN_COVER_CLIPSET_OVERRIDE(Ped ped, const char* p1);
		void CLEAR_PED_MOTION_IN_COVER_CLIPSET_OVERRIDE(Ped ped);
		void CLEAR_PED_FALL_UPPER_BODY_CLIPSET_OVERRIDE(Ped ped);
		/**
		 * PED::SET_PED_IN_VEHICLE_CONTEXT(l_128, MISC::GET_HASH_KEY("MINI_PROSTITUTE_LOW_PASSENGER"));
		 * PED::SET_PED_IN_VEHICLE_CONTEXT(l_128, MISC::GET_HASH_KEY("MINI_PROSTITUTE_LOW_RESTRICTED_PASSENGER"));
		 * PED::SET_PED_IN_VEHICLE_CONTEXT(l_3212, MISC::GET_HASH_KEY("MISS_FAMILY1_JIMMY_SIT"));
		 * PED::SET_PED_IN_VEHICLE_CONTEXT(l_3212, MISC::GET_HASH_KEY("MISS_FAMILY1_JIMMY_SIT_REAR"));
		 * PED::SET_PED_IN_VEHICLE_CONTEXT(l_95, MISC::GET_HASH_KEY("MISS_FAMILY2_JIMMY_BICYCLE"));
		 * PED::SET_PED_IN_VEHICLE_CONTEXT(num3, MISC::GET_HASH_KEY("MISSFBI2_MICHAEL_DRIVEBY"));
		 * PED::SET_PED_IN_VEHICLE_CONTEXT(PLAYER::PLAYER_PED_ID(), MISC::GET_HASH_KEY("MISS_ARMENIAN3_FRANKLIN_TENSE"));
		 * PED::SET_PED_IN_VEHICLE_CONTEXT(PLAYER::PLAYER_PED_ID(), MISC::GET_HASH_KEY("MISSFBI5_TREVOR_DRIVING"));
		 */
		void SET_PED_IN_VEHICLE_CONTEXT(Ped ped, Hash context);
		void RESET_PED_IN_VEHICLE_CONTEXT(Ped ped);
		/**
		 * Full list of animation dictionaries and anims by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/animDictsCompact.json
		 */
		BOOL IS_SCRIPTED_SCENARIO_PED_USING_CONDITIONAL_ANIM(Ped ped, const char* animDict, const char* anim);
		/**
		 * Full list of animation dictionaries and anims by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/animDictsCompact.json
		 * 
		 * Full list of movement clipsets by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/movementClipsetsCompact.json
		 */
		void SET_PED_ALTERNATE_WALK_ANIM(Ped ped, const char* animDict, const char* animName, float p3, BOOL p4);
		void CLEAR_PED_ALTERNATE_WALK_ANIM(Ped ped, float p1);
		/**
		 * stance:
		 * 0 = idle
		 * 1 = walk
		 * 2 = running
		 * 
		 * p5 = usually set to true
		 * 
		 * Full list of animation dictionaries and anims by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/animDictsCompact.json
		 * 
		 * Full list of movement clipsets by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/movementClipsetsCompact.json
		 */
		void SET_PED_ALTERNATE_MOVEMENT_ANIM(Ped ped, int stance, const char* animDictionary, const char* animationName, float p4, BOOL p5);
		void CLEAR_PED_ALTERNATE_MOVEMENT_ANIM(Ped ped, int stance, float p2);
		/**
		 * From the scripts:
		 * PED::SET_PED_GESTURE_GROUP(PLAYER::PLAYER_PED_ID(),
		 * "ANIM_GROUP_GESTURE_MISS_FRA0");
		 * PED::SET_PED_GESTURE_GROUP(PLAYER::PLAYER_PED_ID(),
		 * "ANIM_GROUP_GESTURE_MISS_DocksSetup1");
		 */
		void SET_PED_GESTURE_GROUP(Ped ped, const char* animGroupGesture);
		/**
		 * Full list of animation dictionaries and anims by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/animDictsCompact.json
		 */
		Vector3 GET_ANIM_INITIAL_OFFSET_POSITION(const char* animDict, const char* animName, float x, float y, float z, float xRot, float yRot, float zRot, float p8, int p9);
		/**
		 * Full list of animation dictionaries and anims by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/animDictsCompact.json
		 */
		Vector3 GET_ANIM_INITIAL_OFFSET_ROTATION(const char* animDict, const char* animName, float x, float y, float z, float xRot, float yRot, float zRot, float p8, int p9);
		/**
		 * Ids
		 * 0 - Head
		 * 1 - Beard
		 * 2 - Hair
		 * 3 - Torso
		 * 4 - Legs
		 * 5 - Hands
		 * 6 - Foot
		 * 7 - ------
		 * 8 - Accessories 1
		 * 9 - Accessories 2
		 * 10- Decals
		 * 11 - Auxiliary parts for torso
		 */
		int GET_PED_DRAWABLE_VARIATION(Ped ped, int componentId);
		/**
		 * List of component/props ID
		 * gtaxscripting.blogspot.com/2016/04/gta-v-peds-component-and-props.html
		 */
		int GET_NUMBER_OF_PED_DRAWABLE_VARIATIONS(Ped ped, int componentId);
		/**
		 * List of component/props ID
		 * gtaxscripting.blogspot.com/2016/04/gta-v-peds-component-and-props.html
		 */
		int GET_PED_TEXTURE_VARIATION(Ped ped, int componentId);
		/**
		 * List of component/props ID
		 * gtaxscripting.blogspot.com/2016/04/gta-v-peds-component-and-props.html
		 */
		int GET_NUMBER_OF_PED_TEXTURE_VARIATIONS(Ped ped, int componentId, int drawableId);
		/**
		 * List of component/props ID
		 * gtaxscripting.blogspot.com/2016/04/gta-v-peds-component-and-props.html
		 */
		int GET_NUMBER_OF_PED_PROP_DRAWABLE_VARIATIONS(Ped ped, int propId);
		/**
		 * Need to check behavior when drawableId = -1
		 * 
		 * - Doofy.Ass
		 * Why this function doesn't work and return nill value?
		 * GET_NUMBER_OF_PED_PROP_TEXTURE_VARIATIONS(PLAYER.PLAYER_PED_ID(), 0, 5)
		 * 
		 * tick: scripts/addins/menu_execute.lua:51: attempt to call field 'GET_NUMBER_OF_PED_PROP_TEXTURE_VARIATIONS' (a nil value)
		 * 
		 * 
		 * List of component/props ID
		 * gtaxscripting.blogspot.com/2016/04/gta-v-peds-component-and-props.html
		 */
		int GET_NUMBER_OF_PED_PROP_TEXTURE_VARIATIONS(Ped ped, int propId, int drawableId);
		/**
		 * List of component/props ID
		 * gtaxscripting.blogspot.com/2016/04/gta-v-peds-component-and-props.html
		 */
		int GET_PED_PALETTE_VARIATION(Ped ped, int componentId);
		BOOL GET_MP_OUTFIT_DATA_FROM_METADATA(Any* p0, Any* p1);
		int GET_FM_MALE_SHOP_PED_APPAREL_ITEM_INDEX(int p0);
		int GET_FM_FEMALE_SHOP_PED_APPAREL_ITEM_INDEX(int p0);
		/**
		 * Checks if the component variation is valid, this works great for randomizing components using loops.
		 * 
		 * List of component/props ID
		 * gtaxscripting.blogspot.com/2016/04/gta-v-peds-component-and-props.html
		 * 
		 * Full list of ped components by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/pedComponentVariations.json
		 */
		BOOL IS_PED_COMPONENT_VARIATION_VALID(Ped ped, int componentId, int drawableId, int textureId);
		/**
		 * paletteId: 0 to 3.
		 * 
		 * componentId:
		 * enum ePedVarComp
		 * {
		 * PV_COMP_INVALID = -1,
		 * PV_COMP_HEAD,
		 * PV_COMP_BERD,
		 * PV_COMP_HAIR,
		 * PV_COMP_UPPR,
		 * PV_COMP_LOWR,
		 * PV_COMP_HAND,
		 * PV_COMP_FEET,
		 * PV_COMP_TEEF,
		 * PV_COMP_ACCS,
		 * PV_COMP_TASK,
		 * PV_COMP_DECL,
		 * PV_COMP_JBIB,
		 * PV_COMP_MAX
		 * };
		 * 
		 * Examples: https://gtaxscripting.blogspot.com/2016/04/gta-v-peds-component-and-props.html
		 * 
		 * Full list of ped components by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/pedComponentVariations.json
		 */
		void SET_PED_COMPONENT_VARIATION(Ped ped, int componentId, int drawableId, int textureId, int paletteId);
		/**
		 * p1 is always 0 in R* scripts.
		 * 
		 * List of component/props ID
		 * gtaxscripting.blogspot.com/2016/04/gta-v-peds-component-and-props.html
		 */
		void SET_PED_RANDOM_COMPONENT_VARIATION(Ped ped, int p1);
		/**
		 * List of component/props ID
		 * gtaxscripting.blogspot.com/2016/04/gta-v-peds-component-and-props.html
		 */
		void SET_PED_RANDOM_PROPS(Ped ped);
		/**
		 * Sets Ped Default Clothes
		 */
		void SET_PED_DEFAULT_COMPONENT_VARIATION(Ped ped);
		void SET_PED_BLEND_FROM_PARENTS(Ped ped, Any p1, Any p2, float p3, float p4);
		/**
		 * The "shape" parameters control the shape of the ped's face. The "skin" parameters control the skin tone. ShapeMix and skinMix control how much the first and second IDs contribute,(typically mother and father.) ThirdMix overrides the others in favor of the third IDs. IsParent is set for "children" of the player character's grandparents during old-gen character creation. It has unknown effect otherwise.
		 * 
		 * The IDs start at zero and go Male Non-DLC, Female Non-DLC, Male DLC, and Female DLC.
		 * 
		 * !!!Can someone add working example for this???
		 * 
		 * try this:
		 * headBlendData headData;
		 * GET_PED_HEAD_BLEND_DATA(PLAYER_PED_ID(), &headData);
		 * 
		 * SET_PED_HEAD_BLEND_DATA(PLAYER_PED_ID(), headData.shapeFirst, headData.shapeSecond, headData.shapeThird, headData.skinFirst, headData.skinSecond
		 * , headData.skinThird, headData.shapeMix, headData.skinMix, headData.skinThird, 0);
		 * 
		 * 
		 * For more info please refer to this topic.
		 * gtaforums.com/topic/858970-all-gtao-face-ids-pedset-ped-head-blend-data-explained
		 */
		void SET_PED_HEAD_BLEND_DATA(Ped ped, int shapeFirstID, int shapeSecondID, int shapeThirdID, int skinFirstID, int skinSecondID, int skinThirdID, float shapeMix, float skinMix, float thirdMix, BOOL isParent);
		/**
		 * The pointer is to a padded struct that matches the arguments to SET_PED_HEAD_BLEND_DATA(...). There are 4 bytes of padding after each field.
		 * pass this struct in the second parameter
		 * struct headBlendData
		 * {
		 * int shapeFirst;
		 * int padding1;
		 * int shapeSecond;
		 * int padding2;
		 * int shapeThird;
		 * int padding3;
		 * int skinFirst;
		 * int padding4;
		 * int skinSecond;
		 * int padding5;
		 * int skinThird;
		 * int padding6;
		 * float shapeMix;
		 * int padding7;
		 * float skinMix;
		 * int padding8;
		 * float thirdMix;
		 * int padding9;
		 * bool isParent;
		 * };
		 */
		BOOL GET_PED_HEAD_BLEND_DATA(Ped ped, Any* headBlendData);
		/**
		 * See SET_PED_HEAD_BLEND_DATA().
		 */
		void UPDATE_PED_HEAD_BLEND_DATA(Ped ped, float shapeMix, float skinMix, float thirdMix);
		/**
		 * Used for freemode (online) characters.
		 * 
		 * For some reason, the scripts use a rounded float for the index.
		 * Indexes:
		 * 1. black
		 * 2. very light blue/green
		 * 3. dark blue
		 * 4. brown
		 * 5. darker brown
		 * 6. light brown
		 * 7. blue
		 * 8. light blue
		 * 9. pink
		 * 10. yellow
		 * 11. purple
		 * 12. black
		 * 13. dark green
		 * 14. light brown
		 * 15. yellow/black pattern
		 * 16. light colored spiral pattern
		 * 17. shiny red
		 * 18. shiny half blue/half red
		 * 19. half black/half light blue
		 * 20. white/red perimter
		 * 21. green snake
		 * 22. red snake
		 * 23. dark blue snake
		 * 24. dark yellow
		 * 25. bright yellow
		 * 26. all black
		 * 28. red small pupil
		 * 29. devil blue/black
		 * 30. white small pupil
		 * 31. glossed over
		 */
		void SET_HEAD_BLEND_EYE_COLOR(Ped ped, int index);
		/**
		 * A getter for _SET_PED_EYE_COLOR. Returns -1 if fails to get.
		 */
		int GET_HEAD_BLEND_EYE_COLOR(Ped ped);
		/**
		 * OverlayID ranges from 0 to 12, index from 0 to _GET_NUM_OVERLAY_VALUES(overlayID)-1, and opacity from 0.0 to 1.0.
		 * 
		 * overlayID       Part                  Index, to disable
		 * 0               Blemishes             0 - 23, 255
		 * 1               Facial Hair           0 - 28, 255
		 * 2               Eyebrows              0 - 33, 255
		 * 3               Ageing                0 - 14, 255
		 * 4               Makeup                0 - 74, 255
		 * 5               Blush                 0 - 6, 255
		 * 6               Complexion            0 - 11, 255
		 * 7               Sun Damage            0 - 10, 255
		 * 8               Lipstick              0 - 9, 255
		 * 9               Moles/Freckles        0 - 17, 255
		 * 10              Chest Hair            0 - 16, 255
		 * 11              Body Blemishes        0 - 11, 255
		 * 12              Add Body Blemishes    0 - 1, 255
		 */
		void SET_PED_HEAD_OVERLAY(Ped ped, int overlayID, int index, float opacity);
		/**
		 * Likely a char, if that overlay is not set, e.i. "None" option, returns 255;
		 * 
		 * This might be the once removed native GET_PED_HEAD_OVERLAY.
		 */
		int GET_PED_HEAD_OVERLAY(Ped ped, int overlayID);
		int GET_PED_HEAD_OVERLAY_NUM(int overlayID);
		/**
		 * 
		 * 
		 * ColorType is 1 for eyebrows, beards, and chest hair; 2 for blush and lipstick; and 0 otherwise, though not called in those cases.
		 * 
		 * Called after SET_PED_HEAD_OVERLAY().
		 */
		void SET_PED_HEAD_OVERLAY_TINT(Ped ped, int overlayID, int colorType, int colorID, int secondColorID);
		void SET_PED_HAIR_TINT(Ped ped, int colorID, int highlightColorID);
		int GET_NUM_PED_HAIR_TINTS();
		int GET_NUM_PED_MAKEUP_TINTS();
		/**
		 * Input: Haircolor index, value between 0 and 63 (inclusive).
		 * Output: RGB values for the haircolor specified in the input.
		 * 
		 * This is used with the hair color swatches scaleform.
		 * Use `GET_PED_MAKEUP_TINT_COLOR` to get the makeup colors.
		 */
		void GET_PED_HAIR_TINT_COLOR(int hairColorIndex, int* outR, int* outG, int* outB);
		/**
		 * Input: Makeup color index, value between 0 and 63 (inclusive).
		 * Output: RGB values for the makeup color specified in the input.
		 * 
		 * This is used with the makeup color swatches scaleform.
		 * Use `GET_PED_HAIR_TINT_COLOR` to get the hair colors.
		 */
		void GET_PED_MAKEUP_TINT_COLOR(int makeupColorIndex, int* outR, int* outG, int* outB);
		BOOL IS_PED_HAIR_TINT_FOR_CREATOR(int colorId);
		int GET_DEFAULT_SECONDARY_TINT_FOR_CREATOR(int colorId);
		BOOL IS_PED_LIPSTICK_TINT_FOR_CREATOR(int colorId);
		BOOL IS_PED_BLUSH_TINT_FOR_CREATOR(int colorId);
		BOOL IS_PED_HAIR_TINT_FOR_BARBER(int colorID);
		int GET_DEFAULT_SECONDARY_TINT_FOR_BARBER(int colorID);
		BOOL IS_PED_LIPSTICK_TINT_FOR_BARBER(int colorID);
		BOOL IS_PED_BLUSH_TINT_FOR_BARBER(int colorID);
		BOOL IS_PED_BLUSH_FACEPAINT_TINT_FOR_BARBER(int colorId);
		int GET_TINT_INDEX_FOR_LAST_GEN_HAIR_TEXTURE(Hash modelHash, int drawableId, int textureId);
		/**
		 * Sets the various freemode face features, e.g. nose length, chin shape. Scale ranges from -1.0 to 1.0.
		 * 
		 * 
		 * 
		 * 0 - Nose Width (Thin/Wide)
		 * 
		 * 1 - Nose Peak (Up/Down)
		 * 
		 * 2 - Nose Length (Long/Short)
		 * 
		 * 3 - Nose Bone Curveness (Crooked/Curved)
		 * 
		 * 4 - Nose Tip (Up/Down)
		 * 
		 * 5 - Nose Bone Twist (Left/Right)
		 * 
		 * 6 - Eyebrow (Up/Down)
		 * 
		 * 7 - Eyebrow (In/Out)
		 * 
		 * 8 - Cheek Bones (Up/Down)
		 * 
		 * 9 - Cheek Sideways Bone Size (In/Out)
		 * 
		 * 10 - Cheek Bones Width (Puffed/Gaunt)
		 * 
		 * 11 - Eye Opening (Both) (Wide/Squinted)
		 * 
		 * 12 - Lip Thickness (Both) (Fat/Thin)
		 * 
		 * 13 - Jaw Bone Width (Narrow/Wide)
		 * 
		 * 14 - Jaw Bone Shape (Round/Square)
		 * 
		 * 15 - Chin Bone (Up/Down)
		 * 
		 * 16 - Chin Bone Length (In/Out or Backward/Forward)
		 * 
		 * 17 - Chin Bone Shape (Pointed/Square)
		 * 
		 * 18 - Chin Hole (Chin Bum)
		 * 
		 * 19 - Neck Thickness (Thin/Thick)
		 */
		void SET_PED_MICRO_MORPH(Ped ped, int index, float scale);
		BOOL HAS_PED_HEAD_BLEND_FINISHED(Ped ped);
		void FINALIZE_HEAD_BLEND(Ped ped);
		/**
		 * p4 seems to vary from 0 to 3.
		 * Preview: https://gfycat.com/MaleRareAmazonparrot
		 */
		void SET_HEAD_BLEND_PALETTE_COLOR(Ped ped, int r, int g, int b, int id);
		void DISABLE_HEAD_BLEND_PALETTE_COLOR(Ped ped);
		/**
		 * Type equals 0 for male non-dlc, 1 for female non-dlc, 2 for male dlc, and 3 for female dlc.
		 * 
		 * Used when calling SET_PED_HEAD_BLEND_DATA.
		 */
		int GET_PED_HEAD_BLEND_FIRST_INDEX(int type);
		/**
		 * Type equals 0 for male non-dlc, 1 for female non-dlc, 2 for male dlc, and 3 for female dlc.
		 */
		int GET_PED_HEAD_BLEND_NUM_HEADS(int type);
		/**
		 * from extreme3.c4
		 * PED::SET_PED_PRELOAD_VARIATION_DATA(PLAYER::PLAYER_PED_ID(), 8, PED::GET_PED_DRAWABLE_VARIATION(PLAYER::PLAYER_PED_ID(), 8), PED::GET_PED_TEXTURE_VARIATION(PLAYER::PLAYER_PED_ID(), 8));
		 * 
		 * p1 is probably componentId
		 */
		int SET_PED_PRELOAD_VARIATION_DATA(Ped ped, int slot, int drawableId, int textureId);
		BOOL HAS_PED_PRELOAD_VARIATION_DATA_FINISHED(Ped ped);
		void RELEASE_PED_PRELOAD_VARIATION_DATA(Ped ped);
		/**
		 * List of component/props ID
		 * gtaxscripting.blogspot.com/2016/04/gta-v-peds-component-and-props.html
		 */
		int SET_PED_PRELOAD_PROP_DATA(Ped ped, int componentId, int drawableId, int TextureId);
		BOOL HAS_PED_PRELOAD_PROP_DATA_FINISHED(Ped ped);
		void RELEASE_PED_PRELOAD_PROP_DATA(Ped ped);
		/**
		 * List of component/props ID
		 * gtaxscripting.blogspot.com/2016/04/gta-v-peds-component-and-props.html
		 */
		int GET_PED_PROP_INDEX(Ped ped, int componentId, Any p2);
		/**
		 * ComponentId can be set to various things based on what category you're wanting to set
		 * enum PedPropsData
		 * {
		 * PED_PROP_HATS = 0,
		 * PED_PROP_GLASSES = 1,
		 * PED_PROP_EARS = 2,
		 * PED_PROP_WATCHES = 3,
		 * };
		 * Usage: SET_PED_PROP_INDEX(playerPed, PED_PROP_HATS, GET_NUMBER_OF_PED_PROP_DRAWABLE_VARIATIONS(playerPed, PED_PROP_HATS), GET_NUMBER_OF_PED_PROP_TEXTURE_VARIATIONS(playerPed, PED_PROP_HATS, 0), TRUE);
		 * 
		 * List of component/props ID
		 * gtaxscripting.blogspot.com/2016/04/gta-v-peds-component-and-props.html
		 */
		void SET_PED_PROP_INDEX(Ped ped, int componentId, int drawableId, int TextureId, BOOL attach, Any p5);
		/**
		 * List of component/props ID
		 * gtaxscripting.blogspot.com/2016/04/gta-v-peds-component-and-props.html
		 */
		void KNOCK_OFF_PED_PROP(Ped ped, BOOL p1, BOOL p2, BOOL p3, BOOL p4);
		/**
		 * List of component/props ID
		 * gtaxscripting.blogspot.com/2016/04/gta-v-peds-component-and-props.html
		 */
		void CLEAR_PED_PROP(Ped ped, int propId, Any p2);
		/**
		 * List of component/props ID
		 * gtaxscripting.blogspot.com/2016/04/gta-v-peds-component-and-props.html
		 */
		void CLEAR_ALL_PED_PROPS(Ped ped, Any p1);
		void DROP_AMBIENT_PROP(Ped ped);
		/**
		 * List of component/props ID
		 * gtaxscripting.blogspot.com/2016/04/gta-v-peds-component-and-props.html
		 */
		int GET_PED_PROP_TEXTURE_INDEX(Ped ped, int componentId);
		void CLEAR_PED_PARACHUTE_PACK_VARIATION(Ped ped);
		/**
		 * This native sets a scuba mask for freemode models and an oxygen bottle for player_* models. It works on freemode and player_* models.
		 */
		void SET_PED_SCUBA_GEAR_VARIATION(Ped ped);
		/**
		 * Removes the scubagear (for mp male: component id: 8, drawableId: 123, textureId: any) from peds. Does not play the 'remove scuba gear' animation, but instantly removes it.
		 */
		void CLEAR_PED_SCUBA_GEAR_VARIATION(Ped ped);
		BOOL IS_USING_PED_SCUBA_GEAR_VARIATION(Any p0);
		/**
		 * works with TASK::TASK_SET_BLOCKING_OF_NON_TEMPORARY_EVENTS to make a ped completely oblivious to all events going on around him
		 */
		void SET_BLOCKING_OF_NON_TEMPORARY_EVENTS(Ped ped, BOOL toggle);
		void SET_PED_BOUNDS_ORIENTATION(Ped ped, float p1, float p2, float x, float y, float z);
		/**
		 * PED::REGISTER_TARGET(l_216, PLAYER::PLAYER_PED_ID()); from re_prisonbreak.txt.
		 * 
		 * l_216 = RECSBRobber1
		 */
		void REGISTER_TARGET(Ped ped, Ped target);
		/**
		 * Based on TASK_COMBAT_HATED_TARGETS_AROUND_PED, the parameters are likely similar (PedHandle, and area to attack in).
		 */
		void REGISTER_HATED_TARGETS_AROUND_PED(Ped ped, float radius);
		/**
		 * Gets a random ped in the x/y/zRadius near the x/y/z coordinates passed.
		 * 
		 * Ped Types:
		 * Any = -1
		 * Player = 1
		 * Male = 4
		 * Female = 5
		 * Cop = 6
		 * Human = 26
		 * SWAT = 27
		 * Animal = 28
		 * Army = 29
		 */
		Ped GET_RANDOM_PED_AT_COORD(float x, float y, float z, float xRadius, float yRadius, float zRadius, int pedType);
		/**
		 * Gets the closest ped in a radius.
		 * 
		 * Ped Types:
		 * Any ped = -1
		 * Player = 1
		 * Male = 4
		 * Female = 5
		 * Cop = 6
		 * Human = 26
		 * SWAT = 27
		 * Animal = 28
		 * Army = 29
		 * 
		 * ------------------
		 * P4 P5 P7 P8
		 * 1  0  x  x  = return nearest walking Ped
		 * 1  x  0  x  = return nearest walking Ped
		 * x  1  1  x  = return Ped you are using
		 * 0  0  x  x  = no effect
		 * 0  x  0  x  = no effect
		 * 
		 * x = can be 1 or 0. Does not have any obvious changes.
		 * 
		 * This function does not return ped who is:
		 * 1. Standing still
		 * 2. Driving
		 * 3. Fleeing
		 * 4. Attacking
		 * 
		 * This function only work if the ped is:
		 * 1. walking normally.
		 * 2. waiting to cross a road.
		 * 
		 * Note: PED::GET_PED_NEARBY_PEDS works for more peds.
		 */
		BOOL GET_CLOSEST_PED(float x, float y, float z, float radius, BOOL p4, BOOL p5, Ped* outPed, BOOL p7, BOOL p8, int pedType);
		/**
		 * Sets a value indicating whether scenario peds should be returned by the next call to a command that returns peds. Eg. GET_CLOSEST_PED.
		 */
		void SET_SCENARIO_PEDS_TO_BE_RETURNED_BY_NEXT_COMMAND(BOOL value);
		BOOL GET_CAN_PED_BE_GRABBED_BY_SCRIPT(Ped ped, BOOL p1, BOOL p2, BOOL p3, BOOL p4, BOOL p5, BOOL p6, BOOL p7, Any p8);
		/**
		 * Scripts use 0.2, 0.5 and 1.0. Value must be >= 0.0 && <= 1.0
		 */
		void SET_DRIVER_RACING_MODIFIER(Ped driver, float modifier);
		/**
		 * The function specifically verifies the value is equal to, or less than 1.0f. If it is greater than 1.0f, the function does nothing at all.
		 */
		void SET_DRIVER_ABILITY(Ped driver, float ability);
		/**
		 * range 0.0f - 1.0f
		 */
		void SET_DRIVER_AGGRESSIVENESS(Ped driver, float aggressiveness);
		/**
		 * Prevents the ped from going limp.
		 * 
		 * [Example: Can prevent peds from falling when standing on moving vehicles.]
		 */
		BOOL CAN_PED_RAGDOLL(Ped ped);
		/**
		 * p4/p5: Unused in TU27
		 * Ragdoll Types:
		 * **0**: CTaskNMRelax
		 * **1**: CTaskNMScriptControl: Hardcoded not to work in networked environments.
		 * **Else**: CTaskNMBalance
		 * time1- Time(ms) Ped is in ragdoll mode; only applies to ragdoll types 0 and not 1.
		 * 
		 * time2- Unknown time, in milliseconds
		 * 
		 * ragdollType-
		 * 0 : Normal ragdoll
		 * 1 : Falls with stiff legs/body
		 * 2 : Narrow leg stumble(may not fall)
		 * 3 : Wide leg stumble(may not fall)
		 * 
		 * p4, p5, p6- No idea. In R*'s scripts they are usually either "true, true, false" or "false, false, false".
		 * 
		 * 
		 * 
		 * 
		 * EDIT 3/11/16: unclear what 'mircoseconds' mean-- a microsecond is 1000x a ms, so time2 must be 1000x time1?  more testing needed.  -sob
		 * 
		 * Edit Mar 21, 2017: removed part about time2 being the microseconds version of time1. this just isn't correct. time2 is in milliseconds, and time1 and time2 don't seem to be connected in any way.
		 */
		BOOL SET_PED_TO_RAGDOLL(Ped ped, int time1, int time2, int ragdollType, BOOL p4, BOOL p5, BOOL p6);
		/**
		 * Return variable is never used in R*'s scripts.
		 * 
		 * Not sure what p2 does. It seems like it would be a time judging by it's usage in R*'s scripts, but didn't seem to affect anything in my testings.
		 * 
		 * enum eRagdollType
		 * {
		 * RD_MALE=0,
		 * RD_FEMALE = 1,
		 * RD_MALE_LARGE = 2,
		 * RD_CUSTOM = 3,
		 * }
		 * 
		 * x, y, and z are coordinates, most likely to where the ped will fall.
		 * 
		 * p8 to p13 are always 0f in R*'s scripts.
		 * 
		 * (Simplified) Example of the usage of the function from R*'s scripts:
		 * ped::set_ped_to_ragdoll_with_fall(ped, 1500, 2000, 1, -entity::get_entity_forward_vector(ped), 1f, 0f, 0f, 0f, 0f, 0f, 0f);
		 * 
		 */
		BOOL SET_PED_TO_RAGDOLL_WITH_FALL(Ped ped, int time, int p2, int ragdollType, float x, float y, float z, float velocity, float p8, float p9, float p10, float p11, float p12, float p13);
		/**
		 * Causes Ped to ragdoll on collision with any object (e.g Running into trashcan). If applied to player you will sometimes trip on the sidewalk.
		 */
		void SET_PED_RAGDOLL_ON_COLLISION(Ped ped, BOOL toggle);
		/**
		 * If the ped handle passed through the parenthesis is in a ragdoll state this will return true.
		 */
		BOOL IS_PED_RAGDOLL(Ped ped);
		BOOL IS_PED_RUNNING_RAGDOLL_TASK(Ped ped);
		void SET_PED_RAGDOLL_FORCE_FALL(Ped ped);
		void RESET_PED_RAGDOLL_TIMER(Ped ped);
		void SET_PED_CAN_RAGDOLL(Ped ped, BOOL toggle);
		BOOL IS_PED_RUNNING_MELEE_TASK(Ped ped);
		BOOL IS_PED_RUNNING_MOBILE_PHONE_TASK(Ped ped);
		BOOL IS_MOBILE_PHONE_TO_PED_EAR(Ped ped);
		/**
		 * Works for both player and peds,
		 * 
		 * enum eRagdollBlockingFlags
		 * {
		 * RBF_BULLET_IMPACT = 0,
		 * RBF_VEHICLE_IMPACT = 1,
		 * RBF_FIRE = 2,
		 * RBF_ELECTROCUTION = 3,
		 * RBF_PLAYER_IMPACT = 4,
		 * RBF_EXPLOSION = 5,0
		 * RBF_IMPACT_OBJECT = 6,
		 * RBF_MELEE = 7,
		 * RBF_RUBBER_BULLET = 8,
		 * RBF_FALLING = 9,
		 * RBF_WATER_JET = 10,
		 * RBF_DROWNING = 11,
		 * _0x9F52E2C4 = 12,
		 * RBF_PLAYER_BUMP = 13,
		 * RBF_PLAYER_RAGDOLL_BUMP = 14,
		 * RBF_PED_RAGDOLL_BUMP = 15,
		 * RBF_VEHICLE_GRAB = 16,
		 * RBF_SMOKE_GRENADE = 17,
		 * };
		 * 
		 * 
		 */
		void SET_RAGDOLL_BLOCKING_FLAGS(Ped ped, int blockingFlag);
		/**
		 * See SET_RAGDOLL_BLOCKING_FLAGS for flags
		 */
		void CLEAR_RAGDOLL_BLOCKING_FLAGS(Ped ped, int blockingFlag);
		void SET_PED_ANGLED_DEFENSIVE_AREA(Ped ped, float p1, float p2, float p3, float p4, float p5, float p6, float p7, BOOL p8, BOOL p9);
		void SET_PED_SPHERE_DEFENSIVE_AREA(Ped ped, float x, float y, float z, float radius, BOOL p5, BOOL p6);
		void SET_PED_DEFENSIVE_SPHERE_ATTACHED_TO_PED(Ped ped, Ped target, float xOffset, float yOffset, float zOffset, float radius, BOOL p6);
		void SET_PED_DEFENSIVE_SPHERE_ATTACHED_TO_VEHICLE(Ped ped, Vehicle target, float xOffset, float yOffset, float zOffset, float radius, BOOL p6);
		void SET_PED_DEFENSIVE_AREA_ATTACHED_TO_PED(Ped ped, Ped attachPed, float p2, float p3, float p4, float p5, float p6, float p7, float p8, BOOL p9, BOOL p10);
		void SET_PED_DEFENSIVE_AREA_DIRECTION(Ped ped, float p1, float p2, float p3, BOOL p4);
		/**
		 * Ped will no longer get angry when you stay near him.
		 */
		void REMOVE_PED_DEFENSIVE_AREA(Ped ped, BOOL toggle);
		Vector3 GET_PED_DEFENSIVE_AREA_POSITION(Ped ped, BOOL p1);
		BOOL IS_PED_DEFENSIVE_AREA_ACTIVE(Ped ped, BOOL p1);
		void SET_PED_PREFERRED_COVER_SET(Ped ped, Any itemSet);
		void REMOVE_PED_PREFERRED_COVER_SET(Ped ped);
		/**
		 * It will revive/cure the injured ped. The condition is ped must not be dead.
		 * 
		 * Upon setting and converting the health int, found, if health falls below 5, the ped will lay on the ground in pain(Maximum default health is 100).
		 * 
		 * This function is well suited there.
		 */
		void REVIVE_INJURED_PED(Ped ped);
		/**
		 * This function will simply bring the dead person back to life.
		 * 
		 * Try not to use it alone, since using this function alone, will make peds fall through ground in hell(well for the most of the times).
		 * 
		 * Instead, before calling this function, you may want to declare the position, where your Resurrected ped to be spawn at.(For instance, Around 2 floats of Player's current position.)
		 * 
		 * Also, disabling any assigned task immediately helped in the number of scenarios, where If you want peds to perform certain decided tasks.
		 */
		void RESURRECT_PED(Ped ped);
		/**
		 * NOTE: Debugging functions are not present in the retail version of the game.
		 * 
		 * *untested but char *name could also be a hash for a localized string
		 */
		void SET_PED_NAME_DEBUG(Ped ped, const char* name);
		/**
		 * Gets the offset the specified ped has moved since the previous tick.
		 * 
		 * If worldSpace is false, the returned offset is relative to the ped. That is, if the ped has moved 1 meter right and 5 meters forward, it'll return 1,5,0.
		 * 
		 * If worldSpace is true, the returned offset is relative to the world. That is, if the ped has moved 1 meter on the X axis and 5 meters on the Y axis, it'll return 1,5,0.
		 */
		Vector3 GET_PED_EXTRACTED_DISPLACEMENT(Ped ped, BOOL worldSpace);
		void SET_PED_DIES_WHEN_INJURED(Ped ped, BOOL toggle);
		void SET_PED_ENABLE_WEAPON_BLOCKING(Ped ped, BOOL toggle);
		/**
		 * p1 was always 1 (true).
		 * 
		 * Kicks the ped from the current vehicle and keeps the rendering-focus on this ped (also disables its collision). If doing this for your player ped, you'll still be able to drive the vehicle.
		 */
		void SPECIAL_FUNCTION_DO_NOT_USE(Ped ped, BOOL p1);
		void RESET_PED_VISIBLE_DAMAGE(Ped ped);
		void APPLY_PED_BLOOD_DAMAGE_BY_ZONE(Ped ped, Any p1, float p2, float p3, Any p4);
		/**
		 * woundTypes:
		 * - soak_splat
		 * - wound_sheet
		 * - BulletSmall
		 * - BulletLarge
		 * - ShotgunSmall
		 * - ShotgunSmallMonolithic
		 * - ShotgunLarge
		 * - ShotgunLargeMonolithic
		 * - NonFatalHeadshot
		 * - stab
		 * - BasicSlash
		 * - Scripted_Ped_Splash_Back
		 * - BackSplash
		 */
		void APPLY_PED_BLOOD(Ped ped, int boneIndex, float xRot, float yRot, float zRot, const char* woundType);
		void APPLY_PED_BLOOD_BY_ZONE(Ped ped, int p1, float p2, float p3, const char* p4);
		void APPLY_PED_BLOOD_SPECIFIC(Ped ped, int p1, float p2, float p3, float p4, float p5, int p6, float p7, const char* p8);
		/**
		 * enum eDamageZone
		 * {
		 * DZ_Torso = 0,
		 * DZ_Head,
		 * DZ_LeftArm,
		 * DZ_RightArm,
		 * DZ_LeftLeg,
		 * DZ_RightLeg,
		 * };
		 * 
		 * Decal Names:
		 * scar
		 * blushing
		 * cs_flush_anger
		 * cs_flush_anger_face
		 * bruise
		 * bruise_large
		 * herpes
		 * ArmorBullet
		 * basic_dirt_cloth
		 * basic_dirt_skin
		 * cs_trev1_dirt
		 * 
		 * APPLY_PED_DAMAGE_DECAL(ped, 1, 0.5f, 0.513f, 0f, 1f, unk, 0, 0, "blushing");
		 */
		void APPLY_PED_DAMAGE_DECAL(Ped ped, int damageZone, float xOffset, float yOffset, float heading, float scale, float alpha, int variation, BOOL fadeIn, const char* decalName);
		/**
		 * Damage Packs:
		 * 
		 * "SCR_TrevorTreeBang"
		 * "HOSPITAL_0"
		 * "HOSPITAL_1"
		 * "HOSPITAL_2"
		 * "HOSPITAL_3"
		 * "HOSPITAL_4"
		 * "HOSPITAL_5"
		 * "HOSPITAL_6"
		 * "HOSPITAL_7"
		 * "HOSPITAL_8"
		 * "HOSPITAL_9"
		 * "SCR_Dumpster"
		 * "BigHitByVehicle"
		 * "SCR_Finale_Michael_Face"
		 * "SCR_Franklin_finb"
		 * "SCR_Finale_Michael"
		 * "SCR_Franklin_finb2"
		 * "Explosion_Med"
		 * "SCR_Torture"
		 * "SCR_TracySplash"
		 * "Skin_Melee_0"
		 * 
		 * Additional damage packs:
		 * 
		 * gist.github.com/alexguirre/f3f47f75ddcf617f416f3c8a55ae2227
		 * Full list of ped damage packs by DurtyFree https://github.com/DurtyFree/gta-v-data-dumps/blob/master/pedDamagePacks.json
		 */
		void APPLY_PED_DAMAGE_PACK(Ped ped, const char* damagePack, float damage, float mult);
		void CLEAR_PED_BLOOD_DAMAGE(Ped ped);
		/**
		 * Somehow related to changing ped's clothes.
		 */
		void CLEAR_PED_BLOOD_DAMAGE_BY_ZONE(Ped ped, int p1);
		void HIDE_PED_BLOOD_DAMAGE_BY_ZONE(Ped ped, Any p1, BOOL p2);
		/**
		 * p1: from 0 to 5 in the b617d scripts.
		 * p2: "blushing" and "ALL" found in the b617d scripts.
		 */
		void CLEAR_PED_DAMAGE_DECAL_BY_ZONE(Ped ped, int p1, const char* p2);
		int GET_PED_DECORATIONS_STATE(Ped ped);
		void MARK_PED_DECORATIONS_AS_CLONED_FROM_LOCAL_PLAYER(Ped ped, BOOL p1);
		/**
		 * It clears the wetness of the selected Ped/Player. Clothes have to be wet to notice the difference.
		 */
		void CLEAR_PED_WETNESS(Ped ped);
		/**
		 * It adds the wetness level to the player clothing/outfit. As if player just got out from water surface.
		 * 
		 * 
		 */
		void SET_PED_WETNESS_HEIGHT(Ped ped, float height);
		/**
		 * combined with PED::SET_PED_WETNESS_HEIGHT(), this native makes the ped drenched in water up to the height specified in the other function
		 */
		void SET_PED_WETNESS_ENABLED_THIS_FRAME(Ped ped);
		void SET_PED_WETNESS(Ped ped, float wetLevel);
		void CLEAR_PED_ENV_DIRT(Ped ped);
		/**
		 * Sweat is set to 100.0 or 0.0 in the decompiled scripts.
		 */
		void SET_PED_SWEAT(Ped ped, float sweat);
		/**
		 * Applies an Item from a PedDecorationCollection to a ped. These include tattoos and shirt decals.
		 * 
		 * collection - PedDecorationCollection filename hash
		 * overlay - Item name hash
		 * 
		 * Example:
		 * Entry inside "mpbeach_overlays.xml" -
		 * <Item>
		 * <uvPos x="0.500000" y="0.500000" />
		 * <scale x="0.600000" y="0.500000" />
		 * <rotation value="0.000000" />
		 * <nameHash>FM_Hair_Fuzz</nameHash>
		 * <txdHash>mp_hair_fuzz</txdHash>
		 * <txtHash>mp_hair_fuzz</txtHash>
		 * <zone>ZONE_HEAD</zone>
		 * <type>TYPE_TATTOO</type>
		 * <faction>FM</faction>
		 * <garment>All</garment>
		 * <gender>GENDER_DONTCARE</gender>
		 * <award />
		 * <awardLevel />
		 * </Item>
		 * 
		 * Code:
		 * PED::ADD_PED_DECORATION_FROM_HASHES(PLAYER::PLAYER_PED_ID(), MISC::GET_HASH_KEY("mpbeach_overlays"), MISC::GET_HASH_KEY("fm_hair_fuzz"))
		 * 
		 * Full list of ped overlays / decorations by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/pedOverlayCollections.json
		 */
		void ADD_PED_DECORATION_FROM_HASHES(Ped ped, Hash collection, Hash overlay);
		/**
		 * Full list of ped overlays / decorations by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/pedOverlayCollections.json
		 */
		void ADD_PED_DECORATION_FROM_HASHES_IN_CORONA(Ped ped, Hash collection, Hash overlay);
		/**
		 * Returns the zoneID for the overlay if it is a member of collection.
		 * enum ePedDecorationZone
		 * {
		 * ZONE_TORSO = 0,
		 * ZONE_HEAD = 1,
		 * ZONE_LEFT_ARM = 2,
		 * ZONE_RIGHT_ARM = 3,
		 * ZONE_LEFT_LEG = 4,
		 * ZONE_RIGHT_LEG = 5,
		 * ZONE_MEDALS = 6,
		 * ZONE_INVALID = 7
		 * };
		 * 
		 * Full list of ped overlays / decorations by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/pedOverlayCollections.json
		 */
		int GET_PED_DECORATION_ZONE_FROM_HASHES(Hash collection, Hash overlay);
		void CLEAR_PED_DECORATIONS(Ped ped);
		void CLEAR_PED_DECORATIONS_LEAVE_SCARS(Ped ped);
		/**
		 * Despite this function's name, it simply returns whether the specified handle is a Ped.
		 */
		BOOL WAS_PED_SKELETON_UPDATED(Ped ped);
		/**
		 * Gets the position of the specified bone of the specified ped.
		 * 
		 * ped: The ped to get the position of a bone from.
		 * boneId: The ID of the bone to get the position from. This is NOT the index.
		 * offsetX: The X-component of the offset to add to the position relative to the bone's rotation.
		 * offsetY: The Y-component of the offset to add to the position relative to the bone's rotation.
		 * offsetZ: The Z-component of the offset to add to the position relative to the bone's rotation.
		 */
		Vector3 GET_PED_BONE_COORDS(Ped ped, int boneId, float offsetX, float offsetY, float offsetZ);
		/**
		 * Creates a new NaturalMotion message.
		 * 
		 * startImmediately: If set to true, the character will perform the message the moment it receives it by GIVE_PED_NM_MESSAGE. If false, the Ped will get the message but won't perform it yet. While it's a boolean value, if negative, the message will not be initialized.
		 * messageId: The ID of the NaturalMotion message.
		 * 
		 * If a message already exists, this function does nothing. A message exists until the point it has been successfully dispatched by GIVE_PED_NM_MESSAGE.
		 */
		void CREATE_NM_MESSAGE(BOOL startImmediately, int messageId);
		/**
		 * Sends the message that was created by a call to CREATE_NM_MESSAGE to the specified Ped.
		 * 
		 * If a message hasn't been created already, this function does nothing.
		 * If the Ped is not ragdolled with Euphoria enabled, this function does nothing.
		 * The following call can be used to ragdoll the Ped with Euphoria enabled: SET_PED_TO_RAGDOLL(ped, 4000, 5000, 1, 1, 1, 0);
		 * 
		 * Call order:
		 * SET_PED_TO_RAGDOLL
		 * CREATE_NM_MESSAGE
		 * GIVE_PED_NM_MESSAGE
		 * 
		 * Multiple messages can be chained. Eg. to make the ped stagger and swing his arms around, the following calls can be made:
		 * SET_PED_TO_RAGDOLL(ped, 4000, 5000, 1, 1, 1, 0);
		 * CREATE_NM_MESSAGE(true, 0); // stopAllBehaviours - Stop all other behaviours, in case the Ped is already doing some Euphoria stuff.
		 * GIVE_PED_NM_MESSAGE(ped); // Dispatch message to Ped.
		 * CREATE_NM_MESSAGE(true, 1151); // staggerFall - Attempt to walk while falling.
		 * GIVE_PED_NM_MESSAGE(ped); // Dispatch message to Ped.
		 * CREATE_NM_MESSAGE(true, 372); // armsWindmill - Swing arms around.
		 * GIVE_PED_NM_MESSAGE(ped); // Dispatch message to Ped.
		 */
		void GIVE_PED_NM_MESSAGE(Ped ped);
		int ADD_SCENARIO_BLOCKING_AREA(float x1, float y1, float z1, float x2, float y2, float z2, BOOL p6, BOOL p7, BOOL p8, BOOL p9, Any p10);
		void REMOVE_SCENARIO_BLOCKING_AREAS();
		void REMOVE_SCENARIO_BLOCKING_AREA(Any p0, BOOL p1);
		void SET_SCENARIO_PEDS_SPAWN_IN_SPHERE_AREA(float x, float y, float z, float range, int p4);
		BOOL DOES_SCENARIO_BLOCKING_AREA_EXISTS(float x1, float y1, float z1, float x2, float y2, float z2);
		/**
		 * Full list of ped scenarios by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/scenariosCompact.json
		 */
		BOOL IS_PED_USING_SCENARIO(Ped ped, const char* scenario);
		BOOL IS_PED_USING_ANY_SCENARIO(Ped ped);
		BOOL SET_PED_PANIC_EXIT_SCENARIO(Any p0, Any p1, Any p2, Any p3);
		void TOGGLE_SCENARIO_PED_COWER_IN_PLACE(Ped ped, BOOL toggle);
		BOOL TRIGGER_PED_SCENARIO_PANICEXITTOFLEE(Any p0, Any p1, Any p2, Any p3);
		BOOL SET_PED_SHOULD_PLAY_DIRECTED_NORMAL_SCENARIO_EXIT(Any p0, Any p1, Any p2, Any p3);
		void SET_PED_SHOULD_PLAY_NORMAL_SCENARIO_EXIT(Ped ped);
		void SET_PED_SHOULD_PLAY_IMMEDIATE_SCENARIO_EXIT(Ped ped);
		BOOL SET_PED_SHOULD_PLAY_FLEE_SCENARIO_EXIT(Ped ped, Any p1, Any p2, Any p3);
		void SET_PED_SHOULD_IGNORE_SCENARIO_EXIT_COLLISION_CHECKS(Ped ped, BOOL p1);
		void SET_PED_SHOULD_IGNORE_SCENARIO_NAV_CHECKS(Any p0, BOOL p1);
		void SET_PED_SHOULD_PROBE_FOR_SCENARIO_EXITS_IN_ONE_FRAME(Any p0, BOOL p1);
		BOOL IS_PED_GESTURING(Any p0);
		void RESET_FACIAL_IDLE_ANIM(Ped ped);
		/**
		 * Full list of animation dictionaries and anims by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/animDictsCompact.json
		 */
		void PLAY_FACIAL_ANIM(Ped ped, const char* animName, const char* animDict);
		/**
		 * Clipsets:
		 * facials@gen_female@base
		 * facials@gen_male@base
		 * facials@p_m_zero@base
		 * 
		 * Typically followed with SET_FACIAL_IDLE_ANIM_OVERRIDE:
		 * mood_drunk_1
		 * mood_stressed_1
		 * mood_happy_1
		 * mood_talking_1
		 * 
		 */
		void SET_FACIAL_CLIPSET(Ped ped, const char* animDict);
		/**
		 * Full list of animation dictionaries and anims by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/animDictsCompact.json
		 */
		void SET_FACIAL_IDLE_ANIM_OVERRIDE(Ped ped, const char* animName, const char* animDict);
		void CLEAR_FACIAL_IDLE_ANIM_OVERRIDE(Ped ped);
		void SET_PED_CAN_PLAY_GESTURE_ANIMS(Ped ped, BOOL toggle);
		/**
		 * p2 usually 0
		 */
		void SET_PED_CAN_PLAY_VISEME_ANIMS(Ped ped, BOOL toggle, BOOL p2);
		void SET_PED_IS_IGNORED_BY_AUTO_OPEN_DOORS(Ped ped, BOOL p1);
		void SET_PED_CAN_PLAY_AMBIENT_ANIMS(Ped ped, BOOL toggle);
		void SET_PED_CAN_PLAY_AMBIENT_BASE_ANIMS(Ped ped, BOOL toggle);
		void TRIGGER_IDLE_ANIMATION_ON_PED(Ped ped);
		void SET_PED_CAN_ARM_IK(Ped ped, BOOL toggle);
		void SET_PED_CAN_BODY_RECOIL_IK(Ped ped, BOOL toggle); // Missing in crossmap
		void SET_PED_CAN_HEAD_IK(Ped ped, BOOL toggle);
		void SET_PED_CAN_LEG_IK(Ped ped, BOOL toggle);
		void SET_PED_CAN_TORSO_IK(Ped ped, BOOL toggle);
		void SET_PED_CAN_TORSO_REACT_IK(Ped ped, BOOL p1);
		void SET_PED_CAN_TORSO_VEHICLE_IK(Ped ped, BOOL p1);
		void SET_PED_CAN_USE_AUTO_CONVERSATION_LOOKAT(Ped ped, BOOL toggle);
		BOOL IS_PED_HEADTRACKING_PED(Ped ped1, Ped ped2);
		BOOL IS_PED_HEADTRACKING_ENTITY(Ped ped, Entity entity);
		/**
		 * This is only called once in the scripts.
		 * 
		 * sub_1CD9(&l_49, 0, getElem(3, &l_34, 4), "MICHAEL", 0, 1);
		 * sub_1CA8("WORLD_HUMAN_SMOKING", 2);
		 * PED::SET_PED_PRIMARY_LOOKAT(getElem(3, &l_34, 4), PLAYER::PLAYER_PED_ID());
		 */
		void SET_PED_PRIMARY_LOOKAT(Ped ped, Ped lookAt);
		void SET_PED_CLOTH_PIN_FRAMES(Any p0, Any p1);
		void SET_PED_CLOTH_PACKAGE_INDEX(Any p0, Any p1);
		void SET_PED_CLOTH_PRONE(Any p0, BOOL p1);
		/**
		 * enum ePedConfigFlags
		 * {
		 * _CPED_CONFIG_FLAG_0xC63DE95E = 1,
		 * CPED_CONFIG_FLAG_NoCriticalHits = 2,
		 * CPED_CONFIG_FLAG_DrownsInWater = 3,
		 * CPED_CONFIG_FLAG_DisableReticuleFixedLockon = 4,
		 * _CPED_CONFIG_FLAG_0x37D196F4 = 5,
		 * _CPED_CONFIG_FLAG_0xE2462399 = 6,
		 * CPED_CONFIG_FLAG_UpperBodyDamageAnimsOnly = 7,
		 * _CPED_CONFIG_FLAG_0xEDDEB838 = 8,
		 * _CPED_CONFIG_FLAG_0xB398B6FD = 9,
		 * _CPED_CONFIG_FLAG_0xF6664E68 = 10,
		 * _CPED_CONFIG_FLAG_0xA05E7CA3 = 11,
		 * _CPED_CONFIG_FLAG_0xCE394045 = 12,
		 * CPED_CONFIG_FLAG_NeverLeavesGroup = 13,
		 * _CPED_CONFIG_FLAG_0xCD8D1411 = 14,
		 * _CPED_CONFIG_FLAG_0xB031F1A9 = 15,
		 * _CPED_CONFIG_FLAG_0xFE65BEE3 = 16,
		 * CPED_CONFIG_FLAG_BlockNonTemporaryEvents = 17,
		 * _CPED_CONFIG_FLAG_0x380165BD = 18,
		 * _CPED_CONFIG_FLAG_0x07C045C7 = 19,
		 * _CPED_CONFIG_FLAG_0x583B5E2D = 20,
		 * _CPED_CONFIG_FLAG_0x475EDA58 = 21,
		 * _CPED_CONFIG_FLAG_0x8629D05B = 22,
		 * _CPED_CONFIG_FLAG_0x1522968B = 23,
		 * CPED_CONFIG_FLAG_IgnoreSeenMelee = 24,
		 * _CPED_CONFIG_FLAG_0x4CC09C4B = 25,
		 * _CPED_CONFIG_FLAG_0x034F3053 = 26,
		 * _CPED_CONFIG_FLAG_0xD91BA7CC = 27,
		 * _CPED_CONFIG_FLAG_0x5C8DC66E = 28,
		 * CPED_CONFIG_FLAG_GetOutUndriveableVehicle = 29,
		 * _CPED_CONFIG_FLAG_0x6580B9D2 = 30,
		 * _CPED_CONFIG_FLAG_0x0EF7A297 = 31,
		 * CPED_CONFIG_FLAG_WillFlyThruWindscreen = 32,
		 * CPED_CONFIG_FLAG_DieWhenRagdoll = 33,
		 * CPED_CONFIG_FLAG_HasHelmet = 34,
		 * CPED_CONFIG_FLAG_UseHelmet = 35,
		 * CPED_CONFIG_FLAG_DontTakeOffHelmet = 36,
		 * _CPED_CONFIG_FLAG_0xB130D17B = 37,
		 * _CPED_CONFIG_FLAG_0x5F071200 = 38,
		 * CPED_CONFIG_FLAG_DisableEvasiveDives = 39,
		 * _CPED_CONFIG_FLAG_0xC287AAFF = 40,
		 * _CPED_CONFIG_FLAG_0x203328CC = 41,
		 * CPED_CONFIG_FLAG_DontInfluenceWantedLevel = 42,
		 * CPED_CONFIG_FLAG_DisablePlayerLockon = 43,
		 * CPED_CONFIG_FLAG_DisableLockonToRandomPeds = 44,
		 * CPED_CONFIG_FLAG_AllowLockonToFriendlyPlayers = 45,
		 * _CPED_CONFIG_FLAG_0xDB115BFA = 46,
		 * CPED_CONFIG_FLAG_PedBeingDeleted = 47,
		 * CPED_CONFIG_FLAG_BlockWeaponSwitching = 48,
		 * _CPED_CONFIG_FLAG_0xF8E99565 = 49,
		 * _CPED_CONFIG_FLAG_0xDD17FEE6 = 50,
		 * _CPED_CONFIG_FLAG_0x7ED9B2C9 = 51,
		 * _CPED_CONFIG_FLAG_NoCollison = 52,
		 * _CPED_CONFIG_FLAG_0x5A6C1F6E = 53,
		 * _CPED_CONFIG_FLAG_0xD749FC41 = 54,
		 * _CPED_CONFIG_FLAG_0x357F63F3 = 55,
		 * _CPED_CONFIG_FLAG_0xC5E60961 = 56,
		 * _CPED_CONFIG_FLAG_0x29275C3E = 57,
		 * CPED_CONFIG_FLAG_IsFiring = 58,
		 * CPED_CONFIG_FLAG_WasFiring = 59,
		 * CPED_CONFIG_FLAG_IsStanding = 60,
		 * CPED_CONFIG_FLAG_WasStanding = 61,
		 * CPED_CONFIG_FLAG_InVehicle = 62,
		 * CPED_CONFIG_FLAG_OnMount = 63,
		 * CPED_CONFIG_FLAG_AttachedToVehicle = 64,
		 * CPED_CONFIG_FLAG_IsSwimming = 65,
		 * CPED_CONFIG_FLAG_WasSwimming = 66,
		 * CPED_CONFIG_FLAG_IsSkiing = 67,
		 * CPED_CONFIG_FLAG_IsSitting = 68,
		 * CPED_CONFIG_FLAG_KilledByStealth = 69,
		 * CPED_CONFIG_FLAG_KilledByTakedown = 70,
		 * CPED_CONFIG_FLAG_Knockedout = 71,
		 * _CPED_CONFIG_FLAG_0x3E3C4560 = 72,
		 * _CPED_CONFIG_FLAG_0x2994C7B7 = 73,
		 * _CPED_CONFIG_FLAG_0x6D59D275 = 74,
		 * CPED_CONFIG_FLAG_UsingCoverPoint = 75,
		 * CPED_CONFIG_FLAG_IsInTheAir = 76,
		 * _CPED_CONFIG_FLAG_0x2D493FB7 = 77,
		 * CPED_CONFIG_FLAG_IsAimingGun = 78,
		 * _CPED_CONFIG_FLAG_0x14D69875 = 79,
		 * _CPED_CONFIG_FLAG_0x40B05311 = 80,
		 * _CPED_CONFIG_FLAG_0x8B230BC5 = 81,
		 * _CPED_CONFIG_FLAG_0xC74E5842 = 82,
		 * _CPED_CONFIG_FLAG_0x9EA86147 = 83,
		 * _CPED_CONFIG_FLAG_0x674C746C = 84,
		 * _CPED_CONFIG_FLAG_0x3E56A8C2 = 85,
		 * _CPED_CONFIG_FLAG_0xC144A1EF = 86,
		 * _CPED_CONFIG_FLAG_0x0548512D = 87,
		 * _CPED_CONFIG_FLAG_0x31C93909 = 88,
		 * _CPED_CONFIG_FLAG_0xA0269315 = 89,
		 * _CPED_CONFIG_FLAG_0xD4D59D4D = 90,
		 * _CPED_CONFIG_FLAG_0x411D4420 = 91,
		 * _CPED_CONFIG_FLAG_0xDF4AEF0D = 92,
		 * CPED_CONFIG_FLAG_ForcePedLoadCover = 93,
		 * _CPED_CONFIG_FLAG_0x300E4CD3 = 94,
		 * _CPED_CONFIG_FLAG_0xF1C5BF04 = 95,
		 * _CPED_CONFIG_FLAG_0x89C2EF13 = 96,
		 * CPED_CONFIG_FLAG_VaultFromCover = 97,
		 * _CPED_CONFIG_FLAG_0x02A852C8 = 98,
		 * _CPED_CONFIG_FLAG_0x3D9407F1 = 99,
		 * _CPED_CONFIG_FLAG_IsDrunk = 100, // 0x319B4558
		 * CPED_CONFIG_FLAG_ForcedAim = 101,
		 * _CPED_CONFIG_FLAG_0xB942D71A = 102,
		 * _CPED_CONFIG_FLAG_0xD26C55A8 = 103,
		 * CPED_CONFIG_FLAG_OpenDoorArmIK = 104,
		 * CPED_CONFIG_FLAG_ForceReload = 105,
		 * CPED_CONFIG_FLAG_DontActivateRagdollFromVehicleImpact = 106,
		 * CPED_CONFIG_FLAG_DontActivateRagdollFromBulletImpact = 107,
		 * CPED_CONFIG_FLAG_DontActivateRagdollFromExplosions = 108,
		 * CPED_CONFIG_FLAG_DontActivateRagdollFromFire = 109,
		 * CPED_CONFIG_FLAG_DontActivateRagdollFromElectrocution = 110,
		 * _CPED_CONFIG_FLAG_0x83C0A4BF = 111,
		 * _CPED_CONFIG_FLAG_0x0E0FAF8C = 112,
		 * CPED_CONFIG_FLAG_KeepWeaponHolsteredUnlessFired = 113,
		 * _CPED_CONFIG_FLAG_0x43B80B79 = 114,
		 * _CPED_CONFIG_FLAG_0x0D2A9309 = 115,
		 * CPED_CONFIG_FLAG_GetOutBurningVehicle = 116,
		 * CPED_CONFIG_FLAG_BumpedByPlayer = 117,
		 * CPED_CONFIG_FLAG_RunFromFiresAndExplosions = 118,
		 * CPED_CONFIG_FLAG_TreatAsPlayerDuringTargeting = 119,
		 * CPED_CONFIG_FLAG_IsHandCuffed = 120,
		 * CPED_CONFIG_FLAG_IsAnkleCuffed = 121,
		 * CPED_CONFIG_FLAG_DisableMelee = 122,
		 * CPED_CONFIG_FLAG_DisableUnarmedDrivebys = 123,
		 * CPED_CONFIG_FLAG_JustGetsPulledOutWhenElectrocuted = 124,
		 * _CPED_CONFIG_FLAG_0x5FED6BFD = 125,
		 * CPED_CONFIG_FLAG_WillNotHotwireLawEnforcementVehicle = 126,
		 * CPED_CONFIG_FLAG_WillCommandeerRatherThanJack = 127,
		 * CPED_CONFIG_FLAG_CanBeAgitated = 128,
		 * CPED_CONFIG_FLAG_ForcePedToFaceLeftInCover = 129,
		 * CPED_CONFIG_FLAG_ForcePedToFaceRightInCover = 130,
		 * CPED_CONFIG_FLAG_BlockPedFromTurningInCover = 131,
		 * CPED_CONFIG_FLAG_KeepRelationshipGroupAfterCleanUp = 132,
		 * CPED_CONFIG_FLAG_ForcePedToBeDragged = 133,
		 * CPED_CONFIG_FLAG_PreventPedFromReactingToBeingJacked = 134,
		 * CPED_CONFIG_FLAG_IsScuba = 135,
		 * CPED_CONFIG_FLAG_WillArrestRatherThanJack = 136,
		 * CPED_CONFIG_FLAG_RemoveDeadExtraFarAway = 137,
		 * CPED_CONFIG_FLAG_RidingTrain = 138,
		 * CPED_CONFIG_FLAG_ArrestResult = 139,
		 * CPED_CONFIG_FLAG_CanAttackFriendly = 140,
		 * CPED_CONFIG_FLAG_WillJackAnyPlayer = 141,
		 * _CPED_CONFIG_FLAG_0x6901E731 = 142,
		 * _CPED_CONFIG_FLAG_0x9EC9BF6C = 143,
		 * CPED_CONFIG_FLAG_WillJackWantedPlayersRatherThanStealCar = 144,
		 * CPED_CONFIG_FLAG_ShootingAnimFlag = 145,
		 * CPED_CONFIG_FLAG_DisableLadderClimbing = 146,
		 * CPED_CONFIG_FLAG_StairsDetected = 147,
		 * CPED_CONFIG_FLAG_SlopeDetected = 148,
		 * _CPED_CONFIG_FLAG_0x1A15670B = 149,
		 * CPED_CONFIG_FLAG_CowerInsteadOfFlee = 150,
		 * CPED_CONFIG_FLAG_CanActivateRagdollWhenVehicleUpsideDown = 151,
		 * CPED_CONFIG_FLAG_AlwaysRespondToCriesForHelp = 152,
		 * CPED_CONFIG_FLAG_DisableBloodPoolCreation = 153,
		 * CPED_CONFIG_FLAG_ShouldFixIfNoCollision = 154,
		 * CPED_CONFIG_FLAG_CanPerformArrest = 155,
		 * CPED_CONFIG_FLAG_CanPerformUncuff = 156,
		 * CPED_CONFIG_FLAG_CanBeArrested = 157,
		 * _CPED_CONFIG_FLAG_0xF7960FF5 = 158,
		 * CPED_CONFIG_FLAG_PlayerPreferFrontSeatMP = 159,
		 * _CPED_CONFIG_FLAG_0x0C6C3099 = 160,
		 * _CPED_CONFIG_FLAG_0x645F927A = 161,
		 * _CPED_CONFIG_FLAG_0xA86549B9 = 162,
		 * _CPED_CONFIG_FLAG_0x8AAF337A = 163,
		 * _CPED_CONFIG_FLAG_0x13BAA6E7 = 164,
		 * _CPED_CONFIG_FLAG_0x5FB9D1F5 = 165,
		 * CPED_CONFIG_FLAG_IsInjured = 166,
		 * CPED_CONFIG_FLAG_DontEnterVehiclesInPlayersGroup = 167,
		 * _CPED_CONFIG_FLAG_0xD8072639 = 168,
		 * CPED_CONFIG_FLAG_PreventAllMeleeTaunts = 169,
		 * CPED_CONFIG_FLAG_ForceDirectEntry = 170,
		 * CPED_CONFIG_FLAG_AlwaysSeeApproachingVehicles = 171,
		 * CPED_CONFIG_FLAG_CanDiveAwayFromApproachingVehicles = 172,
		 * CPED_CONFIG_FLAG_AllowPlayerToInterruptVehicleEntryExit = 173,
		 * CPED_CONFIG_FLAG_OnlyAttackLawIfPlayerIsWanted = 174,
		 * _CPED_CONFIG_FLAG_0x90008BFA = 175,
		 * _CPED_CONFIG_FLAG_0x07C7A910 = 176,
		 * CPED_CONFIG_FLAG_PedsJackingMeDontGetIn = 177,
		 * _CPED_CONFIG_FLAG_0xCE4E8BE2 = 178,
		 * CPED_CONFIG_FLAG_PedIgnoresAnimInterruptEvents = 179,
		 * CPED_CONFIG_FLAG_IsInCustody = 180,
		 * CPED_CONFIG_FLAG_ForceStandardBumpReactionThresholds = 181,
		 * CPED_CONFIG_FLAG_LawWillOnlyAttackIfPlayerIsWanted = 182,
		 * CPED_CONFIG_FLAG_IsAgitated = 183,
		 * CPED_CONFIG_FLAG_PreventAutoShuffleToDriversSeat = 184,
		 * CPED_CONFIG_FLAG_UseKinematicModeWhenStationary = 185,
		 * CPED_CONFIG_FLAG_EnableWeaponBlocking = 186,
		 * CPED_CONFIG_FLAG_HasHurtStarted = 187,
		 * CPED_CONFIG_FLAG_DisableHurt = 188,
		 * CPED_CONFIG_FLAG_PlayerIsWeird = 189,
		 * _CPED_CONFIG_FLAG_0x32FC208B = 190,
		 * _CPED_CONFIG_FLAG_0x0C296E5A = 191,
		 * _CPED_CONFIG_FLAG_0xE63B73EC = 192,
		 * CPED_CONFIG_FLAG_DoNothingWhenOnFootByDefault = 193,
		 * CPED_CONFIG_FLAG_UsingScenario = 194,
		 * CPED_CONFIG_FLAG_VisibleOnScreen = 195,
		 * _CPED_CONFIG_FLAG_0xD88C58A1 = 196,
		 * _CPED_CONFIG_FLAG_0x5A3DCF43 = 197,
		 * _CPED_CONFIG_FLAG_0xEA02B420 = 198,
		 * CPED_CONFIG_FLAG_DontActivateRagdollOnVehicleCollisionWhenDead = 199,
		 * CPED_CONFIG_FLAG_HasBeenInArmedCombat = 200,
		 * _CPED_CONFIG_FLAG_0x5E6466F6 = 201,
		 * CPED_CONFIG_FLAG_Avoidance_Ignore_All = 202,
		 * CPED_CONFIG_FLAG_Avoidance_Ignored_by_All = 203,
		 * CPED_CONFIG_FLAG_Avoidance_Ignore_Group1 = 204,
		 * CPED_CONFIG_FLAG_Avoidance_Member_of_Group1 = 205,
		 * CPED_CONFIG_FLAG_ForcedToUseSpecificGroupSeatIndex = 206,
		 * _CPED_CONFIG_FLAG_0x415B26B9 = 207,
		 * CPED_CONFIG_FLAG_DisableExplosionReactions = 208,
		 * CPED_CONFIG_FLAG_DodgedPlayer = 209,
		 * CPED_CONFIG_FLAG_WaitingForPlayerControlInterrupt = 210,
		 * CPED_CONFIG_FLAG_ForcedToStayInCover = 211,
		 * CPED_CONFIG_FLAG_GeneratesSoundEvents = 212,
		 * CPED_CONFIG_FLAG_ListensToSoundEvents = 213,
		 * CPED_CONFIG_FLAG_AllowToBeTargetedInAVehicle = 214,
		 * CPED_CONFIG_FLAG_WaitForDirectEntryPointToBeFreeWhenExiting = 215,
		 * CPED_CONFIG_FLAG_OnlyRequireOnePressToExitVehicle = 216,
		 * CPED_CONFIG_FLAG_ForceExitToSkyDive = 217,
		 * _CPED_CONFIG_FLAG_0x3C7DF9DF = 218,
		 * _CPED_CONFIG_FLAG_0x848FFEF2 = 219,
		 * CPED_CONFIG_FLAG_DontEnterLeadersVehicle = 220,
		 * CPED_CONFIG_FLAG_DisableExitToSkyDive = 221,
		 * _CPED_CONFIG_FLAG_0x84F722FA = 222,
		 * _CPED_CONFIG_FLAG_Shrink = 223, // 0xD1B87B1F
		 * _CPED_CONFIG_FLAG_0x728AA918 = 224,
		 * CPED_CONFIG_FLAG_DisablePotentialToBeWalkedIntoResponse = 225,
		 * CPED_CONFIG_FLAG_DisablePedAvoidance = 226,
		 * CPED_CONFIG_FLAG_ForceRagdollUponDeath = 227,
		 * _CPED_CONFIG_FLAG_0x1EA7225F = 228,
		 * CPED_CONFIG_FLAG_DisablePanicInVehicle = 229,
		 * CPED_CONFIG_FLAG_AllowedToDetachTrailer = 230,
		 * _CPED_CONFIG_FLAG_0xFC3E572D = 231,
		 * _CPED_CONFIG_FLAG_0x08E9F9CF = 232,
		 * _CPED_CONFIG_FLAG_0x2D3BA52D = 233,
		 * _CPED_CONFIG_FLAG_0xFD2F53EA = 234,
		 * _CPED_CONFIG_FLAG_0x31A1B03B = 235,
		 * CPED_CONFIG_FLAG_IsHoldingProp = 236,
		 * CPED_CONFIG_FLAG_BlocksPathingWhenDead = 237,
		 * _CPED_CONFIG_FLAG_0xCE57C9A3 = 238,
		 * _CPED_CONFIG_FLAG_0x26149198 = 239,
		 * CPED_CONFIG_FLAG_ForceSkinCharacterCloth = 240,
		 * CPED_CONFIG_FLAG_LeaveEngineOnWhenExitingVehicles = 241,
		 * CPED_CONFIG_FLAG_PhoneDisableTextingAnimations = 242,
		 * CPED_CONFIG_FLAG_PhoneDisableTalkingAnimations = 243,
		 * CPED_CONFIG_FLAG_PhoneDisableCameraAnimations = 244,
		 * CPED_CONFIG_FLAG_DisableBlindFiringInShotReactions = 245,
		 * CPED_CONFIG_FLAG_AllowNearbyCoverUsage = 246,
		 * _CPED_CONFIG_FLAG_0x0C754ACA = 247,
		 * CPED_CONFIG_FLAG_CanPlayInCarIdles = 248,
		 * CPED_CONFIG_FLAG_CanAttackNonWantedPlayerAsLaw = 249,
		 * CPED_CONFIG_FLAG_WillTakeDamageWhenVehicleCrashes = 250,
		 * CPED_CONFIG_FLAG_AICanDrivePlayerAsRearPassenger = 251,
		 * CPED_CONFIG_FLAG_PlayerCanJackFriendlyPlayers = 252,
		 * CPED_CONFIG_FLAG_OnStairs = 253,
		 * _CPED_CONFIG_FLAG_0xE1A2F73F = 254,
		 * CPED_CONFIG_FLAG_AIDriverAllowFriendlyPassengerSeatEntry = 255,
		 * _CPED_CONFIG_FLAG_0xF1EB20A9 = 256,
		 * CPED_CONFIG_FLAG_AllowMissionPedToUseInjuredMovement = 257,
		 * _CPED_CONFIG_FLAG_0x329DCF1A = 258,
		 * _CPED_CONFIG_FLAG_0x8D90DD1B = 259,
		 * _CPED_CONFIG_FLAG_0xB8A292B7 = 260,
		 * CPED_CONFIG_FLAG_PreventUsingLowerPrioritySeats = 261,
		 * _CPED_CONFIG_FLAG_0x2AF558F0 = 262,
		 * _CPED_CONFIG_FLAG_0x82251455 = 263,
		 * _CPED_CONFIG_FLAG_0x30CF498B = 264,
		 * _CPED_CONFIG_FLAG_0xE1CD50AF = 265,
		 * _CPED_CONFIG_FLAG_0x72E4AE48 = 266,
		 * _CPED_CONFIG_FLAG_0xC2657EA1 = 267,
		 * CPED_CONFIG_FLAG_TeleportToLeaderVehicle = 268,
		 * CPED_CONFIG_FLAG_Avoidance_Ignore_WeirdPedBuffer = 269,
		 * CPED_CONFIG_FLAG_OnStairSlope = 270,
		 * _CPED_CONFIG_FLAG_0xA0897933 = 271,
		 * CPED_CONFIG_FLAG_DontBlipCop = 272,
		 * CPED_CONFIG_FLAG_ClimbedShiftedFence = 273,
		 * _CPED_CONFIG_FLAG_0xF7823618 = 274,
		 * CPED_CONFIG_FLAG_KillWhenTrapped = 275,
		 * CPED_CONFIG_FLAG_EdgeDetected = 276,
		 * _CPED_CONFIG_FLAG_0x92B67896 = 277,
		 * _CPED_CONFIG_FLAG_0xCAD677C9 = 278,
		 * CPED_CONFIG_FLAG_AvoidTearGas = 279,
		 * _CPED_CONFIG_FLAG_0x5276AC7B = 280,
		 * CPED_CONFIG_FLAG_DisableGoToWritheWhenInjured = 281,
		 * CPED_CONFIG_FLAG_OnlyUseForcedSeatWhenEnteringHeliInGroup = 282,
		 * _CPED_CONFIG_FLAG_0x9139724D = 283,
		 * _CPED_CONFIG_FLAG_0xA1457461 = 284,
		 * CPED_CONFIG_FLAG_DisableWeirdPedEvents = 285,
		 * CPED_CONFIG_FLAG_ShouldChargeNow = 286,
		 * CPED_CONFIG_FLAG_RagdollingOnBoat = 287,
		 * CPED_CONFIG_FLAG_HasBrandishedWeapon = 288,
		 * _CPED_CONFIG_FLAG_0x1B9EE8A1 = 289,
		 * _CPED_CONFIG_FLAG_0xF3F5758C = 290,
		 * _CPED_CONFIG_FLAG_0x2A9307F1 = 291,
		 * _CPED_CONFIG_FLAG_FreezePosition = 292, // 0x7403D216
		 * _CPED_CONFIG_FLAG_0xA06A3C6C = 293,
		 * CPED_CONFIG_FLAG_DisableShockingEvents = 294,
		 * _CPED_CONFIG_FLAG_0xF8DA25A5 = 295,
		 * CPED_CONFIG_FLAG_NeverReactToPedOnRoof = 296,
		 * _CPED_CONFIG_FLAG_0xB31F1187 = 297,
		 * _CPED_CONFIG_FLAG_0x84315402 = 298,
		 * CPED_CONFIG_FLAG_DisableShockingDrivingOnPavementEvents = 299,
		 * _CPED_CONFIG_FLAG_0xC7829B67 = 300,
		 * CPED_CONFIG_FLAG_DisablePedConstraints = 301,
		 * CPED_CONFIG_FLAG_ForceInitialPeekInCover = 302,
		 * _CPED_CONFIG_FLAG_0x2ADA871B = 303,
		 * _CPED_CONFIG_FLAG_0x47BC8A58 = 304,
		 * CPED_CONFIG_FLAG_DisableJumpingFromVehiclesAfterLeader = 305,
		 * _CPED_CONFIG_FLAG_0x4A133C50 = 306,
		 * _CPED_CONFIG_FLAG_0xC58099C3 = 307,
		 * _CPED_CONFIG_FLAG_0xF3D76D41 = 308,
		 * _CPED_CONFIG_FLAG_0xB0EEE9F2 = 309,
		 * CPED_CONFIG_FLAG_IsInCluster = 310,
		 * CPED_CONFIG_FLAG_ShoutToGroupOnPlayerMelee = 311,
		 * CPED_CONFIG_FLAG_IgnoredByAutoOpenDoors = 312,
		 * _CPED_CONFIG_FLAG_0xD4136C22 = 313,
		 * CPED_CONFIG_FLAG_ForceIgnoreMeleeActiveCombatant = 314,
		 * CPED_CONFIG_FLAG_CheckLoSForSoundEvents = 315,
		 * _CPED_CONFIG_FLAG_0xD5C98277 = 316,
		 * CPED_CONFIG_FLAG_CanSayFollowedByPlayerAudio = 317,
		 * CPED_CONFIG_FLAG_ActivateRagdollFromMinorPlayerContact = 318,
		 * _CPED_CONFIG_FLAG_0xD8BE1D54 = 319,
		 * CPED_CONFIG_FLAG_ForcePoseCharacterCloth = 320,
		 * CPED_CONFIG_FLAG_HasClothCollisionBounds = 321,
		 * CPED_CONFIG_FLAG_HasHighHeels = 322,
		 * _CPED_CONFIG_FLAG_0x86B01E54 = 323,
		 * CPED_CONFIG_FLAG_DontBehaveLikeLaw = 324,
		 * _CPED_CONFIG_FLAG_0xC03B736C = 325, // SpawnedAtScenario?
		 * CPED_CONFIG_FLAG_DisablePoliceInvestigatingBody = 326,
		 * CPED_CONFIG_FLAG_DisableWritheShootFromGround = 327,
		 * CPED_CONFIG_FLAG_LowerPriorityOfWarpSeats = 328,
		 * CPED_CONFIG_FLAG_DisableTalkTo = 329,
		 * CPED_CONFIG_FLAG_DontBlip = 330,
		 * CPED_CONFIG_FLAG_IsSwitchingWeapon = 331,
		 * CPED_CONFIG_FLAG_IgnoreLegIkRestrictions = 332,
		 * _CPED_CONFIG_FLAG_0x150468FD = 333,
		 * _CPED_CONFIG_FLAG_0x914EBD6B = 334,
		 * _CPED_CONFIG_FLAG_0x79AF3B6D = 335,
		 * _CPED_CONFIG_FLAG_0x75C7A632 = 336,
		 * _CPED_CONFIG_FLAG_0x52D530E2 = 337,
		 * _CPED_CONFIG_FLAG_0xDB2A90E0 = 338,
		 * CPED_CONFIG_FLAG_AllowTaskDoNothingTimeslicing = 339,
		 * _CPED_CONFIG_FLAG_0x12ADB567 = 340,
		 * _CPED_CONFIG_FLAG_0x105C8518 = 341,
		 * CPED_CONFIG_FLAG_NotAllowedToJackAnyPlayers = 342,
		 * _CPED_CONFIG_FLAG_0xED152C3E = 343,
		 * _CPED_CONFIG_FLAG_0xA0EFE6A8 = 344,
		 * CPED_CONFIG_FLAG_AlwaysLeaveTrainUponArrival = 345,
		 * _CPED_CONFIG_FLAG_0xCDDFE830 = 346,
		 * CPED_CONFIG_FLAG_OnlyWritheFromWeaponDamage = 347,
		 * CPED_CONFIG_FLAG_UseSloMoBloodVfx = 348,
		 * CPED_CONFIG_FLAG_EquipJetpack = 349,
		 * CPED_CONFIG_FLAG_PreventDraggedOutOfCarThreatResponse = 350,
		 * _CPED_CONFIG_FLAG_0xE13D1F7C = 351,
		 * _CPED_CONFIG_FLAG_0x40E25FB9 = 352,
		 * _CPED_CONFIG_FLAG_0x930629D9 = 353,
		 * _CPED_CONFIG_FLAG_0xECCF0C7F = 354,
		 * _CPED_CONFIG_FLAG_0xB6E9613B = 355,
		 * CPED_CONFIG_FLAG_ForceDeepSurfaceCheck = 356,
		 * CPED_CONFIG_FLAG_DisableDeepSurfaceAnims = 357,
		 * CPED_CONFIG_FLAG_DontBlipNotSynced = 358,
		 * CPED_CONFIG_FLAG_IsDuckingInVehicle = 359,
		 * CPED_CONFIG_FLAG_PreventAutoShuffleToTurretSeat = 360,
		 * CPED_CONFIG_FLAG_DisableEventInteriorStatusCheck = 361,
		 * CPED_CONFIG_FLAG_HasReserveParachute = 362,
		 * CPED_CONFIG_FLAG_UseReserveParachute = 363,
		 * CPED_CONFIG_FLAG_TreatDislikeAsHateWhenInCombat = 364,
		 * CPED_CONFIG_FLAG_OnlyUpdateTargetWantedIfSeen = 365,
		 * CPED_CONFIG_FLAG_AllowAutoShuffleToDriversSeat = 366,
		 * _CPED_CONFIG_FLAG_0xD7E07D37 = 367,
		 * _CPED_CONFIG_FLAG_0x03C4FD24 = 368,
		 * _CPED_CONFIG_FLAG_0x7675789A = 369,
		 * _CPED_CONFIG_FLAG_0xB7288A88 = 370,
		 * _CPED_CONFIG_FLAG_0xC06B6291 = 371,
		 * CPED_CONFIG_FLAG_PreventReactingToSilencedCloneBullets = 372,
		 * CPED_CONFIG_FLAG_DisableInjuredCryForHelpEvents = 373,
		 * CPED_CONFIG_FLAG_NeverLeaveTrain = 374,
		 * CPED_CONFIG_FLAG_DontDropJetpackOnDeath = 375,
		 * _CPED_CONFIG_FLAG_0x147F1FFB = 376,
		 * _CPED_CONFIG_FLAG_0x4376DD79 = 377,
		 * _CPED_CONFIG_FLAG_0xCD3DB518 = 378,
		 * _CPED_CONFIG_FLAG_0xFE4BA4B6 = 379,
		 * CPED_CONFIG_FLAG_DisableAutoEquipHelmetsInBikes = 380,
		 * _CPED_CONFIG_FLAG_0xBCD816CD = 381,
		 * _CPED_CONFIG_FLAG_0xCF02DD69 = 382,
		 * _CPED_CONFIG_FLAG_0xF73AFA2E = 383,
		 * _CPED_CONFIG_FLAG_0x80B9A9D0 = 384,
		 * _CPED_CONFIG_FLAG_0xF601F7EE = 385,
		 * _CPED_CONFIG_FLAG_0xA91350FC = 386,
		 * _CPED_CONFIG_FLAG_0x3AB23B96 = 387,
		 * CPED_CONFIG_FLAG_IsClimbingLadder = 388,
		 * CPED_CONFIG_FLAG_HasBareFeet = 389,
		 * CPED_CONFIG_FLAG_UNUSED_REPLACE_ME_2 = 390,
		 * CPED_CONFIG_FLAG_GoOnWithoutVehicleIfItIsUnableToGetBackToRoad = 391,
		 * CPED_CONFIG_FLAG_BlockDroppingHealthSnacksOnDeath = 392,
		 * _CPED_CONFIG_FLAG_0xC11D3E8F = 393,
		 * CPED_CONFIG_FLAG_ForceThreatResponseToNonFriendToFriendMeleeActions = 394,
		 * CPED_CONFIG_FLAG_DontRespondToRandomPedsDamage = 395,
		 * CPED_CONFIG_FLAG_AllowContinuousThreatResponseWantedLevelUpdates = 396,
		 * CPED_CONFIG_FLAG_KeepTargetLossResponseOnCleanup = 397,
		 * CPED_CONFIG_FLAG_PlayersDontDragMeOutOfCar = 398,
		 * CPED_CONFIG_FLAG_BroadcastRepondedToThreatWhenGoingToPointShooting = 399,
		 * CPED_CONFIG_FLAG_IgnorePedTypeForIsFriendlyWith = 400,
		 * CPED_CONFIG_FLAG_TreatNonFriendlyAsHateWhenInCombat = 401,
		 * CPED_CONFIG_FLAG_DontLeaveVehicleIfLeaderNotInVehicle = 402,
		 * _CPED_CONFIG_FLAG_0x5E5B9591 = 403,
		 * CPED_CONFIG_FLAG_AllowMeleeReactionIfMeleeProofIsOn = 404,
		 * _CPED_CONFIG_FLAG_0x77840177 = 405,
		 * _CPED_CONFIG_FLAG_0x1C7ACAC4 = 406,
		 * CPED_CONFIG_FLAG_UseNormalExplosionDamageWhenBlownUpInVehicle = 407,
		 * CPED_CONFIG_FLAG_DisableHomingMissileLockForVehiclePedInside = 408,
		 * CPED_CONFIG_FLAG_DisableTakeOffScubaGear = 409,
		 * CPED_CONFIG_FLAG_IgnoreMeleeFistWeaponDamageMult = 410,
		 * CPED_CONFIG_FLAG_LawPedsCanFleeFromNonWantedPlayer = 411,
		 * CPED_CONFIG_FLAG_ForceBlipSecurityPedsIfPlayerIsWanted = 412,
		 * CPED_CONFIG_FLAG_IsHolsteringWeapon = 413,
		 * CPED_CONFIG_FLAG_UseGoToPointForScenarioNavigation = 414,
		 * CPED_CONFIG_FLAG_DontClearLocalPassengersWantedLevel = 415,
		 * CPED_CONFIG_FLAG_BlockAutoSwapOnWeaponPickups = 416,
		 * CPED_CONFIG_FLAG_ThisPedIsATargetPriorityForAI = 417,
		 * CPED_CONFIG_FLAG_IsSwitchingHelmetVisor = 418,
		 * CPED_CONFIG_FLAG_ForceHelmetVisorSwitch = 419,
		 * _CPED_CONFIG_FLAG_0xCFF5F6DE = 420,
		 * CPED_CONFIG_FLAG_UseOverrideFootstepPtFx = 421,
		 * CPED_CONFIG_FLAG_DisableVehicleCombat = 422,
		 * _CPED_CONFIG_FLAG_0xFE401D26 = 423,
		 * CPED_CONFIG_FLAG_FallsLikeAircraft = 424,
		 * _CPED_CONFIG_FLAG_0x2B42AE82 = 425,
		 * CPED_CONFIG_FLAG_UseLockpickVehicleEntryAnimations = 426,
		 * CPED_CONFIG_FLAG_IgnoreInteriorCheckForSprinting = 427,
		 * CPED_CONFIG_FLAG_SwatHeliSpawnWithinLastSpottedLocation = 428,
		 * CPED_CONFIG_FLAG_DisableStartEngine = 429,
		 * CPED_CONFIG_FLAG_IgnoreBeingOnFire = 430,
		 * CPED_CONFIG_FLAG_DisableTurretOrRearSeatPreference = 431,
		 * CPED_CONFIG_FLAG_DisableWantedHelicopterSpawning = 432,
		 * CPED_CONFIG_FLAG_UseTargetPerceptionForCreatingAimedAtEvents = 433,
		 * CPED_CONFIG_FLAG_DisableHomingMissileLockon = 434,
		 * CPED_CONFIG_FLAG_ForceIgnoreMaxMeleeActiveSupportCombatants = 435,
		 * CPED_CONFIG_FLAG_StayInDefensiveAreaWhenInVehicle = 436,
		 * CPED_CONFIG_FLAG_DontShoutTargetPosition = 437,
		 * CPED_CONFIG_FLAG_DisableHelmetArmor = 438,
		 * _CPED_CONFIG_FLAG_0xCB7F3A1E = 439,
		 * _CPED_CONFIG_FLAG_0x50178878 = 440,
		 * CPED_CONFIG_FLAG_PreventVehExitDueToInvalidWeapon = 441,
		 * CPED_CONFIG_FLAG_IgnoreNetSessionFriendlyFireCheckForAllowDamage = 442,
		 * CPED_CONFIG_FLAG_DontLeaveCombatIfTargetPlayerIsAttackedByPolice = 443,
		 * CPED_CONFIG_FLAG_CheckLockedBeforeWarp = 444,
		 * CPED_CONFIG_FLAG_DontShuffleInVehicleToMakeRoom = 445,
		 * CPED_CONFIG_FLAG_GiveWeaponOnGetup = 446,
		 * CPED_CONFIG_FLAG_DontHitVehicleWithProjectiles = 447,
		 * CPED_CONFIG_FLAG_DisableForcedEntryForOpenVehiclesFromTryLockedDoor = 448,
		 * CPED_CONFIG_FLAG_FiresDummyRockets = 449,
		 * CPED_CONFIG_FLAG_PedIsArresting = 450,
		 * CPED_CONFIG_FLAG_IsDecoyPed = 451,
		 * CPED_CONFIG_FLAG_HasEstablishedDecoy = 452,
		 * CPED_CONFIG_FLAG_BlockDispatchedHelicoptersFromLanding = 453,
		 * CPED_CONFIG_FLAG_DontCryForHelpOnStun = 454,
		 * _CPED_CONFIG_FLAG_0xB68D3EAB = 455,
		 * CPED_CONFIG_FLAG_CanBeIncapacitated = 456,
		 * _CPED_CONFIG_FLAG_0x4BD5EBAD = 457,
		 * CPED_CONFIG_FLAG_DontChangeTargetFromMelee = 458,
		 * };
		 */
		void SET_PED_CONFIG_FLAG(Ped ped, int flagId, BOOL value);
		/**
		 * PED::SET_PED_RESET_FLAG(PLAYER::PLAYER_PED_ID(), 240, 1);
		 * Known values:
		 * PRF_PreventGoingIntoStillInVehicleState = 236 *(fanatic2.c)*
		 */
		void SET_PED_RESET_FLAG(Ped ped, int flagId, BOOL doReset);
		/**
		 * See SET_PED_CONFIG_FLAG
		 */
		BOOL GET_PED_CONFIG_FLAG(Ped ped, int flagId, BOOL p2);
		BOOL GET_PED_RESET_FLAG(Ped ped, int flagId);
		void SET_PED_GROUP_MEMBER_PASSENGER_INDEX(Ped ped, int index);
		void SET_PED_CAN_EVASIVE_DIVE(Ped ped, BOOL toggle);
		/**
		 * Presumably returns the Entity that the Ped is currently diving out of the way of.
		 * 
		 * var num3;
		 * if (PED::IS_PED_EVASIVE_DIVING(A_0, &num3) != 0)
		 * if (ENTITY::IS_ENTITY_A_VEHICLE(num3) != 0)
		 */
		BOOL IS_PED_EVASIVE_DIVING(Ped ped, Entity* evadingEntity);
		void SET_PED_SHOOTS_AT_COORD(Ped ped, float x, float y, float z, BOOL toggle);
		/**
		 * Full list of peds by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/peds.json
		 */
		void SET_PED_MODEL_IS_SUPPRESSED(Hash modelHash, BOOL toggle);
		void STOP_ANY_PED_MODEL_BEING_SUPPRESSED();
		void SET_PED_CAN_BE_TARGETED_WHEN_INJURED(Ped ped, BOOL toggle);
		void SET_PED_GENERATES_DEAD_BODY_EVENTS(Ped ped, BOOL toggle);
		void BLOCK_PED_FROM_GENERATING_DEAD_BODY_EVENTS_WHEN_DEAD(Ped ped, BOOL toggle);
		void SET_PED_WILL_ONLY_ATTACK_WANTED_PLAYER(Any p0, Any p1);
		void SET_PED_CAN_RAGDOLL_FROM_PLAYER_IMPACT(Ped ped, BOOL toggle);
		/**
		 * PoliceMotorcycleHelmet   1024
		 * RegularMotorcycleHelmet   4096
		 * FiremanHelmet 16384
		 * PilotHeadset  32768
		 * PilotHelmet   65536
		 * --
		 * p2 is generally 4096 or 16384 in the scripts. p1 varies between 1 and 0.
		 */
		void GIVE_PED_HELMET(Ped ped, BOOL cannotRemove, int helmetFlag, int textureIndex);
		void REMOVE_PED_HELMET(Ped ped, BOOL instantly);
		BOOL IS_PED_TAKING_OFF_HELMET(Ped ped);
		void SET_PED_HELMET(Ped ped, BOOL canWearHelmet);
		void SET_PED_HELMET_FLAG(Ped ped, int helmetFlag);
		/**
		 * List of component/props ID
		 * gtaxscripting.blogspot.com/2016/04/gta-v-peds-component-and-props.html
		 */
		void SET_PED_HELMET_PROP_INDEX(Ped ped, int propIndex, BOOL p2);
		void SET_PED_HELMET_VISOR_PROP_INDICES(Ped ped, BOOL p1, int p2, int p3);
		BOOL IS_PED_HELMET_VISOR_UP(Ped ped);
		void SET_PED_HELMET_TEXTURE_INDEX(Ped ped, int textureIndex);
		/**
		 * Returns true if the ped passed through the parenthesis is wearing a helmet.
		 */
		BOOL IS_PED_WEARING_HELMET(Ped ped);
		void CLEAR_PED_STORED_HAT_PROP(Ped ped);
		int GET_PED_HELMET_STORED_HAT_PROP_INDEX(Ped ped);
		int GET_PED_HELMET_STORED_HAT_TEX_INDEX(Ped ped);
		BOOL IS_CURRENT_HEAD_PROP_A_HELMET(Any p0);
		void SET_PED_TO_LOAD_COVER(Ped ped, BOOL toggle);
		/**
		 * It simply makes the said ped to cower behind cover object(wall, desk, car)
		 * 
		 * Peds flee attributes must be set to not to flee, first. Else, most of the peds, will just flee from gunshot sounds or any other panic situations.
		 */
		void SET_PED_CAN_COWER_IN_COVER(Ped ped, BOOL toggle);
		void SET_PED_CAN_PEEK_IN_COVER(Ped ped, BOOL toggle);
		/**
		 * This native does absolutely nothing, just a nullsub
		 */
		void SET_PED_PLAYS_HEAD_ON_HORN_ANIM_WHEN_DIES_IN_VEHICLE(Ped ped, BOOL toggle);
		/**
		 * "IK" stands for "Inverse kinematics." I assume this has something to do with how the ped uses his legs to balance. In the scripts, the second parameter is always an int with a value of 2, 0, or sometimes 1
		 */
		void SET_PED_LEG_IK_MODE(Ped ped, int mode);
		void SET_PED_MOTION_BLUR(Ped ped, BOOL toggle);
		void SET_PED_CAN_SWITCH_WEAPON(Ped ped, BOOL toggle);
		void SET_PED_DIES_INSTANTLY_IN_WATER(Ped ped, BOOL toggle);
		/**
		 * Only appears in lamar1 script.
		 */
		void SET_LADDER_CLIMB_INPUT_STATE(Ped ped, int p1);
		void STOP_PED_WEAPON_FIRING_WHEN_DROPPED(Ped ped);
		void SET_SCRIPTED_ANIM_SEAT_OFFSET(Ped ped, float p1);
		/**
		 * enum eCombatMovement // 0x4F456B61
		 * {
		 * CM_Stationary,
		 * CM_Defensive,
		 * CM_WillAdvance,
		 * CM_WillRetreat
		 * };
		 */
		void SET_PED_COMBAT_MOVEMENT(Ped ped, int combatMovement);
		/**
		 * See SET_PED_COMBAT_MOVEMENT
		 */
		int GET_PED_COMBAT_MOVEMENT(Ped ped);
		/**
		 * enum eCombatAbility // 0xE793438C
		 * {
		 * CA_Poor,
		 * CA_Average,
		 * CA_Professional,
		 * CA_NumTypes
		 * };
		 */
		void SET_PED_COMBAT_ABILITY(Ped ped, int abilityLevel);
		/**
		 * enum eCombatRange // 0xB69160F5
		 * {
		 * CR_Near,
		 * CR_Medium,
		 * CR_Far,
		 * CR_VeryFar,
		 * CR_NumRanges
		 * };
		 */
		void SET_PED_COMBAT_RANGE(Ped ped, int combatRange);
		/**
		 * See SET_PED_COMBAT_RANGE
		 */
		int GET_PED_COMBAT_RANGE(Ped ped);
		/**
		 * enum eCombatAttributes // 0x0E8E7201
		 * {
		 * BF_CanUseCover = 0,
		 * BF_CanUseVehicles = 1,
		 * BF_CanDoDrivebys = 2,
		 * BF_CanLeaveVehicle = 3,
		 * BF_CanUseDynamicStrafeDecisions = 4,
		 * BF_AlwaysFight = 5,
		 * BF_0x66BB9FCC = 6,
		 * BF_0x6837DA41 = 7,
		 * BF_0xB4A13A5A = 8,
		 * BF_0xEE326AAD = 9,
		 * BF_0x7DF2CCFA = 10,
		 * BF_0x0036D422 = 11,
		 * BF_BlindFireWhenInCover = 12,
		 * BF_Aggressive = 13,
		 * BF_CanInvestigate = 14,
		 * BF_HasRadio = 15,
		 * BF_0x6BDE28D1 = 16,
		 * BF_AlwaysFlee = 17,
		 * BF_0x7852797D = 18,
		 * BF_0x33497B95 = 19,
		 * BF_CanTauntInVehicle = 20,
		 * BF_CanChaseTargetOnFoot = 21,
		 * BF_WillDragInjuredPedsToSafety = 22,
		 * BF_0xCD7168B8 = 23,
		 * BF_UseProximityFiringRate = 24,
		 * BF_0x48F914F8 = 25,
		 * BF_0x2EA543D0 = 26,
		 * BF_PerfectAccuracy = 27,
		 * BF_CanUseFrustratedAdvance = 28,
		 * BF_0x3D131AC1 = 29,
		 * BF_0x3AD95F27 = 30,
		 * BF_MaintainMinDistanceToTarget = 31,
		 * BF_0xEAD68AD2 = 32,
		 * BF_0xA206C2E0 = 33,
		 * BF_CanUsePeekingVariations = 34,
		 * BF_0xA5715184 = 35,
		 * BF_0xD5265533 = 36,
		 * BF_0x2B84C2BF = 37,
		 * BF_DisableBulletReactions = 38,
		 * BF_CanBust = 39,
		 * BF_0xAA525726 = 40,
		 * BF_CanCommandeerVehicles = 41,
		 * BF_CanFlank = 42,
		 * BF_SwitchToAdvanceIfCantFindCover = 43,
		 * BF_SwitchToDefensiveIfInCover = 44,
		 * BF_0xEB4786A0 = 45,
		 * BF_CanFightArmedPedsWhenNotArmed = 46,
		 * BF_0xA08E9402 = 47,
		 * BF_0x952EAD7D = 48,
		 * BF_UseEnemyAccuracyScaling = 49,
		 * BF_CanCharge = 50,
		 * BF_0xDA8C2BD3 = 51,
		 * BF_0x6562F017 = 52,
		 * BF_0xA2C3D53B = 53,
		 * BF_AlwaysEquipBestWeapon = 54,
		 * BF_CanSeeUnderwaterPeds = 55,
		 * BF_0xF619486B = 56,
		 * BF_0x61EB63A3 = 57,
		 * BF_DisableFleeFromCombat = 58,
		 * BF_0x8976D12B = 59,
		 * BF_CanThrowSmokeGrenade = 60,
		 * BF_NonMissionPedsFleeFromThisPedUnlessArmed = 61,
		 * BF_0x5452A10C = 62,
		 * BF_FleesFromInvincibleOpponents = 63,
		 * BF_DisableBlockFromPursueDuringVehicleChase = 64,
		 * BF_DisableSpinOutDuringVehicleChase = 65,
		 * BF_DisableCruiseInFrontDuringBlockDuringVehicleChase = 66,
		 * BF_0x0B404731 = 67,
		 * BF_DisableReactToBuddyShot = 68,
		 * BF_0x7FFD6AEB = 69,
		 * BF_0x51F4AEF8 = 70,
		 * BF_PermitChargeBeyondDefensiveArea = 71,
		 * BF_0x63E0A8E2 = 72,
		 * BF_0xDF974436 = 73,
		 * BF_0x556C080B = 74,
		 * BF_0xA4D50035 = 75,
		 * BF_SetDisableShoutTargetPositionOnCombatStart = 76,
		 * BF_DisableRespondedToThreatBroadcast = 77,
		 * BF_0xCBB01765 = 78,
		 * BF_0x4F862ED4 = 79,
		 * BF_0xEF9C7C40 = 80,
		 * BF_0xE51B494F = 81,
		 * BF_0x054D0199 = 82,
		 * BF_0xD36BCE94 = 83,
		 * BF_0xFB11F690 = 84,
		 * BF_0xD208A9AD = 85,
		 * BF_AllowDogFighting = 86,
		 * BF_0x07A6E531 = 87,
		 * BF_0x34F9317B = 88,
		 * BF_0x4240F5A9 = 89,
		 * BF_0xEE129DBD = 90,
		 * BF_0x053AEAD9 = 91
		 * };
		 */
		void SET_PED_COMBAT_ATTRIBUTES(Ped ped, int attributeId, BOOL enabled);
		/**
		 * enum eTargetLossResponseType
		 * {
		 * TLR_ExitTask,
		 * TLR_NeverLoseTarget,
		 * TLR_SearchForTarget
		 * };
		 */
		void SET_PED_TARGET_LOSS_RESPONSE(Ped ped, int responseType);
		BOOL IS_PED_PERFORMING_MELEE_ACTION(Ped ped);
		BOOL IS_PED_PERFORMING_STEALTH_KILL(Ped ped);
		BOOL IS_PED_PERFORMING_A_COUNTER_ATTACK(Ped ped);
		BOOL IS_PED_BEING_STEALTH_KILLED(Ped ped);
		Ped GET_MELEE_TARGET_FOR_PED(Ped ped);
		BOOL WAS_PED_KILLED_BY_STEALTH(Ped ped);
		BOOL WAS_PED_KILLED_BY_TAKEDOWN(Ped ped);
		BOOL WAS_PED_KNOCKED_OUT(Ped ped);
		/**
		 * bit 1 (0x2) = use vehicle
		 * bit 15 (0x8000) = force cower
		 */
		void SET_PED_FLEE_ATTRIBUTES(Ped ped, int attributeFlags, BOOL enable);
		/**
		 * p1: Only "CODE_HUMAN_STAND_COWER" found in the b617d scripts.
		 */
		void SET_PED_COWER_HASH(Ped ped, const char* p1);
		void SET_PED_STEERS_AROUND_DEAD_BODIES(Ped ped, BOOL toggle);
		void SET_PED_STEERS_AROUND_PEDS(Ped ped, BOOL toggle);
		void SET_PED_STEERS_AROUND_OBJECTS(Ped ped, BOOL toggle);
		void SET_PED_STEERS_AROUND_VEHICLES(Ped ped, BOOL toggle);
		void SET_PED_IS_AVOIDED_BY_OTHERS(Any p0, BOOL p1);
		void SET_PED_INCREASED_AVOIDANCE_RADIUS(Ped ped);
		void SET_PED_BLOCKS_PATHING_WHEN_DEAD(Ped ped, BOOL toggle);
		void SET_PED_NO_TIME_DELAY_BEFORE_SHOT(Any p0);
		BOOL IS_ANY_PED_NEAR_POINT(float x, float y, float z, float radius);
		void FORCE_PED_AI_AND_ANIMATION_UPDATE(Ped ped, BOOL p1, BOOL p2);
		BOOL IS_PED_HEADING_TOWARDS_POSITION(Ped ped, float x, float y, float z, float p4);
		void REQUEST_PED_VISIBILITY_TRACKING(Ped ped);
		void REQUEST_PED_VEHICLE_VISIBILITY_TRACKING(Ped ped, BOOL p1);
		void REQUEST_PED_RESTRICTED_VEHICLE_VISIBILITY_TRACKING(Ped ped, BOOL p1);
		void REQUEST_PED_USE_SMALL_BBOX_VISIBILITY_TRACKING(Ped ped, BOOL p1);
		/**
		 * returns whether or not a ped is visible within your FOV, not this check auto's to false after a certain distance.
		 * 
		 * 
		 * Target needs to be tracked.. won't work otherwise.
		 */
		BOOL IS_TRACKED_PED_VISIBLE(Ped ped);
		int GET_TRACKED_PED_PIXELCOUNT(Ped ped);
		BOOL IS_PED_TRACKED(Ped ped);
		BOOL HAS_PED_RECEIVED_EVENT(Ped ped, int eventId);
		BOOL CAN_PED_SEE_HATED_PED(Ped ped1, Ped ped2);
		BOOL CAN_PED_SHUFFLE_TO_OR_FROM_TURRET_SEAT(Ped ped, int* p1);
		BOOL CAN_PED_SHUFFLE_TO_OR_FROM_EXTRA_SEAT(Ped ped, int* p1);
		/**
		 * no bone= -1
		 * 
		 * boneIds:
		 * SKEL_ROOT = 0x0,
		 * SKEL_Pelvis = 0x2e28,
		 * SKEL_L_Thigh = 0xe39f,
		 * SKEL_L_Calf = 0xf9bb,
		 * SKEL_L_Foot = 0x3779,
		 * SKEL_L_Toe0 = 0x83c,
		 * IK_L_Foot = 0xfedd,
		 * PH_L_Foot = 0xe175,
		 * MH_L_Knee = 0xb3fe,
		 * SKEL_R_Thigh = 0xca72,
		 * SKEL_R_Calf = 0x9000,
		 * SKEL_R_Foot = 0xcc4d,
		 * SKEL_R_Toe0 = 0x512d,
		 * IK_R_Foot = 0x8aae,
		 * PH_R_Foot = 0x60e6,
		 * MH_R_Knee = 0x3fcf,
		 * RB_L_ThighRoll = 0x5c57,
		 * RB_R_ThighRoll = 0x192a,
		 * SKEL_Spine_Root = 0xe0fd,
		 * SKEL_Spine0 = 0x5c01,
		 * SKEL_Spine1 = 0x60f0,
		 * SKEL_Spine2 = 0x60f1,
		 * SKEL_Spine3 = 0x60f2,
		 * SKEL_L_Clavicle = 0xfcd9,
		 * SKEL_L_UpperArm = 0xb1c5,
		 * SKEL_L_Forearm = 0xeeeb,
		 * SKEL_L_Hand = 0x49d9,
		 * SKEL_L_Finger00 = 0x67f2,
		 * SKEL_L_Finger01 = 0xff9,
		 * SKEL_L_Finger02 = 0xffa,
		 * SKEL_L_Finger10 = 0x67f3,
		 * SKEL_L_Finger11 = 0x1049,
		 * SKEL_L_Finger12 = 0x104a,
		 * SKEL_L_Finger20 = 0x67f4,
		 * SKEL_L_Finger21 = 0x1059,
		 * SKEL_L_Finger22 = 0x105a,
		 * SKEL_L_Finger30 = 0x67f5,
		 * SKEL_L_Finger31 = 0x1029,
		 * SKEL_L_Finger32 = 0x102a,
		 * SKEL_L_Finger40 = 0x67f6,
		 * SKEL_L_Finger41 = 0x1039,
		 * SKEL_L_Finger42 = 0x103a,
		 * PH_L_Hand = 0xeb95,
		 * IK_L_Hand = 0x8cbd,
		 * RB_L_ForeArmRoll = 0xee4f,
		 * RB_L_ArmRoll = 0x1470,
		 * MH_L_Elbow = 0x58b7,
		 * SKEL_R_Clavicle = 0x29d2,
		 * SKEL_R_UpperArm = 0x9d4d,
		 * SKEL_R_Forearm = 0x6e5c,
		 * SKEL_R_Hand = 0xdead,
		 * SKEL_R_Finger00 = 0xe5f2,
		 * SKEL_R_Finger01 = 0xfa10,
		 * SKEL_R_Finger02 = 0xfa11,
		 * SKEL_R_Finger10 = 0xe5f3,
		 * SKEL_R_Finger11 = 0xfa60,
		 * SKEL_R_Finger12 = 0xfa61,
		 * SKEL_R_Finger20 = 0xe5f4,
		 * SKEL_R_Finger21 = 0xfa70,
		 * SKEL_R_Finger22 = 0xfa71,
		 * SKEL_R_Finger30 = 0xe5f5,
		 * SKEL_R_Finger31 = 0xfa40,
		 * SKEL_R_Finger32 = 0xfa41,
		 * SKEL_R_Finger40 = 0xe5f6,
		 * SKEL_R_Finger41 = 0xfa50,
		 * SKEL_R_Finger42 = 0xfa51,
		 * PH_R_Hand = 0x6f06,
		 * IK_R_Hand = 0x188e,
		 * RB_R_ForeArmRoll = 0xab22,
		 * RB_R_ArmRoll = 0x90ff,
		 * MH_R_Elbow = 0xbb0,
		 * SKEL_Neck_1 = 0x9995,
		 * SKEL_Head = 0x796e,
		 * IK_Head = 0x322c,
		 * FACIAL_facialRoot = 0xfe2c,
		 * FB_L_Brow_Out_000 = 0xe3db,
		 * FB_L_Lid_Upper_000 = 0xb2b6,
		 * FB_L_Eye_000 = 0x62ac,
		 * FB_L_CheekBone_000 = 0x542e,
		 * FB_L_Lip_Corner_000 = 0x74ac,
		 * FB_R_Lid_Upper_000 = 0xaa10,
		 * FB_R_Eye_000 = 0x6b52,
		 * FB_R_CheekBone_000 = 0x4b88,
		 * FB_R_Brow_Out_000 = 0x54c,
		 * FB_R_Lip_Corner_000 = 0x2ba6,
		 * FB_Brow_Centre_000 = 0x9149,
		 * FB_UpperLipRoot_000 = 0x4ed2,
		 * FB_UpperLip_000 = 0xf18f,
		 * FB_L_Lip_Top_000 = 0x4f37,
		 * FB_R_Lip_Top_000 = 0x4537,
		 * FB_Jaw_000 = 0xb4a0,
		 * FB_LowerLipRoot_000 = 0x4324,
		 * FB_LowerLip_000 = 0x508f,
		 * FB_L_Lip_Bot_000 = 0xb93b,
		 * FB_R_Lip_Bot_000 = 0xc33b,
		 * FB_Tongue_000 = 0xb987,
		 * RB_Neck_1 = 0x8b93,
		 * IK_Root = 0xdd1c
		 */
		int GET_PED_BONE_INDEX(Ped ped, int boneId);
		int GET_PED_RAGDOLL_BONE_INDEX(Ped ped, int bone);
		/**
		 * Values look to be between 0.0 and 1.0
		 * From decompiled scripts: 0.0, 0.6, 0.65, 0.8, 1.0
		 * 
		 * You are correct, just looked in IDA it breaks from the function if it's less than 0.0f or greater than 1.0f.
		 */
		void SET_PED_ENVEFF_SCALE(Ped ped, float value);
		float GET_PED_ENVEFF_SCALE(Ped ped);
		void SET_ENABLE_PED_ENVEFF_SCALE(Ped ped, BOOL toggle);
		/**
		 * In agency_heist3b.c4, its like this 90% of the time:
		 * 
		 * PED::SET_PED_ENVEFF_CPV_ADD(ped, 0.099);
		 * PED::SET_PED_ENVEFF_SCALE(ped, 1.0);
		 * PED::SET_PED_ENVEFF_CPV_ADD(ped, 87, 81, 68);
		 * PED::SET_ENABLE_PED_ENVEFF_SCALE(ped, 1);
		 * 
		 * and its like this 10% of the time:
		 * 
		 * PED::SET_PED_ENVEFF_CPV_ADD(ped, 0.2);
		 * PED::SET_PED_ENVEFF_SCALE(ped, 0.65);
		 * PED::SET_PED_ENVEFF_COLOR_MODULATOR(ped, 74, 69, 60);
		 * PED::SET_ENABLE_PED_ENVEFF_SCALE(ped, 1);
		 */
		void SET_PED_ENVEFF_CPV_ADD(Ped ped, float p1);
		/**
		 * Something related to the environmental effects natives.
		 * In the "agency_heist3b" script, p1 - p3 are always under 100 - usually they are {87, 81, 68}. If SET_PED_ENVEFF_SCALE is set to 0.65 (instead of the usual 1.0), they use {74, 69, 60}
		 */
		void SET_PED_ENVEFF_COLOR_MODULATOR(Ped ped, int p1, int p2, int p3);
		/**
		 * intensity: 0.0f - 1.0f
		 * 
		 * This native sets the emissive intensity for the given ped. It is used for different 'glow' levels on illuminated clothing.
		 */
		void SET_PED_EMISSIVE_SCALE(Ped ped, float intensity);
		/**
		 * Use 0x4E90D746056E273D to set the illuminated clothing glow intensity for a specific ped.
		 * Returns a float between 0.0 and 1.0 representing the current illuminated clothing glow intensity.
		 */
		float GET_PED_EMISSIVE_SCALE(Ped ped);
		BOOL IS_PED_SHADER_READY(Ped ped);
		void SET_PED_ENABLE_CREW_EMBLEM(Ped ped, BOOL toggle);
		/**
		 * This native does absolutely nothing, just a nullsub
		 */
		void REQUEST_RAGDOLL_BOUNDS_UPDATE(Any p0, Any p1);
		/**
		 * Enable/disable ped shadow (ambient occlusion). https://gfycat.com/thankfulesteemedgecko
		 */
		void SET_PED_AO_BLOB_RENDERING(Ped ped, BOOL toggle);
		BOOL IS_PED_SHELTERED(Ped ped);
		/**
		 * p6 always 2 (but it doesnt seem to matter...)
		 * 
		 * roll and pitch 0
		 * yaw to Ped.rotation
		 */
		int CREATE_SYNCHRONIZED_SCENE(float x, float y, float z, float roll, float pitch, float yaw, int p6);
		int CREATE_SYNCHRONIZED_SCENE_AT_MAP_OBJECT(float x, float y, float z, float radius, Hash object);
		/**
		 * Returns true if a synchronized scene is running
		 */
		BOOL IS_SYNCHRONIZED_SCENE_RUNNING(int sceneId);
		void SET_SYNCHRONIZED_SCENE_ORIGIN(int sceneID, float x, float y, float z, float roll, float pitch, float yaw, BOOL p7);
		void SET_SYNCHRONIZED_SCENE_PHASE(int sceneID, float phase);
		float GET_SYNCHRONIZED_SCENE_PHASE(int sceneID);
		void SET_SYNCHRONIZED_SCENE_RATE(int sceneID, float rate);
		float GET_SYNCHRONIZED_SCENE_RATE(int sceneID);
		void SET_SYNCHRONIZED_SCENE_LOOPED(int sceneID, BOOL toggle);
		BOOL IS_SYNCHRONIZED_SCENE_LOOPED(int sceneID);
		void SET_SYNCHRONIZED_SCENE_HOLD_LAST_FRAME(int sceneID, BOOL toggle);
		BOOL IS_SYNCHRONIZED_SCENE_HOLD_LAST_FRAME(int sceneID);
		void ATTACH_SYNCHRONIZED_SCENE_TO_ENTITY(int sceneID, Entity entity, int boneIndex);
		void DETACH_SYNCHRONIZED_SCENE(int sceneID);
		void TAKE_OWNERSHIP_OF_SYNCHRONIZED_SCENE(int scene);
		/**
		 * Regarding p2, p3 and p4: Most common is 0, 0, 0); followed by 0, 1, 0); and 1, 1, 0); in R* scripts. p4 is very rarely something other than 0.
		 * 
		 * enum eMotionState // 0x92A659FE
		 * {
		 * MotionState_None = 0xEE717723,
		 * MotionState_Idle = 0x9072A713,
		 * MotionState_Walk = 0xD827C3DB,
		 * MotionState_Run = 0xFFF7E7A4,
		 * MotionState_Sprint = 0xBD8817DB,
		 * MotionState_Crouch_Idle = 0x43FB099E,
		 * MotionState_Crouch_Walk = 0x08C31A98,
		 * MotionState_Crouch_Run = 0x3593CF09,
		 * MotionState_DoNothing = 0x0EC17E58,
		 * MotionState_AnimatedVelocity = 0x551AAC43,
		 * MotionState_InVehicle = 0x94D9D58D,
		 * MotionState_Aiming = 0x3F67C6AF,
		 * MotionState_Diving_Idle = 0x4848CDED,
		 * MotionState_Diving_Swim = 0x916E828C,
		 * MotionState_Swimming_TreadWater = 0xD1BF11C7,
		 * MotionState_Dead = 0x0DBB071C,
		 * MotionState_Stealth_Idle = 0x422D7A25,
		 * MotionState_Stealth_Walk = 0x042AB6A2,
		 * MotionState_Stealth_Run = 0xFB0B79E1,
		 * MotionState_Parachuting = 0xBAC0F10B,
		 * MotionState_ActionMode_Idle = 0xDA40A0DC,
		 * MotionState_ActionMode_Walk = 0xD2905EA7,
		 * MotionState_ActionMode_Run = 0x31BADE14,
		 * MotionState_Jetpack = 0x535E6A5E
		 * };
		 */
		BOOL FORCE_PED_MOTION_STATE(Ped ped, Hash motionStateHash, BOOL p2, int p3, BOOL p4);
		BOOL GET_PED_CURRENT_MOVE_BLEND_RATIO(Ped ped, float* speedX, float* speedY);
		void SET_PED_MAX_MOVE_BLEND_RATIO(Ped ped, float value);
		void SET_PED_MIN_MOVE_BLEND_RATIO(Ped ped, float value);
		/**
		 * Min: 0.00
		 * Max: 10.00
		 * 
		 * Can be used in combo with fast run cheat.
		 * 
		 * When value is set to 10.00:
		 * Sprinting without fast run cheat: 66 m/s
		 * Sprinting with fast run cheat: 77 m/s
		 * 
		 * Needs to be looped!
		 * 
		 * Note: According to IDA for the Xbox360 xex, when they check bgt they seem to have the min to 0.0f, but the max set to 1.15f not 10.0f.
		 */
		void SET_PED_MOVE_RATE_OVERRIDE(Ped ped, float value);
		void SET_PED_MOVE_RATE_IN_WATER_OVERRIDE(Ped ped, float p1);
		/**
		 * Checks if the specified sexiness flag is set
		 * 
		 * enum eSexinessFlags
		 * {
		 * SF_JEER_AT_HOT_PED = 0,
		 * SF_HURRIEDFEMALES_SEXY = 1,
		 * SF_HOT_PERSON = 2,
		 * };
		 */
		BOOL PED_HAS_SEXINESS_FLAG_SET(Ped ped, int sexinessFlag);
		/**
		 * Returns size of array, passed into the second variable.
		 * 
		 * See below for usage information.
		 * 
		 * This function actually requires a struct, where the first value is the maximum number of elements to return.  Here is a sample of how I was able to get it to work correctly, without yet knowing the struct format.
		 * 
		 * //Setup the array
		 * const int numElements = 10;
		 * const int arrSize = numElements * 2 + 2;
		 * Any veh[arrSize];
		 * //0 index is the size of the array
		 * veh[0] = numElements;
		 * 
		 * int count = PED::GET_PED_NEARBY_VEHICLES(PLAYER::PLAYER_PED_ID(), veh);
		 * 
		 * if (veh != NULL)
		 * {
		 * //Simple loop to go through results
		 * for (int i = 0; i < count; i++)
		 * {
		 * int offsettedID = i * 2 + 2;
		 * //Make sure it exists
		 * if (veh[offsettedID] != NULL && ENTITY::DOES_ENTITY_EXIST(veh[offsettedID]))
		 * {
		 * //Do something
		 * }
		 * }
		 * }
		 */
		int GET_PED_NEARBY_VEHICLES(Ped ped, Any* sizeAndVehs);
		/**
		 * sizeAndPeds - is a pointer to an array. The array is filled with peds found nearby the ped supplied to the first argument.
		 * ignore - ped type to ignore
		 * 
		 * Return value is the number of peds found and added to the array passed.
		 * 
		 * -----------------------------------
		 * 
		 * To make this work in most menu bases at least in C++ do it like so,
		 * 
		 * Formatted Example: https://pastebin.com/D8an9wwp
		 * 
		 * -----------------------------------
		 * 
		 * Example: gtaforums.com/topic/789788-function-args-to-pedget-ped-nearby-peds/?p=1067386687
		 */
		int GET_PED_NEARBY_PEDS(Ped ped, Any* sizeAndPeds, int ignore);
		BOOL HAVE_ALL_STREAMING_REQUESTS_COMPLETED(Ped ped);
		BOOL IS_PED_USING_ACTION_MODE(Ped ped);
		/**
		 * p2 is usually -1 in the scripts. action is either 0 or "DEFAULT_ACTION".
		 */
		void SET_PED_USING_ACTION_MODE(Ped ped, BOOL p1, int p2, const char* action);
		/**
		 * name: "MP_FEMALE_ACTION" found multiple times in the b617d scripts.
		 */
		void SET_MOVEMENT_MODE_OVERRIDE(Ped ped, const char* name);
		/**
		 * Overrides the ped's collision capsule radius for the current tick.
		 * Must be called every tick to be effective.
		 * 
		 * Setting this to 0.001 will allow warping through some objects.
		 */
		void SET_PED_CAPSULE(Ped ped, float value);
		/**
		 * gtaforums.com/topic/885580-ped-headshotmugshot-txd/
		 */
		int REGISTER_PEDHEADSHOT(Ped ped);
		int REGISTER_PEDHEADSHOT_HIRES(Ped ped);
		/**
		 * Similar to REGISTER_PEDHEADSHOT but creates a transparent background instead of black. Example: https://i.imgur.com/iHz8ztn.png
		 */
		int REGISTER_PEDHEADSHOT_TRANSPARENT(Ped ped);
		/**
		 * gtaforums.com/topic/885580-ped-headshotmugshot-txd/
		 */
		void UNREGISTER_PEDHEADSHOT(int id);
		/**
		 * gtaforums.com/topic/885580-ped-headshotmugshot-txd/
		 */
		BOOL IS_PEDHEADSHOT_VALID(int id);
		/**
		 * gtaforums.com/topic/885580-ped-headshotmugshot-txd/
		 */
		BOOL IS_PEDHEADSHOT_READY(int id);
		/**
		 * gtaforums.com/topic/885580-ped-headshotmugshot-txd/
		 */
		const char* GET_PEDHEADSHOT_TXD_STRING(int id);
		BOOL REQUEST_PEDHEADSHOT_IMG_UPLOAD(int id);
		void RELEASE_PEDHEADSHOT_IMG_UPLOAD(int id);
		BOOL IS_PEDHEADSHOT_IMG_UPLOAD_AVAILABLE();
		BOOL HAS_PEDHEADSHOT_IMG_UPLOAD_FAILED();
		BOOL HAS_PEDHEADSHOT_IMG_UPLOAD_SUCCEEDED();
		void SET_PED_HEATSCALE_OVERRIDE(Ped ped, float heatScale);
		void DISABLE_PED_HEATSCALE_OVERRIDE(Ped ped);
		void SPAWNPOINTS_START_SEARCH(float p0, float p1, float p2, float p3, float p4, int interiorFlags, float scale, int duration);
		void SPAWNPOINTS_START_SEARCH_IN_ANGLED_AREA(float x1, float y1, float z1, float x2, float y2, float z2, float width, int interiorFlags, float scale, int duration);
		void SPAWNPOINTS_CANCEL_SEARCH();
		BOOL SPAWNPOINTS_IS_SEARCH_ACTIVE();
		BOOL SPAWNPOINTS_IS_SEARCH_COMPLETE();
		BOOL SPAWNPOINTS_IS_SEARCH_FAILED();
		int SPAWNPOINTS_GET_NUM_SEARCH_RESULTS();
		void SPAWNPOINTS_GET_SEARCH_RESULT(int randomInt, float* x, float* y, float* z);
		void SPAWNPOINTS_GET_SEARCH_RESULT_FLAGS(int p0, int* p1);
		void SET_IK_TARGET(Ped ped, int ikIndex, Entity entityLookAt, int boneLookAt, float offsetX, float offsetY, float offsetZ, Any p7, int blendInDuration, int blendOutDuration);
		void FORCE_INSTANT_LEG_IK_SETUP(Ped ped);
		void REQUEST_ACTION_MODE_ASSET(const char* asset);
		BOOL HAS_ACTION_MODE_ASSET_LOADED(const char* asset);
		void REMOVE_ACTION_MODE_ASSET(const char* asset);
		void REQUEST_STEALTH_MODE_ASSET(const char* asset);
		BOOL HAS_STEALTH_MODE_ASSET_LOADED(const char* asset);
		void REMOVE_STEALTH_MODE_ASSET(const char* asset);
		void SET_PED_LOD_MULTIPLIER(Ped ped, float multiplier);
		void SET_PED_CAN_LOSE_PROPS_ON_DAMAGE(Ped ped, BOOL toggle, int p2);
		void SET_FORCE_FOOTSTEP_UPDATE(Ped ped, BOOL toggle);
		void SET_FORCE_STEP_TYPE(Ped ped, BOOL p1, int type, int p3);
		BOOL IS_ANY_HOSTILE_PED_NEAR_POINT(Ped ped, float x, float y, float z, float radius);
		/**
		 * Toggles config flag CPED_CONFIG_FLAG_CanPlayInCarIdles.
		 */
		void SET_PED_CAN_PLAY_IN_CAR_IDLES(Ped ped, BOOL toggle);
		BOOL IS_TARGET_PED_IN_PERCEPTION_AREA(Ped ped, Ped targetPed, float p2, float p3, float p4, float p5);
		/**
		 * Min and max are usually 100.0 and 200.0
		 */
		void SET_POP_CONTROL_SPHERE_THIS_FRAME(float x, float y, float z, float min, float max);
		void FORCE_ZERO_MASS_IN_COLLISIONS(Ped ped);
		void SET_DISABLE_HIGH_FALL_DEATH(Ped ped, BOOL toggle);
		void SET_PED_PHONE_PALETTE_IDX(Any p0, Any p1);
		void SET_PED_STEER_BIAS(Ped ped, float value);
		BOOL IS_PED_SWITCHING_WEAPON(Ped Ped);
		void SET_PED_TREATED_AS_FRIENDLY(Any p0, Any p1, Any p2);
		void SET_DISABLE_PED_MAP_COLLISION(Ped ped);
		void ENABLE_MP_LIGHT(Ped ped, BOOL toggle);
		BOOL GET_MP_LIGHT_ENABLED(Ped ped);
		void CLEAR_COVER_POINT_FOR_PED(Ped ped);
		void SET_ALLOW_STUNT_JUMP_CAMERA(Ped ped, BOOL toggle);

	} // namespace PED

	namespace PHYSICS {

		/**
		 * Creates a rope at the specific position, that extends in the specified direction when not attached to any entities.
		 * __
		 * 
		 * Add_Rope(pos.x,pos.y,pos.z,0.0,0.0,0.0,20.0,4,20.0,1.0,0.0,false,false,false,5.0,false,NULL)
		 * 
		 * When attached, Position<vector> does not matter
		 * When attached, Angle<vector> does not matter
		 * 
		 * Rope Type:
		 * 4 and bellow is a thick rope
		 * 5 and up are small metal wires
		 * 0 crashes the game
		 * 
		 * Max_length - Rope is forced to this length, generally best to keep this the same as your rope length.
		 * 
		 * windingSpeed - Speed the Rope is being winded, using native START_ROPE_WINDING. Set positive for winding and negative for unwinding.
		 * 
		 * Rigid - If max length is zero, and this is set to false the rope will become rigid (it will force a specific distance, what ever length is, between the objects).
		 * 
		 * breakable - Whether or not shooting the rope will break it.
		 * 
		 * unkPtr - unknown ptr, always 0 in orig scripts
		 * __
		 * 
		 * Lengths can be calculated like so:
		 * 
		 * float distance = abs(x1 - x2) + abs(y1 - y2) + abs(z1 - z2); // Rope length
		 * 
		 * 
		 * NOTES:
		 * 
		 * Rope does NOT interact with anything you attach it to, in some cases it make interact with the world AFTER it breaks (seems to occur if you set the type to -1).
		 * 
		 * Rope will sometimes contract and fall to the ground like you'd expect it to, but since it doesn't interact with the world the effect is just jaring.
		 */
		int ADD_ROPE(float x, float y, float z, float rotX, float rotY, float rotZ, float length, int ropeType, float maxLength, float minLength, float windingSpeed, BOOL p11, BOOL p12, BOOL rigid, float p14, BOOL breakWhenShot, Any* unkPtr);
		void DELETE_ROPE(int* ropeId);
		void DELETE_CHILD_ROPE(int ropeId);
		BOOL DOES_ROPE_EXIST(int* ropeId);
		void ROPE_DRAW_ENABLED(int* ropeId, BOOL p1);
		void ROPE_DRAW_SHADOW_ENABLED(int* ropeId, BOOL toggle);
		/**
		 * Rope presets can be found in the gamefiles. One example is "ropeFamily3", it is NOT a hash but rather a string.
		 */
		void LOAD_ROPE_DATA(int ropeId, const char* rope_preset);
		void PIN_ROPE_VERTEX(int ropeId, int vertex, float x, float y, float z);
		void UNPIN_ROPE_VERTEX(int ropeId, int vertex);
		int GET_ROPE_VERTEX_COUNT(int ropeId);
		/**
		 * Attaches entity 1 to entity 2.
		 */
		void ATTACH_ENTITIES_TO_ROPE(int ropeId, Entity ent1, Entity ent2, float ent1_x, float ent1_y, float ent1_z, float ent2_x, float ent2_y, float ent2_z, float length, BOOL p10, BOOL p11, Any* p12, Any* p13);
		/**
		 * The position supplied can be anywhere, and the entity should anchor relative to that point from it's origin.
		 */
		void ATTACH_ROPE_TO_ENTITY(int ropeId, Entity entity, float x, float y, float z, BOOL p5);
		void DETACH_ROPE_FROM_ENTITY(int ropeId, Entity entity);
		void ROPE_SET_UPDATE_PINVERTS(int ropeId);
		void ROPE_SET_UPDATE_ORDER(int ropeId, Any p1);
		void ROPE_SET_SMOOTH_REELIN(int ropeId, BOOL p1);
		BOOL IS_ROPE_ATTACHED_AT_BOTH_ENDS(int* ropeId);
		Vector3 GET_ROPE_LAST_VERTEX_COORD(int ropeId);
		Vector3 GET_ROPE_VERTEX_COORD(int ropeId, int vertex);
		void START_ROPE_WINDING(int ropeId);
		void STOP_ROPE_WINDING(int ropeId);
		void START_ROPE_UNWINDING_FRONT(int ropeId);
		void STOP_ROPE_UNWINDING_FRONT(int ropeId);
		void ROPE_CONVERT_TO_SIMPLE(int ropeId);
		/**
		 * Loads rope textures for all ropes in the current scene.
		 */
		void ROPE_LOAD_TEXTURES();
		BOOL ROPE_ARE_TEXTURES_LOADED();
		/**
		 * Unloads rope textures for all ropes in the current scene.
		 */
		void ROPE_UNLOAD_TEXTURES();
		BOOL DOES_SCRIPT_OWN_ROPE(int ropeId);
		void ROPE_ATTACH_VIRTUAL_BOUND_GEOM(int ropeId, int p1, float p2, float p3, float p4, float p5, float p6, float p7, float p8, float p9, float p10, float p11, float p12, float p13);
		void ROPE_CHANGE_SCRIPT_OWNER(Any p0, BOOL p1, BOOL p2);
		void ROPE_SET_REFFRAMEVELOCITY_COLLIDERORDER(int ropeId, int p1);
		float ROPE_GET_DISTANCE_BETWEEN_ENDS(int ropeId);
		/**
		 * Forces a rope to a certain length.
		 */
		void ROPE_FORCE_LENGTH(int ropeId, float length);
		/**
		 * Reset a rope to a certain length.
		 */
		void ROPE_RESET_LENGTH(int ropeId, float length);
		void APPLY_IMPULSE_TO_CLOTH(float posX, float posY, float posZ, float vecX, float vecY, float vecZ, float impulse);
		void SET_DAMPING(Entity entity, int vertex, float value);
		void GET_DAMPING(Entity entity, int type); // Missing in crossmap
		void ACTIVATE_PHYSICS(Entity entity);
		void SET_CGOFFSET(Entity entity, float x, float y, float z);
		Vector3 GET_CGOFFSET(Entity entity);
		void SET_CG_AT_BOUNDCENTER(Entity entity);
		void BREAK_ENTITY_GLASS(Entity entity, float p1, float p2, float p3, float p4, float p5, float p6, float p7, float p8, Any p9, BOOL p10);
		BOOL GET_IS_ENTITY_A_FRAG(Object object);
		void SET_DISABLE_BREAKING(Object object, BOOL toggle);
		void RESET_DISABLE_BREAKING(Object object);
		void SET_DISABLE_FRAG_DAMAGE(Object object, BOOL toggle);
		void SET_USE_KINEMATIC_PHYSICS(Entity entity, BOOL toggle);
		void SET_IN_STUNT_MODE(BOOL p0);
		/**
		 * Related to the lower-end of a vehicles fTractionCurve, e.g., from standing starts and acceleration from low/zero speeds.
		 */
		void SET_IN_ARENA_MODE(BOOL toggle);

	} // namespace PHYSICS

	namespace PLAYER {

		/**
		 * Gets the ped for a specified player index.
		 */
		Ped GET_PLAYER_PED(Player player);
		/**
		 * Does the same like PLAYER::GET_PLAYER_PED
		 */
		Ped GET_PLAYER_PED_SCRIPT_INDEX(Player player);
		/**
		 * Set the model for a specific Player. Be aware that this will destroy the current Ped for the Player and create a new one, any reference to the old ped should be reset
		 * Make sure to request the model first and wait until it has loaded.
		 */
		void SET_PLAYER_MODEL(Player player, Hash model);
		void CHANGE_PLAYER_PED(Player player, Ped ped, BOOL p2, BOOL resetDamage);
		void GET_PLAYER_RGB_COLOUR(Player player, int* r, int* g, int* b);
		/**
		 * Gets the number of players in the current session.
		 * If not multiplayer, always returns 1.
		 */
		int GET_NUMBER_OF_PLAYERS();
		/**
		 * Gets the player's team.
		 * Does nothing in singleplayer.
		 */
		int GET_PLAYER_TEAM(Player player);
		/**
		 * Set player team on deathmatch and last team standing..
		 */
		void SET_PLAYER_TEAM(Player player, int team);
		int GET_NUMBER_OF_PLAYERS_IN_TEAM(int team);
		const char* GET_PLAYER_NAME(Player player);
		/**
		 * Remnant from GTA IV. Does nothing in GTA V.
		 */
		float GET_WANTED_LEVEL_RADIUS(Player player);
		Vector3 GET_PLAYER_WANTED_CENTRE_POSITION(Player player);
		/**
		 * # Predominant call signatures
		 * PLAYER::SET_PLAYER_WANTED_CENTRE_POSITION(PLAYER::PLAYER_ID(), ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), 1));
		 * 
		 * # Parameter value ranges
		 * P0: PLAYER::PLAYER_ID()
		 * P1: ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), 1)
		 * P2: Not set by any call
		 */
		void SET_PLAYER_WANTED_CENTRE_POSITION(Player player, Vector3* position, BOOL p2, BOOL p3);
		/**
		 * Drft
		 */
		int GET_WANTED_LEVEL_THRESHOLD(int wantedLevel);
		/**
		 * Call SET_PLAYER_WANTED_LEVEL_NOW for immediate effect
		 * 
		 * wantedLevel is an integer value representing 0 to 5 stars even though the game supports the 6th wanted level but no police will appear since no definitions are present for it in the game files
		 * 
		 * disableNoMission-  Disables When Off Mission- appears to always be false
		 * 
		 */
		void SET_PLAYER_WANTED_LEVEL(Player player, int wantedLevel, BOOL disableNoMission);
		/**
		 * p2 is always false in R* scripts
		 */
		void SET_PLAYER_WANTED_LEVEL_NO_DROP(Player player, int wantedLevel, BOOL p2);
		/**
		 * Forces any pending wanted level to be applied to the specified player immediately.
		 * 
		 * Call SET_PLAYER_WANTED_LEVEL with the desired wanted level, followed by SET_PLAYER_WANTED_LEVEL_NOW.
		 * 
		 * Second parameter is unknown (always false).
		 */
		void SET_PLAYER_WANTED_LEVEL_NOW(Player player, BOOL p1);
		BOOL ARE_PLAYER_FLASHING_STARS_ABOUT_TO_DROP(Player player);
		BOOL ARE_PLAYER_STARS_GREYED_OUT(Player player);
		BOOL IS_WANTED_AND_HAS_BEEN_SEEN_BY_COPS(Player player);
		void SET_DISPATCH_COPS_FOR_PLAYER(Player player, BOOL toggle);
		BOOL IS_PLAYER_WANTED_LEVEL_GREATER(Player player, int wantedLevel);
		/**
		 * This executes at the same as speed as PLAYER::SET_PLAYER_WANTED_LEVEL(player, 0, false);
		 * 
		 * PLAYER::GET_PLAYER_WANTED_LEVEL(player); executes in less than half the time. Which means that it's worth first checking if the wanted level needs to be cleared before clearing. However, this is mostly about good code practice and can important in other situations. The difference in time in this example is negligible.
		 */
		void CLEAR_PLAYER_WANTED_LEVEL(Player player);
		BOOL IS_PLAYER_DEAD(Player player);
		BOOL IS_PLAYER_PRESSING_HORN(Player player);
		/**
		 * Flags:
		 * SPC_AMBIENT_SCRIPT = (1 << 1),
		 * SPC_CLEAR_TASKS = (1 << 2),
		 * SPC_REMOVE_FIRES = (1 << 3),
		 * SPC_REMOVE_EXPLOSIONS = (1 << 4),
		 * SPC_REMOVE_PROJECTILES = (1 << 5),
		 * SPC_DEACTIVATE_GADGETS = (1 << 6),
		 * SPC_REENABLE_CONTROL_ON_DEATH = (1 << 7),
		 * SPC_LEAVE_CAMERA_CONTROL_ON = (1 << 8),
		 * SPC_ALLOW_PLAYER_DAMAGE = (1 << 9),
		 * SPC_DONT_STOP_OTHER_CARS_AROUND_PLAYER = (1 << 10),
		 * SPC_PREVENT_EVERYBODY_BACKOFF = (1 << 11),
		 * SPC_ALLOW_PAD_SHAKE = (1 << 12)
		 * 
		 * See: https://alloc8or.re/gta5/doc/enums/eSetPlayerControlFlag.txt
		 */
		void SET_PLAYER_CONTROL(Player player, BOOL bHasControl, int flags);
		int GET_PLAYER_WANTED_LEVEL(Player player);
		void SET_MAX_WANTED_LEVEL(int maxWantedLevel);
		/**
		 * If toggle is set to false:
		 * The police won't be shown on the (mini)map
		 * 
		 * If toggle is set to true:
		 * The police will be shown on the (mini)map
		 */
		void SET_POLICE_RADAR_BLIPS(BOOL toggle);
		/**
		 * The player will be ignored by the police if toggle is set to true
		 */
		void SET_POLICE_IGNORE_PLAYER(Player player, BOOL toggle);
		/**
		 * Checks whether the specified player has a Ped, the Ped is not dead, is not injured and is not arrested.
		 */
		BOOL IS_PLAYER_PLAYING(Player player);
		void SET_EVERYONE_IGNORE_PLAYER(Player player, BOOL toggle);
		void SET_ALL_RANDOM_PEDS_FLEE(Player player, BOOL toggle);
		void SET_ALL_RANDOM_PEDS_FLEE_THIS_FRAME(Player player);
		void SET_ALL_NEUTRAL_RANDOM_PEDS_FLEE(Player player, BOOL toggle);
		/**
		 * - This is called after SET_ALL_RANDOM_PEDS_FLEE_THIS_FRAME
		 * 
		 */
		void SET_ALL_NEUTRAL_RANDOM_PEDS_FLEE_THIS_FRAME(Player player);
		void SET_LAW_PEDS_CAN_ATTACK_NON_WANTED_PLAYER_THIS_FRAME(Player player);
		void SET_IGNORE_LOW_PRIORITY_SHOCKING_EVENTS(Player player, BOOL toggle);
		void SET_WANTED_LEVEL_MULTIPLIER(float multiplier);
		/**
		 * Max value is 1.0
		 */
		void SET_WANTED_LEVEL_DIFFICULTY(Player player, float difficulty);
		void RESET_WANTED_LEVEL_DIFFICULTY(Player player);
		int GET_WANTED_LEVEL_TIME_TO_ESCAPE();
		void SET_WANTED_LEVEL_HIDDEN_ESCAPE_TIME(Player player, int wantedLevel, int lossTime);
		void RESET_WANTED_LEVEL_HIDDEN_ESCAPE_TIME(Player player);
		void START_FIRING_AMNESTY(int duration);
		/**
		 * PLAYER::REPORT_CRIME(PLAYER::PLAYER_ID(), 37, PLAYER::GET_WANTED_LEVEL_THRESHOLD(1));
		 * 
		 * From am_armybase.ysc.c4:
		 * 
		 * PLAYER::REPORT_CRIME(PLAYER::PLAYER_ID(4), 36, PLAYER::GET_WANTED_LEVEL_THRESHOLD(4));
		 * 
		 * -----
		 * 
		 * This was taken from the GTAV.exe v1.334. The function is called sub_140592CE8. For a full decompilation of the function, see here: https://pastebin.com/09qSMsN7
		 * 
		 * -----
		 * crimeType:
		 * 1: Firearms possession
		 * 2: Person running a red light ("5-0-5")
		 * 3: Reckless driver
		 * 4: Speeding vehicle (a "5-10")
		 * 5: Traffic violation (a "5-0-5")
		 * 6: Motorcycle rider without a helmet
		 * 7: Vehicle theft (a "5-0-3")
		 * 8: Grand Theft Auto
		 * 9: ???
		 * 10: ???
		 * 11: Assault on a civilian (a "2-40")
		 * 12: Assault on an officer
		 * 13: Assault with a deadly weapon (a "2-45")
		 * 14: Officer shot (a "2-45")
		 * 15: Pedestrian struck by a vehicle
		 * 16: Officer struck by a vehicle
		 * 17: Helicopter down (an "AC"?)
		 * 18: Civilian on fire (a "2-40")
		 * 19: Officer set on fire (a "10-99")
		 * 20: Car on fire
		 * 21: Air unit down (an "AC"?)
		 * 22: An explosion (a "9-96")
		 * 23: A stabbing (a "2-45") (also something else I couldn't understand)
		 * 24: Officer stabbed (also something else I couldn't understand)
		 * 25: Attack on a vehicle ("MDV"?)
		 * 26: Damage to property
		 * 27: Suspect threatening officer with a firearm
		 * 28: Shots fired
		 * 29: ???
		 * 30: ???
		 * 31: ???
		 * 32: ???
		 * 33: ???
		 * 34: A "2-45"
		 * 35: ???
		 * 36: A "9-25"
		 * 37: ???
		 * 38: ???
		 * 39: ???
		 * 40: ???
		 * 41: ???
		 * 42: ???
		 * 43: Possible disturbance
		 * 44: Civilian in need of assistance
		 * 45: ???
		 * 46: ???
		 */
		void REPORT_CRIME(Player player, int crimeType, int wantedLvlThresh);
		/**
		 * crimeType: see REPORT_CRIME
		 */
		void SUPPRESS_CRIME_THIS_FRAME(Player player, int crimeType);
		/**
		 * This native is used in both singleplayer and multiplayer scripts.
		 * 
		 * Always used like this in scripts
		 * PLAYER::UPDATE_WANTED_POSITION_THIS_FRAME(PLAYER::PLAYER_ID());
		 */
		void UPDATE_WANTED_POSITION_THIS_FRAME(Player player);
		/**
		 * This has been found in use in the decompiled files.
		 */
		void SUPPRESS_LOSING_WANTED_LEVEL_IF_HIDDEN_THIS_FRAME(Player player);
		void ALLOW_EVASION_HUD_IF_DISABLING_HIDDEN_EVASION_THIS_FRAME(Player player, Any p1);
		/**
		 * This has been found in use in the decompiled files.
		 */
		void FORCE_START_HIDDEN_EVASION(Player player);
		void SUPPRESS_WITNESSES_CALLING_POLICE_THIS_FRAME(Player player);
		void REPORT_POLICE_SPOTTED_PLAYER(Player player);
		/**
		 * PLAYER::SET_LAW_RESPONSE_DELAY_OVERRIDE(rPtr((&l_122) + 71)); // Found in decompilation
		 * 
		 * ***
		 * 
		 * In "am_hold_up.ysc" used once:
		 * 
		 * l_8d._f47 = MISC::GET_RANDOM_FLOAT_IN_RANGE(18.0, 28.0);
		 * PLAYER::SET_LAW_RESPONSE_DELAY_OVERRIDE((l_8d._f47));
		 */
		void SET_LAW_RESPONSE_DELAY_OVERRIDE(float p0);
		void RESET_LAW_RESPONSE_DELAY_OVERRIDE();
		BOOL CAN_PLAYER_START_MISSION(Player player);
		BOOL IS_PLAYER_READY_FOR_CUTSCENE(Player player);
		BOOL IS_PLAYER_TARGETTING_ENTITY(Player player, Entity entity);
		/**
		 * Assigns the handle of locked-on melee target to *entity that you pass it.
		 * Returns false if no entity found.
		 */
		BOOL GET_PLAYER_TARGET_ENTITY(Player player, Entity* entity);
		/**
		 * Gets a value indicating whether the specified player is currently aiming freely.
		 */
		BOOL IS_PLAYER_FREE_AIMING(Player player);
		/**
		 * Gets a value indicating whether the specified player is currently aiming freely at the specified entity.
		 */
		BOOL IS_PLAYER_FREE_AIMING_AT_ENTITY(Player player, Entity entity);
		/**
		 * Returns TRUE if it found an entity in your crosshair within range of your weapon. Assigns the handle of the target to the *entity that you pass it.
		 * Returns false if no entity found.
		 */
		BOOL GET_ENTITY_PLAYER_IS_FREE_AIMING_AT(Player player, Entity* entity);
		/**
		 * Affects the range of auto aim target.
		 */
		void SET_PLAYER_LOCKON_RANGE_OVERRIDE(Player player, float range);
		/**
		 * Set whether this player should be able to do drive-bys.
		 * 
		 * "A drive-by is when a ped is aiming/shooting from vehicle. This includes middle finger taunts. By setting this value to false I confirm the player is unable to do all that. Tested on tick."
		 * 
		 */
		void SET_PLAYER_CAN_DO_DRIVE_BY(Player player, BOOL toggle);
		/**
		 * Sets whether this player can be hassled by gangs.
		 */
		void SET_PLAYER_CAN_BE_HASSLED_BY_GANGS(Player player, BOOL toggle);
		/**
		 * Sets whether this player can take cover.
		 */
		void SET_PLAYER_CAN_USE_COVER(Player player, BOOL toggle);
		/**
		 * Gets the maximum wanted level the player can get.
		 * Ranges from 0 to 5.
		 */
		int GET_MAX_WANTED_LEVEL();
		BOOL IS_PLAYER_TARGETTING_ANYTHING(Player player);
		void SET_PLAYER_SPRINT(Player player, BOOL toggle);
		void RESET_PLAYER_STAMINA(Player player);
		void RESTORE_PLAYER_STAMINA(Player player, float p1);
		float GET_PLAYER_SPRINT_STAMINA_REMAINING(Player player);
		float GET_PLAYER_SPRINT_TIME_REMAINING(Player player);
		float GET_PLAYER_UNDERWATER_TIME_REMAINING(Player player);
		float SET_PLAYER_UNDERWATER_BREATH_PERCENT_REMAINING(Player player, float time);
		/**
		 * Returns the group ID the player is member of.
		 */
		int GET_PLAYER_GROUP(Player player);
		int GET_PLAYER_MAX_ARMOUR(Player player);
		/**
		 * Can the player control himself, used to disable controls for player for things like a cutscene.
		 * 
		 * ---
		 * 
		 * You can't disable controls with this, use SET_PLAYER_CONTROL(...) for this.
		 */
		BOOL IS_PLAYER_CONTROL_ON(Player player);
		/**
		 * Returns true when the player is not able to control the cam i.e. when running a benchmark test, switching the player or viewing a cutscene.
		 * 
		 * Note: I am not 100% sure if the native actually checks if the cam control is disabled but it seems promising.
		 */
		BOOL GET_ARE_CAMERA_CONTROLS_DISABLED();
		BOOL IS_PLAYER_SCRIPT_CONTROL_ON(Player player);
		/**
		 * Returns TRUE if the player ('s ped) is climbing at the moment.
		 */
		BOOL IS_PLAYER_CLIMBING(Player player);
		/**
		 * Return true while player is being arrested / busted.
		 * 
		 * If atArresting is set to 1, this function will return 1 when player is being arrested (while player is putting his hand up, but still have control)
		 * 
		 * If atArresting is set to 0, this function will return 1 only when the busted screen is shown.
		 */
		BOOL IS_PLAYER_BEING_ARRESTED(Player player, BOOL atArresting);
		void RESET_PLAYER_ARREST_STATE(Player player);
		/**
		 * Alternative: GET_VEHICLE_PED_IS_IN(PLAYER_PED_ID(), 1);
		 */
		Vehicle GET_PLAYERS_LAST_VEHICLE();
		/**
		 * Returns the same as PLAYER_ID and NETWORK_PLAYER_ID_TO_INT
		 */
		Player GET_PLAYER_INDEX();
		/**
		 * Simply returns whatever is passed to it (Regardless of whether the handle is valid or not).
		 */
		Player INT_TO_PLAYERINDEX(int value);
		/**
		 * Simply returns whatever is passed to it (Regardless of whether the handle is valid or not).
		 * --------------------------------------------------------
		 * if (NETWORK::NETWORK_IS_PARTICIPANT_ACTIVE(PLAYER::INT_TO_PARTICIPANTINDEX(i)))
		 * 
		 */
		int INT_TO_PARTICIPANTINDEX(int value);
		int GET_TIME_SINCE_PLAYER_HIT_VEHICLE(Player player);
		int GET_TIME_SINCE_PLAYER_HIT_PED(Player player);
		int GET_TIME_SINCE_PLAYER_DROVE_ON_PAVEMENT(Player player);
		int GET_TIME_SINCE_PLAYER_DROVE_AGAINST_TRAFFIC(Player player);
		BOOL IS_PLAYER_FREE_FOR_AMBIENT_TASK(Player player);
		/**
		 * This returns YOUR 'identity' as a Player type.
		 * 
		 * Always returns 0 in story mode.
		 */
		Player PLAYER_ID();
		/**
		 * Returns current player ped
		 */
		Ped PLAYER_PED_ID();
		/**
		 * Does exactly the same thing as PLAYER_ID()
		 */
		int NETWORK_PLAYER_ID_TO_INT();
		BOOL HAS_FORCE_CLEANUP_OCCURRED(int cleanupFlags);
		/**
		 * used with 1,2,8,64,128 in the scripts
		 */
		void FORCE_CLEANUP(int cleanupFlags);
		/**
		 * PLAYER::FORCE_CLEANUP_FOR_ALL_THREADS_WITH_THIS_NAME("pb_prostitute", 1); // Found in decompilation
		 */
		void FORCE_CLEANUP_FOR_ALL_THREADS_WITH_THIS_NAME(const char* name, int cleanupFlags);
		void FORCE_CLEANUP_FOR_THREAD_WITH_THIS_ID(int id, int cleanupFlags);
		int GET_CAUSE_OF_MOST_RECENT_FORCE_CLEANUP();
		void SET_PLAYER_MAY_ONLY_ENTER_THIS_VEHICLE(Player player, Vehicle vehicle);
		void SET_PLAYER_MAY_NOT_ENTER_ANY_VEHICLE(Player player);
		/**
		 * 1 - Welcome to Los Santos
		 * 2 - A Friendship Resurrected
		 * 3 - A Fair Day's Pay
		 * 4 - The Moment of Truth
		 * 5 - To Live or Die in Los Santos
		 * 6 - Diamond Hard
		 * 7 - Subversive
		 * 8 - Blitzed
		 * 9 - Small Town, Big Job
		 * 10 - The Government Gimps
		 * 11 - The Big One!
		 * 12 - Solid Gold, Baby!
		 * 13 - Career Criminal
		 * 14 - San Andreas Sightseer
		 * 15 - All's Fare in Love and War
		 * 16 - TP Industries Arms Race
		 * 17 - Multi-Disciplined
		 * 18 - From Beyond the Stars
		 * 19 - A Mystery, Solved
		 * 20 - Waste Management
		 * 21 - Red Mist
		 * 22 - Show Off
		 * 23 - Kifflom!
		 * 24 - Three Man Army
		 * 25 - Out of Your Depth
		 * 26 - Altruist Acolyte
		 * 27 - A Lot of Cheddar
		 * 28 - Trading Pure Alpha
		 * 29 - Pimp My Sidearm
		 * 30 - Wanted: Alive Or Alive
		 * 31 - Los Santos Customs
		 * 32 - Close Shave
		 * 33 - Off the Plane
		 * 34 - Three-Bit Gangster
		 * 35 - Making Moves
		 * 36 - Above the Law
		 * 37 - Numero Uno
		 * 38 - The Midnight Club
		 * 39 - Unnatural Selection
		 * 40 - Backseat Driver
		 * 41 - Run Like The Wind
		 * 42 - Clean Sweep
		 * 43 - Decorated
		 * 44 - Stick Up Kid
		 * 45 - Enjoy Your Stay
		 * 46 - Crew Cut
		 * 47 - Full Refund
		 * 48 - Dialling Digits
		 * 49 - American Dream
		 * 50 - A New Perspective
		 * 51 - Be Prepared
		 * 52 - In the Name of Science
		 * 53 - Dead Presidents
		 * 54 - Parole Day
		 * 55 - Shot Caller
		 * 56 - Four Way
		 * 57 - Live a Little
		 * 58 - Can't Touch This
		 * 59 - Mastermind
		 * 60 - Vinewood Visionary
		 * 61 - Majestic
		 * 62 - Humans of Los Santos
		 * 63 - First Time Director
		 * 64 - Animal Lover
		 * 65 - Ensemble Piece
		 * 66 - Cult Movie
		 * 67 - Location Scout
		 * 68 - Method Actor
		 * 69 - Cryptozoologist
		 * 70 - Getting Started
		 * 71 - The Data Breaches
		 * 72 - The Bogdan Problem
		 * 73 - The Doomsday Scenario
		 * 74 - A World Worth Saving
		 * 75 - Orbital Obliteration
		 * 76 - Elitist
		 * 77 - Masterminds
		 */
		BOOL GIVE_ACHIEVEMENT_TO_PLAYER(int achievementId);
		/**
		 * For Steam.
		 * Does nothing and always returns false in the retail version of the game.
		 */
		BOOL SET_ACHIEVEMENT_PROGRESS(int achievementId, int progress);
		/**
		 * For Steam.
		 * Always returns 0 in retail version of the game.
		 */
		int GET_ACHIEVEMENT_PROGRESS(int achievementId);
		/**
		 * See GIVE_ACHIEVEMENT_TO_PLAYER
		 */
		BOOL HAS_ACHIEVEMENT_BEEN_PASSED(int achievementId);
		/**
		 * Returns TRUE if the game is in online mode and FALSE if in offline mode.
		 * 
		 * This is an alias for NETWORK_IS_SIGNED_ONLINE.
		 */
		BOOL IS_PLAYER_ONLINE();
		/**
		 * this function is hard-coded to always return 0.
		 */
		BOOL IS_PLAYER_LOGGING_IN_NP();
		/**
		 * Purpose of the BOOL currently unknown.
		 * Both, true and false, work
		 */
		void DISPLAY_SYSTEM_SIGNIN_UI(BOOL p0);
		BOOL IS_SYSTEM_UI_BEING_DISPLAYED();
		/**
		 * Simply sets you as invincible (Health will not deplete).
		 * 
		 * Use 0x733A643B5B0C53C1 instead if you want Ragdoll enabled, which is equal to:
		 * *(DWORD *)(playerPedAddress + 0x188) |= (1 << 9);
		 */
		void SET_PLAYER_INVINCIBLE(Player player, BOOL toggle);
		/**
		 * Returns the Player's Invincible status.
		 * 
		 * This function will always return false if 0x733A643B5B0C53C1 is used to set the invincibility status. To always get the correct result, use this:
		 * 
		 * bool IsPlayerInvincible(Player player)
		 * {
		 * auto addr = getScriptHandleBaseAddress(GET_PLAYER_PED(player));
		 * 
		 * if (addr)
		 * {
		 * DWORD flag = *(DWORD *)(addr + 0x188);
		 * return ((flag & (1 << 8)) != 0) || ((flag & (1 << 9)) != 0);
		 * }
		 * 
		 * return false;
		 * }
		 * 
		 * 
		 */
		BOOL GET_PLAYER_INVINCIBLE(Player player);
		/**
		 * Always returns false.
		 */
		BOOL GET_PLAYER_DEBUG_INVINCIBLE(Player player);
		void SET_PLAYER_INVINCIBLE_BUT_HAS_REACTIONS(Player player, BOOL toggle);
		void SET_PLAYER_CAN_COLLECT_DROPPED_MONEY(Player player, BOOL p1);
		void REMOVE_PLAYER_HELMET(Player player, BOOL p2);
		void GIVE_PLAYER_RAGDOLL_CONTROL(Player player, BOOL toggle);
		/**
		 * Example from fm_mission_controler.ysc.c4:
		 * 
		 * PLAYER::SET_PLAYER_LOCKON(PLAYER::PLAYER_ID(), 1);
		 * 
		 * All other decompiled scripts using this seem to be using the player id as the first parameter, so I feel the need to confirm it as so.
		 * 
		 * No need to confirm it says PLAYER_ID() so it uses PLAYER_ID() lol.
		 */
		void SET_PLAYER_LOCKON(Player player, BOOL toggle);
		/**
		 * Sets your targeting mode.
		 * 0 = Assisted Aim - Full
		 * 1 = Assisted Aim - Partial
		 * 2 = Free Aim - Assisted
		 * 3 = Free Aim
		 */
		void SET_PLAYER_TARGETING_MODE(int targetMode);
		int GET_PLAYER_TARGETING_MODE();
		void SET_PLAYER_TARGET_LEVEL(int targetLevel);
		/**
		 * Returns profile setting 237.
		 */
		BOOL GET_IS_USING_FPS_THIRD_PERSON_COVER();
		/**
		 * Returns profile setting 243.
		 */
		BOOL GET_IS_USING_HOOD_CAMERA();
		void CLEAR_PLAYER_HAS_DAMAGED_AT_LEAST_ONE_PED(Player player);
		BOOL HAS_PLAYER_DAMAGED_AT_LEAST_ONE_PED(Player player);
		void CLEAR_PLAYER_HAS_DAMAGED_AT_LEAST_ONE_NON_ANIMAL_PED(Player player);
		BOOL HAS_PLAYER_DAMAGED_AT_LEAST_ONE_NON_ANIMAL_PED(Player player);
		/**
		 * This can be between 1.0f - 14.9f
		 * 
		 * You can change the max in IDA from 15.0. I say 15.0 as the function blrs if what you input is greater than or equal to 15.0 hence why it's 14.9 max default.
		 * 
		 * 
		 */
		void SET_AIR_DRAG_MULTIPLIER_FOR_PLAYERS_VEHICLE(Player player, float multiplier);
		/**
		 * Swim speed multiplier.
		 * Multiplier goes up to 1.49
		 * 
		 * Just call it one time, it is not required to be called once every tick. - Note copied from below native.
		 * 
		 * Note: At least the IDA method if you change the max float multiplier from 1.5 it will change it for both this and RUN_SPRINT below. I say 1.5 as the function blrs if what you input is greater than or equal to 1.5 hence why it's 1.49 max default.
		 */
		void SET_SWIM_MULTIPLIER_FOR_PLAYER(Player player, float multiplier);
		/**
		 * Multiplier goes up to 1.49 any value above will be completely overruled by the game and the multiplier will not take effect, this can be edited in memory however.
		 * 
		 * Just call it one time, it is not required to be called once every tick.
		 * 
		 * Note: At least the IDA method if you change the max float multiplier from 1.5 it will change it for both this and SWIM above. I say 1.5 as the function blrs if what you input is greater than or equal to 1.5 hence why it's 1.49 max default.
		 */
		void SET_RUN_SPRINT_MULTIPLIER_FOR_PLAYER(Player player, float multiplier);
		/**
		 * Returns the time since the character was arrested in (ms) milliseconds.
		 * 
		 * example
		 * 
		 * var time = Function.call<int>(Hash.GET_TIME_SINCE_LAST_ARREST();
		 * 
		 * UI.DrawSubtitle(time.ToString());
		 * 
		 * if player has not been arrested, the int returned will be -1.
		 */
		int GET_TIME_SINCE_LAST_ARREST();
		/**
		 * Returns the time since the character died in (ms) milliseconds.
		 * 
		 * example
		 * 
		 * var time = Function.call<int>(Hash.GET_TIME_SINCE_LAST_DEATH();
		 * 
		 * UI.DrawSubtitle(time.ToString());
		 * 
		 * if player has not died, the int returned will be -1.
		 */
		int GET_TIME_SINCE_LAST_DEATH();
		void ASSISTED_MOVEMENT_CLOSE_ROUTE();
		void ASSISTED_MOVEMENT_FLUSH_ROUTE();
		void SET_PLAYER_FORCED_AIM(Player player, BOOL toggle);
		void SET_PLAYER_FORCED_ZOOM(Player player, BOOL toggle);
		void SET_PLAYER_FORCE_SKIP_AIM_INTRO(Player player, BOOL toggle);
		/**
		 * Inhibits the player from using any method of combat including melee and firearms.
		 * 
		 * NOTE: Only disables the firing for one frame
		 */
		void DISABLE_PLAYER_FIRING(Player player, BOOL toggle);
		/**
		 * Used only once in R* scripts (freemode.ysc).
		 */
		void DISABLE_PLAYER_THROW_GRENADE_WHILE_USING_GUN();
		void SET_DISABLE_AMBIENT_MELEE_MOVE(Player player, BOOL toggle);
		/**
		 * Default is 100. Use player id and not ped id. For instance: PLAYER::SET_PLAYER_MAX_ARMOUR(PLAYER::PLAYER_ID(), 100); // main_persistent.ct4
		 */
		void SET_PLAYER_MAX_ARMOUR(Player player, int value);
		/**
		 * p1 is always 0 in the scripts
		 */
		void SPECIAL_ABILITY_ACTIVATE(Player player, int p1);
		void SET_SPECIAL_ABILITY_MP(Player player, int p1, Any p2);
		/**
		 * p1 is always 0 in the scripts
		 */
		void SPECIAL_ABILITY_DEACTIVATE_MP(Player player, int p1);
		void SPECIAL_ABILITY_DEACTIVATE(Player player, Any p1);
		void SPECIAL_ABILITY_DEACTIVATE_FAST(Player player, Any p1);
		void SPECIAL_ABILITY_RESET(Player player, Any p1);
		void SPECIAL_ABILITY_CHARGE_ON_MISSION_FAILED(Player player, Any p1);
		/**
		 * Every occurrence of p1 & p2 were both true.
		 */
		void SPECIAL_ABILITY_CHARGE_SMALL(Player player, BOOL p1, BOOL p2, Any p3);
		/**
		 * Only 1 match. Both p1 & p2 were true.
		 */
		void SPECIAL_ABILITY_CHARGE_MEDIUM(Player player, BOOL p1, BOOL p2, Any p3);
		/**
		 * 2 matches. p1 was always true.
		 */
		void SPECIAL_ABILITY_CHARGE_LARGE(Player player, BOOL p1, BOOL p2, Any p3);
		/**
		 * p1 appears to always be 1 (only comes up twice)
		 */
		void SPECIAL_ABILITY_CHARGE_CONTINUOUS(Player player, Ped p1, Any p2);
		/**
		 * p1 appears as 5, 10, 15, 25, or 30. p2 is always true.
		 */
		void SPECIAL_ABILITY_CHARGE_ABSOLUTE(Player player, int p1, BOOL p2, Any p3);
		/**
		 * 
		 * normalizedValue is from 0.0 - 1.0
		 * p2 is always 1
		 */
		void SPECIAL_ABILITY_CHARGE_NORMALIZED(Player player, float normalizedValue, BOOL p2, Any p3);
		/**
		 * Also known as _RECHARGE_SPECIAL_ABILITY
		 */
		void SPECIAL_ABILITY_FILL_METER(Player player, BOOL p1, Any p2);
		/**
		 * p1 was always true.
		 */
		void SPECIAL_ABILITY_DEPLETE_METER(Player player, BOOL p1, Any p2);
		void SPECIAL_ABILITY_LOCK(Hash playerModel, Any p1);
		void SPECIAL_ABILITY_UNLOCK(Hash playerModel, Any p1);
		BOOL IS_SPECIAL_ABILITY_UNLOCKED(Hash playerModel);
		BOOL IS_SPECIAL_ABILITY_ACTIVE(Player player, Any p1);
		BOOL IS_SPECIAL_ABILITY_METER_FULL(Player player, Any p1);
		void ENABLE_SPECIAL_ABILITY(Player player, BOOL toggle, Any p2);
		BOOL IS_SPECIAL_ABILITY_ENABLED(Player player, Any p1);
		void SET_SPECIAL_ABILITY_MULTIPLIER(float multiplier);
		void UPDATE_SPECIAL_ABILITY_FROM_STAT(Player player, Any p1);
		/**
		 * Appears once in "re_dealgonewrong"
		 */
		BOOL GET_IS_PLAYER_DRIVING_ON_HIGHWAY(Player player);
		/**
		 * Only 1 occurrence. p1 was 2.
		 */
		BOOL GET_IS_PLAYER_DRIVING_WRECKLESS(Player player, int p1);
		/**
		 * 2 occurrences in agency_heist3a. p1 was 0.7f then 0.4f.
		 */
		BOOL GET_IS_MOPPING_AREA_FREE_IN_FRONT_OF_PLAYER(Player player, float p1);
		/**
		 * `findCollisionLand`: This teleports the player to land when set to true and will not consider the Z coordinate parameter provided by you. It will automatically put the Z coordinate so that you don't fall from sky.
		 */
		void START_PLAYER_TELEPORT(Player player, float x, float y, float z, float heading, BOOL p5, BOOL findCollisionLand, BOOL p7);
		BOOL UPDATE_PLAYER_TELEPORT(Player player);
		/**
		 * Disables the player's teleportation
		 */
		void STOP_PLAYER_TELEPORT();
		BOOL IS_PLAYER_TELEPORT_ACTIVE();
		float GET_PLAYER_CURRENT_STEALTH_NOISE(Player player);
		/**
		 * `regenRate`: The recharge multiplier, a value between 0.0 and 1.0.
		 * Use 1.0 to reset it back to normal
		 */
		void SET_PLAYER_HEALTH_RECHARGE_MULTIPLIER(Player player, float regenRate);
		float GET_PLAYER_HEALTH_RECHARGE_MAX_PERCENT(Player player);
		void SET_PLAYER_HEALTH_RECHARGE_MAX_PERCENT(Player player, float limit);
		/**
		 * Needs to be called every frame.
		 */
		void DISABLE_PLAYER_HEALTH_RECHARGE(Player player);
		void SET_PLAYER_FALL_DISTANCE_TO_TRIGGER_RAGDOLL_OVERRIDE(Player player, float p1);
		/**
		 * This modifies the damage value of your weapon. Whether it is a multiplier or base damage is unknown.
		 * 
		 * Based on tests, it is unlikely to be a multiplier.
		 * 
		 * modifier's min value is 0.1
		 */
		void SET_PLAYER_WEAPON_DAMAGE_MODIFIER(Player player, float modifier);
		/**
		 * modifier's min value is 0.1
		 */
		void SET_PLAYER_WEAPON_DEFENSE_MODIFIER(Player player, float modifier);
		/**
		 * modifier's min value is 0.1
		 */
		void SET_PLAYER_WEAPON_MINIGUN_DEFENSE_MODIFIER(Player player, float modifier);
		/**
		 * modifier's min value is 0.1
		 */
		void SET_PLAYER_MELEE_WEAPON_DAMAGE_MODIFIER(Player player, float modifier, BOOL p2);
		/**
		 * modifier's min value is 0.1
		 */
		void SET_PLAYER_MELEE_WEAPON_DEFENSE_MODIFIER(Player player, float modifier);
		/**
		 * modifier's min value is 0.1
		 */
		void SET_PLAYER_VEHICLE_DAMAGE_MODIFIER(Player player, float modifier);
		/**
		 * modifier's min value is 0.1
		 */
		void SET_PLAYER_VEHICLE_DEFENSE_MODIFIER(Player player, float modifier);
		void SET_PLAYER_MAX_EXPLOSIVE_DAMAGE(Player player, float p1);
		void SET_PLAYER_EXPLOSIVE_DAMAGE_MODIFIER(Player player, Any p1);
		void SET_PLAYER_WEAPON_TAKEDOWN_DEFENSE_MODIFIER(Player player, float p1);
		/**
		 * Tints:
		 * None = -1,
		 * Rainbow = 0,
		 * Red = 1,
		 * SeasideStripes = 2,
		 * WidowMaker = 3,
		 * Patriot = 4,
		 * Blue = 5,
		 * Black = 6,
		 * Hornet = 7,
		 * AirFocce = 8,
		 * Desert = 9,
		 * Shadow = 10,
		 * HighAltitude = 11,
		 * Airbone = 12,
		 * Sunrise = 13,
		 * 
		 */
		void SET_PLAYER_PARACHUTE_TINT_INDEX(Player player, int tintIndex);
		/**
		 * Tints:
		 * None = -1,
		 * Rainbow = 0,
		 * Red = 1,
		 * SeasideStripes = 2,
		 * WidowMaker = 3,
		 * Patriot = 4,
		 * Blue = 5,
		 * Black = 6,
		 * Hornet = 7,
		 * AirFocce = 8,
		 * Desert = 9,
		 * Shadow = 10,
		 * HighAltitude = 11,
		 * Airbone = 12,
		 * Sunrise = 13,
		 */
		void GET_PLAYER_PARACHUTE_TINT_INDEX(Player player, int* tintIndex);
		/**
		 * Tints:
		 * None = -1,
		 * Rainbow = 0,
		 * Red = 1,
		 * SeasideStripes = 2,
		 * WidowMaker = 3,
		 * Patriot = 4,
		 * Blue = 5,
		 * Black = 6,
		 * Hornet = 7,
		 * AirFocce = 8,
		 * Desert = 9,
		 * Shadow = 10,
		 * HighAltitude = 11,
		 * Airbone = 12,
		 * Sunrise = 13,
		 */
		void SET_PLAYER_RESERVE_PARACHUTE_TINT_INDEX(Player player, int index);
		/**
		 * Tints:
		 * None = -1,
		 * Rainbow = 0,
		 * Red = 1,
		 * SeasideStripes = 2,
		 * WidowMaker = 3,
		 * Patriot = 4,
		 * Blue = 5,
		 * Black = 6,
		 * Hornet = 7,
		 * AirFocce = 8,
		 * Desert = 9,
		 * Shadow = 10,
		 * HighAltitude = 11,
		 * Airbone = 12,
		 * Sunrise = 13,
		 */
		void GET_PLAYER_RESERVE_PARACHUTE_TINT_INDEX(Player player, int* index);
		/**
		 * tints 0- 13
		 * 0 - unkown
		 * 1 - unkown
		 * 2 - unkown
		 * 3 - unkown
		 * 4 - unkown
		 */
		void SET_PLAYER_PARACHUTE_PACK_TINT_INDEX(Player player, int tintIndex);
		void GET_PLAYER_PARACHUTE_PACK_TINT_INDEX(Player player, int* tintIndex);
		void SET_PLAYER_HAS_RESERVE_PARACHUTE(Player player);
		BOOL GET_PLAYER_HAS_RESERVE_PARACHUTE(Player player);
		void SET_PLAYER_CAN_LEAVE_PARACHUTE_SMOKE_TRAIL(Player player, BOOL enabled);
		void SET_PLAYER_PARACHUTE_SMOKE_TRAIL_COLOR(Player player, int r, int g, int b);
		void GET_PLAYER_PARACHUTE_SMOKE_TRAIL_COLOR(Player player, int* r, int* g, int* b);
		void SET_PLAYER_PHONE_PALETTE_IDX(Player player, int idx);
		void SET_PLAYER_NOISE_MULTIPLIER(Player player, float multiplier);
		/**
		 * Values around 1.0f to 2.0f used in game scripts.
		 */
		void SET_PLAYER_SNEAKING_NOISE_MULTIPLIER(Player player, float multiplier);
		BOOL CAN_PED_HEAR_PLAYER(Player player, Ped ped);
		/**
		 * This is to make the player walk without accepting input from INPUT.
		 * 
		 * gaitType is in increments of 100s. 2000, 500, 300, 200, etc.
		 * 
		 * p4 is always 1 and p5 is always 0.
		 * 
		 * C# Example :
		 * 
		 * Function.Call(Hash.SIMULATE_PLAYER_INPUT_GAIT, Game.Player, 1.0f, 100, 1.0f, 1, 0); //Player will go forward for 100ms
		 */
		void SIMULATE_PLAYER_INPUT_GAIT(Player player, float amount, int gaitType, float speed, BOOL p4, BOOL p5, Any p6);
		void RESET_PLAYER_INPUT_GAIT(Player player);
		void SET_AUTO_GIVE_PARACHUTE_WHEN_ENTER_PLANE(Player player, BOOL toggle);
		void SET_AUTO_GIVE_SCUBA_GEAR_WHEN_EXIT_VEHICLE(Player player, BOOL toggle);
		void SET_PLAYER_STEALTH_PERCEPTION_MODIFIER(Player player, float value);
		BOOL IS_REMOTE_PLAYER_IN_NON_CLONED_VEHICLE(Player player);
		void INCREASE_PLAYER_JUMP_SUPPRESSION_RANGE(Player player);
		void SET_PLAYER_SIMULATE_AIMING(Player player, BOOL toggle);
		void SET_PLAYER_CLOTH_PIN_FRAMES(Player player, int p1);
		/**
		 * Every occurrence was either 0 or 2.
		 */
		void SET_PLAYER_CLOTH_PACKAGE_INDEX(int index);
		/**
		 * 6 matches across 4 scripts. 5 occurrences were 240. The other was 255.
		 */
		void SET_PLAYER_CLOTH_LOCK_COUNTER(int value);
		/**
		 * Only 1 match. ob_sofa_michael.
		 * 
		 * PLAYER::PLAYER_ATTACH_VIRTUAL_BOUND(-804.5928f, 173.1801f, 71.68436f, 0f, 0f, 0.590625f, 1f, 0.7f);1.0.335.2, 1.0.350.1/2, 1.0.372.2, 1.0.393.2, 1.0.393.4, 1.0.463.1;
		 */
		void PLAYER_ATTACH_VIRTUAL_BOUND(float p0, float p1, float p2, float p3, float p4, float p5, float p6, float p7);
		/**
		 * 1.0.335.2, 1.0.350.1/2, 1.0.372.2, 1.0.393.2, 1.0.393.4, 1.0.463.1;
		 */
		void PLAYER_DETACH_VIRTUAL_BOUND();
		BOOL HAS_PLAYER_BEEN_SPOTTED_IN_STOLEN_VEHICLE(Player player);
		/**
		 * Returns true if an unk value is greater than 0.0f
		 */
		BOOL IS_PLAYER_BATTLE_AWARE(Player player);
		BOOL GET_PLAYER_RECEIVED_BATTLE_EVENT_RECENTLY(Player player, int p1, BOOL p2);
		/**
		 * Appears only 3 times in the scripts, more specifically in michael1.ysc
		 * 
		 * -
		 * This can be used to prevent dying if you are "out of the world"
		 */
		void EXTEND_WORLD_BOUNDARY_FOR_PLAYER(float x, float y, float z);
		void RESET_WORLD_BOUNDARY_FOR_PLAYER();
		/**
		 * Returns true if the player is riding a train.
		 */
		BOOL IS_PLAYER_RIDING_TRAIN(Player player);
		BOOL HAS_PLAYER_LEFT_THE_WORLD(Player player);
		void SET_PLAYER_LEAVE_PED_BEHIND(Player player, BOOL toggle);
		/**
		 * p1 was always 5.
		 * p4 was always false.
		 */
		void SET_PLAYER_PARACHUTE_VARIATION_OVERRIDE(Player player, int p1, Any p2, Any p3, BOOL p4);
		void CLEAR_PLAYER_PARACHUTE_VARIATION_OVERRIDE(Player player);
		void SET_PLAYER_PARACHUTE_MODEL_OVERRIDE(Player player, Hash model);
		void SET_PLAYER_RESERVE_PARACHUTE_MODEL_OVERRIDE(Player player, Hash model);
		Hash GET_PLAYER_PARACHUTE_MODEL_OVERRIDE(Player player);
		Hash GET_PLAYER_RESERVE_PARACHUTE_MODEL_OVERRIDE(Player player);
		void CLEAR_PLAYER_PARACHUTE_MODEL_OVERRIDE(Player player);
		void CLEAR_PLAYER_RESERVE_PARACHUTE_MODEL_OVERRIDE(Player player);
		void SET_PLAYER_PARACHUTE_PACK_MODEL_OVERRIDE(Player player, Hash model);
		void CLEAR_PLAYER_PARACHUTE_PACK_MODEL_OVERRIDE(Player player);
		void DISABLE_PLAYER_VEHICLE_REWARDS(Player player);
		void SET_PLAYER_SPECTATED_VEHICLE_RADIO_OVERRIDE(BOOL p0);
		void SET_PLAYER_BLUETOOTH_STATE(Player player, BOOL state);
		BOOL IS_PLAYER_BLUETOOTH_ENABLE(Player player);
		void DISABLE_CAMERA_VIEW_MODE_CYCLE(Player player);
		int GET_PLAYER_FAKE_WANTED_LEVEL(Player player);
		void SET_PLAYER_CAN_DAMAGE_PLAYER(Player player1, Player player2, BOOL toggle);
		void SET_APPLY_WAYPOINT_OF_PLAYER(Player player, int hudColor);
		BOOL IS_PLAYER_VEHICLE_WEAPON_TOGGLED_TO_NON_HOMING(Any p0);
		/**
		 * Unsets playerPed+330 if the current weapon has certain flags.
		 */
		void SET_PLAYER_VEHICLE_WEAPON_TO_NON_HOMING(Any p0);
		void SET_PLAYER_HOMING_DISABLED_FOR_ALL_VEHICLE_WEAPONS(Any p0, Any p1);
		void ADD_PLAYER_TARGETABLE_ENTITY(Player player, Entity entity);
		void REMOVE_PLAYER_TARGETABLE_ENTITY(Player player, Entity entity);
		void SET_PLAYER_PREVIOUS_VARIATION_DATA(Player player, int p1, int p2, Any p3, Any p4, Any p5);
		/**
		 * Resets values set by SET_SCRIPT_FIRE_POSITION
		 */
		void REMOVE_SCRIPT_FIRE_POSITION();
		void SET_SCRIPT_FIRE_POSITION(float coordX, float coordY, float coordZ);

	} // namespace PLAYER

	namespace RECORDING {

		void REPLAY_START_EVENT(int p0);
		void REPLAY_STOP_EVENT();
		void REPLAY_CANCEL_EVENT();
		void REPLAY_RECORD_BACK_FOR_TIME(float p0, float p1, int p2);
		/**
		 * -This function appears to be deprecated/ unused. Tracing the call internally leads to a _nullsub -
		 * 
		 * first one seems to be a string of a mission name, second one seems to be a bool/toggle
		 * 
		 * p1 was always 0.
		 * 
		 */
		void REPLAY_CHECK_FOR_EVENT_THIS_FRAME(const char* missionNameLabel, Any p1);
		/**
		 * This disable the recording feature and has to be called every frame.
		 */
		void REPLAY_PREVENT_RECORDING_THIS_FRAME();
		void REPLAY_RESET_EVENT_INFO();
		/**
		 * This will disable the ability to make camera changes in R* Editor.
		 */
		void REPLAY_DISABLE_CAMERA_MOVEMENT_THIS_FRAME();
		/**
		 * Does nothing (it's a nullsub).
		 */
		void RECORD_GREATEST_MOMENT(int p0, int p1, int p2);
		/**
		 * Starts recording a replay.
		 * If mode is 0, turns on action replay.
		 * If mode is 1, starts recording.
		 * If already recording a replay, does nothing.
		 */
		void START_REPLAY_RECORDING(int mode);
		/**
		 * Stops recording and saves the recorded clip.
		 */
		void STOP_REPLAY_RECORDING();
		/**
		 * Stops recording and discards the recorded clip.
		 */
		void CANCEL_REPLAY_RECORDING();
		BOOL SAVE_REPLAY_RECORDING();
		/**
		 * Checks if you're recording, returns TRUE when you start recording (F1) or turn on action replay (F2)
		 * 
		 * mov al, cs:g_bIsRecordingGameplay // byte_141DD0CD0 in b944
		 * retn
		 */
		BOOL IS_REPLAY_RECORDING();
		BOOL IS_REPLAY_INITIALIZED();
		BOOL IS_REPLAY_AVAILABLE();
		BOOL IS_REPLAY_RECORD_SPACE_AVAILABLE(BOOL p0);

	} // namespace RECORDING

	namespace REPLAY {

		/**
		 * Does nothing (it's a nullsub).
		 */
		void REGISTER_EFFECT_FOR_REPLAY_EDITOR(const char* p0, BOOL p1);
		/**
		 * Returns a bool if interior rendering is disabled, if yes, all "normal" rendered interiors are invisible
		 */
		BOOL REPLAY_SYSTEM_HAS_REQUESTED_A_SCRIPT_CLEANUP();
		/**
		 * Disables some other rendering (internal)
		 */
		void SET_SCRIPTS_HAVE_CLEANED_UP_FOR_REPLAY_SYSTEM();
		void SET_REPLAY_SYSTEM_PAUSED_FOR_SAVE(BOOL p0);
		/**
		 * Sets (almost, not sure) all Rockstar Editor values (bIsRecording etc) to 0.
		 */
		void REPLAY_CONTROL_SHUTDOWN();
		/**
		 * Please note that you will need to call DO_SCREEN_FADE_IN after exiting the Rockstar Editor when you call this.
		 */
		void ACTIVATE_ROCKSTAR_EDITOR(int p0);

	} // namespace REPLAY

	namespace SAVEMIGRATION {

		BOOL SAVEMIGRATION_IS_MP_ENABLED();
		BOOL SAVEMIGRATION_MP_REQUEST_ACCOUNTS();
		int SAVEMIGRATION_MP_GET_ACCOUNTS_STATUS();
		int SAVEMIGRATION_MP_NUM_ACCOUNTS();
		BOOL SAVEMIGRATION_MP_GET_ACCOUNT(int p0, Any* p1);
		BOOL SAVEMIGRATION_MP_REQUEST_STATUS();
		int SAVEMIGRATION_MP_GET_STATUS();
		void SAVEMIGRATION_MP_IS_PLATFORM_GENERATION(int generation); // Missing in crossmap

	} // namespace SAVEMIGRATION

	namespace SCRIPT {

		void REQUEST_SCRIPT(const char* scriptName);
		void SET_SCRIPT_AS_NO_LONGER_NEEDED(const char* scriptName);
		/**
		 * Returns if a script has been loaded into the game. Used to see if a script was loaded after requesting.
		 */
		BOOL HAS_SCRIPT_LOADED(const char* scriptName);
		BOOL DOES_SCRIPT_EXIST(const char* scriptName);
		/**
		 * formerly _REQUEST_STREAMED_SCRIPT
		 */
		void REQUEST_SCRIPT_WITH_NAME_HASH(Hash scriptHash);
		void SET_SCRIPT_WITH_NAME_HASH_AS_NO_LONGER_NEEDED(Hash scriptHash);
		BOOL HAS_SCRIPT_WITH_NAME_HASH_LOADED(Hash scriptHash);
		BOOL DOES_SCRIPT_WITH_NAME_HASH_EXIST(Hash scriptHash);
		void TERMINATE_THREAD(int threadId);
		BOOL IS_THREAD_ACTIVE(int threadId);
		const char* GET_NAME_OF_SCRIPT_WITH_THIS_ID(int threadId);
		/**
		 * Starts a new iteration of the current threads.
		 * Call this first, then SCRIPT_THREAD_ITERATOR_GET_NEXT_THREAD_ID (0x30B4FA1C82DD4B9F)
		 */
		void SCRIPT_THREAD_ITERATOR_RESET();
		/**
		 * If the function returns 0, the end of the iteration has been reached.
		 */
		int SCRIPT_THREAD_ITERATOR_GET_NEXT_THREAD_ID();
		int GET_ID_OF_THIS_THREAD();
		void TERMINATE_THIS_THREAD();
		/**
		 * Gets the number of instances of the specified script is currently running.
		 * 
		 * Actually returns numRefs - 1.
		 * if (program)
		 * v3 = rage::scrProgram::GetNumRefs(program) - 1;
		 * return v3;
		 */
		int GET_NUMBER_OF_THREADS_RUNNING_THE_SCRIPT_WITH_THIS_HASH(Hash scriptHash);
		const char* GET_THIS_SCRIPT_NAME();
		Hash GET_HASH_OF_THIS_SCRIPT_NAME();
		/**
		 * eventGroup: 0 = SCRIPT_EVENT_QUEUE_AI (CEventGroupScriptAI), 1 = SCRIPT_EVENT_QUEUE_NETWORK (CEventGroupScriptNetwork)
		 */
		int GET_NUMBER_OF_EVENTS(int eventGroup);
		/**
		 * eventGroup: 0 = SCRIPT_EVENT_QUEUE_AI (CEventGroupScriptAI), 1 = SCRIPT_EVENT_QUEUE_NETWORK (CEventGroupScriptNetwork)
		 */
		BOOL GET_EVENT_EXISTS(int eventGroup, int eventIndex);
		/**
		 * eventGroup: 0 = SCRIPT_EVENT_QUEUE_AI (CEventGroupScriptAI), 1 = SCRIPT_EVENT_QUEUE_NETWORK (CEventGroupScriptNetwork)
		 */
		int GET_EVENT_AT_INDEX(int eventGroup, int eventIndex);
		/**
		 * eventGroup: 0 = SCRIPT_EVENT_QUEUE_AI (CEventGroupScriptAI), 1 = SCRIPT_EVENT_QUEUE_NETWORK (CEventGroupScriptNetwork)
		 * 
		 * Note: eventDataSize is NOT the size in bytes, it is the size determined by the SIZE_OF operator (RAGE Script operator, not C/C++ sizeof). That is, the size in bytes divided by 8 (script variables are always 8-byte aligned!).
		 */
		BOOL GET_EVENT_DATA(int eventGroup, int eventIndex, Any* eventData, int eventDataSize);
		/**
		 * eventGroup: 0 = SCRIPT_EVENT_QUEUE_AI (CEventGroupScriptAI), 1 = SCRIPT_EVENT_QUEUE_NETWORK (CEventGroupScriptNetwork)
		 * 
		 * Note: eventDataSize is NOT the size in bytes, it is the size determined by the SIZE_OF operator (RAGE Script operator, not C/C++ sizeof). That is, the size in bytes divided by 8 (script variables are always 8-byte aligned!).
		 * 
		 * playerBits (also known as playersToBroadcastTo) is a bitset that indicates which players this event should be sent to. In order to send the event to specific players only, use (1 << playerIndex). Set all bits if it should be broadcast to all players.
		 */
		void TRIGGER_SCRIPT_EVENT(int eventGroup, Any* eventData, int eventDataSize, int playerBits);
		void SHUTDOWN_LOADING_SCREEN();
		void SET_NO_LOADING_SCREEN(BOOL toggle);
		BOOL GET_NO_LOADING_SCREEN();
		void SET_LOADING_SCREEN_BLANK(); // Missing in crossmap
		void COMMIT_TO_LOADINGSCREEN_SELCTION();
		/**
		 * Returns true if bit 0 in GtaThread+0x154 is set.
		 */
		BOOL BG_IS_EXITFLAG_SET();
		/**
		 * Sets bit 1 in GtaThread+0x154
		 */
		void BG_SET_EXITFLAG_RESPONSE();
		/**
		 * Hashed version of BG_START_CONTEXT.
		 */
		void BG_START_CONTEXT_HASH(Hash contextHash);
		/**
		 * Hashed version of BG_END_CONTEXT.
		 */
		void BG_END_CONTEXT_HASH(Hash contextHash);
		/**
		 * Inserts the given context into the background scripts context map.
		 */
		void BG_START_CONTEXT(const char* contextName);
		/**
		 * Deletes the given context from the background scripts context map.
		 */
		void BG_END_CONTEXT(const char* contextName);
		BOOL BG_DOES_LAUNCH_PARAM_EXIST(int scriptIndex, const char* p1);
		int BG_GET_LAUNCH_PARAM_VALUE(int scriptIndex, const char* p1);
		int BG_GET_SCRIPT_ID_FROM_NAME_HASH(Hash p0);
		/**
		 * New variant of SEND_TU_SCRIPT_EVENT that automatically initializes the event data header.
		 * See TRIGGER_SCRIPT_EVENT for more info.
		 */
		void _SEND_TU_SCRIPT_EVENT_NEW(int eventGroup, Any* eventData, int eventDataSize, int playerBits, Hash eventType);

	} // namespace SCRIPT

	namespace SECURITY {

		/**
		 * Registers a protected variable that will be checked for modifications by the anticheat
		 */
		void REGISTER_SCRIPT_VARIABLE(Any* variable);
		void UNREGISTER_SCRIPT_VARIABLE(Any* variable);
		void FORCE_CHECK_SCRIPT_VARIABLES();

	} // namespace SECURITY

	namespace SHAPETEST {

		/**
		 * Asynchronously starts a line-of-sight (raycast) world probe shape test.
		 * 
		 * Use the handle with 0x3D87450E15D98694 or 0x65287525D951F6BE until it returns 0 or 2.
		 * 
		 * p8 is a bit mask with bits 1, 2 and/or 4, relating to collider types; 4 should usually be used.
		 */
		int START_SHAPE_TEST_LOS_PROBE(float x1, float y1, float z1, float x2, float y2, float z2, int flags, Entity entity, int p8);
		/**
		 * Does the same as 0x7EE9F5D83DD4F90E, except blocking until the shape test completes.
		 */
		int START_EXPENSIVE_SYNCHRONOUS_SHAPE_TEST_LOS_PROBE(float x1, float y1, float z1, float x2, float y2, float z2, int flags, Entity entity, int p8);
		int START_SHAPE_TEST_BOUNDING_BOX(Entity entity, int flags1, int flags2);
		int START_SHAPE_TEST_BOX(float x, float y, float z, float dimX, float dimY, float dimZ, float rotX, float rotY, float rotZ, Any p9, int flags, Entity entity, Any p12);
		int START_SHAPE_TEST_BOUND(Entity entity, int flags1, int flags2);
		/**
		 * Raycast from point to point, where the ray has a radius.
		 * 
		 * flags:
		 * vehicles=10
		 * peds =12
		 * 
		 * Iterating through flags yields many ped / vehicle/ object combinations
		 * 
		 * p9 = 7, but no idea what it does
		 * 
		 * Entity is an entity to ignore
		 */
		int START_SHAPE_TEST_CAPSULE(float x1, float y1, float z1, float x2, float y2, float z2, float radius, int flags, Entity entity, int p9);
		int START_SHAPE_TEST_SWEPT_SPHERE(float x1, float y1, float z1, float x2, float y2, float z2, float radius, int flags, Entity entity, Any p9);
		/**
		 * Returns a ShapeTest handle that can be used with GET_SHAPE_TEST_RESULT.
		 * 
		 * In its only usage in game scripts its called with flag set to 511, entity to player_ped_id and flag2 set to 7
		 */
		int START_SHAPE_TEST_MOUSE_CURSOR_LOS_PROBE(Vector3* pVec1, Vector3* pVec2, int flag, Entity entity, int flag2);
		/**
		 * Returns the result of a shape test: 0 if the handle is invalid, 1 if the shape test is still pending, or 2 if the shape test has completed, and the handle should be invalidated.
		 * 
		 * When used with an asynchronous shape test, this native should be looped until returning 0 or 2, after which the handle is invalidated.
		 */
		int GET_SHAPE_TEST_RESULT(int shapeTestHandle, BOOL* hit, Vector3* endCoords, Vector3* surfaceNormal, Entity* entityHit);
		/**
		 * Returns the result of a shape test, also returning the material of any touched surface.
		 * 
		 * When used with an asynchronous shape test, this native should be looped until returning 0 or 2, after which the handle is invalidated.
		 * 
		 * Unless the return value is 2, the other return values are undefined.
		 */
		int GET_SHAPE_TEST_RESULT_INCLUDING_MATERIAL(int shapeTestHandle, BOOL* hit, Vector3* endCoords, Vector3* surfaceNormal, Hash* materialHash, Entity* entityHit);
		/**
		 * Invalidates the entity handle passed by removing the fwScriptGuid from the entity. This should be used when receiving an ambient entity from shape testing natives, but can also be used for other natives returning an 'irrelevant' entity handle.
		 */
		void RELEASE_SCRIPT_GUID_FROM_ENTITY(Entity entityHit);

	} // namespace SHAPETEST

	namespace SOCIALCLUB {

		int SC_INBOX_GET_TOTAL_NUM_MESSAGES();
		Hash SC_INBOX_GET_MESSAGE_TYPE_AT_INDEX(int msgIndex);
		BOOL SC_INBOX_GET_MESSAGE_IS_READ_AT_INDEX(int msgIndex);
		BOOL SC_INBOX_SET_MESSAGE_AS_READ_AT_INDEX(int msgIndex);
		BOOL SC_INBOX_MESSAGE_GET_DATA_INT(int p0, const char* context, int* out);
		BOOL SC_INBOX_MESSAGE_GET_DATA_BOOL(int p0, const char* p1);
		BOOL SC_INBOX_MESSAGE_GET_DATA_STRING(int p0, const char* context, char* out);
		BOOL SC_INBOX_MESSAGE_DO_APPLY(int p0);
		const char* SC_INBOX_MESSAGE_GET_RAW_TYPE_AT_INDEX(int p0);
		void SC_INBOX_MESSAGE_PUSH_GAMER_T0_RECIP_LIST(Any* gamerHandle);
		void SC_INBOX_SEND_UGCSTATUPDATE_TO_RECIP_LIST(Any* data);
		BOOL SC_INBOX_MESSAGE_GET_UGCDATA(int p0, Any* p1);
		BOOL SC_INBOX_GET_BOUNTY_DATA_AT_INDEX(int index, Any* outData);
		void SC_EMAIL_RETRIEVE_EMAILS(int offset, int limit);
		int SC_EMAIL_GET_RETRIEVAL_STATUS();
		int SC_EMAIL_GET_NUM_RETRIEVED_EMAILS();
		BOOL SC_EMAIL_GET_EMAIL_AT_INDEX(int p0, Any* p1);
		void _SC_EMAIL_MARKETING_EMAIL_OPENED(int index, Hash type);
		void SC_EMAIL_DELETE_EMAILS(Any* p0, Any p1);
		void SC_EMAIL_MESSAGE_PUSH_GAMER_TO_RECIP_LIST(Any* gamerHandle);
		void SC_EMAIL_MESSAGE_CLEAR_RECIP_LIST();
		void SC_EMAIL_SEND_EMAIL(const char* p0);
		BOOL SC_EMAIL_SET_CURRENT_EMAIL_TAG(Any p0);
		void SC_CACHE_NEW_ROCKSTAR_MSGS(BOOL toggle);
		BOOL SC_HAS_NEW_ROCKSTAR_MSG();
		const char* SC_GET_NEW_ROCKSTAR_MSG();
		BOOL SC_PRESENCE_ATTR_SET_INT(Hash attrHash, int value);
		BOOL SC_PRESENCE_ATTR_SET_FLOAT(Hash attrHash, float value);
		BOOL SC_PRESENCE_ATTR_SET_STRING(Hash attrHash, const char* value);
		BOOL SC_PRESENCE_SET_ACTIVITY_RATING(Any p0, float p1);
		BOOL SC_GAMERDATA_GET_INT(const char* name, int* value);
		BOOL SC_GAMERDATA_GET_FLOAT(const char* name, float* value);
		BOOL SC_GAMERDATA_GET_BOOL(const char* name);
		BOOL SC_GAMERDATA_GET_STRING(const char* name, char* value);
		BOOL SC_GAMERDATA_GET_ACTIVE_XP_BONUS(float* value);
		/**
		 * Starts a task to check an entered string for profanity on the ROS/Social Club services.
		 * 
		 * See also: 1753344C770358AE, 82E4A58BABC15AE7.
		 */
		BOOL SC_PROFANITY_CHECK_STRING(const char* string, int* token);
		BOOL SC_PROFANITY_CHECK_STRING_UGC(const char* string, int* token);
		BOOL SC_PROFANITY_GET_CHECK_IS_VALID(int token);
		BOOL SC_PROFANITY_GET_CHECK_IS_PENDING(int token);
		BOOL SC_PROFANITY_GET_STRING_PASSED(int token);
		int SC_PROFANITY_GET_STRING_STATUS(int token);
		/**
		 * Retrieves the profane part of the string with max size of 128.
		 * Returns true if profane term was retrieved.
		 */
		BOOL SC_PROFANITY_GET_PROFANE_WORD(int token, char* outProfaneWord);
		BOOL SC_LICENSEPLATE_CHECK_STRING(const char* p0, int* p1);
		BOOL SC_LICENSEPLATE_GET_CHECK_IS_VALID(Any p0);
		BOOL SC_LICENSEPLATE_GET_CHECK_IS_PENDING(Any p0);
		int SC_LICENSEPLATE_GET_COUNT(int token);
		const char* SC_LICENSEPLATE_GET_PLATE(int token, int plateIndex);
		const char* SC_LICENSEPLATE_GET_PLATE_DATA(int token, int plateIndex);
		BOOL SC_LICENSEPLATE_SET_PLATE_DATA(const char* oldPlateText, const char* newPlateText, Any* plateData);
		BOOL SC_LICENSEPLATE_ADD(const char* plateText, Any* plateData, int* token);
		BOOL SC_LICENSEPLATE_GET_ADD_IS_PENDING(int token);
		int SC_LICENSEPLATE_GET_ADD_STATUS(int token);
		BOOL SC_LICENSEPLATE_ISVALID(const char* plateText, int* token);
		BOOL SC_LICENSEPLATE_GET_ISVALID_IS_PENDING(int token);
		int SC_LICENSEPLATE_GET_ISVALID_STATUS(int token);
		BOOL SC_COMMUNITY_EVENT_IS_ACTIVE();
		int SC_COMMUNITY_EVENT_GET_EVENT_ID();
		BOOL SC_COMMUNITY_EVENT_GET_EXTRA_DATA_INT(const char* p0, int* p1);
		BOOL SC_COMMUNITY_EVENT_GET_EXTRA_DATA_FLOAT(const char* p0, float* p1);
		BOOL SC_COMMUNITY_EVENT_GET_EXTRA_DATA_STRING(const char* p0, char* p1);
		BOOL SC_COMMUNITY_EVENT_GET_DISPLAY_NAME(char* p0);
		BOOL SC_COMMUNITY_EVENT_IS_ACTIVE_FOR_TYPE(const char* p0);
		int SC_COMMUNITY_EVENT_GET_EVENT_ID_FOR_TYPE(const char* p0);
		BOOL SC_COMMUNITY_EVENT_GET_EXTRA_DATA_INT_FOR_TYPE(const char* p0, int* p1, const char* p2);
		BOOL SC_COMMUNITY_EVENT_GET_EXTRA_DATA_FLOAT_FOR_TYPE(const char* p0, float* p1, const char* p2);
		BOOL SC_COMMUNITY_EVENT_GET_EXTRA_DATA_STRING_FOR_TYPE(const char* p0, char* p1, const char* p2);
		BOOL SC_COMMUNITY_EVENT_GET_DISPLAY_NAME_FOR_TYPE(char* p0, const char* p1);
		BOOL SC_COMMUNITY_EVENT_IS_ACTIVE_BY_ID(int p0);
		BOOL SC_COMMUNITY_EVENT_GET_EXTRA_DATA_INT_BY_ID(int p0, const char* p1, int* p2);
		BOOL SC_COMMUNITY_EVENT_GET_EXTRA_DATA_FLOAT_BY_ID(int p0, const char* p1, float* p2);
		BOOL SC_COMMUNITY_EVENT_GET_EXTRA_DATA_STRING_BY_ID(int p0, const char* p1, char* p2);
		BOOL SC_COMMUNITY_EVENT_GET_DISPLAY_NAME_BY_ID(int p0, char* p1);
		BOOL SC_TRANSITION_NEWS_SHOW(Any p0);
		BOOL SC_TRANSITION_NEWS_SHOW_TIMED(Any p0, Any p1);
		BOOL SC_TRANSITION_NEWS_SHOW_NEXT_ITEM();
		BOOL SC_TRANSITION_NEWS_HAS_EXTRA_DATA_TU();
		BOOL SC_TRANSITION_NEWS_GET_EXTRA_DATA_INT_TU(const char* p0, int* p1);
		void SC_TRANSITION_NEWS_END();
		void SC_PAUSE_NEWS_INIT_STORY_TYPE(int storyType, BOOL textOnly); // Missing in crossmap
		/**
		 * Fills some 0x30 sized struct
		 */
		BOOL SC_PAUSE_NEWS_GET_PENDING_STORY(Any p0);
		void SC_PAUSE_NEWS_SHUTDOWN();
		/**
		 * Returns the nickname of the logged-in Rockstar Social Club account.
		 */
		const char* SC_ACCOUNT_INFO_GET_NICKNAME();
		BOOL SC_ACHIEVEMENT_INFO_STATUS(int* p0);
		/**
		 * Same as HAS_ACHIEVEMENT_BEEN_PASSED
		 */
		BOOL SC_HAS_ACHIEVEMENT_BEEN_PASSED(int achievementId);

	} // namespace SOCIALCLUB

	namespace STATS {

		/**
		 * Example:
		 * 
		 * for (v_2 = 0; v_2 <= 4; v_2 += 1) {
		 * STATS::STAT_CLEAR_SLOT_FOR_RELOAD(v_2);
		 * }
		 */
		BOOL STAT_CLEAR_SLOT_FOR_RELOAD(int statSlot);
		BOOL STAT_LOAD(int statSlot);
		BOOL STAT_SAVE(int p0, BOOL p1, int p2, BOOL p3);
		void STAT_SET_OPEN_SAVETYPE_IN_JOB(int p0);
		BOOL STAT_LOAD_PENDING(int statSlot);
		BOOL STAT_SAVE_PENDING();
		BOOL STAT_SAVE_PENDING_OR_REQUESTED();
		/**
		 * p0 is characterSlot? seems range from 0 to 2
		 */
		BOOL STAT_DELETE_SLOT(int p0);
		BOOL STAT_SLOT_IS_LOADED(int statSlot);
		BOOL STAT_CLOUD_SLOT_LOAD_FAILED(int p0);
		int STAT_CLOUD_SLOT_LOAD_FAILED_CODE(Any p0);
		void STAT_SET_BLOCK_SAVES(BOOL toggle);
		BOOL STAT_GET_BLOCK_SAVES();
		BOOL STAT_CLOUD_SLOT_SAVE_FAILED(Any p0);
		void STAT_CLEAR_PENDING_SAVES(Any p0);
		BOOL STAT_LOAD_DIRTY_READ_DETECTED();
		void STAT_CLEAR_DIRTY_READ_DETECTED();
		BOOL STAT_GET_LOAD_SAFE_TO_PROGRESS_TO_MP_FROM_SP();
		/**
		 * Returns stat hash based on dataType, statIndex/statId and characterSlot. Related to CStatsMpCharacterMappingData
		 */
		Hash _GET_STAT_HASH_FOR_CHARACTER_STAT(int dataType, int statIndex, int charSlot);
		/**
		 * Example:
		 * STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MPPLY_KILLS_PLAYERS"), 1337, true);
		 */
		BOOL STAT_SET_INT(Hash statName, int value, BOOL save);
		/**
		 * Example:
		 * STATS::STAT_SET_FLOAT(MISC::GET_HASH_KEY("MP0_WEAPON_ACCURACY"), 66.6f, true);
		 */
		BOOL STAT_SET_FLOAT(Hash statName, float value, BOOL save);
		/**
		 * Example:
		 * STATS::STAT_SET_BOOL(MISC::GET_HASH_KEY("MPPLY_MELEECHLENGECOMPLETED"), trur, true);
		 */
		BOOL STAT_SET_BOOL(Hash statName, BOOL value, BOOL save);
		/**
		 * The following values have been found in the decompiled scripts:
		 * "RC_ABI1"
		 * "RC_ABI2"
		 * "RC_BA1"
		 * "RC_BA2"
		 * "RC_BA3"
		 * "RC_BA3A"
		 * "RC_BA3C"
		 * "RC_BA4"
		 * "RC_DRE1"
		 * "RC_EPS1"
		 * "RC_EPS2"
		 * "RC_EPS3"
		 * "RC_EPS4"
		 * "RC_EPS5"
		 * "RC_EPS6"
		 * "RC_EPS7"
		 * "RC_EPS8"
		 * "RC_EXT1"
		 * "RC_EXT2"
		 * "RC_EXT3"
		 * "RC_EXT4"
		 * "RC_FAN1"
		 * "RC_FAN2"
		 * "RC_FAN3"
		 * "RC_HAO1"
		 * "RC_HUN1"
		 * "RC_HUN2"
		 * "RC_JOS1"
		 * "RC_JOS2"
		 * "RC_JOS3"
		 * "RC_JOS4"
		 * "RC_MAU1"
		 * "RC_MIN1"
		 * "RC_MIN2"
		 * "RC_MIN3"
		 * "RC_MRS1"
		 * "RC_MRS2"
		 * "RC_NI1"
		 * "RC_NI1A"
		 * "RC_NI1B"
		 * "RC_NI1C"
		 * "RC_NI1D"
		 * "RC_NI2"
		 * "RC_NI3"
		 * "RC_OME1"
		 * "RC_OME2"
		 * "RC_PA1"
		 * "RC_PA2"
		 * "RC_PA3"
		 * "RC_PA3A"
		 * "RC_PA3B"
		 * "RC_PA4"
		 * "RC_RAM1"
		 * "RC_RAM2"
		 * "RC_RAM3"
		 * "RC_RAM4"
		 * "RC_RAM5"
		 * "RC_SAS1"
		 * "RC_TON1"
		 * "RC_TON2"
		 * "RC_TON3"
		 * "RC_TON4"
		 * "RC_TON5"
		 */
		BOOL STAT_SET_GXT_LABEL(Hash statName, const char* value, BOOL save);
		/**
		 * 'value' is a structure to a structure, 'numFields' is how many fields there are in said structure (usually 7).
		 * 
		 * The structure looks like this:
		 * 
		 * int year
		 * int month
		 * int day
		 * int hour
		 * int minute
		 * int second
		 * int millisecond
		 * 
		 * The decompiled scripts use TIME::GET_POSIX_TIME to fill this structure.
		 */
		BOOL STAT_SET_DATE(Hash statName, Any* value, int numFields, BOOL save);
		BOOL STAT_SET_STRING(Hash statName, const char* value, BOOL save);
		BOOL STAT_SET_POS(Hash statName, float x, float y, float z, BOOL save);
		BOOL STAT_SET_MASKED_INT(Hash statName, int p1, int p2, int p3, BOOL save);
		BOOL STAT_SET_USER_ID(Hash statName, const char* value, BOOL save);
		/**
		 * p1 always true.
		 */
		BOOL STAT_SET_CURRENT_POSIX_TIME(Hash statName, BOOL p1);
		/**
		 * p2 appears to always be -1
		 * 
		 */
		BOOL STAT_GET_INT(Hash statHash, int* outValue, int p2);
		BOOL STAT_GET_FLOAT(Hash statHash, float* outValue, Any p2);
		BOOL STAT_GET_BOOL(Hash statHash, BOOL* outValue, Any p2);
		/**
		 * p3 is probably characterSlot or BOOL save, always -1
		 */
		BOOL STAT_GET_DATE(Hash statHash, Any* outValue, int numFields, Any p3);
		/**
		 * p1 is always -1 in the script files
		 */
		const char* STAT_GET_STRING(Hash statHash, int p1);
		/**
		 * p3 is probably characterSlot or BOOL save, always -1
		 */
		BOOL STAT_GET_POS(Hash statName, float* outX, float* outY, float* outZ, Any p4);
		/**
		 * p4 is probably characterSlot or BOOL save
		 */
		BOOL STAT_GET_MASKED_INT(Hash statHash, int* outValue, int p2, int p3, Any p4);
		/**
		 * Returns the rockstar ID (user id) value of a given stat. Returns "STAT_UNKNOWN" if the statHash is invalid or the stat has no userId
		 */
		const char* STAT_GET_USER_ID(Hash statHash);
		const char* STAT_GET_LICENSE_PLATE(Hash statName);
		BOOL STAT_SET_LICENSE_PLATE(Hash statName, const char* str);
		void STAT_INCREMENT(Hash statName, float value);
		BOOL STAT_COMMUNITY_START_SYNCH();
		BOOL STAT_COMMUNITY_SYNCH_IS_PENDING();
		BOOL STAT_COMMUNITY_GET_HISTORY(Hash statName, int p1, float* outValue);
		/**
		 * p0 seems to range from 0 to 7
		 */
		void STAT_RESET_ALL_ONLINE_CHARACTER_STATS(int p0);
		/**
		 * p0 seems to range from 0 to 7
		 */
		void STAT_LOCAL_RESET_ALL_ONLINE_CHARACTER_STATS(int p0);
		int STAT_GET_NUMBER_OF_DAYS(Hash statName);
		int STAT_GET_NUMBER_OF_HOURS(Hash statName);
		int STAT_GET_NUMBER_OF_MINUTES(Hash statName);
		int STAT_GET_NUMBER_OF_SECONDS(Hash statName);
		/**
		 * Does not take effect immediately, unfortunately.
		 * 
		 * profileSetting seems to only be 936, 937 and 938 in scripts
		 */
		void STAT_SET_PROFILE_SETTING_VALUE(int profileSetting, int value);
		void STATS_CHARACTER_CREATION_OUTFIT_SELECTED(int outfit); // Missing in crossmap
		/**
		 * This native does absolutely nothing, just a nullsub
		 */
		void STATS_COMPLETED_CHARACTER_CREATION(Any p0);
		/**
		 * Needs more research. Possibly used to calculate the "mask" when calling "STAT_SET_MASKED_INT"?
		 */
		int PACKED_STAT_GET_INT_STAT_INDEX(int p0);
		Hash GET_PACKED_INT_STAT_KEY(int index, BOOL spStat, BOOL charStat, int character);
		Hash GET_PACKED_TU_INT_STAT_KEY(int index, BOOL spStat, BOOL charStat, int character);
		/**
		 * Needs more research. Gets the stat name of a masked int?
		 * 
		 * section - values used in the decompiled scripts:
		 * "_NGPSTAT_INT"
		 * "_MP_NGPSTAT_INT"
		 * "_MP_LRPSTAT_INT"
		 * "_MP_APAPSTAT_INT"
		 * "_MP_LR2PSTAT_INT"
		 * "_MP_BIKEPSTAT_INT"
		 * "_MP_IMPEXPPSTAT_INT"
		 * "_MP_GUNRPSTAT_INT"
		 * "_NGDLCPSTAT_INT"
		 * "_MP_NGDLCPSTAT_INT"
		 * "_DLCSMUGCHARPSTAT_INT"
		 * "_GANGOPSPSTAT_INT"
		 * "_BUSINESSBATPSTAT_INT"
		 * "_ARENAWARSPSTAT_INT"
		 * "_CASINOPSTAT_INT"
		 * "_CASINOHSTPSTAT_INT"
		 */
		Hash GET_PACKED_NG_INT_STAT_KEY(int index, BOOL spStat, BOOL charStat, int character, const char* section);
		BOOL GET_PACKED_STAT_BOOL_CODE(int index, int characterSlot);
		int GET_PACKED_STAT_INT_CODE(int index, int characterSlot);
		void SET_PACKED_STAT_BOOL_CODE(int index, BOOL value, int characterSlot);
		void SET_PACKED_STAT_INT_CODE(int index, int value, int characterSlot);
		void PLAYSTATS_BACKGROUND_SCRIPT_ACTION(const char* action, int value);
		void _PLAYSTATS_FLOW_LOW(float posX, float posY, float posZ, const char* action, BOOL p4, int p5);
		void _PLAYSTATS_FLOW_MEDIUM(float posX, float posY, float posZ, const char* action, BOOL p4, int p5);
		void _PLAYSTATS_FLOW_HIGH(float posX, float posY, float posZ, const char* action, BOOL p4, int p5); // Missing in crossmap
		void PLAYSTATS_NPC_INVITE(const char* p0);
		void PLAYSTATS_AWARD_XP(int amount, Hash type, Hash category);
		void PLAYSTATS_RANK_UP(int rank);
		void PLAYSTATS_STARTED_SESSION_IN_OFFLINEMODE();
		void PLAYSTATS_ACTIVITY_DONE(int p0, int activityId, Any p2);
		void PLAYSTATS_LEAVE_JOB_CHAIN(Any p0, Any p1, Any p2, Any p3, Any p4);
		void PLAYSTATS_MISSION_STARTED(const char* p0, Any p1, Any p2, BOOL p3);
		void PLAYSTATS_MISSION_OVER(const char* p0, Any p1, Any p2, BOOL p3, BOOL p4, BOOL p5);
		void PLAYSTATS_MISSION_CHECKPOINT(const char* p0, Any p1, Any p2, Any p3);
		void PLAYSTATS_RANDOM_MISSION_DONE(const char* name, Any p1, Any p2, Any p3);
		void PLAYSTATS_ROS_BET(int amount, int act, Player player, float cm);
		void PLAYSTATS_RACE_CHECKPOINT(Vehicle p0, Any p1, int p2, int p3, Any p4);
		BOOL PLAYSTATS_CREATE_MATCH_HISTORY_ID_2(int* playerAccountId, int* posixTime);
		void PLAYSTATS_MATCH_STARTED(Any p0, Any p1, Any p2);
		void PLAYSTATS_SHOP_ITEM(Any p0, Any p1, Any p2, Any p3, Any p4);
		void PLAYSTATS_CRATE_DROP_MISSION_DONE(Any p0, Any p1, Any p2, Any p3, Any p4, Any p5, Any p6, Any p7);
		void PLAYSTATS_CRATE_CREATED(float p0, float p1, float p2);
		void PLAYSTATS_HOLD_UP_MISSION_DONE(Any p0, Any p1, Any p2, Any p3);
		void PLAYSTATS_IMPORT_EXPORT_MISSION_DONE(Any p0, Any p1, Any p2, Any p3);
		void PLAYSTATS_RACE_TO_POINT_MISSION_DONE(int p0, Any p1, Any p2, Any p3);
		void PLAYSTATS_ACQUIRED_HIDDEN_PACKAGE(Any p0);
		void PLAYSTATS_WEBSITE_VISITED(Hash scaleformHash, int p1);
		void PLAYSTATS_FRIEND_ACTIVITY(int p0, BOOL p1);
		/**
		 * This native does absolutely nothing, just a nullsub
		 */
		void PLAYSTATS_ODDJOB_DONE(int totalTimeMs, int p1, BOOL p2);
		void PLAYSTATS_PROP_CHANGE(Ped p0, int p1, int p2, int p3);
		void PLAYSTATS_CLOTH_CHANGE(Ped p0, Any p1, Any p2, Any p3, Any p4);
		/**
		 * This is a typo made by R*. It's supposed to be called PLAYSTATS_WEAPON_MOD_CHANGE.
		 */
		void PLAYSTATS_WEAPON_MODE_CHANGE(Hash weaponHash, Hash componentHashTo, Hash componentHashFrom);
		void PLAYSTATS_CHEAT_APPLIED(const char* cheat);
		void PLAYSTATS_JOB_ACTIVITY_END(Any* p0, Any* p1, Any* p2, Any* p3);
		void PLAYSTATS_JOB_BEND(Any* p0, Any* p1, Any* p2, Any* p3);
		void PLAYSTATS_JOB_LTS_END(Any* p0, Any* p1, Any* p2, Any* p3);
		void PLAYSTATS_JOB_LTS_ROUND_END(Any* p0, Any* p1, Any* p2, Any* p3);
		void PLAYSTATS_QUICKFIX_TOOL(int element, const char* item);
		void PLAYSTATS_IDLE_KICK(int msStoodIdle);
		void PLAYSTATS_SET_JOIN_TYPE(int joinType);
		void PLAYSTATS_HEIST_SAVE_CHEAT(Hash hash, int p1);
		void PLAYSTATS_APPEND_DIRECTOR_METRIC(Any* p0);
		void PLAYSTATS_AWARD_BAD_SPORT(int id);
		void PLAYSTATS_PEGASUS_AS_PERSONAL_AIRCRAFT(Hash modelHash);
		void _PLAYSTATS_SHOPMENU_NAV(Any p0, Any p1, Any p2, Any p3);
		void PLAYSTATS_FM_EVENT_CHALLENGES(Any p0);
		void PLAYSTATS_FM_EVENT_VEHICLETARGET(Any p0);
		void PLAYSTATS_FM_EVENT_URBANWARFARE(Any p0);
		void PLAYSTATS_FM_EVENT_CHECKPOINTCOLLECTION(Any p0);
		void PLAYSTATS_FM_EVENT_ATOB(Any p0);
		void PLAYSTATS_FM_EVENT_PENNEDIN(Any p0);
		void PLAYSTATS_FM_EVENT_PASSTHEPARCEL(Any p0);
		void PLAYSTATS_FM_EVENT_HOTPROPERTY(Any p0);
		void PLAYSTATS_FM_EVENT_DEADDROP(Any p0);
		void PLAYSTATS_FM_EVENT_KINGOFTHECASTLE(Any p0);
		void PLAYSTATS_FM_EVENT_CRIMINALDAMAGE(Any p0);
		void PLAYSTATS_FM_EVENT_COMPETITIVEURBANWARFARE(Any p0);
		void PLAYSTATS_FM_EVENT_HUNTBEAST(Any p0);
		void PLAYSTATS_PIMENU_HIDE_OPTIONS(Any* data);
		/**
		 * Known text types: NAME_YACHT, NAME_ORG_OFFICE
		 */
		void _PLAYSTATS_NAMED_USER_CONTENT(BOOL isBoss, int bossType, int bossId1, int bossId2, int textType, const char* textString, int textSource);
		int LEADERBOARDS_GET_NUMBER_OF_COLUMNS(int p0, Any p1);
		int LEADERBOARDS_GET_COLUMN_ID(int p0, int p1, int p2);
		int LEADERBOARDS_GET_COLUMN_TYPE(int p0, Any p1, Any p2);
		int LEADERBOARDS_READ_CLEAR_ALL();
		int LEADERBOARDS_READ_CLEAR(Any p0, Any p1, Any p2);
		BOOL LEADERBOARDS_READ_PENDING(Any p0, Any p1, Any p2);
		BOOL LEADERBOARDS_READ_ANY_PENDING();
		BOOL LEADERBOARDS_READ_SUCCESSFUL(Any p0, Any p1, Any p2);
		BOOL LEADERBOARDS2_READ_FRIENDS_BY_ROW(Any* p0, Any* p1, Any p2, BOOL p3, Any p4, Any p5);
		BOOL LEADERBOARDS2_READ_BY_HANDLE(Any* p0, Any* p1);
		BOOL LEADERBOARDS2_READ_BY_RANK(Any* p0, Any p1, Any p2);
		BOOL LEADERBOARDS2_READ_BY_RADIUS(Any* p0, Any p1, Any* p2);
		BOOL LEADERBOARDS2_READ_BY_SCORE_INT(Any* p0, Any p1, Any p2);
		BOOL LEADERBOARDS2_READ_BY_SCORE_FLOAT(Any* p0, float p1, Any p2);
		BOOL LEADERBOARDS2_READ_RANK_PREDICTION(Any* p0, Any* p1, Any* p2);
		BOOL LEADERBOARDS2_READ_BY_PLAFORM(Any* p0, const char* gamerHandleCsv, const char* platformName);
		BOOL LEADERBOARDS2_READ_GET_ROW_DATA_START(Any* p0);
		void LEADERBOARDS2_READ_GET_ROW_DATA_END();
		BOOL LEADERBOARDS2_READ_GET_ROW_DATA_INFO(Any p0, Any* p1);
		int LEADERBOARDS2_READ_GET_ROW_DATA_INT(Any p0, Any p1);
		float LEADERBOARDS2_READ_GET_ROW_DATA_FLOAT(Any p0, Any p1);
		BOOL LEADERBOARDS2_WRITE_DATA(Any* p0);
		void LEADERBOARDS_WRITE_ADD_COLUMN(Any p0, Any p1, float p2);
		void LEADERBOARDS_WRITE_ADD_COLUMN_LONG(Any p0, Any p1, Any p2);
		BOOL LEADERBOARDS_CACHE_DATA_ROW(Any* p0);
		void LEADERBOARDS_CLEAR_CACHE_DATA();
		void LEADERBOARDS_CLEAR_CACHE_DATA_ID(Any p0);
		BOOL LEADERBOARDS_GET_CACHE_EXISTS(Any p0);
		int LEADERBOARDS_GET_CACHE_TIME(Any p0);
		int LEADERBOARDS_GET_CACHE_NUMBER_OF_ROWS(Any p0);
		BOOL LEADERBOARDS_GET_CACHE_DATA_ROW(Any p0, Any p1, Any* p2);
		void PRESENCE_EVENT_UPDATESTAT_INT(Hash statHash, int value, int p2);
		void PRESENCE_EVENT_UPDATESTAT_FLOAT(Hash statHash, float value, int p2);
		void PRESENCE_EVENT_UPDATESTAT_INT_WITH_STRING(Hash statHash, int value, int p2, const char* string);
		BOOL GET_PLAYER_HAS_DRIVEN_ALL_VEHICLES();
		void SET_HAS_POSTED_ALL_VEHICLES_DRIVEN();
		void SET_PROFILE_SETTING_PROLOGUE_COMPLETE();
		/**
		 * Sets profile setting 939
		 */
		void SET_PROFILE_SETTING_SP_CHOP_MISSION_COMPLETE();
		/**
		 * Sets profile setting 933
		 */
		void SET_PROFILE_SETTING_CREATOR_RACES_DONE(int value);
		/**
		 * Sets profile setting 934
		 */
		void SET_PROFILE_SETTING_CREATOR_DM_DONE(int value);
		/**
		 * Sets profile setting 935
		 */
		void SET_PROFILE_SETTING_CREATOR_CTF_DONE(int value);
		void SET_JOB_ACTIVITY_ID_STARTED(Any p0, int characterSlot);
		void SET_FREEMODE_PROLOGUE_DONE(Any p0, int characterSlot);
		/**
		 * Sets profile setting 940 and 941
		 */
		void SET_FREEMODE_STRAND_PROGRESSION_STATUS(int profileSetting, int settingValue);
		void STAT_NETWORK_INCREMENT_ON_SUICIDE(Any p0, float p1);
		void STAT_SET_CHEAT_IS_ACTIVE();
		BOOL LEADERBOARDS2_WRITE_DATA_FOR_EVENT_TYPE(Any* p0, Any* p1);
		void FORCE_CLOUD_MP_STATS_DOWNLOAD_AND_OVERWRITE_LOCAL_SAVE();
		void STAT_MIGRATE_CLEAR_FOR_RESTART();
		/**
		 * platformName must be one of the following: ps3, xbox360, ps4, xboxone
		 */
		BOOL STAT_MIGRATE_SAVEGAME_START(const char* platformName);
		int STAT_MIGRATE_SAVEGAME_GET_STATUS();
		BOOL STAT_MIGRATE_CHECK_ALREADY_DONE();
		BOOL STAT_MIGRATE_CHECK_START();
		int STAT_MIGRATE_CHECK_GET_IS_PLATFORM_AVAILABLE(int p0);
		int STAT_MIGRATE_CHECK_GET_PLATFORM_STATUS(int p0, Any* p1);
		int STAT_GET_SAVE_MIGRATION_STATUS(Any* data);
		BOOL STAT_SAVE_MIGRATION_CANCEL_PENDING_OPERATION();
		int STAT_GET_CANCEL_SAVE_MIGRATION_STATUS();
		BOOL STAT_SAVE_MIGRATION_CONSUME_CONTENT(Hash contentId, const char* srcPlatform, const char* srcGamerHandle);
		int STAT_GET_SAVE_MIGRATION_CONSUME_CONTENT_STATUS(int* p0);
		void STAT_ENABLE_STATS_TRACKING();
		/**
		 * Prevents updates to CStatsMgr (e.g., STAT_SET_* natives)
		 */
		void STAT_DISABLE_STATS_TRACKING();
		BOOL STAT_IS_STATS_TRACKING_ENABLED();
		/**
		 * enum StatTrackingType
		 * {
		 * LongestWheelie = 1,
		 * LongestStoppie = 2,
		 * NoCrashes = 3,
		 * HighestSpeed = 4,
		 * _MostFlips = 5,
		 * _LongestSpin = 6,
		 * _HighestJumpReached = 7,
		 * LongestJump = 8,
		 * _NearMissesNoCrash = 9,
		 * LongestFallSurvived = 10,
		 * LowestParachute = 11,
		 * ReverseDriving = 12,
		 * LongestFreefall = 13,
		 * VehiclesStolen = 14,
		 * _SomeCFireEventCount = 15,
		 * _Unk16 = 16,
		 * _LowFlyingTime = 17,
		 * LowFlying = 18,
		 * _InvertedFlyingTime = 19,
		 * InvertedFlying = 20,
		 * _PlaneSpinCount = 21,
		 * MeleeKills = 22, // Players
		 * _LongestSniperKill = 23,
		 * SniperSkills = 24, // Players
		 * DrivebyKills = 25, // Players
		 * HeadshotKills = 26, // Players
		 * LongestBail = 27,
		 * _TotalRammedByCar = 28,
		 * NearMissesPrecise = 29,
		 * _FreefallTime = 30,
		 * Unk31 = 31,
		 * }
		 * 
		 * enum StatTrackingValueType
		 * {
		 * Total,
		 * Max,
		 * Min
		 * }
		 */
		BOOL STAT_START_RECORD_STAT(int statType, int valueType);
		BOOL STAT_STOP_RECORD_STAT();
		BOOL STAT_GET_RECORDED_VALUE(float* value);
		BOOL STAT_IS_RECORDING_STAT();
		/**
		 * Perform the most near misses with other vehicles in a land vehicle without crashing
		 */
		int STAT_GET_CURRENT_NEAR_MISS_NOCRASH_PRECISE();
		/**
		 * Perform the longest wheelie on a motorcycle
		 */
		float STAT_GET_CURRENT_REAR_WHEEL_DISTANCE();
		/**
		 * Perform the longest stoppie on a motorcycle
		 */
		float STAT_GET_CURRENT_FRONT_WHEEL_DISTANCE();
		/**
		 * Perform the longest jump in a land vehicle
		 */
		float STAT_GET_CURRENT_JUMP_DISTANCE();
		/**
		 * Drive the furthest distance in a land vehicle without crashing
		 */
		float STAT_GET_CURRENT_DRIVE_NOCRASH_DISTANCE();
		/**
		 * Achieve the highest speed in a land vehicle
		 */
		float STAT_GET_CURRENT_SPEED();
		/**
		 * Reverse the longest distance without crashing
		 */
		float STAT_GET_CURRENT_DRIVING_REVERSE_DISTANCE();
		/**
		 * Fall the longest distance with a parachute before opening it
		 */
		float STAT_GET_CURRENT_SKYDIVING_DISTANCE();
		/**
		 * Fly low to the ground for the longest distance
		 */
		float STAT_GET_CHALLENGE_FLYING_DIST();
		BOOL STAT_GET_FLYING_ALTITUDE(float* outValue);
		/**
		 * Or non-flyable area
		 */
		BOOL STAT_IS_PLAYER_VEHICLE_ABOVE_OCEAN();
		/**
		 * Travel the furthest distance when bailing from a vehicle
		 */
		float STAT_GET_VEHICLE_BAIL_DISTANCE();
		/**
		 * This function is hard-coded to always return 1.
		 */
		BOOL STAT_ROLLBACK_SAVE_MIGRATION();
		/**
		 * Sets profile setting 866
		 */
		void SET_HAS_SPECIALEDITION_CONTENT(int value);
		/**
		 * Sets profile setting 501
		 */
		void SET_SAVE_MIGRATION_TRANSACTION_ID_WARNING(int transactionId);
		void GET_BOSS_GOON_UUID(int characterSlot, Any p1, Any p2);
		void PLAYSTATS_BW_BOSSONBOSSDEATHMATCH(Any p0);
		void PLAYSTATS_BW_YATCHATTACK(Any p0);
		void PLAYSTATS_BW_HUNT_THE_BOSS(Any p0);
		void PLAYSTATS_BW_SIGHTSEER(Any p0);
		void PLAYSTATS_BW_ASSAULT(Any p0);
		void PLAYSTATS_BW_BELLY_OF_THE_BEAST(Any p0);
		void PLAYSTATS_BW_HEAD_HUNTER(Any p0);
		void PLAYSTATS_BW_FRAGILE_GOODS(Any p0);
		void PLAYSTATS_BW_AIR_FREIGHT(Any p0);
		void PLAYSTATS_BC_CAR_JACKING(Any p0);
		void PLAYSTATS_BC_SMASH_AND_GRAB(Any p0);
		void PLAYSTATS_BC_PROTECTION_RACKET(Any p0);
		void PLAYSTATS_BC_MOST_WANTED(Any p0);
		void PLAYSTATS_BC_FINDERS_KEEPERS(Any p0);
		void PLAYSTATS_BC_POINT_TO_POINT(Any p0);
		void PLAYSTATS_BC_CASHING(Any p0);
		void PLAYSTATS_BC_SALVAGE(Any p0);
		void PLAYSTATS_SPENT_PI_CUSTOM_LOADOUT(int amount);
		void PLAYSTATS_BUY_CONTRABAND_MISSION(Any* data);
		void PLAYSTATS_SELL_CONTRABAND_MISSION(Any* data);
		void PLAYSTATS_DEFEND_CONTRABAND_MISSION(Any* data);
		void PLAYSTATS_RECOVER_CONTRABAND_MISSION(Any* data);
		void PLAYSTATS_HIT_CONTRABAND_DESTROY_LIMIT(Any p0);
		void START_BEING_BOSS(Any p0, Any p1, Any p2);
		void START_BEING_GOON(Any p0, Any p1, Any p2);
		void END_BEING_BOSS(Any p0, Any p1, Any p2);
		void END_BEING_GOON(Any p0, Any p1, Any p2, Any p3, Any p4);
		void HIRED_LIMO(Any p0, Any p1);
		void ORDER_BOSS_VEHICLE(Any p0, Any p1, Hash vehicleHash);
		void CHANGE_UNIFORM(Any p0, Any p1, Any p2);
		void CHANGE_GOON_LOOKING_FOR_WORK(Any p0);
		void SEND_METRIC_GHOSTING_TO_PLAYER(Any p0);
		void SEND_METRIC_VIP_POACH(Any p0, Any p1, Any p2);
		void SEND_METRIC_PUNISH_BODYGUARD(Any p0);
		/**
		 * Allows CEventNetworkStuntPerformed to be triggered.
		 */
		void PLAYSTATS_START_TRACKING_STUNTS();
		/**
		 * Disallows CEventNetworkStuntPerformed to be triggered.
		 */
		void PLAYSTATS_STOP_TRACKING_STUNTS();
		void PLAYSTATS_MISSION_ENDED(Any p0);
		void PLAYSTATS_IMPEXP_MISSION_ENDED(Any p0);
		void PLAYSTATS_CHANGE_MC_ROLE(Any p0, Any p1, Any p2, Any p3, int role, int p5, Any p6);
		void PLAYSTATS_CHANGE_MC_OUTFIT(Any p0, Any p1, Any p2, Any p3, Any p4);
		void PLAYSTATS_SWITCH_MC_EMBLEM(Any p0, Any p1, Any p2, Any p3, Any p4);
		void PLAYSTATS_MC_REQUEST_BIKE(Any p0, Any p1, Any p2, Any p3, Any p4);
		void PLAYSTATS_MC_KILLED_RIVAL_MC_MEMBER(Any p0, Any p1, Any p2, Any p3, Any p4);
		void PLAYSTATS_ABANDONED_MC(Any p0, Any p1, Any p2, Any p3, Any p4);
		void PLAYSTATS_EARNED_MC_POINTS(Any p0, Any p1, Any p2, Any p3, Any p4, Any p5);
		void PLAYSTATS_MC_FORMATION_ENDS(Any p0, Any p1, Any p2, Any p3, Any p4, Any p5, Any p6);
		void PLAYSTATS_MC_CLUBHOUSE_ACTIVITY(Any p0, Any p1, Any p2, Any p3, Any p4, Any p5, Any p6, Any p7);
		void PLAYSTATS_RIVAL_BEHAVIOR(Any p0, Any p1, Any p2, Any p3, Any p4, Any p5, Any p6, Any p7, Any p8, Any p9);
		void PLAYSTATS_COPY_RANK_INTO_NEW_SLOT(Any p0, Any p1, Any p2, Any p3, Any p4, Any p5, Any p6);
		void PLAYSTATS_DUPE_DETECTED(Any* data);
		void PLAYSTATS_BAN_ALERT(int p0);
		void PLAYSTATS_GUNRUNNING_MISSION_ENDED(Any* data);
		void PLAYSTATS_GUNRUNNING_RND(Any p0);
		void PLAYSTATS_BUSINESS_BATTLE_ENDED(Any p0);
		void PLAYSTATS_WAREHOUSE_MISSION_ENDED(Any p0);
		void PLAYSTATS_NIGHTCLUB_MISSION_ENDED(Any p0);
		void PLAYSTATS_DJ_USAGE(Any p0, Any p1);
		void PLAYSTATS_MINIGAME_USAGE(Any p0, Any p1, Any p2);
		void PLAYSTATS_STONE_HATCHET_ENDED(Any* data);
		void PLAYSTATS_SMUGGLER_MISSION_ENDED(Any* data);
		void PLAYSTATS_FM_HEIST_PREP_ENDED(Any* data);
		void PLAYSTATS_INSTANCED_HEIST_ENDED(Any* data, Any p1, Any p2, Any p3);
		void PLAYSTATS_DAR_CHECKPOINT(Any* data);
		void PLAYSTATS_ENTER_SESSION_PACK(Any* data);
		void PLAYSTATS_DRONE_USAGE(int p0, int p1, int p2);
		void PLAYSTATS_SPIN_WHEEL(int p0, int p1, int p2, int p3);
		void PLAYSTATS_ARENA_WARS_SPECTATOR(int p0, int p1, int p2, int p3, int p4);
		void PLAYSTATS_ARENA_WARS_ENDED(Any* data);
		void PLAYSTATS_SWITCH_PASSIVE_MODE(BOOL p0, int p1, int p2, int p3);
		void PLAYSTATS_COLLECTIBLE_PICKED_UP(int p0, Hash objectHash, Any p2, Any p3, int moneyAmount, int rpAmount, int chipsAmount, Any p7, int p8, Any p9, Any p10);
		void PLAYSTATS_CASINO_STORY_MISSION_ENDED(Any p0, Any p1);
		void PLAYSTATS_CASINO_CHIP(Any p0);
		void PLAYSTATS_CASINO_ROULETTE(Any p0);
		void PLAYSTATS_CASINO_BLACKJACK(Any p0);
		void PLAYSTATS_CASINO_THREE_CARD_POKER(Any p0);
		void PLAYSTATS_CASINO_SLOT_MACHINE(Any p0);
		void PLAYSTATS_CASINO_INSIDE_TRACK(Any p0);
		void PLAYSTATS_CASINO_LUCKY_SEVEN(Any p0);
		void PLAYSTATS_CASINO_ROULETTE_LIGHT(Any p0);
		void PLAYSTATS_CASINO_BLACKJACK_LIGHT(Any p0);
		void PLAYSTATS_CASINO_THREE_CARD_POKER_LIGHT(Any p0);
		void PLAYSTATS_CASINO_SLOT_MACHINE_LIGHT(Any p0);
		void PLAYSTATS_CASINO_INSIDE_TRACK_LIGHT(Any p0);
		void PLAYSTATS_ARCADE_GAME(Any p0, Any p1, Any p2, Any p3, Any p4, Any p5, Any p6);
		void PLAYSTATS_ARCADE_LOVE_MATCH(Any p0, Any p1);
		void PLAYSTATS_FREEMODE_CASINO_MISSION_ENDED(Any* data);
		void PLAYSTATS_HEIST3_DRONE(Any p0);
		void PLAYSTATS_HEIST3_HACK(Any p0, Any p1, Any p2, Any p3, Any p4, Any p5);
		void PLAYSTATS_NPC_PHONE(Any* p0);
		void PLAYSTATS_ARCADE_CABINET(Any p0);
		void PLAYSTATS_HEIST3_FINALE(Any p0);
		void PLAYSTATS_HEIST3_PREP(Any p0);
		void PLAYSTATS_MASTER_CONTROL(Any p0, Any p1, Any p2, Any p3, Any p4);
		void PLAYSTATS_QUIT_MODE(Any p0, Any p1, Any p2, Any p3, Any p4);
		void PLAYSTATS_MISSION_VOTE(Any p0);
		void PLAYSTATS_NJVS_VOTE(Any p0);
		void _PLAYSTATS_DEATH_INFO(Ped victimPed, Ped killerPed, int mentalState, BOOL revengeKill, int victimKvK, int killerKvK); // Missing in crossmap
		void PLAYSTATS_FM_MISSION_END(Any p0, Any p1, Any p2, Any p3);
		void PLAYSTATS_HEIST4_PREP(Any p0);
		void PLAYSTATS_HEIST4_FINALE(Any p0);
		void PLAYSTATS_HEIST4_HACK(Any p0, Any p1, Any p2, Any p3, Any p4);
		void PLAYSTATS_SUB_WEAP(Any p0, Any p1, Any p2, Any p3);
		void PLAYSTATS_FAST_TRVL(Any p0, Any p1, Any p2, Any p3, Any p4, Any p5, Any p6, Any p7, Any p8, Any p9, Any p10);
		void PLAYSTATS_HUB_ENTRY(Any p0);
		void PLAYSTATS_DJ_MISSION_ENDED(Any p0);
		void PLAYSTATS_ROBBERY_PREP(Any p0);
		void PLAYSTATS_ROBBERY_FINALE(Any p0);
		void PLAYSTATS_EXTRA_EVENT(Any p0);
		void PLAYSTATS_CARCLUB_POINTS(Any p0);
		void PLAYSTATS_CARCLUB_CHALLENGE(Any p0, Any p1, Any p2, Any p3);
		void PLAYSTATS_CARCLUB_PRIZE(int p0, Hash vehicleModel);
		void PLAYSTATS_AWARD_NAV(Any p0, Any p1, Any p2, Any p3);
		void _PLAYSTATS_INIT_MULTIPLAYER(Any p0, Any p1, Any p2); // Missing in crossmap
		void PLAYSTATS_INST_MISSION_END(Any p0);
		void PLAYSTATS_HUB_EXIT(Any p0);
		void PLAYSTATS_LP_NAV(Any* data); // Missing in crossmap
		void _PLAYSTATS_LP_SUBTILE(Any p0, Any p1, Any p2, Any p3, Any p4); // Missing in crossmap
		void PLAYSTATS_VEH_DEL(int bossId1, int bossId2, int bossType, int vehicleID, int reason);
		void PLAYSTATS_INVENTORY(Any p0);
		void _PLAYSTATS_ACID_MISSION_END(Any p0);
		void _PLAYSTATS_ACID_RND(Any p0);
		void _PLAYSTATS_IDLE(Any p0, Any p1, Any p2);
		void _PLAYSTATS_PLAYER_STYLE(Any p0);
		void _PLAYSTATS_RANDOM_EVENT(Any p0);
		void _PLAYSTATS_PH_SNAPSHOT(Any p0); // Missing in crossmap
		void _PLAYSTATS_PH_PROGRESS_TIERS(Any p0); // Missing in crossmap
		void _PLAYSTATS_PH_NAV(Any p0); // Missing in crossmap
		void _PLAYSTATS_PH_ACTIVITY(Any p0, Any p1, Any p2, Any p3, Any p4, Any p5, Any p6, Any p7); // Missing in crossmap
		void _PLAYSTATS_PH_PROGRESS_OBJECTIVES(Any p0); // Missing in crossmap
		void _PLAYSTATS_ALERT(Any* data);
		void _PLAYSTATS_ATTRITION_STAGE_END(Any p0);
		void _PLAYSTATS_SHOWROOM_NAV(Any p0, Any p1, Hash entity);
		/**
		 * Data struct contains various tunables related to test drives at Simeons Showroom or Luxury Showcase.
		 */
		void _PLAYSTATS_SHOWROOM_OVERVIEW(Any* data);
		void _PLAYSTATS_PIMENU_NAV(Any* data);
		void _PLAYSTATS_RECOVER_VEHICLE(Any* data);
		void _PLAYSTATS_SCRIPT_EVENT_FPOM(Any* data); // Missing in crossmap
		void _PLAYSTATS_LOBBY_EXIT(Any* p0); // Missing in crossmap
		void _PLAYSTATS_LOBBY_STARTED(Any* p0); // Missing in crossmap
		void _PLAYSTATS_CREATOR_END(Any* p0); // Missing in crossmap

	} // namespace STATS

	namespace STREAMING {

		void LOAD_ALL_OBJECTS_NOW();
		void LOAD_SCENE(float x, float y, float z);
		BOOL NETWORK_UPDATE_LOAD_SCENE();
		BOOL IS_NETWORK_LOADING_SCENE();
		void SET_INTERIOR_ACTIVE(int interiorID, BOOL toggle);
		/**
		 * Request a model to be loaded into memory.
		 * 
		 */
		void REQUEST_MODEL(Hash model);
		void REQUEST_MENU_PED_MODEL(Hash model);
		/**
		 * Checks if the specified model has loaded into memory.
		 */
		BOOL HAS_MODEL_LOADED(Hash model);
		/**
		 * STREAMING::REQUEST_MODELS_IN_ROOM(l_13BC, "V_FIB01_cur_elev");
		 * STREAMING::REQUEST_MODELS_IN_ROOM(l_13BC, "limbo");
		 * STREAMING::REQUEST_MODELS_IN_ROOM(l_13BB, "V_Office_gnd_lifts");
		 * STREAMING::REQUEST_MODELS_IN_ROOM(l_13BB, "limbo");
		 * STREAMING::REQUEST_MODELS_IN_ROOM(l_13BC, "v_fib01_jan_elev");
		 * STREAMING::REQUEST_MODELS_IN_ROOM(l_13BC, "limbo");
		 */
		void REQUEST_MODELS_IN_ROOM(Interior interior, const char* roomName);
		/**
		 * Unloads model from memory
		 */
		void SET_MODEL_AS_NO_LONGER_NEEDED(Hash model);
		/**
		 * Check if model is in cdimage(rpf)
		 */
		BOOL IS_MODEL_IN_CDIMAGE(Hash model);
		/**
		 * Returns whether the specified model exists in the game.
		 */
		BOOL IS_MODEL_VALID(Hash model);
		BOOL IS_MODEL_A_PED(Hash model);
		/**
		 * Returns whether the specified model represents a vehicle.
		 */
		BOOL IS_MODEL_A_VEHICLE(Hash model);
		void REQUEST_COLLISION_AT_COORD(float x, float y, float z);
		void REQUEST_COLLISION_FOR_MODEL(Hash model);
		BOOL HAS_COLLISION_FOR_MODEL_LOADED(Hash model);
		/**
		 * Alias of REQUEST_COLLISION_AT_COORD.
		 */
		void REQUEST_ADDITIONAL_COLLISION_AT_COORD(float x, float y, float z);
		/**
		 * Full list of animation dictionaries and anims by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/animDictsCompact.json
		 */
		BOOL DOES_ANIM_DICT_EXIST(const char* animDict);
		/**
		 * Full list of animation dictionaries and anims by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/animDictsCompact.json
		 */
		void REQUEST_ANIM_DICT(const char* animDict);
		/**
		 * Full list of animation dictionaries and anims by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/animDictsCompact.json
		 */
		BOOL HAS_ANIM_DICT_LOADED(const char* animDict);
		/**
		 * Full list of animation dictionaries and anims by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/animDictsCompact.json
		 */
		void REMOVE_ANIM_DICT(const char* animDict);
		/**
		 * Starts loading the specified animation set. An animation set provides movement animations for a ped. See SET_PED_MOVEMENT_CLIPSET.
		 * 
		 * Full list of animation dictionaries and anims by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/animDictsCompact.json
		 * 
		 * Full list of movement clipsets by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/movementClipsetsCompact.json
		 */
		void REQUEST_ANIM_SET(const char* animSet);
		/**
		 * Gets whether the specified animation set has finished loading. An animation set provides movement animations for a ped. See SET_PED_MOVEMENT_CLIPSET.
		 * 
		 * Animation set and clip set are synonymous.
		 * 
		 * Full list of animation dictionaries and anims by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/animDictsCompact.json
		 * 
		 * Full list of movement clipsets by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/movementClipsetsCompact.json
		 */
		BOOL HAS_ANIM_SET_LOADED(const char* animSet);
		/**
		 * Unloads the specified animation set. An animation set provides movement animations for a ped. See SET_PED_MOVEMENT_CLIPSET.
		 * 
		 * Animation set and clip set are synonymous.
		 * 
		 * Full list of animation dictionaries and anims by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/animDictsCompact.json
		 * 
		 * Full list of movement clipsets by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/movementClipsetsCompact.json
		 */
		void REMOVE_ANIM_SET(const char* animSet);
		/**
		 * Full list of animation dictionaries and anims by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/animDictsCompact.json
		 * 
		 * Full list of movement clipsets by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/movementClipsetsCompact.json
		 */
		void REQUEST_CLIP_SET(const char* clipSet);
		/**
		 * Alias for HAS_ANIM_SET_LOADED.
		 * 
		 * Full list of animation dictionaries and anims by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/animDictsCompact.json
		 * 
		 * Full list of movement clipsets by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/movementClipsetsCompact.json
		 */
		BOOL HAS_CLIP_SET_LOADED(const char* clipSet);
		/**
		 * Alias for REMOVE_ANIM_SET.
		 * 
		 * Full list of animation dictionaries and anims by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/animDictsCompact.json
		 * 
		 * Full list of movement clipsets by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/movementClipsetsCompact.json
		 */
		void REMOVE_CLIP_SET(const char* clipSet);
		/**
		 * Exemple: REQUEST_IPL("TrevorsTrailerTrash");
		 * 
		 * Full list of IPLs and interior entity sets by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/ipls.json
		 */
		void REQUEST_IPL(const char* iplName);
		/**
		 * Removes an IPL from the map.
		 * 
		 * Full list of IPLs and interior entity sets by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/ipls.json
		 * 
		 * Example:
		 * C#:
		 * Function.Call(Hash.REMOVE_IPL, "trevorstrailertidy");
		 * 
		 * C++:
		 * STREAMING::REMOVE_IPL("trevorstrailertidy");
		 * 
		 * iplName = Name of IPL you want to remove.
		 */
		void REMOVE_IPL(const char* iplName);
		/**
		 * Full list of IPLs and interior entity sets by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/ipls.json
		 */
		BOOL IS_IPL_ACTIVE(const char* iplName);
		void SET_STREAMING(BOOL toggle);
		/**
		 * 0 - default
		 * 1 - HeistIsland
		 */
		void LOAD_GLOBAL_WATER_FILE(int waterType);
		int GET_GLOBAL_WATER_FILE();
		void SET_GAME_PAUSES_FOR_STREAMING(BOOL toggle);
		void SET_REDUCE_PED_MODEL_BUDGET(BOOL toggle);
		void SET_REDUCE_VEHICLE_MODEL_BUDGET(BOOL toggle);
		/**
		 * This is a NOP function. It does nothing at all.
		 */
		void SET_DITCH_POLICE_MODELS(BOOL toggle);
		int GET_NUMBER_OF_STREAMING_REQUESTS();
		/**
		 * maps script name (thread + 0xD0) by lookup via scriptfx.dat - does nothing when script name is empty
		 */
		void REQUEST_PTFX_ASSET();
		BOOL HAS_PTFX_ASSET_LOADED();
		void REMOVE_PTFX_ASSET();
		/**
		 * From the b678d decompiled scripts:
		 * 
		 * STREAMING::REQUEST_NAMED_PTFX_ASSET("core_snow");
		 * STREAMING::REQUEST_NAMED_PTFX_ASSET("fm_mission_controler");
		 * STREAMING::REQUEST_NAMED_PTFX_ASSET("proj_xmas_firework");
		 * STREAMING::REQUEST_NAMED_PTFX_ASSET("scr_apartment_mp");
		 * STREAMING::REQUEST_NAMED_PTFX_ASSET("scr_biolab_heist");
		 * STREAMING::REQUEST_NAMED_PTFX_ASSET("scr_indep_fireworks");
		 * STREAMING::REQUEST_NAMED_PTFX_ASSET("scr_indep_parachute");
		 * STREAMING::REQUEST_NAMED_PTFX_ASSET("scr_indep_wheelsmoke");
		 * STREAMING::REQUEST_NAMED_PTFX_ASSET("scr_mp_cig_plane");
		 * STREAMING::REQUEST_NAMED_PTFX_ASSET("scr_mp_creator");
		 * STREAMING::REQUEST_NAMED_PTFX_ASSET("scr_mp_tankbattle");
		 * STREAMING::REQUEST_NAMED_PTFX_ASSET("scr_ornate_heist");
		 * STREAMING::REQUEST_NAMED_PTFX_ASSET("scr_prison_break_heist_station");
		 */
		void REQUEST_NAMED_PTFX_ASSET(const char* fxName);
		BOOL HAS_NAMED_PTFX_ASSET_LOADED(const char* fxName);
		void REMOVE_NAMED_PTFX_ASSET(const char* fxName);
		void SET_VEHICLE_POPULATION_BUDGET(int p0);
		/**
		 * Control how many new (ambient?) peds will spawn in the game world.
		 * Range for p0 seems to be 0-3, where 0 is none and 3 is the normal level.
		 */
		void SET_PED_POPULATION_BUDGET(int p0);
		void CLEAR_FOCUS();
		/**
		 * Override the area where the camera will render the terrain.
		 * p3, p4 and p5 are usually set to 0.0
		 * 
		 */
		void SET_FOCUS_POS_AND_VEL(float x, float y, float z, float offsetX, float offsetY, float offsetZ);
		/**
		 * It seems to make the entity's coords mark the point from which LOD-distances are measured. In my testing, setting a vehicle as the focus entity and moving that vehicle more than 300 distance units away from the player will make the level of detail around the player go down drastically (shadows disappear, textures go extremely low res, etc). The player seems to be the default focus entity.
		 */
		void SET_FOCUS_ENTITY(Entity entity);
		BOOL IS_ENTITY_FOCUS(Entity entity);
		void SET_RESTORE_FOCUS_ENTITY(Entity p0);
		/**
		 * Possible p0 values:
		 * 
		 * "prologue"
		 * "Prologue_Main"
		 */
		void SET_MAPDATACULLBOX_ENABLED(const char* name, BOOL toggle);
		/**
		 * This native does absolutely nothing, just a nullsub
		 */
		void SET_ALL_MAPDATA_CULLED(Any p0);
		/**
		 * Always returns zero.
		 */
		int STREAMVOL_CREATE_SPHERE(float x, float y, float z, float rad, Any p4, Any p5);
		/**
		 * Always returns zero.
		 */
		int STREAMVOL_CREATE_FRUSTUM(float p0, float p1, float p2, float p3, float p4, float p5, float p6, Any p7, Any p8);
		/**
		 * Always returns zero.
		 */
		int STREAMVOL_CREATE_LINE(float p0, float p1, float p2, float p3, float p4, float p5, Any p6);
		void STREAMVOL_DELETE(Any unused);
		BOOL STREAMVOL_HAS_LOADED(Any unused);
		BOOL STREAMVOL_IS_VALID(Any unused);
		BOOL IS_STREAMVOL_ACTIVE();
		/**
		 * `radius` value is usually between `3f` and `7000f` in original 1868 scripts.
		 * `p7` is 0, 1, 2, 3 or 4 used in decompiled scripts, 0 is by far the most common.
		 * Returns True if success, used only 7 times in decompiled scripts of 1868
		 */
		BOOL NEW_LOAD_SCENE_START(float posX, float posY, float posZ, float offsetX, float offsetY, float offsetZ, float radius, int p7);
		BOOL NEW_LOAD_SCENE_START_SPHERE(float x, float y, float z, float radius, Any p4);
		void NEW_LOAD_SCENE_STOP();
		BOOL IS_NEW_LOAD_SCENE_ACTIVE();
		BOOL IS_NEW_LOAD_SCENE_LOADED();
		BOOL IS_SAFE_TO_START_PLAYER_SWITCH();
		/**
		 * // this enum comes directly from R* so don't edit this
		 * enum ePlayerSwitchTypes
		 * {
		 * SWITCH_TYPE_AUTO,
		 * SWITCH_TYPE_LONG,
		 * SWITCH_TYPE_MEDIUM,
		 * SWITCH_TYPE_SHORT
		 * };
		 * 
		 * Use GET_IDEAL_PLAYER_SWITCH_TYPE for the best switch type.
		 * 
		 * ----------------------------------------------------
		 * 
		 * Examples from the decompiled scripts:
		 * 
		 * STREAMING::START_PLAYER_SWITCH(l_832._f3, PLAYER::PLAYER_PED_ID(), 0, 3);
		 * STREAMING::START_PLAYER_SWITCH(l_832._f3, PLAYER::PLAYER_PED_ID(), 2050, 3);
		 * STREAMING::START_PLAYER_SWITCH(PLAYER::PLAYER_PED_ID(), l_832._f3, 1024, 3);
		 * STREAMING::START_PLAYER_SWITCH(g_141F27, PLAYER::PLAYER_PED_ID(), 513, v_14);
		 * 
		 * Note: DO NOT, use SWITCH_TYPE_LONG with flag 513. It leaves you stuck in the clouds. You'll have to call STOP_PLAYER_SWITCH() to return to your ped.
		 * 
		 * Flag 8 w/ SWITCH_TYPE_LONG will zoom out 3 steps, then zoom in 2/3 steps and stop on the 3rd and just hang there.
		 * Flag 8 w/ SWITCH_TYPE_MEDIUM will zoom out 1 step, and just hang there.
		 */
		void START_PLAYER_SWITCH(Ped from, Ped to, int flags, int switchType);
		void STOP_PLAYER_SWITCH();
		/**
		 * Returns true if the player is currently switching, false otherwise.
		 * (When the camera is in the sky moving from Trevor to Franklin for example)
		 */
		BOOL IS_PLAYER_SWITCH_IN_PROGRESS();
		int GET_PLAYER_SWITCH_TYPE();
		/**
		 * x1, y1, z1 -- Coords of your ped model
		 * x2, y2, z2 -- Coords of the ped you want to switch to
		 */
		int GET_IDEAL_PLAYER_SWITCH_TYPE(float x1, float y1, float z1, float x2, float y2, float z2);
		int GET_PLAYER_SWITCH_STATE();
		int GET_PLAYER_SHORT_SWITCH_STATE();
		void SET_PLAYER_SHORT_SWITCH_STYLE(int p0);
		int GET_PLAYER_SWITCH_JUMP_CUT_INDEX();
		void SET_PLAYER_SWITCH_OUTRO(float cameraCoordX, float cameraCoordY, float cameraCoordZ, float camRotationX, float camRotationY, float camRotationZ, float camFov, float camFarClip, int rotationOrder);
		/**
		 * All names can be found in playerswitchestablishingshots.meta
		 */
		void SET_PLAYER_SWITCH_ESTABLISHING_SHOT(const char* name);
		void ALLOW_PLAYER_SWITCH_PAN();
		void ALLOW_PLAYER_SWITCH_OUTRO();
		void ALLOW_PLAYER_SWITCH_ASCENT();
		void ALLOW_PLAYER_SWITCH_DESCENT();
		BOOL IS_SWITCH_READY_FOR_DESCENT();
		void ENABLE_SWITCH_PAUSE_BEFORE_DESCENT();
		void DISABLE_SWITCH_OUTRO_FX();
		/**
		 * doesn't act normally when used on mount chilliad
		 * Flags is a bitflag:
		 * 2^n - Enabled Functionality:
		 * 0 - Skip camera rotate up
		 * 3 - Wait for SET_PLAYER_SWITCH_ESTABLISHING_SHOT / hang at last step. You will still need to run 0x74DE2E8739086740 to exit "properly" and then STOP_PLAYER_SWITCH
		 * 6 - Invert Switch Direction (false = out, true = in)
		 * 8 - Hang above ped
		 * 
		 * switchType: 0 - 3
		 * 0: 1 step towards ped
		 * 1: 3 steps out from ped
		 * 2: 1 step out from ped
		 * 3: 1 step towards ped
		 */
		void SWITCH_TO_MULTI_FIRSTPART(Ped ped, int flags, int switchType);
		void SWITCH_TO_MULTI_SECONDPART(Ped ped);
		BOOL IS_SWITCH_TO_MULTI_FIRSTPART_FINISHED();
		int GET_PLAYER_SWITCH_INTERP_OUT_DURATION();
		int GET_PLAYER_SWITCH_INTERP_OUT_CURRENT_TIME();
		BOOL IS_SWITCH_SKIPPING_DESCENT();
		void SET_SCENE_STREAMING_TRACKS_CAM_POS_THIS_FRAME();
		float GET_LODSCALE();
		/**
		 * This allows you to override "extended distance scaling" setting. Needs to be called each frame.
		 * Max scaling seems to be 200.0, normal is 1.0
		 * See https://gfycat.com/DetailedHauntingIncatern
		 */
		void OVERRIDE_LODSCALE_THIS_FRAME(float scaling);
		void REMAP_LODSCALE_RANGE_THIS_FRAME(float p0, float p1, float p2, float p3);
		void SUPPRESS_HD_MAP_STREAMING_THIS_FRAME();
		void SET_RENDER_HD_ONLY(BOOL toggle);
		void FORCE_ALLOW_TIME_BASED_FADING_THIS_FRAME();
		void IPL_GROUP_SWAP_START(const char* iplName1, const char* iplName2);
		void IPL_GROUP_SWAP_CANCEL();
		BOOL IPL_GROUP_SWAP_IS_READY();
		void IPL_GROUP_SWAP_FINISH();
		BOOL IPL_GROUP_SWAP_IS_ACTIVE();
		/**
		 * This native is used to attribute the SRL that BEGIN_SRL is going to load. This is usually used for 'in-game' cinematics (not cutscenes but camera stuff) instead of SET_FOCUS_POS_AND_VEL because it loads a specific area of the map which is pretty useful when the camera moves from distant areas.
		 * For instance, GTA:O opening cutscene.
		 * https://pastebin.com/2EeKVeLA : a list of SRL found in srllist.meta
		 * https://pastebin.com/zd9XYUWY here is the content of a SRL file opened with codewalker.
		 */
		void PREFETCH_SRL(const char* srl);
		/**
		 * Returns true when the srl from BEGIN_SRL is loaded.
		 */
		BOOL IS_SRL_LOADED();
		void BEGIN_SRL();
		/**
		 * Clear the current srl and stop rendering the area selected by PREFETCH_SRL and started with BEGIN_SRL.
		 */
		void END_SRL();
		void SET_SRL_TIME(float p0);
		void SET_SRL_POST_CUTSCENE_CAMERA(Any p0, Any p1, Any p2, Any p3, Any p4, Any p5);
		void SET_SRL_READAHEAD_TIMES(Any p0, Any p1, Any p2, Any p3);
		void SET_SRL_LONG_JUMP_MODE(BOOL p0);
		void SET_SRL_FORCE_PRESTREAM(Any p0);
		void SET_HD_AREA(float x, float y, float z, float radius);
		void CLEAR_HD_AREA();
		void INIT_CREATOR_BUDGET();
		void SHUTDOWN_CREATOR_BUDGET();
		BOOL ADD_MODEL_TO_CREATOR_BUDGET(Hash modelHash);
		void REMOVE_MODEL_FROM_CREATOR_BUDGET(Hash modelHash);
		/**
		 * 0.0 = no memory used
		 * 1.0 = all memory used
		 * 
		 * Maximum model memory (as defined in common\data\missioncreatordata.meta) is 100 MiB
		 */
		float GET_USED_CREATOR_BUDGET();
		void _GET_MODEL_ADDITIONAL_COST(Hash modelHash); // Missing in crossmap
		void _GET_TOTAL_MODEL_COST(Hash modelHash); // Missing in crossmap
		/**
		 * Enables the specified island. For more information, see islandhopper.meta
		 */
		void SET_ISLAND_ENABLED(const char* name, BOOL toggle);
		void _SET_SPHERICAL_STREAM_DISTANT_HILODS_THIS_FRAME(); // Missing in crossmap
		void IS_GAME_INSTALLED(); // Missing in crossmap

	} // namespace STREAMING

	namespace TASK {

		/**
		 * Stand still (?)
		 */
		void TASK_PAUSE(Ped ped, int ms);
		/**
		 * Makes the specified ped stand still for (time) milliseconds.
		 */
		void TASK_STAND_STILL(Ped ped, int time);
		/**
		 * Definition is wrong. This has 4 parameters (Not sure when they were added. v350 has 2, v678 has 4).
		 * 
		 * v350: Ped ped, bool unused
		 * v678: Ped ped, bool unused, bool flag1, bool flag2
		 * 
		 * flag1 = super jump, flag2 = do nothing if flag1 is false and doubles super jump height if flag1 is true.
		 */
		void TASK_JUMP(Ped ped, BOOL usePlayerLaunchForce, BOOL doSuperJump, BOOL useFullSuperJumpForce);
		void TASK_COWER(Ped ped, int duration);
		/**
		 * In the scripts, p3 was always -1.
		 * 
		 * p3 seems to be duration or timeout of turn animation.
		 * Also facingPed can be 0 or -1 so ped will just raise hands up.
		 */
		void TASK_HANDS_UP(Ped ped, int duration, Ped facingPed, int timeToFacePed, int flags);
		void UPDATE_TASK_HANDS_UP_DURATION(Ped ped, int duration);
		/**
		 * The given ped will try to open the nearest door to 'seat'.
		 * Example: telling the ped to open the door for the driver seat does not necessarily mean it will open the driver door, it may choose to open the passenger door instead if that one is closer.
		 */
		void TASK_OPEN_VEHICLE_DOOR(Ped ped, Vehicle vehicle, int timeOut, int seat, float speed);
		/**
		 * speed 1.0 = walk, 2.0 = run
		 * p5 1 = normal, 3 = teleport to vehicle, 16 = teleport directly into vehicle
		 * p6 is always 0
		 * 
		 * Usage of seat
		 * -1 = driver
		 * 0 = passenger
		 * 1 = left back seat
		 * 2 = right back seat
		 * 3 = outside left
		 * 4 = outside right
		 */
		void TASK_ENTER_VEHICLE(Ped ped, Vehicle vehicle, int timeout, int seat, float speed, int flag, const char* overrideEntryClipsetName);
		/**
		 * Flags from decompiled scripts:
		 * 0 = normal exit and closes door.
		 * 1 = normal exit and closes door.
		 * 16 = teleports outside, door kept closed.
		 * 64 = normal exit and closes door, maybe a bit slower animation than 0.
		 * 256 = normal exit but does not close the door.
		 * 4160 = ped is throwing himself out, even when the vehicle is still.
		 * 262144 = ped moves to passenger seat first, then exits normally
		 * 
		 * Others to be tried out: 320, 512, 131072.
		 */
		void TASK_LEAVE_VEHICLE(Ped ped, Vehicle vehicle, int flags);
		void TASK_GET_OFF_BOAT(Ped ped, Vehicle boat);
		void TASK_SKY_DIVE(Ped ped, BOOL instant);
		/**
		 * Second parameter is unused.
		 * 
		 * second parameter was for jetpack in the early stages of gta and the hard coded code is now removed
		 */
		void TASK_PARACHUTE(Ped ped, BOOL giveParachuteItem, BOOL instant);
		/**
		 * makes ped parachute to coords x y z. Works well with PATHFIND::GET_SAFE_COORD_FOR_PED
		 */
		void TASK_PARACHUTE_TO_TARGET(Ped ped, float x, float y, float z);
		void SET_PARACHUTE_TASK_TARGET(Ped ped, float x, float y, float z);
		void SET_PARACHUTE_TASK_THRUST(Ped ped, float thrust);
		/**
		 * minHeightAboveGround: the minimum height above ground the heli must be at before the ped can start rappelling
		 * 
		 * Only appears twice in the scripts.
		 * 
		 * TASK::TASK_RAPPEL_FROM_HELI(PLAYER::PLAYER_PED_ID(), 10.0f);
		 * TASK::TASK_RAPPEL_FROM_HELI(a_0, 10.0f);
		 */
		void TASK_RAPPEL_FROM_HELI(Ped ped, float minHeightAboveGround);
		/**
		 * info about driving modes: https://gtaforums.com/topic/822314-guide-driving-styles/
		 */
		void TASK_VEHICLE_DRIVE_TO_COORD(Ped ped, Vehicle vehicle, float x, float y, float z, float speed, Any p6, Hash vehicleModel, int drivingMode, float stopRange, float straightLineDistance);
		void TASK_VEHICLE_DRIVE_TO_COORD_LONGRANGE(Ped ped, Vehicle vehicle, float x, float y, float z, float speed, int driveMode, float stopRange);
		void TASK_VEHICLE_DRIVE_WANDER(Ped ped, Vehicle vehicle, float speed, int drivingStyle);
		/**
		 * p6 always -1
		 * p7 always 10.0
		 * p8 always 1
		 */
		void TASK_FOLLOW_TO_OFFSET_OF_ENTITY(Ped ped, Entity entity, float offsetX, float offsetY, float offsetZ, float movementSpeed, int timeout, float stoppingRange, BOOL persistFollowing);
		void TASK_GO_STRAIGHT_TO_COORD(Ped ped, float x, float y, float z, float speed, int timeout, float targetHeading, float distanceToSlide);
		void TASK_GO_STRAIGHT_TO_COORD_RELATIVE_TO_ENTITY(Ped ped, Entity entity, float x, float y, float z, float moveBlendRatio, int time);
		/**
		 * Makes the specified ped achieve the specified heading.
		 * 
		 * pedHandle: The handle of the ped to assign the task to.
		 * heading: The desired heading.
		 * timeout: The time, in milliseconds, to allow the task to complete. If the task times out, it is cancelled, and the ped will stay at the heading it managed to reach in the time.
		 */
		void TASK_ACHIEVE_HEADING(Ped ped, float heading, int timeout);
		/**
		 * MulleKD19: Clears the current point route. Call this before TASK_EXTEND_ROUTE and TASK_FOLLOW_POINT_ROUTE.
		 */
		void TASK_FLUSH_ROUTE();
		/**
		 * MulleKD19: Adds a new point to the current point route. Call TASK_FLUSH_ROUTE before the first call to this. Call TASK_FOLLOW_POINT_ROUTE to make the Ped go the route.
		 * 
		 * A maximum of 8 points can be added.
		 */
		void TASK_EXTEND_ROUTE(float x, float y, float z);
		/**
		 * MulleKD19: Makes the ped go on the created point route.
		 * 
		 * ped: The ped to give the task to.
		 * speed: The speed to move at in m/s.
		 * int: Unknown. Can be 0, 1, 2 or 3.
		 * 
		 * Example:
		 * TASK_FLUSH_ROUTE();
		 * TASK_EXTEND_ROUTE(0f, 0f, 70f);
		 * TASK_EXTEND_ROUTE(10f, 0f, 70f);
		 * TASK_EXTEND_ROUTE(10f, 10f, 70f);
		 * TASK_FOLLOW_POINT_ROUTE(GET_PLAYER_PED(), 1f, 0);
		 */
		void TASK_FOLLOW_POINT_ROUTE(Ped ped, float speed, int mode);
		/**
		 * The entity will move towards the target until time is over (duration) or get in target's range (distance). p5 and p6 are unknown, but you could leave p5 = 1073741824 or 100 or even 0 (didn't see any difference but on the decompiled scripts, they use 1073741824 mostly) and p6 = 0
		 * 
		 * Note: I've only tested it on entity -> ped and target -> vehicle. It could work differently on other entities, didn't try it yet.
		 * 
		 * Example: TASK::TASK_GO_TO_ENTITY(pedHandle, vehicleHandle, 5000, 4.0, 100, 1073741824, 0)
		 * 
		 * Ped will run towards the vehicle for 5 seconds and stop when time is over or when he gets 4 meters(?) around the vehicle (with duration = -1, the task duration will be ignored).
		 * 
		 * enum EGOTO_ENTITY_SCRIPT_FLAGS
		 * {
		 * EGOTO_ENTITY_NEVER_SLOW_FOR_PATH_LENGTH = 0x01,
		 * };
		 */
		void TASK_GO_TO_ENTITY(Entity entity, Entity target, int duration, float distance, float moveBlendRatio, float slowDownDistance, int flags);
		/**
		 * Makes the specified ped flee the specified distance from the specified position.
		 */
		void TASK_SMART_FLEE_COORD(Ped ped, float x, float y, float z, float distance, int time, BOOL preferPavements, BOOL quitIfOutOfRange);
		/**
		 * Makes a ped run away from another ped (fleeTarget).
		 * 
		 * distance = ped will flee this distance.
		 * fleeTime = ped will flee for this amount of time, set to "-1" to flee forever
		 */
		void TASK_SMART_FLEE_PED(Ped ped, Ped fleeTarget, float safeDistance, int fleeTime, BOOL preferPavements, BOOL updateToNearestHatedPed);
		void TASK_REACT_AND_FLEE_PED(Ped ped, Ped fleeTarget);
		void TASK_SHOCKING_EVENT_REACT(Ped ped, int eventHandle);
		void TASK_WANDER_IN_AREA(Ped ped, float x, float y, float z, float radius, float minimalLength, float timeBetweenWalks);
		/**
		 * Makes ped walk around the area.
		 * 
		 * set p1 to 10.0f and p2 to 10 if you want the ped to walk anywhere without a duration.
		 */
		void TASK_WANDER_STANDARD(Ped ped, float heading, int flags);
		void TASK_WANDER_SPECIFIC(Ped ped, const char* conditionalAnimGroupStr, const char* conditionalAnimStr, float heading);
		/**
		 * Modes:
		 * 0 - ignore heading
		 * 1 - park forward
		 * 2 - park backwards
		 * 
		 * Depending on the angle of approach, the vehicle can park at the specified heading or at its exact opposite (-180) angle.
		 * 
		 * Radius seems to define how close the vehicle has to be -after parking- to the position for this task considered completed. If the value is too small, the vehicle will try to park again until it's exactly where it should be. 20.0 Works well but lower values don't, like the radius is measured in centimeters or something.
		 */
		void TASK_VEHICLE_PARK(Ped ped, Vehicle vehicle, float x, float y, float z, float heading, int mode, float radius, BOOL keepEngineOn);
		/**
		 * known "killTypes" are: "AR_stealth_kill_knife" and "AR_stealth_kill_a".
		 */
		void TASK_STEALTH_KILL(Ped killer, Ped target, Hash stealthKillActionResultHash, float desiredMoveBlendRatio, int stealthFlags);
		void TASK_PLANT_BOMB(Ped ped, float x, float y, float z, float heading);
		void TASK_SHARK_CIRCLE_COORD(Ped ped, float x, float y, float z, float moveBlendRatio, float radius); // Missing in crossmap
		/**
		 * If no timeout, set timeout to -1.
		 */
		void TASK_FOLLOW_NAV_MESH_TO_COORD(Ped ped, float x, float y, float z, float moveBlendRatio, int time, float targetRadius, int flags, float targetHeading);
		void TASK_FOLLOW_NAV_MESH_TO_COORD_ADVANCED(Ped ped, float x, float y, float z, float moveBlendRatio, int time, float targetRadius, int flags, float slideToCoordHeading, float maxSlopeNavigable, float clampMaxSearchDistance, float targetHeading);
		void SET_PED_PATH_CAN_USE_CLIMBOVERS(Ped ped, BOOL Toggle);
		void SET_PED_PATH_CAN_USE_LADDERS(Ped ped, BOOL Toggle);
		void SET_PED_PATH_CAN_DROP_FROM_HEIGHT(Ped ped, BOOL Toggle);
		/**
		 * Default modifier is 1.0, minimum is 0.0 and maximum is 10.0.
		 */
		void SET_PED_PATH_CLIMB_COST_MODIFIER(Ped ped, float modifier);
		void SET_PED_PATH_MAY_ENTER_WATER(Ped ped, BOOL mayEnterWater);
		void SET_PED_PATH_PREFER_TO_AVOID_WATER(Ped ped, BOOL avoidWater);
		void SET_PED_PATH_AVOID_FIRE(Ped ped, BOOL avoidFire);
		/**
		 * Needs to be looped! And yes, it does work and is not a hash collision.
		 * Birds will try to reach the given height.
		 */
		void SET_GLOBAL_MIN_BIRD_FLIGHT_HEIGHT(float height);
		/**
		 * Looks like the last parameter returns true if the path has been calculated, while the first returns the remaining distance to the end of the path.
		 * Return value of native is the same as GET_NAVMESH_ROUTE_RESULT
		 * Looks like the native returns an int for the path's state:
		 * 1 - ???
		 * 2 - ???
		 * 3 - Finished Generating
		 */
		int GET_NAVMESH_ROUTE_DISTANCE_REMAINING(Ped ped, float* distanceRemaining, BOOL* isPathReady);
		/**
		 * See GET_NAVMESH_ROUTE_DISTANCE_REMAINING for more details.
		 */
		int GET_NAVMESH_ROUTE_RESULT(Ped ped);
		BOOL IS_CONTROLLED_VEHICLE_UNABLE_TO_GET_TO_ROAD(Ped ped);
		/**
		 * example from fm_mission_controller
		 * 
		 * TASK::TASK_GO_TO_COORD_ANY_MEANS(l_649, sub_f7e86(-1, 0), 1.0, 0, 0, 786603, 0xbf800000);
		 * 
		 */
		void TASK_GO_TO_COORD_ANY_MEANS(Ped ped, float x, float y, float z, float moveBlendRatio, Vehicle vehicle, BOOL useLongRangeVehiclePathing, int drivingFlags, float maxRangeToShootTargets);
		void TASK_GO_TO_COORD_ANY_MEANS_EXTRA_PARAMS(Ped ped, float x, float y, float z, float moveBlendRatio, Vehicle vehicle, BOOL useLongRangeVehiclePathing, int drivingFlags, float maxRangeToShootTargets, float extraVehToTargetDistToPreferVehicle, float driveStraightLineDistance, int extraFlags, float warpTimerMS);
		void TASK_GO_TO_COORD_ANY_MEANS_EXTRA_PARAMS_WITH_CRUISE_SPEED(Ped ped, float x, float y, float z, float moveBlendRatio, Vehicle vehicle, BOOL useLongRangeVehiclePathing, int drivingFlags, float maxRangeToShootTargets, float extraVehToTargetDistToPreferVehicle, float driveStraightLineDistance, int extraFlags, float cruiseSpeed, float targetArriveDist);
		/**
		 * Full list of animation dictionaries and anims by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/animDictsCompact.json
		 * 
		 * float speed > normal speed is 8.0f
		 * ----------------------
		 * 
		 * float speedMultiplier > multiply the playback speed
		 * ----------------------
		 * 
		 * int duration: time in millisecond
		 * ----------------------
		 * -1 _ _ _ _ _ _ _> Default (see flag)
		 * 0 _ _ _ _ _ _ _ > Not play at all
		 * Small value _ _ > Slow down animation speed
		 * Other _ _ _ _ _ > freeze player control until specific time (ms) has
		 * _ _ _ _ _ _ _ _ _ passed. (No effect if flag is set to be
		 * _ _ _ _ _ _ _ _ _ controllable.)
		 * 
		 * int flag:
		 * ----------------------
		 * enum eAnimationFlags
		 * {
		 * ANIM_FLAG_NORMAL = 0,
		 * ANIM_FLAG_REPEAT = 1,
		 * ANIM_FLAG_STOP_LAST_FRAME = 2,
		 * ANIM_FLAG_UPPERBODY = 16,
		 * ANIM_FLAG_ENABLE_PLAYER_CONTROL = 32,
		 * ANIM_FLAG_CANCELABLE = 120,
		 * };
		 * Odd number : loop infinitely
		 * Even number : Freeze at last frame
		 * Multiple of 4: Freeze at last frame but controllable
		 * 
		 * 01 to 15 > Full body
		 * 10 to 31 > Upper body
		 * 32 to 47 > Full body > Controllable
		 * 48 to 63 > Upper body > Controllable
		 * ...
		 * 001 to 255 > Normal
		 * 256 to 511 > Garbled
		 * ...
		 * 
		 * playbackRate:
		 * 
		 * values are between 0.0 and 1.0
		 * 
		 * 
		 * lockX:
		 * 
		 * 0 in most cases 1 for rcmepsilonism8 and rcmpaparazzo_3
		 * > 1 for mini@sprunk
		 * 
		 * 
		 * lockY:
		 * 
		 * 0 in most cases
		 * 1 for missfam5_yoga, missfra1mcs_2_crew_react
		 * 
		 * 
		 * lockZ:
		 * 
		 * 0 for single player
		 * Can be 1 but only for MP
		 */
		void TASK_PLAY_ANIM(Ped ped, const char* animDictionary, const char* animationName, float blendInSpeed, float blendOutSpeed, int duration, int flag, float playbackRate, BOOL lockX, BOOL lockY, BOOL lockZ);
		/**
		 * It's similar to TASK_PLAY_ANIM, except the first 6 floats let you specify the initial position and rotation of the task. (Ped gets teleported to the position).
		 * 
		 * Full list of animation dictionaries and anims by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/animDictsCompact.json
		 */
		void TASK_PLAY_ANIM_ADVANCED(Ped ped, const char* animDict, const char* animName, float posX, float posY, float posZ, float rotX, float rotY, float rotZ, float animEnterSpeed, float animExitSpeed, int duration, Any flag, float animTime, int rotOrder, int ikFlags);
		/**
		 * Full list of animation dictionaries and anims by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/animDictsCompact.json
		 */
		void STOP_ANIM_TASK(Entity entity, const char* animDictionary, const char* animationName, float blendDelta);
		/**
		 * From fm_mission_controller.c:
		 * reserve_network_mission_objects(get_num_reserved_mission_objects(0) + 1);
		 * vVar28 = {0.094f, 0.02f, -0.005f};
		 * vVar29 = {-92.24f, 63.64f, 150.24f};
		 * func_253(&uVar30, joaat("prop_ld_case_01"), Global_1592429.imm_34757[iParam1 <268>], 1, 1, 0, 1);
		 * set_entity_lod_dist(net_to_ent(uVar30), 500);
		 * attach_entity_to_entity(net_to_ent(uVar30), iParam0, get_ped_bone_index(iParam0, 28422), vVar28, vVar29, 1, 0, 0, 0, 2, 1);
		 * Var31.imm_4 = 1065353216;
		 * Var31.imm_5 = 1065353216;
		 * Var31.imm_9 = 1065353216;
		 * Var31.imm_10 = 1065353216;
		 * Var31.imm_14 = 1065353216;
		 * Var31.imm_15 = 1065353216;
		 * Var31.imm_17 = 1040187392;
		 * Var31.imm_18 = 1040187392;
		 * Var31.imm_19 = -1;
		 * Var32.imm_4 = 1065353216;
		 * Var32.imm_5 = 1065353216;
		 * Var32.imm_9 = 1065353216;
		 * Var32.imm_10 = 1065353216;
		 * Var32.imm_14 = 1065353216;
		 * Var32.imm_15 = 1065353216;
		 * Var32.imm_17 = 1040187392;
		 * Var32.imm_18 = 1040187392;
		 * Var32.imm_19 = -1;
		 * Var31 = 1;
		 * Var31.imm_1 = "weapons@misc@jerrycan@mp_male";
		 * Var31.imm_2 = "idle";
		 * Var31.imm_20 = 1048633;
		 * Var31.imm_4 = 0.5f;
		 * Var31.imm_16 = get_hash_key("BONEMASK_ARMONLY_R");
		 * task_scripted_animation(iParam0, &Var31, &Var32, &Var32, 0f, 0.25f);
		 * set_model_as_no_longer_needed(joaat("prop_ld_case_01"));
		 * remove_anim_dict("anim@heists@biolab@");
		 */
		void TASK_SCRIPTED_ANIMATION(Ped ped, int* priorityLowData, int* priorityMidData, int* priorityHighData, float blendInDelta, float blendOutDelta);
		void PLAY_ENTITY_SCRIPTED_ANIM(Entity entity, int* priorityLowData, int* priorityMidData, int* priorityHighData, float blendInDelta, float blendOutDelta);
		/**
		 * Looks like p1 may be a flag, still need to do some research, though.
		 */
		void STOP_ANIM_PLAYBACK(Entity entity, int priority, BOOL secondary);
		void SET_ANIM_WEIGHT(Entity entity, float weight, int priority, int index, BOOL secondary);
		void SET_ANIM_PHASE(Entity entity, float phase, int priority, BOOL secondary);
		void SET_ANIM_RATE(Entity entity, float rate, int priority, BOOL secondary);
		void SET_ANIM_LOOPED(Entity entity, BOOL looped, int priority, BOOL secondary);
		/**
		 * Example from the scripts:
		 * TASK::TASK_PLAY_PHONE_GESTURE_ANIMATION(PLAYER::PLAYER_PED_ID(), v_3, v_2, v_4, 0.25, 0.25, 0, 0);
		 * 
		 * =========================================================
		 * ^^ No offense, but Idk how that would really help anyone.
		 * 
		 * As for the animDict & animation, they're both store in a global in all 5 scripts. So if anyone would be so kind as to read that global and comment what strings they use. Thanks.
		 * 
		 * Known boneMaskTypes'
		 * "BONEMASK_HEADONLY"
		 * "BONEMASK_HEAD_NECK_AND_ARMS"
		 * "BONEMASK_HEAD_NECK_AND_L_ARM"
		 * "BONEMASK_HEAD_NECK_AND_R_ARM"
		 * 
		 * p4 known args - 0.0f, 0.5f, 0.25f
		 * p5 known args - 0.0f, 0.25f
		 * p6 known args - 1 if a global if check is passed.
		 * p7 known args - 1 if a global if check is passed.
		 * 
		 * The values found above, I found within the 5 scripts this is ever called in. (fmmc_launcher, fm_deathmatch_controller, fm_impromptu_dm_controller, fm_mission_controller, and freemode).
		 * =========================================================
		 * 
		 * Full list of animation dictionaries and anims by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/animDictsCompact.json
		 */
		void TASK_PLAY_PHONE_GESTURE_ANIMATION(Ped ped, const char* animDict, const char* animation, const char* boneMaskType, float blendInDuration, float blendOutDuration, BOOL isLooping, BOOL holdLastFrame);
		void TASK_STOP_PHONE_GESTURE_ANIMATION(Ped ped, float blendOutOverride);
		BOOL IS_PLAYING_PHONE_GESTURE_ANIM(Ped ped);
		float GET_PHONE_GESTURE_ANIM_CURRENT_TIME(Ped ped);
		float GET_PHONE_GESTURE_ANIM_TOTAL_TIME(Ped ped);
		/**
		 * Most probably plays a specific animation on vehicle. For example getting chop out of van etc...
		 * 
		 * Here's how its used -
		 * 
		 * TASK::TASK_VEHICLE_PLAY_ANIM(l_325, "rcmnigel1b", "idle_speedo");
		 * 
		 * TASK::TASK_VEHICLE_PLAY_ANIM(l_556[0/*1* /], "missfra0_chop_drhome", "InCar_GetOutofBack_Speedo");
		 * 
		 * FYI : Speedo is the name of van in which chop was put in the mission.
		 */
		void TASK_VEHICLE_PLAY_ANIM(Vehicle vehicle, const char* animationSet, const char* animationName);
		/**
		 * enum eScriptLookatFlags
		 * {
		 * SLF_SLOW_TURN_RATE            = 1,    // turn the head toward the target slowly
		 * SLF_FAST_TURN_RATE            = 2,    // turn the head toward the target quickly
		 * SLF_EXTEND_YAW_LIMIT        = 4,    // wide yaw head limits
		 * SLF_EXTEND_PITCH_LIMIT        = 8,    // wide pitch head limit
		 * SLF_WIDEST_YAW_LIMIT        = 16,   // widest yaw head limit
		 * SLF_WIDEST_PITCH_LIMIT        = 32,   // widest pitch head limit
		 * SLF_NARROW_YAW_LIMIT        = 64,   // narrow yaw head limits
		 * SLF_NARROW_PITCH_LIMIT        = 128,  // narrow pitch head limit
		 * SLF_NARROWEST_YAW_LIMIT        = 256,  // narrowest yaw head limit
		 * SLF_NARROWEST_PITCH_LIMIT    = 512,  // narrowest pitch head limit
		 * SLF_USE_TORSO                = 1024, // use the torso aswell as the neck and head (currently disabled)
		 * SLF_WHILE_NOT_IN_FOV        = 2048, // keep tracking the target even if they are not in the hard coded FOV
		 * SLF_USE_CAMERA_FOCUS        = 4096, // use the camera as the target
		 * SLF_USE_EYES_ONLY            = 8192, // only track the target with the eyes
		 * SLF_USE_LOOK_DIR            = 16384, // use information in look dir DOF
		 * SLF_FROM_SCRIPT                = 32768, // internal use only
		 * SLF_USE_REF_DIR_ABSOLUTE    = 65536  // use absolute reference direction mode for solver
		 * };
		 */
		void TASK_LOOK_AT_COORD(Entity entity, float x, float y, float z, int duration, int flags, int priority);
		/**
		 * For flags, please refer to TASK_LOOK_AT_COORD.
		 */
		void TASK_LOOK_AT_ENTITY(Ped ped, Entity lookAt, int duration, int flags, int priority);
		void TASK_CLEAR_LOOK_AT(Ped ped);
		void OPEN_SEQUENCE_TASK(int* taskSequenceId);
		void CLOSE_SEQUENCE_TASK(int taskSequenceId);
		void TASK_PERFORM_SEQUENCE(Ped ped, int taskSequenceId);
		void TASK_PERFORM_SEQUENCE_LOCALLY(Ped ped, int taskSequenceId);
		void CLEAR_SEQUENCE_TASK(int* taskSequenceId);
		void SET_SEQUENCE_TO_REPEAT(int taskSequenceId, BOOL repeat);
		void SET_SEQUENCE_PREVENT_MIGRATION(int taskSequenceId); // Missing in crossmap
		/**
		 * returned values:
		 * 0 to 7 = task that's currently in progress, 0 meaning the first one.
		 * -1 no task sequence in progress.
		 */
		int GET_SEQUENCE_PROGRESS(Ped ped);
		/**
		 * Task index enum: https://alloc8or.re/gta5/doc/enums/eTaskTypeIndex.txt
		 */
		BOOL GET_IS_TASK_ACTIVE(Ped ped, int taskIndex);
		/**
		 * Gets the status of a script-assigned task.
		 * taskHash: https://alloc8or.re/gta5/doc/enums/eScriptTaskHash.txt
		 */
		int GET_SCRIPT_TASK_STATUS(Ped ped, Hash taskHash);
		/**
		 * https://alloc8or.re/gta5/doc/enums/eVehicleMissionType.txt
		 */
		int GET_ACTIVE_VEHICLE_MISSION_TYPE(Vehicle vehicle);
		/**
		 * Flags are the same flags used in TASK_LEAVE_VEHICLE
		 */
		void TASK_LEAVE_ANY_VEHICLE(Ped ped, int delayTime, int flags);
		void TASK_AIM_GUN_SCRIPTED(Ped ped, Hash scriptTask, BOOL disableBlockingClip, BOOL instantBlendToAim);
		void TASK_AIM_GUN_SCRIPTED_WITH_TARGET(Ped ped, Ped target, float x, float y, float z, int gunTaskType, BOOL disableBlockingClip, BOOL forceAim);
		void UPDATE_TASK_AIM_GUN_SCRIPTED_TARGET(Ped ped, Ped target, float x, float y, float z, BOOL disableBlockingClip);
		const char* GET_CLIP_SET_FOR_SCRIPTED_GUN_TASK(int gunTaskType);
		/**
		 * duration: the amount of time in milliseconds to do the task.  -1 will keep the task going until either another task is applied, or CLEAR_ALL_TASKS() is called with the ped
		 */
		void TASK_AIM_GUN_AT_ENTITY(Ped ped, Entity entity, int duration, BOOL instantBlendToAim);
		/**
		 * duration: the amount of time in milliseconds to do the task. -1 will keep the task going until either another task is applied, or CLEAR_ALL_TASKS() is called with the ped
		 */
		void TASK_TURN_PED_TO_FACE_ENTITY(Ped ped, Entity entity, int duration);
		void TASK_AIM_GUN_AT_COORD(Ped ped, float x, float y, float z, int time, BOOL instantBlendToAim, BOOL playAnimIntro);
		/**
		 * Firing Pattern Hash Information: https://pastebin.com/Px036isB
		 */
		void TASK_SHOOT_AT_COORD(Ped ped, float x, float y, float z, int duration, Hash firingPattern);
		/**
		 * Makes the specified ped shuffle to the next vehicle seat.
		 * The ped MUST be in a vehicle and the vehicle parameter MUST be the ped's current vehicle.
		 */
		void TASK_SHUFFLE_TO_NEXT_VEHICLE_SEAT(Ped ped, Vehicle vehicle, BOOL useAlternateShuffle);
		void CLEAR_PED_TASKS(Ped ped);
		void CLEAR_PED_SCRIPT_TASK_IF_RUNNING_THREAT_RESPONSE_NON_TEMP_TASK(Ped ped); // Missing in crossmap
		void CLEAR_PED_SECONDARY_TASK(Ped ped);
		void TASK_EVERYONE_LEAVE_VEHICLE(Vehicle vehicle);
		/**
		 * enum ESEEK_ENTITY_OFFSET_FLAGS
		 * {
		 * ESEEK_OFFSET_ORIENTATES_WITH_ENTITY = 0x01,
		 * ESEEK_KEEP_TO_PAVEMENTS = 0x02
		 * };
		 */
		void TASK_GOTO_ENTITY_OFFSET(Ped ped, Entity entity, int time, float seekRadius, float seekAngleDeg, float moveBlendRatio, int gotoEntityOffsetFlags);
		void TASK_GOTO_ENTITY_OFFSET_XY(Ped ped, Entity entity, int duration, float targetRadius, float offsetX, float offsetY, float moveBlendRatio, int gotoEntityOffsetFlags);
		/**
		 * duration in milliseconds
		 */
		void TASK_TURN_PED_TO_FACE_COORD(Ped ped, float x, float y, float z, int duration);
		/**
		 * '1 - brake
		 * '3 - brake + reverse
		 * '4 - turn left 90 + braking
		 * '5 - turn right 90 + braking
		 * '6 - brake strong (handbrake?) until time ends
		 * '7 - turn left + accelerate
		 * '8 - turn right + accelerate
		 * '9 - weak acceleration
		 * '10 - turn left + restore wheel pos to center in the end
		 * '11 - turn right + restore wheel pos to center in the end
		 * '13 - turn left + go reverse
		 * '14 - turn left + go reverse
		 * '16 - crash the game after like 2 seconds :)
		 * '17 - keep actual state, game crashed after few tries
		 * '18 - game crash
		 * '19 - strong brake + turn left/right
		 * '20 - weak brake + turn left then turn right
		 * '21 - weak brake + turn right then turn left
		 * '22 - brake + reverse
		 * '23 - accelerate fast
		 * '24 - brake
		 * '25 - brake turning left then when almost stopping it turns left more
		 * '26 - brake turning right then when almost stopping it turns right more
		 * '27 - brake until car stop or until time ends
		 * '28 - brake + strong reverse acceleration
		 * '30 - performs a burnout (brake until stop + brake and accelerate)
		 * '31 - accelerate + handbrake
		 * '32 - accelerate very strong
		 * 
		 * Seems to be this:
		 * Works on NPCs, but overrides their current task. If inside a task sequence (and not being the last task), "time" will work, otherwise the task will be performed forever until tasked with something else
		 */
		void TASK_VEHICLE_TEMP_ACTION(Ped driver, Vehicle vehicle, int action, int time);
		/**
		 * missionType: https://alloc8or.re/gta5/doc/enums/eVehicleMissionType.txt
		 */
		void TASK_VEHICLE_MISSION(Ped driver, Vehicle vehicle, Vehicle vehicleTarget, int missionType, float cruiseSpeed, int drivingStyle, float targetReached, float straightLineDistance, BOOL DriveAgainstTraffic);
		/**
		 * See TASK_VEHICLE_MISSION
		 */
		void TASK_VEHICLE_MISSION_PED_TARGET(Ped ped, Vehicle vehicle, Ped pedTarget, int missionType, float maxSpeed, int drivingStyle, float minDistance, float straightLineDistance, BOOL DriveAgainstTraffic);
		/**
		 * See TASK_VEHICLE_MISSION
		 */
		void TASK_VEHICLE_MISSION_COORS_TARGET(Ped ped, Vehicle vehicle, float x, float y, float z, int mission, float cruiseSpeed, int drivingStyle, float targetReached, float straightLineDistance, BOOL DriveAgainstTraffic);
		/**
		 * Makes a ped follow the targetVehicle with <minDistance> in between.
		 * 
		 * note: minDistance is ignored if drivingstyle is avoiding traffic, but Rushed is fine.
		 * 
		 * Mode: The mode defines the relative position to the targetVehicle. The ped will try to position its vehicle there.
		 * -1 = behind
		 * 0 = ahead
		 * 1 = left
		 * 2 = right
		 * 3 = back left
		 * 4 = back right
		 * 
		 * if the target is closer than noRoadsDistance, the driver will ignore pathing/roads and follow you directly.
		 * 
		 * Driving Styles guide: gtaforums.com/topic/822314-guide-driving-styles/
		 */
		void TASK_VEHICLE_ESCORT(Ped ped, Vehicle vehicle, Vehicle targetVehicle, int mode, float speed, int drivingStyle, float minDistance, int minHeightAboveTerrain, float noRoadsDistance);
		/**
		 * Makes a ped in a vehicle follow an entity (ped, vehicle, etc.)
		 * 
		 * drivingStyle: http://gtaforums.com/topic/822314-guide-driving-styles/
		 */
		void TASK_VEHICLE_FOLLOW(Ped driver, Vehicle vehicle, Entity targetEntity, float speed, int drivingStyle, int minDistance);
		/**
		 * chases targetEnt fast and aggressively
		 * --
		 * Makes ped (needs to be in vehicle) chase targetEnt.
		 */
		void TASK_VEHICLE_CHASE(Ped driver, Entity targetEnt);
		/**
		 * pilot, vehicle and altitude are rather self-explanatory.
		 * 
		 * p4: is unused variable in the function.
		 * 
		 * entityToFollow: you can provide a Vehicle entity or a Ped entity, the heli will protect them.
		 * 
		 * 'targetSpeed':  The pilot will dip the nose AS MUCH AS POSSIBLE so as to reach this value AS FAST AS POSSIBLE.  As such, you'll want to modulate it as opposed to calling it via a hard-wired, constant #.
		 * 
		 * 'radius' isn't just "stop within radius of X of target" like with ground vehicles.  In this case, the pilot will fly an entire circle around 'radius' and continue to do so.
		 * 
		 * NOT CONFIRMED:  p7 appears to be a FlyingStyle enum.  Still investigating it as of this writing, but playing around with values here appears to result in different -behavior- as opposed to offsetting coordinates, altitude, target speed, etc.
		 * 
		 * NOTE: If the pilot finds enemies, it will engage them until it kills them, but will return to protect the ped/vehicle given shortly thereafter.
		 */
		void TASK_VEHICLE_HELI_PROTECT(Ped pilot, Vehicle vehicle, Entity entityToFollow, float targetSpeed, int drivingFlags, float radius, int altitude, int heliFlags);
		/**
		 * Flag 8: Medium-aggressive boxing tactic with a bit of PIT
		 * Flag 1: Aggressive ramming of suspect
		 * Flag 2: Ram attempts
		 * Flag 32: Stay back from suspect, no tactical contact. Convoy-like.
		 * Flag 16: Ramming, seems to be slightly less aggressive than 1-2.
		 */
		void SET_TASK_VEHICLE_CHASE_BEHAVIOR_FLAG(Ped ped, int flag, BOOL set);
		void SET_TASK_VEHICLE_CHASE_IDEAL_PURSUIT_DISTANCE(Ped ped, float distance);
		/**
		 * Ped pilot should be in a heli.
		 * EntityToFollow can be a vehicle or Ped.
		 * 
		 * x,y,z appear to be how close to the EntityToFollow the heli should be. Scripts use 0.0, 0.0, 80.0. Then the heli tries to position itself 80 units above the EntityToFollow. If you reduce it to -5.0, it tries to go below (if the EntityToFollow is a heli or plane)
		 * 
		 * 
		 * NOTE: If the pilot finds enemies, it will engage them, then remain there idle, not continuing to chase the Entity given.
		 */
		void TASK_HELI_CHASE(Ped pilot, Entity entityToFollow, float x, float y, float z);
		void TASK_PLANE_CHASE(Ped pilot, Entity entityToFollow, float x, float y, float z);
		void TASK_PLANE_LAND(Ped pilot, Vehicle plane, float runwayStartX, float runwayStartY, float runwayStartZ, float runwayEndX, float runwayEndY, float runwayEndZ);
		void CLEAR_DEFAULT_PRIMARY_TASK(Ped ped);
		/**
		 * This native is very useful when switching the player to a ped inside a vehicle that has a task assigned prior to the player switch.
		 * It is necessary to clear the ped's tasks AND call this native with the vehicle the player is switching into in order to allow the player to control the vehicle after the player switches.
		 */
		void CLEAR_PRIMARY_VEHICLE_TASK(Vehicle vehicle);
		void CLEAR_VEHICLE_CRASH_TASK(Vehicle vehicle);
		void TASK_PLANE_GOTO_PRECISE_VTOL(Ped ped, Vehicle vehicle, float x, float y, float z, int flightHeight, int minHeightAboveTerrain, BOOL useDesiredOrientation, float desiredOrientation, BOOL autopilot);
		/**
		 * Used in am_vehicle_spawn.ysc and am_mp_submarine.ysc.
		 * 
		 * p0 is always 0, p5 is always 1
		 * 
		 * p1 is the vehicle handle of the submarine. Submarine must have a driver, but the ped handle is not passed to the native.
		 * 
		 * Speed can be set by calling SET_DRIVE_TASK_CRUISE_SPEED after
		 */
		void TASK_SUBMARINE_GOTO_AND_STOP(Ped ped, Vehicle submarine, float x, float y, float z, BOOL autopilot);
		/**
		 * Must have targetVehicle, targetPed, OR destination X/Y/Z set
		 * Will follow targeted vehicle/ped, or fly to destination
		 * Set whichever is not being used to 0
		 * 
		 * 
		 * Mission mode type:
		 * - 4, 7: Forces heli to snap to the heading if set, flies to destination or tracks specified entity (mode 4 only works for coordinates, 7 works for coordinates OR ped/vehicle)
		 * - 6: Attacks the target ped/vehicle with mounted weapons. If radius is set, will maintain that distance from target.
		 * - 8: Makes the heli flee from the ped/vehicle/coordinate
		 * - 9: Circles around target ped/vehicle, snaps to angle if set. Behavior flag (last parameter) of 2048 switches from counter-clockwise to clockwise circling. Does not work with coordinate destination.
		 * - 10, 11: Follows ped/vehicle target and imitates target heading. Only works with ped/vehicle target, not coord target
		 * - 19: Heli lands at specified coordinate, ignores heading (lands facing whatever direction it is facing when the task is started)
		 * - 20: Makes the heli land when near target ped. It won't resume chasing.
		 * - 21: Emulates a helicopter crash
		 * - 23: makes the heli circle erratically around ped
		 * 
		 * 
		 * Heli will fly at maxSpeed (up to actual maximum speed defined by the model's handling config)
		 * You can use SET_DRIVE_TASK_CRUISE_SPEED to modulate the speed based on distance to the target without having to re-invoke the task native. Setting to 8.0 when close to the destination results in a much smoother approach.
		 * 
		 * If minHeight and maxHeight are set, heli will fly between those specified elevations, relative to ground level and any obstructions/buildings below. You can specify -1 for either if you only want to specify one. Usually it is easiest to leave maxHeight at -1, and specify a reasonable minHeight to ensure clearance over any obstacles. Note this MUST be passed as an INT, not a FLOAT.
		 * 
		 * Radius affects how closely the heli will follow tracked ped/vehicle, and when circling (mission type 9) sets the radius (in meters) that it will circle the target from
		 * 
		 * Heading is -1.0 for default behavior, which will point the nose of the helicopter towards the destination. Set a heading and the heli will lock to that direction when near its destination/target, but may still turn towards the destination when flying at higher speed from a further distance.
		 * 
		 * Behavior Flags is a bitwise value that modifies the AI behavior. Not clear what all flags do, but here are some guesses/notes:
		 * 1: Forces heading to face E
		 * 2: Unknown
		 * 4: Tight circles around coordinate destination
		 * 8: Unknown
		 * 16: Circles around coordinate destination facing towards destination
		 * 32: Flys to normally, then lands at coordinate destination and stays on the ground (using mission type 4)
		 * 64: Ignores obstacles when flying, will follow at specified minHeight above ground level but will not avoid buildings, vehicles, etc.
		 * 128: Unknown
		 * 256: Unknown
		 * 512: Unknown
		 * 1024: Unknown
		 * 2048: Reverses direction of circling (mission type 9) to clockwise
		 * 4096: Hugs closer to the ground, maintains minHeight from ground generally, but barely clears buildings and dips down more between buildings instead of taking a more efficient/safe route
		 * 8192: Unknown
		 * 
		 * Unk3 is a float value, you may see -1082130432 for this value in decompiled native scripts, this is the equivalent to -1.0f. Seems to affect acceleration/aggressiveness, but not sure exactly how it works. Higher value seems to result in lower acceleration/less aggressive flying. Almost always -1.0 in native scripts, occasionally 20.0 or 50.0. Setting to 400.0 seems to work well for making the pilot not overshoot the destination when using coordinate destination.
		 * 
		 * Notes updated by PNWParksFan, May 2021
		 * 
		 */
		void TASK_HELI_MISSION(Ped pilot, Vehicle aircraft, Vehicle targetVehicle, Ped targetPed, float destinationX, float destinationY, float destinationZ, int missionFlag, float maxSpeed, float radius, float targetHeading, int maxHeight, int minHeight, float slowDownDistance, int behaviorFlags);
		void TASK_HELI_ESCORT_HELI(Ped pilot, Vehicle heli1, Vehicle heli2, float offsetX, float offsetY, float offsetZ);
		/**
		 * EXAMPLE USAGE:
		 * 
		 * Fly around target (Precautiously, keeps high altitude):
		 * Function.Call(Hash.TASK_PLANE_MISSION, pilot, selectedAirplane, 0, 0, Target.X, Target.Y, Target.Z, 4, 100f, 0f, 90f, 0, 200f);
		 * 
		 * Fly around target (Dangerously, keeps VERY low altitude):
		 * Function.Call(Hash.TASK_PLANE_MISSION, pilot, selectedAirplane, 0, 0, Target.X, Target.Y, Target.Z, 4, 100f, 0f, 90f, 0, -500f);
		 * 
		 * Fly directly into target:
		 * Function.Call(Hash.TASK_PLANE_MISSION, pilot, selectedAirplane, 0, 0, Target.X, Target.Y, Target.Z, 4, 100f, 0f, 90f, 0, -5000f);
		 * 
		 * EXPANDED INFORMATION FOR ADVANCED USAGE (custom pilot)
		 * 
		 * 'physicsSpeed': (THIS IS NOT YOUR ORDINARY SPEED PARAMETER: READ!!)
		 * Think of this -first- as a radius value, not a true speed value.  The ACTUAL effective speed of the plane will be that of the maximum speed permissible to successfully fly in a -circle- with a radius of 'physicsSpeed'.  This also means that the plane must complete a circle before it can begin its "bombing run", its straight line pass towards the target.  p9 appears to influence the angle at which a "bombing run" begins, although I can't confirm yet.
		 * 
		 * VERY IMPORTANT: A "bombing run" will only occur if a plane can successfully determine a possible navigable route (the slower the value of 'physicsSpeed', the more precise the pilot can be due to less influence of physics on flightpath).  Otherwise, the pilot will continue to patrol around Destination (be it a dynamic Entity position vector or a fixed world coordinate vector.)
		 * 
		 * 0 = Plane's physics are almost entirely frozen, plane appears to "orbit" around precise destination point
		 * 1-299 = Blend of "frozen, small radius" vs. normal vs. "accelerated, hyperfast, large radius"
		 * 300+ =  Vehicle behaves entirely like a normal gameplay plane.
		 * 
		 * 'patrolBlend' (The lower the value, the more the Destination is treated as a "fly AT" rather than a "fly AROUND point".)
		 * 
		 * Scenario: Destination is an Entity on ground level, wide open field
		 * -5000 = Pilot kamikazes directly into Entity
		 * -1000 = Pilot flies extremely low -around- Entity, very prone to crashing
		 * -200 = Pilot flies lower than average around Entity.
		 * 0 = Pilot flies around Entity, normal altitude
		 * 200 = Pilot flies an extra eighty units or so higher than 0 while flying around Destination (this doesn't seem to correlate directly into distance units.)
		 * 
		 * -- Valid mission types found in the exe: --
		 * 
		 * 0 = None
		 * 1 = Unk
		 * 2 = CTaskVehicleRam
		 * 3 = CTaskVehicleBlock
		 * 4 = CTaskVehicleGoToPlane
		 * 5 = CTaskVehicleStop
		 * 6 = CTaskVehicleAttack
		 * 7 = CTaskVehicleFollow
		 * 8 = CTaskVehicleFleeAirborne
		 * 9= CTaskVehicleCircle
		 * 10 = CTaskVehicleEscort
		 * 15 = CTaskVehicleFollowRecording
		 * 16 = CTaskVehiclePoliceBehaviour
		 * 17 = CTaskVehicleCrash
		 */
		void TASK_PLANE_MISSION(Ped pilot, Vehicle aircraft, Vehicle targetVehicle, Ped targetPed, float destinationX, float destinationY, float destinationZ, int missionFlag, float angularDrag, float targetReached, float targetHeading, float maxZ, float minZ, BOOL precise);
		void TASK_PLANE_TAXI(Ped pilot, Vehicle aircraft, float x, float y, float z, float cruiseSpeed, float targetReached);
		/**
		 * You need to call PED::SET_BLOCKING_OF_NON_TEMPORARY_EVENTS after TASK_BOAT_MISSION in order for the task to execute.
		 * 
		 * Working example
		 * float vehicleMaxSpeed = VEHICLE::GET_VEHICLE_ESTIMATED_MAX_SPEED(ENTITY::GET_ENTITY_MODEL(pedVehicle));
		 * TASK::TASK_BOAT_MISSION(pedDriver, pedVehicle, 0, 0, waypointCoord.x, waypointCoord.y, waypointCoord.z, 4, vehicleMaxSpeed, 786469, -1.0, 7);
		 * PED::SET_BLOCKING_OF_NON_TEMPORARY_EVENTS(pedDriver, 1);
		 * 
		 * P8 appears to be driving style flag - see gtaforums.com/topic/822314-guide-driving-styles/ for documentation
		 */
		void TASK_BOAT_MISSION(Ped pedDriver, Vehicle vehicle, Vehicle targetVehicle, Ped targetPed, float x, float y, float z, int mission, float maxSpeed, int drivingStyle, float targetReached, Any boatFlags);
		/**
		 * Example:
		 * 
		 * TASK::TASK_DRIVE_BY(l_467[1/*22* /], PLAYER::PLAYER_PED_ID(), 0, 0.0, 0.0, 2.0, 300.0, 100, 0, ${firing_pattern_burst_fire_driveby});
		 * 
		 * 
		 * 
		 * 
		 * Needs working example. Doesn't seem to do anything.
		 * 
		 * I marked p2 as targetVehicle as all these shooting related tasks seem to have that in common.
		 * I marked p6 as distanceToShoot as if you think of GTA's Logic with the native SET_VEHICLE_SHOOT natives, it won't shoot till it gets within a certain distance of the target.
		 * I marked p7 as pedAccuracy as it seems it's mostly 100 (Completely Accurate), 75, 90, etc. Although this could be the ammo count within the gun, but I highly doubt it. I will change this comment once I find out if it's ammo count or not.
		 */
		void TASK_DRIVE_BY(Ped driverPed, Ped targetPed, Vehicle targetVehicle, float targetX, float targetY, float targetZ, float distanceToShoot, int pedAccuracy, BOOL pushUnderneathDrivingTaskIfDriving, Hash firingPattern);
		/**
		 * For p1 & p2 (Ped, Vehicle). I could be wrong, as the only time this native is called in scripts is once and both are 0, but I assume this native will work like SET_MOUNTED_WEAPON_TARGET in which has the same exact amount of parameters and the 1st and last 3 parameters are right and the same for both natives.
		 */
		void SET_DRIVEBY_TASK_TARGET(Ped shootingPed, Ped targetPed, Vehicle targetVehicle, float x, float y, float z);
		void CLEAR_DRIVEBY_TASK_UNDERNEATH_DRIVING_TASK(Ped ped);
		BOOL IS_DRIVEBY_TASK_UNDERNEATH_DRIVING_TASK(Ped ped);
		/**
		 * Forces the ped to use the mounted weapon.
		 * Returns false if task is not possible.
		 */
		BOOL CONTROL_MOUNTED_WEAPON(Ped ped);
		/**
		 * Note: Look in decompiled scripts and the times that p1 and p2 aren't 0. They are filled with vars. If you look through out that script what other natives those vars are used in, you can tell p1 is a ped and p2 is a vehicle. Which most likely means if you want the mounted weapon to target a ped set targetVehicle to 0 or vice-versa.
		 */
		void SET_MOUNTED_WEAPON_TARGET(Ped shootingPed, Ped targetPed, Vehicle targetVehicle, float x, float y, float z, int taskMode, BOOL ignoreTargetVehDeadCheck);
		BOOL IS_MOUNTED_WEAPON_TASK_UNDERNEATH_DRIVING_TASK(Ped ped);
		/**
		 * Actually has 3 params, not 2.
		 * 
		 * p0: Ped
		 * p1: int (or bool?)
		 * p2: int
		 */
		void TASK_USE_MOBILE_PHONE(Ped ped, BOOL usePhone, int desiredPhoneMode);
		void TASK_USE_MOBILE_PHONE_TIMED(Ped ped, int duration);
		/**
		 * p2 tend to be 16, 17 or 1
		 * p3 to p7 tend to be 0.0
		 */
		void TASK_CHAT_TO_PED(Ped ped, Ped target, int flags, float goToLocationX, float goToLocationY, float goToLocationZ, float headingDegs, float idleTime);
		/**
		 * Seat Numbers
		 * -------------------------------
		 * Driver = -1
		 * Any = -2
		 * Left-Rear = 1
		 * Right-Front = 0
		 * Right-Rear = 2
		 * Extra seats = 3-14(This may differ from vehicle type e.g. Firetruck Rear Stand, Ambulance Rear)
		 */
		void TASK_WARP_PED_INTO_VEHICLE(Ped ped, Vehicle vehicle, int seat);
		/**
		 * //this part of the code is to determine at which entity the player is aiming, for example if you want to create a mod where you give orders to peds
		 * Entity aimedentity;
		 * Player player = PLAYER::PLAYER_ID();
		 * PLAYER::_GET_AIMED_ENTITY(player, &aimedentity);
		 * 
		 * //bg is an array of peds
		 * TASK::TASK_SHOOT_AT_ENTITY(bg[i], aimedentity, 5000, MISC::GET_HASH_KEY("FIRING_PATTERN_FULL_AUTO"));
		 * 
		 * in practical usage, getting the entity the player is aiming at and then task the peds to shoot at the entity, at a button press event would be better.
		 * 
		 * Firing Pattern Hash Information: https://pastebin.com/Px036isB
		 */
		void TASK_SHOOT_AT_ENTITY(Entity entity, Entity target, int duration, Hash firingPattern);
		/**
		 * Climbs or vaults the nearest thing.
		 * usePlayerLaunchForce is unused.
		 */
		void TASK_CLIMB(Ped ped, BOOL usePlayerLaunchForce);
		void TASK_CLIMB_LADDER(Ped ped, BOOL fast);
		/**
		 * Attaches a ped to a rope and allows player control to rappel down a wall. Disables all collisions while on the rope.
		 * p10: Usually 1 in the scripts, clipSet: Clipset to use for the task, minZ: Minimum Z that the player can descend to, ropeHandle: Rope to attach this task to created with ADD_ROPE
		 */
		void TASK_RAPPEL_DOWN_WALL_USING_CLIPSET_OVERRIDE(Ped ped, float x1, float y1, float z1, float x2, float y2, float z2, float minZ, int ropeHandle, const char* clipSet, Any p10, Any p11);
		int GET_TASK_RAPPEL_DOWN_WALL_STATE(Ped ped);
		/**
		 * Immediately stops the pedestrian from whatever it's doing. They stop fighting, animations, etc. they forget what they were doing.
		 */
		void CLEAR_PED_TASKS_IMMEDIATELY(Ped ped);
		void TASK_PERFORM_SEQUENCE_FROM_PROGRESS(Ped ped, int taskIndex, int progress1, int progress2);
		/**
		 * This native does absolutely nothing, just a nullsub
		 * 
		 * R* Comment:
		 * SET_NEXT_DESIRED_MOVE_STATE - Function is deprecated - do not use anymore
		 */
		void SET_NEXT_DESIRED_MOVE_STATE(float nextMoveState);
		void SET_PED_DESIRED_MOVE_BLEND_RATIO(Ped ped, float newMoveBlendRatio);
		float GET_PED_DESIRED_MOVE_BLEND_RATIO(Ped ped);
		/**
		 * eg
		 * 
		 * TASK::TASK_GOTO_ENTITY_AIMING(v_2, PLAYER::PLAYER_PED_ID(), 5.0, 25.0);
		 * 
		 * ped = Ped you want to perform this task.
		 * target = the Entity they should aim at.
		 * distanceToStopAt = distance from the target, where the ped should stop to aim.
		 * StartAimingDist = distance where the ped should start to aim.
		 */
		void TASK_GOTO_ENTITY_AIMING(Ped ped, Entity target, float distanceToStopAt, float StartAimingDist);
		/**
		 * p1 is always GET_HASH_KEY("empty") in scripts, for the rare times this is used
		 */
		void TASK_SET_DECISION_MAKER(Ped ped, Hash decisionMakerId);
		void TASK_SET_SPHERE_DEFENSIVE_AREA(Ped ped, float x, float y, float z, float radius);
		void TASK_CLEAR_DEFENSIVE_AREA(Ped ped);
		void TASK_PED_SLIDE_TO_COORD(Ped ped, float x, float y, float z, float heading, float speed);
		void TASK_PED_SLIDE_TO_COORD_HDG_RATE(Ped ped, float x, float y, float z, float heading, float speed, float headingChangeRate);
		ScrHandle ADD_COVER_POINT(float x, float y, float z, float direction, int usage, int height, int arc, BOOL isPriority);
		void REMOVE_COVER_POINT(ScrHandle coverpoint);
		/**
		 * Checks if there is a cover point at position
		 */
		BOOL DOES_SCRIPTED_COVER_POINT_EXIST_AT_COORDS(float x, float y, float z);
		Vector3 GET_SCRIPTED_COVER_POINT_COORDS(ScrHandle coverpoint);
		void ADD_SCRIPTED_COVER_AREA(float x, float y, float z, float radius);
		/**
		 * Makes the specified ped attack the target ped.
		 * p2 should be 0
		 * p3 should be 16
		 */
		void TASK_COMBAT_PED(Ped ped, Ped targetPed, int combatFlags, int threatResponseFlags);
		void TASK_COMBAT_PED_TIMED(Ped ped, Ped target, int time, int flags);
		void TASK_SEEK_COVER_FROM_POS(Ped ped, float x, float y, float z, int duration, BOOL allowPeekingAndFiring);
		void TASK_SEEK_COVER_FROM_PED(Ped ped, Ped target, int duration, BOOL allowPeekingAndFiring);
		/**
		 * p5 is always -1
		 */
		void TASK_SEEK_COVER_TO_COVER_POINT(Ped ped, ScrHandle coverpoint, float x, float y, float z, int time, BOOL allowPeekingAndFiring);
		/**
		 * p8 causes the ped to take the shortest route to the cover position. It may have something to do with navmesh or pathfinding mechanics.
		 * 
		 * from michael2:
		 * TASK::TASK_SEEK_COVER_TO_COORDS(ped, 967.5164794921875, -2121.603515625, 30.479299545288086, 978.94677734375, -2125.84130859375, 29.4752, -1, 1);
		 * 
		 * 
		 * appears to be shorter variation
		 * from michael3:
		 * TASK::TASK_SEEK_COVER_TO_COORDS(ped, -2231.011474609375, 263.6326599121094, 173.60195922851562, -1, 0);
		 */
		void TASK_SEEK_COVER_TO_COORDS(Ped ped, float x1, float y1, float z1, float x2, float y2, float z2, int timeout, BOOL shortRoute);
		void TASK_PUT_PED_DIRECTLY_INTO_COVER(Ped ped, float x, float y, float z, int time, BOOL allowPeekingAndFiring, float blendInDuration, BOOL forceInitialFacingDirection, BOOL forceFaceLeft, int identifier, BOOL doEntry);
		void TASK_WARP_PED_DIRECTLY_INTO_COVER(Ped ped, int time, BOOL allowPeekingAndFiring, BOOL forceInitialFacingDirection, BOOL forceFaceLeft, int identifier);
		/**
		 * p1 is 1, 2, or 3 in scripts
		 */
		void TASK_EXIT_COVER(Ped ped, int exitType, float x, float y, float z);
		/**
		 * from armenian3.c4
		 * 
		 * TASK::TASK_PUT_PED_DIRECTLY_INTO_MELEE(PlayerPed, armenianPed, 0.0, -1.0, 0.0, 0);
		 * 
		 */
		void TASK_PUT_PED_DIRECTLY_INTO_MELEE(Ped ped, Ped meleeTarget, float blendInDuration, float timeInMelee, float strafePhaseSync, int aiCombatFlags);
		/**
		 * used in sequence task
		 * 
		 * both parameters seems to be always 0
		 */
		void TASK_TOGGLE_DUCK(Ped ped, int toggleType);
		/**
		 * From re_prisonvanbreak:
		 * 
		 * TASK::TASK_GUARD_CURRENT_POSITION(l_DD, 35.0, 35.0, 1);
		 */
		void TASK_GUARD_CURRENT_POSITION(Ped ped, float maxPatrolProximity, float defensiveAreaRadius, BOOL setDefensiveArea);
		void TASK_GUARD_ASSIGNED_DEFENSIVE_AREA(Ped ped, float x, float y, float z, float heading, float maxPatrolProximity, int timer);
		void TASK_GUARD_SPHERE_DEFENSIVE_AREA(Ped ped, float defendPositionX, float defendPositionY, float defendPositionZ, float heading, float maxPatrolProximity, int time, float x, float y, float z, float defensiveAreaRadius);
		/**
		 * scenarioName example: "WORLD_HUMAN_GUARD_STAND"
		 */
		void TASK_STAND_GUARD(Ped ped, float x, float y, float z, float heading, const char* scenarioName);
		void SET_DRIVE_TASK_CRUISE_SPEED(Ped driver, float cruiseSpeed);
		void SET_DRIVE_TASK_MAX_CRUISE_SPEED(Ped ped, float speed, BOOL updateBaseTask);
		/**
		 * This native is used to set the driving style for specific ped.
		 * 
		 * Driving styles id seems to be:
		 * 786468
		 * 262144
		 * 786469
		 * 
		 * http://gtaforums.com/topic/822314-guide-driving-styles/
		 */
		void SET_DRIVE_TASK_DRIVING_STYLE(Ped ped, int drivingStyle);
		void ADD_COVER_BLOCKING_AREA(float startX, float startY, float startZ, float endX, float endY, float endZ, BOOL blockObjects, BOOL blockVehicles, BOOL blockMap, BOOL blockPlayer);
		void REMOVE_ALL_COVER_BLOCKING_AREAS();
		void REMOVE_COVER_BLOCKING_AREAS_AT_POSITION(float x, float y, float z);
		void REMOVE_SPECIFIC_COVER_BLOCKING_AREAS(float startX, float startY, float startZ, float endX, float endY, float endZ, BOOL blockObjects, BOOL blockVehicles, BOOL blockMap, BOOL blockPlayer);
		/**
		 * Plays a scenario on a Ped at their current location.
		 * 
		 * unkDelay - Usually 0 or -1, doesn't seem to have any effect. Might be a delay between sequences.
		 * playEnterAnim - Plays the "Enter" anim if true, otherwise plays the "Exit" anim. Scenarios that don't have any "Enter" anims won't play if this is set to true.
		 * 
		 * ----
		 * 
		 * From "am_hold_up.ysc.c4" at line 339:
		 * 
		 * TASK::TASK_START_SCENARIO_IN_PLACE(NETWORK::NET_TO_PED(l_8D._f4), sub_adf(), 0, 1);
		 * 
		 * I'm unsure of what the last two parameters are, however sub_adf() randomly returns 1 of 3 scenarios, those being:
		 * WORLD_HUMAN_SMOKING
		 * WORLD_HUMAN_HANG_OUT_STREET
		 * WORLD_HUMAN_STAND_MOBILE
		 * 
		 * This makes sense, as these are what I commonly see when going by a liquor store.
		 * -------------------------
		 * List of scenarioNames: https://pastebin.com/6mrYTdQv
		 * (^ Thank you so fucking much for this)
		 * 
		 * Also these:
		 * WORLD_FISH_FLEE
		 * DRIVE
		 * WORLD_HUMAN_HIKER
		 * WORLD_VEHICLE_ATTRACTOR
		 * WORLD_VEHICLE_BICYCLE_MOUNTAIN
		 * WORLD_VEHICLE_BIKE_OFF_ROAD_RACE
		 * WORLD_VEHICLE_BIKER
		 * WORLD_VEHICLE_CONSTRUCTION_PASSENGERS
		 * WORLD_VEHICLE_CONSTRUCTION_SOLO
		 * WORLD_VEHICLE_DRIVE_PASSENGERS
		 * WORLD_VEHICLE_DRIVE_SOLO
		 * WORLD_VEHICLE_EMPTY
		 * WORLD_VEHICLE_PARK_PARALLEL
		 * WORLD_VEHICLE_PARK_PERPENDICULAR_NOSE_IN
		 * WORLD_VEHICLE_POLICE_BIKE
		 * WORLD_VEHICLE_POLICE_CAR
		 * WORLD_VEHICLE_POLICE_NEXT_TO_CAR
		 * WORLD_VEHICLE_SALTON_DIRT_BIKE
		 * WORLD_VEHICLE_TRUCK_LOGS
		 * 
		 * Full list of ped scenarios by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/scenariosCompact.json
		 */
		void TASK_START_SCENARIO_IN_PLACE(Ped ped, const char* scenarioName, int unkDelay, BOOL playEnterAnim);
		/**
		 * Full list of ped scenarios by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/scenariosCompact.json
		 * 
		 * Also a few more listed at TASK::TASK_START_SCENARIO_IN_PLACE just above.
		 * ---------------
		 * The first parameter in every scenario has always been a Ped of some sort. The second like TASK_START_SCENARIO_IN_PLACE is the name of the scenario.
		 * 
		 * The next 4 parameters were harder to decipher. After viewing "hairdo_shop_mp.ysc.c4", and being confused from seeing the case in other scripts, they passed the first three of the arguments as one array from a function, and it looked like it was obviously x, y, and z.
		 * 
		 * I haven't seen the sixth parameter go to or over 360, making me believe that it is rotation, but I really can't confirm anything.
		 * 
		 * I have no idea what the last 3 parameters are, but I'll try to find out.
		 * 
		 * -going on the last 3 parameters, they appear to always be "0, 0, 1"
		 * 
		 * p6 -1 also used in scrips
		 * 
		 * p7 used for sitting scenarios
		 * 
		 * p8 teleports ped to position
		 */
		void TASK_START_SCENARIO_AT_POSITION(Ped ped, const char* scenarioName, float x, float y, float z, float heading, int duration, BOOL sittingScenario, BOOL teleport);
		/**
		 * Updated variables
		 * 
		 * An alternative to TASK::TASK_USE_NEAREST_SCENARIO_TO_COORD_WARP. Makes the ped walk to the scenario instead.
		 */
		void TASK_USE_NEAREST_SCENARIO_TO_COORD(Ped ped, float x, float y, float z, float distance, int duration);
		void TASK_USE_NEAREST_SCENARIO_TO_COORD_WARP(Ped ped, float x, float y, float z, float radius, int timeToLeave);
		/**
		 * p5 is always 0 in scripts
		 */
		void TASK_USE_NEAREST_SCENARIO_CHAIN_TO_COORD(Ped ped, float x, float y, float z, float maxRange, int timeToLeave);
		/**
		 * p5 is always -1 or 0 in scripts
		 */
		void TASK_USE_NEAREST_SCENARIO_CHAIN_TO_COORD_WARP(Ped ped, float x, float y, float z, float radius, int timeToLeave);
		BOOL DOES_SCENARIO_EXIST_IN_AREA(float x, float y, float z, float radius, BOOL mustBeFree);
		BOOL DOES_SCENARIO_OF_TYPE_EXIST_IN_AREA(float x, float y, float z, const char* scenarioName, float radius, BOOL mustBeFree);
		BOOL IS_SCENARIO_OCCUPIED(float x, float y, float z, float maxRange, BOOL onlyUsersActuallyAtScenario);
		BOOL PED_HAS_USE_SCENARIO_TASK(Ped ped);
		/**
		 * Full list of animation dictionaries and anims by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/animDictsCompact.json
		 */
		void PLAY_ANIM_ON_RUNNING_SCENARIO(Ped ped, const char* animDict, const char* animName);
		/**
		 * Full list of scenario groups used in scripts by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/scenarioGroupNames.json
		 * Occurrences in the b617d scripts:
		 * 
		 * "ARMY_GUARD",
		 * "ARMY_HELI",
		 * "Cinema_Downtown",
		 * "Cinema_Morningwood",
		 * "Cinema_Textile",
		 * "City_Banks",
		 * "Countryside_Banks",
		 * "DEALERSHIP",
		 * "GRAPESEED_PLANES",
		 * "KORTZ_SECURITY",
		 * "LOST_BIKERS",
		 * "LSA_Planes",
		 * "LSA_Planes",
		 * "MP_POLICE",
		 * "Observatory_Bikers",
		 * "POLICE_POUND1",
		 * "POLICE_POUND2",
		 * "POLICE_POUND3",
		 * "POLICE_POUND4",
		 * "POLICE_POUND5"
		 * "QUARRY",
		 * "SANDY_PLANES",
		 * "SCRAP_SECURITY",
		 * "SEW_MACHINE",
		 * "SOLOMON_GATE",
		 * "Triathlon_1_Start",
		 * "Triathlon_2_Start",
		 * "Triathlon_3_Start"
		 * 
		 * Sometimes used with IS_SCENARIO_GROUP_ENABLED:
		 * if (TASK::DOES_SCENARIO_GROUP_EXIST("Observatory_Bikers") && (!TASK::IS_SCENARIO_GROUP_ENABLED("Observatory_Bikers"))) {
		 * else if (TASK::IS_SCENARIO_GROUP_ENABLED("BLIMP")) {
		 * 
		 */
		BOOL DOES_SCENARIO_GROUP_EXIST(const char* scenarioGroup);
		/**
		 * Full list of scenario groups used in scripts by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/scenarioGroupNames.json
		 * Occurrences in the b617d scripts:
		 * 
		 * "ARMY_GUARD",
		 * "ARMY_HELI",
		 * "BLIMP",
		 * "Cinema_Downtown",
		 * "Cinema_Morningwood",
		 * "Cinema_Textile",
		 * "City_Banks",
		 * "Countryside_Banks",
		 * "DEALERSHIP",
		 * "KORTZ_SECURITY",
		 * "LSA_Planes",
		 * "MP_POLICE",
		 * "Observatory_Bikers",
		 * "POLICE_POUND1",
		 * "POLICE_POUND2",
		 * "POLICE_POUND3",
		 * "POLICE_POUND4",
		 * "POLICE_POUND5",
		 * "Rampage1",
		 * "SANDY_PLANES",
		 * "SCRAP_SECURITY",
		 * "SEW_MACHINE",
		 * "SOLOMON_GATE"
		 * 
		 * Sometimes used with DOES_SCENARIO_GROUP_EXIST:
		 * if (TASK::DOES_SCENARIO_GROUP_EXIST("Observatory_Bikers") &&   (!TASK::IS_SCENARIO_GROUP_ENABLED("Observatory_Bikers"))) {
		 * else if (TASK::IS_SCENARIO_GROUP_ENABLED("BLIMP")) {
		 */
		BOOL IS_SCENARIO_GROUP_ENABLED(const char* scenarioGroup);
		/**
		 * Full list of scenario groups used in scripts by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/scenarioGroupNames.json
		 * Occurrences in the b617d scripts: https://pastebin.com/Tvg2PRHU
		 */
		void SET_SCENARIO_GROUP_ENABLED(const char* scenarioGroup, BOOL enabled);
		void RESET_SCENARIO_GROUPS_ENABLED();
		/**
		 * Full list of scenario groups used in scripts by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/scenarioGroupNames.json
		 * Groups found in the scripts used with this native:
		 * 
		 * "AMMUNATION",
		 * "QUARRY",
		 * "Triathlon_1",
		 * "Triathlon_2",
		 * "Triathlon_3"
		 */
		void SET_EXCLUSIVE_SCENARIO_GROUP(const char* scenarioGroup);
		void RESET_EXCLUSIVE_SCENARIO_GROUP();
		/**
		 * Full list of scenario types used in scripts by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/scenariosCompact.json
		 * Occurrences in the b617d scripts:
		 * "PROP_HUMAN_SEAT_CHAIR",
		 * "WORLD_HUMAN_DRINKING",
		 * "WORLD_HUMAN_HANG_OUT_STREET",
		 * "WORLD_HUMAN_SMOKING",
		 * "WORLD_MOUNTAIN_LION_WANDER",
		 * "WORLD_HUMAN_DRINKING"
		 * 
		 * Sometimes used together with MISC::IS_STRING_NULL_OR_EMPTY in the scripts.
		 * 
		 * scenarioType could be the same as scenarioName, used in for example TASK::TASK_START_SCENARIO_AT_POSITION.
		 * 
		 */
		BOOL IS_SCENARIO_TYPE_ENABLED(const char* scenarioType);
		/**
		 * Full list of scenario types used in scripts by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/scenariosCompact.json
		 * seems to enable/disable specific scenario-types from happening in the game world.
		 * 
		 * Here are some scenario types from the scripts:
		 * "WORLD_MOUNTAIN_LION_REST"
		 * "WORLD_MOUNTAIN_LION_WANDER"
		 * "DRIVE"
		 * "WORLD_VEHICLE_POLICE_BIKE"
		 * "WORLD_VEHICLE_POLICE_CAR"
		 * "WORLD_VEHICLE_POLICE_NEXT_TO_CAR"
		 * "WORLD_VEHICLE_DRIVE_SOLO"
		 * "WORLD_VEHICLE_BIKER"
		 * "WORLD_VEHICLE_DRIVE_PASSENGERS"
		 * "WORLD_VEHICLE_SALTON_DIRT_BIKE"
		 * "WORLD_VEHICLE_BICYCLE_MOUNTAIN"
		 * "PROP_HUMAN_SEAT_CHAIR"
		 * "WORLD_VEHICLE_ATTRACTOR"
		 * "WORLD_HUMAN_LEANING"
		 * "WORLD_HUMAN_HANG_OUT_STREET"
		 * "WORLD_HUMAN_DRINKING"
		 * "WORLD_HUMAN_SMOKING"
		 * "WORLD_HUMAN_GUARD_STAND"
		 * "WORLD_HUMAN_CLIPBOARD"
		 * "WORLD_HUMAN_HIKER"
		 * "WORLD_VEHICLE_EMPTY"
		 * "WORLD_VEHICLE_BIKE_OFF_ROAD_RACE"
		 * "WORLD_HUMAN_PAPARAZZI"
		 * "WORLD_VEHICLE_PARK_PERPENDICULAR_NOSE_IN"
		 * "WORLD_VEHICLE_PARK_PARALLEL"
		 * "WORLD_VEHICLE_CONSTRUCTION_SOLO"
		 * "WORLD_VEHICLE_CONSTRUCTION_PASSENGERS"
		 * "WORLD_VEHICLE_TRUCK_LOGS"
		 * 
		 * scenarioType could be the same as scenarioName, used in for example TASK::TASK_START_SCENARIO_AT_POSITION.
		 */
		void SET_SCENARIO_TYPE_ENABLED(const char* scenarioType, BOOL toggle);
		void RESET_SCENARIO_TYPES_ENABLED();
		BOOL IS_PED_ACTIVE_IN_SCENARIO(Ped ped);
		/**
		 * Used only once (am_mp_property_int)
		 * 
		 * ped was PLAYER_PED_ID()
		 * 
		 * Related to CTaskAmbientClips.
		 */
		BOOL IS_PED_PLAYING_BASE_CLIP_IN_SCENARIO(Ped ped);
		/**
		 * Appears only in fm_mission_controller and used only 3 times.
		 * 
		 * ped was always PLAYER_PED_ID()
		 * p1 was always true
		 * p2 was always true
		 */
		void SET_PED_CAN_PLAY_AMBIENT_IDLES(Ped ped, BOOL blockIdleClips, BOOL removeIdleClipIfPlaying);
		/**
		 * Despite its name, it only attacks ONE hated target. The one closest to the specified position.
		 */
		void TASK_COMBAT_HATED_TARGETS_IN_AREA(Ped ped, float x, float y, float z, float radius, int combatFlags);
		/**
		 * Despite its name, it only attacks ONE hated target. The one closest hated target.
		 * 
		 * p2 seems to be always 0
		 */
		void TASK_COMBAT_HATED_TARGETS_AROUND_PED(Ped ped, float radius, int combatFlags);
		void TASK_COMBAT_HATED_TARGETS_AROUND_PED_TIMED(Ped ped, float radius, int time, int combatFlags);
		/**
		 * In every case of this native, I've only seen the first parameter passed as 0, although I believe it's a Ped after seeing tasks around it using 0. That's because it's used in a Sequence Task.
		 * 
		 * The last 3 parameters are definitely coordinates after seeing them passed in other scripts, and even being used straight from the player's coordinates.
		 * ---
		 * It seems that - in the decompiled scripts - this native was used on a ped who was in a vehicle to throw a projectile out the window at the player. This is something any ped will naturally do if they have a throwable and they are doing driveby-combat (although not very accurately).
		 * It is possible, however, that this is how SWAT throws smoke grenades at the player when in cover.
		 * ----------------------------------------------------
		 * The first comment is right it definately is the ped as if you look in script finale_heist2b.c line 59628 in Xbox Scripts atleast you will see task_throw_projectile and the first param is Local_559[2 <14>] if you look above it a little bit line 59622 give_weapon_to_ped uses the same exact param Local_559[2 <14>] and we all know the first param of that native is ped. So it guaranteed has to be ped. 0 just may mean to use your ped by default for some reason.
		 */
		void TASK_THROW_PROJECTILE(Ped ped, float x, float y, float z, int ignoreCollisionEntityIndex, BOOL createInvincibleProjectile);
		void TASK_SWAP_WEAPON(Ped ped, BOOL drawWeapon);
		/**
		 * The 2nd param (drawWeapon) is not implemented.
		 * 
		 * -----------------------------------------------------------------------
		 * 
		 * The only occurrence I found in a R* script ("assassin_construction.ysc.c4"):
		 * 
		 * if (((v_3 < v_4) && (TASK::GET_SCRIPT_TASK_STATUS(PLAYER::PLAYER_PED_ID(), 0x6a67a5cc) != 1)) && (v_5 > v_3)) {
		 * TASK::TASK_RELOAD_WEAPON(PLAYER::PLAYER_PED_ID(), 1);
		 * }
		 */
		void TASK_RELOAD_WEAPON(Ped ped, BOOL drawWeapon);
		BOOL IS_PED_GETTING_UP(Ped ped);
		/**
		 * EX: Function.Call(Ped1, Ped2, Time, 0);
		 * 
		 * The last parameter is always 0 for some reason I do not know. The first parameter is the pedestrian who will writhe to the pedestrian in the other parameter. The third paremeter is how long until the Writhe task ends. When the task ends, the ped will die. If set to -1, he will not die automatically, and the task will continue until something causes it to end. This can be being touched by an entity, being shot, explosion, going into ragdoll, having task cleared. Anything that ends the current task will kill the ped at this point.
		 * 
		 * 
		 * 
		 * Third parameter does not appear to be time. The last parameter is not implemented (It's not used, regardless of value).
		 */
		void TASK_WRITHE(Ped ped, Ped target, int minFireLoops, int startState, BOOL forceShootOnGround, int shootFromGroundTimer);
		/**
		 * This native checks if a ped is on the ground, in pain from a (gunshot) wound.
		 * Returns `true` if the ped is in writhe, `false` otherwise.
		 */
		BOOL IS_PED_IN_WRITHE(Ped ped);
		/**
		 * patrolRoutes found in the b617d scripts:
		 * "miss_Ass0",
		 * "miss_Ass1",
		 * "miss_Ass2",
		 * "miss_Ass3",
		 * "miss_Ass4",
		 * "miss_Ass5",
		 * "miss_Ass6",
		 * "MISS_PATROL_6",
		 * "MISS_PATROL_7",
		 * "MISS_PATROL_8",
		 * "MISS_PATROL_9",
		 * "miss_Tower_01",
		 * "miss_Tower_02",
		 * "miss_Tower_03",
		 * "miss_Tower_04",
		 * "miss_Tower_05",
		 * "miss_Tower_06",
		 * "miss_Tower_07",
		 * "miss_Tower_08",
		 * "miss_Tower_10"
		 */
		void OPEN_PATROL_ROUTE(const char* patrolRoute);
		void CLOSE_PATROL_ROUTE();
		/**
		 * Example:
		 * TASK::ADD_PATROL_ROUTE_NODE(2, "WORLD_HUMAN_GUARD_STAND", -193.4915, -2378.864990234375, 10.9719, -193.4915, -2378.864990234375, 10.9719, 3000);
		 * 
		 * p0 is between 0 and 4 in the scripts.
		 * 
		 * p1 is "WORLD_HUMAN_GUARD_STAND" or "StandGuard".
		 * 
		 * p2, p3 and p4 is only one parameter sometimes in the scripts. Most likely a Vector3 hence p2, p3 and p4 are coordinates.
		 * Examples:
		 * TASK::ADD_PATROL_ROUTE_NODE(1, "WORLD_HUMAN_GUARD_STAND", l_739[7/*3* /], 0.0, 0.0, 0.0, 0);
		 * 
		 * TASK::ADD_PATROL_ROUTE_NODE(1, "WORLD_HUMAN_GUARD_STAND", l_B0[17/*44* /]._f3, l_B0[17/*44* /]._f3, 2000);
		 * 
		 * p5, p6 and p7 are for example set to: 1599.0406494140625, 2713.392578125, 44.4309.
		 * 
		 * p8 is an int, often random set to for example: MISC::GET_RANDOM_INT_IN_RANGE(5000, 10000).
		 */
		void ADD_PATROL_ROUTE_NODE(int nodeId, const char* nodeType, float posX, float posY, float posZ, float headingX, float headingY, float headingZ, int duration);
		void ADD_PATROL_ROUTE_LINK(int nodeId1, int nodeId2);
		void CREATE_PATROL_ROUTE();
		/**
		 * From the b617d scripts:
		 * 
		 * TASK::DELETE_PATROL_ROUTE("miss_merc0");
		 * TASK::DELETE_PATROL_ROUTE("miss_merc1");
		 * TASK::DELETE_PATROL_ROUTE("miss_merc2");
		 * TASK::DELETE_PATROL_ROUTE("miss_dock");
		 */
		void DELETE_PATROL_ROUTE(const char* patrolRoute);
		BOOL GET_PATROL_TASK_INFO(Ped ped, int* timeLeftAtNode, int* nodeId);
		/**
		 * After looking at some scripts the second parameter seems to be an id of some kind. Here are some I found from some R* scripts:
		 * 
		 * "miss_Tower_01" (this went from 01 - 10)
		 * "miss_Ass0" (0, 4, 6, 3)
		 * "MISS_PATROL_8"
		 * 
		 * I think they're patrol routes, but I'm not sure. And I believe the 3rd parameter is a BOOL, but I can't confirm other than only seeing 0 and 1 being passed.
		 * 
		 * 
		 * As far as I can see the patrol routes names such as "miss_Ass0" have been defined earlier in the scripts. This leads me to believe we can defined our own new patrol routes by following the same approach.
		 * From the scripts
		 * 
		 * TASK::OPEN_PATROL_ROUTE("miss_Ass0");
		 * TASK::ADD_PATROL_ROUTE_NODE(0, "WORLD_HUMAN_GUARD_STAND", l_738[0/*3* /], -139.4076690673828, -993.4732055664062, 26.2754, MISC::GET_RANDOM_INT_IN_RANGE(5000, 10000));
		 * TASK::ADD_PATROL_ROUTE_NODE(1, "WORLD_HUMAN_GUARD_STAND", l_738[1/*3* /], -116.1391830444336, -987.4984130859375, 26.38541030883789, MISC::GET_RANDOM_INT_IN_RANGE(5000, 10000));
		 * TASK::ADD_PATROL_ROUTE_NODE(2, "WORLD_HUMAN_GUARD_STAND", l_738[2/*3* /], -128.46847534179688, -979.0340576171875, 26.2754, MISC::GET_RANDOM_INT_IN_RANGE(5000, 10000));
		 * TASK::ADD_PATROL_ROUTE_LINK(0, 1);
		 * TASK::ADD_PATROL_ROUTE_LINK(1, 2);
		 * TASK::ADD_PATROL_ROUTE_LINK(2, 0);
		 * TASK::CLOSE_PATROL_ROUTE();
		 * TASK::CREATE_PATROL_ROUTE();
		 * 
		 * 
		 */
		void TASK_PATROL(Ped ped, const char* patrolRouteName, int alertState, BOOL canChatToPeds, BOOL useHeadLookAt);
		/**
		 * Makes the ped run to take cover
		 */
		void TASK_STAY_IN_COVER(Ped ped);
		/**
		 * x, y, z: offset in world coords from some entity.
		 */
		void ADD_VEHICLE_SUBTASK_ATTACK_COORD(Ped ped, float x, float y, float z);
		void ADD_VEHICLE_SUBTASK_ATTACK_PED(Ped ped, Ped target);
		void TASK_VEHICLE_SHOOT_AT_PED(Ped ped, Ped target, float fireTolerance);
		void TASK_VEHICLE_AIM_AT_PED(Ped ped, Ped target);
		void TASK_VEHICLE_SHOOT_AT_COORD(Ped ped, float x, float y, float z, float fireTolerance);
		void TASK_VEHICLE_AIM_AT_COORD(Ped ped, float x, float y, float z);
		/**
		 * Differs from TASK_VEHICLE_DRIVE_TO_COORDS in that it will pick the shortest possible road route without taking one-way streets and other "road laws" into consideration.
		 * 
		 * WARNING:
		 * A behaviorFlag value of 0 will result in a clunky, stupid driver!
		 * 
		 * Recommended settings:
		 * speed = 30.0f,
		 * behaviorFlag = 156,
		 * stoppingRange = 5.0f;
		 * 
		 * If you simply want to have your driver move to a fixed location, call it only once, or, when necessary in the event of interruption.
		 * 
		 * If using this to continually follow a Ped who is on foot:  You will need to run this in a tick loop.  Call it in with the Ped's updated coordinates every 20 ticks or so and you will have one hell of a smart, fast-reacting NPC driver -- provided he doesn't get stuck.  If your update frequency is too fast, the Ped may not have enough time to figure his way out of being stuck, and thus, remain stuck.  One way around this would be to implement an "anti-stuck" mechanism, which allows the driver to realize he's stuck, temporarily pause the tick, unstuck, then resume the tick.
		 * 
		 * EDIT:  This is being discussed in more detail at http://gtaforums.com/topic/818504-any-idea-on-how-to-make-peds-clever-and-insanely-fast-c/
		 */
		void TASK_VEHICLE_GOTO_NAVMESH(Ped ped, Vehicle vehicle, float x, float y, float z, float speed, int behaviorFlag, float stoppingRange);
		/**
		 * movement_speed: mostly 2f, but also 1/1.2f, etc.
		 * p8: always false
		 * p9: 2f
		 * p10: 0.5f
		 * p11: true
		 * p12: 0 / 512 / 513, etc.
		 * p13: 0
		 * firing_pattern: ${firing_pattern_full_auto}, 0xC6EE6B4C
		 */
		void TASK_GO_TO_COORD_WHILE_AIMING_AT_COORD(Ped ped, float x, float y, float z, float aimAtX, float aimAtY, float aimAtZ, float moveBlendRatio, BOOL shoot, float targetRadius, float slowDistance, BOOL useNavMesh, int navFlags, BOOL instantBlendToAim, Hash firingPattern);
		void TASK_GO_TO_COORD_WHILE_AIMING_AT_ENTITY(Ped ped, float x, float y, float z, Entity aimAtID, float moveBlendRatio, BOOL shoot, float targetRadius, float slowDistance, BOOL useNavMesh, int navFlags, BOOL instantBlendToAim, Hash firingPattern, int time);
		/**
		 * The ped will walk or run towards goToLocation, aiming towards goToLocation or focusLocation (depending on the aimingFlag) and shooting if shootAtEnemies = true to any enemy in his path.
		 * 
		 * If the ped is closer than noRoadsDistance, the ped will ignore pathing/navmesh and go towards goToLocation directly. This could cause the ped to get stuck behind tall walls if the goToLocation is on the other side. To avoid this, use 0.0f and the ped will always use pathing/navmesh to reach his destination.
		 * 
		 * If the speed is set to 0.0f, the ped will just stand there while aiming, if set to 1.0f he will walk while aiming, 2.0f will run while aiming.
		 * 
		 * The ped will stop aiming when he is closer than distanceToStopAt to goToLocation.
		 * 
		 * I still can't figure out what unkTrue is used for. I don't notice any difference if I set it to false but in the decompiled scripts is always true.
		 * 
		 * I think that unkFlag, like the driving styles, could be a flag that "work as a list of 32 bits converted to a decimal integer. Each bit acts as a flag, and enables or disables a function". What leads me to this conclusion is the fact that in the decompiled scripts, unkFlag takes values like: 0, 1, 5 (101 in binary) and 4097 (4096 + 1 or 1000000000001 in binary). For now, I don't know what behavior enable or disable this possible flag so I leave it at 0.
		 * 
		 * Note: After some testing, using unkFlag = 16 (0x10) enables the use of sidewalks while moving towards goToLocation.
		 * 
		 * The aimingFlag takes 2 values: 0 to aim at the focusLocation, 1 to aim at where the ped is heading (goToLocation).
		 * 
		 * Example:
		 * 
		 * enum AimFlag
		 * {
		 * AimAtFocusLocation,
		 * AimAtGoToLocation
		 * };
		 * 
		 * Vector3 goToLocation1 = { 996.2867f, 0, -2143.044f, 0, 28.4763f, 0 }; // remember the padding.
		 * 
		 * Vector3 goToLocation2 = { 990.2867f, 0, -2140.044f, 0, 28.4763f, 0 }; // remember the padding.
		 * 
		 * Vector3 focusLocation = { 994.3478f, 0, -2136.118f, 0, 29.2463f, 0 }; // the coord z should be a little higher, around +1.0f to avoid aiming at the ground
		 * 
		 * // 1st example
		 * TASK::TASK_GO_TO_COORD_AND_AIM_AT_HATED_ENTITIES_NEAR_COORD(pedHandle, goToLocation1.x, goToLocation1.y, goToLocation1.z, focusLocation.x, focusLocation.y, focusLocation.z, 2.0f /*run* /, true /*shoot* /, 3.0f /*stop at* /, 0.0f /*noRoadsDistance* /, true /*always true* /, 0 /*possible flag* /, AimFlag::AimAtGoToLocation, -957453492 /*FullAuto pattern* /);
		 * 
		 * // 2nd example
		 * TASK::TASK_GO_TO_COORD_AND_AIM_AT_HATED_ENTITIES_NEAR_COORD(pedHandle, goToLocation2.x, goToLocation2.y, goToLocation2.z, focusLocation.x, focusLocation.y, focusLocation.z, 1.0f /*walk* /, false /*don't shoot* /, 3.0f /*stop at* /, 0.0f /*noRoadsDistance* /, true /*always true* /, 0 /*possible flag* /, AimFlag::AimAtFocusLocation, -957453492 /*FullAuto pattern* /);
		 * 
		 * 
		 * 1st example: The ped (pedhandle) will run towards goToLocation1. While running and aiming towards goToLocation1, the ped will shoot on sight to any enemy in his path, using "FullAuto" firing pattern. The ped will stop once he is closer than distanceToStopAt to goToLocation1.
		 * 
		 * 2nd example: The ped will walk towards goToLocation2. This time, while walking towards goToLocation2 and aiming at focusLocation, the ped will point his weapon on sight to any enemy in his path without shooting. The ped will stop once he is closer than distanceToStopAt to goToLocation2.
		 */
		void TASK_GO_TO_COORD_AND_AIM_AT_HATED_ENTITIES_NEAR_COORD(Ped pedHandle, float goToLocationX, float goToLocationY, float goToLocationZ, float focusLocationX, float focusLocationY, float focusLocationZ, float speed, BOOL shootAtEnemies, float distanceToStopAt, float noRoadsDistance, BOOL useNavMesh, int navFlags, int taskFlags, Hash firingPattern);
		void TASK_GO_TO_ENTITY_WHILE_AIMING_AT_COORD(Ped ped, Entity entity, float aimX, float aimY, float aimZ, float moveBlendRatio, BOOL shoot, float targetRadius, float slowDistance, BOOL useNavMesh, BOOL instantBlendToAim, Hash firingPattern);
		/**
		 * shootatEntity:
		 * If true, peds will shoot at Entity till it is dead.
		 * If false, peds will just walk till they reach the entity and will cease shooting.
		 */
		void TASK_GO_TO_ENTITY_WHILE_AIMING_AT_ENTITY(Ped ped, Entity entityToWalkTo, Entity entityToAimAt, float speed, BOOL shootatEntity, float targetRadius, float slowDistance, BOOL useNavMesh, BOOL instantBlendToAim, Hash firingPattern);
		/**
		 * Makes the ped ragdoll like when falling from a great height
		 */
		void SET_HIGH_FALL_TASK(Ped ped, int minTime, int maxTime, int entryType);
		/**
		 * Full list of waypoint recordings by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/waypointRecordings.json
		 * For a full list of the points, see here: goo.gl/wIH0vn
		 * 
		 * Max number of loaded recordings is 32.
		 */
		void REQUEST_WAYPOINT_RECORDING(const char* name);
		/**
		 * Full list of waypoint recordings by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/waypointRecordings.json
		 */
		BOOL GET_IS_WAYPOINT_RECORDING_LOADED(const char* name);
		/**
		 * Full list of waypoint recordings by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/waypointRecordings.json
		 */
		void REMOVE_WAYPOINT_RECORDING(const char* name);
		/**
		 * Full list of waypoint recordings by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/waypointRecordings.json
		 * For a full list of the points, see here: goo.gl/wIH0vn
		 */
		BOOL WAYPOINT_RECORDING_GET_NUM_POINTS(const char* name, int* points);
		/**
		 * Full list of waypoint recordings by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/waypointRecordings.json
		 * For a full list of the points, see here: goo.gl/wIH0vn
		 */
		BOOL WAYPOINT_RECORDING_GET_COORD(const char* name, int point, Vector3* coord);
		/**
		 * Full list of waypoint recordings by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/waypointRecordings.json
		 */
		float WAYPOINT_RECORDING_GET_SPEED_AT_POINT(const char* name, int point);
		/**
		 * Full list of waypoint recordings by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/waypointRecordings.json
		 * For a full list of the points, see here: goo.gl/wIH0vn
		 */
		BOOL WAYPOINT_RECORDING_GET_CLOSEST_WAYPOINT(const char* name, float x, float y, float z, int* point);
		void TASK_FOLLOW_WAYPOINT_RECORDING(Ped ped, const char* name, int p2, int p3, int p4);
		BOOL IS_WAYPOINT_PLAYBACK_GOING_ON_FOR_PED(Ped ped);
		int GET_PED_WAYPOINT_PROGRESS(Ped ped);
		void SET_PED_WAYPOINT_PROGRESS(Ped ped, int progress); // Missing in crossmap
		float GET_PED_WAYPOINT_DISTANCE(Any p0);
		BOOL SET_PED_WAYPOINT_ROUTE_OFFSET(Ped ped, float x, float y, float z);
		float GET_WAYPOINT_DISTANCE_ALONG_ROUTE(const char* name, int point);
		BOOL WAYPOINT_PLAYBACK_GET_IS_PAUSED(Any p0);
		void WAYPOINT_PLAYBACK_PAUSE(Any p0, BOOL p1, BOOL p2);
		void WAYPOINT_PLAYBACK_RESUME(Any p0, BOOL p1, Any p2, Any p3);
		void WAYPOINT_PLAYBACK_OVERRIDE_SPEED(Any p0, float p1, BOOL p2);
		void WAYPOINT_PLAYBACK_USE_DEFAULT_SPEED(Any p0);
		void USE_WAYPOINT_RECORDING_AS_ASSISTED_MOVEMENT_ROUTE(const char* name, BOOL p1, float p2, float p3);
		void WAYPOINT_PLAYBACK_START_AIMING_AT_PED(Ped ped, Ped target, BOOL p2);
		void WAYPOINT_PLAYBACK_START_AIMING_AT_COORD(Ped ped, float x, float y, float z, BOOL p4);
		void WAYPOINT_PLAYBACK_START_SHOOTING_AT_PED(Ped ped, Ped ped2, BOOL p2, BOOL p3);
		void WAYPOINT_PLAYBACK_START_SHOOTING_AT_COORD(Ped ped, float x, float y, float z, BOOL p4, Hash firingPattern);
		void WAYPOINT_PLAYBACK_STOP_AIMING_OR_SHOOTING(Ped ped);
		/**
		 * Routes: "1_FIBStairs", "2_FIBStairs", "3_FIBStairs", "4_FIBStairs", "5_FIBStairs", "5_TowardsFire", "6a_FIBStairs", "7_FIBStairs", "8_FIBStairs", "Aprtmnt_1", "AssAfterLift", "ATM_1", "coroner2", "coroner_stairs", "f5_jimmy1", "fame1", "family5b", "family5c", "Family5d", "family5d", "FIB_Glass1", "FIB_Glass2", "FIB_Glass3", "finaBroute1A", "finalb1st", "finalB1sta", "finalbround", "finalbroute2", "Hairdresser1", "jan_foyet_ft_door", "Jo_3", "Lemar1", "Lemar2", "mansion_1", "Mansion_1", "pols_1", "pols_2", "pols_3", "pols_4", "pols_5", "pols_6", "pols_7", "pols_8", "Pro_S1", "Pro_S1a", "Pro_S2", "Towards_case", "trev_steps", "tunrs1", "tunrs2", "tunrs3", "Wave01457s"
		 */
		void ASSISTED_MOVEMENT_REQUEST_ROUTE(const char* route);
		void ASSISTED_MOVEMENT_REMOVE_ROUTE(const char* route);
		BOOL ASSISTED_MOVEMENT_IS_ROUTE_LOADED(const char* route);
		void ASSISTED_MOVEMENT_SET_ROUTE_PROPERTIES(const char* route, int props);
		void ASSISTED_MOVEMENT_OVERRIDE_LOAD_DISTANCE_THIS_FRAME(float dist);
		/**
		 * 
		 * 
		 * p2 = Waypoint recording string (found in update\update.rpf\x64\levels\gta5\waypointrec.rpf
		 * p3 = 786468
		 * p4 = 0
		 * p5 = 16
		 * p6 = -1 (angle?)
		 * p7/8/9 = usually v3.zero
		 * p10 = bool (repeat?)
		 * p11 = 1073741824
		 * 
		 * Full list of waypoint recordings by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/waypointRecordings.json
		 */
		void TASK_VEHICLE_FOLLOW_WAYPOINT_RECORDING(Ped ped, Vehicle vehicle, const char* WPRecording, int p3, int p4, int p5, int p6, float p7, BOOL p8, float p9);
		BOOL IS_WAYPOINT_PLAYBACK_GOING_ON_FOR_VEHICLE(Vehicle vehicle);
		int GET_VEHICLE_WAYPOINT_PROGRESS(Vehicle vehicle);
		int GET_VEHICLE_WAYPOINT_TARGET_POINT(Vehicle vehicle);
		void VEHICLE_WAYPOINT_PLAYBACK_PAUSE(Vehicle vehicle);
		void VEHICLE_WAYPOINT_PLAYBACK_GET_IS_PAUSED(Vehicle vehicle); // Missing in crossmap
		void VEHICLE_WAYPOINT_PLAYBACK_RESUME(Vehicle vehicle);
		void VEHICLE_WAYPOINT_PLAYBACK_USE_DEFAULT_SPEED(Vehicle vehicle);
		void VEHICLE_WAYPOINT_PLAYBACK_OVERRIDE_SPEED(Vehicle vehicle, float speed);
		/**
		 * I cant believe I have to define this, this is one of the best natives.
		 * 
		 * It makes the ped ignore basically all shocking events around it. Occasionally the ped may comment or gesture, but other than that they just continue their daily activities. This includes shooting and wounding the ped. And - most importantly - they do not flee.
		 * 
		 * Since it is a task, every time the native is called the ped will stop for a moment.
		 */
		void TASK_SET_BLOCKING_OF_NON_TEMPORARY_EVENTS(Ped ped, BOOL toggle);
		/**
		 * p2 always false
		 * 
		 * [30/03/2017] ins1de :
		 * 
		 * See FORCE_PED_MOTION_STATE
		 */
		void TASK_FORCE_MOTION_STATE(Ped ped, Hash state, BOOL forceRestart);
		/**
		 * Example:
		 * TASK::TASK_MOVE_NETWORK_BY_NAME(PLAYER::PLAYER_PED_ID(), "arm_wrestling_sweep_paired_a_rev3", 0.0f, true, "mini@arm_wrestling", 0);
		 */
		void TASK_MOVE_NETWORK_BY_NAME(Ped ped, const char* task, float multiplier, BOOL allowOverrideCloneUpdate, const char* animDict, int flags);
		/**
		 * Example:
		 * TASK::TASK_MOVE_NETWORK_ADVANCED_BY_NAME(PLAYER::PLAYER_PED_ID(), "minigame_tattoo_michael_parts", 324.13f, 181.29f, 102.6f, 0.0f, 0.0f, 22.32f, 2, 0, false, 0, 0);
		 */
		void TASK_MOVE_NETWORK_ADVANCED_BY_NAME(Ped ped, const char* network, float x, float y, float z, float rotX, float rotY, float rotZ, int rotOrder, float blendDuration, BOOL allowOverrideCloneUpdate, const char* animDict, int flags);
		/**
		 * Used only once in the scripts (am_mp_nightclub)
		 */
		void TASK_MOVE_NETWORK_BY_NAME_WITH_INIT_PARAMS(Ped ped, const char* network, int* initialParameters, float blendDuration, BOOL allowOverrideCloneUpdate, const char* animDict, int flags);
		void TASK_MOVE_NETWORK_ADVANCED_BY_NAME_WITH_INIT_PARAMS(Ped ped, const char* network, int* initialParameters, float x, float y, float z, float rotX, float rotY, float rotZ, int rotOrder, float blendDuration, BOOL allowOverrideCloneUpdate, const char* dictionary, int flags);
		BOOL IS_TASK_MOVE_NETWORK_ACTIVE(Ped ped);
		BOOL IS_TASK_MOVE_NETWORK_READY_FOR_TRANSITION(Ped ped);
		BOOL REQUEST_TASK_MOVE_NETWORK_STATE_TRANSITION(Ped ped, const char* name);
		/**
		 * Used only once in the scripts (fm_mission_controller) like so:
		 * 
		 * TASK::SET_EXPECTED_CLONE_NEXT_TASK_MOVE_NETWORK_STATE(iLocal_3160, "Cutting");
		 */
		BOOL SET_EXPECTED_CLONE_NEXT_TASK_MOVE_NETWORK_STATE(Ped ped, const char* state);
		const char* GET_TASK_MOVE_NETWORK_STATE(Ped ped);
		void SET_TASK_MOVE_NETWORK_ANIM_SET(Ped ped, Hash clipSet, Hash variableClipSet);
		/**
		 * signalName - "Phase", "Wobble", "x_axis","y_axis","introphase","speed".
		 * p2 - From what i can see it goes up to 1f (maybe).
		 * 
		 * Example: TASK::SET_TASK_MOVE_NETWORK_SIGNAL_FLOAT(PLAYER::PLAYER_PED_ID(), "Phase", 0.5);
		 */
		void SET_TASK_MOVE_NETWORK_SIGNAL_FLOAT(Ped ped, const char* signalName, float value);
		void SET_TASK_MOVE_NETWORK_SIGNAL_LOCAL_FLOAT(Ped ped, const char* signalName, float value);
		void SET_TASK_MOVE_NETWORK_SIGNAL_FLOAT_LERP_RATE(Ped ped, const char* signalName, float value);
		void SET_TASK_MOVE_NETWORK_SIGNAL_BOOL(Ped ped, const char* signalName, BOOL value);
		float GET_TASK_MOVE_NETWORK_SIGNAL_FLOAT(Ped ped, const char* signalName);
		BOOL GET_TASK_MOVE_NETWORK_SIGNAL_BOOL(Ped ped, const char* signalName);
		BOOL GET_TASK_MOVE_NETWORK_EVENT(Ped ped, const char* eventName);
		/**
		 * Doesn't actually return anything.
		 */
		BOOL SET_TASK_MOVE_NETWORK_ENABLE_COLLISION_ON_NETWORK_CLONE_WHEN_FIXED(Ped ped, BOOL enable);
		void _SET_SCRIPT_TASK_ENABLE_COLLISION_ON_NETWORK_CLONE_WHEN_FIXED(Ped ped, BOOL enable);
		void _SET_AMBIENT_PED_ENABLE_COLLISION_ON_NETWORK_CLONE_WHEN_FIXED(Ped ped, BOOL enable); // Missing in crossmap
		BOOL IS_MOVE_BLEND_RATIO_STILL(Ped ped);
		BOOL IS_MOVE_BLEND_RATIO_WALKING(Ped ped);
		BOOL IS_MOVE_BLEND_RATIO_RUNNING(Ped ped);
		BOOL IS_MOVE_BLEND_RATIO_SPRINTING(Ped ped);
		BOOL IS_PED_STILL(Ped ped);
		BOOL IS_PED_WALKING(Ped ped);
		BOOL IS_PED_RUNNING(Ped ped);
		BOOL IS_PED_SPRINTING(Ped ped);
		BOOL IS_PED_STRAFING(Ped ped);
		/**
		 * TASK::TASK_SYNCHRONIZED_SCENE(ped, scene, "creatures@rottweiler@in_vehicle@std_car", "get_in", 1000.0, -8.0, 4, 0, 0x447a0000, 0);
		 * 
		 * Full list of animation dictionaries and anims by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/animDictsCompact.json
		 */
		void TASK_SYNCHRONIZED_SCENE(Ped ped, int scene, const char* animDictionary, const char* animationName, float blendIn, float blendOut, int flags, int ragdollBlockingFlags, float moverBlendDelta, int ikFlags);
		void TASK_AGITATED_ACTION_CONFRONT_RESPONSE(Ped ped, Ped ped2);
		/**
		 * This function is called on peds in vehicles.
		 * 
		 * anim: animation name
		 * p2, p3, p4: "sweep_low", "sweep_med" or "sweep_high"
		 * p5: no idea what it does but is usually -1
		 */
		void TASK_SWEEP_AIM_ENTITY(Ped ped, const char* animDict, const char* lowAnimName, const char* medAnimName, const char* hiAnimName, int runtime, Entity targetEntity, float turnRate, float blendInDuration);
		void UPDATE_TASK_SWEEP_AIM_ENTITY(Ped ped, Entity entity);
		void TASK_SWEEP_AIM_POSITION(Ped ped, const char* animDict, const char* lowAnimName, const char* medAnimName, const char* hiAnimName, int runtime, float x, float y, float z, float turnRate, float blendInDuration);
		void UPDATE_TASK_SWEEP_AIM_POSITION(Ped ped, float x, float y, float z);
		/**
		 * Example from "me_amanda1.ysc.c4":
		 * TASK::TASK_ARREST_PED(l_19F /* This is a Ped * / , PLAYER::PLAYER_PED_ID());
		 * 
		 * Example from "armenian1.ysc.c4":
		 * if (!PED::IS_PED_INJURED(l_B18[0/*1* /])) {
		 * TASK::TASK_ARREST_PED(l_B18[0/*1* /], PLAYER::PLAYER_PED_ID());
		 * }
		 * 
		 * I would love to have time to experiment to see if a player Ped can arrest another Ped. Might make for a good cop mod.
		 * 
		 * 
		 * Looks like only the player can be arrested this way. Peds react and try to arrest you if you task them, but the player charater doesn't do anything if tasked to arrest another ped.
		 */
		void TASK_ARREST_PED(Ped ped, Ped target);
		BOOL IS_PED_RUNNING_ARREST_TASK(Ped ped);
		/**
		 * This function is hard-coded to always return 0.
		 */
		BOOL IS_PED_BEING_ARRESTED(Ped ped);
		void UNCUFF_PED(Ped ped);
		BOOL IS_PED_CUFFED(Ped ped);

	} // namespace TASK

	namespace VEHICLE {

		/**
		 * p7 when set to true allows you to spawn vehicles under -100 z.
		 * Full list of vehicles by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/vehicles.json
		 */
		Vehicle CREATE_VEHICLE(Hash modelHash, float x, float y, float z, float heading, BOOL isNetwork, BOOL bScriptHostVeh, BOOL p7);
		/**
		 * Deletes a vehicle.
		 * The vehicle must be a mission entity to delete, so call this before deleting: SET_ENTITY_AS_MISSION_ENTITY(vehicle, true, true);
		 * 
		 * eg how to use:
		 * SET_ENTITY_AS_MISSION_ENTITY(vehicle, true, true);
		 * DELETE_VEHICLE(&vehicle);
		 * 
		 * Deletes the specified vehicle, then sets the handle pointed to by the pointer to NULL.
		 */
		void DELETE_VEHICLE(Vehicle* vehicle);
		void SET_VEHICLE_ALLOW_HOMING_MISSLE_LOCKON(Vehicle vehicle, BOOL toggle, BOOL p2);
		void _GET_VEHICLE_ALLOW_HOMING_MISSLE_LOCKON_SYNCED(Vehicle vehicle); // Missing in crossmap
		void SET_VEHICLE_ALLOW_HOMING_MISSLE_LOCKON_SYNCED(Vehicle vehicle, BOOL canBeLockedOn, BOOL p2);
		/**
		 * Makes the vehicle accept no passengers.
		 */
		void SET_VEHICLE_ALLOW_NO_PASSENGERS_LOCKON(Vehicle veh, BOOL toggle);
		/**
		 * Returns a value depending on the lock-on state of vehicle weapons.
		 * 0: not locked on
		 * 1: locking on
		 * 2: locked on
		 */
		int GET_VEHICLE_HOMING_LOCKON_STATE(Vehicle vehicle);
		int GET_VEHICLE_HOMING_LOCKEDONTO_STATE(Any p0);
		void SET_VEHICLE_HOMING_LOCKEDONTO_STATE(Any p0, Any p1);
		BOOL IS_VEHICLE_MODEL(Vehicle vehicle, Hash model);
		BOOL DOES_SCRIPT_VEHICLE_GENERATOR_EXIST(int vehicleGenerator);
		/**
		 * Creates a script vehicle generator at the given coordinates. Most parameters after the model hash are unknown.
		 * 
		 * Parameters:
		 * x/y/z - Generator position
		 * heading - Generator heading
		 * p4 - Unknown (always 5.0)
		 * p5 - Unknown (always 3.0)
		 * modelHash - Vehicle model hash
		 * p7/8/9/10 - Unknown (always -1)
		 * p11 - Unknown (usually TRUE, only one instance of FALSE)
		 * p12/13 - Unknown (always FALSE)
		 * p14 - Unknown (usally FALSE, only two instances of TRUE)
		 * p15 - Unknown (always TRUE)
		 * p16 - Unknown (always -1)
		 * 
		 * Vector3 coords = GET_ENTITY_COORDS(PLAYER_PED_ID(), 0);    CREATE_SCRIPT_VEHICLE_GENERATOR(coords.x, coords.y, coords.z, 1.0f, 5.0f, 3.0f, GET_HASH_KEY("adder"), -1. -1, -1, -1, -1, true, false, false, false, true, -1);
		 */
		int CREATE_SCRIPT_VEHICLE_GENERATOR(float x, float y, float z, float heading, float p4, float p5, Hash modelHash, int p7, int p8, int p9, int p10, BOOL p11, BOOL p12, BOOL p13, BOOL p14, BOOL p15, int p16);
		void DELETE_SCRIPT_VEHICLE_GENERATOR(int vehicleGenerator);
		/**
		 * Only called once in the decompiled scripts. Presumably activates the specified generator.
		 */
		void SET_SCRIPT_VEHICLE_GENERATOR(int vehicleGenerator, BOOL enabled);
		/**
		 * When p6 is true, vehicle generators are active.
		 * p7 seems to always be true in story mode scripts, but it's sometimes false in online scripts.
		 */
		void SET_ALL_VEHICLE_GENERATORS_ACTIVE_IN_AREA(float x1, float y1, float z1, float x2, float y2, float z2, BOOL toggle, BOOL p7);
		void SET_ALL_VEHICLE_GENERATORS_ACTIVE();
		void SET_ALL_LOW_PRIORITY_VEHICLE_GENERATORS_ACTIVE(BOOL active);
		/**
		 * Related to car generators & CPlayerSwitchMgrLong
		 */
		void SET_VEHICLE_GENERATOR_AREA_OF_INTEREST(float x, float y, float z, float radius);
		void CLEAR_VEHICLE_GENERATOR_AREA_OF_INTEREST();
		/**
		 * Sets a vehicle on the ground on all wheels.  Returns whether or not the operation was successful.
		 * 
		 * sfink: This has an additional param(Vehicle vehicle, float p1) which is always set to 5.0f in the b944 scripts.
		 */
		BOOL SET_VEHICLE_ON_GROUND_PROPERLY(Vehicle vehicle, float p1);
		BOOL SET_VEHICLE_USE_CUTSCENE_WHEEL_COMPRESSION(Vehicle p0, BOOL p1, BOOL p2, BOOL p3);
		BOOL IS_VEHICLE_STUCK_ON_ROOF(Vehicle vehicle);
		void ADD_VEHICLE_UPSIDEDOWN_CHECK(Vehicle vehicle);
		void REMOVE_VEHICLE_UPSIDEDOWN_CHECK(Vehicle vehicle);
		/**
		 * Returns true if the vehicle's current speed is less than, or equal to 0.0025f.
		 * 
		 * For some vehicles it returns true if the current speed is <= 0.00039999999.
		 */
		BOOL IS_VEHICLE_STOPPED(Vehicle vehicle);
		/**
		 * Gets the number of passengers.
		 * 
		 * This native was modified in b2545 to take two additional parameters, allowing you to include the driver or exclude dead passengers.
		 * 
		 * To keep it working like before b2545, set includeDriver to false and includeDeadOccupants to true.
		 */
		int GET_VEHICLE_NUMBER_OF_PASSENGERS(Vehicle vehicle, BOOL includeDriver, BOOL includeDeadOccupants);
		int GET_VEHICLE_MAX_NUMBER_OF_PASSENGERS(Vehicle vehicle);
		/**
		 * Returns max number of passengers (including the driver) for the specified vehicle model.
		 * 
		 * Full list of vehicles by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/vehicles.json
		 */
		int GET_VEHICLE_MODEL_NUMBER_OF_SEATS(Hash modelHash);
		BOOL IS_SEAT_WARP_ONLY(Vehicle vehicle, int seatIndex);
		BOOL IS_TURRET_SEAT(Vehicle vehicle, int seatIndex);
		/**
		 * Returns true if the vehicle has the FLAG_ALLOWS_RAPPEL flag set.
		 */
		BOOL DOES_VEHICLE_ALLOW_RAPPEL(Vehicle vehicle);
		/**
		 * Use this native inside a looped function.
		 * Values:
		 * - `0.0` = no vehicles on streets
		 * - `1.0` = normal vehicles on streets
		 */
		void SET_VEHICLE_DENSITY_MULTIPLIER_THIS_FRAME(float multiplier);
		void SET_RANDOM_VEHICLE_DENSITY_MULTIPLIER_THIS_FRAME(float multiplier);
		void SET_PARKED_VEHICLE_DENSITY_MULTIPLIER_THIS_FRAME(float multiplier);
		void SET_DISABLE_RANDOM_TRAINS_THIS_FRAME(BOOL toggle);
		void SET_AMBIENT_VEHICLE_RANGE_MULTIPLIER_THIS_FRAME(float value);
		void SET_FAR_DRAW_VEHICLES(BOOL toggle);
		void SET_NUMBER_OF_PARKED_VEHICLES(int value);
		/**
		 * enum eVehicleLockState
		 * {
		 * VEHICLELOCK_NONE,
		 * VEHICLELOCK_UNLOCKED,
		 * VEHICLELOCK_LOCKED,
		 * VEHICLELOCK_LOCKOUT_PLAYER_ONLY,
		 * VEHICLELOCK_LOCKED_PLAYER_INSIDE,
		 * VEHICLELOCK_LOCKED_INITIALLY,
		 * VEHICLELOCK_FORCE_SHUT_DOORS,
		 * VEHICLELOCK_LOCKED_BUT_CAN_BE_DAMAGED,
		 * VEHICLELOCK_LOCKED_BUT_BOOT_UNLOCKED,
		 * VEHICLELOCK_LOCKED_NO_PASSENGERS,
		 * VEHICLELOCK_CANNOT_ENTER
		 * };
		 */
		void SET_VEHICLE_DOORS_LOCKED(Vehicle vehicle, int doorLockStatus);
		/**
		 * doorId: see SET_VEHICLE_DOOR_SHUT
		 */
		void SET_VEHICLE_INDIVIDUAL_DOORS_LOCKED(Vehicle vehicle, int doorId, int doorLockStatus);
		/**
		 * If set to true, prevents vehicle sirens from having sound, leaving only the lights.
		 */
		void SET_VEHICLE_HAS_MUTED_SIRENS(Vehicle vehicle, BOOL toggle);
		void SET_VEHICLE_DOORS_LOCKED_FOR_PLAYER(Vehicle vehicle, Player player, BOOL toggle);
		BOOL GET_VEHICLE_DOORS_LOCKED_FOR_PLAYER(Vehicle vehicle, Player player);
		/**
		 * After some analysis, I've decided that these are what the parameters are.
		 * 
		 * We can see this being used in R* scripts such as "am_mp_property_int.ysc.c4":
		 * l_11A1 = PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), 1);
		 * ...
		 * VEHICLE::SET_VEHICLE_DOORS_LOCKED_FOR_ALL_PLAYERS(l_11A1, 1);
		 */
		void SET_VEHICLE_DOORS_LOCKED_FOR_ALL_PLAYERS(Vehicle vehicle, BOOL toggle);
		void SET_VEHICLE_DOORS_LOCKED_FOR_NON_SCRIPT_PLAYERS(Vehicle vehicle, BOOL toggle);
		void SET_VEHICLE_DOORS_LOCKED_FOR_TEAM(Vehicle vehicle, int team, BOOL toggle);
		void SET_VEHICLE_DOORS_LOCKED_FOR_ALL_TEAMS(Vehicle vehicle, BOOL toggle);
		void SET_VEHICLE_DONT_TERMINATE_TASK_WHEN_ACHIEVED(Vehicle vehicle);
		/**
		 * 0.0f = engine rev minimum
		 * 1.0f = engine rev limit
		 */
		void _SET_VEHICLE_MAX_LAUNCH_ENGINE_REVS(Vehicle vehicle, float modifier);
		float _GET_VEHICLE_THROTTLE(Vehicle vehicle);
		/**
		 * Explodes a selected vehicle.
		 * 
		 * Vehicle vehicle = Vehicle you want to explode.
		 * BOOL isAudible = If explosion makes a sound.
		 * BOOL isInvisible = If the explosion is invisible or not.
		 * 
		 * First BOOL does not give any visual explosion, the vehicle just falls apart completely but slowly and starts to burn.
		 */
		void EXPLODE_VEHICLE(Vehicle vehicle, BOOL isAudible, BOOL isInvisible);
		/**
		 * Tested on the player's current vehicle. Unless you kill the driver, the vehicle doesn't loose control, however, if enabled, explodeOnImpact is still active. The moment you crash, boom.
		 */
		void SET_VEHICLE_OUT_OF_CONTROL(Vehicle vehicle, BOOL killDriver, BOOL explodeOnImpact);
		void SET_VEHICLE_TIMED_EXPLOSION(Vehicle vehicle, Ped ped, BOOL toggle);
		void ADD_VEHICLE_PHONE_EXPLOSIVE_DEVICE(Vehicle vehicle);
		void CLEAR_VEHICLE_PHONE_EXPLOSIVE_DEVICE();
		BOOL HAS_VEHICLE_PHONE_EXPLOSIVE_DEVICE();
		void DETONATE_VEHICLE_PHONE_EXPLOSIVE_DEVICE();
		BOOL HAVE_VEHICLE_REAR_DOORS_BEEN_BLOWN_OPEN_BY_STICKYBOMB(Vehicle vehicle);
		/**
		 * This is not tested - it's just an assumption.
		 * - Nac
		 * 
		 * Doesn't seem to work.  I'll try with an int instead. --JT
		 * 
		 * Read the scripts, im dumpass.
		 * 
		 * if (!VEHICLE::IS_TAXI_LIGHT_ON(l_115)) {
		 * VEHICLE::SET_TAXI_LIGHTS(l_115, 1);
		 * }
		 */
		void SET_TAXI_LIGHTS(Vehicle vehicle, BOOL state);
		BOOL IS_TAXI_LIGHT_ON(Vehicle vehicle);
		/**
		 * garageName example "Michael - Beverly Hills"
		 * 
		 * Full list of garages by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/garages.json
		 */
		BOOL IS_VEHICLE_IN_GARAGE_AREA(const char* garageName, Vehicle vehicle);
		/**
		 * colorPrimary & colorSecondary are the paint index for the vehicle.
		 * For a list of valid paint indexes, view: https://pastebin.com/pwHci0xK
		 * -------------------------------------------------------------------------
		 * Note: minimum color index is 0, maximum color index is (numColorIndices - 1)
		 * 
		 * Full list of vehicle colors by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/vehicleColors.json
		 */
		void SET_VEHICLE_COLOURS(Vehicle vehicle, int colorPrimary, int colorSecondary);
		/**
		 * It switch to highbeam when p1 is set to true.
		 */
		void SET_VEHICLE_FULLBEAM(Vehicle vehicle, BOOL toggle);
		/**
		 * p1 (toggle) was always 1 (true) except in one case in the b678 scripts.
		 */
		void SET_VEHICLE_IS_RACING(Vehicle vehicle, BOOL toggle);
		/**
		 * p1, p2, p3 are RGB values for color (255,0,0 for Red, ect)
		 */
		void SET_VEHICLE_CUSTOM_PRIMARY_COLOUR(Vehicle vehicle, int r, int g, int b);
		void GET_VEHICLE_CUSTOM_PRIMARY_COLOUR(Vehicle vehicle, int* r, int* g, int* b);
		void CLEAR_VEHICLE_CUSTOM_PRIMARY_COLOUR(Vehicle vehicle);
		BOOL GET_IS_VEHICLE_PRIMARY_COLOUR_CUSTOM(Vehicle vehicle);
		/**
		 * p1, p2, p3 are RGB values for color (255,0,0 for Red, ect)
		 */
		void SET_VEHICLE_CUSTOM_SECONDARY_COLOUR(Vehicle vehicle, int r, int g, int b);
		void GET_VEHICLE_CUSTOM_SECONDARY_COLOUR(Vehicle vehicle, int* r, int* g, int* b);
		void CLEAR_VEHICLE_CUSTOM_SECONDARY_COLOUR(Vehicle vehicle);
		/**
		 * Check if Vehicle Secondary is avaliable for customize
		 */
		BOOL GET_IS_VEHICLE_SECONDARY_COLOUR_CUSTOM(Vehicle vehicle);
		/**
		 * The parameter fade is a value from 0-1, where 0 is fresh paint.
		 */
		void SET_VEHICLE_ENVEFF_SCALE(Vehicle vehicle, float fade);
		/**
		 * The result is a value from 0-1, where 0 is fresh paint.
		 */
		float GET_VEHICLE_ENVEFF_SCALE(Vehicle vehicle);
		/**
		 * Hardcoded to not work in multiplayer.
		 */
		void SET_CAN_RESPRAY_VEHICLE(Vehicle vehicle, BOOL state);
		/**
		 * Used for GTAO CEO/Associate spawned vehicles.
		 */
		void SET_GOON_BOSS_VEHICLE(Vehicle vehicle, BOOL toggle);
		void SET_OPEN_REAR_DOORS_ON_EXPLOSION(Vehicle vehicle, BOOL toggle);
		void FORCE_SUBMARINE_SURFACE_MODE(Vehicle vehicle, BOOL toggle);
		void FORCE_SUBMARINE_NEURTAL_BUOYANCY(Any p0, Any p1);
		void SET_SUBMARINE_CRUSH_DEPTHS(Vehicle vehicle, BOOL p1, float depth1, float depth2, float depth3);
		BOOL GET_SUBMARINE_IS_UNDER_DESIGN_DEPTH(Vehicle submarine);
		int GET_SUBMARINE_NUMBER_OF_AIR_LEAKS(Vehicle submarine);
		void SET_BOAT_IGNORE_LAND_PROBES(Any p0, Any p1);
		/**
		 * Use the vehicle bounds (instead of viewport) when deciding if a vehicle is sufficiently above the water (waterheight.dat), bypassing wave simulation checks
		 */
		void _SET_BOUNDS_AFFECT_WATER_PROBES(Vehicle vehicle, BOOL toggle);
		void SET_BOAT_ANCHOR(Vehicle vehicle, BOOL toggle);
		BOOL CAN_ANCHOR_BOAT_HERE(Vehicle vehicle);
		BOOL CAN_ANCHOR_BOAT_HERE_IGNORE_PLAYERS(Vehicle vehicle);
		void SET_BOAT_REMAINS_ANCHORED_WHILE_PLAYER_IS_DRIVER(Vehicle vehicle, BOOL toggle);
		/**
		 * No observed effect.
		 */
		void SET_FORCE_LOW_LOD_ANCHOR_MODE(Vehicle vehicle, BOOL p1);
		void SET_BOAT_LOW_LOD_ANCHOR_DISTANCE(Vehicle vehicle, float value);
		BOOL IS_BOAT_ANCHORED(Vehicle vehicle);
		void SET_BOAT_SINKS_WHEN_WRECKED(Vehicle vehicle, BOOL toggle);
		void SET_BOAT_WRECKED(Vehicle vehicle);
		/**
		 * Activate siren on vehicle (Only works if the vehicle has a siren).
		 */
		void SET_VEHICLE_SIREN(Vehicle vehicle, BOOL toggle);
		BOOL IS_VEHICLE_SIREN_ON(Vehicle vehicle);
		BOOL IS_VEHICLE_SIREN_AUDIO_ON(Vehicle vehicle);
		/**
		 * If set to true, vehicle will not take crash damage, but is still susceptible to damage from bullets and explosives
		 */
		void SET_VEHICLE_STRONG(Vehicle vehicle, BOOL toggle);
		void REMOVE_VEHICLE_STUCK_CHECK(Vehicle vehicle);
		void GET_VEHICLE_COLOURS(Vehicle vehicle, int* colorPrimary, int* colorSecondary);
		/**
		 * Check if a vehicle seat is free.
		 * 
		 * seatIndex  = -1 being the driver seat.
		 * Use GET_VEHICLE_MAX_NUMBER_OF_PASSENGERS(vehicle) - 1 for last seat index.
		 * isTaskRunning = on true the function returns already false while a task on the target seat is running (TASK_ENTER_VEHICLE/TASK_SHUFFLE_TO_NEXT_VEHICLE_SEAT) - on false only when a ped is finally sitting in the seat.
		 */
		BOOL IS_VEHICLE_SEAT_FREE(Vehicle vehicle, int seatIndex, BOOL isTaskRunning);
		/**
		 * If there is no ped in the seat, and the game considers the vehicle as ambient population, this will create a random occupant ped in the seat, which may be cleaned up by the game fairly soon if not marked as script-owned mission entity.
		 * 
		 * Seat indexes:
		 * -1 = Driver
		 * 0 = Front Right Passenger
		 * 1 = Back Left Passenger
		 * 2 = Back Right Passenger
		 * 3 = Further Back Left Passenger (vehicles > 4 seats)
		 * 4 = Further Back Right Passenger (vehicles > 4 seats)
		 * etc.
		 * 
		 * If p2 is true it uses a different GetOccupant function.
		 */
		Ped GET_PED_IN_VEHICLE_SEAT(Vehicle vehicle, int seatIndex, BOOL p2);
		Ped GET_LAST_PED_IN_VEHICLE_SEAT(Vehicle vehicle, int seatIndex);
		BOOL GET_VEHICLE_LIGHTS_STATE(Vehicle vehicle, BOOL* lightsOn, BOOL* highbeamsOn);
		/**
		 * wheelID used for 4 wheelers seem to be (0, 1, 4, 5)
		 * completely - is to check if tire completely gone from rim.
		 * 
		 * '0 = wheel_lf / bike, plane or jet front
		 * '1 = wheel_rf
		 * '2 = wheel_lm / in 6 wheels trailer, plane or jet is first one on left
		 * '3 = wheel_rm / in 6 wheels trailer, plane or jet is first one on right
		 * '4 = wheel_lr / bike rear / in 6 wheels trailer, plane or jet is last one on left
		 * '5 = wheel_rr / in 6 wheels trailer, plane or jet is last one on right
		 * '45 = 6 wheels trailer mid wheel left
		 * '47 = 6 wheels trailer mid wheel right
		 */
		BOOL IS_VEHICLE_TYRE_BURST(Vehicle vehicle, int wheelID, BOOL completely);
		/**
		 * SCALE: Setting the speed to 30 would result in a speed of roughly 60mph, according to speedometer.
		 * 
		 * Speed is in meters per second
		 * You can convert meters/s to mph here:
		 * http://www.calculateme.com/Speed/MetersperSecond/ToMilesperHour.htm
		 */
		void SET_VEHICLE_FORWARD_SPEED(Vehicle vehicle, float speed);
		/**
		 * Seems to be identical to SET_VEHICLE_FORWARD_SPEED
		 */
		void SET_VEHICLE_FORWARD_SPEED_XY(Vehicle vehicle, float speed);
		/**
		 * This native makes the vehicle stop immediately, as happens when we enter a MP garage.
		 * 
		 * . distance defines how far it will travel until stopping. Garage doors use 3.0.
		 * 
		 * . If killEngine is set to 1, you cannot resume driving the vehicle once it stops. This looks like is a bitmapped integer.
		 */
		void BRING_VEHICLE_TO_HALT(Vehicle vehicle, float distance, int duration, BOOL p3);
		void SET_VEHICLE_STEER_FOR_BUILDINGS(Vehicle vehicle, Any p1);
		void SET_VEHICLE_CAUSES_SWERVING(Vehicle vehicle, BOOL toggle);
		void SET_IGNORE_PLANES_SMALL_PITCH_CHANGE(Any p0, Any p1);
		/**
		 * Stops CTaskBringVehicleToHalt
		 */
		void STOP_BRINGING_VEHICLE_TO_HALT(Vehicle vehicle);
		/**
		 * Returns true if vehicle is halted by BRING_VEHICLE_TO_HALT
		 */
		BOOL IS_VEHICLE_BEING_BROUGHT_TO_HALT(Vehicle vehicle);
		void LOWER_FORKLIFT_FORKS(Vehicle forklift);
		/**
		 * 0.0 = Lowest 1.0 = Highest. This is best to be used if you wanna pick-up a car since un-realistically on GTA V forklifts can't pick up much of anything due to vehicle mass. If you put this under a car then set it above 0.0 to a 'lifted-value' it will raise the car with no issue lol
		 */
		void SET_FORKLIFT_FORK_HEIGHT(Vehicle vehicle, float height);
		BOOL IS_ENTITY_ATTACHED_TO_HANDLER_FRAME(Vehicle vehicle, Entity entity);
		BOOL IS_ANY_ENTITY_ATTACHED_TO_HANDLER_FRAME(Vehicle vehicle);
		/**
		 * Finds the vehicle that is carrying this entity with a handler frame.
		 * The model of the entity must be prop_contr_03b_ld or the function will return 0.
		 */
		Vehicle FIND_HANDLER_VEHICLE_CONTAINER_IS_ATTACHED_TO(Entity entity);
		BOOL IS_HANDLER_FRAME_LINED_UP_WITH_CONTAINER(Vehicle vehicle, Entity entity);
		void ATTACH_CONTAINER_TO_HANDLER_FRAME_WHEN_LINED_UP(Vehicle vehicle, Entity entity);
		void DETACH_CONTAINER_FROM_HANDLER_FRAME(Vehicle vehicle);
		void SET_VEHICLE_DISABLE_HEIGHT_MAP_AVOIDANCE(Vehicle vehicle, BOOL p1);
		void _SET_PLANE_AVOIDS_OTHERS(Vehicle vehicle, BOOL toggle); // Missing in crossmap
		void SET_BOAT_DISABLE_AVOIDANCE(Vehicle vehicle, BOOL p1);
		BOOL IS_HELI_LANDING_AREA_BLOCKED(Vehicle vehicle);
		/**
		 * Used on helicopters and blimps during the CTaskVehicleLand task. Sets a value on the task to 10f
		 */
		void SET_SHORT_SLOWDOWN_FOR_LANDING(Vehicle vehicle);
		void SET_HELI_TURBULENCE_SCALAR(Vehicle vehicle, float p1);
		/**
		 * Initially used in Max Payne 3, that's why we know the name.
		 */
		void SET_CAR_BOOT_OPEN(Vehicle vehicle);
		/**
		 * "To burst tyres VEHICLE::SET_VEHICLE_TYRE_BURST(vehicle, 0, true, 1000.0)
		 * to burst all tyres type it 8 times where p1 = 0 to 7.
		 * 
		 * p3 seems to be how much damage it has taken. 0 doesn't deflate them, 1000 completely deflates them.
		 * 
		 * '0 = wheel_lf / bike, plane or jet front
		 * '1 = wheel_rf
		 * '2 = wheel_lm / in 6 wheels trailer, plane or jet is first one on left
		 * '3 = wheel_rm / in 6 wheels trailer, plane or jet is first one on right
		 * '4 = wheel_lr / bike rear / in 6 wheels trailer, plane or jet is last one on left
		 * '5 = wheel_rr / in 6 wheels trailer, plane or jet is last one on right
		 * '45 = 6 wheels trailer mid wheel left
		 * '47 = 6 wheels trailer mid wheel right
		 */
		void SET_VEHICLE_TYRE_BURST(Vehicle vehicle, int index, BOOL onRim, float p3);
		/**
		 * Closes all doors of a vehicle:
		 */
		void SET_VEHICLE_DOORS_SHUT(Vehicle vehicle, BOOL closeInstantly);
		/**
		 * Allows you to toggle bulletproof tires.
		 */
		void SET_VEHICLE_TYRES_CAN_BURST(Vehicle vehicle, BOOL toggle);
		BOOL GET_VEHICLE_TYRES_CAN_BURST(Vehicle vehicle);
		void SET_VEHICLE_WHEELS_CAN_BREAK(Vehicle vehicle, BOOL enabled);
		/**
		 * doorId: see SET_VEHICLE_DOOR_SHUT
		 */
		void SET_VEHICLE_DOOR_OPEN(Vehicle vehicle, int doorId, BOOL loose, BOOL openInstantly);
		/**
		 * doorId: see SET_VEHICLE_DOOR_SHUT
		 * 
		 * Usually used alongside other vehicle door natives.
		 */
		void SET_VEHICLE_DOOR_AUTO_LOCK(Vehicle vehicle, int doorId, BOOL toggle);
		void SET_FLEEING_VEHICLES_USE_SWITCHED_OFF_NODES(Any p0);
		/**
		 * windowIndex:
		 * 0 = Front Right Window
		 * 1 = Front Left Window
		 * 2 = Back Right Window
		 * 3 = Back Left Window
		 * 4 = Unknown
		 * 5 = Unknown
		 * 6 = Windscreen
		 * 7 = Rear Windscreen
		 */
		void REMOVE_VEHICLE_WINDOW(Vehicle vehicle, int windowIndex);
		/**
		 * Roll down all the windows of the vehicle passed through the first parameter.
		 */
		void ROLL_DOWN_WINDOWS(Vehicle vehicle);
		/**
		 * windowIndex:
		 * 0 = Front Left Window
		 * 1 = Front Right Window
		 * 2 = Rear Left Window
		 * 3 = Rear Right Window
		 * 4 = Front Windscreen
		 * 5 = Rear Windscreen
		 * 6 = Mid Left
		 * 7 = Mid Right
		 * 8 = Invalid
		 */
		void ROLL_DOWN_WINDOW(Vehicle vehicle, int windowIndex);
		/**
		 * windowIndex:
		 * 0 = Front Left Window
		 * 1 = Front Right Window
		 * 2 = Rear Left Window
		 * 3 = Rear Right Window
		 * 4 = Front Windscreen
		 * 5 = Rear Windscreen
		 * 6 = Mid Left
		 * 7 = Mid Right
		 * 8 = Invalid
		 */
		void ROLL_UP_WINDOW(Vehicle vehicle, int windowIndex);
		/**
		 * windowIndex:
		 * 0 = Front Left Window
		 * 1 = Front Right Window
		 * 2 = Rear Left Window
		 * 3 = Rear Right Window
		 * 4 = Front Windscreen
		 * 5 = Rear Windscreen
		 * 6 = Mid Left
		 * 7 = Mid Right
		 * 8 = Invalid
		 */
		void SMASH_VEHICLE_WINDOW(Vehicle vehicle, int windowIndex);
		/**
		 * windowIndex:
		 * 0 = Front Left Window
		 * 1 = Front Right Window
		 * 2 = Rear Left Window
		 * 3 = Rear Right Window
		 * 4 = Front Windscreen
		 * 5 = Rear Windscreen
		 * 6 = Mid Left
		 * 7 = Mid Right
		 * 8 = Invalid
		 * 
		 * Additional information: FIX_VEHICLE_WINDOW (0x140D0BB88) references an array of bone vehicle indices (0x141D4B3E0) { 2Ah, 2Bh, 2Ch, 2Dh, 2Eh, 2Fh, 28h, 29h } that correspond to: window_lf, window_rf, window_lr, window_rr, window_lm, window_rm, windscreen, windscreen_r. This array is used by most vehwindow natives.
		 * 
		 * Also, this function is coded to not work on vehicles of type: CBike, Bmx, CBoat, CTrain, and CSubmarine.
		 */
		void FIX_VEHICLE_WINDOW(Vehicle vehicle, int windowIndex);
		/**
		 * Detaches the vehicle's windscreen.
		 * For further information, see : gtaforums.com/topic/859570-glass/#entry1068894566
		 */
		void POP_OUT_VEHICLE_WINDSCREEN(Vehicle vehicle);
		/**
		 * Pops off the "roof" bone in the direction of the specified offset from the vehicle.
		 */
		void POP_OFF_VEHICLE_ROOF_WITH_IMPULSE(Vehicle vehicle, float x, float y, float z);
		/**
		 * set's if the vehicle has lights or not.
		 * not an on off toggle.
		 * p1 = 0 ;vehicle normal lights, off then lowbeams, then highbeams
		 * p1 = 1 ;vehicle doesn't have lights, always off
		 * p1 = 2 ;vehicle has always on lights
		 * p1 = 3 ;or even larger like 4,5,... normal lights like =1
		 * note1: when using =2 on day it's lowbeam,highbeam
		 * but at night it's lowbeam,lowbeam,highbeam
		 * note2: when using =0 it's affected by day or night for highbeams don't exist in daytime.
		 */
		void SET_VEHICLE_LIGHTS(Vehicle vehicle, int state);
		void SET_VEHICLE_USE_PLAYER_LIGHT_SETTINGS(Vehicle vehicle, BOOL toggle);
		/**
		 * p1 can be either 0, 1 or 2.
		 * 
		 * Determines how vehicle lights behave when toggled.
		 * 
		 * 0 = Default (Lights can be toggled between off, normal and high beams)
		 * 1 = Lights Disabled (Lights are fully disabled, cannot be toggled)
		 * 2 = Always On (Lights can be toggled between normal and high beams)
		 */
		void SET_VEHICLE_HEADLIGHT_SHADOWS(Vehicle vehicle, int p1);
		void SET_VEHICLE_ALARM(Vehicle vehicle, BOOL state);
		void START_VEHICLE_ALARM(Vehicle vehicle);
		BOOL IS_VEHICLE_ALARM_ACTIVATED(Vehicle vehicle);
		void SET_VEHICLE_INTERIORLIGHT(Vehicle vehicle, BOOL toggle);
		/**
		 * Sets some bit of vehicle
		 */
		void SET_VEHICLE_FORCE_INTERIORLIGHT(Vehicle vehicle, BOOL toggle);
		/**
		 * multiplier = brightness of head lights.
		 * this value isn't capped afaik.
		 * 
		 * multiplier = 0.0 no lights
		 * multiplier = 1.0 default game value
		 * 
		 */
		void SET_VEHICLE_LIGHT_MULTIPLIER(Vehicle vehicle, float multiplier);
		void ATTACH_VEHICLE_TO_TRAILER(Vehicle vehicle, Vehicle trailer, float radius);
		/**
		 * This is the proper way of attaching vehicles to the car carrier, it's what Rockstar uses. Video Demo: https://www.youtube.com/watch?v=2lVEIzf7bgo
		 */
		void ATTACH_VEHICLE_ON_TO_TRAILER(Vehicle vehicle, Vehicle trailer, float offsetX, float offsetY, float offsetZ, float coordsX, float coordsY, float coordsZ, float rotationX, float rotationY, float rotationZ, float disableCollisions);
		void STABILISE_ENTITY_ATTACHED_TO_HELI(Vehicle vehicle, Entity entity, float p2);
		void DETACH_VEHICLE_FROM_TRAILER(Vehicle vehicle);
		BOOL IS_VEHICLE_ATTACHED_TO_TRAILER(Vehicle vehicle);
		void SET_TRAILER_INVERSE_MASS_SCALE(Vehicle vehicle, float p1);
		/**
		 * in the decompiled scripts, seems to be always called on the vehicle right after being attached to a trailer.
		 */
		void SET_TRAILER_LEGS_RAISED(Vehicle vehicle);
		void SET_TRAILER_LEGS_LOWERED(Vehicle vehicle);
		/**
		 * tyreIndex = 0 to 4 on normal vehicles
		 * 
		 * '0 = wheel_lf / bike, plane or jet front
		 * '1 = wheel_rf
		 * '2 = wheel_lm / in 6 wheels trailer, plane or jet is first one on left
		 * '3 = wheel_rm / in 6 wheels trailer, plane or jet is first one on right
		 * '4 = wheel_lr / bike rear / in 6 wheels trailer, plane or jet is last one on left
		 * '5 = wheel_rr / in 6 wheels trailer, plane or jet is last one on right
		 * '45 = 6 wheels trailer mid wheel left
		 * '47 = 6 wheels trailer mid wheel right
		 */
		void SET_VEHICLE_TYRE_FIXED(Vehicle vehicle, int tyreIndex);
		/**
		 * Sets a vehicle's license plate text.  8 chars maximum.
		 * 
		 * Example:
		 * Ped playerPed = PLAYER::PLAYER_PED_ID();
		 * Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(playerPed);
		 * char *plateText = "KING";
		 * VEHICLE::SET_VEHICLE_NUMBER_PLATE_TEXT(veh, plateText);
		 */
		void SET_VEHICLE_NUMBER_PLATE_TEXT(Vehicle vehicle, const char* plateText);
		/**
		 * Returns the license plate text from a vehicle. 8 chars maximum.
		 */
		const char* GET_VEHICLE_NUMBER_PLATE_TEXT(Vehicle vehicle);
		/**
		 * Returns the number of *types* of licence plates, enumerated below in SET_VEHICLE_NUMBER_PLATE_TEXT_INDEX.
		 */
		int GET_NUMBER_OF_VEHICLE_NUMBER_PLATES();
		/**
		 * Plates:
		 * Blue/White - 0
		 * Yellow/black - 1
		 * Yellow/Blue - 2
		 * Blue/White2 - 3
		 * Blue/White3 - 4
		 * Yankton - 5
		 */
		void SET_VEHICLE_NUMBER_PLATE_TEXT_INDEX(Vehicle vehicle, int plateIndex);
		/**
		 * Returns the PlateType of a vehicle
		 * Blue_on_White_1 = 3,
		 * Blue_on_White_2 = 0,
		 * Blue_on_White_3 = 4,
		 * Yellow_on_Blue = 2,
		 * Yellow_on_Black = 1,
		 * North_Yankton = 5,
		 */
		int GET_VEHICLE_NUMBER_PLATE_TEXT_INDEX(Vehicle vehicle);
		void SET_RANDOM_TRAINS(BOOL toggle);
		/**
		 * Train models HAVE TO be loaded (requested) before you use this.
		 * For variation 15 - request:
		 * 
		 * freight
		 * freightcar
		 * freightgrain
		 * freightcont1
		 * freightcont2
		 * freighttrailer
		 * 
		 * 
		 */
		Vehicle CREATE_MISSION_TRAIN(int variation, float x, float y, float z, BOOL direction, Any p5, Any p6);
		/**
		 * Toggles whether ambient trains can spawn on the specified track or not
		 * 
		 * `trackId` is the internal id of the train track to switch.
		 * `state` is whether ambient trains can spawn or not
		 * 
		 * trackIds
		 * 0 (`trains1.dat`) Main track around SA
		 * 1 (`trains2.dat`) Davis Quartz Quarry branch
		 * 2 (`trains3.dat`) Second track alongside live track along Roy Lewenstein Blv.
		 * 3 (`trains4.dat`) Metro track circuit
		 * 4 (`trains5.dat`) Branch in Mirror Park Railyard
		 * 5 (`trains6.dat`) Branch in Mirror Park Railyard
		 * 6 (`trains7.dat`) LS branch to Mirror Park Railyard
		 * 7 (`trains8.dat`) Overground part of metro track along Forum Dr.
		 * 8 (`trains9.dat`) Branch to Mirror Park Railyard
		 * 9 (`trains10.dat`) Yankton train
		 * 10 (`trains11.dat`) Part of metro track near mission row
		 * 11 (`trains12.dat`) Yankton prologue mission train
		 * Full list of all train tracks + track nodes by DurtyFree https://github.com/DurtyFree/gta-v-data-dumps/blob/master/traintracks.json
		 */
		void SWITCH_TRAIN_TRACK(int trackId, BOOL state);
		/**
		 * Only called once inside main_persitant with the parameters p0 = 0, p1 = 120000
		 * 
		 * trackIndex: 0 - 26
		 * Full list of all train tracks + track nodes by DurtyFree https://github.com/DurtyFree/gta-v-data-dumps/blob/master/traintracks.json
		 */
		void SET_TRAIN_TRACK_SPAWN_FREQUENCY(int trackIndex, int frequency);
		void ALLOW_TRAIN_TO_BE_REMOVED_BY_POPULATION(Any p0);
		void DELETE_ALL_TRAINS();
		void SET_TRAIN_SPEED(Vehicle train, float speed);
		void SET_TRAIN_CRUISE_SPEED(Vehicle train, float speed);
		void SET_RANDOM_BOATS(BOOL toggle);
		void SET_RANDOM_BOATS_MP(BOOL toggle);
		void SET_GARBAGE_TRUCKS(BOOL toggle);
		/**
		 * Maximum amount of vehicles with vehicle stuck check appears to be 16.
		 */
		BOOL DOES_VEHICLE_HAVE_STUCK_VEHICLE_CHECK(Vehicle vehicle);
		/**
		 * See REQUEST_VEHICLE_RECORDING
		 */
		int GET_VEHICLE_RECORDING_ID(int recording, const char* script);
		/**
		 * Request the vehicle recording defined by the lowercase format string "%s%03d.yvr". For example, REQUEST_VEHICLE_RECORDING(1, "FBIs1UBER") corresponds to fbis1uber001.yvr.
		 * For all vehicle recording/playback natives, "script" is a common prefix that usually corresponds to the script/mission the recording is used in, "recording" is its int suffix, and "id" (e.g., in native GET_TOTAL_DURATION_OF_VEHICLE_RECORDING_ID) corresponds to a unique identifier within the recording streaming module.
		 * Note that only 24 recordings (hardcoded in multiple places) can ever active at a given time before clobbering begins.
		 */
		void REQUEST_VEHICLE_RECORDING(int recording, const char* script);
		/**
		 * See REQUEST_VEHICLE_RECORDING
		 */
		BOOL HAS_VEHICLE_RECORDING_BEEN_LOADED(int recording, const char* script);
		/**
		 * See REQUEST_VEHICLE_RECORDING
		 */
		void REMOVE_VEHICLE_RECORDING(int recording, const char* script);
		Vector3 GET_POSITION_OF_VEHICLE_RECORDING_ID_AT_TIME(int id, float time);
		/**
		 * This native does no interpolation between pathpoints. The same position will be returned for all times up to the next pathpoint in the recording.
		 * 
		 * See REQUEST_VEHICLE_RECORDING
		 */
		Vector3 GET_POSITION_OF_VEHICLE_RECORDING_AT_TIME(int recording, float time, const char* script);
		Vector3 GET_ROTATION_OF_VEHICLE_RECORDING_ID_AT_TIME(int id, float time);
		/**
		 * This native does no interpolation between pathpoints. The same rotation will be returned for all times up to the next pathpoint in the recording.
		 * 
		 * See REQUEST_VEHICLE_RECORDING
		 */
		Vector3 GET_ROTATION_OF_VEHICLE_RECORDING_AT_TIME(int recording, float time, const char* script);
		float GET_TOTAL_DURATION_OF_VEHICLE_RECORDING_ID(int id);
		/**
		 * See REQUEST_VEHICLE_RECORDING
		 */
		float GET_TOTAL_DURATION_OF_VEHICLE_RECORDING(int recording, const char* script);
		/**
		 * Distance traveled in the vehicles current recording.
		 */
		float GET_POSITION_IN_RECORDING(Vehicle vehicle);
		/**
		 * Can be used with GET_TOTAL_DURATION_OF_VEHICLE_RECORDING{_ID} to compute a percentage.
		 */
		float GET_TIME_POSITION_IN_RECORDING(Vehicle vehicle);
		/**
		 * p3 is some flag related to 'trailers' (invokes CVehicle::GetTrailer).
		 * 
		 * See REQUEST_VEHICLE_RECORDING
		 */
		void START_PLAYBACK_RECORDED_VEHICLE(Vehicle vehicle, int recording, const char* script, BOOL p3);
		/**
		 * flags requires further research, e.g., 0x4/0x8 are related to the AI driving task and 0x20 is internally set and interacts with dynamic entity components.
		 * 
		 * time, often zero and capped at 500, is related to SET_PLAYBACK_TO_USE_AI_TRY_TO_REVERT_BACK_LATER
		 */
		void START_PLAYBACK_RECORDED_VEHICLE_WITH_FLAGS(Vehicle vehicle, int recording, const char* script, int flags, int time, int drivingStyle);
		/**
		 * Often called after START_PLAYBACK_RECORDED_VEHICLE and SKIP_TIME_IN_PLAYBACK_RECORDED_VEHICLE; similar in use to FORCE_ENTITY_AI_AND_ANIMATION_UPDATE.
		 */
		void FORCE_PLAYBACK_RECORDED_VEHICLE_UPDATE(Vehicle vehicle, BOOL p1);
		void STOP_PLAYBACK_RECORDED_VEHICLE(Vehicle vehicle);
		void PAUSE_PLAYBACK_RECORDED_VEHICLE(Vehicle vehicle);
		void UNPAUSE_PLAYBACK_RECORDED_VEHICLE(Vehicle vehicle);
		BOOL IS_PLAYBACK_GOING_ON_FOR_VEHICLE(Vehicle vehicle);
		BOOL IS_PLAYBACK_USING_AI_GOING_ON_FOR_VEHICLE(Vehicle vehicle);
		int GET_CURRENT_PLAYBACK_FOR_VEHICLE(Vehicle vehicle);
		void SKIP_TO_END_AND_STOP_PLAYBACK_RECORDED_VEHICLE(Vehicle vehicle);
		void SET_PLAYBACK_SPEED(Vehicle vehicle, float speed);
		/**
		 * AI abides by the provided driving style (e.g., stopping at red lights or waiting behind traffic) while executing the specificed vehicle recording.
		 * 
		 * FORCE_PLAYBACK_RECORDED_VEHICLE_UPDATE is a related native that deals with the AI physics for such recordings.
		 */
		void START_PLAYBACK_RECORDED_VEHICLE_USING_AI(Vehicle vehicle, int recording, const char* script, float speed, int drivingStyle);
		/**
		 * SET_TIME_POSITION_IN_RECORDING can be emulated by: desired_time - GET_TIME_POSITION_IN_RECORDING(vehicle)
		 */
		void SKIP_TIME_IN_PLAYBACK_RECORDED_VEHICLE(Vehicle vehicle, float time);
		/**
		 * Identical to SET_PLAYBACK_TO_USE_AI_TRY_TO_REVERT_BACK_LATER with 0 as arguments for p1 and p3.
		 */
		void SET_PLAYBACK_TO_USE_AI(Vehicle vehicle, int drivingStyle);
		/**
		 * Time is number of milliseconds before reverting, zero for indefinitely.
		 */
		void SET_PLAYBACK_TO_USE_AI_TRY_TO_REVERT_BACK_LATER(Vehicle vehicle, int time, int drivingStyle, BOOL p3);
		void SET_ADDITIONAL_ROTATION_FOR_RECORDED_VEHICLE_PLAYBACK(Vehicle vehicle, float x, float y, float z, Any p4);
		void SET_POSITION_OFFSET_FOR_RECORDED_VEHICLE_PLAYBACK(Vehicle vehicle, float x, float y, float z);
		void SET_GLOBAL_POSITION_OFFSET_FOR_RECORDED_VEHICLE_PLAYBACK(Vehicle vehicle, float x, float y, float z);
		/**
		 * A vehicle recording playback flag only used in jewelry_heist
		 */
		void SET_SHOULD_LERP_FROM_AI_TO_FULL_RECORDING(Vehicle vehicle, BOOL p1);
		void EXPLODE_VEHICLE_IN_CUTSCENE(Vehicle vehicle, BOOL p1);
		void ADD_VEHICLE_STUCK_CHECK_WITH_WARP(Any p0, float p1, Any p2, BOOL p3, BOOL p4, BOOL p5, Any p6);
		/**
		 * Makes the vehicle stop spawning naturally in traffic. Here's an essential example:
		 * 
		 * VEHICLE::SET_VEHICLE_MODEL_IS_SUPPRESSED(MISC::GET_HASH_KEY("taco"), true);
		 * 
		 * Full list of vehicles by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/vehicles.json
		 */
		void SET_VEHICLE_MODEL_IS_SUPPRESSED(Hash model, BOOL suppressed);
		/**
		 * Gets a random vehicle in a sphere at the specified position, of the specified radius.
		 * 
		 * x: The X-component of the position of the sphere.
		 * y: The Y-component of the position of the sphere.
		 * z: The Z-component of the position of the sphere.
		 * radius: The radius of the sphere. Max is 9999.9004.
		 * modelHash: The vehicle model to limit the selection to. Pass 0 for any model.
		 * flags: The bitwise flags that modifies the behaviour of this function.
		 * 
		 * Full list of vehicles by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/vehicles.json
		 */
		Vehicle GET_RANDOM_VEHICLE_IN_SPHERE(float x, float y, float z, float radius, Hash modelHash, int flags);
		Vehicle GET_RANDOM_VEHICLE_FRONT_BUMPER_IN_SPHERE(float p0, float p1, float p2, float p3, int p4, int p5, int p6);
		Vehicle GET_RANDOM_VEHICLE_BACK_BUMPER_IN_SPHERE(float p0, float p1, float p2, float p3, int p4, int p5, int p6);
		/**
		 * Example usage
		 * VEHICLE::GET_CLOSEST_VEHICLE(x, y, z, radius, hash, unknown leave at 70)
		 * 
		 * x, y, z: Position to get closest vehicle to.
		 * radius: Max radius to get a vehicle.
		 * modelHash: Limit to vehicles with this model. 0 for any.
		 * flags: The bitwise flags altering the function's behaviour.
		 * 
		 * Does not return police cars or helicopters.
		 * 
		 * It seems to return police cars for me, does not seem to return helicopters, planes or boats for some reason
		 * 
		 * Only returns non police cars and motorbikes with the flag set to 70 and modelHash to 0. ModelHash seems to always be 0 when not a modelHash in the scripts, as stated above.
		 * 
		 * These flags were found in the b617d scripts: 0,2,4,6,7,23,127,260,2146,2175,12294,16384,16386,20503,32768,67590,67711,98309,100359.
		 * Converted to binary, each bit probably represents a flag as explained regarding another native here: gtaforums.com/topic/822314-guide-driving-styles
		 * 
		 * Conversion of found flags to binary: https://pastebin.com/kghNFkRi
		 * 
		 * At exactly 16384 which is 0100000000000000 in binary and 4000 in hexadecimal only planes are returned.
		 * 
		 * It's probably more convenient to use worldGetAllVehicles(int *arr, int arrSize) and check the shortest distance yourself and sort if you want by checking the vehicle type with for example VEHICLE::IS_THIS_MODEL_A_BOAT
		 * 
		 * -------------------------------------------------------------------------
		 * 
		 * Conclusion: This native is not worth trying to use. Use something like this instead: https://pastebin.com/xiFdXa7h
		 * 
		 * Full list of vehicles by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/vehicles.json
		 */
		Vehicle GET_CLOSEST_VEHICLE(float x, float y, float z, float radius, Hash modelHash, int flags);
		/**
		 * Corrected p1. it's basically the 'carriage/trailer number'. So if the train has 3 trailers you'd call the native once with a var or 3 times with 1, 2, 3.
		 */
		Vehicle GET_TRAIN_CARRIAGE(Vehicle train, int trailerNumber);
		void _SET_MAKE_TRAIN_SCAN_FOR_BLOCKING_ENTITIES(Vehicle train, BOOL toggle); // Missing in crossmap
		void _GET_MISSION_TRAIN_CONFIG_INDEX_BY_NAME(const char* name); // Missing in crossmap
		BOOL IS_MISSION_TRAIN(Vehicle train);
		void DELETE_MISSION_TRAIN(Vehicle* train);
		/**
		 * p1 is always 0
		 */
		void SET_MISSION_TRAIN_AS_NO_LONGER_NEEDED(Vehicle* train, BOOL p1);
		void SET_MISSION_TRAIN_COORDS(Vehicle train, float x, float y, float z);
		BOOL IS_THIS_MODEL_A_BOAT(Hash model);
		/**
		 * Checks if model is a boat, then checks for FLAG_IS_JETSKI.
		 */
		BOOL IS_THIS_MODEL_A_JETSKI(Hash model);
		BOOL IS_THIS_MODEL_A_PLANE(Hash model);
		BOOL IS_THIS_MODEL_A_HELI(Hash model);
		/**
		 * To check if the model is an amphibious car, use IS_THIS_MODEL_AN_AMPHIBIOUS_CAR.
		 */
		BOOL IS_THIS_MODEL_A_CAR(Hash model);
		BOOL IS_THIS_MODEL_A_TRAIN(Hash model);
		BOOL IS_THIS_MODEL_A_BIKE(Hash model);
		BOOL IS_THIS_MODEL_A_BICYCLE(Hash model);
		BOOL IS_THIS_MODEL_A_QUADBIKE(Hash model);
		BOOL IS_THIS_MODEL_AN_AMPHIBIOUS_CAR(Hash model);
		BOOL IS_THIS_MODEL_AN_AMPHIBIOUS_QUADBIKE(Hash model);
		/**
		 * Equivalent of SET_HELI_BLADES_SPEED(vehicleHandle, 1.0f);
		 */
		void SET_HELI_BLADES_FULL_SPEED(Vehicle vehicle);
		/**
		 * Sets the speed of the helicopter blades in percentage of the full speed.
		 * 
		 * vehicleHandle: The helicopter.
		 * speed: The speed in percentage, 0.0f being 0% and 1.0f being 100%.
		 */
		void SET_HELI_BLADES_SPEED(Vehicle vehicle, float speed);
		void FORCE_SUB_THROTTLE_FOR_TIME(Vehicle vehicle, float p1, float p2);
		/**
		 * This has not yet been tested - it's just an assumption of what the types could be.
		 */
		void SET_VEHICLE_CAN_BE_TARGETTED(Vehicle vehicle, BOOL state);
		void SET_DONT_ALLOW_PLAYER_TO_ENTER_VEHICLE_IF_LOCKED_FOR_PLAYER(Vehicle vehicle, BOOL p1);
		void SET_VEHICLE_CAN_BE_VISIBLY_DAMAGED(Vehicle vehicle, BOOL state);
		void SET_VEHICLE_HAS_UNBREAKABLE_LIGHTS(Vehicle vehicle, BOOL toggle);
		void SET_VEHICLE_RESPECTS_LOCKS_WHEN_HAS_DRIVER(Vehicle vehicle, BOOL p1);
		void SET_VEHICLE_CAN_EJECT_PASSENGERS_IF_LOCKED(Any p0, Any p1);
		/**
		 * Dirt level does not become greater than 15.0
		 */
		float GET_VEHICLE_DIRT_LEVEL(Vehicle vehicle);
		/**
		 * You can't use values greater than 15.0
		 * Also, R* does (float)(rand() % 15) to get a random dirt level when generating a vehicle.
		 */
		void SET_VEHICLE_DIRT_LEVEL(Vehicle vehicle, float dirtLevel);
		/**
		 * Appears to return true if the vehicle has any damage, including cosmetically.
		 */
		BOOL GET_DOES_VEHICLE_HAVE_DAMAGE_DECALS(Vehicle vehicle);
		/**
		 * doorId: see SET_VEHICLE_DOOR_SHUT
		 */
		BOOL IS_VEHICLE_DOOR_FULLY_OPEN(Vehicle vehicle, int doorId);
		/**
		 * Starts or stops the engine on the specified vehicle.
		 * 
		 * vehicle: The vehicle to start or stop the engine on.
		 * value: true to turn the vehicle on; false to turn it off.
		 * instantly: if true, the vehicle will be set to the state immediately; otherwise, the current driver will physically turn on or off the engine.
		 * disableAutoStart: If true, the system will prevent the engine from starting when the player got into it.
		 */
		void SET_VEHICLE_ENGINE_ON(Vehicle vehicle, BOOL value, BOOL instantly, BOOL disableAutoStart);
		void SET_VEHICLE_UNDRIVEABLE(Vehicle vehicle, BOOL toggle);
		void SET_VEHICLE_PROVIDES_COVER(Vehicle vehicle, BOOL toggle);
		/**
		 * doorId: see SET_VEHICLE_DOOR_SHUT
		 */
		void SET_VEHICLE_DOOR_CONTROL(Vehicle vehicle, int doorId, int speed, float angle);
		/**
		 * doorId: see SET_VEHICLE_DOOR_SHUT
		 */
		void SET_VEHICLE_DOOR_LATCHED(Vehicle vehicle, int doorId, BOOL p2, BOOL p3, BOOL p4);
		/**
		 * doorId: see SET_VEHICLE_DOOR_SHUT
		 */
		float GET_VEHICLE_DOOR_ANGLE_RATIO(Vehicle vehicle, int doorId);
		/**
		 * doorId: see SET_VEHICLE_DOOR_SHUT
		 */
		Ped GET_PED_USING_VEHICLE_DOOR(Vehicle vehicle, int doord);
		/**
		 * enum eDoorId
		 * {
		 * VEH_EXT_DOOR_INVALID_ID = -1,
		 * VEH_EXT_DOOR_DSIDE_F,
		 * VEH_EXT_DOOR_DSIDE_R,
		 * VEH_EXT_DOOR_PSIDE_F,
		 * VEH_EXT_DOOR_PSIDE_R,
		 * VEH_EXT_BONNET,
		 * VEH_EXT_BOOT
		 * };
		 */
		void SET_VEHICLE_DOOR_SHUT(Vehicle vehicle, int doorId, BOOL closeInstantly);
		/**
		 * doorId: see SET_VEHICLE_DOOR_SHUT
		 */
		void SET_VEHICLE_DOOR_BROKEN(Vehicle vehicle, int doorId, BOOL deleteDoor);
		void SET_VEHICLE_CAN_BREAK(Vehicle vehicle, BOOL toggle);
		BOOL DOES_VEHICLE_HAVE_ROOF(Vehicle vehicle);
		void SET_VEHICLE_REMOVE_AGGRESSIVE_CARJACK_MISSION(Any p0);
		void SET_VEHICLE_AVOID_PLAYER_VEHICLE_RIOT_VAN_MISSION(Any p0);
		void SET_CARJACK_MISSION_REMOVAL_PARAMETERS(Any p0, Any p1);
		/**
		 * Returns true if MF_IS_BIG (strModelFlags 0x8) handling model flag is set.
		 */
		BOOL IS_BIG_VEHICLE(Vehicle vehicle);
		/**
		 * Returns the total amount of color combinations found in the vehicle's carvariations.meta entry.
		 */
		int GET_NUMBER_OF_VEHICLE_COLOURS(Vehicle vehicle);
		/**
		 * Sets the selected vehicle's colors to the specified index of the color combination found in the vehicle's carvariations.meta entry.
		 */
		void SET_VEHICLE_COLOUR_COMBINATION(Vehicle vehicle, int colorCombination);
		/**
		 * Returns the index of the color combination found in the vehicle's carvariations.meta entry.
		 */
		int GET_VEHICLE_COLOUR_COMBINATION(Vehicle vehicle);
		/**
		 * `color`: is the paint index for the vehicle.
		 * Paint index goes from 0 to 12.
		 * Be aware that it only works on xenon lights. Example: https://i.imgur.com/yV3cpG9.png
		 * Full list of all vehicle xenon lights by DurtyFree https://github.com/DurtyFree/gta-v-data-dumps/blob/master/vehicleColors.json
		 */
		void SET_VEHICLE_XENON_LIGHT_COLOR_INDEX(Vehicle vehicle, int colorIndex);
		/**
		 * Returns the headlight color index from the vehicle. Value between 0, 12.
		 * Use SET_VEHICLE_XENON_LIGHT_COLOR_INDEX to set the headlights color for the vehicle.
		 * Must enable xenon headlights before it'll take affect.
		 * 
		 * Returns an int, value between 0-12 or 255 if no color is set.
		 */
		int GET_VEHICLE_XENON_LIGHT_COLOR_INDEX(Vehicle vehicle);
		/**
		 * Setting this to false, makes the specified vehicle to where if you press Y your character doesn't even attempt the animation to enter the vehicle. Hence it's not considered aka ignored.
		 */
		void SET_VEHICLE_IS_CONSIDERED_BY_PLAYER(Vehicle vehicle, BOOL toggle);
		void SET_VEHICLE_WILL_FORCE_OTHER_VEHICLES_TO_STOP(Vehicle vehicle, BOOL toggle);
		void SET_VEHICLE_ACT_AS_IF_HAS_SIREN_ON(Vehicle vehicle, BOOL p1);
		void SET_VEHICLE_USE_MORE_RESTRICTIVE_SPAWN_CHECKS(Vehicle vehicle, BOOL p1);
		void SET_VEHICLE_MAY_BE_USED_BY_GOTO_POINT_ANY_MEANS(Vehicle vehicle, BOOL p1);
		/**
		 * Not present in the retail version! It's just a nullsub.
		 * 
		 * p0 always true (except in one case)
		 * successIndicator: 0 if success, -1 if failed
		 */
		void GET_RANDOM_VEHICLE_MODEL_IN_MEMORY(BOOL p0, Hash* modelHash, int* successIndicator);
		/**
		 * enum VehicleLockStatus = {
		 * None = 0,
		 * Unlocked = 1,
		 * Locked = 2,
		 * LockedForPlayer = 3,
		 * StickPlayerInside = 4, -- Doesn't allow players to exit the vehicle with the exit vehicle key.
		 * CanBeBrokenInto = 7, -- Can be broken into the car. If the glass is broken, the value will be set to 1
		 * CanBeBrokenIntoPersist = 8, -- Can be broken into persist
		 * CannotBeTriedToEnter = 10, -- Cannot be tried to enter (Nothing happens when you press the vehicle enter key).
		 * }
		 */
		int GET_VEHICLE_DOOR_LOCK_STATUS(Vehicle vehicle);
		/**
		 * Returns vehicle door lock state previously set with SET_VEHICLE_INDIVIDUAL_DOORS_LOCKED
		 */
		int GET_VEHICLE_INDIVIDUAL_DOOR_LOCK_STATUS(Vehicle vehicle, int doorId);
		/**
		 * doorID starts at 0, not seeming to skip any numbers. Four door vehicles intuitively range from 0 to 3.
		 */
		BOOL IS_VEHICLE_DOOR_DAMAGED(Vehicle veh, int doorID);
		/**
		 * doorId: see SET_VEHICLE_DOOR_SHUT
		 */
		void SET_DOOR_ALLOWED_TO_BE_BROKEN_OFF(Vehicle vehicle, int doorId, BOOL isBreakable);
		BOOL IS_VEHICLE_BUMPER_BOUNCING(Vehicle vehicle, BOOL frontBumper);
		BOOL IS_VEHICLE_BUMPER_BROKEN_OFF(Vehicle vehicle, BOOL frontBumper);
		/**
		 * Usage:
		 * 
		 * public bool isCopInRange(Vector3 Location, float Range)
		 * {
		 * return Function.Call<bool>(Hash.IS_COP_PED_IN_AREA_3D, Location.X - Range, Location.Y - Range, Location.Z - Range, Location.X + Range, Location.Y + Range, Location.Z + Range);
		 * }
		 */
		BOOL IS_COP_VEHICLE_IN_AREA_3D(float x1, float x2, float y1, float y2, float z1, float z2);
		/**
		 * Public Function isVehicleOnAllWheels(vh As Vehicle) As Boolean
		 * Return Native.Function.Call(Of Boolean)(Hash.IS_VEHICLE_ON_ALL_WHEELS, vh)
		 * End Function
		 * 
		 */
		BOOL IS_VEHICLE_ON_ALL_WHEELS(Vehicle vehicle);
		/**
		 * Returns `nMonetaryValue` from handling.meta for specific model.
		 */
		int GET_VEHICLE_MODEL_VALUE(Hash vehicleModel);
		/**
		 * Full list of vehicles by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/vehicles.json
		 */
		Hash GET_VEHICLE_LAYOUT_HASH(Vehicle vehicle);
		Hash GET_IN_VEHICLE_CLIPSET_HASH_FOR_SEAT(Vehicle vehicle, int p1);
		/**
		 * Makes the train all jumbled up and derailed as it moves on the tracks (though that wont stop it from its normal operations)
		 */
		void SET_RENDER_TRAIN_AS_DERAILED(Vehicle train, BOOL toggle);
		/**
		 * They use the same color indexs as SET_VEHICLE_COLOURS.
		 */
		void SET_VEHICLE_EXTRA_COLOURS(Vehicle vehicle, int pearlescentColor, int wheelColor);
		void GET_VEHICLE_EXTRA_COLOURS(Vehicle vehicle, int* pearlescentColor, int* wheelColor);
		void SET_VEHICLE_EXTRA_COLOUR_5(Vehicle vehicle, int color);
		void GET_VEHICLE_EXTRA_COLOUR_5(Vehicle vehicle, int* color);
		void SET_VEHICLE_EXTRA_COLOUR_6(Vehicle vehicle, int color);
		void GET_VEHICLE_EXTRA_COLOUR_6(Vehicle vehicle, int* color);
		void STOP_ALL_GARAGE_ACTIVITY();
		/**
		 * This fixes a vehicle.
		 * If the vehicle's engine's broken then you cannot fix it with this native.
		 */
		void SET_VEHICLE_FIXED(Vehicle vehicle);
		/**
		 * This fixes the deformation of a vehicle but the vehicle health doesn't improve
		 */
		void SET_VEHICLE_DEFORMATION_FIXED(Vehicle vehicle);
		void SET_VEHICLE_CAN_ENGINE_MISSFIRE(Vehicle vehicle, BOOL toggle);
		void SET_VEHICLE_CAN_LEAK_OIL(Vehicle vehicle, BOOL toggle);
		void SET_VEHICLE_CAN_LEAK_PETROL(Vehicle vehicle, BOOL toggle);
		void SET_DISABLE_VEHICLE_PETROL_TANK_FIRES(Vehicle vehicle, BOOL toggle);
		void SET_DISABLE_VEHICLE_PETROL_TANK_DAMAGE(Vehicle vehicle, BOOL toggle);
		void SET_DISABLE_VEHICLE_ENGINE_FIRES(Vehicle vehicle, BOOL toggle);
		void SET_VEHICLE_LIMIT_SPEED_WHEN_PLAYER_INACTIVE(Vehicle vehicle, BOOL toggle);
		/**
		 * sfink: sets bit in vehicle's structure, used by maintransition, fm_mission_controller, mission_race and a couple of other scripts. see dissassembly:
		 * CVehicle *__fastcall sub_140CDAA10(signed int a1, char a2)
		 * {
		 * CVehicle *result; // rax@1
		 * 
		 * result = EntityAsCVehicle(a1);
		 * if ( result )
		 * {
		 * result->field_886 &= 0xEFu;
		 * result->field_886 |= 16 * (a2 & 1);
		 * }
		 * return result;
		 * }
		 */
		void SET_VEHICLE_STOP_INSTANTLY_WHEN_PLAYER_INACTIVE(Vehicle vehicle, BOOL toggle);
		void SET_DISABLE_PRETEND_OCCUPANTS(Vehicle vehicle, BOOL toggle);
		void REMOVE_VEHICLES_FROM_GENERATORS_IN_AREA(float x1, float y1, float z1, float x2, float y2, float z2, Any p6);
		/**
		 * Locks the vehicle's steering to the desired angle, explained below.
		 * 
		 * Requires to be called onTick. Steering is unlocked the moment the function stops being called on the vehicle.
		 * 
		 * Steer bias:
		 * -1.0 = full right
		 * 0.0 = centered steering
		 * 1.0 = full left
		 */
		void SET_VEHICLE_STEER_BIAS(Vehicle vehicle, float value);
		BOOL IS_VEHICLE_EXTRA_TURNED_ON(Vehicle vehicle, int extraId);
		/**
		 * Available extraIds are 1-14, however none of the vehicles have extras above 12.
		 */
		void SET_VEHICLE_EXTRA(Vehicle vehicle, int extraId, BOOL disable);
		/**
		 * Checks via CVehicleModelInfo
		 */
		BOOL DOES_EXTRA_EXIST(Vehicle vehicle, int extraId);
		/**
		 * Returns true if specified extra part is broken off. It only works for extras that can break off during collisions, non-breakable extras always return false. Also returns true if the breakable extra is toggled off through script.
		 */
		BOOL IS_EXTRA_BROKEN_OFF(Vehicle vehicle, int extraId);
		void SET_CONVERTIBLE_ROOF(Vehicle vehicle, BOOL p1);
		void LOWER_CONVERTIBLE_ROOF(Vehicle vehicle, BOOL instantlyLower);
		void RAISE_CONVERTIBLE_ROOF(Vehicle vehicle, BOOL instantlyRaise);
		/**
		 * 0 -> up
		 * 1 -> lowering down
		 * 2 -> down
		 * 3 -> raising up
		 */
		int GET_CONVERTIBLE_ROOF_STATE(Vehicle vehicle);
		/**
		 * Returns true if the vehicle has a convertible roof.
		 * 
		 * p1 is false almost always. However, in launcher_carwash/carwash1/carwash2 scripts, p1 is true and is accompanied by DOES_VEHICLE_HAVE_ROOF. If p1 is true, it seems that every single vehicle will return true irrespective of being a convertible.
		 */
		BOOL IS_VEHICLE_A_CONVERTIBLE(Vehicle vehicle, BOOL p1);
		/**
		 * Transforms the `stormberg`/`toreador` to its "submarine" variant. If the vehicle is already in that state then the vehicle transformation audio will still play, but the vehicle won't change at all.
		 */
		BOOL TRANSFORM_TO_SUBMARINE(Vehicle vehicle, BOOL noAnimation);
		/**
		 * Transforms the `stormberg`/`toreador` to its "road vehicle" variant. If the vehicle is already in that state then the vehicle transformation audio will still play, but the vehicle won't change at all.
		 */
		void TRANSFORM_TO_CAR(Vehicle vehicle, BOOL noAnimation);
		BOOL IS_VEHICLE_IN_SUBMARINE_MODE(Vehicle vehicle);
		BOOL IS_VEHICLE_STOPPED_AT_TRAFFIC_LIGHTS(Vehicle vehicle);
		/**
		 * Apply damage to vehicle at a location. Location is relative to vehicle model (not world).
		 * 
		 * Radius of effect damage applied in a sphere at impact location
		 * When `focusOnModel` set to `true`, the damage sphere will travel towards the vehicle from the given point, thus guaranteeing an impact
		 */
		void SET_VEHICLE_DAMAGE(Vehicle vehicle, float xOffset, float yOffset, float zOffset, float damage, float radius, BOOL focusOnModel);
		void SET_VEHICLE_OCCUPANTS_TAKE_EXPLOSIVE_DAMAGE(Vehicle vehicle, BOOL toggle);
		/**
		 * Returns 1000.0 if the function is unable to get the address of the specified vehicle or if it's not a vehicle.
		 * 
		 * Minimum: -4000
		 * Maximum: 1000
		 * 
		 * -4000: Engine is destroyed
		 * 0 and below: Engine catches fire and health rapidly declines
		 * 300: Engine is smoking and losing functionality
		 * 1000: Engine is perfect
		 */
		float GET_VEHICLE_ENGINE_HEALTH(Vehicle vehicle);
		/**
		 * 1000 is max health
		 * Begins leaking gas at around 650 health
		 * Minimum: -4000
		 * Maximum: 1000
		 * 
		 * -4000: Engine is destroyed
		 * 0 and below: Engine catches fire and health rapidly declines
		 * 300: Engine is smoking and losing functionality
		 * 1000: Engine is perfect
		 */
		void SET_VEHICLE_ENGINE_HEALTH(Vehicle vehicle, float health);
		/**
		 * Works just like SET_VEHICLE_ENGINE_HEALTH, but only for planes.
		 */
		void SET_PLANE_ENGINE_HEALTH(Vehicle vehicle, float health);
		/**
		 * 1000 is max health
		 * Begins leaking gas at around 650 health
		 * -999.90002441406 appears to be minimum health, although nothing special occurs
		 */
		float GET_VEHICLE_PETROL_TANK_HEALTH(Vehicle vehicle);
		/**
		 * 1000 is max health
		 * Begins leaking gas at around 650 health
		 * -999.90002441406 appears to be minimum health, although nothing special occurs
		 */
		void SET_VEHICLE_PETROL_TANK_HEALTH(Vehicle vehicle, float health);
		/**
		 * p1 can be anywhere from 0 to 3 in the scripts.
		 * p2 being how long in milliseconds the vehicle has been stuck
		 */
		BOOL IS_VEHICLE_STUCK_TIMER_UP(Vehicle vehicle, int p1, int ms);
		/**
		 * The inner function has a switch on the second parameter. It's the stuck timer index.
		 * 
		 * Here's some pseudo code I wrote for the inner function:
		 * void __fastcall NATIVE_RESET_VEHICLE_STUCK_TIMER_INNER(CUnknown* unknownClassInVehicle, int timerIndex)
		 * {
		 * switch (timerIndex)
		 * {
		 * case 0:
		 * unknownClassInVehicle->FirstStuckTimer = (WORD)0u;
		 * case 1:
		 * unknownClassInVehicle->SecondStuckTimer = (WORD)0u;
		 * case 2:
		 * unknownClassInVehicle->ThirdStuckTimer = (WORD)0u;
		 * case 3:
		 * unknownClassInVehicle->FourthStuckTimer = (WORD)0u;
		 * case 4:
		 * unknownClassInVehicle->FirstStuckTimer = (WORD)0u;
		 * unknownClassInVehicle->SecondStuckTimer = (WORD)0u;
		 * unknownClassInVehicle->ThirdStuckTimer = (WORD)0u;
		 * unknownClassInVehicle->FourthStuckTimer = (WORD)0u;
		 * break;
		 * };
		 * }
		 */
		void RESET_VEHICLE_STUCK_TIMER(Vehicle vehicle, int nullAttributes);
		/**
		 * p1 is always 0 in the scripts.
		 * 
		 * p1 = check if vehicle is on fire
		 */
		BOOL IS_VEHICLE_DRIVEABLE(Vehicle vehicle, BOOL isOnFireCheck);
		void SET_VEHICLE_HAS_BEEN_OWNED_BY_PLAYER(Vehicle vehicle, BOOL owned);
		void SET_VEHICLE_NEEDS_TO_BE_HOTWIRED(Vehicle vehicle, BOOL toggle);
		void SET_VEHICLE_BLIP_THROTTLE_RANDOMLY(Vehicle vehicle, BOOL p1);
		void SET_POLICE_FOCUS_WILL_TRACK_VEHICLE(Vehicle vehicle, BOOL toggle);
		/**
		 * Sounds the horn for the specified vehicle.
		 * 
		 * vehicle: The vehicle to activate the horn for.
		 * mode: The hash of "NORMAL" or "HELDDOWN". Can be 0.
		 * duration: The duration to sound the horn, in milliseconds.
		 * 
		 * Note: If a player is in the vehicle, it will only sound briefly.
		 */
		void START_VEHICLE_HORN(Vehicle vehicle, int duration, Hash mode, BOOL forever);
		/**
		 * If set to TRUE, it seems to suppress door noises and doesn't allow the horn to be continuous.
		 */
		void SET_VEHICLE_IN_CAR_MOD_SHOP(Vehicle vehicle, BOOL toggle);
		/**
		 * if true, axles won't bend.
		 */
		void SET_VEHICLE_HAS_STRONG_AXLES(Vehicle vehicle, BOOL toggle);
		/**
		 * Returns model name of vehicle in all caps. Needs to be displayed through localizing text natives to get proper display name.
		 * -----------------------------------------------------------------------------------------------------------------------------------------
		 * While often the case, this does not simply return the model name of the vehicle (which could be hashed to return the model hash). Variations of the same vehicle may also use the same display name.
		 * -----------------------------------------------------------------------------------------------------------------------------------------
		 * 
		 * Returns "CARNOTFOUND" if the hash doesn't match a vehicle hash.
		 * 
		 * Using HUD::GET_FILENAME_FOR_AUDIO_CONVERSATION, you can get the localized name.
		 * 
		 * Full list of vehicles by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/vehicles.json
		 */
		const char* GET_DISPLAY_NAME_FROM_VEHICLE_MODEL(Hash modelHash);
		/**
		 * Will return a vehicle's manufacturer display label.
		 * Returns "CARNOTFOUND" if the hash doesn't match a vehicle hash.
		 * 
		 * Full list of vehicles by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/vehicles.json
		 */
		const char* GET_MAKE_NAME_FROM_VEHICLE_MODEL(Hash modelHash);
		/**
		 * The only example I can find of this function in the scripts, is this:
		 * 
		 * struct _s = VEHICLE::GET_VEHICLE_DEFORMATION_AT_POS(rPtr((A_0) + 4), 1.21f, 6.15f, 0.3f);
		 * 
		 * -----------------------------------------------------------------------------------------------------------------------------------------
		 * PC scripts:
		 * 
		 * v_5/*{3}* / = VEHICLE::GET_VEHICLE_DEFORMATION_AT_POS(a_0._f1, 1.21, 6.15, 0.3);
		 */
		Vector3 GET_VEHICLE_DEFORMATION_AT_POS(Vehicle vehicle, float offsetX, float offsetY, float offsetZ);
		void SET_VEHICLE_LIVERY(Vehicle vehicle, int livery);
		/**
		 * -1 = no livery
		 */
		int GET_VEHICLE_LIVERY(Vehicle vehicle);
		/**
		 * Returns -1 if the vehicle has no livery
		 */
		int GET_VEHICLE_LIVERY_COUNT(Vehicle vehicle);
		/**
		 * Used to set the secondary livery (the roof on Tornado Custom being one such example.)
		 * 
		 * Livery value is dependent on the amount of liveries present in the vehicle's texture dictionary, for Tornado Custom this would be 0-6.
		 */
		void SET_VEHICLE_LIVERY2(Vehicle vehicle, int livery);
		/**
		 * Returns index of the current vehicle's secondary livery. A getter for SET_VEHICLE_LIVERY2.
		 */
		int GET_VEHICLE_LIVERY2(Vehicle vehicle);
		/**
		 * Returns a number of available secondary liveries, or -1 if vehicle has no secondary liveries available.
		 */
		int GET_VEHICLE_LIVERY2_COUNT(Vehicle vehicle);
		/**
		 * This will return false if the window is broken, or rolled down.
		 * Window indexes:
		 * 0 = Front Right Window
		 * 1 = Front Left Window
		 * 2 = Back Right Window
		 * 3 = Back Left Window
		 * 
		 * 
		 * Those numbers go on for vehicles that have more than 4 doors with windows.
		 */
		BOOL IS_VEHICLE_WINDOW_INTACT(Vehicle vehicle, int windowIndex);
		/**
		 * Appears to return false if any window is broken.
		 */
		BOOL ARE_ALL_VEHICLE_WINDOWS_INTACT(Vehicle vehicle);
		/**
		 * Returns false if every seat is occupied.
		 */
		BOOL ARE_ANY_VEHICLE_SEATS_FREE(Vehicle vehicle);
		void RESET_VEHICLE_WHEELS(Vehicle vehicle, BOOL toggle);
		BOOL IS_HELI_PART_BROKEN(Vehicle vehicle, BOOL p1, BOOL p2, BOOL p3);
		/**
		 * Max 1000.
		 * At 0 the main rotor will stall.
		 */
		float GET_HELI_MAIN_ROTOR_HEALTH(Vehicle vehicle);
		/**
		 * Max 1000.
		 * At 0 the tail rotor will stall.
		 */
		float GET_HELI_TAIL_ROTOR_HEALTH(Vehicle vehicle);
		/**
		 * Max 1000.
		 * At -100 both helicopter rotors will stall.
		 */
		float GET_HELI_TAIL_BOOM_HEALTH(Vehicle vehicle);
		void SET_HELI_MAIN_ROTOR_HEALTH(Vehicle vehicle, float health);
		void SET_HELI_TAIL_ROTOR_HEALTH(Vehicle vehicle, float health);
		BOOL SET_HELI_TAIL_BOOM_CAN_BREAK_OFF(Vehicle vehicle, BOOL toggle);
		/**
		 * NOTE: Debugging functions are not present in the retail version of the game.
		 */
		void SET_VEHICLE_NAME_DEBUG(Vehicle vehicle, const char* name);
		/**
		 * Sets a vehicle to be strongly resistant to explosions. p0 is the vehicle; set p1 to false to toggle the effect on/off.
		 */
		void SET_VEHICLE_EXPLODES_ON_HIGH_EXPLOSION_DAMAGE(Vehicle vehicle, BOOL toggle);
		void SET_VEHICLE_EXPLODES_ON_EXPLOSION_DAMAGE_AT_ZERO_BODY_HEALTH(Vehicle vehicle, BOOL toggle);
		void SET_ALLOW_VEHICLE_EXPLODES_ON_CONTACT(Vehicle vehicle, BOOL toggle);
		void SET_VEHICLE_DISABLE_TOWING(Vehicle vehicle, BOOL toggle);
		BOOL GET_VEHICLE_HAS_LANDING_GEAR(Vehicle vehicle);
		/**
		 * Works for vehicles with a retractable landing gear
		 * 
		 * Landing gear states:
		 * 
		 * 0: Deployed
		 * 1: Closing
		 * 2: Opening
		 * 3: Retracted
		 */
		void CONTROL_LANDING_GEAR(Vehicle vehicle, int state);
		/**
		 * Landing gear states:
		 * 
		 * 0: Deployed
		 * 1: Closing (Retracting)
		 * 2: (Landing gear state 2 is never used.)
		 * 3: Opening (Deploying)
		 * 4: Retracted
		 * 
		 * Returns the current state of the vehicles landing gear.
		 */
		int GET_LANDING_GEAR_STATE(Vehicle vehicle);
		BOOL IS_ANY_VEHICLE_NEAR_POINT(float x, float y, float z, float radius);
		void REQUEST_VEHICLE_HIGH_DETAIL_MODEL(Vehicle vehicle);
		int _GET_VEHICLE_MODEL_NUM_DRIVE_GEARS(Hash vehicleModel);
		int _GET_VEHICLE_MAX_DRIVE_GEAR_COUNT(Vehicle vehicle);
		BOOL _GET_IS_VEHICLE_ELECTRIC(Hash vehicleModel);
		/**
		 * Returns the vehicle's drivetrain type.
		 * 
		 * enum eVehicleDrivetrainType
		 * {
		 * VehicleDrivetrainType_INVALID,
		 * VehicleDrivetrainType_FWD,
		 * VehicleDrivetrainType_RWD,
		 * VehicleDrivetrainType_AWD
		 * };
		 * 
		 * _GET_VEHICLE_DE* - _GET_VEHICLE_DI*
		 */
		int _GET_VEHICLE_DRIVETRAIN_TYPE(Hash vehicleModel);
		int _GET_VEHICLE_CURRENT_DRIVE_GEAR(Vehicle vehicle);
		float _GET_VEHICLE_CURRENT_REV_RATIO(Vehicle vehicle);
		void REMOVE_VEHICLE_HIGH_DETAIL_MODEL(Vehicle vehicle);
		BOOL IS_VEHICLE_HIGH_DETAIL(Vehicle vehicle);
		/**
		 * REQUEST_VEHICLE_ASSET(GET_HASH_KEY(cargobob3), 3);
		 * 
		 * vehicle found that have asset's:
		 * cargobob3
		 * submersible
		 * blazer
		 */
		void REQUEST_VEHICLE_ASSET(Hash vehicleHash, int vehicleAsset);
		BOOL HAS_VEHICLE_ASSET_LOADED(int vehicleAsset);
		void REMOVE_VEHICLE_ASSET(int vehicleAsset);
		/**
		 * Sets how much the crane on the tow truck is raised, where 0.0 is fully lowered and 1.0 is fully raised.
		 */
		void SET_VEHICLE_TOW_TRUCK_ARM_POSITION(Vehicle vehicle, float position);
		void _SET_ATTACHED_VEHICLE_TO_TOW_TRUCK_ARM(Vehicle towTruck, Vehicle vehicle);
		/**
		 * HookOffset defines where the hook is attached. leave at 0 for default attachment.
		 */
		void ATTACH_VEHICLE_TO_TOW_TRUCK(Vehicle towTruck, Vehicle vehicle, BOOL rear, float hookOffsetX, float hookOffsetY, float hookOffsetZ);
		/**
		 * First two parameters swapped. Scripts verify that towTruck is the first parameter, not the second.
		 */
		void DETACH_VEHICLE_FROM_TOW_TRUCK(Vehicle towTruck, Vehicle vehicle);
		BOOL DETACH_VEHICLE_FROM_ANY_TOW_TRUCK(Vehicle vehicle);
		/**
		 * Scripts verify that towTruck is the first parameter, not the second.
		 */
		BOOL IS_VEHICLE_ATTACHED_TO_TOW_TRUCK(Vehicle towTruck, Vehicle vehicle);
		Entity GET_ENTITY_ATTACHED_TO_TOW_TRUCK(Vehicle towTruck);
		Entity SET_VEHICLE_AUTOMATICALLY_ATTACHES(Vehicle vehicle, BOOL p1, Any p2);
		/**
		 * Sets the arm position of a bulldozer. Position must be a value between 0.0 and 1.0. Ignored when `p2` is set to false, instead incrementing arm position by 0.1 (or 10%).
		 */
		void SET_VEHICLE_BULLDOZER_ARM_POSITION(Vehicle vehicle, float position, BOOL p2);
		void SET_VEHICLE_TANK_TURRET_POSITION(Vehicle vehicle, float position, BOOL p2);
		void SET_VEHICLE_TURRET_TARGET(Vehicle vehicle, BOOL p1, float x, float y, float z, BOOL p5);
		void SET_VEHICLE_TANK_STATIONARY(Vehicle vehicle, BOOL p1);
		void SET_VEHICLE_TURRET_SPEED_THIS_FRAME(Vehicle vehicle, float speed);
		void DISABLE_VEHICLE_TURRET_MOVEMENT_THIS_FRAME(Vehicle vehicle);
		void SET_VEHICLE_FLIGHT_NOZZLE_POSITION(Vehicle vehicle, float angleRatio);
		void SET_VEHICLE_FLIGHT_NOZZLE_POSITION_IMMEDIATE(Vehicle vehicle, float angle);
		float GET_VEHICLE_FLIGHT_NOZZLE_POSITION(Vehicle plane);
		/**
		 * True stops vtols from switching modes. Doesn't stop the sound though.
		 */
		void SET_DISABLE_VERTICAL_FLIGHT_MODE_TRANSITION(Vehicle vehicle, BOOL toggle);
		BOOL GENERATE_VEHICLE_CREATION_POS_FROM_PATHS(Vector3* outVec, Any p1, Vector3* outVec1, Any p3, Any p4, Any p5, Any p6, Any p7, Any p8);
		/**
		 * On accelerating, spins the driven wheels with the others braked, so you don't go anywhere.
		 */
		void SET_VEHICLE_BURNOUT(Vehicle vehicle, BOOL toggle);
		/**
		 * Returns whether the specified vehicle is currently in a burnout.
		 * 
		 * 
		 * vb.net
		 * Public Function isVehicleInBurnout(vh As Vehicle) As Boolean
		 * Return Native.Function.Call(Of Boolean)(Hash.IS_VEHICLE_IN_BURNOUT, vh)
		 * End Function
		 */
		BOOL IS_VEHICLE_IN_BURNOUT(Vehicle vehicle);
		/**
		 * Reduces grip significantly so it's hard to go anywhere.
		 */
		void SET_VEHICLE_REDUCE_GRIP(Vehicle vehicle, BOOL toggle);
		/**
		 * val is 0-3
		 * Often used in conjunction with: SET_VEHICLE_REDUCE_GRIP
		 */
		void SET_VEHICLE_REDUCE_GRIP_LEVEL(Vehicle vehicle, int val);
		/**
		 * Sets the turn signal enabled for a vehicle.
		 * Set turnSignal to 1 for left light, 0 for right light.
		 */
		void SET_VEHICLE_INDICATOR_LIGHTS(Vehicle vehicle, int turnSignal, BOOL toggle);
		void SET_VEHICLE_BRAKE_LIGHTS(Vehicle vehicle, BOOL toggle);
		void SET_VEHICLE_TAIL_LIGHTS(Vehicle vehicle, BOOL toggle);
		void SET_VEHICLE_HANDBRAKE(Vehicle vehicle, BOOL toggle);
		void SET_VEHICLE_BRAKE(Vehicle vehicle, BOOL toggle);
		void INSTANTLY_FILL_VEHICLE_POPULATION();
		BOOL HAS_INSTANT_FILL_VEHICLE_POPULATION_FINISHED();
		void NETWORK_ENABLE_EMPTY_CROWDING_VEHICLES_REMOVAL(BOOL toggle);
		/**
		 * Default:1000||This sets a value which is used when NETWORK_ENABLE_EMPTY_CROWDING_VEHICLES_REMOVAL(true) is called each frame.
		 */
		void NETWORK_CAP_EMPTY_CROWDING_VEHICLES_REMOVAL(int p0);
		/**
		 * Gets the trailer of a vehicle and puts it into the trailer parameter.
		 */
		BOOL GET_VEHICLE_TRAILER_VEHICLE(Vehicle vehicle, Vehicle* trailer);
		Vehicle _GET_VEHICLE_TRAILER_PARENT_VEHICLE(Vehicle trailer);
		/**
		 * vehicle must be a plane
		 */
		void SET_VEHICLE_USES_LARGE_REAR_RAMP(Vehicle vehicle, BOOL toggle);
		void SET_VEHICLE_RUDDER_BROKEN(Vehicle vehicle, BOOL toggle);
		void SET_VEHICLE_TAIL_BROKEN(Vehicle vehicle, BOOL toggle); // Missing in crossmap
		void SET_CONVERTIBLE_ROOF_LATCH_STATE(Vehicle vehicle, BOOL state);
		float GET_VEHICLE_ESTIMATED_MAX_SPEED(Vehicle vehicle);
		float GET_VEHICLE_MAX_BRAKING(Vehicle vehicle);
		float GET_VEHICLE_MAX_TRACTION(Vehicle vehicle);
		/**
		 * static - max acceleration
		 */
		float GET_VEHICLE_ACCELERATION(Vehicle vehicle);
		/**
		 * Returns max speed (without mods) of the specified vehicle model in m/s.
		 * 
		 * Full list of vehicles by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/vehicles.json
		 */
		float GET_VEHICLE_MODEL_ESTIMATED_MAX_SPEED(Hash modelHash);
		/**
		 * Returns max braking of the specified vehicle model.
		 * 
		 * Full list of vehicles by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/vehicles.json
		 */
		float GET_VEHICLE_MODEL_MAX_BRAKING(Hash modelHash);
		/**
		 * Full list of vehicles by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/vehicles.json
		 */
		float GET_VEHICLE_MODEL_MAX_BRAKING_MAX_MODS(Hash modelHash);
		/**
		 * Returns max traction of the specified vehicle model.
		 * 
		 * Full list of vehicles by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/vehicles.json
		 */
		float GET_VEHICLE_MODEL_MAX_TRACTION(Hash modelHash);
		/**
		 * Returns the acceleration of the specified model.
		 * 
		 * Full list of vehicles by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/vehicles.json
		 */
		float GET_VEHICLE_MODEL_ACCELERATION(Hash modelHash);
		/**
		 * 9.8 * thrust if air vehicle, else 0.38 + drive force?
		 * 
		 * Full list of vehicles by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/vehicles.json
		 */
		float GET_VEHICLE_MODEL_ACCELERATION_MAX_MODS(Hash modelHash);
		/**
		 * Full list of vehicles by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/vehicles.json
		 */
		float GET_FLYING_VEHICLE_MODEL_AGILITY(Hash modelHash);
		/**
		 * Full list of vehicles by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/vehicles.json
		 */
		float GET_BOAT_VEHICLE_MODEL_AGILITY(Hash modelHash);
		float GET_VEHICLE_CLASS_ESTIMATED_MAX_SPEED(int vehicleClass);
		float GET_VEHICLE_CLASS_MAX_TRACTION(int vehicleClass);
		float GET_VEHICLE_CLASS_MAX_AGILITY(int vehicleClass);
		float GET_VEHICLE_CLASS_MAX_ACCELERATION(int vehicleClass);
		float GET_VEHICLE_CLASS_MAX_BRAKING(int vehicleClass);
		int ADD_ROAD_NODE_SPEED_ZONE(float x, float y, float z, float radius, float speed, BOOL p5);
		BOOL REMOVE_ROAD_NODE_SPEED_ZONE(int speedzone);
		void OPEN_BOMB_BAY_DOORS(Vehicle vehicle);
		void CLOSE_BOMB_BAY_DOORS(Vehicle vehicle);
		/**
		 * Returns true when the bomb bay doors of this plane are open. False if they're closed.
		 */
		BOOL GET_ARE_BOMB_BAY_DOORS_OPEN(Vehicle aircraft);
		/**
		 * Possibly: Returns whether the searchlight (found on police vehicles) is toggled on.
		 * 
		 * @Author Nac
		 */
		BOOL IS_VEHICLE_SEARCHLIGHT_ON(Vehicle vehicle);
		/**
		 * Only works during nighttime.
		 */
		void SET_VEHICLE_SEARCHLIGHT(Vehicle heli, BOOL toggle, BOOL canBeUsedByAI);
		BOOL DOES_VEHICLE_HAVE_SEARCHLIGHT(Vehicle vehicle);
		/**
		 * Check if a vehicle seat is accessible. If you park your vehicle near a wall and the ped cannot enter/exit this side, the return value toggles from true (not blocked) to false (blocked).
		 * 
		 * seatIndex  = -1 being the driver seat.
		 * Use GET_VEHICLE_MAX_NUMBER_OF_PASSENGERS(vehicle) - 1 for last seat index.
		 * side = only relevant for bikes/motorcycles to check if the left (false)/right (true) side is blocked.
		 * onEnter = check if you can enter (true) or exit (false) a vehicle.
		 */
		BOOL IS_ENTRY_POINT_FOR_SEAT_CLEAR(Ped ped, Vehicle vehicle, int seatIndex, BOOL side, BOOL onEnter);
		/**
		 * doorId: see SET_VEHICLE_DOOR_SHUT
		 */
		Vector3 GET_ENTRY_POINT_POSITION(Vehicle vehicle, int doorId);
		BOOL CAN_SHUFFLE_SEAT(Vehicle vehicle, int seatIndex);
		int GET_NUM_MOD_KITS(Vehicle vehicle);
		/**
		 * Set modKit to 0 if you plan to call SET_VEHICLE_MOD. That's what the game does. Most body modifications through SET_VEHICLE_MOD will not take effect until this is set to 0.
		 * 
		 * Full list of vehicle mod kits and mods by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/vehicleModKits.json
		 */
		void SET_VEHICLE_MOD_KIT(Vehicle vehicle, int modKit);
		int GET_VEHICLE_MOD_KIT(Vehicle vehicle);
		int GET_VEHICLE_MOD_KIT_TYPE(Vehicle vehicle);
		/**
		 * Returns an int
		 * 
		 * Wheel Types:
		 * 0: Sport
		 * 1: Muscle
		 * 2: Lowrider
		 * 3: SUV
		 * 4: Offroad
		 * 5: Tuner
		 * 6: Bike Wheels
		 * 7: High End
		 * 8: Benny's Originals
		 * 9: Benny's Bespoke
		 * 10: Racing
		 * 11: Street
		 * 12: Track
		 * 
		 * Tested in Los Santos Customs
		 */
		int GET_VEHICLE_WHEEL_TYPE(Vehicle vehicle);
		/**
		 * 0: Sport
		 * 1: Muscle
		 * 2: Lowrider
		 * 3: SUV
		 * 4: Offroad
		 * 5: Tuner
		 * 6: Bike Wheels
		 * 7: High End
		 * 8: Benny's Originals
		 * 9: Benny's Bespoke
		 * 10: Racing
		 * 11: Street
		 * 12: Track
		 */
		void SET_VEHICLE_WHEEL_TYPE(Vehicle vehicle, int WheelType);
		/**
		 * paintType:
		 * 0: Normal
		 * 1: Metallic
		 * 2: Pearl
		 * 3: Matte
		 * 4: Metal
		 * 5: Chrome
		 * 6: Chameleon
		 */
		int GET_NUM_MOD_COLORS(int paintType, BOOL p1);
		/**
		 * paintType:
		 * 0: Normal
		 * 1: Metallic
		 * 2: Pearl
		 * 3: Matte
		 * 4: Metal
		 * 5: Chrome
		 * 6: Chameleon
		 * 
		 * color: number of the color.
		 * 
		 * p3 seems to always be 0.
		 * 
		 * Full list of vehicle colors and vehicle plates by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/vehicleColors.json
		 */
		void SET_VEHICLE_MOD_COLOR_1(Vehicle vehicle, int paintType, int color, int pearlescentColor);
		/**
		 * Changes the secondary paint type and color
		 * paintType:
		 * 0: Normal
		 * 1: Metallic
		 * 2: Pearl
		 * 3: Matte
		 * 4: Metal
		 * 5: Chrome
		 * 6: Chameleon
		 * 
		 * color: number of the color
		 * 
		 * Full list of vehicle colors and vehicle plates by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/vehicleColors.json
		 */
		void SET_VEHICLE_MOD_COLOR_2(Vehicle vehicle, int paintType, int color);
		void GET_VEHICLE_MOD_COLOR_1(Vehicle vehicle, int* paintType, int* color, int* pearlescentColor);
		void GET_VEHICLE_MOD_COLOR_2(Vehicle vehicle, int* paintType, int* color);
		/**
		 * Returns a string which is the codename of the vehicle's currently selected primary color
		 * 
		 * p1 is always 0
		 */
		const char* GET_VEHICLE_MOD_COLOR_1_NAME(Vehicle vehicle, BOOL p1);
		/**
		 * Returns a string which is the codename of the vehicle's currently selected secondary color
		 */
		const char* GET_VEHICLE_MOD_COLOR_2_NAME(Vehicle vehicle);
		BOOL HAVE_VEHICLE_MODS_STREAMED_IN(Vehicle vehicle);
		/**
		 * Returns true for any mod part listed in GEN9_EXCLUSIVE_ASSETS_VEHICLES_FILE.
		 */
		BOOL IS_VEHICLE_MOD_GEN9_EXCLUSIVE(Vehicle vehicle, int modType, int modIndex);
		/**
		 * In b944, there are 50 (0 - 49) mod types.
		 * 
		 * Sets the vehicle mod.
		 * The vehicle must have a mod kit first.
		 * 
		 * Any out of range ModIndex is stock.
		 * 
		 * #Mod Type
		 * Spoilers - 0
		 * Front Bumper - 1
		 * Rear Bumper - 2
		 * Side Skirt - 3
		 * Exhaust - 4
		 * Frame - 5
		 * Grille - 6
		 * Hood - 7
		 * Fender - 8
		 * Right Fender - 9
		 * Roof - 10
		 * Engine - 11
		 * Brakes - 12
		 * Transmission - 13
		 * Horns - 14 (modIndex from 0 to 51)
		 * Suspension - 15
		 * Armor - 16
		 * Front Wheels - 23
		 * Back Wheels - 24 //only for motocycles
		 * Plate holders - 25
		 * Trim Design - 27
		 * Ornaments - 28
		 * Dial Design - 30
		 * Steering Wheel - 33
		 * Shifter Leavers - 34
		 * Plaques - 35
		 * Hydraulics - 38
		 * Livery - 48
		 * 
		 * ENUMS: https://pastebin.com/QzEAn02v
		 */
		void SET_VEHICLE_MOD(Vehicle vehicle, int modType, int modIndex, BOOL customTires);
		/**
		 * In b944, there are 50 (0 - 49) mod types. See SET_VEHICLE_MOD for the list.
		 * 
		 * Returns -1 if the vehicle mod is stock
		 */
		int GET_VEHICLE_MOD(Vehicle vehicle, int modType);
		/**
		 * Only used for wheels(ModType = 23/24) Returns true if the wheels are custom wheels
		 */
		int GET_VEHICLE_MOD_VARIATION(Vehicle vehicle, int modType);
		/**
		 * Returns how many possible mods a vehicle has for a given mod type
		 */
		int GET_NUM_VEHICLE_MODS(Vehicle vehicle, int modType);
		void REMOVE_VEHICLE_MOD(Vehicle vehicle, int modType);
		/**
		 * Toggles:
		 * UNK17 - 17
		 * Turbo - 18
		 * UNK19 - 19
		 * Tire Smoke - 20
		 * UNK21 - 21
		 * Xenon Headlights - 22
		 */
		void TOGGLE_VEHICLE_MOD(Vehicle vehicle, int modType, BOOL toggle);
		BOOL IS_TOGGLE_MOD_ON(Vehicle vehicle, int modType);
		/**
		 * Returns the text label of a mod type for a given vehicle
		 * 
		 * Use GET_FILENAME_FOR_AUDIO_CONVERSATION to get the part name in the game's language
		 */
		const char* GET_MOD_TEXT_LABEL(Vehicle vehicle, int modType, int modValue);
		/**
		 * Returns the name for the type of vehicle mod(Armour, engine etc)
		 * 
		 */
		const char* GET_MOD_SLOT_NAME(Vehicle vehicle, int modType);
		/**
		 * Returns the text label of the vehicle's liveryIndex, as specified by the liveryNames section of the vehicle's modkit data in the carcols file.
		 * 
		 * example
		 * 
		 * int count = VEHICLE::GET_VEHICLE_LIVERY_COUNT(veh);
		 * for (int i = 0; i < count; i++)
		 * {
		 * const char* LiveryName = VEHICLE::GET_LIVERY_NAME(veh, i);
		 * }
		 * 
		 * 
		 * this example will work fine to fetch all names
		 * for example for Sanchez we get
		 * 
		 * SANC_LV1
		 * SANC_LV2
		 * SANC_LV3
		 * SANC_LV4
		 * SANC_LV5
		 * 
		 * 
		 * Use GET_FILENAME_FOR_AUDIO_CONVERSATION, to get the localized livery name.
		 * 
		 * Full list of vehicle mod kits and mods by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/vehicleModKits.json
		 */
		const char* GET_LIVERY_NAME(Vehicle vehicle, int liveryIndex);
		int GET_VEHICLE_MOD_MODIFIER_VALUE(Vehicle vehicle, int modType, int modIndex);
		/**
		 * Can be used for IS_DLC_VEHICLE_MOD and GET_DLC_VEHICLE_MOD_LOCK_HASH
		 */
		Hash GET_VEHICLE_MOD_IDENTIFIER_HASH(Vehicle vehicle, int modType, int modIndex);
		void PRELOAD_VEHICLE_MOD(Vehicle vehicle, int modType, int modIndex);
		BOOL HAS_PRELOAD_MODS_FINISHED(Vehicle vehicle);
		void RELEASE_PRELOAD_MODS(Vehicle vehicle);
		/**
		 * Sets the tire smoke's color of this vehicle.
		 * 
		 * vehicle: The vehicle that is the target of this method.
		 * r: The red level in the RGB color code.
		 * g: The green level in the RGB color code.
		 * b: The blue level in the RGB color code.
		 * 
		 * Note: setting r,g,b to 0 will give the car the "Patriot" tire smoke.
		 */
		void SET_VEHICLE_TYRE_SMOKE_COLOR(Vehicle vehicle, int r, int g, int b);
		void GET_VEHICLE_TYRE_SMOKE_COLOR(Vehicle vehicle, int* r, int* g, int* b);
		/**
		 * enum WindowTints
		 * {
		 * WINDOWTINT_NONE,
		 * WINDOWTINT_PURE_BLACK,
		 * WINDOWTINT_DARKSMOKE,
		 * WINDOWTINT_LIGHTSMOKE,
		 * WINDOWTINT_STOCK,
		 * WINDOWTINT_LIMO,
		 * WINDOWTINT_GREEN
		 * };
		 * Full list of all vehicle window tints by DurtyFree https://github.com/DurtyFree/gta-v-data-dumps/blob/master/vehicleColors.json
		 */
		void SET_VEHICLE_WINDOW_TINT(Vehicle vehicle, int tint);
		int GET_VEHICLE_WINDOW_TINT(Vehicle vehicle);
		int GET_NUM_VEHICLE_WINDOW_TINTS();
		/**
		 * What's this for? Primary and Secondary RGB have their own natives and this one doesn't seem specific.
		 */
		void GET_VEHICLE_COLOR(Vehicle vehicle, int* r, int* g, int* b);
		/**
		 * Some kind of flags.
		 */
		int GET_VEHICLE_COLOURS_WHICH_CAN_BE_SET(Vehicle vehicle);
		/**
		 * iVar3 = get_vehicle_cause_of_destruction(uLocal_248[iVar2]);
		 * if (iVar3 == joaat("weapon_stickybomb"))
		 * {
		 * func_171(726);
		 * iLocal_260 = 1;
		 * }
		 */
		Hash GET_VEHICLE_CAUSE_OF_DESTRUCTION(Vehicle vehicle);
		/**
		 * Used for helis.
		 */
		void OVERRIDE_PLANE_DAMAGE_THREHSOLD(Vehicle vehicle, float health);
		void _SET_TRANSMISSION_REDUCED_GEAR_RATIO(Vehicle vehicle, BOOL toggle);
		int _GET_VEHICLE_DESIRED_DRIVE_GEAR(Vehicle vehicle);
		/**
		 * From the driver's perspective, is the left headlight broken.
		 */
		BOOL GET_IS_LEFT_VEHICLE_HEADLIGHT_DAMAGED(Vehicle vehicle);
		/**
		 * From the driver's perspective, is the right headlight broken.
		 */
		BOOL GET_IS_RIGHT_VEHICLE_HEADLIGHT_DAMAGED(Vehicle vehicle);
		/**
		 * Returns true when both headlights are broken. This does not include extralights.
		 */
		BOOL GET_BOTH_VEHICLE_HEADLIGHTS_DAMAGED(Vehicle vehicle);
		void MODIFY_VEHICLE_TOP_SPEED(Vehicle vehicle, float value);
		/**
		 * To reset the max speed, set the `speed` value to `0.0` or lower.
		 */
		void SET_VEHICLE_MAX_SPEED(Vehicle vehicle, float speed);
		/**
		 * Has something to do with trains. Always precedes SET_MISSION_TRAIN_AS_NO_LONGER_NEEDED.
		 * May be true that it can be used with trains not sure, but not specifically for trains. Go find Xbox360 decompiled scripts and search for 'func_1333' in freemode.c it isn't used just for trains. Thanks for the info tho.
		 */
		void SET_VEHICLE_STAYS_FROZEN_WHEN_CLEANED_UP(Vehicle vehicle, BOOL toggle);
		void SET_VEHICLE_ACT_AS_IF_HIGH_SPEED_FOR_FRAG_SMASHING(Vehicle vehicle, BOOL p1);
		/**
		 * Sets some bit and float of vehicle. float is >= 0
		 */
		void SET_PEDS_CAN_FALL_OFF_THIS_VEHICLE_FROM_LARGE_FALL_DAMAGE(Vehicle vehicle, BOOL toggle, float p2);
		int ADD_VEHICLE_COMBAT_ANGLED_AVOIDANCE_AREA(float p0, float p1, float p2, float p3, float p4, float p5, float p6);
		void REMOVE_VEHICLE_COMBAT_AVOIDANCE_AREA(int p0);
		BOOL IS_ANY_PED_RAPPELLING_FROM_HELI(Vehicle vehicle);
		/**
		 * <1.0 - Decreased torque
		 * =1.0 - Default torque
		 * >1.0 - Increased torque
		 * 
		 * Negative values will cause the vehicle to go backwards instead of forwards while accelerating.
		 * 
		 * value - is between 0.2 and 1.8 in the decompiled scripts.
		 * 
		 * This needs to be called every frame to take effect.
		 */
		void SET_VEHICLE_CHEAT_POWER_INCREASE(Vehicle vehicle, float value);
		void SET_VEHICLE_INFLUENCES_WANTED_LEVEL(Any p0, BOOL p1);
		/**
		 * Sets the wanted state of this vehicle.
		 */
		void SET_VEHICLE_IS_WANTED(Vehicle vehicle, BOOL state);
		/**
		 * Sets the boat boom position for the `TR3` trailer.
		 * Ratio value is between `0.0` and `1.0`, where `0.0` is 90 degrees to the left of the boat, and `1.0` is just slightly to the right/back of the boat.
		 * To get the current boom position ratio, use GET_BOAT_BOOM_POSITION_RATIO
		 */
		void SWING_BOAT_BOOM_TO_RATIO(Vehicle vehicle, float ratio);
		/**
		 * Same call as ALLOW_BOAT_BOOM_TO_ANIMATE
		 */
		void SWING_BOAT_BOOM_FREELY(Vehicle vehicle, BOOL toggle);
		void ALLOW_BOAT_BOOM_TO_ANIMATE(Vehicle vehicle, BOOL toggle);
		float GET_BOAT_BOOM_POSITION_RATIO(Vehicle vehicle);
		void DISABLE_PLANE_AILERON(Vehicle vehicle, BOOL p1, BOOL p2);
		/**
		 * Returns true when in a vehicle, false whilst entering/exiting.
		 */
		BOOL GET_IS_VEHICLE_ENGINE_RUNNING(Vehicle vehicle);
		void SET_VEHICLE_USE_ALTERNATE_HANDLING(Vehicle vehicle, BOOL toggle);
		/**
		 * Only works on bikes, both X and Y work in the -1 - 1 range.
		 * 
		 * X forces the bike to turn left or right (-1, 1)
		 * Y forces the bike to lean to the left or to the right (-1, 1)
		 * 
		 * Example with X -1/Y 1
		 * http://i.imgur.com/TgIuAPJ.jpg
		 */
		void SET_BIKE_ON_STAND(Vehicle vehicle, float x, float y);
		void SET_VEHICLE_NOT_STEALABLE_AMBIENTLY(Vehicle vehicle, BOOL p1);
		void LOCK_DOORS_WHEN_NO_LONGER_NEEDED(Vehicle vehicle);
		void SET_LAST_DRIVEN_VEHICLE(Vehicle vehicle);
		Vehicle GET_LAST_DRIVEN_VEHICLE();
		void CLEAR_LAST_DRIVEN_VEHICLE();
		void SET_VEHICLE_HAS_BEEN_DRIVEN_FLAG(Vehicle vehicle, BOOL toggle);
		void SET_TASK_VEHICLE_GOTO_PLANE_MIN_HEIGHT_ABOVE_TERRAIN(Vehicle plane, int height);
		void SET_VEHICLE_LOD_MULTIPLIER(Vehicle vehicle, float multiplier);
		void SET_VEHICLE_CAN_SAVE_IN_GARAGE(Vehicle vehicle, BOOL toggle);
		/**
		 * Also includes some "turnOffBones" when vehicle mods are installed.
		 */
		int GET_VEHICLE_NUM_OF_BROKEN_OFF_PARTS(Vehicle vehicle);
		int GET_VEHICLE_NUM_OF_BROKEN_LOOSEN_PARTS(Vehicle vehicle);
		void SET_FORCE_VEHICLE_ENGINE_DAMAGE_BY_BULLET(Any p0, BOOL p1);
		/**
		 * Allows creation of CEventShockingPlaneFlyby, CEventShockingHelicopterOverhead, and other(?) Shocking events
		 */
		void SET_VEHICLE_GENERATES_ENGINE_SHOCKING_EVENTS(Vehicle vehicle, BOOL toggle);
		/**
		 * Copies sourceVehicle's damage (broken bumpers, broken lights, etc.) to targetVehicle.
		 */
		void COPY_VEHICLE_DAMAGES(Vehicle sourceVehicle, Vehicle targetVehicle);
		void DISABLE_VEHICLE_EXPLOSION_BREAK_OFF_PARTS();
		void SET_LIGHTS_CUTOFF_DISTANCE_TWEAK(float distance);
		/**
		 * Commands the driver of an armed vehicle (p0) to shoot its weapon at a target (p1). p3, p4 and p5 are the coordinates of the target. Example:
		 * 
		 * WEAPON::SET_CURRENT_PED_VEHICLE_WEAPON(pilot,MISC::GET_HASH_KEY("VEHICLE_WEAPON_PLANE_ROCKET"));                        VEHICLE::SET_VEHICLE_SHOOT_AT_TARGET(pilot, target, targPos.x, targPos.y, targPos.z);
		 */
		void SET_VEHICLE_SHOOT_AT_TARGET(Ped driver, Entity entity, float xTarget, float yTarget, float zTarget);
		BOOL GET_VEHICLE_LOCK_ON_TARGET(Vehicle vehicle, Entity* entity);
		void SET_FORCE_HD_VEHICLE(Vehicle vehicle, BOOL toggle);
		void SET_VEHICLE_CUSTOM_PATH_NODE_STREAMING_RADIUS(Vehicle vehicle, float p1);
		int GET_VEHICLE_PLATE_TYPE(Vehicle vehicle);
		/**
		 * in script hook .net
		 * 
		 * Vehicle v = ...;
		 * Function.Call(Hash.TRACK_VEHICLE_VISIBILITY, v.Handle);
		 */
		void TRACK_VEHICLE_VISIBILITY(Vehicle vehicle);
		/**
		 * must be called after TRACK_VEHICLE_VISIBILITY
		 * 
		 * it's not instant so probabilly must pass an 'update' to see correct result.
		 */
		BOOL IS_VEHICLE_VISIBLE(Vehicle vehicle);
		void SET_VEHICLE_GRAVITY(Vehicle vehicle, BOOL toggle);
		/**
		 * Enable/Disables global slipstream physics
		 */
		void SET_ENABLE_VEHICLE_SLIPSTREAMING(BOOL toggle);
		void SET_VEHICLE_SLIPSTREAMING_SHOULD_TIME_OUT(BOOL toggle);
		/**
		 * Returns a float value between 0.0 and 3.0 related to its slipstream draft (boost/speedup).
		 */
		float GET_VEHICLE_CURRENT_TIME_IN_SLIP_STREAM(Vehicle vehicle);
		/**
		 * Returns true if the vehicle is being slipstreamed by another vehicle
		 */
		BOOL IS_VEHICLE_PRODUCING_SLIP_STREAM(Vehicle vehicle);
		void SET_VEHICLE_INACTIVE_DURING_PLAYBACK(Vehicle vehicle, BOOL toggle);
		void SET_VEHICLE_ACTIVE_DURING_PLAYBACK(Vehicle vehicle, BOOL toggle);
		/**
		 * Returns false if the vehicle has the FLAG_NO_RESPRAY flag set.
		 */
		BOOL IS_VEHICLE_SPRAYABLE(Vehicle vehicle);
		void SET_VEHICLE_ENGINE_CAN_DEGRADE(Vehicle vehicle, BOOL toggle);
		/**
		 * Adds some kind of shadow to the vehicle.
		 * 
		 * p1 and p2 use values from 0-255 and both make the shadow darker the lower the value is. -1 disables the effect.
		 */
		void DISABLE_VEHCILE_DYNAMIC_AMBIENT_SCALES(Vehicle vehicle, int p1, int p2);
		/**
		 * Remove the weird shadow applied by DISABLE_VEHCILE_DYNAMIC_AMBIENT_SCALES.
		 */
		void ENABLE_VEHICLE_DYNAMIC_AMBIENT_SCALES(Vehicle vehicle);
		BOOL IS_PLANE_LANDING_GEAR_INTACT(Vehicle plane);
		BOOL ARE_PLANE_PROPELLERS_INTACT(Vehicle plane);
		BOOL SET_PLANE_PROPELLER_HEALTH(Vehicle plane, float health);
		void SET_VEHICLE_CAN_DEFORM_WHEELS(Vehicle vehicle, BOOL toggle);
		/**
		 * Only returns true if the vehicle was marked as stolen with SET_VEHICLE_IS_STOLEN.
		 */
		BOOL IS_VEHICLE_STOLEN(Vehicle vehicle);
		void SET_VEHICLE_IS_STOLEN(Vehicle vehicle, BOOL isStolen);
		/**
		 * This native sets the turbulence multiplier. It only works for planes.
		 * 0.0 = no turbulence at all.
		 * 1.0 = heavy turbulence.
		 * Works by just calling it once, does not need to be called every tick.
		 */
		void SET_PLANE_TURBULENCE_MULTIPLIER(Vehicle vehicle, float multiplier);
		BOOL ARE_WINGS_OF_PLANE_INTACT(Vehicle plane);
		/**
		 * This native doesn't seem to do anything, might be a debug-only native.
		 * 
		 * Confirmed, it is a debug native.
		 */
		void ALLOW_AMBIENT_VEHICLES_TO_AVOID_ADVERSE_CONDITIONS(Vehicle vehicle);
		void DETACH_VEHICLE_FROM_CARGOBOB(Vehicle vehicle, Vehicle cargobob);
		BOOL DETACH_VEHICLE_FROM_ANY_CARGOBOB(Vehicle vehicle);
		BOOL DETACH_ENTITY_FROM_CARGOBOB(Vehicle cargobob, Entity entity);
		BOOL IS_VEHICLE_ATTACHED_TO_CARGOBOB(Vehicle cargobob, Vehicle vehicleAttached);
		/**
		 * Returns attached vehicle (Vehicle in parameter must be cargobob)
		 */
		Vehicle GET_VEHICLE_ATTACHED_TO_CARGOBOB(Vehicle cargobob);
		Entity GET_ENTITY_ATTACHED_TO_CARGOBOB(Any p0);
		void ATTACH_VEHICLE_TO_CARGOBOB(Vehicle cargobob, Vehicle vehicle, int p2, float x, float y, float z);
		void ATTACH_ENTITY_TO_CARGOBOB(Any p0, Any p1, Any p2, Any p3, Any p4, Any p5);
		/**
		 * Stops cargobob from being able to detach the attached vehicle.
		 */
		void SET_CARGOBOB_FORCE_DONT_DETACH_VEHICLE(Vehicle cargobob, BOOL toggle);
		void SET_CARGOBOB_EXCLUDE_FROM_PICKUP_ENTITY(Any p0, Any p1);
		BOOL CAN_CARGOBOB_PICK_UP_ENTITY(Any p0, Any p1);
		/**
		 * Gets the position of the cargobob hook, in world coords.
		 */
		Vector3 GET_ATTACHED_PICK_UP_HOOK_POSITION(Vehicle cargobob);
		/**
		 * Returns true only when the hook is active, will return false if the magnet is active
		 */
		BOOL DOES_CARGOBOB_HAVE_PICK_UP_ROPE(Vehicle cargobob);
		/**
		 * Drops the Hook/Magnet on a cargobob
		 * 
		 * state
		 * enum eCargobobHook
		 * {
		 * CARGOBOB_HOOK = 0,
		 * CARGOBOB_MAGNET = 1,
		 * };
		 */
		void CREATE_PICK_UP_ROPE_FOR_CARGOBOB(Vehicle cargobob, int state);
		/**
		 * Retracts the hook on the cargobob.
		 * 
		 * Note: after you retract it the natives for dropping the hook no longer work
		 */
		void REMOVE_PICK_UP_ROPE_FOR_CARGOBOB(Vehicle cargobob);
		/**
		 * min: 1.9f, max: 100.0f
		 */
		void SET_PICKUP_ROPE_LENGTH_FOR_CARGOBOB(Vehicle cargobob, float length1, float length2, BOOL p3);
		void SET_PICKUP_ROPE_LENGTH_WITHOUT_CREATING_ROPE_FOR_CARGOBOB(Any p0, Any p1, Any p2);
		void SET_CARGOBOB_PICKUP_ROPE_DAMPING_MULTIPLIER(Any p0, Any p1);
		void SET_CARGOBOB_PICKUP_ROPE_TYPE(Any p0, Any p1);
		/**
		 * Returns true only when the magnet is active, will return false if the hook is active
		 */
		BOOL DOES_CARGOBOB_HAVE_PICKUP_MAGNET(Vehicle cargobob);
		/**
		 * Won't attract or magnetize to any helicopters or planes of course, but that's common sense.
		 */
		void SET_CARGOBOB_PICKUP_MAGNET_ACTIVE(Vehicle cargobob, BOOL isActive);
		void SET_CARGOBOB_PICKUP_MAGNET_STRENGTH(Vehicle cargobob, float strength);
		void SET_CARGOBOB_PICKUP_MAGNET_FALLOFF(Vehicle cargobob, float p1);
		void SET_CARGOBOB_PICKUP_MAGNET_REDUCED_STRENGTH(Vehicle cargobob, float p1);
		void SET_CARGOBOB_PICKUP_MAGNET_REDUCED_FALLOFF(Vehicle cargobob, float p1);
		void SET_CARGOBOB_PICKUP_MAGNET_PULL_STRENGTH(Vehicle cargobob, float p1);
		void SET_CARGOBOB_PICKUP_MAGNET_PULL_ROPE_LENGTH(Vehicle vehicle, float p1);
		void SET_CARGOBOB_PICKUP_MAGNET_SET_TARGETED_MODE(Vehicle vehicle, Vehicle cargobob);
		void SET_CARGOBOB_PICKUP_MAGNET_SET_AMBIENT_MODE(Vehicle vehicle, BOOL p1, BOOL p2);
		void SET_CARGOBOB_PICKUP_MAGNET_ENSURE_PICKUP_ENTITY_UPRIGHT(Vehicle vehicle, BOOL p1);
		BOOL DOES_VEHICLE_HAVE_WEAPONS(Vehicle vehicle);
		void SET_VEHICLE_WILL_TELL_OTHERS_TO_HURRY(Vehicle vehicle, BOOL p1);
		/**
		 * Full list of weapons by DurtyFree (Search for VEHICLE_*): https://github.com/DurtyFree/gta-v-data-dumps/blob/master/weapons.json
		 */
		void DISABLE_VEHICLE_WEAPON(BOOL disabled, Hash weaponHash, Vehicle vehicle, Ped owner);
		/**
		 * Full list of weapons by DurtyFree (Search for VEHICLE_*): https://github.com/DurtyFree/gta-v-data-dumps/blob/master/weapons.json
		 */
		BOOL IS_VEHICLE_WEAPON_DISABLED(Hash weaponHash, Vehicle vehicle, Ped owner);
		void SET_VEHICLE_USED_FOR_PILOT_SCHOOL(Vehicle vehicle, BOOL toggle);
		void SET_VEHICLE_ACTIVE_FOR_PED_NAVIGATION(Vehicle vehicle, BOOL toggle);
		/**
		 * Returns an int
		 * 
		 * Vehicle Classes:
		 * 0: Compacts
		 * 1: Sedans
		 * 2: SUVs
		 * 3: Coupes
		 * 4: Muscle
		 * 5: Sports Classics
		 * 6: Sports
		 * 7: Super
		 * 8: Motorcycles
		 * 9: Off-road
		 * 10: Industrial
		 * 11: Utility
		 * 12: Vans
		 * 13: Cycles
		 * 14: Boats
		 * 15: Helicopters
		 * 16: Planes
		 * 17: Service
		 * 18: Emergency
		 * 19: Military
		 * 20: Commercial
		 * 21: Trains
		 * 
		 * char buffer[128];
		 * std::sprintf(buffer, "VEH_CLASS_%i", VEHICLE::GET_VEHICLE_CLASS(vehicle));
		 * 
		 * const char* className = HUD::GET_FILENAME_FOR_AUDIO_CONVERSATION(buffer);
		 */
		int GET_VEHICLE_CLASS(Vehicle vehicle);
		/**
		 * char buffer[128];
		 * std::sprintf(buffer, "VEH_CLASS_%i", VEHICLE::GET_VEHICLE_CLASS_FROM_NAME (hash));
		 * 
		 * const char* className = HUD::GET_FILENAME_FOR_AUDIO_CONVERSATION(buffer);
		 * 
		 * Full list of vehicles by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/vehicles.json
		 */
		int GET_VEHICLE_CLASS_FROM_NAME(Hash modelHash);
		void SET_PLAYERS_LAST_VEHICLE(Vehicle vehicle);
		void SET_VEHICLE_CAN_BE_USED_BY_FLEEING_PEDS(Vehicle vehicle, BOOL toggle);
		void SET_AIRCRAFT_PILOT_SKILL_NOISE_SCALAR(Vehicle vehicle, float p1);
		/**
		 * Money pickups are created around cars when they explode. Only works when the vehicle model is a car. A single pickup is between 1 and 18 dollars in size. All car models seem to give the same amount of money.
		 * 
		 * youtu.be/3arlUxzHl5Y
		 * i.imgur.com/WrNpYFs.jpg
		 */
		void SET_VEHICLE_DROPS_MONEY_WHEN_BLOWN_UP(Vehicle vehicle, BOOL toggle);
		void SET_VEHICLE_KEEP_ENGINE_ON_WHEN_ABANDONED(Vehicle vehicle, BOOL toggle);
		/**
		 * Seems to copy some values in vehicle
		 */
		void SET_VEHICLE_IMPATIENCE_TIMER(Vehicle vehicle, Any p1);
		/**
		 * Use the "AIHandling" string found in handling.meta
		 */
		void SET_VEHICLE_HANDLING_OVERRIDE(Vehicle vehicle, Hash hash);
		/**
		 * Max value is 32767
		 */
		void SET_VEHICLE_EXTENDED_REMOVAL_RANGE(Vehicle vehicle, int range);
		void SET_VEHICLE_STEERING_BIAS_SCALAR(Any p0, float p1);
		/**
		 * value between 0.0 and 1.0
		 */
		void SET_HELI_CONTROL_LAGGING_RATE_SCALAR(Vehicle helicopter, float multiplier);
		/**
		 * Seems to be related to the metal parts, not tyres (like i was expecting lol)
		 */
		void SET_VEHICLE_FRICTION_OVERRIDE(Vehicle vehicle, float friction);
		void SET_VEHICLE_WHEELS_CAN_BREAK_OFF_WHEN_BLOW_UP(Vehicle vehicle, BOOL toggle);
		BOOL ARE_PLANE_CONTROL_PANELS_INTACT(Vehicle vehicle, BOOL p1);
		void SET_VEHICLE_CEILING_HEIGHT(Vehicle vehicle, float height);
		void SET_VEHICLE_NO_EXPLOSION_DAMAGE_FROM_DRIVER(Vehicle vehicle, BOOL toggle);
		void CLEAR_VEHICLE_ROUTE_HISTORY(Vehicle vehicle);
		Vehicle DOES_VEHICLE_EXIST_WITH_DECORATOR(const char* decorator);
		/**
		 * Used to be incorrectly named SET_VEHICLE_EXCLUSIVE_DRIVER
		 * Toggles a flag related to SET_VEHICLE_EXCLUSIVE_DRIVER, however, doesn't enable that feature (or trigger script events related to it).
		 */
		void SET_VEHICLE_AI_CAN_USE_EXCLUSIVE_SEATS(Vehicle vehicle, BOOL toggle);
		/**
		 * index: 0 - 1
		 * 
		 * Used to be incorrectly named _SET_VEHICLE_EXCLUSIVE_DRIVER_2
		 */
		void SET_VEHICLE_EXCLUSIVE_DRIVER(Vehicle vehicle, Ped ped, int index);
		BOOL IS_PED_EXCLUSIVE_DRIVER_OF_VEHICLE(Ped ped, Vehicle vehicle, int* outIndex);
		void DISABLE_INDIVIDUAL_PLANE_PROPELLER(Vehicle vehicle, int propeller);
		void _ENABLE_INDIVIDUAL_PLANE_PROPELLER(Vehicle vehicle, int propeller); // Missing in crossmap
		void SET_VEHICLE_FORCE_AFTERBURNER(Vehicle vehicle, BOOL toggle);
		/**
		 * R* used it to "remove" vehicle windows when "nightshark" had some mod, which adding some kind of armored windows. When enabled, you can't break vehicles glass. All your bullets wiil shoot through glass. You also will not able to break the glass with any other way (hitting and etc)
		 */
		void SET_DONT_PROCESS_VEHICLE_GLASS(Vehicle vehicle, BOOL toggle);
		void SET_DISABLE_WANTED_CONES_RESPONSE(Vehicle vehicle, BOOL toggle);
		void SET_USE_DESIRED_Z_CRUISE_SPEED_FOR_LANDING(Vehicle vehicle, BOOL toggle);
		void SET_ARRIVE_DISTANCE_OVERRIDE_FOR_VEHICLE_PERSUIT_ATTACK(Vehicle vehicle, float p1);
		void SET_VEHICLE_READY_FOR_CLEANUP(Any p0);
		/**
		 * Toggles to render distant vehicles. They may not be vehicles but images to look like vehicles.
		 */
		void SET_DISTANT_CARS_ENABLED(BOOL toggle);
		/**
		 * Sets the color of the neon lights of the specified vehicle.
		 */
		void SET_VEHICLE_NEON_COLOUR(Vehicle vehicle, int r, int g, int b);
		/**
		 * Index references CVehicleModelColor
		 */
		void SET_VEHICLE_NEON_INDEX_COLOUR(Vehicle vehicle, int index);
		/**
		 * Gets the color of the neon lights of the specified vehicle.
		 * 
		 * See SET_VEHICLE_NEON_COLOUR (0x8E0A582209A62695) for more information
		 */
		void GET_VEHICLE_NEON_COLOUR(Vehicle vehicle, int* r, int* g, int* b);
		/**
		 * Sets the neon lights of the specified vehicle on/off.
		 * 
		 * Indices:
		 * 0 = Left
		 * 1 = Right
		 * 2 = Front
		 * 3 = Back
		 */
		void SET_VEHICLE_NEON_ENABLED(Vehicle vehicle, int index, BOOL toggle);
		/**
		 * indices:
		 * 0 = Left
		 * 1 = Right
		 * 2 = Front
		 * 3 = Back
		 */
		BOOL GET_VEHICLE_NEON_ENABLED(Vehicle vehicle, int index);
		void SET_AMBIENT_VEHICLE_NEON_ENABLED(BOOL p0);
		void SUPPRESS_NEONS_ON_VEHICLE(Vehicle vehicle, BOOL toggle);
		void SET_DISABLE_SUPERDUMMY(Vehicle vehicle, BOOL p1);
		void REQUEST_VEHICLE_DIAL(Vehicle vehicle);
		/**
		 * Seems related to vehicle health, like the one in IV.
		 * Max 1000, min 0.
		 * Vehicle does not necessarily explode or become undrivable at 0.
		 */
		float GET_VEHICLE_BODY_HEALTH(Vehicle vehicle);
		/**
		 * p2 often set to 1000.0 in the decompiled scripts.
		 */
		void SET_VEHICLE_BODY_HEALTH(Vehicle vehicle, float value);
		/**
		 * Outputs 2 Vector3's.
		 * Scripts check if out2.x - out1.x > someshit.x
		 * Could be suspension related, as in max suspension height and min suspension height, considering the natives location.
		 */
		void GET_VEHICLE_SIZE(Vehicle vehicle, Vector3* out1, Vector3* out2);
		/**
		 * Gets the height of the vehicle's suspension.
		 * The higher the value the lower the suspension. Each 0.002 corresponds with one more level lowered.
		 * 0.000 is the stock suspension.
		 * 0.008 is Ultra Suspension.
		 */
		float GET_FAKE_SUSPENSION_LOWERING_AMOUNT(Vehicle vehicle);
		void SET_CAR_HIGH_SPEED_BUMP_SEVERITY_MULTIPLIER(float multiplier);
		int GET_NUMBER_OF_VEHICLE_DOORS(Vehicle vehicle);
		/**
		 * If false, lowers hydraulics (if raised) and disables hydraulics controls. If true, raises hydraulics and enables hydraulics controls.
		 * 
		 * Only used once in each carmod script, on a car that does not have hydraulics to begin with.
		 */
		void SET_HYDRAULICS_CONTROL(Vehicle vehicle, BOOL toggle);
		void SET_CAN_ADJUST_GROUND_CLEARANCE(Vehicle vehicle, BOOL p1);
		/**
		 * 0 min 100 max
		 * starts at 100
		 * Seams to have health zones
		 * Front of vehicle when damaged goes from 100-50 and stops at 50.
		 * Rear can be damaged from 100-0
		 * Only tested with two cars.
		 * 
		 * any idea how this differs from the first one?
		 * 
		 * --
		 * May return the vehicle health on a scale of 0.0 - 100.0 (needs to be confirmed)
		 * 
		 * example:
		 * 
		 * v_F = ENTITY::GET_ENTITY_MODEL(v_3);
		 * if (((v_F == ${tanker}) || (v_F == ${armytanker})) || (v_F == ${tanker2})) {
		 * if (VEHICLE::GET_VEHICLE_HEALTH_PERCENTAGE(v_3) <= 1.0) {
		 * NETWORK::NETWORK_EXPLODE_VEHICLE(v_3, 1, 1, -1);
		 * }
		 * }
		 */
		float GET_VEHICLE_HEALTH_PERCENTAGE(Vehicle vehicle, float maxEngineHealth, float maxPetrolTankHealth, float maxBodyHealth, float maxMainRotorHealth, float maxTailRotorHealth, float maxUnkHealth);
		BOOL GET_VEHICLE_IS_MERCENARY(Vehicle vehicle);
		void SET_VEHICLE_BROKEN_PARTS_DONT_AFFECT_AI_HANDLING(Vehicle vehicle, BOOL p1);
		void SET_VEHICLE_KERS_ALLOWED(Vehicle vehicle, BOOL toggle);
		/**
		 * Returns true if the vehicle has a HF_HAS_KERS (strHandlingFlags 0x4) handing flag set, for instance the lectro/vindicator bikes or the open wheelers.
		 */
		BOOL GET_VEHICLE_HAS_KERS(Vehicle vehicle);
		void SET_PLANE_RESIST_TO_EXPLOSION(Vehicle vehicle, BOOL toggle);
		void SET_HELI_RESIST_TO_EXPLOSION(Vehicle vehicle, BOOL toggle);
		void SET_DISABLE_BMX_EXTRA_TRICK_FORCES(Any p0);
		/**
		 * Works only on vehicles that support hydraulics.
		 */
		void SET_HYDRAULIC_SUSPENSION_RAISE_FACTOR(Vehicle vehicle, int wheelId, float value);
		float GET_HYDRAULIC_SUSPENSION_RAISE_FACTOR(Vehicle vehicle, int wheelId);
		void SET_CAN_USE_HYDRAULICS(Vehicle vehicle, BOOL toggle);
		/**
		 * States:
		 * 4 = raise
		 * 5 = lower
		 * 6 = jump
		 */
		void SET_HYDRAULIC_VEHICLE_STATE(Vehicle vehicle, int state);
		/**
		 * Sets vehicle wheel hydraulic states transition. Known states:
		 * 0 - reset
		 * 1 - raise wheel (uses value arg, works just like _SET_VEHICLE_HYDRAULIC_WHEEL_VALUE)
		 * 2 - jump using wheel
		 */
		void SET_HYDRAULIC_WHEEL_STATE(Vehicle vehicle, int wheelId, int state, float value, Any p4);
		BOOL HAS_VEHICLE_PETROLTANK_SET_ON_FIRE_BY_ENTITY(Any p0, Any p1);
		void CLEAR_VEHICLE_PETROLTANK_FIRE_CULPRIT(Vehicle vehicle);
		/**
		 * Controls how fast bobbleheads and tsurikawas move on each axis.
		 * 
		 * p2 is probably z, but changing that value didn't seem to have a noticeable effect.
		 */
		void SET_VEHICLE_BOBBLEHEAD_VELOCITY(float x, float y, float p2);
		BOOL GET_VEHICLE_IS_DUMMY(Any p0);
		BOOL SET_VEHICLE_DAMAGE_SCALE(Vehicle vehicle, float p1);
		/**
		 * Does not actually return anything.
		 */
		void _SET_VEHICLE_EXPLOSIVE_DAMAGE_SCALE(Vehicle vehicle, float scale); // Missing in crossmap
		BOOL SET_VEHICLE_WEAPON_DAMAGE_SCALE(Vehicle vehicle, float multiplier);
		BOOL SET_DISABLE_DAMAGE_WITH_PICKED_UP_ENTITY(Any p0, Any p1);
		void SET_VEHICLE_USES_MP_PLAYER_DAMAGE_MULTIPLIER(Any p0, Any p1);
		/**
		 * When enabled, the player won't fall off the bike when landing from large heights.
		 */
		void SET_BIKE_EASY_TO_LAND(Vehicle vehicle, BOOL toggle);
		/**
		 * Inverts vehicle's controls. So INPUT_VEH_ACCELERATE will be INPUT_VEH_BRAKE and vise versa (same for A/D controls)
		 * Doesn't work for planes/helis.
		 */
		void SET_INVERT_VEHICLE_CONTROLS(Vehicle vehicle, BOOL state);
		/**
		 * Disables the screen effects and sound effects when driving over a speed boost pad.
		 */
		void SET_SPEED_BOOST_EFFECT_DISABLED(BOOL disabled);
		/**
		 * Disables the screen effects and sound effects when driving over a slowdown pad.
		 */
		void SET_SLOW_DOWN_EFFECT_DISABLED(BOOL disabled);
		void SET_FORMATION_LEADER(Vehicle vehicle, float x, float y, float z, float p4);
		/**
		 * Resets the effect of SET_FORMATION_LEADER
		 */
		void RESET_FORMATION_LEADER();
		BOOL GET_IS_BOAT_CAPSIZED(Vehicle vehicle);
		void SET_ALLOW_RAMMING_SOOP_OR_RAMP(Any p0, Any p1);
		void SET_SCRIPT_RAMP_IMPULSE_SCALE(Vehicle vehicle, float impulseScale);
		/**
		 * doorId: see SET_VEHICLE_DOOR_SHUT
		 */
		BOOL GET_IS_DOOR_VALID(Vehicle vehicle, int doorId);
		void SET_SCRIPT_ROCKET_BOOST_RECHARGE_TIME(Vehicle vehicle, float seconds);
		BOOL GET_HAS_ROCKET_BOOST(Vehicle vehicle);
		BOOL IS_ROCKET_BOOST_ACTIVE(Vehicle vehicle);
		void SET_ROCKET_BOOST_ACTIVE(Vehicle vehicle, BOOL active);
		BOOL GET_HAS_RETRACTABLE_WHEELS(Vehicle vehicle);
		BOOL GET_IS_WHEELS_RETRACTED(Vehicle vehicle);
		void SET_WHEELS_EXTENDED_INSTANTLY(Vehicle vehicle);
		void SET_WHEELS_RETRACTED_INSTANTLY(Vehicle vehicle);
		/**
		 * Returns true if the vehicle has the FLAG_JUMPING_CAR flag set.
		 */
		BOOL GET_CAR_HAS_JUMP(Vehicle vehicle);
		/**
		 * Allows vehicles with the FLAG_JUMPING_CAR flag to jump higher (i.e. Ruiner 2000).
		 */
		void SET_USE_HIGHER_CAR_JUMP(Vehicle vehicle, BOOL toggle);
		void SET_CLEAR_FREEZE_WAITING_ON_COLLISION_ONCE_PLAYER_ENTERS(Vehicle vehicle, BOOL toggle);
		/**
		 * Set vehicle's primary mounted weapon 2 ammo. For example, use it on APC.
		 * For example, you can "remove" any vehicle weapon from any vehicle.
		 * ammoAmount -1 = infinite ammo (default value for any spawned vehicle tho)
		 */
		void SET_VEHICLE_WEAPON_RESTRICTED_AMMO(Vehicle vehicle, int weaponIndex, int capacity);
		int GET_VEHICLE_WEAPON_RESTRICTED_AMMO(Vehicle vehicle, int weaponIndex);
		BOOL GET_VEHICLE_HAS_PARACHUTE(Vehicle vehicle);
		BOOL GET_VEHICLE_CAN_DEPLOY_PARACHUTE(Vehicle vehicle);
		void VEHICLE_START_PARACHUTING(Vehicle vehicle, BOOL active);
		BOOL IS_VEHICLE_PARACHUTE_DEPLOYED(Vehicle vehicle);
		void VEHICLE_SET_RAMP_AND_RAMMING_CARS_TAKE_DAMAGE(Vehicle vehicle, BOOL toggle);
		void VEHICLE_SET_ENABLE_RAMP_CAR_SIDE_IMPULSE(Any p0, Any p1);
		void VEHICLE_SET_ENABLE_NORMALISE_RAMP_CAR_VERTICAL_VELOCTIY(Any p0, Any p1);
		void VEHICLE_SET_JET_WASH_FORCE_ENABLED(Any p0);
		void SET_VEHICLE_WEAPON_CAN_TARGET_OBJECTS(Vehicle vehicle, BOOL toggle);
		/**
		 * Used for blazer5. Changes the quadbike-jetski transformation input from raise/lower convertible roof (hold H by default) to horn (press E by default.)
		 */
		void SET_VEHICLE_USE_BOOST_BUTTON_FOR_WHEEL_RETRACT(BOOL toggle);
		void _SET_VEHICLE_USE_HORN_BUTTON_FOR_NITROUS(BOOL toggle);
		/**
		 * Parachute models:
		 * - sr_prop_specraces_para_s_01
		 * - imp_prop_impexp_para_s (SecuroServ; Default)
		 * Plus, many more props can be used as vehicle parachutes, like umbrellas (prop_beach_parasol_03), and unlike SET_PLAYER_PARACHUTE_MODEL_OVERRIDE, you won't get stuck mid-air when using an umbrella.
		 */
		void VEHICLE_SET_PARACHUTE_MODEL_OVERRIDE(Vehicle vehicle, Hash modelHash);
		/**
		 * Variations available for the generic parachute (sr_prop_specraces_para_s_01):
		 * - 0: Rainbow
		 * - 1: Red
		 * - 2: White, blue, yellow
		 * - 3: Black, red, white
		 * - 4: Red, white, blue
		 * - 5: Blue
		 * - 6: Black
		 * - 7: Black, yellow
		 */
		void VEHICLE_SET_PARACHUTE_MODEL_TINT_INDEX(Vehicle vehicle, int textureVariation);
		int VEHICLE_SET_OVERRIDE_EXTENABLE_SIDE_RATIO(Any p0, Any p1);
		int VEHICLE_SET_EXTENABLE_SIDE_TARGET_RATIO(Any p0, Any p1);
		int VEHICLE_SET_OVERRIDE_SIDE_RATIO(Any p0, Any p1);
		int GET_ALL_VEHICLES(Any* vehsStruct);
		void SET_CARGOBOB_EXTA_PICKUP_RANGE(Any p0, Any p1);
		void SET_OVERRIDE_VEHICLE_DOOR_TORQUE(Any p0, Any p1, Any p2);
		/**
		 * Enables/disables the ability to wheelie on motorcycles.
		 */
		void SET_WHEELIE_ENABLED(Vehicle vehicle, BOOL enabled);
		/**
		 * Prevents the vehicle from exploding when taking body damage. Only works for helis and planes.
		 */
		void SET_DISABLE_HELI_EXPLODE_FROM_BODY_DAMAGE(Vehicle vehicle, BOOL disable);
		/**
		 * Prevents the vehicle from exploding when taking body damage if it was caused by a collision. Only works for planes.
		 */
		void SET_DISABLE_EXPLODE_FROM_BODY_DAMAGE_ON_COLLISION(Vehicle vehicle, BOOL disable);
		/**
		 * Prevents the vehicle from exploding when taking body damage if the inflictor is an AI-controlled vehicle. Only works for planes.
		 */
		void _SET_DISABLE_EXPLODE_FROM_BODY_DAMAGE_RECEIVED_BY_AI_VEHICLE(Vehicle vehicle, BOOL disable); // Missing in crossmap
		void SET_TRAILER_ATTACHMENT_ENABLED(Any p0, Any p1);
		void SET_ROCKET_BOOST_FILL(Vehicle vehicle, float percentage);
		/**
		 * Set state to true to extend the wings, false to retract them.
		 */
		void SET_GLIDER_ACTIVE(Vehicle vehicle, BOOL state);
		/**
		 * Resets the vehicle's turret to its default position in scripted cameras. Doesn't seem to affect turrets that are occupied by a ped.
		 */
		void SET_SHOULD_RESET_TURRET_IN_SCRIPTED_CAMERAS(Vehicle vehicle, BOOL shouldReset);
		void SET_VEHICLE_DISABLE_COLLISION_UPON_CREATION(Vehicle vehicle, BOOL disable);
		void SET_GROUND_EFFECT_REDUCES_DRAG(BOOL toggle);
		/**
		 * Disables collision for this vehicle (maybe it also supports other entities, not sure).
		 * Only world/building/fixed world objects will have their collisions disabled, props, peds, or any other entity still collides with the vehicle.
		 * Example: https://streamable.com/6n45d5
		 * Not sure if there is a native (and if so, which one) that resets the collisions.
		 */
		void SET_DISABLE_MAP_COLLISION(Vehicle vehicle);
		void SET_DISABLE_PED_STAND_ON_TOP(Vehicle vehicle, BOOL toggle);
		void SET_VEHICLE_DAMAGE_SCALES(Vehicle vehicle, Any p1, Any p2, Any p3, Any p4);
		void SET_PLANE_SECTION_DAMAGE_SCALE(Vehicle vehicle, Any p1, Any p2);
		void SET_PLANE_CONTROL_SECTIONS_SHOULD_BREAK_OFF_FROM_EXPLOSIONS(Vehicle vehicle, BOOL toggle); // Missing in crossmap
		/**
		 * Stops the cargobob from being able to attach any vehicle
		 */
		void SET_HELI_CAN_PICKUP_ENTITY_THAT_HAS_PICK_UP_DISABLED(Vehicle vehicle, BOOL toggle);
		/**
		 * Sets the amount of bombs that this vehicle has. As far as I know, this does _not_ impact vehicle weapons or the ammo of those weapons in any way, it is just a way to keep track of the amount of bombs in a specific plane.
		 */
		void SET_VEHICLE_BOMB_AMMO(Vehicle vehicle, int bombCount);
		/**
		 * Gets the amount of bombs that this vehicle has. As far as I know, this does _not_ impact vehicle weapons or the ammo of those weapons in any way, it is just a way to keep track of the amount of bombs in a specific plane.
		 */
		int GET_VEHICLE_BOMB_AMMO(Vehicle vehicle);
		/**
		 * Similar to SET_VEHICLE_BOMB_AMMO, this sets the amount of countermeasures that are present on this vehicle.
		 * Use GET_VEHICLE_BOMB_AMMO to get the current amount.
		 */
		void SET_VEHICLE_COUNTERMEASURE_AMMO(Vehicle vehicle, int counterMeasureCount);
		/**
		 * Similar to `GET_VEHICLE_BOMB_AMMO`, this gets the amount of countermeasures that are present on this vehicle.
		 * Use SET_VEHICLE_COUNTERMEASURE_AMMO to set the current amount.
		 */
		int GET_VEHICLE_COUNTERMEASURE_AMMO(Vehicle vehicle);
		void SET_HELI_COMBAT_OFFSET(Vehicle vehicle, float x, float y, float z);
		/**
		 * Used in decompiled scripts in combination with GET_VEHICLE_SIZE
		 * p7 is usually 2
		 * p8 is usually 1
		 */
		BOOL GET_CAN_VEHICLE_BE_PLACED_HERE(Vehicle vehicle, float x, float y, float z, float rotX, float rotY, float rotZ, int p7, Any p8);
		/**
		 * Sets a flag on heli and another vehicle type.
		 */
		void SET_DISABLE_AUTOMATIC_CRASH_TASK(Vehicle vehicle, BOOL toggle);
		/**
		 * Used in conjunction with SET_SPECIAL_FLIGHT_MODE_TARGET_RATIO, in Rockstar's scripts. Using this will instantly transform the vehicle into hover mode starting from the given ratio (ranging from 0.0 to 1.0, values greater than 1.0 will put the vehicle into a glitched state.) If this is not used alongside SET_SPECIAL_FLIGHT_MODE_TARGET_RATIO, the vehicle will automatically transform back into car mode.
		 * 
		 * Usable only with the deluxo and other vehicles with deluxo-like hover mode toggle, modded or otherwise. Does nothing when used on oppressor2.
		 * 
		 * Example:
		 * Ped playerPed = PLAYER::PLAYER_PED_ID();
		 * Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(playerPed);
		 * VEHICLE::SET_SPECIAL_FLIGHT_MODE_RATIO(veh, 0.7f);
		 * VEHICLE::SET_SPECIAL_FLIGHT_MODE_TARGET_RATIO(veh, 1.0f);
		 */
		void SET_SPECIAL_FLIGHT_MODE_RATIO(Vehicle vehicle, float ratio);
		/**
		 * Used in conjunction with SET_SPECIAL_FLIGHT_MODE_RATIO, in Rockstar's scripts. The vehicle will transform into the given targetRatio, starting from the vehicle's current hover mode transform ratio (which can also be manually set by SET_SPECIAL_FLIGHT_MODE_RATIO,) i.e. setting targetRatio to 0.0 while the vehicle is in hover mode will transform the vehicle into car mode, likewise setting targetRatio to 1.0 while the vehicle is in car mode will transform the vehicle into hover mode, and if the current transform ratio is set to 0.7 while targetRatio is 1.0 the vehicle will transform into hover mode starting from being already partially transformed.
		 * 
		 * targetRatio is recommended to always be 0.0 or 1.0, otherwise the vehicle will transform into a glitched state.
		 * 
		 * Usable only with the deluxo and other vehicles with deluxo-like hover mode toggle, modded or otherwise. Does nothing when used on oppressor2.
		 * 
		 * Example:
		 * Ped playerPed = PLAYER::PLAYER_PED_ID();
		 * Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(playerPed);
		 * VEHICLE::SET_SPECIAL_FLIGHT_MODE_RATIO(veh, 0.7f);
		 * VEHICLE::SET_SPECIAL_FLIGHT_MODE_TARGET_RATIO(veh, 1.0f);
		 */
		void SET_SPECIAL_FLIGHT_MODE_TARGET_RATIO(Vehicle vehicle, float targetRatio);
		/**
		 * It will override the ability to transform deluxo. For oppressor it will work just like SET_DISABLE_HOVER_MODE_FLIGHT
		 */
		void SET_SPECIAL_FLIGHT_MODE_ALLOWED(Vehicle vehicle, BOOL toggle);
		/**
		 * Disables "wings" for some flying vehicles. Works only for oppressor _2_ and deluxo.
		 * For deluxo it just immediately removes vehicle's "wings" and you will be not able to fly up.
		 * For oppressor 2 it will remove wings right after you land. And you will not able to fly up anymore too.
		 * But for opressor 2 you still can fly if you somehow get back in the air.
		 */
		void SET_DISABLE_HOVER_MODE_FLIGHT(Vehicle vehicle, BOOL toggle);
		/**
		 * Checks if Chernobog's stabilizers are deployed or not.
		 * These are the metal supports that allow it to fire.
		 * This native only applies to the Chernobog.
		 */
		BOOL GET_OUTRIGGERS_DEPLOYED(Vehicle vehicle);
		/**
		 * Native is significantly more complicated than simply generating a random vector & length.
		 * The 'point' is either 400.0 or 250.0 units away from the Ped's current coordinates; and paths into functions like rage::grcViewport___IsSphereVisible
		 */
		Vector3 FIND_SPAWN_COORDINATES_FOR_HELI(Ped ped);
		/**
		 * Only used with the "akula" and "annihilator2" in the decompiled native scripts.
		 */
		void SET_DEPLOY_FOLDING_WINGS(Vehicle vehicle, BOOL deploy, BOOL p2);
		/**
		 * Only used with the "akula" and "annihilator2" in the decompiled native scripts.
		 */
		BOOL ARE_FOLDING_WINGS_DEPLOYED(Vehicle vehicle);
		void _SET_DEPLOY_MISSILE_BAYS(Vehicle vehicle, BOOL deploy);
		BOOL _ARE_MISSILE_BAYS_DEPLOYED(Vehicle vehicle);
		void SET_DIP_STRAIGHT_DOWN_WHEN_CRASHING_PLANE(Vehicle vehicle, BOOL toggle);
		/**
		 * Toggles specific flag on turret
		 */
		void SET_TURRET_HIDDEN(Vehicle vehicle, int index, BOOL toggle);
		void SET_HOVER_MODE_WING_RATIO(Vehicle vehicle, float ratio);
		/**
		 * Disables turret movement when called in a loop. You can still fire and aim. You cannot shoot backwards though.
		 */
		void SET_DISABLE_TURRET_MOVEMENT(Vehicle vehicle, int turretId);
		void SET_FORCE_FIX_LINK_MATRICES(Vehicle vehicle);
		/**
		 * Affects the playback speed of the submarine car conversion animations. Does not affect hardcoded animations such as the wheels being retracted.
		 */
		void SET_TRANSFORM_RATE_FOR_ANIMATION(Vehicle vehicle, float transformRate);
		/**
		 * When set to true, the key to transform a car to submarine mode changes from raise/lower convertible roof (hold H by default) to special vehicle transform (press X by default.)
		 */
		void SET_TRANSFORM_TO_SUBMARINE_USES_ALTERNATE_INPUT(Vehicle vehicle, BOOL toggle);
		/**
		 * Does nothing. It's a nullsub.
		 */
		void SET_VEHICLE_COMBAT_MODE(BOOL toggle);
		/**
		 * Does nothing. It's a nullsub.
		 */
		void SET_VEHICLE_DETONATION_MODE(BOOL toggle);
		/**
		 * Does nothing. It's a nullsub.
		 */
		void SET_VEHICLE_SHUNT_ON_STICK(BOOL toggle);
		void _IS_VEHICLE_ON_BOOST_PAD(Vehicle vehicle); // Missing in crossmap
		BOOL GET_IS_VEHICLE_SHUNTING(Vehicle vehicle);
		BOOL GET_HAS_VEHICLE_BEEN_HIT_BY_SHUNT(Vehicle vehicle);
		/**
		 * Returns last vehicle that was rammed by the given vehicle using the shunt boost.
		 */
		Vehicle GET_LAST_SHUNT_VEHICLE(Vehicle vehicle);
		void SET_DISABLE_VEHICLE_EXPLOSIONS_DAMAGE(BOOL toggle);
		void SET_OVERRIDE_NITROUS_LEVEL(Vehicle vehicle, BOOL toggle, float level, float power, float rechargeTime, BOOL disableSound);
		void _SET_NITROUS_IS_VISIBLE(Vehicle vehicle, BOOL enabled);
		void _SET_OVERRIDE_TRACTION_LOSS_MULTIPLIER(Vehicle vehicle, float modifier);
		/**
		 * First two floats relate to rumble, the last is a threshold
		 */
		void _SET_DRIFT_SLIP_ANGLE_LIMITS(Vehicle vehicle, float durationScalar, float amplitudeScalar, float slipAngleLimit);
		void _SET_MINIMUM_TIME_BETWEEN_GEAR_SHIFTS(Vehicle vehicle, int time);
		void FULLY_CHARGE_NITROUS(Vehicle vehicle);
		void _SET_REMAINING_NITROUS_DURATION(Vehicle vehicle, float duration); // Missing in crossmap
		float _GET_REMAINING_NITROUS_DURATION(Vehicle vehicle);
		BOOL IS_NITROUS_ACTIVE(Vehicle vehicle);
		void CLEAR_NITROUS(Vehicle vehicle);
		void SET_NITROUS_IS_ACTIVE(Vehicle vehicle, BOOL toggle); // Missing in crossmap
		void SET_INCREASE_WHEEL_CRUSH_DAMAGE(Vehicle vehicle, BOOL toggle);
		/**
		 * Sets some global vehicle related bool
		 */
		void SET_DISABLE_WEAPON_BLADE_FORCES(BOOL toggle);
		/**
		 * Changes the car jump control to require a double-tap to activate.
		 */
		void SET_USE_DOUBLE_CLICK_FOR_CAR_JUMP(BOOL toggle);
		/**
		 * Returns true only if the "tombstone" bone is attached to the vehicle, irrespective of "FLAG_HAS_TOMBSTONE" being present or not. Detaching the tombstone will return false.
		 */
		BOOL GET_DOES_VEHICLE_HAVE_TOMBSTONE(Vehicle vehicle);
		/**
		 * Disables detachable bumber from domnator4, dominator5, dominator6, see https://gfycat.com/SecondUnluckyGosling
		 */
		void HIDE_TOMBSTONE(Vehicle vehicle, BOOL toggle);
		void APPLY_EMP_EFFECT(Vehicle vehicle);
		/**
		 * Returns whether this vehicle is currently disabled by an EMP mine.
		 */
		BOOL GET_IS_VEHICLE_DISABLED_BY_EMP(Vehicle vehicle);
		void SET_DISABLE_RETRACTING_WEAPON_BLADES(BOOL toggle);
		/**
		 * Usable wheels:
		 * 0: wheel_lf
		 * 1: wheel_rf
		 * 2: wheel_lm1
		 * 3: wheel_rm1
		 * 4: wheel_lr
		 * 5: wheel_rr
		 */
		float GET_TYRE_HEALTH(Vehicle vehicle, int wheelIndex);
		/**
		 * SET_TYRE_WEAR_RATE must be active, otherwise values set to <1000.0f will default to 350.0f
		 * 
		 * Usable wheels:
		 * 0: wheel_lf
		 * 1: wheel_rf
		 * 2: wheel_lm1
		 * 3: wheel_rm1
		 * 4: wheel_lr
		 * 5: wheel_rr
		 */
		void SET_TYRE_HEALTH(Vehicle vehicle, int wheelIndex, float health);
		/**
		 * Returns the multiplier value from SET_TYRE_WEAR_RATE
		 * 
		 * Usable wheels:
		 * 0: wheel_lf
		 * 1: wheel_rf
		 * 2: wheel_lm1
		 * 3: wheel_rm1
		 * 4: wheel_lr
		 * 5: wheel_rr
		 */
		float GET_TYRE_WEAR_RATE(Vehicle vehicle, int wheelIndex);
		/**
		 * Needs to be run for tire wear to work. Multiplier affects the downforce and how fast the tires will wear out, higher values essentially make the vehicle slower on straights and its tires will wear down quicker when cornering. Value must be >0f.
		 * Default value in Rockstar's Open Wheel Race JSON's ("owrtws", "owrtwm", "owrtwh") is 1.0
		 * 
		 * Usable wheels:
		 * 0: wheel_lf
		 * 1: wheel_rf
		 * 2: wheel_lm1
		 * 3: wheel_rm1
		 * 4: wheel_lr
		 * 5: wheel_rr
		 */
		void SET_TYRE_WEAR_RATE(Vehicle vehicle, int wheelIndex, float multiplier);
		/**
		 * Controls how fast the tires wear out.
		 * 
		 * Default values from Rockstar's Open Wheel Race JSON's:
		 * "owrtss" (Soft): 2.2
		 * "owrtsm" (Medium): 1.7
		 * "owrtsh" (Hard): 1.2
		 * 
		 * Usable wheels:
		 * 0: wheel_lf
		 * 1: wheel_rf
		 * 2: wheel_lm1
		 * 3: wheel_rm1
		 * 4: wheel_lr
		 * 5: wheel_rr
		 */
		void SET_TYRE_WEAR_RATE_SCALE(Vehicle vehicle, int wheelIndex, float multiplier);
		/**
		 * Controls how much traction the wheel loses.
		 * 
		 * Default values from Rockstar's Open Wheel Race JSON's:
		 * "owrtds" (Soft): 0.05
		 * "owrtdm" (Medium): 0.45
		 * "owrtdh" (Hard): 0.8
		 * 
		 * Usable wheels:
		 * 0: wheel_lf
		 * 1: wheel_rf
		 * 2: wheel_lm1
		 * 3: wheel_rm1
		 * 4: wheel_lr
		 * 5: wheel_rr
		 */
		void SET_TYRE_MAXIMUM_GRIP_DIFFERENCE_DUE_TO_WEAR_RATE(Vehicle vehicle, int wheelIndex, float multiplier);
		void SET_AIRCRAFT_IGNORE_HIGHTMAP_OPTIMISATION(Vehicle vehicle, int p1);
		/**
		 * Lowers the vehicle's stance. Only works for vehicles that have strAdvancedFlags 0x8000 and 0x4000000 set.
		 */
		void SET_REDUCED_SUSPENSION_FORCE(Vehicle vehicle, BOOL enable);
		void SET_DRIFT_TYRES(Vehicle vehicle, BOOL toggle);
		BOOL GET_DRIFT_TYRES_SET(Vehicle vehicle);
		/**
		 * Implemented only for trains.
		 */
		void NETWORK_USE_HIGH_PRECISION_TRAIN_BLENDING(Vehicle vehicle, BOOL toggle);
		/**
		 * Only used in R* Script fm_content_cargo
		 */
		void SET_CHECK_FOR_ENOUGH_ROOM_FOR_PED(Vehicle vehicle, BOOL p1);
		/**
		 * _SET_ALLOW_R* - _SET_ALLOW_V*
		 */
		void _SET_ALLOW_COLLISION_WHEN_IN_VEHICLE(Vehicle vehicle, BOOL toggle);
		int _GET_VEHICLE_MAX_EXHAUST_BONE_COUNT();
		BOOL _GET_VEHICLE_EXHAUST_BONE(Vehicle vehicle, int index, int* boneIndex, BOOL* axisX);

	} // namespace VEHICLE

	namespace WATER {

		/**
		 * This function set height to the value of z-axis of the water surface.
		 * 
		 * This function works with sea and lake. However it does not work with shallow rivers (e.g. raton canyon will return -100000.0f)
		 * 
		 * note: seems to return true when you are in water
		 */
		BOOL GET_WATER_HEIGHT(float x, float y, float z, float* height);
		BOOL GET_WATER_HEIGHT_NO_WAVES(float x, float y, float z, float* height);
		BOOL TEST_PROBE_AGAINST_WATER(float x1, float y1, float z1, float x2, float y2, float z2, Vector3* result);
		/**
		 * enum eScriptWaterTestResult
		 * {
		 * SCRIPT_WATER_TEST_RESULT_NONE,
		 * SCRIPT_WATER_TEST_RESULT_WATER,
		 * SCRIPT_WATER_TEST_RESULT_BLOCKED,
		 * };
		 */
		int TEST_PROBE_AGAINST_ALL_WATER(float x1, float y1, float z1, float x2, float y2, float z2, int flags, float* waterHeight);
		/**
		 * See TEST_PROBE_AGAINST_ALL_WATER.
		 */
		int TEST_VERTICAL_PROBE_AGAINST_ALL_WATER(float x, float y, float z, int flags, float* waterHeight);
		/**
		 * Sets the water height for a given position and radius.
		 * 
		 */
		void MODIFY_WATER(float x, float y, float radius, float height);
		int ADD_EXTRA_CALMING_QUAD(float xLow, float yLow, float xHigh, float yHigh, float height);
		/**
		 * p0 is the handle returned from ADD_EXTRA_CALMING_QUAD
		 */
		void REMOVE_EXTRA_CALMING_QUAD(int calmingQuad);
		/**
		 * Sets a value that determines how aggressive the ocean waves will be. Values of 2.0 or more make for very aggressive waves like you see during a thunderstorm.
		 * 
		 * Works only ~200 meters around the player.
		 */
		void SET_DEEP_OCEAN_SCALER(float intensity);
		/**
		 * Gets the aggressiveness factor of the ocean waves.
		 */
		float GET_DEEP_OCEAN_SCALER();
		void SET_CALMED_WAVE_HEIGHT_SCALER(float height);
		/**
		 * Sets the waves intensity back to original (1.0 in most cases).
		 */
		void RESET_DEEP_OCEAN_SCALER();

	} // namespace WATER

	namespace WEAPON {

		/**
		 * Enables laser sight on any weapon.
		 * 
		 * It doesn't work. Neither on tick nor OnKeyDown
		 */
		void ENABLE_LASER_SIGHT_RENDERING(BOOL toggle);
		Hash GET_WEAPON_COMPONENT_TYPE_MODEL(Hash componentHash);
		/**
		 * Returns the model of any weapon.
		 * 
		 * Can also take an ammo hash?
		 * sub_6663a(&l_115B, WEAPON::GET_WEAPONTYPE_MODEL(${ammo_rpg}));
		 */
		Hash GET_WEAPONTYPE_MODEL(Hash weaponHash);
		Hash GET_WEAPONTYPE_SLOT(Hash weaponHash);
		Hash GET_WEAPONTYPE_GROUP(Hash weaponHash);
		/**
		 * Returns the amount of extra components the specified component has.
		 * Returns -1 if the component isn't of type CWeaponComponentVariantModel.
		 */
		int GET_WEAPON_COMPONENT_VARIANT_EXTRA_COUNT(Hash componentHash);
		/**
		 * Returns the model hash of the extra component at specified index.
		 */
		Hash GET_WEAPON_COMPONENT_VARIANT_EXTRA_MODEL(Hash componentHash, int extraComponentIndex);
		/**
		 * Full list of weapons by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/weapons.json
		 */
		void SET_CURRENT_PED_WEAPON(Ped ped, Hash weaponHash, BOOL bForceInHand);
		/**
		 * The return value seems to indicate returns true if the hash of the weapon object weapon equals the weapon hash.
		 * p2 seems to be 1 most of the time.
		 * 
		 * 
		 * 
		 * 
		 * 
		 * p2 is not implemented
		 * 
		 * disassembly said that?
		 * 
		 */
		BOOL GET_CURRENT_PED_WEAPON(Ped ped, Hash* weaponHash, BOOL p2);
		Entity GET_CURRENT_PED_WEAPON_ENTITY_INDEX(Ped ped, Any p1);
		/**
		 * p1 is always 0 in the scripts.
		 */
		Hash GET_BEST_PED_WEAPON(Ped ped, BOOL p1);
		/**
		 * Full list of weapons by DurtyFree (Search for VEHICLE_*): https://github.com/DurtyFree/gta-v-data-dumps/blob/master/weapons.json
		 */
		BOOL SET_CURRENT_PED_VEHICLE_WEAPON(Ped ped, Hash weaponHash);
		/**
		 * Example in VB
		 * 
		 * Public Shared Function GetVehicleCurrentWeapon(Ped As Ped) As Integer
		 * Dim arg As New OutputArgument()
		 * Native.Function.Call(Hash.GET_CURRENT_PED_VEHICLE_WEAPON, Ped, arg)
		 * Return arg.GetResult(Of Integer)()
		 * End Function
		 * 
		 * Usage:
		 * If GetVehicleCurrentWeapon(Game.Player.Character) = -821520672 Then ...Do something
		 * Note: -821520672 = VEHICLE_WEAPON_PLANE_ROCKET
		 */
		BOOL GET_CURRENT_PED_VEHICLE_WEAPON(Ped ped, Hash* weaponHash);
		void SET_PED_CYCLE_VEHICLE_WEAPONS_ONLY(Ped ped);
		/**
		 * Checks if the ped is currently equipped with a weapon matching a bit specified using a bitwise-or in typeFlags.
		 * 
		 * Type flag bit values:
		 * 1 = Melee weapons
		 * 2 = Explosive weapons
		 * 4 = Any other weapons
		 * 
		 * Not specifying any bit will lead to the native *always* returning 'false', and for example specifying '4 | 2' will check for any weapon except fists and melee weapons.
		 * 7 returns true if you are equipped with any weapon except your fists.
		 * 6 returns true if you are equipped with any weapon except melee weapons.
		 * 5 returns true if you are equipped with any weapon except the Explosives weapon group.
		 * 4 returns true if you are equipped with any weapon except Explosives weapon group AND melee weapons.
		 * 3 returns true if you are equipped with either Explosives or Melee weapons (the exact opposite of 4).
		 * 2 returns true only if you are equipped with any weapon from the Explosives weapon group.
		 * 1 returns true only if you are equipped with any Melee weapon.
		 * 0 never returns true.
		 * 
		 * Note: When I say "Explosives weapon group", it does not include the Jerry can and Fire Extinguisher.
		 */
		BOOL IS_PED_ARMED(Ped ped, int typeFlags);
		/**
		 * Full list of weapons by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/weapons.json
		 */
		BOOL IS_WEAPON_VALID(Hash weaponHash);
		/**
		 * p2 should be FALSE, otherwise it seems to always return FALSE
		 * 
		 * Bool does not check if the weapon is current equipped, unfortunately.
		 * Full list of weapons by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/weapons.json
		 */
		BOOL HAS_PED_GOT_WEAPON(Ped ped, Hash weaponHash, BOOL p2);
		BOOL IS_PED_WEAPON_READY_TO_SHOOT(Ped ped);
		Hash GET_PED_WEAPONTYPE_IN_SLOT(Ped ped, Hash weaponSlot);
		/**
		 * WEAPON::GET_AMMO_IN_PED_WEAPON(PLAYER::PLAYER_PED_ID(), a_0)
		 * 
		 * From decompiled scripts
		 * Returns total ammo in weapon
		 * 
		 * GTALua Example :
		 * natives.WEAPON.GET_AMMO_IN_PED_WEAPON(plyPed, WeaponHash)
		 * Full list of weapons by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/weapons.json
		 */
		int GET_AMMO_IN_PED_WEAPON(Ped ped, Hash weaponhash);
		/**
		 * Full list of weapons by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/weapons.json
		 */
		void ADD_AMMO_TO_PED(Ped ped, Hash weaponHash, int ammo);
		/**
		 * Full list of weapons by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/weapons.json
		 */
		void SET_PED_AMMO(Ped ped, Hash weaponHash, int ammo, BOOL p3);
		/**
		 * Full list of weapons by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/weapons.json
		 */
		void SET_PED_INFINITE_AMMO(Ped ped, BOOL toggle, Hash weaponHash);
		void SET_PED_INFINITE_AMMO_CLIP(Ped ped, BOOL toggle);
		void SET_PED_STUN_GUN_FINITE_AMMO(Any p0, Any p1);
		/**
		 * Full list of weapons by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/weapons.json
		 */
		void GIVE_WEAPON_TO_PED(Ped ped, Hash weaponHash, int ammoCount, BOOL isHidden, BOOL bForceInHand);
		/**
		 * Gives a weapon to PED with a delay, example:
		 * 
		 * WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PED::PLAYER_PED_ID(), MISC::GET_HASH_KEY("WEAPON_PISTOL"), 1000, false)
		 * Full list of weapons by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/weapons.json
		 */
		void GIVE_DELAYED_WEAPON_TO_PED(Ped ped, Hash weaponHash, int ammoCount, BOOL bForceInHand);
		/**
		 * setting the last params to false it does that same so I would suggest its not a toggle
		 */
		void REMOVE_ALL_PED_WEAPONS(Ped ped, BOOL p1);
		/**
		 * This native removes a specified weapon from your selected ped.
		 * 
		 * Example:
		 * C#:
		 * Function.Call(Hash.REMOVE_WEAPON_FROM_PED, Game.Player.Character, 0x99B507EA);
		 * 
		 * C++:
		 * WEAPON::REMOVE_WEAPON_FROM_PED(PLAYER::PLAYER_PED_ID(), 0x99B507EA);
		 * 
		 * The code above removes the knife from the player.
		 * Full list of weapons by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/weapons.json
		 */
		void REMOVE_WEAPON_FROM_PED(Ped ped, Hash weaponHash);
		/**
		 * Hides the players weapon during a cutscene.
		 */
		void HIDE_PED_WEAPON_FOR_SCRIPTED_CUTSCENE(Ped ped, BOOL toggle);
		/**
		 * Has 5 parameters since latest patches.
		 */
		void SET_PED_CURRENT_WEAPON_VISIBLE(Ped ped, BOOL visible, BOOL deselectWeapon, BOOL p3, BOOL p4);
		void SET_PED_DROPS_WEAPONS_WHEN_DEAD(Ped ped, BOOL toggle);
		/**
		 * It determines what weapons caused damage:
		 * 
		 * If you want to define only a specific weapon, second parameter=weapon hash code, third parameter=0
		 * If you want to define any melee weapon, second parameter=0, third parameter=1.
		 * If you want to identify any weapon (firearms, melee, rockets, etc.), second parameter=0, third parameter=2.
		 * Full list of weapons by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/weapons.json
		 */
		BOOL HAS_PED_BEEN_DAMAGED_BY_WEAPON(Ped ped, Hash weaponHash, int weaponType);
		/**
		 * Does NOT seem to work with HAS_PED_BEEN_DAMAGED_BY_WEAPON. Use CLEAR_ENTITY_LAST_WEAPON_DAMAGE and HAS_ENTITY_BEEN_DAMAGED_BY_WEAPON instead.
		 */
		void CLEAR_PED_LAST_WEAPON_DAMAGE(Ped ped);
		/**
		 * It determines what weapons caused damage:
		 * 
		 * If you want to define only a specific weapon, second parameter=weapon hash code, third parameter=0
		 * If you want to define any melee weapon, second parameter=0, third parameter=1.
		 * If you want to identify any weapon (firearms, melee, rockets, etc.), second parameter=0, third parameter=2.
		 * Full list of weapons by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/weapons.json
		 */
		BOOL HAS_ENTITY_BEEN_DAMAGED_BY_WEAPON(Entity entity, Hash weaponHash, int weaponType);
		void CLEAR_ENTITY_LAST_WEAPON_DAMAGE(Entity entity);
		void SET_PED_DROPS_WEAPON(Ped ped);
		/**
		 * Full list of weapons by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/weapons.json
		 */
		void SET_PED_DROPS_INVENTORY_WEAPON(Ped ped, Hash weaponHash, float xOffset, float yOffset, float zOffset, int ammoCount);
		/**
		 * p2 is mostly 1 in the scripts.
		 * Full list of weapons by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/weapons.json
		 */
		int GET_MAX_AMMO_IN_CLIP(Ped ped, Hash weaponHash, BOOL p2);
		void _GET_TIME_BEFORE_VEHICLE_WEAPON_RELOAD_FINISHES(Vehicle vehicle, int seat); // Missing in crossmap
		void _HAS_WEAPON_RELOADING_IN_VEHICLE(Vehicle vehicle, int seat); // Missing in crossmap
		void _GET_VEHICLE_WEAPON_RELOAD_TIME(Vehicle vehicle, int seat); // Missing in crossmap
		void _GET_AMMO_IN_VEHICLE_WEAPON_CLIP(Vehicle vehicle, int seat, int* ammo); // Missing in crossmap
		/**
		 * Full list of weapons by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/weapons.json
		 */
		BOOL GET_AMMO_IN_CLIP(Ped ped, Hash weaponHash, int* ammo);
		void _SET_AMMO_IN_VEHICLE_WEAPON_CLIP(Vehicle vehicle, int seat, int ammo); // Missing in crossmap
		/**
		 * Full list of weapons by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/weapons.json
		 */
		BOOL SET_AMMO_IN_CLIP(Ped ped, Hash weaponHash, int ammo);
		/**
		 * Full list of weapons by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/weapons.json
		 */
		BOOL GET_MAX_AMMO(Ped ped, Hash weaponHash, int* ammo);
		/**
		 * Returns the max ammo for an ammo type. Ammo types: https://gist.github.com/root-cause/faf41f59f7a6d818b7db0b839bd147c1
		 */
		BOOL GET_MAX_AMMO_BY_TYPE(Ped ped, Hash ammoTypeHash, int* ammo);
		/**
		 * Ammo types: https://gist.github.com/root-cause/faf41f59f7a6d818b7db0b839bd147c1
		 */
		void ADD_PED_AMMO_BY_TYPE(Ped ped, Hash ammoTypeHash, int ammo);
		/**
		 * Ammo types: https://gist.github.com/root-cause/faf41f59f7a6d818b7db0b839bd147c1
		 */
		void SET_PED_AMMO_BY_TYPE(Ped ped, Hash ammoTypeHash, int ammo);
		int GET_PED_AMMO_BY_TYPE(Ped ped, Hash ammoTypeHash);
		void SET_PED_AMMO_TO_DROP(Ped ped, int p1);
		void SET_PICKUP_AMMO_AMOUNT_SCALER(float p0);
		/**
		 * Returns the current ammo type of the specified ped's specified weapon.
		 * MkII magazines will change the return value, like Pistol MkII returning AMMO_PISTOL without any components and returning AMMO_PISTOL_TRACER after Tracer Rounds component is attached.
		 * Use 0xF489B44DD5AF4BD9 if you always want AMMO_PISTOL.
		 * Full list of weapons by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/weapons.json
		 */
		Hash GET_PED_AMMO_TYPE_FROM_WEAPON(Ped ped, Hash weaponHash);
		/**
		 * Returns the base/default ammo type of the specified ped's specified weapon.
		 * Use GET_PED_AMMO_TYPE_FROM_WEAPON if you want current ammo type (like AMMO_MG_INCENDIARY/AMMO_MG_TRACER while using MkII magazines) and use this if you want base ammo type. (AMMO_MG)
		 * Full list of weapons by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/weapons.json
		 */
		Hash GET_PED_ORIGINAL_AMMO_TYPE_FROM_WEAPON(Ped ped, Hash weaponHash);
		/**
		 * Pass ped. Pass address of Vector3.
		 * The coord will be put into the Vector3.
		 * The return will determine whether there was a coord found or not.
		 */
		BOOL GET_PED_LAST_WEAPON_IMPACT_COORD(Ped ped, Vector3* coords);
		/**
		 * p1/gadgetHash was always 0xFBAB5776 ("GADGET_PARACHUTE").
		 * p2 is always true.
		 */
		void SET_PED_GADGET(Ped ped, Hash gadgetHash, BOOL p2);
		/**
		 * gadgetHash - was always 0xFBAB5776 ("GADGET_PARACHUTE").
		 */
		BOOL GET_IS_PED_GADGET_EQUIPPED(Ped ped, Hash gadgetHash);
		/**
		 * Returns the hash of the weapon.
		 * 
		 * var num7 = WEAPON::GET_SELECTED_PED_WEAPON(num4);
		 * sub_27D3(num7);
		 * switch (num7)
		 * {
		 * case 0x24B17070:
		 * 
		 * Also see WEAPON::GET_CURRENT_PED_WEAPON. Difference?
		 * 
		 * -------------------------------------------------------------------------
		 * 
		 * The difference is that GET_SELECTED_PED_WEAPON simply returns the ped's current weapon hash but GET_CURRENT_PED_WEAPON also checks the weapon object and returns true if the hash of the weapon object equals the weapon hash
		 * Full list of weapons by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/weapons.json
		 */
		Hash GET_SELECTED_PED_WEAPON(Ped ped);
		/**
		 * WEAPON::EXPLODE_PROJECTILES(PLAYER::PLAYER_PED_ID(), func_221(0x00000003), 0x00000001);
		 */
		void EXPLODE_PROJECTILES(Ped ped, Hash weaponHash, BOOL p2);
		/**
		 * If `explode` true, then removal is done through exploding the projectile. Basically the same as EXPLODE_PROJECTILES but without defining the owner ped.
		 */
		void REMOVE_ALL_PROJECTILES_OF_TYPE(Hash weaponHash, BOOL explode);
		float GET_LOCKON_DISTANCE_OF_CURRENT_PED_WEAPON(Ped ped);
		float GET_MAX_RANGE_OF_CURRENT_PED_WEAPON(Ped ped);
		/**
		 * Fourth Parameter = unsure, almost always -1
		 */
		BOOL HAS_VEHICLE_GOT_PROJECTILE_ATTACHED(Ped driver, Vehicle vehicle, Hash weaponHash, Any p3);
		/**
		 * Full list of weapons & components by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/weapons.json
		 */
		void GIVE_WEAPON_COMPONENT_TO_PED(Ped ped, Hash weaponHash, Hash componentHash);
		/**
		 * Full list of weapons & components by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/weapons.json
		 */
		void REMOVE_WEAPON_COMPONENT_FROM_PED(Ped ped, Hash weaponHash, Hash componentHash);
		/**
		 * Full list of weapons & components by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/weapons.json
		 */
		BOOL HAS_PED_GOT_WEAPON_COMPONENT(Ped ped, Hash weaponHash, Hash componentHash);
		/**
		 * Full list of weapons & components by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/weapons.json
		 */
		BOOL IS_PED_WEAPON_COMPONENT_ACTIVE(Ped ped, Hash weaponHash, Hash componentHash);
		BOOL REFILL_AMMO_INSTANTLY(Ped ped);
		/**
		 * Forces a ped to reload only if they are able to; if they have a full magazine, they will not reload.
		 */
		BOOL MAKE_PED_RELOAD(Ped ped);
		void _TRIGGER_VEHICLE_WEAPON_RELOAD(Vehicle vehicle, int seat, Ped ped); // Missing in crossmap
		/**
		 * Nearly every instance of p1 I found was 31. Nearly every instance of p2 I found was 0.
		 * 
		 * REQUEST_WEAPON_ASSET(iLocal_1888, 31, 26);
		 */
		void REQUEST_WEAPON_ASSET(Hash weaponHash, int p1, int p2);
		BOOL HAS_WEAPON_ASSET_LOADED(Hash weaponHash);
		void REMOVE_WEAPON_ASSET(Hash weaponHash);
		/**
		 * Now has 8 params.
		 */
		Object CREATE_WEAPON_OBJECT(Hash weaponHash, int ammoCount, float x, float y, float z, BOOL showWorldModel, float scale, Any p7, Any p8, Any p9);
		/**
		 * componentHash:
		 * (use WEAPON::GET_WEAPON_COMPONENT_TYPE_MODEL() to get hash value)
		 * ${component_at_ar_flsh}, ${component_at_ar_supp}, ${component_at_pi_flsh}, ${component_at_scope_large}, ${component_at_ar_supp_02}
		 */
		void GIVE_WEAPON_COMPONENT_TO_WEAPON_OBJECT(Object weaponObject, Hash componentHash);
		/**
		 * see DOES_WEAPON_TAKE_WEAPON_COMPONENT for full list of weapons & components
		 */
		void REMOVE_WEAPON_COMPONENT_FROM_WEAPON_OBJECT(Object object, Hash componentHash);
		/**
		 * see DOES_WEAPON_TAKE_WEAPON_COMPONENT for full list of weapons & components
		 */
		BOOL HAS_WEAPON_GOT_WEAPON_COMPONENT(Object weapon, Hash componentHash);
		void GIVE_WEAPON_OBJECT_TO_PED(Object weaponObject, Ped ped);
		/**
		 * Full list of weapons & components by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/weapons.json
		 */
		BOOL DOES_WEAPON_TAKE_WEAPON_COMPONENT(Hash weaponHash, Hash componentHash);
		/**
		 * Drops the current weapon and returns the object
		 * 
		 * Unknown behavior when unarmed.
		 */
		Object GET_WEAPON_OBJECT_FROM_PED(Ped ped, BOOL p1);
		/**
		 * Gives the specified loadout to the specified ped.
		 * Loadouts are defined in common.rpf\data\ai\loadouts.meta
		 */
		void GIVE_LOADOUT_TO_PED(Ped ped, Hash loadoutHash);
		/**
		 * tintIndex can be the following:
		 * 
		 * 0 - Normal
		 * 1 - Green
		 * 2 - Gold
		 * 3 - Pink
		 * 4 - Army
		 * 5 - LSPD
		 * 6 - Orange
		 * 7 - Platinum
		 * Full list of weapons, components & tint indexes by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/weapons.json
		 */
		void SET_PED_WEAPON_TINT_INDEX(Ped ped, Hash weaponHash, int tintIndex);
		/**
		 * Full list of weapons, components & tint indexes by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/weapons.json
		 */
		int GET_PED_WEAPON_TINT_INDEX(Ped ped, Hash weaponHash);
		/**
		 * Full list of weapons, components & tint indexes by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/weapons.json
		 */
		void SET_WEAPON_OBJECT_TINT_INDEX(Object weapon, int tintIndex);
		int GET_WEAPON_OBJECT_TINT_INDEX(Object weapon);
		/**
		 * Full list of weapons by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/weapons.json
		 */
		int GET_WEAPON_TINT_COUNT(Hash weaponHash);
		/**
		 * Colors:
		 * 0 = Gray
		 * 1 = Dark Gray
		 * 2 = Black
		 * 3 = White
		 * 4 = Blue
		 * 5 = Cyan
		 * 6 = Aqua
		 * 7 = Cool Blue
		 * 8 = Dark Blue
		 * 9 = Royal Blue
		 * 10 = Plum
		 * 11 = Dark Purple
		 * 12 = Purple
		 * 13 = Red
		 * 14 = Wine Red
		 * 15 = Magenta
		 * 16 = Pink
		 * 17 = Salmon
		 * 18 = Hot Pink
		 * 19 = Rust Orange
		 * 20 = Brown
		 * 21 = Earth
		 * 22 = Orange
		 * 23 = Light Orange
		 * 24 = Dark Yellow
		 * 25 = Yellow
		 * 26 = Light Brown
		 * 27 = Lime Green
		 * 28 = Olive
		 * 29 = Moss
		 * 30 = Turquoise
		 * 31 = Dark Green
		 * Full list of weapons, components, tint indexes & weapon liveries by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/weapons.json
		 */
		void SET_PED_WEAPON_COMPONENT_TINT_INDEX(Ped ped, Hash weaponHash, Hash camoComponentHash, int colorIndex);
		/**
		 * Returns -1 if camoComponentHash is invalid/not attached to the weapon.
		 * Full list of weapons, components, tint indexes & weapon liveries by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/weapons.json
		 */
		int GET_PED_WEAPON_COMPONENT_TINT_INDEX(Ped ped, Hash weaponHash, Hash camoComponentHash);
		/**
		 * Colors:
		 * 0 = Gray
		 * 1 = Dark Gray
		 * 2 = Black
		 * 3 = White
		 * 4 = Blue
		 * 5 = Cyan
		 * 6 = Aqua
		 * 7 = Cool Blue
		 * 8 = Dark Blue
		 * 9 = Royal Blue
		 * 10 = Plum
		 * 11 = Dark Purple
		 * 12 = Purple
		 * 13 = Red
		 * 14 = Wine Red
		 * 15 = Magenta
		 * 16 = Pink
		 * 17 = Salmon
		 * 18 = Hot Pink
		 * 19 = Rust Orange
		 * 20 = Brown
		 * 21 = Earth
		 * 22 = Orange
		 * 23 = Light Orange
		 * 24 = Dark Yellow
		 * 25 = Yellow
		 * 26 = Light Brown
		 * 27 = Lime Green
		 * 28 = Olive
		 * 29 = Moss
		 * 30 = Turquoise
		 * 31 = Dark Green
		 * Full list of weapons, components, tint indexes & weapon liveries by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/weapons.json
		 */
		void SET_WEAPON_OBJECT_COMPONENT_TINT_INDEX(Object weaponObject, Hash camoComponentHash, int colorIndex);
		/**
		 * Returns -1 if camoComponentHash is invalid/not attached to the weapon object.
		 * Full list of weapons, components, tint indexes & weapon liveries by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/weapons.json
		 */
		int GET_WEAPON_OBJECT_COMPONENT_TINT_INDEX(Object weaponObject, Hash camoComponentHash);
		int GET_PED_WEAPON_CAMO_INDEX(Ped ped, Hash weaponHash);
		void SET_WEAPON_OBJECT_CAMO_INDEX(Object weaponObject, int p1);
		/**
		 * struct WeaponHudStatsData
		 * {
		 * BYTE hudDamage; // 0x0000
		 * char _0x0001[0x7]; // 0x0001
		 * BYTE hudSpeed; // 0x0008
		 * char _0x0009[0x7]; // 0x0009
		 * BYTE hudCapacity; // 0x0010
		 * char _0x0011[0x7]; // 0x0011
		 * BYTE hudAccuracy; // 0x0018
		 * char _0x0019[0x7]; // 0x0019
		 * BYTE hudRange; // 0x0020
		 * };
		 * 
		 * Usage:
		 * 
		 * WeaponHudStatsData data;
		 * if (GET_WEAPON_HUD_STATS(weaponHash, (int *)&data))
		 * {
		 * // BYTE damagePercentage = data.hudDamage and so on
		 * }
		 * Full list of weapons by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/weapons.json
		 */
		BOOL GET_WEAPON_HUD_STATS(Hash weaponHash, Any* outData);
		BOOL GET_WEAPON_COMPONENT_HUD_STATS(Hash componentHash, Any* outData);
		/**
		 * This native does not return damages of weapons from the melee and explosive group.
		 * Full list of weapons by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/weapons.json
		 */
		float GET_WEAPON_DAMAGE(Hash weaponHash, Hash componentHash);
		/**
		 * // Returns the size of the default weapon component clip.
		 * 
		 * Use it like this:
		 * 
		 * char cClipSize[32];
		 * Hash cur;
		 * if (WEAPON::GET_CURRENT_PED_WEAPON(playerPed, &cur, 1))
		 * {
		 * if (WEAPON::IS_WEAPON_VALID(cur))
		 * {
		 * int iClipSize = WEAPON::GET_WEAPON_CLIP_SIZE(cur);
		 * sprintf_s(cClipSize, "ClipSize: %.d", iClipSize);
		 * vDrawString(cClipSize, 0.5f, 0.5f);
		 * }
		 * }
		 * Full list of weapons by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/weapons.json
		 */
		int GET_WEAPON_CLIP_SIZE(Hash weaponHash);
		/**
		 * Full list of weapons by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/weapons.json
		 */
		float GET_WEAPON_TIME_BETWEEN_SHOTS(Hash weaponHash);
		void SET_PED_CHANCE_OF_FIRING_BLANKS(Ped ped, float xBias, float yBias);
		/**
		 * Returns handle of the projectile.
		 */
		Object SET_PED_SHOOT_ORDNANCE_WEAPON(Ped ped, float p1);
		void REQUEST_WEAPON_HIGH_DETAIL_MODEL(Entity weaponObject);
		void _SET_WEAPON_PED_DAMAGE_MODIFIER(Hash weapon, float damageModifier);
		/**
		 * Changes the weapon damage output by the given multiplier value. Must be run every frame.
		 * Full list of weapons by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/weapons.json
		 */
		void SET_WEAPON_DAMAGE_MODIFIER(Hash weaponHash, float damageMultiplier);
		void SET_WEAPON_AOE_MODIFIER(Hash weaponHash, float multiplier);
		/**
		 * ex, WEAPON::SET_WEAPON_EFFECT_DURATION_MODIFIER(joaat("vehicle_weapon_mine_slick"), 1.0);
		 */
		void SET_WEAPON_EFFECT_DURATION_MODIFIER(Hash p0, float p1);
		/**
		 * This native returns a true or false value.
		 * 
		 * Ped ped = The ped whose weapon you want to check.
		 */
		BOOL IS_PED_CURRENT_WEAPON_SILENCED(Ped ped);
		BOOL IS_FLASH_LIGHT_ON(Ped ped);
		BOOL SET_FLASH_LIGHT_FADE_DISTANCE(float distance);
		/**
		 * Enables/disables flashlight on ped's weapon.
		 */
		void SET_FLASH_LIGHT_ACTIVE_HISTORY(Ped ped, BOOL toggle);
		/**
		 * Changes the selected ped aiming animation style.
		 * Note : You must use GET_HASH_KEY!
		 * 
		 * Strings to use with GET_HASH_KEY :
		 * 
		 * "Ballistic",
		 * "Default",
		 * "Fat",
		 * "Female",
		 * "FirstPerson",
		 * "FirstPersonAiming",
		 * "FirstPersonFranklin",
		 * "FirstPersonFranklinAiming",
		 * "FirstPersonFranklinRNG",
		 * "FirstPersonFranklinScope",
		 * "FirstPersonMPFemale",
		 * "FirstPersonMichael",
		 * "FirstPersonMichaelAiming",
		 * "FirstPersonMichaelRNG",
		 * "FirstPersonMichaelScope",
		 * "FirstPersonRNG",
		 * "FirstPersonScope",
		 * "FirstPersonTrevor",
		 * "FirstPersonTrevorAiming",
		 * "FirstPersonTrevorRNG",
		 * "FirstPersonTrevorScope",
		 * "Franklin",
		 * "Gang",
		 * "Gang1H",
		 * "GangFemale",
		 * "Hillbilly",
		 * "MP_F_Freemode",
		 * "Michael",
		 * "SuperFat",
		 * "Trevor"
		 */
		void SET_WEAPON_ANIMATION_OVERRIDE(Ped ped, Hash animStyle);
		/**
		 * enum class eDamageType
		 * {
		 * UNKNOWN = 0,
		 * NONE = 1,
		 * MELEE = 2,
		 * BULLET = 3,
		 * BULLET_RUBBER = 4,
		 * EXPLOSIVE = 5,
		 * FIRE = 6,
		 * COLLISION = 7,
		 * FALL = 8,
		 * DROWN = 9,
		 * ELECTRIC = 10,
		 * BARBED_WIRE = 11,
		 * FIRE_EXTINGUISHER = 12,
		 * SMOKE = 13,
		 * WATER_CANNON = 14,
		 * TRANQUILIZER = 15,
		 * };
		 * 
		 * Full list of weapons by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/weapons.json
		 */
		int GET_WEAPON_DAMAGE_TYPE(Hash weaponHash);
		void SET_EQIPPED_WEAPON_START_SPINNING_AT_FULL_SPEED(Ped ped);
		/**
		 * this returns if you can use the weapon while using a parachute
		 * Full list of weapons by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/weapons.json
		 */
		BOOL CAN_USE_WEAPON_ON_PARACHUTE(Hash weaponHash);
		/**
		 * Both coordinates are from objects in the decompiled scripts. Native related to 0xECDC202B25E5CF48 p1 value. The only weapon hash used in the decompiled scripts is weapon_air_defence_gun. These two natives are used by the yacht script, decompiled scripts suggest it and the weapon hash used (valkyrie's rockets) are also used by yachts.
		 */
		int CREATE_AIR_DEFENCE_SPHERE(float x, float y, float z, float radius, float p4, float p5, float p6, Hash weaponHash);
		/**
		 * Full list of weapons by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/weapons.json
		 */
		int CREATE_AIR_DEFENCE_ANGLED_AREA(float p0, float p1, float p2, float p3, float p4, float p5, float p6, float p7, float p8, float radius, Hash weaponHash);
		BOOL REMOVE_AIR_DEFENCE_SPHERE(int zoneId);
		void REMOVE_ALL_AIR_DEFENCE_SPHERES();
		void SET_PLAYER_TARGETTABLE_FOR_AIR_DEFENCE_SPHERE(Player player, int zoneId, BOOL enable);
		BOOL IS_AIR_DEFENCE_SPHERE_IN_AREA(float x, float y, float z, float radius, int* outZoneId);
		void FIRE_AIR_DEFENCE_SPHERE_WEAPON_AT_POSITION(int zoneId, float x, float y, float z);
		BOOL DOES_AIR_DEFENCE_SPHERE_EXIST(int zoneId);
		/**
		 * Disables selecting the given weapon. Ped isn't forced to put the gun away. However you can't reselect the weapon if you holster then unholster. Weapon is also grayed out on the weapon wheel.
		 * Full list of weapons by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/weapons.json
		 */
		void SET_CAN_PED_SELECT_INVENTORY_WEAPON(Ped ped, Hash weaponHash, BOOL toggle);
		/**
		 * Disable all weapons. Does the same as 0xB4771B9AAF4E68E4 except for all weapons.
		 */
		void SET_CAN_PED_SELECT_ALL_WEAPONS(Ped ped, BOOL toggle);

	} // namespace WEAPON

	namespace ZONE {

		int GET_ZONE_AT_COORDS(float x, float y, float z);
		/**
		 * 'zoneName' corresponds to an entry in 'popzone.ipl'.
		 * 
		 * AIRP = Los Santos International Airport
		 * ALAMO = Alamo Sea
		 * ALTA = Alta
		 * ARMYB = Fort Zancudo
		 * BANHAMC = Banham Canyon Dr
		 * BANNING = Banning
		 * BEACH = Vespucci Beach
		 * BHAMCA = Banham Canyon
		 * BRADP = Braddock Pass
		 * BRADT = Braddock Tunnel
		 * BURTON = Burton
		 * CALAFB = Calafia Bridge
		 * CANNY = Raton Canyon
		 * CCREAK = Cassidy Creek
		 * CHAMH = Chamberlain Hills
		 * CHIL = Vinewood Hills
		 * CHU = Chumash
		 * CMSW = Chiliad Mountain State Wilderness
		 * CYPRE = Cypress Flats
		 * DAVIS = Davis
		 * DELBE = Del Perro Beach
		 * DELPE = Del Perro
		 * DELSOL = La Puerta
		 * DESRT = Grand Senora Desert
		 * DOWNT = Downtown
		 * DTVINE = Downtown Vinewood
		 * EAST_V = East Vinewood
		 * EBURO = El Burro Heights
		 * ELGORL = El Gordo Lighthouse
		 * ELYSIAN = Elysian Island
		 * GALFISH = Galilee
		 * GOLF = GWC and Golfing Society
		 * GRAPES = Grapeseed
		 * GREATC = Great Chaparral
		 * HARMO = Harmony
		 * HAWICK = Hawick
		 * HORS = Vinewood Racetrack
		 * HUMLAB = Humane Labs and Research
		 * JAIL = Bolingbroke Penitentiary
		 * KOREAT = Little Seoul
		 * LACT = Land Act Reservoir
		 * LAGO = Lago Zancudo
		 * LDAM = Land Act Dam
		 * LEGSQU = Legion Square
		 * LMESA = La Mesa
		 * LOSPUER = La Puerta
		 * MIRR = Mirror Park
		 * MORN = Morningwood
		 * MOVIE = Richards Majestic
		 * MTCHIL = Mount Chiliad
		 * MTGORDO = Mount Gordo
		 * MTJOSE = Mount Josiah
		 * MURRI = Murrieta Heights
		 * NCHU = North Chumash
		 * NOOSE = N.O.O.S.E
		 * OCEANA = Pacific Ocean
		 * PALCOV = Paleto Cove
		 * PALETO = Paleto Bay
		 * PALFOR = Paleto Forest
		 * PALHIGH = Palomino Highlands
		 * PALMPOW = Palmer-Taylor Power Station
		 * PBLUFF = Pacific Bluffs
		 * PBOX = Pillbox Hill
		 * PROCOB = Procopio Beach
		 * RANCHO = Rancho
		 * RGLEN = Richman Glen
		 * RICHM = Richman
		 * ROCKF = Rockford Hills
		 * RTRAK = Redwood Lights Track
		 * SANAND = San Andreas
		 * SANCHIA = San Chianski Mountain Range
		 * SANDY = Sandy Shores
		 * SKID = Mission Row
		 * SLAB = Stab City
		 * STAD = Maze Bank Arena
		 * STRAW = Strawberry
		 * TATAMO = Tataviam Mountains
		 * TERMINA = Terminal
		 * TEXTI = Textile City
		 * TONGVAH = Tongva Hills
		 * TONGVAV = Tongva Valley
		 * VCANA = Vespucci Canals
		 * VESP = Vespucci
		 * VINE = Vinewood
		 * WINDF = Ron Alternates Wind Farm
		 * WVINE = West Vinewood
		 * ZANCUDO = Zancudo River
		 * ZP_ORT = Port of South Los Santos
		 * ZQ_UAR = Davis Quartz
		 * 
		 * Full list of zones by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/zones.json
		 */
		int GET_ZONE_FROM_NAME_ID(const char* zoneName);
		int GET_ZONE_POPSCHEDULE(int zoneId);
		/**
		 * AIRP = Los Santos International Airport
		 * ALAMO = Alamo Sea
		 * ALTA = Alta
		 * ARMYB = Fort Zancudo
		 * BANHAMC = Banham Canyon Dr
		 * BANNING = Banning
		 * BEACH = Vespucci Beach
		 * BHAMCA = Banham Canyon
		 * BRADP = Braddock Pass
		 * BRADT = Braddock Tunnel
		 * BURTON = Burton
		 * CALAFB = Calafia Bridge
		 * CANNY = Raton Canyon
		 * CCREAK = Cassidy Creek
		 * CHAMH = Chamberlain Hills
		 * CHIL = Vinewood Hills
		 * CHU = Chumash
		 * CMSW = Chiliad Mountain State Wilderness
		 * CYPRE = Cypress Flats
		 * DAVIS = Davis
		 * DELBE = Del Perro Beach
		 * DELPE = Del Perro
		 * DELSOL = La Puerta
		 * DESRT = Grand Senora Desert
		 * DOWNT = Downtown
		 * DTVINE = Downtown Vinewood
		 * EAST_V = East Vinewood
		 * EBURO = El Burro Heights
		 * ELGORL = El Gordo Lighthouse
		 * ELYSIAN = Elysian Island
		 * GALFISH = Galilee
		 * GOLF = GWC and Golfing Society
		 * GRAPES = Grapeseed
		 * GREATC = Great Chaparral
		 * HARMO = Harmony
		 * HAWICK = Hawick
		 * HORS = Vinewood Racetrack
		 * HUMLAB = Humane Labs and Research
		 * JAIL = Bolingbroke Penitentiary
		 * KOREAT = Little Seoul
		 * LACT = Land Act Reservoir
		 * LAGO = Lago Zancudo
		 * LDAM = Land Act Dam
		 * LEGSQU = Legion Square
		 * LMESA = La Mesa
		 * LOSPUER = La Puerta
		 * MIRR = Mirror Park
		 * MORN = Morningwood
		 * MOVIE = Richards Majestic
		 * MTCHIL = Mount Chiliad
		 * MTGORDO = Mount Gordo
		 * MTJOSE = Mount Josiah
		 * MURRI = Murrieta Heights
		 * NCHU = North Chumash
		 * NOOSE = N.O.O.S.E
		 * OCEANA = Pacific Ocean
		 * PALCOV = Paleto Cove
		 * PALETO = Paleto Bay
		 * PALFOR = Paleto Forest
		 * PALHIGH = Palomino Highlands
		 * PALMPOW = Palmer-Taylor Power Station
		 * PBLUFF = Pacific Bluffs
		 * PBOX = Pillbox Hill
		 * PROCOB = Procopio Beach
		 * RANCHO = Rancho
		 * RGLEN = Richman Glen
		 * RICHM = Richman
		 * ROCKF = Rockford Hills
		 * RTRAK = Redwood Lights Track
		 * SANAND = San Andreas
		 * SANCHIA = San Chianski Mountain Range
		 * SANDY = Sandy Shores
		 * SKID = Mission Row
		 * SLAB = Stab City
		 * STAD = Maze Bank Arena
		 * STRAW = Strawberry
		 * TATAMO = Tataviam Mountains
		 * TERMINA = Terminal
		 * TEXTI = Textile City
		 * TONGVAH = Tongva Hills
		 * TONGVAV = Tongva Valley
		 * VCANA = Vespucci Canals
		 * VESP = Vespucci
		 * VINE = Vinewood
		 * WINDF = Ron Alternates Wind Farm
		 * WVINE = West Vinewood
		 * ZANCUDO = Zancudo River
		 * ZP_ORT = Port of South Los Santos
		 * ZQ_UAR = Davis Quartz
		 * 
		 * Full list of zones by DurtyFree: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/zones.json
		 */
		const char* GET_NAME_OF_ZONE(float x, float y, float z);
		void SET_ZONE_ENABLED(int zoneId, BOOL toggle);
		/**
		 * cellphone range 1- 5 used for signal bar in iFruit phone
		 */
		int GET_ZONE_SCUMMINESS(int zoneId);
		/**
		 * Only used once in the decompiled scripts. Seems to be related to scripted vehicle generators.
		 * 
		 * Modified example from "am_imp_exp.c4", line 6406:
		 * /* popSchedules[0] = ZONE::GET_ZONE_POPSCHEDULE(ZONE::GET_ZONE_AT_COORDS(891.3, 807.9, 188.1));
		 * etc.
		 * * /
		 * ZONE::OVERRIDE_POPSCHEDULE_VEHICLE_MODEL(popSchedules[index], vehicleHash);
		 * STREAMING::REQUEST_MODEL(vehicleHash);
		 */
		void OVERRIDE_POPSCHEDULE_VEHICLE_MODEL(int scheduleId, Hash vehicleHash);
		/**
		 * Only used once in the decompiled scripts. Seems to be related to scripted vehicle generators.
		 * 
		 * Modified example from "am_imp_exp.c4", line 6418:
		 * /* popSchedules[0] = ZONE::GET_ZONE_POPSCHEDULE(ZONE::GET_ZONE_AT_COORDS(891.3, 807.9, 188.1));
		 * etc.
		 * * /
		 * STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(vehicleHash);
		 * ZONE::CLEAR_POPSCHEDULE_OVERRIDE_VEHICLE_MODEL(popSchedules[index]);
		 */
		void CLEAR_POPSCHEDULE_OVERRIDE_VEHICLE_MODEL(int scheduleId);
		/**
		 * Returns a hash representing which part of the map the given coords are located.
		 * 
		 * Possible return values:
		 * (Hash of) city -> -289320599
		 * (Hash of) countryside -> 2072609373
		 * 
		 * C# Example :
		 * 
		 * Ped player = Game.Player.Character;
		 * Hash h = Function.Call<Hash>(Hash.GET_HASH_OF_MAP_AREA_AT_COORDS, player.Position.X, player.Position.Y, player.Position.Z);
		 */
		Hash GET_HASH_OF_MAP_AREA_AT_COORDS(float x, float y, float z);

	} // namespace ZONE

} // namespace base::menu::natives

#pragma warning(pop)
