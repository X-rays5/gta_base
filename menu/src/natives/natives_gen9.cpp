#pragma warning(push)
#pragma warning(disable: 4100)

// Auto-generated file - DO NOT EDIT
// Generated from GTA5 NativeDB data

#include "natives_gen9.hpp"

namespace base::menu::natives {

	namespace APP {

		BOOL APP_DATA_VALID() {
			return Invoker::Invoke<BOOL, 400>();
		}

		int APP_GET_INT(const char* property) {
			return Invoker::Invoke<int, 401>(property);
		}

		float APP_GET_FLOAT(const char* property) {
			return Invoker::Invoke<float, 402>(property);
		}

		const char* APP_GET_STRING(const char* property) {
			return Invoker::Invoke<const char*, 403>(property);
		}

		void APP_SET_INT(const char* property, int value) {
			Invoker::Invoke<void, 404>(property, value);
		}

		void APP_SET_FLOAT(const char* property, float value) {
			Invoker::Invoke<void, 405>(property, value);
		}

		void APP_SET_STRING(const char* property, const char* value) {
			Invoker::Invoke<void, 406>(property, value);
		}

		void APP_SET_APP(const char* appName) {
			Invoker::Invoke<void, 407>(appName);
		}

		void APP_SET_BLOCK(const char* blockName) {
			Invoker::Invoke<void, 408>(blockName);
		}

		void APP_CLEAR_BLOCK() {
			Invoker::Invoke<void, 409>();
		}

		void APP_CLOSE_APP() {
			Invoker::Invoke<void, 410>();
		}

		void APP_CLOSE_BLOCK() {
			Invoker::Invoke<void, 411>();
		}

		BOOL APP_HAS_LINKED_SOCIAL_CLUB_ACCOUNT() {
			return Invoker::Invoke<BOOL, 412>();
		}

		BOOL APP_HAS_SYNCED_DATA(const char* appName) {
			return Invoker::Invoke<BOOL, 413>(appName);
		}

		void APP_SAVE_DATA() {
			Invoker::Invoke<void, 414>();
		}

		int APP_GET_DELETED_FILE_STATUS() {
			return Invoker::Invoke<int, 415>();
		}

		BOOL APP_DELETE_APP_DATA(const char* appName) {
			return Invoker::Invoke<BOOL, 416>(appName);
		}

	} // namespace APP

	namespace AUDIO {

		void PLAY_PED_RINGTONE(const char* ringtoneName, Ped ped, BOOL p2) {
			Invoker::Invoke<void, 2895>(ringtoneName, ped, p2);
		}

		BOOL IS_PED_RINGTONE_PLAYING(Ped ped) {
			return Invoker::Invoke<BOOL, 2896>(ped);
		}

		void STOP_PED_RINGTONE(Ped ped) {
			Invoker::Invoke<void, 2897>(ped);
		}

		BOOL IS_MOBILE_PHONE_CALL_ONGOING() {
			return Invoker::Invoke<BOOL, 2898>();
		}

		BOOL IS_MOBILE_INTERFERENCE_ACTIVE() {
			return Invoker::Invoke<BOOL, 2899>();
		}

		int GET_CURRENT_TV_SHOW_PLAY_TIME() {
			return Invoker::Invoke<int, 2900>();
		}

		void CREATE_NEW_SCRIPTED_CONVERSATION() {
			Invoker::Invoke<void, 2901>();
		}

		void ADD_LINE_TO_CONVERSATION(int index, const char* p1, const char* p2, int p3, int p4, BOOL p5, BOOL p6, BOOL p7, BOOL p8, int p9, BOOL p10, BOOL p11, BOOL p12) {
			Invoker::Invoke<void, 2902>(index, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12);
		}

		void ADD_PED_TO_CONVERSATION(int index, Ped ped, const char* p2) {
			Invoker::Invoke<void, 2903>(index, ped, p2);
		}

		void SET_POSITION_FOR_NULL_CONV_PED(Any p0, float p1, float p2, float p3) {
			Invoker::Invoke<void, 2904>(p0, p1, p2, p3);
		}

		void SET_ENTITY_FOR_NULL_CONV_PED(int p0, Entity entity) {
			Invoker::Invoke<void, 2905>(p0, entity);
		}

		void SET_MICROPHONE_POSITION(BOOL toggle, float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3) {
			Invoker::Invoke<void, 2906>(toggle, x1, y1, z1, x2, y2, z2, x3, y3, z3);
		}

		void SET_CONVERSATION_AUDIO_CONTROLLED_BY_ANIM(BOOL p0) {
			Invoker::Invoke<void, 2907>(p0);
		}

		void SET_CONVERSATION_AUDIO_PLACEHOLDER(BOOL p0) {
			Invoker::Invoke<void, 2908>(p0);
		}

		void START_SCRIPT_PHONE_CONVERSATION(BOOL p0, BOOL p1) {
			Invoker::Invoke<void, 2909>(p0, p1);
		}

		void PRELOAD_SCRIPT_PHONE_CONVERSATION(BOOL p0, BOOL p1) {
			Invoker::Invoke<void, 2910>(p0, p1);
		}

		void START_SCRIPT_CONVERSATION(BOOL p0, BOOL p1, BOOL p2, BOOL p3) {
			Invoker::Invoke<void, 2911>(p0, p1, p2, p3);
		}

		void PRELOAD_SCRIPT_CONVERSATION(BOOL p0, BOOL p1, BOOL p2, BOOL p3) {
			Invoker::Invoke<void, 2912>(p0, p1, p2, p3);
		}

		void START_PRELOADED_CONVERSATION() {
			Invoker::Invoke<void, 2913>();
		}

		BOOL GET_IS_PRELOADED_CONVERSATION_READY() {
			return Invoker::Invoke<BOOL, 2914>();
		}

		BOOL IS_SCRIPTED_CONVERSATION_ONGOING() {
			return Invoker::Invoke<BOOL, 2915>();
		}

		BOOL IS_SCRIPTED_CONVERSATION_LOADED() {
			return Invoker::Invoke<BOOL, 2916>();
		}

		int GET_CURRENT_SCRIPTED_CONVERSATION_LINE() {
			return Invoker::Invoke<int, 2917>();
		}

		void PAUSE_SCRIPTED_CONVERSATION(BOOL p0) {
			Invoker::Invoke<void, 2918>(p0);
		}

		void RESTART_SCRIPTED_CONVERSATION() {
			Invoker::Invoke<void, 2919>();
		}

		int STOP_SCRIPTED_CONVERSATION(BOOL p0) {
			return Invoker::Invoke<int, 2920>(p0);
		}

		void SKIP_TO_NEXT_SCRIPTED_CONVERSATION_LINE() {
			Invoker::Invoke<void, 2921>();
		}

		void INTERRUPT_CONVERSATION(Ped ped, const char* voiceline, const char* speaker) {
			Invoker::Invoke<void, 2922>(ped, voiceline, speaker);
		}

		void INTERRUPT_CONVERSATION_AND_PAUSE(Ped ped, const char* p1, const char* speaker) {
			Invoker::Invoke<void, 2923>(ped, p1, speaker);
		}

		int GET_VARIATION_CHOSEN_FOR_SCRIPTED_LINE(Any* p0) {
			return Invoker::Invoke<int, 2924>(p0);
		}

		void SET_NO_DUCKING_FOR_CONVERSATION(BOOL p0) {
			Invoker::Invoke<void, 2925>(p0);
		}

		void REGISTER_SCRIPT_WITH_AUDIO(int p0) {
			Invoker::Invoke<void, 2926>(p0);
		}

		void UNREGISTER_SCRIPT_WITH_AUDIO() {
			Invoker::Invoke<void, 2927>();
		}

		BOOL REQUEST_MISSION_AUDIO_BANK(const char* audioBank, BOOL p1, Any p2) {
			return Invoker::Invoke<BOOL, 2928>(audioBank, p1, p2);
		}

		BOOL REQUEST_AMBIENT_AUDIO_BANK(const char* audioBank, BOOL p1, Any p2) {
			return Invoker::Invoke<BOOL, 2929>(audioBank, p1, p2);
		}

		BOOL REQUEST_SCRIPT_AUDIO_BANK(const char* audioBank, BOOL p1, Any p2) {
			return Invoker::Invoke<BOOL, 2930>(audioBank, p1, p2);
		}

		BOOL HINT_MISSION_AUDIO_BANK(const char* audioBank, BOOL p1, Any p2) {
			return Invoker::Invoke<BOOL, 2931>(audioBank, p1, p2);
		}

		BOOL HINT_AMBIENT_AUDIO_BANK(const char* audioBank, BOOL p1, Any p2) {
			return Invoker::Invoke<BOOL, 2932>(audioBank, p1, p2);
		}

		BOOL HINT_SCRIPT_AUDIO_BANK(const char* audioBank, BOOL p1, Any p2) {
			return Invoker::Invoke<BOOL, 2933>(audioBank, p1, p2);
		}

		void RELEASE_MISSION_AUDIO_BANK() {
			Invoker::Invoke<void, 2934>();
		}

		void RELEASE_AMBIENT_AUDIO_BANK() {
			Invoker::Invoke<void, 2935>();
		}

		void RELEASE_NAMED_SCRIPT_AUDIO_BANK(const char* audioBank) {
			Invoker::Invoke<void, 2936>(audioBank);
		}

		void RELEASE_SCRIPT_AUDIO_BANK() {
			Invoker::Invoke<void, 2937>();
		}

		void UNHINT_AMBIENT_AUDIO_BANK() {
			Invoker::Invoke<void, 2938>();
		}

		void UNHINT_SCRIPT_AUDIO_BANK() {
			Invoker::Invoke<void, 2939>();
		}

		void UNHINT_NAMED_SCRIPT_AUDIO_BANK(const char* audioBank) {
			Invoker::Invoke<void, 2940>(audioBank);
		}

		int GET_SOUND_ID() {
			return Invoker::Invoke<int, 2941>();
		}

		void RELEASE_SOUND_ID(int soundId) {
			Invoker::Invoke<void, 2942>(soundId);
		}

		void PLAY_SOUND(int soundId, const char* audioName, const char* audioRef, BOOL p3, Any p4, BOOL p5) {
			Invoker::Invoke<void, 2943>(soundId, audioName, audioRef, p3, p4, p5);
		}

		void PLAY_SOUND_FRONTEND(int soundId, const char* audioName, const char* audioRef, BOOL p3) {
			Invoker::Invoke<void, 2944>(soundId, audioName, audioRef, p3);
		}

		void PLAY_DEFERRED_SOUND_FRONTEND(const char* soundName, const char* soundsetName) {
			Invoker::Invoke<void, 2945>(soundName, soundsetName);
		}

		void PLAY_SOUND_FROM_ENTITY(int soundId, const char* audioName, Entity entity, const char* audioRef, BOOL isNetwork, Any p5) {
			Invoker::Invoke<void, 2946>(soundId, audioName, entity, audioRef, isNetwork, p5);
		}

		void PLAY_SOUND_FROM_ENTITY_HASH(int soundId, Hash model, Entity entity, Hash soundSetHash, Any p4, Any p5) {
			Invoker::Invoke<void, 2947>(soundId, model, entity, soundSetHash, p4, p5);
		}

		void PLAY_SOUND_FROM_COORD(int soundId, const char* audioName, float x, float y, float z, const char* audioRef, BOOL isNetwork, int range, BOOL p8) {
			Invoker::Invoke<void, 2948>(soundId, audioName, x, y, z, audioRef, isNetwork, range, p8);
		}

		void UPDATE_SOUND_COORD(int soundId, float x, float y, float z) {
			Invoker::Invoke<void, 2949>(soundId, x, y, z);
		}

		void STOP_SOUND(int soundId) {
			Invoker::Invoke<void, 2950>(soundId);
		}

		int GET_NETWORK_ID_FROM_SOUND_ID(int soundId) {
			return Invoker::Invoke<int, 2951>(soundId);
		}

		int GET_SOUND_ID_FROM_NETWORK_ID(int netId) {
			return Invoker::Invoke<int, 2952>(netId);
		}

		void SET_VARIABLE_ON_SOUND(int soundId, const char* variable, float p2) {
			Invoker::Invoke<void, 2953>(soundId, variable, p2);
		}

		void SET_VARIABLE_ON_STREAM(const char* variable, float p1) {
			Invoker::Invoke<void, 2954>(variable, p1);
		}

		void OVERRIDE_UNDERWATER_STREAM(const char* p0, BOOL p1) {
			Invoker::Invoke<void, 2955>(p0, p1);
		}

		void SET_VARIABLE_ON_UNDER_WATER_STREAM(const char* variableName, float value) {
			Invoker::Invoke<void, 2956>(variableName, value);
		}

		BOOL HAS_SOUND_FINISHED(int soundId) {
			return Invoker::Invoke<BOOL, 2957>(soundId);
		}

		void PLAY_PED_AMBIENT_SPEECH_NATIVE(Ped ped, const char* speechName, const char* speechParam, Any p3) {
			Invoker::Invoke<void, 2958>(ped, speechName, speechParam, p3);
		}

		void PLAY_PED_AMBIENT_SPEECH_AND_CLONE_NATIVE(Ped ped, const char* speechName, const char* speechParam, Any p3) {
			Invoker::Invoke<void, 2959>(ped, speechName, speechParam, p3);
		}

		void PLAY_PED_AMBIENT_SPEECH_WITH_VOICE_NATIVE(Ped ped, const char* speechName, const char* voiceName, const char* speechParam, BOOL p4) {
			Invoker::Invoke<void, 2960>(ped, speechName, voiceName, speechParam, p4);
		}

		void PLAY_AMBIENT_SPEECH_FROM_POSITION_NATIVE(const char* speechName, const char* voiceName, float x, float y, float z, const char* speechParam) {
			Invoker::Invoke<void, 2961>(speechName, voiceName, x, y, z, speechParam);
		}

		void OVERRIDE_TREVOR_RAGE(const char* voiceEffect) {
			Invoker::Invoke<void, 2962>(voiceEffect);
		}

		void RESET_TREVOR_RAGE() {
			Invoker::Invoke<void, 2963>();
		}

		void SET_PLAYER_ANGRY(Ped ped, BOOL toggle) {
			Invoker::Invoke<void, 2964>(ped, toggle);
		}

		void PLAY_PAIN(Ped ped, int painID, int p1, Any p3) {
			Invoker::Invoke<void, 2965>(ped, painID, p1, p3);
		}

		void RELEASE_WEAPON_AUDIO() {
			Invoker::Invoke<void, 2966>();
		}

		void ACTIVATE_AUDIO_SLOWMO_MODE(const char* mode) {
			Invoker::Invoke<void, 2967>(mode);
		}

		void DEACTIVATE_AUDIO_SLOWMO_MODE(const char* mode) {
			Invoker::Invoke<void, 2968>(mode);
		}

		void SET_AMBIENT_VOICE_NAME(Ped ped, const char* name) {
			Invoker::Invoke<void, 2969>(ped, name);
		}

		void SET_AMBIENT_VOICE_NAME_HASH(Ped ped, Hash hash) {
			Invoker::Invoke<void, 2970>(ped, hash);
		}

		Hash GET_AMBIENT_VOICE_NAME_HASH(Ped ped) {
			return Invoker::Invoke<Hash, 2971>(ped);
		}

		void SET_PED_VOICE_FULL(Ped ped) {
			Invoker::Invoke<void, 2972>(ped);
		}

		void SET_PED_RACE_AND_VOICE_GROUP(Ped ped, int p1, Hash voiceGroup) {
			Invoker::Invoke<void, 2973>(ped, p1, voiceGroup);
		}

		void SET_PED_VOICE_GROUP(Ped ped, Hash voiceGroupHash) {
			Invoker::Invoke<void, 2974>(ped, voiceGroupHash);
		}

		void SET_PED_VOICE_GROUP_FROM_RACE_TO_PVG(Ped ped, Hash voiceGroupHash) {
			Invoker::Invoke<void, 2975>(ped, voiceGroupHash);
		}

		void SET_PED_GENDER(Ped ped, BOOL p1) {
			Invoker::Invoke<void, 2976>(ped, p1);
		}

		void STOP_CURRENT_PLAYING_SPEECH(Ped ped) {
			Invoker::Invoke<void, 2977>(ped);
		}

		void STOP_CURRENT_PLAYING_AMBIENT_SPEECH(Ped ped) {
			Invoker::Invoke<void, 2978>(ped);
		}

		BOOL IS_AMBIENT_SPEECH_PLAYING(Ped ped) {
			return Invoker::Invoke<BOOL, 2979>(ped);
		}

		BOOL IS_SCRIPTED_SPEECH_PLAYING(Ped p0) {
			return Invoker::Invoke<BOOL, 2980>(p0);
		}

		BOOL IS_ANY_SPEECH_PLAYING(Ped ped) {
			return Invoker::Invoke<BOOL, 2981>(ped);
		}

		BOOL IS_ANY_POSITIONAL_SPEECH_PLAYING() {
			return Invoker::Invoke<BOOL, 2982>();
		}

		BOOL DOES_CONTEXT_EXIST_FOR_THIS_PED(Ped ped, const char* speechName, BOOL p2) {
			return Invoker::Invoke<BOOL, 2983>(ped, speechName, p2);
		}

		BOOL IS_PED_IN_CURRENT_CONVERSATION(Ped ped) {
			return Invoker::Invoke<BOOL, 2984>(ped);
		}

		void SET_PED_IS_DRUNK(Ped ped, BOOL toggle) {
			Invoker::Invoke<void, 2985>(ped, toggle);
		}

		void PLAY_ANIMAL_VOCALIZATION(Ped pedHandle, int p1, const char* speechName) {
			Invoker::Invoke<void, 2986>(pedHandle, p1, speechName);
		}

		BOOL IS_ANIMAL_VOCALIZATION_PLAYING(Ped pedHandle) {
			return Invoker::Invoke<BOOL, 2987>(pedHandle);
		}

		void SET_ANIMAL_MOOD(Ped animal, int mood) {
			Invoker::Invoke<void, 2988>(animal, mood);
		}

		void PLAY_PED_AUDIO_EVENT_ANIM(Ped pedHandle, const char* audioEvent) {
			Invoker::Invoke<void, 6703>(pedHandle, audioEvent);
		}

		BOOL IS_MOBILE_PHONE_RADIO_ACTIVE() {
			return Invoker::Invoke<BOOL, 2989>();
		}

		void SET_MOBILE_PHONE_RADIO_STATE(BOOL state) {
			Invoker::Invoke<void, 2990>(state);
		}

		int GET_PLAYER_RADIO_STATION_INDEX() {
			return Invoker::Invoke<int, 2991>();
		}

		const char* GET_PLAYER_RADIO_STATION_NAME() {
			return Invoker::Invoke<const char*, 2992>();
		}

		const char* GET_RADIO_STATION_NAME(int radioStation) {
			return Invoker::Invoke<const char*, 2993>(radioStation);
		}

		int GET_PLAYER_RADIO_STATION_GENRE() {
			return Invoker::Invoke<int, 2994>();
		}

		BOOL IS_RADIO_RETUNING() {
			return Invoker::Invoke<BOOL, 2995>();
		}

		BOOL IS_RADIO_FADED_OUT() {
			return Invoker::Invoke<BOOL, 2996>();
		}

		void SET_RADIO_RETUNE_UP() {
			Invoker::Invoke<void, 2997>();
		}

		void SET_RADIO_RETUNE_DOWN() {
			Invoker::Invoke<void, 2998>();
		}

		void SET_RADIO_TO_STATION_NAME(const char* stationName) {
			Invoker::Invoke<void, 2999>(stationName);
		}

		void SET_VEH_RADIO_STATION(Vehicle vehicle, const char* radioStation) {
			Invoker::Invoke<void, 3000>(vehicle, radioStation);
		}

		void SET_VEH_HAS_NORMAL_RADIO(Vehicle vehicle) {
			Invoker::Invoke<void, 3001>(vehicle);
		}

		BOOL IS_VEHICLE_RADIO_ON(Vehicle vehicle) {
			return Invoker::Invoke<BOOL, 3002>(vehicle);
		}

		void SET_VEH_FORCED_RADIO_THIS_FRAME(Vehicle vehicle) {
			Invoker::Invoke<void, 3003>(vehicle);
		}

		void SET_EMITTER_RADIO_STATION(const char* emitterName, const char* radioStation, Any p2) {
			Invoker::Invoke<void, 3004>(emitterName, radioStation, p2);
		}

		void SET_STATIC_EMITTER_ENABLED(const char* emitterName, BOOL toggle) {
			Invoker::Invoke<void, 3005>(emitterName, toggle);
		}

		void LINK_STATIC_EMITTER_TO_ENTITY(const char* emitterName, Entity entity) {
			Invoker::Invoke<void, 3006>(emitterName, entity);
		}

		void SET_RADIO_TO_STATION_INDEX(int radioStation) {
			Invoker::Invoke<void, 3007>(radioStation);
		}

		void SET_FRONTEND_RADIO_ACTIVE(BOOL active) {
			Invoker::Invoke<void, 3008>(active);
		}

		void UNLOCK_MISSION_NEWS_STORY(int newsStory) {
			Invoker::Invoke<void, 3009>(newsStory);
		}

		BOOL IS_MISSION_NEWS_STORY_UNLOCKED(int newsStory) {
			return Invoker::Invoke<BOOL, 3010>(newsStory);
		}

		int GET_AUDIBLE_MUSIC_TRACK_TEXT_ID() {
			return Invoker::Invoke<int, 3011>();
		}

		void PLAY_END_CREDITS_MUSIC(BOOL play) {
			Invoker::Invoke<void, 3012>(play);
		}

		void SKIP_RADIO_FORWARD() {
			Invoker::Invoke<void, 3013>();
		}

		void FREEZE_RADIO_STATION(const char* radioStation) {
			Invoker::Invoke<void, 3014>(radioStation);
		}

		void UNFREEZE_RADIO_STATION(const char* radioStation) {
			Invoker::Invoke<void, 3015>(radioStation);
		}

		void SET_RADIO_AUTO_UNFREEZE(BOOL toggle) {
			Invoker::Invoke<void, 3016>(toggle);
		}

		void SET_INITIAL_PLAYER_STATION(const char* radioStation) {
			Invoker::Invoke<void, 3017>(radioStation);
		}

		void SET_USER_RADIO_CONTROL_ENABLED(BOOL toggle) {
			Invoker::Invoke<void, 3018>(toggle);
		}

		void SET_RADIO_TRACK(const char* radioStation, const char* radioTrack) {
			Invoker::Invoke<void, 3019>(radioStation, radioTrack);
		}

		void SET_RADIO_TRACK_WITH_START_OFFSET(const char* radioStationName, const char* mixName, int p2) {
			Invoker::Invoke<void, 3020>(radioStationName, mixName, p2);
		}

		void SET_NEXT_RADIO_TRACK(const char* radioName, const char* radioTrack, const char* p2, const char* p3) {
			Invoker::Invoke<void, 3021>(radioName, radioTrack, p2, p3);
		}

		void SET_VEHICLE_RADIO_LOUD(Vehicle vehicle, BOOL toggle) {
			Invoker::Invoke<void, 3022>(vehicle, toggle);
		}

		BOOL CAN_VEHICLE_RECEIVE_CB_RADIO(Vehicle vehicle) {
			return Invoker::Invoke<BOOL, 3023>(vehicle);
		}

		void SET_MOBILE_RADIO_ENABLED_DURING_GAMEPLAY(BOOL toggle) {
			Invoker::Invoke<void, 3024>(toggle);
		}

		BOOL DOES_PLAYER_VEH_HAVE_RADIO() {
			return Invoker::Invoke<BOOL, 3025>();
		}

		BOOL IS_PLAYER_VEH_RADIO_ENABLE() {
			return Invoker::Invoke<BOOL, 3026>();
		}

		void SET_VEHICLE_RADIO_ENABLED(Vehicle vehicle, BOOL toggle) {
			Invoker::Invoke<void, 3027>(vehicle, toggle);
		}

		void SET_POSITIONED_PLAYER_VEHICLE_RADIO_EMITTER_ENABLED(Any p0) {
			Invoker::Invoke<void, 3028>(p0);
		}

		void SET_CUSTOM_RADIO_TRACK_LIST(const char* radioStation, const char* trackListName, BOOL p2) {
			Invoker::Invoke<void, 3029>(radioStation, trackListName, p2);
		}

		void CLEAR_CUSTOM_RADIO_TRACK_LIST(const char* radioStation) {
			Invoker::Invoke<void, 3030>(radioStation);
		}

		int GET_NUM_UNLOCKED_RADIO_STATIONS() {
			return Invoker::Invoke<int, 3031>();
		}

		int FIND_RADIO_STATION_INDEX(Hash stationNameHash) {
			return Invoker::Invoke<int, 3032>(stationNameHash);
		}

		void SET_RADIO_STATION_MUSIC_ONLY(const char* radioStation, BOOL toggle) {
			Invoker::Invoke<void, 3033>(radioStation, toggle);
		}

		void SET_RADIO_FRONTEND_FADE_TIME(float fadeTime) {
			Invoker::Invoke<void, 3034>(fadeTime);
		}

		void UNLOCK_RADIO_STATION_TRACK_LIST(const char* radioStation, const char* trackListName) {
			Invoker::Invoke<void, 3035>(radioStation, trackListName);
		}

		void LOCK_RADIO_STATION_TRACK_LIST(const char* radioStation, const char* trackListName) {
			Invoker::Invoke<void, 3036>(radioStation, trackListName);
		}

		void UPDATE_UNLOCKABLE_DJ_RADIO_TRACKS(BOOL enableMixes) {
			Invoker::Invoke<void, 3037>(enableMixes);
		}

		void LOCK_RADIO_STATION(const char* radioStationName, BOOL toggle) {
			Invoker::Invoke<void, 3038>(radioStationName, toggle);
		}

		void SET_RADIO_STATION_AS_FAVOURITE(const char* radioStation, BOOL toggle) {
			Invoker::Invoke<void, 3039>(radioStation, toggle);
		}

		BOOL IS_RADIO_STATION_FAVOURITED(const char* radioStation) {
			return Invoker::Invoke<BOOL, 3040>(radioStation);
		}

		BOOL GET_NEXT_AUDIBLE_BEAT(float* out1, float* out2, int* out3) {
			return Invoker::Invoke<BOOL, 3041>(out1, out2, out3);
		}

		void FORCE_MUSIC_TRACK_LIST(const char* radioStation, const char* trackListName, int milliseconds) {
			Invoker::Invoke<void, 3042>(radioStation, trackListName, milliseconds);
		}

		int GET_CURRENT_TRACK_PLAY_TIME(const char* radioStationName) {
			return Invoker::Invoke<int, 3043>(radioStationName);
		}

		Hash GET_CURRENT_TRACK_SOUND_NAME(const char* radioStationName) {
			return Invoker::Invoke<Hash, 3044>(radioStationName);
		}

		void SET_VEHICLE_MISSILE_WARNING_ENABLED(Vehicle vehicle, BOOL toggle) {
			Invoker::Invoke<void, 3045>(vehicle, toggle);
		}

		void SET_AMBIENT_ZONE_STATE(const char* zoneName, BOOL p1, BOOL p2) {
			Invoker::Invoke<void, 3046>(zoneName, p1, p2);
		}

		void CLEAR_AMBIENT_ZONE_STATE(const char* zoneName, BOOL p1) {
			Invoker::Invoke<void, 3047>(zoneName, p1);
		}

		void SET_AMBIENT_ZONE_LIST_STATE(const char* ambientZone, BOOL p1, BOOL p2) {
			Invoker::Invoke<void, 3048>(ambientZone, p1, p2);
		}

		void CLEAR_AMBIENT_ZONE_LIST_STATE(const char* ambientZone, BOOL p1) {
			Invoker::Invoke<void, 3049>(ambientZone, p1);
		}

		void SET_AMBIENT_ZONE_STATE_PERSISTENT(const char* ambientZone, BOOL p1, BOOL p2) {
			Invoker::Invoke<void, 3050>(ambientZone, p1, p2);
		}

		void SET_AMBIENT_ZONE_LIST_STATE_PERSISTENT(const char* ambientZone, BOOL p1, BOOL p2) {
			Invoker::Invoke<void, 3051>(ambientZone, p1, p2);
		}

		BOOL IS_AMBIENT_ZONE_ENABLED(const char* ambientZone) {
			return Invoker::Invoke<BOOL, 3052>(ambientZone);
		}

		void REFRESH_CLOSEST_OCEAN_SHORELINE() {
			Invoker::Invoke<void, 3053>();
		}

		void SET_CUTSCENE_AUDIO_OVERRIDE(const char* name) {
			Invoker::Invoke<void, 3054>(name);
		}

		void SET_VARIABLE_ON_SYNCH_SCENE_AUDIO(const char* variableName, float value) {
			Invoker::Invoke<void, 3055>(variableName, value);
		}

		int PLAY_POLICE_REPORT(const char* name, float p1) {
			return Invoker::Invoke<int, 3056>(name, p1);
		}

		void CANCEL_ALL_POLICE_REPORTS() {
			Invoker::Invoke<void, 3057>();
		}

		void BLIP_SIREN(Vehicle vehicle) {
			Invoker::Invoke<void, 3058>(vehicle);
		}

		void OVERRIDE_VEH_HORN(Vehicle vehicle, BOOL override, int hornHash) {
			Invoker::Invoke<void, 3059>(vehicle, override, hornHash);
		}

		BOOL IS_HORN_ACTIVE(Vehicle vehicle) {
			return Invoker::Invoke<BOOL, 3060>(vehicle);
		}

		void SET_AGGRESSIVE_HORNS(BOOL toggle) {
			Invoker::Invoke<void, 3061>(toggle);
		}

		void SET_RADIO_POSITION_AUDIO_MUTE(BOOL p0) {
			Invoker::Invoke<void, 3062>(p0);
		}

		void SET_VEHICLE_CONVERSATIONS_PERSIST(BOOL p0, BOOL p1) {
			Invoker::Invoke<void, 3063>(p0, p1);
		}

		void SET_VEHICLE_CONVERSATIONS_PERSIST_NEW(BOOL p0, BOOL p1, BOOL p2) {
			Invoker::Invoke<void, 3064>(p0, p1, p2);
		}

		BOOL IS_STREAM_PLAYING() {
			return Invoker::Invoke<BOOL, 3065>();
		}

		int GET_STREAM_PLAY_TIME() {
			return Invoker::Invoke<int, 3066>();
		}

		BOOL LOAD_STREAM(const char* streamName, const char* soundSet) {
			return Invoker::Invoke<BOOL, 3067>(streamName, soundSet);
		}

		BOOL LOAD_STREAM_WITH_START_OFFSET(const char* streamName, int startOffset, const char* soundSet) {
			return Invoker::Invoke<BOOL, 3068>(streamName, startOffset, soundSet);
		}

		void PLAY_STREAM_FROM_PED(Ped ped) {
			Invoker::Invoke<void, 3069>(ped);
		}

		void PLAY_STREAM_FROM_VEHICLE(Vehicle vehicle) {
			Invoker::Invoke<void, 3070>(vehicle);
		}

		void PLAY_STREAM_FROM_OBJECT(Object object) {
			Invoker::Invoke<void, 3071>(object);
		}

		void PLAY_STREAM_FRONTEND() {
			Invoker::Invoke<void, 3072>();
		}

		void PLAY_STREAM_FROM_POSITION(float x, float y, float z) {
			Invoker::Invoke<void, 3073>(x, y, z);
		}

		void STOP_STREAM() {
			Invoker::Invoke<void, 3074>();
		}

		void STOP_PED_SPEAKING(Ped ped, BOOL shaking) {
			Invoker::Invoke<void, 3075>(ped, shaking);
		}

		void BLOCK_ALL_SPEECH_FROM_PED(Ped ped, BOOL p1, BOOL p2) {
			Invoker::Invoke<void, 3076>(ped, p1, p2);
		}

		void STOP_PED_SPEAKING_SYNCED(Ped ped, BOOL p1) {
			Invoker::Invoke<void, 3077>(ped, p1);
		}

		void DISABLE_PED_PAIN_AUDIO(Ped ped, BOOL toggle) {
			Invoker::Invoke<void, 3078>(ped, toggle);
		}

		BOOL IS_AMBIENT_SPEECH_DISABLED(Ped ped) {
			return Invoker::Invoke<BOOL, 3079>(ped);
		}

		void BLOCK_SPEECH_CONTEXT_GROUP(const char* p0, int p1) {
			Invoker::Invoke<void, 3080>(p0, p1);
		}

		void UNBLOCK_SPEECH_CONTEXT_GROUP(const char* p0) {
			Invoker::Invoke<void, 3081>(p0);
		}

		void SET_SIREN_WITH_NO_DRIVER(Vehicle vehicle, BOOL toggle) {
			Invoker::Invoke<void, 3082>(vehicle, toggle);
		}

		void SET_SIREN_BYPASS_MP_DRIVER_CHECK(Vehicle vehicle, BOOL toggle) {
			Invoker::Invoke<void, 3083>(vehicle, toggle);
		}

		void TRIGGER_SIREN_AUDIO(Vehicle vehicle) {
			Invoker::Invoke<void, 3084>(vehicle);
		}

		void SET_HORN_PERMANENTLY_ON(Vehicle vehicle) {
			Invoker::Invoke<void, 3085>(vehicle);
		}

		void SET_HORN_ENABLED(Vehicle vehicle, BOOL toggle) {
			Invoker::Invoke<void, 3086>(vehicle, toggle);
		}

		void SET_AUDIO_VEHICLE_PRIORITY(Vehicle vehicle, Any p1) {
			Invoker::Invoke<void, 3087>(vehicle, p1);
		}

		void SET_HORN_PERMANENTLY_ON_TIME(Vehicle vehicle, float time) {
			Invoker::Invoke<void, 3088>(vehicle, time);
		}

		void USE_SIREN_AS_HORN(Vehicle vehicle, BOOL toggle) {
			Invoker::Invoke<void, 3089>(vehicle, toggle);
		}

		void FORCE_USE_AUDIO_GAME_OBJECT(Vehicle vehicle, const char* audioName) {
			Invoker::Invoke<void, 3090>(vehicle, audioName);
		}

		void PRELOAD_VEHICLE_AUDIO_BANK(Hash vehicleModel) {
			Invoker::Invoke<void, 3091>(vehicleModel);
		}

		void SET_VEHICLE_STARTUP_REV_SOUND(Vehicle vehicle, const char* p1, const char* p2) {
			Invoker::Invoke<void, 3092>(vehicle, p1, p2);
		}

		void RESET_VEHICLE_STARTUP_REV_SOUND(Vehicle vehicle) {
			Invoker::Invoke<void, 3093>(vehicle);
		}

		void SET_VEHICLE_FORCE_REVERSE_WARNING(Any p0, Any p1) {
			Invoker::Invoke<void, 3094>(p0, p1);
		}

		BOOL IS_VEHICLE_AUDIBLY_DAMAGED(Vehicle vehicle) {
			return Invoker::Invoke<BOOL, 3095>(vehicle);
		}

		void SET_VEHICLE_AUDIO_ENGINE_DAMAGE_FACTOR(Vehicle vehicle, float damageFactor) {
			Invoker::Invoke<void, 3096>(vehicle, damageFactor);
		}

		void SET_VEHICLE_AUDIO_BODY_DAMAGE_FACTOR(Vehicle vehicle, float intensity) {
			Invoker::Invoke<void, 3097>(vehicle, intensity);
		}

		void _FORCE_VEHICLE_ENGINE_SYNTH(Vehicle vehicle, BOOL force) {
			Invoker::Invoke<void, 6568>(vehicle, force);
		}

		void ENABLE_VEHICLE_FANBELT_DAMAGE(Vehicle vehicle, BOOL toggle) {
			Invoker::Invoke<void, 3098>(vehicle, toggle);
		}

		void ENABLE_VEHICLE_EXHAUST_POPS(Vehicle vehicle, BOOL toggle) {
			Invoker::Invoke<void, 3099>(vehicle, toggle);
		}

		void SET_VEHICLE_BOOST_ACTIVE(Vehicle vehicle, BOOL toggle) {
			Invoker::Invoke<void, 3100>(vehicle, toggle);
		}

		void SET_PLAYER_VEHICLE_ALARM_AUDIO_ACTIVE(Vehicle vehicle, BOOL toggle) {
			Invoker::Invoke<void, 3101>(vehicle, toggle);
		}

		void SET_SCRIPT_UPDATE_DOOR_AUDIO(Hash doorHash, BOOL toggle) {
			Invoker::Invoke<void, 3102>(doorHash, toggle);
		}

		void PLAY_VEHICLE_DOOR_OPEN_SOUND(Vehicle vehicle, int doorId) {
			Invoker::Invoke<void, 3103>(vehicle, doorId);
		}

		void PLAY_VEHICLE_DOOR_CLOSE_SOUND(Vehicle vehicle, int doorId) {
			Invoker::Invoke<void, 3104>(vehicle, doorId);
		}

		void ENABLE_STALL_WARNING_SOUNDS(Vehicle vehicle, BOOL toggle) {
			Invoker::Invoke<void, 3105>(vehicle, toggle);
		}

		void _ENABLE_DRAG_RACE_STATIONARY_WARNING_SOUNDS(Vehicle vehicle, BOOL enable) {
			Invoker::Invoke<void, 3106>(vehicle, enable);
		}

		BOOL IS_GAME_IN_CONTROL_OF_MUSIC() {
			return Invoker::Invoke<BOOL, 3107>();
		}

		void SET_GPS_ACTIVE(BOOL active) {
			Invoker::Invoke<void, 3108>(active);
		}

		void PLAY_MISSION_COMPLETE_AUDIO(const char* audioName) {
			Invoker::Invoke<void, 3109>(audioName);
		}

		BOOL IS_MISSION_COMPLETE_PLAYING() {
			return Invoker::Invoke<BOOL, 3110>();
		}

		BOOL IS_MISSION_COMPLETE_READY_FOR_UI() {
			return Invoker::Invoke<BOOL, 3111>();
		}

		void BLOCK_DEATH_JINGLE(BOOL toggle) {
			Invoker::Invoke<void, 3112>(toggle);
		}

		BOOL START_AUDIO_SCENE(const char* scene) {
			return Invoker::Invoke<BOOL, 3113>(scene);
		}

		void STOP_AUDIO_SCENE(const char* scene) {
			Invoker::Invoke<void, 3114>(scene);
		}

		void STOP_AUDIO_SCENES() {
			Invoker::Invoke<void, 3115>();
		}

		BOOL IS_AUDIO_SCENE_ACTIVE(const char* scene) {
			return Invoker::Invoke<BOOL, 3116>(scene);
		}

		void SET_AUDIO_SCENE_VARIABLE(const char* scene, const char* variable, float value) {
			Invoker::Invoke<void, 3117>(scene, variable, value);
		}

		void SET_AUDIO_SCRIPT_CLEANUP_TIME(int time) {
			Invoker::Invoke<void, 3118>(time);
		}

		void ADD_ENTITY_TO_AUDIO_MIX_GROUP(Entity entity, const char* groupName, float p2) {
			Invoker::Invoke<void, 3119>(entity, groupName, p2);
		}

		void REMOVE_ENTITY_FROM_AUDIO_MIX_GROUP(Entity entity, float p1) {
			Invoker::Invoke<void, 3120>(entity, p1);
		}

		BOOL AUDIO_IS_MUSIC_PLAYING() {
			return Invoker::Invoke<BOOL, 3121>();
		}

		BOOL AUDIO_IS_SCRIPTED_MUSIC_PLAYING() {
			return Invoker::Invoke<BOOL, 3122>();
		}

		BOOL PREPARE_MUSIC_EVENT(const char* eventName) {
			return Invoker::Invoke<BOOL, 3123>(eventName);
		}

		BOOL CANCEL_MUSIC_EVENT(const char* eventName) {
			return Invoker::Invoke<BOOL, 3124>(eventName);
		}

		BOOL TRIGGER_MUSIC_EVENT(const char* eventName) {
			return Invoker::Invoke<BOOL, 3125>(eventName);
		}

		BOOL IS_MUSIC_ONESHOT_PLAYING() {
			return Invoker::Invoke<BOOL, 3126>();
		}

		int GET_MUSIC_PLAYTIME() {
			return Invoker::Invoke<int, 3127>();
		}

		void SET_GLOBAL_RADIO_SIGNAL_LEVEL(Any p0) {
			Invoker::Invoke<void, 3128>(p0);
		}

		void RECORD_BROKEN_GLASS(float x, float y, float z, float radius) {
			Invoker::Invoke<void, 3129>(x, y, z, radius);
		}

		void CLEAR_ALL_BROKEN_GLASS() {
			Invoker::Invoke<void, 3130>();
		}

		void SCRIPT_OVERRIDES_WIND_ELEVATION(BOOL p0, Any p1) {
			Invoker::Invoke<void, 3131>(p0, p1);
		}

		void SET_PED_WALLA_DENSITY(float p0, float p1) {
			Invoker::Invoke<void, 3132>(p0, p1);
		}

		void SET_PED_INTERIOR_WALLA_DENSITY(float p0, float p1) {
			Invoker::Invoke<void, 3133>(p0, p1);
		}

		void FORCE_PED_PANIC_WALLA() {
			Invoker::Invoke<void, 3134>();
		}

		BOOL PREPARE_ALARM(const char* alarmName) {
			return Invoker::Invoke<BOOL, 3135>(alarmName);
		}

		void START_ALARM(const char* alarmName, BOOL p2) {
			Invoker::Invoke<void, 3136>(alarmName, p2);
		}

		void STOP_ALARM(const char* alarmName, BOOL toggle) {
			Invoker::Invoke<void, 3137>(alarmName, toggle);
		}

		void STOP_ALL_ALARMS(BOOL stop) {
			Invoker::Invoke<void, 3138>(stop);
		}

		BOOL IS_ALARM_PLAYING(const char* alarmName) {
			return Invoker::Invoke<BOOL, 3139>(alarmName);
		}

		Hash GET_VEHICLE_DEFAULT_HORN(Vehicle vehicle) {
			return Invoker::Invoke<Hash, 3140>(vehicle);
		}

		Hash GET_VEHICLE_DEFAULT_HORN_IGNORE_MODS(Vehicle vehicle) {
			return Invoker::Invoke<Hash, 3141>(vehicle);
		}

		void RESET_PED_AUDIO_FLAGS(Ped ped) {
			Invoker::Invoke<void, 3142>(ped);
		}

		void SET_PED_FOOTSTEPS_EVENTS_ENABLED(Ped ped, BOOL toggle) {
			Invoker::Invoke<void, 3143>(ped, toggle);
		}

		void SET_PED_CLOTH_EVENTS_ENABLED(Ped ped, BOOL toggle) {
			Invoker::Invoke<void, 3144>(ped, toggle);
		}

		void OVERRIDE_PLAYER_GROUND_MATERIAL(Hash hash, BOOL toggle) {
			Invoker::Invoke<void, 3145>(hash, toggle);
		}

		void USE_FOOTSTEP_SCRIPT_SWEETENERS(Ped ped, BOOL p1, Hash hash) {
			Invoker::Invoke<void, 3146>(ped, p1, hash);
		}

		void OVERRIDE_MICROPHONE_SETTINGS(Hash hash, BOOL toggle) {
			Invoker::Invoke<void, 3147>(hash, toggle);
		}

		void FREEZE_MICROPHONE() {
			Invoker::Invoke<void, 3148>();
		}

		void DISTANT_COP_CAR_SIRENS(BOOL value) {
			Invoker::Invoke<void, 3149>(value);
		}

		void SET_SIREN_CAN_BE_CONTROLLED_BY_AUDIO(Vehicle vehicle, BOOL p1) {
			Invoker::Invoke<void, 3150>(vehicle, p1);
		}

		void ENABLE_STUNT_JUMP_AUDIO() {
			Invoker::Invoke<void, 3151>();
		}

		void SET_AUDIO_FLAG(const char* flagName, BOOL toggle) {
			Invoker::Invoke<void, 3152>(flagName, toggle);
		}

		BOOL PREPARE_SYNCHRONIZED_AUDIO_EVENT(const char* audioEvent, Any p1) {
			return Invoker::Invoke<BOOL, 3153>(audioEvent, p1);
		}

		BOOL PREPARE_SYNCHRONIZED_AUDIO_EVENT_FOR_SCENE(int sceneID, const char* audioEvent) {
			return Invoker::Invoke<BOOL, 3154>(sceneID, audioEvent);
		}

		BOOL PLAY_SYNCHRONIZED_AUDIO_EVENT(int sceneID) {
			return Invoker::Invoke<BOOL, 3155>(sceneID);
		}

		BOOL STOP_SYNCHRONIZED_AUDIO_EVENT(int sceneID) {
			return Invoker::Invoke<BOOL, 3156>(sceneID);
		}

		void INIT_SYNCH_SCENE_AUDIO_WITH_POSITION(const char* audioEvent, float x, float y, float z) {
			Invoker::Invoke<void, 3157>(audioEvent, x, y, z);
		}

		void INIT_SYNCH_SCENE_AUDIO_WITH_ENTITY(const char* audioEvent, Entity entity) {
			Invoker::Invoke<void, 3158>(audioEvent, entity);
		}

		void SET_AUDIO_SPECIAL_EFFECT_MODE(int mode) {
			Invoker::Invoke<void, 3159>(mode);
		}

		void SET_PORTAL_SETTINGS_OVERRIDE(const char* p0, const char* p1) {
			Invoker::Invoke<void, 3160>(p0, p1);
		}

		void SET_INDIVIDUAL_PORTAL_SETTINGS_OVERRIDE(Hash interiorNameHash, int roomIndex, int doorIndex, const char* newPortalSettingsName) {
			Invoker::Invoke<void, 6702>(interiorNameHash, roomIndex, doorIndex, newPortalSettingsName);
		}

		void REMOVE_PORTAL_SETTINGS_OVERRIDE(const char* p0) {
			Invoker::Invoke<void, 3161>(p0);
		}

		void REMOVE_INDIVIDUAL_PORTAL_SETTINGS_OVERRIDE(Hash interiorNameHash, int roomIndex, int doorIndex) {
			Invoker::Invoke<void, 6701>(interiorNameHash, roomIndex, doorIndex);
		}

		void STOP_SMOKE_GRENADE_EXPLOSION_SOUNDS() {
			Invoker::Invoke<void, 3162>();
		}

		int GET_MUSIC_VOL_SLIDER() {
			return Invoker::Invoke<int, 3163>();
		}

		void REQUEST_TENNIS_BANKS(Ped ped) {
			Invoker::Invoke<void, 3164>(ped);
		}

		void UNREQUEST_TENNIS_BANKS() {
			Invoker::Invoke<void, 3165>();
		}

		void SET_SKIP_MINIGUN_SPIN_UP_AUDIO(BOOL p0) {
			Invoker::Invoke<void, 3166>(p0);
		}

		void STOP_CUTSCENE_AUDIO() {
			Invoker::Invoke<void, 3167>();
		}

		BOOL HAS_LOADED_MP_DATA_SET() {
			return Invoker::Invoke<BOOL, 3168>();
		}

		BOOL HAS_LOADED_SP_DATA_SET() {
			return Invoker::Invoke<BOOL, 3169>();
		}

		int GET_VEHICLE_HORN_SOUND_INDEX(Vehicle vehicle) {
			return Invoker::Invoke<int, 3170>(vehicle);
		}

		void SET_VEHICLE_HORN_SOUND_INDEX(Vehicle vehicle, int value) {
			Invoker::Invoke<void, 3171>(vehicle, value);
		}

	} // namespace AUDIO

	namespace BRAIN {

		void ADD_SCRIPT_TO_RANDOM_PED(const char* name, Hash model, float p2, float p3) {
			Invoker::Invoke<void, 266>(name, model, p2, p3);
		}

		void REGISTER_OBJECT_SCRIPT_BRAIN(const char* scriptName, Hash modelHash, int p2, float activationRange, int p4, int p5) {
			Invoker::Invoke<void, 267>(scriptName, modelHash, p2, activationRange, p4, p5);
		}

		BOOL IS_OBJECT_WITHIN_BRAIN_ACTIVATION_RANGE(Object object) {
			return Invoker::Invoke<BOOL, 268>(object);
		}

		void REGISTER_WORLD_POINT_SCRIPT_BRAIN(const char* scriptName, float activationRange, int p2) {
			Invoker::Invoke<void, 269>(scriptName, activationRange, p2);
		}

		BOOL IS_WORLD_POINT_WITHIN_BRAIN_ACTIVATION_RANGE() {
			return Invoker::Invoke<BOOL, 270>();
		}

		void ENABLE_SCRIPT_BRAIN_SET(int brainSet) {
			Invoker::Invoke<void, 271>(brainSet);
		}

		void DISABLE_SCRIPT_BRAIN_SET(int brainSet) {
			Invoker::Invoke<void, 272>(brainSet);
		}

		void REACTIVATE_ALL_WORLD_BRAINS_THAT_ARE_WAITING_TILL_OUT_OF_RANGE() {
			Invoker::Invoke<void, 273>();
		}

		void REACTIVATE_ALL_OBJECT_BRAINS_THAT_ARE_WAITING_TILL_OUT_OF_RANGE() {
			Invoker::Invoke<void, 274>();
		}

		void REACTIVATE_NAMED_WORLD_BRAINS_WAITING_TILL_OUT_OF_RANGE(const char* scriptName) {
			Invoker::Invoke<void, 275>(scriptName);
		}

		void REACTIVATE_NAMED_OBJECT_BRAINS_WAITING_TILL_OUT_OF_RANGE(const char* scriptName) {
			Invoker::Invoke<void, 276>(scriptName);
		}

	} // namespace BRAIN

	namespace CAM {

		void RENDER_SCRIPT_CAMS(BOOL render, BOOL ease, int easeTime, BOOL p3, BOOL p4, Any p5) {
			Invoker::Invoke<void, 3789>(render, ease, easeTime, p3, p4, p5);
		}

		void STOP_RENDERING_SCRIPT_CAMS_USING_CATCH_UP(BOOL render, float p1, int p2, Any p3) {
			Invoker::Invoke<void, 3790>(render, p1, p2, p3);
		}

		Cam CREATE_CAM(const char* camName, BOOL p1) {
			return Invoker::Invoke<Cam, 3791>(camName, p1);
		}

		Cam CREATE_CAM_WITH_PARAMS(const char* camName, float posX, float posY, float posZ, float rotX, float rotY, float rotZ, float fov, BOOL p8, int p9) {
			return Invoker::Invoke<Cam, 3792>(camName, posX, posY, posZ, rotX, rotY, rotZ, fov, p8, p9);
		}

		Cam CREATE_CAMERA(Hash camHash, BOOL p1) {
			return Invoker::Invoke<Cam, 3793>(camHash, p1);
		}

		Cam CREATE_CAMERA_WITH_PARAMS(Hash camHash, float posX, float posY, float posZ, float rotX, float rotY, float rotZ, float fov, BOOL p8, Any p9) {
			return Invoker::Invoke<Cam, 3794>(camHash, posX, posY, posZ, rotX, rotY, rotZ, fov, p8, p9);
		}

		void DESTROY_CAM(Cam cam, BOOL bScriptHostCam) {
			Invoker::Invoke<void, 3795>(cam, bScriptHostCam);
		}

		void DESTROY_ALL_CAMS(BOOL bScriptHostCam) {
			Invoker::Invoke<void, 3796>(bScriptHostCam);
		}

		BOOL DOES_CAM_EXIST(Cam cam) {
			return Invoker::Invoke<BOOL, 3797>(cam);
		}

		void SET_CAM_ACTIVE(Cam cam, BOOL active) {
			Invoker::Invoke<void, 3798>(cam, active);
		}

		BOOL IS_CAM_ACTIVE(Cam cam) {
			return Invoker::Invoke<BOOL, 3799>(cam);
		}

		BOOL IS_CAM_RENDERING(Cam cam) {
			return Invoker::Invoke<BOOL, 3800>(cam);
		}

		Cam GET_RENDERING_CAM() {
			return Invoker::Invoke<Cam, 3801>();
		}

		Vector3 GET_CAM_COORD(Cam cam) {
			return Invoker::Invoke<Vector3, 3802>(cam);
		}

		Vector3 GET_CAM_ROT(Cam cam, int rotationOrder) {
			return Invoker::Invoke<Vector3, 3803>(cam, rotationOrder);
		}

		float GET_CAM_FOV(Cam cam) {
			return Invoker::Invoke<float, 3804>(cam);
		}

		float GET_CAM_NEAR_CLIP(Cam cam) {
			return Invoker::Invoke<float, 3805>(cam);
		}

		float GET_CAM_FAR_CLIP(Cam cam) {
			return Invoker::Invoke<float, 3806>(cam);
		}

		float GET_CAM_NEAR_DOF(Cam cam) {
			return Invoker::Invoke<float, 3807>(cam);
		}

		float GET_CAM_FAR_DOF(Cam cam) {
			return Invoker::Invoke<float, 3808>(cam);
		}

		float GET_CAM_DOF_STRENGTH(Cam cam) {
			return Invoker::Invoke<float, 3809>(cam);
		}

		void SET_CAM_PARAMS(Cam cam, float posX, float posY, float posZ, float rotX, float rotY, float rotZ, float fieldOfView, Any p8, int p9, int p10, int p11) {
			Invoker::Invoke<void, 3810>(cam, posX, posY, posZ, rotX, rotY, rotZ, fieldOfView, p8, p9, p10, p11);
		}

		void SET_CAM_COORD(Cam cam, float posX, float posY, float posZ) {
			Invoker::Invoke<void, 3811>(cam, posX, posY, posZ);
		}

		void SET_CAM_ROT(Cam cam, float rotX, float rotY, float rotZ, int rotationOrder) {
			Invoker::Invoke<void, 3812>(cam, rotX, rotY, rotZ, rotationOrder);
		}

		void SET_CAM_FOV(Cam cam, float fieldOfView) {
			Invoker::Invoke<void, 3813>(cam, fieldOfView);
		}

		void SET_CAM_NEAR_CLIP(Cam cam, float nearClip) {
			Invoker::Invoke<void, 3814>(cam, nearClip);
		}

		void SET_CAM_FAR_CLIP(Cam cam, float farClip) {
			Invoker::Invoke<void, 3815>(cam, farClip);
		}

		void FORCE_CAM_FAR_CLIP(Cam cam, float p1) {
			Invoker::Invoke<void, 3816>(cam, p1);
		}

		void SET_CAM_MOTION_BLUR_STRENGTH(Cam cam, float strength) {
			Invoker::Invoke<void, 3817>(cam, strength);
		}

		void SET_CAM_NEAR_DOF(Cam cam, float nearDOF) {
			Invoker::Invoke<void, 3818>(cam, nearDOF);
		}

		void SET_CAM_FAR_DOF(Cam cam, float farDOF) {
			Invoker::Invoke<void, 3819>(cam, farDOF);
		}

		void SET_CAM_DOF_STRENGTH(Cam cam, float dofStrength) {
			Invoker::Invoke<void, 3820>(cam, dofStrength);
		}

		void SET_CAM_DOF_PLANES(Cam cam, float p1, float p2, float p3, float p4) {
			Invoker::Invoke<void, 3821>(cam, p1, p2, p3, p4);
		}

		void SET_CAM_USE_SHALLOW_DOF_MODE(Cam cam, BOOL toggle) {
			Invoker::Invoke<void, 3822>(cam, toggle);
		}

		void SET_USE_HI_DOF() {
			Invoker::Invoke<void, 3823>();
		}

		void SET_USE_HI_DOF_ON_SYNCED_SCENE_THIS_UPDATE() {
			Invoker::Invoke<void, 3824>();
		}

		void SET_CAM_DOF_OVERRIDDEN_FOCUS_DISTANCE(Cam camera, float p1) {
			Invoker::Invoke<void, 3825>(camera, p1);
		}

		void SET_CAM_DOF_OVERRIDDEN_FOCUS_DISTANCE_BLEND_LEVEL(Any p0, float p1) {
			Invoker::Invoke<void, 3826>(p0, p1);
		}

		void SET_CAM_DOF_FNUMBER_OF_LENS(Cam camera, float p1) {
			Invoker::Invoke<void, 3827>(camera, p1);
		}

		void SET_CAM_DOF_FOCAL_LENGTH_MULTIPLIER(Cam camera, float multiplier) {
			Invoker::Invoke<void, 3828>(camera, multiplier);
		}

		void SET_CAM_DOF_FOCUS_DISTANCE_BIAS(Cam camera, float p1) {
			Invoker::Invoke<void, 3829>(camera, p1);
		}

		void SET_CAM_DOF_MAX_NEAR_IN_FOCUS_DISTANCE(Cam camera, float p1) {
			Invoker::Invoke<void, 3830>(camera, p1);
		}

		void SET_CAM_DOF_MAX_NEAR_IN_FOCUS_DISTANCE_BLEND_LEVEL(Cam camera, float p1) {
			Invoker::Invoke<void, 3831>(camera, p1);
		}

		void SET_CAM_DOF_SHOULD_KEEP_LOOK_AT_TARGET_IN_FOCUS(Cam camera, BOOL state) {
			Invoker::Invoke<void, 3832>(camera, state);
		}

		void ATTACH_CAM_TO_ENTITY(Cam cam, Entity entity, float xOffset, float yOffset, float zOffset, BOOL isRelative) {
			Invoker::Invoke<void, 3833>(cam, entity, xOffset, yOffset, zOffset, isRelative);
		}

		void ATTACH_CAM_TO_PED_BONE(Cam cam, Ped ped, int boneIndex, float x, float y, float z, BOOL heading) {
			Invoker::Invoke<void, 3834>(cam, ped, boneIndex, x, y, z, heading);
		}

		void HARD_ATTACH_CAM_TO_PED_BONE(Cam cam, Ped ped, int boneIndex, float p3, float p4, float p5, float p6, float p7, float p8, BOOL p9) {
			Invoker::Invoke<void, 3835>(cam, ped, boneIndex, p3, p4, p5, p6, p7, p8, p9);
		}

		void HARD_ATTACH_CAM_TO_ENTITY(Cam cam, Entity entity, float xRot, float yRot, float zRot, float xOffset, float yOffset, float zOffset, BOOL isRelative) {
			Invoker::Invoke<void, 3836>(cam, entity, xRot, yRot, zRot, xOffset, yOffset, zOffset, isRelative);
		}

		void ATTACH_CAM_TO_VEHICLE_BONE(Cam cam, Vehicle vehicle, int boneIndex, BOOL relativeRotation, float rotX, float rotY, float rotZ, float offsetX, float offsetY, float offsetZ, BOOL fixedDirection) {
			Invoker::Invoke<void, 3837>(cam, vehicle, boneIndex, relativeRotation, rotX, rotY, rotZ, offsetX, offsetY, offsetZ, fixedDirection);
		}

		void DETACH_CAM(Cam cam) {
			Invoker::Invoke<void, 3838>(cam);
		}

		void SET_CAM_INHERIT_ROLL_VEHICLE(Cam cam, BOOL p1) {
			Invoker::Invoke<void, 3839>(cam, p1);
		}

		void POINT_CAM_AT_COORD(Cam cam, float x, float y, float z) {
			Invoker::Invoke<void, 3840>(cam, x, y, z);
		}

		void POINT_CAM_AT_ENTITY(Cam cam, Entity entity, float p2, float p3, float p4, BOOL p5) {
			Invoker::Invoke<void, 3841>(cam, entity, p2, p3, p4, p5);
		}

		void POINT_CAM_AT_PED_BONE(Cam cam, Ped ped, int boneIndex, float x, float y, float z, BOOL p6) {
			Invoker::Invoke<void, 3842>(cam, ped, boneIndex, x, y, z, p6);
		}

		void STOP_CAM_POINTING(Cam cam) {
			Invoker::Invoke<void, 3843>(cam);
		}

		void SET_CAM_AFFECTS_AIMING(Cam cam, BOOL toggle) {
			Invoker::Invoke<void, 3844>(cam, toggle);
		}

		void SET_CAM_CONTROLS_MINI_MAP_HEADING(Cam cam, BOOL toggle) {
			Invoker::Invoke<void, 3845>(cam, toggle);
		}

		void SET_CAM_IS_INSIDE_VEHICLE(Cam cam, BOOL toggle) {
			Invoker::Invoke<void, 3846>(cam, toggle);
		}

		void ALLOW_MOTION_BLUR_DECAY(Any p0, BOOL p1) {
			Invoker::Invoke<void, 3847>(p0, p1);
		}

		void SET_CAM_DEBUG_NAME(Cam camera, const char* name) {
			Invoker::Invoke<void, 3848>(camera, name);
		}

		Cam GET_DEBUG_CAM() {
			return Invoker::Invoke<Cam, 3849>();
		}

		void ADD_CAM_SPLINE_NODE(Cam camera, float x, float y, float z, float xRot, float yRot, float zRot, int length, int smoothingStyle, int rotationOrder) {
			Invoker::Invoke<void, 3850>(camera, x, y, z, xRot, yRot, zRot, length, smoothingStyle, rotationOrder);
		}

		void ADD_CAM_SPLINE_NODE_USING_CAMERA_FRAME(Cam cam, Cam cam2, int length, int p3) {
			Invoker::Invoke<void, 3851>(cam, cam2, length, p3);
		}

		void ADD_CAM_SPLINE_NODE_USING_CAMERA(Cam cam, Cam cam2, int length, int p3) {
			Invoker::Invoke<void, 3852>(cam, cam2, length, p3);
		}

		void ADD_CAM_SPLINE_NODE_USING_GAMEPLAY_FRAME(Cam cam, int length, int p2) {
			Invoker::Invoke<void, 3853>(cam, length, p2);
		}

		void SET_CAM_SPLINE_PHASE(Cam cam, float p1) {
			Invoker::Invoke<void, 3854>(cam, p1);
		}

		float GET_CAM_SPLINE_PHASE(Cam cam) {
			return Invoker::Invoke<float, 3855>(cam);
		}

		float GET_CAM_SPLINE_NODE_PHASE(Cam cam) {
			return Invoker::Invoke<float, 3856>(cam);
		}

		void SET_CAM_SPLINE_DURATION(Cam cam, int timeDuration) {
			Invoker::Invoke<void, 3857>(cam, timeDuration);
		}

		void SET_CAM_SPLINE_SMOOTHING_STYLE(Cam cam, int smoothingStyle) {
			Invoker::Invoke<void, 3858>(cam, smoothingStyle);
		}

		int GET_CAM_SPLINE_NODE_INDEX(Cam cam) {
			return Invoker::Invoke<int, 3859>(cam);
		}

		void SET_CAM_SPLINE_NODE_EASE(Cam cam, int easingFunction, int p2, float p3) {
			Invoker::Invoke<void, 3860>(cam, easingFunction, p2, p3);
		}

		void SET_CAM_SPLINE_NODE_VELOCITY_SCALE(Cam cam, int p1, float scale) {
			Invoker::Invoke<void, 3861>(cam, p1, scale);
		}

		void OVERRIDE_CAM_SPLINE_VELOCITY(Cam cam, int p1, float p2, float p3) {
			Invoker::Invoke<void, 3862>(cam, p1, p2, p3);
		}

		void OVERRIDE_CAM_SPLINE_MOTION_BLUR(Cam cam, int p1, float p2, float p3) {
			Invoker::Invoke<void, 3863>(cam, p1, p2, p3);
		}

		void SET_CAM_SPLINE_NODE_EXTRA_FLAGS(Cam cam, int p1, int flags) {
			Invoker::Invoke<void, 3864>(cam, p1, flags);
		}

		BOOL IS_CAM_SPLINE_PAUSED(Cam cam) {
			return Invoker::Invoke<BOOL, 3865>(cam);
		}

		void _INTERPOLATE_CAM_WITH_PARAMS(Cam camera, float camPosX, float camPosY, float camPosZ, float camRotX, float camRotY, float camRotZ, float fov, int duration, int posCurveType, int rotCurveType, int rotOrder, int fovCurveType) {
			Invoker::Invoke<void, 6569>(camera, camPosX, camPosY, camPosZ, camRotX, camRotY, camRotZ, fov, duration, posCurveType, rotCurveType, rotOrder, fovCurveType);
		}

		void _ACTIVATE_CAM_WITH_INTERP_AND_FOV_CURVE(Cam camTo, Cam camFrom, int duration, int easeLocation, int easeRotation, int easeFov) {
			Invoker::Invoke<void, 6570>(camTo, camFrom, duration, easeLocation, easeRotation, easeFov);
		}

		void SET_CAM_ACTIVE_WITH_INTERP(Cam camTo, Cam camFrom, int duration, int easeLocation, int easeRotation) {
			Invoker::Invoke<void, 3866>(camTo, camFrom, duration, easeLocation, easeRotation);
		}

		BOOL IS_CAM_INTERPOLATING(Cam cam) {
			return Invoker::Invoke<BOOL, 3867>(cam);
		}

		void SHAKE_CAM(Cam cam, const char* type, float amplitude) {
			Invoker::Invoke<void, 3868>(cam, type, amplitude);
		}

		void ANIMATED_SHAKE_CAM(Cam cam, const char* p1, const char* p2, const char* p3, float amplitude) {
			Invoker::Invoke<void, 3869>(cam, p1, p2, p3, amplitude);
		}

		BOOL IS_CAM_SHAKING(Cam cam) {
			return Invoker::Invoke<BOOL, 3870>(cam);
		}

		void SET_CAM_SHAKE_AMPLITUDE(Cam cam, float amplitude) {
			Invoker::Invoke<void, 3871>(cam, amplitude);
		}

		void STOP_CAM_SHAKING(Cam cam, BOOL p1) {
			Invoker::Invoke<void, 3872>(cam, p1);
		}

		void SHAKE_SCRIPT_GLOBAL(const char* p0, float p1) {
			Invoker::Invoke<void, 3873>(p0, p1);
		}

		void ANIMATED_SHAKE_SCRIPT_GLOBAL(const char* p0, const char* p1, const char* p2, float p3) {
			Invoker::Invoke<void, 3874>(p0, p1, p2, p3);
		}

		BOOL IS_SCRIPT_GLOBAL_SHAKING() {
			return Invoker::Invoke<BOOL, 3875>();
		}

		void STOP_SCRIPT_GLOBAL_SHAKING(BOOL p0) {
			Invoker::Invoke<void, 3876>(p0);
		}

		void TRIGGER_VEHICLE_PART_BROKEN_CAMERA_SHAKE(Vehicle vehicle, int p1, float p2) {
			Invoker::Invoke<void, 3877>(vehicle, p1, p2);
		}

		BOOL PLAY_CAM_ANIM(Cam cam, const char* animName, const char* animDictionary, float x, float y, float z, float xRot, float yRot, float zRot, BOOL p9, int p10) {
			return Invoker::Invoke<BOOL, 3878>(cam, animName, animDictionary, x, y, z, xRot, yRot, zRot, p9, p10);
		}

		BOOL IS_CAM_PLAYING_ANIM(Cam cam, const char* animName, const char* animDictionary) {
			return Invoker::Invoke<BOOL, 3879>(cam, animName, animDictionary);
		}

		void SET_CAM_ANIM_CURRENT_PHASE(Cam cam, float phase) {
			Invoker::Invoke<void, 3880>(cam, phase);
		}

		float GET_CAM_ANIM_CURRENT_PHASE(Cam cam) {
			return Invoker::Invoke<float, 3881>(cam);
		}

		BOOL PLAY_SYNCHRONIZED_CAM_ANIM(Any p0, Any p1, const char* animName, const char* animDictionary) {
			return Invoker::Invoke<BOOL, 3882>(p0, p1, animName, animDictionary);
		}

		void SET_FLY_CAM_HORIZONTAL_RESPONSE(Cam cam, float p1, float p2, float p3) {
			Invoker::Invoke<void, 3883>(cam, p1, p2, p3);
		}

		void SET_FLY_CAM_VERTICAL_RESPONSE(Cam cam, float p1, float p2, float p3) {
			Invoker::Invoke<void, 3884>(cam, p1, p2, p3);
		}

		void SET_FLY_CAM_MAX_HEIGHT(Cam cam, float height) {
			Invoker::Invoke<void, 3885>(cam, height);
		}

		void SET_FLY_CAM_COORD_AND_CONSTRAIN(Cam cam, float x, float y, float z) {
			Invoker::Invoke<void, 3886>(cam, x, y, z);
		}

		void SET_FLY_CAM_VERTICAL_CONTROLS_THIS_UPDATE(Cam cam) {
			Invoker::Invoke<void, 3887>(cam);
		}

		BOOL WAS_FLY_CAM_CONSTRAINED_ON_PREVIOUS_UDPATE(Cam cam) {
			return Invoker::Invoke<BOOL, 3888>(cam);
		}

		BOOL IS_SCREEN_FADED_OUT() {
			return Invoker::Invoke<BOOL, 3889>();
		}

		BOOL IS_SCREEN_FADED_IN() {
			return Invoker::Invoke<BOOL, 3890>();
		}

		BOOL IS_SCREEN_FADING_OUT() {
			return Invoker::Invoke<BOOL, 3891>();
		}

		BOOL IS_SCREEN_FADING_IN() {
			return Invoker::Invoke<BOOL, 3892>();
		}

		void DO_SCREEN_FADE_IN(int duration) {
			Invoker::Invoke<void, 3893>(duration);
		}

		void DO_SCREEN_FADE_OUT(int duration) {
			Invoker::Invoke<void, 3894>(duration);
		}

		void SET_WIDESCREEN_BORDERS(BOOL p0, int p1) {
			Invoker::Invoke<void, 3895>(p0, p1);
		}

		BOOL ARE_WIDESCREEN_BORDERS_ACTIVE() {
			return Invoker::Invoke<BOOL, 3896>();
		}

		Vector3 GET_GAMEPLAY_CAM_COORD() {
			return Invoker::Invoke<Vector3, 3897>();
		}

		Vector3 GET_GAMEPLAY_CAM_ROT(int rotationOrder) {
			return Invoker::Invoke<Vector3, 3898>(rotationOrder);
		}

		float GET_GAMEPLAY_CAM_FOV() {
			return Invoker::Invoke<float, 3899>();
		}

		void SET_GAMEPLAY_CAM_MOTION_BLUR_SCALING_THIS_UPDATE(float p0) {
			Invoker::Invoke<void, 3900>(p0);
		}

		void SET_GAMEPLAY_CAM_MAX_MOTION_BLUR_STRENGTH_THIS_UPDATE(float p0) {
			Invoker::Invoke<void, 3901>(p0);
		}

		float GET_GAMEPLAY_CAM_RELATIVE_HEADING() {
			return Invoker::Invoke<float, 3902>();
		}

		void SET_GAMEPLAY_CAM_RELATIVE_HEADING(float heading) {
			Invoker::Invoke<void, 3903>(heading);
		}

		float GET_GAMEPLAY_CAM_RELATIVE_PITCH() {
			return Invoker::Invoke<float, 3904>();
		}

		void SET_GAMEPLAY_CAM_RELATIVE_PITCH(float angle, float scalingFactor) {
			Invoker::Invoke<void, 3905>(angle, scalingFactor);
		}

		void RESET_GAMEPLAY_CAM_FULL_ATTACH_PARENT_TRANSFORM_TIMER() {
			Invoker::Invoke<void, 3906>();
		}

		void FORCE_CAMERA_RELATIVE_HEADING_AND_PITCH(float roll, float pitch, float yaw) {
			Invoker::Invoke<void, 3907>(roll, pitch, yaw);
		}

		void FORCE_BONNET_CAMERA_RELATIVE_HEADING_AND_PITCH(float p0, float p1) {
			Invoker::Invoke<void, 3908>(p0, p1);
		}

		void SET_FIRST_PERSON_SHOOTER_CAMERA_HEADING(float yaw) {
			Invoker::Invoke<void, 3909>(yaw);
		}

		void SET_FIRST_PERSON_SHOOTER_CAMERA_PITCH(float pitch) {
			Invoker::Invoke<void, 3910>(pitch);
		}

		void SET_SCRIPTED_CAMERA_IS_FIRST_PERSON_THIS_FRAME(BOOL p0) {
			Invoker::Invoke<void, 3911>(p0);
		}

		void SHAKE_GAMEPLAY_CAM(const char* shakeName, float intensity) {
			Invoker::Invoke<void, 3912>(shakeName, intensity);
		}

		BOOL IS_GAMEPLAY_CAM_SHAKING() {
			return Invoker::Invoke<BOOL, 3913>();
		}

		void SET_GAMEPLAY_CAM_SHAKE_AMPLITUDE(float amplitude) {
			Invoker::Invoke<void, 3914>(amplitude);
		}

		void STOP_GAMEPLAY_CAM_SHAKING(BOOL p0) {
			Invoker::Invoke<void, 3915>(p0);
		}

		void SET_GAMEPLAY_CAM_FOLLOW_PED_THIS_UPDATE(Ped ped) {
			Invoker::Invoke<void, 3916>(ped);
		}

		BOOL IS_GAMEPLAY_CAM_RENDERING() {
			return Invoker::Invoke<BOOL, 3917>();
		}

		BOOL IS_INTERPOLATING_FROM_SCRIPT_CAMS() {
			return Invoker::Invoke<BOOL, 3918>();
		}

		BOOL IS_INTERPOLATING_TO_SCRIPT_CAMS() {
			return Invoker::Invoke<BOOL, 3919>();
		}

		void SET_GAMEPLAY_CAM_ALTITUDE_FOV_SCALING_STATE(BOOL p0) {
			Invoker::Invoke<void, 3920>(p0);
		}

		void DISABLE_GAMEPLAY_CAM_ALTITUDE_FOV_SCALING_THIS_UPDATE() {
			Invoker::Invoke<void, 3921>();
		}

		BOOL IS_GAMEPLAY_CAM_LOOKING_BEHIND() {
			return Invoker::Invoke<BOOL, 3922>();
		}

		void SET_GAMEPLAY_CAM_IGNORE_ENTITY_COLLISION_THIS_UPDATE(Entity entity) {
			Invoker::Invoke<void, 3923>(entity);
		}

		void DISABLE_CAM_COLLISION_FOR_OBJECT(Entity entity) {
			Invoker::Invoke<void, 3924>(entity);
		}

		void BYPASS_CAMERA_COLLISION_BUOYANCY_TEST_THIS_UPDATE() {
			Invoker::Invoke<void, 3925>();
		}

		void SET_GAMEPLAY_CAM_ENTITY_TO_LIMIT_FOCUS_OVER_BOUNDING_SPHERE_THIS_UPDATE(Entity entity) {
			Invoker::Invoke<void, 3926>(entity);
		}

		void DISABLE_FIRST_PERSON_CAMERA_WATER_CLIPPING_TEST_THIS_UPDATE() {
			Invoker::Invoke<void, 3927>();
		}

		void SET_FOLLOW_CAM_IGNORE_ATTACH_PARENT_MOVEMENT_THIS_UPDATE() {
			Invoker::Invoke<void, 3928>();
		}

		BOOL IS_SPHERE_VISIBLE(float x, float y, float z, float radius) {
			return Invoker::Invoke<BOOL, 3929>(x, y, z, radius);
		}

		BOOL IS_FOLLOW_PED_CAM_ACTIVE() {
			return Invoker::Invoke<BOOL, 3930>();
		}

		BOOL SET_FOLLOW_PED_CAM_THIS_UPDATE(const char* camName, int p1) {
			return Invoker::Invoke<BOOL, 3931>(camName, p1);
		}

		void USE_SCRIPT_CAM_FOR_AMBIENT_POPULATION_ORIGIN_THIS_FRAME(BOOL p0, BOOL p1) {
			Invoker::Invoke<void, 3932>(p0, p1);
		}

		void SET_FOLLOW_PED_CAM_LADDER_ALIGN_THIS_UPDATE() {
			Invoker::Invoke<void, 3933>();
		}

		void SET_THIRD_PERSON_CAM_RELATIVE_HEADING_LIMITS_THIS_UPDATE(float minimum, float maximum) {
			Invoker::Invoke<void, 3934>(minimum, maximum);
		}

		void SET_THIRD_PERSON_CAM_RELATIVE_PITCH_LIMITS_THIS_UPDATE(float minimum, float maximum) {
			Invoker::Invoke<void, 3935>(minimum, maximum);
		}

		void SET_THIRD_PERSON_CAM_ORBIT_DISTANCE_LIMITS_THIS_UPDATE(float p0, float distance) {
			Invoker::Invoke<void, 3936>(p0, distance);
		}

		float _GET_THIRD_PERSON_CAM_MIN_ORBIT_DISTANCE_SPRING() {
			return Invoker::Invoke<float, 3937>();
		}

		float _GET_THIRD_PERSON_CAM_MAX_ORBIT_DISTANCE_SPRING() {
			return Invoker::Invoke<float, 3938>();
		}

		void SET_IN_VEHICLE_CAM_STATE_THIS_UPDATE(Vehicle p0, int p1) {
			Invoker::Invoke<void, 3939>(p0, p1);
		}

		void DISABLE_ON_FOOT_FIRST_PERSON_VIEW_THIS_UPDATE() {
			Invoker::Invoke<void, 3940>();
		}

		void DISABLE_FIRST_PERSON_FLASH_EFFECT_THIS_UPDATE() {
			Invoker::Invoke<void, 3941>();
		}

		void BLOCK_FIRST_PERSON_ORIENTATION_RESET_THIS_UPDATE() {
			Invoker::Invoke<void, 3942>();
		}

		int GET_FOLLOW_PED_CAM_ZOOM_LEVEL() {
			return Invoker::Invoke<int, 3943>();
		}

		int GET_FOLLOW_PED_CAM_VIEW_MODE() {
			return Invoker::Invoke<int, 3944>();
		}

		void SET_FOLLOW_PED_CAM_VIEW_MODE(int viewMode) {
			Invoker::Invoke<void, 3945>(viewMode);
		}

		BOOL IS_FOLLOW_VEHICLE_CAM_ACTIVE() {
			return Invoker::Invoke<BOOL, 3946>();
		}

		void SET_FOLLOW_VEHICLE_CAM_HIGH_ANGLE_MODE_THIS_UPDATE(BOOL p0) {
			Invoker::Invoke<void, 3947>(p0);
		}

		void SET_FOLLOW_VEHICLE_CAM_HIGH_ANGLE_MODE_EVERY_UPDATE(BOOL p0, BOOL p1) {
			Invoker::Invoke<void, 3948>(p0, p1);
		}

		BOOL SET_TABLE_GAMES_CAMERA_THIS_UPDATE(Hash hash) {
			return Invoker::Invoke<BOOL, 3949>(hash);
		}

		int GET_FOLLOW_VEHICLE_CAM_ZOOM_LEVEL() {
			return Invoker::Invoke<int, 3950>();
		}

		void SET_FOLLOW_VEHICLE_CAM_ZOOM_LEVEL(int zoomLevel) {
			Invoker::Invoke<void, 3951>(zoomLevel);
		}

		int GET_FOLLOW_VEHICLE_CAM_VIEW_MODE() {
			return Invoker::Invoke<int, 3952>();
		}

		void SET_FOLLOW_VEHICLE_CAM_VIEW_MODE(int viewMode) {
			Invoker::Invoke<void, 3953>(viewMode);
		}

		int GET_CAM_VIEW_MODE_FOR_CONTEXT(int context) {
			return Invoker::Invoke<int, 3954>(context);
		}

		void SET_CAM_VIEW_MODE_FOR_CONTEXT(int context, int viewMode) {
			Invoker::Invoke<void, 3955>(context, viewMode);
		}

		int GET_CAM_ACTIVE_VIEW_MODE_CONTEXT() {
			return Invoker::Invoke<int, 3956>();
		}

		void USE_VEHICLE_CAM_STUNT_SETTINGS_THIS_UPDATE() {
			Invoker::Invoke<void, 3957>();
		}

		void USE_DEDICATED_STUNT_CAMERA_THIS_UPDATE(const char* camName) {
			Invoker::Invoke<void, 3958>(camName);
		}

		void FORCE_VEHICLE_CAM_STUNT_SETTINGS_THIS_UPDATE() {
			Invoker::Invoke<void, 3959>();
		}

		void SET_FOLLOW_VEHICLE_CAM_SEAT_THIS_UPDATE(int seatIndex) {
			Invoker::Invoke<void, 3960>(seatIndex);
		}

		BOOL IS_AIM_CAM_ACTIVE() {
			return Invoker::Invoke<BOOL, 3961>();
		}

		BOOL IS_AIM_CAM_ACTIVE_IN_ACCURATE_MODE() {
			return Invoker::Invoke<BOOL, 3962>();
		}

		BOOL IS_FIRST_PERSON_AIM_CAM_ACTIVE() {
			return Invoker::Invoke<BOOL, 3963>();
		}

		void DISABLE_AIM_CAM_THIS_UPDATE() {
			Invoker::Invoke<void, 3964>();
		}

		float GET_FIRST_PERSON_AIM_CAM_ZOOM_FACTOR() {
			return Invoker::Invoke<float, 3965>();
		}

		void SET_FIRST_PERSON_AIM_CAM_ZOOM_FACTOR(float zoomFactor) {
			Invoker::Invoke<void, 3966>(zoomFactor);
		}

		void SET_FIRST_PERSON_AIM_CAM_ZOOM_FACTOR_LIMITS_THIS_UPDATE(float p0, float p1) {
			Invoker::Invoke<void, 3967>(p0, p1);
		}

		void SET_FIRST_PERSON_AIM_CAM_RELATIVE_HEADING_LIMITS_THIS_UPDATE(float p0, float p1) {
			Invoker::Invoke<void, 3968>(p0, p1);
		}

		void SET_FIRST_PERSON_AIM_CAM_RELATIVE_PITCH_LIMITS_THIS_UPDATE(float p0, float p1) {
			Invoker::Invoke<void, 3969>(p0, p1);
		}

		void SET_FIRST_PERSON_AIM_CAM_NEAR_CLIP_THIS_UPDATE(float p0) {
			Invoker::Invoke<void, 3970>(p0);
		}

		void SET_THIRD_PERSON_AIM_CAM_NEAR_CLIP_THIS_UPDATE(float p0) {
			Invoker::Invoke<void, 3971>(p0);
		}

		void SET_ALLOW_CUSTOM_VEHICLE_DRIVE_BY_CAM_THIS_UPDATE(BOOL p0) {
			Invoker::Invoke<void, 3972>(p0);
		}

		void FORCE_TIGHTSPACE_CUSTOM_FRAMING_THIS_UPDATE() {
			Invoker::Invoke<void, 3973>();
		}

		Vector3 GET_FINAL_RENDERED_CAM_COORD() {
			return Invoker::Invoke<Vector3, 3974>();
		}

		Vector3 GET_FINAL_RENDERED_CAM_ROT(int rotationOrder) {
			return Invoker::Invoke<Vector3, 3975>(rotationOrder);
		}

		Vector3 GET_FINAL_RENDERED_REMOTE_PLAYER_CAM_ROT(Player player, int rotationOrder) {
			return Invoker::Invoke<Vector3, 3976>(player, rotationOrder);
		}

		float GET_FINAL_RENDERED_CAM_FOV() {
			return Invoker::Invoke<float, 3977>();
		}

		float GET_FINAL_RENDERED_REMOTE_PLAYER_CAM_FOV(Player player) {
			return Invoker::Invoke<float, 3978>(player);
		}

		float GET_FINAL_RENDERED_CAM_NEAR_CLIP() {
			return Invoker::Invoke<float, 3979>();
		}

		float GET_FINAL_RENDERED_CAM_FAR_CLIP() {
			return Invoker::Invoke<float, 3980>();
		}

		float GET_FINAL_RENDERED_CAM_NEAR_DOF() {
			return Invoker::Invoke<float, 3981>();
		}

		float GET_FINAL_RENDERED_CAM_FAR_DOF() {
			return Invoker::Invoke<float, 3982>();
		}

		float GET_FINAL_RENDERED_CAM_MOTION_BLUR_STRENGTH() {
			return Invoker::Invoke<float, 3983>();
		}

		void SET_GAMEPLAY_COORD_HINT(float x, float y, float z, int duration, int blendOutDuration, int blendInDuration, int p6) {
			Invoker::Invoke<void, 3984>(x, y, z, duration, blendOutDuration, blendInDuration, p6);
		}

		void SET_GAMEPLAY_PED_HINT(Ped ped, float x1, float y1, float z1, BOOL p4, int duration, int blendOutDuration, int blendInDuration) {
			Invoker::Invoke<void, 3985>(ped, x1, y1, z1, p4, duration, blendOutDuration, blendInDuration);
		}

		void SET_GAMEPLAY_VEHICLE_HINT(Vehicle vehicle, float offsetX, float offsetY, float offsetZ, BOOL p4, int time, int easeInTime, int easeOutTime) {
			Invoker::Invoke<void, 3986>(vehicle, offsetX, offsetY, offsetZ, p4, time, easeInTime, easeOutTime);
		}

		void SET_GAMEPLAY_OBJECT_HINT(Object object, float xOffset, float yOffset, float zOffset, BOOL p4, int time, int easeInTime, int easeOutTime) {
			Invoker::Invoke<void, 3987>(object, xOffset, yOffset, zOffset, p4, time, easeInTime, easeOutTime);
		}

		void SET_GAMEPLAY_ENTITY_HINT(Entity entity, float xOffset, float yOffset, float zOffset, BOOL p4, int time, int easeInTime, int easeOutTime, int p8) {
			Invoker::Invoke<void, 3988>(entity, xOffset, yOffset, zOffset, p4, time, easeInTime, easeOutTime, p8);
		}

		BOOL IS_GAMEPLAY_HINT_ACTIVE() {
			return Invoker::Invoke<BOOL, 3989>();
		}

		void STOP_GAMEPLAY_HINT(BOOL p0) {
			Invoker::Invoke<void, 3990>(p0);
		}

		void STOP_GAMEPLAY_HINT_BEING_CANCELLED_THIS_UPDATE(BOOL p0) {
			Invoker::Invoke<void, 3991>(p0);
		}

		void STOP_CODE_GAMEPLAY_HINT(BOOL p0) {
			Invoker::Invoke<void, 3992>(p0);
		}

		BOOL IS_CODE_GAMEPLAY_HINT_ACTIVE() {
			return Invoker::Invoke<BOOL, 3993>();
		}

		void SET_GAMEPLAY_HINT_FOV(float FOV) {
			Invoker::Invoke<void, 3994>(FOV);
		}

		void SET_GAMEPLAY_HINT_FOLLOW_DISTANCE_SCALAR(float value) {
			Invoker::Invoke<void, 3995>(value);
		}

		void SET_GAMEPLAY_HINT_BASE_ORBIT_PITCH_OFFSET(float value) {
			Invoker::Invoke<void, 3996>(value);
		}

		void SET_GAMEPLAY_HINT_CAMERA_RELATIVE_SIDE_OFFSET(float xOffset) {
			Invoker::Invoke<void, 3997>(xOffset);
		}

		void SET_GAMEPLAY_HINT_CAMERA_RELATIVE_VERTICAL_OFFSET(float yOffset) {
			Invoker::Invoke<void, 3998>(yOffset);
		}

		void SET_GAMEPLAY_HINT_CAMERA_BLEND_TO_FOLLOW_PED_MEDIUM_VIEW_MODE(BOOL toggle) {
			Invoker::Invoke<void, 3999>(toggle);
		}

		void SET_CINEMATIC_BUTTON_ACTIVE(BOOL p0) {
			Invoker::Invoke<void, 4000>(p0);
		}

		BOOL IS_CINEMATIC_CAM_RENDERING() {
			return Invoker::Invoke<BOOL, 4001>();
		}

		void SHAKE_CINEMATIC_CAM(const char* shakeType, float amount) {
			Invoker::Invoke<void, 4002>(shakeType, amount);
		}

		BOOL IS_CINEMATIC_CAM_SHAKING() {
			return Invoker::Invoke<BOOL, 4003>();
		}

		void SET_CINEMATIC_CAM_SHAKE_AMPLITUDE(float p0) {
			Invoker::Invoke<void, 4004>(p0);
		}

		void STOP_CINEMATIC_CAM_SHAKING(BOOL p0) {
			Invoker::Invoke<void, 4005>(p0);
		}

		void DISABLE_CINEMATIC_BONNET_CAMERA_THIS_UPDATE() {
			Invoker::Invoke<void, 4006>();
		}

		void DISABLE_CINEMATIC_VEHICLE_IDLE_MODE_THIS_UPDATE() {
			Invoker::Invoke<void, 4007>();
		}

		void INVALIDATE_CINEMATIC_VEHICLE_IDLE_MODE() {
			Invoker::Invoke<void, 4008>();
		}

		void INVALIDATE_IDLE_CAM() {
			Invoker::Invoke<void, 4009>();
		}

		BOOL IS_CINEMATIC_IDLE_CAM_RENDERING() {
			return Invoker::Invoke<BOOL, 4010>();
		}

		BOOL IS_CINEMATIC_FIRST_PERSON_VEHICLE_INTERIOR_CAM_RENDERING() {
			return Invoker::Invoke<BOOL, 4011>();
		}

		void CREATE_CINEMATIC_SHOT(Hash p0, int time, BOOL p2, Entity entity) {
			Invoker::Invoke<void, 4012>(p0, time, p2, entity);
		}

		BOOL IS_CINEMATIC_SHOT_ACTIVE(Hash p0) {
			return Invoker::Invoke<BOOL, 4013>(p0);
		}

		void STOP_CINEMATIC_SHOT(Hash p0) {
			Invoker::Invoke<void, 4014>(p0);
		}

		void FORCE_CINEMATIC_RENDERING_THIS_UPDATE(BOOL toggle) {
			Invoker::Invoke<void, 4015>(toggle);
		}

		void SET_CINEMATIC_NEWS_CHANNEL_ACTIVE_THIS_UPDATE() {
			Invoker::Invoke<void, 4016>();
		}

		void SET_CINEMATIC_MODE_ACTIVE(BOOL toggle) {
			Invoker::Invoke<void, 4017>(toggle);
		}

		BOOL IS_IN_VEHICLE_MOBILE_PHONE_CAMERA_RENDERING() {
			return Invoker::Invoke<BOOL, 4018>();
		}

		BOOL DISABLE_CINEMATIC_SLOW_MO_THIS_UPDATE() {
			return Invoker::Invoke<BOOL, 4019>();
		}

		BOOL IS_BONNET_CINEMATIC_CAM_RENDERING() {
			return Invoker::Invoke<BOOL, 4020>();
		}

		BOOL IS_CINEMATIC_CAM_INPUT_ACTIVE() {
			return Invoker::Invoke<BOOL, 4021>();
		}

		void IGNORE_MENU_PREFERENCE_FOR_BONNET_CAMERA_THIS_UPDATE() {
			Invoker::Invoke<void, 4022>();
		}

		void BYPASS_CUTSCENE_CAM_RENDERING_THIS_UPDATE() {
			Invoker::Invoke<void, 4023>();
		}

		void STOP_CUTSCENE_CAM_SHAKING(Any p0) {
			Invoker::Invoke<void, 4024>(p0);
		}

		void SET_CUTSCENE_CAM_FAR_CLIP_THIS_UPDATE(float p0) {
			Invoker::Invoke<void, 4025>(p0);
		}

		Ped GET_FOCUS_PED_ON_SCREEN(float p0, int p1, float p2, float p3, float p4, float p5, float p6, int p7, int p8) {
			return Invoker::Invoke<Ped, 4026>(p0, p1, p2, p3, p4, p5, p6, p7, p8);
		}

		void DISABLE_NEAR_CLIP_SCAN_THIS_UPDATE() {
			Invoker::Invoke<void, 4027>();
		}

		void SET_CAM_DEATH_FAIL_EFFECT_STATE(int p0) {
			Invoker::Invoke<void, 4028>(p0);
		}

		void SET_FIRST_PERSON_FLASH_EFFECT_TYPE(Any p0) {
			Invoker::Invoke<void, 4029>(p0);
		}

		void SET_FIRST_PERSON_FLASH_EFFECT_VEHICLE_MODEL_NAME(const char* vehicleName) {
			Invoker::Invoke<void, 4030>(vehicleName);
		}

		void SET_FIRST_PERSON_FLASH_EFFECT_VEHICLE_MODEL_HASH(Hash vehicleModel) {
			Invoker::Invoke<void, 4031>(vehicleModel);
		}

		BOOL IS_ALLOWED_INDEPENDENT_CAMERA_MODES() {
			return Invoker::Invoke<BOOL, 4032>();
		}

		void CAMERA_PREVENT_COLLISION_SETTINGS_FOR_TRIPLEHEAD_IN_INTERIORS_THIS_UPDATE() {
			Invoker::Invoke<void, 4033>();
		}

		float REPLAY_GET_MAX_DISTANCE_ALLOWED_FROM_PLAYER() {
			return Invoker::Invoke<float, 4034>();
		}

	} // namespace CAM

	namespace CLOCK {

		void SET_CLOCK_TIME(int hour, int minute, int second) {
			Invoker::Invoke<void, 1047>(hour, minute, second);
		}

		void PAUSE_CLOCK(BOOL toggle) {
			Invoker::Invoke<void, 1048>(toggle);
		}

		void ADVANCE_CLOCK_TIME_TO(int hour, int minute, int second) {
			Invoker::Invoke<void, 1049>(hour, minute, second);
		}

		void ADD_TO_CLOCK_TIME(int hours, int minutes, int seconds) {
			Invoker::Invoke<void, 1050>(hours, minutes, seconds);
		}

		int GET_CLOCK_HOURS() {
			return Invoker::Invoke<int, 1051>();
		}

		int GET_CLOCK_MINUTES() {
			return Invoker::Invoke<int, 1052>();
		}

		int GET_CLOCK_SECONDS() {
			return Invoker::Invoke<int, 1053>();
		}

		void SET_CLOCK_DATE(int day, int month, int year) {
			Invoker::Invoke<void, 1054>(day, month, year);
		}

		int GET_CLOCK_DAY_OF_WEEK() {
			return Invoker::Invoke<int, 1055>();
		}

		int GET_CLOCK_DAY_OF_MONTH() {
			return Invoker::Invoke<int, 1056>();
		}

		int GET_CLOCK_MONTH() {
			return Invoker::Invoke<int, 1057>();
		}

		int GET_CLOCK_YEAR() {
			return Invoker::Invoke<int, 1058>();
		}

		int GET_MILLISECONDS_PER_GAME_MINUTE() {
			return Invoker::Invoke<int, 1059>();
		}

		void GET_POSIX_TIME(int* year, int* month, int* day, int* hour, int* minute, int* second) {
			Invoker::Invoke<void, 1060>(year, month, day, hour, minute, second);
		}

		void GET_UTC_TIME(int* year, int* month, int* day, int* hour, int* minute, int* second) {
			Invoker::Invoke<void, 1061>(year, month, day, hour, minute, second);
		}

		void GET_LOCAL_TIME(int* year, int* month, int* day, int* hour, int* minute, int* second) {
			Invoker::Invoke<void, 1062>(year, month, day, hour, minute, second);
		}

	} // namespace CLOCK

	namespace CUTSCENE {

		void REQUEST_CUTSCENE(const char* cutsceneName, int flags) {
			Invoker::Invoke<void, 3334>(cutsceneName, flags);
		}

		void REQUEST_CUTSCENE_WITH_PLAYBACK_LIST(const char* cutsceneName, int playbackFlags, int flags) {
			Invoker::Invoke<void, 3335>(cutsceneName, playbackFlags, flags);
		}

		void REMOVE_CUTSCENE() {
			Invoker::Invoke<void, 3336>();
		}

		BOOL HAS_CUTSCENE_LOADED() {
			return Invoker::Invoke<BOOL, 3337>();
		}

		BOOL HAS_THIS_CUTSCENE_LOADED(const char* cutsceneName) {
			return Invoker::Invoke<BOOL, 3338>(cutsceneName);
		}

		void SET_SCRIPT_CAN_START_CUTSCENE(int threadId) {
			Invoker::Invoke<void, 3339>(threadId);
		}

		BOOL CAN_REQUEST_ASSETS_FOR_CUTSCENE_ENTITY() {
			return Invoker::Invoke<BOOL, 3340>();
		}

		BOOL IS_CUTSCENE_PLAYBACK_FLAG_SET(int flag) {
			return Invoker::Invoke<BOOL, 3341>(flag);
		}

		void SET_CUTSCENE_ENTITY_STREAMING_FLAGS(const char* cutsceneEntName, int p1, int p2) {
			Invoker::Invoke<void, 3342>(cutsceneEntName, p1, p2);
		}

		void REQUEST_CUT_FILE(const char* cutsceneName) {
			Invoker::Invoke<void, 3343>(cutsceneName);
		}

		BOOL HAS_CUT_FILE_LOADED(const char* cutsceneName) {
			return Invoker::Invoke<BOOL, 3344>(cutsceneName);
		}

		void REMOVE_CUT_FILE(const char* cutsceneName) {
			Invoker::Invoke<void, 3345>(cutsceneName);
		}

		Vector3 GET_CUT_FILE_OFFSET(const char* cutsceneName, int index) {
			return Invoker::Invoke<Vector3, 6700>(cutsceneName, index);
		}

		int GET_CUT_FILE_CONCAT_COUNT(const char* cutsceneName) {
			return Invoker::Invoke<int, 3346>(cutsceneName);
		}

		void START_CUTSCENE(int flags) {
			Invoker::Invoke<void, 3347>(flags);
		}

		void START_CUTSCENE_AT_COORDS(float x, float y, float z, int flags) {
			Invoker::Invoke<void, 3348>(x, y, z, flags);
		}

		void STOP_CUTSCENE(BOOL p0) {
			Invoker::Invoke<void, 3349>(p0);
		}

		void STOP_CUTSCENE_IMMEDIATELY() {
			Invoker::Invoke<void, 3350>();
		}

		void SET_CUTSCENE_ORIGIN(float x, float y, float z, float p3, int p4) {
			Invoker::Invoke<void, 3351>(x, y, z, p3, p4);
		}

		void SET_CUTSCENE_ORIGIN_AND_ORIENTATION(float x1, float y1, float z1, float x2, float y2, float z2, int p6) {
			Invoker::Invoke<void, 3352>(x1, y1, z1, x2, y2, z2, p6);
		}

		int GET_CUTSCENE_TIME() {
			return Invoker::Invoke<int, 3353>();
		}

		int GET_CUTSCENE_PLAY_TIME() {
			return Invoker::Invoke<int, 6571>();
		}

		int GET_CUTSCENE_TOTAL_DURATION() {
			return Invoker::Invoke<int, 3354>();
		}

		int GET_CUTSCENE_END_TIME() {
			return Invoker::Invoke<int, 3355>();
		}

		int GET_CUTSCENE_PLAY_DURATION() {
			return Invoker::Invoke<int, 3356>();
		}

		BOOL WAS_CUTSCENE_SKIPPED() {
			return Invoker::Invoke<BOOL, 3357>();
		}

		BOOL HAS_CUTSCENE_FINISHED() {
			return Invoker::Invoke<BOOL, 3358>();
		}

		BOOL IS_CUTSCENE_ACTIVE() {
			return Invoker::Invoke<BOOL, 3359>();
		}

		BOOL IS_CUTSCENE_PLAYING() {
			return Invoker::Invoke<BOOL, 3360>();
		}

		int GET_CUTSCENE_SECTION_PLAYING() {
			return Invoker::Invoke<int, 3361>();
		}

		Entity GET_ENTITY_INDEX_OF_CUTSCENE_ENTITY(const char* cutsceneEntName, Hash modelHash) {
			return Invoker::Invoke<Entity, 3362>(cutsceneEntName, modelHash);
		}

		int GET_CUTSCENE_CONCAT_SECTION_PLAYING() {
			return Invoker::Invoke<int, 3363>();
		}

		BOOL IS_CUTSCENE_AUTHORIZED(const char* cutsceneName) {
			return Invoker::Invoke<BOOL, 3364>(cutsceneName);
		}

		int DOES_CUTSCENE_HANDLE_EXIST(int cutsceneHandle) {
			return Invoker::Invoke<int, 3365>(cutsceneHandle);
		}

		void REGISTER_ENTITY_FOR_CUTSCENE(Ped cutscenePed, const char* cutsceneEntName, int p2, Hash modelHash, int p4) {
			Invoker::Invoke<void, 3366>(cutscenePed, cutsceneEntName, p2, modelHash, p4);
		}

		Entity GET_ENTITY_INDEX_OF_REGISTERED_ENTITY(const char* cutsceneEntName, Hash modelHash) {
			return Invoker::Invoke<Entity, 3367>(cutsceneEntName, modelHash);
		}

		void SET_VEHICLE_MODEL_PLAYER_WILL_EXIT_SCENE(Hash modelHash) {
			Invoker::Invoke<void, 3368>(modelHash);
		}

		void SET_CUTSCENE_TRIGGER_AREA(float x1, float y1, float z1, float x2, float y2, float z2) {
			Invoker::Invoke<void, 3369>(x1, y1, z1, x2, y2, z2);
		}

		BOOL CAN_SET_ENTER_STATE_FOR_REGISTERED_ENTITY(const char* cutsceneEntName, Hash modelHash) {
			return Invoker::Invoke<BOOL, 3370>(cutsceneEntName, modelHash);
		}

		BOOL CAN_SET_EXIT_STATE_FOR_REGISTERED_ENTITY(const char* cutsceneEntName, Hash modelHash) {
			return Invoker::Invoke<BOOL, 3371>(cutsceneEntName, modelHash);
		}

		BOOL CAN_SET_EXIT_STATE_FOR_CAMERA(BOOL p0) {
			return Invoker::Invoke<BOOL, 3372>(p0);
		}

		void SET_PAD_CAN_SHAKE_DURING_CUTSCENE(BOOL toggle) {
			Invoker::Invoke<void, 3373>(toggle);
		}

		void SET_CUTSCENE_FADE_VALUES(BOOL p0, BOOL p1, BOOL p2, BOOL p3) {
			Invoker::Invoke<void, 3374>(p0, p1, p2, p3);
		}

		void SET_CUTSCENE_MULTIHEAD_FADE(BOOL p0, BOOL p1, BOOL p2, BOOL p3) {
			Invoker::Invoke<void, 3375>(p0, p1, p2, p3);
		}

		void SET_CUTSCENE_MULTIHEAD_FADE_MANUAL(BOOL p0) {
			Invoker::Invoke<void, 3376>(p0);
		}

		BOOL IS_MULTIHEAD_FADE_UP() {
			return Invoker::Invoke<BOOL, 3377>();
		}

		void NETWORK_SET_MOCAP_CUTSCENE_CAN_BE_SKIPPED(BOOL toggle) {
			Invoker::Invoke<void, 3378>(toggle);
		}

		void SET_CAR_GENERATORS_CAN_UPDATE_DURING_CUTSCENE(BOOL p0) {
			Invoker::Invoke<void, 3379>(p0);
		}

		BOOL CAN_USE_MOBILE_PHONE_DURING_CUTSCENE() {
			return Invoker::Invoke<BOOL, 3380>();
		}

		void SET_CUTSCENE_CAN_BE_SKIPPED(BOOL p0) {
			Invoker::Invoke<void, 3381>(p0);
		}

		void SET_CAN_DISPLAY_MINIMAP_DURING_CUTSCENE_THIS_UPDATE() {
			Invoker::Invoke<void, 3382>();
		}

		void SET_CUTSCENE_PED_COMPONENT_VARIATION(const char* cutsceneEntName, int componentId, int drawableId, int textureId, Hash modelHash) {
			Invoker::Invoke<void, 3383>(cutsceneEntName, componentId, drawableId, textureId, modelHash);
		}

		void SET_CUTSCENE_PED_COMPONENT_VARIATION_FROM_PED(const char* cutsceneEntName, Ped ped, Hash modelHash) {
			Invoker::Invoke<void, 3384>(cutsceneEntName, ped, modelHash);
		}

		BOOL DOES_CUTSCENE_ENTITY_EXIST(const char* cutsceneEntName, Hash modelHash) {
			return Invoker::Invoke<BOOL, 3385>(cutsceneEntName, modelHash);
		}

		void SET_CUTSCENE_PED_PROP_VARIATION(const char* cutsceneEntName, int componentId, int drawableId, int textureId, Hash modelHash) {
			Invoker::Invoke<void, 3386>(cutsceneEntName, componentId, drawableId, textureId, modelHash);
		}

		BOOL HAS_CUTSCENE_CUT_THIS_FRAME() {
			return Invoker::Invoke<BOOL, 3387>();
		}

	} // namespace CUTSCENE

	namespace DATAFILE {

		void DATAFILE_WATCH_REQUEST_ID(int requestId) {
			Invoker::Invoke<void, 3266>(requestId);
		}

		void DATAFILE_CLEAR_WATCH_LIST() {
			Invoker::Invoke<void, 3267>();
		}

		BOOL DATAFILE_IS_VALID_REQUEST_ID(int index) {
			return Invoker::Invoke<BOOL, 3268>(index);
		}

		BOOL DATAFILE_HAS_LOADED_FILE_DATA(int requestId) {
			return Invoker::Invoke<BOOL, 3269>(requestId);
		}

		BOOL DATAFILE_HAS_VALID_FILE_DATA(int requestId) {
			return Invoker::Invoke<BOOL, 3270>(requestId);
		}

		BOOL DATAFILE_SELECT_ACTIVE_FILE(int requestId, Any p1) {
			return Invoker::Invoke<BOOL, 3271>(requestId, p1);
		}

		BOOL DATAFILE_DELETE_REQUESTED_FILE(int requestId) {
			return Invoker::Invoke<BOOL, 3272>(requestId);
		}

		BOOL UGC_CREATE_CONTENT(Any* data, int dataCount, const char* contentName, const char* description, const char* tagsCsv, const char* contentTypeName, BOOL publish, Any p7) {
			return Invoker::Invoke<BOOL, 3273>(data, dataCount, contentName, description, tagsCsv, contentTypeName, publish, p7);
		}

		BOOL UGC_CREATE_MISSION(const char* contentName, const char* description, const char* tagsCsv, const char* contentTypeName, BOOL publish, Any p5) {
			return Invoker::Invoke<BOOL, 3274>(contentName, description, tagsCsv, contentTypeName, publish, p5);
		}

		BOOL UGC_UPDATE_CONTENT(const char* contentId, Any* data, int dataCount, const char* contentName, const char* description, const char* tagsCsv, const char* contentTypeName, Any p7) {
			return Invoker::Invoke<BOOL, 3275>(contentId, data, dataCount, contentName, description, tagsCsv, contentTypeName, p7);
		}

		BOOL UGC_UPDATE_MISSION(const char* contentId, const char* contentName, const char* description, const char* tagsCsv, const char* contentTypeName, Any p5) {
			return Invoker::Invoke<BOOL, 3276>(contentId, contentName, description, tagsCsv, contentTypeName, p5);
		}

		BOOL UGC_SET_PLAYER_DATA(const char* contentId, float rating, const char* contentTypeName, Any p3) {
			return Invoker::Invoke<BOOL, 3277>(contentId, rating, contentTypeName, p3);
		}

		BOOL DATAFILE_SELECT_UGC_DATA(int p0, Any p1) {
			return Invoker::Invoke<BOOL, 3278>(p0, p1);
		}

		BOOL DATAFILE_SELECT_UGC_STATS(int p0, BOOL p1, Any p2) {
			return Invoker::Invoke<BOOL, 3279>(p0, p1, p2);
		}

		BOOL DATAFILE_SELECT_UGC_PLAYER_DATA(int p0, Any p1) {
			return Invoker::Invoke<BOOL, 3280>(p0, p1);
		}

		BOOL DATAFILE_SELECT_CREATOR_STATS(int p0, Any p1) {
			return Invoker::Invoke<BOOL, 3281>(p0, p1);
		}

		BOOL DATAFILE_LOAD_OFFLINE_UGC(const char* filename, Any p1) {
			return Invoker::Invoke<BOOL, 3282>(filename, p1);
		}

		void DATAFILE_CREATE(int p0) {
			Invoker::Invoke<void, 3283>(p0);
		}

		void DATAFILE_DELETE(int p0) {
			Invoker::Invoke<void, 3284>(p0);
		}

		void DATAFILE_STORE_MISSION_HEADER(int p0) {
			Invoker::Invoke<void, 3285>(p0);
		}

		void DATAFILE_FLUSH_MISSION_HEADER() {
			Invoker::Invoke<void, 3286>();
		}

		Any* DATAFILE_GET_FILE_DICT(int p0) {
			return Invoker::Invoke<Any*, 3287>(p0);
		}

		BOOL DATAFILE_START_SAVE_TO_CLOUD(const char* filename, Any p1) {
			return Invoker::Invoke<BOOL, 3288>(filename, p1);
		}

		BOOL DATAFILE_UPDATE_SAVE_TO_CLOUD(BOOL* p0) {
			return Invoker::Invoke<BOOL, 3289>(p0);
		}

		BOOL DATAFILE_IS_SAVE_PENDING() {
			return Invoker::Invoke<BOOL, 3290>();
		}

		BOOL DATAFILE_LOAD_OFFLINE_UGC_FOR_ADDITIONAL_DATA_FILE(Any p0, Any p1) {
			return Invoker::Invoke<BOOL, 3291>(p0, p1);
		}

		void DATAFILE_DELETE_FOR_ADDITIONAL_DATA_FILE(Any p0) {
			Invoker::Invoke<void, 3292>(p0);
		}

		Any* DATAFILE_GET_FILE_DICT_FOR_ADDITIONAL_DATA_FILE(Any p0) {
			return Invoker::Invoke<Any*, 3293>(p0);
		}

		void DATADICT_SET_BOOL(Any* objectData, const char* key, BOOL value) {
			Invoker::Invoke<void, 3294>(objectData, key, value);
		}

		void DATADICT_SET_INT(Any* objectData, const char* key, int value) {
			Invoker::Invoke<void, 3295>(objectData, key, value);
		}

		void DATADICT_SET_FLOAT(Any* objectData, const char* key, float value) {
			Invoker::Invoke<void, 3296>(objectData, key, value);
		}

		void DATADICT_SET_STRING(Any* objectData, const char* key, const char* value) {
			Invoker::Invoke<void, 3297>(objectData, key, value);
		}

		void DATADICT_SET_VECTOR(Any* objectData, const char* key, float valueX, float valueY, float valueZ) {
			Invoker::Invoke<void, 3298>(objectData, key, valueX, valueY, valueZ);
		}

		Any* DATADICT_CREATE_DICT(Any* objectData, const char* key) {
			return Invoker::Invoke<Any*, 3299>(objectData, key);
		}

		Any* DATADICT_CREATE_ARRAY(Any* objectData, const char* key) {
			return Invoker::Invoke<Any*, 3300>(objectData, key);
		}

		BOOL DATADICT_GET_BOOL(Any* objectData, const char* key) {
			return Invoker::Invoke<BOOL, 3301>(objectData, key);
		}

		int DATADICT_GET_INT(Any* objectData, const char* key) {
			return Invoker::Invoke<int, 3302>(objectData, key);
		}

		float DATADICT_GET_FLOAT(Any* objectData, const char* key) {
			return Invoker::Invoke<float, 3303>(objectData, key);
		}

		const char* DATADICT_GET_STRING(Any* objectData, const char* key) {
			return Invoker::Invoke<const char*, 3304>(objectData, key);
		}

		Vector3 DATADICT_GET_VECTOR(Any* objectData, const char* key) {
			return Invoker::Invoke<Vector3, 3305>(objectData, key);
		}

		Any* DATADICT_GET_DICT(Any* objectData, const char* key) {
			return Invoker::Invoke<Any*, 3306>(objectData, key);
		}

		Any* DATADICT_GET_ARRAY(Any* objectData, const char* key) {
			return Invoker::Invoke<Any*, 3307>(objectData, key);
		}

		int DATADICT_GET_TYPE(Any* objectData, const char* key) {
			return Invoker::Invoke<int, 3308>(objectData, key);
		}

		void DATAARRAY_ADD_BOOL(Any* arrayData, BOOL value) {
			Invoker::Invoke<void, 3309>(arrayData, value);
		}

		void DATAARRAY_ADD_INT(Any* arrayData, int value) {
			Invoker::Invoke<void, 3310>(arrayData, value);
		}

		void DATAARRAY_ADD_FLOAT(Any* arrayData, float value) {
			Invoker::Invoke<void, 3311>(arrayData, value);
		}

		void DATAARRAY_ADD_STRING(Any* arrayData, const char* value) {
			Invoker::Invoke<void, 3312>(arrayData, value);
		}

		void DATAARRAY_ADD_VECTOR(Any* arrayData, float valueX, float valueY, float valueZ) {
			Invoker::Invoke<void, 3313>(arrayData, valueX, valueY, valueZ);
		}

		Any* DATAARRAY_ADD_DICT(Any* arrayData) {
			return Invoker::Invoke<Any*, 3314>(arrayData);
		}

		BOOL DATAARRAY_GET_BOOL(Any* arrayData, int arrayIndex) {
			return Invoker::Invoke<BOOL, 3315>(arrayData, arrayIndex);
		}

		int DATAARRAY_GET_INT(Any* arrayData, int arrayIndex) {
			return Invoker::Invoke<int, 3316>(arrayData, arrayIndex);
		}

		float DATAARRAY_GET_FLOAT(Any* arrayData, int arrayIndex) {
			return Invoker::Invoke<float, 3317>(arrayData, arrayIndex);
		}

		const char* DATAARRAY_GET_STRING(Any* arrayData, int arrayIndex) {
			return Invoker::Invoke<const char*, 3318>(arrayData, arrayIndex);
		}

		Vector3 DATAARRAY_GET_VECTOR(Any* arrayData, int arrayIndex) {
			return Invoker::Invoke<Vector3, 3319>(arrayData, arrayIndex);
		}

		Any* DATAARRAY_GET_DICT(Any* arrayData, int arrayIndex) {
			return Invoker::Invoke<Any*, 3320>(arrayData, arrayIndex);
		}

		int DATAARRAY_GET_COUNT(Any* arrayData) {
			return Invoker::Invoke<int, 3321>(arrayData);
		}

		int DATAARRAY_GET_TYPE(Any* arrayData, int arrayIndex) {
			return Invoker::Invoke<int, 3322>(arrayData, arrayIndex);
		}

	} // namespace DATAFILE

	namespace DECORATOR {

		BOOL DECOR_SET_TIME(Entity entity, const char* propertyName, int timestamp) {
			return Invoker::Invoke<BOOL, 6032>(entity, propertyName, timestamp);
		}

		BOOL DECOR_SET_BOOL(Entity entity, const char* propertyName, BOOL value) {
			return Invoker::Invoke<BOOL, 6033>(entity, propertyName, value);
		}

		BOOL DECOR_SET_FLOAT(Entity entity, const char* propertyName, float value) {
			return Invoker::Invoke<BOOL, 6034>(entity, propertyName, value);
		}

		BOOL DECOR_SET_INT(Entity entity, const char* propertyName, int value) {
			return Invoker::Invoke<BOOL, 6035>(entity, propertyName, value);
		}

		BOOL DECOR_GET_BOOL(Entity entity, const char* propertyName) {
			return Invoker::Invoke<BOOL, 6036>(entity, propertyName);
		}

		float DECOR_GET_FLOAT(Entity entity, const char* propertyName) {
			return Invoker::Invoke<float, 6037>(entity, propertyName);
		}

		int DECOR_GET_INT(Entity entity, const char* propertyName) {
			return Invoker::Invoke<int, 6038>(entity, propertyName);
		}

		BOOL DECOR_EXIST_ON(Entity entity, const char* propertyName) {
			return Invoker::Invoke<BOOL, 6039>(entity, propertyName);
		}

		BOOL DECOR_REMOVE(Entity entity, const char* propertyName) {
			return Invoker::Invoke<BOOL, 6040>(entity, propertyName);
		}

		void DECOR_REGISTER(const char* propertyName, int type) {
			Invoker::Invoke<void, 6041>(propertyName, type);
		}

		BOOL DECOR_IS_REGISTERED_AS_TYPE(const char* propertyName, int type) {
			return Invoker::Invoke<BOOL, 6042>(propertyName, type);
		}

		void DECOR_REGISTER_LOCK() {
			Invoker::Invoke<void, 6043>();
		}

	} // namespace DECORATOR

	namespace DLC {

		BOOL ARE_ANY_CCS_PENDING() {
			return Invoker::Invoke<BOOL, 3323>();
		}

		BOOL IS_DLC_PRESENT(Hash dlcHash) {
			return Invoker::Invoke<BOOL, 3324>(dlcHash);
		}

		BOOL DLC_CHECK_CLOUD_DATA_CORRECT() {
			return Invoker::Invoke<BOOL, 3325>();
		}

		int GET_EXTRACONTENT_CLOUD_RESULT() {
			return Invoker::Invoke<int, 3326>();
		}

		BOOL DLC_CHECK_COMPAT_PACK_CONFIGURATION() {
			return Invoker::Invoke<BOOL, 3327>();
		}

		BOOL GET_EVER_HAD_BAD_PACK_ORDER() {
			return Invoker::Invoke<BOOL, 3328>();
		}

		BOOL GET_IS_LOADING_SCREEN_ACTIVE() {
			return Invoker::Invoke<BOOL, 3329>();
		}

		BOOL GET_IS_INITIAL_LOADING_SCREEN_ACTIVE() {
			return Invoker::Invoke<BOOL, 3330>();
		}

		BOOL HAS_CLOUD_REQUESTS_FINISHED(BOOL* p0, int unused) {
			return Invoker::Invoke<BOOL, 3331>(p0, unused);
		}

		void ON_ENTER_SP() {
			Invoker::Invoke<void, 3332>();
		}

		void ON_ENTER_MP() {
			Invoker::Invoke<void, 3333>();
		}

		BOOL ARE_USER_ENTITLEMENTS_UP_TO_DATE() {
			return Invoker::Invoke<BOOL, 6699>();
		}

		BOOL TRY_GET_USER_ENTITLEMENTS(Any* entitlements) {
			return Invoker::Invoke<BOOL, 6698>(entitlements);
		}

		void DECLARE_IN_MULTIPLAYER_THIS_FRAME() {
			Invoker::Invoke<void, 6697>();
		}

	} // namespace DLC

	namespace ENTITY {

		BOOL DOES_ENTITY_EXIST(Entity entity) {
			return Invoker::Invoke<BOOL, 1856>(entity);
		}

		BOOL DOES_ENTITY_BELONG_TO_THIS_SCRIPT(Entity entity, BOOL p1) {
			return Invoker::Invoke<BOOL, 1857>(entity, p1);
		}

		BOOL DOES_ENTITY_HAVE_DRAWABLE(Entity entity) {
			return Invoker::Invoke<BOOL, 1858>(entity);
		}

		BOOL DOES_ENTITY_HAVE_PHYSICS(Entity entity) {
			return Invoker::Invoke<BOOL, 1859>(entity);
		}

		BOOL DOES_ENTITY_HAVE_SKELETON(Entity entity) {
			return Invoker::Invoke<BOOL, 1860>(entity);
		}

		BOOL DOES_ENTITY_HAVE_ANIM_DIRECTOR(Entity entity) {
			return Invoker::Invoke<BOOL, 1861>(entity);
		}

		BOOL HAS_ENTITY_ANIM_FINISHED(Entity entity, const char* animDict, const char* animName, int p3) {
			return Invoker::Invoke<BOOL, 1862>(entity, animDict, animName, p3);
		}

		BOOL HAS_ENTITY_BEEN_DAMAGED_BY_ANY_OBJECT(Entity entity) {
			return Invoker::Invoke<BOOL, 1863>(entity);
		}

		BOOL HAS_ENTITY_BEEN_DAMAGED_BY_ANY_PED(Entity entity) {
			return Invoker::Invoke<BOOL, 1864>(entity);
		}

		BOOL HAS_ENTITY_BEEN_DAMAGED_BY_ANY_VEHICLE(Entity entity) {
			return Invoker::Invoke<BOOL, 1865>(entity);
		}

		BOOL HAS_ENTITY_BEEN_DAMAGED_BY_ENTITY(Entity entity1, Entity entity2, BOOL p2) {
			return Invoker::Invoke<BOOL, 1866>(entity1, entity2, p2);
		}

		BOOL HAS_ENTITY_CLEAR_LOS_TO_ENTITY(Entity entity1, Entity entity2, int traceType) {
			return Invoker::Invoke<BOOL, 1867>(entity1, entity2, traceType);
		}

		BOOL HAS_ENTITY_CLEAR_LOS_TO_ENTITY_ADJUST_FOR_COVER(Entity entity1, Entity entity2, int traceType) {
			return Invoker::Invoke<BOOL, 1868>(entity1, entity2, traceType);
		}

		BOOL HAS_ENTITY_CLEAR_LOS_TO_ENTITY_IN_FRONT(Entity entity1, Entity entity2) {
			return Invoker::Invoke<BOOL, 1869>(entity1, entity2);
		}

		BOOL HAS_ENTITY_COLLIDED_WITH_ANYTHING(Entity entity) {
			return Invoker::Invoke<BOOL, 1870>(entity);
		}

		Entity _GET_LAST_ENTITY_HIT_BY_ENTITY(Entity entity) {
			return Invoker::Invoke<Entity, 1871>(entity);
		}

		Hash GET_LAST_MATERIAL_HIT_BY_ENTITY(Entity entity) {
			return Invoker::Invoke<Hash, 1872>(entity);
		}

		Vector3 GET_COLLISION_NORMAL_OF_LAST_HIT_FOR_ENTITY(Entity entity) {
			return Invoker::Invoke<Vector3, 1873>(entity);
		}

		void FORCE_ENTITY_AI_AND_ANIMATION_UPDATE(Entity entity) {
			Invoker::Invoke<void, 1874>(entity);
		}

		float GET_ENTITY_ANIM_CURRENT_TIME(Entity entity, const char* animDict, const char* animName) {
			return Invoker::Invoke<float, 1875>(entity, animDict, animName);
		}

		float GET_ENTITY_ANIM_TOTAL_TIME(Entity entity, const char* animDict, const char* animName) {
			return Invoker::Invoke<float, 1876>(entity, animDict, animName);
		}

		float GET_ANIM_DURATION(const char* animDict, const char* animName) {
			return Invoker::Invoke<float, 1877>(animDict, animName);
		}

		Entity GET_ENTITY_ATTACHED_TO(Entity entity) {
			return Invoker::Invoke<Entity, 1878>(entity);
		}

		Vector3 GET_ENTITY_COORDS(Entity entity, BOOL alive) {
			return Invoker::Invoke<Vector3, 1879>(entity, alive);
		}

		Vector3 GET_ENTITY_FORWARD_VECTOR(Entity entity) {
			return Invoker::Invoke<Vector3, 1880>(entity);
		}

		float GET_ENTITY_FORWARD_X(Entity entity) {
			return Invoker::Invoke<float, 1881>(entity);
		}

		float GET_ENTITY_FORWARD_Y(Entity entity) {
			return Invoker::Invoke<float, 1882>(entity);
		}

		float GET_ENTITY_HEADING(Entity entity) {
			return Invoker::Invoke<float, 1883>(entity);
		}

		float GET_ENTITY_HEADING_FROM_EULERS(Entity entity) {
			return Invoker::Invoke<float, 1884>(entity);
		}

		int GET_ENTITY_HEALTH(Entity entity) {
			return Invoker::Invoke<int, 1885>(entity);
		}

		int GET_ENTITY_MAX_HEALTH(Entity entity) {
			return Invoker::Invoke<int, 1886>(entity);
		}

		void SET_ENTITY_MAX_HEALTH(Entity entity, int value) {
			Invoker::Invoke<void, 1887>(entity, value);
		}

		float GET_ENTITY_HEIGHT(Entity entity, float X, float Y, float Z, BOOL atTop, BOOL inWorldCoords) {
			return Invoker::Invoke<float, 1888>(entity, X, Y, Z, atTop, inWorldCoords);
		}

		float GET_ENTITY_HEIGHT_ABOVE_GROUND(Entity entity) {
			return Invoker::Invoke<float, 1889>(entity);
		}

		void GET_ENTITY_MATRIX(Entity entity, Vector3* forwardVector, Vector3* rightVector, Vector3* upVector, Vector3* position) {
			Invoker::Invoke<void, 1890>(entity, forwardVector, rightVector, upVector, position);
		}

		Hash GET_ENTITY_MODEL(Entity entity) {
			return Invoker::Invoke<Hash, 1891>(entity);
		}

		Vector3 GET_OFFSET_FROM_ENTITY_GIVEN_WORLD_COORDS(Entity entity, float posX, float posY, float posZ) {
			return Invoker::Invoke<Vector3, 1892>(entity, posX, posY, posZ);
		}

		Vector3 GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(Entity entity, float offsetX, float offsetY, float offsetZ) {
			return Invoker::Invoke<Vector3, 1893>(entity, offsetX, offsetY, offsetZ);
		}

		float GET_ENTITY_PITCH(Entity entity) {
			return Invoker::Invoke<float, 1894>(entity);
		}

		void GET_ENTITY_QUATERNION(Entity entity, float* x, float* y, float* z, float* w) {
			Invoker::Invoke<void, 1895>(entity, x, y, z, w);
		}

		float GET_ENTITY_ROLL(Entity entity) {
			return Invoker::Invoke<float, 1896>(entity);
		}

		Vector3 GET_ENTITY_ROTATION(Entity entity, int rotationOrder) {
			return Invoker::Invoke<Vector3, 1897>(entity, rotationOrder);
		}

		Vector3 GET_ENTITY_ROTATION_VELOCITY(Entity entity) {
			return Invoker::Invoke<Vector3, 1898>(entity);
		}

		const char* GET_ENTITY_SCRIPT(Entity entity, ScrHandle* script) {
			return Invoker::Invoke<const char*, 1899>(entity, script);
		}

		float GET_ENTITY_SPEED(Entity entity) {
			return Invoker::Invoke<float, 1900>(entity);
		}

		Vector3 GET_ENTITY_SPEED_VECTOR(Entity entity, BOOL relative) {
			return Invoker::Invoke<Vector3, 1901>(entity, relative);
		}

		float GET_ENTITY_UPRIGHT_VALUE(Entity entity) {
			return Invoker::Invoke<float, 1902>(entity);
		}

		Vector3 GET_ENTITY_VELOCITY(Entity entity) {
			return Invoker::Invoke<Vector3, 1903>(entity);
		}

		Object GET_OBJECT_INDEX_FROM_ENTITY_INDEX(Entity entity) {
			return Invoker::Invoke<Object, 1904>(entity);
		}

		Ped GET_PED_INDEX_FROM_ENTITY_INDEX(Entity entity) {
			return Invoker::Invoke<Ped, 1905>(entity);
		}

		Vehicle GET_VEHICLE_INDEX_FROM_ENTITY_INDEX(Entity entity) {
			return Invoker::Invoke<Vehicle, 1906>(entity);
		}

		Vector3 GET_WORLD_POSITION_OF_ENTITY_BONE(Entity entity, int boneIndex) {
			return Invoker::Invoke<Vector3, 1907>(entity, boneIndex);
		}

		Player GET_NEAREST_PLAYER_TO_ENTITY(Entity entity) {
			return Invoker::Invoke<Player, 1908>(entity);
		}

		Player GET_NEAREST_PLAYER_TO_ENTITY_ON_TEAM(Entity entity, int team) {
			return Invoker::Invoke<Player, 1909>(entity, team);
		}

		int GET_NEAREST_PARTICIPANT_TO_ENTITY(Entity entity) {
			return Invoker::Invoke<int, 1910>(entity);
		}

		int GET_ENTITY_TYPE(Entity entity) {
			return Invoker::Invoke<int, 1911>(entity);
		}

		int GET_ENTITY_POPULATION_TYPE(Entity entity) {
			return Invoker::Invoke<int, 1912>(entity);
		}

		BOOL IS_AN_ENTITY(ScrHandle handle) {
			return Invoker::Invoke<BOOL, 1913>(handle);
		}

		BOOL IS_ENTITY_A_PED(Entity entity) {
			return Invoker::Invoke<BOOL, 1914>(entity);
		}

		BOOL IS_ENTITY_A_MISSION_ENTITY(Entity entity) {
			return Invoker::Invoke<BOOL, 1915>(entity);
		}

		BOOL IS_ENTITY_A_VEHICLE(Entity entity) {
			return Invoker::Invoke<BOOL, 1916>(entity);
		}

		BOOL IS_ENTITY_AN_OBJECT(Entity entity) {
			return Invoker::Invoke<BOOL, 1917>(entity);
		}

		BOOL IS_ENTITY_AT_COORD(Entity entity, float xPos, float yPos, float zPos, float xSize, float ySize, float zSize, BOOL p7, BOOL p8, int p9) {
			return Invoker::Invoke<BOOL, 1918>(entity, xPos, yPos, zPos, xSize, ySize, zSize, p7, p8, p9);
		}

		BOOL IS_ENTITY_AT_ENTITY(Entity entity1, Entity entity2, float xSize, float ySize, float zSize, BOOL p5, BOOL p6, int p7) {
			return Invoker::Invoke<BOOL, 1919>(entity1, entity2, xSize, ySize, zSize, p5, p6, p7);
		}

		BOOL IS_ENTITY_ATTACHED(Entity entity) {
			return Invoker::Invoke<BOOL, 1920>(entity);
		}

		BOOL IS_ENTITY_ATTACHED_TO_ANY_OBJECT(Entity entity) {
			return Invoker::Invoke<BOOL, 1921>(entity);
		}

		BOOL IS_ENTITY_ATTACHED_TO_ANY_PED(Entity entity) {
			return Invoker::Invoke<BOOL, 1922>(entity);
		}

		BOOL IS_ENTITY_ATTACHED_TO_ANY_VEHICLE(Entity entity) {
			return Invoker::Invoke<BOOL, 1923>(entity);
		}

		BOOL IS_ENTITY_ATTACHED_TO_ENTITY(Entity from, Entity to) {
			return Invoker::Invoke<BOOL, 1924>(from, to);
		}

		BOOL IS_ENTITY_DEAD(Entity entity, BOOL p1) {
			return Invoker::Invoke<BOOL, 1925>(entity, p1);
		}

		BOOL IS_ENTITY_IN_AIR(Entity entity) {
			return Invoker::Invoke<BOOL, 1926>(entity);
		}

		BOOL IS_ENTITY_IN_ANGLED_AREA(Entity entity, float x1, float y1, float z1, float x2, float y2, float z2, float width, BOOL debug, BOOL includeZ, Any p10) {
			return Invoker::Invoke<BOOL, 1927>(entity, x1, y1, z1, x2, y2, z2, width, debug, includeZ, p10);
		}

		BOOL IS_ENTITY_IN_AREA(Entity entity, float x1, float y1, float z1, float x2, float y2, float z2, BOOL p7, BOOL p8, Any p9) {
			return Invoker::Invoke<BOOL, 1928>(entity, x1, y1, z1, x2, y2, z2, p7, p8, p9);
		}

		BOOL IS_ENTITY_IN_ZONE(Entity entity, const char* zone) {
			return Invoker::Invoke<BOOL, 1929>(entity, zone);
		}

		BOOL IS_ENTITY_IN_WATER(Entity entity) {
			return Invoker::Invoke<BOOL, 1930>(entity);
		}

		float GET_ENTITY_SUBMERGED_LEVEL(Entity entity) {
			return Invoker::Invoke<float, 1931>(entity);
		}

		void SET_ENTITY_REQUIRES_MORE_EXPENSIVE_RIVER_CHECK(Entity entity, BOOL toggle) {
			Invoker::Invoke<void, 1932>(entity, toggle);
		}

		BOOL IS_ENTITY_ON_SCREEN(Entity entity) {
			return Invoker::Invoke<BOOL, 1933>(entity);
		}

		BOOL IS_ENTITY_PLAYING_ANIM(Entity entity, const char* animDict, const char* animName, int taskFlag) {
			return Invoker::Invoke<BOOL, 1934>(entity, animDict, animName, taskFlag);
		}

		BOOL IS_ENTITY_STATIC(Entity entity) {
			return Invoker::Invoke<BOOL, 1935>(entity);
		}

		BOOL IS_ENTITY_TOUCHING_ENTITY(Entity entity, Entity targetEntity) {
			return Invoker::Invoke<BOOL, 1936>(entity, targetEntity);
		}

		BOOL IS_ENTITY_TOUCHING_MODEL(Entity entity, Hash modelHash) {
			return Invoker::Invoke<BOOL, 1937>(entity, modelHash);
		}

		BOOL IS_ENTITY_UPRIGHT(Entity entity, float angle) {
			return Invoker::Invoke<BOOL, 1938>(entity, angle);
		}

		BOOL IS_ENTITY_UPSIDEDOWN(Entity entity) {
			return Invoker::Invoke<BOOL, 1939>(entity);
		}

		BOOL IS_ENTITY_VISIBLE(Entity entity) {
			return Invoker::Invoke<BOOL, 1940>(entity);
		}

		BOOL IS_ENTITY_VISIBLE_TO_SCRIPT(Entity entity) {
			return Invoker::Invoke<BOOL, 1941>(entity);
		}

		BOOL IS_ENTITY_OCCLUDED(Entity entity) {
			return Invoker::Invoke<BOOL, 1942>(entity);
		}

		BOOL WOULD_ENTITY_BE_OCCLUDED(Hash entityModelHash, float x, float y, float z, BOOL p4) {
			return Invoker::Invoke<BOOL, 1943>(entityModelHash, x, y, z, p4);
		}

		BOOL IS_ENTITY_WAITING_FOR_WORLD_COLLISION(Entity entity) {
			return Invoker::Invoke<BOOL, 1944>(entity);
		}

		void APPLY_FORCE_TO_ENTITY_CENTER_OF_MASS(Entity entity, int forceType, float x, float y, float z, BOOL p5, BOOL isDirectionRel, BOOL isForceRel, BOOL p8) {
			Invoker::Invoke<void, 1945>(entity, forceType, x, y, z, p5, isDirectionRel, isForceRel, p8);
		}

		void APPLY_FORCE_TO_ENTITY(Entity entity, int forceFlags, float x, float y, float z, float offX, float offY, float offZ, int boneIndex, BOOL isDirectionRel, BOOL ignoreUpVec, BOOL isForceRel, BOOL p12, BOOL p13) {
			Invoker::Invoke<void, 1946>(entity, forceFlags, x, y, z, offX, offY, offZ, boneIndex, isDirectionRel, ignoreUpVec, isForceRel, p12, p13);
		}

		void ATTACH_ENTITY_TO_ENTITY(Entity entity1, Entity entity2, int boneIndex, float xPos, float yPos, float zPos, float xRot, float yRot, float zRot, BOOL p9, BOOL useSoftPinning, BOOL collision, BOOL isPed, int vertexIndex, BOOL fixedRot, Any p15) {
			Invoker::Invoke<void, 1947>(entity1, entity2, boneIndex, xPos, yPos, zPos, xRot, yRot, zRot, p9, useSoftPinning, collision, isPed, vertexIndex, fixedRot, p15);
		}

		void ATTACH_ENTITY_BONE_TO_ENTITY_BONE(Entity entity1, Entity entity2, int boneIndex1, int boneIndex2, BOOL p4, BOOL p5) {
			Invoker::Invoke<void, 1948>(entity1, entity2, boneIndex1, boneIndex2, p4, p5);
		}

		void ATTACH_ENTITY_BONE_TO_ENTITY_BONE_Y_FORWARD(Entity entity1, Entity entity2, int boneIndex1, int boneIndex2, BOOL p4, BOOL p5) {
			Invoker::Invoke<void, 1949>(entity1, entity2, boneIndex1, boneIndex2, p4, p5);
		}

		void ATTACH_ENTITY_TO_ENTITY_PHYSICALLY(Entity entity1, Entity entity2, int boneIndex1, int boneIndex2, float xPos1, float yPos1, float zPos1, float xPos2, float yPos2, float zPos2, float xRot, float yRot, float zRot, float breakForce, BOOL fixedRot, BOOL p15, BOOL collision, BOOL p17, int p18) {
			Invoker::Invoke<void, 1950>(entity1, entity2, boneIndex1, boneIndex2, xPos1, yPos1, zPos1, xPos2, yPos2, zPos2, xRot, yRot, zRot, breakForce, fixedRot, p15, collision, p17, p18);
		}

		void ATTACH_ENTITY_TO_ENTITY_PHYSICALLY_OVERRIDE_INVERSE_MASS(Entity firstEntityIndex, Entity secondEntityIndex, int firstEntityBoneIndex, int secondEntityBoneIndex, float secondEntityOffsetX, float secondEntityOffsetY, float secondEntityOffsetZ, float firstEntityOffsetX, float firstEntityOffsetY, float firstEntityOffsetZ, float vecRotationX, float vecRotationY, float vecRotationZ, float physicalStrength, BOOL constrainRotation, BOOL doInitialWarp, BOOL collideWithEntity, BOOL addInitialSeperation, int rotOrder, float invMassScaleA, float invMassScaleB) {
			Invoker::Invoke<void, 1951>(firstEntityIndex, secondEntityIndex, firstEntityBoneIndex, secondEntityBoneIndex, secondEntityOffsetX, secondEntityOffsetY, secondEntityOffsetZ, firstEntityOffsetX, firstEntityOffsetY, firstEntityOffsetZ, vecRotationX, vecRotationY, vecRotationZ, physicalStrength, constrainRotation, doInitialWarp, collideWithEntity, addInitialSeperation, rotOrder, invMassScaleA, invMassScaleB);
		}

		void PROCESS_ENTITY_ATTACHMENTS(Entity entity) {
			Invoker::Invoke<void, 1952>(entity);
		}

		int GET_ENTITY_BONE_INDEX_BY_NAME(Entity entity, const char* boneName) {
			return Invoker::Invoke<int, 1953>(entity, boneName);
		}

		void CLEAR_ENTITY_LAST_DAMAGE_ENTITY(Entity entity) {
			Invoker::Invoke<void, 1954>(entity);
		}

		void DELETE_ENTITY(Entity* entity) {
			Invoker::Invoke<void, 1955>(entity);
		}

		void DETACH_ENTITY(Entity entity, BOOL dynamic, BOOL collision) {
			Invoker::Invoke<void, 1956>(entity, dynamic, collision);
		}

		void FREEZE_ENTITY_POSITION(Entity entity, BOOL toggle) {
			Invoker::Invoke<void, 1957>(entity, toggle);
		}

		void SET_ENTITY_SHOULD_FREEZE_WAITING_ON_COLLISION(Entity entity, BOOL toggle) {
			Invoker::Invoke<void, 1958>(entity, toggle);
		}

		BOOL PLAY_ENTITY_ANIM(Entity entity, const char* animName, const char* animDict, float p3, BOOL loop, BOOL stayInAnim, BOOL p6, float delta, Any bitset) {
			return Invoker::Invoke<BOOL, 1959>(entity, animName, animDict, p3, loop, stayInAnim, p6, delta, bitset);
		}

		BOOL PLAY_SYNCHRONIZED_ENTITY_ANIM(Entity entity, int syncedScene, const char* animation, const char* propName, float p4, float p5, Any p6, float p7) {
			return Invoker::Invoke<BOOL, 1960>(entity, syncedScene, animation, propName, p4, p5, p6, p7);
		}

		BOOL PLAY_SYNCHRONIZED_MAP_ENTITY_ANIM(float x1, float y1, float z1, float x2, Any y2, float z2, const char* p6, const char* p7, float p8, float p9, Any p10, float p11) {
			return Invoker::Invoke<BOOL, 1961>(x1, y1, z1, x2, y2, z2, p6, p7, p8, p9, p10, p11);
		}

		BOOL STOP_SYNCHRONIZED_MAP_ENTITY_ANIM(float x1, float y1, float z1, float x2, Any y2, float z2) {
			return Invoker::Invoke<BOOL, 1962>(x1, y1, z1, x2, y2, z2);
		}

		BOOL STOP_ENTITY_ANIM(Entity entity, const char* animation, const char* animGroup, float p3) {
			return Invoker::Invoke<BOOL, 1963>(entity, animation, animGroup, p3);
		}

		BOOL STOP_SYNCHRONIZED_ENTITY_ANIM(Entity entity, float p1, BOOL p2) {
			return Invoker::Invoke<BOOL, 1964>(entity, p1, p2);
		}

		BOOL HAS_ANIM_EVENT_FIRED(Entity entity, Hash actionHash) {
			return Invoker::Invoke<BOOL, 1965>(entity, actionHash);
		}

		BOOL FIND_ANIM_EVENT_PHASE(const char* animDictionary, const char* animName, const char* p2, Any* p3, Any* p4) {
			return Invoker::Invoke<BOOL, 1966>(animDictionary, animName, p2, p3, p4);
		}

		void SET_ENTITY_ANIM_CURRENT_TIME(Entity entity, const char* animDictionary, const char* animName, float time) {
			Invoker::Invoke<void, 1967>(entity, animDictionary, animName, time);
		}

		void SET_ENTITY_ANIM_SPEED(Entity entity, const char* animDictionary, const char* animName, float speedMultiplier) {
			Invoker::Invoke<void, 1968>(entity, animDictionary, animName, speedMultiplier);
		}

		void SET_ENTITY_AS_MISSION_ENTITY(Entity entity, BOOL p1, BOOL p2) {
			Invoker::Invoke<void, 1969>(entity, p1, p2);
		}

		void SET_ENTITY_AS_NO_LONGER_NEEDED(Entity* entity) {
			Invoker::Invoke<void, 1970>(entity);
		}

		void SET_PED_AS_NO_LONGER_NEEDED(Ped* ped) {
			Invoker::Invoke<void, 1971>(ped);
		}

		void SET_VEHICLE_AS_NO_LONGER_NEEDED(Vehicle* vehicle) {
			Invoker::Invoke<void, 1972>(vehicle);
		}

		void SET_OBJECT_AS_NO_LONGER_NEEDED(Object* object) {
			Invoker::Invoke<void, 1973>(object);
		}

		void SET_ENTITY_CAN_BE_DAMAGED(Entity entity, BOOL toggle) {
			Invoker::Invoke<void, 1974>(entity, toggle);
		}

		BOOL GET_ENTITY_CAN_BE_DAMAGED(Entity entity) {
			return Invoker::Invoke<BOOL, 1975>(entity);
		}

		void SET_ENTITY_CAN_BE_DAMAGED_BY_RELATIONSHIP_GROUP(Entity entity, BOOL bCanBeDamaged, int relGroup) {
			Invoker::Invoke<void, 1976>(entity, bCanBeDamaged, relGroup);
		}

		void SET_ENTITY_CAN_ONLY_BE_DAMAGED_BY_SCRIPT_PARTICIPANTS(Entity entity, BOOL toggle) {
			Invoker::Invoke<void, 1977>(entity, toggle);
		}

		void SET_ENTITY_CAN_BE_TARGETED_WITHOUT_LOS(Entity entity, BOOL toggle) {
			Invoker::Invoke<void, 1978>(entity, toggle);
		}

		void SET_ENTITY_COLLISION(Entity entity, BOOL toggle, BOOL keepPhysics) {
			Invoker::Invoke<void, 1979>(entity, toggle, keepPhysics);
		}

		BOOL GET_ENTITY_COLLISION_DISABLED(Entity entity) {
			return Invoker::Invoke<BOOL, 1980>(entity);
		}

		void SET_ENTITY_COMPLETELY_DISABLE_COLLISION(Entity entity, BOOL toggle, BOOL keepPhysics) {
			Invoker::Invoke<void, 1981>(entity, toggle, keepPhysics);
		}

		void SET_ENTITY_COORDS(Entity entity, float xPos, float yPos, float zPos, BOOL xAxis, BOOL yAxis, BOOL zAxis, BOOL clearArea) {
			Invoker::Invoke<void, 1982>(entity, xPos, yPos, zPos, xAxis, yAxis, zAxis, clearArea);
		}

		void SET_ENTITY_COORDS_WITHOUT_PLANTS_RESET(Entity entity, float xPos, float yPos, float zPos, BOOL alive, BOOL deadFlag, BOOL ragdollFlag, BOOL clearArea) {
			Invoker::Invoke<void, 1983>(entity, xPos, yPos, zPos, alive, deadFlag, ragdollFlag, clearArea);
		}

		void SET_ENTITY_COORDS_NO_OFFSET(Entity entity, float xPos, float yPos, float zPos, BOOL xAxis, BOOL yAxis, BOOL zAxis) {
			Invoker::Invoke<void, 1984>(entity, xPos, yPos, zPos, xAxis, yAxis, zAxis);
		}

		void SET_ENTITY_DYNAMIC(Entity entity, BOOL toggle) {
			Invoker::Invoke<void, 1985>(entity, toggle);
		}

		void SET_ENTITY_HEADING(Entity entity, float heading) {
			Invoker::Invoke<void, 1986>(entity, heading);
		}

		void SET_ENTITY_HEALTH(Entity entity, int health, Entity instigator, Hash weaponType) {
			Invoker::Invoke<void, 1987>(entity, health, instigator, weaponType);
		}

		void SET_ENTITY_INVINCIBLE(Entity entity, BOOL toggle, BOOL dontResetOnCleanup) {
			Invoker::Invoke<void, 1988>(entity, toggle, dontResetOnCleanup);
		}

		void SET_ENTITY_IS_TARGET_PRIORITY(Entity entity, BOOL p1, float p2) {
			Invoker::Invoke<void, 1989>(entity, p1, p2);
		}

		void SET_ENTITY_LIGHTS(Entity entity, BOOL toggle) {
			Invoker::Invoke<void, 1990>(entity, toggle);
		}

		void SET_ENTITY_LOAD_COLLISION_FLAG(Entity entity, BOOL toggle, Any p2) {
			Invoker::Invoke<void, 1991>(entity, toggle, p2);
		}

		BOOL HAS_COLLISION_LOADED_AROUND_ENTITY(Entity entity) {
			return Invoker::Invoke<BOOL, 1992>(entity);
		}

		void SET_ENTITY_MAX_SPEED(Entity entity, float speed) {
			Invoker::Invoke<void, 1993>(entity, speed);
		}

		void SET_ENTITY_ONLY_DAMAGED_BY_PLAYER(Entity entity, BOOL toggle) {
			Invoker::Invoke<void, 1994>(entity, toggle);
		}

		void SET_ENTITY_ONLY_DAMAGED_BY_RELATIONSHIP_GROUP(Entity entity, BOOL p1, Any p2) {
			Invoker::Invoke<void, 1995>(entity, p1, p2);
		}

		void SET_ENTITY_PROOFS(Entity entity, BOOL bulletProof, BOOL fireProof, BOOL explosionProof, BOOL collisionProof, BOOL meleeProof, BOOL steamProof, BOOL dontResetOnCleanup, BOOL waterProof) {
			Invoker::Invoke<void, 1996>(entity, bulletProof, fireProof, explosionProof, collisionProof, meleeProof, steamProof, dontResetOnCleanup, waterProof);
		}

		BOOL GET_ENTITY_PROOFS(Entity entity, BOOL* bulletProof, BOOL* fireProof, BOOL* explosionProof, BOOL* collisionProof, BOOL* meleeProof, BOOL* steamProof, BOOL* p7, BOOL* drownProof) {
			return Invoker::Invoke<BOOL, 1997>(entity, bulletProof, fireProof, explosionProof, collisionProof, meleeProof, steamProof, p7, drownProof);
		}

		void SET_ENTITY_QUATERNION(Entity entity, float x, float y, float z, float w) {
			Invoker::Invoke<void, 1998>(entity, x, y, z, w);
		}

		void SET_ENTITY_RECORDS_COLLISIONS(Entity entity, BOOL toggle) {
			Invoker::Invoke<void, 1999>(entity, toggle);
		}

		void SET_ENTITY_ROTATION(Entity entity, float pitch, float roll, float yaw, int rotationOrder, BOOL p5) {
			Invoker::Invoke<void, 2000>(entity, pitch, roll, yaw, rotationOrder, p5);
		}

		void SET_ENTITY_VISIBLE(Entity entity, BOOL toggle, BOOL p2) {
			Invoker::Invoke<void, 2001>(entity, toggle, p2);
		}

		void SET_ENTITY_WATER_REFLECTION_FLAG(Entity entity, BOOL toggle) {
			Invoker::Invoke<void, 2002>(entity, toggle);
		}

		void SET_ENTITY_MIRROR_REFLECTION_FLAG(Entity entity, BOOL p1) {
			Invoker::Invoke<void, 2003>(entity, p1);
		}

		void SET_ENTITY_VELOCITY(Entity entity, float x, float y, float z) {
			Invoker::Invoke<void, 2004>(entity, x, y, z);
		}

		void SET_ENTITY_ANGULAR_VELOCITY(Entity entity, float x, float y, float z) {
			Invoker::Invoke<void, 2005>(entity, x, y, z);
		}

		void SET_ENTITY_HAS_GRAVITY(Entity entity, BOOL toggle) {
			Invoker::Invoke<void, 2006>(entity, toggle);
		}

		void SET_ENTITY_LOD_DIST(Entity entity, int value) {
			Invoker::Invoke<void, 2007>(entity, value);
		}

		int GET_ENTITY_LOD_DIST(Entity entity) {
			return Invoker::Invoke<int, 2008>(entity);
		}

		void SET_ENTITY_ALPHA(Entity entity, int alphaLevel, BOOL skin) {
			Invoker::Invoke<void, 2009>(entity, alphaLevel, skin);
		}

		int GET_ENTITY_ALPHA(Entity entity) {
			return Invoker::Invoke<int, 2010>(entity);
		}

		void RESET_ENTITY_ALPHA(Entity entity) {
			Invoker::Invoke<void, 2011>(entity);
		}

		void RESET_PICKUP_ENTITY_GLOW(Entity entity) {
			Invoker::Invoke<void, 2012>(entity);
		}

		void SET_PICKUP_COLLIDES_WITH_PROJECTILES(Any p0, Any p1) {
			Invoker::Invoke<void, 2013>(p0, p1);
		}

		void SET_ENTITY_SORT_BIAS(Entity entity, float p1) {
			Invoker::Invoke<void, 2014>(entity, p1);
		}

		void SET_ENTITY_ALWAYS_PRERENDER(Entity entity, BOOL toggle) {
			Invoker::Invoke<void, 2015>(entity, toggle);
		}

		void SET_ENTITY_RENDER_SCORCHED(Entity entity, BOOL toggle) {
			Invoker::Invoke<void, 2016>(entity, toggle);
		}

		void SET_ENTITY_TRAFFICLIGHT_OVERRIDE(Entity entity, int state) {
			Invoker::Invoke<void, 2017>(entity, state);
		}

		void SET_ENTITY_IS_IN_VEHICLE(Entity entity) {
			Invoker::Invoke<void, 2018>(entity);
		}

		void CREATE_MODEL_SWAP(float x, float y, float z, float radius, Hash originalModel, Hash newModel, BOOL p6) {
			Invoker::Invoke<void, 2019>(x, y, z, radius, originalModel, newModel, p6);
		}

		void REMOVE_MODEL_SWAP(float x, float y, float z, float radius, Hash originalModel, Hash newModel, BOOL p6) {
			Invoker::Invoke<void, 2020>(x, y, z, radius, originalModel, newModel, p6);
		}

		void CREATE_MODEL_HIDE(float x, float y, float z, float radius, Hash modelHash, BOOL p5) {
			Invoker::Invoke<void, 2021>(x, y, z, radius, modelHash, p5);
		}

		void CREATE_MODEL_HIDE_EXCLUDING_SCRIPT_OBJECTS(float x, float y, float z, float radius, Hash modelHash, BOOL p5) {
			Invoker::Invoke<void, 2022>(x, y, z, radius, modelHash, p5);
		}

		void REMOVE_MODEL_HIDE(float x, float y, float z, float radius, Hash modelHash, BOOL p5) {
			Invoker::Invoke<void, 2023>(x, y, z, radius, modelHash, p5);
		}

		void CREATE_FORCED_OBJECT(float x, float y, float z, Any p3, Hash modelHash, BOOL p5) {
			Invoker::Invoke<void, 2024>(x, y, z, p3, modelHash, p5);
		}

		void REMOVE_FORCED_OBJECT(float x, float y, float z, float p3, Hash modelHash) {
			Invoker::Invoke<void, 2025>(x, y, z, p3, modelHash);
		}

		void SET_ENTITY_NO_COLLISION_ENTITY(Entity entity1, Entity entity2, BOOL thisFrameOnly) {
			Invoker::Invoke<void, 2026>(entity1, entity2, thisFrameOnly);
		}

		void _SET_ENTITY_NO_COLLISION_WITH_NETWORKED_ENTITY(Entity entity1, Entity entity2) {
			Invoker::Invoke<void, 6696>(entity1, entity2);
		}

		void SET_ENTITY_MOTION_BLUR(Entity entity, BOOL toggle) {
			Invoker::Invoke<void, 2027>(entity, toggle);
		}

		void SET_CAN_AUTO_VAULT_ON_ENTITY(Entity entity, BOOL toggle) {
			Invoker::Invoke<void, 2028>(entity, toggle);
		}

		void SET_CAN_CLIMB_ON_ENTITY(Entity entity, BOOL toggle) {
			Invoker::Invoke<void, 2029>(entity, toggle);
		}

		void SET_WAIT_FOR_COLLISIONS_BEFORE_PROBE(Entity entity, BOOL toggle) {
			Invoker::Invoke<void, 2030>(entity, toggle);
		}

		void SET_ENTITY_NOWEAPONDECALS(Entity entity, BOOL p1) {
			Invoker::Invoke<void, 2031>(entity, p1);
		}

		void SET_ENTITY_USE_MAX_DISTANCE_FOR_WATER_REFLECTION(Entity entity, BOOL p1) {
			Invoker::Invoke<void, 2032>(entity, p1);
		}

		Vector3 GET_ENTITY_BONE_ROTATION(Entity entity, int boneIndex) {
			return Invoker::Invoke<Vector3, 2033>(entity, boneIndex);
		}

		Vector3 GET_ENTITY_BONE_POSTION(Entity entity, int boneIndex) {
			return Invoker::Invoke<Vector3, 2034>(entity, boneIndex);
		}

		Vector3 GET_ENTITY_BONE_OBJECT_ROTATION(Entity entity, int boneIndex) {
			return Invoker::Invoke<Vector3, 2035>(entity, boneIndex);
		}

		Vector3 GET_ENTITY_BONE_OBJECT_POSTION(Entity entity, int boneIndex) {
			return Invoker::Invoke<Vector3, 2036>(entity, boneIndex);
		}

		int GET_ENTITY_BONE_COUNT(Entity entity) {
			return Invoker::Invoke<int, 2037>(entity);
		}

		void ENABLE_ENTITY_BULLET_COLLISION(Entity entity) {
			Invoker::Invoke<void, 2038>(entity);
		}

		void SET_ENTITY_CAN_ONLY_BE_DAMAGED_BY_ENTITY(Entity entity1, Entity entity2) {
			Invoker::Invoke<void, 2039>(entity1, entity2);
		}

		void SET_ENTITY_CANT_CAUSE_COLLISION_DAMAGED_ENTITY(Entity entity1, Entity entity2) {
			Invoker::Invoke<void, 2040>(entity1, entity2);
		}

		void SET_ALLOW_MIGRATE_TO_SPECTATOR(Entity entity, Any p1) {
			Invoker::Invoke<void, 2041>(entity, p1);
		}

		Entity GET_ENTITY_OF_TYPE_ATTACHED_TO_ENTITY(Entity entity, Hash modelHash) {
			return Invoker::Invoke<Entity, 2042>(entity, modelHash);
		}

		void SET_PICK_UP_BY_CARGOBOB_DISABLED(Entity entity, BOOL toggle) {
			Invoker::Invoke<void, 2043>(entity, toggle);
		}

	} // namespace ENTITY

	namespace EVENT {

		void SET_DECISION_MAKER(Ped ped, Hash name) {
			Invoker::Invoke<void, 4808>(ped, name);
		}

		void CLEAR_DECISION_MAKER_EVENT_RESPONSE(Hash name, int eventType) {
			Invoker::Invoke<void, 4809>(name, eventType);
		}

		void BLOCK_DECISION_MAKER_EVENT(Hash name, int eventType) {
			Invoker::Invoke<void, 4810>(name, eventType);
		}

		void UNBLOCK_DECISION_MAKER_EVENT(Hash name, int eventType) {
			Invoker::Invoke<void, 4811>(name, eventType);
		}

		int ADD_SHOCKING_EVENT_AT_POSITION(int eventType, float x, float y, float z, float duration) {
			return Invoker::Invoke<int, 4812>(eventType, x, y, z, duration);
		}

		int ADD_SHOCKING_EVENT_FOR_ENTITY(int eventType, Entity entity, float duration) {
			return Invoker::Invoke<int, 4813>(eventType, entity, duration);
		}

		BOOL IS_SHOCKING_EVENT_IN_SPHERE(int eventType, float x, float y, float z, float radius) {
			return Invoker::Invoke<BOOL, 4814>(eventType, x, y, z, radius);
		}

		BOOL REMOVE_SHOCKING_EVENT(ScrHandle event) {
			return Invoker::Invoke<BOOL, 4815>(event);
		}

		void REMOVE_ALL_SHOCKING_EVENTS(BOOL p0) {
			Invoker::Invoke<void, 4816>(p0);
		}

		void REMOVE_SHOCKING_EVENT_SPAWN_BLOCKING_AREAS() {
			Invoker::Invoke<void, 4817>();
		}

		void SUPPRESS_SHOCKING_EVENTS_NEXT_FRAME() {
			Invoker::Invoke<void, 4818>();
		}

		void SUPPRESS_SHOCKING_EVENT_TYPE_NEXT_FRAME(int eventType) {
			Invoker::Invoke<void, 4819>(eventType);
		}

		void SUPPRESS_AGITATION_EVENTS_NEXT_FRAME() {
			Invoker::Invoke<void, 4820>();
		}

	} // namespace EVENT

	namespace EXTRAMETADATA {

		int GET_NUM_TATTOO_SHOP_DLC_ITEMS(int character) {
			return Invoker::Invoke<int, 3740>(character);
		}

		BOOL GET_TATTOO_SHOP_DLC_ITEM_DATA(int characterType, int decorationIndex, Any* outComponent) {
			return Invoker::Invoke<BOOL, 3741>(characterType, decorationIndex, outComponent);
		}

		int GET_TATTOO_SHOP_DLC_ITEM_INDEX(Hash overlayHash, Any p1, int character) {
			return Invoker::Invoke<int, 3742>(overlayHash, p1, character);
		}

		void INIT_SHOP_PED_COMPONENT(Any* outComponent) {
			Invoker::Invoke<void, 3743>(outComponent);
		}

		void INIT_SHOP_PED_PROP(Any* outProp) {
			Invoker::Invoke<void, 3744>(outProp);
		}

		int SETUP_SHOP_PED_APPAREL_QUERY(int p0, int p1, int p2, int p3) {
			return Invoker::Invoke<int, 3745>(p0, p1, p2, p3);
		}

		int SETUP_SHOP_PED_APPAREL_QUERY_TU(int character, int p1, int p2, BOOL p3, int p4, int componentId) {
			return Invoker::Invoke<int, 3746>(character, p1, p2, p3, p4, componentId);
		}

		void GET_SHOP_PED_QUERY_COMPONENT(int componentId, Any* outComponent) {
			Invoker::Invoke<void, 3747>(componentId, outComponent);
		}

		int GET_SHOP_PED_QUERY_COMPONENT_INDEX(Hash componentHash) {
			return Invoker::Invoke<int, 3748>(componentHash);
		}

		void GET_SHOP_PED_COMPONENT(Hash componentHash, Any* outComponent) {
			Invoker::Invoke<void, 3749>(componentHash, outComponent);
		}

		void GET_SHOP_PED_QUERY_PROP(int componentId, Any* outProp) {
			Invoker::Invoke<void, 3750>(componentId, outProp);
		}

		int GET_SHOP_PED_QUERY_PROP_INDEX(Hash componentHash) {
			return Invoker::Invoke<int, 3751>(componentHash);
		}

		void GET_SHOP_PED_PROP(Hash componentHash, Any* outProp) {
			Invoker::Invoke<void, 3752>(componentHash, outProp);
		}

		Hash GET_HASH_NAME_FOR_COMPONENT(Entity entity, int componentId, int drawableVariant, int textureVariant) {
			return Invoker::Invoke<Hash, 3753>(entity, componentId, drawableVariant, textureVariant);
		}

		Hash GET_HASH_NAME_FOR_PROP(Entity entity, int componentId, int propIndex, int propTextureIndex) {
			return Invoker::Invoke<Hash, 3754>(entity, componentId, propIndex, propTextureIndex);
		}

		int GET_SHOP_PED_APPAREL_VARIANT_COMPONENT_COUNT(Hash componentHash) {
			return Invoker::Invoke<int, 3755>(componentHash);
		}

		int GET_SHOP_PED_APPAREL_VARIANT_PROP_COUNT(Hash propHash) {
			return Invoker::Invoke<int, 3756>(propHash);
		}

		void GET_VARIANT_COMPONENT(Hash componentHash, int variantComponentIndex, Hash* nameHash, int* enumValue, int* componentType) {
			Invoker::Invoke<void, 3757>(componentHash, variantComponentIndex, nameHash, enumValue, componentType);
		}

		void GET_VARIANT_PROP(Hash componentHash, int variantPropIndex, Hash* nameHash, int* enumValue, int* anchorPoint) {
			Invoker::Invoke<void, 3758>(componentHash, variantPropIndex, nameHash, enumValue, anchorPoint);
		}

		int GET_SHOP_PED_APPAREL_FORCED_COMPONENT_COUNT(Hash componentHash) {
			return Invoker::Invoke<int, 3759>(componentHash);
		}

		int GET_SHOP_PED_APPAREL_FORCED_PROP_COUNT(Hash componentHash) {
			return Invoker::Invoke<int, 3760>(componentHash);
		}

		void GET_FORCED_COMPONENT(Hash componentHash, int forcedComponentIndex, Hash* nameHash, int* enumValue, int* componentType) {
			Invoker::Invoke<void, 3761>(componentHash, forcedComponentIndex, nameHash, enumValue, componentType);
		}

		void GET_FORCED_PROP(Hash componentHash, int forcedPropIndex, Hash* nameHash, int* enumValue, int* anchorPoint) {
			Invoker::Invoke<void, 3762>(componentHash, forcedPropIndex, nameHash, enumValue, anchorPoint);
		}

		BOOL DOES_SHOP_PED_APPAREL_HAVE_RESTRICTION_TAG(Hash componentHash, Hash restrictionTagHash, int componentId) {
			return Invoker::Invoke<BOOL, 3763>(componentHash, restrictionTagHash, componentId);
		}

		BOOL DOES_CURRENT_PED_COMPONENT_HAVE_RESTRICTION_TAG(Ped ped, int componentId, Hash restrictionTagHash) {
			return Invoker::Invoke<BOOL, 3764>(ped, componentId, restrictionTagHash);
		}

		BOOL DOES_CURRENT_PED_PROP_HAVE_RESTRICTION_TAG(Ped ped, int componentId, Hash restrictionTagHash) {
			return Invoker::Invoke<BOOL, 3765>(ped, componentId, restrictionTagHash);
		}

		int SETUP_SHOP_PED_OUTFIT_QUERY(int character, BOOL p1) {
			return Invoker::Invoke<int, 3766>(character, p1);
		}

		void GET_SHOP_PED_QUERY_OUTFIT(int outfitIndex, Any* outfit) {
			Invoker::Invoke<void, 3767>(outfitIndex, outfit);
		}

		void GET_SHOP_PED_OUTFIT(Any p0, Any* p1) {
			Invoker::Invoke<void, 3768>(p0, p1);
		}

		int GET_SHOP_PED_OUTFIT_LOCATE(Any p0) {
			return Invoker::Invoke<int, 3769>(p0);
		}

		BOOL GET_SHOP_PED_OUTFIT_PROP_VARIANT(Hash outfitHash, int variantIndex, Any* outPropVariant) {
			return Invoker::Invoke<BOOL, 3770>(outfitHash, variantIndex, outPropVariant);
		}

		BOOL GET_SHOP_PED_OUTFIT_COMPONENT_VARIANT(Hash outfitHash, int variantIndex, Any* outComponentVariant) {
			return Invoker::Invoke<BOOL, 3771>(outfitHash, variantIndex, outComponentVariant);
		}

		int GET_NUM_DLC_VEHICLES() {
			return Invoker::Invoke<int, 3772>();
		}

		Hash GET_DLC_VEHICLE_MODEL(int dlcVehicleIndex) {
			return Invoker::Invoke<Hash, 3773>(dlcVehicleIndex);
		}

		BOOL GET_DLC_VEHICLE_DATA(int dlcVehicleIndex, Any* outData) {
			return Invoker::Invoke<BOOL, 3774>(dlcVehicleIndex, outData);
		}

		int GET_DLC_VEHICLE_FLAGS(int dlcVehicleIndex) {
			return Invoker::Invoke<int, 3775>(dlcVehicleIndex);
		}

		int GET_NUM_DLC_WEAPONS() {
			return Invoker::Invoke<int, 3776>();
		}

		int GET_NUM_DLC_WEAPONS_SP() {
			return Invoker::Invoke<int, 3777>();
		}

		BOOL GET_DLC_WEAPON_DATA(int dlcWeaponIndex, Any* outData) {
			return Invoker::Invoke<BOOL, 3778>(dlcWeaponIndex, outData);
		}

		BOOL GET_DLC_WEAPON_DATA_SP(int dlcWeaponIndex, Any* outData) {
			return Invoker::Invoke<BOOL, 3779>(dlcWeaponIndex, outData);
		}

		int GET_NUM_DLC_WEAPON_COMPONENTS(int dlcWeaponIndex) {
			return Invoker::Invoke<int, 3780>(dlcWeaponIndex);
		}

		int GET_NUM_DLC_WEAPON_COMPONENTS_SP(int dlcWeaponIndex) {
			return Invoker::Invoke<int, 3781>(dlcWeaponIndex);
		}

		BOOL GET_DLC_WEAPON_COMPONENT_DATA(int dlcWeaponIndex, int dlcWeapCompIndex, Any* ComponentDataPtr) {
			return Invoker::Invoke<BOOL, 3782>(dlcWeaponIndex, dlcWeapCompIndex, ComponentDataPtr);
		}

		BOOL GET_DLC_WEAPON_COMPONENT_DATA_SP(int dlcWeaponIndex, int dlcWeapCompIndex, Any* ComponentDataPtr) {
			return Invoker::Invoke<BOOL, 3783>(dlcWeaponIndex, dlcWeapCompIndex, ComponentDataPtr);
		}

		BOOL IS_CONTENT_ITEM_LOCKED(Hash itemHash) {
			return Invoker::Invoke<BOOL, 3784>(itemHash);
		}

		BOOL IS_DLC_VEHICLE_MOD(Hash hash) {
			return Invoker::Invoke<BOOL, 3785>(hash);
		}

		Hash GET_DLC_VEHICLE_MOD_LOCK_HASH(Hash hash) {
			return Invoker::Invoke<Hash, 3786>(hash);
		}

		void EXECUTE_CONTENT_CHANGESET_GROUP_FOR_ALL(Hash hash) {
			Invoker::Invoke<void, 3787>(hash);
		}

		void REVERT_CONTENT_CHANGESET_GROUP_FOR_ALL(Hash hash) {
			Invoker::Invoke<void, 3788>(hash);
		}

	} // namespace EXTRAMETADATA

	namespace FIRE {

		FireId START_SCRIPT_FIRE(float X, float Y, float Z, int maxChildren, BOOL isGasFire) {
			return Invoker::Invoke<FireId, 1029>(X, Y, Z, maxChildren, isGasFire);
		}

		void REMOVE_SCRIPT_FIRE(FireId fireHandle) {
			Invoker::Invoke<void, 1030>(fireHandle);
		}

		FireId START_ENTITY_FIRE(Entity entity) {
			return Invoker::Invoke<FireId, 1031>(entity);
		}

		void STOP_ENTITY_FIRE(Entity entity) {
			Invoker::Invoke<void, 1032>(entity);
		}

		BOOL IS_ENTITY_ON_FIRE(Entity entity) {
			return Invoker::Invoke<BOOL, 1033>(entity);
		}

		int GET_NUMBER_OF_FIRES_IN_RANGE(float x, float y, float z, float radius) {
			return Invoker::Invoke<int, 1034>(x, y, z, radius);
		}

		void SET_FLAMMABILITY_MULTIPLIER(float p0) {
			Invoker::Invoke<void, 1035>(p0);
		}

		void STOP_FIRE_IN_RANGE(float x, float y, float z, float radius) {
			Invoker::Invoke<void, 1036>(x, y, z, radius);
		}

		BOOL GET_CLOSEST_FIRE_POS(Vector3* outPosition, float x, float y, float z) {
			return Invoker::Invoke<BOOL, 1037>(outPosition, x, y, z);
		}

		void ADD_EXPLOSION(float x, float y, float z, int explosionType, float damageScale, BOOL isAudible, BOOL isInvisible, float cameraShake, BOOL noDamage) {
			Invoker::Invoke<void, 1038>(x, y, z, explosionType, damageScale, isAudible, isInvisible, cameraShake, noDamage);
		}

		void ADD_OWNED_EXPLOSION(Ped ped, float x, float y, float z, int explosionType, float damageScale, BOOL isAudible, BOOL isInvisible, float cameraShake) {
			Invoker::Invoke<void, 1039>(ped, x, y, z, explosionType, damageScale, isAudible, isInvisible, cameraShake);
		}

		void ADD_EXPLOSION_WITH_USER_VFX(float x, float y, float z, int explosionType, Hash explosionFx, float damageScale, BOOL isAudible, BOOL isInvisible, float cameraShake) {
			Invoker::Invoke<void, 1040>(x, y, z, explosionType, explosionFx, damageScale, isAudible, isInvisible, cameraShake);
		}

		BOOL IS_EXPLOSION_IN_AREA(int explosionType, float x1, float y1, float z1, float x2, float y2, float z2) {
			return Invoker::Invoke<BOOL, 1041>(explosionType, x1, y1, z1, x2, y2, z2);
		}

		BOOL IS_EXPLOSION_ACTIVE_IN_AREA(int explosionType, float x1, float y1, float z1, float x2, float y2, float z2) {
			return Invoker::Invoke<BOOL, 1042>(explosionType, x1, y1, z1, x2, y2, z2);
		}

		BOOL IS_EXPLOSION_IN_SPHERE(int explosionType, float x, float y, float z, float radius) {
			return Invoker::Invoke<BOOL, 1043>(explosionType, x, y, z, radius);
		}

		Entity GET_OWNER_OF_EXPLOSION_IN_SPHERE(int explosionType, float x, float y, float z, float radius) {
			return Invoker::Invoke<Entity, 1044>(explosionType, x, y, z, radius);
		}

		BOOL IS_EXPLOSION_IN_ANGLED_AREA(int explosionType, float x1, float y1, float z1, float x2, float y2, float z2, float width) {
			return Invoker::Invoke<BOOL, 1045>(explosionType, x1, y1, z1, x2, y2, z2, width);
		}

		Entity GET_OWNER_OF_EXPLOSION_IN_ANGLED_AREA(int explosionType, float x1, float y1, float z1, float x2, float y2, float z2, float radius) {
			return Invoker::Invoke<Entity, 1046>(explosionType, x1, y1, z1, x2, y2, z2, radius);
		}

		void _NETWORK_EXPECT_EXPLOSION_EVENTS_FOR_PLAYER(BOOL expect, Player player) {
			Invoker::Invoke<void, 6695>(expect, player);
		}

		int _GET_MAXIMUM_NUMBER_OF_WATER_CANNONS() {
			return Invoker::Invoke<int, 6694>();
		}

		Vector3 _GET_WATER_CANNON_COORDS(int index) {
			return Invoker::Invoke<Vector3, 6693>(index);
		}

	} // namespace FIRE

	namespace GRAPHICS {

		void SET_DEBUG_LINES_AND_SPHERES_DRAWING_ACTIVE(BOOL enabled) {
			Invoker::Invoke<void, 1156>(enabled);
		}

		void DRAW_DEBUG_LINE(float x1, float y1, float z1, float x2, float y2, float z2, int r, int g, int b, int alpha) {
			Invoker::Invoke<void, 1157>(x1, y1, z1, x2, y2, z2, r, g, b, alpha);
		}

		void DRAW_DEBUG_LINE_WITH_TWO_COLOURS(float x1, float y1, float z1, float x2, float y2, float z2, int r1, int g1, int b1, int r2, int g2, int b2, int alpha1, int alpha2) {
			Invoker::Invoke<void, 1158>(x1, y1, z1, x2, y2, z2, r1, g1, b1, r2, g2, b2, alpha1, alpha2);
		}

		void DRAW_DEBUG_SPHERE(float x, float y, float z, float radius, int red, int green, int blue, int alpha) {
			Invoker::Invoke<void, 1159>(x, y, z, radius, red, green, blue, alpha);
		}

		void DRAW_DEBUG_BOX(float x1, float y1, float z1, float x2, float y2, float z2, int r, int g, int b, int alpha) {
			Invoker::Invoke<void, 1160>(x1, y1, z1, x2, y2, z2, r, g, b, alpha);
		}

		void DRAW_DEBUG_CROSS(float x, float y, float z, float size, int red, int green, int blue, int alpha) {
			Invoker::Invoke<void, 1161>(x, y, z, size, red, green, blue, alpha);
		}

		void DRAW_DEBUG_TEXT(const char* text, float x, float y, float z, int red, int green, int blue, int alpha) {
			Invoker::Invoke<void, 1162>(text, x, y, z, red, green, blue, alpha);
		}

		void DRAW_DEBUG_TEXT_2D(const char* text, float x, float y, float z, int red, int green, int blue, int alpha) {
			Invoker::Invoke<void, 1163>(text, x, y, z, red, green, blue, alpha);
		}

		void DRAW_LINE(float x1, float y1, float z1, float x2, float y2, float z2, int red, int green, int blue, int alpha) {
			Invoker::Invoke<void, 1164>(x1, y1, z1, x2, y2, z2, red, green, blue, alpha);
		}

		void DRAW_POLY(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3, int red, int green, int blue, int alpha) {
			Invoker::Invoke<void, 1165>(x1, y1, z1, x2, y2, z2, x3, y3, z3, red, green, blue, alpha);
		}

		void DRAW_TEXTURED_POLY(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3, int red, int green, int blue, int alpha, const char* textureDict, const char* textureName, float u1, float v1, float w1, float u2, float v2, float w2, float u3, float v3, float w3) {
			Invoker::Invoke<void, 1166>(x1, y1, z1, x2, y2, z2, x3, y3, z3, red, green, blue, alpha, textureDict, textureName, u1, v1, w1, u2, v2, w2, u3, v3, w3);
		}

		void DRAW_TEXTURED_POLY_WITH_THREE_COLOURS(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3, float red1, float green1, float blue1, int alpha1, float red2, float green2, float blue2, int alpha2, float red3, float green3, float blue3, int alpha3, const char* textureDict, const char* textureName, float u1, float v1, float w1, float u2, float v2, float w2, float u3, float v3, float w3) {
			Invoker::Invoke<void, 1167>(x1, y1, z1, x2, y2, z2, x3, y3, z3, red1, green1, blue1, alpha1, red2, green2, blue2, alpha2, red3, green3, blue3, alpha3, textureDict, textureName, u1, v1, w1, u2, v2, w2, u3, v3, w3);
		}

		void DRAW_BOX(float x1, float y1, float z1, float x2, float y2, float z2, int red, int green, int blue, int alpha) {
			Invoker::Invoke<void, 1168>(x1, y1, z1, x2, y2, z2, red, green, blue, alpha);
		}

		void SET_BACKFACECULLING(BOOL toggle) {
			Invoker::Invoke<void, 1169>(toggle);
		}

		void SET_DEPTHWRITING(BOOL toggle) {
			Invoker::Invoke<void, 1170>(toggle);
		}

		BOOL BEGIN_TAKE_MISSION_CREATOR_PHOTO() {
			return Invoker::Invoke<BOOL, 1171>();
		}

		int GET_STATUS_OF_TAKE_MISSION_CREATOR_PHOTO() {
			return Invoker::Invoke<int, 1172>();
		}

		void FREE_MEMORY_FOR_MISSION_CREATOR_PHOTO() {
			Invoker::Invoke<void, 1173>();
		}

		BOOL LOAD_MISSION_CREATOR_PHOTO(Any* p0, Any p1, Any p2, Any p3) {
			return Invoker::Invoke<BOOL, 1174>(p0, p1, p2, p3);
		}

		int GET_STATUS_OF_LOAD_MISSION_CREATOR_PHOTO(Any* p0) {
			return Invoker::Invoke<int, 1175>(p0);
		}

		BOOL BEGIN_CREATE_MISSION_CREATOR_PHOTO_PREVIEW() {
			return Invoker::Invoke<BOOL, 1176>();
		}

		int GET_STATUS_OF_CREATE_MISSION_CREATOR_PHOTO_PREVIEW() {
			return Invoker::Invoke<int, 1177>();
		}

		void FREE_MEMORY_FOR_MISSION_CREATOR_PHOTO_PREVIEW() {
			Invoker::Invoke<void, 1178>();
		}

		BOOL BEGIN_TAKE_HIGH_QUALITY_PHOTO() {
			return Invoker::Invoke<BOOL, 1179>();
		}

		int GET_STATUS_OF_TAKE_HIGH_QUALITY_PHOTO() {
			return Invoker::Invoke<int, 1180>();
		}

		void FREE_MEMORY_FOR_HIGH_QUALITY_PHOTO() {
			Invoker::Invoke<void, 1181>();
		}

		void SET_TAKEN_PHOTO_IS_MUGSHOT(BOOL toggle) {
			Invoker::Invoke<void, 1182>(toggle);
		}

		void SET_ARENA_THEME_AND_VARIATION_FOR_TAKEN_PHOTO(Any p0, int p1) {
			Invoker::Invoke<void, 1183>(p0, p1);
		}

		void SET_ON_ISLAND_X_FOR_TAKEN_PHOTO(Any p0) {
			Invoker::Invoke<void, 1184>(p0);
		}

		BOOL SAVE_HIGH_QUALITY_PHOTO(int unused) {
			return Invoker::Invoke<BOOL, 1185>(unused);
		}

		int GET_STATUS_OF_SAVE_HIGH_QUALITY_PHOTO() {
			return Invoker::Invoke<int, 1186>();
		}

		BOOL BEGIN_CREATE_LOW_QUALITY_COPY_OF_PHOTO(Any p0) {
			return Invoker::Invoke<BOOL, 1187>(p0);
		}

		int GET_STATUS_OF_CREATE_LOW_QUALITY_COPY_OF_PHOTO(int p0) {
			return Invoker::Invoke<int, 1188>(p0);
		}

		void FREE_MEMORY_FOR_LOW_QUALITY_PHOTO() {
			Invoker::Invoke<void, 1189>();
		}

		void DRAW_LOW_QUALITY_PHOTO_TO_PHONE(BOOL p0, BOOL p1) {
			Invoker::Invoke<void, 1190>(p0, p1);
		}

		int GET_MAXIMUM_NUMBER_OF_PHOTOS() {
			return Invoker::Invoke<int, 1191>();
		}

		int GET_MAXIMUM_NUMBER_OF_CLOUD_PHOTOS() {
			return Invoker::Invoke<int, 1192>();
		}

		int GET_CURRENT_NUMBER_OF_CLOUD_PHOTOS() {
			return Invoker::Invoke<int, 1193>();
		}

		BOOL QUEUE_OPERATION_TO_CREATE_SORTED_LIST_OF_PHOTOS(Any p0) {
			return Invoker::Invoke<BOOL, 1194>(p0);
		}

		int GET_STATUS_OF_SORTED_LIST_OPERATION(Any p0) {
			return Invoker::Invoke<int, 1195>(p0);
		}

		void CLEAR_STATUS_OF_SORTED_LIST_OPERATION() {
			Invoker::Invoke<void, 1196>();
		}

		BOOL DOES_THIS_PHOTO_SLOT_CONTAIN_A_VALID_PHOTO(Any p0) {
			return Invoker::Invoke<BOOL, 1197>(p0);
		}

		BOOL LOAD_HIGH_QUALITY_PHOTO(Any p0) {
			return Invoker::Invoke<BOOL, 1198>(p0);
		}

		int GET_LOAD_HIGH_QUALITY_PHOTO_STATUS(int p0) {
			return Invoker::Invoke<int, 1199>(p0);
		}

		void DRAW_LIGHT_WITH_RANGEEX(float x, float y, float z, int r, int g, int b, float range, float intensity, float shadow) {
			Invoker::Invoke<void, 1200>(x, y, z, r, g, b, range, intensity, shadow);
		}

		void DRAW_LIGHT_WITH_RANGE(float posX, float posY, float posZ, int colorR, int colorG, int colorB, float range, float intensity) {
			Invoker::Invoke<void, 1201>(posX, posY, posZ, colorR, colorG, colorB, range, intensity);
		}

		void DRAW_SPOT_LIGHT(float posX, float posY, float posZ, float dirX, float dirY, float dirZ, int colorR, int colorG, int colorB, float distance, float brightness, float hardness, float radius, float falloff) {
			Invoker::Invoke<void, 1202>(posX, posY, posZ, dirX, dirY, dirZ, colorR, colorG, colorB, distance, brightness, hardness, radius, falloff);
		}

		void DRAW_SHADOWED_SPOT_LIGHT(float posX, float posY, float posZ, float dirX, float dirY, float dirZ, int colorR, int colorG, int colorB, float distance, float brightness, float roundness, float radius, float falloff, int shadowId) {
			Invoker::Invoke<void, 1203>(posX, posY, posZ, dirX, dirY, dirZ, colorR, colorG, colorB, distance, brightness, roundness, radius, falloff, shadowId);
		}

		void FADE_UP_PED_LIGHT(float p0) {
			Invoker::Invoke<void, 1204>(p0);
		}

		void UPDATE_LIGHTS_ON_ENTITY(Entity entity) {
			Invoker::Invoke<void, 1205>(entity);
		}

		void SET_LIGHT_OVERRIDE_MAX_INTENSITY_SCALE(Any p0) {
			Invoker::Invoke<void, 1206>(p0);
		}

		float GET_LIGHT_OVERRIDE_MAX_INTENSITY_SCALE() {
			return Invoker::Invoke<float, 1207>();
		}

		void DRAW_MARKER(int type, float posX, float posY, float posZ, float dirX, float dirY, float dirZ, float rotX, float rotY, float rotZ, float scaleX, float scaleY, float scaleZ, int red, int green, int blue, int alpha, BOOL bobUpAndDown, BOOL faceCamera, int p19, BOOL rotate, const char* textureDict, const char* textureName, BOOL drawOnEnts) {
			Invoker::Invoke<void, 1208>(type, posX, posY, posZ, dirX, dirY, dirZ, rotX, rotY, rotZ, scaleX, scaleY, scaleZ, red, green, blue, alpha, bobUpAndDown, faceCamera, p19, rotate, textureDict, textureName, drawOnEnts);
		}

		void DRAW_MARKER_EX(int type, float posX, float posY, float posZ, float dirX, float dirY, float dirZ, float rotX, float rotY, float rotZ, float scaleX, float scaleY, float scaleZ, int red, int green, int blue, int alpha, BOOL bobUpAndDown, BOOL faceCamera, Any p19, BOOL rotate, const char* textureDict, const char* textureName, BOOL drawOnEnts, BOOL p24, BOOL p25) {
			Invoker::Invoke<void, 1209>(type, posX, posY, posZ, dirX, dirY, dirZ, rotX, rotY, rotZ, scaleX, scaleY, scaleZ, red, green, blue, alpha, bobUpAndDown, faceCamera, p19, rotate, textureDict, textureName, drawOnEnts, p24, p25);
		}

		void DRAW_MARKER_SPHERE(float x, float y, float z, float radius, int red, int green, int blue, float alpha) {
			Invoker::Invoke<void, 1210>(x, y, z, radius, red, green, blue, alpha);
		}

		int CREATE_CHECKPOINT(int type, float posX1, float posY1, float posZ1, float posX2, float posY2, float posZ2, float diameter, int red, int green, int blue, int alpha, int reserved) {
			return Invoker::Invoke<int, 1211>(type, posX1, posY1, posZ1, posX2, posY2, posZ2, diameter, red, green, blue, alpha, reserved);
		}

		void SET_CHECKPOINT_INSIDE_CYLINDER_HEIGHT_SCALE(int checkpoint, float scale) {
			Invoker::Invoke<void, 1212>(checkpoint, scale);
		}

		void SET_CHECKPOINT_INSIDE_CYLINDER_SCALE(int checkpoint, float scale) {
			Invoker::Invoke<void, 1213>(checkpoint, scale);
		}

		void SET_CHECKPOINT_CYLINDER_HEIGHT(int checkpoint, float nearHeight, float farHeight, float radius) {
			Invoker::Invoke<void, 1214>(checkpoint, nearHeight, farHeight, radius);
		}

		void SET_CHECKPOINT_RGBA(int checkpoint, int red, int green, int blue, int alpha) {
			Invoker::Invoke<void, 1215>(checkpoint, red, green, blue, alpha);
		}

		void SET_CHECKPOINT_RGBA2(int checkpoint, int red, int green, int blue, int alpha) {
			Invoker::Invoke<void, 1216>(checkpoint, red, green, blue, alpha);
		}

		void SET_CHECKPOINT_CLIPPLANE_WITH_POS_NORM(int checkpoint, float posX, float posY, float posZ, float unkX, float unkY, float unkZ) {
			Invoker::Invoke<void, 1217>(checkpoint, posX, posY, posZ, unkX, unkY, unkZ);
		}

		void SET_CHECKPOINT_FORCE_OLD_ARROW_POINTING(int checkpoint) {
			Invoker::Invoke<void, 1218>(checkpoint);
		}

		void SET_CHECKPOINT_DECAL_ROT_ALIGNED_TO_CAMERA_ROT(int checkpoint) {
			Invoker::Invoke<void, 1219>(checkpoint);
		}

		void SET_CHECKPOINT_FORCE_DIRECTION(int checkpoint) {
			Invoker::Invoke<void, 1220>(checkpoint);
		}

		void SET_CHECKPOINT_DIRECTION(int checkpoint, float posX, float posY, float posZ) {
			Invoker::Invoke<void, 1221>(checkpoint, posX, posY, posZ);
		}

		void DELETE_CHECKPOINT(int checkpoint) {
			Invoker::Invoke<void, 1222>(checkpoint);
		}

		void DONT_RENDER_IN_GAME_UI(BOOL p0) {
			Invoker::Invoke<void, 1223>(p0);
		}

		void FORCE_RENDER_IN_GAME_UI(BOOL toggle) {
			Invoker::Invoke<void, 1224>(toggle);
		}

		void REQUEST_STREAMED_TEXTURE_DICT(const char* textureDict, BOOL p1) {
			Invoker::Invoke<void, 1225>(textureDict, p1);
		}

		BOOL HAS_STREAMED_TEXTURE_DICT_LOADED(const char* textureDict) {
			return Invoker::Invoke<BOOL, 1226>(textureDict);
		}

		void SET_STREAMED_TEXTURE_DICT_AS_NO_LONGER_NEEDED(const char* textureDict) {
			Invoker::Invoke<void, 1227>(textureDict);
		}

		void DRAW_RECT(float x, float y, float width, float height, int r, int g, int b, int a, BOOL p8) {
			Invoker::Invoke<void, 1228>(x, y, width, height, r, g, b, a, p8);
		}

		void SET_SCRIPT_GFX_DRAW_BEHIND_PAUSEMENU(BOOL toggle) {
			Invoker::Invoke<void, 1229>(toggle);
		}

		void SET_SCRIPT_GFX_DRAW_ORDER(int drawOrder) {
			Invoker::Invoke<void, 1230>(drawOrder);
		}

		void SET_SCRIPT_GFX_ALIGN(int horizontalAlign, int verticalAlign) {
			Invoker::Invoke<void, 1231>(horizontalAlign, verticalAlign);
		}

		void RESET_SCRIPT_GFX_ALIGN() {
			Invoker::Invoke<void, 1232>();
		}

		void SET_SCRIPT_GFX_ALIGN_PARAMS(float x, float y, float w, float h) {
			Invoker::Invoke<void, 1233>(x, y, w, h);
		}

		void GET_SCRIPT_GFX_ALIGN_POSITION(float x, float y, float* calculatedX, float* calculatedY) {
			Invoker::Invoke<void, 1234>(x, y, calculatedX, calculatedY);
		}

		float GET_SAFE_ZONE_SIZE() {
			return Invoker::Invoke<float, 1235>();
		}

		void DRAW_SPRITE(const char* textureDict, const char* textureName, float screenX, float screenY, float width, float height, float heading, int red, int green, int blue, int alpha, BOOL p11, Any p12) {
			Invoker::Invoke<void, 1236>(textureDict, textureName, screenX, screenY, width, height, heading, red, green, blue, alpha, p11, p12);
		}

		void DRAW_SPRITE_ARX(const char* textureDict, const char* textureName, float x, float y, float width, float height, float p6, int red, int green, int blue, int alpha, Any p11, Any p12) {
			Invoker::Invoke<void, 1237>(textureDict, textureName, x, y, width, height, p6, red, green, blue, alpha, p11, p12);
		}

		void DRAW_SPRITE_NAMED_RENDERTARGET(const char* textureDict, const char* textureName, float screenX, float screenY, float width, float height, float heading, int red, int green, int blue, int alpha, Any p11) {
			Invoker::Invoke<void, 1238>(textureDict, textureName, screenX, screenY, width, height, heading, red, green, blue, alpha, p11);
		}

		void DRAW_SPRITE_ARX_WITH_UV(const char* textureDict, const char* textureName, float x, float y, float width, float height, float u1, float v1, float u2, float v2, float heading, int red, int green, int blue, int alpha, Any p15) {
			Invoker::Invoke<void, 1239>(textureDict, textureName, x, y, width, height, u1, v1, u2, v2, heading, red, green, blue, alpha, p15);
		}

		int ADD_ENTITY_ICON(Entity entity, const char* icon) {
			return Invoker::Invoke<int, 1240>(entity, icon);
		}

		void SET_ENTITY_ICON_VISIBILITY(Entity entity, BOOL toggle) {
			Invoker::Invoke<void, 1241>(entity, toggle);
		}

		void SET_ENTITY_ICON_COLOR(Entity entity, int red, int green, int blue, int alpha) {
			Invoker::Invoke<void, 1242>(entity, red, green, blue, alpha);
		}

		void SET_DRAW_ORIGIN(float x, float y, float z, BOOL p3) {
			Invoker::Invoke<void, 1243>(x, y, z, p3);
		}

		void CLEAR_DRAW_ORIGIN() {
			Invoker::Invoke<void, 1244>();
		}

		int SET_BINK_MOVIE(const char* name) {
			return Invoker::Invoke<int, 1245>(name);
		}

		void PLAY_BINK_MOVIE(int binkMovie) {
			Invoker::Invoke<void, 1246>(binkMovie);
		}

		void STOP_BINK_MOVIE(int binkMovie) {
			Invoker::Invoke<void, 1247>(binkMovie);
		}

		void RELEASE_BINK_MOVIE(int binkMovie) {
			Invoker::Invoke<void, 1248>(binkMovie);
		}

		void DRAW_BINK_MOVIE(int binkMovie, float p1, float p2, float p3, float p4, float p5, int r, int g, int b, int a) {
			Invoker::Invoke<void, 1249>(binkMovie, p1, p2, p3, p4, p5, r, g, b, a);
		}

		void SET_BINK_MOVIE_TIME(int binkMovie, float progress) {
			Invoker::Invoke<void, 1250>(binkMovie, progress);
		}

		float GET_BINK_MOVIE_TIME(int binkMovie) {
			return Invoker::Invoke<float, 1251>(binkMovie);
		}

		void SET_BINK_MOVIE_VOLUME(int binkMovie, float value) {
			Invoker::Invoke<void, 1252>(binkMovie, value);
		}

		void ATTACH_TV_AUDIO_TO_ENTITY(Entity entity) {
			Invoker::Invoke<void, 1253>(entity);
		}

		void SET_BINK_MOVIE_AUDIO_FRONTEND(int binkMovie, BOOL p1) {
			Invoker::Invoke<void, 1254>(binkMovie, p1);
		}

		void SET_TV_AUDIO_FRONTEND(BOOL toggle) {
			Invoker::Invoke<void, 1255>(toggle);
		}

		void SET_BINK_SHOULD_SKIP(int binkMovie, BOOL bShouldSkip) {
			Invoker::Invoke<void, 1256>(binkMovie, bShouldSkip);
		}

		int LOAD_MOVIE_MESH_SET(const char* movieMeshSetName) {
			return Invoker::Invoke<int, 1257>(movieMeshSetName);
		}

		void RELEASE_MOVIE_MESH_SET(int movieMeshSet) {
			Invoker::Invoke<void, 1258>(movieMeshSet);
		}

		int QUERY_MOVIE_MESH_SET_STATE(Any p0) {
			return Invoker::Invoke<int, 1259>(p0);
		}

		void GET_SCREEN_RESOLUTION(int* x, int* y) {
			Invoker::Invoke<void, 1260>(x, y);
		}

		void GET_ACTUAL_SCREEN_RESOLUTION(int* x, int* y) {
			Invoker::Invoke<void, 1261>(x, y);
		}

		float GET_ASPECT_RATIO(BOOL b) {
			return Invoker::Invoke<float, 1262>(b);
		}

		float GET_SCREEN_ASPECT_RATIO() {
			return Invoker::Invoke<float, 1263>();
		}

		BOOL GET_IS_WIDESCREEN() {
			return Invoker::Invoke<BOOL, 1264>();
		}

		BOOL GET_IS_HIDEF() {
			return Invoker::Invoke<BOOL, 1265>();
		}

		void ADJUST_NEXT_POS_SIZE_AS_NORMALIZED_16_9() {
			Invoker::Invoke<void, 1266>();
		}

		void DISPLAY_LOADING_SCREEN_NOW(BOOL onOff) {
			Invoker::Invoke<void, 6692>(onOff);
		}

		void SET_NIGHTVISION(BOOL toggle) {
			Invoker::Invoke<void, 1267>(toggle);
		}

		BOOL GET_REQUESTINGNIGHTVISION() {
			return Invoker::Invoke<BOOL, 1268>();
		}

		BOOL GET_USINGNIGHTVISION() {
			return Invoker::Invoke<BOOL, 1269>();
		}

		void SET_EXPOSURETWEAK(BOOL toggle) {
			Invoker::Invoke<void, 1270>(toggle);
		}

		void FORCE_EXPOSURE_READBACK(BOOL toggle) {
			Invoker::Invoke<void, 1271>(toggle);
		}

		void OVERRIDE_NIGHTVISION_LIGHT_RANGE(float p0) {
			Invoker::Invoke<void, 1272>(p0);
		}

		void SET_NOISEOVERIDE(BOOL toggle) {
			Invoker::Invoke<void, 1273>(toggle);
		}

		void SET_NOISINESSOVERIDE(float value) {
			Invoker::Invoke<void, 1274>(value);
		}

		BOOL GET_SCREEN_COORD_FROM_WORLD_COORD(float worldX, float worldY, float worldZ, float* screenX, float* screenY) {
			return Invoker::Invoke<BOOL, 1275>(worldX, worldY, worldZ, screenX, screenY);
		}

		Vector3 GET_TEXTURE_RESOLUTION(const char* textureDict, const char* textureName) {
			return Invoker::Invoke<Vector3, 1276>(textureDict, textureName);
		}

		BOOL OVERRIDE_PED_CREW_LOGO_TEXTURE(Ped ped, const char* txd, const char* txn) {
			return Invoker::Invoke<BOOL, 1277>(ped, txd, txn);
		}

		void SET_DISTANCE_BLUR_STRENGTH_OVERRIDE(float p0) {
			Invoker::Invoke<void, 1278>(p0);
		}

		void SET_FLASH(float p0, float p1, float fadeIn, float duration, float fadeOut) {
			Invoker::Invoke<void, 1279>(p0, p1, fadeIn, duration, fadeOut);
		}

		void DISABLE_OCCLUSION_THIS_FRAME() {
			Invoker::Invoke<void, 1280>();
		}

		void SET_ARTIFICIAL_LIGHTS_STATE(BOOL state) {
			Invoker::Invoke<void, 1281>(state);
		}

		void SET_ARTIFICIAL_VEHICLE_LIGHTS_STATE(BOOL toggle) {
			Invoker::Invoke<void, 1282>(toggle);
		}

		void DISABLE_HDTEX_THIS_FRAME() {
			Invoker::Invoke<void, 1283>();
		}

		int CREATE_TRACKED_POINT() {
			return Invoker::Invoke<int, 1284>();
		}

		void SET_TRACKED_POINT_INFO(int point, float x, float y, float z, float radius) {
			Invoker::Invoke<void, 1285>(point, x, y, z, radius);
		}

		BOOL IS_TRACKED_POINT_VISIBLE(int point) {
			return Invoker::Invoke<BOOL, 1286>(point);
		}

		void DESTROY_TRACKED_POINT(int point) {
			Invoker::Invoke<void, 1287>(point);
		}

		int SET_GRASS_CULL_SPHERE(float p0, float p1, float p2, float p3) {
			return Invoker::Invoke<int, 1288>(p0, p1, p2, p3);
		}

		void REMOVE_GRASS_CULL_SPHERE(int handle) {
			Invoker::Invoke<void, 1289>(handle);
		}

		void PROCGRASS_ENABLE_CULLSPHERE(int handle, float x, float y, float z, float scale) {
			Invoker::Invoke<void, 1290>(handle, x, y, z, scale);
		}

		void PROCGRASS_DISABLE_CULLSPHERE(int handle) {
			Invoker::Invoke<void, 1291>(handle);
		}

		BOOL PROCGRASS_IS_CULLSPHERE_ENABLED(int handle) {
			return Invoker::Invoke<BOOL, 1292>(handle);
		}

		void PROCGRASS_ENABLE_AMBSCALESCAN() {
			Invoker::Invoke<void, 1293>();
		}

		void PROCGRASS_DISABLE_AMBSCALESCAN() {
			Invoker::Invoke<void, 1294>();
		}

		void DISABLE_PROCOBJ_CREATION() {
			Invoker::Invoke<void, 1295>();
		}

		void ENABLE_PROCOBJ_CREATION() {
			Invoker::Invoke<void, 1296>();
		}

		void GRASSBATCH_ENABLE_FLATTENING_EXT_IN_SPHERE(float x, float y, float z, Any p3, float p4, float p5, float p6, float scale) {
			Invoker::Invoke<void, 1297>(x, y, z, p3, p4, p5, p6, scale);
		}

		void GRASSBATCH_ENABLE_FLATTENING_IN_SPHERE(float x, float y, float z, float radius, float p4, float p5, float p6) {
			Invoker::Invoke<void, 1298>(x, y, z, radius, p4, p5, p6);
		}

		void GRASSBATCH_DISABLE_FLATTENING() {
			Invoker::Invoke<void, 1299>();
		}

		void CASCADE_SHADOWS_INIT_SESSION() {
			Invoker::Invoke<void, 1300>();
		}

		void CASCADE_SHADOWS_SET_CASCADE_BOUNDS(Any p0, BOOL p1, float p2, float p3, float p4, float p5, BOOL p6, float p7) {
			Invoker::Invoke<void, 1301>(p0, p1, p2, p3, p4, p5, p6, p7);
		}

		void CASCADE_SHADOWS_SET_CASCADE_BOUNDS_SCALE(float p0) {
			Invoker::Invoke<void, 1302>(p0);
		}

		void CASCADE_SHADOWS_SET_ENTITY_TRACKER_SCALE(float p0) {
			Invoker::Invoke<void, 1303>(p0);
		}

		void CASCADE_SHADOWS_SET_SPLIT_Z_EXP_WEIGHT(float p0) {
			Invoker::Invoke<void, 1304>(p0);
		}

		void CASCADE_SHADOWS_SET_BOUND_POSITION(Any p0) {
			Invoker::Invoke<void, 1305>(p0);
		}

		void CASCADE_SHADOWS_ENABLE_ENTITY_TRACKER(BOOL toggle) {
			Invoker::Invoke<void, 1306>(toggle);
		}

		void CASCADE_SHADOWS_SET_SCREEN_SIZE_CHECK_ENABLED(BOOL p0) {
			Invoker::Invoke<void, 1307>(p0);
		}

		void CASCADE_SHADOWS_SET_SHADOW_SAMPLE_TYPE(const char* type) {
			Invoker::Invoke<void, 1308>(type);
		}

		void CASCADE_SHADOWS_CLEAR_SHADOW_SAMPLE_TYPE() {
			Invoker::Invoke<void, 1309>();
		}

		void CASCADE_SHADOWS_SET_AIRCRAFT_MODE(BOOL p0) {
			Invoker::Invoke<void, 1310>(p0);
		}

		void CASCADE_SHADOWS_SET_DYNAMIC_DEPTH_MODE(BOOL p0) {
			Invoker::Invoke<void, 1311>(p0);
		}

		void CASCADE_SHADOWS_SET_DYNAMIC_DEPTH_VALUE(float p0) {
			Invoker::Invoke<void, 1312>(p0);
		}

		void CASCADE_SHADOWS_ENABLE_FREEZER(BOOL p0) {
			Invoker::Invoke<void, 1313>(p0);
		}

		void WATER_REFLECTION_SET_SCRIPT_OBJECT_VISIBILITY(Any p0) {
			Invoker::Invoke<void, 1314>(p0);
		}

		void GOLF_TRAIL_SET_ENABLED(BOOL toggle) {
			Invoker::Invoke<void, 1315>(toggle);
		}

		void GOLF_TRAIL_SET_PATH(float p0, float p1, float p2, float p3, float p4, float p5, float p6, float p7, BOOL p8) {
			Invoker::Invoke<void, 1316>(p0, p1, p2, p3, p4, p5, p6, p7, p8);
		}

		void GOLF_TRAIL_SET_RADIUS(float p0, float p1, float p2) {
			Invoker::Invoke<void, 1317>(p0, p1, p2);
		}

		void GOLF_TRAIL_SET_COLOUR(int p0, int p1, int p2, int p3, int p4, int p5, int p6, int p7, int p8, int p9, int p10, int p11) {
			Invoker::Invoke<void, 1318>(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11);
		}

		void GOLF_TRAIL_SET_TESSELLATION(int p0, int p1) {
			Invoker::Invoke<void, 1319>(p0, p1);
		}

		void GOLF_TRAIL_SET_FIXED_CONTROL_POINT_ENABLE(BOOL p0) {
			Invoker::Invoke<void, 1320>(p0);
		}

		void GOLF_TRAIL_SET_FIXED_CONTROL_POINT(int type, float xPos, float yPos, float zPos, float p4, int red, int green, int blue, int alpha) {
			Invoker::Invoke<void, 1321>(type, xPos, yPos, zPos, p4, red, green, blue, alpha);
		}

		void GOLF_TRAIL_SET_SHADER_PARAMS(float p0, float p1, float p2, float p3, float p4) {
			Invoker::Invoke<void, 1322>(p0, p1, p2, p3, p4);
		}

		void GOLF_TRAIL_SET_FACING(BOOL p0) {
			Invoker::Invoke<void, 1323>(p0);
		}

		float GOLF_TRAIL_GET_MAX_HEIGHT() {
			return Invoker::Invoke<float, 1324>();
		}

		Vector3 GOLF_TRAIL_GET_VISUAL_CONTROL_POINT(int p0) {
			return Invoker::Invoke<Vector3, 1325>(p0);
		}

		void SET_SEETHROUGH(BOOL toggle) {
			Invoker::Invoke<void, 1326>(toggle);
		}

		BOOL GET_USINGSEETHROUGH() {
			return Invoker::Invoke<BOOL, 1327>();
		}

		void SEETHROUGH_RESET() {
			Invoker::Invoke<void, 1328>();
		}

		void SEETHROUGH_SET_FADE_STARTDISTANCE(float distance) {
			Invoker::Invoke<void, 1329>(distance);
		}

		void SEETHROUGH_SET_FADE_ENDDISTANCE(float distance) {
			Invoker::Invoke<void, 1330>(distance);
		}

		float SEETHROUGH_GET_MAX_THICKNESS() {
			return Invoker::Invoke<float, 1331>();
		}

		void SEETHROUGH_SET_MAX_THICKNESS(float thickness) {
			Invoker::Invoke<void, 1332>(thickness);
		}

		void SEETHROUGH_SET_NOISE_MIN(float amount) {
			Invoker::Invoke<void, 1333>(amount);
		}

		void SEETHROUGH_SET_NOISE_MAX(float amount) {
			Invoker::Invoke<void, 1334>(amount);
		}

		void SEETHROUGH_SET_HILIGHT_INTENSITY(float intensity) {
			Invoker::Invoke<void, 1335>(intensity);
		}

		void SEETHROUGH_SET_HIGHLIGHT_NOISE(float noise) {
			Invoker::Invoke<void, 1336>(noise);
		}

		void SEETHROUGH_SET_HEATSCALE(int index, float heatScale) {
			Invoker::Invoke<void, 1337>(index, heatScale);
		}

		void SEETHROUGH_SET_COLOR_NEAR(int red, int green, int blue) {
			Invoker::Invoke<void, 1338>(red, green, blue);
		}

		void SET_MOTIONBLUR_MAX_VEL_SCALER(float p0) {
			Invoker::Invoke<void, 1339>(p0);
		}

		float GET_MOTIONBLUR_MAX_VEL_SCALER() {
			return Invoker::Invoke<float, 1340>();
		}

		void SET_FORCE_MOTIONBLUR(BOOL toggle) {
			Invoker::Invoke<void, 1341>(toggle);
		}

		void TOGGLE_PLAYER_DAMAGE_OVERLAY(BOOL toggle) {
			Invoker::Invoke<void, 1342>(toggle);
		}

		void RESET_ADAPTATION(int p0) {
			Invoker::Invoke<void, 1343>(p0);
		}

		BOOL TRIGGER_SCREENBLUR_FADE_IN(float transitionTime) {
			return Invoker::Invoke<BOOL, 1344>(transitionTime);
		}

		BOOL TRIGGER_SCREENBLUR_FADE_OUT(float transitionTime) {
			return Invoker::Invoke<BOOL, 1345>(transitionTime);
		}

		void DISABLE_SCREENBLUR_FADE() {
			Invoker::Invoke<void, 1346>();
		}

		float GET_SCREENBLUR_FADE_CURRENT_TIME() {
			return Invoker::Invoke<float, 1347>();
		}

		BOOL IS_SCREENBLUR_FADE_RUNNING() {
			return Invoker::Invoke<BOOL, 1348>();
		}

		void TOGGLE_PAUSED_RENDERPHASES(BOOL toggle) {
			Invoker::Invoke<void, 1349>(toggle);
		}

		BOOL GET_TOGGLE_PAUSED_RENDERPHASES_STATUS() {
			return Invoker::Invoke<BOOL, 1350>();
		}

		void RESET_PAUSED_RENDERPHASES() {
			Invoker::Invoke<void, 1351>();
		}

		void GRAB_PAUSEMENU_OWNERSHIP() {
			Invoker::Invoke<void, 1352>();
		}

		void SET_HIDOF_OVERRIDE(BOOL p0, BOOL p1, float nearplaneOut, float nearplaneIn, float farplaneOut, float farplaneIn) {
			Invoker::Invoke<void, 1353>(p0, p1, nearplaneOut, nearplaneIn, farplaneOut, farplaneIn);
		}

		void SET_LOCK_ADAPTIVE_DOF_DISTANCE(BOOL p0) {
			Invoker::Invoke<void, 1354>(p0);
		}

		BOOL PHONEPHOTOEDITOR_TOGGLE(BOOL p0) {
			return Invoker::Invoke<BOOL, 1355>(p0);
		}

		BOOL PHONEPHOTOEDITOR_IS_ACTIVE() {
			return Invoker::Invoke<BOOL, 1356>();
		}

		BOOL PHONEPHOTOEDITOR_SET_FRAME_TXD(const char* textureDict, BOOL p1) {
			return Invoker::Invoke<BOOL, 1357>(textureDict, p1);
		}

		BOOL START_PARTICLE_FX_NON_LOOPED_AT_COORD(const char* effectName, float xPos, float yPos, float zPos, float xRot, float yRot, float zRot, float scale, BOOL xAxis, BOOL yAxis, BOOL zAxis) {
			return Invoker::Invoke<BOOL, 1358>(effectName, xPos, yPos, zPos, xRot, yRot, zRot, scale, xAxis, yAxis, zAxis);
		}

		BOOL START_NETWORKED_PARTICLE_FX_NON_LOOPED_AT_COORD(const char* effectName, float xPos, float yPos, float zPos, float xRot, float yRot, float zRot, float scale, BOOL xAxis, BOOL yAxis, BOOL zAxis, BOOL p11) {
			return Invoker::Invoke<BOOL, 1359>(effectName, xPos, yPos, zPos, xRot, yRot, zRot, scale, xAxis, yAxis, zAxis, p11);
		}

		BOOL START_PARTICLE_FX_NON_LOOPED_ON_PED_BONE(const char* effectName, Ped ped, float offsetX, float offsetY, float offsetZ, float rotX, float rotY, float rotZ, int boneIndex, float scale, BOOL axisX, BOOL axisY, BOOL axisZ) {
			return Invoker::Invoke<BOOL, 1360>(effectName, ped, offsetX, offsetY, offsetZ, rotX, rotY, rotZ, boneIndex, scale, axisX, axisY, axisZ);
		}

		BOOL START_NETWORKED_PARTICLE_FX_NON_LOOPED_ON_PED_BONE(const char* effectName, Ped ped, float offsetX, float offsetY, float offsetZ, float rotX, float rotY, float rotZ, int boneIndex, float scale, BOOL axisX, BOOL axisY, BOOL axisZ) {
			return Invoker::Invoke<BOOL, 1361>(effectName, ped, offsetX, offsetY, offsetZ, rotX, rotY, rotZ, boneIndex, scale, axisX, axisY, axisZ);
		}

		BOOL START_PARTICLE_FX_NON_LOOPED_ON_ENTITY(const char* effectName, Entity entity, float offsetX, float offsetY, float offsetZ, float rotX, float rotY, float rotZ, float scale, BOOL axisX, BOOL axisY, BOOL axisZ) {
			return Invoker::Invoke<BOOL, 1362>(effectName, entity, offsetX, offsetY, offsetZ, rotX, rotY, rotZ, scale, axisX, axisY, axisZ);
		}

		BOOL START_NETWORKED_PARTICLE_FX_NON_LOOPED_ON_ENTITY(const char* effectName, Entity entity, float offsetX, float offsetY, float offsetZ, float rotX, float rotY, float rotZ, float scale, BOOL axisX, BOOL axisY, BOOL axisZ) {
			return Invoker::Invoke<BOOL, 1363>(effectName, entity, offsetX, offsetY, offsetZ, rotX, rotY, rotZ, scale, axisX, axisY, axisZ);
		}

		BOOL START_PARTICLE_FX_NON_LOOPED_ON_ENTITY_BONE(const char* effectName, Entity entity, float offsetX, float offsetY, float offsetZ, float rotX, float rotY, float rotZ, int boneIndex, float scale, BOOL axisX, BOOL axisY, BOOL axisZ) {
			return Invoker::Invoke<BOOL, 1364>(effectName, entity, offsetX, offsetY, offsetZ, rotX, rotY, rotZ, boneIndex, scale, axisX, axisY, axisZ);
		}

		void SET_PARTICLE_FX_NON_LOOPED_COLOUR(float r, float g, float b) {
			Invoker::Invoke<void, 1365>(r, g, b);
		}

		void SET_PARTICLE_FX_NON_LOOPED_ALPHA(float alpha) {
			Invoker::Invoke<void, 1366>(alpha);
		}

		void SET_PARTICLE_FX_NON_LOOPED_SCALE(float scale) {
			Invoker::Invoke<void, 1367>(scale);
		}

		void SET_PARTICLE_FX_NON_LOOPED_EMITTER_SIZE(float p0, float p1, float scale) {
			Invoker::Invoke<void, 1368>(p0, p1, scale);
		}

		void SET_PARTICLE_FX_FORCE_VEHICLE_INTERIOR(BOOL toggle) {
			Invoker::Invoke<void, 1369>(toggle);
		}

		int START_PARTICLE_FX_LOOPED_AT_COORD(const char* effectName, float x, float y, float z, float xRot, float yRot, float zRot, float scale, BOOL xAxis, BOOL yAxis, BOOL zAxis, BOOL p11) {
			return Invoker::Invoke<int, 1370>(effectName, x, y, z, xRot, yRot, zRot, scale, xAxis, yAxis, zAxis, p11);
		}

		int START_PARTICLE_FX_LOOPED_ON_PED_BONE(const char* effectName, Ped ped, float xOffset, float yOffset, float zOffset, float xRot, float yRot, float zRot, int boneIndex, float scale, BOOL xAxis, BOOL yAxis, BOOL zAxis) {
			return Invoker::Invoke<int, 1371>(effectName, ped, xOffset, yOffset, zOffset, xRot, yRot, zRot, boneIndex, scale, xAxis, yAxis, zAxis);
		}

		int START_PARTICLE_FX_LOOPED_ON_ENTITY(const char* effectName, Entity entity, float xOffset, float yOffset, float zOffset, float xRot, float yRot, float zRot, float scale, BOOL xAxis, BOOL yAxis, BOOL zAxis) {
			return Invoker::Invoke<int, 1372>(effectName, entity, xOffset, yOffset, zOffset, xRot, yRot, zRot, scale, xAxis, yAxis, zAxis);
		}

		int START_PARTICLE_FX_LOOPED_ON_ENTITY_BONE(const char* effectName, Entity entity, float xOffset, float yOffset, float zOffset, float xRot, float yRot, float zRot, int boneIndex, float scale, BOOL xAxis, BOOL yAxis, BOOL zAxis) {
			return Invoker::Invoke<int, 1373>(effectName, entity, xOffset, yOffset, zOffset, xRot, yRot, zRot, boneIndex, scale, xAxis, yAxis, zAxis);
		}

		int START_NETWORKED_PARTICLE_FX_LOOPED_ON_ENTITY(const char* effectName, Entity entity, float xOffset, float yOffset, float zOffset, float xRot, float yRot, float zRot, float scale, BOOL xAxis, BOOL yAxis, BOOL zAxis, float r, float g, float b, float a) {
			return Invoker::Invoke<int, 1374>(effectName, entity, xOffset, yOffset, zOffset, xRot, yRot, zRot, scale, xAxis, yAxis, zAxis, r, g, b, a);
		}

		int START_NETWORKED_PARTICLE_FX_LOOPED_ON_ENTITY_BONE(const char* effectName, Entity entity, float xOffset, float yOffset, float zOffset, float xRot, float yRot, float zRot, int boneIndex, float scale, BOOL xAxis, BOOL yAxis, BOOL zAxis, float r, float g, float b, float a) {
			return Invoker::Invoke<int, 1375>(effectName, entity, xOffset, yOffset, zOffset, xRot, yRot, zRot, boneIndex, scale, xAxis, yAxis, zAxis, r, g, b, a);
		}

		void STOP_PARTICLE_FX_LOOPED(int ptfxHandle, BOOL p1) {
			Invoker::Invoke<void, 1376>(ptfxHandle, p1);
		}

		void REMOVE_PARTICLE_FX(int ptfxHandle, BOOL p1) {
			Invoker::Invoke<void, 1377>(ptfxHandle, p1);
		}

		void REMOVE_PARTICLE_FX_FROM_ENTITY(Entity entity) {
			Invoker::Invoke<void, 1378>(entity);
		}

		void REMOVE_PARTICLE_FX_IN_RANGE(float X, float Y, float Z, float radius) {
			Invoker::Invoke<void, 1379>(X, Y, Z, radius);
		}

		void FORCE_PARTICLE_FX_IN_VEHICLE_INTERIOR(Any p0, Any p1) {
			Invoker::Invoke<void, 1380>(p0, p1);
		}

		BOOL DOES_PARTICLE_FX_LOOPED_EXIST(int ptfxHandle) {
			return Invoker::Invoke<BOOL, 1381>(ptfxHandle);
		}

		void SET_PARTICLE_FX_LOOPED_OFFSETS(int ptfxHandle, float x, float y, float z, float rotX, float rotY, float rotZ) {
			Invoker::Invoke<void, 1382>(ptfxHandle, x, y, z, rotX, rotY, rotZ);
		}

		void SET_PARTICLE_FX_LOOPED_EVOLUTION(int ptfxHandle, const char* propertyName, float amount, BOOL noNetwork) {
			Invoker::Invoke<void, 1383>(ptfxHandle, propertyName, amount, noNetwork);
		}

		void SET_PARTICLE_FX_LOOPED_COLOUR(int ptfxHandle, float r, float g, float b, BOOL p4) {
			Invoker::Invoke<void, 1384>(ptfxHandle, r, g, b, p4);
		}

		void SET_PARTICLE_FX_LOOPED_ALPHA(int ptfxHandle, float alpha) {
			Invoker::Invoke<void, 1385>(ptfxHandle, alpha);
		}

		void SET_PARTICLE_FX_LOOPED_SCALE(int ptfxHandle, float scale) {
			Invoker::Invoke<void, 1386>(ptfxHandle, scale);
		}

		void SET_PARTICLE_FX_LOOPED_FAR_CLIP_DIST(int ptfxHandle, float range) {
			Invoker::Invoke<void, 1387>(ptfxHandle, range);
		}

		void _SET_PARTICLE_FX_LOOPED_CAMERA_BIAS(int ptfxHandle, float p1) {
			Invoker::Invoke<void, 1388>(ptfxHandle, p1);
		}

		void SET_PARTICLE_FX_CAM_INSIDE_VEHICLE(BOOL p0) {
			Invoker::Invoke<void, 1389>(p0);
		}

		void SET_PARTICLE_FX_CAM_INSIDE_NONPLAYER_VEHICLE(Vehicle vehicle, BOOL p1) {
			Invoker::Invoke<void, 1390>(vehicle, p1);
		}

		void SET_PARTICLE_FX_SHOOTOUT_BOAT(Any p0) {
			Invoker::Invoke<void, 1391>(p0);
		}

		void CLEAR_PARTICLE_FX_SHOOTOUT_BOAT() {
			Invoker::Invoke<void, 1392>();
		}

		void SET_PARTICLE_FX_BLOOD_SCALE(Any p0) {
			Invoker::Invoke<void, 1393>(p0);
		}

		void DISABLE_IN_WATER_PTFX(BOOL toggle) {
			Invoker::Invoke<void, 1394>(toggle);
		}

		void DISABLE_DOWNWASH_PTFX(BOOL toggle) {
			Invoker::Invoke<void, 1395>(toggle);
		}

		void SET_PARTICLE_FX_SLIPSTREAM_LODRANGE_SCALE(float scale) {
			Invoker::Invoke<void, 1396>(scale);
		}

		void ENABLE_CLOWN_BLOOD_VFX(BOOL toggle) {
			Invoker::Invoke<void, 1397>(toggle);
		}

		void ENABLE_ALIEN_BLOOD_VFX(BOOL toggle) {
			Invoker::Invoke<void, 1398>(toggle);
		}

		void SET_PARTICLE_FX_BULLET_IMPACT_SCALE(float scale) {
			Invoker::Invoke<void, 1399>(scale);
		}

		void SET_PARTICLE_FX_BULLET_IMPACT_LODRANGE_SCALE(float p0) {
			Invoker::Invoke<void, 1400>(p0);
		}

		void SET_PARTICLE_FX_BULLET_TRACE_NO_ANGLE_REJECT(BOOL p0) {
			Invoker::Invoke<void, 1401>(p0);
		}

		void SET_PARTICLE_FX_BANG_SCRAPE_LODRANGE_SCALE(float p0) {
			Invoker::Invoke<void, 1402>(p0);
		}

		void SET_PARTICLE_FX_FOOT_LODRANGE_SCALE(float p0) {
			Invoker::Invoke<void, 1403>(p0);
		}

		void SET_PARTICLE_FX_FOOT_OVERRIDE_NAME(const char* p0) {
			Invoker::Invoke<void, 1404>(p0);
		}

		void SET_SKIDMARK_RANGE_SCALE(float scale) {
			Invoker::Invoke<void, 1405>(scale);
		}

		void SET_PTFX_FORCE_VEHICLE_INTERIOR_FLAG(Any p0) {
			Invoker::Invoke<void, 1406>(p0);
		}

		void _REGISTER_PTFX_FOG_VOLUME(float posX, float posY, float posZ, float range, float density) {
			Invoker::Invoke<void, 6691>(posX, posY, posZ, range, density);
		}

		void REGISTER_POSTFX_BULLET_IMPACT(float weaponWorldPosX, float weaponWorldPosY, float weaponWorldPosZ, float intensity) {
			Invoker::Invoke<void, 1407>(weaponWorldPosX, weaponWorldPosY, weaponWorldPosZ, intensity);
		}

		void FORCE_POSTFX_BULLET_IMPACTS_AFTER_HUD(BOOL p0) {
			Invoker::Invoke<void, 1408>(p0);
		}

		void USE_PARTICLE_FX_ASSET(const char* name) {
			Invoker::Invoke<void, 1409>(name);
		}

		void SET_PARTICLE_FX_OVERRIDE(const char* oldAsset, const char* newAsset) {
			Invoker::Invoke<void, 1410>(oldAsset, newAsset);
		}

		void RESET_PARTICLE_FX_OVERRIDE(const char* name) {
			Invoker::Invoke<void, 1411>(name);
		}

		int _START_VEHICLE_PARTICLE_FX_LOOPED(Vehicle vehicle, const char* effectName, BOOL frontBack, BOOL leftRight, BOOL localOnly) {
			return Invoker::Invoke<int, 1412>(vehicle, effectName, frontBack, leftRight, localOnly);
		}

		void SET_WEATHER_PTFX_USE_OVERRIDE_SETTINGS(BOOL p0) {
			Invoker::Invoke<void, 1413>(p0);
		}

		void SET_WEATHER_PTFX_OVERRIDE_CURR_LEVEL(float p0) {
			Invoker::Invoke<void, 1414>(p0);
		}

		void WASH_DECALS_IN_RANGE(float x, float y, float z, float range, float p4) {
			Invoker::Invoke<void, 1415>(x, y, z, range, p4);
		}

		void WASH_DECALS_FROM_VEHICLE(Vehicle vehicle, float p1) {
			Invoker::Invoke<void, 1416>(vehicle, p1);
		}

		void FADE_DECALS_IN_RANGE(float x, float y, float z, float p3, float p4) {
			Invoker::Invoke<void, 1417>(x, y, z, p3, p4);
		}

		void REMOVE_DECALS_IN_RANGE(float x, float y, float z, float range) {
			Invoker::Invoke<void, 1418>(x, y, z, range);
		}

		void REMOVE_DECALS_FROM_OBJECT(Object obj) {
			Invoker::Invoke<void, 1419>(obj);
		}

		void REMOVE_DECALS_FROM_OBJECT_FACING(Object obj, float x, float y, float z) {
			Invoker::Invoke<void, 1420>(obj, x, y, z);
		}

		void REMOVE_DECALS_FROM_VEHICLE(Vehicle vehicle) {
			Invoker::Invoke<void, 1421>(vehicle);
		}

		int ADD_DECAL(int decalType, float posX, float posY, float posZ, float p4, float p5, float p6, float p7, float p8, float p9, float width, float height, float rCoef, float gCoef, float bCoef, float opacity, float timeout, BOOL p17, BOOL p18, BOOL p19) {
			return Invoker::Invoke<int, 1422>(decalType, posX, posY, posZ, p4, p5, p6, p7, p8, p9, width, height, rCoef, gCoef, bCoef, opacity, timeout, p17, p18, p19);
		}

		int ADD_PETROL_DECAL(float x, float y, float z, float groundLvl, float width, float transparency) {
			return Invoker::Invoke<int, 1423>(x, y, z, groundLvl, width, transparency);
		}

		int ADD_OIL_DECAL(float x, float y, float z, float groundLvl, float width, float transparency) {
			return Invoker::Invoke<int, 1424>(x, y, z, groundLvl, width, transparency);
		}

		void START_PETROL_TRAIL_DECALS(float p0) {
			Invoker::Invoke<void, 1425>(p0);
		}

		void ADD_PETROL_TRAIL_DECAL_INFO(float x, float y, float z, float p3) {
			Invoker::Invoke<void, 1426>(x, y, z, p3);
		}

		void END_PETROL_TRAIL_DECALS() {
			Invoker::Invoke<void, 1427>();
		}

		void REMOVE_DECAL(int decal) {
			Invoker::Invoke<void, 1428>(decal);
		}

		BOOL IS_DECAL_ALIVE(int decal) {
			return Invoker::Invoke<BOOL, 1429>(decal);
		}

		float GET_DECAL_WASH_LEVEL(int decal) {
			return Invoker::Invoke<float, 1430>(decal);
		}

		void SET_DISABLE_PETROL_DECALS_IGNITING_THIS_FRAME() {
			Invoker::Invoke<void, 1431>();
		}

		void SET_DISABLE_PETROL_DECALS_RECYCLING_THIS_FRAME() {
			Invoker::Invoke<void, 1432>();
		}

		void SET_DISABLE_DECAL_RENDERING_THIS_FRAME() {
			Invoker::Invoke<void, 1433>();
		}

		BOOL GET_IS_PETROL_DECAL_IN_RANGE(float xCoord, float yCoord, float zCoord, float radius) {
			return Invoker::Invoke<BOOL, 1434>(xCoord, yCoord, zCoord, radius);
		}

		void PATCH_DECAL_DIFFUSE_MAP(int decalType, const char* textureDict, const char* textureName) {
			Invoker::Invoke<void, 1435>(decalType, textureDict, textureName);
		}

		void UNPATCH_DECAL_DIFFUSE_MAP(int decalType) {
			Invoker::Invoke<void, 1436>(decalType);
		}

		void MOVE_VEHICLE_DECALS(Any p0, Any p1) {
			Invoker::Invoke<void, 1437>(p0, p1);
		}

		BOOL ADD_VEHICLE_CREW_EMBLEM(Vehicle vehicle, Ped ped, int boneIndex, float x1, float x2, float x3, float y1, float y2, float y3, float z1, float z2, float z3, float scale, Any p13, int alpha) {
			return Invoker::Invoke<BOOL, 1438>(vehicle, ped, boneIndex, x1, x2, x3, y1, y2, y3, z1, z2, z3, scale, p13, alpha);
		}

		BOOL ABORT_VEHICLE_CREW_EMBLEM_REQUEST(int* p0) {
			return Invoker::Invoke<BOOL, 1439>(p0);
		}

		void REMOVE_VEHICLE_CREW_EMBLEM(Vehicle vehicle, int p1) {
			Invoker::Invoke<void, 1440>(vehicle, p1);
		}

		int GET_VEHICLE_CREW_EMBLEM_REQUEST_STATE(Vehicle vehicle, int p1) {
			return Invoker::Invoke<int, 1441>(vehicle, p1);
		}

		BOOL DOES_VEHICLE_HAVE_CREW_EMBLEM(Vehicle vehicle, int p1) {
			return Invoker::Invoke<BOOL, 1442>(vehicle, p1);
		}

		void DISABLE_COMPOSITE_SHOTGUN_DECALS(BOOL toggle) {
			Invoker::Invoke<void, 1443>(toggle);
		}

		void DISABLE_SCUFF_DECALS(BOOL toggle) {
			Invoker::Invoke<void, 1444>(toggle);
		}

		void SET_DECAL_BULLET_IMPACT_RANGE_SCALE(float p0) {
			Invoker::Invoke<void, 1445>(p0);
		}

		void OVERRIDE_INTERIOR_SMOKE_NAME(const char* name) {
			Invoker::Invoke<void, 1446>(name);
		}

		void OVERRIDE_INTERIOR_SMOKE_LEVEL(float level) {
			Invoker::Invoke<void, 1447>(level);
		}

		void OVERRIDE_INTERIOR_SMOKE_END() {
			Invoker::Invoke<void, 1448>();
		}

		void REGISTER_NOIR_LENS_EFFECT() {
			Invoker::Invoke<void, 1449>();
		}

		void DISABLE_VEHICLE_DISTANTLIGHTS(BOOL toggle) {
			Invoker::Invoke<void, 1450>(toggle);
		}

		void RENDER_SHADOWED_LIGHTS_WITH_NO_SHADOWS(BOOL p0) {
			Invoker::Invoke<void, 1451>(p0);
		}

		void REQUEST_EARLY_LIGHT_CHECK() {
			Invoker::Invoke<void, 1452>();
		}

		void USE_SNOW_FOOT_VFX_WHEN_UNSHELTERED(BOOL toggle) {
			Invoker::Invoke<void, 1453>(toggle);
		}

		void _FORCE_ALLOW_SNOW_FOOT_VFX_ON_ICE(BOOL toggle) {
			Invoker::Invoke<void, 1454>(toggle);
		}

		void USE_SNOW_WHEEL_VFX_WHEN_UNSHELTERED(BOOL toggle) {
			Invoker::Invoke<void, 1455>(toggle);
		}

		void DISABLE_REGION_VFX(Any p0) {
			Invoker::Invoke<void, 1456>(p0);
		}

		void _FORCE_GROUND_SNOW_PASS(BOOL toggle) {
			Invoker::Invoke<void, 1457>(toggle);
		}

		void PRESET_INTERIOR_AMBIENT_CACHE(const char* timecycleModifierName) {
			Invoker::Invoke<void, 1458>(timecycleModifierName);
		}

		void SET_TIMECYCLE_MODIFIER(const char* modifierName) {
			Invoker::Invoke<void, 1459>(modifierName);
		}

		void SET_TIMECYCLE_MODIFIER_STRENGTH(float strength) {
			Invoker::Invoke<void, 1460>(strength);
		}

		void SET_TRANSITION_TIMECYCLE_MODIFIER(const char* modifierName, float transition) {
			Invoker::Invoke<void, 1461>(modifierName, transition);
		}

		void SET_TRANSITION_OUT_OF_TIMECYCLE_MODIFIER(float strength) {
			Invoker::Invoke<void, 1462>(strength);
		}

		void CLEAR_TIMECYCLE_MODIFIER() {
			Invoker::Invoke<void, 1463>();
		}

		int GET_TIMECYCLE_MODIFIER_INDEX() {
			return Invoker::Invoke<int, 1464>();
		}

		int GET_TIMECYCLE_TRANSITION_MODIFIER_INDEX() {
			return Invoker::Invoke<int, 1465>();
		}

		BOOL GET_IS_TIMECYCLE_TRANSITIONING_OUT() {
			return Invoker::Invoke<BOOL, 1466>();
		}

		void PUSH_TIMECYCLE_MODIFIER() {
			Invoker::Invoke<void, 1467>();
		}

		void POP_TIMECYCLE_MODIFIER() {
			Invoker::Invoke<void, 1468>();
		}

		void SET_CURRENT_PLAYER_TCMODIFIER(const char* modifierName) {
			Invoker::Invoke<void, 1469>(modifierName);
		}

		void SET_PLAYER_TCMODIFIER_TRANSITION(float value) {
			Invoker::Invoke<void, 1470>(value);
		}

		void SET_NEXT_PLAYER_TCMODIFIER(const char* modifierName) {
			Invoker::Invoke<void, 1471>(modifierName);
		}

		void ADD_TCMODIFIER_OVERRIDE(const char* modifierName1, const char* modifierName2) {
			Invoker::Invoke<void, 1472>(modifierName1, modifierName2);
		}

		void CLEAR_ALL_TCMODIFIER_OVERRIDES(const char* p0) {
			Invoker::Invoke<void, 1473>(p0);
		}

		void SET_EXTRA_TCMODIFIER(const char* modifierName) {
			Invoker::Invoke<void, 1474>(modifierName);
		}

		void CLEAR_EXTRA_TCMODIFIER() {
			Invoker::Invoke<void, 1475>();
		}

		int GET_EXTRA_TCMODIFIER() {
			return Invoker::Invoke<int, 1476>();
		}

		void ENABLE_MOON_CYCLE_OVERRIDE(float strength) {
			Invoker::Invoke<void, 1477>(strength);
		}

		void DISABLE_MOON_CYCLE_OVERRIDE() {
			Invoker::Invoke<void, 1478>();
		}

		int REQUEST_SCALEFORM_MOVIE(const char* scaleformName) {
			return Invoker::Invoke<int, 1479>(scaleformName);
		}

		int REQUEST_SCALEFORM_MOVIE_WITH_IGNORE_SUPER_WIDESCREEN(const char* scaleformName) {
			return Invoker::Invoke<int, 1480>(scaleformName);
		}

		int REQUEST_SCALEFORM_MOVIE_INSTANCE(const char* scaleformName) {
			return Invoker::Invoke<int, 1481>(scaleformName);
		}

		int REQUEST_SCALEFORM_MOVIE_SKIP_RENDER_WHILE_PAUSED(const char* scaleformName) {
			return Invoker::Invoke<int, 1482>(scaleformName);
		}

		BOOL HAS_SCALEFORM_MOVIE_LOADED(int scaleformHandle) {
			return Invoker::Invoke<BOOL, 1483>(scaleformHandle);
		}

		BOOL _HAS_SCALEFORM_MOVIE_NAMED_LOADED(int* scaleformHandle, const char* scaleformName) {
			return Invoker::Invoke<BOOL, 6690>(scaleformHandle, scaleformName);
		}

		BOOL IS_ACTIVE_SCALEFORM_MOVIE_DELETING(int val) {
			return Invoker::Invoke<BOOL, 1484>(val);
		}

		BOOL IS_SCALEFORM_MOVIE_DELETING(int val) {
			return Invoker::Invoke<BOOL, 1485>(val);
		}

		BOOL HAS_SCALEFORM_MOVIE_FILENAME_LOADED(const char* scaleformName) {
			return Invoker::Invoke<BOOL, 1486>(scaleformName);
		}

		BOOL HAS_SCALEFORM_CONTAINER_MOVIE_LOADED_INTO_PARENT(int scaleformHandle) {
			return Invoker::Invoke<BOOL, 1487>(scaleformHandle);
		}

		void SET_SCALEFORM_MOVIE_AS_NO_LONGER_NEEDED(int* scaleformHandle) {
			Invoker::Invoke<void, 1488>(scaleformHandle);
		}

		void _SET_SCALEFORM_MOVIE_NAMED_AS_NO_LONGER_NEEDED(int scaleformHandle, const char* scaleformName) {
			Invoker::Invoke<void, 6689>(scaleformHandle, scaleformName);
		}

		void SET_SCALEFORM_MOVIE_TO_USE_SYSTEM_TIME(int scaleform, BOOL toggle) {
			Invoker::Invoke<void, 1489>(scaleform, toggle);
		}

		void SET_SCALEFORM_MOVIE_TO_USE_LARGE_RT(int scaleformHandle, BOOL toggle) {
			Invoker::Invoke<void, 1490>(scaleformHandle, toggle);
		}

		void SET_SCALEFORM_MOVIE_TO_USE_SUPER_LARGE_RT(int scaleformHandle, BOOL toggle) {
			Invoker::Invoke<void, 1491>(scaleformHandle, toggle);
		}

		void DRAW_SCALEFORM_MOVIE(int scaleformHandle, float x, float y, float width, float height, int red, int green, int blue, int alpha, int p9) {
			Invoker::Invoke<void, 1492>(scaleformHandle, x, y, width, height, red, green, blue, alpha, p9);
		}

		void DRAW_SCALEFORM_MOVIE_FULLSCREEN(int scaleform, int red, int green, int blue, int alpha, int p5) {
			Invoker::Invoke<void, 1493>(scaleform, red, green, blue, alpha, p5);
		}

		void DRAW_SCALEFORM_MOVIE_FULLSCREEN_MASKED(int scaleform1, int scaleform2, int red, int green, int blue, int alpha) {
			Invoker::Invoke<void, 1494>(scaleform1, scaleform2, red, green, blue, alpha);
		}

		void DRAW_SCALEFORM_MOVIE_3D(int scaleform, float posX, float posY, float posZ, float rotX, float rotY, float rotZ, float p7, float p8, float p9, float scaleX, float scaleY, float scaleZ, int rotationOrder) {
			Invoker::Invoke<void, 1495>(scaleform, posX, posY, posZ, rotX, rotY, rotZ, p7, p8, p9, scaleX, scaleY, scaleZ, rotationOrder);
		}

		void DRAW_SCALEFORM_MOVIE_3D_SOLID(int scaleform, float posX, float posY, float posZ, float rotX, float rotY, float rotZ, float p7, float p8, float p9, float scaleX, float scaleY, float scaleZ, int rotationOrder) {
			Invoker::Invoke<void, 1496>(scaleform, posX, posY, posZ, rotX, rotY, rotZ, p7, p8, p9, scaleX, scaleY, scaleZ, rotationOrder);
		}

		void CALL_SCALEFORM_MOVIE_METHOD(int scaleform, const char* method) {
			Invoker::Invoke<void, 1497>(scaleform, method);
		}

		void CALL_SCALEFORM_MOVIE_METHOD_WITH_NUMBER(int scaleform, const char* methodName, float param1, float param2, float param3, float param4, float param5) {
			Invoker::Invoke<void, 1498>(scaleform, methodName, param1, param2, param3, param4, param5);
		}

		void CALL_SCALEFORM_MOVIE_METHOD_WITH_STRING(int scaleform, const char* methodName, const char* param1, const char* param2, const char* param3, const char* param4, const char* param5) {
			Invoker::Invoke<void, 1499>(scaleform, methodName, param1, param2, param3, param4, param5);
		}

		void CALL_SCALEFORM_MOVIE_METHOD_WITH_NUMBER_AND_STRING(int scaleform, const char* methodName, float floatParam1, float floatParam2, float floatParam3, float floatParam4, float floatParam5, const char* stringParam1, const char* stringParam2, const char* stringParam3, const char* stringParam4, const char* stringParam5) {
			Invoker::Invoke<void, 1500>(scaleform, methodName, floatParam1, floatParam2, floatParam3, floatParam4, floatParam5, stringParam1, stringParam2, stringParam3, stringParam4, stringParam5);
		}

		BOOL BEGIN_SCALEFORM_SCRIPT_HUD_MOVIE_METHOD(int hudComponent, const char* methodName) {
			return Invoker::Invoke<BOOL, 1501>(hudComponent, methodName);
		}

		BOOL BEGIN_SCALEFORM_MOVIE_METHOD(int scaleform, const char* methodName) {
			return Invoker::Invoke<BOOL, 1502>(scaleform, methodName);
		}

		BOOL BEGIN_SCALEFORM_MOVIE_METHOD_ON_FRONTEND(const char* methodName) {
			return Invoker::Invoke<BOOL, 1503>(methodName);
		}

		BOOL BEGIN_SCALEFORM_MOVIE_METHOD_ON_FRONTEND_HEADER(const char* methodName) {
			return Invoker::Invoke<BOOL, 1504>(methodName);
		}

		void END_SCALEFORM_MOVIE_METHOD() {
			Invoker::Invoke<void, 1505>();
		}

		int END_SCALEFORM_MOVIE_METHOD_RETURN_VALUE() {
			return Invoker::Invoke<int, 1506>();
		}

		BOOL IS_SCALEFORM_MOVIE_METHOD_RETURN_VALUE_READY(int methodReturn) {
			return Invoker::Invoke<BOOL, 1507>(methodReturn);
		}

		int GET_SCALEFORM_MOVIE_METHOD_RETURN_VALUE_INT(int methodReturn) {
			return Invoker::Invoke<int, 1508>(methodReturn);
		}

		BOOL GET_SCALEFORM_MOVIE_METHOD_RETURN_VALUE_BOOL(int methodReturn) {
			return Invoker::Invoke<BOOL, 1509>(methodReturn);
		}

		const char* GET_SCALEFORM_MOVIE_METHOD_RETURN_VALUE_STRING(int methodReturn) {
			return Invoker::Invoke<const char*, 1510>(methodReturn);
		}

		void SCALEFORM_MOVIE_METHOD_ADD_PARAM_INT(int value) {
			Invoker::Invoke<void, 1511>(value);
		}

		void SCALEFORM_MOVIE_METHOD_ADD_PARAM_FLOAT(float value) {
			Invoker::Invoke<void, 1512>(value);
		}

		void SCALEFORM_MOVIE_METHOD_ADD_PARAM_BOOL(BOOL value) {
			Invoker::Invoke<void, 1513>(value);
		}

		void BEGIN_TEXT_COMMAND_SCALEFORM_STRING(const char* componentType) {
			Invoker::Invoke<void, 1514>(componentType);
		}

		void END_TEXT_COMMAND_SCALEFORM_STRING() {
			Invoker::Invoke<void, 1515>();
		}

		void END_TEXT_COMMAND_UNPARSED_SCALEFORM_STRING() {
			Invoker::Invoke<void, 1516>();
		}

		void SCALEFORM_MOVIE_METHOD_ADD_PARAM_LITERAL_STRING(const char* string) {
			Invoker::Invoke<void, 1517>(string);
		}

		void SCALEFORM_MOVIE_METHOD_ADD_PARAM_TEXTURE_NAME_STRING(const char* string) {
			Invoker::Invoke<void, 1518>(string);
		}

		void SCALEFORM_MOVIE_METHOD_ADD_PARAM_PLAYER_NAME_STRING(const char* string) {
			Invoker::Invoke<void, 1519>(string);
		}

		BOOL DOES_LATEST_BRIEF_STRING_EXIST(int p0) {
			return Invoker::Invoke<BOOL, 1520>(p0);
		}

		void SCALEFORM_MOVIE_METHOD_ADD_PARAM_LATEST_BRIEF_STRING(int value) {
			Invoker::Invoke<void, 1521>(value);
		}

		void REQUEST_SCALEFORM_SCRIPT_HUD_MOVIE(int hudComponent) {
			Invoker::Invoke<void, 1522>(hudComponent);
		}

		BOOL HAS_SCALEFORM_SCRIPT_HUD_MOVIE_LOADED(int hudComponent) {
			return Invoker::Invoke<BOOL, 1523>(hudComponent);
		}

		void REMOVE_SCALEFORM_SCRIPT_HUD_MOVIE(int hudComponent) {
			Invoker::Invoke<void, 1524>(hudComponent);
		}

		BOOL PASS_KEYBOARD_INPUT_TO_SCALEFORM(int scaleformHandle) {
			return Invoker::Invoke<BOOL, 1525>(scaleformHandle);
		}

		void SET_TV_CHANNEL(int channel) {
			Invoker::Invoke<void, 1526>(channel);
		}

		int GET_TV_CHANNEL() {
			return Invoker::Invoke<int, 1527>();
		}

		void SET_TV_VOLUME(float volume) {
			Invoker::Invoke<void, 1528>(volume);
		}

		float GET_TV_VOLUME() {
			return Invoker::Invoke<float, 1529>();
		}

		void DRAW_TV_CHANNEL(float xPos, float yPos, float xScale, float yScale, float rotation, int red, int green, int blue, int alpha) {
			Invoker::Invoke<void, 1530>(xPos, yPos, xScale, yScale, rotation, red, green, blue, alpha);
		}

		void SET_TV_CHANNEL_PLAYLIST(int tvChannel, const char* playlistName, BOOL restart) {
			Invoker::Invoke<void, 1531>(tvChannel, playlistName, restart);
		}

		void SET_TV_CHANNEL_PLAYLIST_AT_HOUR(int tvChannel, const char* playlistName, int hour) {
			Invoker::Invoke<void, 1532>(tvChannel, playlistName, hour);
		}

		Any _SET_TV_CHANNEL_PLAYLIST_DIRTY(int tvChannel, BOOL p1) {
			return Invoker::Invoke<Any, 1533>(tvChannel, p1);
		}

		void CLEAR_TV_CHANNEL_PLAYLIST(int tvChannel) {
			Invoker::Invoke<void, 1534>(tvChannel);
		}

		BOOL IS_PLAYLIST_ON_CHANNEL(int tvChannel, Any p1) {
			return Invoker::Invoke<BOOL, 1535>(tvChannel, p1);
		}

		BOOL IS_TVSHOW_CURRENTLY_PLAYING(Hash videoCliphash) {
			return Invoker::Invoke<BOOL, 1536>(videoCliphash);
		}

		void ENABLE_MOVIE_KEYFRAME_WAIT(BOOL toggle) {
			Invoker::Invoke<void, 1537>(toggle);
		}

		void SET_TV_PLAYER_WATCHING_THIS_FRAME(Any p0) {
			Invoker::Invoke<void, 1538>(p0);
		}

		Hash GET_CURRENT_TV_CLIP_NAMEHASH() {
			return Invoker::Invoke<Hash, 1539>();
		}

		void ENABLE_MOVIE_SUBTITLES(BOOL toggle) {
			Invoker::Invoke<void, 1540>(toggle);
		}

		BOOL UI3DSCENE_IS_AVAILABLE() {
			return Invoker::Invoke<BOOL, 1541>();
		}

		BOOL UI3DSCENE_PUSH_PRESET(const char* presetName) {
			return Invoker::Invoke<BOOL, 1542>(presetName);
		}

		BOOL UI3DSCENE_ASSIGN_PED_TO_SLOT(const char* presetName, Ped ped, int slot, float posX, float posY, float posZ) {
			return Invoker::Invoke<BOOL, 1543>(presetName, ped, slot, posX, posY, posZ);
		}

		void UI3DSCENE_CLEAR_PATCHED_DATA() {
			Invoker::Invoke<void, 1544>();
		}

		void UI3DSCENE_MAKE_PUSHED_PRESET_PERSISTENT(BOOL toggle) {
			Invoker::Invoke<void, 1545>(toggle);
		}

		void TERRAINGRID_ACTIVATE(BOOL toggle) {
			Invoker::Invoke<void, 1546>(toggle);
		}

		void TERRAINGRID_SET_PARAMS(float x, float y, float z, float forwardX, float forwardY, float forwardZ, float sizeX, float sizeY, float sizeZ, float gridScale, float glowIntensity, float normalHeight, float heightDiff) {
			Invoker::Invoke<void, 1547>(x, y, z, forwardX, forwardY, forwardZ, sizeX, sizeY, sizeZ, gridScale, glowIntensity, normalHeight, heightDiff);
		}

		void TERRAINGRID_SET_COLOURS(int lowR, int lowG, int lowB, int lowAlpha, int r, int g, int b, int alpha, int highR, int highG, int highB, int highAlpha) {
			Invoker::Invoke<void, 1548>(lowR, lowG, lowB, lowAlpha, r, g, b, alpha, highR, highG, highB, highAlpha);
		}

		void ANIMPOSTFX_PLAY(const char* effectName, int duration, BOOL looped) {
			Invoker::Invoke<void, 1549>(effectName, duration, looped);
		}

		void ANIMPOSTFX_STOP(const char* effectName) {
			Invoker::Invoke<void, 1550>(effectName);
		}

		float ANIMPOSTFX_GET_CURRENT_TIME(const char* effectName) {
			return Invoker::Invoke<float, 1551>(effectName);
		}

		BOOL ANIMPOSTFX_IS_RUNNING(const char* effectName) {
			return Invoker::Invoke<BOOL, 1552>(effectName);
		}

		void ANIMPOSTFX_STOP_ALL() {
			Invoker::Invoke<void, 1553>();
		}

		void ANIMPOSTFX_STOP_AND_FLUSH_REQUESTS(const char* effectName) {
			Invoker::Invoke<void, 1554>(effectName);
		}

	} // namespace GRAPHICS

	namespace HUD {

		void BEGIN_TEXT_COMMAND_BUSYSPINNER_ON(const char* string) {
			Invoker::Invoke<void, 6044>(string);
		}

		void END_TEXT_COMMAND_BUSYSPINNER_ON(int busySpinnerType) {
			Invoker::Invoke<void, 6045>(busySpinnerType);
		}

		void BUSYSPINNER_OFF() {
			Invoker::Invoke<void, 6046>();
		}

		void PRELOAD_BUSYSPINNER() {
			Invoker::Invoke<void, 6047>();
		}

		BOOL BUSYSPINNER_IS_ON() {
			return Invoker::Invoke<BOOL, 6048>();
		}

		BOOL BUSYSPINNER_IS_DISPLAYING() {
			return Invoker::Invoke<BOOL, 6049>();
		}

		void DISABLE_PAUSEMENU_SPINNER(BOOL p0) {
			Invoker::Invoke<void, 6050>(p0);
		}

		void SET_MOUSE_CURSOR_THIS_FRAME() {
			Invoker::Invoke<void, 6051>();
		}

		void SET_MOUSE_CURSOR_STYLE(int spriteId) {
			Invoker::Invoke<void, 6052>(spriteId);
		}

		void SET_MOUSE_CURSOR_VISIBLE(BOOL toggle) {
			Invoker::Invoke<void, 6053>(toggle);
		}

		BOOL IS_MOUSE_ROLLED_OVER_INSTRUCTIONAL_BUTTONS() {
			return Invoker::Invoke<BOOL, 6054>();
		}

		BOOL GET_MOUSE_EVENT(int scaleformHandle, Any* p1, Any* p2, Any* p3) {
			return Invoker::Invoke<BOOL, 6055>(scaleformHandle, p1, p2, p3);
		}

		void THEFEED_ONLY_SHOW_TOOLTIPS(BOOL toggle) {
			Invoker::Invoke<void, 6056>(toggle);
		}

		void THEFEED_SET_SCRIPTED_MENU_HEIGHT(float pos) {
			Invoker::Invoke<void, 6057>(pos);
		}

		void THEFEED_HIDE() {
			Invoker::Invoke<void, 6058>();
		}

		void THEFEED_HIDE_THIS_FRAME() {
			Invoker::Invoke<void, 6059>();
		}

		void THEFEED_SHOW() {
			Invoker::Invoke<void, 6060>();
		}

		void THEFEED_FLUSH_QUEUE() {
			Invoker::Invoke<void, 6061>();
		}

		void THEFEED_REMOVE_ITEM(int notificationId) {
			Invoker::Invoke<void, 6062>(notificationId);
		}

		void THEFEED_FORCE_RENDER_ON() {
			Invoker::Invoke<void, 6063>();
		}

		void THEFEED_FORCE_RENDER_OFF() {
			Invoker::Invoke<void, 6064>();
		}

		void THEFEED_PAUSE() {
			Invoker::Invoke<void, 6065>();
		}

		void THEFEED_RESUME() {
			Invoker::Invoke<void, 6066>();
		}

		BOOL THEFEED_IS_PAUSED() {
			return Invoker::Invoke<BOOL, 6067>();
		}

		void THEFEED_REPORT_LOGO_ON() {
			Invoker::Invoke<void, 6068>();
		}

		void THEFEED_REPORT_LOGO_OFF() {
			Invoker::Invoke<void, 6069>();
		}

		int THEFEED_GET_LAST_SHOWN_PHONE_ACTIVATABLE_FEED_ID() {
			return Invoker::Invoke<int, 6070>();
		}

		void THEFEED_AUTO_POST_GAMETIPS_ON() {
			Invoker::Invoke<void, 6071>();
		}

		void THEFEED_AUTO_POST_GAMETIPS_OFF() {
			Invoker::Invoke<void, 6072>();
		}

		void THEFEED_SET_BACKGROUND_COLOR_FOR_NEXT_POST(int hudColorIndex) {
			Invoker::Invoke<void, 6073>(hudColorIndex);
		}

		void THEFEED_SET_RGBA_PARAMETER_FOR_NEXT_MESSAGE(int red, int green, int blue, int alpha) {
			Invoker::Invoke<void, 6074>(red, green, blue, alpha);
		}

		void THEFEED_SET_FLASH_DURATION_PARAMETER_FOR_NEXT_MESSAGE(int count) {
			Invoker::Invoke<void, 6075>(count);
		}

		void THEFEED_SET_VIBRATE_PARAMETER_FOR_NEXT_MESSAGE(BOOL toggle) {
			Invoker::Invoke<void, 6076>(toggle);
		}

		void THEFEED_RESET_ALL_PARAMETERS() {
			Invoker::Invoke<void, 6077>();
		}

		void THEFEED_FREEZE_NEXT_POST() {
			Invoker::Invoke<void, 6078>();
		}

		void THEFEED_CLEAR_FROZEN_POST() {
			Invoker::Invoke<void, 6079>();
		}

		void THEFEED_SET_SNAP_FEED_ITEM_POSITIONS(BOOL p0) {
			Invoker::Invoke<void, 6080>(p0);
		}

		void THEFEED_UPDATE_ITEM_TEXTURE(const char* txdString1, const char* txnString1, const char* txdString2, const char* txnString2) {
			Invoker::Invoke<void, 6081>(txdString1, txnString1, txdString2, txnString2);
		}

		void BEGIN_TEXT_COMMAND_THEFEED_POST(const char* text) {
			Invoker::Invoke<void, 6082>(text);
		}

		int END_TEXT_COMMAND_THEFEED_POST_STATS(const char* statTitle, int iconEnum, BOOL stepVal, int barValue, BOOL isImportant, const char* pictureTextureDict, const char* pictureTextureName) {
			return Invoker::Invoke<int, 6083>(statTitle, iconEnum, stepVal, barValue, isImportant, pictureTextureDict, pictureTextureName);
		}

		int END_TEXT_COMMAND_THEFEED_POST_MESSAGETEXT(const char* txdName, const char* textureName, BOOL flash, int iconType, const char* sender, const char* subject) {
			return Invoker::Invoke<int, 6084>(txdName, textureName, flash, iconType, sender, subject);
		}

		int END_TEXT_COMMAND_THEFEED_POST_MESSAGETEXT_SUBTITLE_LABEL(const char* txdName, const char* textureName, BOOL flash, int iconType, const char* sender, const char* subject) {
			return Invoker::Invoke<int, 6085>(txdName, textureName, flash, iconType, sender, subject);
		}

		int END_TEXT_COMMAND_THEFEED_POST_MESSAGETEXT_TU(const char* txdName, const char* textureName, BOOL flash, int iconType, const char* sender, const char* subject, float duration) {
			return Invoker::Invoke<int, 6086>(txdName, textureName, flash, iconType, sender, subject, duration);
		}

		int END_TEXT_COMMAND_THEFEED_POST_MESSAGETEXT_WITH_CREW_TAG(const char* txdName, const char* textureName, BOOL flash, int iconType, const char* sender, const char* subject, float duration, const char* clanTag) {
			return Invoker::Invoke<int, 6087>(txdName, textureName, flash, iconType, sender, subject, duration, clanTag);
		}

		int END_TEXT_COMMAND_THEFEED_POST_MESSAGETEXT_WITH_CREW_TAG_AND_ADDITIONAL_ICON(const char* txdName, const char* textureName, BOOL flash, int iconType1, const char* sender, const char* subject, float duration, const char* clanTag, int iconType2, int p9) {
			return Invoker::Invoke<int, 6088>(txdName, textureName, flash, iconType1, sender, subject, duration, clanTag, iconType2, p9);
		}

		int END_TEXT_COMMAND_THEFEED_POST_TICKER(BOOL blink, BOOL p1) {
			return Invoker::Invoke<int, 6089>(blink, p1);
		}

		int END_TEXT_COMMAND_THEFEED_POST_TICKER_FORCED(BOOL blink, BOOL p1) {
			return Invoker::Invoke<int, 6090>(blink, p1);
		}

		int END_TEXT_COMMAND_THEFEED_POST_TICKER_WITH_TOKENS(BOOL blink, BOOL p1) {
			return Invoker::Invoke<int, 6091>(blink, p1);
		}

		int END_TEXT_COMMAND_THEFEED_POST_AWARD(const char* textureDict, const char* textureName, int rpBonus, int colorOverlay, const char* titleLabel) {
			return Invoker::Invoke<int, 6092>(textureDict, textureName, rpBonus, colorOverlay, titleLabel);
		}

		int END_TEXT_COMMAND_THEFEED_POST_CREWTAG(BOOL p0, BOOL p1, int* p2, int p3, BOOL isLeader, BOOL unk0, int clanDesc, int R, int G, int B) {
			return Invoker::Invoke<int, 6093>(p0, p1, p2, p3, isLeader, unk0, clanDesc, R, G, B);
		}

		int END_TEXT_COMMAND_THEFEED_POST_CREWTAG_WITH_GAME_NAME(BOOL p0, BOOL p1, int* p2, int p3, BOOL isLeader, BOOL unk0, int clanDesc, const char* playerName, int R, int G, int B) {
			return Invoker::Invoke<int, 6094>(p0, p1, p2, p3, isLeader, unk0, clanDesc, playerName, R, G, B);
		}

		int END_TEXT_COMMAND_THEFEED_POST_UNLOCK(const char* gxtLabel1, int p1, const char* gxtLabel2) {
			return Invoker::Invoke<int, 6095>(gxtLabel1, p1, gxtLabel2);
		}

		int END_TEXT_COMMAND_THEFEED_POST_UNLOCK_TU(const char* gxtLabel1, int p1, const char* gxtLabel2, int p3) {
			return Invoker::Invoke<int, 6096>(gxtLabel1, p1, gxtLabel2, p3);
		}

		int END_TEXT_COMMAND_THEFEED_POST_UNLOCK_TU_WITH_COLOR(Any p0, Any p1, Any p2, Any p3, Any p4, Any p5) {
			return Invoker::Invoke<int, 6097>(p0, p1, p2, p3, p4, p5);
		}

		int END_TEXT_COMMAND_THEFEED_POST_MPTICKER(BOOL blink, BOOL p1) {
			return Invoker::Invoke<int, 6098>(blink, p1);
		}

		int END_TEXT_COMMAND_THEFEED_POST_CREW_RANKUP_WITH_LITERAL_FLAG(const char* p0, const char* p1, const char* p2, BOOL p3, BOOL p4) {
			return Invoker::Invoke<int, 6099>(p0, p1, p2, p3, p4);
		}

		int END_TEXT_COMMAND_THEFEED_POST_VERSUS_TU(const char* txdName1, const char* textureName1, int count1, const char* txdName2, const char* textureName2, int count2, int hudColor1, int hudColor2) {
			return Invoker::Invoke<int, 6100>(txdName1, textureName1, count1, txdName2, textureName2, count2, hudColor1, hudColor2);
		}

		int END_TEXT_COMMAND_THEFEED_POST_REPLAY(int type, int image, const char* text) {
			return Invoker::Invoke<int, 6101>(type, image, text);
		}

		int END_TEXT_COMMAND_THEFEED_POST_REPLAY_INPUT(int type, const char* button, const char* text) {
			return Invoker::Invoke<int, 6102>(type, button, text);
		}

		void BEGIN_TEXT_COMMAND_PRINT(const char* GxtEntry) {
			Invoker::Invoke<void, 6103>(GxtEntry);
		}

		void END_TEXT_COMMAND_PRINT(int duration, BOOL drawImmediately) {
			Invoker::Invoke<void, 6104>(duration, drawImmediately);
		}

		void BEGIN_TEXT_COMMAND_IS_MESSAGE_DISPLAYED(const char* text) {
			Invoker::Invoke<void, 6105>(text);
		}

		BOOL END_TEXT_COMMAND_IS_MESSAGE_DISPLAYED() {
			return Invoker::Invoke<BOOL, 6106>();
		}

		void BEGIN_TEXT_COMMAND_DISPLAY_TEXT(const char* text) {
			Invoker::Invoke<void, 6107>(text);
		}

		void END_TEXT_COMMAND_DISPLAY_TEXT(float x, float y, int p2) {
			Invoker::Invoke<void, 6108>(x, y, p2);
		}

		void BEGIN_TEXT_COMMAND_GET_SCREEN_WIDTH_OF_DISPLAY_TEXT(const char* text) {
			Invoker::Invoke<void, 6109>(text);
		}

		float END_TEXT_COMMAND_GET_SCREEN_WIDTH_OF_DISPLAY_TEXT(BOOL p0) {
			return Invoker::Invoke<float, 6110>(p0);
		}

		void BEGIN_TEXT_COMMAND_GET_NUMBER_OF_LINES_FOR_STRING(const char* entry) {
			Invoker::Invoke<void, 6111>(entry);
		}

		int END_TEXT_COMMAND_GET_NUMBER_OF_LINES_FOR_STRING(float x, float y) {
			return Invoker::Invoke<int, 6112>(x, y);
		}

		void BEGIN_TEXT_COMMAND_DISPLAY_HELP(const char* inputType) {
			Invoker::Invoke<void, 6113>(inputType);
		}

		void END_TEXT_COMMAND_DISPLAY_HELP(int p0, BOOL loop, BOOL beep, int shape) {
			Invoker::Invoke<void, 6114>(p0, loop, beep, shape);
		}

		void BEGIN_TEXT_COMMAND_IS_THIS_HELP_MESSAGE_BEING_DISPLAYED(const char* labelName) {
			Invoker::Invoke<void, 6115>(labelName);
		}

		BOOL END_TEXT_COMMAND_IS_THIS_HELP_MESSAGE_BEING_DISPLAYED(int p0) {
			return Invoker::Invoke<BOOL, 6116>(p0);
		}

		void BEGIN_TEXT_COMMAND_SET_BLIP_NAME(const char* textLabel) {
			Invoker::Invoke<void, 6117>(textLabel);
		}

		void END_TEXT_COMMAND_SET_BLIP_NAME(Blip blip) {
			Invoker::Invoke<void, 6118>(blip);
		}

		void BEGIN_TEXT_COMMAND_ADD_DIRECTLY_TO_PREVIOUS_BRIEFS(const char* p0) {
			Invoker::Invoke<void, 6119>(p0);
		}

		void END_TEXT_COMMAND_ADD_DIRECTLY_TO_PREVIOUS_BRIEFS(BOOL p0) {
			Invoker::Invoke<void, 6120>(p0);
		}

		void BEGIN_TEXT_COMMAND_CLEAR_PRINT(const char* text) {
			Invoker::Invoke<void, 6121>(text);
		}

		void END_TEXT_COMMAND_CLEAR_PRINT() {
			Invoker::Invoke<void, 6122>();
		}

		void BEGIN_TEXT_COMMAND_OVERRIDE_BUTTON_TEXT(const char* gxtEntry) {
			Invoker::Invoke<void, 6123>(gxtEntry);
		}

		void END_TEXT_COMMAND_OVERRIDE_BUTTON_TEXT(int p0) {
			Invoker::Invoke<void, 6124>(p0);
		}

		void ADD_TEXT_COMPONENT_INTEGER(int value) {
			Invoker::Invoke<void, 6125>(value);
		}

		void ADD_TEXT_COMPONENT_FLOAT(float value, int decimalPlaces) {
			Invoker::Invoke<void, 6126>(value, decimalPlaces);
		}

		void ADD_TEXT_COMPONENT_SUBSTRING_TEXT_LABEL(const char* labelName) {
			Invoker::Invoke<void, 6127>(labelName);
		}

		void ADD_TEXT_COMPONENT_SUBSTRING_TEXT_LABEL_HASH_KEY(Hash gxtEntryHash) {
			Invoker::Invoke<void, 6128>(gxtEntryHash);
		}

		void ADD_TEXT_COMPONENT_SUBSTRING_BLIP_NAME(Blip blip) {
			Invoker::Invoke<void, 6129>(blip);
		}

		void ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME(const char* text) {
			Invoker::Invoke<void, 6130>(text);
		}

		void ADD_TEXT_COMPONENT_SUBSTRING_TIME(int timestamp, int flags) {
			Invoker::Invoke<void, 6131>(timestamp, flags);
		}

		void ADD_TEXT_COMPONENT_FORMATTED_INTEGER(int value, BOOL commaSeparated) {
			Invoker::Invoke<void, 6132>(value, commaSeparated);
		}

		void ADD_TEXT_COMPONENT_SUBSTRING_PHONE_NUMBER(const char* p0, int p1) {
			Invoker::Invoke<void, 6133>(p0, p1);
		}

		void ADD_TEXT_COMPONENT_SUBSTRING_WEBSITE(const char* website) {
			Invoker::Invoke<void, 6134>(website);
		}

		void ADD_TEXT_COMPONENT_SUBSTRING_KEYBOARD_DISPLAY(const char* string) {
			Invoker::Invoke<void, 6135>(string);
		}

		void SET_COLOUR_OF_NEXT_TEXT_COMPONENT(int hudColor) {
			Invoker::Invoke<void, 6136>(hudColor);
		}

		const char* GET_CHARACTER_FROM_AUDIO_CONVERSATION_FILENAME(const char* text, int position, int length) {
			return Invoker::Invoke<const char*, 6137>(text, position, length);
		}

		const char* GET_CHARACTER_FROM_AUDIO_CONVERSATION_FILENAME_WITH_BYTE_LIMIT(const char* text, int position, int length, int maxLength) {
			return Invoker::Invoke<const char*, 6138>(text, position, length, maxLength);
		}

		const char* GET_CHARACTER_FROM_AUDIO_CONVERSATION_FILENAME_BYTES(const char* text, int startPosition, int endPosition) {
			return Invoker::Invoke<const char*, 6139>(text, startPosition, endPosition);
		}

		const char* GET_FILENAME_FOR_AUDIO_CONVERSATION(const char* labelName) {
			return Invoker::Invoke<const char*, 6140>(labelName);
		}

		void CLEAR_PRINTS() {
			Invoker::Invoke<void, 6141>();
		}

		void CLEAR_BRIEF() {
			Invoker::Invoke<void, 6142>();
		}

		void CLEAR_ALL_HELP_MESSAGES() {
			Invoker::Invoke<void, 6143>();
		}

		void CLEAR_THIS_PRINT(const char* p0) {
			Invoker::Invoke<void, 6144>(p0);
		}

		void CLEAR_SMALL_PRINTS() {
			Invoker::Invoke<void, 6145>();
		}

		BOOL DOES_TEXT_BLOCK_EXIST(const char* gxt) {
			return Invoker::Invoke<BOOL, 6146>(gxt);
		}

		void REQUEST_ADDITIONAL_TEXT(const char* gxt, int slot) {
			Invoker::Invoke<void, 6147>(gxt, slot);
		}

		void REQUEST_ADDITIONAL_TEXT_FOR_DLC(const char* gxt, int slot) {
			Invoker::Invoke<void, 6148>(gxt, slot);
		}

		BOOL HAS_ADDITIONAL_TEXT_LOADED(int slot) {
			return Invoker::Invoke<BOOL, 6149>(slot);
		}

		void CLEAR_ADDITIONAL_TEXT(int p0, BOOL p1) {
			Invoker::Invoke<void, 6150>(p0, p1);
		}

		BOOL IS_STREAMING_ADDITIONAL_TEXT(int p0) {
			return Invoker::Invoke<BOOL, 6151>(p0);
		}

		BOOL HAS_THIS_ADDITIONAL_TEXT_LOADED(const char* gxt, int slot) {
			return Invoker::Invoke<BOOL, 6152>(gxt, slot);
		}

		BOOL IS_MESSAGE_BEING_DISPLAYED() {
			return Invoker::Invoke<BOOL, 6153>();
		}

		BOOL DOES_TEXT_LABEL_EXIST(const char* gxt) {
			return Invoker::Invoke<BOOL, 6154>(gxt);
		}

		const char* GET_FIRST_N_CHARACTERS_OF_LITERAL_STRING(const char* string, int length) {
			return Invoker::Invoke<const char*, 6155>(string, length);
		}

		int GET_LENGTH_OF_STRING_WITH_THIS_TEXT_LABEL(const char* gxt) {
			return Invoker::Invoke<int, 6156>(gxt);
		}

		int GET_LENGTH_OF_LITERAL_STRING(const char* string) {
			return Invoker::Invoke<int, 6157>(string);
		}

		int GET_LENGTH_OF_LITERAL_STRING_IN_BYTES(const char* string) {
			return Invoker::Invoke<int, 6158>(string);
		}

		const char* GET_STREET_NAME_FROM_HASH_KEY(Hash hash) {
			return Invoker::Invoke<const char*, 6159>(hash);
		}

		BOOL IS_HUD_PREFERENCE_SWITCHED_ON() {
			return Invoker::Invoke<BOOL, 6160>();
		}

		BOOL IS_RADAR_PREFERENCE_SWITCHED_ON() {
			return Invoker::Invoke<BOOL, 6161>();
		}

		BOOL IS_SUBTITLE_PREFERENCE_SWITCHED_ON() {
			return Invoker::Invoke<BOOL, 6162>();
		}

		void DISPLAY_HUD(BOOL toggle) {
			Invoker::Invoke<void, 6163>(toggle);
		}

		void DISPLAY_HUD_WHEN_NOT_IN_STATE_OF_PLAY_THIS_FRAME() {
			Invoker::Invoke<void, 6164>();
		}

		void DISPLAY_HUD_WHEN_PAUSED_THIS_FRAME() {
			Invoker::Invoke<void, 6165>();
		}

		void DISPLAY_RADAR(BOOL toggle) {
			Invoker::Invoke<void, 6166>(toggle);
		}

		void SET_FAKE_SPECTATOR_MODE(BOOL toggle) {
			Invoker::Invoke<void, 6167>(toggle);
		}

		BOOL GET_FAKE_SPECTATOR_MODE() {
			return Invoker::Invoke<BOOL, 6168>();
		}

		BOOL IS_HUD_HIDDEN() {
			return Invoker::Invoke<BOOL, 6169>();
		}

		BOOL IS_RADAR_HIDDEN() {
			return Invoker::Invoke<BOOL, 6170>();
		}

		BOOL IS_MINIMAP_RENDERING() {
			return Invoker::Invoke<BOOL, 6171>();
		}

		void USE_VEHICLE_TARGETING_RETICULE(Any p0) {
			Invoker::Invoke<void, 6172>(p0);
		}

		void _USE_VEHICLE_TARGETING_RETICULE_ON_VEHICLES(BOOL enable) {
			Invoker::Invoke<void, 6173>(enable);
		}

		void ADD_VALID_VEHICLE_HIT_HASH(Any p0) {
			Invoker::Invoke<void, 6174>(p0);
		}

		void CLEAR_VALID_VEHICLE_HIT_HASHES() {
			Invoker::Invoke<void, 6175>();
		}

		void SET_BLIP_ROUTE(Blip blip, BOOL enabled) {
			Invoker::Invoke<void, 6176>(blip, enabled);
		}

		void CLEAR_ALL_BLIP_ROUTES() {
			Invoker::Invoke<void, 6177>();
		}

		void SET_BLIP_ROUTE_COLOUR(Blip blip, int colour) {
			Invoker::Invoke<void, 6178>(blip, colour);
		}

		void SET_FORCE_SHOW_GPS(BOOL toggle) {
			Invoker::Invoke<void, 6179>(toggle);
		}

		void SET_USE_SET_DESTINATION_IN_PAUSE_MAP(BOOL toggle) {
			Invoker::Invoke<void, 6180>(toggle);
		}

		void SET_BLOCK_WANTED_FLASH(BOOL disabled) {
			Invoker::Invoke<void, 6181>(disabled);
		}

		void ADD_NEXT_MESSAGE_TO_PREVIOUS_BRIEFS(BOOL p0) {
			Invoker::Invoke<void, 6182>(p0);
		}

		void FORCE_NEXT_MESSAGE_TO_PREVIOUS_BRIEFS_LIST(int p0) {
			Invoker::Invoke<void, 6183>(p0);
		}

		void SET_RADAR_ZOOM_PRECISE(float zoom) {
			Invoker::Invoke<void, 6184>(zoom);
		}

		void SET_RADAR_ZOOM(int zoomLevel) {
			Invoker::Invoke<void, 6185>(zoomLevel);
		}

		void SET_RADAR_ZOOM_TO_BLIP(Blip blip, float zoom) {
			Invoker::Invoke<void, 6186>(blip, zoom);
		}

		void SET_RADAR_ZOOM_TO_DISTANCE(float zoom) {
			Invoker::Invoke<void, 6187>(zoom);
		}

		void UPDATE_RADAR_ZOOM_TO_BLIP() {
			Invoker::Invoke<void, 6188>();
		}

		void GET_HUD_COLOUR(int hudColorIndex, int* r, int* g, int* b, int* a) {
			Invoker::Invoke<void, 6189>(hudColorIndex, r, g, b, a);
		}

		void SET_SCRIPT_VARIABLE_HUD_COLOUR(int r, int g, int b, int a) {
			Invoker::Invoke<void, 6190>(r, g, b, a);
		}

		void SET_SECOND_SCRIPT_VARIABLE_HUD_COLOUR(int r, int g, int b, int a) {
			Invoker::Invoke<void, 6191>(r, g, b, a);
		}

		void REPLACE_HUD_COLOUR(int hudColorIndex, int hudColorIndex2) {
			Invoker::Invoke<void, 6192>(hudColorIndex, hudColorIndex2);
		}

		void REPLACE_HUD_COLOUR_WITH_RGBA(int hudColorIndex, int r, int g, int b, int a) {
			Invoker::Invoke<void, 6193>(hudColorIndex, r, g, b, a);
		}

		void SET_ABILITY_BAR_VISIBILITY(BOOL visible) {
			Invoker::Invoke<void, 6194>(visible);
		}

		void SET_ALLOW_ABILITY_BAR(BOOL toggle) {
			Invoker::Invoke<void, 6195>(toggle);
		}

		void FLASH_ABILITY_BAR(int millisecondsToFlash) {
			Invoker::Invoke<void, 6196>(millisecondsToFlash);
		}

		void SET_ABILITY_BAR_VALUE(float p0, float p1) {
			Invoker::Invoke<void, 6197>(p0, p1);
		}

		void FLASH_WANTED_DISPLAY(BOOL p0) {
			Invoker::Invoke<void, 6198>(p0);
		}

		void FORCE_OFF_WANTED_STAR_FLASH(BOOL toggle) {
			Invoker::Invoke<void, 6199>(toggle);
		}

		void SET_CUSTOM_MP_HUD_COLOR(int hudColorId) {
			Invoker::Invoke<void, 6200>(hudColorId);
		}

		float GET_RENDERED_CHARACTER_HEIGHT(float size, int font) {
			return Invoker::Invoke<float, 6201>(size, font);
		}

		void SET_TEXT_SCALE(float scale, float size) {
			Invoker::Invoke<void, 6202>(scale, size);
		}

		void SET_TEXT_COLOUR(int red, int green, int blue, int alpha) {
			Invoker::Invoke<void, 6203>(red, green, blue, alpha);
		}

		void SET_TEXT_CENTRE(BOOL align) {
			Invoker::Invoke<void, 6204>(align);
		}

		void SET_TEXT_RIGHT_JUSTIFY(BOOL toggle) {
			Invoker::Invoke<void, 6205>(toggle);
		}

		void SET_TEXT_JUSTIFICATION(int justifyType) {
			Invoker::Invoke<void, 6206>(justifyType);
		}

		void SET_TEXT_LINE_HEIGHT_MULT(float lineHeightMult) {
			Invoker::Invoke<void, 6207>(lineHeightMult);
		}

		void SET_TEXT_WRAP(float start, float end) {
			Invoker::Invoke<void, 6208>(start, end);
		}

		void SET_TEXT_LEADING(int p0) {
			Invoker::Invoke<void, 6209>(p0);
		}

		void SET_TEXT_PROPORTIONAL(BOOL p0) {
			Invoker::Invoke<void, 6210>(p0);
		}

		void SET_TEXT_FONT(int fontType) {
			Invoker::Invoke<void, 6211>(fontType);
		}

		void SET_TEXT_DROP_SHADOW() {
			Invoker::Invoke<void, 6212>();
		}

		void SET_TEXT_DROPSHADOW(int distance, int r, int g, int b, int a) {
			Invoker::Invoke<void, 6213>(distance, r, g, b, a);
		}

		void SET_TEXT_OUTLINE() {
			Invoker::Invoke<void, 6214>();
		}

		void SET_TEXT_EDGE(int p0, int r, int g, int b, int a) {
			Invoker::Invoke<void, 6215>(p0, r, g, b, a);
		}

		void SET_TEXT_RENDER_ID(int renderId) {
			Invoker::Invoke<void, 6216>(renderId);
		}

		int GET_DEFAULT_SCRIPT_RENDERTARGET_RENDER_ID() {
			return Invoker::Invoke<int, 6217>();
		}

		BOOL REGISTER_NAMED_RENDERTARGET(const char* name, BOOL p1) {
			return Invoker::Invoke<BOOL, 6218>(name, p1);
		}

		BOOL IS_NAMED_RENDERTARGET_REGISTERED(const char* name) {
			return Invoker::Invoke<BOOL, 6219>(name);
		}

		BOOL RELEASE_NAMED_RENDERTARGET(const char* name) {
			return Invoker::Invoke<BOOL, 6220>(name);
		}

		void LINK_NAMED_RENDERTARGET(Hash modelHash) {
			Invoker::Invoke<void, 6221>(modelHash);
		}

		int GET_NAMED_RENDERTARGET_RENDER_ID(const char* name) {
			return Invoker::Invoke<int, 6222>(name);
		}

		BOOL IS_NAMED_RENDERTARGET_LINKED(Hash modelHash) {
			return Invoker::Invoke<BOOL, 6223>(modelHash);
		}

		void CLEAR_HELP(BOOL toggle) {
			Invoker::Invoke<void, 6224>(toggle);
		}

		BOOL IS_HELP_MESSAGE_ON_SCREEN() {
			return Invoker::Invoke<BOOL, 6225>();
		}

		BOOL HAS_SCRIPT_HIDDEN_HELP_THIS_FRAME() {
			return Invoker::Invoke<BOOL, 6226>();
		}

		BOOL IS_HELP_MESSAGE_BEING_DISPLAYED() {
			return Invoker::Invoke<BOOL, 6227>();
		}

		BOOL IS_HELP_MESSAGE_FADING_OUT() {
			return Invoker::Invoke<BOOL, 6228>();
		}

		void SET_HELP_MESSAGE_STYLE(int style, int hudColor, int alpha, int p3, int p4) {
			Invoker::Invoke<void, 6229>(style, hudColor, alpha, p3, p4);
		}

		int GET_STANDARD_BLIP_ENUM_ID() {
			return Invoker::Invoke<int, 6230>();
		}

		int GET_WAYPOINT_BLIP_ENUM_ID() {
			return Invoker::Invoke<int, 6231>();
		}

		int GET_NUMBER_OF_ACTIVE_BLIPS() {
			return Invoker::Invoke<int, 6232>();
		}

		Blip GET_NEXT_BLIP_INFO_ID(int blipSprite) {
			return Invoker::Invoke<Blip, 6233>(blipSprite);
		}

		Blip GET_FIRST_BLIP_INFO_ID(int blipSprite) {
			return Invoker::Invoke<Blip, 6234>(blipSprite);
		}

		Blip GET_CLOSEST_BLIP_INFO_ID(int blipSprite) {
			return Invoker::Invoke<Blip, 6235>(blipSprite);
		}

		Vector3 GET_BLIP_INFO_ID_COORD(Blip blip) {
			return Invoker::Invoke<Vector3, 6236>(blip);
		}

		int GET_BLIP_INFO_ID_DISPLAY(Blip blip) {
			return Invoker::Invoke<int, 6237>(blip);
		}

		int GET_BLIP_INFO_ID_TYPE(Blip blip) {
			return Invoker::Invoke<int, 6238>(blip);
		}

		Entity GET_BLIP_INFO_ID_ENTITY_INDEX(Blip blip) {
			return Invoker::Invoke<Entity, 6239>(blip);
		}

		Pickup GET_BLIP_INFO_ID_PICKUP_INDEX(Blip blip) {
			return Invoker::Invoke<Pickup, 6240>(blip);
		}

		Blip GET_BLIP_FROM_ENTITY(Entity entity) {
			return Invoker::Invoke<Blip, 6241>(entity);
		}

		Blip ADD_BLIP_FOR_RADIUS(float posX, float posY, float posZ, float radius) {
			return Invoker::Invoke<Blip, 6242>(posX, posY, posZ, radius);
		}

		Blip ADD_BLIP_FOR_AREA(float x, float y, float z, float width, float height) {
			return Invoker::Invoke<Blip, 6243>(x, y, z, width, height);
		}

		Blip ADD_BLIP_FOR_ENTITY(Entity entity) {
			return Invoker::Invoke<Blip, 6244>(entity);
		}

		Blip ADD_BLIP_FOR_PICKUP(Pickup pickup) {
			return Invoker::Invoke<Blip, 6245>(pickup);
		}

		Blip ADD_BLIP_FOR_COORD(float x, float y, float z) {
			return Invoker::Invoke<Blip, 6246>(x, y, z);
		}

		void TRIGGER_SONAR_BLIP(float posX, float posY, float posZ, float radius, int p4) {
			Invoker::Invoke<void, 6247>(posX, posY, posZ, radius, p4);
		}

		void ALLOW_SONAR_BLIPS(BOOL toggle) {
			Invoker::Invoke<void, 6248>(toggle);
		}

		void SET_BLIP_COORDS(Blip blip, float posX, float posY, float posZ) {
			Invoker::Invoke<void, 6249>(blip, posX, posY, posZ);
		}

		Vector3 GET_BLIP_COORDS(Blip blip) {
			return Invoker::Invoke<Vector3, 6250>(blip);
		}

		void SET_BLIP_SPRITE(Blip blip, int spriteId) {
			Invoker::Invoke<void, 6251>(blip, spriteId);
		}

		int GET_BLIP_SPRITE(Blip blip) {
			return Invoker::Invoke<int, 6252>(blip);
		}

		void SET_COP_BLIP_SPRITE(int p0, float p1) {
			Invoker::Invoke<void, 6253>(p0, p1);
		}

		void SET_COP_BLIP_SPRITE_AS_STANDARD() {
			Invoker::Invoke<void, 6254>();
		}

		void SET_BLIP_NAME_FROM_TEXT_FILE(Blip blip, const char* gxtEntry) {
			Invoker::Invoke<void, 6255>(blip, gxtEntry);
		}

		void SET_BLIP_NAME_TO_PLAYER_NAME(Blip blip, Player player) {
			Invoker::Invoke<void, 6256>(blip, player);
		}

		void SET_BLIP_ALPHA(Blip blip, int alpha) {
			Invoker::Invoke<void, 6257>(blip, alpha);
		}

		int GET_BLIP_ALPHA(Blip blip) {
			return Invoker::Invoke<int, 6258>(blip);
		}

		void SET_BLIP_FADE(Blip blip, int opacity, int duration) {
			Invoker::Invoke<void, 6259>(blip, opacity, duration);
		}

		int GET_BLIP_FADE_DIRECTION(Blip blip) {
			return Invoker::Invoke<int, 6260>(blip);
		}

		void SET_BLIP_ROTATION(Blip blip, int rotation) {
			Invoker::Invoke<void, 6261>(blip, rotation);
		}

		void SET_BLIP_ROTATION_WITH_FLOAT(Blip blip, float heading) {
			Invoker::Invoke<void, 6262>(blip, heading);
		}

		int GET_BLIP_ROTATION(Blip blip) {
			return Invoker::Invoke<int, 6263>(blip);
		}

		void SET_BLIP_FLASH_TIMER(Blip blip, int duration) {
			Invoker::Invoke<void, 6264>(blip, duration);
		}

		void SET_BLIP_FLASH_INTERVAL(Blip blip, Any p1) {
			Invoker::Invoke<void, 6265>(blip, p1);
		}

		void SET_BLIP_COLOUR(Blip blip, int color) {
			Invoker::Invoke<void, 6266>(blip, color);
		}

		void SET_BLIP_SECONDARY_COLOUR(Blip blip, int r, int g, int b) {
			Invoker::Invoke<void, 6267>(blip, r, g, b);
		}

		int GET_BLIP_COLOUR(Blip blip) {
			return Invoker::Invoke<int, 6268>(blip);
		}

		int GET_BLIP_HUD_COLOUR(Blip blip) {
			return Invoker::Invoke<int, 6269>(blip);
		}

		BOOL IS_BLIP_SHORT_RANGE(Blip blip) {
			return Invoker::Invoke<BOOL, 6270>(blip);
		}

		BOOL IS_BLIP_ON_MINIMAP(Blip blip) {
			return Invoker::Invoke<BOOL, 6271>(blip);
		}

		BOOL DOES_BLIP_HAVE_GPS_ROUTE(Blip blip) {
			return Invoker::Invoke<BOOL, 6272>(blip);
		}

		void SET_BLIP_HIDDEN_ON_LEGEND(Blip blip, BOOL toggle) {
			Invoker::Invoke<void, 6273>(blip, toggle);
		}

		void SET_BLIP_HIGH_DETAIL(Blip blip, BOOL toggle) {
			Invoker::Invoke<void, 6274>(blip, toggle);
		}

		void SET_BLIP_AS_MISSION_CREATOR_BLIP(Blip blip, BOOL toggle) {
			Invoker::Invoke<void, 6275>(blip, toggle);
		}

		BOOL IS_MISSION_CREATOR_BLIP(Blip blip) {
			return Invoker::Invoke<BOOL, 6276>(blip);
		}

		Blip GET_NEW_SELECTED_MISSION_CREATOR_BLIP() {
			return Invoker::Invoke<Blip, 6277>();
		}

		BOOL IS_HOVERING_OVER_MISSION_CREATOR_BLIP() {
			return Invoker::Invoke<BOOL, 6278>();
		}

		void SHOW_START_MISSION_INSTRUCTIONAL_BUTTON(BOOL toggle) {
			Invoker::Invoke<void, 6279>(toggle);
		}

		void SHOW_CONTACT_INSTRUCTIONAL_BUTTON(BOOL toggle) {
			Invoker::Invoke<void, 6280>(toggle);
		}

		void _SHOW_PURCHASE_INSTRUCTIONAL_BUTTON(BOOL toggle) {
			Invoker::Invoke<void, 6688>(toggle);
		}

		void RELOAD_MAP_MENU() {
			Invoker::Invoke<void, 6281>();
		}

		void SET_BLIP_MARKER_LONG_DISTANCE(Any p0, Any p1) {
			Invoker::Invoke<void, 6282>(p0, p1);
		}

		void SET_BLIP_FLASHES(Blip blip, BOOL toggle) {
			Invoker::Invoke<void, 6283>(blip, toggle);
		}

		void SET_BLIP_FLASHES_ALTERNATE(Blip blip, BOOL toggle) {
			Invoker::Invoke<void, 6284>(blip, toggle);
		}

		BOOL IS_BLIP_FLASHING(Blip blip) {
			return Invoker::Invoke<BOOL, 6285>(blip);
		}

		void SET_BLIP_AS_SHORT_RANGE(Blip blip, BOOL toggle) {
			Invoker::Invoke<void, 6286>(blip, toggle);
		}

		void SET_BLIP_SCALE(Blip blip, float scale) {
			Invoker::Invoke<void, 6287>(blip, scale);
		}

		void SET_BLIP_SCALE_2D(Blip blip, float xScale, float yScale) {
			Invoker::Invoke<void, 6288>(blip, xScale, yScale);
		}

		void SET_BLIP_PRIORITY(Blip blip, int priority) {
			Invoker::Invoke<void, 6289>(blip, priority);
		}

		void SET_BLIP_DISPLAY(Blip blip, int displayId) {
			Invoker::Invoke<void, 6290>(blip, displayId);
		}

		void SET_BLIP_CATEGORY(Blip blip, int index) {
			Invoker::Invoke<void, 6291>(blip, index);
		}

		void REMOVE_BLIP(Blip* blip) {
			Invoker::Invoke<void, 6292>(blip);
		}

		void SET_BLIP_AS_FRIENDLY(Blip blip, BOOL toggle) {
			Invoker::Invoke<void, 6293>(blip, toggle);
		}

		void PULSE_BLIP(Blip blip) {
			Invoker::Invoke<void, 6294>(blip);
		}

		void SHOW_NUMBER_ON_BLIP(Blip blip, int number) {
			Invoker::Invoke<void, 6295>(blip, number);
		}

		void HIDE_NUMBER_ON_BLIP(Blip blip) {
			Invoker::Invoke<void, 6296>(blip);
		}

		void SHOW_HEIGHT_ON_BLIP(Blip blip, BOOL toggle) {
			Invoker::Invoke<void, 6297>(blip, toggle);
		}

		void SHOW_TICK_ON_BLIP(Blip blip, BOOL toggle) {
			Invoker::Invoke<void, 6298>(blip, toggle);
		}

		void SHOW_GOLD_TICK_ON_BLIP(Blip blip, BOOL toggle) {
			Invoker::Invoke<void, 6299>(blip, toggle);
		}

		void SHOW_FOR_SALE_ICON_ON_BLIP(Blip blip, BOOL toggle) {
			Invoker::Invoke<void, 6300>(blip, toggle);
		}

		void SHOW_HEADING_INDICATOR_ON_BLIP(Blip blip, BOOL toggle) {
			Invoker::Invoke<void, 6301>(blip, toggle);
		}

		void SHOW_OUTLINE_INDICATOR_ON_BLIP(Blip blip, BOOL toggle) {
			Invoker::Invoke<void, 6302>(blip, toggle);
		}

		void SHOW_FRIEND_INDICATOR_ON_BLIP(Blip blip, BOOL toggle) {
			Invoker::Invoke<void, 6303>(blip, toggle);
		}

		void SHOW_CREW_INDICATOR_ON_BLIP(Blip blip, BOOL toggle) {
			Invoker::Invoke<void, 6304>(blip, toggle);
		}

		void SET_BLIP_EXTENDED_HEIGHT_THRESHOLD(Blip blip, BOOL toggle) {
			Invoker::Invoke<void, 6305>(blip, toggle);
		}

		void SET_BLIP_SHORT_HEIGHT_THRESHOLD(Any p0, Any p1) {
			Invoker::Invoke<void, 6306>(p0, p1);
		}

		void SET_BLIP_USE_HEIGHT_INDICATOR_ON_EDGE(Blip blip, Any p1) {
			Invoker::Invoke<void, 6307>(blip, p1);
		}

		void SET_BLIP_AS_MINIMAL_ON_EDGE(Blip blip, BOOL toggle) {
			Invoker::Invoke<void, 6308>(blip, toggle);
		}

		void SET_RADIUS_BLIP_EDGE(Blip blip, BOOL toggle) {
			Invoker::Invoke<void, 6309>(blip, toggle);
		}

		BOOL DOES_BLIP_EXIST(Blip blip) {
			return Invoker::Invoke<BOOL, 6310>(blip);
		}

		void SET_WAYPOINT_OFF() {
			Invoker::Invoke<void, 6311>();
		}

		void DELETE_WAYPOINTS_FROM_THIS_PLAYER() {
			Invoker::Invoke<void, 6312>();
		}

		void REFRESH_WAYPOINT() {
			Invoker::Invoke<void, 6313>();
		}

		BOOL IS_WAYPOINT_ACTIVE() {
			return Invoker::Invoke<BOOL, 6314>();
		}

		void SET_NEW_WAYPOINT(float x, float y) {
			Invoker::Invoke<void, 6315>(x, y);
		}

		int GET_WAYPOINT_CLEAR_ON_ARRIVAL_MODE() {
			return Invoker::Invoke<int, 6687>();
		}

		void SET_WAYPOINT_CLEAR_ON_ARRIVAL_MODE(int mode) {
			Invoker::Invoke<void, 6686>(mode);
		}

		void SET_BLIP_BRIGHT(Blip blip, BOOL toggle) {
			Invoker::Invoke<void, 6316>(blip, toggle);
		}

		void SET_BLIP_SHOW_CONE(Blip blip, BOOL toggle, int hudColorIndex) {
			Invoker::Invoke<void, 6317>(blip, toggle, hudColorIndex);
		}

		void REMOVE_COP_BLIP_FROM_PED(Ped ped) {
			Invoker::Invoke<void, 6318>(ped);
		}

		void SETUP_FAKE_CONE_DATA(Blip blip, float p1, float p2, float p3, float p4, float p5, float p6, Any p7, int p8) {
			Invoker::Invoke<void, 6319>(blip, p1, p2, p3, p4, p5, p6, p7, p8);
		}

		void REMOVE_FAKE_CONE_DATA(Blip blip) {
			Invoker::Invoke<void, 6320>(blip);
		}

		void CLEAR_FAKE_CONE_ARRAY() {
			Invoker::Invoke<void, 6321>();
		}

		void _SET_BLIP_GPS_ROUTE_DISPLAY_DISTANCE(Blip blip, int blipChangeParam46, BOOL blipChangeParam47) {
			Invoker::Invoke<void, 6322>(blip, blipChangeParam46, blipChangeParam47);
		}

		BOOL SET_MINIMAP_COMPONENT(int componentId, BOOL toggle, int overrideColor) {
			return Invoker::Invoke<BOOL, 6323>(componentId, toggle, overrideColor);
		}

		void SET_MINIMAP_SONAR_SWEEP(BOOL toggle) {
			Invoker::Invoke<void, 6324>(toggle);
		}

		void SHOW_ACCOUNT_PICKER() {
			Invoker::Invoke<void, 6325>();
		}

		Blip GET_MAIN_PLAYER_BLIP_ID() {
			return Invoker::Invoke<Blip, 6326>();
		}

		void SET_PM_WARNINGSCREEN_ACTIVE(BOOL p0) {
			Invoker::Invoke<void, 6327>(p0);
		}

		void HIDE_LOADING_ON_FADE_THIS_FRAME() {
			Invoker::Invoke<void, 6328>();
		}

		void SET_RADAR_AS_INTERIOR_THIS_FRAME(Hash interior, float x, float y, int z, int zoom) {
			Invoker::Invoke<void, 6329>(interior, x, y, z, zoom);
		}

		void SET_INSIDE_VERY_SMALL_INTERIOR(BOOL toggle) {
			Invoker::Invoke<void, 6330>(toggle);
		}

		void SET_INSIDE_VERY_LARGE_INTERIOR(BOOL toggle) {
			Invoker::Invoke<void, 6331>(toggle);
		}

		void SET_RADAR_AS_EXTERIOR_THIS_FRAME() {
			Invoker::Invoke<void, 6332>();
		}

		void SET_FAKE_PAUSEMAP_PLAYER_POSITION_THIS_FRAME(float x, float y) {
			Invoker::Invoke<void, 6333>(x, y);
		}

		void SET_FAKE_GPS_PLAYER_POSITION_THIS_FRAME(float x, float y, float z) {
			Invoker::Invoke<void, 6334>(x, y, z);
		}

		BOOL IS_PAUSEMAP_IN_INTERIOR_MODE() {
			return Invoker::Invoke<BOOL, 6335>();
		}

		void HIDE_MINIMAP_EXTERIOR_MAP_THIS_FRAME() {
			Invoker::Invoke<void, 6336>();
		}

		void HIDE_MINIMAP_INTERIOR_MAP_THIS_FRAME() {
			Invoker::Invoke<void, 6337>();
		}

		void SET_USE_ISLAND_MAP(BOOL toggle) {
			Invoker::Invoke<void, 6338>(toggle);
		}

		void _SET_PAUSE_EXTERIOR_RENDERING_WHILE_IN_INTERIOR() {
			Invoker::Invoke<void, 6339>();
		}

		void DONT_TILT_MINIMAP_THIS_FRAME() {
			Invoker::Invoke<void, 6340>();
		}

		void DONT_ZOOM_MINIMAP_WHEN_RUNNING_THIS_FRAME() {
			Invoker::Invoke<void, 6341>();
		}

		void DONT_ZOOM_MINIMAP_WHEN_SNIPING_THIS_FRAME() {
			Invoker::Invoke<void, 6342>();
		}

		void SET_WIDESCREEN_FORMAT(Any p0) {
			Invoker::Invoke<void, 6343>(p0);
		}

		void DISPLAY_AREA_NAME(BOOL toggle) {
			Invoker::Invoke<void, 6344>(toggle);
		}

		void DISPLAY_CASH(BOOL toggle) {
			Invoker::Invoke<void, 6345>(toggle);
		}

		void USE_FAKE_MP_CASH(BOOL toggle) {
			Invoker::Invoke<void, 6346>(toggle);
		}

		void CHANGE_FAKE_MP_CASH(int cash, int bank) {
			Invoker::Invoke<void, 6347>(cash, bank);
		}

		void DISPLAY_AMMO_THIS_FRAME(BOOL display) {
			Invoker::Invoke<void, 6348>(display);
		}

		void DISPLAY_SNIPER_SCOPE_THIS_FRAME() {
			Invoker::Invoke<void, 6349>();
		}

		void HIDE_HUD_AND_RADAR_THIS_FRAME() {
			Invoker::Invoke<void, 6350>();
		}

		void ALLOW_DISPLAY_OF_MULTIPLAYER_CASH_TEXT(BOOL allow) {
			Invoker::Invoke<void, 6351>(allow);
		}

		void SET_MULTIPLAYER_WALLET_CASH() {
			Invoker::Invoke<void, 6352>();
		}

		void REMOVE_MULTIPLAYER_WALLET_CASH() {
			Invoker::Invoke<void, 6353>();
		}

		void SET_MULTIPLAYER_BANK_CASH() {
			Invoker::Invoke<void, 6354>();
		}

		void REMOVE_MULTIPLAYER_BANK_CASH() {
			Invoker::Invoke<void, 6355>();
		}

		void SET_MULTIPLAYER_HUD_CASH(int p0, BOOL p1) {
			Invoker::Invoke<void, 6356>(p0, p1);
		}

		void REMOVE_MULTIPLAYER_HUD_CASH() {
			Invoker::Invoke<void, 6357>();
		}

		void HIDE_HELP_TEXT_THIS_FRAME() {
			Invoker::Invoke<void, 6358>();
		}

		BOOL IS_IME_IN_PROGRESS() {
			return Invoker::Invoke<BOOL, 6359>();
		}

		void DISPLAY_HELP_TEXT_THIS_FRAME(const char* message, BOOL p1) {
			Invoker::Invoke<void, 6360>(message, p1);
		}

		void HUD_FORCE_WEAPON_WHEEL(BOOL show) {
			Invoker::Invoke<void, 6361>(show);
		}

		void HUD_FORCE_SPECIAL_VEHICLE_WEAPON_WHEEL() {
			Invoker::Invoke<void, 6362>();
		}

		void HUD_SUPPRESS_WEAPON_WHEEL_RESULTS_THIS_FRAME() {
			Invoker::Invoke<void, 6363>();
		}

		Hash HUD_GET_WEAPON_WHEEL_CURRENTLY_HIGHLIGHTED() {
			return Invoker::Invoke<Hash, 6364>();
		}

		void HUD_SET_WEAPON_WHEEL_TOP_SLOT(Hash weaponHash) {
			Invoker::Invoke<void, 6365>(weaponHash);
		}

		Hash HUD_GET_WEAPON_WHEEL_TOP_SLOT(int weaponTypeIndex) {
			return Invoker::Invoke<Hash, 6366>(weaponTypeIndex);
		}

		void HUD_SHOWING_CHARACTER_SWITCH_SELECTION(BOOL toggle) {
			Invoker::Invoke<void, 6367>(toggle);
		}

		void SET_GPS_FLAGS(int p0, float p1) {
			Invoker::Invoke<void, 6368>(p0, p1);
		}

		void CLEAR_GPS_FLAGS() {
			Invoker::Invoke<void, 6369>();
		}

		void SET_RACE_TRACK_RENDER(BOOL toggle) {
			Invoker::Invoke<void, 6370>(toggle);
		}

		void CLEAR_GPS_RACE_TRACK() {
			Invoker::Invoke<void, 6371>();
		}

		void START_GPS_CUSTOM_ROUTE(int hudColor, BOOL displayOnFoot, BOOL followPlayer) {
			Invoker::Invoke<void, 6372>(hudColor, displayOnFoot, followPlayer);
		}

		void ADD_POINT_TO_GPS_CUSTOM_ROUTE(float x, float y, float z) {
			Invoker::Invoke<void, 6373>(x, y, z);
		}

		void SET_GPS_CUSTOM_ROUTE_RENDER(BOOL toggle, int radarThickness, int mapThickness) {
			Invoker::Invoke<void, 6374>(toggle, radarThickness, mapThickness);
		}

		void CLEAR_GPS_CUSTOM_ROUTE() {
			Invoker::Invoke<void, 6375>();
		}

		void START_GPS_MULTI_ROUTE(int hudColor, BOOL routeFromPlayer, BOOL displayOnFoot) {
			Invoker::Invoke<void, 6376>(hudColor, routeFromPlayer, displayOnFoot);
		}

		void ADD_POINT_TO_GPS_MULTI_ROUTE(float x, float y, float z) {
			Invoker::Invoke<void, 6377>(x, y, z);
		}

		void SET_GPS_MULTI_ROUTE_RENDER(BOOL toggle) {
			Invoker::Invoke<void, 6378>(toggle);
		}

		void CLEAR_GPS_MULTI_ROUTE() {
			Invoker::Invoke<void, 6379>();
		}

		void CLEAR_GPS_PLAYER_WAYPOINT() {
			Invoker::Invoke<void, 6380>();
		}

		void SET_GPS_FLASHES(BOOL toggle) {
			Invoker::Invoke<void, 6381>(toggle);
		}

		void SET_PLAYER_ICON_COLOUR(int color) {
			Invoker::Invoke<void, 6382>(color);
		}

		void FLASH_MINIMAP_DISPLAY() {
			Invoker::Invoke<void, 6383>();
		}

		void FLASH_MINIMAP_DISPLAY_WITH_COLOR(int hudColorIndex) {
			Invoker::Invoke<void, 6384>(hudColorIndex);
		}

		void TOGGLE_STEALTH_RADAR(BOOL toggle) {
			Invoker::Invoke<void, 6385>(toggle);
		}

		void SET_MINIMAP_IN_SPECTATOR_MODE(BOOL toggle, Ped ped) {
			Invoker::Invoke<void, 6386>(toggle, ped);
		}

		void SET_MISSION_NAME(BOOL p0, const char* name) {
			Invoker::Invoke<void, 6387>(p0, name);
		}

		void SET_MISSION_NAME_FOR_UGC_MISSION(BOOL p0, const char* name) {
			Invoker::Invoke<void, 6388>(p0, name);
		}

		void SET_DESCRIPTION_FOR_UGC_MISSION_EIGHT_STRINGS(BOOL p0, const char* p1, const char* p2, const char* p3, const char* p4, const char* p5, const char* p6, const char* p7, const char* p8) {
			Invoker::Invoke<void, 6389>(p0, p1, p2, p3, p4, p5, p6, p7, p8);
		}

		void SET_MINIMAP_BLOCK_WAYPOINT(BOOL toggle) {
			Invoker::Invoke<void, 6390>(toggle);
		}

		void SET_MINIMAP_IN_PROLOGUE(BOOL toggle) {
			Invoker::Invoke<void, 6391>(toggle);
		}

		void SET_MINIMAP_BACKGROUND_HIDDEN(BOOL toggle) {
			Invoker::Invoke<void, 6572>(toggle);
		}

		void SET_MINIMAP_HIDE_FOW(BOOL toggle) {
			Invoker::Invoke<void, 6392>(toggle);
		}

		float GET_MINIMAP_FOW_DISCOVERY_RATIO() {
			return Invoker::Invoke<float, 6393>();
		}

		BOOL GET_MINIMAP_FOW_COORDINATE_IS_REVEALED(float x, float y, float z) {
			return Invoker::Invoke<BOOL, 6394>(x, y, z);
		}

		void SET_MINIMAP_FOW_DO_NOT_UPDATE(BOOL p0) {
			Invoker::Invoke<void, 6395>(p0);
		}

		void SET_MINIMAP_FOW_REVEAL_COORDINATE(float x, float y, float z) {
			Invoker::Invoke<void, 6396>(x, y, z);
		}

		void SET_MINIMAP_GOLF_COURSE(int hole) {
			Invoker::Invoke<void, 6397>(hole);
		}

		void SET_MINIMAP_GOLF_COURSE_OFF() {
			Invoker::Invoke<void, 6398>();
		}

		void LOCK_MINIMAP_ANGLE(int angle) {
			Invoker::Invoke<void, 6399>(angle);
		}

		void UNLOCK_MINIMAP_ANGLE() {
			Invoker::Invoke<void, 6400>();
		}

		void LOCK_MINIMAP_POSITION(float x, float y) {
			Invoker::Invoke<void, 6401>(x, y);
		}

		void UNLOCK_MINIMAP_POSITION() {
			Invoker::Invoke<void, 6402>();
		}

		void SET_FAKE_MINIMAP_MAX_ALTIMETER_HEIGHT(float altitude, BOOL p1, Any p2) {
			Invoker::Invoke<void, 6403>(altitude, p1, p2);
		}

		void SET_HEALTH_HUD_DISPLAY_VALUES(int health, int capacity, BOOL wasAdded) {
			Invoker::Invoke<void, 6404>(health, capacity, wasAdded);
		}

		void SET_MAX_HEALTH_HUD_DISPLAY(int maximumValue) {
			Invoker::Invoke<void, 6405>(maximumValue);
		}

		void SET_MAX_ARMOUR_HUD_DISPLAY(int maximumValue) {
			Invoker::Invoke<void, 6406>(maximumValue);
		}

		void SET_BIGMAP_ACTIVE(BOOL toggleBigMap, BOOL showFullMap) {
			Invoker::Invoke<void, 6407>(toggleBigMap, showFullMap);
		}

		BOOL IS_HUD_COMPONENT_ACTIVE(int id) {
			return Invoker::Invoke<BOOL, 6408>(id);
		}

		BOOL IS_SCRIPTED_HUD_COMPONENT_ACTIVE(int id) {
			return Invoker::Invoke<BOOL, 6409>(id);
		}

		void HIDE_SCRIPTED_HUD_COMPONENT_THIS_FRAME(int id) {
			Invoker::Invoke<void, 6410>(id);
		}

		void SHOW_SCRIPTED_HUD_COMPONENT_THIS_FRAME(int id) {
			Invoker::Invoke<void, 6411>(id);
		}

		BOOL IS_SCRIPTED_HUD_COMPONENT_HIDDEN_THIS_FRAME(int id) {
			return Invoker::Invoke<BOOL, 6412>(id);
		}

		void HIDE_HUD_COMPONENT_THIS_FRAME(int id) {
			Invoker::Invoke<void, 6413>(id);
		}

		BOOL IS_HUD_COMPONENT_HIDDEN_THIS_FRAME(int id) {
			return Invoker::Invoke<BOOL, 6685>(id);
		}

		void SHOW_HUD_COMPONENT_THIS_FRAME(int id) {
			Invoker::Invoke<void, 6414>(id);
		}

		void HIDE_STREET_AND_CAR_NAMES_THIS_FRAME() {
			Invoker::Invoke<void, 6415>();
		}

		void RESET_RETICULE_VALUES() {
			Invoker::Invoke<void, 6416>();
		}

		void RESET_HUD_COMPONENT_VALUES(int id) {
			Invoker::Invoke<void, 6417>(id);
		}

		void SET_HUD_COMPONENT_POSITION(int id, float x, float y) {
			Invoker::Invoke<void, 6418>(id, x, y);
		}

		Vector3 GET_HUD_COMPONENT_POSITION(int id) {
			return Invoker::Invoke<Vector3, 6419>(id);
		}

		void CLEAR_REMINDER_MESSAGE() {
			Invoker::Invoke<void, 6420>();
		}

		int GET_HUD_SCREEN_POSITION_FROM_WORLD_POSITION(float worldX, float worldY, float worldZ, float* screenX, float* screenY) {
			return Invoker::Invoke<int, 6421>(worldX, worldY, worldZ, screenX, screenY);
		}

		void OPEN_REPORTUGC_MENU() {
			Invoker::Invoke<void, 6422>();
		}

		void FORCE_CLOSE_REPORTUGC_MENU() {
			Invoker::Invoke<void, 6423>();
		}

		BOOL IS_REPORTUGC_MENU_OPEN() {
			return Invoker::Invoke<BOOL, 6424>();
		}

		BOOL IS_FLOATING_HELP_TEXT_ON_SCREEN(int hudIndex) {
			return Invoker::Invoke<BOOL, 6425>(hudIndex);
		}

		void SET_FLOATING_HELP_TEXT_SCREEN_POSITION(int hudIndex, float x, float y) {
			Invoker::Invoke<void, 6426>(hudIndex, x, y);
		}

		void SET_FLOATING_HELP_TEXT_WORLD_POSITION(int hudIndex, float x, float y, float z) {
			Invoker::Invoke<void, 6427>(hudIndex, x, y, z);
		}

		void SET_FLOATING_HELP_TEXT_TO_ENTITY(int hudIndex, Entity entity, float offsetX, float offsetY) {
			Invoker::Invoke<void, 6428>(hudIndex, entity, offsetX, offsetY);
		}

		void SET_FLOATING_HELP_TEXT_STYLE(int hudIndex, int p1, int p2, int p3, int p4, int p5) {
			Invoker::Invoke<void, 6429>(hudIndex, p1, p2, p3, p4, p5);
		}

		void CLEAR_FLOATING_HELP(int hudIndex, BOOL p1) {
			Invoker::Invoke<void, 6430>(hudIndex, p1);
		}

		void CREATE_MP_GAMER_TAG_WITH_CREW_COLOR(Player player, const char* username, BOOL pointedClanTag, BOOL isRockstarClan, const char* clanTag, int clanFlag, int r, int g, int b) {
			Invoker::Invoke<void, 6431>(player, username, pointedClanTag, isRockstarClan, clanTag, clanFlag, r, g, b);
		}

		BOOL IS_MP_GAMER_TAG_MOVIE_ACTIVE() {
			return Invoker::Invoke<BOOL, 6432>();
		}

		int CREATE_FAKE_MP_GAMER_TAG(Ped ped, const char* username, BOOL pointedClanTag, BOOL isRockstarClan, const char* clanTag, int clanFlag) {
			return Invoker::Invoke<int, 6433>(ped, username, pointedClanTag, isRockstarClan, clanTag, clanFlag);
		}

		void REMOVE_MP_GAMER_TAG(int gamerTagId) {
			Invoker::Invoke<void, 6434>(gamerTagId);
		}

		BOOL IS_MP_GAMER_TAG_ACTIVE(int gamerTagId) {
			return Invoker::Invoke<BOOL, 6435>(gamerTagId);
		}

		BOOL IS_MP_GAMER_TAG_FREE(int gamerTagId) {
			return Invoker::Invoke<BOOL, 6436>(gamerTagId);
		}

		void SET_MP_GAMER_TAG_VISIBILITY(int gamerTagId, int component, BOOL toggle, Any p3) {
			Invoker::Invoke<void, 6437>(gamerTagId, component, toggle, p3);
		}

		void SET_ALL_MP_GAMER_TAGS_VISIBILITY(int gamerTagId, BOOL toggle) {
			Invoker::Invoke<void, 6438>(gamerTagId, toggle);
		}

		void SET_MP_GAMER_TAGS_SHOULD_USE_VEHICLE_HEALTH(int gamerTagId, BOOL toggle) {
			Invoker::Invoke<void, 6439>(gamerTagId, toggle);
		}

		void SET_MP_GAMER_TAGS_SHOULD_USE_POINTS_HEALTH(int gamerTagId, BOOL toggle) {
			Invoker::Invoke<void, 6440>(gamerTagId, toggle);
		}

		void SET_MP_GAMER_TAGS_POINT_HEALTH(int gamerTagId, int value, int maximumValue) {
			Invoker::Invoke<void, 6441>(gamerTagId, value, maximumValue);
		}

		void SET_MP_GAMER_TAG_COLOUR(int gamerTagId, int component, int hudColorIndex) {
			Invoker::Invoke<void, 6442>(gamerTagId, component, hudColorIndex);
		}

		void SET_MP_GAMER_TAG_HEALTH_BAR_COLOUR(int gamerTagId, int hudColorIndex) {
			Invoker::Invoke<void, 6443>(gamerTagId, hudColorIndex);
		}

		void SET_MP_GAMER_TAG_ALPHA(int gamerTagId, int component, int alpha) {
			Invoker::Invoke<void, 6444>(gamerTagId, component, alpha);
		}

		void SET_MP_GAMER_TAG_WANTED_LEVEL(int gamerTagId, int wantedlvl) {
			Invoker::Invoke<void, 6445>(gamerTagId, wantedlvl);
		}

		void SET_MP_GAMER_TAG_NUM_PACKAGES(int gamerTagId, int p1) {
			Invoker::Invoke<void, 6446>(gamerTagId, p1);
		}

		void SET_MP_GAMER_TAG_NAME(int gamerTagId, const char* string) {
			Invoker::Invoke<void, 6447>(gamerTagId, string);
		}

		BOOL IS_UPDATING_MP_GAMER_TAG_NAME_AND_CREW_DETAILS(int gamerTagId) {
			return Invoker::Invoke<BOOL, 6448>(gamerTagId);
		}

		void SET_MP_GAMER_TAG_BIG_TEXT(int gamerTagId, const char* string) {
			Invoker::Invoke<void, 6449>(gamerTagId, string);
		}

		int GET_CURRENT_WEBPAGE_ID() {
			return Invoker::Invoke<int, 6450>();
		}

		int GET_CURRENT_WEBSITE_ID() {
			return Invoker::Invoke<int, 6451>();
		}

		int GET_GLOBAL_ACTIONSCRIPT_FLAG(int flagIndex) {
			return Invoker::Invoke<int, 6452>(flagIndex);
		}

		void RESET_GLOBAL_ACTIONSCRIPT_FLAG(int flagIndex) {
			Invoker::Invoke<void, 6453>(flagIndex);
		}

		BOOL IS_WARNING_MESSAGE_READY_FOR_CONTROL() {
			return Invoker::Invoke<BOOL, 6454>();
		}

		void SET_WARNING_MESSAGE(const char* titleMsg, int flags, const char* promptMsg, BOOL p3, int p4, const char* p5, const char* p6, BOOL showBackground, int errorCode) {
			Invoker::Invoke<void, 6455>(titleMsg, flags, promptMsg, p3, p4, p5, p6, showBackground, errorCode);
		}

		void SET_WARNING_MESSAGE_WITH_HEADER(const char* entryHeader, const char* entryLine1, int instructionalKey, const char* entryLine2, BOOL p4, Any p5, Any* showBackground, Any* p7, BOOL p8, Any p9) {
			Invoker::Invoke<void, 6456>(entryHeader, entryLine1, instructionalKey, entryLine2, p4, p5, showBackground, p7, p8, p9);
		}

		void SET_WARNING_MESSAGE_WITH_HEADER_AND_SUBSTRING_FLAGS(const char* entryHeader, const char* entryLine1, int instructionalKey, const char* entryLine2, BOOL p4, Any p5, Any additionalIntInfo, const char* additionalTextInfoLine1, const char* additionalTextInfoLine2, BOOL showBackground, int errorCode) {
			Invoker::Invoke<void, 6457>(entryHeader, entryLine1, instructionalKey, entryLine2, p4, p5, additionalIntInfo, additionalTextInfoLine1, additionalTextInfoLine2, showBackground, errorCode);
		}

		void SET_WARNING_MESSAGE_WITH_HEADER_EXTENDED(const char* entryHeader, const char* entryLine1, int flags, const char* entryLine2, BOOL p4, Any p5, Any* p6, Any* p7, BOOL showBg, Any p9, Any p10) {
			Invoker::Invoke<void, 6458>(entryHeader, entryLine1, flags, entryLine2, p4, p5, p6, p7, showBg, p9, p10);
		}

		void SET_WARNING_MESSAGE_WITH_HEADER_AND_SUBSTRING_FLAGS_EXTENDED(const char* labelTitle, const char* labelMessage, int p2, int p3, const char* labelMessage2, BOOL p5, int p6, int p7, const char* p8, const char* p9, BOOL background, int errorCode) {
			Invoker::Invoke<void, 6459>(labelTitle, labelMessage, p2, p3, labelMessage2, p5, p6, p7, p8, p9, background, errorCode);
		}

		Hash GET_WARNING_SCREEN_MESSAGE_HASH() {
			return Invoker::Invoke<Hash, 6460>();
		}

		BOOL SET_WARNING_MESSAGE_OPTION_ITEMS(int index, const char* name, int cash, int rp, int lvl, int colour) {
			return Invoker::Invoke<BOOL, 6461>(index, name, cash, rp, lvl, colour);
		}

		BOOL SET_WARNING_MESSAGE_OPTION_HIGHLIGHT(Any p0) {
			return Invoker::Invoke<BOOL, 6462>(p0);
		}

		void REMOVE_WARNING_MESSAGE_OPTION_ITEMS() {
			Invoker::Invoke<void, 6463>();
		}

		BOOL IS_WARNING_MESSAGE_ACTIVE() {
			return Invoker::Invoke<BOOL, 6464>();
		}

		void CLEAR_DYNAMIC_PAUSE_MENU_ERROR_MESSAGE() {
			Invoker::Invoke<void, 6465>();
		}

		void CUSTOM_MINIMAP_SET_ACTIVE(BOOL toggle) {
			Invoker::Invoke<void, 6466>(toggle);
		}

		void CUSTOM_MINIMAP_SET_BLIP_OBJECT(int spriteId) {
			Invoker::Invoke<void, 6467>(spriteId);
		}

		int CUSTOM_MINIMAP_CREATE_BLIP(float x, float y, float z) {
			return Invoker::Invoke<int, 6468>(x, y, z);
		}

		void CUSTOM_MINIMAP_CLEAR_BLIPS() {
			Invoker::Invoke<void, 6469>();
		}

		BOOL FORCE_SONAR_BLIPS_THIS_FRAME() {
			return Invoker::Invoke<BOOL, 6470>();
		}

		Blip GET_NORTH_BLID_INDEX() {
			return Invoker::Invoke<Blip, 6471>();
		}

		void DISPLAY_PLAYER_NAME_TAGS_ON_BLIPS(BOOL toggle) {
			Invoker::Invoke<void, 6472>(toggle);
		}

		void DRAW_FRONTEND_BACKGROUND_THIS_FRAME() {
			Invoker::Invoke<void, 6473>();
		}

		void DRAW_HUD_OVER_FADE_THIS_FRAME() {
			Invoker::Invoke<void, 6474>();
		}

		void ACTIVATE_FRONTEND_MENU(Hash menuhash, BOOL togglePause, int component) {
			Invoker::Invoke<void, 6475>(menuhash, togglePause, component);
		}

		void RESTART_FRONTEND_MENU(Hash menuHash, int p1) {
			Invoker::Invoke<void, 6476>(menuHash, p1);
		}

		Hash GET_CURRENT_FRONTEND_MENU_VERSION() {
			return Invoker::Invoke<Hash, 6477>();
		}

		void SET_PAUSE_MENU_ACTIVE(BOOL toggle) {
			Invoker::Invoke<void, 6478>(toggle);
		}

		void DISABLE_FRONTEND_THIS_FRAME() {
			Invoker::Invoke<void, 6479>();
		}

		void SUPPRESS_FRONTEND_RENDERING_THIS_FRAME() {
			Invoker::Invoke<void, 6480>();
		}

		void ALLOW_PAUSE_WHEN_NOT_IN_STATE_OF_PLAY_THIS_FRAME() {
			Invoker::Invoke<void, 6481>();
		}

		void SET_FRONTEND_ACTIVE(BOOL active) {
			Invoker::Invoke<void, 6482>(active);
		}

		BOOL IS_PAUSE_MENU_ACTIVE() {
			return Invoker::Invoke<BOOL, 6483>();
		}

		BOOL IS_STORE_PENDING_NETWORK_SHUTDOWN_TO_OPEN() {
			return Invoker::Invoke<BOOL, 6484>();
		}

		int GET_PAUSE_MENU_STATE() {
			return Invoker::Invoke<int, 6485>();
		}

		Vector3 GET_PAUSE_MENU_POSITION() {
			return Invoker::Invoke<Vector3, 6486>();
		}

		BOOL IS_PAUSE_MENU_RESTARTING() {
			return Invoker::Invoke<BOOL, 6487>();
		}

		void FORCE_SCRIPTED_GFX_WHEN_FRONTEND_ACTIVE(const char* p0) {
			Invoker::Invoke<void, 6488>(p0);
		}

		void PAUSE_MENUCEPTION_GO_DEEPER(int page) {
			Invoker::Invoke<void, 6489>(page);
		}

		void PAUSE_MENUCEPTION_THE_KICK() {
			Invoker::Invoke<void, 6490>();
		}

		void PAUSE_TOGGLE_FULLSCREEN_MAP(Any p0) {
			Invoker::Invoke<void, 6491>(p0);
		}

		void PAUSE_MENU_ACTIVATE_CONTEXT(Hash contextHash) {
			Invoker::Invoke<void, 6492>(contextHash);
		}

		void PAUSE_MENU_DEACTIVATE_CONTEXT(Hash contextHash) {
			Invoker::Invoke<void, 6493>(contextHash);
		}

		BOOL PAUSE_MENU_IS_CONTEXT_ACTIVE(Hash contextHash) {
			return Invoker::Invoke<BOOL, 6494>(contextHash);
		}

		BOOL PAUSE_MENU_IS_CONTEXT_MENU_ACTIVE() {
			return Invoker::Invoke<BOOL, 6495>();
		}

		int PAUSE_MENU_GET_HAIR_COLOUR_INDEX() {
			return Invoker::Invoke<int, 6496>();
		}

		int PAUSE_MENU_GET_MOUSE_HOVER_INDEX() {
			return Invoker::Invoke<int, 6497>();
		}

		int PAUSE_MENU_GET_MOUSE_HOVER_UNIQUE_ID() {
			return Invoker::Invoke<int, 6498>();
		}

		BOOL PAUSE_MENU_GET_MOUSE_CLICK_EVENT(Any* p0, Any* p1, Any* p2) {
			return Invoker::Invoke<BOOL, 6499>(p0, p1, p2);
		}

		void PAUSE_MENU_REDRAW_INSTRUCTIONAL_BUTTONS(int p0) {
			Invoker::Invoke<void, 6500>(p0);
		}

		void PAUSE_MENU_SET_BUSY_SPINNER(BOOL p0, int position, int spinnerIndex) {
			Invoker::Invoke<void, 6501>(p0, position, spinnerIndex);
		}

		void PAUSE_MENU_SET_WARN_ON_TAB_CHANGE(BOOL p0) {
			Invoker::Invoke<void, 6502>(p0);
		}

		void PAUSE_MENU_SET_CLOUD_BUSY_SPINNER(const char* TextLabel, BOOL bNoMenu, BOOL bBlackBackground) {
			Invoker::Invoke<void, 6684>(TextLabel, bNoMenu, bBlackBackground);
		}

		void PAUSE_MENU_CLEAR_CLOUD_BUSY_SPINNER() {
			Invoker::Invoke<void, 6683>();
		}

		BOOL IS_FRONTEND_READY_FOR_CONTROL() {
			return Invoker::Invoke<BOOL, 6503>();
		}

		void TAKE_CONTROL_OF_FRONTEND() {
			Invoker::Invoke<void, 6504>();
		}

		void RELEASE_CONTROL_OF_FRONTEND() {
			Invoker::Invoke<void, 6505>();
		}

		BOOL CODE_WANTS_SCRIPT_TO_TAKE_CONTROL() {
			return Invoker::Invoke<BOOL, 6506>();
		}

		int GET_SCREEN_CODE_WANTS_SCRIPT_TO_CONTROL() {
			return Invoker::Invoke<int, 6507>();
		}

		BOOL IS_NAVIGATING_MENU_CONTENT() {
			return Invoker::Invoke<BOOL, 6508>();
		}

		BOOL HAS_MENU_TRIGGER_EVENT_OCCURRED() {
			return Invoker::Invoke<BOOL, 6509>();
		}

		BOOL HAS_MENU_LAYOUT_CHANGED_EVENT_OCCURRED() {
			return Invoker::Invoke<BOOL, 6510>();
		}

		void SET_SAVEGAME_LIST_UNIQUE_ID(Any p0) {
			Invoker::Invoke<void, 6511>(p0);
		}

		void GET_MENU_TRIGGER_EVENT_DETAILS(int* lastItemMenuId, int* selectedItemUniqueId) {
			Invoker::Invoke<void, 6512>(lastItemMenuId, selectedItemUniqueId);
		}

		void GET_MENU_LAYOUT_CHANGED_EVENT_DETAILS(int* lastItemMenuId, int* selectedItemMenuId, int* selectedItemUniqueId) {
			Invoker::Invoke<void, 6513>(lastItemMenuId, selectedItemMenuId, selectedItemUniqueId);
		}

		BOOL GET_PM_PLAYER_CREW_COLOR(int* r, int* g, int* b) {
			return Invoker::Invoke<BOOL, 6514>(r, g, b);
		}

		BOOL GET_MENU_PED_INT_STAT(Any p0, Any* p1) {
			return Invoker::Invoke<BOOL, 6515>(p0, p1);
		}

		BOOL GET_CHARACTER_MENU_PED_INT_STAT(Any p0, Any* p1, Any p2) {
			return Invoker::Invoke<BOOL, 6516>(p0, p1, p2);
		}

		BOOL GET_MENU_PED_MASKED_INT_STAT(Hash statHash, int* outValue, int mask, BOOL p3) {
			return Invoker::Invoke<BOOL, 6517>(statHash, outValue, mask, p3);
		}

		BOOL GET_CHARACTER_MENU_PED_MASKED_INT_STAT(Hash statHash, Any* outValue, int p2, int mask, BOOL p4) {
			return Invoker::Invoke<BOOL, 6518>(statHash, outValue, p2, mask, p4);
		}

		BOOL GET_MENU_PED_FLOAT_STAT(Hash statHash, float* outValue) {
			return Invoker::Invoke<BOOL, 6519>(statHash, outValue);
		}

		BOOL GET_CHARACTER_MENU_PED_FLOAT_STAT(float statHash, float* outValue, BOOL p2) {
			return Invoker::Invoke<BOOL, 6520>(statHash, outValue, p2);
		}

		BOOL GET_MENU_PED_BOOL_STAT(Hash statHash, BOOL* outValue) {
			return Invoker::Invoke<BOOL, 6521>(statHash, outValue);
		}

		void CLEAR_PED_IN_PAUSE_MENU() {
			Invoker::Invoke<void, 6522>();
		}

		void GIVE_PED_TO_PAUSE_MENU(Ped ped, int p1) {
			Invoker::Invoke<void, 6523>(ped, p1);
		}

		void SET_PAUSE_MENU_PED_LIGHTING(BOOL state) {
			Invoker::Invoke<void, 6524>(state);
		}

		void SET_PAUSE_MENU_PED_SLEEP_STATE(BOOL state) {
			Invoker::Invoke<void, 6525>(state);
		}

		void OPEN_ONLINE_POLICIES_MENU() {
			Invoker::Invoke<void, 6526>();
		}

		BOOL ARE_ONLINE_POLICIES_UP_TO_DATE() {
			return Invoker::Invoke<BOOL, 6527>();
		}

		BOOL IS_ONLINE_POLICIES_MENU_ACTIVE() {
			return Invoker::Invoke<BOOL, 6528>();
		}

		void OPEN_SOCIAL_CLUB_MENU(Hash menu) {
			Invoker::Invoke<void, 6529>(menu);
		}

		void CLOSE_SOCIAL_CLUB_MENU() {
			Invoker::Invoke<void, 6530>();
		}

		void SET_SOCIAL_CLUB_TOUR(const char* name) {
			Invoker::Invoke<void, 6531>(name);
		}

		BOOL IS_SOCIAL_CLUB_ACTIVE() {
			return Invoker::Invoke<BOOL, 6532>();
		}

		void SET_TEXT_INPUT_BOX_ENABLED(BOOL p0) {
			Invoker::Invoke<void, 6533>(p0);
		}

		void FORCE_CLOSE_TEXT_INPUT_BOX() {
			Invoker::Invoke<void, 6534>();
		}

		void SET_ALLOW_COMMA_ON_TEXT_INPUT(Any p0) {
			Invoker::Invoke<void, 6535>(p0);
		}

		void OVERRIDE_MP_TEXT_CHAT_TEAM_STRING(Hash gxtEntryHash) {
			Invoker::Invoke<void, 6536>(gxtEntryHash);
		}

		BOOL IS_MP_TEXT_CHAT_TYPING() {
			return Invoker::Invoke<BOOL, 6537>();
		}

		void CLOSE_MP_TEXT_CHAT() {
			Invoker::Invoke<void, 6538>();
		}

		void MP_TEXT_CHAT_IS_TEAM_JOB(Any p0) {
			Invoker::Invoke<void, 6539>(p0);
		}

		void OVERRIDE_MP_TEXT_CHAT_COLOR(int p0, int hudColor) {
			Invoker::Invoke<void, 6540>(p0, hudColor);
		}

		void MP_TEXT_CHAT_DISABLE(BOOL toggle) {
			Invoker::Invoke<void, 6541>(toggle);
		}

		void FLAG_PLAYER_CONTEXT_IN_TOURNAMENT(BOOL toggle) {
			Invoker::Invoke<void, 6542>(toggle);
		}

		void SET_PED_HAS_AI_BLIP(Ped ped, BOOL hasCone) {
			Invoker::Invoke<void, 6543>(ped, hasCone);
		}

		void SET_PED_HAS_AI_BLIP_WITH_COLOUR(Ped ped, BOOL hasCone, int color) {
			Invoker::Invoke<void, 6544>(ped, hasCone, color);
		}

		BOOL DOES_PED_HAVE_AI_BLIP(Ped ped) {
			return Invoker::Invoke<BOOL, 6545>(ped);
		}

		void SET_PED_AI_BLIP_GANG_ID(Ped ped, int gangId) {
			Invoker::Invoke<void, 6546>(ped, gangId);
		}

		void SET_PED_AI_BLIP_HAS_CONE(Ped ped, BOOL toggle) {
			Invoker::Invoke<void, 6547>(ped, toggle);
		}

		void SET_PED_AI_BLIP_FORCED_ON(Ped ped, BOOL toggle) {
			Invoker::Invoke<void, 6548>(ped, toggle);
		}

		void SET_PED_AI_BLIP_NOTICE_RANGE(Ped ped, float range) {
			Invoker::Invoke<void, 6549>(ped, range);
		}

		void SET_PED_AI_BLIP_SPRITE(Ped ped, int spriteId) {
			Invoker::Invoke<void, 6550>(ped, spriteId);
		}

		Blip GET_AI_PED_PED_BLIP_INDEX(Ped ped) {
			return Invoker::Invoke<Blip, 6551>(ped);
		}

		Blip GET_AI_PED_VEHICLE_BLIP_INDEX(Ped ped) {
			return Invoker::Invoke<Blip, 6552>(ped);
		}

		BOOL HAS_DIRECTOR_MODE_BEEN_LAUNCHED_BY_CODE() {
			return Invoker::Invoke<BOOL, 6553>();
		}

		void SET_DIRECTOR_MODE_LAUNCHED_BY_SCRIPT() {
			Invoker::Invoke<void, 6554>();
		}

		void SET_PLAYER_IS_IN_DIRECTOR_MODE(BOOL toggle) {
			Invoker::Invoke<void, 6555>(toggle);
		}

		void SET_DIRECTOR_MODE_AVAILABLE(BOOL toggle) {
			Invoker::Invoke<void, 6556>(toggle);
		}

		void HIDE_HUDMARKERS_THIS_FRAME() {
			Invoker::Invoke<void, 6557>();
		}

	} // namespace HUD

	namespace IIS {

		void SET_SHOULD_LAUNCH_IIS() {
			Invoker::Invoke<void, 6682>();
		}

	} // namespace IIS

	namespace INTERIOR {

		float GET_INTERIOR_HEADING(Interior interior) {
			return Invoker::Invoke<float, 1063>(interior);
		}

		void GET_INTERIOR_LOCATION_AND_NAMEHASH(Interior interior, Vector3* position, Hash* nameHash) {
			Invoker::Invoke<void, 1064>(interior, position, nameHash);
		}

		int GET_INTERIOR_GROUP_ID(Interior interior) {
			return Invoker::Invoke<int, 1065>(interior);
		}

		Vector3 GET_OFFSET_FROM_INTERIOR_IN_WORLD_COORDS(Interior interior, float x, float y, float z) {
			return Invoker::Invoke<Vector3, 1066>(interior, x, y, z);
		}

		BOOL IS_INTERIOR_SCENE() {
			return Invoker::Invoke<BOOL, 1067>();
		}

		BOOL IS_VALID_INTERIOR(Interior interior) {
			return Invoker::Invoke<BOOL, 1068>(interior);
		}

		void CLEAR_ROOM_FOR_ENTITY(Entity entity) {
			Invoker::Invoke<void, 1069>(entity);
		}

		void FORCE_ROOM_FOR_ENTITY(Entity entity, Interior interior, Hash roomHashKey) {
			Invoker::Invoke<void, 1070>(entity, interior, roomHashKey);
		}

		Hash GET_ROOM_KEY_FROM_ENTITY(Entity entity) {
			return Invoker::Invoke<Hash, 1071>(entity);
		}

		Hash GET_KEY_FOR_ENTITY_IN_ROOM(Entity entity) {
			return Invoker::Invoke<Hash, 1072>(entity);
		}

		Interior GET_INTERIOR_FROM_ENTITY(Entity entity) {
			return Invoker::Invoke<Interior, 1073>(entity);
		}

		void RETAIN_ENTITY_IN_INTERIOR(Entity entity, Interior interior) {
			Invoker::Invoke<void, 1074>(entity, interior);
		}

		void CLEAR_INTERIOR_STATE_OF_ENTITY(Entity entity) {
			Invoker::Invoke<void, 1075>(entity);
		}

		void FORCE_ACTIVATING_TRACKING_ON_ENTITY(Any p0, Any p1) {
			Invoker::Invoke<void, 1076>(p0, p1);
		}

		void FORCE_ROOM_FOR_GAME_VIEWPORT(int interiorID, Hash roomHashKey) {
			Invoker::Invoke<void, 1077>(interiorID, roomHashKey);
		}

		void SET_ROOM_FOR_GAME_VIEWPORT_BY_NAME(const char* roomName) {
			Invoker::Invoke<void, 1078>(roomName);
		}

		void SET_ROOM_FOR_GAME_VIEWPORT_BY_KEY(Hash roomHashKey) {
			Invoker::Invoke<void, 1079>(roomHashKey);
		}

		Hash GET_ROOM_KEY_FOR_GAME_VIEWPORT() {
			return Invoker::Invoke<Hash, 1080>();
		}

		void CLEAR_ROOM_FOR_GAME_VIEWPORT() {
			Invoker::Invoke<void, 1081>();
		}

		Interior GET_INTERIOR_FROM_PRIMARY_VIEW() {
			return Invoker::Invoke<Interior, 1082>();
		}

		Interior GET_INTERIOR_AT_COORDS(float x, float y, float z) {
			return Invoker::Invoke<Interior, 1083>(x, y, z);
		}

		void ADD_PICKUP_TO_INTERIOR_ROOM_BY_NAME(Pickup pickup, const char* roomName) {
			Invoker::Invoke<void, 1084>(pickup, roomName);
		}

		void PIN_INTERIOR_IN_MEMORY(Interior interior) {
			Invoker::Invoke<void, 1085>(interior);
		}

		void UNPIN_INTERIOR(Interior interior) {
			Invoker::Invoke<void, 1086>(interior);
		}

		BOOL IS_INTERIOR_READY(Interior interior) {
			return Invoker::Invoke<BOOL, 1087>(interior);
		}

		BOOL SET_INTERIOR_IN_USE(Interior interior) {
			return Invoker::Invoke<BOOL, 1088>(interior);
		}

		Interior GET_INTERIOR_AT_COORDS_WITH_TYPE(float x, float y, float z, const char* interiorType) {
			return Invoker::Invoke<Interior, 1089>(x, y, z, interiorType);
		}

		Interior GET_INTERIOR_AT_COORDS_WITH_TYPEHASH(float x, float y, float z, Hash typeHash) {
			return Invoker::Invoke<Interior, 1090>(x, y, z, typeHash);
		}

		void ACTIVATE_INTERIOR_GROUPS_USING_CAMERA() {
			Invoker::Invoke<void, 1091>();
		}

		BOOL IS_COLLISION_MARKED_OUTSIDE(float x, float y, float z) {
			return Invoker::Invoke<BOOL, 1092>(x, y, z);
		}

		Interior GET_INTERIOR_FROM_COLLISION(float x, float y, float z) {
			return Invoker::Invoke<Interior, 1093>(x, y, z);
		}

		void ENABLE_STADIUM_PROBES_THIS_FRAME(BOOL toggle) {
			Invoker::Invoke<void, 1094>(toggle);
		}

		void ACTIVATE_INTERIOR_ENTITY_SET(Interior interior, const char* entitySetName) {
			Invoker::Invoke<void, 1095>(interior, entitySetName);
		}

		void DEACTIVATE_INTERIOR_ENTITY_SET(Interior interior, const char* entitySetName) {
			Invoker::Invoke<void, 1096>(interior, entitySetName);
		}

		BOOL IS_INTERIOR_ENTITY_SET_ACTIVE(Interior interior, const char* entitySetName) {
			return Invoker::Invoke<BOOL, 1097>(interior, entitySetName);
		}

		void SET_INTERIOR_ENTITY_SET_TINT_INDEX(Interior interior, const char* entitySetName, int color) {
			Invoker::Invoke<void, 1098>(interior, entitySetName, color);
		}

		void REFRESH_INTERIOR(Interior interior) {
			Invoker::Invoke<void, 1099>(interior);
		}

		void ENABLE_EXTERIOR_CULL_MODEL_THIS_FRAME(Hash mapObjectHash) {
			Invoker::Invoke<void, 1100>(mapObjectHash);
		}

		void ENABLE_SHADOW_CULL_MODEL_THIS_FRAME(Hash mapObjectHash) {
			Invoker::Invoke<void, 1101>(mapObjectHash);
		}

		void DISABLE_INTERIOR(Interior interior, BOOL toggle) {
			Invoker::Invoke<void, 1102>(interior, toggle);
		}

		BOOL IS_INTERIOR_DISABLED(Interior interior) {
			return Invoker::Invoke<BOOL, 1103>(interior);
		}

		void CAP_INTERIOR(Interior interior, BOOL toggle) {
			Invoker::Invoke<void, 1104>(interior, toggle);
		}

		BOOL IS_INTERIOR_CAPPED(Interior interior) {
			return Invoker::Invoke<BOOL, 1105>(interior);
		}

		void DISABLE_METRO_SYSTEM(BOOL toggle) {
			Invoker::Invoke<void, 1106>(toggle);
		}

		void SET_IS_EXTERIOR_ONLY(Entity entity, BOOL toggle) {
			Invoker::Invoke<void, 1107>(entity, toggle);
		}

	} // namespace INTERIOR

	namespace ITEMSET {

		ScrHandle CREATE_ITEMSET(BOOL p0) {
			return Invoker::Invoke<ScrHandle, 3257>(p0);
		}

		void DESTROY_ITEMSET(ScrHandle itemset) {
			Invoker::Invoke<void, 3258>(itemset);
		}

		BOOL IS_ITEMSET_VALID(ScrHandle itemset) {
			return Invoker::Invoke<BOOL, 3259>(itemset);
		}

		BOOL ADD_TO_ITEMSET(ScrHandle item, ScrHandle itemset) {
			return Invoker::Invoke<BOOL, 3260>(item, itemset);
		}

		void REMOVE_FROM_ITEMSET(ScrHandle item, ScrHandle itemset) {
			Invoker::Invoke<void, 3261>(item, itemset);
		}

		int GET_ITEMSET_SIZE(ScrHandle itemset) {
			return Invoker::Invoke<int, 3262>(itemset);
		}

		ScrHandle GET_INDEXED_ITEM_IN_ITEMSET(int index, ScrHandle itemset) {
			return Invoker::Invoke<ScrHandle, 3263>(index, itemset);
		}

		BOOL IS_IN_ITEMSET(ScrHandle item, ScrHandle itemset) {
			return Invoker::Invoke<BOOL, 3264>(item, itemset);
		}

		void CLEAN_ITEMSET(ScrHandle itemset) {
			Invoker::Invoke<void, 3265>(itemset);
		}

	} // namespace ITEMSET

	namespace LANDINGPAGE {

		BOOL IS_LANDING_PAGE_ACTIVE() {
			return Invoker::Invoke<BOOL, 6681>();
		}

		void SET_SHOULD_LAUNCH_LANDING_PAGE(int entrypointId) {
			Invoker::Invoke<void, 6680>(entrypointId);
		}

		void SET_SHOULD_DISMISS_LANDING_PAGE() {
			Invoker::Invoke<void, 6679>();
		}

		int GET_LANDING_PAGE_SELECTED_CHARACTER_SLOT() {
			return Invoker::Invoke<int, 6678>();
		}

	} // namespace LANDINGPAGE

	namespace LOBBY {

		BOOL LOBBY_AUTO_MULTIPLAYER_MENU() {
			return Invoker::Invoke<BOOL, 4800>();
		}

		BOOL LOBBY_AUTO_MULTIPLAYER_FREEMODE() {
			return Invoker::Invoke<BOOL, 4801>();
		}

		void LOBBY_SET_AUTO_MULTIPLAYER(BOOL toggle) {
			Invoker::Invoke<void, 4802>(toggle);
		}

		BOOL LOBBY_AUTO_MULTIPLAYER_EVENT() {
			return Invoker::Invoke<BOOL, 4803>();
		}

		void LOBBY_SET_AUTO_MULTIPLAYER_EVENT(BOOL toggle) {
			Invoker::Invoke<void, 4804>(toggle);
		}

		BOOL LOBBY_AUTO_MULTIPLAYER_RANDOM_JOB() {
			return Invoker::Invoke<BOOL, 4805>();
		}

		void LOBBY_SET_AUTO_MP_RANDOM_JOB(BOOL toggle) {
			Invoker::Invoke<void, 4806>(toggle);
		}

		void SHUTDOWN_SESSION_CLEARS_AUTO_MULTIPLAYER(BOOL toggle) {
			Invoker::Invoke<void, 4807>(toggle);
		}

	} // namespace LOBBY

	namespace LOCALIZATION {

		int LOCALIZATION_GET_SYSTEM_LANGUAGE() {
			return Invoker::Invoke<int, 1555>();
		}

		int GET_CURRENT_LANGUAGE() {
			return Invoker::Invoke<int, 1556>();
		}

		int LOCALIZATION_GET_SYSTEM_DATE_TYPE() {
			return Invoker::Invoke<int, 1557>();
		}

	} // namespace LOCALIZATION

	namespace MISC {

		int GET_ALLOCATED_STACK_SIZE() {
			return Invoker::Invoke<int, 2360>();
		}

		int GET_NUMBER_OF_FREE_STACKS_OF_THIS_SIZE(int stackSize) {
			return Invoker::Invoke<int, 2361>(stackSize);
		}

		void SET_RANDOM_SEED(int seed) {
			Invoker::Invoke<void, 2362>(seed);
		}

		void SET_TIME_SCALE(float timeScale) {
			Invoker::Invoke<void, 2363>(timeScale);
		}

		void SET_MISSION_FLAG(BOOL toggle) {
			Invoker::Invoke<void, 2364>(toggle);
		}

		BOOL GET_MISSION_FLAG() {
			return Invoker::Invoke<BOOL, 2365>();
		}

		void SET_RANDOM_EVENT_FLAG(BOOL toggle) {
			Invoker::Invoke<void, 2366>(toggle);
		}

		BOOL GET_RANDOM_EVENT_FLAG() {
			return Invoker::Invoke<BOOL, 2367>();
		}

		const char* GET_CONTENT_TO_LOAD() {
			return Invoker::Invoke<const char*, 2368>();
		}

		void ACTIVITY_FEED_CREATE(const char* p0, const char* p1) {
			Invoker::Invoke<void, 2369>(p0, p1);
		}

		void ACTIVITY_FEED_ADD_SUBSTRING_TO_CAPTION(const char* p0) {
			Invoker::Invoke<void, 2370>(p0);
		}

		void ACTIVITY_FEED_ADD_LITERAL_SUBSTRING_TO_CAPTION(const char* p0) {
			Invoker::Invoke<void, 2371>(p0);
		}

		void ACTIVITY_FEED_ADD_INT_TO_CAPTION(Any p0) {
			Invoker::Invoke<void, 2372>(p0);
		}

		void ACTIVITY_FEED_LARGE_IMAGE_URL(const char* p0) {
			Invoker::Invoke<void, 2373>(p0);
		}

		void ACTIVITY_FEED_ACTION_START_WITH_COMMAND_LINE(const char* p0, const char* p1) {
			Invoker::Invoke<void, 2374>(p0, p1);
		}

		void ACTIVITY_FEED_ACTION_START_WITH_COMMAND_LINE_ADD(const char* p0) {
			Invoker::Invoke<void, 2375>(p0);
		}

		void ACTIVITY_FEED_POST() {
			Invoker::Invoke<void, 2376>();
		}

		void ACTIVITY_FEED_ONLINE_PLAYED_WITH_POST(const char* p0) {
			Invoker::Invoke<void, 2377>(p0);
		}

		BOOL HAS_RESUMED_FROM_SUSPEND() {
			return Invoker::Invoke<BOOL, 2378>();
		}

		void SET_SCRIPT_HIGH_PRIO(BOOL toggle) {
			Invoker::Invoke<void, 2379>(toggle);
		}

		void SET_THIS_IS_A_TRIGGER_SCRIPT(BOOL toggle) {
			Invoker::Invoke<void, 2380>(toggle);
		}

		void INFORM_CODE_OF_CONTENT_ID_OF_CURRENT_UGC_MISSION(const char* p0) {
			Invoker::Invoke<void, 2381>(p0);
		}

		BOOL GET_BASE_ELEMENT_LOCATION_FROM_METADATA_BLOCK(Any* p0, Any* p1, Any p2, BOOL p3) {
			return Invoker::Invoke<BOOL, 2382>(p0, p1, p2, p3);
		}

		Hash GET_PREV_WEATHER_TYPE_HASH_NAME() {
			return Invoker::Invoke<Hash, 2383>();
		}

		Hash GET_NEXT_WEATHER_TYPE_HASH_NAME() {
			return Invoker::Invoke<Hash, 2384>();
		}

		BOOL IS_PREV_WEATHER_TYPE(const char* weatherType) {
			return Invoker::Invoke<BOOL, 2385>(weatherType);
		}

		BOOL IS_NEXT_WEATHER_TYPE(const char* weatherType) {
			return Invoker::Invoke<BOOL, 2386>(weatherType);
		}

		void SET_WEATHER_TYPE_PERSIST(const char* weatherType) {
			Invoker::Invoke<void, 2387>(weatherType);
		}

		void SET_WEATHER_TYPE_NOW_PERSIST(const char* weatherType) {
			Invoker::Invoke<void, 2388>(weatherType);
		}

		void SET_WEATHER_TYPE_NOW(const char* weatherType) {
			Invoker::Invoke<void, 2389>(weatherType);
		}

		void SET_WEATHER_TYPE_OVERTIME_PERSIST(const char* weatherType, float time) {
			Invoker::Invoke<void, 2390>(weatherType, time);
		}

		void SET_RANDOM_WEATHER_TYPE() {
			Invoker::Invoke<void, 2391>();
		}

		void CLEAR_WEATHER_TYPE_PERSIST() {
			Invoker::Invoke<void, 2392>();
		}

		void CLEAR_WEATHER_TYPE_NOW_PERSIST_NETWORK(int milliseconds) {
			Invoker::Invoke<void, 2393>(milliseconds);
		}

		void GET_CURR_WEATHER_STATE(Hash* weatherType1, Hash* weatherType2, float* percentWeather2) {
			Invoker::Invoke<void, 2394>(weatherType1, weatherType2, percentWeather2);
		}

		void SET_CURR_WEATHER_STATE(Hash weatherType1, Hash weatherType2, float percentWeather2) {
			Invoker::Invoke<void, 2395>(weatherType1, weatherType2, percentWeather2);
		}

		void SET_OVERRIDE_WEATHER(const char* weatherType) {
			Invoker::Invoke<void, 2396>(weatherType);
		}

		void SET_OVERRIDE_WEATHEREX(const char* weatherType, BOOL p1) {
			Invoker::Invoke<void, 2397>(weatherType, p1);
		}

		void CLEAR_OVERRIDE_WEATHER() {
			Invoker::Invoke<void, 2398>();
		}

		void WATER_OVERRIDE_SET_SHOREWAVEAMPLITUDE(float amplitude) {
			Invoker::Invoke<void, 2399>(amplitude);
		}

		void WATER_OVERRIDE_SET_SHOREWAVEMINAMPLITUDE(float minAmplitude) {
			Invoker::Invoke<void, 2400>(minAmplitude);
		}

		void WATER_OVERRIDE_SET_SHOREWAVEMAXAMPLITUDE(float maxAmplitude) {
			Invoker::Invoke<void, 2401>(maxAmplitude);
		}

		void WATER_OVERRIDE_SET_OCEANNOISEMINAMPLITUDE(float minAmplitude) {
			Invoker::Invoke<void, 2402>(minAmplitude);
		}

		void WATER_OVERRIDE_SET_OCEANWAVEAMPLITUDE(float amplitude) {
			Invoker::Invoke<void, 2403>(amplitude);
		}

		void WATER_OVERRIDE_SET_OCEANWAVEMINAMPLITUDE(float minAmplitude) {
			Invoker::Invoke<void, 2404>(minAmplitude);
		}

		void WATER_OVERRIDE_SET_OCEANWAVEMAXAMPLITUDE(float maxAmplitude) {
			Invoker::Invoke<void, 2405>(maxAmplitude);
		}

		void WATER_OVERRIDE_SET_RIPPLEBUMPINESS(float bumpiness) {
			Invoker::Invoke<void, 2406>(bumpiness);
		}

		void WATER_OVERRIDE_SET_RIPPLEMINBUMPINESS(float minBumpiness) {
			Invoker::Invoke<void, 2407>(minBumpiness);
		}

		void WATER_OVERRIDE_SET_RIPPLEMAXBUMPINESS(float maxBumpiness) {
			Invoker::Invoke<void, 2408>(maxBumpiness);
		}

		void WATER_OVERRIDE_SET_RIPPLEDISTURB(float disturb) {
			Invoker::Invoke<void, 2409>(disturb);
		}

		void WATER_OVERRIDE_SET_STRENGTH(float strength) {
			Invoker::Invoke<void, 2410>(strength);
		}

		void WATER_OVERRIDE_FADE_IN(float p0) {
			Invoker::Invoke<void, 2411>(p0);
		}

		void WATER_OVERRIDE_FADE_OUT(float p0) {
			Invoker::Invoke<void, 2412>(p0);
		}

		void SET_WIND(float speed) {
			Invoker::Invoke<void, 2413>(speed);
		}

		void SET_WIND_SPEED(float speed) {
			Invoker::Invoke<void, 2414>(speed);
		}

		float GET_WIND_SPEED() {
			return Invoker::Invoke<float, 2415>();
		}

		void SET_WIND_DIRECTION(float direction) {
			Invoker::Invoke<void, 2416>(direction);
		}

		Vector3 GET_WIND_DIRECTION() {
			return Invoker::Invoke<Vector3, 2417>();
		}

		void SET_RAIN(float intensity) {
			Invoker::Invoke<void, 2418>(intensity);
		}

		float GET_RAIN_LEVEL() {
			return Invoker::Invoke<float, 2419>();
		}

		void SET_SNOW(float level) {
			Invoker::Invoke<void, 2420>(level);
		}

		float GET_SNOW_LEVEL() {
			return Invoker::Invoke<float, 2421>();
		}

		void FORCE_LIGHTNING_FLASH() {
			Invoker::Invoke<void, 2422>();
		}

		void SET_CLOUD_SETTINGS_OVERRIDE(const char* p0) {
			Invoker::Invoke<void, 2423>(p0);
		}

		void PRELOAD_CLOUD_HAT(const char* name) {
			Invoker::Invoke<void, 2424>(name);
		}

		void LOAD_CLOUD_HAT(const char* name, float transitionTime) {
			Invoker::Invoke<void, 2425>(name, transitionTime);
		}

		void UNLOAD_CLOUD_HAT(const char* name, float p1) {
			Invoker::Invoke<void, 2426>(name, p1);
		}

		void UNLOAD_ALL_CLOUD_HATS() {
			Invoker::Invoke<void, 2427>();
		}

		void SET_CLOUDS_ALPHA(float opacity) {
			Invoker::Invoke<void, 2428>(opacity);
		}

		float GET_CLOUDS_ALPHA() {
			return Invoker::Invoke<float, 2429>();
		}

		int GET_GAME_TIMER() {
			return Invoker::Invoke<int, 2430>();
		}

		float GET_FRAME_TIME() {
			return Invoker::Invoke<float, 2431>();
		}

		float GET_SYSTEM_TIME_STEP() {
			return Invoker::Invoke<float, 2432>();
		}

		int GET_FRAME_COUNT() {
			return Invoker::Invoke<int, 2433>();
		}

		float GET_RANDOM_FLOAT_IN_RANGE(float startRange, float endRange) {
			return Invoker::Invoke<float, 2434>(startRange, endRange);
		}

		int GET_RANDOM_INT_IN_RANGE(int startRange, int endRange) {
			return Invoker::Invoke<int, 2435>(startRange, endRange);
		}

		int GET_RANDOM_MWC_INT_IN_RANGE(int startRange, int endRange) {
			return Invoker::Invoke<int, 2436>(startRange, endRange);
		}

		BOOL GET_GROUND_Z_FOR_3D_COORD(float x, float y, float z, float* groundZ, BOOL ignoreWater, BOOL p5) {
			return Invoker::Invoke<BOOL, 2437>(x, y, z, groundZ, ignoreWater, p5);
		}

		BOOL GET_GROUND_Z_AND_NORMAL_FOR_3D_COORD(float x, float y, float z, float* groundZ, Vector3* normal) {
			return Invoker::Invoke<BOOL, 2438>(x, y, z, groundZ, normal);
		}

		BOOL GET_GROUND_Z_EXCLUDING_OBJECTS_FOR_3D_COORD(float x, float y, float z, float* groundZ, BOOL p4, BOOL p5) {
			return Invoker::Invoke<BOOL, 2439>(x, y, z, groundZ, p4, p5);
		}

		float ASIN(float p0) {
			return Invoker::Invoke<float, 2440>(p0);
		}

		float ACOS(float p0) {
			return Invoker::Invoke<float, 2441>(p0);
		}

		float TAN(float p0) {
			return Invoker::Invoke<float, 2442>(p0);
		}

		float ATAN(float p0) {
			return Invoker::Invoke<float, 2443>(p0);
		}

		float ATAN2(float p0, float p1) {
			return Invoker::Invoke<float, 2444>(p0, p1);
		}

		float GET_DISTANCE_BETWEEN_COORDS(float x1, float y1, float z1, float x2, float y2, float z2, BOOL useZ) {
			return Invoker::Invoke<float, 2445>(x1, y1, z1, x2, y2, z2, useZ);
		}

		float GET_ANGLE_BETWEEN_2D_VECTORS(float x1, float y1, float x2, float y2) {
			return Invoker::Invoke<float, 2446>(x1, y1, x2, y2);
		}

		float GET_HEADING_FROM_VECTOR_2D(float dx, float dy) {
			return Invoker::Invoke<float, 2447>(dx, dy);
		}

		float GET_RATIO_OF_CLOSEST_POINT_ON_LINE(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3, BOOL clamp) {
			return Invoker::Invoke<float, 2448>(x1, y1, z1, x2, y2, z2, x3, y3, z3, clamp);
		}

		Vector3 GET_CLOSEST_POINT_ON_LINE(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3, BOOL clamp) {
			return Invoker::Invoke<Vector3, 2449>(x1, y1, z1, x2, y2, z2, x3, y3, z3, clamp);
		}

		BOOL GET_LINE_PLANE_INTERSECTION(float p0, float p1, float p2, float p3, float p4, float p5, float p6, float p7, float p8, float p9, float p10, float p11, float* p12) {
			return Invoker::Invoke<BOOL, 2450>(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12);
		}

		BOOL GET_POINT_AREA_OVERLAP(Any p0, Any p1, Any p2, Any p3, Any p4, Any p5, Any p6, Any p7, Any p8, Any p9, Any p10, Any p11, Any p12, Any p13) {
			return Invoker::Invoke<BOOL, 2451>(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13);
		}

		void SET_BIT(int* address, int offset) {
			Invoker::Invoke<void, 2452>(address, offset);
		}

		void CLEAR_BIT(int* address, int offset) {
			Invoker::Invoke<void, 2453>(address, offset);
		}

		Hash GET_HASH_KEY(const char* string) {
			return Invoker::Invoke<Hash, 2454>(string);
		}

		void SLERP_NEAR_QUATERNION(float t, float x, float y, float z, float w, float x1, float y1, float z1, float w1, float* outX, float* outY, float* outZ, float* outW) {
			Invoker::Invoke<void, 2455>(t, x, y, z, w, x1, y1, z1, w1, outX, outY, outZ, outW);
		}

		BOOL IS_AREA_OCCUPIED(float p0, float p1, float p2, float p3, float p4, float p5, BOOL p6, BOOL p7, BOOL p8, BOOL p9, BOOL p10, Any p11, BOOL p12) {
			return Invoker::Invoke<BOOL, 2456>(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12);
		}

		BOOL IS_AREA_OCCUPIED_SLOW(Any p0, Any p1, Any p2, Any p3, Any p4, Any p5, Any p6, Any p7, Any p8, Any p9, Any p10, Any p11, Any p12) {
			return Invoker::Invoke<BOOL, 2457>(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12);
		}

		BOOL IS_POSITION_OCCUPIED(float x, float y, float z, float range, BOOL p4, BOOL checkVehicles, BOOL checkPeds, BOOL p7, BOOL p8, Entity ignoreEntity, BOOL p10) {
			return Invoker::Invoke<BOOL, 2458>(x, y, z, range, p4, checkVehicles, checkPeds, p7, p8, ignoreEntity, p10);
		}

		BOOL IS_POINT_OBSCURED_BY_A_MISSION_ENTITY(float p0, float p1, float p2, float p3, float p4, float p5, Any p6) {
			return Invoker::Invoke<BOOL, 2459>(p0, p1, p2, p3, p4, p5, p6);
		}

		void CLEAR_AREA(float X, float Y, float Z, float radius, BOOL p4, BOOL ignoreCopCars, BOOL ignoreObjects, BOOL p7) {
			Invoker::Invoke<void, 2460>(X, Y, Z, radius, p4, ignoreCopCars, ignoreObjects, p7);
		}

		void CLEAR_AREA_LEAVE_VEHICLE_HEALTH(float x, float y, float z, float radius, BOOL p4, BOOL p5, BOOL p6, BOOL p7) {
			Invoker::Invoke<void, 2461>(x, y, z, radius, p4, p5, p6, p7);
		}

		void CLEAR_AREA_OF_VEHICLES(float x, float y, float z, float radius, BOOL p4, BOOL p5, BOOL p6, BOOL p7, BOOL p8, BOOL p9, Any p10) {
			Invoker::Invoke<void, 2462>(x, y, z, radius, p4, p5, p6, p7, p8, p9, p10);
		}

		void CLEAR_ANGLED_AREA_OF_VEHICLES(float x1, float y1, float z1, float x2, float y2, float z2, float width, BOOL p7, BOOL p8, BOOL p9, BOOL p10, BOOL p11, Any p12, Any p13) {
			Invoker::Invoke<void, 2463>(x1, y1, z1, x2, y2, z2, width, p7, p8, p9, p10, p11, p12, p13);
		}

		void CLEAR_AREA_OF_OBJECTS(float x, float y, float z, float radius, int flags) {
			Invoker::Invoke<void, 2464>(x, y, z, radius, flags);
		}

		void CLEAR_AREA_OF_PEDS(float x, float y, float z, float radius, int flags) {
			Invoker::Invoke<void, 2465>(x, y, z, radius, flags);
		}

		void CLEAR_AREA_OF_COPS(float x, float y, float z, float radius, int flags) {
			Invoker::Invoke<void, 2466>(x, y, z, radius, flags);
		}

		void CLEAR_AREA_OF_PROJECTILES(float x, float y, float z, float radius, int flags) {
			Invoker::Invoke<void, 2467>(x, y, z, radius, flags);
		}

		void CLEAR_SCENARIO_SPAWN_HISTORY() {
			Invoker::Invoke<void, 2468>();
		}

		void SET_SAVE_MENU_ACTIVE(BOOL ignoreVehicle) {
			Invoker::Invoke<void, 2469>(ignoreVehicle);
		}

		int GET_STATUS_OF_MANUAL_SAVE() {
			return Invoker::Invoke<int, 2470>();
		}

		void SET_CREDITS_ACTIVE(BOOL toggle) {
			Invoker::Invoke<void, 2471>(toggle);
		}

		void SET_CREDITS_FADE_OUT_WITH_SCREEN(BOOL toggle) {
			Invoker::Invoke<void, 2472>(toggle);
		}

		BOOL HAVE_CREDITS_REACHED_END() {
			return Invoker::Invoke<BOOL, 2473>();
		}

		BOOL ARE_CREDITS_RUNNING() {
			return Invoker::Invoke<BOOL, 2474>();
		}

		void TERMINATE_ALL_SCRIPTS_WITH_THIS_NAME(const char* scriptName) {
			Invoker::Invoke<void, 2475>(scriptName);
		}

		void NETWORK_SET_SCRIPT_IS_SAFE_FOR_NETWORK_GAME() {
			Invoker::Invoke<void, 2476>();
		}

		int ADD_HOSPITAL_RESTART(float x, float y, float z, float p3, Any p4) {
			return Invoker::Invoke<int, 2477>(x, y, z, p3, p4);
		}

		void DISABLE_HOSPITAL_RESTART(int hospitalIndex, BOOL toggle) {
			Invoker::Invoke<void, 2478>(hospitalIndex, toggle);
		}

		int ADD_POLICE_RESTART(float p0, float p1, float p2, float p3, Any p4) {
			return Invoker::Invoke<int, 2479>(p0, p1, p2, p3, p4);
		}

		void DISABLE_POLICE_RESTART(int policeIndex, BOOL toggle) {
			Invoker::Invoke<void, 2480>(policeIndex, toggle);
		}

		void SET_RESTART_COORD_OVERRIDE(float x, float y, float z, float heading) {
			Invoker::Invoke<void, 2481>(x, y, z, heading);
		}

		void CLEAR_RESTART_COORD_OVERRIDE() {
			Invoker::Invoke<void, 2482>();
		}

		void PAUSE_DEATH_ARREST_RESTART(BOOL toggle) {
			Invoker::Invoke<void, 2483>(toggle);
		}

		void IGNORE_NEXT_RESTART(BOOL toggle) {
			Invoker::Invoke<void, 2484>(toggle);
		}

		void SET_FADE_OUT_AFTER_DEATH(BOOL toggle) {
			Invoker::Invoke<void, 2485>(toggle);
		}

		void SET_FADE_OUT_AFTER_ARREST(BOOL toggle) {
			Invoker::Invoke<void, 2486>(toggle);
		}

		void SET_FADE_IN_AFTER_DEATH_ARREST(BOOL toggle) {
			Invoker::Invoke<void, 2487>(toggle);
		}

		void SET_FADE_IN_AFTER_LOAD(BOOL toggle) {
			Invoker::Invoke<void, 2488>(toggle);
		}

		int REGISTER_SAVE_HOUSE(float x, float y, float z, float p3, const char* p4, Any p5, Any p6) {
			return Invoker::Invoke<int, 2489>(x, y, z, p3, p4, p5, p6);
		}

		void SET_SAVE_HOUSE(int savehouseHandle, BOOL p1, BOOL p2) {
			Invoker::Invoke<void, 2490>(savehouseHandle, p1, p2);
		}

		BOOL OVERRIDE_SAVE_HOUSE(BOOL p0, float p1, float p2, float p3, float p4, BOOL p5, float p6, float p7) {
			return Invoker::Invoke<BOOL, 2491>(p0, p1, p2, p3, p4, p5, p6, p7);
		}

		BOOL GET_SAVE_HOUSE_DETAILS_AFTER_SUCCESSFUL_LOAD(Vector3* p0, float* p1, BOOL* fadeInAfterLoad, BOOL* p3) {
			return Invoker::Invoke<BOOL, 2492>(p0, p1, fadeInAfterLoad, p3);
		}

		void DO_AUTO_SAVE() {
			Invoker::Invoke<void, 2493>();
		}

		BOOL GET_IS_AUTO_SAVE_OFF() {
			return Invoker::Invoke<BOOL, 2494>();
		}

		BOOL IS_AUTO_SAVE_IN_PROGRESS() {
			return Invoker::Invoke<BOOL, 2495>();
		}

		BOOL HAS_CODE_REQUESTED_AUTOSAVE() {
			return Invoker::Invoke<BOOL, 2496>();
		}

		void CLEAR_CODE_REQUESTED_AUTOSAVE() {
			Invoker::Invoke<void, 2497>();
		}

		void BEGIN_REPLAY_STATS(Any p0, Any p1) {
			Invoker::Invoke<void, 2498>(p0, p1);
		}

		void ADD_REPLAY_STAT_VALUE(Any value) {
			Invoker::Invoke<void, 2499>(value);
		}

		void END_REPLAY_STATS() {
			Invoker::Invoke<void, 2500>();
		}

		BOOL HAVE_REPLAY_STATS_BEEN_STORED() {
			return Invoker::Invoke<BOOL, 2501>();
		}

		int GET_REPLAY_STAT_MISSION_ID() {
			return Invoker::Invoke<int, 2502>();
		}

		int GET_REPLAY_STAT_MISSION_TYPE() {
			return Invoker::Invoke<int, 2503>();
		}

		int GET_REPLAY_STAT_COUNT() {
			return Invoker::Invoke<int, 2504>();
		}

		int GET_REPLAY_STAT_AT_INDEX(int index) {
			return Invoker::Invoke<int, 2505>(index);
		}

		void CLEAR_REPLAY_STATS() {
			Invoker::Invoke<void, 2506>();
		}

		BOOL QUEUE_MISSION_REPEAT_LOAD() {
			return Invoker::Invoke<BOOL, 2507>();
		}

		BOOL QUEUE_MISSION_REPEAT_SAVE() {
			return Invoker::Invoke<BOOL, 2508>();
		}

		BOOL QUEUE_MISSION_REPEAT_SAVE_FOR_BENCHMARK_TEST() {
			return Invoker::Invoke<BOOL, 2509>();
		}

		int GET_STATUS_OF_MISSION_REPEAT_SAVE() {
			return Invoker::Invoke<int, 2510>();
		}

		BOOL IS_MEMORY_CARD_IN_USE() {
			return Invoker::Invoke<BOOL, 2511>();
		}

		void SHOOT_SINGLE_BULLET_BETWEEN_COORDS(float x1, float y1, float z1, float x2, float y2, float z2, int damage, BOOL p7, Hash weaponHash, Ped ownerPed, BOOL isAudible, BOOL isInvisible, float speed) {
			Invoker::Invoke<void, 2512>(x1, y1, z1, x2, y2, z2, damage, p7, weaponHash, ownerPed, isAudible, isInvisible, speed);
		}

		void SHOOT_SINGLE_BULLET_BETWEEN_COORDS_IGNORE_ENTITY(float x1, float y1, float z1, float x2, float y2, float z2, int damage, BOOL p7, Hash weaponHash, Ped ownerPed, BOOL isAudible, BOOL isInvisible, float speed, Entity entity, Any p14) {
			Invoker::Invoke<void, 2513>(x1, y1, z1, x2, y2, z2, damage, p7, weaponHash, ownerPed, isAudible, isInvisible, speed, entity, p14);
		}

		void SHOOT_SINGLE_BULLET_BETWEEN_COORDS_IGNORE_ENTITY_NEW(float x1, float y1, float z1, float x2, float y2, float z2, int damage, BOOL p7, Hash weaponHash, Ped ownerPed, BOOL isAudible, BOOL isInvisible, float speed, Entity entity, BOOL p14, BOOL p15, Entity targetEntity, BOOL p17, Any p18, Any p19, Any p20) {
			Invoker::Invoke<void, 2514>(x1, y1, z1, x2, y2, z2, damage, p7, weaponHash, ownerPed, isAudible, isInvisible, speed, entity, p14, p15, targetEntity, p17, p18, p19, p20);
		}

		void GET_MODEL_DIMENSIONS(Hash modelHash, Vector3* minimum, Vector3* maximum) {
			Invoker::Invoke<void, 2515>(modelHash, minimum, maximum);
		}

		void SET_FAKE_WANTED_LEVEL(int fakeWantedLevel) {
			Invoker::Invoke<void, 2516>(fakeWantedLevel);
		}

		int GET_FAKE_WANTED_LEVEL() {
			return Invoker::Invoke<int, 2517>();
		}

		void USING_MISSION_CREATOR(BOOL toggle) {
			Invoker::Invoke<void, 2518>(toggle);
		}

		void ALLOW_MISSION_CREATOR_WARP(BOOL toggle) {
			Invoker::Invoke<void, 2519>(toggle);
		}

		void SET_MINIGAME_IN_PROGRESS(BOOL toggle) {
			Invoker::Invoke<void, 2520>(toggle);
		}

		BOOL IS_MINIGAME_IN_PROGRESS() {
			return Invoker::Invoke<BOOL, 2521>();
		}

		BOOL IS_THIS_A_MINIGAME_SCRIPT() {
			return Invoker::Invoke<BOOL, 2522>();
		}

		BOOL IS_SNIPER_INVERTED() {
			return Invoker::Invoke<BOOL, 2523>();
		}

		BOOL SHOULD_USE_METRIC_MEASUREMENTS() {
			return Invoker::Invoke<BOOL, 2524>();
		}

		int GET_PROFILE_SETTING(int profileSetting) {
			return Invoker::Invoke<int, 2525>(profileSetting);
		}

		BOOL ARE_STRINGS_EQUAL(const char* string1, const char* string2) {
			return Invoker::Invoke<BOOL, 2526>(string1, string2);
		}

		int COMPARE_STRINGS(const char* str1, const char* str2, BOOL matchCase, int maxLength) {
			return Invoker::Invoke<int, 2527>(str1, str2, matchCase, maxLength);
		}

		int ABSI(int value) {
			return Invoker::Invoke<int, 2528>(value);
		}

		float ABSF(float value) {
			return Invoker::Invoke<float, 2529>(value);
		}

		BOOL IS_SNIPER_BULLET_IN_AREA(float x1, float y1, float z1, float x2, float y2, float z2) {
			return Invoker::Invoke<BOOL, 2530>(x1, y1, z1, x2, y2, z2);
		}

		BOOL IS_PROJECTILE_IN_AREA(float x1, float y1, float z1, float x2, float y2, float z2, BOOL ownedByPlayer) {
			return Invoker::Invoke<BOOL, 2531>(x1, y1, z1, x2, y2, z2, ownedByPlayer);
		}

		BOOL IS_PROJECTILE_TYPE_IN_AREA(float x1, float y1, float z1, float x2, float y2, float z2, int type, BOOL ownedByPlayer) {
			return Invoker::Invoke<BOOL, 2532>(x1, y1, z1, x2, y2, z2, type, ownedByPlayer);
		}

		BOOL IS_PROJECTILE_TYPE_IN_ANGLED_AREA(float x1, float y1, float z1, float x2, float y2, float z2, float width, Any p7, BOOL ownedByPlayer) {
			return Invoker::Invoke<BOOL, 2533>(x1, y1, z1, x2, y2, z2, width, p7, ownedByPlayer);
		}

		BOOL IS_PROJECTILE_TYPE_WITHIN_DISTANCE(float x, float y, float z, Hash projectileHash, float radius, BOOL ownedByPlayer) {
			return Invoker::Invoke<BOOL, 2534>(x, y, z, projectileHash, radius, ownedByPlayer);
		}

		BOOL GET_COORDS_OF_PROJECTILE_TYPE_IN_AREA(float x1, float y1, float z1, float x2, float y2, float z2, Hash projectileHash, Vector3* projectilePos, BOOL ownedByPlayer) {
			return Invoker::Invoke<BOOL, 2535>(x1, y1, z1, x2, y2, z2, projectileHash, projectilePos, ownedByPlayer);
		}

		BOOL GET_COORDS_OF_PROJECTILE_TYPE_IN_ANGLED_AREA(float vecAngledAreaPoint1X, float vecAngledAreaPoint1Y, float vecAngledAreaPoint1Z, float vecAngledAreaPoint2X, float vecAngledAreaPoint2Y, float vecAngledAreaPoint2Z, float distanceOfOppositeFace, Hash weaponType, Vector3* positionOut, BOOL bIsPlayer) {
			return Invoker::Invoke<BOOL, 2536>(vecAngledAreaPoint1X, vecAngledAreaPoint1Y, vecAngledAreaPoint1Z, vecAngledAreaPoint2X, vecAngledAreaPoint2Y, vecAngledAreaPoint2Z, distanceOfOppositeFace, weaponType, positionOut, bIsPlayer);
		}

		BOOL GET_COORDS_OF_PROJECTILE_TYPE_WITHIN_DISTANCE(Ped ped, Hash weaponHash, float distance, Vector3* outCoords, BOOL p4) {
			return Invoker::Invoke<BOOL, 2537>(ped, weaponHash, distance, outCoords, p4);
		}

		BOOL GET_PROJECTILE_OF_PROJECTILE_TYPE_WITHIN_DISTANCE(Ped ped, Hash weaponHash, float distance, Vector3* outCoords, Object* outProjectile, BOOL p5) {
			return Invoker::Invoke<BOOL, 2538>(ped, weaponHash, distance, outCoords, outProjectile, p5);
		}

		BOOL IS_BULLET_IN_ANGLED_AREA(float x1, float y1, float z1, float x2, float y2, float z2, float width, BOOL ownedByPlayer) {
			return Invoker::Invoke<BOOL, 2539>(x1, y1, z1, x2, y2, z2, width, ownedByPlayer);
		}

		BOOL IS_BULLET_IN_AREA(float x, float y, float z, float radius, BOOL ownedByPlayer) {
			return Invoker::Invoke<BOOL, 2540>(x, y, z, radius, ownedByPlayer);
		}

		BOOL IS_BULLET_IN_BOX(float x1, float y1, float z1, float x2, float y2, float z2, BOOL ownedByPlayer) {
			return Invoker::Invoke<BOOL, 2541>(x1, y1, z1, x2, y2, z2, ownedByPlayer);
		}

		BOOL HAS_BULLET_IMPACTED_IN_AREA(float x, float y, float z, float p3, BOOL p4, BOOL p5) {
			return Invoker::Invoke<BOOL, 2542>(x, y, z, p3, p4, p5);
		}

		BOOL HAS_BULLET_IMPACTED_IN_BOX(float p0, float p1, float p2, float p3, float p4, float p5, BOOL p6, BOOL p7) {
			return Invoker::Invoke<BOOL, 2543>(p0, p1, p2, p3, p4, p5, p6, p7);
		}

		BOOL IS_ORBIS_VERSION() {
			return Invoker::Invoke<BOOL, 2544>();
		}

		BOOL IS_DURANGO_VERSION() {
			return Invoker::Invoke<BOOL, 2545>();
		}

		BOOL IS_XBOX360_VERSION() {
			return Invoker::Invoke<BOOL, 2546>();
		}

		BOOL IS_PS3_VERSION() {
			return Invoker::Invoke<BOOL, 2547>();
		}

		BOOL IS_PC_VERSION() {
			return Invoker::Invoke<BOOL, 2548>();
		}

		BOOL IS_STEAM_VERSION() {
			return Invoker::Invoke<BOOL, 2549>();
		}

		BOOL _IS_XBOXPC_VERSION() {
			return Invoker::Invoke<BOOL, 6677>();
		}

		BOOL IS_AUSSIE_VERSION() {
			return Invoker::Invoke<BOOL, 2550>();
		}

		BOOL IS_JAPANESE_VERSION() {
			return Invoker::Invoke<BOOL, 2551>();
		}

		BOOL IS_XBOX_PLATFORM() {
			return Invoker::Invoke<BOOL, 2552>();
		}

		BOOL IS_SCARLETT_VERSION() {
			return Invoker::Invoke<BOOL, 2553>();
		}

		BOOL IS_SCE_PLATFORM() {
			return Invoker::Invoke<BOOL, 2554>();
		}

		BOOL IS_PROSPERO_VERSION() {
			return Invoker::Invoke<BOOL, 2555>();
		}

		BOOL IS_STRING_NULL(const char* string) {
			return Invoker::Invoke<BOOL, 2556>(string);
		}

		BOOL IS_STRING_NULL_OR_EMPTY(const char* string) {
			return Invoker::Invoke<BOOL, 2557>(string);
		}

		BOOL STRING_TO_INT(const char* string, int* outInteger) {
			return Invoker::Invoke<BOOL, 2558>(string, outInteger);
		}

		void SET_BITS_IN_RANGE(int* var, int rangeStart, int rangeEnd, int p3) {
			Invoker::Invoke<void, 2559>(var, rangeStart, rangeEnd, p3);
		}

		int GET_BITS_IN_RANGE(int var, int rangeStart, int rangeEnd) {
			return Invoker::Invoke<int, 2560>(var, rangeStart, rangeEnd);
		}

		int ADD_STUNT_JUMP(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3, float x4, float y4, float z4, float camX, float camY, float camZ, int p15, int p16, int p17) {
			return Invoker::Invoke<int, 2561>(x1, y1, z1, x2, y2, z2, x3, y3, z3, x4, y4, z4, camX, camY, camZ, p15, p16, p17);
		}

		int ADD_STUNT_JUMP_ANGLED(float x1, float y1, float z1, float x2, float y2, float z2, float radius1, float x3, float y3, float z3, float x4, float y4, float z4, float radius2, float camX, float camY, float camZ, int p17, int p18, int p19) {
			return Invoker::Invoke<int, 2562>(x1, y1, z1, x2, y2, z2, radius1, x3, y3, z3, x4, y4, z4, radius2, camX, camY, camZ, p17, p18, p19);
		}

		void TOGGLE_SHOW_OPTIONAL_STUNT_JUMP_CAMERA(BOOL toggle) {
			Invoker::Invoke<void, 2563>(toggle);
		}

		void DELETE_STUNT_JUMP(int p0) {
			Invoker::Invoke<void, 2564>(p0);
		}

		void ENABLE_STUNT_JUMP_SET(int p0) {
			Invoker::Invoke<void, 2565>(p0);
		}

		void DISABLE_STUNT_JUMP_SET(int p0) {
			Invoker::Invoke<void, 2566>(p0);
		}

		void SET_STUNT_JUMPS_CAN_TRIGGER(BOOL toggle) {
			Invoker::Invoke<void, 2567>(toggle);
		}

		BOOL IS_STUNT_JUMP_IN_PROGRESS() {
			return Invoker::Invoke<BOOL, 2568>();
		}

		BOOL IS_STUNT_JUMP_MESSAGE_SHOWING() {
			return Invoker::Invoke<BOOL, 2569>();
		}

		int GET_NUM_SUCCESSFUL_STUNT_JUMPS() {
			return Invoker::Invoke<int, 2570>();
		}

		int GET_TOTAL_SUCCESSFUL_STUNT_JUMPS() {
			return Invoker::Invoke<int, 2571>();
		}

		void CANCEL_STUNT_JUMP() {
			Invoker::Invoke<void, 2572>();
		}

		void SET_GAME_PAUSED(BOOL toggle) {
			Invoker::Invoke<void, 2573>(toggle);
		}

		void SET_THIS_SCRIPT_CAN_BE_PAUSED(BOOL toggle) {
			Invoker::Invoke<void, 2574>(toggle);
		}

		void SET_THIS_SCRIPT_CAN_REMOVE_BLIPS_CREATED_BY_ANY_SCRIPT(BOOL toggle) {
			Invoker::Invoke<void, 2575>(toggle);
		}

		BOOL HAS_CHEAT_WITH_HASH_BEEN_ACTIVATED(Hash hash, int amount) {
			return Invoker::Invoke<BOOL, 2576>(hash, amount);
		}

		BOOL HAS_PC_CHEAT_WITH_HASH_BEEN_ACTIVATED(Hash hash) {
			return Invoker::Invoke<BOOL, 2577>(hash);
		}

		void OVERRIDE_FREEZE_FLAGS(BOOL p0) {
			Invoker::Invoke<void, 2578>(p0);
		}

		void SET_INSTANCE_PRIORITY_MODE(int p0) {
			Invoker::Invoke<void, 2579>(p0);
		}

		void SET_INSTANCE_PRIORITY_HINT(int flag) {
			Invoker::Invoke<void, 2580>(flag);
		}

		BOOL IS_FRONTEND_FADING() {
			return Invoker::Invoke<BOOL, 2581>();
		}

		void POPULATE_NOW() {
			Invoker::Invoke<void, 2582>();
		}

		int GET_INDEX_OF_CURRENT_LEVEL() {
			return Invoker::Invoke<int, 2583>();
		}

		void SET_GRAVITY_LEVEL(int level) {
			Invoker::Invoke<void, 2584>(level);
		}

		void START_SAVE_DATA(Any* p0, Any p1, BOOL p2) {
			Invoker::Invoke<void, 2585>(p0, p1, p2);
		}

		void STOP_SAVE_DATA() {
			Invoker::Invoke<void, 2586>();
		}

		int GET_SIZE_OF_SAVE_DATA(BOOL p0) {
			return Invoker::Invoke<int, 2587>(p0);
		}

		void REGISTER_INT_TO_SAVE(Any* p0, const char* name) {
			Invoker::Invoke<void, 2588>(p0, name);
		}

		void REGISTER_INT64_TO_SAVE(Any* p0, const char* name) {
			Invoker::Invoke<void, 2589>(p0, name);
		}

		void REGISTER_ENUM_TO_SAVE(Any* p0, const char* name) {
			Invoker::Invoke<void, 2590>(p0, name);
		}

		void REGISTER_FLOAT_TO_SAVE(Any* p0, const char* name) {
			Invoker::Invoke<void, 2591>(p0, name);
		}

		void REGISTER_BOOL_TO_SAVE(Any* p0, const char* name) {
			Invoker::Invoke<void, 2592>(p0, name);
		}

		void REGISTER_TEXT_LABEL_TO_SAVE(Any* p0, const char* name) {
			Invoker::Invoke<void, 2593>(p0, name);
		}

		void REGISTER_TEXT_LABEL_15_TO_SAVE(Any* p0, const char* name) {
			Invoker::Invoke<void, 2594>(p0, name);
		}

		void REGISTER_TEXT_LABEL_23_TO_SAVE(Any* p0, const char* name) {
			Invoker::Invoke<void, 2595>(p0, name);
		}

		void REGISTER_TEXT_LABEL_31_TO_SAVE(Any* p0, const char* name) {
			Invoker::Invoke<void, 2596>(p0, name);
		}

		void REGISTER_TEXT_LABEL_63_TO_SAVE(Any* p0, const char* name) {
			Invoker::Invoke<void, 2597>(p0, name);
		}

		void START_SAVE_STRUCT_WITH_SIZE(Any* p0, int size, const char* structName) {
			Invoker::Invoke<void, 2598>(p0, size, structName);
		}

		void STOP_SAVE_STRUCT() {
			Invoker::Invoke<void, 2599>();
		}

		void START_SAVE_ARRAY_WITH_SIZE(Any* p0, int size, const char* arrayName) {
			Invoker::Invoke<void, 2600>(p0, size, arrayName);
		}

		void STOP_SAVE_ARRAY() {
			Invoker::Invoke<void, 2601>();
		}

		void COPY_SCRIPT_STRUCT(Any* dst, Any* src, int size) {
			Invoker::Invoke<void, 2602>(dst, src, size);
		}

		void ENABLE_DISPATCH_SERVICE(int dispatchService, BOOL toggle) {
			Invoker::Invoke<void, 2603>(dispatchService, toggle);
		}

		void BLOCK_DISPATCH_SERVICE_RESOURCE_CREATION(int dispatchService, BOOL toggle) {
			Invoker::Invoke<void, 2604>(dispatchService, toggle);
		}

		int GET_NUMBER_RESOURCES_ALLOCATED_TO_WANTED_LEVEL(int dispatchService) {
			return Invoker::Invoke<int, 2605>(dispatchService);
		}

		BOOL CREATE_INCIDENT(int dispatchService, float x, float y, float z, int numUnits, float radius, int* outIncidentID, Any p7, Any p8) {
			return Invoker::Invoke<BOOL, 2606>(dispatchService, x, y, z, numUnits, radius, outIncidentID, p7, p8);
		}

		BOOL CREATE_INCIDENT_WITH_ENTITY(int dispatchService, Ped ped, int numUnits, float radius, int* outIncidentID, Any p5, Any p6) {
			return Invoker::Invoke<BOOL, 2607>(dispatchService, ped, numUnits, radius, outIncidentID, p5, p6);
		}

		void DELETE_INCIDENT(int incidentId) {
			Invoker::Invoke<void, 2608>(incidentId);
		}

		BOOL IS_INCIDENT_VALID(int incidentId) {
			return Invoker::Invoke<BOOL, 2609>(incidentId);
		}

		void SET_INCIDENT_REQUESTED_UNITS(int incidentId, int dispatchService, int numUnits) {
			Invoker::Invoke<void, 2610>(incidentId, dispatchService, numUnits);
		}

		void SET_IDEAL_SPAWN_DISTANCE_FOR_INCIDENT(int incidentId, float p1) {
			Invoker::Invoke<void, 2611>(incidentId, p1);
		}

		BOOL FIND_SPAWN_POINT_IN_DIRECTION(float posX, float posY, float posZ, float fwdVecX, float fwdVecY, float fwdVecZ, float distance, Vector3* spawnPoint) {
			return Invoker::Invoke<BOOL, 2612>(posX, posY, posZ, fwdVecX, fwdVecY, fwdVecZ, distance, spawnPoint);
		}

		int ADD_POP_MULTIPLIER_AREA(float x1, float y1, float z1, float x2, float y2, float z2, float p6, float p7, BOOL p8, BOOL p9) {
			return Invoker::Invoke<int, 2613>(x1, y1, z1, x2, y2, z2, p6, p7, p8, p9);
		}

		BOOL DOES_POP_MULTIPLIER_AREA_EXIST(int id) {
			return Invoker::Invoke<BOOL, 2614>(id);
		}

		void REMOVE_POP_MULTIPLIER_AREA(int id, BOOL p1) {
			Invoker::Invoke<void, 2615>(id, p1);
		}

		BOOL IS_POP_MULTIPLIER_AREA_NETWORKED(int id) {
			return Invoker::Invoke<BOOL, 2616>(id);
		}

		int ADD_POP_MULTIPLIER_SPHERE(float x, float y, float z, float radius, float pedMultiplier, float vehicleMultiplier, BOOL p6, BOOL p7) {
			return Invoker::Invoke<int, 2617>(x, y, z, radius, pedMultiplier, vehicleMultiplier, p6, p7);
		}

		BOOL DOES_POP_MULTIPLIER_SPHERE_EXIST(int id) {
			return Invoker::Invoke<BOOL, 2618>(id);
		}

		void REMOVE_POP_MULTIPLIER_SPHERE(int id, BOOL p1) {
			Invoker::Invoke<void, 2619>(id, p1);
		}

		void ENABLE_TENNIS_MODE(Ped ped, BOOL toggle, BOOL p2) {
			Invoker::Invoke<void, 2620>(ped, toggle, p2);
		}

		BOOL IS_TENNIS_MODE(Ped ped) {
			return Invoker::Invoke<BOOL, 2621>(ped);
		}

		void PLAY_TENNIS_SWING_ANIM(Ped ped, const char* animDict, const char* animName, float p3, float p4, BOOL p5) {
			Invoker::Invoke<void, 2622>(ped, animDict, animName, p3, p4, p5);
		}

		BOOL GET_TENNIS_SWING_ANIM_COMPLETE(Ped ped) {
			return Invoker::Invoke<BOOL, 2623>(ped);
		}

		BOOL GET_TENNIS_SWING_ANIM_CAN_BE_INTERRUPTED(Ped ped) {
			return Invoker::Invoke<BOOL, 2624>(ped);
		}

		BOOL GET_TENNIS_SWING_ANIM_SWUNG(Ped ped) {
			return Invoker::Invoke<BOOL, 2625>(ped);
		}

		void PLAY_TENNIS_DIVE_ANIM(Ped ped, int p1, float p2, float p3, float p4, BOOL p5) {
			Invoker::Invoke<void, 2626>(ped, p1, p2, p3, p4, p5);
		}

		void SET_TENNIS_MOVE_NETWORK_SIGNAL_FLOAT(Ped ped, const char* p1, float p2) {
			Invoker::Invoke<void, 2627>(ped, p1, p2);
		}

		void RESET_DISPATCH_SPAWN_LOCATION() {
			Invoker::Invoke<void, 2628>();
		}

		void SET_DISPATCH_SPAWN_LOCATION(float x, float y, float z) {
			Invoker::Invoke<void, 2629>(x, y, z);
		}

		void RESET_DISPATCH_IDEAL_SPAWN_DISTANCE() {
			Invoker::Invoke<void, 2630>();
		}

		void SET_DISPATCH_IDEAL_SPAWN_DISTANCE(float distance) {
			Invoker::Invoke<void, 2631>(distance);
		}

		void RESET_DISPATCH_TIME_BETWEEN_SPAWN_ATTEMPTS(Any p0) {
			Invoker::Invoke<void, 2632>(p0);
		}

		void SET_DISPATCH_TIME_BETWEEN_SPAWN_ATTEMPTS(Any p0, float p1) {
			Invoker::Invoke<void, 2633>(p0, p1);
		}

		void SET_DISPATCH_TIME_BETWEEN_SPAWN_ATTEMPTS_MULTIPLIER(Any p0, float p1) {
			Invoker::Invoke<void, 2634>(p0, p1);
		}

		int ADD_DISPATCH_SPAWN_ANGLED_BLOCKING_AREA(float x1, float y1, float z1, float x2, float y2, float z2, float width) {
			return Invoker::Invoke<int, 2635>(x1, y1, z1, x2, y2, z2, width);
		}

		int ADD_DISPATCH_SPAWN_SPHERE_BLOCKING_AREA(float x1, float y1, float x2, float y2) {
			return Invoker::Invoke<int, 2636>(x1, y1, x2, y2);
		}

		void REMOVE_DISPATCH_SPAWN_BLOCKING_AREA(int p0) {
			Invoker::Invoke<void, 2637>(p0);
		}

		void RESET_DISPATCH_SPAWN_BLOCKING_AREAS() {
			Invoker::Invoke<void, 2638>();
		}

		void RESET_WANTED_RESPONSE_NUM_PEDS_TO_SPAWN() {
			Invoker::Invoke<void, 2639>();
		}

		void SET_WANTED_RESPONSE_NUM_PEDS_TO_SPAWN(int p0, int p1) {
			Invoker::Invoke<void, 2640>(p0, p1);
		}

		void ADD_TACTICAL_NAV_MESH_POINT(float x, float y, float z) {
			Invoker::Invoke<void, 2641>(x, y, z);
		}

		void CLEAR_TACTICAL_NAV_MESH_POINTS() {
			Invoker::Invoke<void, 2642>();
		}

		void SET_RIOT_MODE_ENABLED(BOOL toggle) {
			Invoker::Invoke<void, 2643>(toggle);
		}

		void DISPLAY_ONSCREEN_KEYBOARD_WITH_LONGER_INITIAL_STRING(int p0, const char* windowTitle, Any* p2, const char* defaultText, const char* defaultConcat1, const char* defaultConcat2, const char* defaultConcat3, const char* defaultConcat4, const char* defaultConcat5, const char* defaultConcat6, const char* defaultConcat7, int maxInputLength) {
			Invoker::Invoke<void, 2644>(p0, windowTitle, p2, defaultText, defaultConcat1, defaultConcat2, defaultConcat3, defaultConcat4, defaultConcat5, defaultConcat6, defaultConcat7, maxInputLength);
		}

		void DISPLAY_ONSCREEN_KEYBOARD(int p0, const char* windowTitle, const char* p2, const char* defaultText, const char* defaultConcat1, const char* defaultConcat2, const char* defaultConcat3, int maxInputLength) {
			Invoker::Invoke<void, 2645>(p0, windowTitle, p2, defaultText, defaultConcat1, defaultConcat2, defaultConcat3, maxInputLength);
		}

		int UPDATE_ONSCREEN_KEYBOARD() {
			return Invoker::Invoke<int, 2646>();
		}

		const char* GET_ONSCREEN_KEYBOARD_RESULT() {
			return Invoker::Invoke<const char*, 2647>();
		}

		void CANCEL_ONSCREEN_KEYBOARD() {
			Invoker::Invoke<void, 2648>();
		}

		void NEXT_ONSCREEN_KEYBOARD_RESULT_WILL_DISPLAY_USING_THESE_FONTS(int p0) {
			Invoker::Invoke<void, 2649>(p0);
		}

		void ACTION_MANAGER_ENABLE_ACTION(Hash hash, BOOL enable) {
			Invoker::Invoke<void, 2650>(hash, enable);
		}

		int GET_REAL_WORLD_TIME() {
			return Invoker::Invoke<int, 2651>();
		}

		void SUPRESS_RANDOM_EVENT_THIS_FRAME(int eventType, BOOL suppress) {
			Invoker::Invoke<void, 2652>(eventType, suppress);
		}

		void SET_EXPLOSIVE_AMMO_THIS_FRAME(Player player) {
			Invoker::Invoke<void, 2653>(player);
		}

		void SET_FIRE_AMMO_THIS_FRAME(Player player) {
			Invoker::Invoke<void, 2654>(player);
		}

		void SET_EXPLOSIVE_MELEE_THIS_FRAME(Player player) {
			Invoker::Invoke<void, 2655>(player);
		}

		void SET_SUPER_JUMP_THIS_FRAME(Player player) {
			Invoker::Invoke<void, 2656>(player);
		}

		void SET_BEAST_JUMP_THIS_FRAME(Player player) {
			Invoker::Invoke<void, 2657>(player);
		}

		void SET_FORCED_JUMP_THIS_FRAME(Player player) {
			Invoker::Invoke<void, 2658>(player);
		}

		BOOL HAS_GAME_INSTALLED_THIS_SESSION() {
			return Invoker::Invoke<BOOL, 2659>();
		}

		void SET_TICKER_JOHNMARSTON_IS_DONE() {
			Invoker::Invoke<void, 2660>();
		}

		BOOL ARE_PROFILE_SETTINGS_VALID() {
			return Invoker::Invoke<BOOL, 2661>();
		}

		void PREVENT_ARREST_STATE_THIS_FRAME() {
			Invoker::Invoke<void, 2662>();
		}

		void FORCE_GAME_STATE_PLAYING() {
			Invoker::Invoke<void, 2663>();
		}

		void SCRIPT_RACE_INIT(int p0, int p1, Any p2, Any p3) {
			Invoker::Invoke<void, 2664>(p0, p1, p2, p3);
		}

		void SCRIPT_RACE_SHUTDOWN() {
			Invoker::Invoke<void, 2665>();
		}

		void SCRIPT_RACE_PLAYER_HIT_CHECKPOINT(Player player, Any p1, Any p2, Any p3) {
			Invoker::Invoke<void, 2666>(player, p1, p2, p3);
		}

		BOOL SCRIPT_RACE_GET_PLAYER_SPLIT_TIME(Player player, int* p1, int* p2) {
			return Invoker::Invoke<BOOL, 2667>(player, p1, p2);
		}

		void START_END_USER_BENCHMARK() {
			Invoker::Invoke<void, 2668>();
		}

		void STOP_END_USER_BENCHMARK() {
			Invoker::Invoke<void, 2669>();
		}

		void RESET_END_USER_BENCHMARK() {
			Invoker::Invoke<void, 2670>();
		}

		void SAVE_END_USER_BENCHMARK() {
			Invoker::Invoke<void, 2671>();
		}

		BOOL UI_STARTED_END_USER_BENCHMARK() {
			return Invoker::Invoke<BOOL, 2672>();
		}

		BOOL LANDING_SCREEN_STARTED_END_USER_BENCHMARK() {
			return Invoker::Invoke<BOOL, 2673>();
		}

		BOOL IS_COMMANDLINE_END_USER_BENCHMARK() {
			return Invoker::Invoke<BOOL, 2674>();
		}

		int GET_BENCHMARK_ITERATIONS() {
			return Invoker::Invoke<int, 2675>();
		}

		int GET_BENCHMARK_PASS() {
			return Invoker::Invoke<int, 2676>();
		}

		void RESTART_GAME() {
			Invoker::Invoke<void, 2677>();
		}

		void QUIT_GAME() {
			Invoker::Invoke<void, 2678>();
		}

		BOOL HAS_ASYNC_INSTALL_FINISHED() {
			return Invoker::Invoke<BOOL, 2679>();
		}

		void CLEANUP_ASYNC_INSTALL() {
			Invoker::Invoke<void, 2680>();
		}

		BOOL PLM_IS_IN_CONSTRAINED_MODE() {
			return Invoker::Invoke<BOOL, 2681>();
		}

		int PLM_GET_CONSTRAINED_DURATION_MS() {
			return Invoker::Invoke<int, 2682>();
		}

		void SET_PLAYER_IS_IN_ANIMAL_FORM(BOOL toggle) {
			Invoker::Invoke<void, 2683>(toggle);
		}

		BOOL GET_IS_PLAYER_IN_ANIMAL_FORM() {
			return Invoker::Invoke<BOOL, 2684>();
		}

		void SET_PLAYER_IS_REPEATING_A_MISSION(BOOL toggle) {
			Invoker::Invoke<void, 2685>(toggle);
		}

		void DISABLE_SCREEN_DIMMING_THIS_FRAME() {
			Invoker::Invoke<void, 2686>();
		}

		float GET_CITY_DENSITY() {
			return Invoker::Invoke<float, 2687>();
		}

		void USE_ACTIVE_CAMERA_FOR_TIMESLICING_CENTRE() {
			Invoker::Invoke<void, 2688>();
		}

		BOOL GET_SCRIPT_ROUTER_CONTEXT(Any* contextData) {
			return Invoker::Invoke<BOOL, 6676>(contextData);
		}

		BOOL SET_SCRIPT_ROUTER_LINK(Any* contextData) {
			return Invoker::Invoke<BOOL, 6675>(contextData);
		}

		BOOL HAS_PENDING_SCRIPT_ROUTER_LINK() {
			return Invoker::Invoke<BOOL, 6674>();
		}

		void CLEAR_SCRIPT_ROUTER_LINK() {
			Invoker::Invoke<void, 6673>();
		}

		void REPORT_INVALID_SCRIPT_ROUTER_ARGUMENT(const char* argument) {
			Invoker::Invoke<void, 6672>(argument);
		}

		void SET_ACTIVITY_SCRIPT_ROUTING_ENABLED(BOOL enabled) {
			Invoker::Invoke<void, 6671>(enabled);
		}

		BOOL IS_SESSION_INITIALIZED() {
			return Invoker::Invoke<BOOL, 6670>();
		}

		int GET_CHOSEN_CRIMINAL_CAREER() {
			return Invoker::Invoke<int, 6669>();
		}

		BOOL HAS_FINALIZED_CHOSEN_CRIMINAL_CAREER() {
			return Invoker::Invoke<BOOL, 6668>();
		}

		int GET_CHOSEN_MP_CHARACTER_SLOT() {
			return Invoker::Invoke<int, 6667>();
		}

		void RESET_CHOSEN_MP_CHARACTER_SLOT() {
			Invoker::Invoke<void, 6666>();
		}

		void SET_CONTENT_ID_INDEX(Hash contentId, int index) {
			Invoker::Invoke<void, 2689>(contentId, index);
		}

		int GET_CONTENT_ID_INDEX(Hash contentId) {
			return Invoker::Invoke<int, 2690>(contentId);
		}

		void _SET_CONTENT_PROP_TYPE(Hash model, int type) {
			Invoker::Invoke<void, 2691>(model, type);
		}

		int _GET_CONTENT_PROP_TYPE(Hash model) {
			return Invoker::Invoke<int, 2692>(model);
		}

	} // namespace MISC

	namespace MOBILE {

		void CREATE_MOBILE_PHONE(int phoneType) {
			Invoker::Invoke<void, 3172>(phoneType);
		}

		void DESTROY_MOBILE_PHONE() {
			Invoker::Invoke<void, 3173>();
		}

		void SET_MOBILE_PHONE_SCALE(float scale) {
			Invoker::Invoke<void, 3174>(scale);
		}

		void SET_MOBILE_PHONE_ROTATION(float rotX, float rotY, float rotZ, Any p3) {
			Invoker::Invoke<void, 3175>(rotX, rotY, rotZ, p3);
		}

		void GET_MOBILE_PHONE_ROTATION(Vector3* rotation, Vehicle p1) {
			Invoker::Invoke<void, 3176>(rotation, p1);
		}

		void SET_MOBILE_PHONE_POSITION(float posX, float posY, float posZ) {
			Invoker::Invoke<void, 3177>(posX, posY, posZ);
		}

		void GET_MOBILE_PHONE_POSITION(Vector3* position) {
			Invoker::Invoke<void, 3178>(position);
		}

		void SCRIPT_IS_MOVING_MOBILE_PHONE_OFFSCREEN(BOOL toggle) {
			Invoker::Invoke<void, 3179>(toggle);
		}

		BOOL CAN_PHONE_BE_SEEN_ON_SCREEN() {
			return Invoker::Invoke<BOOL, 3180>();
		}

		void SET_MOBILE_PHONE_DOF_STATE(BOOL toggle) {
			Invoker::Invoke<void, 3181>(toggle);
		}

		void CELL_SET_INPUT(int direction) {
			Invoker::Invoke<void, 3182>(direction);
		}

		void CELL_HORIZONTAL_MODE_TOGGLE(BOOL toggle) {
			Invoker::Invoke<void, 3183>(toggle);
		}

		void CELL_CAM_ACTIVATE(BOOL p0, BOOL p1) {
			Invoker::Invoke<void, 3184>(p0, p1);
		}

		void CELL_CAM_ACTIVATE_SELFIE_MODE(BOOL toggle) {
			Invoker::Invoke<void, 3185>(toggle);
		}

		void CELL_CAM_ACTIVATE_SHALLOW_DOF_MODE(BOOL toggle) {
			Invoker::Invoke<void, 3186>(toggle);
		}

		void CELL_CAM_SET_SELFIE_MODE_SIDE_OFFSET_SCALING(float p0) {
			Invoker::Invoke<void, 3187>(p0);
		}

		void CELL_CAM_SET_SELFIE_MODE_HORZ_PAN_OFFSET(float horizontalPan) {
			Invoker::Invoke<void, 3188>(horizontalPan);
		}

		void CELL_CAM_SET_SELFIE_MODE_VERT_PAN_OFFSET(float vertPan) {
			Invoker::Invoke<void, 3189>(vertPan);
		}

		void CELL_CAM_SET_SELFIE_MODE_ROLL_OFFSET(float roll) {
			Invoker::Invoke<void, 3190>(roll);
		}

		void CELL_CAM_SET_SELFIE_MODE_DISTANCE_SCALING(float distanceScaling) {
			Invoker::Invoke<void, 3191>(distanceScaling);
		}

		void CELL_CAM_SET_SELFIE_MODE_HEAD_YAW_OFFSET(float yaw) {
			Invoker::Invoke<void, 3192>(yaw);
		}

		void CELL_CAM_SET_SELFIE_MODE_HEAD_ROLL_OFFSET(float roll) {
			Invoker::Invoke<void, 3193>(roll);
		}

		void CELL_CAM_SET_SELFIE_MODE_HEAD_PITCH_OFFSET(float pitch) {
			Invoker::Invoke<void, 3194>(pitch);
		}

		BOOL CELL_CAM_IS_CHAR_VISIBLE_NO_FACE_CHECK(Entity entity) {
			return Invoker::Invoke<BOOL, 3195>(entity);
		}

		void GET_MOBILE_PHONE_RENDER_ID(int* renderId) {
			Invoker::Invoke<void, 3196>(renderId);
		}

	} // namespace MOBILE

	namespace MONEY {

		void NETWORK_INITIALIZE_CASH(int wallet, int bank) {
			Invoker::Invoke<void, 4833>(wallet, bank);
		}

		void NETWORK_DELETE_CHARACTER(int characterSlot, BOOL p1, BOOL p2) {
			Invoker::Invoke<void, 4834>(characterSlot, p1, p2);
		}

		void NETWORK_MANUAL_DELETE_CHARACTER(int characterSlot) {
			Invoker::Invoke<void, 4835>(characterSlot);
		}

		BOOL NETWORK_GET_PLAYER_IS_HIGH_EARNER() {
			return Invoker::Invoke<BOOL, 4836>();
		}

		void NETWORK_CLEAR_CHARACTER_WALLET(int characterSlot) {
			Invoker::Invoke<void, 4837>(characterSlot);
		}

		void NETWORK_GIVE_PLAYER_JOBSHARE_CASH(int amount, Any* gamerHandle) {
			Invoker::Invoke<void, 4838>(amount, gamerHandle);
		}

		void NETWORK_RECEIVE_PLAYER_JOBSHARE_CASH(int value, Any* gamerHandle) {
			Invoker::Invoke<void, 4839>(value, gamerHandle);
		}

		BOOL NETWORK_CAN_SHARE_JOB_CASH() {
			return Invoker::Invoke<BOOL, 4840>();
		}

		void NETWORK_REFUND_CASH(int index, const char* context, const char* reason, BOOL p3) {
			Invoker::Invoke<void, 4841>(index, context, reason, p3);
		}

		void NETWORK_DEDUCT_CASH(int amount, const char* p1, const char* p2, BOOL p3, BOOL p4, BOOL p5) {
			Invoker::Invoke<void, 4842>(amount, p1, p2, p3, p4, p5);
		}

		BOOL NETWORK_MONEY_CAN_BET(int amount, BOOL p1, BOOL p2) {
			return Invoker::Invoke<BOOL, 4843>(amount, p1, p2);
		}

		BOOL NETWORK_CAN_BET(int amount) {
			return Invoker::Invoke<BOOL, 4844>(amount);
		}

		BOOL NETWORK_CASINO_CAN_BET(Hash hash) {
			return Invoker::Invoke<BOOL, 4845>(hash);
		}

		BOOL NETWORK_CASINO_CAN_BET_PVC() {
			return Invoker::Invoke<BOOL, 4846>();
		}

		BOOL NETWORK_CASINO_CAN_BET_AMOUNT(Any p0) {
			return Invoker::Invoke<BOOL, 4847>(p0);
		}

		BOOL NETWORK_CASINO_CAN_BUY_CHIPS_PVC() {
			return Invoker::Invoke<BOOL, 4848>();
		}

		BOOL NETWORK_CASINO_BUY_CHIPS(int p0, int p1) {
			return Invoker::Invoke<BOOL, 4849>(p0, p1);
		}

		BOOL NETWORK_CASINO_SELL_CHIPS(int p0, int p1) {
			return Invoker::Invoke<BOOL, 4850>(p0, p1);
		}

		void NETWORK_DEFER_CASH_TRANSACTIONS_UNTIL_SHOP_SAVE() {
			Invoker::Invoke<void, 4851>();
		}

		BOOL CAN_PAY_AMOUNT_TO_BOSS(int p0, int p1, int amount, int* p3) {
			return Invoker::Invoke<BOOL, 4852>(p0, p1, amount, p3);
		}

		void NETWORK_EARN_FROM_PICKUP(int amount) {
			Invoker::Invoke<void, 4853>(amount);
		}

		void NETWORK_EARN_FROM_CASHING_OUT(int amount) {
			Invoker::Invoke<void, 4854>(amount);
		}

		void NETWORK_EARN_FROM_GANGATTACK_PICKUP(int amount) {
			Invoker::Invoke<void, 4855>(amount);
		}

		void NETWORK_EARN_ASSASSINATE_TARGET_KILLED(int amount) {
			Invoker::Invoke<void, 4856>(amount);
		}

		void NETWORK_EARN_FROM_ROB_ARMORED_CARS(int amount) {
			Invoker::Invoke<void, 4857>(amount);
		}

		void NETWORK_EARN_FROM_CRATE_DROP(int amount) {
			Invoker::Invoke<void, 4858>(amount);
		}

		void NETWORK_EARN_FROM_BETTING(int amount, const char* p1) {
			Invoker::Invoke<void, 4859>(amount, p1);
		}

		void NETWORK_EARN_FROM_JOB(int amount, const char* p1) {
			Invoker::Invoke<void, 4860>(amount, p1);
		}

		void NETWORK_EARN_FROM_JOBX2(int amount, const char* p1) {
			Invoker::Invoke<void, 4861>(amount, p1);
		}

		void NETWORK_EARN_FROM_PREMIUM_JOB(int amount, const char* p1) {
			Invoker::Invoke<void, 4862>(amount, p1);
		}

		void NETWORK_EARN_FROM_BEND_JOB(int amount, const char* heistHash) {
			Invoker::Invoke<void, 4863>(amount, heistHash);
		}

		void NETWORK_EARN_FROM_CHALLENGE_WIN(Any p0, Any* p1, BOOL p2) {
			Invoker::Invoke<void, 4864>(p0, p1, p2);
		}

		void NETWORK_EARN_FROM_BOUNTY(int amount, Any* gamerHandle, Any* p2, Any p3) {
			Invoker::Invoke<void, 4865>(amount, gamerHandle, p2, p3);
		}

		void NETWORK_EARN_FROM_IMPORT_EXPORT(int amount, Hash modelHash) {
			Invoker::Invoke<void, 4866>(amount, modelHash);
		}

		void NETWORK_EARN_FROM_HOLDUPS(int amount) {
			Invoker::Invoke<void, 4867>(amount);
		}

		void NETWORK_EARN_FROM_PROPERTY(int amount, Hash propertyName) {
			Invoker::Invoke<void, 4868>(amount, propertyName);
		}

		void NETWORK_EARN_FROM_AI_TARGET_KILL(Any p0, Any p1) {
			Invoker::Invoke<void, 4869>(p0, p1);
		}

		void NETWORK_EARN_FROM_NOT_BADSPORT(int amount) {
			Invoker::Invoke<void, 4870>(amount);
		}

		void NETWORK_EARN_FROM_VEHICLE(Any p0, Any p1, Any p2, Any p3, Any p4, Any p5, Any p6, Any p7) {
			Invoker::Invoke<void, 4871>(p0, p1, p2, p3, p4, p5, p6, p7);
		}

		void NETWORK_EARN_FROM_PERSONAL_VEHICLE(Any p0, Any p1, Any p2, Any p3, Any p4, Any p5, Any p6, Any p7, Any p8) {
			Invoker::Invoke<void, 4872>(p0, p1, p2, p3, p4, p5, p6, p7, p8);
		}

		void NETWORK_EARN_FROM_DAILY_OBJECTIVES(int amount, const char* type, int characterSlot) {
			Invoker::Invoke<void, 4873>(amount, type, characterSlot);
		}

		void NETWORK_EARN_FROM_AMBIENT_JOB(int p0, const char* p1, Any* p2) {
			Invoker::Invoke<void, 4874>(p0, p1, p2);
		}

		void NETWORK_EARN_FROM_JOB_BONUS(Any p0, Any* p1, Any* p2) {
			Invoker::Invoke<void, 4875>(p0, p1, p2);
		}

		void NETWORK_EARN_FROM_CRIMINAL_MASTERMIND(Any p0, Any p1, Any p2) {
			Invoker::Invoke<void, 4876>(p0, p1, p2);
		}

		void NETWORK_EARN_HEIST_AWARD(Any p0, Any p1, Any p2) {
			Invoker::Invoke<void, 4877>(p0, p1, p2);
		}

		void NETWORK_EARN_FIRST_TIME_BONUS(Any p0, Any p1, Any p2) {
			Invoker::Invoke<void, 4878>(p0, p1, p2);
		}

		void NETWORK_EARN_GOON(Any p0, Any p1, Any p2) {
			Invoker::Invoke<void, 4879>(p0, p1, p2);
		}

		void NETWORK_EARN_BOSS(Any p0, Any p1, Any p2) {
			Invoker::Invoke<void, 4880>(p0, p1, p2);
		}

		void NETWORK_EARN_AGENCY(Any p0, Any p1, Any p2, Any p3) {
			Invoker::Invoke<void, 4881>(p0, p1, p2, p3);
		}

		void NETWORK_EARN_FROM_WAREHOUSE(int amount, int id) {
			Invoker::Invoke<void, 4882>(amount, id);
		}

		void NETWORK_EARN_FROM_CONTRABAND(int amount, Any p1) {
			Invoker::Invoke<void, 4883>(amount, p1);
		}

		void NETWORK_EARN_FROM_DESTROYING_CONTRABAND(Any p0, Any p1, Any p2) {
			Invoker::Invoke<void, 4884>(p0, p1, p2);
		}

		void NETWORK_EARN_FROM_SMUGGLER_WORK(Any p0, Any p1, Any p2, Any p3, Any p4, Any p5) {
			Invoker::Invoke<void, 4885>(p0, p1, p2, p3, p4, p5);
		}

		void NETWORK_EARN_FROM_HANGAR_TRADE(Any p0, Any p1) {
			Invoker::Invoke<void, 4886>(p0, p1);
		}

		void NETWORK_EARN_PURCHASE_CLUB_HOUSE(Any p0, Any p1) {
			Invoker::Invoke<void, 4887>(p0, p1);
		}

		void NETWORK_EARN_FROM_BUSINESS_PRODUCT(int amount, Any p1, Any p2, Any p3) {
			Invoker::Invoke<void, 4888>(amount, p1, p2, p3);
		}

		void NETWORK_EARN_FROM_VEHICLE_EXPORT(int amount, Any p1, Any p2) {
			Invoker::Invoke<void, 4889>(amount, p1, p2);
		}

		void NETWORK_EARN_SMUGGLER_AGENCY(int amount, Any p1, Any p2, Any p3) {
			Invoker::Invoke<void, 4890>(amount, p1, p2, p3);
		}

		void NETWORK_EARN_BOUNTY_HUNTER_REWARD(Any p0) {
			Invoker::Invoke<void, 4891>(p0);
		}

		void NETWORK_EARN_FROM_BUSINESS_BATTLE(Any p0) {
			Invoker::Invoke<void, 4892>(p0);
		}

		void NETWORK_EARN_FROM_CLUB_MANAGEMENT_PARTICIPATION(Any p0, int p1) {
			Invoker::Invoke<void, 4893>(p0, p1);
		}

		void NETWORK_EARN_FROM_FMBB_PHONECALL_MISSION(Any p0) {
			Invoker::Invoke<void, 4894>(p0);
		}

		void NETWORK_EARN_FROM_BUSINESS_HUB_SELL(Any p0, Any p1, Any p2) {
			Invoker::Invoke<void, 4895>(p0, p1, p2);
		}

		void NETWORK_EARN_FROM_FMBB_BOSS_WORK(Any p0) {
			Invoker::Invoke<void, 4896>(p0);
		}

		void NETWORK_EARN_FMBB_WAGE_BONUS(Any p0) {
			Invoker::Invoke<void, 4897>(p0);
		}

		BOOL NETWORK_CAN_SPEND_MONEY(Any p0, BOOL p1, BOOL p2, BOOL p3, Any p4, Any p5) {
			return Invoker::Invoke<BOOL, 4898>(p0, p1, p2, p3, p4, p5);
		}

		BOOL NETWORK_CAN_SPEND_MONEY2(Any p0, BOOL p1, BOOL p2, BOOL p3, Any* p4, Any p5, Any p6) {
			return Invoker::Invoke<BOOL, 4899>(p0, p1, p2, p3, p4, p5, p6);
		}

		void NETWORK_BUY_ITEM(int amount, Hash item, Any p2, Any p3, BOOL p4, const char* item_name, Any p6, Any p7, Any p8, BOOL p9) {
			Invoker::Invoke<void, 4900>(amount, item, p2, p3, p4, item_name, p6, p7, p8, p9);
		}

		void NETWORK_SPENT_TAXI(int amount, BOOL p1, BOOL p2, Any p3, Any p4) {
			Invoker::Invoke<void, 4901>(amount, p1, p2, p3, p4);
		}

		void NETWORK_PAY_EMPLOYEE_WAGE(Any p0, BOOL p1, BOOL p2) {
			Invoker::Invoke<void, 4902>(p0, p1, p2);
		}

		void NETWORK_PAY_MATCH_ENTRY_FEE(int amount, const char* matchId, BOOL p2, BOOL p3) {
			Invoker::Invoke<void, 4903>(amount, matchId, p2, p3);
		}

		void NETWORK_SPENT_BETTING(int amount, int p1, const char* matchId, BOOL p3, BOOL p4) {
			Invoker::Invoke<void, 4904>(amount, p1, matchId, p3, p4);
		}

		void NETWORK_SPENT_WAGER(Any p0, Any p1, int amount) {
			Invoker::Invoke<void, 4905>(p0, p1, amount);
		}

		void NETWORK_SPENT_IN_STRIPCLUB(Any p0, BOOL p1, Any p2, BOOL p3) {
			Invoker::Invoke<void, 4906>(p0, p1, p2, p3);
		}

		void NETWORK_BUY_HEALTHCARE(int cost, BOOL p1, BOOL p2) {
			Invoker::Invoke<void, 4907>(cost, p1, p2);
		}

		void NETWORK_BUY_AIRSTRIKE(int cost, BOOL p1, BOOL p2, Any p3) {
			Invoker::Invoke<void, 4908>(cost, p1, p2, p3);
		}

		void NETWORK_BUY_BACKUP_GANG(int p0, int p1, BOOL p2, BOOL p3, int npcProvider) {
			Invoker::Invoke<void, 4909>(p0, p1, p2, p3, npcProvider);
		}

		void NETWORK_BUY_HELI_STRIKE(int cost, BOOL p1, BOOL p2, Any p3) {
			Invoker::Invoke<void, 4910>(cost, p1, p2, p3);
		}

		void NETWORK_SPENT_AMMO_DROP(Any p0, BOOL p1, BOOL p2, Any p3) {
			Invoker::Invoke<void, 4911>(p0, p1, p2, p3);
		}

		void NETWORK_BUY_BOUNTY(int amount, Player victim, BOOL p2, BOOL p3, Any p4) {
			Invoker::Invoke<void, 4912>(amount, victim, p2, p3, p4);
		}

		void NETWORK_BUY_PROPERTY(int cost, Hash propertyName, BOOL p2, BOOL p3) {
			Invoker::Invoke<void, 4913>(cost, propertyName, p2, p3);
		}

		void NETWORK_BUY_SMOKES(int p0, BOOL p1, BOOL p2) {
			Invoker::Invoke<void, 4914>(p0, p1, p2);
		}

		void NETWORK_SPENT_HELI_PICKUP(Any p0, BOOL p1, BOOL p2, Any p3) {
			Invoker::Invoke<void, 4915>(p0, p1, p2, p3);
		}

		void NETWORK_SPENT_BOAT_PICKUP(Any p0, BOOL p1, BOOL p2, Any p3) {
			Invoker::Invoke<void, 4916>(p0, p1, p2, p3);
		}

		void NETWORK_SPENT_BULL_SHARK(Any p0, BOOL p1, BOOL p2, Any p3) {
			Invoker::Invoke<void, 4917>(p0, p1, p2, p3);
		}

		void NETWORK_SPENT_CASH_DROP(int amount, BOOL p1, BOOL p2) {
			Invoker::Invoke<void, 4918>(amount, p1, p2);
		}

		void NETWORK_SPENT_HIRE_MUGGER(Any p0, BOOL p1, BOOL p2, Any p3) {
			Invoker::Invoke<void, 4919>(p0, p1, p2, p3);
		}

		void NETWORK_SPENT_ROBBED_BY_MUGGER(int amount, BOOL p1, BOOL p2, Any p3) {
			Invoker::Invoke<void, 4920>(amount, p1, p2, p3);
		}

		void NETWORK_SPENT_HIRE_MERCENARY(Any p0, BOOL p1, BOOL p2, Any p3) {
			Invoker::Invoke<void, 4921>(p0, p1, p2, p3);
		}

		void NETWORK_SPENT_BUY_WANTEDLEVEL(Any p0, Any* p1, BOOL p2, BOOL p3, Any p4) {
			Invoker::Invoke<void, 4922>(p0, p1, p2, p3, p4);
		}

		void NETWORK_SPENT_BUY_OFFTHERADAR(Any p0, BOOL p1, BOOL p2, Any p3) {
			Invoker::Invoke<void, 4923>(p0, p1, p2, p3);
		}

		void NETWORK_SPENT_BUY_REVEAL_PLAYERS(Any p0, BOOL p1, BOOL p2, Any p3) {
			Invoker::Invoke<void, 4924>(p0, p1, p2, p3);
		}

		void NETWORK_SPENT_CARWASH(Any p0, Any p1, Any p2, BOOL p3, BOOL p4) {
			Invoker::Invoke<void, 4925>(p0, p1, p2, p3, p4);
		}

		void NETWORK_SPENT_CINEMA(Any p0, Any p1, BOOL p2, BOOL p3) {
			Invoker::Invoke<void, 4926>(p0, p1, p2, p3);
		}

		void NETWORK_SPENT_TELESCOPE(Any p0, BOOL p1, BOOL p2) {
			Invoker::Invoke<void, 4927>(p0, p1, p2);
		}

		void NETWORK_SPENT_HOLDUPS(Any p0, BOOL p1, BOOL p2) {
			Invoker::Invoke<void, 4928>(p0, p1, p2);
		}

		void NETWORK_SPENT_BUY_PASSIVE_MODE(Any p0, BOOL p1, BOOL p2, Any p3) {
			Invoker::Invoke<void, 4929>(p0, p1, p2, p3);
		}

		void NETWORK_SPENT_BANK_INTEREST(int p0, BOOL p1, BOOL p2) {
			Invoker::Invoke<void, 4930>(p0, p1, p2);
		}

		void NETWORK_SPENT_PROSTITUTES(Any p0, BOOL p1, BOOL p2) {
			Invoker::Invoke<void, 4931>(p0, p1, p2);
		}

		void NETWORK_SPENT_ARREST_BAIL(Any p0, BOOL p1, BOOL p2) {
			Invoker::Invoke<void, 4932>(p0, p1, p2);
		}

		void NETWORK_SPENT_PAY_VEHICLE_INSURANCE_PREMIUM(int amount, Hash vehicleModel, Any* gamerHandle, BOOL notBankrupt, BOOL hasTheMoney) {
			Invoker::Invoke<void, 4933>(amount, vehicleModel, gamerHandle, notBankrupt, hasTheMoney);
		}

		void NETWORK_SPENT_CALL_PLAYER(Any p0, Any* p1, BOOL p2, BOOL p3) {
			Invoker::Invoke<void, 4934>(p0, p1, p2, p3);
		}

		void NETWORK_SPENT_BOUNTY(Any p0, BOOL p1, BOOL p2) {
			Invoker::Invoke<void, 4935>(p0, p1, p2);
		}

		void NETWORK_SPENT_FROM_ROCKSTAR(int p0, BOOL p1, BOOL p2) {
			Invoker::Invoke<void, 4936>(p0, p1, p2);
		}

		int NETWORK_SPEND_EARNED_FROM_BANK_AND_WALLETS(int amount) {
			return Invoker::Invoke<int, 4937>(amount);
		}

		const char* PROCESS_CASH_GIFT(int* p0, int* p1, const char* p2) {
			return Invoker::Invoke<const char*, 4938>(p0, p1, p2);
		}

		void NETWORK_SPENT_MOVE_SUBMARINE(Any p0, Any p1, Any p2) {
			Invoker::Invoke<void, 4939>(p0, p1, p2);
		}

		void NETWORK_SPENT_PLAYER_HEALTHCARE(int p0, int p1, BOOL p2, BOOL p3) {
			Invoker::Invoke<void, 4940>(p0, p1, p2, p3);
		}

		void NETWORK_SPENT_NO_COPS(Any p0, BOOL p1, BOOL p2, Any p3) {
			Invoker::Invoke<void, 4941>(p0, p1, p2, p3);
		}

		void NETWORK_SPENT_CARGO_SOURCING(Any p0, Any p1, Any p2, Any p3, Any p4, Any p5, Any p6) {
			Invoker::Invoke<void, 4942>(p0, p1, p2, p3, p4, p5, p6);
		}

		void NETWORK_SPENT_REQUEST_JOB(Any p0, BOOL p1, BOOL p2, Any p3) {
			Invoker::Invoke<void, 4943>(p0, p1, p2, p3);
		}

		void NETWORK_SPENT_REQUEST_HEIST(Any p0, BOOL p1, BOOL p2, Any p3) {
			Invoker::Invoke<void, 4944>(p0, p1, p2, p3);
		}

		void NETWORK_BUY_FAIRGROUND_RIDE(int amount, Any p1, BOOL p2, BOOL p3, Any p4) {
			Invoker::Invoke<void, 4945>(amount, p1, p2, p3, p4);
		}

		BOOL NETWORK_ECONOMY_HAS_FIXED_CRAZY_NUMBERS() {
			return Invoker::Invoke<BOOL, 4946>();
		}

		void NETWORK_SPENT_JOB_SKIP(int amount, const char* matchId, BOOL p2, BOOL p3) {
			Invoker::Invoke<void, 4947>(amount, matchId, p2, p3);
		}

		BOOL NETWORK_SPENT_BOSS_GOON(int amount, BOOL p1, BOOL p2) {
			return Invoker::Invoke<BOOL, 4948>(amount, p1, p2);
		}

		void NETWORK_SPEND_GOON(int p0, int p1, int amount) {
			Invoker::Invoke<void, 4949>(p0, p1, amount);
		}

		void NETWORK_SPEND_BOSS(Any p0, Any p1, Any p2) {
			Invoker::Invoke<void, 4950>(p0, p1, p2);
		}

		void NETWORK_SPENT_MOVE_YACHT(int amount, BOOL p1, BOOL p2) {
			Invoker::Invoke<void, 4951>(amount, p1, p2);
		}

		void NETWORK_SPENT_RENAME_ORGANIZATION(Any p0, Any p1, Any p2, Any p3) {
			Invoker::Invoke<void, 4952>(p0, p1, p2, p3);
		}

		void NETWORK_BUY_CONTRABAND_MISSION(int p0, int p1, Hash p2, BOOL p3, BOOL p4) {
			Invoker::Invoke<void, 4953>(p0, p1, p2, p3, p4);
		}

		void NETWORK_SPENT_PA_SERVICE_HELI(Any p0, Any p1, Any p2, Any p3) {
			Invoker::Invoke<void, 4954>(p0, p1, p2, p3);
		}

		void NETWORK_SPENT_PA_SERVICE_VEHICLE(Any p0, Any p1, Any p2, Any p3) {
			Invoker::Invoke<void, 4955>(p0, p1, p2, p3);
		}

		void NETWORK_SPENT_PA_SERVICE_SNACK(Any p0, Any p1, Any p2, Any p3) {
			Invoker::Invoke<void, 4956>(p0, p1, p2, p3);
		}

		void NETWORK_SPENT_PA_SERVICE_DANCER(Any p0, Any p1, Any p2, Any p3) {
			Invoker::Invoke<void, 4957>(p0, p1, p2, p3);
		}

		void NETWORK_SPENT_PA_SERVICE_IMPOUND(Any p0, Any p1, Any p2) {
			Invoker::Invoke<void, 4958>(p0, p1, p2);
		}

		void NETWORK_SPENT_PA_HELI_PICKUP(Any p0, Any p1, Any p2, Any p3) {
			Invoker::Invoke<void, 4959>(p0, p1, p2, p3);
		}

		void NETWORK_SPENT_PURCHASE_OFFICE_PROPERTY(Any p0, Any p1, Any p2, Any p3, Any p4) {
			Invoker::Invoke<void, 4960>(p0, p1, p2, p3, p4);
		}

		void NETWORK_SPENT_UPGRADE_OFFICE_PROPERTY(Any p0, Any p1, Any p2, Any p3, Any p4) {
			Invoker::Invoke<void, 4961>(p0, p1, p2, p3, p4);
		}

		void NETWORK_SPENT_PURCHASE_WAREHOUSE_PROPERTY(Any p0, Any p1, Any p2, Any p3) {
			Invoker::Invoke<void, 4962>(p0, p1, p2, p3);
		}

		void NETWORK_SPENT_UPGRADE_WAREHOUSE_PROPERTY(Any p0, Any p1, Any p2, Any p3) {
			Invoker::Invoke<void, 4963>(p0, p1, p2, p3);
		}

		void NETWORK_SPENT_PURCHASE_IMPEXP_WAREHOUSE_PROPERTY(int amount, Any* data, BOOL p2, BOOL p3) {
			Invoker::Invoke<void, 4964>(amount, data, p2, p3);
		}

		void NETWORK_SPENT_UPGRADE_IMPEXP_WAREHOUSE_PROPERTY(Any p0, Any p1, Any p2, Any p3) {
			Invoker::Invoke<void, 4965>(p0, p1, p2, p3);
		}

		void NETWORK_SPENT_TRADE_IMPEXP_WAREHOUSE_PROPERTY(Any p0, Any p1, Any p2, Any p3) {
			Invoker::Invoke<void, 4966>(p0, p1, p2, p3);
		}

		void NETWORK_SPENT_ORDER_WAREHOUSE_VEHICLE(Any p0, Any p1, Any p2, Any p3) {
			Invoker::Invoke<void, 4967>(p0, p1, p2, p3);
		}

		void NETWORK_SPENT_ORDER_BODYGUARD_VEHICLE(Any p0, Any p1, Any p2, Any p3) {
			Invoker::Invoke<void, 4968>(p0, p1, p2, p3);
		}

		void NETWORK_SPENT_JUKEBOX(Any p0, Any p1, Any p2, Any p3) {
			Invoker::Invoke<void, 4969>(p0, p1, p2, p3);
		}

		void NETWORK_SPENT_PURCHASE_CLUB_HOUSE(Any p0, Any p1, Any p2, Any p3) {
			Invoker::Invoke<void, 4970>(p0, p1, p2, p3);
		}

		void NETWORK_SPENT_UPGRADE_CLUB_HOUSE(Any p0, Any p1, Any p2, Any p3) {
			Invoker::Invoke<void, 4971>(p0, p1, p2, p3);
		}

		void NETWORK_SPENT_PURCHASE_BUSINESS_PROPERTY(Any p0, Any p1, Any p2, Any p3) {
			Invoker::Invoke<void, 4972>(p0, p1, p2, p3);
		}

		void NETWORK_SPENT_UPGRADE_BUSINESS_PROPERTY(Any p0, Any p1, Any p2, Any p3) {
			Invoker::Invoke<void, 4973>(p0, p1, p2, p3);
		}

		void NETWORK_SPENT_TRADE_BUSINESS_PROPERTY(Any p0, Any p1, Any p2, Any p3) {
			Invoker::Invoke<void, 4974>(p0, p1, p2, p3);
		}

		void NETWORK_SPENT_MC_ABILITY(Any p0, Any p1, Any p2, Any p3, Any p4) {
			Invoker::Invoke<void, 4975>(p0, p1, p2, p3, p4);
		}

		void NETWORK_SPENT_PAY_BUSINESS_SUPPLIES(Any p0, Any p1, Any p2, Any p3) {
			Invoker::Invoke<void, 4976>(p0, p1, p2, p3);
		}

		void NETWORK_SPENT_CHANGE_APPEARANCE(Any p0, Any p1, Any p2) {
			Invoker::Invoke<void, 4977>(p0, p1, p2);
		}

		void NETWORK_SPENT_VEHICLE_EXPORT_MODS(Any p0, Any p1, Any p2, Any p3, Any p4, Any p5, Any p6, Any p7, Any p8, Any p9) {
			Invoker::Invoke<void, 4978>(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9);
		}

		void NETWORK_SPENT_PURCHASE_OFFICE_GARAGE(Any p0, Any p1, Any p2, Any p3) {
			Invoker::Invoke<void, 4979>(p0, p1, p2, p3);
		}

		void NETWORK_SPENT_UPGRADE_OFFICE_GARAGE(Any p0, Any p1, Any p2, Any p3) {
			Invoker::Invoke<void, 4980>(p0, p1, p2, p3);
		}

		void NETWORK_SPENT_IMPORT_EXPORT_REPAIR(Any p0, Any p1, Any p2) {
			Invoker::Invoke<void, 4981>(p0, p1, p2);
		}

		void NETWORK_SPENT_PURCHASE_HANGAR(Any p0, Any p1, Any p2, Any p3) {
			Invoker::Invoke<void, 4982>(p0, p1, p2, p3);
		}

		void NETWORK_SPENT_UPGRADE_HANGAR(Any p0, Any p1, Any p2, Any p3) {
			Invoker::Invoke<void, 4983>(p0, p1, p2, p3);
		}

		void NETWORK_SPENT_HANGAR_UTILITY_CHARGES(int amount, BOOL p1, BOOL p2) {
			Invoker::Invoke<void, 4984>(amount, p1, p2);
		}

		void NETWORK_SPENT_HANGAR_STAFF_CHARGES(int amount, BOOL p1, BOOL p2) {
			Invoker::Invoke<void, 4985>(amount, p1, p2);
		}

		void NETWORK_SPENT_BUY_TRUCK(Any p0, Any p1, Any p2, Any p3) {
			Invoker::Invoke<void, 4986>(p0, p1, p2, p3);
		}

		void NETWORK_SPENT_UPGRADE_TRUCK(Any p0, Any p1, Any p2, Any p3) {
			Invoker::Invoke<void, 4987>(p0, p1, p2, p3);
		}

		void NETWORK_SPENT_BUY_BUNKER(Any p0, Any p1, Any p2, Any p3) {
			Invoker::Invoke<void, 4988>(p0, p1, p2, p3);
		}

		void NETWORK_SPENT_UPRADE_BUNKER(Any p0, Any p1, Any p2, Any p3) {
			Invoker::Invoke<void, 4989>(p0, p1, p2, p3);
		}

		void NETWORK_EARN_FROM_SELL_BUNKER(int amount, Hash bunkerHash) {
			Invoker::Invoke<void, 4990>(amount, bunkerHash);
		}

		void NETWORK_SPENT_BALLISTIC_EQUIPMENT(int amount, BOOL p1, BOOL p2) {
			Invoker::Invoke<void, 4991>(amount, p1, p2);
		}

		void NETWORK_EARN_RDR_BONUS(int amount, Any p1) {
			Invoker::Invoke<void, 4992>(amount, p1);
		}

		void NETWORK_EARN_WAGE_PAYMENT(int amount, Any p1) {
			Invoker::Invoke<void, 4993>(amount, p1);
		}

		void NETWORK_EARN_WAGE_PAYMENT_BONUS(int amount) {
			Invoker::Invoke<void, 4994>(amount);
		}

		void NETWORK_SPENT_BUY_BASE(Any p0, Any p1, Any p2, Any p3) {
			Invoker::Invoke<void, 4995>(p0, p1, p2, p3);
		}

		void NETWORK_SPENT_UPGRADE_BASE(Any p0, Any p1, Any p2, Any p3) {
			Invoker::Invoke<void, 4996>(p0, p1, p2, p3);
		}

		void NETWORK_SPENT_BUY_TILTROTOR(Any p0, Any p1, Any p2, Any p3) {
			Invoker::Invoke<void, 4997>(p0, p1, p2, p3);
		}

		void NETWORK_SPENT_UPGRADE_TILTROTOR(Any p0, Any p1, Any p2, Any p3) {
			Invoker::Invoke<void, 4998>(p0, p1, p2, p3);
		}

		void NETWORK_SPENT_EMPLOY_ASSASSINS(Any p0, Any p1, Any p2, Any p3) {
			Invoker::Invoke<void, 4999>(p0, p1, p2, p3);
		}

		void NETWORK_SPEND_GANGOPS_CANNON(Any p0, Any p1, Any p2, Any p3) {
			Invoker::Invoke<void, 5000>(p0, p1, p2, p3);
		}

		void NETWORK_SPEND_GANGOPS_SKIP_MISSION(Any p0, Any p1, Any p2, Any p3) {
			Invoker::Invoke<void, 5001>(p0, p1, p2, p3);
		}

		void NETWORK_SPEND_CASINO_HEIST_SKIP_MISSION(Any p0, Any p1, Any p2, Any p3) {
			Invoker::Invoke<void, 5002>(p0, p1, p2, p3);
		}

		void NETWORK_EARN_SELL_BASE(int amount, Hash baseNameHash) {
			Invoker::Invoke<void, 5003>(amount, baseNameHash);
		}

		void NETWORK_EARN_TARGET_REFUND(int amount, int p1) {
			Invoker::Invoke<void, 5004>(amount, p1);
		}

		void NETWORK_EARN_GANGOPS_WAGES(int amount, int p1) {
			Invoker::Invoke<void, 5005>(amount, p1);
		}

		void NETWORK_EARN_GANGOPS_WAGES_BONUS(int amount, int p1) {
			Invoker::Invoke<void, 5006>(amount, p1);
		}

		void NETWORK_EARN_DAR_CHALLENGE(int amount, Any p1) {
			Invoker::Invoke<void, 5007>(amount, p1);
		}

		void NETWORK_EARN_DOOMSDAY_FINALE_BONUS(int amount, Hash vehicleHash) {
			Invoker::Invoke<void, 5008>(amount, vehicleHash);
		}

		void NETWORK_EARN_GANGOPS_AWARD(int amount, const char* p1, Any p2) {
			Invoker::Invoke<void, 5009>(amount, p1, p2);
		}

		void NETWORK_EARN_GANGOPS_ELITE(int amount, const char* p1, int actIndex) {
			Invoker::Invoke<void, 5010>(amount, p1, actIndex);
		}

		void NETWORK_SERVICE_EARN_GANGOPS_RIVAL_DELIVERY(int earnedMoney) {
			Invoker::Invoke<void, 5011>(earnedMoney);
		}

		void NETWORK_SPEND_GANGOPS_START_STRAND(int type, int amount, BOOL p2, BOOL p3) {
			Invoker::Invoke<void, 5012>(type, amount, p2, p3);
		}

		void NETWORK_SPEND_GANGOPS_TRIP_SKIP(int amount, BOOL p1, BOOL p2) {
			Invoker::Invoke<void, 5013>(amount, p1, p2);
		}

		void NETWORK_EARN_GANGOPS_PREP_PARTICIPATION(int amount) {
			Invoker::Invoke<void, 5014>(amount);
		}

		void NETWORK_EARN_GANGOPS_SETUP(int amount, const char* p1) {
			Invoker::Invoke<void, 5015>(amount, p1);
		}

		void NETWORK_EARN_GANGOPS_FINALE(int amount, const char* p1) {
			Invoker::Invoke<void, 5016>(amount, p1);
		}

		void NETWORK_SPEND_GANGOPS_REPAIR_COST(Any p0, Any p1, Any p2) {
			Invoker::Invoke<void, 5017>(p0, p1, p2);
		}

		void NETWORK_EARN_NIGHTCLUB(Any p0, Any p1) {
			Invoker::Invoke<void, 5018>(p0, p1);
		}

		void NETWORK_EARN_NIGHTCLUB_DANCING(Any p0) {
			Invoker::Invoke<void, 5019>(p0);
		}

		void NETWORK_EARN_BB_EVENT_BONUS(int amount) {
			Invoker::Invoke<void, 5020>(amount);
		}

		void NETWORK_SPENT_PURCHASE_HACKER_TRUCK(Any p0, Any p1, Any p2, Any p3) {
			Invoker::Invoke<void, 5021>(p0, p1, p2, p3);
		}

		void NETWORK_SPENT_UPGRADE_HACKER_TRUCK(Any p0, Any p1, Any p2, Any p3) {
			Invoker::Invoke<void, 5022>(p0, p1, p2, p3);
		}

		void NETWORK_EARN_HACKER_TRUCK(Any p0, int amount, Any p2, Any p3) {
			Invoker::Invoke<void, 5023>(p0, amount, p2, p3);
		}

		void NETWORK_SPENT_PURCHASE_NIGHTCLUB_AND_WAREHOUSE(Any p0, Any p1, Any p2, Any p3) {
			Invoker::Invoke<void, 5024>(p0, p1, p2, p3);
		}

		void NETWORK_SPENT_UPGRADE_NIGHTCLUB_AND_WAREHOUSE(Any p0, Any p1, Any p2, Any p3) {
			Invoker::Invoke<void, 5025>(p0, p1, p2, p3);
		}

		void NETWORK_EARN_NIGHTCLUB_AND_WAREHOUSE(Any p0, Any p1, Any p2, Any p3, Any p4, Any p5, Any p6) {
			Invoker::Invoke<void, 5026>(p0, p1, p2, p3, p4, p5, p6);
		}

		void NETWORK_SPEND_NIGHTCLUB_AND_WAREHOUSE(Any p0, Any p1, Any p2, Any p3) {
			Invoker::Invoke<void, 5027>(p0, p1, p2, p3);
		}

		void NETWORK_SPENT_RDR_HATCHET_BONUS(int amount, BOOL p1, BOOL p2) {
			Invoker::Invoke<void, 5028>(amount, p1, p2);
		}

		void NETWORK_SPENT_NIGHTCLUB_ENTRY_FEE(Player player, int amount, Any p1, BOOL p2, BOOL p3) {
			Invoker::Invoke<void, 5029>(player, amount, p1, p2, p3);
		}

		void NETWORK_SPEND_NIGHTCLUB_BAR_DRINK(int amount, Any p1, BOOL p2, BOOL p3) {
			Invoker::Invoke<void, 5030>(amount, p1, p2, p3);
		}

		void NETWORK_SPEND_BOUNTY_HUNTER_MISSION(int amount, BOOL p1, BOOL p2) {
			Invoker::Invoke<void, 5031>(amount, p1, p2);
		}

		void NETWORK_SPENT_REHIRE_DJ(int amount, Any p1, BOOL p2, BOOL p3) {
			Invoker::Invoke<void, 5032>(amount, p1, p2, p3);
		}

		void NETWORK_SPENT_ARENA_JOIN_SPECTATOR(int amount, Any p1, BOOL p2, BOOL p3) {
			Invoker::Invoke<void, 5033>(amount, p1, p2, p3);
		}

		void NETWORK_EARN_ARENA_SKILL_LEVEL_PROGRESSION(int amount, Any p1) {
			Invoker::Invoke<void, 5034>(amount, p1);
		}

		void NETWORK_EARN_ARENA_CAREER_PROGRESSION(int amount, Any p1) {
			Invoker::Invoke<void, 5035>(amount, p1);
		}

		void NETWORK_SPEND_MAKE_IT_RAIN(int amount, BOOL p1, BOOL p2) {
			Invoker::Invoke<void, 5036>(amount, p1, p2);
		}

		void NETWORK_SPEND_BUY_ARENA(int amount, BOOL p1, BOOL p2, const char* p3) {
			Invoker::Invoke<void, 5037>(amount, p1, p2, p3);
		}

		void NETWORK_SPEND_UPGRADE_ARENA(int amount, BOOL p1, BOOL p2, const char* p3) {
			Invoker::Invoke<void, 5038>(amount, p1, p2, p3);
		}

		void NETWORK_SPEND_ARENA_SPECTATOR_BOX(int amount, int type, BOOL p2, BOOL p3) {
			Invoker::Invoke<void, 5039>(amount, type, p2, p3);
		}

		void NETWORK_SPEND_SPIN_THE_WHEEL_PAYMENT(int amount, Any p1, BOOL p2) {
			Invoker::Invoke<void, 5040>(amount, p1, p2);
		}

		void NETWORK_EARN_SPIN_THE_WHEEL_CASH(int amount) {
			Invoker::Invoke<void, 5041>(amount);
		}

		void NETWORK_SPEND_ARENA_PREMIUM(int amount, BOOL p1, BOOL p2) {
			Invoker::Invoke<void, 5042>(amount, p1, p2);
		}

		void NETWORK_EARN_ARENA_WAR(int amount, Any p1, Any p2, Any p3) {
			Invoker::Invoke<void, 5043>(amount, p1, p2, p3);
		}

		void NETWORK_EARN_ARENA_WAR_ASSASSINATE_TARGET(int amount) {
			Invoker::Invoke<void, 5044>(amount);
		}

		void NETWORK_EARN_ARENA_WAR_EVENT_CARGO(int amount) {
			Invoker::Invoke<void, 5045>(amount);
		}

		void NETWORK_EARN_RC_TIME_TRIAL(int amount) {
			Invoker::Invoke<void, 5046>(amount);
		}

		void NETWORK_EARN_DAILY_OBJECTIVE_EVENT(int amount) {
			Invoker::Invoke<void, 5047>(amount);
		}

		void NETWORK_SPEND_CASINO_MEMBERSHIP(int amount, BOOL p1, BOOL p2, int p3) {
			Invoker::Invoke<void, 5048>(amount, p1, p2, p3);
		}

		void NETWORK_SPEND_BUY_CASINO(int amount, BOOL p1, BOOL p2, Any* data) {
			Invoker::Invoke<void, 5049>(amount, p1, p2, data);
		}

		void NETWORK_SPEND_UPGRADE_CASINO(int amount, BOOL p1, BOOL p2, Any* data) {
			Invoker::Invoke<void, 5050>(amount, p1, p2, data);
		}

		void NETWORK_SPEND_CASINO_GENERIC(int amount, Any p1, Any p2, Any p3, Any p4) {
			Invoker::Invoke<void, 5051>(amount, p1, p2, p3, p4);
		}

		void NETWORK_EARN_CASINO_TIME_TRIAL_WIN(int amount) {
			Invoker::Invoke<void, 5052>(amount);
		}

		void NETWORK_EARN_COLLECTABLES_ACTION_FIGURES(int amount) {
			Invoker::Invoke<void, 5053>(amount);
		}

		void NETWORK_EARN_CASINO_COLLECTABLE_COMPLETED_COLLECTION(int amount) {
			Invoker::Invoke<void, 5054>(amount);
		}

		void NETWORK_EARN_SELL_PRIZE_VEHICLE(int amount, Any p1, Any p2) {
			Invoker::Invoke<void, 5055>(amount, p1, p2);
		}

		void NETWORK_EARN_CASINO_MISSION_REWARD(int amount) {
			Invoker::Invoke<void, 5056>(amount);
		}

		void NETWORK_EARN_CASINO_STORY_MISSION_REWARD(int amount) {
			Invoker::Invoke<void, 5057>(amount);
		}

		void NETWORK_EARN_CASINO_MISSION_PARTICIPATION(int amount) {
			Invoker::Invoke<void, 5058>(amount);
		}

		void NETWORK_EARN_CASINO_AWARD(int amount, Hash hash) {
			Invoker::Invoke<void, 5059>(amount, hash);
		}

		void NETWORK_SPEND_BUY_ARCADE(Any p0, Any p1, Any p2, Any p3) {
			Invoker::Invoke<void, 5060>(p0, p1, p2, p3);
		}

		void NETWORK_SPEND_UPGRADE_ARCADE(Any p0, Any p1, Any p2, Any p3) {
			Invoker::Invoke<void, 5061>(p0, p1, p2, p3);
		}

		void NETWORK_SPEND_CASINO_HEIST(Any p0, Any p1, Any p2, Any p3, Any p4, Any p5, Any p6, Any p7, Any p8, Any p9, Any p10) {
			Invoker::Invoke<void, 5062>(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10);
		}

		void NETWORK_SPEND_ARCADE_MGMT(Any p0, Any p1, Any p2, Any p3, Any p4) {
			Invoker::Invoke<void, 5063>(p0, p1, p2, p3, p4);
		}

		void NETWORK_SPEND_PLAY_ARCADE(Any p0, Any p1, Any p2, Any p3, Any p4) {
			Invoker::Invoke<void, 5064>(p0, p1, p2, p3, p4);
		}

		void NETWORK_SPEND_ARCADE(Any p0, Any p1, Any p2, Any p3, Any p4) {
			Invoker::Invoke<void, 5065>(p0, p1, p2, p3, p4);
		}

		void NETWORK_EARN_CASINO_HEIST(Any p0, Any p1, Any p2, Any p3, Any p4, Any p5, Any p6) {
			Invoker::Invoke<void, 5066>(p0, p1, p2, p3, p4, p5, p6);
		}

		void NETWORK_EARN_UPGRADE_ARCADE(Any p0, Any p1, Any p2) {
			Invoker::Invoke<void, 5067>(p0, p1, p2);
		}

		void NETWORK_EARN_ARCADE(Any p0, Any p1, Any p2, Any p3, Any p4, Any p5) {
			Invoker::Invoke<void, 5068>(p0, p1, p2, p3, p4, p5);
		}

		void NETWORK_EARN_COLLECTABLES(Any p0, Any p1, Any p2) {
			Invoker::Invoke<void, 5069>(p0, p1, p2);
		}

		void NETWORK_EARN_CHALLENGE(int amount, Any p1, Any p2) {
			Invoker::Invoke<void, 5070>(amount, p1, p2);
		}

		void NETWORK_EARN_CASINO_HEIST_AWARDS(Any p0, Any p1, Any p2, Any p3, Any p4) {
			Invoker::Invoke<void, 5071>(p0, p1, p2, p3, p4);
		}

		void NETWORK_EARN_COLLECTABLE_ITEM(int amount, Any p1) {
			Invoker::Invoke<void, 5072>(amount, p1);
		}

		void NETWORK_EARN_COLLECTABLE_COMPLETED_COLLECTION(int amount, Any p1) {
			Invoker::Invoke<void, 5073>(amount, p1);
		}

		void NETWORK_EARN_YATCH_MISSION(int amount, Any p1) {
			Invoker::Invoke<void, 5074>(amount, p1);
		}

		void NETWORK_EARN_DISPATCH_CALL(int amount, Any p1) {
			Invoker::Invoke<void, 5075>(amount, p1);
		}

		void NETWORK_SPEND_BEACH_PARTY(Any p0) {
			Invoker::Invoke<void, 5076>(p0);
		}

		void NETWORK_SPEND_SUBMARINE(Any p0, Any p1, Any p2, Any p3, Any p4, Any p5) {
			Invoker::Invoke<void, 5077>(p0, p1, p2, p3, p4, p5);
		}

		void NETWORK_SPEND_CASINO_CLUB(int amount1, Any p1, BOOL p2, Any p3, int p4, int p5, int p6, int amount2, Any p8) {
			Invoker::Invoke<void, 5078>(amount1, p1, p2, p3, p4, p5, p6, amount2, p8);
		}

		void NETWORK_SPEND_BUY_SUB(Any p0, Any p1, Any p2, Any p3) {
			Invoker::Invoke<void, 5079>(p0, p1, p2, p3);
		}

		void NETWORK_SPEND_UPGRADE_SUB(Any p0, Any p1, Any p2, Any p3) {
			Invoker::Invoke<void, 5080>(p0, p1, p2, p3);
		}

		void NETWORK_SPEND_ISLAND_HEIST(Any p0, Any p1, Any p2, Any p3) {
			Invoker::Invoke<void, 5081>(p0, p1, p2, p3);
		}

		void NETWORK_EARN_ISLAND_HEIST(int amount1, Any p1, Any p2, Any p3, int amount2, int p5) {
			Invoker::Invoke<void, 5082>(amount1, p1, p2, p3, amount2, p5);
		}

		void NETWORK_EARN_BEACH_PARTY_LOST_FOUND(Any p0, Any p1, Any p2) {
			Invoker::Invoke<void, 5083>(p0, p1, p2);
		}

		void NETWORK_EARN_FROM_ISLAND_HEIST_DJ_MISSION(Any p0, Any p1) {
			Invoker::Invoke<void, 5084>(p0, p1);
		}

		void NETWORK_SPEND_CAR_CLUB_MEMBERSHIP(int amount1, Any p1, Any p2, int amount2, Any p4) {
			Invoker::Invoke<void, 5085>(amount1, p1, p2, amount2, p4);
		}

		void NETWORK_SPEND_CAR_CLUB_BAR(Any p0, BOOL p1, BOOL p2, Any p3) {
			Invoker::Invoke<void, 5086>(p0, p1, p2, p3);
		}

		void NETWORK_SPEND_AUTOSHOP_MODIFY(Any p0, Any p1, Any p2, Any p3, Any p4) {
			Invoker::Invoke<void, 5087>(p0, p1, p2, p3, p4);
		}

		void NETWORK_SPEND_CAR_CLUB_TAKEOVER(Any p0, Any p1, Any p2, Any p3) {
			Invoker::Invoke<void, 5088>(p0, p1, p2, p3);
		}

		void NETWORK_SPEND_BUY_AUTOSHOP(Any p0, Any p1, Any p2, Any p3) {
			Invoker::Invoke<void, 5089>(p0, p1, p2, p3);
		}

		void NETWORK_SPEND_UPGRADE_AUTOSHOP(Any p0, Any p1, Any p2, Any p3) {
			Invoker::Invoke<void, 5090>(p0, p1, p2, p3);
		}

		void NETWORK_EARN_AUTOSHOP_BUSINESS(Any p0, Any p1, Any p2) {
			Invoker::Invoke<void, 5091>(p0, p1, p2);
		}

		void NETWORK_EARN_AUTOSHOP_INCOME(Any p0, int p1) {
			Invoker::Invoke<void, 5092>(p0, p1);
		}

		void NETWORK_EARN_CARCLUB_MEMBERSHIP(Any p0) {
			Invoker::Invoke<void, 5093>(p0);
		}

		void NETWORK_EARN_DAILY_VEHICLE(Any p0, Any p1) {
			Invoker::Invoke<void, 5094>(p0, p1);
		}

		void NETWORK_EARN_DAILY_VEHICLE_BONUS(Any p0) {
			Invoker::Invoke<void, 5095>(p0);
		}

		void NETWORK_EARN_TUNER_AWARD(Any p0, Any p1, Any p2) {
			Invoker::Invoke<void, 5096>(p0, p1, p2);
		}

		void NETWORK_EARN_TUNER_ROBBERY(Any p0, Any p1, Any p2, Any p3, Any p4) {
			Invoker::Invoke<void, 5097>(p0, p1, p2, p3, p4);
		}

		void NETWORK_EARN_UPGRADE_AUTOSHOP(Any p0, Any p1) {
			Invoker::Invoke<void, 5098>(p0, p1);
		}

		void NETWORK_SPEND_INTERACTION_MENU_ABILITY(Any p0, Any p1, Any p2, Any p3) {
			Invoker::Invoke<void, 5099>(p0, p1, p2, p3);
		}

		void NETWORK_SPEND_SET_COMMON_FIELDS(Any p0, Any p1, Any p2, BOOL p3) {
			Invoker::Invoke<void, 5100>(p0, p1, p2, p3);
		}

		void NETWORK_SPEND_SET_DISCOUNT(BOOL p0) {
			Invoker::Invoke<void, 5101>(p0);
		}

		void NETWORK_SPEND_BUY_AGENCY(Any p0, Any p1, Any p2, Any p3) {
			Invoker::Invoke<void, 5102>(p0, p1, p2, p3);
		}

		void NETWORK_SPEND_UPGRADE_AGENCY(Any p0, Any p1, Any p2, Any p3) {
			Invoker::Invoke<void, 5103>(p0, p1, p2, p3);
		}

		void NETWORK_SPEND_AGENCY(Any p0, Any p1, Any p2, Any p3, Any p4) {
			Invoker::Invoke<void, 5104>(p0, p1, p2, p3, p4);
		}

		void NETWORK_SPEND_HIDDEN(Any p0, Any p1, Any p2, Any p3) {
			Invoker::Invoke<void, 5105>(p0, p1, p2, p3);
		}

		void NETWORK_SPEND_SOURCE_BIKE(Any p0, Any p1, Any p2, Any p3) {
			Invoker::Invoke<void, 5106>(p0, p1, p2, p3);
		}

		void NETWORK_SPEND_COMP_SUV(Any p0, Any p1, Any p2, Any p3) {
			Invoker::Invoke<void, 5107>(p0, p1, p2, p3);
		}

		void NETWORK_SPEND_SUV_FST_TRVL(int p0, BOOL p1, BOOL p2, Any p3) {
			Invoker::Invoke<void, 5108>(p0, p1, p2, p3);
		}

		void NETWORK_SPEND_SUPPLY(Any p0, Any p1, Any p2, Any p3) {
			Invoker::Invoke<void, 5109>(p0, p1, p2, p3);
		}

		void NETWORK_SPEND_BIKE_SHOP(Any p0, Any p1, Any p2, Any p3) {
			Invoker::Invoke<void, 5110>(p0, p1, p2, p3);
		}

		void NETWORK_SPEND_VEHICLE_REQUESTED(Any p0, Any p1, Any p2, Any p3, Any p4) {
			Invoker::Invoke<void, 5111>(p0, p1, p2, p3, p4);
		}

		void NETWORK_SPEND_GUNRUNNING(Any p0, Any p1, Any p2, Any p3) {
			Invoker::Invoke<void, 5112>(p0, p1, p2, p3);
		}

		void NETWORK_EARN_AGENCY_SAFE(Any p0, Any p1) {
			Invoker::Invoke<void, 5113>(p0, p1);
		}

		void NETWORK_EARN_AWARD_CONTRACT(Any p0, Any p1) {
			Invoker::Invoke<void, 5114>(p0, p1);
		}

		void NETWORK_EARN_AGENCY_CONTRACT(Any p0, Any p1) {
			Invoker::Invoke<void, 5115>(p0, p1);
		}

		void NETWORK_EARN_AWARD_PHONE(Any p0, Any p1) {
			Invoker::Invoke<void, 5116>(p0, p1);
		}

		void NETWORK_EARN_AGENCY_PHONE(Any p0, Any p1, Any p2) {
			Invoker::Invoke<void, 5117>(p0, p1, p2);
		}

		void NETWORK_EARN_AWARD_FIXER_MISSION(Any p0, Any p1) {
			Invoker::Invoke<void, 5118>(p0, p1);
		}

		void NETWORK_EARN_FIXER_PREP(Any p0, Any p1) {
			Invoker::Invoke<void, 5119>(p0, p1);
		}

		void NETWORK_EARN_FIXER_FINALE(Any p0, Any p1) {
			Invoker::Invoke<void, 5120>(p0, p1);
		}

		void NETWORK_EARN_FIXER_AGENCY_SHORT_TRIP(Any p0, Any p1) {
			Invoker::Invoke<void, 5121>(p0, p1);
		}

		void NETWORK_EARN_AWARD_SHORT_TRIP(Any p0, Any p1) {
			Invoker::Invoke<void, 5122>(p0, p1);
		}

		void NETWORK_EARN_FIXER_RIVAL_DELIVERY(Any p0, Any p1) {
			Invoker::Invoke<void, 5123>(p0, p1);
		}

		void NETWORK_EARN_UPGRADE_AGENCY(Any p0, Any p1) {
			Invoker::Invoke<void, 5124>(p0, p1);
		}

		void NETWORK_SPEND_APARTMENT_UTILITIES(int amount, BOOL p1, BOOL p2, Any* data) {
			Invoker::Invoke<void, 5125>(amount, p1, p2, data);
		}

		void NETWORK_SPEND_BUSINESS_PROPERTY_FEES(Any p0, Any p1, Any p2, Any p3) {
			Invoker::Invoke<void, 5126>(p0, p1, p2, p3);
		}

		void NETWORK_EARN_SIGHTSEEING_REWARD(Any p0, Any p1, Any p2, Any p3) {
			Invoker::Invoke<void, 5127>(p0, p1, p2, p3);
		}

		void NETWORK_EARN_BIKER_SHOP(Any p0, Any p1) {
			Invoker::Invoke<void, 5128>(p0, p1);
		}

		void NETWORK_EARN_BIKER(Any p0) {
			Invoker::Invoke<void, 5129>(p0);
		}

		void NETWORK_YOHAN_SOURCE_GOODS(Any p0, Any p1, Any p2, Any p3) {
			Invoker::Invoke<void, 5130>(p0, p1, p2, p3);
		}

		void _NETWORK_SPEND_BUY_MFGARAGE(Any p0, Any p1, Any p2, Any p3) {
			Invoker::Invoke<void, 5131>(p0, p1, p2, p3);
		}

		void _NETWORK_SPEND_UPGRADE_MFGARAGE(Any p0, Any p1, Any p2, Any p3) {
			Invoker::Invoke<void, 5132>(p0, p1, p2, p3);
		}

		void _NETWORK_SPEND_BUY_SUPPLIES(int p0, BOOL p1, BOOL p2, int p3) {
			Invoker::Invoke<void, 5133>(p0, p1, p2, p3);
		}

		void _NETWORK_SPEND_BUY_ACID_LAB(Any p0, Any p1, Any p2, Any p3) {
			Invoker::Invoke<void, 5134>(p0, p1, p2, p3);
		}

		void _NETWORK_SPEND_UPGRADE_ACID_LAB_EQUIPMENT(Any p0, Any p1, Any p2, Any p3) {
			Invoker::Invoke<void, 5135>(p0, p1, p2, p3);
		}

		void _NETWORK_SPEND_UPGRADE_ACID_LAB_ARMOR(int p0, BOOL p1, BOOL p2, int p3) {
			Invoker::Invoke<void, 5136>(p0, p1, p2, p3);
		}

		void _NETWORK_SPEND_UPGRADE_ACID_LAB_SCOOP(int p0, BOOL p1, BOOL p2, int p3) {
			Invoker::Invoke<void, 5137>(p0, p1, p2, p3);
		}

		void _NETWORK_SPEND_UPGRADE_ACID_LAB_MINES(int p0, BOOL p1, BOOL p2, int p3) {
			Invoker::Invoke<void, 5138>(p0, p1, p2, p3);
		}

		void _NETWORK_SPEND_RENAME_ACID_LAB(Any p0, Any p1, Any p2, Any p3) {
			Invoker::Invoke<void, 5139>(p0, p1, p2, p3);
		}

		void _NETWORK_SPEND_RENAME_ACID_PRODUCT(Any p0, Any p1, Any p2, Any p3) {
			Invoker::Invoke<void, 5140>(p0, p1, p2, p3);
		}

		void _NETWORK_EARN_AWARD_JUGGALO_MISSION(Any p0, Any p1) {
			Invoker::Invoke<void, 5141>(p0, p1);
		}

		void _NETWORK_EARN_AWARD_ACID_LAB(Any p0, Any p1) {
			Invoker::Invoke<void, 5142>(p0, p1);
		}

		void _NETWORK_EARN_AWARD_DAILY_STASH(Any p0, Any p1) {
			Invoker::Invoke<void, 5143>(p0, p1);
		}

		void _NETWORK_EARN_AWARD_DEAD_DROP(Any p0, Any p1) {
			Invoker::Invoke<void, 5144>(p0, p1);
		}

		void _NETWORK_EARN_AWARD_RANDOM_EVENT(Any p0, Any p1) {
			Invoker::Invoke<void, 5145>(p0, p1);
		}

		void _NETWORK_EARN_AWARD_TAXI(Any p0, Any p1) {
			Invoker::Invoke<void, 5146>(p0, p1);
		}

		void _NETWORK_EARN_STREET_DEALER(Any p0, Any p1) {
			Invoker::Invoke<void, 5147>(p0, p1);
		}

		void _NETWORK_EARN_SELL_ACID(Any p0, Any p1) {
			Invoker::Invoke<void, 5148>(p0, p1);
		}

		void _NETWORK_EARN_SETUP_PARTICIPATION_ACID_LAB(Any p0, Any p1) {
			Invoker::Invoke<void, 5149>(p0, p1);
		}

		void _NETWORK_EARN_SOURCE_PARTICIPATION_ACID_LAB(Any p0, Any p1) {
			Invoker::Invoke<void, 5150>(p0, p1);
		}

		void _NETWORK_EARN_SELL_PARTICIPATION_ACID_LAB(Any p0, Any p1) {
			Invoker::Invoke<void, 5151>(p0, p1);
		}

		void _NETWORK_EARN_JUGGALO_STORY_MISSION(Any p0, Any p1) {
			Invoker::Invoke<void, 5152>(p0, p1);
		}

		void _NETWORK_EARN_JUGGALO_STORY_MISSION_PARTICIPATION(Any p0, Any p1) {
			Invoker::Invoke<void, 5153>(p0, p1);
		}

		void _NETWORK_EARN_FOOLIGAN_JOB(Any p0, Any p1) {
			Invoker::Invoke<void, 5154>(p0, p1);
		}

		void _NETWORK_EARN_FOOLIGAN_JOB_PARTICIPATION(Any p0, Any p1) {
			Invoker::Invoke<void, 5155>(p0, p1);
		}

		void _NETWORK_EARN_TAXI_JOB(Any p0, Any p1) {
			Invoker::Invoke<void, 5156>(p0, p1);
		}

		void _NETWORK_EARN_DAILY_STASH_HOUSE_COMPLETED(Any p0, Any p1) {
			Invoker::Invoke<void, 5157>(p0, p1);
		}

		void _NETWORK_EARN_DAILY_STASH_HOUSE_PARTICIPATION(Any p0, Any p1) {
			Invoker::Invoke<void, 5158>(p0, p1);
		}

		void _NETWORK_EARN_AVENGER(int amount, int p1) {
			Invoker::Invoke<void, 5159>(amount, p1);
		}

		void _NETWORK_EARN_SMUGGLER_OPS(Any p0, Any p1, Any p2) {
			Invoker::Invoke<void, 5160>(p0, p1, p2);
		}

		void _NETWORK_EARN_BONUS_OBJECTIVE(int amount, Any p1, Any p2) {
			Invoker::Invoke<void, 5161>(amount, p1, p2);
		}

		void _NETWORK_EARN_PROGRESS_HUB(Any p0, Any p1) {
			Invoker::Invoke<void, 5162>(p0, p1);
		}

		void _NETWORK_SPENT_AIR_FREIGHT(int hangarCargoSourcingPrice, BOOL fromBank, BOOL fromBankAndWallet, int cost, int warehouseId, int warehouseSlot, Any p6) {
			Invoker::Invoke<void, 5163>(hangarCargoSourcingPrice, fromBank, fromBankAndWallet, cost, warehouseId, warehouseSlot, p6);
		}

		void _NETWORK_SPENT_SKIP_CARGO_SOURCE_SETUP(int amount, BOOL fromBank, BOOL fromBankAndWallet, int cost) {
			Invoker::Invoke<void, 5164>(amount, fromBank, fromBankAndWallet, cost);
		}

		void _NETWORK_SPENT_STEALTH_MODULE(int amount, BOOL fromBank, BOOL fromBankAndWallet, Hash p3) {
			Invoker::Invoke<void, 5165>(amount, fromBank, fromBankAndWallet, p3);
		}

		void _NETWORK_SPENT_MISSILE_JAMMER(int amount, BOOL fromBank, BOOL fromBankAndWallet, Hash p3) {
			Invoker::Invoke<void, 5166>(amount, fromBank, fromBankAndWallet, p3);
		}

		void _NETWORK_SPENT_GENERIC(int price, BOOL p1, BOOL p2, Hash stat, Hash spent, const char* p5, const char* p6, Any* data) {
			Invoker::Invoke<void, 5167>(price, p1, p2, stat, spent, p5, p6, data);
		}

		void _NETWORK_EARN_GENERIC(int amount, Hash earn, const char* p2, const char* p3, Any* data) {
			Invoker::Invoke<void, 5168>(amount, earn, p2, p3, data);
		}

		void _NETWORK_CLEAR_TRANSACTION_TELEMETRY_NONCE() {
			Invoker::Invoke<void, 5169>();
		}

		int NETWORK_GET_VC_BANK_BALANCE() {
			return Invoker::Invoke<int, 5170>();
		}

		int NETWORK_GET_VC_WALLET_BALANCE(int characterSlot) {
			return Invoker::Invoke<int, 5171>(characterSlot);
		}

		int NETWORK_GET_VC_BALANCE() {
			return Invoker::Invoke<int, 5172>();
		}

		int NETWORK_GET_EVC_BALANCE() {
			return Invoker::Invoke<int, 5173>();
		}

		int NETWORK_GET_PVC_BALANCE() {
			return Invoker::Invoke<int, 5174>();
		}

		const char* NETWORK_GET_STRING_WALLET_BALANCE(int characterSlot) {
			return Invoker::Invoke<const char*, 5175>(characterSlot);
		}

		const char* NETWORK_GET_STRING_BANK_BALANCE() {
			return Invoker::Invoke<const char*, 5176>();
		}

		const char* NETWORK_GET_STRING_BANK_WALLET_BALANCE(int character) {
			return Invoker::Invoke<const char*, 5177>(character);
		}

		BOOL NETWORK_GET_CAN_SPEND_FROM_WALLET(int amount, int characterSlot) {
			return Invoker::Invoke<BOOL, 5178>(amount, characterSlot);
		}

		BOOL NETWORK_GET_CAN_SPEND_FROM_BANK(int amount) {
			return Invoker::Invoke<BOOL, 5179>(amount);
		}

		BOOL NETWORK_GET_CAN_SPEND_FROM_BANK_AND_WALLET(int amount, int characterSlot) {
			return Invoker::Invoke<BOOL, 5180>(amount, characterSlot);
		}

		int NETWORK_GET_PVC_TRANSFER_BALANCE() {
			return Invoker::Invoke<int, 5181>();
		}

		BOOL NETWORK_GET_CAN_TRANSFER_CASH(int amount) {
			return Invoker::Invoke<BOOL, 5182>(amount);
		}

		BOOL NETWORK_CAN_RECEIVE_PLAYER_CASH(Any p0, Any p1, Any p2, Any p3) {
			return Invoker::Invoke<BOOL, 5183>(p0, p1, p2, p3);
		}

		int NETWORK_GET_REMAINING_TRANSFER_BALANCE() {
			return Invoker::Invoke<int, 5184>();
		}

		int WITHDRAW_VC(int amount) {
			return Invoker::Invoke<int, 5185>(amount);
		}

		BOOL DEPOSIT_VC(int amount) {
			return Invoker::Invoke<BOOL, 5186>(amount);
		}

		BOOL HAS_VC_WITHDRAWAL_COMPLETED(Any p0) {
			return Invoker::Invoke<BOOL, 5187>(p0);
		}

		BOOL WAS_VC_WITHDRAWAL_SUCCESSFUL(Any p0) {
			return Invoker::Invoke<BOOL, 5188>(p0);
		}

		BOOL NETWORK_GET_MP_WINDFALL_AVAILABLE() {
			return Invoker::Invoke<BOOL, 6665>();
		}

	} // namespace MONEY

	namespace NETSHOPPING {

		BOOL NET_GAMESERVER_USE_SERVER_TRANSACTIONS() {
			return Invoker::Invoke<BOOL, 26>();
		}

		BOOL NET_GAMESERVER_CATALOG_ITEM_IS_VALID(const char* name) {
			return Invoker::Invoke<BOOL, 27>(name);
		}

		BOOL NET_GAMESERVER_CATALOG_ITEM_KEY_IS_VALID(Hash hash) {
			return Invoker::Invoke<BOOL, 28>(hash);
		}

		int NET_GAMESERVER_GET_PRICE(Hash itemHash, Hash categoryHash, BOOL p2) {
			return Invoker::Invoke<int, 29>(itemHash, categoryHash, p2);
		}

		BOOL NET_GAMESERVER_CATALOG_IS_VALID() {
			return Invoker::Invoke<BOOL, 30>();
		}

		BOOL NET_GAMESERVER_IS_CATALOG_CURRENT() {
			return Invoker::Invoke<BOOL, 31>();
		}

		Hash NET_GAMESERVER_GET_CATALOG_CLOUD_CRC() {
			return Invoker::Invoke<Hash, 32>();
		}

		BOOL NET_GAMESERVER_REFRESH_SERVER_CATALOG() {
			return Invoker::Invoke<BOOL, 33>();
		}

		BOOL NET_GAMESERVER_RETRIEVE_CATALOG_REFRESH_STATUS(int* state) {
			return Invoker::Invoke<BOOL, 34>(state);
		}

		BOOL NET_GAMESERVER_INIT_SESSION() {
			return Invoker::Invoke<BOOL, 35>();
		}

		BOOL NET_GAMESERVER_RETRIEVE_INIT_SESSION_STATUS(int* p0) {
			return Invoker::Invoke<BOOL, 36>(p0);
		}

		BOOL NET_GAMESERVER_START_SESSION(int charSlot) {
			return Invoker::Invoke<BOOL, 37>(charSlot);
		}

		BOOL NET_GAMESERVER_START_SESSION_PENDING() {
			return Invoker::Invoke<BOOL, 38>();
		}

		BOOL NET_GAMESERVER_RETRIEVE_START_SESSION_STATUS(int* p0) {
			return Invoker::Invoke<BOOL, 39>(p0);
		}

		BOOL NET_GAMESERVER_RETRIEVE_SESSION_ERROR_CODE(int* p0) {
			return Invoker::Invoke<BOOL, 40>(p0);
		}

		BOOL NET_GAMESERVER_IS_SESSION_VALID(int charSlot) {
			return Invoker::Invoke<BOOL, 41>(charSlot);
		}

		BOOL NET_GAMESERVER_CLEAR_SESSION(int p0) {
			return Invoker::Invoke<BOOL, 42>(p0);
		}

		BOOL NET_GAMESERVER_SESSION_APPLY_RECEIVED_DATA(int charSlot) {
			return Invoker::Invoke<BOOL, 43>(charSlot);
		}

		BOOL NET_GAMESERVER_IS_SESSION_REFRESH_PENDING() {
			return Invoker::Invoke<BOOL, 44>();
		}

		BOOL _NET_GAMESERVER_FLAG_FOR_SESSION_RESTART() {
			return Invoker::Invoke<BOOL, 6664>();
		}

		BOOL NET_GAMESERVER_START_SESSION_RESTART(BOOL inventory, BOOL playerbalance) {
			return Invoker::Invoke<BOOL, 45>(inventory, playerbalance);
		}

		BOOL NET_GAMESERVER_TRANSACTION_IN_PROGRESS() {
			return Invoker::Invoke<BOOL, 46>();
		}

		BOOL NET_GAMESERVER_GET_SESSION_STATE_AND_STATUS(int* p0, BOOL* p1) {
			return Invoker::Invoke<BOOL, 47>(p0, p1);
		}

		BOOL NET_GAMESERVER_BASKET_START(int* transactionId, Hash categoryHash, Hash actionHash, int flags) {
			return Invoker::Invoke<BOOL, 48>(transactionId, categoryHash, actionHash, flags);
		}

		BOOL NET_GAMESERVER_BASKET_END() {
			return Invoker::Invoke<BOOL, 49>();
		}

		BOOL NET_GAMESERVER_BASKET_IS_ACTIVE() {
			return Invoker::Invoke<BOOL, 50>();
		}

		BOOL NET_GAMESERVER_BASKET_ADD_ITEM(Any* itemData, int quantity) {
			return Invoker::Invoke<BOOL, 51>(itemData, quantity);
		}

		BOOL NET_GAMESERVER_BASKET_IS_FULL() {
			return Invoker::Invoke<BOOL, 52>();
		}

		BOOL NET_GAMESERVER_BASKET_APPLY_SERVER_DATA(Any p0, Any* p1) {
			return Invoker::Invoke<BOOL, 53>(p0, p1);
		}

		BOOL NET_GAMESERVER_CHECKOUT_START(int transactionId) {
			return Invoker::Invoke<BOOL, 54>(transactionId);
		}

		BOOL NET_GAMESERVER_CHECKOUT_PENDING(int transactionId) {
			return Invoker::Invoke<BOOL, 6663>(transactionId);
		}

		BOOL NET_GAMESERVER_BEGIN_SERVICE(int* transactionId, Hash categoryHash, Hash itemHash, Hash actionTypeHash, int value, int flags) {
			return Invoker::Invoke<BOOL, 55>(transactionId, categoryHash, itemHash, actionTypeHash, value, flags);
		}

		BOOL NET_GAMESERVER_END_SERVICE(int transactionId) {
			return Invoker::Invoke<BOOL, 56>(transactionId);
		}

		BOOL NET_GAMESERVER_DELETE_CHARACTER(int slot, BOOL transfer, Hash reason) {
			return Invoker::Invoke<BOOL, 57>(slot, transfer, reason);
		}

		int NET_GAMESERVER_DELETE_CHARACTER_GET_STATUS() {
			return Invoker::Invoke<int, 58>();
		}

		BOOL NET_GAMESERVER_DELETE_SET_TELEMETRY_NONCE_SEED() {
			return Invoker::Invoke<BOOL, 59>();
		}

		BOOL NET_GAMESERVER_TRANSFER_BANK_TO_WALLET(int charSlot, int amount) {
			return Invoker::Invoke<BOOL, 60>(charSlot, amount);
		}

		BOOL NET_GAMESERVER_TRANSFER_WALLET_TO_BANK(int charSlot, int amount) {
			return Invoker::Invoke<BOOL, 61>(charSlot, amount);
		}

		int NET_GAMESERVER_TRANSFER_BANK_TO_WALLET_GET_STATUS() {
			return Invoker::Invoke<int, 62>();
		}

		int NET_GAMESERVER_TRANSFER_WALLET_TO_BANK_GET_STATUS() {
			return Invoker::Invoke<int, 63>();
		}

		BOOL NET_GAMESERVER_TRANSFER_CASH_SET_TELEMETRY_NONCE_SEED() {
			return Invoker::Invoke<BOOL, 64>();
		}

		BOOL NET_GAMESERVER_SET_TELEMETRY_NONCE_SEED(int p0) {
			return Invoker::Invoke<BOOL, 65>(p0);
		}

	} // namespace NETSHOPPING

	namespace NETWORK {

		const char* GET_ONLINE_VERSION() {
			return Invoker::Invoke<const char*, 5189>();
		}

		BOOL NETWORK_IS_SIGNED_IN() {
			return Invoker::Invoke<BOOL, 5190>();
		}

		BOOL NETWORK_IS_SIGNED_ONLINE() {
			return Invoker::Invoke<BOOL, 5191>();
		}

		BOOL NETWORK_IS_NP_AVAILABLE() {
			return Invoker::Invoke<BOOL, 5192>();
		}

		BOOL NETWORK_IS_NP_PENDING() {
			return Invoker::Invoke<BOOL, 5193>();
		}

		int NETWORK_GET_NP_UNAVAILABLE_REASON() {
			return Invoker::Invoke<int, 5194>();
		}

		BOOL NETWORK_IS_CONNETED_TO_NP_PRESENCE() {
			return Invoker::Invoke<BOOL, 5195>();
		}

		BOOL NETWORK_IS_LOGGED_IN_TO_PSN() {
			return Invoker::Invoke<BOOL, 5196>();
		}

		BOOL NETWORK_HAS_VALID_ROS_CREDENTIALS() {
			return Invoker::Invoke<BOOL, 5197>();
		}

		BOOL NETWORK_IS_REFRESHING_ROS_CREDENTIALS() {
			return Invoker::Invoke<BOOL, 5198>();
		}

		BOOL NETWORK_IS_CLOUD_AVAILABLE() {
			return Invoker::Invoke<BOOL, 5199>();
		}

		BOOL NETWORK_HAS_SOCIAL_CLUB_ACCOUNT() {
			return Invoker::Invoke<BOOL, 5200>();
		}

		BOOL NETWORK_ARE_SOCIAL_CLUB_POLICIES_CURRENT() {
			return Invoker::Invoke<BOOL, 5201>();
		}

		BOOL NETWORK_IS_HOST() {
			return Invoker::Invoke<BOOL, 5202>();
		}

		Player NETWORK_GET_HOST_PLAYER_INDEX() {
			return Invoker::Invoke<Player, 5203>();
		}

		BOOL NETWORK_WAS_GAME_SUSPENDED() {
			return Invoker::Invoke<BOOL, 5204>();
		}

		BOOL NETWORK_HAVE_ONLINE_PRIVILEGES() {
			return Invoker::Invoke<BOOL, 5205>();
		}

		BOOL _NETWORK_MULTIPLAYER_CROSSPLAY_NOT_ALLOWED() {
			return Invoker::Invoke<BOOL, 6662>();
		}

		BOOL NETWORK_HAS_AGE_RESTRICTIONS() {
			return Invoker::Invoke<BOOL, 5206>();
		}

		BOOL NETWORK_HAVE_USER_CONTENT_PRIVILEGES(int p0) {
			return Invoker::Invoke<BOOL, 5207>(p0);
		}

		BOOL NETWORK_HAVE_COMMUNICATION_PRIVILEGES(int p0, Player player) {
			return Invoker::Invoke<BOOL, 5208>(p0, player);
		}

		BOOL _NETWORK_HAVE_PLATFORM_COMMUNICATION_PRIVILEGES() {
			return Invoker::Invoke<BOOL, 5209>();
		}

		BOOL NETWORK_CHECK_ONLINE_PRIVILEGES(Any p0, BOOL p1) {
			return Invoker::Invoke<BOOL, 5210>(p0, p1);
		}

		BOOL NETWORK_CHECK_USER_CONTENT_PRIVILEGES(int p0, int p1, BOOL p2) {
			return Invoker::Invoke<BOOL, 5211>(p0, p1, p2);
		}

		BOOL NETWORK_CHECK_COMMUNICATION_PRIVILEGES(int p0, int p1, BOOL p2) {
			return Invoker::Invoke<BOOL, 5212>(p0, p1, p2);
		}

		BOOL NETWORK_CHECK_TEXT_COMMUNICATION_PRIVILEGES(Any p0, Any p1, Any p2) {
			return Invoker::Invoke<BOOL, 5213>(p0, p1, p2);
		}

		BOOL NETWORK_IS_USING_ONLINE_PROMOTION() {
			return Invoker::Invoke<BOOL, 5214>();
		}

		BOOL NETWORK_SHOULD_SHOW_PROMOTION_ALERT_SCREEN() {
			return Invoker::Invoke<BOOL, 5215>();
		}

		BOOL NETWORK_HAS_SOCIAL_NETWORKING_SHARING_PRIV() {
			return Invoker::Invoke<BOOL, 5216>();
		}

		int NETWORK_GET_AGE_GROUP() {
			return Invoker::Invoke<int, 5217>();
		}

		BOOL NETWORK_CHECK_PRIVILEGES(Any p0, Any p1, Any p2) {
			return Invoker::Invoke<BOOL, 5218>(p0, p1, p2);
		}

		BOOL NETWORK_IS_PRIVILEGE_CHECK_IN_PROGRESS() {
			return Invoker::Invoke<BOOL, 5219>();
		}

		void NETWORK_SET_PRIVILEGE_CHECK_RESULT_NOT_NEEDED() {
			Invoker::Invoke<void, 5220>();
		}

		BOOL NETWORK_RESOLVE_PRIVILEGE_USER_CONTENT() {
			return Invoker::Invoke<BOOL, 5221>();
		}

		BOOL NETWORK_HAVE_PLATFORM_SUBSCRIPTION() {
			return Invoker::Invoke<BOOL, 5222>();
		}

		BOOL NETWORK_IS_PLATFORM_SUBSCRIPTION_CHECK_PENDING() {
			return Invoker::Invoke<BOOL, 5223>();
		}

		void NETWORK_SHOW_ACCOUNT_UPGRADE_UI() {
			Invoker::Invoke<void, 5224>();
		}

		BOOL NETWORK_IS_SHOWING_SYSTEM_UI_OR_RECENTLY_REQUESTED_UPSELL() {
			return Invoker::Invoke<BOOL, 5225>();
		}

		BOOL NETWORK_NEED_TO_START_NEW_GAME_BUT_BLOCKED() {
			return Invoker::Invoke<BOOL, 5226>();
		}

		BOOL NETWORK_CAN_BAIL() {
			return Invoker::Invoke<BOOL, 5227>();
		}

		void NETWORK_BAIL(int p0, int p1, int p2) {
			Invoker::Invoke<void, 5228>(p0, p1, p2);
		}

		void NETWORK_ON_RETURN_TO_SINGLE_PLAYER() {
			Invoker::Invoke<void, 5229>();
		}

		BOOL NETWORK_TRANSITION_START(int p0, Any p1, Any p2, Any p3) {
			return Invoker::Invoke<BOOL, 5230>(p0, p1, p2, p3);
		}

		BOOL NETWORK_TRANSITION_ADD_STAGE(Hash hash, int p1, int p2, int state, int p4) {
			return Invoker::Invoke<BOOL, 5231>(hash, p1, p2, state, p4);
		}

		BOOL NETWORK_TRANSITION_FINISH(Any p0, Any p1, Any p2) {
			return Invoker::Invoke<BOOL, 5232>(p0, p1, p2);
		}

		BOOL NETWORK_CAN_ACCESS_MULTIPLAYER(int* loadingState) {
			return Invoker::Invoke<BOOL, 5233>(loadingState);
		}

		BOOL NETWORK_CHECK_CAN_ACCESS_AND_ALERT() {
			return Invoker::Invoke<BOOL, 6661>();
		}

		const char* _NETWORK_GET_ACCESS_CODE_LABEL_HEADING(int accessCode) {
			return Invoker::Invoke<const char*, 6660>(accessCode);
		}

		const char* _NETWORK_GET_ACCESS_CODE_LABEL_BODY(int accessCode) {
			return Invoker::Invoke<const char*, 6659>(accessCode);
		}

		BOOL NETWORK_IS_MULTIPLAYER_DISABLED() {
			return Invoker::Invoke<BOOL, 5234>();
		}

		BOOL NETWORK_CAN_ENTER_MULTIPLAYER() {
			return Invoker::Invoke<BOOL, 5235>();
		}

		BOOL NETWORK_SESSION_DO_FREEROAM_QUICKMATCH(Any p0, Any p1, Any p2) {
			return Invoker::Invoke<BOOL, 5236>(p0, p1, p2);
		}

		BOOL NETWORK_SESSION_DO_FRIEND_MATCHMAKING(int p0, int p1, int p2) {
			return Invoker::Invoke<BOOL, 5237>(p0, p1, p2);
		}

		BOOL NETWORK_SESSION_DO_CREW_MATCHMAKING(int crewId, int p1, int p2, int maxPlayers) {
			return Invoker::Invoke<BOOL, 5238>(crewId, p1, p2, maxPlayers);
		}

		BOOL NETWORK_SESSION_DO_ACTIVITY_QUICKMATCH(Any p0, Any p1, Any p2, Any p3, Any p4) {
			return Invoker::Invoke<BOOL, 5239>(p0, p1, p2, p3, p4);
		}

		BOOL NETWORK_SESSION_HOST(int p0, int maxPlayers, BOOL p2) {
			return Invoker::Invoke<BOOL, 5240>(p0, maxPlayers, p2);
		}

		BOOL NETWORK_SESSION_HOST_CLOSED(int p0, int maxPlayers) {
			return Invoker::Invoke<BOOL, 5241>(p0, maxPlayers);
		}

		BOOL NETWORK_SESSION_HOST_FRIENDS_ONLY(int p0, int maxPlayers) {
			return Invoker::Invoke<BOOL, 5242>(p0, maxPlayers);
		}

		BOOL NETWORK_SESSION_IS_CLOSED_FRIENDS() {
			return Invoker::Invoke<BOOL, 5243>();
		}

		BOOL NETWORK_SESSION_IS_CLOSED_CREW() {
			return Invoker::Invoke<BOOL, 5244>();
		}

		BOOL NETWORK_SESSION_IS_SOLO() {
			return Invoker::Invoke<BOOL, 5245>();
		}

		BOOL NETWORK_SESSION_IS_PRIVATE() {
			return Invoker::Invoke<BOOL, 5246>();
		}

		BOOL _NETWORK_SESSION_LEAVE_INCLUDING_REASON(int leaveFlags, int leaveReason) {
			return Invoker::Invoke<BOOL, 5247>(leaveFlags, leaveReason);
		}

		BOOL NETWORK_SESSION_LEAVE(Any p0) {
			return Invoker::Invoke<BOOL, 5249>(p0);
		}

		void NETWORK_SESSION_KICK_PLAYER(Player player) {
			Invoker::Invoke<void, 5250>(player);
		}

		BOOL NETWORK_SESSION_GET_KICK_VOTE(Player player) {
			return Invoker::Invoke<BOOL, 5251>(player);
		}

		BOOL NETWORK_SESSION_RESERVE_SLOTS_TRANSITION(Any p0, Any p1, Any p2) {
			return Invoker::Invoke<BOOL, 5252>(p0, p1, p2);
		}

		BOOL NETWORK_JOIN_PREVIOUSLY_FAILED_SESSION() {
			return Invoker::Invoke<BOOL, 5253>();
		}

		BOOL NETWORK_JOIN_PREVIOUSLY_FAILED_TRANSITION() {
			return Invoker::Invoke<BOOL, 5254>();
		}

		void NETWORK_SESSION_SET_MATCHMAKING_GROUP(int matchmakingGroup) {
			Invoker::Invoke<void, 5255>(matchmakingGroup);
		}

		void NETWORK_SESSION_SET_MATCHMAKING_GROUP_MAX(int playerType, int playerCount) {
			Invoker::Invoke<void, 5256>(playerType, playerCount);
		}

		int NETWORK_SESSION_GET_MATCHMAKING_GROUP_FREE(int p0) {
			return Invoker::Invoke<int, 5257>(p0);
		}

		void NETWORK_SESSION_ADD_ACTIVE_MATCHMAKING_GROUP(int groupId) {
			Invoker::Invoke<void, 5258>(groupId);
		}

		void NETWORK_SESSION_SET_UNIQUE_CREW_LIMIT(Any p0) {
			Invoker::Invoke<void, 5259>(p0);
		}

		int NETWORK_SESSION_GET_UNIQUE_CREW_LIMIT() {
			return Invoker::Invoke<int, 5260>();
		}

		void NETWORK_SESSION_SET_UNIQUE_CREW_LIMIT_TRANSITION(Any p0) {
			Invoker::Invoke<void, 5261>(p0);
		}

		void NETWORK_SESSION_SET_UNIQUE_CREW_ONLY_CREWS_TRANSITION(BOOL p0) {
			Invoker::Invoke<void, 5262>(p0);
		}

		void NETWORK_SESSION_SET_CREW_LIMIT_MAX_MEMBERS_TRANSITION(Any p0) {
			Invoker::Invoke<void, 5263>(p0);
		}

		void NETWORK_SESSION_SET_MATCHMAKING_PROPERTY_ID(BOOL p0) {
			Invoker::Invoke<void, 5264>(p0);
		}

		void NETWORK_SESSION_SET_MATCHMAKING_MENTAL_STATE(int p0) {
			Invoker::Invoke<void, 5265>(p0);
		}

		void NETWORK_SESSION_SET_NUM_BOSSES(int num) {
			Invoker::Invoke<void, 5266>(num);
		}

		void NETWORK_SESSION_SET_SCRIPT_VALIDATE_JOIN() {
			Invoker::Invoke<void, 5267>();
		}

		void NETWORK_SESSION_VALIDATE_JOIN(BOOL p0) {
			Invoker::Invoke<void, 5268>(p0);
		}

		void NETWORK_ADD_FOLLOWERS(int* p0, int p1) {
			Invoker::Invoke<void, 5269>(p0, p1);
		}

		void NETWORK_CLEAR_FOLLOWERS() {
			Invoker::Invoke<void, 5270>();
		}

		void NETWORK_GET_GLOBAL_MULTIPLAYER_CLOCK(int* hours, int* minutes, int* seconds) {
			Invoker::Invoke<void, 5271>(hours, minutes, seconds);
		}

		void NETWORK_SESSION_SET_GAMEMODE(Any p0) {
			Invoker::Invoke<void, 5272>(p0);
		}

		int NETWORK_SESSION_GET_HOST_AIM_PREFERENCE() {
			return Invoker::Invoke<int, 5273>();
		}

		BOOL NETWORK_FIND_GAMERS_IN_CREW(int crewId) {
			return Invoker::Invoke<BOOL, 5274>(crewId);
		}

		BOOL NETWORK_FIND_MATCHED_GAMERS(int attribute, float fallbackLimit, float lowerLimit, float upperLimit) {
			return Invoker::Invoke<BOOL, 5275>(attribute, fallbackLimit, lowerLimit, upperLimit);
		}

		BOOL NETWORK_IS_FINDING_GAMERS() {
			return Invoker::Invoke<BOOL, 5276>();
		}

		BOOL NETWORK_DID_FIND_GAMERS_SUCCEED() {
			return Invoker::Invoke<BOOL, 5277>();
		}

		int NETWORK_GET_NUM_FOUND_GAMERS() {
			return Invoker::Invoke<int, 5278>();
		}

		BOOL NETWORK_GET_FOUND_GAMER(Any* p0, Any p1) {
			return Invoker::Invoke<BOOL, 5279>(p0, p1);
		}

		void NETWORK_CLEAR_FOUND_GAMERS() {
			Invoker::Invoke<void, 5280>();
		}

		BOOL NETWORK_QUEUE_GAMER_FOR_STATUS(Any* p0) {
			return Invoker::Invoke<BOOL, 5281>(p0);
		}

		BOOL NETWORK_GET_GAMER_STATUS_FROM_QUEUE() {
			return Invoker::Invoke<BOOL, 5282>();
		}

		BOOL NETWORK_IS_GETTING_GAMER_STATUS() {
			return Invoker::Invoke<BOOL, 5283>();
		}

		BOOL NETWORK_DID_GET_GAMER_STATUS_SUCCEED() {
			return Invoker::Invoke<BOOL, 5284>();
		}

		BOOL NETWORK_GET_GAMER_STATUS_RESULT(Any* p0, Any p1) {
			return Invoker::Invoke<BOOL, 5285>(p0, p1);
		}

		void NETWORK_CLEAR_GET_GAMER_STATUS() {
			Invoker::Invoke<void, 5286>();
		}

		void NETWORK_SESSION_JOIN_INVITE() {
			Invoker::Invoke<void, 5287>();
		}

		void NETWORK_SESSION_CANCEL_INVITE() {
			Invoker::Invoke<void, 5288>();
		}

		void NETWORK_SESSION_FORCE_CANCEL_INVITE() {
			Invoker::Invoke<void, 5289>();
		}

		BOOL NETWORK_HAS_PENDING_INVITE() {
			return Invoker::Invoke<BOOL, 5290>();
		}

		BOOL NETWORK_HAS_CONFIRMED_INVITE() {
			return Invoker::Invoke<BOOL, 5291>();
		}

		BOOL NETWORK_REQUEST_INVITE_CONFIRMED_EVENT() {
			return Invoker::Invoke<BOOL, 5292>();
		}

		BOOL NETWORK_SESSION_WAS_INVITED() {
			return Invoker::Invoke<BOOL, 5293>();
		}

		void NETWORK_SESSION_GET_INVITER(Any* gamerHandle) {
			Invoker::Invoke<void, 5294>(gamerHandle);
		}

		BOOL NETWORK_SESSION_IS_AWAITING_INVITE_RESPONSE() {
			return Invoker::Invoke<BOOL, 5295>();
		}

		BOOL NETWORK_SESSION_IS_DISPLAYING_INVITE_CONFIRMATION() {
			return Invoker::Invoke<BOOL, 5296>();
		}

		void NETWORK_SUPPRESS_INVITE(BOOL toggle) {
			Invoker::Invoke<void, 5297>(toggle);
		}

		void NETWORK_BLOCK_INVITES(BOOL toggle) {
			Invoker::Invoke<void, 5298>(toggle);
		}

		void NETWORK_BLOCK_JOIN_QUEUE_INVITES(BOOL toggle) {
			Invoker::Invoke<void, 5299>(toggle);
		}

		void NETWORK_SET_CAN_RECEIVE_RS_INVITES(BOOL p0) {
			Invoker::Invoke<void, 5300>(p0);
		}

		void NETWORK_STORE_INVITE_THROUGH_RESTART() {
			Invoker::Invoke<void, 5301>();
		}

		void NETWORK_ALLOW_INVITE_PROCESS_IN_PLAYER_SWITCH(BOOL p0) {
			Invoker::Invoke<void, 5302>(p0);
		}

		void NETWORK_SET_SCRIPT_READY_FOR_EVENTS(BOOL toggle) {
			Invoker::Invoke<void, 5303>(toggle);
		}

		BOOL NETWORK_IS_OFFLINE_INVITE_PENDING() {
			return Invoker::Invoke<BOOL, 5304>();
		}

		void NETWORK_CLEAR_OFFLINE_INVITE_PENDING() {
			Invoker::Invoke<void, 5305>();
		}

		const char* _NETWORK_INVITE_GET_JOIN_FAIL_REASON() {
			return Invoker::Invoke<const char*, 6658>();
		}

		void _NETWORK_INVITE_CLEAR_JOIN_FAIL_REASON() {
			Invoker::Invoke<void, 6657>();
		}

		void NETWORK_SESSION_HOST_SINGLE_PLAYER(int p0) {
			Invoker::Invoke<void, 5306>(p0);
		}

		void NETWORK_SESSION_LEAVE_SINGLE_PLAYER() {
			Invoker::Invoke<void, 5307>();
		}

		BOOL NETWORK_IS_GAME_IN_PROGRESS() {
			return Invoker::Invoke<BOOL, 5308>();
		}

		BOOL NETWORK_IS_SESSION_ACTIVE() {
			return Invoker::Invoke<BOOL, 5309>();
		}

		BOOL NETWORK_IS_IN_SESSION() {
			return Invoker::Invoke<BOOL, 5310>();
		}

		BOOL _NETWORK_IS_AMERICAS_VERSION() {
			return Invoker::Invoke<BOOL, 5311>();
		}

		BOOL NETWORK_IS_SESSION_STARTED() {
			return Invoker::Invoke<BOOL, 5312>();
		}

		BOOL NETWORK_IS_SESSION_BUSY() {
			return Invoker::Invoke<BOOL, 5313>();
		}

		BOOL NETWORK_CAN_SESSION_END() {
			return Invoker::Invoke<BOOL, 5314>();
		}

		int NETWORK_GET_GAME_MODE() {
			return Invoker::Invoke<int, 5315>();
		}

		void NETWORK_SESSION_MARK_VISIBLE(BOOL toggle) {
			Invoker::Invoke<void, 5316>(toggle);
		}

		BOOL NETWORK_SESSION_IS_VISIBLE() {
			return Invoker::Invoke<BOOL, 5317>();
		}

		void NETWORK_SESSION_BLOCK_JOIN_REQUESTS(BOOL toggle) {
			Invoker::Invoke<void, 5318>(toggle);
		}

		void NETWORK_SESSION_CHANGE_SLOTS(int slots, BOOL p1) {
			Invoker::Invoke<void, 5319>(slots, p1);
		}

		int NETWORK_SESSION_GET_PRIVATE_SLOTS() {
			return Invoker::Invoke<int, 5320>();
		}

		BOOL NETWORK_SESSION_VOICE_HOST() {
			return Invoker::Invoke<BOOL, 5321>();
		}

		BOOL NETWORK_SESSION_VOICE_LEAVE() {
			return Invoker::Invoke<BOOL, 5322>();
		}

		void NETWORK_SESSION_VOICE_CONNECT_TO_PLAYER(Any* gamerHandle) {
			Invoker::Invoke<void, 5323>(gamerHandle);
		}

		void NETWORK_SESSION_VOICE_RESPOND_TO_REQUEST(BOOL p0, int p1) {
			Invoker::Invoke<void, 5324>(p0, p1);
		}

		void NETWORK_SESSION_VOICE_SET_TIMEOUT(int timeout) {
			Invoker::Invoke<void, 5325>(timeout);
		}

		BOOL NETWORK_SESSION_IS_IN_VOICE_SESSION() {
			return Invoker::Invoke<BOOL, 5326>();
		}

		BOOL NETWORK_SESSION_IS_VOICE_SESSION_ACTIVE() {
			return Invoker::Invoke<BOOL, 5327>();
		}

		BOOL NETWORK_SESSION_IS_VOICE_SESSION_BUSY() {
			return Invoker::Invoke<BOOL, 5328>();
		}

		BOOL NETWORK_SEND_TEXT_MESSAGE(const char* message, Any* gamerHandle) {
			return Invoker::Invoke<BOOL, 5329>(message, gamerHandle);
		}

		void NETWORK_SET_ACTIVITY_SPECTATOR(BOOL toggle) {
			Invoker::Invoke<void, 5330>(toggle);
		}

		BOOL NETWORK_IS_ACTIVITY_SPECTATOR() {
			return Invoker::Invoke<BOOL, 5331>();
		}

		void NETWORK_SET_ACTIVITY_PLAYER_MAX(Any p0) {
			Invoker::Invoke<void, 5332>(p0);
		}

		void NETWORK_SET_ACTIVITY_SPECTATOR_MAX(int maxSpectators) {
			Invoker::Invoke<void, 5333>(maxSpectators);
		}

		int NETWORK_GET_ACTIVITY_PLAYER_NUM(BOOL p0) {
			return Invoker::Invoke<int, 5334>(p0);
		}

		BOOL NETWORK_IS_ACTIVITY_SPECTATOR_FROM_HANDLE(Any* gamerHandle) {
			return Invoker::Invoke<BOOL, 5335>(gamerHandle);
		}

		BOOL NETWORK_HOST_TRANSITION(int p0, int p1, int p2, int p3, Any p4, BOOL p5, BOOL p6, int p7, Any p8, int p9) {
			return Invoker::Invoke<BOOL, 5336>(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9);
		}

		BOOL NETWORK_DO_TRANSITION_QUICKMATCH(Any p0, Any p1, Any p2, Any p3, Any p4, Any p5) {
			return Invoker::Invoke<BOOL, 5337>(p0, p1, p2, p3, p4, p5);
		}

		BOOL NETWORK_DO_TRANSITION_QUICKMATCH_ASYNC(Any p0, Any p1, Any p2, Any p3, Any p4, Any p5) {
			return Invoker::Invoke<BOOL, 5338>(p0, p1, p2, p3, p4, p5);
		}

		BOOL NETWORK_DO_TRANSITION_QUICKMATCH_WITH_GROUP(Any p0, Any p1, Any p2, Any p3, Any* p4, Any p5, Any p6, Any p7) {
			return Invoker::Invoke<BOOL, 5339>(p0, p1, p2, p3, p4, p5, p6, p7);
		}

		BOOL NETWORK_JOIN_GROUP_ACTIVITY() {
			return Invoker::Invoke<BOOL, 5340>();
		}

		void NETWORK_CLEAR_GROUP_ACTIVITY() {
			Invoker::Invoke<void, 5341>();
		}

		void NETWORK_RETAIN_ACTIVITY_GROUP() {
			Invoker::Invoke<void, 5342>();
		}

		BOOL NETWORK_IS_TRANSITION_CLOSED_FRIENDS() {
			return Invoker::Invoke<BOOL, 5343>();
		}

		BOOL NETWORK_IS_TRANSITION_CLOSED_CREW() {
			return Invoker::Invoke<BOOL, 5344>();
		}

		BOOL NETWORK_IS_TRANSITION_SOLO() {
			return Invoker::Invoke<BOOL, 5345>();
		}

		BOOL NETWORK_IS_TRANSITION_PRIVATE() {
			return Invoker::Invoke<BOOL, 5346>();
		}

		int NETWORK_GET_NUM_TRANSITION_NON_ASYNC_GAMERS() {
			return Invoker::Invoke<int, 5347>();
		}

		void NETWORK_MARK_AS_PREFERRED_ACTIVITY(BOOL p0) {
			Invoker::Invoke<void, 5348>(p0);
		}

		void NETWORK_MARK_AS_WAITING_ASYNC(BOOL p0) {
			Invoker::Invoke<void, 5349>(p0);
		}

		void NETWORK_SET_IN_PROGRESS_FINISH_TIME(Any p0) {
			Invoker::Invoke<void, 5350>(p0);
		}

		void NETWORK_SET_TRANSITION_CREATOR_HANDLE(Any* p0) {
			Invoker::Invoke<void, 5351>(p0);
		}

		void NETWORK_CLEAR_TRANSITION_CREATOR_HANDLE() {
			Invoker::Invoke<void, 5352>();
		}

		BOOL NETWORK_INVITE_GAMERS_TO_TRANSITION(Any* p0, Any p1) {
			return Invoker::Invoke<BOOL, 5353>(p0, p1);
		}

		void NETWORK_SET_GAMER_INVITED_TO_TRANSITION(Any* gamerHandle) {
			Invoker::Invoke<void, 5354>(gamerHandle);
		}

		BOOL NETWORK_LEAVE_TRANSITION() {
			return Invoker::Invoke<BOOL, 5355>();
		}

		BOOL NETWORK_LAUNCH_TRANSITION() {
			return Invoker::Invoke<BOOL, 5356>();
		}

		void NETWORK_SET_DO_NOT_LAUNCH_FROM_JOIN_AS_MIGRATED_HOST(BOOL toggle) {
			Invoker::Invoke<void, 5357>(toggle);
		}

		void NETWORK_CANCEL_TRANSITION_MATCHMAKING() {
			Invoker::Invoke<void, 5358>();
		}

		void NETWORK_BAIL_TRANSITION(int p0, int p1, int p2) {
			Invoker::Invoke<void, 5359>(p0, p1, p2);
		}

		BOOL NETWORK_DO_TRANSITION_TO_GAME(BOOL p0, int maxPlayers) {
			return Invoker::Invoke<BOOL, 5360>(p0, maxPlayers);
		}

		BOOL NETWORK_DO_TRANSITION_TO_NEW_GAME(BOOL p0, int maxPlayers, BOOL p2) {
			return Invoker::Invoke<BOOL, 5361>(p0, maxPlayers, p2);
		}

		BOOL NETWORK_DO_TRANSITION_TO_FREEMODE(Any* p0, Any p1, BOOL p2, int players, BOOL p4) {
			return Invoker::Invoke<BOOL, 5362>(p0, p1, p2, players, p4);
		}

		BOOL NETWORK_DO_TRANSITION_TO_NEW_FREEMODE(Any* p0, Any p1, int players, BOOL p3, BOOL p4, BOOL p5) {
			return Invoker::Invoke<BOOL, 5363>(p0, p1, players, p3, p4, p5);
		}

		BOOL NETWORK_IS_TRANSITION_TO_GAME() {
			return Invoker::Invoke<BOOL, 5364>();
		}

		int NETWORK_GET_TRANSITION_MEMBERS(Any* data, int dataCount) {
			return Invoker::Invoke<int, 5365>(data, dataCount);
		}

		void NETWORK_APPLY_TRANSITION_PARAMETER(int p0, int p1) {
			Invoker::Invoke<void, 5366>(p0, p1);
		}

		void NETWORK_APPLY_TRANSITION_PARAMETER_STRING(int p0, const char* string, BOOL p2) {
			Invoker::Invoke<void, 5367>(p0, string, p2);
		}

		BOOL NETWORK_SEND_TRANSITION_GAMER_INSTRUCTION(Any* gamerHandle, const char* p1, int p2, int p3, BOOL p4) {
			return Invoker::Invoke<BOOL, 5368>(gamerHandle, p1, p2, p3, p4);
		}

		BOOL NETWORK_MARK_TRANSITION_GAMER_AS_FULLY_JOINED(Any* p0) {
			return Invoker::Invoke<BOOL, 5369>(p0);
		}

		BOOL NETWORK_IS_TRANSITION_HOST() {
			return Invoker::Invoke<BOOL, 5370>();
		}

		BOOL NETWORK_IS_TRANSITION_HOST_FROM_HANDLE(Any* gamerHandle) {
			return Invoker::Invoke<BOOL, 5371>(gamerHandle);
		}

		BOOL NETWORK_GET_TRANSITION_HOST(Any* gamerHandle) {
			return Invoker::Invoke<BOOL, 5372>(gamerHandle);
		}

		BOOL NETWORK_IS_IN_TRANSITION() {
			return Invoker::Invoke<BOOL, 5373>();
		}

		BOOL NETWORK_IS_TRANSITION_STARTED() {
			return Invoker::Invoke<BOOL, 5374>();
		}

		BOOL NETWORK_IS_TRANSITION_BUSY() {
			return Invoker::Invoke<BOOL, 5375>();
		}

		BOOL NETWORK_IS_TRANSITION_MATCHMAKING() {
			return Invoker::Invoke<BOOL, 5376>();
		}

		BOOL NETWORK_IS_TRANSITION_LEAVE_POSTPONED() {
			return Invoker::Invoke<BOOL, 5377>();
		}

		void NETWORK_TRANSITION_SET_IN_PROGRESS(Any p0) {
			Invoker::Invoke<void, 5378>(p0);
		}

		void NETWORK_TRANSITION_SET_CONTENT_CREATOR(Any p0) {
			Invoker::Invoke<void, 5379>(p0);
		}

		void NETWORK_TRANSITION_SET_ACTIVITY_ISLAND(Any p0) {
			Invoker::Invoke<void, 5380>(p0);
		}

		void NETWORK_OPEN_TRANSITION_MATCHMAKING() {
			Invoker::Invoke<void, 5381>();
		}

		void NETWORK_CLOSE_TRANSITION_MATCHMAKING() {
			Invoker::Invoke<void, 5382>();
		}

		BOOL NETWORK_IS_TRANSITION_OPEN_TO_MATCHMAKING() {
			return Invoker::Invoke<BOOL, 5383>();
		}

		void NETWORK_SET_TRANSITION_VISIBILITY_LOCK(BOOL p0, BOOL p1) {
			Invoker::Invoke<void, 5384>(p0, p1);
		}

		BOOL NETWORK_IS_TRANSITION_VISIBILITY_LOCKED() {
			return Invoker::Invoke<BOOL, 5385>();
		}

		void NETWORK_SET_TRANSITION_ACTIVITY_ID(Any p0) {
			Invoker::Invoke<void, 5386>(p0);
		}

		void NETWORK_CHANGE_TRANSITION_SLOTS(Any p0, BOOL p1) {
			Invoker::Invoke<void, 5387>(p0, p1);
		}

		void NETWORK_TRANSITION_BLOCK_JOIN_REQUESTS(BOOL p0) {
			Invoker::Invoke<void, 5388>(p0);
		}

		BOOL NETWORK_HAS_PLAYER_STARTED_TRANSITION(Player player) {
			return Invoker::Invoke<BOOL, 5389>(player);
		}

		BOOL NETWORK_ARE_TRANSITION_DETAILS_VALID(Any p0) {
			return Invoker::Invoke<BOOL, 5390>(p0);
		}

		BOOL NETWORK_JOIN_TRANSITION(Player player) {
			return Invoker::Invoke<BOOL, 5391>(player);
		}

		BOOL NETWORK_HAS_INVITED_GAMER_TO_TRANSITION(Any* p0) {
			return Invoker::Invoke<BOOL, 5392>(p0);
		}

		BOOL NETWORK_HAS_TRANSITION_INVITE_BEEN_ACKED(Any* p0) {
			return Invoker::Invoke<BOOL, 5393>(p0);
		}

		BOOL NETWORK_IS_ACTIVITY_SESSION() {
			return Invoker::Invoke<BOOL, 5394>();
		}

		void NETWORK_DISABLE_REALTIME_MULTIPLAYER() {
			Invoker::Invoke<void, 5395>();
		}

		void NETWORK_OVERRIDE_REALTIME_MULTIPLAYER_CONTROL_DISABLE() {
			Invoker::Invoke<void, 6656>();
		}

		void NETWORK_DISABLE_REALTIME_MULTIPLAYER_SPECTATOR() {
			Invoker::Invoke<void, 6655>();
		}

		void NETWORK_SET_PRESENCE_SESSION_INVITES_BLOCKED(BOOL toggle) {
			Invoker::Invoke<void, 5396>(toggle);
		}

		BOOL NETWORK_SEND_INVITE_VIA_PRESENCE(Any* gamerHandle, const char* p1, int dataCount, int p3) {
			return Invoker::Invoke<BOOL, 5397>(gamerHandle, p1, dataCount, p3);
		}

		BOOL NETWORK_SEND_TRANSITION_INVITE_VIA_PRESENCE(Any* gamerHandle, const char* p1, int dataCount, int p3) {
			return Invoker::Invoke<BOOL, 5398>(gamerHandle, p1, dataCount, p3);
		}

		BOOL NETWORK_SEND_IMPORTANT_TRANSITION_INVITE_VIA_PRESENCE(Any* gamerHandle, const char* p1, int dataCount, int p3) {
			return Invoker::Invoke<BOOL, 5399>(gamerHandle, p1, dataCount, p3);
		}

		int NETWORK_GET_PRESENCE_INVITE_INDEX_BY_ID(int p0) {
			return Invoker::Invoke<int, 5400>(p0);
		}

		int NETWORK_GET_NUM_PRESENCE_INVITES() {
			return Invoker::Invoke<int, 5401>();
		}

		BOOL NETWORK_ACCEPT_PRESENCE_INVITE(int p0) {
			return Invoker::Invoke<BOOL, 5402>(p0);
		}

		BOOL NETWORK_REMOVE_PRESENCE_INVITE(int p0) {
			return Invoker::Invoke<BOOL, 5403>(p0);
		}

		int NETWORK_GET_PRESENCE_INVITE_ID(int p0) {
			return Invoker::Invoke<int, 5404>(p0);
		}

		const char* NETWORK_GET_PRESENCE_INVITE_INVITER(int p0) {
			return Invoker::Invoke<const char*, 5405>(p0);
		}

		BOOL NETWORK_GET_PRESENCE_INVITE_HANDLE(Any p0, Any* p1) {
			return Invoker::Invoke<BOOL, 5406>(p0, p1);
		}

		int NETWORK_GET_PRESENCE_INVITE_SESSION_ID(Any p0) {
			return Invoker::Invoke<int, 5407>(p0);
		}

		const char* NETWORK_GET_PRESENCE_INVITE_CONTENT_ID(int p0) {
			return Invoker::Invoke<const char*, 5408>(p0);
		}

		int NETWORK_GET_PRESENCE_INVITE_PLAYLIST_LENGTH(int p0) {
			return Invoker::Invoke<int, 5409>(p0);
		}

		int NETWORK_GET_PRESENCE_INVITE_PLAYLIST_CURRENT(int p0) {
			return Invoker::Invoke<int, 5410>(p0);
		}

		BOOL NETWORK_GET_PRESENCE_INVITE_FROM_ADMIN(int p0) {
			return Invoker::Invoke<BOOL, 5411>(p0);
		}

		BOOL NETWORK_GET_PRESENCE_INVITE_IS_TOURNAMENT(Any p0) {
			return Invoker::Invoke<BOOL, 5412>(p0);
		}

		BOOL NETWORK_HAS_FOLLOW_INVITE() {
			return Invoker::Invoke<BOOL, 5413>();
		}

		BOOL NETWORK_ACTION_FOLLOW_INVITE() {
			return Invoker::Invoke<BOOL, 5414>();
		}

		BOOL NETWORK_CLEAR_FOLLOW_INVITE() {
			return Invoker::Invoke<BOOL, 5415>();
		}

		void NETWORK_REMOVE_AND_CANCEL_ALL_INVITES() {
			Invoker::Invoke<void, 5416>();
		}

		void NETWORK_REMOVE_TRANSITION_INVITE(Any* p0) {
			Invoker::Invoke<void, 5417>(p0);
		}

		void NETWORK_REMOVE_ALL_TRANSITION_INVITE() {
			Invoker::Invoke<void, 5418>();
		}

		void NETWORK_REMOVE_AND_CANCEL_ALL_TRANSITION_INVITES() {
			Invoker::Invoke<void, 5419>();
		}

		BOOL NETWORK_INVITE_GAMERS(Any* p0, Any p1, Any* p2, Any p3) {
			return Invoker::Invoke<BOOL, 5420>(p0, p1, p2, p3);
		}

		BOOL NETWORK_HAS_INVITED_GAMER(Any* p0) {
			return Invoker::Invoke<BOOL, 5421>(p0);
		}

		BOOL NETWORK_HAS_MADE_INVITE_DECISION(Any* gamerHandle) {
			return Invoker::Invoke<BOOL, 5422>(gamerHandle);
		}

		int NETWORK_GET_INVITE_REPLY_STATUS(Any p0) {
			return Invoker::Invoke<int, 5423>(p0);
		}

		BOOL NETWORK_GET_CURRENTLY_SELECTED_GAMER_HANDLE_FROM_INVITE_MENU(Any* p0) {
			return Invoker::Invoke<BOOL, 5424>(p0);
		}

		BOOL NETWORK_SET_CURRENTLY_SELECTED_GAMER_HANDLE_FROM_INVITE_MENU(Any* p0) {
			return Invoker::Invoke<BOOL, 5425>(p0);
		}

		void NETWORK_SET_INVITE_ON_CALL_FOR_INVITE_MENU(Any* p0) {
			Invoker::Invoke<void, 5426>(p0);
		}

		BOOL NETWORK_CHECK_DATA_MANAGER_SUCCEEDED_FOR_HANDLE(int p0, Any* gamerHandle) {
			return Invoker::Invoke<BOOL, 5427>(p0, gamerHandle);
		}

		BOOL NETWORK_CHECK_DATA_MANAGER_FOR_HANDLE(Any p0, Any* gamerHandle) {
			return Invoker::Invoke<BOOL, 5428>(p0, gamerHandle);
		}

		void NETWORK_SET_INVITE_FAILED_MESSAGE_FOR_INVITE_MENU(Any* p0, Any* p1) {
			Invoker::Invoke<void, 5429>(p0, p1);
		}

		BOOL FILLOUT_PM_PLAYER_LIST(Any* gamerHandle, Any p1, Any p2) {
			return Invoker::Invoke<BOOL, 5430>(gamerHandle, p1, p2);
		}

		BOOL FILLOUT_PM_PLAYER_LIST_WITH_NAMES(Any* p0, Any* p1, Any p2, Any p3) {
			return Invoker::Invoke<BOOL, 5431>(p0, p1, p2, p3);
		}

		BOOL REFRESH_PLAYER_LIST_STATS(int p0) {
			return Invoker::Invoke<BOOL, 5432>(p0);
		}

		BOOL NETWORK_SET_CURRENT_DATA_MANAGER_HANDLE(Any* p0) {
			return Invoker::Invoke<BOOL, 5433>(p0);
		}

		BOOL NETWORK_IS_IN_PLATFORM_PARTY() {
			return Invoker::Invoke<BOOL, 5434>();
		}

		int NETWORK_GET_PLATFORM_PARTY_MEMBER_COUNT() {
			return Invoker::Invoke<int, 5435>();
		}

		int NETWORK_GET_PLATFORM_PARTY_MEMBERS(Any* data, int dataSize) {
			return Invoker::Invoke<int, 5436>(data, dataSize);
		}

		BOOL NETWORK_IS_IN_PLATFORM_PARTY_CHAT() {
			return Invoker::Invoke<BOOL, 5437>();
		}

		BOOL NETWORK_IS_CHATTING_IN_PLATFORM_PARTY(Any* gamerHandle) {
			return Invoker::Invoke<BOOL, 5438>(gamerHandle);
		}

		BOOL NETWORK_CAN_QUEUE_FOR_PREVIOUS_SESSION_JOIN() {
			return Invoker::Invoke<BOOL, 5439>();
		}

		BOOL NETWORK_IS_QUEUING_FOR_SESSION_JOIN() {
			return Invoker::Invoke<BOOL, 5440>();
		}

		void NETWORK_CLEAR_QUEUED_JOIN_REQUEST() {
			Invoker::Invoke<void, 5441>();
		}

		void NETWORK_SEND_QUEUED_JOIN_REQUEST() {
			Invoker::Invoke<void, 5442>();
		}

		void NETWORK_REMOVE_ALL_QUEUED_JOIN_REQUESTS() {
			Invoker::Invoke<void, 5443>();
		}

		void NETWORK_SEED_RANDOM_NUMBER_GENERATOR(int seed) {
			Invoker::Invoke<void, 5444>(seed);
		}

		int NETWORK_GET_RANDOM_INT() {
			return Invoker::Invoke<int, 5445>();
		}

		int NETWORK_GET_RANDOM_INT_RANGED(int rangeStart, int rangeEnd) {
			return Invoker::Invoke<int, 5446>(rangeStart, rangeEnd);
		}

		float _NETWORK_GET_RANDOM_FLOAT_RANGED(float rangeStart, float rangeEnd) {
			return Invoker::Invoke<float, 5447>(rangeStart, rangeEnd);
		}

		BOOL NETWORK_PLAYER_IS_CHEATER() {
			return Invoker::Invoke<BOOL, 5448>();
		}

		int NETWORK_PLAYER_GET_CHEATER_REASON() {
			return Invoker::Invoke<int, 5449>();
		}

		BOOL NETWORK_PLAYER_IS_BADSPORT() {
			return Invoker::Invoke<BOOL, 5450>();
		}

		BOOL REMOTE_CHEATER_PLAYER_DETECTED(Player player, int a, int b) {
			return Invoker::Invoke<BOOL, 5451>(player, a, b);
		}

		BOOL BAD_SPORT_PLAYER_LEFT_DETECTED(Any* gamerHandle, int event, int amountReceived) {
			return Invoker::Invoke<BOOL, 5452>(gamerHandle, event, amountReceived);
		}

		void NETWORK_ADD_INVALID_OBJECT_MODEL(Hash modelHash, Any p1) {
			Invoker::Invoke<void, 5453>(modelHash, p1);
		}

		void NETWORK_REMOVE_INVALID_OBJECT_MODEL(Hash modelHash) {
			Invoker::Invoke<void, 5454>(modelHash);
		}

		void NETWORK_CLEAR_INVALID_OBJECT_MODELS() {
			Invoker::Invoke<void, 5455>();
		}

		void NETWORK_APPLY_PED_SCAR_DATA(Ped ped, int p1) {
			Invoker::Invoke<void, 5456>(ped, p1);
		}

		void NETWORK_SET_THIS_SCRIPT_IS_NETWORK_SCRIPT(int maxNumMissionParticipants, BOOL p1, int instanceId) {
			Invoker::Invoke<void, 5457>(maxNumMissionParticipants, p1, instanceId);
		}

		BOOL NETWORK_TRY_TO_SET_THIS_SCRIPT_IS_NETWORK_SCRIPT(Any p0, BOOL p1, Any p2) {
			return Invoker::Invoke<BOOL, 5458>(p0, p1, p2);
		}

		BOOL NETWORK_GET_THIS_SCRIPT_IS_NETWORK_SCRIPT() {
			return Invoker::Invoke<BOOL, 5459>();
		}

		int NETWORK_GET_MAX_NUM_PARTICIPANTS() {
			return Invoker::Invoke<int, 5460>();
		}

		int NETWORK_GET_NUM_PARTICIPANTS() {
			return Invoker::Invoke<int, 5461>();
		}

		int NETWORK_GET_SCRIPT_STATUS() {
			return Invoker::Invoke<int, 5462>();
		}

		void NETWORK_REGISTER_HOST_BROADCAST_VARIABLES(int* vars, int numVars, const char* debugName) {
			Invoker::Invoke<void, 5463>(vars, numVars, debugName);
		}

		void NETWORK_REGISTER_PLAYER_BROADCAST_VARIABLES(int* vars, int numVars, const char* debugName) {
			Invoker::Invoke<void, 5464>(vars, numVars, debugName);
		}

		void NETWORK_REGISTER_HIGH_FREQUENCY_HOST_BROADCAST_VARIABLES(Any p0, Any p1, Any p2) {
			Invoker::Invoke<void, 5465>(p0, p1, p2);
		}

		void NETWORK_REGISTER_HIGH_FREQUENCY_PLAYER_BROADCAST_VARIABLES(Any p0, Any p1, Any p2) {
			Invoker::Invoke<void, 5466>(p0, p1, p2);
		}

		void NETWORK_FINISH_BROADCASTING_DATA() {
			Invoker::Invoke<void, 5467>();
		}

		int _NETWORK_GET_HOST_BROADCAST_DATA_SIZE_UNSYNCED(Hash scriptNameHash, int instance, Hash positionHash, int handlerNum) {
			return Invoker::Invoke<int, 6654>(scriptNameHash, instance, positionHash, handlerNum);
		}

		int _NETWORK_GET_PLAYER_BROADCAST_DATA_SIZE_UNSYNCED(Hash scriptNameHash, int instance, Hash positionHash, int handlerNum) {
			return Invoker::Invoke<int, 6653>(scriptNameHash, instance, positionHash, handlerNum);
		}

		int _NETWORK_GET_BROADCAST_DATA_HOST_UPDATE_SIZE(Hash scriptNameHash, int instance, Hash positionHash, int handlerNum) {
			return Invoker::Invoke<int, 6652>(scriptNameHash, instance, positionHash, handlerNum);
		}

		int _NETWORK_GET_BROADCAST_DATA_PLAYER_UPDATE_SIZE(Hash scriptNameHash, int instance, Hash positionHash, int handlerNum) {
			return Invoker::Invoke<int, 6651>(scriptNameHash, instance, positionHash, handlerNum);
		}

		BOOL NETWORK_HAS_RECEIVED_HOST_BROADCAST_DATA() {
			return Invoker::Invoke<BOOL, 5468>();
		}

		Player NETWORK_GET_PLAYER_INDEX(Player player) {
			return Invoker::Invoke<Player, 5469>(player);
		}

		int NETWORK_GET_PARTICIPANT_INDEX(int index) {
			return Invoker::Invoke<int, 5470>(index);
		}

		Player NETWORK_GET_PLAYER_INDEX_FROM_PED(Ped ped) {
			return Invoker::Invoke<Player, 5471>(ped);
		}

		int NETWORK_GET_NUM_CONNECTED_PLAYERS() {
			return Invoker::Invoke<int, 5472>();
		}

		BOOL NETWORK_IS_PLAYER_CONNECTED(Player player) {
			return Invoker::Invoke<BOOL, 5473>(player);
		}

		int NETWORK_GET_TOTAL_NUM_PLAYERS() {
			return Invoker::Invoke<int, 5474>();
		}

		BOOL NETWORK_IS_PARTICIPANT_ACTIVE(int p0) {
			return Invoker::Invoke<BOOL, 5475>(p0);
		}

		BOOL NETWORK_IS_PLAYER_ACTIVE(Player player) {
			return Invoker::Invoke<BOOL, 5476>(player);
		}

		BOOL NETWORK_IS_PLAYER_A_PARTICIPANT(Player player) {
			return Invoker::Invoke<BOOL, 5477>(player);
		}

		BOOL NETWORK_IS_HOST_OF_THIS_SCRIPT() {
			return Invoker::Invoke<BOOL, 5478>();
		}

		Player NETWORK_GET_HOST_OF_THIS_SCRIPT() {
			return Invoker::Invoke<Player, 5479>();
		}

		Player NETWORK_GET_HOST_OF_SCRIPT(const char* scriptName, int instance_id, int position_hash) {
			return Invoker::Invoke<Player, 5480>(scriptName, instance_id, position_hash);
		}

		Player NETWORK_GET_HOST_OF_THREAD(int threadId) {
			return Invoker::Invoke<Player, 6650>(threadId);
		}

		void NETWORK_SET_MISSION_FINISHED() {
			Invoker::Invoke<void, 5481>();
		}

		BOOL NETWORK_IS_SCRIPT_ACTIVE(const char* scriptName, int instance_id, BOOL p2, int position_hash) {
			return Invoker::Invoke<BOOL, 5482>(scriptName, instance_id, p2, position_hash);
		}

		BOOL NETWORK_IS_SCRIPT_ACTIVE_BY_HASH(Hash scriptHash, int p1, BOOL p2, int p3) {
			return Invoker::Invoke<BOOL, 5483>(scriptHash, p1, p2, p3);
		}

		BOOL NETWORK_IS_THREAD_A_NETWORK_SCRIPT(int threadId) {
			return Invoker::Invoke<BOOL, 5484>(threadId);
		}

		int NETWORK_GET_NUM_SCRIPT_PARTICIPANTS(const char* scriptName, int instance_id, int position_hash) {
			return Invoker::Invoke<int, 5485>(scriptName, instance_id, position_hash);
		}

		int NETWORK_GET_INSTANCE_ID_OF_THIS_SCRIPT() {
			return Invoker::Invoke<int, 5486>();
		}

		Hash NETWORK_GET_POSITION_HASH_OF_THIS_SCRIPT() {
			return Invoker::Invoke<Hash, 5487>();
		}

		BOOL NETWORK_IS_PLAYER_A_PARTICIPANT_ON_SCRIPT(Player player, const char* script, int instance_id) {
			return Invoker::Invoke<BOOL, 5488>(player, script, instance_id);
		}

		void NETWORK_PREVENT_SCRIPT_HOST_MIGRATION() {
			Invoker::Invoke<void, 5489>();
		}

		void NETWORK_REQUEST_TO_BE_HOST_OF_THIS_SCRIPT() {
			Invoker::Invoke<void, 5490>();
		}

		Player PARTICIPANT_ID() {
			return Invoker::Invoke<Player, 5491>();
		}

		int PARTICIPANT_ID_TO_INT() {
			return Invoker::Invoke<int, 5492>();
		}

		Player NETWORK_GET_KILLER_OF_PLAYER(Player player, Hash* weaponHash) {
			return Invoker::Invoke<Player, 5493>(player, weaponHash);
		}

		Player NETWORK_GET_DESTROYER_OF_NETWORK_ID(int netId, Hash* weaponHash) {
			return Invoker::Invoke<Player, 5494>(netId, weaponHash);
		}

		Player NETWORK_GET_DESTROYER_OF_ENTITY(Entity entity, Hash* weaponHash) {
			return Invoker::Invoke<Player, 5495>(entity, weaponHash);
		}

		BOOL NETWORK_GET_ASSISTED_KILL_OF_ENTITY(Player player, Entity entity, int* p2) {
			return Invoker::Invoke<BOOL, 5496>(player, entity, p2);
		}

		BOOL NETWORK_GET_ASSISTED_DAMAGE_OF_ENTITY(Player player, Entity entity, int* p2) {
			return Invoker::Invoke<BOOL, 5497>(player, entity, p2);
		}

		Entity NETWORK_GET_ENTITY_KILLER_OF_PLAYER(Player player, Hash* weaponHash) {
			return Invoker::Invoke<Entity, 5498>(player, weaponHash);
		}

		void NETWORK_SET_CURRENT_PUBLIC_CONTENT_ID(const char* missionId) {
			Invoker::Invoke<void, 5499>(missionId);
		}

		void NETWORK_SET_CURRENT_CHAT_OPTION(int newChatOption) {
			Invoker::Invoke<void, 5500>(newChatOption);
		}

		void NETWORK_SET_CURRENT_SPAWN_LOCATION_OPTION(Hash mpSettingSpawn) {
			Invoker::Invoke<void, 5501>(mpSettingSpawn);
		}

		void NETWORK_SET_VEHICLE_DRIVEN_IN_TEST_DRIVE(BOOL toggle) {
			Invoker::Invoke<void, 5502>(toggle);
		}

		void NETWORK_SET_VEHICLE_DRIVEN_LOCATION(Hash location) {
			Invoker::Invoke<void, 5503>(location);
		}

		void NETWORK_RESURRECT_LOCAL_PLAYER(float x, float y, float z, float heading, BOOL p4, BOOL changetime, BOOL p6, int p7, int p8) {
			Invoker::Invoke<void, 5504>(x, y, z, heading, p4, changetime, p6, p7, p8);
		}

		void NETWORK_SET_LOCAL_PLAYER_INVINCIBLE_TIME(int time) {
			Invoker::Invoke<void, 5505>(time);
		}

		BOOL NETWORK_IS_LOCAL_PLAYER_INVINCIBLE() {
			return Invoker::Invoke<BOOL, 5506>();
		}

		void NETWORK_DISABLE_INVINCIBLE_FLASHING(Player player, BOOL toggle) {
			Invoker::Invoke<void, 5507>(player, toggle);
		}

		void NETWORK_PATCH_POST_CUTSCENE_HS4F_TUN_ENT(Ped ped) {
			Invoker::Invoke<void, 5508>(ped);
		}

		void NETWORK_SET_LOCAL_PLAYER_SYNC_LOOK_AT(BOOL toggle) {
			Invoker::Invoke<void, 5509>(toggle);
		}

		BOOL NETWORK_HAS_ENTITY_BEEN_REGISTERED_WITH_THIS_THREAD(Entity entity) {
			return Invoker::Invoke<BOOL, 5510>(entity);
		}

		int NETWORK_GET_NETWORK_ID_FROM_ENTITY(Entity entity) {
			return Invoker::Invoke<int, 5511>(entity);
		}

		Entity NETWORK_GET_ENTITY_FROM_NETWORK_ID(int netId) {
			return Invoker::Invoke<Entity, 5512>(netId);
		}

		BOOL NETWORK_GET_ENTITY_IS_NETWORKED(Entity entity) {
			return Invoker::Invoke<BOOL, 5513>(entity);
		}

		BOOL NETWORK_GET_ENTITY_IS_LOCAL(Entity entity) {
			return Invoker::Invoke<BOOL, 5514>(entity);
		}

		void NETWORK_REGISTER_ENTITY_AS_NETWORKED(Entity entity) {
			Invoker::Invoke<void, 5515>(entity);
		}

		void NETWORK_UNREGISTER_NETWORKED_ENTITY(Entity entity) {
			Invoker::Invoke<void, 5516>(entity);
		}

		BOOL NETWORK_DOES_NETWORK_ID_EXIST(int netId) {
			return Invoker::Invoke<BOOL, 5517>(netId);
		}

		BOOL NETWORK_DOES_ENTITY_EXIST_WITH_NETWORK_ID(int netId) {
			return Invoker::Invoke<BOOL, 5518>(netId);
		}

		BOOL NETWORK_REQUEST_CONTROL_OF_NETWORK_ID(int netId) {
			return Invoker::Invoke<BOOL, 5519>(netId);
		}

		BOOL NETWORK_HAS_CONTROL_OF_NETWORK_ID(int netId) {
			return Invoker::Invoke<BOOL, 5520>(netId);
		}

		BOOL NETWORK_IS_NETWORK_ID_REMOTELY_CONTROLLED(int netId) {
			return Invoker::Invoke<BOOL, 5521>(netId);
		}

		BOOL NETWORK_REQUEST_CONTROL_OF_ENTITY(Entity entity) {
			return Invoker::Invoke<BOOL, 5522>(entity);
		}

		BOOL NETWORK_REQUEST_CONTROL_OF_DOOR(int doorID) {
			return Invoker::Invoke<BOOL, 5523>(doorID);
		}

		BOOL NETWORK_HAS_CONTROL_OF_ENTITY(Entity entity) {
			return Invoker::Invoke<BOOL, 5524>(entity);
		}

		BOOL NETWORK_HAS_CONTROL_OF_PICKUP(Pickup pickup) {
			return Invoker::Invoke<BOOL, 5525>(pickup);
		}

		BOOL NETWORK_HAS_CONTROL_OF_DOOR(Hash doorHash) {
			return Invoker::Invoke<BOOL, 5526>(doorHash);
		}

		BOOL NETWORK_IS_DOOR_NETWORKED(Hash doorHash) {
			return Invoker::Invoke<BOOL, 5527>(doorHash);
		}

		int VEH_TO_NET(Vehicle vehicle) {
			return Invoker::Invoke<int, 5528>(vehicle);
		}

		int PED_TO_NET(Ped ped) {
			return Invoker::Invoke<int, 5529>(ped);
		}

		int OBJ_TO_NET(Object object) {
			return Invoker::Invoke<int, 5530>(object);
		}

		Vehicle NET_TO_VEH(int netHandle) {
			return Invoker::Invoke<Vehicle, 5531>(netHandle);
		}

		Ped NET_TO_PED(int netHandle) {
			return Invoker::Invoke<Ped, 5532>(netHandle);
		}

		Object NET_TO_OBJ(int netHandle) {
			return Invoker::Invoke<Object, 5533>(netHandle);
		}

		Entity NET_TO_ENT(int netHandle) {
			return Invoker::Invoke<Entity, 5534>(netHandle);
		}

		void NETWORK_GET_LOCAL_HANDLE(Any* gamerHandle, int gamerHandleSize) {
			Invoker::Invoke<void, 5535>(gamerHandle, gamerHandleSize);
		}

		void NETWORK_HANDLE_FROM_USER_ID(const char* userId, Any* gamerHandle, int gamerHandleSize) {
			Invoker::Invoke<void, 5536>(userId, gamerHandle, gamerHandleSize);
		}

		void NETWORK_HANDLE_FROM_MEMBER_ID(const char* memberId, Any* gamerHandle, int gamerHandleSize) {
			Invoker::Invoke<void, 5537>(memberId, gamerHandle, gamerHandleSize);
		}

		void NETWORK_HANDLE_FROM_PLAYER(Player player, Any* gamerHandle, int gamerHandleSize) {
			Invoker::Invoke<void, 5538>(player, gamerHandle, gamerHandleSize);
		}

		Hash NETWORK_HASH_FROM_PLAYER_HANDLE(Player player) {
			return Invoker::Invoke<Hash, 5539>(player);
		}

		Hash NETWORK_HASH_FROM_GAMER_HANDLE(Any* gamerHandle) {
			return Invoker::Invoke<Hash, 5540>(gamerHandle);
		}

		void NETWORK_HANDLE_FROM_FRIEND(int friendIndex, Any* gamerHandle, int gamerHandleSize) {
			Invoker::Invoke<void, 5541>(friendIndex, gamerHandle, gamerHandleSize);
		}

		BOOL NETWORK_GAMERTAG_FROM_HANDLE_START(Any* gamerHandle) {
			return Invoker::Invoke<BOOL, 5542>(gamerHandle);
		}

		BOOL NETWORK_GAMERTAG_FROM_HANDLE_PENDING() {
			return Invoker::Invoke<BOOL, 5543>();
		}

		BOOL NETWORK_GAMERTAG_FROM_HANDLE_SUCCEEDED() {
			return Invoker::Invoke<BOOL, 5544>();
		}

		const char* NETWORK_GET_GAMERTAG_FROM_HANDLE(Any* gamerHandle) {
			return Invoker::Invoke<const char*, 5545>(gamerHandle);
		}

		int NETWORK_DISPLAYNAMES_FROM_HANDLES_START(Any* p0, Any p1) {
			return Invoker::Invoke<int, 5546>(p0, p1);
		}

		int NETWORK_GET_DISPLAYNAMES_FROM_HANDLES(Any p0, Any p1, Any p2) {
			return Invoker::Invoke<int, 5547>(p0, p1, p2);
		}

		BOOL NETWORK_ARE_HANDLES_THE_SAME(Any* gamerHandle1, Any* gamerHandle2) {
			return Invoker::Invoke<BOOL, 5548>(gamerHandle1, gamerHandle2);
		}

		BOOL NETWORK_IS_HANDLE_VALID(Any* gamerHandle, int gamerHandleSize) {
			return Invoker::Invoke<BOOL, 5549>(gamerHandle, gamerHandleSize);
		}

		Player NETWORK_GET_PLAYER_FROM_GAMER_HANDLE(Any* gamerHandle) {
			return Invoker::Invoke<Player, 5550>(gamerHandle);
		}

		const char* NETWORK_MEMBER_ID_FROM_GAMER_HANDLE(Any* gamerHandle) {
			return Invoker::Invoke<const char*, 5551>(gamerHandle);
		}

		BOOL NETWORK_IS_GAMER_IN_MY_SESSION(Any* gamerHandle) {
			return Invoker::Invoke<BOOL, 5552>(gamerHandle);
		}

		void NETWORK_SHOW_PROFILE_UI(Any* gamerHandle) {
			Invoker::Invoke<void, 5553>(gamerHandle);
		}

		const char* NETWORK_PLAYER_GET_NAME(Player player) {
			return Invoker::Invoke<const char*, 5554>(player);
		}

		const char* NETWORK_PLAYER_GET_USERID(Player player, int* userID) {
			return Invoker::Invoke<const char*, 5555>(player, userID);
		}

		BOOL NETWORK_PLAYER_IS_ROCKSTAR_DEV(Player player) {
			return Invoker::Invoke<BOOL, 5556>(player);
		}

		BOOL NETWORK_PLAYER_INDEX_IS_CHEATER(Player player) {
			return Invoker::Invoke<BOOL, 5557>(player);
		}

		int NETWORK_ENTITY_GET_OBJECT_ID(Entity entity) {
			return Invoker::Invoke<int, 5558>(entity);
		}

		Entity NETWORK_GET_ENTITY_FROM_OBJECT_ID(Any p0) {
			return Invoker::Invoke<Entity, 5559>(p0);
		}

		BOOL NETWORK_IS_INACTIVE_PROFILE(Any* p0) {
			return Invoker::Invoke<BOOL, 5560>(p0);
		}

		const char* _NETWORK_LOAD_GAMER_DISPLAY_NAME(Any* gamerHandle) {
			return Invoker::Invoke<const char*, 6573>(gamerHandle);
		}

		int NETWORK_GET_MAX_FRIENDS() {
			return Invoker::Invoke<int, 5561>();
		}

		int NETWORK_GET_FRIEND_COUNT() {
			return Invoker::Invoke<int, 5562>();
		}

		const char* NETWORK_GET_FRIEND_NAME(int friendIndex) {
			return Invoker::Invoke<const char*, 5563>(friendIndex);
		}

		const char* NETWORK_GET_FRIEND_DISPLAY_NAME(int friendIndex) {
			return Invoker::Invoke<const char*, 5564>(friendIndex);
		}

		BOOL NETWORK_IS_FRIEND_ONLINE(const char* name) {
			return Invoker::Invoke<BOOL, 5565>(name);
		}

		BOOL NETWORK_IS_FRIEND_HANDLE_ONLINE(Any* gamerHandle) {
			return Invoker::Invoke<BOOL, 5566>(gamerHandle);
		}

		BOOL NETWORK_IS_FRIEND_IN_SAME_TITLE(const char* friendName) {
			return Invoker::Invoke<BOOL, 5567>(friendName);
		}

		BOOL NETWORK_IS_FRIEND_IN_MULTIPLAYER(const char* friendName) {
			return Invoker::Invoke<BOOL, 5568>(friendName);
		}

		BOOL NETWORK_IS_FRIEND(Any* gamerHandle) {
			return Invoker::Invoke<BOOL, 5569>(gamerHandle);
		}

		BOOL NETWORK_IS_PENDING_FRIEND(Any p0) {
			return Invoker::Invoke<BOOL, 5570>(p0);
		}

		BOOL NETWORK_IS_ADDING_FRIEND() {
			return Invoker::Invoke<BOOL, 5571>();
		}

		BOOL NETWORK_ADD_FRIEND(Any* gamerHandle, const char* message) {
			return Invoker::Invoke<BOOL, 5572>(gamerHandle, message);
		}

		BOOL NETWORK_IS_FRIEND_INDEX_ONLINE(int friendIndex) {
			return Invoker::Invoke<BOOL, 5573>(friendIndex);
		}

		void NETWORK_SET_PLAYER_IS_PASSIVE(BOOL toggle) {
			Invoker::Invoke<void, 5574>(toggle);
		}

		BOOL NETWORK_GET_PLAYER_OWNS_WAYPOINT(Player player) {
			return Invoker::Invoke<BOOL, 5575>(player);
		}

		BOOL NETWORK_CAN_SET_WAYPOINT() {
			return Invoker::Invoke<BOOL, 5576>();
		}

		void NETWORK_IGNORE_REMOTE_WAYPOINTS() {
			Invoker::Invoke<void, 5577>();
		}

		BOOL _NETWORK_DOES_COMMUNICATION_GROUP_HAVE_PERMISSION(int communicationType) {
			return Invoker::Invoke<BOOL, 5578>(communicationType);
		}

		BOOL _NETWORK_DOES_COMMUNICATION_GROUP_HAVE_SETTINGS_ENABLED(int communicationType) {
			return Invoker::Invoke<BOOL, 6649>(communicationType);
		}

		int _NETWORK_GET_COMMUNICATION_GROUP_FLAGS(int communicationType) {
			return Invoker::Invoke<int, 5579>(communicationType);
		}

		int _NETWORK_GET_COMMUNICATION_GROUP_DEFAULT_FLAGS(int communicationType) {
			return Invoker::Invoke<int, 6648>(communicationType);
		}

		int _NETWORK_GET_COMMUNICATION_GROUP_VALUE(int communicationType) {
			return Invoker::Invoke<int, 6647>(communicationType);
		}

		void _NETWORK_SET_COMMUNICATION_GROUP_FLAGS(int communicationType, int communicationGroupFlag) {
			Invoker::Invoke<void, 5580>(communicationType, communicationGroupFlag);
		}

		BOOL _NETWORK_HAS_PLAYER_PASSED_CHECK_TYPE(int checkType, Any* gamerHandle) {
			return Invoker::Invoke<BOOL, 6574>(checkType, gamerHandle);
		}

		BOOL NETWORK_IS_PLAYER_ON_BLOCKLIST(Any* gamerHandle) {
			return Invoker::Invoke<BOOL, 5581>(gamerHandle);
		}

		BOOL NETWORK_SET_SCRIPT_AUTOMUTED(Any p0) {
			return Invoker::Invoke<BOOL, 5582>(p0);
		}

		BOOL NETWORK_HAS_AUTOMUTE_OVERRIDE() {
			return Invoker::Invoke<BOOL, 5583>();
		}

		BOOL NETWORK_HAS_HEADSET() {
			return Invoker::Invoke<BOOL, 5584>();
		}

		void NETWORK_SET_LOOK_AT_TALKERS(BOOL p0) {
			Invoker::Invoke<void, 5585>(p0);
		}

		BOOL NETWORK_IS_PUSH_TO_TALK_ACTIVE() {
			return Invoker::Invoke<BOOL, 5586>();
		}

		BOOL NETWORK_GAMER_HAS_HEADSET(Any* gamerHandle) {
			return Invoker::Invoke<BOOL, 5587>(gamerHandle);
		}

		BOOL NETWORK_IS_GAMER_TALKING(Any* gamerHandle) {
			return Invoker::Invoke<BOOL, 5588>(gamerHandle);
		}

		BOOL NETWORK_PERMISSIONS_HAS_GAMER_RECORD(Any* gamerHandle) {
			return Invoker::Invoke<BOOL, 5589>(gamerHandle);
		}

		BOOL NETWORK_CAN_COMMUNICATE_WITH_GAMER(Any* gamerHandle) {
			return Invoker::Invoke<BOOL, 5590>(gamerHandle);
		}

		BOOL NETWORK_CAN_TEXT_CHAT_WITH_GAMER(Any* gamerHandle) {
			return Invoker::Invoke<BOOL, 5591>(gamerHandle);
		}

		BOOL NETWORK_IS_GAMER_MUTED_BY_ME(Any* gamerHandle) {
			return Invoker::Invoke<BOOL, 5592>(gamerHandle);
		}

		BOOL NETWORK_AM_I_MUTED_BY_GAMER(Any* gamerHandle) {
			return Invoker::Invoke<BOOL, 5593>(gamerHandle);
		}

		BOOL NETWORK_IS_GAMER_BLOCKED_BY_ME(Any* gamerHandle) {
			return Invoker::Invoke<BOOL, 5594>(gamerHandle);
		}

		BOOL NETWORK_AM_I_BLOCKED_BY_GAMER(Any* gamerHandle) {
			return Invoker::Invoke<BOOL, 5595>(gamerHandle);
		}

		BOOL NETWORK_CAN_VIEW_GAMER_USER_CONTENT(Any* gamerHandle) {
			return Invoker::Invoke<BOOL, 5596>(gamerHandle);
		}

		BOOL NETWORK_HAS_VIEW_GAMER_USER_CONTENT_RESULT(Any* gamerHandle) {
			return Invoker::Invoke<BOOL, 5597>(gamerHandle);
		}

		BOOL _NETWORK_CAN_TEXT_FROM_GAMER_BE_VIEWED(Any* gamerHandle) {
			return Invoker::Invoke<BOOL, 6575>(gamerHandle);
		}

		BOOL NETWORK_CAN_PLAY_MULTIPLAYER_WITH_GAMER(Any* gamerHandle) {
			return Invoker::Invoke<BOOL, 5598>(gamerHandle);
		}

		BOOL NETWORK_CAN_GAMER_PLAY_MULTIPLAYER_WITH_ME(Any* gamerHandle) {
			return Invoker::Invoke<BOOL, 5599>(gamerHandle);
		}

		BOOL NETWORK_CAN_SEND_LOCAL_INVITE(Any* gamerHandle) {
			return Invoker::Invoke<BOOL, 5600>(gamerHandle);
		}

		BOOL NETWORK_CAN_RECEIVE_LOCAL_INVITE(Any* gamerHandle) {
			return Invoker::Invoke<BOOL, 5601>(gamerHandle);
		}

		BOOL NETWORK_IS_PLAYER_TALKING(Player player) {
			return Invoker::Invoke<BOOL, 5602>(player);
		}

		BOOL NETWORK_PLAYER_HAS_HEADSET(Player player) {
			return Invoker::Invoke<BOOL, 5603>(player);
		}

		BOOL NETWORK_IS_PLAYER_MUTED_BY_ME(Player player) {
			return Invoker::Invoke<BOOL, 5604>(player);
		}

		BOOL NETWORK_AM_I_MUTED_BY_PLAYER(Player player) {
			return Invoker::Invoke<BOOL, 5605>(player);
		}

		BOOL NETWORK_IS_PLAYER_BLOCKED_BY_ME(Player player) {
			return Invoker::Invoke<BOOL, 5606>(player);
		}

		BOOL NETWORK_AM_I_BLOCKED_BY_PLAYER(Player player) {
			return Invoker::Invoke<BOOL, 5607>(player);
		}

		float NETWORK_GET_PLAYER_LOUDNESS(Player player) {
			return Invoker::Invoke<float, 5608>(player);
		}

		void NETWORK_SET_TALKER_PROXIMITY(float value) {
			Invoker::Invoke<void, 5609>(value);
		}

		float NETWORK_GET_TALKER_PROXIMITY() {
			return Invoker::Invoke<float, 5610>();
		}

		void NETWORK_SET_VOICE_ACTIVE(BOOL toggle) {
			Invoker::Invoke<void, 5611>(toggle);
		}

		void NETWORK_REMAIN_IN_GAME_CHAT(BOOL p0) {
			Invoker::Invoke<void, 5612>(p0);
		}

		void NETWORK_OVERRIDE_TRANSITION_CHAT(BOOL p0) {
			Invoker::Invoke<void, 5613>(p0);
		}

		void NETWORK_SET_TEAM_ONLY_CHAT(BOOL toggle) {
			Invoker::Invoke<void, 5614>(toggle);
		}

		void NETWORK_SET_SCRIPT_CONTROLLING_TEAMS(Any p0) {
			Invoker::Invoke<void, 5615>(p0);
		}

		BOOL NETWORK_SET_SAME_TEAM_AS_LOCAL_PLAYER(Any p0, Any p1) {
			return Invoker::Invoke<BOOL, 5616>(p0, p1);
		}

		void NETWORK_OVERRIDE_TEAM_RESTRICTIONS(int team, BOOL toggle) {
			Invoker::Invoke<void, 5617>(team, toggle);
		}

		void NETWORK_SET_OVERRIDE_SPECTATOR_MODE(BOOL toggle) {
			Invoker::Invoke<void, 5618>(toggle);
		}

		void NETWORK_SET_OVERRIDE_TUTORIAL_SESSION_CHAT(BOOL toggle) {
			Invoker::Invoke<void, 5619>(toggle);
		}

		void NETWORK_SET_PROXIMITY_AFFECTS_TEAM(BOOL toggle) {
			Invoker::Invoke<void, 5620>(toggle);
		}

		void NETWORK_SET_NO_SPECTATOR_CHAT(BOOL toggle) {
			Invoker::Invoke<void, 5621>(toggle);
		}

		void NETWORK_SET_IGNORE_SPECTATOR_CHAT_LIMITS_SAME_TEAM(BOOL toggle) {
			Invoker::Invoke<void, 5622>(toggle);
		}

		void NETWORK_OVERRIDE_CHAT_RESTRICTIONS(Player player, BOOL toggle) {
			Invoker::Invoke<void, 5623>(player, toggle);
		}

		void NETWORK_OVERRIDE_SEND_RESTRICTIONS(Player player, BOOL toggle) {
			Invoker::Invoke<void, 5624>(player, toggle);
		}

		void NETWORK_OVERRIDE_SEND_RESTRICTIONS_ALL(BOOL toggle) {
			Invoker::Invoke<void, 5625>(toggle);
		}

		void NETWORK_OVERRIDE_RECEIVE_RESTRICTIONS(Player player, BOOL toggle) {
			Invoker::Invoke<void, 5626>(player, toggle);
		}

		void NETWORK_OVERRIDE_RECEIVE_RESTRICTIONS_ALL(BOOL toggle) {
			Invoker::Invoke<void, 5627>(toggle);
		}

		void NETWORK_SET_VOICE_CHANNEL(int channel) {
			Invoker::Invoke<void, 5628>(channel);
		}

		void NETWORK_CLEAR_VOICE_CHANNEL() {
			Invoker::Invoke<void, 5629>();
		}

		void NETWORK_APPLY_VOICE_PROXIMITY_OVERRIDE(float x, float y, float z) {
			Invoker::Invoke<void, 5630>(x, y, z);
		}

		void NETWORK_CLEAR_VOICE_PROXIMITY_OVERRIDE() {
			Invoker::Invoke<void, 5631>();
		}

		void NETWORK_ENABLE_VOICE_BANDWIDTH_RESTRICTION(Player player) {
			Invoker::Invoke<void, 5632>(player);
		}

		void NETWORK_DISABLE_VOICE_BANDWIDTH_RESTRICTION(Player player) {
			Invoker::Invoke<void, 5633>(player);
		}

		void NETWORK_GET_MUTE_COUNT_FOR_PLAYER(Player p0, float* p1, float* p2) {
			Invoker::Invoke<void, 5634>(p0, p1, p2);
		}

		void NETWORK_SET_SPECTATOR_TO_NON_SPECTATOR_TEXT_CHAT(BOOL toggle) {
			Invoker::Invoke<void, 5635>(toggle);
		}

		BOOL NETWORK_TEXT_CHAT_IS_TYPING() {
			return Invoker::Invoke<BOOL, 5636>();
		}

		void SHUTDOWN_AND_LAUNCH_SINGLE_PLAYER_GAME() {
			Invoker::Invoke<void, 5637>();
		}

		BOOL SHUTDOWN_AND_LOAD_MOST_RECENT_SAVE() {
			return Invoker::Invoke<BOOL, 5638>();
		}

		void NETWORK_SET_FRIENDLY_FIRE_OPTION(BOOL toggle) {
			Invoker::Invoke<void, 5639>(toggle);
		}

		void NETWORK_SET_RICH_PRESENCE(int p0, int p1, Any p2, Any p3) {
			Invoker::Invoke<void, 5640>(p0, p1, p2, p3);
		}

		void NETWORK_SET_RICH_PRESENCE_STRING(int p0, const char* textLabel) {
			Invoker::Invoke<void, 5641>(p0, textLabel);
		}

		int NETWORK_GET_TIMEOUT_TIME() {
			return Invoker::Invoke<int, 5642>();
		}

		void NETWORK_LEAVE_PED_BEHIND_BEFORE_WARP(Player player, float x, float y, float z, BOOL p4, BOOL p5) {
			Invoker::Invoke<void, 5643>(player, x, y, z, p4, p5);
		}

		void NETWORK_LEAVE_PED_BEHIND_BEFORE_CUTSCENE(Player player, BOOL p1) {
			Invoker::Invoke<void, 5644>(player, p1);
		}

		void REMOVE_ALL_STICKY_BOMBS_FROM_ENTITY(Entity entity, Ped ped) {
			Invoker::Invoke<void, 5645>(entity, ped);
		}

		void NETWORK_KEEP_ENTITY_COLLISION_DISABLED_AFTER_ANIM_SCENE(Any p0, Any p1) {
			Invoker::Invoke<void, 5646>(p0, p1);
		}

		BOOL NETWORK_IS_ANY_PLAYER_NEAR(Any p0, Any p1, Any p2, Any p3, Any p4, Any p5, Any p6) {
			return Invoker::Invoke<BOOL, 5647>(p0, p1, p2, p3, p4, p5, p6);
		}

		BOOL NETWORK_CLAN_SERVICE_IS_VALID() {
			return Invoker::Invoke<BOOL, 5648>();
		}

		BOOL NETWORK_CLAN_PLAYER_IS_ACTIVE(Any* gamerHandle) {
			return Invoker::Invoke<BOOL, 5649>(gamerHandle);
		}

		BOOL NETWORK_CLAN_PLAYER_GET_DESC(Any* clanDesc, int bufferSize, Any* gamerHandle) {
			return Invoker::Invoke<BOOL, 5650>(clanDesc, bufferSize, gamerHandle);
		}

		BOOL NETWORK_CLAN_IS_ROCKSTAR_CLAN(Any* clanDesc, int bufferSize) {
			return Invoker::Invoke<BOOL, 5651>(clanDesc, bufferSize);
		}

		void NETWORK_CLAN_GET_UI_FORMATTED_TAG(Any* clanDesc, int bufferSize, char* formattedTag) {
			Invoker::Invoke<void, 5652>(clanDesc, bufferSize, formattedTag);
		}

		int NETWORK_CLAN_GET_LOCAL_MEMBERSHIPS_COUNT() {
			return Invoker::Invoke<int, 5653>();
		}

		BOOL NETWORK_CLAN_GET_MEMBERSHIP_DESC(Any* memberDesc, int p1) {
			return Invoker::Invoke<BOOL, 5654>(memberDesc, p1);
		}

		BOOL NETWORK_CLAN_DOWNLOAD_MEMBERSHIP(Any* gamerHandle) {
			return Invoker::Invoke<BOOL, 5655>(gamerHandle);
		}

		BOOL NETWORK_CLAN_DOWNLOAD_MEMBERSHIP_PENDING(Any* p0) {
			return Invoker::Invoke<BOOL, 5656>(p0);
		}

		BOOL NETWORK_CLAN_ANY_DOWNLOAD_MEMBERSHIP_PENDING() {
			return Invoker::Invoke<BOOL, 5657>();
		}

		BOOL NETWORK_CLAN_REMOTE_MEMBERSHIPS_ARE_IN_CACHE(int* p0) {
			return Invoker::Invoke<BOOL, 5658>(p0);
		}

		int NETWORK_CLAN_GET_MEMBERSHIP_COUNT(int* p0) {
			return Invoker::Invoke<int, 5659>(p0);
		}

		BOOL NETWORK_CLAN_GET_MEMBERSHIP_VALID(int* p0, Any p1) {
			return Invoker::Invoke<BOOL, 5660>(p0, p1);
		}

		BOOL NETWORK_CLAN_GET_MEMBERSHIP(int* p0, Any* clanMembership, int p2) {
			return Invoker::Invoke<BOOL, 5661>(p0, clanMembership, p2);
		}

		BOOL NETWORK_CLAN_JOIN(int clanDesc) {
			return Invoker::Invoke<BOOL, 5662>(clanDesc);
		}

		BOOL NETWORK_CLAN_CREWINFO_GET_STRING_VALUE(const char* animDict, const char* animName) {
			return Invoker::Invoke<BOOL, 5663>(animDict, animName);
		}

		BOOL NETWORK_CLAN_CREWINFO_GET_CREWRANKTITLE(int p0, const char* p1) {
			return Invoker::Invoke<BOOL, 5664>(p0, p1);
		}

		BOOL NETWORK_CLAN_HAS_CREWINFO_METADATA_BEEN_RECEIVED() {
			return Invoker::Invoke<BOOL, 5665>();
		}

		BOOL NETWORK_CLAN_GET_EMBLEM_TXD_NAME(Any* netHandle, char* txdName) {
			return Invoker::Invoke<BOOL, 5666>(netHandle, txdName);
		}

		BOOL NETWORK_CLAN_REQUEST_EMBLEM(Any p0) {
			return Invoker::Invoke<BOOL, 5667>(p0);
		}

		BOOL NETWORK_CLAN_IS_EMBLEM_READY(Any p0, Any* p1) {
			return Invoker::Invoke<BOOL, 5668>(p0, p1);
		}

		void NETWORK_CLAN_RELEASE_EMBLEM(Any p0) {
			Invoker::Invoke<void, 5669>(p0);
		}

		BOOL NETWORK_GET_PRIMARY_CLAN_DATA_CLEAR() {
			return Invoker::Invoke<BOOL, 5670>();
		}

		void NETWORK_GET_PRIMARY_CLAN_DATA_CANCEL() {
			Invoker::Invoke<void, 5671>();
		}

		BOOL NETWORK_GET_PRIMARY_CLAN_DATA_START(Any* p0, Any p1) {
			return Invoker::Invoke<BOOL, 5672>(p0, p1);
		}

		BOOL NETWORK_GET_PRIMARY_CLAN_DATA_PENDING() {
			return Invoker::Invoke<BOOL, 5673>();
		}

		BOOL NETWORK_GET_PRIMARY_CLAN_DATA_SUCCESS() {
			return Invoker::Invoke<BOOL, 5674>();
		}

		BOOL NETWORK_GET_PRIMARY_CLAN_DATA_NEW(Any* p0, Any* p1) {
			return Invoker::Invoke<BOOL, 5675>(p0, p1);
		}

		void SET_NETWORK_ID_CAN_MIGRATE(int netId, BOOL toggle) {
			Invoker::Invoke<void, 5676>(netId, toggle);
		}

		void SET_NETWORK_ID_EXISTS_ON_ALL_MACHINES(int netId, BOOL toggle) {
			Invoker::Invoke<void, 5677>(netId, toggle);
		}

		void SET_NETWORK_ID_ALWAYS_EXISTS_FOR_PLAYER(int netId, Player player, BOOL toggle) {
			Invoker::Invoke<void, 5678>(netId, player, toggle);
		}

		void SET_NETWORK_ID_CAN_BE_REASSIGNED(int netId, BOOL toggle) {
			Invoker::Invoke<void, 5679>(netId, toggle);
		}

		void NETWORK_SET_ENTITY_CAN_BLEND(Entity entity, BOOL toggle) {
			Invoker::Invoke<void, 5680>(entity, toggle);
		}

		void NETWORK_SET_OBJECT_CAN_BLEND_WHEN_FIXED(Object object, BOOL toggle) {
			Invoker::Invoke<void, 5681>(object, toggle);
		}

		void NETWORK_SET_ENTITY_ONLY_EXISTS_FOR_PARTICIPANTS(Entity entity, BOOL toggle) {
			Invoker::Invoke<void, 5682>(entity, toggle);
		}

		void SET_NETWORK_ID_VISIBLE_IN_CUTSCENE(int netId, BOOL p1, BOOL p2) {
			Invoker::Invoke<void, 5683>(netId, p1, p2);
		}

		void SET_NETWORK_ID_VISIBLE_IN_CUTSCENE_HACK(int netId, BOOL p1, BOOL p2) {
			Invoker::Invoke<void, 5684>(netId, p1, p2);
		}

		void SET_NETWORK_ID_VISIBLE_IN_CUTSCENE_REMAIN_HACK(Any p0, Any p1, Any p2) {
			Invoker::Invoke<void, 5685>(p0, p1, p2);
		}

		void SET_NETWORK_CUTSCENE_ENTITIES(BOOL toggle) {
			Invoker::Invoke<void, 5686>(toggle);
		}

		BOOL ARE_CUTSCENE_ENTITIES_NETWORKED() {
			return Invoker::Invoke<BOOL, 5687>();
		}

		void SET_NETWORK_ID_PASS_CONTROL_IN_TUTORIAL(int netId, BOOL state) {
			Invoker::Invoke<void, 5688>(netId, state);
		}

		void _NETWORK_SET_TUTORIAL_SPECIAL_SESSION(BOOL toggle) {
			Invoker::Invoke<void, 6646>(toggle);
		}

		BOOL _NETWORK_IS_SPECIAL_TUTORIAL_SESSION() {
			return Invoker::Invoke<BOOL, 6645>();
		}

		BOOL IS_NETWORK_ID_OWNED_BY_PARTICIPANT(int netId) {
			return Invoker::Invoke<BOOL, 5689>(netId);
		}

		void SET_REMOTE_PLAYER_VISIBLE_IN_CUTSCENE(Player player, BOOL locallyVisible) {
			Invoker::Invoke<void, 5690>(player, locallyVisible);
		}

		void SET_LOCAL_PLAYER_VISIBLE_IN_CUTSCENE(BOOL p0, BOOL p1) {
			Invoker::Invoke<void, 5691>(p0, p1);
		}

		void SET_LOCAL_PLAYER_INVISIBLE_LOCALLY(BOOL bIncludePlayersVehicle) {
			Invoker::Invoke<void, 5692>(bIncludePlayersVehicle);
		}

		void SET_LOCAL_PLAYER_VISIBLE_LOCALLY(BOOL bIncludePlayersVehicle) {
			Invoker::Invoke<void, 5693>(bIncludePlayersVehicle);
		}

		void SET_PLAYER_INVISIBLE_LOCALLY(Player player, BOOL bIncludePlayersVehicle) {
			Invoker::Invoke<void, 5694>(player, bIncludePlayersVehicle);
		}

		void SET_PLAYER_VISIBLE_LOCALLY(Player player, BOOL bIncludePlayersVehicle) {
			Invoker::Invoke<void, 5695>(player, bIncludePlayersVehicle);
		}

		void FADE_OUT_LOCAL_PLAYER(BOOL p0) {
			Invoker::Invoke<void, 5696>(p0);
		}

		void NETWORK_FADE_OUT_ENTITY(Entity entity, BOOL normal, BOOL slow) {
			Invoker::Invoke<void, 5697>(entity, normal, slow);
		}

		void NETWORK_FADE_IN_ENTITY(Entity entity, BOOL state, Any p2) {
			Invoker::Invoke<void, 5698>(entity, state, p2);
		}

		BOOL NETWORK_IS_PLAYER_FADING(Player player) {
			return Invoker::Invoke<BOOL, 5699>(player);
		}

		BOOL NETWORK_IS_ENTITY_FADING(Entity entity) {
			return Invoker::Invoke<BOOL, 5700>(entity);
		}

		BOOL IS_PLAYER_IN_CUTSCENE(Player player) {
			return Invoker::Invoke<BOOL, 5701>(player);
		}

		void SET_ENTITY_VISIBLE_IN_CUTSCENE(Any p0, BOOL p1, BOOL p2) {
			Invoker::Invoke<void, 5702>(p0, p1, p2);
		}

		void SET_ENTITY_LOCALLY_INVISIBLE(Entity entity) {
			Invoker::Invoke<void, 5703>(entity);
		}

		void SET_ENTITY_LOCALLY_VISIBLE(Entity entity) {
			Invoker::Invoke<void, 5704>(entity);
		}

		BOOL IS_DAMAGE_TRACKER_ACTIVE_ON_NETWORK_ID(int netID) {
			return Invoker::Invoke<BOOL, 5705>(netID);
		}

		void ACTIVATE_DAMAGE_TRACKER_ON_NETWORK_ID(int netID, BOOL toggle) {
			Invoker::Invoke<void, 5706>(netID, toggle);
		}

		BOOL IS_DAMAGE_TRACKER_ACTIVE_ON_PLAYER(Player player) {
			return Invoker::Invoke<BOOL, 5707>(player);
		}

		void ACTIVATE_DAMAGE_TRACKER_ON_PLAYER(Player player, BOOL toggle) {
			Invoker::Invoke<void, 5708>(player, toggle);
		}

		BOOL IS_SPHERE_VISIBLE_TO_ANOTHER_MACHINE(float p0, float p1, float p2, float p3) {
			return Invoker::Invoke<BOOL, 5709>(p0, p1, p2, p3);
		}

		BOOL IS_SPHERE_VISIBLE_TO_PLAYER(Any p0, float p1, float p2, float p3, float p4) {
			return Invoker::Invoke<BOOL, 5710>(p0, p1, p2, p3, p4);
		}

		void RESERVE_NETWORK_MISSION_OBJECTS(int amount) {
			Invoker::Invoke<void, 5711>(amount);
		}

		void RESERVE_NETWORK_MISSION_PEDS(int amount) {
			Invoker::Invoke<void, 5712>(amount);
		}

		void RESERVE_NETWORK_MISSION_VEHICLES(int amount) {
			Invoker::Invoke<void, 5713>(amount);
		}

		void RESERVE_LOCAL_NETWORK_MISSION_OBJECTS(int amount) {
			Invoker::Invoke<void, 5714>(amount);
		}

		void RESERVE_LOCAL_NETWORK_MISSION_PEDS(int amount) {
			Invoker::Invoke<void, 5715>(amount);
		}

		void RESERVE_LOCAL_NETWORK_MISSION_VEHICLES(int amount) {
			Invoker::Invoke<void, 5716>(amount);
		}

		BOOL CAN_REGISTER_MISSION_OBJECTS(int amount) {
			return Invoker::Invoke<BOOL, 5717>(amount);
		}

		BOOL CAN_REGISTER_MISSION_PEDS(int amount) {
			return Invoker::Invoke<BOOL, 5718>(amount);
		}

		BOOL CAN_REGISTER_MISSION_VEHICLES(int amount) {
			return Invoker::Invoke<BOOL, 5719>(amount);
		}

		BOOL CAN_REGISTER_MISSION_PICKUPS(int amount) {
			return Invoker::Invoke<BOOL, 5720>(amount);
		}

		BOOL CAN_REGISTER_MISSION_DOORS(Any p0) {
			return Invoker::Invoke<BOOL, 5721>(p0);
		}

		BOOL CAN_REGISTER_MISSION_ENTITIES(int ped_amt, int vehicle_amt, int object_amt, int pickup_amt) {
			return Invoker::Invoke<BOOL, 5722>(ped_amt, vehicle_amt, object_amt, pickup_amt);
		}

		int GET_NUM_RESERVED_MISSION_OBJECTS(BOOL p0, Any p1) {
			return Invoker::Invoke<int, 5723>(p0, p1);
		}

		int GET_NUM_RESERVED_MISSION_PEDS(BOOL p0, Any p1) {
			return Invoker::Invoke<int, 5724>(p0, p1);
		}

		int GET_NUM_RESERVED_MISSION_VEHICLES(BOOL p0, Any p1) {
			return Invoker::Invoke<int, 5725>(p0, p1);
		}

		int GET_NUM_CREATED_MISSION_OBJECTS(BOOL p0) {
			return Invoker::Invoke<int, 5726>(p0);
		}

		int GET_NUM_CREATED_MISSION_PEDS(BOOL p0) {
			return Invoker::Invoke<int, 5727>(p0);
		}

		int GET_NUM_CREATED_MISSION_VEHICLES(BOOL p0) {
			return Invoker::Invoke<int, 5728>(p0);
		}

		void GET_RESERVED_MISSION_ENTITIES_IN_AREA(float x, float y, float z, Any p3, Any* out1, Any* out2, Any* out3) {
			Invoker::Invoke<void, 5729>(x, y, z, p3, out1, out2, out3);
		}

		int GET_MAX_NUM_NETWORK_OBJECTS() {
			return Invoker::Invoke<int, 5730>();
		}

		int GET_MAX_NUM_NETWORK_PEDS() {
			return Invoker::Invoke<int, 5731>();
		}

		int GET_MAX_NUM_NETWORK_VEHICLES() {
			return Invoker::Invoke<int, 5732>();
		}

		int GET_MAX_NUM_NETWORK_PICKUPS() {
			return Invoker::Invoke<int, 5733>();
		}

		void NETWORK_SET_OBJECT_SCOPE_DISTANCE(Object object, float range) {
			Invoker::Invoke<void, 5734>(object, range);
		}

		void NETWORK_ALLOW_CLONING_WHILE_IN_TUTORIAL(Any p0, Any p1) {
			Invoker::Invoke<void, 5735>(p0, p1);
		}

		void NETWORK_SET_TASK_CUTSCENE_INSCOPE_MULTIPLER(float multiplier) {
			Invoker::Invoke<void, 5736>(multiplier);
		}

		void _NETWORK_HIDE_ENTITY_IN_TUTORIAL_SESSION(int netHandle, BOOL hide) {
			Invoker::Invoke<void, 6576>(netHandle, hide);
		}

		int GET_NETWORK_TIME() {
			return Invoker::Invoke<int, 5737>();
		}

		int GET_NETWORK_TIME_ACCURATE() {
			return Invoker::Invoke<int, 5738>();
		}

		BOOL HAS_NETWORK_TIME_STARTED() {
			return Invoker::Invoke<BOOL, 5739>();
		}

		int GET_TIME_OFFSET(int timeA, int timeB) {
			return Invoker::Invoke<int, 5740>(timeA, timeB);
		}

		BOOL IS_TIME_LESS_THAN(int timeA, int timeB) {
			return Invoker::Invoke<BOOL, 5741>(timeA, timeB);
		}

		BOOL IS_TIME_MORE_THAN(int timeA, int timeB) {
			return Invoker::Invoke<BOOL, 5742>(timeA, timeB);
		}

		BOOL IS_TIME_EQUAL_TO(int timeA, int timeB) {
			return Invoker::Invoke<BOOL, 5743>(timeA, timeB);
		}

		int GET_TIME_DIFFERENCE(int timeA, int timeB) {
			return Invoker::Invoke<int, 5744>(timeA, timeB);
		}

		const char* GET_TIME_AS_STRING(int time) {
			return Invoker::Invoke<const char*, 5745>(time);
		}

		const char* GET_CLOUD_TIME_AS_STRING() {
			return Invoker::Invoke<const char*, 5746>();
		}

		int GET_CLOUD_TIME_AS_INT() {
			return Invoker::Invoke<int, 5747>();
		}

		void CONVERT_POSIX_TIME(int posixTime, Any* timeStructure) {
			Invoker::Invoke<void, 5748>(posixTime, timeStructure);
		}

		void NETWORK_SET_IN_SPECTATOR_MODE(BOOL toggle, Ped playerPed) {
			Invoker::Invoke<void, 5749>(toggle, playerPed);
		}

		void NETWORK_SET_IN_SPECTATOR_MODE_EXTENDED(BOOL toggle, Ped playerPed, BOOL p2) {
			Invoker::Invoke<void, 5750>(toggle, playerPed, p2);
		}

		void NETWORK_SET_IN_FREE_CAM_MODE(BOOL toggle) {
			Invoker::Invoke<void, 5751>(toggle);
		}

		void NETWORK_SET_ANTAGONISTIC_TO_PLAYER(BOOL toggle, Player player) {
			Invoker::Invoke<void, 5752>(toggle, player);
		}

		BOOL NETWORK_IS_IN_SPECTATOR_MODE() {
			return Invoker::Invoke<BOOL, 5753>();
		}

		void NETWORK_SET_IN_MP_CUTSCENE(BOOL p0, BOOL p1) {
			Invoker::Invoke<void, 5754>(p0, p1);
		}

		BOOL NETWORK_IS_IN_MP_CUTSCENE() {
			return Invoker::Invoke<BOOL, 5755>();
		}

		BOOL NETWORK_IS_PLAYER_IN_MP_CUTSCENE(Player player) {
			return Invoker::Invoke<BOOL, 5756>(player);
		}

		void NETWORK_HIDE_PROJECTILE_IN_CUTSCENE() {
			Invoker::Invoke<void, 5757>();
		}

		void SET_NETWORK_VEHICLE_RESPOT_TIMER(int netId, int time, Any p2, Any p3) {
			Invoker::Invoke<void, 5758>(netId, time, p2, p3);
		}

		BOOL IS_NETWORK_VEHICLE_RUNNING_RESPOT_TIMER(int networkID) {
			return Invoker::Invoke<BOOL, 5759>(networkID);
		}

		void SET_NETWORK_VEHICLE_AS_GHOST(Vehicle vehicle, BOOL toggle) {
			Invoker::Invoke<void, 5760>(vehicle, toggle);
		}

		void SET_NETWORK_VEHICLE_MAX_POSITION_DELTA_MULTIPLIER(Vehicle vehicle, float multiplier) {
			Invoker::Invoke<void, 5761>(vehicle, multiplier);
		}

		void SET_NETWORK_ENABLE_HIGH_SPEED_EDGE_FALL_DETECTION(Vehicle vehicle, BOOL toggle) {
			Invoker::Invoke<void, 5762>(vehicle, toggle);
		}

		void SET_LOCAL_PLAYER_AS_GHOST(BOOL toggle, BOOL p1) {
			Invoker::Invoke<void, 5763>(toggle, p1);
		}

		BOOL IS_ENTITY_A_GHOST(Entity entity) {
			return Invoker::Invoke<BOOL, 5764>(entity);
		}

		void SET_NON_PARTICIPANTS_OF_THIS_SCRIPT_AS_GHOSTS(BOOL p0) {
			Invoker::Invoke<void, 5765>(p0);
		}

		void SET_REMOTE_PLAYER_AS_GHOST(Player player, BOOL p1) {
			Invoker::Invoke<void, 5766>(player, p1);
		}

		void SET_GHOST_ALPHA(int alpha) {
			Invoker::Invoke<void, 5767>(alpha);
		}

		void RESET_GHOST_ALPHA() {
			Invoker::Invoke<void, 5768>();
		}

		void SET_ENTITY_GHOSTED_FOR_GHOST_PLAYERS(Entity entity, BOOL toggle) {
			Invoker::Invoke<void, 5769>(entity, toggle);
		}

		void SET_INVERT_GHOSTING(BOOL p0) {
			Invoker::Invoke<void, 5770>(p0);
		}

		BOOL IS_ENTITY_IN_GHOST_COLLISION(Entity entity) {
			return Invoker::Invoke<BOOL, 5771>(entity);
		}

		void _SET_FREEMODE_REPORT_DATA(Any* gamerHandle, Any* reportData) {
			Invoker::Invoke<void, 6577>(gamerHandle, reportData);
		}

		BOOL IS_OBJECT_REASSIGNMENT_IN_PROGRESS() {
			return Invoker::Invoke<BOOL, 6644>();
		}

		void USE_PLAYER_COLOUR_INSTEAD_OF_TEAM_COLOUR(BOOL toggle) {
			Invoker::Invoke<void, 5772>(toggle);
		}

		int NETWORK_CREATE_SYNCHRONISED_SCENE(float x, float y, float z, float xRot, float yRot, float zRot, int rotationOrder, BOOL useOcclusionPortal, BOOL looped, float p9, float animTime, float p11) {
			return Invoker::Invoke<int, 5773>(x, y, z, xRot, yRot, zRot, rotationOrder, useOcclusionPortal, looped, p9, animTime, p11);
		}

		void NETWORK_ADD_PED_TO_SYNCHRONISED_SCENE(Ped ped, int netScene, const char* animDict, const char* animnName, float speed, float speedMultiplier, int duration, int flag, float playbackRate, Any p9) {
			Invoker::Invoke<void, 5774>(ped, netScene, animDict, animnName, speed, speedMultiplier, duration, flag, playbackRate, p9);
		}

		void NETWORK_ADD_PED_TO_SYNCHRONISED_SCENE_WITH_IK(Any p0, Any p1, Any p2, Any p3, Any p4, Any p5, Any p6, Any p7, Any p8, Any p9) {
			Invoker::Invoke<void, 5775>(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9);
		}

		void NETWORK_ADD_ENTITY_TO_SYNCHRONISED_SCENE(Entity entity, int netScene, const char* animDict, const char* animName, float speed, float speedMulitiplier, int flag) {
			Invoker::Invoke<void, 5776>(entity, netScene, animDict, animName, speed, speedMulitiplier, flag);
		}

		void NETWORK_ADD_MAP_ENTITY_TO_SYNCHRONISED_SCENE(int netScene, Hash modelHash, float x, float y, float z, float p5, const char* p6, float p7, float p8, int flags) {
			Invoker::Invoke<void, 5777>(netScene, modelHash, x, y, z, p5, p6, p7, p8, flags);
		}

		void NETWORK_ADD_SYNCHRONISED_SCENE_CAMERA(int netScene, const char* animDict, const char* animName) {
			Invoker::Invoke<void, 5778>(netScene, animDict, animName);
		}

		void NETWORK_ATTACH_SYNCHRONISED_SCENE_TO_ENTITY(int netScene, Entity entity, int bone) {
			Invoker::Invoke<void, 5779>(netScene, entity, bone);
		}

		void NETWORK_START_SYNCHRONISED_SCENE(int netScene) {
			Invoker::Invoke<void, 5780>(netScene);
		}

		void NETWORK_STOP_SYNCHRONISED_SCENE(int netScene) {
			Invoker::Invoke<void, 5781>(netScene);
		}

		int NETWORK_GET_LOCAL_SCENE_FROM_NETWORK_ID(int netId) {
			return Invoker::Invoke<int, 5782>(netId);
		}

		void NETWORK_FORCE_LOCAL_USE_OF_SYNCED_SCENE_CAMERA(int netScene) {
			Invoker::Invoke<void, 5783>(netScene);
		}

		void NETWORK_ALLOW_REMOTE_SYNCED_SCENE_LOCAL_PLAYER_REQUESTS(Any p0) {
			Invoker::Invoke<void, 5784>(p0);
		}

		BOOL NETWORK_FIND_LARGEST_BUNCH_OF_PLAYERS(int p0, Any p1) {
			return Invoker::Invoke<BOOL, 5785>(p0, p1);
		}

		BOOL NETWORK_START_RESPAWN_SEARCH_FOR_PLAYER(Player player, float x, float y, float z, float radius, float p5, float p6, float p7, int flags) {
			return Invoker::Invoke<BOOL, 5786>(player, x, y, z, radius, p5, p6, p7, flags);
		}

		BOOL NETWORK_START_RESPAWN_SEARCH_IN_ANGLED_AREA_FOR_PLAYER(Player player, float x1, float y1, float z1, float x2, float y2, float z2, float width, float p8, float p9, float p10, int flags) {
			return Invoker::Invoke<BOOL, 5787>(player, x1, y1, z1, x2, y2, z2, width, p8, p9, p10, flags);
		}

		int NETWORK_QUERY_RESPAWN_RESULTS(Any* p0) {
			return Invoker::Invoke<int, 5788>(p0);
		}

		void NETWORK_CANCEL_RESPAWN_SEARCH() {
			Invoker::Invoke<void, 5789>();
		}

		void NETWORK_GET_RESPAWN_RESULT(int randomInt, Vector3* coordinates, float* heading) {
			Invoker::Invoke<void, 5790>(randomInt, coordinates, heading);
		}

		int NETWORK_GET_RESPAWN_RESULT_FLAGS(int p0) {
			return Invoker::Invoke<int, 5791>(p0);
		}

		void NETWORK_START_SOLO_TUTORIAL_SESSION() {
			Invoker::Invoke<void, 5792>();
		}

		void NETWORK_ALLOW_GANG_TO_JOIN_TUTORIAL_SESSION(int teamId, int instanceId) {
			Invoker::Invoke<void, 5793>(teamId, instanceId);
		}

		void NETWORK_END_TUTORIAL_SESSION() {
			Invoker::Invoke<void, 5794>();
		}

		BOOL NETWORK_IS_IN_TUTORIAL_SESSION() {
			return Invoker::Invoke<BOOL, 5795>();
		}

		BOOL NETWORK_WAITING_POP_CLEAR_TUTORIAL_SESSION() {
			return Invoker::Invoke<BOOL, 5796>();
		}

		BOOL NETWORK_IS_TUTORIAL_SESSION_CHANGE_PENDING() {
			return Invoker::Invoke<BOOL, 5797>();
		}

		int NETWORK_GET_PLAYER_TUTORIAL_SESSION_INSTANCE(Player player) {
			return Invoker::Invoke<int, 5798>(player);
		}

		BOOL NETWORK_ARE_PLAYERS_IN_SAME_TUTORIAL_SESSION(Player player, int index) {
			return Invoker::Invoke<BOOL, 5799>(player, index);
		}

		void NETWORK_BLOCK_PROXY_MIGRATION_BETWEEN_TUTORIAL_SESSIONS(Any p0) {
			Invoker::Invoke<void, 5800>(p0);
		}

		void NETWORK_CONCEAL_PLAYER(Player player, BOOL toggle, BOOL p2) {
			Invoker::Invoke<void, 5801>(player, toggle, p2);
		}

		BOOL NETWORK_IS_PLAYER_CONCEALED(Player player) {
			return Invoker::Invoke<BOOL, 5802>(player);
		}

		void NETWORK_CONCEAL_ENTITY(Entity entity, BOOL toggle) {
			Invoker::Invoke<void, 5803>(entity, toggle);
		}

		BOOL NETWORK_IS_ENTITY_CONCEALED(Entity entity) {
			return Invoker::Invoke<BOOL, 5804>(entity);
		}

		void NETWORK_OVERRIDE_CLOCK_TIME(int hours, int minutes, int seconds) {
			Invoker::Invoke<void, 5805>(hours, minutes, seconds);
		}

		void NETWORK_OVERRIDE_CLOCK_RATE(int ms) {
			Invoker::Invoke<void, 5806>(ms);
		}

		void NETWORK_CLEAR_CLOCK_TIME_OVERRIDE() {
			Invoker::Invoke<void, 5807>();
		}

		void _NETWORK_CLEAR_CLOCK_SYNC_TIME_OVERRIDE(BOOL startGlobalTransition, int transitionTime) {
			Invoker::Invoke<void, 6643>(startGlobalTransition, transitionTime);
		}

		void NETWORK_SYNC_CLOCK_TIME_OVERRIDE() {
			Invoker::Invoke<void, 6642>();
		}

		BOOL NETWORK_IS_CLOCK_TIME_OVERRIDDEN() {
			return Invoker::Invoke<BOOL, 5808>();
		}

		int NETWORK_ADD_ENTITY_AREA(float x1, float y1, float z1, float x2, float y2, float z2) {
			return Invoker::Invoke<int, 5809>(x1, y1, z1, x2, y2, z2);
		}

		int NETWORK_ADD_ENTITY_ANGLED_AREA(float x1, float y1, float z1, float x2, float y2, float z2, float width) {
			return Invoker::Invoke<int, 5810>(x1, y1, z1, x2, y2, z2, width);
		}

		int NETWORK_ADD_CLIENT_ENTITY_AREA(float x1, float y1, float z1, float x2, float y2, float z2) {
			return Invoker::Invoke<int, 5811>(x1, y1, z1, x2, y2, z2);
		}

		int NETWORK_ADD_CLIENT_ENTITY_ANGLED_AREA(float x1, float y1, float z1, float x2, float y2, float z2, float radius) {
			return Invoker::Invoke<int, 5812>(x1, y1, z1, x2, y2, z2, radius);
		}

		BOOL NETWORK_REMOVE_ENTITY_AREA(int areaHandle) {
			return Invoker::Invoke<BOOL, 5813>(areaHandle);
		}

		BOOL NETWORK_ENTITY_AREA_DOES_EXIST(int areaHandle) {
			return Invoker::Invoke<BOOL, 5814>(areaHandle);
		}

		BOOL NETWORK_ENTITY_AREA_HAVE_ALL_REPLIED(int areaHandle) {
			return Invoker::Invoke<BOOL, 5815>(areaHandle);
		}

		BOOL NETWORK_ENTITY_AREA_IS_OCCUPIED(int areaHandle) {
			return Invoker::Invoke<BOOL, 5816>(areaHandle);
		}

		void NETWORK_USE_HIGH_PRECISION_BLENDING(int netID, BOOL toggle) {
			Invoker::Invoke<void, 5817>(netID, toggle);
		}

		void NETWORK_SET_CUSTOM_ARENA_BALL_PARAMS(int netId) {
			Invoker::Invoke<void, 5818>(netId);
		}

		void NETWORK_ENTITY_USE_HIGH_PRECISION_ROTATION(int netId, BOOL toggle) {
			Invoker::Invoke<void, 5819>(netId, toggle);
		}

		BOOL NETWORK_REQUEST_CLOUD_BACKGROUND_SCRIPTS() {
			return Invoker::Invoke<BOOL, 5820>();
		}

		BOOL NETWORK_IS_CLOUD_BACKGROUND_SCRIPT_REQUEST_PENDING() {
			return Invoker::Invoke<BOOL, 5821>();
		}

		void NETWORK_REQUEST_CLOUD_TUNABLES() {
			Invoker::Invoke<void, 5822>();
		}

		BOOL NETWORK_IS_TUNABLE_CLOUD_REQUEST_PENDING() {
			return Invoker::Invoke<BOOL, 5823>();
		}

		int NETWORK_GET_TUNABLE_CLOUD_CRC() {
			return Invoker::Invoke<int, 5824>();
		}

		void _NETWORK_SET_TUNABLES_REGISTRATION_CONTEXTS(Any* tunableContextData) {
			Invoker::Invoke<void, 6578>(tunableContextData);
		}

		void _NETWORK_CLEAR_TUNABLES_REGISTRATION_CONTEXTS() {
			Invoker::Invoke<void, 6579>();
		}

		BOOL _NETWORK_GET_TUNABLES_REGISTRATION_BOOL(Hash tunableName, BOOL defaultValue) {
			return Invoker::Invoke<BOOL, 6580>(tunableName, defaultValue);
		}

		int _NETWORK_GET_TUNABLES_REGISTRATION_INT(Hash tunableName, int defaultValue) {
			return Invoker::Invoke<int, 6581>(tunableName, defaultValue);
		}

		float _NETWORK_GET_TUNABLES_REGISTRATION_FLOAT(Hash tunableName, float defaultValue) {
			return Invoker::Invoke<float, 6582>(tunableName, defaultValue);
		}

		BOOL NETWORK_DOES_TUNABLE_EXIST(const char* tunableContext, const char* tunableName) {
			return Invoker::Invoke<BOOL, 5825>(tunableContext, tunableName);
		}

		BOOL NETWORK_ACCESS_TUNABLE_INT(const char* tunableContext, const char* tunableName, int* value) {
			return Invoker::Invoke<BOOL, 5826>(tunableContext, tunableName, value);
		}

		BOOL NETWORK_ACCESS_TUNABLE_FLOAT(const char* tunableContext, const char* tunableName, float* value) {
			return Invoker::Invoke<BOOL, 5827>(tunableContext, tunableName, value);
		}

		BOOL NETWORK_ACCESS_TUNABLE_BOOL(const char* tunableContext, const char* tunableName) {
			return Invoker::Invoke<BOOL, 5828>(tunableContext, tunableName);
		}

		BOOL NETWORK_DOES_TUNABLE_EXIST_HASH(Hash tunableContext, Hash tunableName) {
			return Invoker::Invoke<BOOL, 5829>(tunableContext, tunableName);
		}

		BOOL NETWORK_ACCESS_TUNABLE_MODIFICATION_DETECTION_CLEAR() {
			return Invoker::Invoke<BOOL, 5830>();
		}

		BOOL NETWORK_ACCESS_TUNABLE_INT_HASH(Hash tunableContext, Hash tunableName, int* value) {
			return Invoker::Invoke<BOOL, 5831>(tunableContext, tunableName, value);
		}

		BOOL NETWORK_ACCESS_TUNABLE_INT_MODIFICATION_DETECTION_REGISTRATION_HASH(Hash contextHash, Hash nameHash, int* value) {
			return Invoker::Invoke<BOOL, 5832>(contextHash, nameHash, value);
		}

		BOOL NETWORK_ACCESS_TUNABLE_FLOAT_HASH(Hash tunableContext, Hash tunableName, float* value) {
			return Invoker::Invoke<BOOL, 5833>(tunableContext, tunableName, value);
		}

		BOOL NETWORK_ACCESS_TUNABLE_FLOAT_MODIFICATION_DETECTION_REGISTRATION_HASH(Hash contextHash, Hash nameHash, float* value) {
			return Invoker::Invoke<BOOL, 5834>(contextHash, nameHash, value);
		}

		BOOL NETWORK_ACCESS_TUNABLE_BOOL_HASH(Hash tunableContext, Hash tunableName) {
			return Invoker::Invoke<BOOL, 5835>(tunableContext, tunableName);
		}

		BOOL NETWORK_ACCESS_TUNABLE_BOOL_MODIFICATION_DETECTION_REGISTRATION_HASH(Hash contextHash, Hash nameHash, BOOL* value) {
			return Invoker::Invoke<BOOL, 5836>(contextHash, nameHash, value);
		}

		BOOL NETWORK_TRY_ACCESS_TUNABLE_BOOL_HASH(Hash tunableContext, Hash tunableName, BOOL defaultValue) {
			return Invoker::Invoke<BOOL, 5837>(tunableContext, tunableName, defaultValue);
		}

		int NETWORK_GET_CONTENT_MODIFIER_LIST_ID(Hash contentHash) {
			return Invoker::Invoke<int, 5838>(contentHash);
		}

		int NETWORK_GET_BONE_ID_OF_FATAL_HIT() {
			return Invoker::Invoke<int, 5839>();
		}

		void NETWORK_RESET_BODY_TRACKER() {
			Invoker::Invoke<void, 5840>();
		}

		int NETWORK_GET_NUMBER_BODY_TRACKER_HITS() {
			return Invoker::Invoke<int, 5841>();
		}

		BOOL NETWORK_HAS_BONE_BEEN_HIT_BY_KILLER(int boneIndex) {
			return Invoker::Invoke<BOOL, 5842>(boneIndex);
		}

		BOOL NETWORK_SET_ATTRIBUTE_DAMAGE_TO_PLAYER(Ped ped, Player player) {
			return Invoker::Invoke<BOOL, 5843>(ped, player);
		}

		void NETWORK_TRIGGER_DAMAGE_EVENT_FOR_ZERO_DAMAGE(Entity entity, BOOL toggle) {
			Invoker::Invoke<void, 5844>(entity, toggle);
		}

		void _NETWORK_TRIGGER_DAMAGE_EVENT_FOR_ZERO_PED_DAMAGE(Entity entity, BOOL trigger) {
			Invoker::Invoke<void, 6583>(entity, trigger);
		}

		void NETWORK_TRIGGER_DAMAGE_EVENT_FOR_ZERO_WEAPON_HASH(Entity entity, BOOL toggle) {
			Invoker::Invoke<void, 5845>(entity, toggle);
		}

		void NETWORK_SET_NO_LONGER_NEEDED(Entity entity, BOOL toggle) {
			Invoker::Invoke<void, 5846>(entity, toggle);
		}

		BOOL NETWORK_EXPLODE_VEHICLE(Vehicle vehicle, BOOL isAudible, BOOL isInvisible, int netId) {
			return Invoker::Invoke<BOOL, 5847>(vehicle, isAudible, isInvisible, netId);
		}

		void NETWORK_EXPLODE_HELI(Vehicle vehicle, BOOL isAudible, BOOL isInvisible, int netId) {
			Invoker::Invoke<void, 5848>(vehicle, isAudible, isInvisible, netId);
		}

		void NETWORK_USE_LOGARITHMIC_BLENDING_THIS_FRAME(Entity entity) {
			Invoker::Invoke<void, 5849>(entity);
		}

		void NETWORK_OVERRIDE_COORDS_AND_HEADING(Entity entity, float x, float y, float z, float heading) {
			Invoker::Invoke<void, 5850>(entity, x, y, z, heading);
		}

		void NETWORK_ENABLE_EXTRA_VEHICLE_ORIENTATION_BLEND_CHECKS(int netId, BOOL toggle) {
			Invoker::Invoke<void, 5851>(netId, toggle);
		}

		void NETWORK_DISABLE_PROXIMITY_MIGRATION(int netID) {
			Invoker::Invoke<void, 5852>(netID);
		}

		void NETWORK_SET_PLAYER_MENTAL_STATE(int p0) {
			Invoker::Invoke<void, 5855>(p0);
		}

		void NETWORK_SET_MINIMUM_RANK_FOR_MISSION(BOOL p0) {
			Invoker::Invoke<void, 5856>(p0);
		}

		void NETWORK_CACHE_LOCAL_PLAYER_HEAD_BLEND_DATA() {
			Invoker::Invoke<void, 5857>();
		}

		BOOL NETWORK_HAS_CACHED_PLAYER_HEAD_BLEND_DATA(Player player) {
			return Invoker::Invoke<BOOL, 5858>(player);
		}

		BOOL NETWORK_APPLY_CACHED_PLAYER_HEAD_BLEND_DATA(Ped ped, Player player) {
			return Invoker::Invoke<BOOL, 5859>(ped, player);
		}

		void _NETWORK_SET_IGNORE_VEHICLE_RAMMED_BY_NON_VEHICLE(BOOL toggle) {
			Invoker::Invoke<void, 6641>(toggle);
		}

		int GET_NUM_COMMERCE_ITEMS() {
			return Invoker::Invoke<int, 5860>();
		}

		BOOL IS_COMMERCE_DATA_VALID() {
			return Invoker::Invoke<BOOL, 5861>();
		}

		void TRIGGER_COMMERCE_DATA_FETCH(Any p0) {
			Invoker::Invoke<void, 5862>(p0);
		}

		BOOL IS_COMMERCE_DATA_FETCH_IN_PROGRESS() {
			return Invoker::Invoke<BOOL, 5863>();
		}

		const char* GET_COMMERCE_ITEM_ID(int index) {
			return Invoker::Invoke<const char*, 5864>(index);
		}

		const char* GET_COMMERCE_ITEM_NAME(int index) {
			return Invoker::Invoke<const char*, 5865>(index);
		}

		const char* GET_COMMERCE_PRODUCT_PRICE(int index) {
			return Invoker::Invoke<const char*, 5866>(index);
		}

		int GET_COMMERCE_ITEM_NUM_CATS(int index) {
			return Invoker::Invoke<int, 5867>(index);
		}

		const char* GET_COMMERCE_ITEM_CAT(int index, int index2) {
			return Invoker::Invoke<const char*, 5868>(index, index2);
		}

		void RESERVE_COMMERCE_STORE_PURCHASE_LOCATION(int location) {
			Invoker::Invoke<void, 6640>(location);
		}

		void OPEN_COMMERCE_STORE(const char* productID, const char* category, int location, BOOL launchLandingPageOnClose) {
			Invoker::Invoke<void, 5869>(productID, category, location, launchLandingPageOnClose);
		}

		void CHECKOUT_COMMERCE_PRODUCT(const char* productID, int location, BOOL launchLandingPageOnClose) {
			Invoker::Invoke<void, 6639>(productID, location, launchLandingPageOnClose);
		}

		BOOL IS_COMMERCE_STORE_OPEN() {
			return Invoker::Invoke<BOOL, 5870>();
		}

		void SET_STORE_ENABLED(BOOL toggle) {
			Invoker::Invoke<void, 5871>(toggle);
		}

		BOOL REQUEST_COMMERCE_ITEM_IMAGE(int index) {
			return Invoker::Invoke<BOOL, 5872>(index);
		}

		void RELEASE_ALL_COMMERCE_ITEM_IMAGES() {
			Invoker::Invoke<void, 5873>();
		}

		const char* GET_COMMERCE_ITEM_TEXTURENAME(int index) {
			return Invoker::Invoke<const char*, 5874>(index);
		}

		BOOL IS_STORE_AVAILABLE_TO_USER() {
			return Invoker::Invoke<BOOL, 5875>();
		}

		void DELAY_MP_STORE_OPEN() {
			Invoker::Invoke<void, 5876>();
		}

		void RESET_STORE_NETWORK_GAME_TRACKING() {
			Invoker::Invoke<void, 5877>();
		}

		BOOL IS_USER_OLD_ENOUGH_TO_ACCESS_STORE() {
			return Invoker::Invoke<BOOL, 5878>();
		}

		void SET_LAST_VIEWED_SHOP_ITEM(Hash p0, int p1, Hash p2) {
			Invoker::Invoke<void, 5879>(p0, p1, p2);
		}

		int GET_USER_PREMIUM_ACCESS() {
			return Invoker::Invoke<int, 5880>();
		}

		int GET_USER_STARTER_ACCESS() {
			return Invoker::Invoke<int, 5881>();
		}

		int CLOUD_DELETE_MEMBER_FILE(const char* p0) {
			return Invoker::Invoke<int, 5882>(p0);
		}

		BOOL CLOUD_HAS_REQUEST_COMPLETED(int requestId) {
			return Invoker::Invoke<BOOL, 5883>(requestId);
		}

		BOOL CLOUD_DID_REQUEST_SUCCEED(int requestId) {
			return Invoker::Invoke<BOOL, 5884>(requestId);
		}

		void CLOUD_CHECK_AVAILABILITY() {
			Invoker::Invoke<void, 5885>();
		}

		BOOL CLOUD_IS_CHECKING_AVAILABILITY() {
			return Invoker::Invoke<BOOL, 5886>();
		}

		BOOL CLOUD_GET_AVAILABILITY_CHECK_RESULT() {
			return Invoker::Invoke<BOOL, 5887>();
		}

		int GET_CONTENT_TO_LOAD_TYPE() {
			return Invoker::Invoke<int, 5888>();
		}

		BOOL GET_IS_LAUNCH_FROM_LIVE_AREA() {
			return Invoker::Invoke<BOOL, 5889>();
		}

		BOOL GET_IS_LIVE_AREA_LAUNCH_WITH_CONTENT() {
			return Invoker::Invoke<BOOL, 5890>();
		}

		void CLEAR_SERVICE_EVENT_ARGUMENTS() {
			Invoker::Invoke<void, 5891>();
		}

		BOOL UGC_COPY_CONTENT(Any* p0, Any* p1) {
			return Invoker::Invoke<BOOL, 5892>(p0, p1);
		}

		BOOL UGC_IS_CREATING() {
			return Invoker::Invoke<BOOL, 5893>();
		}

		BOOL UGC_HAS_CREATE_FINISHED() {
			return Invoker::Invoke<BOOL, 5894>();
		}

		BOOL UGC_DID_CREATE_SUCCEED() {
			return Invoker::Invoke<BOOL, 5895>();
		}

		int UGC_GET_CREATE_RESULT() {
			return Invoker::Invoke<int, 5896>();
		}

		const char* UGC_GET_CREATE_CONTENT_ID() {
			return Invoker::Invoke<const char*, 5897>();
		}

		void UGC_CLEAR_CREATE_RESULT() {
			Invoker::Invoke<void, 5898>();
		}

		BOOL UGC_QUERY_MY_CONTENT(Any p0, Any p1, Any* p2, Any p3, Any p4, Any p5) {
			return Invoker::Invoke<BOOL, 5899>(p0, p1, p2, p3, p4, p5);
		}

		BOOL UGC_QUERY_BY_CATEGORY(Any p0, Any p1, Any p2, const char* p3, Any p4, BOOL p5) {
			return Invoker::Invoke<BOOL, 5900>(p0, p1, p2, p3, p4, p5);
		}

		BOOL UGC_QUERY_BY_CONTENT_ID(const char* contentId, BOOL latestVersion, const char* contentTypeName) {
			return Invoker::Invoke<BOOL, 5901>(contentId, latestVersion, contentTypeName);
		}

		BOOL UGC_QUERY_BY_CONTENT_IDS(Any* data, int count, BOOL latestVersion, const char* contentTypeName) {
			return Invoker::Invoke<BOOL, 5902>(data, count, latestVersion, contentTypeName);
		}

		BOOL UGC_QUERY_MOST_RECENTLY_CREATED_CONTENT(int offset, int count, const char* contentTypeName, int p3) {
			return Invoker::Invoke<BOOL, 5903>(offset, count, contentTypeName, p3);
		}

		BOOL UGC_GET_BOOKMARKED_CONTENT(Any p0, Any p1, const char* p2, Any* p3) {
			return Invoker::Invoke<BOOL, 5904>(p0, p1, p2, p3);
		}

		BOOL UGC_GET_MY_CONTENT(Any p0, Any p1, const char* p2, Any* p3) {
			return Invoker::Invoke<BOOL, 5905>(p0, p1, p2, p3);
		}

		BOOL UGC_GET_FRIEND_CONTENT(Any p0, Any p1, const char* p2, Any* p3) {
			return Invoker::Invoke<BOOL, 5906>(p0, p1, p2, p3);
		}

		BOOL UGC_GET_CREW_CONTENT(Any p0, Any p1, Any p2, const char* p3, Any* p4) {
			return Invoker::Invoke<BOOL, 5907>(p0, p1, p2, p3, p4);
		}

		BOOL UGC_GET_GET_BY_CATEGORY(Any p0, Any p1, Any p2, const char* p3, Any* p4) {
			return Invoker::Invoke<BOOL, 5908>(p0, p1, p2, p3, p4);
		}

		BOOL UGC_GET_GET_BY_CONTENT_ID(const char* contentId, const char* contentTypeName) {
			return Invoker::Invoke<BOOL, 5909>(contentId, contentTypeName);
		}

		BOOL UGC_GET_GET_BY_CONTENT_IDS(Any* data, int dataCount, const char* contentTypeName) {
			return Invoker::Invoke<BOOL, 5910>(data, dataCount, contentTypeName);
		}

		BOOL UGC_GET_MOST_RECENTLY_CREATED_CONTENT(Any p0, Any p1, Any* p2, Any* p3) {
			return Invoker::Invoke<BOOL, 5911>(p0, p1, p2, p3);
		}

		BOOL UGC_GET_MOST_RECENTLY_PLAYED_CONTENT(Any p0, Any p1, Any* p2, Any* p3) {
			return Invoker::Invoke<BOOL, 5912>(p0, p1, p2, p3);
		}

		BOOL UGC_GET_TOP_RATED_CONTENT(Any p0, Any p1, Any* p2, Any* p3) {
			return Invoker::Invoke<BOOL, 5913>(p0, p1, p2, p3);
		}

		void UGC_CANCEL_QUERY() {
			Invoker::Invoke<void, 5914>();
		}

		BOOL UGC_IS_GETTING() {
			return Invoker::Invoke<BOOL, 5915>();
		}

		BOOL UGC_HAS_GET_FINISHED() {
			return Invoker::Invoke<BOOL, 5916>();
		}

		BOOL UGC_DID_GET_SUCCEED() {
			return Invoker::Invoke<BOOL, 5917>();
		}

		BOOL UGC_WAS_QUERY_FORCE_CANCELLED() {
			return Invoker::Invoke<BOOL, 5918>();
		}

		int UGC_GET_QUERY_RESULT() {
			return Invoker::Invoke<int, 5919>();
		}

		int UGC_GET_CONTENT_NUM() {
			return Invoker::Invoke<int, 5920>();
		}

		int UGC_GET_CONTENT_TOTAL() {
			return Invoker::Invoke<int, 5921>();
		}

		Hash UGC_GET_CONTENT_HASH() {
			return Invoker::Invoke<Hash, 5922>();
		}

		void UGC_CLEAR_QUERY_RESULTS() {
			Invoker::Invoke<void, 5923>();
		}

		const char* UGC_GET_CONTENT_USER_ID(int p0) {
			return Invoker::Invoke<const char*, 5924>(p0);
		}

		BOOL UGC_GET_CONTENT_CREATOR_GAMER_HANDLE(int p0, Any* p1) {
			return Invoker::Invoke<BOOL, 5925>(p0, p1);
		}

		BOOL UGC_GET_CONTENT_CREATED_BY_LOCAL_PLAYER(Any p0) {
			return Invoker::Invoke<BOOL, 5926>(p0);
		}

		const char* UGC_GET_CONTENT_USER_NAME(Any p0) {
			return Invoker::Invoke<const char*, 5927>(p0);
		}

		BOOL UGC_GET_CONTENT_IS_USING_SC_NICKNAME(Any p0) {
			return Invoker::Invoke<BOOL, 5928>(p0);
		}

		int UGC_GET_CONTENT_CATEGORY(int p0) {
			return Invoker::Invoke<int, 5929>(p0);
		}

		const char* UGC_GET_CONTENT_ID(int p0) {
			return Invoker::Invoke<const char*, 5930>(p0);
		}

		const char* UGC_GET_ROOT_CONTENT_ID(int p0) {
			return Invoker::Invoke<const char*, 5931>(p0);
		}

		const char* UGC_GET_CONTENT_NAME(Any p0) {
			return Invoker::Invoke<const char*, 5932>(p0);
		}

		const char* UGC_GET_CONTENT_DESCRIPTION(int index) {
			return Invoker::Invoke<const char*, 6638>(index);
		}

		int UGC_GET_CONTENT_DESCRIPTION_HASH(Any p0) {
			return Invoker::Invoke<int, 5933>(p0);
		}

		const char* UGC_GET_CONTENT_PATH(int p0, int p1) {
			return Invoker::Invoke<const char*, 5934>(p0, p1);
		}

		void UGC_GET_CONTENT_UPDATED_DATE(Any p0, Any* p1) {
			Invoker::Invoke<void, 5935>(p0, p1);
		}

		int UGC_GET_CONTENT_FILE_VERSION(Any p0, Any p1) {
			return Invoker::Invoke<int, 5936>(p0, p1);
		}

		BOOL UGC_GET_CONTENT_HAS_LO_RES_PHOTO(int p0) {
			return Invoker::Invoke<BOOL, 5937>(p0);
		}

		BOOL UGC_GET_CONTENT_HAS_HI_RES_PHOTO(int p0) {
			return Invoker::Invoke<BOOL, 5938>(p0);
		}

		int UGC_GET_CONTENT_LANGUAGE(Any p0) {
			return Invoker::Invoke<int, 5939>(p0);
		}

		BOOL UGC_GET_CONTENT_IS_PUBLISHED(Any p0) {
			return Invoker::Invoke<BOOL, 5940>(p0);
		}

		BOOL UGC_GET_CONTENT_IS_VERIFIED(Any p0) {
			return Invoker::Invoke<BOOL, 5941>(p0);
		}

		float UGC_GET_CONTENT_RATING(Any p0, Any p1) {
			return Invoker::Invoke<float, 5942>(p0, p1);
		}

		int UGC_GET_CONTENT_RATING_COUNT(Any p0, Any p1) {
			return Invoker::Invoke<int, 5943>(p0, p1);
		}

		int UGC_GET_CONTENT_RATING_POSITIVE_COUNT(Any p0, Any p1) {
			return Invoker::Invoke<int, 5944>(p0, p1);
		}

		int UGC_GET_CONTENT_RATING_NEGATIVE_COUNT(Any p0, Any p1) {
			return Invoker::Invoke<int, 5945>(p0, p1);
		}

		BOOL UGC_GET_CONTENT_HAS_PLAYER_RECORD(Any p0) {
			return Invoker::Invoke<BOOL, 5946>(p0);
		}

		BOOL UGC_GET_CONTENT_HAS_PLAYER_BOOKMARKED(Any p0) {
			return Invoker::Invoke<BOOL, 5947>(p0);
		}

		int UGC_REQUEST_CONTENT_DATA_FROM_INDEX(int p0, int p1) {
			return Invoker::Invoke<int, 5948>(p0, p1);
		}

		int UGC_REQUEST_CONTENT_DATA_FROM_PARAMS(const char* contentTypeName, const char* contentId, int p2, int p3, int p4) {
			return Invoker::Invoke<int, 5949>(contentTypeName, contentId, p2, p3, p4);
		}

		int UGC_REQUEST_CACHED_DESCRIPTION(int p0) {
			return Invoker::Invoke<int, 5950>(p0);
		}

		BOOL UGC_IS_DESCRIPTION_REQUEST_IN_PROGRESS(Any p0) {
			return Invoker::Invoke<BOOL, 5951>(p0);
		}

		BOOL UGC_HAS_DESCRIPTION_REQUEST_FINISHED(Any p0) {
			return Invoker::Invoke<BOOL, 5952>(p0);
		}

		BOOL UGC_DID_DESCRIPTION_REQUEST_SUCCEED(Any p0) {
			return Invoker::Invoke<BOOL, 5953>(p0);
		}

		const char* UGC_GET_CACHED_DESCRIPTION(Any p0, Any p1) {
			return Invoker::Invoke<const char*, 5954>(p0, p1);
		}

		BOOL UGC_RELEASE_CACHED_DESCRIPTION(Any p0) {
			return Invoker::Invoke<BOOL, 5955>(p0);
		}

		void UGC_RELEASE_ALL_CACHED_DESCRIPTIONS() {
			Invoker::Invoke<void, 5956>();
		}

		BOOL UGC_HAS_PERMISSION_TO_WRITE() {
			return Invoker::Invoke<BOOL, 5957>();
		}

		BOOL UGC_PUBLISH(const char* contentId, const char* baseContentId, const char* contentTypeName) {
			return Invoker::Invoke<BOOL, 5958>(contentId, baseContentId, contentTypeName);
		}

		BOOL UGC_SET_BOOKMARKED(const char* contentId, BOOL bookmarked, const char* contentTypeName) {
			return Invoker::Invoke<BOOL, 5959>(contentId, bookmarked, contentTypeName);
		}

		BOOL UGC_SET_DELETED(Any* p0, BOOL p1, const char* p2) {
			return Invoker::Invoke<BOOL, 5960>(p0, p1, p2);
		}

		BOOL UGC_IS_MODIFYING() {
			return Invoker::Invoke<BOOL, 5961>();
		}

		BOOL UGC_HAS_MODIFY_FINISHED() {
			return Invoker::Invoke<BOOL, 5962>();
		}

		BOOL UGC_DID_MODIFY_SUCCEED() {
			return Invoker::Invoke<BOOL, 5963>();
		}

		int UGC_GET_MODIFY_RESULT() {
			return Invoker::Invoke<int, 5964>();
		}

		void UGC_CLEAR_MODIFY_RESULT() {
			Invoker::Invoke<void, 5965>();
		}

		BOOL UGC_GET_CREATORS_BY_USER_ID(Any* p0, Any* p1) {
			return Invoker::Invoke<BOOL, 5966>(p0, p1);
		}

		BOOL UGC_HAS_QUERY_CREATORS_FINISHED() {
			return Invoker::Invoke<BOOL, 5967>();
		}

		BOOL UGC_DID_QUERY_CREATORS_SUCCEED() {
			return Invoker::Invoke<BOOL, 5968>();
		}

		int UGC_GET_CREATOR_NUM() {
			return Invoker::Invoke<int, 5969>();
		}

		BOOL UGC_LOAD_OFFLINE_QUERY(Any p0) {
			return Invoker::Invoke<BOOL, 5970>(p0);
		}

		void UGC_CLEAR_OFFLINE_QUERY() {
			Invoker::Invoke<void, 5971>();
		}

		void UGC_SET_QUERY_DATA_FROM_OFFLINE(BOOL p0) {
			Invoker::Invoke<void, 5972>(p0);
		}

		void UGC_SET_USING_OFFLINE_CONTENT(BOOL p0) {
			Invoker::Invoke<void, 5973>(p0);
		}

		BOOL UGC_IS_LANGUAGE_SUPPORTED(Any p0) {
			return Invoker::Invoke<BOOL, 5974>(p0);
		}

		BOOL FACEBOOK_POST_COMPLETED_HEIST(const char* heistName, int cashEarned, int xpEarned) {
			return Invoker::Invoke<BOOL, 5975>(heistName, cashEarned, xpEarned);
		}

		BOOL FACEBOOK_POST_CREATE_CHARACTER() {
			return Invoker::Invoke<BOOL, 5976>();
		}

		BOOL FACEBOOK_POST_COMPLETED_MILESTONE(int milestoneId) {
			return Invoker::Invoke<BOOL, 5977>(milestoneId);
		}

		BOOL FACEBOOK_HAS_POST_COMPLETED() {
			return Invoker::Invoke<BOOL, 5978>();
		}

		BOOL FACEBOOK_DID_POST_SUCCEED() {
			return Invoker::Invoke<BOOL, 5979>();
		}

		BOOL FACEBOOK_CAN_POST_TO_FACEBOOK() {
			return Invoker::Invoke<BOOL, 5980>();
		}

		int TEXTURE_DOWNLOAD_REQUEST(Any* gamerHandle, const char* filePath, const char* name, BOOL p3) {
			return Invoker::Invoke<int, 5981>(gamerHandle, filePath, name, p3);
		}

		int TITLE_TEXTURE_DOWNLOAD_REQUEST(const char* filePath, const char* name, BOOL p2) {
			return Invoker::Invoke<int, 5982>(filePath, name, p2);
		}

		int UGC_TEXTURE_DOWNLOAD_REQUEST(const char* p0, int p1, int p2, int p3, const char* p4, BOOL p5) {
			return Invoker::Invoke<int, 5983>(p0, p1, p2, p3, p4, p5);
		}

		void TEXTURE_DOWNLOAD_RELEASE(int p0) {
			Invoker::Invoke<void, 5984>(p0);
		}

		BOOL TEXTURE_DOWNLOAD_HAS_FAILED(int p0) {
			return Invoker::Invoke<BOOL, 5985>(p0);
		}

		const char* TEXTURE_DOWNLOAD_GET_NAME(int p0) {
			return Invoker::Invoke<const char*, 5986>(p0);
		}

		int GET_STATUS_OF_TEXTURE_DOWNLOAD(int p0) {
			return Invoker::Invoke<int, 5987>(p0);
		}

		BOOL NETWORK_CHECK_ROS_LINK_WENTDOWN_NOT_NET() {
			return Invoker::Invoke<BOOL, 5988>();
		}

		BOOL NETWORK_SHOULD_SHOW_STRICT_NAT_WARNING() {
			return Invoker::Invoke<BOOL, 5989>();
		}

		BOOL NETWORK_IS_CABLE_CONNECTED() {
			return Invoker::Invoke<BOOL, 5990>();
		}

		BOOL NETWORK_HAVE_SCS_PRIVATE_MSG_PRIV() {
			return Invoker::Invoke<BOOL, 5991>();
		}

		BOOL NETWORK_HAVE_ROS_SOCIAL_CLUB_PRIV() {
			return Invoker::Invoke<BOOL, 5992>();
		}

		BOOL NETWORK_HAVE_ROS_BANNED_PRIV() {
			return Invoker::Invoke<BOOL, 5993>();
		}

		BOOL NETWORK_HAVE_ROS_CREATE_TICKET_PRIV() {
			return Invoker::Invoke<BOOL, 5994>();
		}

		BOOL NETWORK_HAVE_ROS_MULTIPLAYER_PRIV() {
			return Invoker::Invoke<BOOL, 5995>();
		}

		BOOL NETWORK_HAVE_ROS_LEADERBOARD_WRITE_PRIV() {
			return Invoker::Invoke<BOOL, 5996>();
		}

		BOOL NETWORK_HAS_ROS_PRIVILEGE(int index) {
			return Invoker::Invoke<BOOL, 5997>(index);
		}

		BOOL NETWORK_HAS_ROS_PRIVILEGE_END_DATE(int privilege, int* banType, Any* timeData) {
			return Invoker::Invoke<BOOL, 5998>(privilege, banType, timeData);
		}

		BOOL NETWORK_HAS_ROS_PRIVILEGE_PLAYED_LAST_GEN() {
			return Invoker::Invoke<BOOL, 5999>();
		}

		BOOL NETWORK_HAS_ROS_PRIVILEGE_SPECIAL_EDITION_CONTENT() {
			return Invoker::Invoke<BOOL, 6000>();
		}

		BOOL _NETWORK_HAS_ROS_PRIVILEGE_MP_TEXT_COMMUNICATION() {
			return Invoker::Invoke<BOOL, 6001>();
		}

		BOOL _NETWORK_HAS_ROS_PRIVILEGE_MP_VOICE_COMMUNICATION() {
			return Invoker::Invoke<BOOL, 6002>();
		}

		BOOL _NETWORK_HAS_ROS_PRIVILEGE_REPORTING() {
			return Invoker::Invoke<BOOL, 6584>();
		}

		int NETWORK_START_USER_CONTENT_PERMISSIONS_CHECK(Any* netHandle) {
			return Invoker::Invoke<int, 6004>(netHandle);
		}

		void NETWORK_SKIP_RADIO_RESET_NEXT_CLOSE() {
			Invoker::Invoke<void, 6005>();
		}

		void NETWORK_SKIP_RADIO_RESET_NEXT_OPEN() {
			Invoker::Invoke<void, 6006>();
		}

		BOOL NETWORK_SKIP_RADIO_WARNING() {
			return Invoker::Invoke<BOOL, 6007>();
		}

		void NETWORK_FORCE_LOCAL_PLAYER_SCAR_SYNC() {
			Invoker::Invoke<void, 6008>();
		}

		void NETWORK_DISABLE_LEAVE_REMOTE_PED_BEHIND(BOOL toggle) {
			Invoker::Invoke<void, 6009>(toggle);
		}

		void NETWORK_ALLOW_REMOTE_ATTACHMENT_MODIFICATION(Entity entity, BOOL toggle) {
			Invoker::Invoke<void, 6010>(entity, toggle);
		}

		void NETWORK_SHOW_CHAT_RESTRICTION_MSC(Player player) {
			Invoker::Invoke<void, 6011>(player);
		}

		void NETWORK_SHOW_PSN_UGC_RESTRICTION() {
			Invoker::Invoke<void, 6012>();
		}

		BOOL NETWORK_IS_TITLE_UPDATE_REQUIRED() {
			return Invoker::Invoke<BOOL, 6013>();
		}

		void NETWORK_QUIT_MP_TO_DESKTOP() {
			Invoker::Invoke<void, 6014>();
		}

		BOOL NETWORK_IS_CONNECTED_VIA_RELAY(Player player) {
			return Invoker::Invoke<BOOL, 6015>(player);
		}

		float NETWORK_GET_AVERAGE_LATENCY(Player player) {
			return Invoker::Invoke<float, 6016>(player);
		}

		float NETWORK_GET_AVERAGE_PING(Player player) {
			return Invoker::Invoke<float, 6017>(player);
		}

		float NETWORK_GET_AVERAGE_PACKET_LOSS(Player player) {
			return Invoker::Invoke<float, 6018>(player);
		}

		int NETWORK_GET_NUM_UNACKED_RELIABLES(Player player) {
			return Invoker::Invoke<int, 6019>(player);
		}

		int NETWORK_GET_UNRELIABLE_RESEND_COUNT(Player player) {
			return Invoker::Invoke<int, 6020>(player);
		}

		int NETWORK_GET_HIGHEST_RELIABLE_RESEND_COUNT(Player player) {
			return Invoker::Invoke<int, 6021>(player);
		}

		void NETWORK_REPORT_CODE_TAMPER() {
			Invoker::Invoke<void, 6022>();
		}

		void _NETWORK_GET_DUMP_OF_ASSET_VERIFIER(Any* p0) {
			Invoker::Invoke<void, 6637>(p0);
		}

		Vector3 NETWORK_GET_LAST_ENTITY_POS_RECEIVED_OVER_NETWORK(Entity entity) {
			return Invoker::Invoke<Vector3, 6023>(entity);
		}

		Vector3 NETWORK_GET_LAST_PLAYER_POS_RECEIVED_OVER_NETWORK(Player player) {
			return Invoker::Invoke<Vector3, 6024>(player);
		}

		Vector3 NETWORK_GET_LAST_VEL_RECEIVED_OVER_NETWORK(Entity entity) {
			return Invoker::Invoke<Vector3, 6025>(entity);
		}

		Vector3 NETWORK_GET_PREDICTED_VELOCITY(Entity entity, float maxSpeedToPredict) {
			return Invoker::Invoke<Vector3, 6026>(entity, maxSpeedToPredict);
		}

		void NETWORK_DUMP_NET_IF_CONFIG() {
			Invoker::Invoke<void, 6027>();
		}

		void NETWORK_GET_SIGNALLING_INFO(Any* p0) {
			Invoker::Invoke<void, 6028>(p0);
		}

		void NETWORK_GET_NET_STATISTICS_INFO(Any* p0) {
			Invoker::Invoke<void, 6029>(p0);
		}

		int NETWORK_GET_PLAYER_ACCOUNT_ID(Player player) {
			return Invoker::Invoke<int, 6030>(player);
		}

		void NETWORK_POST_UDS_ACTIVITY_START(const char* activityId) {
			Invoker::Invoke<void, 6636>(activityId);
		}

		void NETWORK_POST_UDS_ACTIVITY_END(const char* activityId, int iOutcome, int iScore) {
			Invoker::Invoke<void, 6635>(activityId, iOutcome, iScore);
		}

		void NETWORK_POST_UDS_ACTIVITY_RESUME(const char* activityId) {
			Invoker::Invoke<void, 6634>(activityId);
		}

		void NETWORK_POST_UDS_ACTIVITY_RESUME_WITH_TASKS(const char* activityId, Any* taskStatus) {
			Invoker::Invoke<void, 6633>(activityId, taskStatus);
		}

		void NETWORK_POST_UDS_ACTIVITY_AVAILABILITY_CHANGE(Any* availablityData) {
			Invoker::Invoke<void, 6632>(availablityData);
		}

		void NETWORK_POST_UDS_ACTIVITY_TERMINATE() {
			Invoker::Invoke<void, 6631>();
		}

		BOOL NETWORK_HAS_SC_MEMBERSHIP_INFO() {
			return Invoker::Invoke<BOOL, 6630>();
		}

		BOOL NETWORK_HAS_SC_MEMBERSHIP() {
			return Invoker::Invoke<BOOL, 6629>();
		}

		void NETWORK_GET_SC_MEMBERSHIP_INFO(Any* info) {
			Invoker::Invoke<void, 6628>(info);
		}

		void NETWORK_UGC_NAV(Any p0, Any p1) {
			Invoker::Invoke<void, 6031>(p0, p1);
		}

		int _NETWORK_GET_GAME_RESTART_REASON() {
			return Invoker::Invoke<int, 6595>();
		}

		void _NETWORK_CONFIRM_GAME_RESTART() {
			Invoker::Invoke<void, 6627>();
		}

		const char* _NETWORK_GET_GAME_RESTART_REASON_MESSAGE_LABEL() {
			return Invoker::Invoke<const char*, 6626>();
		}

		const char* _NETWORK_GET_BATTLEYE_ERROR_MESSAGE_LABEL(int errorCode) {
			return Invoker::Invoke<const char*, 6625>(errorCode);
		}

	} // namespace NETWORK

	namespace OBJECT {

		Object CREATE_OBJECT(Hash modelHash, float x, float y, float z, BOOL isNetwork, BOOL bScriptHostObj, BOOL dynamic) {
			return Invoker::Invoke<Object, 103>(modelHash, x, y, z, isNetwork, bScriptHostObj, dynamic);
		}

		Object CREATE_OBJECT_NO_OFFSET(Hash modelHash, float x, float y, float z, BOOL isNetwork, BOOL bScriptHostObj, BOOL dynamic, Any p7) {
			return Invoker::Invoke<Object, 104>(modelHash, x, y, z, isNetwork, bScriptHostObj, dynamic, p7);
		}

		void DELETE_OBJECT(Object* object) {
			Invoker::Invoke<void, 105>(object);
		}

		BOOL PLACE_OBJECT_ON_GROUND_PROPERLY(Object object) {
			return Invoker::Invoke<BOOL, 106>(object);
		}

		BOOL PLACE_OBJECT_ON_GROUND_OR_OBJECT_PROPERLY(Object object) {
			return Invoker::Invoke<BOOL, 107>(object);
		}

		BOOL ROTATE_OBJECT(Object object, float p1, float p2, BOOL p3) {
			return Invoker::Invoke<BOOL, 108>(object, p1, p2, p3);
		}

		BOOL SLIDE_OBJECT(Object object, float toX, float toY, float toZ, float speedX, float speedY, float speedZ, BOOL collision) {
			return Invoker::Invoke<BOOL, 109>(object, toX, toY, toZ, speedX, speedY, speedZ, collision);
		}

		void SET_OBJECT_TARGETTABLE(Object object, BOOL targettable, Any p2) {
			Invoker::Invoke<void, 110>(object, targettable, p2);
		}

		void _SET_OBJECT_TARGETTABLE_BY_PLAYER(Object object, BOOL setFlag34, BOOL setFlag35) {
			Invoker::Invoke<void, 6585>(object, setFlag34, setFlag35);
		}

		void SET_OBJECT_FORCE_VEHICLES_TO_AVOID(Object object, BOOL toggle) {
			Invoker::Invoke<void, 111>(object, toggle);
		}

		Object GET_CLOSEST_OBJECT_OF_TYPE(float x, float y, float z, float radius, Hash modelHash, BOOL isMission, BOOL p6, BOOL p7) {
			return Invoker::Invoke<Object, 112>(x, y, z, radius, modelHash, isMission, p6, p7);
		}

		BOOL HAS_OBJECT_BEEN_BROKEN(Object object, Any p1) {
			return Invoker::Invoke<BOOL, 113>(object, p1);
		}

		BOOL HAS_CLOSEST_OBJECT_OF_TYPE_BEEN_BROKEN(float p0, float p1, float p2, float p3, Hash modelHash, Any p5) {
			return Invoker::Invoke<BOOL, 114>(p0, p1, p2, p3, modelHash, p5);
		}

		BOOL HAS_CLOSEST_OBJECT_OF_TYPE_BEEN_COMPLETELY_DESTROYED(float x, float y, float z, float radius, Hash modelHash, BOOL p5) {
			return Invoker::Invoke<BOOL, 115>(x, y, z, radius, modelHash, p5);
		}

		BOOL GET_HAS_OBJECT_BEEN_COMPLETELY_DESTROYED(Any p0) {
			return Invoker::Invoke<BOOL, 116>(p0);
		}

		Vector3 GET_OFFSET_FROM_COORD_AND_HEADING_IN_WORLD_COORDS(float xPos, float yPos, float zPos, float heading, float xOffset, float yOffset, float zOffset) {
			return Invoker::Invoke<Vector3, 117>(xPos, yPos, zPos, heading, xOffset, yOffset, zOffset);
		}

		BOOL GET_COORDS_AND_ROTATION_OF_CLOSEST_OBJECT_OF_TYPE(float x, float y, float z, float radius, Hash modelHash, Vector3* outPosition, Vector3* outRotation, int rotationOrder) {
			return Invoker::Invoke<BOOL, 118>(x, y, z, radius, modelHash, outPosition, outRotation, rotationOrder);
		}

		void SET_STATE_OF_CLOSEST_DOOR_OF_TYPE(Hash type, float x, float y, float z, BOOL locked, float heading, BOOL p6) {
			Invoker::Invoke<void, 119>(type, x, y, z, locked, heading, p6);
		}

		void GET_STATE_OF_CLOSEST_DOOR_OF_TYPE(Hash type, float x, float y, float z, BOOL* locked, float* heading) {
			Invoker::Invoke<void, 120>(type, x, y, z, locked, heading);
		}

		void SET_LOCKED_UNSTREAMED_IN_DOOR_OF_TYPE(Hash modelHash, float x, float y, float z, BOOL locked, float xRotMult, float yRotMult, float zRotMult) {
			Invoker::Invoke<void, 121>(modelHash, x, y, z, locked, xRotMult, yRotMult, zRotMult);
		}

		void PLAY_OBJECT_AUTO_START_ANIM(Any p0) {
			Invoker::Invoke<void, 122>(p0);
		}

		void ADD_DOOR_TO_SYSTEM(Hash doorHash, Hash modelHash, float x, float y, float z, BOOL p5, BOOL scriptDoor, BOOL isLocal, Any p8) {
			Invoker::Invoke<void, 123>(doorHash, modelHash, x, y, z, p5, scriptDoor, isLocal, p8);
		}

		void REMOVE_DOOR_FROM_SYSTEM(Hash doorHash, Any p1) {
			Invoker::Invoke<void, 124>(doorHash, p1);
		}

		void DOOR_SYSTEM_SET_DOOR_STATE(Hash doorHash, int state, BOOL requestDoor, BOOL forceUpdate) {
			Invoker::Invoke<void, 125>(doorHash, state, requestDoor, forceUpdate);
		}

		int DOOR_SYSTEM_GET_DOOR_STATE(Hash doorHash) {
			return Invoker::Invoke<int, 126>(doorHash);
		}

		int DOOR_SYSTEM_GET_DOOR_PENDING_STATE(Hash doorHash) {
			return Invoker::Invoke<int, 127>(doorHash);
		}

		void DOOR_SYSTEM_SET_AUTOMATIC_RATE(Hash doorHash, float rate, BOOL requestDoor, BOOL forceUpdate) {
			Invoker::Invoke<void, 128>(doorHash, rate, requestDoor, forceUpdate);
		}

		void DOOR_SYSTEM_SET_AUTOMATIC_DISTANCE(Hash doorHash, float distance, BOOL requestDoor, BOOL forceUpdate) {
			Invoker::Invoke<void, 129>(doorHash, distance, requestDoor, forceUpdate);
		}

		void DOOR_SYSTEM_SET_OPEN_RATIO(Hash doorHash, float ajar, BOOL requestDoor, BOOL forceUpdate) {
			Invoker::Invoke<void, 130>(doorHash, ajar, requestDoor, forceUpdate);
		}

		float DOOR_SYSTEM_GET_AUTOMATIC_DISTANCE(Hash doorHash) {
			return Invoker::Invoke<float, 131>(doorHash);
		}

		float DOOR_SYSTEM_GET_OPEN_RATIO(Hash doorHash) {
			return Invoker::Invoke<float, 132>(doorHash);
		}

		BOOL DOOR_SYSTEM_GET_IS_SPRING_REMOVED(Hash doorHash) {
			return Invoker::Invoke<BOOL, 6624>(doorHash);
		}

		void DOOR_SYSTEM_SET_SPRING_REMOVED(Hash doorHash, BOOL removed, BOOL requestDoor, BOOL forceUpdate) {
			Invoker::Invoke<void, 133>(doorHash, removed, requestDoor, forceUpdate);
		}

		void DOOR_SYSTEM_SET_HOLD_OPEN(Hash doorHash, BOOL toggle) {
			Invoker::Invoke<void, 134>(doorHash, toggle);
		}

		void DOOR_SYSTEM_SET_DOOR_OPEN_FOR_RACES(Hash doorHash, BOOL p1) {
			Invoker::Invoke<void, 135>(doorHash, p1);
		}

		BOOL IS_DOOR_REGISTERED_WITH_SYSTEM(Hash doorHash) {
			return Invoker::Invoke<BOOL, 136>(doorHash);
		}

		BOOL IS_DOOR_CLOSED(Hash doorHash) {
			return Invoker::Invoke<BOOL, 137>(doorHash);
		}

		void OPEN_ALL_BARRIERS_FOR_RACE(BOOL p0) {
			Invoker::Invoke<void, 138>(p0);
		}

		void CLOSE_ALL_BARRIERS_FOR_RACE() {
			Invoker::Invoke<void, 139>();
		}

		BOOL DOOR_SYSTEM_GET_IS_PHYSICS_LOADED(Any p0) {
			return Invoker::Invoke<BOOL, 140>(p0);
		}

		BOOL DOOR_SYSTEM_FIND_EXISTING_DOOR(float x, float y, float z, Hash modelHash, Hash* outDoorHash) {
			return Invoker::Invoke<BOOL, 141>(x, y, z, modelHash, outDoorHash);
		}

		BOOL IS_GARAGE_EMPTY(Hash garageHash, BOOL p1, int p2) {
			return Invoker::Invoke<BOOL, 142>(garageHash, p1, p2);
		}

		BOOL IS_PLAYER_ENTIRELY_INSIDE_GARAGE(Hash garageHash, Player player, float p2, int p3) {
			return Invoker::Invoke<BOOL, 143>(garageHash, player, p2, p3);
		}

		BOOL IS_PLAYER_PARTIALLY_INSIDE_GARAGE(Hash garageHash, Player player, int p2) {
			return Invoker::Invoke<BOOL, 144>(garageHash, player, p2);
		}

		BOOL ARE_ENTITIES_ENTIRELY_INSIDE_GARAGE(Hash garageHash, BOOL p1, BOOL p2, BOOL p3, Any p4) {
			return Invoker::Invoke<BOOL, 145>(garageHash, p1, p2, p3, p4);
		}

		BOOL IS_ANY_ENTITY_ENTIRELY_INSIDE_GARAGE(Hash garageHash, BOOL p1, BOOL p2, BOOL p3, Any p4) {
			return Invoker::Invoke<BOOL, 146>(garageHash, p1, p2, p3, p4);
		}

		BOOL IS_OBJECT_ENTIRELY_INSIDE_GARAGE(Hash garageHash, Entity entity, float p2, int p3) {
			return Invoker::Invoke<BOOL, 147>(garageHash, entity, p2, p3);
		}

		BOOL IS_OBJECT_PARTIALLY_INSIDE_GARAGE(Hash garageHash, Entity entity, int p2) {
			return Invoker::Invoke<BOOL, 148>(garageHash, entity, p2);
		}

		void CLEAR_GARAGE(Hash garageHash, BOOL isNetwork) {
			Invoker::Invoke<void, 149>(garageHash, isNetwork);
		}

		void CLEAR_OBJECTS_INSIDE_GARAGE(Hash garageHash, BOOL vehicles, BOOL peds, BOOL objects, BOOL isNetwork) {
			Invoker::Invoke<void, 150>(garageHash, vehicles, peds, objects, isNetwork);
		}

		void DISABLE_TIDYING_UP_IN_GARAGE(int id, BOOL toggle) {
			Invoker::Invoke<void, 151>(id, toggle);
		}

		void ENABLE_SAVING_IN_GARAGE(Hash garageHash, BOOL toggle) {
			Invoker::Invoke<void, 152>(garageHash, toggle);
		}

		void CLOSE_SAFEHOUSE_GARAGES() {
			Invoker::Invoke<void, 153>();
		}

		BOOL DOES_OBJECT_OF_TYPE_EXIST_AT_COORDS(float x, float y, float z, float radius, Hash hash, BOOL p5) {
			return Invoker::Invoke<BOOL, 154>(x, y, z, radius, hash, p5);
		}

		BOOL IS_POINT_IN_ANGLED_AREA(float xPos, float yPos, float zPos, float x1, float y1, float z1, float x2, float y2, float z2, float width, BOOL debug, BOOL includeZ) {
			return Invoker::Invoke<BOOL, 155>(xPos, yPos, zPos, x1, y1, z1, x2, y2, z2, width, debug, includeZ);
		}

		void SET_OBJECT_ALLOW_LOW_LOD_BUOYANCY(Object object, BOOL toggle) {
			Invoker::Invoke<void, 156>(object, toggle);
		}

		void SET_OBJECT_PHYSICS_PARAMS(Object object, float weight, float p2, float p3, float p4, float p5, float gravity, float p7, float p8, float p9, float p10, float buoyancy) {
			Invoker::Invoke<void, 157>(object, weight, p2, p3, p4, p5, gravity, p7, p8, p9, p10, buoyancy);
		}

		float GET_OBJECT_FRAGMENT_DAMAGE_HEALTH(Any p0, BOOL p1) {
			return Invoker::Invoke<float, 158>(p0, p1);
		}

		void SET_ACTIVATE_OBJECT_PHYSICS_AS_SOON_AS_IT_IS_UNFROZEN(Object object, BOOL toggle) {
			Invoker::Invoke<void, 159>(object, toggle);
		}

		BOOL IS_ANY_OBJECT_NEAR_POINT(float x, float y, float z, float range, BOOL p4) {
			return Invoker::Invoke<BOOL, 160>(x, y, z, range, p4);
		}

		BOOL IS_OBJECT_NEAR_POINT(Hash objectHash, float x, float y, float z, float range) {
			return Invoker::Invoke<BOOL, 161>(objectHash, x, y, z, range);
		}

		void REMOVE_OBJECT_HIGH_DETAIL_MODEL(Object object) {
			Invoker::Invoke<void, 162>(object);
		}

		void BREAK_OBJECT_FRAGMENT_CHILD(Object p0, Any p1, BOOL p2) {
			Invoker::Invoke<void, 163>(p0, p1, p2);
		}

		void DAMAGE_OBJECT_FRAGMENT_CHILD(Any p0, Any p1, Any p2) {
			Invoker::Invoke<void, 164>(p0, p1, p2);
		}

		void FIX_OBJECT_FRAGMENT(Object object) {
			Invoker::Invoke<void, 165>(object);
		}

		void TRACK_OBJECT_VISIBILITY(Object object) {
			Invoker::Invoke<void, 166>(object);
		}

		BOOL IS_OBJECT_VISIBLE(Object object) {
			return Invoker::Invoke<BOOL, 167>(object);
		}

		void SET_OBJECT_IS_SPECIAL_GOLFBALL(Object object, BOOL toggle) {
			Invoker::Invoke<void, 168>(object, toggle);
		}

		void SET_OBJECT_TAKES_DAMAGE_FROM_COLLIDING_WITH_BUILDINGS(Any p0, BOOL p1) {
			Invoker::Invoke<void, 169>(p0, p1);
		}

		void ALLOW_DAMAGE_EVENTS_FOR_NON_NETWORKED_OBJECTS(BOOL value) {
			Invoker::Invoke<void, 170>(value);
		}

		void SET_CUTSCENES_WEAPON_FLASHLIGHT_ON_THIS_FRAME(Object object, BOOL toggle) {
			Invoker::Invoke<void, 171>(object, toggle);
		}

		Object GET_RAYFIRE_MAP_OBJECT(float x, float y, float z, float radius, const char* name) {
			return Invoker::Invoke<Object, 172>(x, y, z, radius, name);
		}

		void SET_STATE_OF_RAYFIRE_MAP_OBJECT(Object object, int state) {
			Invoker::Invoke<void, 173>(object, state);
		}

		int GET_STATE_OF_RAYFIRE_MAP_OBJECT(Object object) {
			return Invoker::Invoke<int, 174>(object);
		}

		BOOL DOES_RAYFIRE_MAP_OBJECT_EXIST(Object object) {
			return Invoker::Invoke<BOOL, 175>(object);
		}

		float GET_RAYFIRE_MAP_OBJECT_ANIM_PHASE(Object object) {
			return Invoker::Invoke<float, 176>(object);
		}

		Pickup CREATE_PICKUP(Hash pickupHash, float posX, float posY, float posZ, int p4, int value, BOOL p6, Hash modelHash) {
			return Invoker::Invoke<Pickup, 177>(pickupHash, posX, posY, posZ, p4, value, p6, modelHash);
		}

		Pickup CREATE_PICKUP_ROTATE(Hash pickupHash, float posX, float posY, float posZ, float rotX, float rotY, float rotZ, int flag, int amount, Any p9, BOOL p10, Hash modelHash) {
			return Invoker::Invoke<Pickup, 178>(pickupHash, posX, posY, posZ, rotX, rotY, rotZ, flag, amount, p9, p10, modelHash);
		}

		void FORCE_PICKUP_ROTATE_FACE_UP() {
			Invoker::Invoke<void, 179>();
		}

		void SET_CUSTOM_PICKUP_WEAPON_HASH(Hash pickupHash, Pickup pickup) {
			Invoker::Invoke<void, 180>(pickupHash, pickup);
		}

		Object CREATE_AMBIENT_PICKUP(Hash pickupHash, float posX, float posY, float posZ, int flags, int value, Hash modelHash, BOOL p7, BOOL p8) {
			return Invoker::Invoke<Object, 181>(pickupHash, posX, posY, posZ, flags, value, modelHash, p7, p8);
		}

		Object CREATE_NON_NETWORKED_AMBIENT_PICKUP(Hash pickupHash, float posX, float posY, float posZ, int flags, int value, Hash modelHash, BOOL p7, BOOL p8) {
			return Invoker::Invoke<Object, 182>(pickupHash, posX, posY, posZ, flags, value, modelHash, p7, p8);
		}

		void BLOCK_PLAYERS_FOR_AMBIENT_PICKUP(Any p0, Any p1) {
			Invoker::Invoke<void, 183>(p0, p1);
		}

		Object CREATE_PORTABLE_PICKUP(Hash pickupHash, float x, float y, float z, BOOL placeOnGround, Hash modelHash) {
			return Invoker::Invoke<Object, 184>(pickupHash, x, y, z, placeOnGround, modelHash);
		}

		Object CREATE_NON_NETWORKED_PORTABLE_PICKUP(Hash pickupHash, float x, float y, float z, BOOL placeOnGround, Hash modelHash) {
			return Invoker::Invoke<Object, 185>(pickupHash, x, y, z, placeOnGround, modelHash);
		}

		void ATTACH_PORTABLE_PICKUP_TO_PED(Object pickupObject, Ped ped) {
			Invoker::Invoke<void, 186>(pickupObject, ped);
		}

		void DETACH_PORTABLE_PICKUP_FROM_PED(Object pickupObject) {
			Invoker::Invoke<void, 187>(pickupObject);
		}

		void FORCE_PORTABLE_PICKUP_LAST_ACCESSIBLE_POSITION_SETTING(Object object) {
			Invoker::Invoke<void, 188>(object);
		}

		void HIDE_PORTABLE_PICKUP_WHEN_DETACHED(Object pickupObject, BOOL toggle) {
			Invoker::Invoke<void, 189>(pickupObject, toggle);
		}

		void SET_MAX_NUM_PORTABLE_PICKUPS_CARRIED_BY_PLAYER(Hash modelHash, int number) {
			Invoker::Invoke<void, 190>(modelHash, number);
		}

		void SET_LOCAL_PLAYER_CAN_COLLECT_PORTABLE_PICKUPS(BOOL toggle) {
			Invoker::Invoke<void, 191>(toggle);
		}

		Vector3 GET_SAFE_PICKUP_COORDS(float x, float y, float z, float p3, float p4) {
			return Invoker::Invoke<Vector3, 192>(x, y, z, p3, p4);
		}

		void ADD_EXTENDED_PICKUP_PROBE_AREA(float x, float y, float z, float radius) {
			Invoker::Invoke<void, 193>(x, y, z, radius);
		}

		void CLEAR_EXTENDED_PICKUP_PROBE_AREAS() {
			Invoker::Invoke<void, 194>();
		}

		Vector3 GET_PICKUP_COORDS(Pickup pickup) {
			return Invoker::Invoke<Vector3, 195>(pickup);
		}

		void SUPPRESS_PICKUP_SOUND_FOR_PICKUP(Any p0, Any p1) {
			Invoker::Invoke<void, 196>(p0, p1);
		}

		void REMOVE_ALL_PICKUPS_OF_TYPE(Hash pickupHash) {
			Invoker::Invoke<void, 197>(pickupHash);
		}

		BOOL HAS_PICKUP_BEEN_COLLECTED(Pickup pickup) {
			return Invoker::Invoke<BOOL, 198>(pickup);
		}

		void REMOVE_PICKUP(Pickup pickup) {
			Invoker::Invoke<void, 199>(pickup);
		}

		void CREATE_MONEY_PICKUPS(float x, float y, float z, int value, int amount, Hash model) {
			Invoker::Invoke<void, 200>(x, y, z, value, amount, model);
		}

		BOOL DOES_PICKUP_EXIST(Pickup pickup) {
			return Invoker::Invoke<BOOL, 201>(pickup);
		}

		BOOL DOES_PICKUP_OBJECT_EXIST(Object pickupObject) {
			return Invoker::Invoke<BOOL, 202>(pickupObject);
		}

		Object GET_PICKUP_OBJECT(Pickup pickup) {
			return Invoker::Invoke<Object, 203>(pickup);
		}

		BOOL IS_OBJECT_A_PICKUP(Object object) {
			return Invoker::Invoke<BOOL, 204>(object);
		}

		BOOL IS_OBJECT_A_PORTABLE_PICKUP(Object object) {
			return Invoker::Invoke<BOOL, 205>(object);
		}

		BOOL DOES_PICKUP_OF_TYPE_EXIST_IN_AREA(Hash pickupHash, float x, float y, float z, float radius) {
			return Invoker::Invoke<BOOL, 206>(pickupHash, x, y, z, radius);
		}

		void SET_PICKUP_REGENERATION_TIME(Pickup pickup, int duration) {
			Invoker::Invoke<void, 207>(pickup, duration);
		}

		void FORCE_PICKUP_REGENERATE(Any p0) {
			Invoker::Invoke<void, 208>(p0);
		}

		void SET_PLAYER_PERMITTED_TO_COLLECT_PICKUPS_OF_TYPE(Player player, Hash pickupHash, BOOL toggle) {
			Invoker::Invoke<void, 209>(player, pickupHash, toggle);
		}

		void SET_LOCAL_PLAYER_PERMITTED_TO_COLLECT_PICKUPS_WITH_MODEL(Hash modelHash, BOOL toggle) {
			Invoker::Invoke<void, 210>(modelHash, toggle);
		}

		void ALLOW_ALL_PLAYERS_TO_COLLECT_PICKUPS_OF_TYPE(Hash pickupHash) {
			Invoker::Invoke<void, 211>(pickupHash);
		}

		void SET_TEAM_PICKUP_OBJECT(Object object, Any p1, BOOL p2) {
			Invoker::Invoke<void, 212>(object, p1, p2);
		}

		void PREVENT_COLLECTION_OF_PORTABLE_PICKUP(Object object, BOOL p1, BOOL p2) {
			Invoker::Invoke<void, 213>(object, p1, p2);
		}

		void SET_PICKUP_OBJECT_GLOW_WHEN_UNCOLLECTABLE(Pickup pickup, BOOL toggle) {
			Invoker::Invoke<void, 214>(pickup, toggle);
		}

		void _SET_PICKUP_GLOW_DISABLED(Pickup pickup, BOOL toggle) {
			Invoker::Invoke<void, 6623>(pickup, toggle);
		}

		void SET_PICKUP_GLOW_OFFSET(Pickup pickup, float p1) {
			Invoker::Invoke<void, 215>(pickup, p1);
		}

		void SET_PICKUP_OBJECT_GLOW_OFFSET(Pickup pickup, float p1, BOOL p2) {
			Invoker::Invoke<void, 216>(pickup, p1, p2);
		}

		void SET_OBJECT_GLOW_IN_SAME_TEAM(Pickup pickup) {
			Invoker::Invoke<void, 217>(pickup);
		}

		void SET_PICKUP_OBJECT_ARROW_MARKER(Pickup pickup, BOOL toggle) {
			Invoker::Invoke<void, 218>(pickup, toggle);
		}

		void ALLOW_PICKUP_ARROW_MARKER_WHEN_UNCOLLECTABLE(Pickup pickup, BOOL toggle) {
			Invoker::Invoke<void, 219>(pickup, toggle);
		}

		int GET_DEFAULT_AMMO_FOR_WEAPON_PICKUP(Hash pickupHash) {
			return Invoker::Invoke<int, 220>(pickupHash);
		}

		void SET_PICKUP_GENERATION_RANGE_MULTIPLIER(float multiplier) {
			Invoker::Invoke<void, 221>(multiplier);
		}

		float GET_PICKUP_GENERATION_RANGE_MULTIPLIER() {
			return Invoker::Invoke<float, 222>();
		}

		void SET_ONLY_ALLOW_AMMO_COLLECTION_WHEN_LOW(BOOL p0) {
			Invoker::Invoke<void, 223>(p0);
		}

		void SET_PICKUP_UNCOLLECTABLE(Pickup pickup, BOOL toggle) {
			Invoker::Invoke<void, 224>(pickup, toggle);
		}

		void SET_PICKUP_TRANSPARENT_WHEN_UNCOLLECTABLE(Pickup pickup, BOOL toggle) {
			Invoker::Invoke<void, 225>(pickup, toggle);
		}

		void SET_PICKUP_HIDDEN_WHEN_UNCOLLECTABLE(Pickup pickup, BOOL toggle) {
			Invoker::Invoke<void, 226>(pickup, toggle);
		}

		void SET_PICKUP_OBJECT_TRANSPARENT_WHEN_UNCOLLECTABLE(Pickup pickup, BOOL toggle) {
			Invoker::Invoke<void, 227>(pickup, toggle);
		}

		void SET_PICKUP_OBJECT_ALPHA_WHEN_TRANSPARENT(int p0) {
			Invoker::Invoke<void, 228>(p0);
		}

		void SET_PORTABLE_PICKUP_PERSIST(Pickup pickup, BOOL toggle) {
			Invoker::Invoke<void, 229>(pickup, toggle);
		}

		void ALLOW_PORTABLE_PICKUP_TO_MIGRATE_TO_NON_PARTICIPANTS(Pickup pickup, BOOL toggle) {
			Invoker::Invoke<void, 230>(pickup, toggle);
		}

		void FORCE_ACTIVATE_PHYSICS_ON_UNFIXED_PICKUP(Pickup pickup, BOOL toggle) {
			Invoker::Invoke<void, 231>(pickup, toggle);
		}

		void ALLOW_PICKUP_BY_NONE_PARTICIPANT(Pickup pickup, BOOL toggle) {
			Invoker::Invoke<void, 232>(pickup, toggle);
		}

		void SUPPRESS_PICKUP_REWARD_TYPE(int rewardType, BOOL suppress) {
			Invoker::Invoke<void, 233>(rewardType, suppress);
		}

		void CLEAR_ALL_PICKUP_REWARD_TYPE_SUPPRESSION() {
			Invoker::Invoke<void, 234>();
		}

		void CLEAR_PICKUP_REWARD_TYPE_SUPPRESSION(int rewardType) {
			Invoker::Invoke<void, 235>(rewardType);
		}

		void RENDER_FAKE_PICKUP_GLOW(float x, float y, float z, int colorIndex) {
			Invoker::Invoke<void, 236>(x, y, z, colorIndex);
		}

		void SET_PICKUP_OBJECT_COLLECTABLE_IN_VEHICLE(Pickup pickup) {
			Invoker::Invoke<void, 237>(pickup);
		}

		void SET_PICKUP_TRACK_DAMAGE_EVENTS(Pickup pickup, BOOL toggle) {
			Invoker::Invoke<void, 238>(pickup, toggle);
		}

		void SET_ENTITY_FLAG_SUPPRESS_SHADOW(Entity entity, BOOL toggle) {
			Invoker::Invoke<void, 239>(entity, toggle);
		}

		void SET_ENTITY_FLAG_RENDER_SMALL_SHADOW(Object object, BOOL toggle) {
			Invoker::Invoke<void, 240>(object, toggle);
		}

		Hash GET_WEAPON_TYPE_FROM_PICKUP_TYPE(Hash pickupHash) {
			return Invoker::Invoke<Hash, 241>(pickupHash);
		}

		Hash GET_PICKUP_TYPE_FROM_WEAPON_HASH(Hash weaponHash) {
			return Invoker::Invoke<Hash, 242>(weaponHash);
		}

		BOOL IS_PICKUP_WEAPON_OBJECT_VALID(Object object) {
			return Invoker::Invoke<BOOL, 243>(object);
		}

		int GET_OBJECT_TINT_INDEX(Object object) {
			return Invoker::Invoke<int, 244>(object);
		}

		void SET_OBJECT_TINT_INDEX(Object object, int textureVariation) {
			Invoker::Invoke<void, 245>(object, textureVariation);
		}

		BOOL SET_TINT_INDEX_CLOSEST_BUILDING_OF_TYPE(float x, float y, float z, float radius, Hash modelHash, int textureVariation) {
			return Invoker::Invoke<BOOL, 246>(x, y, z, radius, modelHash, textureVariation);
		}

		void SET_PROP_TINT_INDEX(Any p0, Any p1) {
			Invoker::Invoke<void, 247>(p0, p1);
		}

		BOOL SET_PROP_LIGHT_COLOR(Object object, BOOL p1, int r, int g, int b) {
			return Invoker::Invoke<BOOL, 248>(object, p1, r, g, b);
		}

		BOOL IS_PROP_LIGHT_OVERRIDEN(Object object) {
			return Invoker::Invoke<BOOL, 249>(object);
		}

		void SET_OBJECT_IS_VISIBLE_IN_MIRRORS(Object object, BOOL toggle) {
			Invoker::Invoke<void, 250>(object, toggle);
		}

		void SET_OBJECT_SPEED_BOOST_AMOUNT(Object object, Any p1) {
			Invoker::Invoke<void, 251>(object, p1);
		}

		void SET_OBJECT_SPEED_BOOST_DURATION(Object object, float duration) {
			Invoker::Invoke<void, 252>(object, duration);
		}

		Hash CONVERT_OLD_PICKUP_TYPE_TO_NEW(Hash pickupHash) {
			return Invoker::Invoke<Hash, 253>(pickupHash);
		}

		void SET_FORCE_OBJECT_THIS_FRAME(float x, float y, float z, float p3) {
			Invoker::Invoke<void, 254>(x, y, z, p3);
		}

		void ONLY_CLEAN_UP_OBJECT_WHEN_OUT_OF_RANGE(Object object) {
			Invoker::Invoke<void, 255>(object);
		}

		void SET_DISABLE_COLLISIONS_BETWEEN_CARS_AND_CAR_PARACHUTE(Any p0) {
			Invoker::Invoke<void, 256>(p0);
		}

		void SET_PROJECTILES_SHOULD_EXPLODE_ON_CONTACT(Entity entity, Any p1) {
			Invoker::Invoke<void, 257>(entity, p1);
		}

		void SET_DRIVE_ARTICULATED_JOINT(Object object, BOOL toggle, int p2) {
			Invoker::Invoke<void, 258>(object, toggle, p2);
		}

		void SET_DRIVE_ARTICULATED_JOINT_WITH_INFLICTOR(Object object, BOOL toggle, int p2, Ped ped) {
			Invoker::Invoke<void, 259>(object, toggle, p2, ped);
		}

		void SET_OBJECT_IS_A_PRESSURE_PLATE(Object object, BOOL toggle) {
			Invoker::Invoke<void, 260>(object, toggle);
		}

		void SET_WEAPON_IMPACTS_APPLY_GREATER_FORCE(Object object, BOOL p1) {
			Invoker::Invoke<void, 261>(object, p1);
		}

		BOOL GET_IS_ARTICULATED_JOINT_AT_MIN_ANGLE(Object object, Any p1) {
			return Invoker::Invoke<BOOL, 262>(object, p1);
		}

		BOOL GET_IS_ARTICULATED_JOINT_AT_MAX_ANGLE(Any p0, Any p1) {
			return Invoker::Invoke<BOOL, 263>(p0, p1);
		}

		void SET_IS_OBJECT_ARTICULATED(Object object, BOOL toggle) {
			Invoker::Invoke<void, 264>(object, toggle);
		}

		void SET_IS_OBJECT_BALL(Object object, BOOL toggle) {
			Invoker::Invoke<void, 265>(object, toggle);
		}

	} // namespace OBJECT

	namespace PAD {

		BOOL IS_CONTROL_ENABLED(int control, int action) {
			return Invoker::Invoke<BOOL, 1108>(control, action);
		}

		BOOL IS_CONTROL_PRESSED(int control, int action) {
			return Invoker::Invoke<BOOL, 1109>(control, action);
		}

		BOOL IS_CONTROL_RELEASED(int control, int action) {
			return Invoker::Invoke<BOOL, 1110>(control, action);
		}

		BOOL IS_CONTROL_JUST_PRESSED(int control, int action) {
			return Invoker::Invoke<BOOL, 1111>(control, action);
		}

		BOOL IS_CONTROL_JUST_RELEASED(int control, int action) {
			return Invoker::Invoke<BOOL, 1112>(control, action);
		}

		BOOL _IS_CONTROL_HELD_DOWN(int control, int action, int duration) {
			return Invoker::Invoke<BOOL, 6622>(control, action, duration);
		}

		int GET_CONTROL_VALUE(int control, int action) {
			return Invoker::Invoke<int, 1113>(control, action);
		}

		float GET_CONTROL_NORMAL(int control, int action) {
			return Invoker::Invoke<float, 1114>(control, action);
		}

		void SET_USE_ADJUSTED_MOUSE_COORDS(BOOL toggle) {
			Invoker::Invoke<void, 1115>(toggle);
		}

		float GET_CONTROL_UNBOUND_NORMAL(int control, int action) {
			return Invoker::Invoke<float, 1116>(control, action);
		}

		BOOL SET_CONTROL_VALUE_NEXT_FRAME(int control, int action, float value) {
			return Invoker::Invoke<BOOL, 1117>(control, action, value);
		}

		BOOL IS_DISABLED_CONTROL_PRESSED(int control, int action) {
			return Invoker::Invoke<BOOL, 1118>(control, action);
		}

		BOOL IS_DISABLED_CONTROL_RELEASED(int control, int action) {
			return Invoker::Invoke<BOOL, 1119>(control, action);
		}

		BOOL IS_DISABLED_CONTROL_JUST_PRESSED(int control, int action) {
			return Invoker::Invoke<BOOL, 1120>(control, action);
		}

		BOOL IS_DISABLED_CONTROL_JUST_RELEASED(int control, int action) {
			return Invoker::Invoke<BOOL, 1121>(control, action);
		}

		float GET_DISABLED_CONTROL_NORMAL(int control, int action) {
			return Invoker::Invoke<float, 1122>(control, action);
		}

		float GET_DISABLED_CONTROL_UNBOUND_NORMAL(int control, int action) {
			return Invoker::Invoke<float, 1123>(control, action);
		}

		int GET_CONTROL_HOW_LONG_AGO(int control) {
			return Invoker::Invoke<int, 1124>(control);
		}

		BOOL IS_USING_KEYBOARD_AND_MOUSE(int control) {
			return Invoker::Invoke<BOOL, 1125>(control);
		}

		BOOL IS_USING_CURSOR(int control) {
			return Invoker::Invoke<BOOL, 1126>(control);
		}

		BOOL SET_CURSOR_POSITION(float x, float y) {
			return Invoker::Invoke<BOOL, 1127>(x, y);
		}

		BOOL IS_USING_REMOTE_PLAY(int control) {
			return Invoker::Invoke<BOOL, 1128>(control);
		}

		BOOL HAVE_CONTROLS_CHANGED(int control) {
			return Invoker::Invoke<BOOL, 1129>(control);
		}

		const char* GET_CONTROL_INSTRUCTIONAL_BUTTONS_STRING(int control, int action, BOOL allowXOSwap) {
			return Invoker::Invoke<const char*, 1130>(control, action, allowXOSwap);
		}

		const char* GET_CONTROL_GROUP_INSTRUCTIONAL_BUTTONS_STRING(int control, int controlGroup, BOOL allowXOSwap) {
			return Invoker::Invoke<const char*, 1131>(control, controlGroup, allowXOSwap);
		}

		void SET_CONTROL_LIGHT_EFFECT_COLOR(int control, int red, int green, int blue) {
			Invoker::Invoke<void, 1132>(control, red, green, blue);
		}

		void CLEAR_CONTROL_LIGHT_EFFECT(int control) {
			Invoker::Invoke<void, 1133>(control);
		}

		void SET_CONTROL_SHAKE(int control, int duration, int frequency) {
			Invoker::Invoke<void, 1134>(control, duration, frequency);
		}

		void SET_CONTROL_TRIGGER_SHAKE(int control, int leftDuration, int leftFrequency, int rightDuration, int rightFrequency) {
			Invoker::Invoke<void, 1135>(control, leftDuration, leftFrequency, rightDuration, rightFrequency);
		}

		void STOP_CONTROL_SHAKE(int control) {
			Invoker::Invoke<void, 1136>(control);
		}

		void SET_CONTROL_SHAKE_SUPPRESSED_ID(int control, int uniqueId) {
			Invoker::Invoke<void, 1137>(control, uniqueId);
		}

		void CLEAR_CONTROL_SHAKE_SUPPRESSED_ID(int control) {
			Invoker::Invoke<void, 1138>(control);
		}

		BOOL IS_LOOK_INVERTED() {
			return Invoker::Invoke<BOOL, 1139>();
		}

		BOOL IS_MOUSE_LOOK_INVERTED() {
			return Invoker::Invoke<BOOL, 1140>();
		}

		int GET_LOCAL_PLAYER_AIM_STATE() {
			return Invoker::Invoke<int, 1141>();
		}

		int GET_LOCAL_PLAYER_GAMEPAD_AIM_STATE() {
			return Invoker::Invoke<int, 1142>();
		}

		BOOL GET_IS_USING_ALTERNATE_HANDBRAKE() {
			return Invoker::Invoke<BOOL, 1143>();
		}

		BOOL GET_IS_USING_ALTERNATE_DRIVEBY() {
			return Invoker::Invoke<BOOL, 1144>();
		}

		BOOL GET_ALLOW_MOVEMENT_WHILE_ZOOMED() {
			return Invoker::Invoke<BOOL, 1145>();
		}

		void SET_PLAYERPAD_SHAKES_WHEN_CONTROLLER_DISABLED(BOOL toggle) {
			Invoker::Invoke<void, 1146>(toggle);
		}

		void SET_INPUT_EXCLUSIVE(int control, int action) {
			Invoker::Invoke<void, 1147>(control, action);
		}

		void DISABLE_CONTROL_ACTION(int control, int action, BOOL disableRelatedActions) {
			Invoker::Invoke<void, 1148>(control, action, disableRelatedActions);
		}

		void ENABLE_CONTROL_ACTION(int control, int action, BOOL enableRelatedActions) {
			Invoker::Invoke<void, 1149>(control, action, enableRelatedActions);
		}

		void DISABLE_ALL_CONTROL_ACTIONS(int control) {
			Invoker::Invoke<void, 1150>(control);
		}

		void ENABLE_ALL_CONTROL_ACTIONS(int control) {
			Invoker::Invoke<void, 1151>(control);
		}

		BOOL INIT_PC_SCRIPTED_CONTROLS(const char* schemeName) {
			return Invoker::Invoke<BOOL, 1152>(schemeName);
		}

		BOOL SWITCH_PC_SCRIPTED_CONTROLS(const char* schemeName) {
			return Invoker::Invoke<BOOL, 1153>(schemeName);
		}

		void SHUTDOWN_PC_SCRIPTED_CONTROLS() {
			Invoker::Invoke<void, 1154>();
		}

		void ALLOW_ALTERNATIVE_SCRIPT_CONTROLS_LAYOUT(int control) {
			Invoker::Invoke<void, 1155>(control);
		}

		int _GET_GAMEPAD_TYPE() {
			return Invoker::Invoke<int, 6621>();
		}

	} // namespace PAD

	namespace PATHFIND {

		void SET_ROADS_IN_AREA(float x1, float y1, float z1, float x2, float y2, float z2, BOOL nodeEnabled, BOOL unknown2) {
			Invoker::Invoke<void, 3197>(x1, y1, z1, x2, y2, z2, nodeEnabled, unknown2);
		}

		void SET_ROADS_IN_ANGLED_AREA(float x1, float y1, float z1, float x2, float y2, float z2, float width, BOOL unknown1, BOOL unknown2, BOOL unknown3) {
			Invoker::Invoke<void, 3198>(x1, y1, z1, x2, y2, z2, width, unknown1, unknown2, unknown3);
		}

		void SET_PED_PATHS_IN_AREA(float x1, float y1, float z1, float x2, float y2, float z2, BOOL p6, Any p7) {
			Invoker::Invoke<void, 3199>(x1, y1, z1, x2, y2, z2, p6, p7);
		}

		BOOL GET_SAFE_COORD_FOR_PED(float x, float y, float z, BOOL onGround, Vector3* outPosition, int flags) {
			return Invoker::Invoke<BOOL, 3200>(x, y, z, onGround, outPosition, flags);
		}

		BOOL GET_CLOSEST_VEHICLE_NODE(float x, float y, float z, Vector3* outPosition, int nodeFlags, float p5, float p6) {
			return Invoker::Invoke<BOOL, 3201>(x, y, z, outPosition, nodeFlags, p5, p6);
		}

		BOOL GET_CLOSEST_MAJOR_VEHICLE_NODE(float x, float y, float z, Vector3* outPosition, float unknown1, float unknown2) {
			return Invoker::Invoke<BOOL, 3202>(x, y, z, outPosition, unknown1, unknown2);
		}

		BOOL GET_CLOSEST_VEHICLE_NODE_WITH_HEADING(float x, float y, float z, Vector3* outPosition, float* outHeading, int nodeType, float p6, float p7) {
			return Invoker::Invoke<BOOL, 3203>(x, y, z, outPosition, outHeading, nodeType, p6, p7);
		}

		BOOL GET_NTH_CLOSEST_VEHICLE_NODE(float x, float y, float z, int nthClosest, Vector3* outPosition, int nodeFlags, float unknown1, float unknown2) {
			return Invoker::Invoke<BOOL, 3204>(x, y, z, nthClosest, outPosition, nodeFlags, unknown1, unknown2);
		}

		int GET_NTH_CLOSEST_VEHICLE_NODE_ID(float x, float y, float z, int nth, int nodeFlags, float p5, float p6) {
			return Invoker::Invoke<int, 3205>(x, y, z, nth, nodeFlags, p5, p6);
		}

		BOOL GET_NTH_CLOSEST_VEHICLE_NODE_WITH_HEADING(float x, float y, float z, int nthClosest, Vector3* outPosition, float* outHeading, int* outNumLanes, int nodeFlags, float unknown3, float unknown4) {
			return Invoker::Invoke<BOOL, 3206>(x, y, z, nthClosest, outPosition, outHeading, outNumLanes, nodeFlags, unknown3, unknown4);
		}

		int GET_NTH_CLOSEST_VEHICLE_NODE_ID_WITH_HEADING(float x, float y, float z, int nthClosest, Vector3* outPosition, float* outHeading, int nodeFlags, float p7, float p8) {
			return Invoker::Invoke<int, 3207>(x, y, z, nthClosest, outPosition, outHeading, nodeFlags, p7, p8);
		}

		BOOL GET_NTH_CLOSEST_VEHICLE_NODE_FAVOUR_DIRECTION(float x, float y, float z, float desiredX, float desiredY, float desiredZ, int nthClosest, Vector3* outPosition, float* outHeading, int nodeFlags, float p10, float p11) {
			return Invoker::Invoke<BOOL, 3208>(x, y, z, desiredX, desiredY, desiredZ, nthClosest, outPosition, outHeading, nodeFlags, p10, p11);
		}

		BOOL GET_VEHICLE_NODE_PROPERTIES(float x, float y, float z, int* density, int* flags) {
			return Invoker::Invoke<BOOL, 3209>(x, y, z, density, flags);
		}

		BOOL IS_VEHICLE_NODE_ID_VALID(int vehicleNodeId) {
			return Invoker::Invoke<BOOL, 3210>(vehicleNodeId);
		}

		void GET_VEHICLE_NODE_POSITION(int nodeId, Vector3* outPosition) {
			Invoker::Invoke<void, 3211>(nodeId, outPosition);
		}

		BOOL GET_VEHICLE_NODE_IS_GPS_ALLOWED(int nodeID) {
			return Invoker::Invoke<BOOL, 3212>(nodeID);
		}

		BOOL GET_VEHICLE_NODE_IS_SWITCHED_OFF(int nodeID) {
			return Invoker::Invoke<BOOL, 3213>(nodeID);
		}

		BOOL GET_CLOSEST_ROAD(float x, float y, float z, float p3, int p4, Vector3* p5, Vector3* p6, Any* p7, Any* p8, float* p9, BOOL p10) {
			return Invoker::Invoke<BOOL, 3214>(x, y, z, p3, p4, p5, p6, p7, p8, p9, p10);
		}

		BOOL LOAD_ALL_PATH_NODES(BOOL set) {
			return Invoker::Invoke<BOOL, 3215>(set);
		}

		void SET_ALLOW_STREAM_PROLOGUE_NODES(BOOL toggle) {
			Invoker::Invoke<void, 3216>(toggle);
		}

		void SET_ALLOW_STREAM_HEIST_ISLAND_NODES(int type) {
			Invoker::Invoke<void, 3217>(type);
		}

		BOOL ARE_NODES_LOADED_FOR_AREA(float x1, float y1, float x2, float y2) {
			return Invoker::Invoke<BOOL, 3218>(x1, y1, x2, y2);
		}

		BOOL REQUEST_PATH_NODES_IN_AREA_THIS_FRAME(float x1, float y1, float x2, float y2) {
			return Invoker::Invoke<BOOL, 3219>(x1, y1, x2, y2);
		}

		void SET_ROADS_BACK_TO_ORIGINAL(float p0, float p1, float p2, float p3, float p4, float p5, Any p6) {
			Invoker::Invoke<void, 3220>(p0, p1, p2, p3, p4, p5, p6);
		}

		void SET_ROADS_BACK_TO_ORIGINAL_IN_ANGLED_AREA(float x1, float y1, float z1, float x2, float y2, float z2, float width, Any p7) {
			Invoker::Invoke<void, 3221>(x1, y1, z1, x2, y2, z2, width, p7);
		}

		void SET_AMBIENT_PED_RANGE_MULTIPLIER_THIS_FRAME(float multiplier) {
			Invoker::Invoke<void, 3222>(multiplier);
		}

		void ADJUST_AMBIENT_PED_SPAWN_DENSITIES_THIS_FRAME(Any p0, Any p1, Any p2, Any p3, Any p4, Any p5, Any p6) {
			Invoker::Invoke<void, 3223>(p0, p1, p2, p3, p4, p5, p6);
		}

		void SET_PED_PATHS_BACK_TO_ORIGINAL(float x1, float y1, float z1, float x2, float y2, float z2, Any p6) {
			Invoker::Invoke<void, 3224>(x1, y1, z1, x2, y2, z2, p6);
		}

		BOOL GET_RANDOM_VEHICLE_NODE(float x, float y, float z, float radius, BOOL p4, BOOL p5, BOOL p6, Vector3* outPosition, int* nodeId) {
			return Invoker::Invoke<BOOL, 3225>(x, y, z, radius, p4, p5, p6, outPosition, nodeId);
		}

		void GET_SPAWN_COORDS_FOR_VEHICLE_NODE(int nodeAddress, float towardsCoorsX, float towardsCoorsY, float towardsCoorsZ, Vector3* centrePoint, float* heading) {
			Invoker::Invoke<void, 3226>(nodeAddress, towardsCoorsX, towardsCoorsY, towardsCoorsZ, centrePoint, heading);
		}

		void GET_STREET_NAME_AT_COORD(float x, float y, float z, Hash* streetName, Hash* crossingRoad) {
			Invoker::Invoke<void, 3227>(x, y, z, streetName, crossingRoad);
		}

		int GENERATE_DIRECTIONS_TO_COORD(float x, float y, float z, BOOL p3, int* direction, float* p5, float* distToNxJunction) {
			return Invoker::Invoke<int, 3228>(x, y, z, p3, direction, p5, distToNxJunction);
		}

		void SET_IGNORE_NO_GPS_FLAG(BOOL toggle) {
			Invoker::Invoke<void, 3229>(toggle);
		}

		void SET_IGNORE_NO_GPS_FLAG_UNTIL_FIRST_NORMAL_NODE(BOOL toggle) {
			Invoker::Invoke<void, 3230>(toggle);
		}

		void SET_GPS_DISABLED_ZONE(float x1, float y1, float z1, float x2, float y2, float z3) {
			Invoker::Invoke<void, 3231>(x1, y1, z1, x2, y2, z3);
		}

		int GET_GPS_BLIP_ROUTE_LENGTH() {
			return Invoker::Invoke<int, 3232>();
		}

		BOOL GET_POS_ALONG_GPS_TYPE_ROUTE(Vector3* result, BOOL p1, float p2, int p3) {
			return Invoker::Invoke<BOOL, 3233>(result, p1, p2, p3);
		}

		BOOL GET_GPS_BLIP_ROUTE_FOUND() {
			return Invoker::Invoke<BOOL, 3234>();
		}

		BOOL GET_ROAD_BOUNDARY_USING_HEADING(float x, float y, float z, float heading, Vector3* outPosition) {
			return Invoker::Invoke<BOOL, 3235>(x, y, z, heading, outPosition);
		}

		BOOL GET_POSITION_BY_SIDE_OF_ROAD(float x, float y, float z, int p3, Vector3* outPosition) {
			return Invoker::Invoke<BOOL, 3236>(x, y, z, p3, outPosition);
		}

		BOOL IS_POINT_ON_ROAD(float x, float y, float z, Vehicle vehicle) {
			return Invoker::Invoke<BOOL, 3237>(x, y, z, vehicle);
		}

		int GET_NEXT_GPS_DISABLED_ZONE_INDEX() {
			return Invoker::Invoke<int, 3238>();
		}

		void SET_GPS_DISABLED_ZONE_AT_INDEX(float x1, float y1, float z1, float x2, float y2, float z2, int index) {
			Invoker::Invoke<void, 3239>(x1, y1, z1, x2, y2, z2, index);
		}

		void CLEAR_GPS_DISABLED_ZONE_AT_INDEX(int index) {
			Invoker::Invoke<void, 3240>(index);
		}

		void ADD_NAVMESH_REQUIRED_REGION(float x, float y, float radius) {
			Invoker::Invoke<void, 3241>(x, y, radius);
		}

		void REMOVE_NAVMESH_REQUIRED_REGIONS() {
			Invoker::Invoke<void, 3242>();
		}

		BOOL IS_NAVMESH_REQUIRED_REGION_IN_USE() {
			return Invoker::Invoke<BOOL, 3243>();
		}

		void DISABLE_NAVMESH_IN_AREA(float x1, float y1, float z1, float x2, float y2, float z2, BOOL toggle) {
			Invoker::Invoke<void, 3244>(x1, y1, z1, x2, y2, z2, toggle);
		}

		BOOL ARE_ALL_NAVMESH_REGIONS_LOADED() {
			return Invoker::Invoke<BOOL, 3245>();
		}

		BOOL IS_NAVMESH_LOADED_IN_AREA(float x1, float y1, float z1, float x2, float y2, float z2) {
			return Invoker::Invoke<BOOL, 3246>(x1, y1, z1, x2, y2, z2);
		}

		int GET_NUM_NAVMESHES_EXISTING_IN_AREA(float x1, float y1, float z1, float x2, float y2, float z2) {
			return Invoker::Invoke<int, 3247>(x1, y1, z1, x2, y2, z2);
		}

		int ADD_NAVMESH_BLOCKING_OBJECT(float p0, float p1, float p2, float p3, float p4, float p5, float p6, BOOL p7, Any p8) {
			return Invoker::Invoke<int, 3248>(p0, p1, p2, p3, p4, p5, p6, p7, p8);
		}

		void UPDATE_NAVMESH_BLOCKING_OBJECT(Any p0, float p1, float p2, float p3, float p4, float p5, float p6, float p7, Any p8) {
			Invoker::Invoke<void, 3249>(p0, p1, p2, p3, p4, p5, p6, p7, p8);
		}

		void REMOVE_NAVMESH_BLOCKING_OBJECT(Any p0) {
			Invoker::Invoke<void, 3250>(p0);
		}

		BOOL DOES_NAVMESH_BLOCKING_OBJECT_EXIST(Any p0) {
			return Invoker::Invoke<BOOL, 3251>(p0);
		}

		float GET_APPROX_HEIGHT_FOR_POINT(float x, float y) {
			return Invoker::Invoke<float, 3252>(x, y);
		}

		float GET_APPROX_HEIGHT_FOR_AREA(float x1, float y1, float x2, float y2) {
			return Invoker::Invoke<float, 3253>(x1, y1, x2, y2);
		}

		float GET_APPROX_FLOOR_FOR_POINT(float x, float y) {
			return Invoker::Invoke<float, 3254>(x, y);
		}

		float GET_APPROX_FLOOR_FOR_AREA(float x1, float y1, float x2, float y2) {
			return Invoker::Invoke<float, 3255>(x1, y1, x2, y2);
		}

		float CALCULATE_TRAVEL_DISTANCE_BETWEEN_POINTS(float x1, float y1, float z1, float x2, float y2, float z2) {
			return Invoker::Invoke<float, 3256>(x1, y1, z1, x2, y2, z2);
		}

	} // namespace PATHFIND

	namespace PED {

		Ped CREATE_PED(int pedType, Hash modelHash, float x, float y, float z, float heading, BOOL isNetwork, BOOL bScriptHostPed) {
			return Invoker::Invoke<Ped, 417>(pedType, modelHash, x, y, z, heading, isNetwork, bScriptHostPed);
		}

		void DELETE_PED(Ped* ped) {
			Invoker::Invoke<void, 418>(ped);
		}

		Ped CLONE_PED(Ped ped, BOOL isNetwork, BOOL bScriptHostPed, BOOL copyHeadBlendFlag) {
			return Invoker::Invoke<Ped, 419>(ped, isNetwork, bScriptHostPed, copyHeadBlendFlag);
		}

		Ped CLONE_PED_ALT(Ped ped, BOOL isNetwork, BOOL bScriptHostPed, BOOL copyHeadBlendFlag, BOOL p4) {
			return Invoker::Invoke<Ped, 420>(ped, isNetwork, bScriptHostPed, copyHeadBlendFlag, p4);
		}

		void CLONE_PED_TO_TARGET(Ped ped, Ped targetPed) {
			Invoker::Invoke<void, 421>(ped, targetPed);
		}

		void CLONE_PED_TO_TARGET_ALT(Ped ped, Ped targetPed, BOOL p2) {
			Invoker::Invoke<void, 422>(ped, targetPed, p2);
		}

		BOOL IS_PED_IN_VEHICLE(Ped ped, Vehicle vehicle, BOOL atGetIn) {
			return Invoker::Invoke<BOOL, 423>(ped, vehicle, atGetIn);
		}

		BOOL IS_PED_IN_MODEL(Ped ped, Hash modelHash) {
			return Invoker::Invoke<BOOL, 424>(ped, modelHash);
		}

		BOOL IS_PED_IN_ANY_VEHICLE(Ped ped, BOOL atGetIn) {
			return Invoker::Invoke<BOOL, 425>(ped, atGetIn);
		}

		BOOL IS_COP_PED_IN_AREA_3D(float x1, float y1, float z1, float x2, float y2, float z2) {
			return Invoker::Invoke<BOOL, 426>(x1, y1, z1, x2, y2, z2);
		}

		BOOL IS_PED_INJURED(Ped ped) {
			return Invoker::Invoke<BOOL, 427>(ped);
		}

		BOOL IS_PED_HURT(Ped ped) {
			return Invoker::Invoke<BOOL, 428>(ped);
		}

		BOOL IS_PED_FATALLY_INJURED(Ped ped) {
			return Invoker::Invoke<BOOL, 429>(ped);
		}

		BOOL IS_PED_DEAD_OR_DYING(Ped ped, BOOL p1) {
			return Invoker::Invoke<BOOL, 430>(ped, p1);
		}

		BOOL IS_CONVERSATION_PED_DEAD(Ped ped) {
			return Invoker::Invoke<BOOL, 431>(ped);
		}

		BOOL IS_PED_AIMING_FROM_COVER(Ped ped) {
			return Invoker::Invoke<BOOL, 432>(ped);
		}

		BOOL IS_PED_RELOADING(Ped ped) {
			return Invoker::Invoke<BOOL, 433>(ped);
		}

		BOOL IS_PED_A_PLAYER(Ped ped) {
			return Invoker::Invoke<BOOL, 434>(ped);
		}

		Ped CREATE_PED_INSIDE_VEHICLE(Vehicle vehicle, int pedType, Hash modelHash, int seat, BOOL isNetwork, BOOL bScriptHostPed) {
			return Invoker::Invoke<Ped, 435>(vehicle, pedType, modelHash, seat, isNetwork, bScriptHostPed);
		}

		void SET_PED_DESIRED_HEADING(Ped ped, float heading) {
			Invoker::Invoke<void, 436>(ped, heading);
		}

		void FORCE_ALL_HEADING_VALUES_TO_ALIGN(Ped ped) {
			Invoker::Invoke<void, 437>(ped);
		}

		BOOL IS_PED_FACING_PED(Ped ped, Ped otherPed, float angle) {
			return Invoker::Invoke<BOOL, 438>(ped, otherPed, angle);
		}

		BOOL IS_PED_IN_MELEE_COMBAT(Ped ped) {
			return Invoker::Invoke<BOOL, 439>(ped);
		}

		BOOL IS_PED_STOPPED(Ped ped) {
			return Invoker::Invoke<BOOL, 440>(ped);
		}

		BOOL IS_PED_SHOOTING_IN_AREA(Ped ped, float x1, float y1, float z1, float x2, float y2, float z2, BOOL p7, BOOL p8) {
			return Invoker::Invoke<BOOL, 441>(ped, x1, y1, z1, x2, y2, z2, p7, p8);
		}

		BOOL IS_ANY_PED_SHOOTING_IN_AREA(float x1, float y1, float z1, float x2, float y2, float z2, BOOL p6, BOOL p7) {
			return Invoker::Invoke<BOOL, 442>(x1, y1, z1, x2, y2, z2, p6, p7);
		}

		BOOL IS_PED_SHOOTING(Ped ped) {
			return Invoker::Invoke<BOOL, 443>(ped);
		}

		void SET_PED_ACCURACY(Ped ped, int accuracy) {
			Invoker::Invoke<void, 444>(ped, accuracy);
		}

		int GET_PED_ACCURACY(Ped ped) {
			return Invoker::Invoke<int, 445>(ped);
		}

		void SET_AMBIENT_LAW_PED_ACCURACY_MODIFIER(float multiplier) {
			Invoker::Invoke<void, 446>(multiplier);
		}

		BOOL IS_PED_MODEL(Ped ped, Hash modelHash) {
			return Invoker::Invoke<BOOL, 447>(ped, modelHash);
		}

		void EXPLODE_PED_HEAD(Ped ped, Hash weaponHash) {
			Invoker::Invoke<void, 448>(ped, weaponHash);
		}

		void REMOVE_PED_ELEGANTLY(Ped* ped) {
			Invoker::Invoke<void, 449>(ped);
		}

		void ADD_ARMOUR_TO_PED(Ped ped, int amount) {
			Invoker::Invoke<void, 450>(ped, amount);
		}

		void SET_PED_ARMOUR(Ped ped, int amount) {
			Invoker::Invoke<void, 451>(ped, amount);
		}

		void SET_PED_INTO_VEHICLE(Ped ped, Vehicle vehicle, int seatIndex) {
			Invoker::Invoke<void, 452>(ped, vehicle, seatIndex);
		}

		void SET_PED_ALLOW_VEHICLES_OVERRIDE(Ped ped, BOOL toggle) {
			Invoker::Invoke<void, 453>(ped, toggle);
		}

		BOOL CAN_CREATE_RANDOM_PED(BOOL p0) {
			return Invoker::Invoke<BOOL, 454>(p0);
		}

		Ped CREATE_RANDOM_PED(float posX, float posY, float posZ) {
			return Invoker::Invoke<Ped, 455>(posX, posY, posZ);
		}

		Ped CREATE_RANDOM_PED_AS_DRIVER(Vehicle vehicle, BOOL returnHandle) {
			return Invoker::Invoke<Ped, 456>(vehicle, returnHandle);
		}

		BOOL CAN_CREATE_RANDOM_DRIVER() {
			return Invoker::Invoke<BOOL, 457>();
		}

		BOOL CAN_CREATE_RANDOM_BIKE_RIDER() {
			return Invoker::Invoke<BOOL, 458>();
		}

		void SET_PED_MOVE_ANIMS_BLEND_OUT(Ped ped) {
			Invoker::Invoke<void, 459>(ped);
		}

		void SET_PED_CAN_BE_DRAGGED_OUT(Ped ped, BOOL toggle) {
			Invoker::Invoke<void, 460>(ped, toggle);
		}

		void SET_PED_ALLOW_HURT_COMBAT_FOR_ALL_MISSION_PEDS(BOOL toggle) {
			Invoker::Invoke<void, 461>(toggle);
		}

		BOOL IS_PED_MALE(Ped ped) {
			return Invoker::Invoke<BOOL, 462>(ped);
		}

		BOOL IS_PED_HUMAN(Ped ped) {
			return Invoker::Invoke<BOOL, 463>(ped);
		}

		Vehicle GET_VEHICLE_PED_IS_IN(Ped ped, BOOL includeEntering) {
			return Invoker::Invoke<Vehicle, 464>(ped, includeEntering);
		}

		void RESET_PED_LAST_VEHICLE(Ped ped) {
			Invoker::Invoke<void, 465>(ped);
		}

		void SET_PED_DENSITY_MULTIPLIER_THIS_FRAME(float multiplier) {
			Invoker::Invoke<void, 466>(multiplier);
		}

		void SET_SCENARIO_PED_DENSITY_MULTIPLIER_THIS_FRAME(float p0, float p1) {
			Invoker::Invoke<void, 467>(p0, p1);
		}

		void SUPPRESS_AMBIENT_PED_AGGRESSIVE_CLEANUP_THIS_FRAME() {
			Invoker::Invoke<void, 468>();
		}

		void SET_SCRIPTED_CONVERSION_COORD_THIS_FRAME(float x, float y, float z) {
			Invoker::Invoke<void, 469>(x, y, z);
		}

		void SET_PED_NON_CREATION_AREA(float x1, float y1, float z1, float x2, float y2, float z2) {
			Invoker::Invoke<void, 470>(x1, y1, z1, x2, y2, z2);
		}

		void CLEAR_PED_NON_CREATION_AREA() {
			Invoker::Invoke<void, 471>();
		}

		void INSTANTLY_FILL_PED_POPULATION() {
			Invoker::Invoke<void, 472>();
		}

		BOOL IS_PED_ON_MOUNT(Ped ped) {
			return Invoker::Invoke<BOOL, 473>(ped);
		}

		Ped GET_MOUNT(Ped ped) {
			return Invoker::Invoke<Ped, 474>(ped);
		}

		BOOL IS_PED_ON_VEHICLE(Ped ped) {
			return Invoker::Invoke<BOOL, 475>(ped);
		}

		BOOL IS_PED_ON_SPECIFIC_VEHICLE(Ped ped, Vehicle vehicle) {
			return Invoker::Invoke<BOOL, 476>(ped, vehicle);
		}

		void SET_PED_MONEY(Ped ped, int amount) {
			Invoker::Invoke<void, 477>(ped, amount);
		}

		int GET_PED_MONEY(Ped ped) {
			return Invoker::Invoke<int, 478>(ped);
		}

		void SET_HEALTH_SNACKS_CARRIED_BY_ALL_NEW_PEDS(float p0, Any p1) {
			Invoker::Invoke<void, 479>(p0, p1);
		}

		void SET_AMBIENT_PEDS_DROP_MONEY(BOOL p0) {
			Invoker::Invoke<void, 480>(p0);
		}

		void _SET_BLOCK_AMBIENT_PEDS_FROM_DROPPING_WEAPONS_THIS_FRAME() {
			Invoker::Invoke<void, 6586>();
		}

		void SET_BLOCKING_OF_NON_TEMPORARY_EVENTS_FOR_AMBIENT_PEDS_THIS_FRAME(BOOL p0) {
			Invoker::Invoke<void, 481>(p0);
		}

		void SET_PED_SUFFERS_CRITICAL_HITS(Ped ped, BOOL toggle) {
			Invoker::Invoke<void, 482>(ped, toggle);
		}

		void SET_PED_UPPER_BODY_DAMAGE_ONLY(Ped ped, BOOL toggle) {
			Invoker::Invoke<void, 483>(ped, toggle);
		}

		BOOL IS_PED_SITTING_IN_VEHICLE(Ped ped, Vehicle vehicle) {
			return Invoker::Invoke<BOOL, 484>(ped, vehicle);
		}

		BOOL IS_PED_SITTING_IN_ANY_VEHICLE(Ped ped) {
			return Invoker::Invoke<BOOL, 485>(ped);
		}

		BOOL IS_PED_ON_FOOT(Ped ped) {
			return Invoker::Invoke<BOOL, 486>(ped);
		}

		BOOL IS_PED_ON_ANY_BIKE(Ped ped) {
			return Invoker::Invoke<BOOL, 487>(ped);
		}

		BOOL IS_PED_PLANTING_BOMB(Ped ped) {
			return Invoker::Invoke<BOOL, 488>(ped);
		}

		Vector3 GET_DEAD_PED_PICKUP_COORDS(Ped ped, float p1, float p2) {
			return Invoker::Invoke<Vector3, 489>(ped, p1, p2);
		}

		BOOL IS_PED_IN_ANY_BOAT(Ped ped) {
			return Invoker::Invoke<BOOL, 490>(ped);
		}

		BOOL IS_PED_IN_ANY_SUB(Ped ped) {
			return Invoker::Invoke<BOOL, 491>(ped);
		}

		BOOL IS_PED_IN_ANY_HELI(Ped ped) {
			return Invoker::Invoke<BOOL, 492>(ped);
		}

		BOOL IS_PED_IN_ANY_PLANE(Ped ped) {
			return Invoker::Invoke<BOOL, 493>(ped);
		}

		BOOL IS_PED_IN_FLYING_VEHICLE(Ped ped) {
			return Invoker::Invoke<BOOL, 494>(ped);
		}

		void SET_PED_DIES_IN_WATER(Ped ped, BOOL toggle) {
			Invoker::Invoke<void, 495>(ped, toggle);
		}

		BOOL GET_PED_DIES_IN_WATER(Ped ped) {
			return Invoker::Invoke<BOOL, 496>(ped);
		}

		void SET_PED_DIES_IN_SINKING_VEHICLE(Ped ped, BOOL toggle) {
			Invoker::Invoke<void, 497>(ped, toggle);
		}

		int GET_PED_ARMOUR(Ped ped) {
			return Invoker::Invoke<int, 498>(ped);
		}

		void SET_PED_STAY_IN_VEHICLE_WHEN_JACKED(Ped ped, BOOL toggle) {
			Invoker::Invoke<void, 499>(ped, toggle);
		}

		void SET_PED_CAN_BE_SHOT_IN_VEHICLE(Ped ped, BOOL toggle) {
			Invoker::Invoke<void, 500>(ped, toggle);
		}

		BOOL GET_PED_LAST_DAMAGE_BONE(Ped ped, int* outBone) {
			return Invoker::Invoke<BOOL, 501>(ped, outBone);
		}

		void CLEAR_PED_LAST_DAMAGE_BONE(Ped ped) {
			Invoker::Invoke<void, 502>(ped);
		}

		void SET_AI_WEAPON_DAMAGE_MODIFIER(float value) {
			Invoker::Invoke<void, 503>(value);
		}

		void RESET_AI_WEAPON_DAMAGE_MODIFIER() {
			Invoker::Invoke<void, 504>();
		}

		void SET_AI_MELEE_WEAPON_DAMAGE_MODIFIER(float modifier) {
			Invoker::Invoke<void, 505>(modifier);
		}

		void RESET_AI_MELEE_WEAPON_DAMAGE_MODIFIER() {
			Invoker::Invoke<void, 506>();
		}

		void SET_TREAT_AS_AMBIENT_PED_FOR_DRIVER_LOCKON(Ped ped, BOOL p1) {
			Invoker::Invoke<void, 507>(ped, p1);
		}

		void SET_PED_CAN_BE_TARGETTED(Ped ped, BOOL toggle) {
			Invoker::Invoke<void, 508>(ped, toggle);
		}

		void SET_PED_CAN_BE_TARGETTED_BY_TEAM(Ped ped, int team, BOOL toggle) {
			Invoker::Invoke<void, 509>(ped, team, toggle);
		}

		void SET_PED_CAN_BE_TARGETTED_BY_PLAYER(Ped ped, Player player, BOOL toggle) {
			Invoker::Invoke<void, 510>(ped, player, toggle);
		}

		void SET_ALLOW_LOCKON_TO_PED_IF_FRIENDLY(Ped ped, BOOL toggle) {
			Invoker::Invoke<void, 511>(ped, toggle);
		}

		void SET_USE_CAMERA_HEADING_FOR_DESIRED_DIRECTION_LOCK_ON_TEST(Ped ped, BOOL toggle) {
			Invoker::Invoke<void, 512>(ped, toggle);
		}

		BOOL IS_PED_IN_ANY_POLICE_VEHICLE(Ped ped) {
			return Invoker::Invoke<BOOL, 513>(ped);
		}

		void FORCE_PED_TO_OPEN_PARACHUTE(Ped ped) {
			Invoker::Invoke<void, 514>(ped);
		}

		BOOL IS_PED_IN_PARACHUTE_FREE_FALL(Ped ped) {
			return Invoker::Invoke<BOOL, 515>(ped);
		}

		BOOL IS_PED_FALLING(Ped ped) {
			return Invoker::Invoke<BOOL, 516>(ped);
		}

		BOOL IS_PED_JUMPING(Ped ped) {
			return Invoker::Invoke<BOOL, 517>(ped);
		}

		BOOL IS_PED_LANDING(Any p0) {
			return Invoker::Invoke<BOOL, 518>(p0);
		}

		BOOL IS_PED_DOING_A_BEAST_JUMP(Any p0) {
			return Invoker::Invoke<BOOL, 519>(p0);
		}

		BOOL IS_PED_CLIMBING(Ped ped) {
			return Invoker::Invoke<BOOL, 520>(ped);
		}

		BOOL IS_PED_VAULTING(Ped ped) {
			return Invoker::Invoke<BOOL, 521>(ped);
		}

		BOOL IS_PED_DIVING(Ped ped) {
			return Invoker::Invoke<BOOL, 522>(ped);
		}

		BOOL IS_PED_JUMPING_OUT_OF_VEHICLE(Ped ped) {
			return Invoker::Invoke<BOOL, 523>(ped);
		}

		BOOL IS_PED_OPENING_DOOR(Ped ped) {
			return Invoker::Invoke<BOOL, 524>(ped);
		}

		int GET_PED_PARACHUTE_STATE(Ped ped) {
			return Invoker::Invoke<int, 525>(ped);
		}

		int GET_PED_PARACHUTE_LANDING_TYPE(Ped ped) {
			return Invoker::Invoke<int, 526>(ped);
		}

		void SET_PED_PARACHUTE_TINT_INDEX(Ped ped, int tintIndex) {
			Invoker::Invoke<void, 527>(ped, tintIndex);
		}

		void GET_PED_PARACHUTE_TINT_INDEX(Ped ped, int* outTintIndex) {
			Invoker::Invoke<void, 528>(ped, outTintIndex);
		}

		void SET_PED_RESERVE_PARACHUTE_TINT_INDEX(Ped ped, Any p1) {
			Invoker::Invoke<void, 529>(ped, p1);
		}

		Object CREATE_PARACHUTE_BAG_OBJECT(Ped ped, BOOL p1, BOOL p2) {
			return Invoker::Invoke<Object, 530>(ped, p1, p2);
		}

		void SET_PED_DUCKING(Ped ped, BOOL toggle) {
			Invoker::Invoke<void, 531>(ped, toggle);
		}

		BOOL IS_PED_DUCKING(Ped ped) {
			return Invoker::Invoke<BOOL, 532>(ped);
		}

		BOOL IS_PED_IN_ANY_TAXI(Ped ped) {
			return Invoker::Invoke<BOOL, 533>(ped);
		}

		void SET_PED_ID_RANGE(Ped ped, float value) {
			Invoker::Invoke<void, 534>(ped, value);
		}

		void SET_PED_HIGHLY_PERCEPTIVE(Ped ped, BOOL toggle) {
			Invoker::Invoke<void, 535>(ped, toggle);
		}

		void SET_COP_PERCEPTION_OVERRIDES(float seeingRange, float seeingRangePeripheral, float hearingRange, float visualFieldMinAzimuthAngle, float visualFieldMaxAzimuthAngle, float fieldOfGazeMaxAngle, float p6) {
			Invoker::Invoke<void, 536>(seeingRange, seeingRangePeripheral, hearingRange, visualFieldMinAzimuthAngle, visualFieldMaxAzimuthAngle, fieldOfGazeMaxAngle, p6);
		}

		void SET_PED_INJURED_ON_GROUND_BEHAVIOUR(Ped ped, float p1) {
			Invoker::Invoke<void, 537>(ped, p1);
		}

		void DISABLE_PED_INJURED_ON_GROUND_BEHAVIOUR(Ped ped) {
			Invoker::Invoke<void, 538>(ped);
		}

		void SET_PED_SEEING_RANGE(Ped ped, float value) {
			Invoker::Invoke<void, 539>(ped, value);
		}

		void SET_PED_HEARING_RANGE(Ped ped, float value) {
			Invoker::Invoke<void, 540>(ped, value);
		}

		void SET_PED_VISUAL_FIELD_MIN_ANGLE(Ped ped, float value) {
			Invoker::Invoke<void, 541>(ped, value);
		}

		void SET_PED_VISUAL_FIELD_MAX_ANGLE(Ped ped, float value) {
			Invoker::Invoke<void, 542>(ped, value);
		}

		void SET_PED_VISUAL_FIELD_MIN_ELEVATION_ANGLE(Ped ped, float angle) {
			Invoker::Invoke<void, 543>(ped, angle);
		}

		void SET_PED_VISUAL_FIELD_MAX_ELEVATION_ANGLE(Ped ped, float angle) {
			Invoker::Invoke<void, 544>(ped, angle);
		}

		void SET_PED_VISUAL_FIELD_PERIPHERAL_RANGE(Ped ped, float range) {
			Invoker::Invoke<void, 545>(ped, range);
		}

		void SET_PED_VISUAL_FIELD_CENTER_ANGLE(Ped ped, float angle) {
			Invoker::Invoke<void, 546>(ped, angle);
		}

		float GET_PED_VISUAL_FIELD_CENTER_ANGLE(Ped ped) {
			return Invoker::Invoke<float, 547>(ped);
		}

		void SET_PED_STEALTH_MOVEMENT(Ped ped, BOOL p1, const char* action) {
			Invoker::Invoke<void, 548>(ped, p1, action);
		}

		BOOL GET_PED_STEALTH_MOVEMENT(Ped ped) {
			return Invoker::Invoke<BOOL, 549>(ped);
		}

		int CREATE_GROUP(int unused) {
			return Invoker::Invoke<int, 550>(unused);
		}

		void SET_PED_AS_GROUP_LEADER(Ped ped, int groupId) {
			Invoker::Invoke<void, 551>(ped, groupId);
		}

		void SET_PED_AS_GROUP_MEMBER(Ped ped, int groupId) {
			Invoker::Invoke<void, 552>(ped, groupId);
		}

		void SET_PED_CAN_TELEPORT_TO_GROUP_LEADER(Ped pedHandle, int groupHandle, BOOL toggle) {
			Invoker::Invoke<void, 553>(pedHandle, groupHandle, toggle);
		}

		void REMOVE_GROUP(int groupId) {
			Invoker::Invoke<void, 554>(groupId);
		}

		void REMOVE_PED_FROM_GROUP(Ped ped) {
			Invoker::Invoke<void, 555>(ped);
		}

		BOOL IS_PED_GROUP_MEMBER(Ped ped, int groupId) {
			return Invoker::Invoke<BOOL, 556>(ped, groupId);
		}

		BOOL IS_PED_HANGING_ON_TO_VEHICLE(Ped ped) {
			return Invoker::Invoke<BOOL, 557>(ped);
		}

		void SET_GROUP_SEPARATION_RANGE(int groupHandle, float separationRange) {
			Invoker::Invoke<void, 558>(groupHandle, separationRange);
		}

		void SET_PED_MIN_GROUND_TIME_FOR_STUNGUN(Ped ped, int ms) {
			Invoker::Invoke<void, 559>(ped, ms);
		}

		BOOL IS_PED_PRONE(Ped ped) {
			return Invoker::Invoke<BOOL, 560>(ped);
		}

		BOOL IS_PED_IN_COMBAT(Ped ped, Ped target) {
			return Invoker::Invoke<BOOL, 561>(ped, target);
		}

		Entity GET_PED_TARGET_FROM_COMBAT_PED(Ped ped, Any p1) {
			return Invoker::Invoke<Entity, 562>(ped, p1);
		}

		BOOL CAN_PED_IN_COMBAT_SEE_TARGET(Ped ped, Ped target) {
			return Invoker::Invoke<BOOL, 563>(ped, target);
		}

		BOOL IS_PED_DOING_DRIVEBY(Ped ped) {
			return Invoker::Invoke<BOOL, 564>(ped);
		}

		BOOL IS_PED_JACKING(Ped ped) {
			return Invoker::Invoke<BOOL, 565>(ped);
		}

		BOOL IS_PED_BEING_JACKED(Ped ped) {
			return Invoker::Invoke<BOOL, 566>(ped);
		}

		BOOL IS_PED_BEING_STUNNED(Ped ped, int p1) {
			return Invoker::Invoke<BOOL, 567>(ped, p1);
		}

		Ped GET_PEDS_JACKER(Ped ped) {
			return Invoker::Invoke<Ped, 568>(ped);
		}

		Ped GET_JACK_TARGET(Ped ped) {
			return Invoker::Invoke<Ped, 569>(ped);
		}

		BOOL IS_PED_FLEEING(Ped ped) {
			return Invoker::Invoke<BOOL, 570>(ped);
		}

		BOOL IS_PED_IN_COVER(Ped ped, BOOL exceptUseWeapon) {
			return Invoker::Invoke<BOOL, 571>(ped, exceptUseWeapon);
		}

		BOOL IS_PED_IN_COVER_FACING_LEFT(Ped ped) {
			return Invoker::Invoke<BOOL, 572>(ped);
		}

		BOOL IS_PED_IN_HIGH_COVER(Ped ped) {
			return Invoker::Invoke<BOOL, 573>(ped);
		}

		BOOL IS_PED_GOING_INTO_COVER(Ped ped) {
			return Invoker::Invoke<BOOL, 574>(ped);
		}

		BOOL SET_PED_PINNED_DOWN(Ped ped, BOOL pinned, int i) {
			return Invoker::Invoke<BOOL, 575>(ped, pinned, i);
		}

		BOOL _HAS_PED_CLEAR_LOS_TO_ENTITY(Ped ped, Entity entity, float x, float y, float z, int p5, BOOL p6, BOOL p7) {
			return Invoker::Invoke<BOOL, 576>(ped, entity, x, y, z, p5, p6, p7);
		}

		int GET_SEAT_PED_IS_TRYING_TO_ENTER(Ped ped) {
			return Invoker::Invoke<int, 577>(ped);
		}

		Vehicle GET_VEHICLE_PED_IS_TRYING_TO_ENTER(Ped ped) {
			return Invoker::Invoke<Vehicle, 578>(ped);
		}

		Entity GET_PED_SOURCE_OF_DEATH(Ped ped) {
			return Invoker::Invoke<Entity, 579>(ped);
		}

		Hash GET_PED_CAUSE_OF_DEATH(Ped ped) {
			return Invoker::Invoke<Hash, 580>(ped);
		}

		int GET_PED_TIME_OF_DEATH(Ped ped) {
			return Invoker::Invoke<int, 581>(ped);
		}

		int COUNT_PEDS_IN_COMBAT_WITH_TARGET(Ped ped) {
			return Invoker::Invoke<int, 582>(ped);
		}

		int COUNT_PEDS_IN_COMBAT_WITH_TARGET_WITHIN_RADIUS(Ped ped, float x, float y, float z, float radius) {
			return Invoker::Invoke<int, 583>(ped, x, y, z, radius);
		}

		void SET_PED_RELATIONSHIP_GROUP_DEFAULT_HASH(Ped ped, Hash hash) {
			Invoker::Invoke<void, 584>(ped, hash);
		}

		void SET_PED_RELATIONSHIP_GROUP_HASH(Ped ped, Hash hash) {
			Invoker::Invoke<void, 585>(ped, hash);
		}

		void SET_RELATIONSHIP_BETWEEN_GROUPS(int relationship, Hash group1, Hash group2) {
			Invoker::Invoke<void, 586>(relationship, group1, group2);
		}

		void CLEAR_RELATIONSHIP_BETWEEN_GROUPS(int relationship, Hash group1, Hash group2) {
			Invoker::Invoke<void, 587>(relationship, group1, group2);
		}

		BOOL ADD_RELATIONSHIP_GROUP(const char* name, Hash* groupHash) {
			return Invoker::Invoke<BOOL, 588>(name, groupHash);
		}

		void REMOVE_RELATIONSHIP_GROUP(Hash groupHash) {
			Invoker::Invoke<void, 589>(groupHash);
		}

		BOOL DOES_RELATIONSHIP_GROUP_EXIST(Hash groupHash) {
			return Invoker::Invoke<BOOL, 590>(groupHash);
		}

		int GET_RELATIONSHIP_BETWEEN_PEDS(Ped ped1, Ped ped2) {
			return Invoker::Invoke<int, 591>(ped1, ped2);
		}

		Hash GET_PED_RELATIONSHIP_GROUP_DEFAULT_HASH(Ped ped) {
			return Invoker::Invoke<Hash, 592>(ped);
		}

		Hash GET_PED_RELATIONSHIP_GROUP_HASH(Ped ped) {
			return Invoker::Invoke<Hash, 593>(ped);
		}

		int GET_RELATIONSHIP_BETWEEN_GROUPS(Hash group1, Hash group2) {
			return Invoker::Invoke<int, 594>(group1, group2);
		}

		void SET_RELATIONSHIP_GROUP_AFFECTS_WANTED_LEVEL(Hash group, BOOL p1) {
			Invoker::Invoke<void, 595>(group, p1);
		}

		void TELL_GROUP_PEDS_IN_AREA_TO_ATTACK(Ped ped, Any p1, float p2, Hash hash, Any p4, Any p5) {
			Invoker::Invoke<void, 596>(ped, p1, p2, hash, p4, p5);
		}

		void SET_PED_CAN_BE_TARGETED_WITHOUT_LOS(Ped ped, BOOL toggle) {
			Invoker::Invoke<void, 597>(ped, toggle);
		}

		void SET_PED_TO_INFORM_RESPECTED_FRIENDS(Ped ped, float radius, int maxFriends) {
			Invoker::Invoke<void, 598>(ped, radius, maxFriends);
		}

		BOOL IS_PED_RESPONDING_TO_EVENT(Ped ped, Any event) {
			return Invoker::Invoke<BOOL, 599>(ped, event);
		}

		BOOL GET_POS_FROM_FIRED_EVENT(Ped ped, int eventType, Any* outData) {
			return Invoker::Invoke<BOOL, 600>(ped, eventType, outData);
		}

		void SET_PED_FIRING_PATTERN(Ped ped, Hash patternHash) {
			Invoker::Invoke<void, 601>(ped, patternHash);
		}

		void SET_PED_SHOOT_RATE(Ped ped, int shootRate) {
			Invoker::Invoke<void, 602>(ped, shootRate);
		}

		void SET_COMBAT_FLOAT(Ped ped, int combatType, float p2) {
			Invoker::Invoke<void, 603>(ped, combatType, p2);
		}

		float GET_COMBAT_FLOAT(Ped ped, int p1) {
			return Invoker::Invoke<float, 604>(ped, p1);
		}

		void GET_GROUP_SIZE(int groupID, Any* p1, int* sizeInMembers) {
			Invoker::Invoke<void, 605>(groupID, p1, sizeInMembers);
		}

		BOOL DOES_GROUP_EXIST(int groupId) {
			return Invoker::Invoke<BOOL, 606>(groupId);
		}

		int GET_PED_GROUP_INDEX(Ped ped) {
			return Invoker::Invoke<int, 607>(ped);
		}

		BOOL IS_PED_IN_GROUP(Ped ped) {
			return Invoker::Invoke<BOOL, 608>(ped);
		}

		Player GET_PLAYER_PED_IS_FOLLOWING(Ped ped) {
			return Invoker::Invoke<Player, 609>(ped);
		}

		void SET_GROUP_FORMATION(int groupId, int formationType) {
			Invoker::Invoke<void, 610>(groupId, formationType);
		}

		void SET_GROUP_FORMATION_SPACING(int groupId, float x, float y, float z) {
			Invoker::Invoke<void, 611>(groupId, x, y, z);
		}

		void RESET_GROUP_FORMATION_DEFAULT_SPACING(int groupHandle) {
			Invoker::Invoke<void, 612>(groupHandle);
		}

		Vehicle GET_VEHICLE_PED_IS_USING(Ped ped) {
			return Invoker::Invoke<Vehicle, 613>(ped);
		}

		Vehicle GET_VEHICLE_PED_IS_ENTERING(Ped ped) {
			return Invoker::Invoke<Vehicle, 614>(ped);
		}

		void SET_PED_GRAVITY(Ped ped, BOOL toggle) {
			Invoker::Invoke<void, 615>(ped, toggle);
		}

		BOOL _SET_PED_SURVIVES_BEING_OUT_OF_WATER(Ped ped, BOOL toggle) {
			return Invoker::Invoke<BOOL, 6620>(ped, toggle);
		}

		void APPLY_DAMAGE_TO_PED(Ped ped, int damageAmount, BOOL p2, Any p3, Hash weaponType) {
			Invoker::Invoke<void, 616>(ped, damageAmount, p2, p3, weaponType);
		}

		int GET_TIME_PED_DAMAGED_BY_WEAPON(Ped ped, Hash weaponHash) {
			return Invoker::Invoke<int, 617>(ped, weaponHash);
		}

		void SET_PED_ALLOWED_TO_DUCK(Ped ped, BOOL toggle) {
			Invoker::Invoke<void, 618>(ped, toggle);
		}

		void SET_PED_NEVER_LEAVES_GROUP(Ped ped, BOOL toggle) {
			Invoker::Invoke<void, 619>(ped, toggle);
		}

		int GET_PED_TYPE(Ped ped) {
			return Invoker::Invoke<int, 620>(ped);
		}

		void SET_PED_AS_COP(Ped ped, BOOL toggle) {
			Invoker::Invoke<void, 621>(ped, toggle);
		}

		void SET_PED_HEALTH_PENDING_LAST_DAMAGE_EVENT_OVERRIDE_FLAG(BOOL toggle) {
			Invoker::Invoke<void, 622>(toggle);
		}

		void SET_PED_MAX_HEALTH(Ped ped, int value) {
			Invoker::Invoke<void, 623>(ped, value);
		}

		int GET_PED_MAX_HEALTH(Ped ped) {
			return Invoker::Invoke<int, 624>(ped);
		}

		void SET_PED_MAX_TIME_IN_WATER(Ped ped, float value) {
			Invoker::Invoke<void, 625>(ped, value);
		}

		void SET_PED_MAX_TIME_UNDERWATER(Ped ped, float value) {
			Invoker::Invoke<void, 626>(ped, value);
		}

		void SET_CORPSE_RAGDOLL_FRICTION(Ped ped, float p1) {
			Invoker::Invoke<void, 627>(ped, p1);
		}

		void SET_PED_VEHICLE_FORCED_SEAT_USAGE(Ped ped, Vehicle vehicle, int seatIndex, int flags, Any p4) {
			Invoker::Invoke<void, 628>(ped, vehicle, seatIndex, flags, p4);
		}

		void CLEAR_ALL_PED_VEHICLE_FORCED_SEAT_USAGE(Ped ped) {
			Invoker::Invoke<void, 629>(ped);
		}

		void SET_PED_CAN_BE_KNOCKED_OFF_BIKE(Any p0, Any p1) {
			Invoker::Invoke<void, 630>(p0, p1);
		}

		void SET_PED_CAN_BE_KNOCKED_OFF_VEHICLE(Ped ped, int state) {
			Invoker::Invoke<void, 631>(ped, state);
		}

		BOOL CAN_KNOCK_PED_OFF_VEHICLE(Ped ped) {
			return Invoker::Invoke<BOOL, 632>(ped);
		}

		void KNOCK_PED_OFF_VEHICLE(Ped ped) {
			Invoker::Invoke<void, 633>(ped);
		}

		void SET_PED_COORDS_NO_GANG(Ped ped, float posX, float posY, float posZ) {
			Invoker::Invoke<void, 634>(ped, posX, posY, posZ);
		}

		Ped GET_PED_AS_GROUP_MEMBER(int groupID, int memberNumber) {
			return Invoker::Invoke<Ped, 635>(groupID, memberNumber);
		}

		Ped GET_PED_AS_GROUP_LEADER(int groupID) {
			return Invoker::Invoke<Ped, 636>(groupID);
		}

		void SET_PED_KEEP_TASK(Ped ped, BOOL toggle) {
			Invoker::Invoke<void, 637>(ped, toggle);
		}

		void SET_PED_ALLOW_MINOR_REACTIONS_AS_MISSION_PED(Ped ped, BOOL toggle) {
			Invoker::Invoke<void, 638>(ped, toggle);
		}

		BOOL IS_PED_SWIMMING(Ped ped) {
			return Invoker::Invoke<BOOL, 639>(ped);
		}

		BOOL IS_PED_SWIMMING_UNDER_WATER(Ped ped) {
			return Invoker::Invoke<BOOL, 640>(ped);
		}

		void SET_PED_COORDS_KEEP_VEHICLE(Ped ped, float posX, float posY, float posZ) {
			Invoker::Invoke<void, 641>(ped, posX, posY, posZ);
		}

		void SET_PED_DIES_IN_VEHICLE(Ped ped, BOOL toggle) {
			Invoker::Invoke<void, 642>(ped, toggle);
		}

		void SET_CREATE_RANDOM_COPS(BOOL toggle) {
			Invoker::Invoke<void, 643>(toggle);
		}

		void SET_CREATE_RANDOM_COPS_NOT_ON_SCENARIOS(BOOL toggle) {
			Invoker::Invoke<void, 644>(toggle);
		}

		void SET_CREATE_RANDOM_COPS_ON_SCENARIOS(BOOL toggle) {
			Invoker::Invoke<void, 645>(toggle);
		}

		BOOL CAN_CREATE_RANDOM_COPS() {
			return Invoker::Invoke<BOOL, 646>();
		}

		void SET_PED_AS_ENEMY(Ped ped, BOOL toggle) {
			Invoker::Invoke<void, 647>(ped, toggle);
		}

		void SET_PED_CAN_SMASH_GLASS(Ped ped, BOOL p1, BOOL p2) {
			Invoker::Invoke<void, 648>(ped, p1, p2);
		}

		BOOL IS_PED_IN_ANY_TRAIN(Ped ped) {
			return Invoker::Invoke<BOOL, 649>(ped);
		}

		BOOL IS_PED_GETTING_INTO_A_VEHICLE(Ped ped) {
			return Invoker::Invoke<BOOL, 650>(ped);
		}

		BOOL IS_PED_TRYING_TO_ENTER_A_LOCKED_VEHICLE(Ped ped) {
			return Invoker::Invoke<BOOL, 651>(ped);
		}

		void SET_ENABLE_HANDCUFFS(Ped ped, BOOL toggle) {
			Invoker::Invoke<void, 652>(ped, toggle);
		}

		void SET_ENABLE_BOUND_ANKLES(Ped ped, BOOL toggle) {
			Invoker::Invoke<void, 653>(ped, toggle);
		}

		void SET_ENABLE_SCUBA(Ped ped, BOOL toggle) {
			Invoker::Invoke<void, 654>(ped, toggle);
		}

		void SET_CAN_ATTACK_FRIENDLY(Ped ped, BOOL toggle, BOOL p2) {
			Invoker::Invoke<void, 655>(ped, toggle, p2);
		}

		int GET_PED_ALERTNESS(Ped ped) {
			return Invoker::Invoke<int, 656>(ped);
		}

		void SET_PED_ALERTNESS(Ped ped, int value) {
			Invoker::Invoke<void, 657>(ped, value);
		}

		void SET_PED_GET_OUT_UPSIDE_DOWN_VEHICLE(Ped ped, BOOL toggle) {
			Invoker::Invoke<void, 658>(ped, toggle);
		}

		void SET_PED_MOVEMENT_CLIPSET(Ped ped, const char* clipSet, float transitionSpeed) {
			Invoker::Invoke<void, 659>(ped, clipSet, transitionSpeed);
		}

		void RESET_PED_MOVEMENT_CLIPSET(Ped ped, float p1) {
			Invoker::Invoke<void, 660>(ped, p1);
		}

		void SET_PED_STRAFE_CLIPSET(Ped ped, const char* clipSet) {
			Invoker::Invoke<void, 661>(ped, clipSet);
		}

		void RESET_PED_STRAFE_CLIPSET(Ped ped) {
			Invoker::Invoke<void, 662>(ped);
		}

		void SET_PED_WEAPON_MOVEMENT_CLIPSET(Ped ped, const char* clipSet) {
			Invoker::Invoke<void, 663>(ped, clipSet);
		}

		void RESET_PED_WEAPON_MOVEMENT_CLIPSET(Ped ped) {
			Invoker::Invoke<void, 664>(ped);
		}

		void SET_PED_DRIVE_BY_CLIPSET_OVERRIDE(Ped ped, const char* clipset) {
			Invoker::Invoke<void, 665>(ped, clipset);
		}

		void CLEAR_PED_DRIVE_BY_CLIPSET_OVERRIDE(Ped ped) {
			Invoker::Invoke<void, 666>(ped);
		}

		void SET_PED_MOTION_IN_COVER_CLIPSET_OVERRIDE(Ped ped, const char* p1) {
			Invoker::Invoke<void, 667>(ped, p1);
		}

		void CLEAR_PED_MOTION_IN_COVER_CLIPSET_OVERRIDE(Ped ped) {
			Invoker::Invoke<void, 668>(ped);
		}

		void CLEAR_PED_FALL_UPPER_BODY_CLIPSET_OVERRIDE(Ped ped) {
			Invoker::Invoke<void, 669>(ped);
		}

		void SET_PED_IN_VEHICLE_CONTEXT(Ped ped, Hash context) {
			Invoker::Invoke<void, 670>(ped, context);
		}

		void RESET_PED_IN_VEHICLE_CONTEXT(Ped ped) {
			Invoker::Invoke<void, 671>(ped);
		}

		BOOL IS_SCRIPTED_SCENARIO_PED_USING_CONDITIONAL_ANIM(Ped ped, const char* animDict, const char* anim) {
			return Invoker::Invoke<BOOL, 672>(ped, animDict, anim);
		}

		void SET_PED_ALTERNATE_WALK_ANIM(Ped ped, const char* animDict, const char* animName, float p3, BOOL p4) {
			Invoker::Invoke<void, 673>(ped, animDict, animName, p3, p4);
		}

		void CLEAR_PED_ALTERNATE_WALK_ANIM(Ped ped, float p1) {
			Invoker::Invoke<void, 674>(ped, p1);
		}

		void SET_PED_ALTERNATE_MOVEMENT_ANIM(Ped ped, int stance, const char* animDictionary, const char* animationName, float p4, BOOL p5) {
			Invoker::Invoke<void, 675>(ped, stance, animDictionary, animationName, p4, p5);
		}

		void CLEAR_PED_ALTERNATE_MOVEMENT_ANIM(Ped ped, int stance, float p2) {
			Invoker::Invoke<void, 676>(ped, stance, p2);
		}

		void SET_PED_GESTURE_GROUP(Ped ped, const char* animGroupGesture) {
			Invoker::Invoke<void, 677>(ped, animGroupGesture);
		}

		Vector3 GET_ANIM_INITIAL_OFFSET_POSITION(const char* animDict, const char* animName, float x, float y, float z, float xRot, float yRot, float zRot, float p8, int p9) {
			return Invoker::Invoke<Vector3, 678>(animDict, animName, x, y, z, xRot, yRot, zRot, p8, p9);
		}

		Vector3 GET_ANIM_INITIAL_OFFSET_ROTATION(const char* animDict, const char* animName, float x, float y, float z, float xRot, float yRot, float zRot, float p8, int p9) {
			return Invoker::Invoke<Vector3, 679>(animDict, animName, x, y, z, xRot, yRot, zRot, p8, p9);
		}

		int GET_PED_DRAWABLE_VARIATION(Ped ped, int componentId) {
			return Invoker::Invoke<int, 680>(ped, componentId);
		}

		int GET_NUMBER_OF_PED_DRAWABLE_VARIATIONS(Ped ped, int componentId) {
			return Invoker::Invoke<int, 681>(ped, componentId);
		}

		int GET_PED_TEXTURE_VARIATION(Ped ped, int componentId) {
			return Invoker::Invoke<int, 682>(ped, componentId);
		}

		int GET_NUMBER_OF_PED_TEXTURE_VARIATIONS(Ped ped, int componentId, int drawableId) {
			return Invoker::Invoke<int, 683>(ped, componentId, drawableId);
		}

		int GET_NUMBER_OF_PED_PROP_DRAWABLE_VARIATIONS(Ped ped, int propId) {
			return Invoker::Invoke<int, 684>(ped, propId);
		}

		int GET_NUMBER_OF_PED_PROP_TEXTURE_VARIATIONS(Ped ped, int propId, int drawableId) {
			return Invoker::Invoke<int, 685>(ped, propId, drawableId);
		}

		int GET_PED_PALETTE_VARIATION(Ped ped, int componentId) {
			return Invoker::Invoke<int, 686>(ped, componentId);
		}

		BOOL GET_MP_OUTFIT_DATA_FROM_METADATA(Any* p0, Any* p1) {
			return Invoker::Invoke<BOOL, 687>(p0, p1);
		}

		int GET_FM_MALE_SHOP_PED_APPAREL_ITEM_INDEX(int p0) {
			return Invoker::Invoke<int, 688>(p0);
		}

		int GET_FM_FEMALE_SHOP_PED_APPAREL_ITEM_INDEX(int p0) {
			return Invoker::Invoke<int, 689>(p0);
		}

		BOOL IS_PED_COMPONENT_VARIATION_VALID(Ped ped, int componentId, int drawableId, int textureId) {
			return Invoker::Invoke<BOOL, 690>(ped, componentId, drawableId, textureId);
		}

		void SET_PED_COMPONENT_VARIATION(Ped ped, int componentId, int drawableId, int textureId, int paletteId) {
			Invoker::Invoke<void, 691>(ped, componentId, drawableId, textureId, paletteId);
		}

		void SET_PED_RANDOM_COMPONENT_VARIATION(Ped ped, int p1) {
			Invoker::Invoke<void, 692>(ped, p1);
		}

		void SET_PED_RANDOM_PROPS(Ped ped) {
			Invoker::Invoke<void, 693>(ped);
		}

		void SET_PED_DEFAULT_COMPONENT_VARIATION(Ped ped) {
			Invoker::Invoke<void, 694>(ped);
		}

		void SET_PED_BLEND_FROM_PARENTS(Ped ped, Any p1, Any p2, float p3, float p4) {
			Invoker::Invoke<void, 695>(ped, p1, p2, p3, p4);
		}

		void SET_PED_HEAD_BLEND_DATA(Ped ped, int shapeFirstID, int shapeSecondID, int shapeThirdID, int skinFirstID, int skinSecondID, int skinThirdID, float shapeMix, float skinMix, float thirdMix, BOOL isParent) {
			Invoker::Invoke<void, 696>(ped, shapeFirstID, shapeSecondID, shapeThirdID, skinFirstID, skinSecondID, skinThirdID, shapeMix, skinMix, thirdMix, isParent);
		}

		BOOL GET_PED_HEAD_BLEND_DATA(Ped ped, Any* headBlendData) {
			return Invoker::Invoke<BOOL, 697>(ped, headBlendData);
		}

		void UPDATE_PED_HEAD_BLEND_DATA(Ped ped, float shapeMix, float skinMix, float thirdMix) {
			Invoker::Invoke<void, 698>(ped, shapeMix, skinMix, thirdMix);
		}

		void SET_HEAD_BLEND_EYE_COLOR(Ped ped, int index) {
			Invoker::Invoke<void, 699>(ped, index);
		}

		int GET_HEAD_BLEND_EYE_COLOR(Ped ped) {
			return Invoker::Invoke<int, 700>(ped);
		}

		void SET_PED_HEAD_OVERLAY(Ped ped, int overlayID, int index, float opacity) {
			Invoker::Invoke<void, 701>(ped, overlayID, index, opacity);
		}

		int GET_PED_HEAD_OVERLAY(Ped ped, int overlayID) {
			return Invoker::Invoke<int, 702>(ped, overlayID);
		}

		int GET_PED_HEAD_OVERLAY_NUM(int overlayID) {
			return Invoker::Invoke<int, 703>(overlayID);
		}

		void SET_PED_HEAD_OVERLAY_TINT(Ped ped, int overlayID, int colorType, int colorID, int secondColorID) {
			Invoker::Invoke<void, 704>(ped, overlayID, colorType, colorID, secondColorID);
		}

		void SET_PED_HAIR_TINT(Ped ped, int colorID, int highlightColorID) {
			Invoker::Invoke<void, 705>(ped, colorID, highlightColorID);
		}

		int GET_NUM_PED_HAIR_TINTS() {
			return Invoker::Invoke<int, 706>();
		}

		int GET_NUM_PED_MAKEUP_TINTS() {
			return Invoker::Invoke<int, 707>();
		}

		void GET_PED_HAIR_TINT_COLOR(int hairColorIndex, int* outR, int* outG, int* outB) {
			Invoker::Invoke<void, 708>(hairColorIndex, outR, outG, outB);
		}

		void GET_PED_MAKEUP_TINT_COLOR(int makeupColorIndex, int* outR, int* outG, int* outB) {
			Invoker::Invoke<void, 709>(makeupColorIndex, outR, outG, outB);
		}

		BOOL IS_PED_HAIR_TINT_FOR_CREATOR(int colorId) {
			return Invoker::Invoke<BOOL, 710>(colorId);
		}

		int GET_DEFAULT_SECONDARY_TINT_FOR_CREATOR(int colorId) {
			return Invoker::Invoke<int, 711>(colorId);
		}

		BOOL IS_PED_LIPSTICK_TINT_FOR_CREATOR(int colorId) {
			return Invoker::Invoke<BOOL, 712>(colorId);
		}

		BOOL IS_PED_BLUSH_TINT_FOR_CREATOR(int colorId) {
			return Invoker::Invoke<BOOL, 713>(colorId);
		}

		BOOL IS_PED_HAIR_TINT_FOR_BARBER(int colorID) {
			return Invoker::Invoke<BOOL, 714>(colorID);
		}

		int GET_DEFAULT_SECONDARY_TINT_FOR_BARBER(int colorID) {
			return Invoker::Invoke<int, 715>(colorID);
		}

		BOOL IS_PED_LIPSTICK_TINT_FOR_BARBER(int colorID) {
			return Invoker::Invoke<BOOL, 716>(colorID);
		}

		BOOL IS_PED_BLUSH_TINT_FOR_BARBER(int colorID) {
			return Invoker::Invoke<BOOL, 717>(colorID);
		}

		BOOL IS_PED_BLUSH_FACEPAINT_TINT_FOR_BARBER(int colorId) {
			return Invoker::Invoke<BOOL, 718>(colorId);
		}

		int GET_TINT_INDEX_FOR_LAST_GEN_HAIR_TEXTURE(Hash modelHash, int drawableId, int textureId) {
			return Invoker::Invoke<int, 719>(modelHash, drawableId, textureId);
		}

		void SET_PED_MICRO_MORPH(Ped ped, int index, float scale) {
			Invoker::Invoke<void, 720>(ped, index, scale);
		}

		BOOL HAS_PED_HEAD_BLEND_FINISHED(Ped ped) {
			return Invoker::Invoke<BOOL, 721>(ped);
		}

		void FINALIZE_HEAD_BLEND(Ped ped) {
			Invoker::Invoke<void, 722>(ped);
		}

		void SET_HEAD_BLEND_PALETTE_COLOR(Ped ped, int r, int g, int b, int id) {
			Invoker::Invoke<void, 723>(ped, r, g, b, id);
		}

		void DISABLE_HEAD_BLEND_PALETTE_COLOR(Ped ped) {
			Invoker::Invoke<void, 724>(ped);
		}

		int GET_PED_HEAD_BLEND_FIRST_INDEX(int type) {
			return Invoker::Invoke<int, 725>(type);
		}

		int GET_PED_HEAD_BLEND_NUM_HEADS(int type) {
			return Invoker::Invoke<int, 726>(type);
		}

		int SET_PED_PRELOAD_VARIATION_DATA(Ped ped, int slot, int drawableId, int textureId) {
			return Invoker::Invoke<int, 727>(ped, slot, drawableId, textureId);
		}

		BOOL HAS_PED_PRELOAD_VARIATION_DATA_FINISHED(Ped ped) {
			return Invoker::Invoke<BOOL, 728>(ped);
		}

		void RELEASE_PED_PRELOAD_VARIATION_DATA(Ped ped) {
			Invoker::Invoke<void, 729>(ped);
		}

		int SET_PED_PRELOAD_PROP_DATA(Ped ped, int componentId, int drawableId, int TextureId) {
			return Invoker::Invoke<int, 730>(ped, componentId, drawableId, TextureId);
		}

		BOOL HAS_PED_PRELOAD_PROP_DATA_FINISHED(Ped ped) {
			return Invoker::Invoke<BOOL, 731>(ped);
		}

		void RELEASE_PED_PRELOAD_PROP_DATA(Ped ped) {
			Invoker::Invoke<void, 732>(ped);
		}

		int GET_PED_PROP_INDEX(Ped ped, int componentId, Any p2) {
			return Invoker::Invoke<int, 733>(ped, componentId, p2);
		}

		void SET_PED_PROP_INDEX(Ped ped, int componentId, int drawableId, int TextureId, BOOL attach, Any p5) {
			Invoker::Invoke<void, 734>(ped, componentId, drawableId, TextureId, attach, p5);
		}

		void KNOCK_OFF_PED_PROP(Ped ped, BOOL p1, BOOL p2, BOOL p3, BOOL p4) {
			Invoker::Invoke<void, 735>(ped, p1, p2, p3, p4);
		}

		void CLEAR_PED_PROP(Ped ped, int propId, Any p2) {
			Invoker::Invoke<void, 736>(ped, propId, p2);
		}

		void CLEAR_ALL_PED_PROPS(Ped ped, Any p1) {
			Invoker::Invoke<void, 737>(ped, p1);
		}

		void DROP_AMBIENT_PROP(Ped ped) {
			Invoker::Invoke<void, 738>(ped);
		}

		int GET_PED_PROP_TEXTURE_INDEX(Ped ped, int componentId) {
			return Invoker::Invoke<int, 739>(ped, componentId);
		}

		void CLEAR_PED_PARACHUTE_PACK_VARIATION(Ped ped) {
			Invoker::Invoke<void, 740>(ped);
		}

		void SET_PED_SCUBA_GEAR_VARIATION(Ped ped) {
			Invoker::Invoke<void, 741>(ped);
		}

		void CLEAR_PED_SCUBA_GEAR_VARIATION(Ped ped) {
			Invoker::Invoke<void, 742>(ped);
		}

		BOOL IS_USING_PED_SCUBA_GEAR_VARIATION(Any p0) {
			return Invoker::Invoke<BOOL, 743>(p0);
		}

		void SET_BLOCKING_OF_NON_TEMPORARY_EVENTS(Ped ped, BOOL toggle) {
			Invoker::Invoke<void, 744>(ped, toggle);
		}

		void SET_PED_BOUNDS_ORIENTATION(Ped ped, float p1, float p2, float x, float y, float z) {
			Invoker::Invoke<void, 745>(ped, p1, p2, x, y, z);
		}

		void REGISTER_TARGET(Ped ped, Ped target) {
			Invoker::Invoke<void, 746>(ped, target);
		}

		void REGISTER_HATED_TARGETS_AROUND_PED(Ped ped, float radius) {
			Invoker::Invoke<void, 747>(ped, radius);
		}

		Ped GET_RANDOM_PED_AT_COORD(float x, float y, float z, float xRadius, float yRadius, float zRadius, int pedType) {
			return Invoker::Invoke<Ped, 748>(x, y, z, xRadius, yRadius, zRadius, pedType);
		}

		BOOL GET_CLOSEST_PED(float x, float y, float z, float radius, BOOL p4, BOOL p5, Ped* outPed, BOOL p7, BOOL p8, int pedType) {
			return Invoker::Invoke<BOOL, 749>(x, y, z, radius, p4, p5, outPed, p7, p8, pedType);
		}

		void SET_SCENARIO_PEDS_TO_BE_RETURNED_BY_NEXT_COMMAND(BOOL value) {
			Invoker::Invoke<void, 750>(value);
		}

		BOOL GET_CAN_PED_BE_GRABBED_BY_SCRIPT(Ped ped, BOOL p1, BOOL p2, BOOL p3, BOOL p4, BOOL p5, BOOL p6, BOOL p7, Any p8) {
			return Invoker::Invoke<BOOL, 751>(ped, p1, p2, p3, p4, p5, p6, p7, p8);
		}

		void SET_DRIVER_RACING_MODIFIER(Ped driver, float modifier) {
			Invoker::Invoke<void, 752>(driver, modifier);
		}

		void SET_DRIVER_ABILITY(Ped driver, float ability) {
			Invoker::Invoke<void, 753>(driver, ability);
		}

		void SET_DRIVER_AGGRESSIVENESS(Ped driver, float aggressiveness) {
			Invoker::Invoke<void, 754>(driver, aggressiveness);
		}

		BOOL CAN_PED_RAGDOLL(Ped ped) {
			return Invoker::Invoke<BOOL, 755>(ped);
		}

		BOOL SET_PED_TO_RAGDOLL(Ped ped, int time1, int time2, int ragdollType, BOOL p4, BOOL p5, BOOL p6) {
			return Invoker::Invoke<BOOL, 756>(ped, time1, time2, ragdollType, p4, p5, p6);
		}

		BOOL SET_PED_TO_RAGDOLL_WITH_FALL(Ped ped, int time, int p2, int ragdollType, float x, float y, float z, float velocity, float p8, float p9, float p10, float p11, float p12, float p13) {
			return Invoker::Invoke<BOOL, 757>(ped, time, p2, ragdollType, x, y, z, velocity, p8, p9, p10, p11, p12, p13);
		}

		void SET_PED_RAGDOLL_ON_COLLISION(Ped ped, BOOL toggle) {
			Invoker::Invoke<void, 758>(ped, toggle);
		}

		BOOL IS_PED_RAGDOLL(Ped ped) {
			return Invoker::Invoke<BOOL, 759>(ped);
		}

		BOOL IS_PED_RUNNING_RAGDOLL_TASK(Ped ped) {
			return Invoker::Invoke<BOOL, 760>(ped);
		}

		void SET_PED_RAGDOLL_FORCE_FALL(Ped ped) {
			Invoker::Invoke<void, 761>(ped);
		}

		void RESET_PED_RAGDOLL_TIMER(Ped ped) {
			Invoker::Invoke<void, 762>(ped);
		}

		void SET_PED_CAN_RAGDOLL(Ped ped, BOOL toggle) {
			Invoker::Invoke<void, 763>(ped, toggle);
		}

		BOOL IS_PED_RUNNING_MELEE_TASK(Ped ped) {
			return Invoker::Invoke<BOOL, 764>(ped);
		}

		BOOL IS_PED_RUNNING_MOBILE_PHONE_TASK(Ped ped) {
			return Invoker::Invoke<BOOL, 765>(ped);
		}

		BOOL IS_MOBILE_PHONE_TO_PED_EAR(Ped ped) {
			return Invoker::Invoke<BOOL, 766>(ped);
		}

		void SET_RAGDOLL_BLOCKING_FLAGS(Ped ped, int blockingFlag) {
			Invoker::Invoke<void, 767>(ped, blockingFlag);
		}

		void CLEAR_RAGDOLL_BLOCKING_FLAGS(Ped ped, int blockingFlag) {
			Invoker::Invoke<void, 768>(ped, blockingFlag);
		}

		void SET_PED_ANGLED_DEFENSIVE_AREA(Ped ped, float p1, float p2, float p3, float p4, float p5, float p6, float p7, BOOL p8, BOOL p9) {
			Invoker::Invoke<void, 769>(ped, p1, p2, p3, p4, p5, p6, p7, p8, p9);
		}

		void SET_PED_SPHERE_DEFENSIVE_AREA(Ped ped, float x, float y, float z, float radius, BOOL p5, BOOL p6) {
			Invoker::Invoke<void, 770>(ped, x, y, z, radius, p5, p6);
		}

		void SET_PED_DEFENSIVE_SPHERE_ATTACHED_TO_PED(Ped ped, Ped target, float xOffset, float yOffset, float zOffset, float radius, BOOL p6) {
			Invoker::Invoke<void, 771>(ped, target, xOffset, yOffset, zOffset, radius, p6);
		}

		void SET_PED_DEFENSIVE_SPHERE_ATTACHED_TO_VEHICLE(Ped ped, Vehicle target, float xOffset, float yOffset, float zOffset, float radius, BOOL p6) {
			Invoker::Invoke<void, 772>(ped, target, xOffset, yOffset, zOffset, radius, p6);
		}

		void SET_PED_DEFENSIVE_AREA_ATTACHED_TO_PED(Ped ped, Ped attachPed, float p2, float p3, float p4, float p5, float p6, float p7, float p8, BOOL p9, BOOL p10) {
			Invoker::Invoke<void, 773>(ped, attachPed, p2, p3, p4, p5, p6, p7, p8, p9, p10);
		}

		void SET_PED_DEFENSIVE_AREA_DIRECTION(Ped ped, float p1, float p2, float p3, BOOL p4) {
			Invoker::Invoke<void, 774>(ped, p1, p2, p3, p4);
		}

		void REMOVE_PED_DEFENSIVE_AREA(Ped ped, BOOL toggle) {
			Invoker::Invoke<void, 775>(ped, toggle);
		}

		Vector3 GET_PED_DEFENSIVE_AREA_POSITION(Ped ped, BOOL p1) {
			return Invoker::Invoke<Vector3, 776>(ped, p1);
		}

		BOOL IS_PED_DEFENSIVE_AREA_ACTIVE(Ped ped, BOOL p1) {
			return Invoker::Invoke<BOOL, 777>(ped, p1);
		}

		void SET_PED_PREFERRED_COVER_SET(Ped ped, Any itemSet) {
			Invoker::Invoke<void, 778>(ped, itemSet);
		}

		void REMOVE_PED_PREFERRED_COVER_SET(Ped ped) {
			Invoker::Invoke<void, 779>(ped);
		}

		void REVIVE_INJURED_PED(Ped ped) {
			Invoker::Invoke<void, 780>(ped);
		}

		void RESURRECT_PED(Ped ped) {
			Invoker::Invoke<void, 781>(ped);
		}

		void SET_PED_NAME_DEBUG(Ped ped, const char* name) {
			Invoker::Invoke<void, 782>(ped, name);
		}

		Vector3 GET_PED_EXTRACTED_DISPLACEMENT(Ped ped, BOOL worldSpace) {
			return Invoker::Invoke<Vector3, 783>(ped, worldSpace);
		}

		void SET_PED_DIES_WHEN_INJURED(Ped ped, BOOL toggle) {
			Invoker::Invoke<void, 784>(ped, toggle);
		}

		void SET_PED_ENABLE_WEAPON_BLOCKING(Ped ped, BOOL toggle) {
			Invoker::Invoke<void, 785>(ped, toggle);
		}

		void SPECIAL_FUNCTION_DO_NOT_USE(Ped ped, BOOL p1) {
			Invoker::Invoke<void, 786>(ped, p1);
		}

		void RESET_PED_VISIBLE_DAMAGE(Ped ped) {
			Invoker::Invoke<void, 787>(ped);
		}

		void APPLY_PED_BLOOD_DAMAGE_BY_ZONE(Ped ped, Any p1, float p2, float p3, Any p4) {
			Invoker::Invoke<void, 788>(ped, p1, p2, p3, p4);
		}

		void APPLY_PED_BLOOD(Ped ped, int boneIndex, float xRot, float yRot, float zRot, const char* woundType) {
			Invoker::Invoke<void, 789>(ped, boneIndex, xRot, yRot, zRot, woundType);
		}

		void APPLY_PED_BLOOD_BY_ZONE(Ped ped, int p1, float p2, float p3, const char* p4) {
			Invoker::Invoke<void, 790>(ped, p1, p2, p3, p4);
		}

		void APPLY_PED_BLOOD_SPECIFIC(Ped ped, int p1, float p2, float p3, float p4, float p5, int p6, float p7, const char* p8) {
			Invoker::Invoke<void, 791>(ped, p1, p2, p3, p4, p5, p6, p7, p8);
		}

		void APPLY_PED_DAMAGE_DECAL(Ped ped, int damageZone, float xOffset, float yOffset, float heading, float scale, float alpha, int variation, BOOL fadeIn, const char* decalName) {
			Invoker::Invoke<void, 792>(ped, damageZone, xOffset, yOffset, heading, scale, alpha, variation, fadeIn, decalName);
		}

		void APPLY_PED_DAMAGE_PACK(Ped ped, const char* damagePack, float damage, float mult) {
			Invoker::Invoke<void, 793>(ped, damagePack, damage, mult);
		}

		void CLEAR_PED_BLOOD_DAMAGE(Ped ped) {
			Invoker::Invoke<void, 794>(ped);
		}

		void CLEAR_PED_BLOOD_DAMAGE_BY_ZONE(Ped ped, int p1) {
			Invoker::Invoke<void, 795>(ped, p1);
		}

		void HIDE_PED_BLOOD_DAMAGE_BY_ZONE(Ped ped, Any p1, BOOL p2) {
			Invoker::Invoke<void, 796>(ped, p1, p2);
		}

		void CLEAR_PED_DAMAGE_DECAL_BY_ZONE(Ped ped, int p1, const char* p2) {
			Invoker::Invoke<void, 797>(ped, p1, p2);
		}

		int GET_PED_DECORATIONS_STATE(Ped ped) {
			return Invoker::Invoke<int, 798>(ped);
		}

		void MARK_PED_DECORATIONS_AS_CLONED_FROM_LOCAL_PLAYER(Ped ped, BOOL p1) {
			Invoker::Invoke<void, 799>(ped, p1);
		}

		void CLEAR_PED_WETNESS(Ped ped) {
			Invoker::Invoke<void, 800>(ped);
		}

		void SET_PED_WETNESS_HEIGHT(Ped ped, float height) {
			Invoker::Invoke<void, 801>(ped, height);
		}

		void SET_PED_WETNESS_ENABLED_THIS_FRAME(Ped ped) {
			Invoker::Invoke<void, 802>(ped);
		}

		void SET_PED_WETNESS(Ped ped, float wetLevel) {
			Invoker::Invoke<void, 803>(ped, wetLevel);
		}

		void CLEAR_PED_ENV_DIRT(Ped ped) {
			Invoker::Invoke<void, 804>(ped);
		}

		void SET_PED_SWEAT(Ped ped, float sweat) {
			Invoker::Invoke<void, 805>(ped, sweat);
		}

		void ADD_PED_DECORATION_FROM_HASHES(Ped ped, Hash collection, Hash overlay) {
			Invoker::Invoke<void, 806>(ped, collection, overlay);
		}

		void ADD_PED_DECORATION_FROM_HASHES_IN_CORONA(Ped ped, Hash collection, Hash overlay) {
			Invoker::Invoke<void, 807>(ped, collection, overlay);
		}

		int GET_PED_DECORATION_ZONE_FROM_HASHES(Hash collection, Hash overlay) {
			return Invoker::Invoke<int, 808>(collection, overlay);
		}

		void CLEAR_PED_DECORATIONS(Ped ped) {
			Invoker::Invoke<void, 809>(ped);
		}

		void CLEAR_PED_DECORATIONS_LEAVE_SCARS(Ped ped) {
			Invoker::Invoke<void, 810>(ped);
		}

		BOOL WAS_PED_SKELETON_UPDATED(Ped ped) {
			return Invoker::Invoke<BOOL, 811>(ped);
		}

		Vector3 GET_PED_BONE_COORDS(Ped ped, int boneId, float offsetX, float offsetY, float offsetZ) {
			return Invoker::Invoke<Vector3, 812>(ped, boneId, offsetX, offsetY, offsetZ);
		}

		void CREATE_NM_MESSAGE(BOOL startImmediately, int messageId) {
			Invoker::Invoke<void, 813>(startImmediately, messageId);
		}

		void GIVE_PED_NM_MESSAGE(Ped ped) {
			Invoker::Invoke<void, 814>(ped);
		}

		int ADD_SCENARIO_BLOCKING_AREA(float x1, float y1, float z1, float x2, float y2, float z2, BOOL p6, BOOL p7, BOOL p8, BOOL p9, Any p10) {
			return Invoker::Invoke<int, 815>(x1, y1, z1, x2, y2, z2, p6, p7, p8, p9, p10);
		}

		void REMOVE_SCENARIO_BLOCKING_AREAS() {
			Invoker::Invoke<void, 816>();
		}

		void REMOVE_SCENARIO_BLOCKING_AREA(Any p0, BOOL p1) {
			Invoker::Invoke<void, 817>(p0, p1);
		}

		void SET_SCENARIO_PEDS_SPAWN_IN_SPHERE_AREA(float x, float y, float z, float range, int p4) {
			Invoker::Invoke<void, 818>(x, y, z, range, p4);
		}

		BOOL DOES_SCENARIO_BLOCKING_AREA_EXISTS(float x1, float y1, float z1, float x2, float y2, float z2) {
			return Invoker::Invoke<BOOL, 819>(x1, y1, z1, x2, y2, z2);
		}

		BOOL IS_PED_USING_SCENARIO(Ped ped, const char* scenario) {
			return Invoker::Invoke<BOOL, 820>(ped, scenario);
		}

		BOOL IS_PED_USING_ANY_SCENARIO(Ped ped) {
			return Invoker::Invoke<BOOL, 821>(ped);
		}

		BOOL SET_PED_PANIC_EXIT_SCENARIO(Any p0, Any p1, Any p2, Any p3) {
			return Invoker::Invoke<BOOL, 822>(p0, p1, p2, p3);
		}

		void TOGGLE_SCENARIO_PED_COWER_IN_PLACE(Ped ped, BOOL toggle) {
			Invoker::Invoke<void, 823>(ped, toggle);
		}

		BOOL TRIGGER_PED_SCENARIO_PANICEXITTOFLEE(Any p0, Any p1, Any p2, Any p3) {
			return Invoker::Invoke<BOOL, 824>(p0, p1, p2, p3);
		}

		BOOL SET_PED_SHOULD_PLAY_DIRECTED_NORMAL_SCENARIO_EXIT(Any p0, Any p1, Any p2, Any p3) {
			return Invoker::Invoke<BOOL, 825>(p0, p1, p2, p3);
		}

		void SET_PED_SHOULD_PLAY_NORMAL_SCENARIO_EXIT(Ped ped) {
			Invoker::Invoke<void, 826>(ped);
		}

		void SET_PED_SHOULD_PLAY_IMMEDIATE_SCENARIO_EXIT(Ped ped) {
			Invoker::Invoke<void, 827>(ped);
		}

		BOOL SET_PED_SHOULD_PLAY_FLEE_SCENARIO_EXIT(Ped ped, Any p1, Any p2, Any p3) {
			return Invoker::Invoke<BOOL, 828>(ped, p1, p2, p3);
		}

		void SET_PED_SHOULD_IGNORE_SCENARIO_EXIT_COLLISION_CHECKS(Ped ped, BOOL p1) {
			Invoker::Invoke<void, 829>(ped, p1);
		}

		void SET_PED_SHOULD_IGNORE_SCENARIO_NAV_CHECKS(Any p0, BOOL p1) {
			Invoker::Invoke<void, 830>(p0, p1);
		}

		void SET_PED_SHOULD_PROBE_FOR_SCENARIO_EXITS_IN_ONE_FRAME(Any p0, BOOL p1) {
			Invoker::Invoke<void, 831>(p0, p1);
		}

		BOOL IS_PED_GESTURING(Any p0) {
			return Invoker::Invoke<BOOL, 832>(p0);
		}

		void RESET_FACIAL_IDLE_ANIM(Ped ped) {
			Invoker::Invoke<void, 833>(ped);
		}

		void PLAY_FACIAL_ANIM(Ped ped, const char* animName, const char* animDict) {
			Invoker::Invoke<void, 834>(ped, animName, animDict);
		}

		void SET_FACIAL_CLIPSET(Ped ped, const char* animDict) {
			Invoker::Invoke<void, 835>(ped, animDict);
		}

		void SET_FACIAL_IDLE_ANIM_OVERRIDE(Ped ped, const char* animName, const char* animDict) {
			Invoker::Invoke<void, 836>(ped, animName, animDict);
		}

		void CLEAR_FACIAL_IDLE_ANIM_OVERRIDE(Ped ped) {
			Invoker::Invoke<void, 837>(ped);
		}

		void SET_PED_CAN_PLAY_GESTURE_ANIMS(Ped ped, BOOL toggle) {
			Invoker::Invoke<void, 838>(ped, toggle);
		}

		void SET_PED_CAN_PLAY_VISEME_ANIMS(Ped ped, BOOL toggle, BOOL p2) {
			Invoker::Invoke<void, 839>(ped, toggle, p2);
		}

		void SET_PED_IS_IGNORED_BY_AUTO_OPEN_DOORS(Ped ped, BOOL p1) {
			Invoker::Invoke<void, 840>(ped, p1);
		}

		void SET_PED_CAN_PLAY_AMBIENT_ANIMS(Ped ped, BOOL toggle) {
			Invoker::Invoke<void, 841>(ped, toggle);
		}

		void SET_PED_CAN_PLAY_AMBIENT_BASE_ANIMS(Ped ped, BOOL toggle) {
			Invoker::Invoke<void, 842>(ped, toggle);
		}

		void TRIGGER_IDLE_ANIMATION_ON_PED(Ped ped) {
			Invoker::Invoke<void, 843>(ped);
		}

		void SET_PED_CAN_ARM_IK(Ped ped, BOOL toggle) {
			Invoker::Invoke<void, 844>(ped, toggle);
		}

		void SET_PED_CAN_BODY_RECOIL_IK(Ped ped, BOOL toggle) {
			Invoker::Invoke<void, 6619>(ped, toggle);
		}

		void SET_PED_CAN_HEAD_IK(Ped ped, BOOL toggle) {
			Invoker::Invoke<void, 845>(ped, toggle);
		}

		void SET_PED_CAN_LEG_IK(Ped ped, BOOL toggle) {
			Invoker::Invoke<void, 846>(ped, toggle);
		}

		void SET_PED_CAN_TORSO_IK(Ped ped, BOOL toggle) {
			Invoker::Invoke<void, 847>(ped, toggle);
		}

		void SET_PED_CAN_TORSO_REACT_IK(Ped ped, BOOL p1) {
			Invoker::Invoke<void, 848>(ped, p1);
		}

		void SET_PED_CAN_TORSO_VEHICLE_IK(Ped ped, BOOL p1) {
			Invoker::Invoke<void, 849>(ped, p1);
		}

		void SET_PED_CAN_USE_AUTO_CONVERSATION_LOOKAT(Ped ped, BOOL toggle) {
			Invoker::Invoke<void, 850>(ped, toggle);
		}

		BOOL IS_PED_HEADTRACKING_PED(Ped ped1, Ped ped2) {
			return Invoker::Invoke<BOOL, 851>(ped1, ped2);
		}

		BOOL IS_PED_HEADTRACKING_ENTITY(Ped ped, Entity entity) {
			return Invoker::Invoke<BOOL, 852>(ped, entity);
		}

		void SET_PED_PRIMARY_LOOKAT(Ped ped, Ped lookAt) {
			Invoker::Invoke<void, 853>(ped, lookAt);
		}

		void SET_PED_CLOTH_PIN_FRAMES(Any p0, Any p1) {
			Invoker::Invoke<void, 854>(p0, p1);
		}

		void SET_PED_CLOTH_PACKAGE_INDEX(Any p0, Any p1) {
			Invoker::Invoke<void, 855>(p0, p1);
		}

		void SET_PED_CLOTH_PRONE(Any p0, BOOL p1) {
			Invoker::Invoke<void, 856>(p0, p1);
		}

		void SET_PED_CONFIG_FLAG(Ped ped, int flagId, BOOL value) {
			Invoker::Invoke<void, 857>(ped, flagId, value);
		}

		void SET_PED_RESET_FLAG(Ped ped, int flagId, BOOL doReset) {
			Invoker::Invoke<void, 858>(ped, flagId, doReset);
		}

		BOOL GET_PED_CONFIG_FLAG(Ped ped, int flagId, BOOL p2) {
			return Invoker::Invoke<BOOL, 859>(ped, flagId, p2);
		}

		BOOL GET_PED_RESET_FLAG(Ped ped, int flagId) {
			return Invoker::Invoke<BOOL, 860>(ped, flagId);
		}

		void SET_PED_GROUP_MEMBER_PASSENGER_INDEX(Ped ped, int index) {
			Invoker::Invoke<void, 861>(ped, index);
		}

		void SET_PED_CAN_EVASIVE_DIVE(Ped ped, BOOL toggle) {
			Invoker::Invoke<void, 862>(ped, toggle);
		}

		BOOL IS_PED_EVASIVE_DIVING(Ped ped, Entity* evadingEntity) {
			return Invoker::Invoke<BOOL, 863>(ped, evadingEntity);
		}

		void SET_PED_SHOOTS_AT_COORD(Ped ped, float x, float y, float z, BOOL toggle) {
			Invoker::Invoke<void, 864>(ped, x, y, z, toggle);
		}

		void SET_PED_MODEL_IS_SUPPRESSED(Hash modelHash, BOOL toggle) {
			Invoker::Invoke<void, 865>(modelHash, toggle);
		}

		void STOP_ANY_PED_MODEL_BEING_SUPPRESSED() {
			Invoker::Invoke<void, 866>();
		}

		void SET_PED_CAN_BE_TARGETED_WHEN_INJURED(Ped ped, BOOL toggle) {
			Invoker::Invoke<void, 867>(ped, toggle);
		}

		void SET_PED_GENERATES_DEAD_BODY_EVENTS(Ped ped, BOOL toggle) {
			Invoker::Invoke<void, 868>(ped, toggle);
		}

		void BLOCK_PED_FROM_GENERATING_DEAD_BODY_EVENTS_WHEN_DEAD(Ped ped, BOOL toggle) {
			Invoker::Invoke<void, 869>(ped, toggle);
		}

		void SET_PED_WILL_ONLY_ATTACK_WANTED_PLAYER(Any p0, Any p1) {
			Invoker::Invoke<void, 870>(p0, p1);
		}

		void SET_PED_CAN_RAGDOLL_FROM_PLAYER_IMPACT(Ped ped, BOOL toggle) {
			Invoker::Invoke<void, 871>(ped, toggle);
		}

		void GIVE_PED_HELMET(Ped ped, BOOL cannotRemove, int helmetFlag, int textureIndex) {
			Invoker::Invoke<void, 872>(ped, cannotRemove, helmetFlag, textureIndex);
		}

		void REMOVE_PED_HELMET(Ped ped, BOOL instantly) {
			Invoker::Invoke<void, 873>(ped, instantly);
		}

		BOOL IS_PED_TAKING_OFF_HELMET(Ped ped) {
			return Invoker::Invoke<BOOL, 874>(ped);
		}

		void SET_PED_HELMET(Ped ped, BOOL canWearHelmet) {
			Invoker::Invoke<void, 875>(ped, canWearHelmet);
		}

		void SET_PED_HELMET_FLAG(Ped ped, int helmetFlag) {
			Invoker::Invoke<void, 876>(ped, helmetFlag);
		}

		void SET_PED_HELMET_PROP_INDEX(Ped ped, int propIndex, BOOL p2) {
			Invoker::Invoke<void, 877>(ped, propIndex, p2);
		}

		void SET_PED_HELMET_VISOR_PROP_INDICES(Ped ped, BOOL p1, int p2, int p3) {
			Invoker::Invoke<void, 878>(ped, p1, p2, p3);
		}

		BOOL IS_PED_HELMET_VISOR_UP(Ped ped) {
			return Invoker::Invoke<BOOL, 879>(ped);
		}

		void SET_PED_HELMET_TEXTURE_INDEX(Ped ped, int textureIndex) {
			Invoker::Invoke<void, 880>(ped, textureIndex);
		}

		BOOL IS_PED_WEARING_HELMET(Ped ped) {
			return Invoker::Invoke<BOOL, 881>(ped);
		}

		void CLEAR_PED_STORED_HAT_PROP(Ped ped) {
			Invoker::Invoke<void, 882>(ped);
		}

		int GET_PED_HELMET_STORED_HAT_PROP_INDEX(Ped ped) {
			return Invoker::Invoke<int, 883>(ped);
		}

		int GET_PED_HELMET_STORED_HAT_TEX_INDEX(Ped ped) {
			return Invoker::Invoke<int, 884>(ped);
		}

		BOOL IS_CURRENT_HEAD_PROP_A_HELMET(Any p0) {
			return Invoker::Invoke<BOOL, 885>(p0);
		}

		void SET_PED_TO_LOAD_COVER(Ped ped, BOOL toggle) {
			Invoker::Invoke<void, 886>(ped, toggle);
		}

		void SET_PED_CAN_COWER_IN_COVER(Ped ped, BOOL toggle) {
			Invoker::Invoke<void, 887>(ped, toggle);
		}

		void SET_PED_CAN_PEEK_IN_COVER(Ped ped, BOOL toggle) {
			Invoker::Invoke<void, 888>(ped, toggle);
		}

		void SET_PED_PLAYS_HEAD_ON_HORN_ANIM_WHEN_DIES_IN_VEHICLE(Ped ped, BOOL toggle) {
			Invoker::Invoke<void, 889>(ped, toggle);
		}

		void SET_PED_LEG_IK_MODE(Ped ped, int mode) {
			Invoker::Invoke<void, 890>(ped, mode);
		}

		void SET_PED_MOTION_BLUR(Ped ped, BOOL toggle) {
			Invoker::Invoke<void, 891>(ped, toggle);
		}

		void SET_PED_CAN_SWITCH_WEAPON(Ped ped, BOOL toggle) {
			Invoker::Invoke<void, 892>(ped, toggle);
		}

		void SET_PED_DIES_INSTANTLY_IN_WATER(Ped ped, BOOL toggle) {
			Invoker::Invoke<void, 893>(ped, toggle);
		}

		void SET_LADDER_CLIMB_INPUT_STATE(Ped ped, int p1) {
			Invoker::Invoke<void, 894>(ped, p1);
		}

		void STOP_PED_WEAPON_FIRING_WHEN_DROPPED(Ped ped) {
			Invoker::Invoke<void, 895>(ped);
		}

		void SET_SCRIPTED_ANIM_SEAT_OFFSET(Ped ped, float p1) {
			Invoker::Invoke<void, 896>(ped, p1);
		}

		void SET_PED_COMBAT_MOVEMENT(Ped ped, int combatMovement) {
			Invoker::Invoke<void, 897>(ped, combatMovement);
		}

		int GET_PED_COMBAT_MOVEMENT(Ped ped) {
			return Invoker::Invoke<int, 898>(ped);
		}

		void SET_PED_COMBAT_ABILITY(Ped ped, int abilityLevel) {
			Invoker::Invoke<void, 899>(ped, abilityLevel);
		}

		void SET_PED_COMBAT_RANGE(Ped ped, int combatRange) {
			Invoker::Invoke<void, 900>(ped, combatRange);
		}

		int GET_PED_COMBAT_RANGE(Ped ped) {
			return Invoker::Invoke<int, 901>(ped);
		}

		void SET_PED_COMBAT_ATTRIBUTES(Ped ped, int attributeId, BOOL enabled) {
			Invoker::Invoke<void, 902>(ped, attributeId, enabled);
		}

		void SET_PED_TARGET_LOSS_RESPONSE(Ped ped, int responseType) {
			Invoker::Invoke<void, 903>(ped, responseType);
		}

		BOOL IS_PED_PERFORMING_MELEE_ACTION(Ped ped) {
			return Invoker::Invoke<BOOL, 904>(ped);
		}

		BOOL IS_PED_PERFORMING_STEALTH_KILL(Ped ped) {
			return Invoker::Invoke<BOOL, 905>(ped);
		}

		BOOL IS_PED_PERFORMING_A_COUNTER_ATTACK(Ped ped) {
			return Invoker::Invoke<BOOL, 906>(ped);
		}

		BOOL IS_PED_BEING_STEALTH_KILLED(Ped ped) {
			return Invoker::Invoke<BOOL, 907>(ped);
		}

		Ped GET_MELEE_TARGET_FOR_PED(Ped ped) {
			return Invoker::Invoke<Ped, 908>(ped);
		}

		BOOL WAS_PED_KILLED_BY_STEALTH(Ped ped) {
			return Invoker::Invoke<BOOL, 909>(ped);
		}

		BOOL WAS_PED_KILLED_BY_TAKEDOWN(Ped ped) {
			return Invoker::Invoke<BOOL, 910>(ped);
		}

		BOOL WAS_PED_KNOCKED_OUT(Ped ped) {
			return Invoker::Invoke<BOOL, 911>(ped);
		}

		void SET_PED_FLEE_ATTRIBUTES(Ped ped, int attributeFlags, BOOL enable) {
			Invoker::Invoke<void, 912>(ped, attributeFlags, enable);
		}

		void SET_PED_COWER_HASH(Ped ped, const char* p1) {
			Invoker::Invoke<void, 913>(ped, p1);
		}

		void SET_PED_STEERS_AROUND_DEAD_BODIES(Ped ped, BOOL toggle) {
			Invoker::Invoke<void, 914>(ped, toggle);
		}

		void SET_PED_STEERS_AROUND_PEDS(Ped ped, BOOL toggle) {
			Invoker::Invoke<void, 915>(ped, toggle);
		}

		void SET_PED_STEERS_AROUND_OBJECTS(Ped ped, BOOL toggle) {
			Invoker::Invoke<void, 916>(ped, toggle);
		}

		void SET_PED_STEERS_AROUND_VEHICLES(Ped ped, BOOL toggle) {
			Invoker::Invoke<void, 917>(ped, toggle);
		}

		void SET_PED_IS_AVOIDED_BY_OTHERS(Any p0, BOOL p1) {
			Invoker::Invoke<void, 918>(p0, p1);
		}

		void SET_PED_INCREASED_AVOIDANCE_RADIUS(Ped ped) {
			Invoker::Invoke<void, 919>(ped);
		}

		void SET_PED_BLOCKS_PATHING_WHEN_DEAD(Ped ped, BOOL toggle) {
			Invoker::Invoke<void, 920>(ped, toggle);
		}

		void SET_PED_NO_TIME_DELAY_BEFORE_SHOT(Any p0) {
			Invoker::Invoke<void, 921>(p0);
		}

		BOOL IS_ANY_PED_NEAR_POINT(float x, float y, float z, float radius) {
			return Invoker::Invoke<BOOL, 922>(x, y, z, radius);
		}

		void FORCE_PED_AI_AND_ANIMATION_UPDATE(Ped ped, BOOL p1, BOOL p2) {
			Invoker::Invoke<void, 923>(ped, p1, p2);
		}

		BOOL IS_PED_HEADING_TOWARDS_POSITION(Ped ped, float x, float y, float z, float p4) {
			return Invoker::Invoke<BOOL, 924>(ped, x, y, z, p4);
		}

		void REQUEST_PED_VISIBILITY_TRACKING(Ped ped) {
			Invoker::Invoke<void, 925>(ped);
		}

		void REQUEST_PED_VEHICLE_VISIBILITY_TRACKING(Ped ped, BOOL p1) {
			Invoker::Invoke<void, 926>(ped, p1);
		}

		void REQUEST_PED_RESTRICTED_VEHICLE_VISIBILITY_TRACKING(Ped ped, BOOL p1) {
			Invoker::Invoke<void, 927>(ped, p1);
		}

		void REQUEST_PED_USE_SMALL_BBOX_VISIBILITY_TRACKING(Ped ped, BOOL p1) {
			Invoker::Invoke<void, 928>(ped, p1);
		}

		BOOL IS_TRACKED_PED_VISIBLE(Ped ped) {
			return Invoker::Invoke<BOOL, 929>(ped);
		}

		int GET_TRACKED_PED_PIXELCOUNT(Ped ped) {
			return Invoker::Invoke<int, 930>(ped);
		}

		BOOL IS_PED_TRACKED(Ped ped) {
			return Invoker::Invoke<BOOL, 931>(ped);
		}

		BOOL HAS_PED_RECEIVED_EVENT(Ped ped, int eventId) {
			return Invoker::Invoke<BOOL, 932>(ped, eventId);
		}

		BOOL CAN_PED_SEE_HATED_PED(Ped ped1, Ped ped2) {
			return Invoker::Invoke<BOOL, 933>(ped1, ped2);
		}

		BOOL CAN_PED_SHUFFLE_TO_OR_FROM_TURRET_SEAT(Ped ped, int* p1) {
			return Invoker::Invoke<BOOL, 934>(ped, p1);
		}

		BOOL CAN_PED_SHUFFLE_TO_OR_FROM_EXTRA_SEAT(Ped ped, int* p1) {
			return Invoker::Invoke<BOOL, 935>(ped, p1);
		}

		int GET_PED_BONE_INDEX(Ped ped, int boneId) {
			return Invoker::Invoke<int, 936>(ped, boneId);
		}

		int GET_PED_RAGDOLL_BONE_INDEX(Ped ped, int bone) {
			return Invoker::Invoke<int, 937>(ped, bone);
		}

		void SET_PED_ENVEFF_SCALE(Ped ped, float value) {
			Invoker::Invoke<void, 938>(ped, value);
		}

		float GET_PED_ENVEFF_SCALE(Ped ped) {
			return Invoker::Invoke<float, 939>(ped);
		}

		void SET_ENABLE_PED_ENVEFF_SCALE(Ped ped, BOOL toggle) {
			Invoker::Invoke<void, 940>(ped, toggle);
		}

		void SET_PED_ENVEFF_CPV_ADD(Ped ped, float p1) {
			Invoker::Invoke<void, 941>(ped, p1);
		}

		void SET_PED_ENVEFF_COLOR_MODULATOR(Ped ped, int p1, int p2, int p3) {
			Invoker::Invoke<void, 942>(ped, p1, p2, p3);
		}

		void SET_PED_EMISSIVE_SCALE(Ped ped, float intensity) {
			Invoker::Invoke<void, 943>(ped, intensity);
		}

		float GET_PED_EMISSIVE_SCALE(Ped ped) {
			return Invoker::Invoke<float, 944>(ped);
		}

		BOOL IS_PED_SHADER_READY(Ped ped) {
			return Invoker::Invoke<BOOL, 945>(ped);
		}

		void SET_PED_ENABLE_CREW_EMBLEM(Ped ped, BOOL toggle) {
			Invoker::Invoke<void, 946>(ped, toggle);
		}

		void REQUEST_RAGDOLL_BOUNDS_UPDATE(Any p0, Any p1) {
			Invoker::Invoke<void, 947>(p0, p1);
		}

		void SET_PED_AO_BLOB_RENDERING(Ped ped, BOOL toggle) {
			Invoker::Invoke<void, 948>(ped, toggle);
		}

		BOOL IS_PED_SHELTERED(Ped ped) {
			return Invoker::Invoke<BOOL, 949>(ped);
		}

		int CREATE_SYNCHRONIZED_SCENE(float x, float y, float z, float roll, float pitch, float yaw, int p6) {
			return Invoker::Invoke<int, 950>(x, y, z, roll, pitch, yaw, p6);
		}

		int CREATE_SYNCHRONIZED_SCENE_AT_MAP_OBJECT(float x, float y, float z, float radius, Hash object) {
			return Invoker::Invoke<int, 951>(x, y, z, radius, object);
		}

		BOOL IS_SYNCHRONIZED_SCENE_RUNNING(int sceneId) {
			return Invoker::Invoke<BOOL, 952>(sceneId);
		}

		void SET_SYNCHRONIZED_SCENE_ORIGIN(int sceneID, float x, float y, float z, float roll, float pitch, float yaw, BOOL p7) {
			Invoker::Invoke<void, 953>(sceneID, x, y, z, roll, pitch, yaw, p7);
		}

		void SET_SYNCHRONIZED_SCENE_PHASE(int sceneID, float phase) {
			Invoker::Invoke<void, 954>(sceneID, phase);
		}

		float GET_SYNCHRONIZED_SCENE_PHASE(int sceneID) {
			return Invoker::Invoke<float, 955>(sceneID);
		}

		void SET_SYNCHRONIZED_SCENE_RATE(int sceneID, float rate) {
			Invoker::Invoke<void, 956>(sceneID, rate);
		}

		float GET_SYNCHRONIZED_SCENE_RATE(int sceneID) {
			return Invoker::Invoke<float, 957>(sceneID);
		}

		void SET_SYNCHRONIZED_SCENE_LOOPED(int sceneID, BOOL toggle) {
			Invoker::Invoke<void, 958>(sceneID, toggle);
		}

		BOOL IS_SYNCHRONIZED_SCENE_LOOPED(int sceneID) {
			return Invoker::Invoke<BOOL, 959>(sceneID);
		}

		void SET_SYNCHRONIZED_SCENE_HOLD_LAST_FRAME(int sceneID, BOOL toggle) {
			Invoker::Invoke<void, 960>(sceneID, toggle);
		}

		BOOL IS_SYNCHRONIZED_SCENE_HOLD_LAST_FRAME(int sceneID) {
			return Invoker::Invoke<BOOL, 961>(sceneID);
		}

		void ATTACH_SYNCHRONIZED_SCENE_TO_ENTITY(int sceneID, Entity entity, int boneIndex) {
			Invoker::Invoke<void, 962>(sceneID, entity, boneIndex);
		}

		void DETACH_SYNCHRONIZED_SCENE(int sceneID) {
			Invoker::Invoke<void, 963>(sceneID);
		}

		void TAKE_OWNERSHIP_OF_SYNCHRONIZED_SCENE(int scene) {
			Invoker::Invoke<void, 964>(scene);
		}

		BOOL FORCE_PED_MOTION_STATE(Ped ped, Hash motionStateHash, BOOL p2, int p3, BOOL p4) {
			return Invoker::Invoke<BOOL, 965>(ped, motionStateHash, p2, p3, p4);
		}

		BOOL GET_PED_CURRENT_MOVE_BLEND_RATIO(Ped ped, float* speedX, float* speedY) {
			return Invoker::Invoke<BOOL, 966>(ped, speedX, speedY);
		}

		void SET_PED_MAX_MOVE_BLEND_RATIO(Ped ped, float value) {
			Invoker::Invoke<void, 967>(ped, value);
		}

		void SET_PED_MIN_MOVE_BLEND_RATIO(Ped ped, float value) {
			Invoker::Invoke<void, 968>(ped, value);
		}

		void SET_PED_MOVE_RATE_OVERRIDE(Ped ped, float value) {
			Invoker::Invoke<void, 969>(ped, value);
		}

		void SET_PED_MOVE_RATE_IN_WATER_OVERRIDE(Ped ped, float p1) {
			Invoker::Invoke<void, 970>(ped, p1);
		}

		BOOL PED_HAS_SEXINESS_FLAG_SET(Ped ped, int sexinessFlag) {
			return Invoker::Invoke<BOOL, 971>(ped, sexinessFlag);
		}

		int GET_PED_NEARBY_VEHICLES(Ped ped, Any* sizeAndVehs) {
			return Invoker::Invoke<int, 972>(ped, sizeAndVehs);
		}

		int GET_PED_NEARBY_PEDS(Ped ped, Any* sizeAndPeds, int ignore) {
			return Invoker::Invoke<int, 973>(ped, sizeAndPeds, ignore);
		}

		BOOL HAVE_ALL_STREAMING_REQUESTS_COMPLETED(Ped ped) {
			return Invoker::Invoke<BOOL, 974>(ped);
		}

		BOOL IS_PED_USING_ACTION_MODE(Ped ped) {
			return Invoker::Invoke<BOOL, 975>(ped);
		}

		void SET_PED_USING_ACTION_MODE(Ped ped, BOOL p1, int p2, const char* action) {
			Invoker::Invoke<void, 976>(ped, p1, p2, action);
		}

		void SET_MOVEMENT_MODE_OVERRIDE(Ped ped, const char* name) {
			Invoker::Invoke<void, 977>(ped, name);
		}

		void SET_PED_CAPSULE(Ped ped, float value) {
			Invoker::Invoke<void, 978>(ped, value);
		}

		int REGISTER_PEDHEADSHOT(Ped ped) {
			return Invoker::Invoke<int, 979>(ped);
		}

		int REGISTER_PEDHEADSHOT_HIRES(Ped ped) {
			return Invoker::Invoke<int, 980>(ped);
		}

		int REGISTER_PEDHEADSHOT_TRANSPARENT(Ped ped) {
			return Invoker::Invoke<int, 981>(ped);
		}

		void UNREGISTER_PEDHEADSHOT(int id) {
			Invoker::Invoke<void, 982>(id);
		}

		BOOL IS_PEDHEADSHOT_VALID(int id) {
			return Invoker::Invoke<BOOL, 983>(id);
		}

		BOOL IS_PEDHEADSHOT_READY(int id) {
			return Invoker::Invoke<BOOL, 984>(id);
		}

		const char* GET_PEDHEADSHOT_TXD_STRING(int id) {
			return Invoker::Invoke<const char*, 985>(id);
		}

		BOOL REQUEST_PEDHEADSHOT_IMG_UPLOAD(int id) {
			return Invoker::Invoke<BOOL, 986>(id);
		}

		void RELEASE_PEDHEADSHOT_IMG_UPLOAD(int id) {
			Invoker::Invoke<void, 987>(id);
		}

		BOOL IS_PEDHEADSHOT_IMG_UPLOAD_AVAILABLE() {
			return Invoker::Invoke<BOOL, 988>();
		}

		BOOL HAS_PEDHEADSHOT_IMG_UPLOAD_FAILED() {
			return Invoker::Invoke<BOOL, 989>();
		}

		BOOL HAS_PEDHEADSHOT_IMG_UPLOAD_SUCCEEDED() {
			return Invoker::Invoke<BOOL, 990>();
		}

		void SET_PED_HEATSCALE_OVERRIDE(Ped ped, float heatScale) {
			Invoker::Invoke<void, 991>(ped, heatScale);
		}

		void DISABLE_PED_HEATSCALE_OVERRIDE(Ped ped) {
			Invoker::Invoke<void, 992>(ped);
		}

		void SPAWNPOINTS_START_SEARCH(float p0, float p1, float p2, float p3, float p4, int interiorFlags, float scale, int duration) {
			Invoker::Invoke<void, 993>(p0, p1, p2, p3, p4, interiorFlags, scale, duration);
		}

		void SPAWNPOINTS_START_SEARCH_IN_ANGLED_AREA(float x1, float y1, float z1, float x2, float y2, float z2, float width, int interiorFlags, float scale, int duration) {
			Invoker::Invoke<void, 994>(x1, y1, z1, x2, y2, z2, width, interiorFlags, scale, duration);
		}

		void SPAWNPOINTS_CANCEL_SEARCH() {
			Invoker::Invoke<void, 995>();
		}

		BOOL SPAWNPOINTS_IS_SEARCH_ACTIVE() {
			return Invoker::Invoke<BOOL, 996>();
		}

		BOOL SPAWNPOINTS_IS_SEARCH_COMPLETE() {
			return Invoker::Invoke<BOOL, 997>();
		}

		BOOL SPAWNPOINTS_IS_SEARCH_FAILED() {
			return Invoker::Invoke<BOOL, 998>();
		}

		int SPAWNPOINTS_GET_NUM_SEARCH_RESULTS() {
			return Invoker::Invoke<int, 999>();
		}

		void SPAWNPOINTS_GET_SEARCH_RESULT(int randomInt, float* x, float* y, float* z) {
			Invoker::Invoke<void, 1000>(randomInt, x, y, z);
		}

		void SPAWNPOINTS_GET_SEARCH_RESULT_FLAGS(int p0, int* p1) {
			Invoker::Invoke<void, 1001>(p0, p1);
		}

		void SET_IK_TARGET(Ped ped, int ikIndex, Entity entityLookAt, int boneLookAt, float offsetX, float offsetY, float offsetZ, Any p7, int blendInDuration, int blendOutDuration) {
			Invoker::Invoke<void, 1002>(ped, ikIndex, entityLookAt, boneLookAt, offsetX, offsetY, offsetZ, p7, blendInDuration, blendOutDuration);
		}

		void FORCE_INSTANT_LEG_IK_SETUP(Ped ped) {
			Invoker::Invoke<void, 1003>(ped);
		}

		void REQUEST_ACTION_MODE_ASSET(const char* asset) {
			Invoker::Invoke<void, 1004>(asset);
		}

		BOOL HAS_ACTION_MODE_ASSET_LOADED(const char* asset) {
			return Invoker::Invoke<BOOL, 1005>(asset);
		}

		void REMOVE_ACTION_MODE_ASSET(const char* asset) {
			Invoker::Invoke<void, 1006>(asset);
		}

		void REQUEST_STEALTH_MODE_ASSET(const char* asset) {
			Invoker::Invoke<void, 1007>(asset);
		}

		BOOL HAS_STEALTH_MODE_ASSET_LOADED(const char* asset) {
			return Invoker::Invoke<BOOL, 1008>(asset);
		}

		void REMOVE_STEALTH_MODE_ASSET(const char* asset) {
			Invoker::Invoke<void, 1009>(asset);
		}

		void SET_PED_LOD_MULTIPLIER(Ped ped, float multiplier) {
			Invoker::Invoke<void, 1010>(ped, multiplier);
		}

		void SET_PED_CAN_LOSE_PROPS_ON_DAMAGE(Ped ped, BOOL toggle, int p2) {
			Invoker::Invoke<void, 1011>(ped, toggle, p2);
		}

		void SET_FORCE_FOOTSTEP_UPDATE(Ped ped, BOOL toggle) {
			Invoker::Invoke<void, 1012>(ped, toggle);
		}

		void SET_FORCE_STEP_TYPE(Ped ped, BOOL p1, int type, int p3) {
			Invoker::Invoke<void, 1013>(ped, p1, type, p3);
		}

		BOOL IS_ANY_HOSTILE_PED_NEAR_POINT(Ped ped, float x, float y, float z, float radius) {
			return Invoker::Invoke<BOOL, 1014>(ped, x, y, z, radius);
		}

		void SET_PED_CAN_PLAY_IN_CAR_IDLES(Ped ped, BOOL toggle) {
			Invoker::Invoke<void, 1015>(ped, toggle);
		}

		BOOL IS_TARGET_PED_IN_PERCEPTION_AREA(Ped ped, Ped targetPed, float p2, float p3, float p4, float p5) {
			return Invoker::Invoke<BOOL, 1016>(ped, targetPed, p2, p3, p4, p5);
		}

		void SET_POP_CONTROL_SPHERE_THIS_FRAME(float x, float y, float z, float min, float max) {
			Invoker::Invoke<void, 1017>(x, y, z, min, max);
		}

		void FORCE_ZERO_MASS_IN_COLLISIONS(Ped ped) {
			Invoker::Invoke<void, 1018>(ped);
		}

		void SET_DISABLE_HIGH_FALL_DEATH(Ped ped, BOOL toggle) {
			Invoker::Invoke<void, 1019>(ped, toggle);
		}

		void SET_PED_PHONE_PALETTE_IDX(Any p0, Any p1) {
			Invoker::Invoke<void, 1020>(p0, p1);
		}

		void SET_PED_STEER_BIAS(Ped ped, float value) {
			Invoker::Invoke<void, 1021>(ped, value);
		}

		BOOL IS_PED_SWITCHING_WEAPON(Ped Ped) {
			return Invoker::Invoke<BOOL, 1022>(Ped);
		}

		void SET_PED_TREATED_AS_FRIENDLY(Any p0, Any p1, Any p2) {
			Invoker::Invoke<void, 1023>(p0, p1, p2);
		}

		void SET_DISABLE_PED_MAP_COLLISION(Ped ped) {
			Invoker::Invoke<void, 1024>(ped);
		}

		void ENABLE_MP_LIGHT(Ped ped, BOOL toggle) {
			Invoker::Invoke<void, 1025>(ped, toggle);
		}

		BOOL GET_MP_LIGHT_ENABLED(Ped ped) {
			return Invoker::Invoke<BOOL, 1026>(ped);
		}

		void CLEAR_COVER_POINT_FOR_PED(Ped ped) {
			Invoker::Invoke<void, 1027>(ped);
		}

		void SET_ALLOW_STUNT_JUMP_CAMERA(Ped ped, BOOL toggle) {
			Invoker::Invoke<void, 1028>(ped, toggle);
		}

	} // namespace PED

	namespace PHYSICS {

		int ADD_ROPE(float x, float y, float z, float rotX, float rotY, float rotZ, float length, int ropeType, float maxLength, float minLength, float windingSpeed, BOOL p11, BOOL p12, BOOL rigid, float p14, BOOL breakWhenShot, Any* unkPtr) {
			return Invoker::Invoke<int, 1808>(x, y, z, rotX, rotY, rotZ, length, ropeType, maxLength, minLength, windingSpeed, p11, p12, rigid, p14, breakWhenShot, unkPtr);
		}

		void DELETE_ROPE(int* ropeId) {
			Invoker::Invoke<void, 1809>(ropeId);
		}

		void DELETE_CHILD_ROPE(int ropeId) {
			Invoker::Invoke<void, 1810>(ropeId);
		}

		BOOL DOES_ROPE_EXIST(int* ropeId) {
			return Invoker::Invoke<BOOL, 1811>(ropeId);
		}

		void ROPE_DRAW_ENABLED(int* ropeId, BOOL p1) {
			Invoker::Invoke<void, 1812>(ropeId, p1);
		}

		void ROPE_DRAW_SHADOW_ENABLED(int* ropeId, BOOL toggle) {
			Invoker::Invoke<void, 1813>(ropeId, toggle);
		}

		void LOAD_ROPE_DATA(int ropeId, const char* rope_preset) {
			Invoker::Invoke<void, 1814>(ropeId, rope_preset);
		}

		void PIN_ROPE_VERTEX(int ropeId, int vertex, float x, float y, float z) {
			Invoker::Invoke<void, 1815>(ropeId, vertex, x, y, z);
		}

		void UNPIN_ROPE_VERTEX(int ropeId, int vertex) {
			Invoker::Invoke<void, 1816>(ropeId, vertex);
		}

		int GET_ROPE_VERTEX_COUNT(int ropeId) {
			return Invoker::Invoke<int, 1817>(ropeId);
		}

		void ATTACH_ENTITIES_TO_ROPE(int ropeId, Entity ent1, Entity ent2, float ent1_x, float ent1_y, float ent1_z, float ent2_x, float ent2_y, float ent2_z, float length, BOOL p10, BOOL p11, Any* p12, Any* p13) {
			Invoker::Invoke<void, 1818>(ropeId, ent1, ent2, ent1_x, ent1_y, ent1_z, ent2_x, ent2_y, ent2_z, length, p10, p11, p12, p13);
		}

		void ATTACH_ROPE_TO_ENTITY(int ropeId, Entity entity, float x, float y, float z, BOOL p5) {
			Invoker::Invoke<void, 1819>(ropeId, entity, x, y, z, p5);
		}

		void DETACH_ROPE_FROM_ENTITY(int ropeId, Entity entity) {
			Invoker::Invoke<void, 1820>(ropeId, entity);
		}

		void ROPE_SET_UPDATE_PINVERTS(int ropeId) {
			Invoker::Invoke<void, 1821>(ropeId);
		}

		void ROPE_SET_UPDATE_ORDER(int ropeId, Any p1) {
			Invoker::Invoke<void, 1822>(ropeId, p1);
		}

		void ROPE_SET_SMOOTH_REELIN(int ropeId, BOOL p1) {
			Invoker::Invoke<void, 1823>(ropeId, p1);
		}

		BOOL IS_ROPE_ATTACHED_AT_BOTH_ENDS(int* ropeId) {
			return Invoker::Invoke<BOOL, 1824>(ropeId);
		}

		Vector3 GET_ROPE_LAST_VERTEX_COORD(int ropeId) {
			return Invoker::Invoke<Vector3, 1825>(ropeId);
		}

		Vector3 GET_ROPE_VERTEX_COORD(int ropeId, int vertex) {
			return Invoker::Invoke<Vector3, 1826>(ropeId, vertex);
		}

		void START_ROPE_WINDING(int ropeId) {
			Invoker::Invoke<void, 1827>(ropeId);
		}

		void STOP_ROPE_WINDING(int ropeId) {
			Invoker::Invoke<void, 1828>(ropeId);
		}

		void START_ROPE_UNWINDING_FRONT(int ropeId) {
			Invoker::Invoke<void, 1829>(ropeId);
		}

		void STOP_ROPE_UNWINDING_FRONT(int ropeId) {
			Invoker::Invoke<void, 1830>(ropeId);
		}

		void ROPE_CONVERT_TO_SIMPLE(int ropeId) {
			Invoker::Invoke<void, 1831>(ropeId);
		}

		void ROPE_LOAD_TEXTURES() {
			Invoker::Invoke<void, 1832>();
		}

		BOOL ROPE_ARE_TEXTURES_LOADED() {
			return Invoker::Invoke<BOOL, 1833>();
		}

		void ROPE_UNLOAD_TEXTURES() {
			Invoker::Invoke<void, 1834>();
		}

		BOOL DOES_SCRIPT_OWN_ROPE(int ropeId) {
			return Invoker::Invoke<BOOL, 1835>(ropeId);
		}

		void ROPE_ATTACH_VIRTUAL_BOUND_GEOM(int ropeId, int p1, float p2, float p3, float p4, float p5, float p6, float p7, float p8, float p9, float p10, float p11, float p12, float p13) {
			Invoker::Invoke<void, 1836>(ropeId, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13);
		}

		void ROPE_CHANGE_SCRIPT_OWNER(Any p0, BOOL p1, BOOL p2) {
			Invoker::Invoke<void, 1837>(p0, p1, p2);
		}

		void ROPE_SET_REFFRAMEVELOCITY_COLLIDERORDER(int ropeId, int p1) {
			Invoker::Invoke<void, 1838>(ropeId, p1);
		}

		float ROPE_GET_DISTANCE_BETWEEN_ENDS(int ropeId) {
			return Invoker::Invoke<float, 1839>(ropeId);
		}

		void ROPE_FORCE_LENGTH(int ropeId, float length) {
			Invoker::Invoke<void, 1840>(ropeId, length);
		}

		void ROPE_RESET_LENGTH(int ropeId, float length) {
			Invoker::Invoke<void, 1841>(ropeId, length);
		}

		void APPLY_IMPULSE_TO_CLOTH(float posX, float posY, float posZ, float vecX, float vecY, float vecZ, float impulse) {
			Invoker::Invoke<void, 1842>(posX, posY, posZ, vecX, vecY, vecZ, impulse);
		}

		void SET_DAMPING(Entity entity, int vertex, float value) {
			Invoker::Invoke<void, 1843>(entity, vertex, value);
		}

		Vector3 GET_DAMPING(Entity entity, int type) {
			return Invoker::Invoke<Vector3, 6618>(entity, type);
		}

		void ACTIVATE_PHYSICS(Entity entity) {
			Invoker::Invoke<void, 1844>(entity);
		}

		void SET_CGOFFSET(Entity entity, float x, float y, float z) {
			Invoker::Invoke<void, 1845>(entity, x, y, z);
		}

		Vector3 GET_CGOFFSET(Entity entity) {
			return Invoker::Invoke<Vector3, 1846>(entity);
		}

		void SET_CG_AT_BOUNDCENTER(Entity entity) {
			Invoker::Invoke<void, 1847>(entity);
		}

		void BREAK_ENTITY_GLASS(Entity entity, float p1, float p2, float p3, float p4, float p5, float p6, float p7, float p8, Any p9, BOOL p10) {
			Invoker::Invoke<void, 1848>(entity, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10);
		}

		BOOL GET_IS_ENTITY_A_FRAG(Object object) {
			return Invoker::Invoke<BOOL, 1849>(object);
		}

		void SET_DISABLE_BREAKING(Object object, BOOL toggle) {
			Invoker::Invoke<void, 1850>(object, toggle);
		}

		void RESET_DISABLE_BREAKING(Object object) {
			Invoker::Invoke<void, 1851>(object);
		}

		void SET_DISABLE_FRAG_DAMAGE(Object object, BOOL toggle) {
			Invoker::Invoke<void, 1852>(object, toggle);
		}

		void SET_USE_KINEMATIC_PHYSICS(Entity entity, BOOL toggle) {
			Invoker::Invoke<void, 1853>(entity, toggle);
		}

		void SET_IN_STUNT_MODE(BOOL p0) {
			Invoker::Invoke<void, 1854>(p0);
		}

		void SET_IN_ARENA_MODE(BOOL toggle) {
			Invoker::Invoke<void, 1855>(toggle);
		}

	} // namespace PHYSICS

	namespace PLAYER {

		Ped GET_PLAYER_PED(Player player) {
			return Invoker::Invoke<Ped, 1558>(player);
		}

		Ped GET_PLAYER_PED_SCRIPT_INDEX(Player player) {
			return Invoker::Invoke<Ped, 1559>(player);
		}

		void SET_PLAYER_MODEL(Player player, Hash model) {
			Invoker::Invoke<void, 1560>(player, model);
		}

		void CHANGE_PLAYER_PED(Player player, Ped ped, BOOL p2, BOOL resetDamage) {
			Invoker::Invoke<void, 1561>(player, ped, p2, resetDamage);
		}

		void GET_PLAYER_RGB_COLOUR(Player player, int* r, int* g, int* b) {
			Invoker::Invoke<void, 1562>(player, r, g, b);
		}

		int GET_NUMBER_OF_PLAYERS() {
			return Invoker::Invoke<int, 1563>();
		}

		int GET_PLAYER_TEAM(Player player) {
			return Invoker::Invoke<int, 1564>(player);
		}

		void SET_PLAYER_TEAM(Player player, int team) {
			Invoker::Invoke<void, 1565>(player, team);
		}

		int GET_NUMBER_OF_PLAYERS_IN_TEAM(int team) {
			return Invoker::Invoke<int, 1566>(team);
		}

		const char* GET_PLAYER_NAME(Player player) {
			return Invoker::Invoke<const char*, 1567>(player);
		}

		float GET_WANTED_LEVEL_RADIUS(Player player) {
			return Invoker::Invoke<float, 1568>(player);
		}

		Vector3 GET_PLAYER_WANTED_CENTRE_POSITION(Player player) {
			return Invoker::Invoke<Vector3, 1569>(player);
		}

		void SET_PLAYER_WANTED_CENTRE_POSITION(Player player, Vector3* position, BOOL p2, BOOL p3) {
			Invoker::Invoke<void, 1570>(player, position, p2, p3);
		}

		int GET_WANTED_LEVEL_THRESHOLD(int wantedLevel) {
			return Invoker::Invoke<int, 1571>(wantedLevel);
		}

		void SET_PLAYER_WANTED_LEVEL(Player player, int wantedLevel, BOOL disableNoMission) {
			Invoker::Invoke<void, 1572>(player, wantedLevel, disableNoMission);
		}

		void SET_PLAYER_WANTED_LEVEL_NO_DROP(Player player, int wantedLevel, BOOL p2) {
			Invoker::Invoke<void, 1573>(player, wantedLevel, p2);
		}

		void SET_PLAYER_WANTED_LEVEL_NOW(Player player, BOOL p1) {
			Invoker::Invoke<void, 1574>(player, p1);
		}

		BOOL ARE_PLAYER_FLASHING_STARS_ABOUT_TO_DROP(Player player) {
			return Invoker::Invoke<BOOL, 1575>(player);
		}

		BOOL ARE_PLAYER_STARS_GREYED_OUT(Player player) {
			return Invoker::Invoke<BOOL, 1576>(player);
		}

		BOOL IS_WANTED_AND_HAS_BEEN_SEEN_BY_COPS(Player player) {
			return Invoker::Invoke<BOOL, 1577>(player);
		}

		void SET_DISPATCH_COPS_FOR_PLAYER(Player player, BOOL toggle) {
			Invoker::Invoke<void, 1578>(player, toggle);
		}

		BOOL IS_PLAYER_WANTED_LEVEL_GREATER(Player player, int wantedLevel) {
			return Invoker::Invoke<BOOL, 1579>(player, wantedLevel);
		}

		void CLEAR_PLAYER_WANTED_LEVEL(Player player) {
			Invoker::Invoke<void, 1580>(player);
		}

		BOOL IS_PLAYER_DEAD(Player player) {
			return Invoker::Invoke<BOOL, 1581>(player);
		}

		BOOL IS_PLAYER_PRESSING_HORN(Player player) {
			return Invoker::Invoke<BOOL, 1582>(player);
		}

		void SET_PLAYER_CONTROL(Player player, BOOL bHasControl, int flags) {
			Invoker::Invoke<void, 1583>(player, bHasControl, flags);
		}

		int GET_PLAYER_WANTED_LEVEL(Player player) {
			return Invoker::Invoke<int, 1584>(player);
		}

		void SET_MAX_WANTED_LEVEL(int maxWantedLevel) {
			Invoker::Invoke<void, 1585>(maxWantedLevel);
		}

		void SET_POLICE_RADAR_BLIPS(BOOL toggle) {
			Invoker::Invoke<void, 1586>(toggle);
		}

		void SET_POLICE_IGNORE_PLAYER(Player player, BOOL toggle) {
			Invoker::Invoke<void, 1587>(player, toggle);
		}

		BOOL IS_PLAYER_PLAYING(Player player) {
			return Invoker::Invoke<BOOL, 1588>(player);
		}

		void SET_EVERYONE_IGNORE_PLAYER(Player player, BOOL toggle) {
			Invoker::Invoke<void, 1589>(player, toggle);
		}

		void SET_ALL_RANDOM_PEDS_FLEE(Player player, BOOL toggle) {
			Invoker::Invoke<void, 1590>(player, toggle);
		}

		void SET_ALL_RANDOM_PEDS_FLEE_THIS_FRAME(Player player) {
			Invoker::Invoke<void, 1591>(player);
		}

		void SET_ALL_NEUTRAL_RANDOM_PEDS_FLEE(Player player, BOOL toggle) {
			Invoker::Invoke<void, 1592>(player, toggle);
		}

		void SET_ALL_NEUTRAL_RANDOM_PEDS_FLEE_THIS_FRAME(Player player) {
			Invoker::Invoke<void, 1593>(player);
		}

		void SET_LAW_PEDS_CAN_ATTACK_NON_WANTED_PLAYER_THIS_FRAME(Player player) {
			Invoker::Invoke<void, 1594>(player);
		}

		void SET_IGNORE_LOW_PRIORITY_SHOCKING_EVENTS(Player player, BOOL toggle) {
			Invoker::Invoke<void, 1595>(player, toggle);
		}

		void SET_WANTED_LEVEL_MULTIPLIER(float multiplier) {
			Invoker::Invoke<void, 1596>(multiplier);
		}

		void SET_WANTED_LEVEL_DIFFICULTY(Player player, float difficulty) {
			Invoker::Invoke<void, 1597>(player, difficulty);
		}

		void RESET_WANTED_LEVEL_DIFFICULTY(Player player) {
			Invoker::Invoke<void, 1598>(player);
		}

		int GET_WANTED_LEVEL_TIME_TO_ESCAPE() {
			return Invoker::Invoke<int, 1599>();
		}

		void SET_WANTED_LEVEL_HIDDEN_ESCAPE_TIME(Player player, int wantedLevel, int lossTime) {
			Invoker::Invoke<void, 1600>(player, wantedLevel, lossTime);
		}

		void RESET_WANTED_LEVEL_HIDDEN_ESCAPE_TIME(Player player) {
			Invoker::Invoke<void, 1601>(player);
		}

		void START_FIRING_AMNESTY(int duration) {
			Invoker::Invoke<void, 1602>(duration);
		}

		void REPORT_CRIME(Player player, int crimeType, int wantedLvlThresh) {
			Invoker::Invoke<void, 1603>(player, crimeType, wantedLvlThresh);
		}

		void SUPPRESS_CRIME_THIS_FRAME(Player player, int crimeType) {
			Invoker::Invoke<void, 1604>(player, crimeType);
		}

		void UPDATE_WANTED_POSITION_THIS_FRAME(Player player) {
			Invoker::Invoke<void, 1605>(player);
		}

		void SUPPRESS_LOSING_WANTED_LEVEL_IF_HIDDEN_THIS_FRAME(Player player) {
			Invoker::Invoke<void, 1606>(player);
		}

		void ALLOW_EVASION_HUD_IF_DISABLING_HIDDEN_EVASION_THIS_FRAME(Player player, Any p1) {
			Invoker::Invoke<void, 1607>(player, p1);
		}

		void FORCE_START_HIDDEN_EVASION(Player player) {
			Invoker::Invoke<void, 1608>(player);
		}

		void SUPPRESS_WITNESSES_CALLING_POLICE_THIS_FRAME(Player player) {
			Invoker::Invoke<void, 1609>(player);
		}

		void REPORT_POLICE_SPOTTED_PLAYER(Player player) {
			Invoker::Invoke<void, 1610>(player);
		}

		void SET_LAW_RESPONSE_DELAY_OVERRIDE(float p0) {
			Invoker::Invoke<void, 1611>(p0);
		}

		void RESET_LAW_RESPONSE_DELAY_OVERRIDE() {
			Invoker::Invoke<void, 1612>();
		}

		BOOL CAN_PLAYER_START_MISSION(Player player) {
			return Invoker::Invoke<BOOL, 1613>(player);
		}

		BOOL IS_PLAYER_READY_FOR_CUTSCENE(Player player) {
			return Invoker::Invoke<BOOL, 1614>(player);
		}

		BOOL IS_PLAYER_TARGETTING_ENTITY(Player player, Entity entity) {
			return Invoker::Invoke<BOOL, 1615>(player, entity);
		}

		BOOL GET_PLAYER_TARGET_ENTITY(Player player, Entity* entity) {
			return Invoker::Invoke<BOOL, 1616>(player, entity);
		}

		BOOL IS_PLAYER_FREE_AIMING(Player player) {
			return Invoker::Invoke<BOOL, 1617>(player);
		}

		BOOL IS_PLAYER_FREE_AIMING_AT_ENTITY(Player player, Entity entity) {
			return Invoker::Invoke<BOOL, 1618>(player, entity);
		}

		BOOL GET_ENTITY_PLAYER_IS_FREE_AIMING_AT(Player player, Entity* entity) {
			return Invoker::Invoke<BOOL, 1619>(player, entity);
		}

		void SET_PLAYER_LOCKON_RANGE_OVERRIDE(Player player, float range) {
			Invoker::Invoke<void, 1620>(player, range);
		}

		void SET_PLAYER_CAN_DO_DRIVE_BY(Player player, BOOL toggle) {
			Invoker::Invoke<void, 1621>(player, toggle);
		}

		void SET_PLAYER_CAN_BE_HASSLED_BY_GANGS(Player player, BOOL toggle) {
			Invoker::Invoke<void, 1622>(player, toggle);
		}

		void SET_PLAYER_CAN_USE_COVER(Player player, BOOL toggle) {
			Invoker::Invoke<void, 1623>(player, toggle);
		}

		int GET_MAX_WANTED_LEVEL() {
			return Invoker::Invoke<int, 1624>();
		}

		BOOL IS_PLAYER_TARGETTING_ANYTHING(Player player) {
			return Invoker::Invoke<BOOL, 1625>(player);
		}

		void SET_PLAYER_SPRINT(Player player, BOOL toggle) {
			Invoker::Invoke<void, 1626>(player, toggle);
		}

		void RESET_PLAYER_STAMINA(Player player) {
			Invoker::Invoke<void, 1627>(player);
		}

		void RESTORE_PLAYER_STAMINA(Player player, float p1) {
			Invoker::Invoke<void, 1628>(player, p1);
		}

		float GET_PLAYER_SPRINT_STAMINA_REMAINING(Player player) {
			return Invoker::Invoke<float, 1629>(player);
		}

		float GET_PLAYER_SPRINT_TIME_REMAINING(Player player) {
			return Invoker::Invoke<float, 1630>(player);
		}

		float GET_PLAYER_UNDERWATER_TIME_REMAINING(Player player) {
			return Invoker::Invoke<float, 1631>(player);
		}

		float SET_PLAYER_UNDERWATER_BREATH_PERCENT_REMAINING(Player player, float time) {
			return Invoker::Invoke<float, 1632>(player, time);
		}

		int GET_PLAYER_GROUP(Player player) {
			return Invoker::Invoke<int, 1633>(player);
		}

		int GET_PLAYER_MAX_ARMOUR(Player player) {
			return Invoker::Invoke<int, 1634>(player);
		}

		BOOL IS_PLAYER_CONTROL_ON(Player player) {
			return Invoker::Invoke<BOOL, 1635>(player);
		}

		BOOL GET_ARE_CAMERA_CONTROLS_DISABLED() {
			return Invoker::Invoke<BOOL, 1636>();
		}

		BOOL IS_PLAYER_SCRIPT_CONTROL_ON(Player player) {
			return Invoker::Invoke<BOOL, 1637>(player);
		}

		BOOL IS_PLAYER_CLIMBING(Player player) {
			return Invoker::Invoke<BOOL, 1638>(player);
		}

		BOOL IS_PLAYER_BEING_ARRESTED(Player player, BOOL atArresting) {
			return Invoker::Invoke<BOOL, 1639>(player, atArresting);
		}

		void RESET_PLAYER_ARREST_STATE(Player player) {
			Invoker::Invoke<void, 1640>(player);
		}

		Vehicle GET_PLAYERS_LAST_VEHICLE() {
			return Invoker::Invoke<Vehicle, 1641>();
		}

		Player GET_PLAYER_INDEX() {
			return Invoker::Invoke<Player, 1642>();
		}

		Player INT_TO_PLAYERINDEX(int value) {
			return Invoker::Invoke<Player, 1643>(value);
		}

		int INT_TO_PARTICIPANTINDEX(int value) {
			return Invoker::Invoke<int, 1644>(value);
		}

		int GET_TIME_SINCE_PLAYER_HIT_VEHICLE(Player player) {
			return Invoker::Invoke<int, 1645>(player);
		}

		int GET_TIME_SINCE_PLAYER_HIT_PED(Player player) {
			return Invoker::Invoke<int, 1646>(player);
		}

		int GET_TIME_SINCE_PLAYER_DROVE_ON_PAVEMENT(Player player) {
			return Invoker::Invoke<int, 1647>(player);
		}

		int GET_TIME_SINCE_PLAYER_DROVE_AGAINST_TRAFFIC(Player player) {
			return Invoker::Invoke<int, 1648>(player);
		}

		BOOL IS_PLAYER_FREE_FOR_AMBIENT_TASK(Player player) {
			return Invoker::Invoke<BOOL, 1649>(player);
		}

		Player PLAYER_ID() {
			return Invoker::Invoke<Player, 1650>();
		}

		Ped PLAYER_PED_ID() {
			return Invoker::Invoke<Ped, 1651>();
		}

		int NETWORK_PLAYER_ID_TO_INT() {
			return Invoker::Invoke<int, 1652>();
		}

		BOOL HAS_FORCE_CLEANUP_OCCURRED(int cleanupFlags) {
			return Invoker::Invoke<BOOL, 1653>(cleanupFlags);
		}

		void FORCE_CLEANUP(int cleanupFlags) {
			Invoker::Invoke<void, 1654>(cleanupFlags);
		}

		void FORCE_CLEANUP_FOR_ALL_THREADS_WITH_THIS_NAME(const char* name, int cleanupFlags) {
			Invoker::Invoke<void, 1655>(name, cleanupFlags);
		}

		void FORCE_CLEANUP_FOR_THREAD_WITH_THIS_ID(int id, int cleanupFlags) {
			Invoker::Invoke<void, 1656>(id, cleanupFlags);
		}

		int GET_CAUSE_OF_MOST_RECENT_FORCE_CLEANUP() {
			return Invoker::Invoke<int, 1657>();
		}

		void SET_PLAYER_MAY_ONLY_ENTER_THIS_VEHICLE(Player player, Vehicle vehicle) {
			Invoker::Invoke<void, 1658>(player, vehicle);
		}

		void SET_PLAYER_MAY_NOT_ENTER_ANY_VEHICLE(Player player) {
			Invoker::Invoke<void, 1659>(player);
		}

		BOOL GIVE_ACHIEVEMENT_TO_PLAYER(int achievementId) {
			return Invoker::Invoke<BOOL, 1660>(achievementId);
		}

		BOOL SET_ACHIEVEMENT_PROGRESS(int achievementId, int progress) {
			return Invoker::Invoke<BOOL, 1661>(achievementId, progress);
		}

		int GET_ACHIEVEMENT_PROGRESS(int achievementId) {
			return Invoker::Invoke<int, 1662>(achievementId);
		}

		BOOL HAS_ACHIEVEMENT_BEEN_PASSED(int achievementId) {
			return Invoker::Invoke<BOOL, 1663>(achievementId);
		}

		BOOL IS_PLAYER_ONLINE() {
			return Invoker::Invoke<BOOL, 1664>();
		}

		BOOL IS_PLAYER_LOGGING_IN_NP() {
			return Invoker::Invoke<BOOL, 1665>();
		}

		void DISPLAY_SYSTEM_SIGNIN_UI(BOOL p0) {
			Invoker::Invoke<void, 1666>(p0);
		}

		BOOL IS_SYSTEM_UI_BEING_DISPLAYED() {
			return Invoker::Invoke<BOOL, 1667>();
		}

		void SET_PLAYER_INVINCIBLE(Player player, BOOL toggle) {
			Invoker::Invoke<void, 1668>(player, toggle);
		}

		BOOL GET_PLAYER_INVINCIBLE(Player player) {
			return Invoker::Invoke<BOOL, 1669>(player);
		}

		BOOL GET_PLAYER_DEBUG_INVINCIBLE(Player player) {
			return Invoker::Invoke<BOOL, 1670>(player);
		}

		void SET_PLAYER_INVINCIBLE_BUT_HAS_REACTIONS(Player player, BOOL toggle) {
			Invoker::Invoke<void, 1671>(player, toggle);
		}

		void SET_PLAYER_CAN_COLLECT_DROPPED_MONEY(Player player, BOOL p1) {
			Invoker::Invoke<void, 1672>(player, p1);
		}

		void REMOVE_PLAYER_HELMET(Player player, BOOL p2) {
			Invoker::Invoke<void, 1673>(player, p2);
		}

		void GIVE_PLAYER_RAGDOLL_CONTROL(Player player, BOOL toggle) {
			Invoker::Invoke<void, 1674>(player, toggle);
		}

		void SET_PLAYER_LOCKON(Player player, BOOL toggle) {
			Invoker::Invoke<void, 1675>(player, toggle);
		}

		void SET_PLAYER_TARGETING_MODE(int targetMode) {
			Invoker::Invoke<void, 1676>(targetMode);
		}

		int GET_PLAYER_TARGETING_MODE() {
			return Invoker::Invoke<int, 1677>();
		}

		void SET_PLAYER_TARGET_LEVEL(int targetLevel) {
			Invoker::Invoke<void, 1678>(targetLevel);
		}

		BOOL GET_IS_USING_FPS_THIRD_PERSON_COVER() {
			return Invoker::Invoke<BOOL, 1679>();
		}

		BOOL GET_IS_USING_HOOD_CAMERA() {
			return Invoker::Invoke<BOOL, 1680>();
		}

		void CLEAR_PLAYER_HAS_DAMAGED_AT_LEAST_ONE_PED(Player player) {
			Invoker::Invoke<void, 1681>(player);
		}

		BOOL HAS_PLAYER_DAMAGED_AT_LEAST_ONE_PED(Player player) {
			return Invoker::Invoke<BOOL, 1682>(player);
		}

		void CLEAR_PLAYER_HAS_DAMAGED_AT_LEAST_ONE_NON_ANIMAL_PED(Player player) {
			Invoker::Invoke<void, 1683>(player);
		}

		BOOL HAS_PLAYER_DAMAGED_AT_LEAST_ONE_NON_ANIMAL_PED(Player player) {
			return Invoker::Invoke<BOOL, 1684>(player);
		}

		void SET_AIR_DRAG_MULTIPLIER_FOR_PLAYERS_VEHICLE(Player player, float multiplier) {
			Invoker::Invoke<void, 1685>(player, multiplier);
		}

		void SET_SWIM_MULTIPLIER_FOR_PLAYER(Player player, float multiplier) {
			Invoker::Invoke<void, 1686>(player, multiplier);
		}

		void SET_RUN_SPRINT_MULTIPLIER_FOR_PLAYER(Player player, float multiplier) {
			Invoker::Invoke<void, 1687>(player, multiplier);
		}

		int GET_TIME_SINCE_LAST_ARREST() {
			return Invoker::Invoke<int, 1688>();
		}

		int GET_TIME_SINCE_LAST_DEATH() {
			return Invoker::Invoke<int, 1689>();
		}

		void ASSISTED_MOVEMENT_CLOSE_ROUTE() {
			Invoker::Invoke<void, 1690>();
		}

		void ASSISTED_MOVEMENT_FLUSH_ROUTE() {
			Invoker::Invoke<void, 1691>();
		}

		void SET_PLAYER_FORCED_AIM(Player player, BOOL toggle) {
			Invoker::Invoke<void, 1692>(player, toggle);
		}

		void SET_PLAYER_FORCED_ZOOM(Player player, BOOL toggle) {
			Invoker::Invoke<void, 1693>(player, toggle);
		}

		void SET_PLAYER_FORCE_SKIP_AIM_INTRO(Player player, BOOL toggle) {
			Invoker::Invoke<void, 1694>(player, toggle);
		}

		void DISABLE_PLAYER_FIRING(Player player, BOOL toggle) {
			Invoker::Invoke<void, 1695>(player, toggle);
		}

		void DISABLE_PLAYER_THROW_GRENADE_WHILE_USING_GUN() {
			Invoker::Invoke<void, 1696>();
		}

		void SET_DISABLE_AMBIENT_MELEE_MOVE(Player player, BOOL toggle) {
			Invoker::Invoke<void, 1697>(player, toggle);
		}

		void SET_PLAYER_MAX_ARMOUR(Player player, int value) {
			Invoker::Invoke<void, 1698>(player, value);
		}

		void SPECIAL_ABILITY_ACTIVATE(Player player, int p1) {
			Invoker::Invoke<void, 1699>(player, p1);
		}

		void SET_SPECIAL_ABILITY_MP(Player player, int p1, Any p2) {
			Invoker::Invoke<void, 1700>(player, p1, p2);
		}

		void SPECIAL_ABILITY_DEACTIVATE_MP(Player player, int p1) {
			Invoker::Invoke<void, 1701>(player, p1);
		}

		void SPECIAL_ABILITY_DEACTIVATE(Player player, Any p1) {
			Invoker::Invoke<void, 1702>(player, p1);
		}

		void SPECIAL_ABILITY_DEACTIVATE_FAST(Player player, Any p1) {
			Invoker::Invoke<void, 1703>(player, p1);
		}

		void SPECIAL_ABILITY_RESET(Player player, Any p1) {
			Invoker::Invoke<void, 1704>(player, p1);
		}

		void SPECIAL_ABILITY_CHARGE_ON_MISSION_FAILED(Player player, Any p1) {
			Invoker::Invoke<void, 1705>(player, p1);
		}

		void SPECIAL_ABILITY_CHARGE_SMALL(Player player, BOOL p1, BOOL p2, Any p3) {
			Invoker::Invoke<void, 1706>(player, p1, p2, p3);
		}

		void SPECIAL_ABILITY_CHARGE_MEDIUM(Player player, BOOL p1, BOOL p2, Any p3) {
			Invoker::Invoke<void, 1707>(player, p1, p2, p3);
		}

		void SPECIAL_ABILITY_CHARGE_LARGE(Player player, BOOL p1, BOOL p2, Any p3) {
			Invoker::Invoke<void, 1708>(player, p1, p2, p3);
		}

		void SPECIAL_ABILITY_CHARGE_CONTINUOUS(Player player, Ped p1, Any p2) {
			Invoker::Invoke<void, 1709>(player, p1, p2);
		}

		void SPECIAL_ABILITY_CHARGE_ABSOLUTE(Player player, int p1, BOOL p2, Any p3) {
			Invoker::Invoke<void, 1710>(player, p1, p2, p3);
		}

		void SPECIAL_ABILITY_CHARGE_NORMALIZED(Player player, float normalizedValue, BOOL p2, Any p3) {
			Invoker::Invoke<void, 1711>(player, normalizedValue, p2, p3);
		}

		void SPECIAL_ABILITY_FILL_METER(Player player, BOOL p1, Any p2) {
			Invoker::Invoke<void, 1712>(player, p1, p2);
		}

		void SPECIAL_ABILITY_DEPLETE_METER(Player player, BOOL p1, Any p2) {
			Invoker::Invoke<void, 1713>(player, p1, p2);
		}

		void SPECIAL_ABILITY_LOCK(Hash playerModel, Any p1) {
			Invoker::Invoke<void, 1714>(playerModel, p1);
		}

		void SPECIAL_ABILITY_UNLOCK(Hash playerModel, Any p1) {
			Invoker::Invoke<void, 1715>(playerModel, p1);
		}

		BOOL IS_SPECIAL_ABILITY_UNLOCKED(Hash playerModel) {
			return Invoker::Invoke<BOOL, 1716>(playerModel);
		}

		BOOL IS_SPECIAL_ABILITY_ACTIVE(Player player, Any p1) {
			return Invoker::Invoke<BOOL, 1717>(player, p1);
		}

		BOOL IS_SPECIAL_ABILITY_METER_FULL(Player player, Any p1) {
			return Invoker::Invoke<BOOL, 1718>(player, p1);
		}

		void ENABLE_SPECIAL_ABILITY(Player player, BOOL toggle, Any p2) {
			Invoker::Invoke<void, 1719>(player, toggle, p2);
		}

		BOOL IS_SPECIAL_ABILITY_ENABLED(Player player, Any p1) {
			return Invoker::Invoke<BOOL, 1720>(player, p1);
		}

		void SET_SPECIAL_ABILITY_MULTIPLIER(float multiplier) {
			Invoker::Invoke<void, 1721>(multiplier);
		}

		void UPDATE_SPECIAL_ABILITY_FROM_STAT(Player player, Any p1) {
			Invoker::Invoke<void, 1722>(player, p1);
		}

		BOOL GET_IS_PLAYER_DRIVING_ON_HIGHWAY(Player player) {
			return Invoker::Invoke<BOOL, 1723>(player);
		}

		BOOL GET_IS_PLAYER_DRIVING_WRECKLESS(Player player, int p1) {
			return Invoker::Invoke<BOOL, 1724>(player, p1);
		}

		BOOL GET_IS_MOPPING_AREA_FREE_IN_FRONT_OF_PLAYER(Player player, float p1) {
			return Invoker::Invoke<BOOL, 1725>(player, p1);
		}

		void START_PLAYER_TELEPORT(Player player, float x, float y, float z, float heading, BOOL p5, BOOL findCollisionLand, BOOL p7) {
			Invoker::Invoke<void, 1726>(player, x, y, z, heading, p5, findCollisionLand, p7);
		}

		BOOL UPDATE_PLAYER_TELEPORT(Player player) {
			return Invoker::Invoke<BOOL, 1727>(player);
		}

		void STOP_PLAYER_TELEPORT() {
			Invoker::Invoke<void, 1728>();
		}

		BOOL IS_PLAYER_TELEPORT_ACTIVE() {
			return Invoker::Invoke<BOOL, 1729>();
		}

		float GET_PLAYER_CURRENT_STEALTH_NOISE(Player player) {
			return Invoker::Invoke<float, 1730>(player);
		}

		void SET_PLAYER_HEALTH_RECHARGE_MULTIPLIER(Player player, float regenRate) {
			Invoker::Invoke<void, 1731>(player, regenRate);
		}

		float GET_PLAYER_HEALTH_RECHARGE_MAX_PERCENT(Player player) {
			return Invoker::Invoke<float, 1732>(player);
		}

		void SET_PLAYER_HEALTH_RECHARGE_MAX_PERCENT(Player player, float limit) {
			Invoker::Invoke<void, 1733>(player, limit);
		}

		void DISABLE_PLAYER_HEALTH_RECHARGE(Player player) {
			Invoker::Invoke<void, 1734>(player);
		}

		void SET_PLAYER_FALL_DISTANCE_TO_TRIGGER_RAGDOLL_OVERRIDE(Player player, float p1) {
			Invoker::Invoke<void, 1735>(player, p1);
		}

		void SET_PLAYER_WEAPON_DAMAGE_MODIFIER(Player player, float modifier) {
			Invoker::Invoke<void, 1736>(player, modifier);
		}

		void SET_PLAYER_WEAPON_DEFENSE_MODIFIER(Player player, float modifier) {
			Invoker::Invoke<void, 1737>(player, modifier);
		}

		void SET_PLAYER_WEAPON_MINIGUN_DEFENSE_MODIFIER(Player player, float modifier) {
			Invoker::Invoke<void, 1738>(player, modifier);
		}

		void SET_PLAYER_MELEE_WEAPON_DAMAGE_MODIFIER(Player player, float modifier, BOOL p2) {
			Invoker::Invoke<void, 1739>(player, modifier, p2);
		}

		void SET_PLAYER_MELEE_WEAPON_DEFENSE_MODIFIER(Player player, float modifier) {
			Invoker::Invoke<void, 1740>(player, modifier);
		}

		void SET_PLAYER_VEHICLE_DAMAGE_MODIFIER(Player player, float modifier) {
			Invoker::Invoke<void, 1741>(player, modifier);
		}

		void SET_PLAYER_VEHICLE_DEFENSE_MODIFIER(Player player, float modifier) {
			Invoker::Invoke<void, 1742>(player, modifier);
		}

		void SET_PLAYER_MAX_EXPLOSIVE_DAMAGE(Player player, float p1) {
			Invoker::Invoke<void, 1743>(player, p1);
		}

		void SET_PLAYER_EXPLOSIVE_DAMAGE_MODIFIER(Player player, Any p1) {
			Invoker::Invoke<void, 1744>(player, p1);
		}

		void SET_PLAYER_WEAPON_TAKEDOWN_DEFENSE_MODIFIER(Player player, float p1) {
			Invoker::Invoke<void, 1745>(player, p1);
		}

		void SET_PLAYER_PARACHUTE_TINT_INDEX(Player player, int tintIndex) {
			Invoker::Invoke<void, 1746>(player, tintIndex);
		}

		void GET_PLAYER_PARACHUTE_TINT_INDEX(Player player, int* tintIndex) {
			Invoker::Invoke<void, 1747>(player, tintIndex);
		}

		void SET_PLAYER_RESERVE_PARACHUTE_TINT_INDEX(Player player, int index) {
			Invoker::Invoke<void, 1748>(player, index);
		}

		void GET_PLAYER_RESERVE_PARACHUTE_TINT_INDEX(Player player, int* index) {
			Invoker::Invoke<void, 1749>(player, index);
		}

		void SET_PLAYER_PARACHUTE_PACK_TINT_INDEX(Player player, int tintIndex) {
			Invoker::Invoke<void, 1750>(player, tintIndex);
		}

		void GET_PLAYER_PARACHUTE_PACK_TINT_INDEX(Player player, int* tintIndex) {
			Invoker::Invoke<void, 1751>(player, tintIndex);
		}

		void SET_PLAYER_HAS_RESERVE_PARACHUTE(Player player) {
			Invoker::Invoke<void, 1752>(player);
		}

		BOOL GET_PLAYER_HAS_RESERVE_PARACHUTE(Player player) {
			return Invoker::Invoke<BOOL, 1753>(player);
		}

		void SET_PLAYER_CAN_LEAVE_PARACHUTE_SMOKE_TRAIL(Player player, BOOL enabled) {
			Invoker::Invoke<void, 1754>(player, enabled);
		}

		void SET_PLAYER_PARACHUTE_SMOKE_TRAIL_COLOR(Player player, int r, int g, int b) {
			Invoker::Invoke<void, 1755>(player, r, g, b);
		}

		void GET_PLAYER_PARACHUTE_SMOKE_TRAIL_COLOR(Player player, int* r, int* g, int* b) {
			Invoker::Invoke<void, 1756>(player, r, g, b);
		}

		void SET_PLAYER_PHONE_PALETTE_IDX(Player player, int idx) {
			Invoker::Invoke<void, 1757>(player, idx);
		}

		void SET_PLAYER_NOISE_MULTIPLIER(Player player, float multiplier) {
			Invoker::Invoke<void, 1758>(player, multiplier);
		}

		void SET_PLAYER_SNEAKING_NOISE_MULTIPLIER(Player player, float multiplier) {
			Invoker::Invoke<void, 1759>(player, multiplier);
		}

		BOOL CAN_PED_HEAR_PLAYER(Player player, Ped ped) {
			return Invoker::Invoke<BOOL, 1760>(player, ped);
		}

		void SIMULATE_PLAYER_INPUT_GAIT(Player player, float amount, int gaitType, float speed, BOOL p4, BOOL p5, Any p6) {
			Invoker::Invoke<void, 1761>(player, amount, gaitType, speed, p4, p5, p6);
		}

		void RESET_PLAYER_INPUT_GAIT(Player player) {
			Invoker::Invoke<void, 1762>(player);
		}

		void SET_AUTO_GIVE_PARACHUTE_WHEN_ENTER_PLANE(Player player, BOOL toggle) {
			Invoker::Invoke<void, 1763>(player, toggle);
		}

		void SET_AUTO_GIVE_SCUBA_GEAR_WHEN_EXIT_VEHICLE(Player player, BOOL toggle) {
			Invoker::Invoke<void, 1764>(player, toggle);
		}

		void SET_PLAYER_STEALTH_PERCEPTION_MODIFIER(Player player, float value) {
			Invoker::Invoke<void, 1765>(player, value);
		}

		BOOL IS_REMOTE_PLAYER_IN_NON_CLONED_VEHICLE(Player player) {
			return Invoker::Invoke<BOOL, 1766>(player);
		}

		void INCREASE_PLAYER_JUMP_SUPPRESSION_RANGE(Player player) {
			Invoker::Invoke<void, 1767>(player);
		}

		void SET_PLAYER_SIMULATE_AIMING(Player player, BOOL toggle) {
			Invoker::Invoke<void, 1768>(player, toggle);
		}

		void SET_PLAYER_CLOTH_PIN_FRAMES(Player player, int p1) {
			Invoker::Invoke<void, 1769>(player, p1);
		}

		void SET_PLAYER_CLOTH_PACKAGE_INDEX(int index) {
			Invoker::Invoke<void, 1770>(index);
		}

		void SET_PLAYER_CLOTH_LOCK_COUNTER(int value) {
			Invoker::Invoke<void, 1771>(value);
		}

		void PLAYER_ATTACH_VIRTUAL_BOUND(float p0, float p1, float p2, float p3, float p4, float p5, float p6, float p7) {
			Invoker::Invoke<void, 1772>(p0, p1, p2, p3, p4, p5, p6, p7);
		}

		void PLAYER_DETACH_VIRTUAL_BOUND() {
			Invoker::Invoke<void, 1773>();
		}

		BOOL HAS_PLAYER_BEEN_SPOTTED_IN_STOLEN_VEHICLE(Player player) {
			return Invoker::Invoke<BOOL, 1774>(player);
		}

		BOOL IS_PLAYER_BATTLE_AWARE(Player player) {
			return Invoker::Invoke<BOOL, 1775>(player);
		}

		BOOL GET_PLAYER_RECEIVED_BATTLE_EVENT_RECENTLY(Player player, int p1, BOOL p2) {
			return Invoker::Invoke<BOOL, 1776>(player, p1, p2);
		}

		void EXTEND_WORLD_BOUNDARY_FOR_PLAYER(float x, float y, float z) {
			Invoker::Invoke<void, 1777>(x, y, z);
		}

		void RESET_WORLD_BOUNDARY_FOR_PLAYER() {
			Invoker::Invoke<void, 1778>();
		}

		BOOL IS_PLAYER_RIDING_TRAIN(Player player) {
			return Invoker::Invoke<BOOL, 1779>(player);
		}

		BOOL HAS_PLAYER_LEFT_THE_WORLD(Player player) {
			return Invoker::Invoke<BOOL, 1780>(player);
		}

		void SET_PLAYER_LEAVE_PED_BEHIND(Player player, BOOL toggle) {
			Invoker::Invoke<void, 1781>(player, toggle);
		}

		void SET_PLAYER_PARACHUTE_VARIATION_OVERRIDE(Player player, int p1, Any p2, Any p3, BOOL p4) {
			Invoker::Invoke<void, 1782>(player, p1, p2, p3, p4);
		}

		void CLEAR_PLAYER_PARACHUTE_VARIATION_OVERRIDE(Player player) {
			Invoker::Invoke<void, 1783>(player);
		}

		void SET_PLAYER_PARACHUTE_MODEL_OVERRIDE(Player player, Hash model) {
			Invoker::Invoke<void, 1784>(player, model);
		}

		void SET_PLAYER_RESERVE_PARACHUTE_MODEL_OVERRIDE(Player player, Hash model) {
			Invoker::Invoke<void, 1785>(player, model);
		}

		Hash GET_PLAYER_PARACHUTE_MODEL_OVERRIDE(Player player) {
			return Invoker::Invoke<Hash, 1786>(player);
		}

		Hash GET_PLAYER_RESERVE_PARACHUTE_MODEL_OVERRIDE(Player player) {
			return Invoker::Invoke<Hash, 1787>(player);
		}

		void CLEAR_PLAYER_PARACHUTE_MODEL_OVERRIDE(Player player) {
			Invoker::Invoke<void, 1788>(player);
		}

		void CLEAR_PLAYER_RESERVE_PARACHUTE_MODEL_OVERRIDE(Player player) {
			Invoker::Invoke<void, 1789>(player);
		}

		void SET_PLAYER_PARACHUTE_PACK_MODEL_OVERRIDE(Player player, Hash model) {
			Invoker::Invoke<void, 1790>(player, model);
		}

		void CLEAR_PLAYER_PARACHUTE_PACK_MODEL_OVERRIDE(Player player) {
			Invoker::Invoke<void, 1791>(player);
		}

		void DISABLE_PLAYER_VEHICLE_REWARDS(Player player) {
			Invoker::Invoke<void, 1792>(player);
		}

		void SET_PLAYER_SPECTATED_VEHICLE_RADIO_OVERRIDE(BOOL p0) {
			Invoker::Invoke<void, 1793>(p0);
		}

		void SET_PLAYER_BLUETOOTH_STATE(Player player, BOOL state) {
			Invoker::Invoke<void, 1794>(player, state);
		}

		BOOL IS_PLAYER_BLUETOOTH_ENABLE(Player player) {
			return Invoker::Invoke<BOOL, 1795>(player);
		}

		void DISABLE_CAMERA_VIEW_MODE_CYCLE(Player player) {
			Invoker::Invoke<void, 1796>(player);
		}

		int GET_PLAYER_FAKE_WANTED_LEVEL(Player player) {
			return Invoker::Invoke<int, 1797>(player);
		}

		void SET_PLAYER_CAN_DAMAGE_PLAYER(Player player1, Player player2, BOOL toggle) {
			Invoker::Invoke<void, 1798>(player1, player2, toggle);
		}

		void SET_APPLY_WAYPOINT_OF_PLAYER(Player player, int hudColor) {
			Invoker::Invoke<void, 1799>(player, hudColor);
		}

		BOOL IS_PLAYER_VEHICLE_WEAPON_TOGGLED_TO_NON_HOMING(Any p0) {
			return Invoker::Invoke<BOOL, 1800>(p0);
		}

		void SET_PLAYER_VEHICLE_WEAPON_TO_NON_HOMING(Any p0) {
			Invoker::Invoke<void, 1801>(p0);
		}

		void SET_PLAYER_HOMING_DISABLED_FOR_ALL_VEHICLE_WEAPONS(Any p0, Any p1) {
			Invoker::Invoke<void, 1802>(p0, p1);
		}

		void ADD_PLAYER_TARGETABLE_ENTITY(Player player, Entity entity) {
			Invoker::Invoke<void, 1803>(player, entity);
		}

		void REMOVE_PLAYER_TARGETABLE_ENTITY(Player player, Entity entity) {
			Invoker::Invoke<void, 1804>(player, entity);
		}

		void SET_PLAYER_PREVIOUS_VARIATION_DATA(Player player, int p1, int p2, Any p3, Any p4, Any p5) {
			Invoker::Invoke<void, 1805>(player, p1, p2, p3, p4, p5);
		}

		void REMOVE_SCRIPT_FIRE_POSITION() {
			Invoker::Invoke<void, 1806>();
		}

		void SET_SCRIPT_FIRE_POSITION(float coordX, float coordY, float coordZ) {
			Invoker::Invoke<void, 1807>(coordX, coordY, coordZ);
		}

	} // namespace PLAYER

	namespace RECORDING {

		void REPLAY_START_EVENT(int p0) {
			Invoker::Invoke<void, 3723>(p0);
		}

		void REPLAY_STOP_EVENT() {
			Invoker::Invoke<void, 3724>();
		}

		void REPLAY_CANCEL_EVENT() {
			Invoker::Invoke<void, 3725>();
		}

		void REPLAY_RECORD_BACK_FOR_TIME(float p0, float p1, int p2) {
			Invoker::Invoke<void, 3726>(p0, p1, p2);
		}

		void REPLAY_CHECK_FOR_EVENT_THIS_FRAME(const char* missionNameLabel, Any p1) {
			Invoker::Invoke<void, 3727>(missionNameLabel, p1);
		}

		void REPLAY_PREVENT_RECORDING_THIS_FRAME() {
			Invoker::Invoke<void, 3728>();
		}

		void REPLAY_RESET_EVENT_INFO() {
			Invoker::Invoke<void, 3729>();
		}

		void REPLAY_DISABLE_CAMERA_MOVEMENT_THIS_FRAME() {
			Invoker::Invoke<void, 3730>();
		}

		void RECORD_GREATEST_MOMENT(int p0, int p1, int p2) {
			Invoker::Invoke<void, 3731>(p0, p1, p2);
		}

		void START_REPLAY_RECORDING(int mode) {
			Invoker::Invoke<void, 3732>(mode);
		}

		void STOP_REPLAY_RECORDING() {
			Invoker::Invoke<void, 3733>();
		}

		void CANCEL_REPLAY_RECORDING() {
			Invoker::Invoke<void, 3734>();
		}

		BOOL SAVE_REPLAY_RECORDING() {
			return Invoker::Invoke<BOOL, 3735>();
		}

		BOOL IS_REPLAY_RECORDING() {
			return Invoker::Invoke<BOOL, 3736>();
		}

		BOOL IS_REPLAY_INITIALIZED() {
			return Invoker::Invoke<BOOL, 3737>();
		}

		BOOL IS_REPLAY_AVAILABLE() {
			return Invoker::Invoke<BOOL, 3738>();
		}

		BOOL IS_REPLAY_RECORD_SPACE_AVAILABLE(BOOL p0) {
			return Invoker::Invoke<BOOL, 3739>(p0);
		}

	} // namespace RECORDING

	namespace REPLAY {

		void REGISTER_EFFECT_FOR_REPLAY_EDITOR(const char* p0, BOOL p1) {
			Invoker::Invoke<void, 277>(p0, p1);
		}

		BOOL REPLAY_SYSTEM_HAS_REQUESTED_A_SCRIPT_CLEANUP() {
			return Invoker::Invoke<BOOL, 278>();
		}

		void SET_SCRIPTS_HAVE_CLEANED_UP_FOR_REPLAY_SYSTEM() {
			Invoker::Invoke<void, 279>();
		}

		void SET_REPLAY_SYSTEM_PAUSED_FOR_SAVE(BOOL p0) {
			Invoker::Invoke<void, 280>(p0);
		}

		void REPLAY_CONTROL_SHUTDOWN() {
			Invoker::Invoke<void, 281>();
		}

		void ACTIVATE_ROCKSTAR_EDITOR(int p0) {
			Invoker::Invoke<void, 282>(p0);
		}

	} // namespace REPLAY

	namespace SAVEMIGRATION {

		BOOL SAVEMIGRATION_IS_MP_ENABLED() {
			return Invoker::Invoke<BOOL, 6558>();
		}

		BOOL SAVEMIGRATION_MP_REQUEST_ACCOUNTS() {
			return Invoker::Invoke<BOOL, 6559>();
		}

		int SAVEMIGRATION_MP_GET_ACCOUNTS_STATUS() {
			return Invoker::Invoke<int, 6560>();
		}

		int SAVEMIGRATION_MP_NUM_ACCOUNTS() {
			return Invoker::Invoke<int, 6561>();
		}

		BOOL SAVEMIGRATION_MP_GET_ACCOUNT(int p0, Any* p1) {
			return Invoker::Invoke<BOOL, 6562>(p0, p1);
		}

		BOOL SAVEMIGRATION_MP_REQUEST_STATUS() {
			return Invoker::Invoke<BOOL, 6563>();
		}

		int SAVEMIGRATION_MP_GET_STATUS() {
			return Invoker::Invoke<int, 6564>();
		}

		BOOL SAVEMIGRATION_MP_IS_PLATFORM_GENERATION(int generation) {
			return Invoker::Invoke<BOOL, 6617>(generation);
		}

	} // namespace SAVEMIGRATION

	namespace SCRIPT {

		void REQUEST_SCRIPT(const char* scriptName) {
			Invoker::Invoke<void, 66>(scriptName);
		}

		void SET_SCRIPT_AS_NO_LONGER_NEEDED(const char* scriptName) {
			Invoker::Invoke<void, 67>(scriptName);
		}

		BOOL HAS_SCRIPT_LOADED(const char* scriptName) {
			return Invoker::Invoke<BOOL, 68>(scriptName);
		}

		BOOL DOES_SCRIPT_EXIST(const char* scriptName) {
			return Invoker::Invoke<BOOL, 69>(scriptName);
		}

		void REQUEST_SCRIPT_WITH_NAME_HASH(Hash scriptHash) {
			Invoker::Invoke<void, 70>(scriptHash);
		}

		void SET_SCRIPT_WITH_NAME_HASH_AS_NO_LONGER_NEEDED(Hash scriptHash) {
			Invoker::Invoke<void, 71>(scriptHash);
		}

		BOOL HAS_SCRIPT_WITH_NAME_HASH_LOADED(Hash scriptHash) {
			return Invoker::Invoke<BOOL, 72>(scriptHash);
		}

		BOOL DOES_SCRIPT_WITH_NAME_HASH_EXIST(Hash scriptHash) {
			return Invoker::Invoke<BOOL, 73>(scriptHash);
		}

		void TERMINATE_THREAD(int threadId) {
			Invoker::Invoke<void, 74>(threadId);
		}

		BOOL IS_THREAD_ACTIVE(int threadId) {
			return Invoker::Invoke<BOOL, 75>(threadId);
		}

		const char* GET_NAME_OF_SCRIPT_WITH_THIS_ID(int threadId) {
			return Invoker::Invoke<const char*, 76>(threadId);
		}

		void SCRIPT_THREAD_ITERATOR_RESET() {
			Invoker::Invoke<void, 77>();
		}

		int SCRIPT_THREAD_ITERATOR_GET_NEXT_THREAD_ID() {
			return Invoker::Invoke<int, 78>();
		}

		int GET_ID_OF_THIS_THREAD() {
			return Invoker::Invoke<int, 79>();
		}

		void TERMINATE_THIS_THREAD() {
			Invoker::Invoke<void, 80>();
		}

		int GET_NUMBER_OF_THREADS_RUNNING_THE_SCRIPT_WITH_THIS_HASH(Hash scriptHash) {
			return Invoker::Invoke<int, 81>(scriptHash);
		}

		const char* GET_THIS_SCRIPT_NAME() {
			return Invoker::Invoke<const char*, 82>();
		}

		Hash GET_HASH_OF_THIS_SCRIPT_NAME() {
			return Invoker::Invoke<Hash, 83>();
		}

		int GET_NUMBER_OF_EVENTS(int eventGroup) {
			return Invoker::Invoke<int, 84>(eventGroup);
		}

		BOOL GET_EVENT_EXISTS(int eventGroup, int eventIndex) {
			return Invoker::Invoke<BOOL, 85>(eventGroup, eventIndex);
		}

		int GET_EVENT_AT_INDEX(int eventGroup, int eventIndex) {
			return Invoker::Invoke<int, 86>(eventGroup, eventIndex);
		}

		BOOL GET_EVENT_DATA(int eventGroup, int eventIndex, Any* eventData, int eventDataSize) {
			return Invoker::Invoke<BOOL, 87>(eventGroup, eventIndex, eventData, eventDataSize);
		}

		void TRIGGER_SCRIPT_EVENT(int eventGroup, Any* eventData, int eventDataSize, int playerBits) {
			Invoker::Invoke<void, 88>(eventGroup, eventData, eventDataSize, playerBits);
		}

		void SHUTDOWN_LOADING_SCREEN() {
			Invoker::Invoke<void, 89>();
		}

		void SET_NO_LOADING_SCREEN(BOOL toggle) {
			Invoker::Invoke<void, 90>(toggle);
		}

		BOOL GET_NO_LOADING_SCREEN() {
			return Invoker::Invoke<BOOL, 91>();
		}

		BOOL SET_LOADING_SCREEN_BLANK() {
			return Invoker::Invoke<BOOL, 6616>();
		}

		void COMMIT_TO_LOADINGSCREEN_SELCTION() {
			Invoker::Invoke<void, 92>();
		}

		BOOL BG_IS_EXITFLAG_SET() {
			return Invoker::Invoke<BOOL, 93>();
		}

		void BG_SET_EXITFLAG_RESPONSE() {
			Invoker::Invoke<void, 94>();
		}

		void BG_START_CONTEXT_HASH(Hash contextHash) {
			Invoker::Invoke<void, 95>(contextHash);
		}

		void BG_END_CONTEXT_HASH(Hash contextHash) {
			Invoker::Invoke<void, 96>(contextHash);
		}

		void BG_START_CONTEXT(const char* contextName) {
			Invoker::Invoke<void, 97>(contextName);
		}

		void BG_END_CONTEXT(const char* contextName) {
			Invoker::Invoke<void, 98>(contextName);
		}

		BOOL BG_DOES_LAUNCH_PARAM_EXIST(int scriptIndex, const char* p1) {
			return Invoker::Invoke<BOOL, 99>(scriptIndex, p1);
		}

		int BG_GET_LAUNCH_PARAM_VALUE(int scriptIndex, const char* p1) {
			return Invoker::Invoke<int, 100>(scriptIndex, p1);
		}

		int BG_GET_SCRIPT_ID_FROM_NAME_HASH(Hash p0) {
			return Invoker::Invoke<int, 101>(p0);
		}

		void _SEND_TU_SCRIPT_EVENT_NEW(int eventGroup, Any* eventData, int eventDataSize, int playerBits, Hash eventType) {
			Invoker::Invoke<void, 102>(eventGroup, eventData, eventDataSize, playerBits, eventType);
		}

	} // namespace SCRIPT

	namespace SECURITY {

		void REGISTER_SCRIPT_VARIABLE(Any* variable) {
			Invoker::Invoke<void, 6565>(variable);
		}

		void UNREGISTER_SCRIPT_VARIABLE(Any* variable) {
			Invoker::Invoke<void, 6566>(variable);
		}

		void FORCE_CHECK_SCRIPT_VARIABLES() {
			Invoker::Invoke<void, 6567>();
		}

	} // namespace SECURITY

	namespace SHAPETEST {

		int START_SHAPE_TEST_LOS_PROBE(float x1, float y1, float z1, float x2, float y2, float z2, int flags, Entity entity, int p8) {
			return Invoker::Invoke<int, 3712>(x1, y1, z1, x2, y2, z2, flags, entity, p8);
		}

		int START_EXPENSIVE_SYNCHRONOUS_SHAPE_TEST_LOS_PROBE(float x1, float y1, float z1, float x2, float y2, float z2, int flags, Entity entity, int p8) {
			return Invoker::Invoke<int, 3713>(x1, y1, z1, x2, y2, z2, flags, entity, p8);
		}

		int START_SHAPE_TEST_BOUNDING_BOX(Entity entity, int flags1, int flags2) {
			return Invoker::Invoke<int, 3714>(entity, flags1, flags2);
		}

		int START_SHAPE_TEST_BOX(float x, float y, float z, float dimX, float dimY, float dimZ, float rotX, float rotY, float rotZ, Any p9, int flags, Entity entity, Any p12) {
			return Invoker::Invoke<int, 3715>(x, y, z, dimX, dimY, dimZ, rotX, rotY, rotZ, p9, flags, entity, p12);
		}

		int START_SHAPE_TEST_BOUND(Entity entity, int flags1, int flags2) {
			return Invoker::Invoke<int, 3716>(entity, flags1, flags2);
		}

		int START_SHAPE_TEST_CAPSULE(float x1, float y1, float z1, float x2, float y2, float z2, float radius, int flags, Entity entity, int p9) {
			return Invoker::Invoke<int, 3717>(x1, y1, z1, x2, y2, z2, radius, flags, entity, p9);
		}

		int START_SHAPE_TEST_SWEPT_SPHERE(float x1, float y1, float z1, float x2, float y2, float z2, float radius, int flags, Entity entity, Any p9) {
			return Invoker::Invoke<int, 3718>(x1, y1, z1, x2, y2, z2, radius, flags, entity, p9);
		}

		int START_SHAPE_TEST_MOUSE_CURSOR_LOS_PROBE(Vector3* pVec1, Vector3* pVec2, int flag, Entity entity, int flag2) {
			return Invoker::Invoke<int, 3719>(pVec1, pVec2, flag, entity, flag2);
		}

		int GET_SHAPE_TEST_RESULT(int shapeTestHandle, BOOL* hit, Vector3* endCoords, Vector3* surfaceNormal, Entity* entityHit) {
			return Invoker::Invoke<int, 3720>(shapeTestHandle, hit, endCoords, surfaceNormal, entityHit);
		}

		int GET_SHAPE_TEST_RESULT_INCLUDING_MATERIAL(int shapeTestHandle, BOOL* hit, Vector3* endCoords, Vector3* surfaceNormal, Hash* materialHash, Entity* entityHit) {
			return Invoker::Invoke<int, 3721>(shapeTestHandle, hit, endCoords, surfaceNormal, materialHash, entityHit);
		}

		void RELEASE_SCRIPT_GUID_FROM_ENTITY(Entity entityHit) {
			Invoker::Invoke<void, 3722>(entityHit);
		}

	} // namespace SHAPETEST

	namespace SOCIALCLUB {

		int SC_INBOX_GET_TOTAL_NUM_MESSAGES() {
			return Invoker::Invoke<int, 2813>();
		}

		Hash SC_INBOX_GET_MESSAGE_TYPE_AT_INDEX(int msgIndex) {
			return Invoker::Invoke<Hash, 2814>(msgIndex);
		}

		BOOL SC_INBOX_GET_MESSAGE_IS_READ_AT_INDEX(int msgIndex) {
			return Invoker::Invoke<BOOL, 2815>(msgIndex);
		}

		BOOL SC_INBOX_SET_MESSAGE_AS_READ_AT_INDEX(int msgIndex) {
			return Invoker::Invoke<BOOL, 2816>(msgIndex);
		}

		BOOL SC_INBOX_MESSAGE_GET_DATA_INT(int p0, const char* context, int* out) {
			return Invoker::Invoke<BOOL, 2817>(p0, context, out);
		}

		BOOL SC_INBOX_MESSAGE_GET_DATA_BOOL(int p0, const char* p1) {
			return Invoker::Invoke<BOOL, 2818>(p0, p1);
		}

		BOOL SC_INBOX_MESSAGE_GET_DATA_STRING(int p0, const char* context, char* out) {
			return Invoker::Invoke<BOOL, 2819>(p0, context, out);
		}

		BOOL SC_INBOX_MESSAGE_DO_APPLY(int p0) {
			return Invoker::Invoke<BOOL, 2820>(p0);
		}

		const char* SC_INBOX_MESSAGE_GET_RAW_TYPE_AT_INDEX(int p0) {
			return Invoker::Invoke<const char*, 2821>(p0);
		}

		void SC_INBOX_MESSAGE_PUSH_GAMER_T0_RECIP_LIST(Any* gamerHandle) {
			Invoker::Invoke<void, 2822>(gamerHandle);
		}

		void SC_INBOX_SEND_UGCSTATUPDATE_TO_RECIP_LIST(Any* data) {
			Invoker::Invoke<void, 2823>(data);
		}

		BOOL SC_INBOX_MESSAGE_GET_UGCDATA(int p0, Any* p1) {
			return Invoker::Invoke<BOOL, 2824>(p0, p1);
		}

		BOOL SC_INBOX_GET_BOUNTY_DATA_AT_INDEX(int index, Any* outData) {
			return Invoker::Invoke<BOOL, 2825>(index, outData);
		}

		void SC_EMAIL_RETRIEVE_EMAILS(int offset, int limit) {
			Invoker::Invoke<void, 2826>(offset, limit);
		}

		int SC_EMAIL_GET_RETRIEVAL_STATUS() {
			return Invoker::Invoke<int, 2827>();
		}

		int SC_EMAIL_GET_NUM_RETRIEVED_EMAILS() {
			return Invoker::Invoke<int, 2828>();
		}

		BOOL SC_EMAIL_GET_EMAIL_AT_INDEX(int p0, Any* p1) {
			return Invoker::Invoke<BOOL, 2829>(p0, p1);
		}

		void _SC_EMAIL_MARKETING_EMAIL_OPENED(int index, Hash type) {
			Invoker::Invoke<void, 6587>(index, type);
		}

		void SC_EMAIL_DELETE_EMAILS(Any* p0, Any p1) {
			Invoker::Invoke<void, 2830>(p0, p1);
		}

		void SC_EMAIL_MESSAGE_PUSH_GAMER_TO_RECIP_LIST(Any* gamerHandle) {
			Invoker::Invoke<void, 2831>(gamerHandle);
		}

		void SC_EMAIL_MESSAGE_CLEAR_RECIP_LIST() {
			Invoker::Invoke<void, 2832>();
		}

		void SC_EMAIL_SEND_EMAIL(const char* p0) {
			Invoker::Invoke<void, 2833>(p0);
		}

		BOOL SC_EMAIL_SET_CURRENT_EMAIL_TAG(Any p0) {
			return Invoker::Invoke<BOOL, 2834>(p0);
		}

		void SC_CACHE_NEW_ROCKSTAR_MSGS(BOOL toggle) {
			Invoker::Invoke<void, 2835>(toggle);
		}

		BOOL SC_HAS_NEW_ROCKSTAR_MSG() {
			return Invoker::Invoke<BOOL, 2836>();
		}

		const char* SC_GET_NEW_ROCKSTAR_MSG() {
			return Invoker::Invoke<const char*, 2837>();
		}

		BOOL SC_PRESENCE_ATTR_SET_INT(Hash attrHash, int value) {
			return Invoker::Invoke<BOOL, 2838>(attrHash, value);
		}

		BOOL SC_PRESENCE_ATTR_SET_FLOAT(Hash attrHash, float value) {
			return Invoker::Invoke<BOOL, 2839>(attrHash, value);
		}

		BOOL SC_PRESENCE_ATTR_SET_STRING(Hash attrHash, const char* value) {
			return Invoker::Invoke<BOOL, 2840>(attrHash, value);
		}

		BOOL SC_PRESENCE_SET_ACTIVITY_RATING(Any p0, float p1) {
			return Invoker::Invoke<BOOL, 2841>(p0, p1);
		}

		BOOL SC_GAMERDATA_GET_INT(const char* name, int* value) {
			return Invoker::Invoke<BOOL, 2842>(name, value);
		}

		BOOL SC_GAMERDATA_GET_FLOAT(const char* name, float* value) {
			return Invoker::Invoke<BOOL, 2843>(name, value);
		}

		BOOL SC_GAMERDATA_GET_BOOL(const char* name) {
			return Invoker::Invoke<BOOL, 2844>(name);
		}

		BOOL SC_GAMERDATA_GET_STRING(const char* name, char* value) {
			return Invoker::Invoke<BOOL, 2845>(name, value);
		}

		BOOL SC_GAMERDATA_GET_ACTIVE_XP_BONUS(float* value) {
			return Invoker::Invoke<BOOL, 2846>(value);
		}

		BOOL SC_PROFANITY_CHECK_STRING(const char* string, int* token) {
			return Invoker::Invoke<BOOL, 2847>(string, token);
		}

		BOOL SC_PROFANITY_CHECK_STRING_UGC(const char* string, int* token) {
			return Invoker::Invoke<BOOL, 2848>(string, token);
		}

		BOOL SC_PROFANITY_GET_CHECK_IS_VALID(int token) {
			return Invoker::Invoke<BOOL, 2849>(token);
		}

		BOOL SC_PROFANITY_GET_CHECK_IS_PENDING(int token) {
			return Invoker::Invoke<BOOL, 2850>(token);
		}

		BOOL SC_PROFANITY_GET_STRING_PASSED(int token) {
			return Invoker::Invoke<BOOL, 2851>(token);
		}

		int SC_PROFANITY_GET_STRING_STATUS(int token) {
			return Invoker::Invoke<int, 2852>(token);
		}

		BOOL SC_PROFANITY_GET_PROFANE_WORD(int token, char* outProfaneWord) {
			return Invoker::Invoke<BOOL, 6589>(token, outProfaneWord);
		}

		BOOL SC_LICENSEPLATE_CHECK_STRING(const char* p0, int* p1) {
			return Invoker::Invoke<BOOL, 2853>(p0, p1);
		}

		BOOL SC_LICENSEPLATE_GET_CHECK_IS_VALID(Any p0) {
			return Invoker::Invoke<BOOL, 2854>(p0);
		}

		BOOL SC_LICENSEPLATE_GET_CHECK_IS_PENDING(Any p0) {
			return Invoker::Invoke<BOOL, 2855>(p0);
		}

		int SC_LICENSEPLATE_GET_COUNT(int token) {
			return Invoker::Invoke<int, 2856>(token);
		}

		const char* SC_LICENSEPLATE_GET_PLATE(int token, int plateIndex) {
			return Invoker::Invoke<const char*, 2857>(token, plateIndex);
		}

		const char* SC_LICENSEPLATE_GET_PLATE_DATA(int token, int plateIndex) {
			return Invoker::Invoke<const char*, 2858>(token, plateIndex);
		}

		BOOL SC_LICENSEPLATE_SET_PLATE_DATA(const char* oldPlateText, const char* newPlateText, Any* plateData) {
			return Invoker::Invoke<BOOL, 2859>(oldPlateText, newPlateText, plateData);
		}

		BOOL SC_LICENSEPLATE_ADD(const char* plateText, Any* plateData, int* token) {
			return Invoker::Invoke<BOOL, 2860>(plateText, plateData, token);
		}

		BOOL SC_LICENSEPLATE_GET_ADD_IS_PENDING(int token) {
			return Invoker::Invoke<BOOL, 2861>(token);
		}

		int SC_LICENSEPLATE_GET_ADD_STATUS(int token) {
			return Invoker::Invoke<int, 2862>(token);
		}

		BOOL SC_LICENSEPLATE_ISVALID(const char* plateText, int* token) {
			return Invoker::Invoke<BOOL, 2863>(plateText, token);
		}

		BOOL SC_LICENSEPLATE_GET_ISVALID_IS_PENDING(int token) {
			return Invoker::Invoke<BOOL, 2864>(token);
		}

		int SC_LICENSEPLATE_GET_ISVALID_STATUS(int token) {
			return Invoker::Invoke<int, 2865>(token);
		}

		BOOL SC_COMMUNITY_EVENT_IS_ACTIVE() {
			return Invoker::Invoke<BOOL, 2866>();
		}

		int SC_COMMUNITY_EVENT_GET_EVENT_ID() {
			return Invoker::Invoke<int, 2867>();
		}

		BOOL SC_COMMUNITY_EVENT_GET_EXTRA_DATA_INT(const char* p0, int* p1) {
			return Invoker::Invoke<BOOL, 2868>(p0, p1);
		}

		BOOL SC_COMMUNITY_EVENT_GET_EXTRA_DATA_FLOAT(const char* p0, float* p1) {
			return Invoker::Invoke<BOOL, 2869>(p0, p1);
		}

		BOOL SC_COMMUNITY_EVENT_GET_EXTRA_DATA_STRING(const char* p0, char* p1) {
			return Invoker::Invoke<BOOL, 2870>(p0, p1);
		}

		BOOL SC_COMMUNITY_EVENT_GET_DISPLAY_NAME(char* p0) {
			return Invoker::Invoke<BOOL, 2871>(p0);
		}

		BOOL SC_COMMUNITY_EVENT_IS_ACTIVE_FOR_TYPE(const char* p0) {
			return Invoker::Invoke<BOOL, 2872>(p0);
		}

		int SC_COMMUNITY_EVENT_GET_EVENT_ID_FOR_TYPE(const char* p0) {
			return Invoker::Invoke<int, 2873>(p0);
		}

		BOOL SC_COMMUNITY_EVENT_GET_EXTRA_DATA_INT_FOR_TYPE(const char* p0, int* p1, const char* p2) {
			return Invoker::Invoke<BOOL, 2874>(p0, p1, p2);
		}

		BOOL SC_COMMUNITY_EVENT_GET_EXTRA_DATA_FLOAT_FOR_TYPE(const char* p0, float* p1, const char* p2) {
			return Invoker::Invoke<BOOL, 2875>(p0, p1, p2);
		}

		BOOL SC_COMMUNITY_EVENT_GET_EXTRA_DATA_STRING_FOR_TYPE(const char* p0, char* p1, const char* p2) {
			return Invoker::Invoke<BOOL, 2876>(p0, p1, p2);
		}

		BOOL SC_COMMUNITY_EVENT_GET_DISPLAY_NAME_FOR_TYPE(char* p0, const char* p1) {
			return Invoker::Invoke<BOOL, 2877>(p0, p1);
		}

		BOOL SC_COMMUNITY_EVENT_IS_ACTIVE_BY_ID(int p0) {
			return Invoker::Invoke<BOOL, 2878>(p0);
		}

		BOOL SC_COMMUNITY_EVENT_GET_EXTRA_DATA_INT_BY_ID(int p0, const char* p1, int* p2) {
			return Invoker::Invoke<BOOL, 2879>(p0, p1, p2);
		}

		BOOL SC_COMMUNITY_EVENT_GET_EXTRA_DATA_FLOAT_BY_ID(int p0, const char* p1, float* p2) {
			return Invoker::Invoke<BOOL, 2880>(p0, p1, p2);
		}

		BOOL SC_COMMUNITY_EVENT_GET_EXTRA_DATA_STRING_BY_ID(int p0, const char* p1, char* p2) {
			return Invoker::Invoke<BOOL, 2881>(p0, p1, p2);
		}

		BOOL SC_COMMUNITY_EVENT_GET_DISPLAY_NAME_BY_ID(int p0, char* p1) {
			return Invoker::Invoke<BOOL, 2882>(p0, p1);
		}

		BOOL SC_TRANSITION_NEWS_SHOW(Any p0) {
			return Invoker::Invoke<BOOL, 2883>(p0);
		}

		BOOL SC_TRANSITION_NEWS_SHOW_TIMED(Any p0, Any p1) {
			return Invoker::Invoke<BOOL, 2884>(p0, p1);
		}

		BOOL SC_TRANSITION_NEWS_SHOW_NEXT_ITEM() {
			return Invoker::Invoke<BOOL, 2885>();
		}

		BOOL SC_TRANSITION_NEWS_HAS_EXTRA_DATA_TU() {
			return Invoker::Invoke<BOOL, 2886>();
		}

		BOOL SC_TRANSITION_NEWS_GET_EXTRA_DATA_INT_TU(const char* p0, int* p1) {
			return Invoker::Invoke<BOOL, 2887>(p0, p1);
		}

		void SC_TRANSITION_NEWS_END() {
			Invoker::Invoke<void, 2888>();
		}

		BOOL SC_PAUSE_NEWS_INIT_STORY_TYPE(int storyType, BOOL textOnly) {
			return Invoker::Invoke<BOOL, 6615>(storyType, textOnly);
		}

		BOOL SC_PAUSE_NEWS_GET_PENDING_STORY(Any p0) {
			return Invoker::Invoke<BOOL, 2890>(p0);
		}

		void SC_PAUSE_NEWS_SHUTDOWN() {
			Invoker::Invoke<void, 2891>();
		}

		const char* SC_ACCOUNT_INFO_GET_NICKNAME() {
			return Invoker::Invoke<const char*, 2892>();
		}

		BOOL SC_ACHIEVEMENT_INFO_STATUS(int* p0) {
			return Invoker::Invoke<BOOL, 2893>(p0);
		}

		BOOL SC_HAS_ACHIEVEMENT_BEEN_PASSED(int achievementId) {
			return Invoker::Invoke<BOOL, 2894>(achievementId);
		}

	} // namespace SOCIALCLUB

	namespace STATS {

		BOOL STAT_CLEAR_SLOT_FOR_RELOAD(int statSlot) {
			return Invoker::Invoke<BOOL, 3388>(statSlot);
		}

		BOOL STAT_LOAD(int statSlot) {
			return Invoker::Invoke<BOOL, 3389>(statSlot);
		}

		BOOL STAT_SAVE(int p0, BOOL p1, int p2, BOOL p3) {
			return Invoker::Invoke<BOOL, 3390>(p0, p1, p2, p3);
		}

		void STAT_SET_OPEN_SAVETYPE_IN_JOB(int p0) {
			Invoker::Invoke<void, 3391>(p0);
		}

		BOOL STAT_LOAD_PENDING(int statSlot) {
			return Invoker::Invoke<BOOL, 3392>(statSlot);
		}

		BOOL STAT_SAVE_PENDING() {
			return Invoker::Invoke<BOOL, 3393>();
		}

		BOOL STAT_SAVE_PENDING_OR_REQUESTED() {
			return Invoker::Invoke<BOOL, 3394>();
		}

		BOOL STAT_DELETE_SLOT(int p0) {
			return Invoker::Invoke<BOOL, 3395>(p0);
		}

		BOOL STAT_SLOT_IS_LOADED(int statSlot) {
			return Invoker::Invoke<BOOL, 3396>(statSlot);
		}

		BOOL STAT_CLOUD_SLOT_LOAD_FAILED(int p0) {
			return Invoker::Invoke<BOOL, 3397>(p0);
		}

		int STAT_CLOUD_SLOT_LOAD_FAILED_CODE(Any p0) {
			return Invoker::Invoke<int, 3398>(p0);
		}

		void STAT_SET_BLOCK_SAVES(BOOL toggle) {
			Invoker::Invoke<void, 3399>(toggle);
		}

		BOOL STAT_GET_BLOCK_SAVES() {
			return Invoker::Invoke<BOOL, 3400>();
		}

		BOOL STAT_CLOUD_SLOT_SAVE_FAILED(Any p0) {
			return Invoker::Invoke<BOOL, 3401>(p0);
		}

		void STAT_CLEAR_PENDING_SAVES(Any p0) {
			Invoker::Invoke<void, 3402>(p0);
		}

		BOOL STAT_LOAD_DIRTY_READ_DETECTED() {
			return Invoker::Invoke<BOOL, 3403>();
		}

		void STAT_CLEAR_DIRTY_READ_DETECTED() {
			Invoker::Invoke<void, 3404>();
		}

		BOOL STAT_GET_LOAD_SAFE_TO_PROGRESS_TO_MP_FROM_SP() {
			return Invoker::Invoke<BOOL, 3405>();
		}

		Hash _GET_STAT_HASH_FOR_CHARACTER_STAT(int dataType, int statIndex, int charSlot) {
			return Invoker::Invoke<Hash, 3406>(dataType, statIndex, charSlot);
		}

		BOOL STAT_SET_INT(Hash statName, int value, BOOL save) {
			return Invoker::Invoke<BOOL, 3407>(statName, value, save);
		}

		BOOL STAT_SET_FLOAT(Hash statName, float value, BOOL save) {
			return Invoker::Invoke<BOOL, 3408>(statName, value, save);
		}

		BOOL STAT_SET_BOOL(Hash statName, BOOL value, BOOL save) {
			return Invoker::Invoke<BOOL, 3409>(statName, value, save);
		}

		BOOL STAT_SET_GXT_LABEL(Hash statName, const char* value, BOOL save) {
			return Invoker::Invoke<BOOL, 3410>(statName, value, save);
		}

		BOOL STAT_SET_DATE(Hash statName, Any* value, int numFields, BOOL save) {
			return Invoker::Invoke<BOOL, 3411>(statName, value, numFields, save);
		}

		BOOL STAT_SET_STRING(Hash statName, const char* value, BOOL save) {
			return Invoker::Invoke<BOOL, 3412>(statName, value, save);
		}

		BOOL STAT_SET_POS(Hash statName, float x, float y, float z, BOOL save) {
			return Invoker::Invoke<BOOL, 3413>(statName, x, y, z, save);
		}

		BOOL STAT_SET_MASKED_INT(Hash statName, int p1, int p2, int p3, BOOL save) {
			return Invoker::Invoke<BOOL, 3414>(statName, p1, p2, p3, save);
		}

		BOOL STAT_SET_USER_ID(Hash statName, const char* value, BOOL save) {
			return Invoker::Invoke<BOOL, 3415>(statName, value, save);
		}

		BOOL STAT_SET_CURRENT_POSIX_TIME(Hash statName, BOOL p1) {
			return Invoker::Invoke<BOOL, 3416>(statName, p1);
		}

		BOOL STAT_GET_INT(Hash statHash, int* outValue, int p2) {
			return Invoker::Invoke<BOOL, 3417>(statHash, outValue, p2);
		}

		BOOL STAT_GET_FLOAT(Hash statHash, float* outValue, Any p2) {
			return Invoker::Invoke<BOOL, 3418>(statHash, outValue, p2);
		}

		BOOL STAT_GET_BOOL(Hash statHash, BOOL* outValue, Any p2) {
			return Invoker::Invoke<BOOL, 3419>(statHash, outValue, p2);
		}

		BOOL STAT_GET_DATE(Hash statHash, Any* outValue, int numFields, Any p3) {
			return Invoker::Invoke<BOOL, 3420>(statHash, outValue, numFields, p3);
		}

		const char* STAT_GET_STRING(Hash statHash, int p1) {
			return Invoker::Invoke<const char*, 3421>(statHash, p1);
		}

		BOOL STAT_GET_POS(Hash statName, float* outX, float* outY, float* outZ, Any p4) {
			return Invoker::Invoke<BOOL, 3422>(statName, outX, outY, outZ, p4);
		}

		BOOL STAT_GET_MASKED_INT(Hash statHash, int* outValue, int p2, int p3, Any p4) {
			return Invoker::Invoke<BOOL, 3423>(statHash, outValue, p2, p3, p4);
		}

		const char* STAT_GET_USER_ID(Hash statHash) {
			return Invoker::Invoke<const char*, 3424>(statHash);
		}

		const char* STAT_GET_LICENSE_PLATE(Hash statName) {
			return Invoker::Invoke<const char*, 3425>(statName);
		}

		BOOL STAT_SET_LICENSE_PLATE(Hash statName, const char* str) {
			return Invoker::Invoke<BOOL, 3426>(statName, str);
		}

		void STAT_INCREMENT(Hash statName, float value) {
			Invoker::Invoke<void, 3427>(statName, value);
		}

		BOOL STAT_COMMUNITY_START_SYNCH() {
			return Invoker::Invoke<BOOL, 3428>();
		}

		BOOL STAT_COMMUNITY_SYNCH_IS_PENDING() {
			return Invoker::Invoke<BOOL, 3429>();
		}

		BOOL STAT_COMMUNITY_GET_HISTORY(Hash statName, int p1, float* outValue) {
			return Invoker::Invoke<BOOL, 3430>(statName, p1, outValue);
		}

		void STAT_RESET_ALL_ONLINE_CHARACTER_STATS(int p0) {
			Invoker::Invoke<void, 3431>(p0);
		}

		void STAT_LOCAL_RESET_ALL_ONLINE_CHARACTER_STATS(int p0) {
			Invoker::Invoke<void, 3432>(p0);
		}

		int STAT_GET_NUMBER_OF_DAYS(Hash statName) {
			return Invoker::Invoke<int, 3433>(statName);
		}

		int STAT_GET_NUMBER_OF_HOURS(Hash statName) {
			return Invoker::Invoke<int, 3434>(statName);
		}

		int STAT_GET_NUMBER_OF_MINUTES(Hash statName) {
			return Invoker::Invoke<int, 3435>(statName);
		}

		int STAT_GET_NUMBER_OF_SECONDS(Hash statName) {
			return Invoker::Invoke<int, 3436>(statName);
		}

		void STAT_SET_PROFILE_SETTING_VALUE(int profileSetting, int value) {
			Invoker::Invoke<void, 3437>(profileSetting, value);
		}

		void STATS_CHARACTER_CREATION_OUTFIT_SELECTED(int outfit) {
			Invoker::Invoke<void, 6614>(outfit);
		}

		void STATS_COMPLETED_CHARACTER_CREATION(Any p0) {
			Invoker::Invoke<void, 3438>(p0);
		}

		int PACKED_STAT_GET_INT_STAT_INDEX(int p0) {
			return Invoker::Invoke<int, 3439>(p0);
		}

		Hash GET_PACKED_INT_STAT_KEY(int index, BOOL spStat, BOOL charStat, int character) {
			return Invoker::Invoke<Hash, 3440>(index, spStat, charStat, character);
		}

		Hash GET_PACKED_TU_INT_STAT_KEY(int index, BOOL spStat, BOOL charStat, int character) {
			return Invoker::Invoke<Hash, 3441>(index, spStat, charStat, character);
		}

		Hash GET_PACKED_NG_INT_STAT_KEY(int index, BOOL spStat, BOOL charStat, int character, const char* section) {
			return Invoker::Invoke<Hash, 3442>(index, spStat, charStat, character, section);
		}

		BOOL GET_PACKED_STAT_BOOL_CODE(int index, int characterSlot) {
			return Invoker::Invoke<BOOL, 3443>(index, characterSlot);
		}

		int GET_PACKED_STAT_INT_CODE(int index, int characterSlot) {
			return Invoker::Invoke<int, 3444>(index, characterSlot);
		}

		void SET_PACKED_STAT_BOOL_CODE(int index, BOOL value, int characterSlot) {
			Invoker::Invoke<void, 3445>(index, value, characterSlot);
		}

		void SET_PACKED_STAT_INT_CODE(int index, int value, int characterSlot) {
			Invoker::Invoke<void, 3446>(index, value, characterSlot);
		}

		void PLAYSTATS_BACKGROUND_SCRIPT_ACTION(const char* action, int value) {
			Invoker::Invoke<void, 3447>(action, value);
		}

		void _PLAYSTATS_FLOW_LOW(float posX, float posY, float posZ, const char* action, BOOL p4, int p5) {
			Invoker::Invoke<void, 3448>(posX, posY, posZ, action, p4, p5);
		}

		void _PLAYSTATS_FLOW_MEDIUM(float posX, float posY, float posZ, const char* action, BOOL p4, int p5) {
			Invoker::Invoke<void, 3449>(posX, posY, posZ, action, p4, p5);
		}

		void _PLAYSTATS_FLOW_HIGH(float posX, float posY, float posZ, const char* action, BOOL p4, int p5) {
			Invoker::Invoke<void, 6613>(posX, posY, posZ, action, p4, p5);
		}

		void PLAYSTATS_NPC_INVITE(const char* p0) {
			Invoker::Invoke<void, 3450>(p0);
		}

		void PLAYSTATS_AWARD_XP(int amount, Hash type, Hash category) {
			Invoker::Invoke<void, 3451>(amount, type, category);
		}

		void PLAYSTATS_RANK_UP(int rank) {
			Invoker::Invoke<void, 3452>(rank);
		}

		void PLAYSTATS_STARTED_SESSION_IN_OFFLINEMODE() {
			Invoker::Invoke<void, 3453>();
		}

		void PLAYSTATS_ACTIVITY_DONE(int p0, int activityId, Any p2) {
			Invoker::Invoke<void, 3454>(p0, activityId, p2);
		}

		void PLAYSTATS_LEAVE_JOB_CHAIN(Any p0, Any p1, Any p2, Any p3, Any p4) {
			Invoker::Invoke<void, 3455>(p0, p1, p2, p3, p4);
		}

		void PLAYSTATS_MISSION_STARTED(const char* p0, Any p1, Any p2, BOOL p3) {
			Invoker::Invoke<void, 3456>(p0, p1, p2, p3);
		}

		void PLAYSTATS_MISSION_OVER(const char* p0, Any p1, Any p2, BOOL p3, BOOL p4, BOOL p5) {
			Invoker::Invoke<void, 3457>(p0, p1, p2, p3, p4, p5);
		}

		void PLAYSTATS_MISSION_CHECKPOINT(const char* p0, Any p1, Any p2, Any p3) {
			Invoker::Invoke<void, 3458>(p0, p1, p2, p3);
		}

		void PLAYSTATS_RANDOM_MISSION_DONE(const char* name, Any p1, Any p2, Any p3) {
			Invoker::Invoke<void, 3459>(name, p1, p2, p3);
		}

		void PLAYSTATS_ROS_BET(int amount, int act, Player player, float cm) {
			Invoker::Invoke<void, 3460>(amount, act, player, cm);
		}

		void PLAYSTATS_RACE_CHECKPOINT(Vehicle p0, Any p1, int p2, int p3, Any p4) {
			Invoker::Invoke<void, 3461>(p0, p1, p2, p3, p4);
		}

		BOOL PLAYSTATS_CREATE_MATCH_HISTORY_ID_2(int* playerAccountId, int* posixTime) {
			return Invoker::Invoke<BOOL, 3462>(playerAccountId, posixTime);
		}

		void PLAYSTATS_MATCH_STARTED(Any p0, Any p1, Any p2) {
			Invoker::Invoke<void, 3463>(p0, p1, p2);
		}

		void PLAYSTATS_SHOP_ITEM(Any p0, Any p1, Any p2, Any p3, Any p4) {
			Invoker::Invoke<void, 3464>(p0, p1, p2, p3, p4);
		}

		void PLAYSTATS_CRATE_DROP_MISSION_DONE(Any p0, Any p1, Any p2, Any p3, Any p4, Any p5, Any p6, Any p7) {
			Invoker::Invoke<void, 3465>(p0, p1, p2, p3, p4, p5, p6, p7);
		}

		void PLAYSTATS_CRATE_CREATED(float p0, float p1, float p2) {
			Invoker::Invoke<void, 3466>(p0, p1, p2);
		}

		void PLAYSTATS_HOLD_UP_MISSION_DONE(Any p0, Any p1, Any p2, Any p3) {
			Invoker::Invoke<void, 3467>(p0, p1, p2, p3);
		}

		void PLAYSTATS_IMPORT_EXPORT_MISSION_DONE(Any p0, Any p1, Any p2, Any p3) {
			Invoker::Invoke<void, 3468>(p0, p1, p2, p3);
		}

		void PLAYSTATS_RACE_TO_POINT_MISSION_DONE(int p0, Any p1, Any p2, Any p3) {
			Invoker::Invoke<void, 3469>(p0, p1, p2, p3);
		}

		void PLAYSTATS_ACQUIRED_HIDDEN_PACKAGE(Any p0) {
			Invoker::Invoke<void, 3470>(p0);
		}

		void PLAYSTATS_WEBSITE_VISITED(Hash scaleformHash, int p1) {
			Invoker::Invoke<void, 3471>(scaleformHash, p1);
		}

		void PLAYSTATS_FRIEND_ACTIVITY(int p0, BOOL p1) {
			Invoker::Invoke<void, 3472>(p0, p1);
		}

		void PLAYSTATS_ODDJOB_DONE(int totalTimeMs, int p1, BOOL p2) {
			Invoker::Invoke<void, 3473>(totalTimeMs, p1, p2);
		}

		void PLAYSTATS_PROP_CHANGE(Ped p0, int p1, int p2, int p3) {
			Invoker::Invoke<void, 3474>(p0, p1, p2, p3);
		}

		void PLAYSTATS_CLOTH_CHANGE(Ped p0, Any p1, Any p2, Any p3, Any p4) {
			Invoker::Invoke<void, 3475>(p0, p1, p2, p3, p4);
		}

		void PLAYSTATS_WEAPON_MODE_CHANGE(Hash weaponHash, Hash componentHashTo, Hash componentHashFrom) {
			Invoker::Invoke<void, 3476>(weaponHash, componentHashTo, componentHashFrom);
		}

		void PLAYSTATS_CHEAT_APPLIED(const char* cheat) {
			Invoker::Invoke<void, 3477>(cheat);
		}

		void PLAYSTATS_JOB_ACTIVITY_END(Any* p0, Any* p1, Any* p2, Any* p3) {
			Invoker::Invoke<void, 3478>(p0, p1, p2, p3);
		}

		void PLAYSTATS_JOB_BEND(Any* p0, Any* p1, Any* p2, Any* p3) {
			Invoker::Invoke<void, 3479>(p0, p1, p2, p3);
		}

		void PLAYSTATS_JOB_LTS_END(Any* p0, Any* p1, Any* p2, Any* p3) {
			Invoker::Invoke<void, 3480>(p0, p1, p2, p3);
		}

		void PLAYSTATS_JOB_LTS_ROUND_END(Any* p0, Any* p1, Any* p2, Any* p3) {
			Invoker::Invoke<void, 3481>(p0, p1, p2, p3);
		}

		void PLAYSTATS_QUICKFIX_TOOL(int element, const char* item) {
			Invoker::Invoke<void, 3482>(element, item);
		}

		void PLAYSTATS_IDLE_KICK(int msStoodIdle) {
			Invoker::Invoke<void, 3483>(msStoodIdle);
		}

		void PLAYSTATS_SET_JOIN_TYPE(int joinType) {
			Invoker::Invoke<void, 3484>(joinType);
		}

		void PLAYSTATS_HEIST_SAVE_CHEAT(Hash hash, int p1) {
			Invoker::Invoke<void, 3485>(hash, p1);
		}

		void PLAYSTATS_APPEND_DIRECTOR_METRIC(Any* p0) {
			Invoker::Invoke<void, 3486>(p0);
		}

		void PLAYSTATS_AWARD_BAD_SPORT(int id) {
			Invoker::Invoke<void, 3487>(id);
		}

		void PLAYSTATS_PEGASUS_AS_PERSONAL_AIRCRAFT(Hash modelHash) {
			Invoker::Invoke<void, 3488>(modelHash);
		}

		void _PLAYSTATS_SHOPMENU_NAV(Any p0, Any p1, Any p2, Any p3) {
			Invoker::Invoke<void, 3489>(p0, p1, p2, p3);
		}

		void PLAYSTATS_FM_EVENT_CHALLENGES(Any p0) {
			Invoker::Invoke<void, 3490>(p0);
		}

		void PLAYSTATS_FM_EVENT_VEHICLETARGET(Any p0) {
			Invoker::Invoke<void, 3491>(p0);
		}

		void PLAYSTATS_FM_EVENT_URBANWARFARE(Any p0) {
			Invoker::Invoke<void, 3492>(p0);
		}

		void PLAYSTATS_FM_EVENT_CHECKPOINTCOLLECTION(Any p0) {
			Invoker::Invoke<void, 3493>(p0);
		}

		void PLAYSTATS_FM_EVENT_ATOB(Any p0) {
			Invoker::Invoke<void, 3494>(p0);
		}

		void PLAYSTATS_FM_EVENT_PENNEDIN(Any p0) {
			Invoker::Invoke<void, 3495>(p0);
		}

		void PLAYSTATS_FM_EVENT_PASSTHEPARCEL(Any p0) {
			Invoker::Invoke<void, 3496>(p0);
		}

		void PLAYSTATS_FM_EVENT_HOTPROPERTY(Any p0) {
			Invoker::Invoke<void, 3497>(p0);
		}

		void PLAYSTATS_FM_EVENT_DEADDROP(Any p0) {
			Invoker::Invoke<void, 3498>(p0);
		}

		void PLAYSTATS_FM_EVENT_KINGOFTHECASTLE(Any p0) {
			Invoker::Invoke<void, 3499>(p0);
		}

		void PLAYSTATS_FM_EVENT_CRIMINALDAMAGE(Any p0) {
			Invoker::Invoke<void, 3500>(p0);
		}

		void PLAYSTATS_FM_EVENT_COMPETITIVEURBANWARFARE(Any p0) {
			Invoker::Invoke<void, 3501>(p0);
		}

		void PLAYSTATS_FM_EVENT_HUNTBEAST(Any p0) {
			Invoker::Invoke<void, 3502>(p0);
		}

		void PLAYSTATS_PIMENU_HIDE_OPTIONS(Any* data) {
			Invoker::Invoke<void, 3503>(data);
		}

		void _PLAYSTATS_NAMED_USER_CONTENT(BOOL isBoss, int bossType, int bossId1, int bossId2, int textType, const char* textString, int textSource) {
			Invoker::Invoke<void, 6590>(isBoss, bossType, bossId1, bossId2, textType, textString, textSource);
		}

		int LEADERBOARDS_GET_NUMBER_OF_COLUMNS(int p0, Any p1) {
			return Invoker::Invoke<int, 3504>(p0, p1);
		}

		int LEADERBOARDS_GET_COLUMN_ID(int p0, int p1, int p2) {
			return Invoker::Invoke<int, 3505>(p0, p1, p2);
		}

		int LEADERBOARDS_GET_COLUMN_TYPE(int p0, Any p1, Any p2) {
			return Invoker::Invoke<int, 3506>(p0, p1, p2);
		}

		int LEADERBOARDS_READ_CLEAR_ALL() {
			return Invoker::Invoke<int, 3507>();
		}

		int LEADERBOARDS_READ_CLEAR(Any p0, Any p1, Any p2) {
			return Invoker::Invoke<int, 3508>(p0, p1, p2);
		}

		BOOL LEADERBOARDS_READ_PENDING(Any p0, Any p1, Any p2) {
			return Invoker::Invoke<BOOL, 3509>(p0, p1, p2);
		}

		BOOL LEADERBOARDS_READ_ANY_PENDING() {
			return Invoker::Invoke<BOOL, 3510>();
		}

		BOOL LEADERBOARDS_READ_SUCCESSFUL(Any p0, Any p1, Any p2) {
			return Invoker::Invoke<BOOL, 3511>(p0, p1, p2);
		}

		BOOL LEADERBOARDS2_READ_FRIENDS_BY_ROW(Any* p0, Any* p1, Any p2, BOOL p3, Any p4, Any p5) {
			return Invoker::Invoke<BOOL, 3512>(p0, p1, p2, p3, p4, p5);
		}

		BOOL LEADERBOARDS2_READ_BY_HANDLE(Any* p0, Any* p1) {
			return Invoker::Invoke<BOOL, 3513>(p0, p1);
		}

		BOOL LEADERBOARDS2_READ_BY_RANK(Any* p0, Any p1, Any p2) {
			return Invoker::Invoke<BOOL, 3514>(p0, p1, p2);
		}

		BOOL LEADERBOARDS2_READ_BY_RADIUS(Any* p0, Any p1, Any* p2) {
			return Invoker::Invoke<BOOL, 3515>(p0, p1, p2);
		}

		BOOL LEADERBOARDS2_READ_BY_SCORE_INT(Any* p0, Any p1, Any p2) {
			return Invoker::Invoke<BOOL, 3516>(p0, p1, p2);
		}

		BOOL LEADERBOARDS2_READ_BY_SCORE_FLOAT(Any* p0, float p1, Any p2) {
			return Invoker::Invoke<BOOL, 3517>(p0, p1, p2);
		}

		BOOL LEADERBOARDS2_READ_RANK_PREDICTION(Any* p0, Any* p1, Any* p2) {
			return Invoker::Invoke<BOOL, 3518>(p0, p1, p2);
		}

		BOOL LEADERBOARDS2_READ_BY_PLAFORM(Any* p0, const char* gamerHandleCsv, const char* platformName) {
			return Invoker::Invoke<BOOL, 3519>(p0, gamerHandleCsv, platformName);
		}

		BOOL LEADERBOARDS2_READ_GET_ROW_DATA_START(Any* p0) {
			return Invoker::Invoke<BOOL, 3520>(p0);
		}

		void LEADERBOARDS2_READ_GET_ROW_DATA_END() {
			Invoker::Invoke<void, 3521>();
		}

		BOOL LEADERBOARDS2_READ_GET_ROW_DATA_INFO(Any p0, Any* p1) {
			return Invoker::Invoke<BOOL, 3522>(p0, p1);
		}

		int LEADERBOARDS2_READ_GET_ROW_DATA_INT(Any p0, Any p1) {
			return Invoker::Invoke<int, 3523>(p0, p1);
		}

		float LEADERBOARDS2_READ_GET_ROW_DATA_FLOAT(Any p0, Any p1) {
			return Invoker::Invoke<float, 3524>(p0, p1);
		}

		BOOL LEADERBOARDS2_WRITE_DATA(Any* p0) {
			return Invoker::Invoke<BOOL, 3525>(p0);
		}

		void LEADERBOARDS_WRITE_ADD_COLUMN(Any p0, Any p1, float p2) {
			Invoker::Invoke<void, 3526>(p0, p1, p2);
		}

		void LEADERBOARDS_WRITE_ADD_COLUMN_LONG(Any p0, Any p1, Any p2) {
			Invoker::Invoke<void, 3527>(p0, p1, p2);
		}

		BOOL LEADERBOARDS_CACHE_DATA_ROW(Any* p0) {
			return Invoker::Invoke<BOOL, 3528>(p0);
		}

		void LEADERBOARDS_CLEAR_CACHE_DATA() {
			Invoker::Invoke<void, 3529>();
		}

		void LEADERBOARDS_CLEAR_CACHE_DATA_ID(Any p0) {
			Invoker::Invoke<void, 3530>(p0);
		}

		BOOL LEADERBOARDS_GET_CACHE_EXISTS(Any p0) {
			return Invoker::Invoke<BOOL, 3531>(p0);
		}

		int LEADERBOARDS_GET_CACHE_TIME(Any p0) {
			return Invoker::Invoke<int, 3532>(p0);
		}

		int LEADERBOARDS_GET_CACHE_NUMBER_OF_ROWS(Any p0) {
			return Invoker::Invoke<int, 3533>(p0);
		}

		BOOL LEADERBOARDS_GET_CACHE_DATA_ROW(Any p0, Any p1, Any* p2) {
			return Invoker::Invoke<BOOL, 3534>(p0, p1, p2);
		}

		void PRESENCE_EVENT_UPDATESTAT_INT(Hash statHash, int value, int p2) {
			Invoker::Invoke<void, 3535>(statHash, value, p2);
		}

		void PRESENCE_EVENT_UPDATESTAT_FLOAT(Hash statHash, float value, int p2) {
			Invoker::Invoke<void, 3536>(statHash, value, p2);
		}

		void PRESENCE_EVENT_UPDATESTAT_INT_WITH_STRING(Hash statHash, int value, int p2, const char* string) {
			Invoker::Invoke<void, 3537>(statHash, value, p2, string);
		}

		BOOL GET_PLAYER_HAS_DRIVEN_ALL_VEHICLES() {
			return Invoker::Invoke<BOOL, 3538>();
		}

		void SET_HAS_POSTED_ALL_VEHICLES_DRIVEN() {
			Invoker::Invoke<void, 3539>();
		}

		void SET_PROFILE_SETTING_PROLOGUE_COMPLETE() {
			Invoker::Invoke<void, 3540>();
		}

		void SET_PROFILE_SETTING_SP_CHOP_MISSION_COMPLETE() {
			Invoker::Invoke<void, 3541>();
		}

		void SET_PROFILE_SETTING_CREATOR_RACES_DONE(int value) {
			Invoker::Invoke<void, 3542>(value);
		}

		void SET_PROFILE_SETTING_CREATOR_DM_DONE(int value) {
			Invoker::Invoke<void, 3543>(value);
		}

		void SET_PROFILE_SETTING_CREATOR_CTF_DONE(int value) {
			Invoker::Invoke<void, 3544>(value);
		}

		void SET_JOB_ACTIVITY_ID_STARTED(Any p0, int characterSlot) {
			Invoker::Invoke<void, 3545>(p0, characterSlot);
		}

		void SET_FREEMODE_PROLOGUE_DONE(Any p0, int characterSlot) {
			Invoker::Invoke<void, 3546>(p0, characterSlot);
		}

		void SET_FREEMODE_STRAND_PROGRESSION_STATUS(int profileSetting, int settingValue) {
			Invoker::Invoke<void, 3547>(profileSetting, settingValue);
		}

		void STAT_NETWORK_INCREMENT_ON_SUICIDE(Any p0, float p1) {
			Invoker::Invoke<void, 3548>(p0, p1);
		}

		void STAT_SET_CHEAT_IS_ACTIVE() {
			Invoker::Invoke<void, 3549>();
		}

		BOOL LEADERBOARDS2_WRITE_DATA_FOR_EVENT_TYPE(Any* p0, Any* p1) {
			return Invoker::Invoke<BOOL, 3550>(p0, p1);
		}

		void FORCE_CLOUD_MP_STATS_DOWNLOAD_AND_OVERWRITE_LOCAL_SAVE() {
			Invoker::Invoke<void, 3551>();
		}

		void STAT_MIGRATE_CLEAR_FOR_RESTART() {
			Invoker::Invoke<void, 3552>();
		}

		BOOL STAT_MIGRATE_SAVEGAME_START(const char* platformName) {
			return Invoker::Invoke<BOOL, 3553>(platformName);
		}

		int STAT_MIGRATE_SAVEGAME_GET_STATUS() {
			return Invoker::Invoke<int, 3554>();
		}

		BOOL STAT_MIGRATE_CHECK_ALREADY_DONE() {
			return Invoker::Invoke<BOOL, 3555>();
		}

		BOOL STAT_MIGRATE_CHECK_START() {
			return Invoker::Invoke<BOOL, 3556>();
		}

		int STAT_MIGRATE_CHECK_GET_IS_PLATFORM_AVAILABLE(int p0) {
			return Invoker::Invoke<int, 3557>(p0);
		}

		int STAT_MIGRATE_CHECK_GET_PLATFORM_STATUS(int p0, Any* p1) {
			return Invoker::Invoke<int, 3558>(p0, p1);
		}

		int STAT_GET_SAVE_MIGRATION_STATUS(Any* data) {
			return Invoker::Invoke<int, 3559>(data);
		}

		BOOL STAT_SAVE_MIGRATION_CANCEL_PENDING_OPERATION() {
			return Invoker::Invoke<BOOL, 3560>();
		}

		int STAT_GET_CANCEL_SAVE_MIGRATION_STATUS() {
			return Invoker::Invoke<int, 3561>();
		}

		BOOL STAT_SAVE_MIGRATION_CONSUME_CONTENT(Hash contentId, const char* srcPlatform, const char* srcGamerHandle) {
			return Invoker::Invoke<BOOL, 3562>(contentId, srcPlatform, srcGamerHandle);
		}

		int STAT_GET_SAVE_MIGRATION_CONSUME_CONTENT_STATUS(int* p0) {
			return Invoker::Invoke<int, 3563>(p0);
		}

		void STAT_ENABLE_STATS_TRACKING() {
			Invoker::Invoke<void, 3564>();
		}

		void STAT_DISABLE_STATS_TRACKING() {
			Invoker::Invoke<void, 3565>();
		}

		BOOL STAT_IS_STATS_TRACKING_ENABLED() {
			return Invoker::Invoke<BOOL, 3566>();
		}

		BOOL STAT_START_RECORD_STAT(int statType, int valueType) {
			return Invoker::Invoke<BOOL, 3567>(statType, valueType);
		}

		BOOL STAT_STOP_RECORD_STAT() {
			return Invoker::Invoke<BOOL, 3568>();
		}

		BOOL STAT_GET_RECORDED_VALUE(float* value) {
			return Invoker::Invoke<BOOL, 3569>(value);
		}

		BOOL STAT_IS_RECORDING_STAT() {
			return Invoker::Invoke<BOOL, 3570>();
		}

		int STAT_GET_CURRENT_NEAR_MISS_NOCRASH_PRECISE() {
			return Invoker::Invoke<int, 3571>();
		}

		float STAT_GET_CURRENT_REAR_WHEEL_DISTANCE() {
			return Invoker::Invoke<float, 3572>();
		}

		float STAT_GET_CURRENT_FRONT_WHEEL_DISTANCE() {
			return Invoker::Invoke<float, 3573>();
		}

		float STAT_GET_CURRENT_JUMP_DISTANCE() {
			return Invoker::Invoke<float, 3574>();
		}

		float STAT_GET_CURRENT_DRIVE_NOCRASH_DISTANCE() {
			return Invoker::Invoke<float, 3575>();
		}

		float STAT_GET_CURRENT_SPEED() {
			return Invoker::Invoke<float, 3576>();
		}

		float STAT_GET_CURRENT_DRIVING_REVERSE_DISTANCE() {
			return Invoker::Invoke<float, 3577>();
		}

		float STAT_GET_CURRENT_SKYDIVING_DISTANCE() {
			return Invoker::Invoke<float, 3578>();
		}

		float STAT_GET_CHALLENGE_FLYING_DIST() {
			return Invoker::Invoke<float, 3579>();
		}

		BOOL STAT_GET_FLYING_ALTITUDE(float* outValue) {
			return Invoker::Invoke<BOOL, 3580>(outValue);
		}

		BOOL STAT_IS_PLAYER_VEHICLE_ABOVE_OCEAN() {
			return Invoker::Invoke<BOOL, 3581>();
		}

		float STAT_GET_VEHICLE_BAIL_DISTANCE() {
			return Invoker::Invoke<float, 3582>();
		}

		BOOL STAT_ROLLBACK_SAVE_MIGRATION() {
			return Invoker::Invoke<BOOL, 3583>();
		}

		void SET_HAS_SPECIALEDITION_CONTENT(int value) {
			Invoker::Invoke<void, 3584>(value);
		}

		void SET_SAVE_MIGRATION_TRANSACTION_ID_WARNING(int transactionId) {
			Invoker::Invoke<void, 3585>(transactionId);
		}

		void GET_BOSS_GOON_UUID(int characterSlot, Any p1, Any p2) {
			Invoker::Invoke<void, 3586>(characterSlot, p1, p2);
		}

		void PLAYSTATS_BW_BOSSONBOSSDEATHMATCH(Any p0) {
			Invoker::Invoke<void, 3587>(p0);
		}

		void PLAYSTATS_BW_YATCHATTACK(Any p0) {
			Invoker::Invoke<void, 3588>(p0);
		}

		void PLAYSTATS_BW_HUNT_THE_BOSS(Any p0) {
			Invoker::Invoke<void, 3589>(p0);
		}

		void PLAYSTATS_BW_SIGHTSEER(Any p0) {
			Invoker::Invoke<void, 3590>(p0);
		}

		void PLAYSTATS_BW_ASSAULT(Any p0) {
			Invoker::Invoke<void, 3591>(p0);
		}

		void PLAYSTATS_BW_BELLY_OF_THE_BEAST(Any p0) {
			Invoker::Invoke<void, 3592>(p0);
		}

		void PLAYSTATS_BW_HEAD_HUNTER(Any p0) {
			Invoker::Invoke<void, 3593>(p0);
		}

		void PLAYSTATS_BW_FRAGILE_GOODS(Any p0) {
			Invoker::Invoke<void, 3594>(p0);
		}

		void PLAYSTATS_BW_AIR_FREIGHT(Any p0) {
			Invoker::Invoke<void, 3595>(p0);
		}

		void PLAYSTATS_BC_CAR_JACKING(Any p0) {
			Invoker::Invoke<void, 3596>(p0);
		}

		void PLAYSTATS_BC_SMASH_AND_GRAB(Any p0) {
			Invoker::Invoke<void, 3597>(p0);
		}

		void PLAYSTATS_BC_PROTECTION_RACKET(Any p0) {
			Invoker::Invoke<void, 3598>(p0);
		}

		void PLAYSTATS_BC_MOST_WANTED(Any p0) {
			Invoker::Invoke<void, 3599>(p0);
		}

		void PLAYSTATS_BC_FINDERS_KEEPERS(Any p0) {
			Invoker::Invoke<void, 3600>(p0);
		}

		void PLAYSTATS_BC_POINT_TO_POINT(Any p0) {
			Invoker::Invoke<void, 3601>(p0);
		}

		void PLAYSTATS_BC_CASHING(Any p0) {
			Invoker::Invoke<void, 3602>(p0);
		}

		void PLAYSTATS_BC_SALVAGE(Any p0) {
			Invoker::Invoke<void, 3603>(p0);
		}

		void PLAYSTATS_SPENT_PI_CUSTOM_LOADOUT(int amount) {
			Invoker::Invoke<void, 3604>(amount);
		}

		void PLAYSTATS_BUY_CONTRABAND_MISSION(Any* data) {
			Invoker::Invoke<void, 3605>(data);
		}

		void PLAYSTATS_SELL_CONTRABAND_MISSION(Any* data) {
			Invoker::Invoke<void, 3606>(data);
		}

		void PLAYSTATS_DEFEND_CONTRABAND_MISSION(Any* data) {
			Invoker::Invoke<void, 3607>(data);
		}

		void PLAYSTATS_RECOVER_CONTRABAND_MISSION(Any* data) {
			Invoker::Invoke<void, 3608>(data);
		}

		void PLAYSTATS_HIT_CONTRABAND_DESTROY_LIMIT(Any p0) {
			Invoker::Invoke<void, 3609>(p0);
		}

		void START_BEING_BOSS(Any p0, Any p1, Any p2) {
			Invoker::Invoke<void, 3610>(p0, p1, p2);
		}

		void START_BEING_GOON(Any p0, Any p1, Any p2) {
			Invoker::Invoke<void, 3611>(p0, p1, p2);
		}

		void END_BEING_BOSS(Any p0, Any p1, Any p2) {
			Invoker::Invoke<void, 3612>(p0, p1, p2);
		}

		void END_BEING_GOON(Any p0, Any p1, Any p2, Any p3, Any p4) {
			Invoker::Invoke<void, 3613>(p0, p1, p2, p3, p4);
		}

		void HIRED_LIMO(Any p0, Any p1) {
			Invoker::Invoke<void, 3614>(p0, p1);
		}

		void ORDER_BOSS_VEHICLE(Any p0, Any p1, Hash vehicleHash) {
			Invoker::Invoke<void, 3615>(p0, p1, vehicleHash);
		}

		void CHANGE_UNIFORM(Any p0, Any p1, Any p2) {
			Invoker::Invoke<void, 3616>(p0, p1, p2);
		}

		void CHANGE_GOON_LOOKING_FOR_WORK(Any p0) {
			Invoker::Invoke<void, 3617>(p0);
		}

		void SEND_METRIC_GHOSTING_TO_PLAYER(Any p0) {
			Invoker::Invoke<void, 3618>(p0);
		}

		void SEND_METRIC_VIP_POACH(Any p0, Any p1, Any p2) {
			Invoker::Invoke<void, 3619>(p0, p1, p2);
		}

		void SEND_METRIC_PUNISH_BODYGUARD(Any p0) {
			Invoker::Invoke<void, 3620>(p0);
		}

		void PLAYSTATS_START_TRACKING_STUNTS() {
			Invoker::Invoke<void, 3621>();
		}

		void PLAYSTATS_STOP_TRACKING_STUNTS() {
			Invoker::Invoke<void, 3622>();
		}

		void PLAYSTATS_MISSION_ENDED(Any p0) {
			Invoker::Invoke<void, 3623>(p0);
		}

		void PLAYSTATS_IMPEXP_MISSION_ENDED(Any p0) {
			Invoker::Invoke<void, 3624>(p0);
		}

		void PLAYSTATS_CHANGE_MC_ROLE(Any p0, Any p1, Any p2, Any p3, int role, int p5, Any p6) {
			Invoker::Invoke<void, 3625>(p0, p1, p2, p3, role, p5, p6);
		}

		void PLAYSTATS_CHANGE_MC_OUTFIT(Any p0, Any p1, Any p2, Any p3, Any p4) {
			Invoker::Invoke<void, 3626>(p0, p1, p2, p3, p4);
		}

		void PLAYSTATS_SWITCH_MC_EMBLEM(Any p0, Any p1, Any p2, Any p3, Any p4) {
			Invoker::Invoke<void, 3627>(p0, p1, p2, p3, p4);
		}

		void PLAYSTATS_MC_REQUEST_BIKE(Any p0, Any p1, Any p2, Any p3, Any p4) {
			Invoker::Invoke<void, 3628>(p0, p1, p2, p3, p4);
		}

		void PLAYSTATS_MC_KILLED_RIVAL_MC_MEMBER(Any p0, Any p1, Any p2, Any p3, Any p4) {
			Invoker::Invoke<void, 3629>(p0, p1, p2, p3, p4);
		}

		void PLAYSTATS_ABANDONED_MC(Any p0, Any p1, Any p2, Any p3, Any p4) {
			Invoker::Invoke<void, 3630>(p0, p1, p2, p3, p4);
		}

		void PLAYSTATS_EARNED_MC_POINTS(Any p0, Any p1, Any p2, Any p3, Any p4, Any p5) {
			Invoker::Invoke<void, 3631>(p0, p1, p2, p3, p4, p5);
		}

		void PLAYSTATS_MC_FORMATION_ENDS(Any p0, Any p1, Any p2, Any p3, Any p4, Any p5, Any p6) {
			Invoker::Invoke<void, 3632>(p0, p1, p2, p3, p4, p5, p6);
		}

		void PLAYSTATS_MC_CLUBHOUSE_ACTIVITY(Any p0, Any p1, Any p2, Any p3, Any p4, Any p5, Any p6, Any p7) {
			Invoker::Invoke<void, 3633>(p0, p1, p2, p3, p4, p5, p6, p7);
		}

		void PLAYSTATS_RIVAL_BEHAVIOR(Any p0, Any p1, Any p2, Any p3, Any p4, Any p5, Any p6, Any p7, Any p8, Any p9) {
			Invoker::Invoke<void, 3634>(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9);
		}

		void PLAYSTATS_COPY_RANK_INTO_NEW_SLOT(Any p0, Any p1, Any p2, Any p3, Any p4, Any p5, Any p6) {
			Invoker::Invoke<void, 3635>(p0, p1, p2, p3, p4, p5, p6);
		}

		void PLAYSTATS_DUPE_DETECTED(Any* data) {
			Invoker::Invoke<void, 3636>(data);
		}

		void PLAYSTATS_BAN_ALERT(int p0) {
			Invoker::Invoke<void, 3637>(p0);
		}

		void PLAYSTATS_GUNRUNNING_MISSION_ENDED(Any* data) {
			Invoker::Invoke<void, 3638>(data);
		}

		void PLAYSTATS_GUNRUNNING_RND(Any p0) {
			Invoker::Invoke<void, 3639>(p0);
		}

		void PLAYSTATS_BUSINESS_BATTLE_ENDED(Any p0) {
			Invoker::Invoke<void, 3640>(p0);
		}

		void PLAYSTATS_WAREHOUSE_MISSION_ENDED(Any p0) {
			Invoker::Invoke<void, 3641>(p0);
		}

		void PLAYSTATS_NIGHTCLUB_MISSION_ENDED(Any p0) {
			Invoker::Invoke<void, 3642>(p0);
		}

		void PLAYSTATS_DJ_USAGE(Any p0, Any p1) {
			Invoker::Invoke<void, 3643>(p0, p1);
		}

		void PLAYSTATS_MINIGAME_USAGE(Any p0, Any p1, Any p2) {
			Invoker::Invoke<void, 3644>(p0, p1, p2);
		}

		void PLAYSTATS_STONE_HATCHET_ENDED(Any* data) {
			Invoker::Invoke<void, 3645>(data);
		}

		void PLAYSTATS_SMUGGLER_MISSION_ENDED(Any* data) {
			Invoker::Invoke<void, 3646>(data);
		}

		void PLAYSTATS_FM_HEIST_PREP_ENDED(Any* data) {
			Invoker::Invoke<void, 3647>(data);
		}

		void PLAYSTATS_INSTANCED_HEIST_ENDED(Any* data, Any p1, Any p2, Any p3) {
			Invoker::Invoke<void, 3648>(data, p1, p2, p3);
		}

		void PLAYSTATS_DAR_CHECKPOINT(Any* data) {
			Invoker::Invoke<void, 3649>(data);
		}

		void PLAYSTATS_ENTER_SESSION_PACK(Any* data) {
			Invoker::Invoke<void, 3650>(data);
		}

		void PLAYSTATS_DRONE_USAGE(int p0, int p1, int p2) {
			Invoker::Invoke<void, 3651>(p0, p1, p2);
		}

		void PLAYSTATS_SPIN_WHEEL(int p0, int p1, int p2, int p3) {
			Invoker::Invoke<void, 3652>(p0, p1, p2, p3);
		}

		void PLAYSTATS_ARENA_WARS_SPECTATOR(int p0, int p1, int p2, int p3, int p4) {
			Invoker::Invoke<void, 3653>(p0, p1, p2, p3, p4);
		}

		void PLAYSTATS_ARENA_WARS_ENDED(Any* data) {
			Invoker::Invoke<void, 3654>(data);
		}

		void PLAYSTATS_SWITCH_PASSIVE_MODE(BOOL p0, int p1, int p2, int p3) {
			Invoker::Invoke<void, 3655>(p0, p1, p2, p3);
		}

		void PLAYSTATS_COLLECTIBLE_PICKED_UP(int p0, Hash objectHash, Any p2, Any p3, int moneyAmount, int rpAmount, int chipsAmount, Any p7, int p8, Any p9, Any p10) {
			Invoker::Invoke<void, 3656>(p0, objectHash, p2, p3, moneyAmount, rpAmount, chipsAmount, p7, p8, p9, p10);
		}

		void PLAYSTATS_CASINO_STORY_MISSION_ENDED(Any p0, Any p1) {
			Invoker::Invoke<void, 3657>(p0, p1);
		}

		void PLAYSTATS_CASINO_CHIP(Any p0) {
			Invoker::Invoke<void, 3658>(p0);
		}

		void PLAYSTATS_CASINO_ROULETTE(Any p0) {
			Invoker::Invoke<void, 3659>(p0);
		}

		void PLAYSTATS_CASINO_BLACKJACK(Any p0) {
			Invoker::Invoke<void, 3660>(p0);
		}

		void PLAYSTATS_CASINO_THREE_CARD_POKER(Any p0) {
			Invoker::Invoke<void, 3661>(p0);
		}

		void PLAYSTATS_CASINO_SLOT_MACHINE(Any p0) {
			Invoker::Invoke<void, 3662>(p0);
		}

		void PLAYSTATS_CASINO_INSIDE_TRACK(Any p0) {
			Invoker::Invoke<void, 3663>(p0);
		}

		void PLAYSTATS_CASINO_LUCKY_SEVEN(Any p0) {
			Invoker::Invoke<void, 3664>(p0);
		}

		void PLAYSTATS_CASINO_ROULETTE_LIGHT(Any p0) {
			Invoker::Invoke<void, 3665>(p0);
		}

		void PLAYSTATS_CASINO_BLACKJACK_LIGHT(Any p0) {
			Invoker::Invoke<void, 3666>(p0);
		}

		void PLAYSTATS_CASINO_THREE_CARD_POKER_LIGHT(Any p0) {
			Invoker::Invoke<void, 3667>(p0);
		}

		void PLAYSTATS_CASINO_SLOT_MACHINE_LIGHT(Any p0) {
			Invoker::Invoke<void, 3668>(p0);
		}

		void PLAYSTATS_CASINO_INSIDE_TRACK_LIGHT(Any p0) {
			Invoker::Invoke<void, 3669>(p0);
		}

		void PLAYSTATS_ARCADE_GAME(Any p0, Any p1, Any p2, Any p3, Any p4, Any p5, Any p6) {
			Invoker::Invoke<void, 3670>(p0, p1, p2, p3, p4, p5, p6);
		}

		void PLAYSTATS_ARCADE_LOVE_MATCH(Any p0, Any p1) {
			Invoker::Invoke<void, 3671>(p0, p1);
		}

		void PLAYSTATS_FREEMODE_CASINO_MISSION_ENDED(Any* data) {
			Invoker::Invoke<void, 3672>(data);
		}

		void PLAYSTATS_HEIST3_DRONE(Any p0) {
			Invoker::Invoke<void, 3673>(p0);
		}

		void PLAYSTATS_HEIST3_HACK(Any p0, Any p1, Any p2, Any p3, Any p4, Any p5) {
			Invoker::Invoke<void, 3674>(p0, p1, p2, p3, p4, p5);
		}

		void PLAYSTATS_NPC_PHONE(Any* p0) {
			Invoker::Invoke<void, 3675>(p0);
		}

		void PLAYSTATS_ARCADE_CABINET(Any p0) {
			Invoker::Invoke<void, 3676>(p0);
		}

		void PLAYSTATS_HEIST3_FINALE(Any p0) {
			Invoker::Invoke<void, 3677>(p0);
		}

		void PLAYSTATS_HEIST3_PREP(Any p0) {
			Invoker::Invoke<void, 3678>(p0);
		}

		void PLAYSTATS_MASTER_CONTROL(Any p0, Any p1, Any p2, Any p3, Any p4) {
			Invoker::Invoke<void, 3679>(p0, p1, p2, p3, p4);
		}

		void PLAYSTATS_QUIT_MODE(Any p0, Any p1, Any p2, Any p3, Any p4) {
			Invoker::Invoke<void, 3680>(p0, p1, p2, p3, p4);
		}

		void PLAYSTATS_MISSION_VOTE(Any p0) {
			Invoker::Invoke<void, 3681>(p0);
		}

		void PLAYSTATS_NJVS_VOTE(Any p0) {
			Invoker::Invoke<void, 3682>(p0);
		}

		void _PLAYSTATS_DEATH_INFO(Ped victimPed, Ped killerPed, int mentalState, BOOL revengeKill, int victimKvK, int killerKvK) {
			Invoker::Invoke<void, 6612>(victimPed, killerPed, mentalState, revengeKill, victimKvK, killerKvK);
		}

		void PLAYSTATS_FM_MISSION_END(Any p0, Any p1, Any p2, Any p3) {
			Invoker::Invoke<void, 3684>(p0, p1, p2, p3);
		}

		void PLAYSTATS_HEIST4_PREP(Any p0) {
			Invoker::Invoke<void, 3685>(p0);
		}

		void PLAYSTATS_HEIST4_FINALE(Any p0) {
			Invoker::Invoke<void, 3686>(p0);
		}

		void PLAYSTATS_HEIST4_HACK(Any p0, Any p1, Any p2, Any p3, Any p4) {
			Invoker::Invoke<void, 3687>(p0, p1, p2, p3, p4);
		}

		void PLAYSTATS_SUB_WEAP(Any p0, Any p1, Any p2, Any p3) {
			Invoker::Invoke<void, 3688>(p0, p1, p2, p3);
		}

		void PLAYSTATS_FAST_TRVL(Any p0, Any p1, Any p2, Any p3, Any p4, Any p5, Any p6, Any p7, Any p8, Any p9, Any p10) {
			Invoker::Invoke<void, 3689>(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10);
		}

		void PLAYSTATS_HUB_ENTRY(Any p0) {
			Invoker::Invoke<void, 3690>(p0);
		}

		void PLAYSTATS_DJ_MISSION_ENDED(Any p0) {
			Invoker::Invoke<void, 3691>(p0);
		}

		void PLAYSTATS_ROBBERY_PREP(Any p0) {
			Invoker::Invoke<void, 3692>(p0);
		}

		void PLAYSTATS_ROBBERY_FINALE(Any p0) {
			Invoker::Invoke<void, 3693>(p0);
		}

		void PLAYSTATS_EXTRA_EVENT(Any p0) {
			Invoker::Invoke<void, 3694>(p0);
		}

		void PLAYSTATS_CARCLUB_POINTS(Any p0) {
			Invoker::Invoke<void, 3695>(p0);
		}

		void PLAYSTATS_CARCLUB_CHALLENGE(Any p0, Any p1, Any p2, Any p3) {
			Invoker::Invoke<void, 3696>(p0, p1, p2, p3);
		}

		void PLAYSTATS_CARCLUB_PRIZE(int p0, Hash vehicleModel) {
			Invoker::Invoke<void, 3697>(p0, vehicleModel);
		}

		void PLAYSTATS_AWARD_NAV(Any p0, Any p1, Any p2, Any p3) {
			Invoker::Invoke<void, 3698>(p0, p1, p2, p3);
		}

		void _PLAYSTATS_INIT_MULTIPLAYER(Any p0, Any p1, Any p2) {
			Invoker::Invoke<void, 6611>(p0, p1, p2);
		}

		void PLAYSTATS_INST_MISSION_END(Any p0) {
			Invoker::Invoke<void, 3699>(p0);
		}

		void PLAYSTATS_HUB_EXIT(Any p0) {
			Invoker::Invoke<void, 3700>(p0);
		}

		void PLAYSTATS_LP_NAV(Any* data) {
			Invoker::Invoke<void, 6610>(data);
		}

		void _PLAYSTATS_LP_SUBTILE(Any p0, Any p1, Any p2, Any p3, Any p4) {
			Invoker::Invoke<void, 6609>(p0, p1, p2, p3, p4);
		}

		void PLAYSTATS_VEH_DEL(int bossId1, int bossId2, int bossType, int vehicleID, int reason) {
			Invoker::Invoke<void, 3701>(bossId1, bossId2, bossType, vehicleID, reason);
		}

		void PLAYSTATS_INVENTORY(Any p0) {
			Invoker::Invoke<void, 3702>(p0);
		}

		void _PLAYSTATS_ACID_MISSION_END(Any p0) {
			Invoker::Invoke<void, 3703>(p0);
		}

		void _PLAYSTATS_ACID_RND(Any p0) {
			Invoker::Invoke<void, 3704>(p0);
		}

		void _PLAYSTATS_IDLE(Any p0, Any p1, Any p2) {
			Invoker::Invoke<void, 3705>(p0, p1, p2);
		}

		void _PLAYSTATS_PLAYER_STYLE(Any p0) {
			Invoker::Invoke<void, 3706>(p0);
		}

		void _PLAYSTATS_RANDOM_EVENT(Any p0) {
			Invoker::Invoke<void, 3707>(p0);
		}

		void _PLAYSTATS_PH_SNAPSHOT(Any p0) {
			Invoker::Invoke<void, 6608>(p0);
		}

		void _PLAYSTATS_PH_PROGRESS_TIERS(Any p0) {
			Invoker::Invoke<void, 6607>(p0);
		}

		void _PLAYSTATS_PH_NAV(Any p0) {
			Invoker::Invoke<void, 6606>(p0);
		}

		void _PLAYSTATS_PH_ACTIVITY(Any p0, Any p1, Any p2, Any p3, Any p4, Any p5, Any p6, Any p7) {
			Invoker::Invoke<void, 6605>(p0, p1, p2, p3, p4, p5, p6, p7);
		}

		void _PLAYSTATS_PH_PROGRESS_OBJECTIVES(Any p0) {
			Invoker::Invoke<void, 6604>(p0);
		}

		void _PLAYSTATS_ALERT(Any* data) {
			Invoker::Invoke<void, 3708>(data);
		}

		void _PLAYSTATS_ATTRITION_STAGE_END(Any p0) {
			Invoker::Invoke<void, 3709>(p0);
		}

		void _PLAYSTATS_SHOWROOM_NAV(Any p0, Any p1, Hash entity) {
			Invoker::Invoke<void, 3710>(p0, p1, entity);
		}

		void _PLAYSTATS_SHOWROOM_OVERVIEW(Any* data) {
			Invoker::Invoke<void, 3711>(data);
		}

		void _PLAYSTATS_PIMENU_NAV(Any* data) {
			Invoker::Invoke<void, 6591>(data);
		}

		void _PLAYSTATS_RECOVER_VEHICLE(Any* data) {
			Invoker::Invoke<void, 6592>(data);
		}

		void _PLAYSTATS_SCRIPT_EVENT_FPOM(Any* data) {
			Invoker::Invoke<void, 6603>(data);
		}

		void _PLAYSTATS_LOBBY_EXIT(Any* p0) {
			Invoker::Invoke<void, 6602>(p0);
		}

		void _PLAYSTATS_LOBBY_STARTED(Any* p0) {
			Invoker::Invoke<void, 6601>(p0);
		}

		void _PLAYSTATS_CREATOR_END(Any* p0) {
			Invoker::Invoke<void, 6600>(p0);
		}

	} // namespace STATS

	namespace STREAMING {

		void LOAD_ALL_OBJECTS_NOW() {
			Invoker::Invoke<void, 2693>();
		}

		void LOAD_SCENE(float x, float y, float z) {
			Invoker::Invoke<void, 2694>(x, y, z);
		}

		BOOL NETWORK_UPDATE_LOAD_SCENE() {
			return Invoker::Invoke<BOOL, 2695>();
		}

		BOOL IS_NETWORK_LOADING_SCENE() {
			return Invoker::Invoke<BOOL, 2696>();
		}

		void SET_INTERIOR_ACTIVE(int interiorID, BOOL toggle) {
			Invoker::Invoke<void, 2697>(interiorID, toggle);
		}

		void REQUEST_MODEL(Hash model) {
			Invoker::Invoke<void, 2698>(model);
		}

		void REQUEST_MENU_PED_MODEL(Hash model) {
			Invoker::Invoke<void, 2699>(model);
		}

		BOOL HAS_MODEL_LOADED(Hash model) {
			return Invoker::Invoke<BOOL, 2700>(model);
		}

		void REQUEST_MODELS_IN_ROOM(Interior interior, const char* roomName) {
			Invoker::Invoke<void, 2701>(interior, roomName);
		}

		void SET_MODEL_AS_NO_LONGER_NEEDED(Hash model) {
			Invoker::Invoke<void, 2702>(model);
		}

		BOOL IS_MODEL_IN_CDIMAGE(Hash model) {
			return Invoker::Invoke<BOOL, 2703>(model);
		}

		BOOL IS_MODEL_VALID(Hash model) {
			return Invoker::Invoke<BOOL, 2704>(model);
		}

		BOOL IS_MODEL_A_PED(Hash model) {
			return Invoker::Invoke<BOOL, 2705>(model);
		}

		BOOL IS_MODEL_A_VEHICLE(Hash model) {
			return Invoker::Invoke<BOOL, 2706>(model);
		}

		void REQUEST_COLLISION_AT_COORD(float x, float y, float z) {
			Invoker::Invoke<void, 2707>(x, y, z);
		}

		void REQUEST_COLLISION_FOR_MODEL(Hash model) {
			Invoker::Invoke<void, 2708>(model);
		}

		BOOL HAS_COLLISION_FOR_MODEL_LOADED(Hash model) {
			return Invoker::Invoke<BOOL, 2709>(model);
		}

		void REQUEST_ADDITIONAL_COLLISION_AT_COORD(float x, float y, float z) {
			Invoker::Invoke<void, 2710>(x, y, z);
		}

		BOOL DOES_ANIM_DICT_EXIST(const char* animDict) {
			return Invoker::Invoke<BOOL, 2711>(animDict);
		}

		void REQUEST_ANIM_DICT(const char* animDict) {
			Invoker::Invoke<void, 2712>(animDict);
		}

		BOOL HAS_ANIM_DICT_LOADED(const char* animDict) {
			return Invoker::Invoke<BOOL, 2713>(animDict);
		}

		void REMOVE_ANIM_DICT(const char* animDict) {
			Invoker::Invoke<void, 2714>(animDict);
		}

		void REQUEST_ANIM_SET(const char* animSet) {
			Invoker::Invoke<void, 2715>(animSet);
		}

		BOOL HAS_ANIM_SET_LOADED(const char* animSet) {
			return Invoker::Invoke<BOOL, 2716>(animSet);
		}

		void REMOVE_ANIM_SET(const char* animSet) {
			Invoker::Invoke<void, 2717>(animSet);
		}

		void REQUEST_CLIP_SET(const char* clipSet) {
			Invoker::Invoke<void, 2718>(clipSet);
		}

		BOOL HAS_CLIP_SET_LOADED(const char* clipSet) {
			return Invoker::Invoke<BOOL, 2719>(clipSet);
		}

		void REMOVE_CLIP_SET(const char* clipSet) {
			Invoker::Invoke<void, 2720>(clipSet);
		}

		void REQUEST_IPL(const char* iplName) {
			Invoker::Invoke<void, 2721>(iplName);
		}

		void REMOVE_IPL(const char* iplName) {
			Invoker::Invoke<void, 2722>(iplName);
		}

		BOOL IS_IPL_ACTIVE(const char* iplName) {
			return Invoker::Invoke<BOOL, 2723>(iplName);
		}

		void SET_STREAMING(BOOL toggle) {
			Invoker::Invoke<void, 2724>(toggle);
		}

		void LOAD_GLOBAL_WATER_FILE(int waterType) {
			Invoker::Invoke<void, 2725>(waterType);
		}

		int GET_GLOBAL_WATER_FILE() {
			return Invoker::Invoke<int, 2726>();
		}

		void SET_GAME_PAUSES_FOR_STREAMING(BOOL toggle) {
			Invoker::Invoke<void, 2727>(toggle);
		}

		void SET_REDUCE_PED_MODEL_BUDGET(BOOL toggle) {
			Invoker::Invoke<void, 2728>(toggle);
		}

		void SET_REDUCE_VEHICLE_MODEL_BUDGET(BOOL toggle) {
			Invoker::Invoke<void, 2729>(toggle);
		}

		void SET_DITCH_POLICE_MODELS(BOOL toggle) {
			Invoker::Invoke<void, 2730>(toggle);
		}

		int GET_NUMBER_OF_STREAMING_REQUESTS() {
			return Invoker::Invoke<int, 2731>();
		}

		void REQUEST_PTFX_ASSET() {
			Invoker::Invoke<void, 2732>();
		}

		BOOL HAS_PTFX_ASSET_LOADED() {
			return Invoker::Invoke<BOOL, 2733>();
		}

		void REMOVE_PTFX_ASSET() {
			Invoker::Invoke<void, 2734>();
		}

		void REQUEST_NAMED_PTFX_ASSET(const char* fxName) {
			Invoker::Invoke<void, 2735>(fxName);
		}

		BOOL HAS_NAMED_PTFX_ASSET_LOADED(const char* fxName) {
			return Invoker::Invoke<BOOL, 2736>(fxName);
		}

		void REMOVE_NAMED_PTFX_ASSET(const char* fxName) {
			Invoker::Invoke<void, 2737>(fxName);
		}

		void SET_VEHICLE_POPULATION_BUDGET(int p0) {
			Invoker::Invoke<void, 2738>(p0);
		}

		void SET_PED_POPULATION_BUDGET(int p0) {
			Invoker::Invoke<void, 2739>(p0);
		}

		void CLEAR_FOCUS() {
			Invoker::Invoke<void, 2740>();
		}

		void SET_FOCUS_POS_AND_VEL(float x, float y, float z, float offsetX, float offsetY, float offsetZ) {
			Invoker::Invoke<void, 2741>(x, y, z, offsetX, offsetY, offsetZ);
		}

		void SET_FOCUS_ENTITY(Entity entity) {
			Invoker::Invoke<void, 2742>(entity);
		}

		BOOL IS_ENTITY_FOCUS(Entity entity) {
			return Invoker::Invoke<BOOL, 2743>(entity);
		}

		void SET_RESTORE_FOCUS_ENTITY(Entity p0) {
			Invoker::Invoke<void, 2744>(p0);
		}

		void SET_MAPDATACULLBOX_ENABLED(const char* name, BOOL toggle) {
			Invoker::Invoke<void, 2745>(name, toggle);
		}

		void SET_ALL_MAPDATA_CULLED(Any p0) {
			Invoker::Invoke<void, 2746>(p0);
		}

		int STREAMVOL_CREATE_SPHERE(float x, float y, float z, float rad, Any p4, Any p5) {
			return Invoker::Invoke<int, 2747>(x, y, z, rad, p4, p5);
		}

		int STREAMVOL_CREATE_FRUSTUM(float p0, float p1, float p2, float p3, float p4, float p5, float p6, Any p7, Any p8) {
			return Invoker::Invoke<int, 2748>(p0, p1, p2, p3, p4, p5, p6, p7, p8);
		}

		int STREAMVOL_CREATE_LINE(float p0, float p1, float p2, float p3, float p4, float p5, Any p6) {
			return Invoker::Invoke<int, 2749>(p0, p1, p2, p3, p4, p5, p6);
		}

		void STREAMVOL_DELETE(Any unused) {
			Invoker::Invoke<void, 2750>(unused);
		}

		BOOL STREAMVOL_HAS_LOADED(Any unused) {
			return Invoker::Invoke<BOOL, 2751>(unused);
		}

		BOOL STREAMVOL_IS_VALID(Any unused) {
			return Invoker::Invoke<BOOL, 2752>(unused);
		}

		BOOL IS_STREAMVOL_ACTIVE() {
			return Invoker::Invoke<BOOL, 2753>();
		}

		BOOL NEW_LOAD_SCENE_START(float posX, float posY, float posZ, float offsetX, float offsetY, float offsetZ, float radius, int p7) {
			return Invoker::Invoke<BOOL, 2754>(posX, posY, posZ, offsetX, offsetY, offsetZ, radius, p7);
		}

		BOOL NEW_LOAD_SCENE_START_SPHERE(float x, float y, float z, float radius, Any p4) {
			return Invoker::Invoke<BOOL, 2755>(x, y, z, radius, p4);
		}

		void NEW_LOAD_SCENE_STOP() {
			Invoker::Invoke<void, 2756>();
		}

		BOOL IS_NEW_LOAD_SCENE_ACTIVE() {
			return Invoker::Invoke<BOOL, 2757>();
		}

		BOOL IS_NEW_LOAD_SCENE_LOADED() {
			return Invoker::Invoke<BOOL, 2758>();
		}

		BOOL IS_SAFE_TO_START_PLAYER_SWITCH() {
			return Invoker::Invoke<BOOL, 2759>();
		}

		void START_PLAYER_SWITCH(Ped from, Ped to, int flags, int switchType) {
			Invoker::Invoke<void, 2760>(from, to, flags, switchType);
		}

		void STOP_PLAYER_SWITCH() {
			Invoker::Invoke<void, 2761>();
		}

		BOOL IS_PLAYER_SWITCH_IN_PROGRESS() {
			return Invoker::Invoke<BOOL, 2762>();
		}

		int GET_PLAYER_SWITCH_TYPE() {
			return Invoker::Invoke<int, 2763>();
		}

		int GET_IDEAL_PLAYER_SWITCH_TYPE(float x1, float y1, float z1, float x2, float y2, float z2) {
			return Invoker::Invoke<int, 2764>(x1, y1, z1, x2, y2, z2);
		}

		int GET_PLAYER_SWITCH_STATE() {
			return Invoker::Invoke<int, 2765>();
		}

		int GET_PLAYER_SHORT_SWITCH_STATE() {
			return Invoker::Invoke<int, 2766>();
		}

		void SET_PLAYER_SHORT_SWITCH_STYLE(int p0) {
			Invoker::Invoke<void, 2767>(p0);
		}

		int GET_PLAYER_SWITCH_JUMP_CUT_INDEX() {
			return Invoker::Invoke<int, 2768>();
		}

		void SET_PLAYER_SWITCH_OUTRO(float cameraCoordX, float cameraCoordY, float cameraCoordZ, float camRotationX, float camRotationY, float camRotationZ, float camFov, float camFarClip, int rotationOrder) {
			Invoker::Invoke<void, 2769>(cameraCoordX, cameraCoordY, cameraCoordZ, camRotationX, camRotationY, camRotationZ, camFov, camFarClip, rotationOrder);
		}

		void SET_PLAYER_SWITCH_ESTABLISHING_SHOT(const char* name) {
			Invoker::Invoke<void, 2770>(name);
		}

		void ALLOW_PLAYER_SWITCH_PAN() {
			Invoker::Invoke<void, 2771>();
		}

		void ALLOW_PLAYER_SWITCH_OUTRO() {
			Invoker::Invoke<void, 2772>();
		}

		void ALLOW_PLAYER_SWITCH_ASCENT() {
			Invoker::Invoke<void, 2773>();
		}

		void ALLOW_PLAYER_SWITCH_DESCENT() {
			Invoker::Invoke<void, 2774>();
		}

		BOOL IS_SWITCH_READY_FOR_DESCENT() {
			return Invoker::Invoke<BOOL, 2775>();
		}

		void ENABLE_SWITCH_PAUSE_BEFORE_DESCENT() {
			Invoker::Invoke<void, 2776>();
		}

		void DISABLE_SWITCH_OUTRO_FX() {
			Invoker::Invoke<void, 2777>();
		}

		void SWITCH_TO_MULTI_FIRSTPART(Ped ped, int flags, int switchType) {
			Invoker::Invoke<void, 2778>(ped, flags, switchType);
		}

		void SWITCH_TO_MULTI_SECONDPART(Ped ped) {
			Invoker::Invoke<void, 2779>(ped);
		}

		BOOL IS_SWITCH_TO_MULTI_FIRSTPART_FINISHED() {
			return Invoker::Invoke<BOOL, 2780>();
		}

		int GET_PLAYER_SWITCH_INTERP_OUT_DURATION() {
			return Invoker::Invoke<int, 2781>();
		}

		int GET_PLAYER_SWITCH_INTERP_OUT_CURRENT_TIME() {
			return Invoker::Invoke<int, 2782>();
		}

		BOOL IS_SWITCH_SKIPPING_DESCENT() {
			return Invoker::Invoke<BOOL, 2783>();
		}

		void SET_SCENE_STREAMING_TRACKS_CAM_POS_THIS_FRAME() {
			Invoker::Invoke<void, 2784>();
		}

		float GET_LODSCALE() {
			return Invoker::Invoke<float, 2785>();
		}

		void OVERRIDE_LODSCALE_THIS_FRAME(float scaling) {
			Invoker::Invoke<void, 2786>(scaling);
		}

		void REMAP_LODSCALE_RANGE_THIS_FRAME(float p0, float p1, float p2, float p3) {
			Invoker::Invoke<void, 2787>(p0, p1, p2, p3);
		}

		void SUPPRESS_HD_MAP_STREAMING_THIS_FRAME() {
			Invoker::Invoke<void, 2788>();
		}

		void SET_RENDER_HD_ONLY(BOOL toggle) {
			Invoker::Invoke<void, 2789>(toggle);
		}

		void FORCE_ALLOW_TIME_BASED_FADING_THIS_FRAME() {
			Invoker::Invoke<void, 2790>();
		}

		void IPL_GROUP_SWAP_START(const char* iplName1, const char* iplName2) {
			Invoker::Invoke<void, 2791>(iplName1, iplName2);
		}

		void IPL_GROUP_SWAP_CANCEL() {
			Invoker::Invoke<void, 2792>();
		}

		BOOL IPL_GROUP_SWAP_IS_READY() {
			return Invoker::Invoke<BOOL, 2793>();
		}

		void IPL_GROUP_SWAP_FINISH() {
			Invoker::Invoke<void, 2794>();
		}

		BOOL IPL_GROUP_SWAP_IS_ACTIVE() {
			return Invoker::Invoke<BOOL, 2795>();
		}

		void PREFETCH_SRL(const char* srl) {
			Invoker::Invoke<void, 2796>(srl);
		}

		BOOL IS_SRL_LOADED() {
			return Invoker::Invoke<BOOL, 2797>();
		}

		void BEGIN_SRL() {
			Invoker::Invoke<void, 2798>();
		}

		void END_SRL() {
			Invoker::Invoke<void, 2799>();
		}

		void SET_SRL_TIME(float p0) {
			Invoker::Invoke<void, 2800>(p0);
		}

		void SET_SRL_POST_CUTSCENE_CAMERA(Any p0, Any p1, Any p2, Any p3, Any p4, Any p5) {
			Invoker::Invoke<void, 2801>(p0, p1, p2, p3, p4, p5);
		}

		void SET_SRL_READAHEAD_TIMES(Any p0, Any p1, Any p2, Any p3) {
			Invoker::Invoke<void, 2802>(p0, p1, p2, p3);
		}

		void SET_SRL_LONG_JUMP_MODE(BOOL p0) {
			Invoker::Invoke<void, 2803>(p0);
		}

		void SET_SRL_FORCE_PRESTREAM(Any p0) {
			Invoker::Invoke<void, 2804>(p0);
		}

		void SET_HD_AREA(float x, float y, float z, float radius) {
			Invoker::Invoke<void, 2805>(x, y, z, radius);
		}

		void CLEAR_HD_AREA() {
			Invoker::Invoke<void, 2806>();
		}

		void INIT_CREATOR_BUDGET() {
			Invoker::Invoke<void, 2807>();
		}

		void SHUTDOWN_CREATOR_BUDGET() {
			Invoker::Invoke<void, 2808>();
		}

		BOOL ADD_MODEL_TO_CREATOR_BUDGET(Hash modelHash) {
			return Invoker::Invoke<BOOL, 2809>(modelHash);
		}

		void REMOVE_MODEL_FROM_CREATOR_BUDGET(Hash modelHash) {
			Invoker::Invoke<void, 2810>(modelHash);
		}

		float GET_USED_CREATOR_BUDGET() {
			return Invoker::Invoke<float, 2811>();
		}

		float _GET_MODEL_ADDITIONAL_COST(Hash modelHash) {
			return Invoker::Invoke<float, 6599>(modelHash);
		}

		float _GET_TOTAL_MODEL_COST(Hash modelHash) {
			return Invoker::Invoke<float, 6598>(modelHash);
		}

		void SET_ISLAND_ENABLED(const char* name, BOOL toggle) {
			Invoker::Invoke<void, 2812>(name, toggle);
		}

		void _SET_SPHERICAL_STREAM_DISTANT_HILODS_THIS_FRAME() {
			Invoker::Invoke<void, 6597>();
		}

		BOOL IS_GAME_INSTALLED() {
			return Invoker::Invoke<BOOL, 6596>();
		}

	} // namespace STREAMING

	namespace TASK {

		void TASK_PAUSE(Ped ped, int ms) {
			Invoker::Invoke<void, 2044>(ped, ms);
		}

		void TASK_STAND_STILL(Ped ped, int time) {
			Invoker::Invoke<void, 2045>(ped, time);
		}

		void TASK_JUMP(Ped ped, BOOL usePlayerLaunchForce, BOOL doSuperJump, BOOL useFullSuperJumpForce) {
			Invoker::Invoke<void, 2046>(ped, usePlayerLaunchForce, doSuperJump, useFullSuperJumpForce);
		}

		void TASK_COWER(Ped ped, int duration) {
			Invoker::Invoke<void, 2047>(ped, duration);
		}

		void TASK_HANDS_UP(Ped ped, int duration, Ped facingPed, int timeToFacePed, int flags) {
			Invoker::Invoke<void, 2048>(ped, duration, facingPed, timeToFacePed, flags);
		}

		void UPDATE_TASK_HANDS_UP_DURATION(Ped ped, int duration) {
			Invoker::Invoke<void, 2049>(ped, duration);
		}

		void TASK_OPEN_VEHICLE_DOOR(Ped ped, Vehicle vehicle, int timeOut, int seat, float speed) {
			Invoker::Invoke<void, 2050>(ped, vehicle, timeOut, seat, speed);
		}

		void TASK_ENTER_VEHICLE(Ped ped, Vehicle vehicle, int timeout, int seat, float speed, int flag, const char* overrideEntryClipsetName) {
			Invoker::Invoke<void, 2051>(ped, vehicle, timeout, seat, speed, flag, overrideEntryClipsetName);
		}

		void TASK_LEAVE_VEHICLE(Ped ped, Vehicle vehicle, int flags) {
			Invoker::Invoke<void, 2052>(ped, vehicle, flags);
		}

		void TASK_GET_OFF_BOAT(Ped ped, Vehicle boat) {
			Invoker::Invoke<void, 2053>(ped, boat);
		}

		void TASK_SKY_DIVE(Ped ped, BOOL instant) {
			Invoker::Invoke<void, 2054>(ped, instant);
		}

		void TASK_PARACHUTE(Ped ped, BOOL giveParachuteItem, BOOL instant) {
			Invoker::Invoke<void, 2055>(ped, giveParachuteItem, instant);
		}

		void TASK_PARACHUTE_TO_TARGET(Ped ped, float x, float y, float z) {
			Invoker::Invoke<void, 2056>(ped, x, y, z);
		}

		void SET_PARACHUTE_TASK_TARGET(Ped ped, float x, float y, float z) {
			Invoker::Invoke<void, 2057>(ped, x, y, z);
		}

		void SET_PARACHUTE_TASK_THRUST(Ped ped, float thrust) {
			Invoker::Invoke<void, 2058>(ped, thrust);
		}

		void TASK_RAPPEL_FROM_HELI(Ped ped, float minHeightAboveGround) {
			Invoker::Invoke<void, 2059>(ped, minHeightAboveGround);
		}

		void TASK_VEHICLE_DRIVE_TO_COORD(Ped ped, Vehicle vehicle, float x, float y, float z, float speed, Any p6, Hash vehicleModel, int drivingMode, float stopRange, float straightLineDistance) {
			Invoker::Invoke<void, 2060>(ped, vehicle, x, y, z, speed, p6, vehicleModel, drivingMode, stopRange, straightLineDistance);
		}

		void TASK_VEHICLE_DRIVE_TO_COORD_LONGRANGE(Ped ped, Vehicle vehicle, float x, float y, float z, float speed, int driveMode, float stopRange) {
			Invoker::Invoke<void, 2061>(ped, vehicle, x, y, z, speed, driveMode, stopRange);
		}

		void TASK_VEHICLE_DRIVE_WANDER(Ped ped, Vehicle vehicle, float speed, int drivingStyle) {
			Invoker::Invoke<void, 2062>(ped, vehicle, speed, drivingStyle);
		}

		void TASK_FOLLOW_TO_OFFSET_OF_ENTITY(Ped ped, Entity entity, float offsetX, float offsetY, float offsetZ, float movementSpeed, int timeout, float stoppingRange, BOOL persistFollowing) {
			Invoker::Invoke<void, 2063>(ped, entity, offsetX, offsetY, offsetZ, movementSpeed, timeout, stoppingRange, persistFollowing);
		}

		void TASK_GO_STRAIGHT_TO_COORD(Ped ped, float x, float y, float z, float speed, int timeout, float targetHeading, float distanceToSlide) {
			Invoker::Invoke<void, 2064>(ped, x, y, z, speed, timeout, targetHeading, distanceToSlide);
		}

		void TASK_GO_STRAIGHT_TO_COORD_RELATIVE_TO_ENTITY(Ped ped, Entity entity, float x, float y, float z, float moveBlendRatio, int time) {
			Invoker::Invoke<void, 2065>(ped, entity, x, y, z, moveBlendRatio, time);
		}

		void TASK_ACHIEVE_HEADING(Ped ped, float heading, int timeout) {
			Invoker::Invoke<void, 2066>(ped, heading, timeout);
		}

		void TASK_FLUSH_ROUTE() {
			Invoker::Invoke<void, 2067>();
		}

		void TASK_EXTEND_ROUTE(float x, float y, float z) {
			Invoker::Invoke<void, 2068>(x, y, z);
		}

		void TASK_FOLLOW_POINT_ROUTE(Ped ped, float speed, int mode) {
			Invoker::Invoke<void, 2069>(ped, speed, mode);
		}

		void TASK_GO_TO_ENTITY(Entity entity, Entity target, int duration, float distance, float moveBlendRatio, float slowDownDistance, int flags) {
			Invoker::Invoke<void, 2070>(entity, target, duration, distance, moveBlendRatio, slowDownDistance, flags);
		}

		void TASK_SMART_FLEE_COORD(Ped ped, float x, float y, float z, float distance, int time, BOOL preferPavements, BOOL quitIfOutOfRange) {
			Invoker::Invoke<void, 2071>(ped, x, y, z, distance, time, preferPavements, quitIfOutOfRange);
		}

		void TASK_SMART_FLEE_PED(Ped ped, Ped fleeTarget, float safeDistance, int fleeTime, BOOL preferPavements, BOOL updateToNearestHatedPed) {
			Invoker::Invoke<void, 2072>(ped, fleeTarget, safeDistance, fleeTime, preferPavements, updateToNearestHatedPed);
		}

		void TASK_REACT_AND_FLEE_PED(Ped ped, Ped fleeTarget) {
			Invoker::Invoke<void, 2073>(ped, fleeTarget);
		}

		void TASK_SHOCKING_EVENT_REACT(Ped ped, int eventHandle) {
			Invoker::Invoke<void, 2074>(ped, eventHandle);
		}

		void TASK_WANDER_IN_AREA(Ped ped, float x, float y, float z, float radius, float minimalLength, float timeBetweenWalks) {
			Invoker::Invoke<void, 2075>(ped, x, y, z, radius, minimalLength, timeBetweenWalks);
		}

		void TASK_WANDER_STANDARD(Ped ped, float heading, int flags) {
			Invoker::Invoke<void, 2076>(ped, heading, flags);
		}

		void TASK_WANDER_SPECIFIC(Ped ped, const char* conditionalAnimGroupStr, const char* conditionalAnimStr, float heading) {
			Invoker::Invoke<void, 2077>(ped, conditionalAnimGroupStr, conditionalAnimStr, heading);
		}

		void TASK_VEHICLE_PARK(Ped ped, Vehicle vehicle, float x, float y, float z, float heading, int mode, float radius, BOOL keepEngineOn) {
			Invoker::Invoke<void, 2078>(ped, vehicle, x, y, z, heading, mode, radius, keepEngineOn);
		}

		void TASK_STEALTH_KILL(Ped killer, Ped target, Hash stealthKillActionResultHash, float desiredMoveBlendRatio, int stealthFlags) {
			Invoker::Invoke<void, 2079>(killer, target, stealthKillActionResultHash, desiredMoveBlendRatio, stealthFlags);
		}

		void TASK_PLANT_BOMB(Ped ped, float x, float y, float z, float heading) {
			Invoker::Invoke<void, 2080>(ped, x, y, z, heading);
		}

		void TASK_SHARK_CIRCLE_COORD(Ped ped, float x, float y, float z, float moveBlendRatio, float radius) {
			Invoker::Invoke<void, 6704>(ped, x, y, z, moveBlendRatio, radius);
		}

		void TASK_FOLLOW_NAV_MESH_TO_COORD(Ped ped, float x, float y, float z, float moveBlendRatio, int time, float targetRadius, int flags, float targetHeading) {
			Invoker::Invoke<void, 2081>(ped, x, y, z, moveBlendRatio, time, targetRadius, flags, targetHeading);
		}

		void TASK_FOLLOW_NAV_MESH_TO_COORD_ADVANCED(Ped ped, float x, float y, float z, float moveBlendRatio, int time, float targetRadius, int flags, float slideToCoordHeading, float maxSlopeNavigable, float clampMaxSearchDistance, float targetHeading) {
			Invoker::Invoke<void, 2082>(ped, x, y, z, moveBlendRatio, time, targetRadius, flags, slideToCoordHeading, maxSlopeNavigable, clampMaxSearchDistance, targetHeading);
		}

		void SET_PED_PATH_CAN_USE_CLIMBOVERS(Ped ped, BOOL Toggle) {
			Invoker::Invoke<void, 2083>(ped, Toggle);
		}

		void SET_PED_PATH_CAN_USE_LADDERS(Ped ped, BOOL Toggle) {
			Invoker::Invoke<void, 2084>(ped, Toggle);
		}

		void SET_PED_PATH_CAN_DROP_FROM_HEIGHT(Ped ped, BOOL Toggle) {
			Invoker::Invoke<void, 2085>(ped, Toggle);
		}

		void SET_PED_PATH_CLIMB_COST_MODIFIER(Ped ped, float modifier) {
			Invoker::Invoke<void, 2086>(ped, modifier);
		}

		void SET_PED_PATH_MAY_ENTER_WATER(Ped ped, BOOL mayEnterWater) {
			Invoker::Invoke<void, 2087>(ped, mayEnterWater);
		}

		void SET_PED_PATH_PREFER_TO_AVOID_WATER(Ped ped, BOOL avoidWater) {
			Invoker::Invoke<void, 2088>(ped, avoidWater);
		}

		void SET_PED_PATH_AVOID_FIRE(Ped ped, BOOL avoidFire) {
			Invoker::Invoke<void, 2089>(ped, avoidFire);
		}

		void SET_GLOBAL_MIN_BIRD_FLIGHT_HEIGHT(float height) {
			Invoker::Invoke<void, 2090>(height);
		}

		int GET_NAVMESH_ROUTE_DISTANCE_REMAINING(Ped ped, float* distanceRemaining, BOOL* isPathReady) {
			return Invoker::Invoke<int, 2091>(ped, distanceRemaining, isPathReady);
		}

		int GET_NAVMESH_ROUTE_RESULT(Ped ped) {
			return Invoker::Invoke<int, 2092>(ped);
		}

		BOOL IS_CONTROLLED_VEHICLE_UNABLE_TO_GET_TO_ROAD(Ped ped) {
			return Invoker::Invoke<BOOL, 2093>(ped);
		}

		void TASK_GO_TO_COORD_ANY_MEANS(Ped ped, float x, float y, float z, float moveBlendRatio, Vehicle vehicle, BOOL useLongRangeVehiclePathing, int drivingFlags, float maxRangeToShootTargets) {
			Invoker::Invoke<void, 2094>(ped, x, y, z, moveBlendRatio, vehicle, useLongRangeVehiclePathing, drivingFlags, maxRangeToShootTargets);
		}

		void TASK_GO_TO_COORD_ANY_MEANS_EXTRA_PARAMS(Ped ped, float x, float y, float z, float moveBlendRatio, Vehicle vehicle, BOOL useLongRangeVehiclePathing, int drivingFlags, float maxRangeToShootTargets, float extraVehToTargetDistToPreferVehicle, float driveStraightLineDistance, int extraFlags, float warpTimerMS) {
			Invoker::Invoke<void, 2095>(ped, x, y, z, moveBlendRatio, vehicle, useLongRangeVehiclePathing, drivingFlags, maxRangeToShootTargets, extraVehToTargetDistToPreferVehicle, driveStraightLineDistance, extraFlags, warpTimerMS);
		}

		void TASK_GO_TO_COORD_ANY_MEANS_EXTRA_PARAMS_WITH_CRUISE_SPEED(Ped ped, float x, float y, float z, float moveBlendRatio, Vehicle vehicle, BOOL useLongRangeVehiclePathing, int drivingFlags, float maxRangeToShootTargets, float extraVehToTargetDistToPreferVehicle, float driveStraightLineDistance, int extraFlags, float cruiseSpeed, float targetArriveDist) {
			Invoker::Invoke<void, 2096>(ped, x, y, z, moveBlendRatio, vehicle, useLongRangeVehiclePathing, drivingFlags, maxRangeToShootTargets, extraVehToTargetDistToPreferVehicle, driveStraightLineDistance, extraFlags, cruiseSpeed, targetArriveDist);
		}

		void TASK_PLAY_ANIM(Ped ped, const char* animDictionary, const char* animationName, float blendInSpeed, float blendOutSpeed, int duration, int flag, float playbackRate, BOOL lockX, BOOL lockY, BOOL lockZ) {
			Invoker::Invoke<void, 2097>(ped, animDictionary, animationName, blendInSpeed, blendOutSpeed, duration, flag, playbackRate, lockX, lockY, lockZ);
		}

		void TASK_PLAY_ANIM_ADVANCED(Ped ped, const char* animDict, const char* animName, float posX, float posY, float posZ, float rotX, float rotY, float rotZ, float animEnterSpeed, float animExitSpeed, int duration, Any flag, float animTime, int rotOrder, int ikFlags) {
			Invoker::Invoke<void, 2098>(ped, animDict, animName, posX, posY, posZ, rotX, rotY, rotZ, animEnterSpeed, animExitSpeed, duration, flag, animTime, rotOrder, ikFlags);
		}

		void STOP_ANIM_TASK(Entity entity, const char* animDictionary, const char* animationName, float blendDelta) {
			Invoker::Invoke<void, 2099>(entity, animDictionary, animationName, blendDelta);
		}

		void TASK_SCRIPTED_ANIMATION(Ped ped, int* priorityLowData, int* priorityMidData, int* priorityHighData, float blendInDelta, float blendOutDelta) {
			Invoker::Invoke<void, 2100>(ped, priorityLowData, priorityMidData, priorityHighData, blendInDelta, blendOutDelta);
		}

		void PLAY_ENTITY_SCRIPTED_ANIM(Entity entity, int* priorityLowData, int* priorityMidData, int* priorityHighData, float blendInDelta, float blendOutDelta) {
			Invoker::Invoke<void, 2101>(entity, priorityLowData, priorityMidData, priorityHighData, blendInDelta, blendOutDelta);
		}

		void STOP_ANIM_PLAYBACK(Entity entity, int priority, BOOL secondary) {
			Invoker::Invoke<void, 2102>(entity, priority, secondary);
		}

		void SET_ANIM_WEIGHT(Entity entity, float weight, int priority, int index, BOOL secondary) {
			Invoker::Invoke<void, 2103>(entity, weight, priority, index, secondary);
		}

		void SET_ANIM_PHASE(Entity entity, float phase, int priority, BOOL secondary) {
			Invoker::Invoke<void, 2104>(entity, phase, priority, secondary);
		}

		void SET_ANIM_RATE(Entity entity, float rate, int priority, BOOL secondary) {
			Invoker::Invoke<void, 2105>(entity, rate, priority, secondary);
		}

		void SET_ANIM_LOOPED(Entity entity, BOOL looped, int priority, BOOL secondary) {
			Invoker::Invoke<void, 2106>(entity, looped, priority, secondary);
		}

		void TASK_PLAY_PHONE_GESTURE_ANIMATION(Ped ped, const char* animDict, const char* animation, const char* boneMaskType, float blendInDuration, float blendOutDuration, BOOL isLooping, BOOL holdLastFrame) {
			Invoker::Invoke<void, 2107>(ped, animDict, animation, boneMaskType, blendInDuration, blendOutDuration, isLooping, holdLastFrame);
		}

		void TASK_STOP_PHONE_GESTURE_ANIMATION(Ped ped, float blendOutOverride) {
			Invoker::Invoke<void, 2108>(ped, blendOutOverride);
		}

		BOOL IS_PLAYING_PHONE_GESTURE_ANIM(Ped ped) {
			return Invoker::Invoke<BOOL, 2109>(ped);
		}

		float GET_PHONE_GESTURE_ANIM_CURRENT_TIME(Ped ped) {
			return Invoker::Invoke<float, 2110>(ped);
		}

		float GET_PHONE_GESTURE_ANIM_TOTAL_TIME(Ped ped) {
			return Invoker::Invoke<float, 2111>(ped);
		}

		void TASK_VEHICLE_PLAY_ANIM(Vehicle vehicle, const char* animationSet, const char* animationName) {
			Invoker::Invoke<void, 2112>(vehicle, animationSet, animationName);
		}

		void TASK_LOOK_AT_COORD(Entity entity, float x, float y, float z, int duration, int flags, int priority) {
			Invoker::Invoke<void, 2113>(entity, x, y, z, duration, flags, priority);
		}

		void TASK_LOOK_AT_ENTITY(Ped ped, Entity lookAt, int duration, int flags, int priority) {
			Invoker::Invoke<void, 2114>(ped, lookAt, duration, flags, priority);
		}

		void TASK_CLEAR_LOOK_AT(Ped ped) {
			Invoker::Invoke<void, 2115>(ped);
		}

		void OPEN_SEQUENCE_TASK(int* taskSequenceId) {
			Invoker::Invoke<void, 2116>(taskSequenceId);
		}

		void CLOSE_SEQUENCE_TASK(int taskSequenceId) {
			Invoker::Invoke<void, 2117>(taskSequenceId);
		}

		void TASK_PERFORM_SEQUENCE(Ped ped, int taskSequenceId) {
			Invoker::Invoke<void, 2118>(ped, taskSequenceId);
		}

		void TASK_PERFORM_SEQUENCE_LOCALLY(Ped ped, int taskSequenceId) {
			Invoker::Invoke<void, 2119>(ped, taskSequenceId);
		}

		void CLEAR_SEQUENCE_TASK(int* taskSequenceId) {
			Invoker::Invoke<void, 2120>(taskSequenceId);
		}

		void SET_SEQUENCE_TO_REPEAT(int taskSequenceId, BOOL repeat) {
			Invoker::Invoke<void, 2121>(taskSequenceId, repeat);
		}

		void SET_SEQUENCE_PREVENT_MIGRATION(int taskSequenceId) {
			Invoker::Invoke<void, 6705>(taskSequenceId);
		}

		int GET_SEQUENCE_PROGRESS(Ped ped) {
			return Invoker::Invoke<int, 2122>(ped);
		}

		BOOL GET_IS_TASK_ACTIVE(Ped ped, int taskIndex) {
			return Invoker::Invoke<BOOL, 2123>(ped, taskIndex);
		}

		int GET_SCRIPT_TASK_STATUS(Ped ped, Hash taskHash) {
			return Invoker::Invoke<int, 2124>(ped, taskHash);
		}

		int GET_ACTIVE_VEHICLE_MISSION_TYPE(Vehicle vehicle) {
			return Invoker::Invoke<int, 2125>(vehicle);
		}

		void TASK_LEAVE_ANY_VEHICLE(Ped ped, int delayTime, int flags) {
			Invoker::Invoke<void, 2126>(ped, delayTime, flags);
		}

		void TASK_AIM_GUN_SCRIPTED(Ped ped, Hash scriptTask, BOOL disableBlockingClip, BOOL instantBlendToAim) {
			Invoker::Invoke<void, 2127>(ped, scriptTask, disableBlockingClip, instantBlendToAim);
		}

		void TASK_AIM_GUN_SCRIPTED_WITH_TARGET(Ped ped, Ped target, float x, float y, float z, int gunTaskType, BOOL disableBlockingClip, BOOL forceAim) {
			Invoker::Invoke<void, 2128>(ped, target, x, y, z, gunTaskType, disableBlockingClip, forceAim);
		}

		void UPDATE_TASK_AIM_GUN_SCRIPTED_TARGET(Ped ped, Ped target, float x, float y, float z, BOOL disableBlockingClip) {
			Invoker::Invoke<void, 2129>(ped, target, x, y, z, disableBlockingClip);
		}

		const char* GET_CLIP_SET_FOR_SCRIPTED_GUN_TASK(int gunTaskType) {
			return Invoker::Invoke<const char*, 2130>(gunTaskType);
		}

		void TASK_AIM_GUN_AT_ENTITY(Ped ped, Entity entity, int duration, BOOL instantBlendToAim) {
			Invoker::Invoke<void, 2131>(ped, entity, duration, instantBlendToAim);
		}

		void TASK_TURN_PED_TO_FACE_ENTITY(Ped ped, Entity entity, int duration) {
			Invoker::Invoke<void, 2132>(ped, entity, duration);
		}

		void TASK_AIM_GUN_AT_COORD(Ped ped, float x, float y, float z, int time, BOOL instantBlendToAim, BOOL playAnimIntro) {
			Invoker::Invoke<void, 2133>(ped, x, y, z, time, instantBlendToAim, playAnimIntro);
		}

		void TASK_SHOOT_AT_COORD(Ped ped, float x, float y, float z, int duration, Hash firingPattern) {
			Invoker::Invoke<void, 2134>(ped, x, y, z, duration, firingPattern);
		}

		void TASK_SHUFFLE_TO_NEXT_VEHICLE_SEAT(Ped ped, Vehicle vehicle, BOOL useAlternateShuffle) {
			Invoker::Invoke<void, 2135>(ped, vehicle, useAlternateShuffle);
		}

		void CLEAR_PED_TASKS(Ped ped) {
			Invoker::Invoke<void, 2136>(ped);
		}

		void CLEAR_PED_SCRIPT_TASK_IF_RUNNING_THREAT_RESPONSE_NON_TEMP_TASK(Ped ped) {
			Invoker::Invoke<void, 6706>(ped);
		}

		void CLEAR_PED_SECONDARY_TASK(Ped ped) {
			Invoker::Invoke<void, 2137>(ped);
		}

		void TASK_EVERYONE_LEAVE_VEHICLE(Vehicle vehicle) {
			Invoker::Invoke<void, 2138>(vehicle);
		}

		void TASK_GOTO_ENTITY_OFFSET(Ped ped, Entity entity, int time, float seekRadius, float seekAngleDeg, float moveBlendRatio, int gotoEntityOffsetFlags) {
			Invoker::Invoke<void, 2139>(ped, entity, time, seekRadius, seekAngleDeg, moveBlendRatio, gotoEntityOffsetFlags);
		}

		void TASK_GOTO_ENTITY_OFFSET_XY(Ped ped, Entity entity, int duration, float targetRadius, float offsetX, float offsetY, float moveBlendRatio, int gotoEntityOffsetFlags) {
			Invoker::Invoke<void, 2140>(ped, entity, duration, targetRadius, offsetX, offsetY, moveBlendRatio, gotoEntityOffsetFlags);
		}

		void TASK_TURN_PED_TO_FACE_COORD(Ped ped, float x, float y, float z, int duration) {
			Invoker::Invoke<void, 2141>(ped, x, y, z, duration);
		}

		void TASK_VEHICLE_TEMP_ACTION(Ped driver, Vehicle vehicle, int action, int time) {
			Invoker::Invoke<void, 2142>(driver, vehicle, action, time);
		}

		void TASK_VEHICLE_MISSION(Ped driver, Vehicle vehicle, Vehicle vehicleTarget, int missionType, float cruiseSpeed, int drivingStyle, float targetReached, float straightLineDistance, BOOL DriveAgainstTraffic) {
			Invoker::Invoke<void, 2143>(driver, vehicle, vehicleTarget, missionType, cruiseSpeed, drivingStyle, targetReached, straightLineDistance, DriveAgainstTraffic);
		}

		void TASK_VEHICLE_MISSION_PED_TARGET(Ped ped, Vehicle vehicle, Ped pedTarget, int missionType, float maxSpeed, int drivingStyle, float minDistance, float straightLineDistance, BOOL DriveAgainstTraffic) {
			Invoker::Invoke<void, 2144>(ped, vehicle, pedTarget, missionType, maxSpeed, drivingStyle, minDistance, straightLineDistance, DriveAgainstTraffic);
		}

		void TASK_VEHICLE_MISSION_COORS_TARGET(Ped ped, Vehicle vehicle, float x, float y, float z, int mission, float cruiseSpeed, int drivingStyle, float targetReached, float straightLineDistance, BOOL DriveAgainstTraffic) {
			Invoker::Invoke<void, 2145>(ped, vehicle, x, y, z, mission, cruiseSpeed, drivingStyle, targetReached, straightLineDistance, DriveAgainstTraffic);
		}

		void TASK_VEHICLE_ESCORT(Ped ped, Vehicle vehicle, Vehicle targetVehicle, int mode, float speed, int drivingStyle, float minDistance, int minHeightAboveTerrain, float noRoadsDistance) {
			Invoker::Invoke<void, 2146>(ped, vehicle, targetVehicle, mode, speed, drivingStyle, minDistance, minHeightAboveTerrain, noRoadsDistance);
		}

		void TASK_VEHICLE_FOLLOW(Ped driver, Vehicle vehicle, Entity targetEntity, float speed, int drivingStyle, int minDistance) {
			Invoker::Invoke<void, 2147>(driver, vehicle, targetEntity, speed, drivingStyle, minDistance);
		}

		void TASK_VEHICLE_CHASE(Ped driver, Entity targetEnt) {
			Invoker::Invoke<void, 2148>(driver, targetEnt);
		}

		void TASK_VEHICLE_HELI_PROTECT(Ped pilot, Vehicle vehicle, Entity entityToFollow, float targetSpeed, int drivingFlags, float radius, int altitude, int heliFlags) {
			Invoker::Invoke<void, 2149>(pilot, vehicle, entityToFollow, targetSpeed, drivingFlags, radius, altitude, heliFlags);
		}

		void SET_TASK_VEHICLE_CHASE_BEHAVIOR_FLAG(Ped ped, int flag, BOOL set) {
			Invoker::Invoke<void, 2150>(ped, flag, set);
		}

		void SET_TASK_VEHICLE_CHASE_IDEAL_PURSUIT_DISTANCE(Ped ped, float distance) {
			Invoker::Invoke<void, 2151>(ped, distance);
		}

		void TASK_HELI_CHASE(Ped pilot, Entity entityToFollow, float x, float y, float z) {
			Invoker::Invoke<void, 2152>(pilot, entityToFollow, x, y, z);
		}

		void TASK_PLANE_CHASE(Ped pilot, Entity entityToFollow, float x, float y, float z) {
			Invoker::Invoke<void, 2153>(pilot, entityToFollow, x, y, z);
		}

		void TASK_PLANE_LAND(Ped pilot, Vehicle plane, float runwayStartX, float runwayStartY, float runwayStartZ, float runwayEndX, float runwayEndY, float runwayEndZ) {
			Invoker::Invoke<void, 2154>(pilot, plane, runwayStartX, runwayStartY, runwayStartZ, runwayEndX, runwayEndY, runwayEndZ);
		}

		void CLEAR_DEFAULT_PRIMARY_TASK(Ped ped) {
			Invoker::Invoke<void, 2155>(ped);
		}

		void CLEAR_PRIMARY_VEHICLE_TASK(Vehicle vehicle) {
			Invoker::Invoke<void, 2156>(vehicle);
		}

		void CLEAR_VEHICLE_CRASH_TASK(Vehicle vehicle) {
			Invoker::Invoke<void, 2157>(vehicle);
		}

		void TASK_PLANE_GOTO_PRECISE_VTOL(Ped ped, Vehicle vehicle, float x, float y, float z, int flightHeight, int minHeightAboveTerrain, BOOL useDesiredOrientation, float desiredOrientation, BOOL autopilot) {
			Invoker::Invoke<void, 2158>(ped, vehicle, x, y, z, flightHeight, minHeightAboveTerrain, useDesiredOrientation, desiredOrientation, autopilot);
		}

		void TASK_SUBMARINE_GOTO_AND_STOP(Ped ped, Vehicle submarine, float x, float y, float z, BOOL autopilot) {
			Invoker::Invoke<void, 2159>(ped, submarine, x, y, z, autopilot);
		}

		void TASK_HELI_MISSION(Ped pilot, Vehicle aircraft, Vehicle targetVehicle, Ped targetPed, float destinationX, float destinationY, float destinationZ, int missionFlag, float maxSpeed, float radius, float targetHeading, int maxHeight, int minHeight, float slowDownDistance, int behaviorFlags) {
			Invoker::Invoke<void, 2160>(pilot, aircraft, targetVehicle, targetPed, destinationX, destinationY, destinationZ, missionFlag, maxSpeed, radius, targetHeading, maxHeight, minHeight, slowDownDistance, behaviorFlags);
		}

		void TASK_HELI_ESCORT_HELI(Ped pilot, Vehicle heli1, Vehicle heli2, float offsetX, float offsetY, float offsetZ) {
			Invoker::Invoke<void, 2161>(pilot, heli1, heli2, offsetX, offsetY, offsetZ);
		}

		void TASK_PLANE_MISSION(Ped pilot, Vehicle aircraft, Vehicle targetVehicle, Ped targetPed, float destinationX, float destinationY, float destinationZ, int missionFlag, float angularDrag, float targetReached, float targetHeading, float maxZ, float minZ, BOOL precise) {
			Invoker::Invoke<void, 2162>(pilot, aircraft, targetVehicle, targetPed, destinationX, destinationY, destinationZ, missionFlag, angularDrag, targetReached, targetHeading, maxZ, minZ, precise);
		}

		void TASK_PLANE_TAXI(Ped pilot, Vehicle aircraft, float x, float y, float z, float cruiseSpeed, float targetReached) {
			Invoker::Invoke<void, 2163>(pilot, aircraft, x, y, z, cruiseSpeed, targetReached);
		}

		void TASK_BOAT_MISSION(Ped pedDriver, Vehicle vehicle, Vehicle targetVehicle, Ped targetPed, float x, float y, float z, int mission, float maxSpeed, int drivingStyle, float targetReached, Any boatFlags) {
			Invoker::Invoke<void, 2164>(pedDriver, vehicle, targetVehicle, targetPed, x, y, z, mission, maxSpeed, drivingStyle, targetReached, boatFlags);
		}

		void TASK_DRIVE_BY(Ped driverPed, Ped targetPed, Vehicle targetVehicle, float targetX, float targetY, float targetZ, float distanceToShoot, int pedAccuracy, BOOL pushUnderneathDrivingTaskIfDriving, Hash firingPattern) {
			Invoker::Invoke<void, 2165>(driverPed, targetPed, targetVehicle, targetX, targetY, targetZ, distanceToShoot, pedAccuracy, pushUnderneathDrivingTaskIfDriving, firingPattern);
		}

		void SET_DRIVEBY_TASK_TARGET(Ped shootingPed, Ped targetPed, Vehicle targetVehicle, float x, float y, float z) {
			Invoker::Invoke<void, 2166>(shootingPed, targetPed, targetVehicle, x, y, z);
		}

		void CLEAR_DRIVEBY_TASK_UNDERNEATH_DRIVING_TASK(Ped ped) {
			Invoker::Invoke<void, 2167>(ped);
		}

		BOOL IS_DRIVEBY_TASK_UNDERNEATH_DRIVING_TASK(Ped ped) {
			return Invoker::Invoke<BOOL, 2168>(ped);
		}

		BOOL CONTROL_MOUNTED_WEAPON(Ped ped) {
			return Invoker::Invoke<BOOL, 2169>(ped);
		}

		void SET_MOUNTED_WEAPON_TARGET(Ped shootingPed, Ped targetPed, Vehicle targetVehicle, float x, float y, float z, int taskMode, BOOL ignoreTargetVehDeadCheck) {
			Invoker::Invoke<void, 2170>(shootingPed, targetPed, targetVehicle, x, y, z, taskMode, ignoreTargetVehDeadCheck);
		}

		BOOL IS_MOUNTED_WEAPON_TASK_UNDERNEATH_DRIVING_TASK(Ped ped) {
			return Invoker::Invoke<BOOL, 2171>(ped);
		}

		void TASK_USE_MOBILE_PHONE(Ped ped, BOOL usePhone, int desiredPhoneMode) {
			Invoker::Invoke<void, 2172>(ped, usePhone, desiredPhoneMode);
		}

		void TASK_USE_MOBILE_PHONE_TIMED(Ped ped, int duration) {
			Invoker::Invoke<void, 2173>(ped, duration);
		}

		void TASK_CHAT_TO_PED(Ped ped, Ped target, int flags, float goToLocationX, float goToLocationY, float goToLocationZ, float headingDegs, float idleTime) {
			Invoker::Invoke<void, 2174>(ped, target, flags, goToLocationX, goToLocationY, goToLocationZ, headingDegs, idleTime);
		}

		void TASK_WARP_PED_INTO_VEHICLE(Ped ped, Vehicle vehicle, int seat) {
			Invoker::Invoke<void, 2175>(ped, vehicle, seat);
		}

		void TASK_SHOOT_AT_ENTITY(Entity entity, Entity target, int duration, Hash firingPattern) {
			Invoker::Invoke<void, 2176>(entity, target, duration, firingPattern);
		}

		void TASK_CLIMB(Ped ped, BOOL usePlayerLaunchForce) {
			Invoker::Invoke<void, 2177>(ped, usePlayerLaunchForce);
		}

		void TASK_CLIMB_LADDER(Ped ped, BOOL fast) {
			Invoker::Invoke<void, 2178>(ped, fast);
		}

		void TASK_RAPPEL_DOWN_WALL_USING_CLIPSET_OVERRIDE(Ped ped, float x1, float y1, float z1, float x2, float y2, float z2, float minZ, int ropeHandle, const char* clipSet, Any p10, Any p11) {
			Invoker::Invoke<void, 2179>(ped, x1, y1, z1, x2, y2, z2, minZ, ropeHandle, clipSet, p10, p11);
		}

		int GET_TASK_RAPPEL_DOWN_WALL_STATE(Ped ped) {
			return Invoker::Invoke<int, 2180>(ped);
		}

		void CLEAR_PED_TASKS_IMMEDIATELY(Ped ped) {
			Invoker::Invoke<void, 2181>(ped);
		}

		void TASK_PERFORM_SEQUENCE_FROM_PROGRESS(Ped ped, int taskIndex, int progress1, int progress2) {
			Invoker::Invoke<void, 2182>(ped, taskIndex, progress1, progress2);
		}

		void SET_NEXT_DESIRED_MOVE_STATE(float nextMoveState) {
			Invoker::Invoke<void, 2183>(nextMoveState);
		}

		void SET_PED_DESIRED_MOVE_BLEND_RATIO(Ped ped, float newMoveBlendRatio) {
			Invoker::Invoke<void, 2184>(ped, newMoveBlendRatio);
		}

		float GET_PED_DESIRED_MOVE_BLEND_RATIO(Ped ped) {
			return Invoker::Invoke<float, 2185>(ped);
		}

		void TASK_GOTO_ENTITY_AIMING(Ped ped, Entity target, float distanceToStopAt, float StartAimingDist) {
			Invoker::Invoke<void, 2186>(ped, target, distanceToStopAt, StartAimingDist);
		}

		void TASK_SET_DECISION_MAKER(Ped ped, Hash decisionMakerId) {
			Invoker::Invoke<void, 2187>(ped, decisionMakerId);
		}

		void TASK_SET_SPHERE_DEFENSIVE_AREA(Ped ped, float x, float y, float z, float radius) {
			Invoker::Invoke<void, 2188>(ped, x, y, z, radius);
		}

		void TASK_CLEAR_DEFENSIVE_AREA(Ped ped) {
			Invoker::Invoke<void, 2189>(ped);
		}

		void TASK_PED_SLIDE_TO_COORD(Ped ped, float x, float y, float z, float heading, float speed) {
			Invoker::Invoke<void, 2190>(ped, x, y, z, heading, speed);
		}

		void TASK_PED_SLIDE_TO_COORD_HDG_RATE(Ped ped, float x, float y, float z, float heading, float speed, float headingChangeRate) {
			Invoker::Invoke<void, 2191>(ped, x, y, z, heading, speed, headingChangeRate);
		}

		ScrHandle ADD_COVER_POINT(float x, float y, float z, float direction, int usage, int height, int arc, BOOL isPriority) {
			return Invoker::Invoke<ScrHandle, 2192>(x, y, z, direction, usage, height, arc, isPriority);
		}

		void REMOVE_COVER_POINT(ScrHandle coverpoint) {
			Invoker::Invoke<void, 2193>(coverpoint);
		}

		BOOL DOES_SCRIPTED_COVER_POINT_EXIST_AT_COORDS(float x, float y, float z) {
			return Invoker::Invoke<BOOL, 2194>(x, y, z);
		}

		Vector3 GET_SCRIPTED_COVER_POINT_COORDS(ScrHandle coverpoint) {
			return Invoker::Invoke<Vector3, 2195>(coverpoint);
		}

		void ADD_SCRIPTED_COVER_AREA(float x, float y, float z, float radius) {
			Invoker::Invoke<void, 2196>(x, y, z, radius);
		}

		void TASK_COMBAT_PED(Ped ped, Ped targetPed, int combatFlags, int threatResponseFlags) {
			Invoker::Invoke<void, 2197>(ped, targetPed, combatFlags, threatResponseFlags);
		}

		void TASK_COMBAT_PED_TIMED(Ped ped, Ped target, int time, int flags) {
			Invoker::Invoke<void, 2198>(ped, target, time, flags);
		}

		void TASK_SEEK_COVER_FROM_POS(Ped ped, float x, float y, float z, int duration, BOOL allowPeekingAndFiring) {
			Invoker::Invoke<void, 2199>(ped, x, y, z, duration, allowPeekingAndFiring);
		}

		void TASK_SEEK_COVER_FROM_PED(Ped ped, Ped target, int duration, BOOL allowPeekingAndFiring) {
			Invoker::Invoke<void, 2200>(ped, target, duration, allowPeekingAndFiring);
		}

		void TASK_SEEK_COVER_TO_COVER_POINT(Ped ped, ScrHandle coverpoint, float x, float y, float z, int time, BOOL allowPeekingAndFiring) {
			Invoker::Invoke<void, 2201>(ped, coverpoint, x, y, z, time, allowPeekingAndFiring);
		}

		void TASK_SEEK_COVER_TO_COORDS(Ped ped, float x1, float y1, float z1, float x2, float y2, float z2, int timeout, BOOL shortRoute) {
			Invoker::Invoke<void, 2202>(ped, x1, y1, z1, x2, y2, z2, timeout, shortRoute);
		}

		void TASK_PUT_PED_DIRECTLY_INTO_COVER(Ped ped, float x, float y, float z, int time, BOOL allowPeekingAndFiring, float blendInDuration, BOOL forceInitialFacingDirection, BOOL forceFaceLeft, int identifier, BOOL doEntry) {
			Invoker::Invoke<void, 2203>(ped, x, y, z, time, allowPeekingAndFiring, blendInDuration, forceInitialFacingDirection, forceFaceLeft, identifier, doEntry);
		}

		void TASK_WARP_PED_DIRECTLY_INTO_COVER(Ped ped, int time, BOOL allowPeekingAndFiring, BOOL forceInitialFacingDirection, BOOL forceFaceLeft, int identifier) {
			Invoker::Invoke<void, 2204>(ped, time, allowPeekingAndFiring, forceInitialFacingDirection, forceFaceLeft, identifier);
		}

		void TASK_EXIT_COVER(Ped ped, int exitType, float x, float y, float z) {
			Invoker::Invoke<void, 2205>(ped, exitType, x, y, z);
		}

		void TASK_PUT_PED_DIRECTLY_INTO_MELEE(Ped ped, Ped meleeTarget, float blendInDuration, float timeInMelee, float strafePhaseSync, int aiCombatFlags) {
			Invoker::Invoke<void, 2206>(ped, meleeTarget, blendInDuration, timeInMelee, strafePhaseSync, aiCombatFlags);
		}

		void TASK_TOGGLE_DUCK(Ped ped, int toggleType) {
			Invoker::Invoke<void, 2207>(ped, toggleType);
		}

		void TASK_GUARD_CURRENT_POSITION(Ped ped, float maxPatrolProximity, float defensiveAreaRadius, BOOL setDefensiveArea) {
			Invoker::Invoke<void, 2208>(ped, maxPatrolProximity, defensiveAreaRadius, setDefensiveArea);
		}

		void TASK_GUARD_ASSIGNED_DEFENSIVE_AREA(Ped ped, float x, float y, float z, float heading, float maxPatrolProximity, int timer) {
			Invoker::Invoke<void, 2209>(ped, x, y, z, heading, maxPatrolProximity, timer);
		}

		void TASK_GUARD_SPHERE_DEFENSIVE_AREA(Ped ped, float defendPositionX, float defendPositionY, float defendPositionZ, float heading, float maxPatrolProximity, int time, float x, float y, float z, float defensiveAreaRadius) {
			Invoker::Invoke<void, 2210>(ped, defendPositionX, defendPositionY, defendPositionZ, heading, maxPatrolProximity, time, x, y, z, defensiveAreaRadius);
		}

		void TASK_STAND_GUARD(Ped ped, float x, float y, float z, float heading, const char* scenarioName) {
			Invoker::Invoke<void, 2211>(ped, x, y, z, heading, scenarioName);
		}

		void SET_DRIVE_TASK_CRUISE_SPEED(Ped driver, float cruiseSpeed) {
			Invoker::Invoke<void, 2212>(driver, cruiseSpeed);
		}

		void SET_DRIVE_TASK_MAX_CRUISE_SPEED(Ped ped, float speed, BOOL updateBaseTask) {
			Invoker::Invoke<void, 2213>(ped, speed, updateBaseTask);
		}

		void SET_DRIVE_TASK_DRIVING_STYLE(Ped ped, int drivingStyle) {
			Invoker::Invoke<void, 2214>(ped, drivingStyle);
		}

		void ADD_COVER_BLOCKING_AREA(float startX, float startY, float startZ, float endX, float endY, float endZ, BOOL blockObjects, BOOL blockVehicles, BOOL blockMap, BOOL blockPlayer) {
			Invoker::Invoke<void, 2215>(startX, startY, startZ, endX, endY, endZ, blockObjects, blockVehicles, blockMap, blockPlayer);
		}

		void REMOVE_ALL_COVER_BLOCKING_AREAS() {
			Invoker::Invoke<void, 2216>();
		}

		void REMOVE_COVER_BLOCKING_AREAS_AT_POSITION(float x, float y, float z) {
			Invoker::Invoke<void, 2217>(x, y, z);
		}

		void REMOVE_SPECIFIC_COVER_BLOCKING_AREAS(float startX, float startY, float startZ, float endX, float endY, float endZ, BOOL blockObjects, BOOL blockVehicles, BOOL blockMap, BOOL blockPlayer) {
			Invoker::Invoke<void, 2218>(startX, startY, startZ, endX, endY, endZ, blockObjects, blockVehicles, blockMap, blockPlayer);
		}

		void TASK_START_SCENARIO_IN_PLACE(Ped ped, const char* scenarioName, int unkDelay, BOOL playEnterAnim) {
			Invoker::Invoke<void, 2219>(ped, scenarioName, unkDelay, playEnterAnim);
		}

		void TASK_START_SCENARIO_AT_POSITION(Ped ped, const char* scenarioName, float x, float y, float z, float heading, int duration, BOOL sittingScenario, BOOL teleport) {
			Invoker::Invoke<void, 2220>(ped, scenarioName, x, y, z, heading, duration, sittingScenario, teleport);
		}

		void TASK_USE_NEAREST_SCENARIO_TO_COORD(Ped ped, float x, float y, float z, float distance, int duration) {
			Invoker::Invoke<void, 2221>(ped, x, y, z, distance, duration);
		}

		void TASK_USE_NEAREST_SCENARIO_TO_COORD_WARP(Ped ped, float x, float y, float z, float radius, int timeToLeave) {
			Invoker::Invoke<void, 2222>(ped, x, y, z, radius, timeToLeave);
		}

		void TASK_USE_NEAREST_SCENARIO_CHAIN_TO_COORD(Ped ped, float x, float y, float z, float maxRange, int timeToLeave) {
			Invoker::Invoke<void, 2223>(ped, x, y, z, maxRange, timeToLeave);
		}

		void TASK_USE_NEAREST_SCENARIO_CHAIN_TO_COORD_WARP(Ped ped, float x, float y, float z, float radius, int timeToLeave) {
			Invoker::Invoke<void, 2224>(ped, x, y, z, radius, timeToLeave);
		}

		BOOL DOES_SCENARIO_EXIST_IN_AREA(float x, float y, float z, float radius, BOOL mustBeFree) {
			return Invoker::Invoke<BOOL, 2225>(x, y, z, radius, mustBeFree);
		}

		BOOL DOES_SCENARIO_OF_TYPE_EXIST_IN_AREA(float x, float y, float z, const char* scenarioName, float radius, BOOL mustBeFree) {
			return Invoker::Invoke<BOOL, 2226>(x, y, z, scenarioName, radius, mustBeFree);
		}

		BOOL IS_SCENARIO_OCCUPIED(float x, float y, float z, float maxRange, BOOL onlyUsersActuallyAtScenario) {
			return Invoker::Invoke<BOOL, 2227>(x, y, z, maxRange, onlyUsersActuallyAtScenario);
		}

		BOOL PED_HAS_USE_SCENARIO_TASK(Ped ped) {
			return Invoker::Invoke<BOOL, 2228>(ped);
		}

		void PLAY_ANIM_ON_RUNNING_SCENARIO(Ped ped, const char* animDict, const char* animName) {
			Invoker::Invoke<void, 2229>(ped, animDict, animName);
		}

		BOOL DOES_SCENARIO_GROUP_EXIST(const char* scenarioGroup) {
			return Invoker::Invoke<BOOL, 2230>(scenarioGroup);
		}

		BOOL IS_SCENARIO_GROUP_ENABLED(const char* scenarioGroup) {
			return Invoker::Invoke<BOOL, 2231>(scenarioGroup);
		}

		void SET_SCENARIO_GROUP_ENABLED(const char* scenarioGroup, BOOL enabled) {
			Invoker::Invoke<void, 2232>(scenarioGroup, enabled);
		}

		void RESET_SCENARIO_GROUPS_ENABLED() {
			Invoker::Invoke<void, 2233>();
		}

		void SET_EXCLUSIVE_SCENARIO_GROUP(const char* scenarioGroup) {
			Invoker::Invoke<void, 2234>(scenarioGroup);
		}

		void RESET_EXCLUSIVE_SCENARIO_GROUP() {
			Invoker::Invoke<void, 2235>();
		}

		BOOL IS_SCENARIO_TYPE_ENABLED(const char* scenarioType) {
			return Invoker::Invoke<BOOL, 2236>(scenarioType);
		}

		void SET_SCENARIO_TYPE_ENABLED(const char* scenarioType, BOOL toggle) {
			Invoker::Invoke<void, 2237>(scenarioType, toggle);
		}

		void RESET_SCENARIO_TYPES_ENABLED() {
			Invoker::Invoke<void, 2238>();
		}

		BOOL IS_PED_ACTIVE_IN_SCENARIO(Ped ped) {
			return Invoker::Invoke<BOOL, 2239>(ped);
		}

		BOOL IS_PED_PLAYING_BASE_CLIP_IN_SCENARIO(Ped ped) {
			return Invoker::Invoke<BOOL, 2240>(ped);
		}

		void SET_PED_CAN_PLAY_AMBIENT_IDLES(Ped ped, BOOL blockIdleClips, BOOL removeIdleClipIfPlaying) {
			Invoker::Invoke<void, 2241>(ped, blockIdleClips, removeIdleClipIfPlaying);
		}

		void TASK_COMBAT_HATED_TARGETS_IN_AREA(Ped ped, float x, float y, float z, float radius, int combatFlags) {
			Invoker::Invoke<void, 2242>(ped, x, y, z, radius, combatFlags);
		}

		void TASK_COMBAT_HATED_TARGETS_AROUND_PED(Ped ped, float radius, int combatFlags) {
			Invoker::Invoke<void, 2243>(ped, radius, combatFlags);
		}

		void TASK_COMBAT_HATED_TARGETS_AROUND_PED_TIMED(Ped ped, float radius, int time, int combatFlags) {
			Invoker::Invoke<void, 2244>(ped, radius, time, combatFlags);
		}

		void TASK_THROW_PROJECTILE(Ped ped, float x, float y, float z, int ignoreCollisionEntityIndex, BOOL createInvincibleProjectile) {
			Invoker::Invoke<void, 2245>(ped, x, y, z, ignoreCollisionEntityIndex, createInvincibleProjectile);
		}

		void TASK_SWAP_WEAPON(Ped ped, BOOL drawWeapon) {
			Invoker::Invoke<void, 2246>(ped, drawWeapon);
		}

		void TASK_RELOAD_WEAPON(Ped ped, BOOL drawWeapon) {
			Invoker::Invoke<void, 2247>(ped, drawWeapon);
		}

		BOOL IS_PED_GETTING_UP(Ped ped) {
			return Invoker::Invoke<BOOL, 2248>(ped);
		}

		void TASK_WRITHE(Ped ped, Ped target, int minFireLoops, int startState, BOOL forceShootOnGround, int shootFromGroundTimer) {
			Invoker::Invoke<void, 2249>(ped, target, minFireLoops, startState, forceShootOnGround, shootFromGroundTimer);
		}

		BOOL IS_PED_IN_WRITHE(Ped ped) {
			return Invoker::Invoke<BOOL, 2250>(ped);
		}

		void OPEN_PATROL_ROUTE(const char* patrolRoute) {
			Invoker::Invoke<void, 2251>(patrolRoute);
		}

		void CLOSE_PATROL_ROUTE() {
			Invoker::Invoke<void, 2252>();
		}

		void ADD_PATROL_ROUTE_NODE(int nodeId, const char* nodeType, float posX, float posY, float posZ, float headingX, float headingY, float headingZ, int duration) {
			Invoker::Invoke<void, 2253>(nodeId, nodeType, posX, posY, posZ, headingX, headingY, headingZ, duration);
		}

		void ADD_PATROL_ROUTE_LINK(int nodeId1, int nodeId2) {
			Invoker::Invoke<void, 2254>(nodeId1, nodeId2);
		}

		void CREATE_PATROL_ROUTE() {
			Invoker::Invoke<void, 2255>();
		}

		void DELETE_PATROL_ROUTE(const char* patrolRoute) {
			Invoker::Invoke<void, 2256>(patrolRoute);
		}

		BOOL GET_PATROL_TASK_INFO(Ped ped, int* timeLeftAtNode, int* nodeId) {
			return Invoker::Invoke<BOOL, 2257>(ped, timeLeftAtNode, nodeId);
		}

		void TASK_PATROL(Ped ped, const char* patrolRouteName, int alertState, BOOL canChatToPeds, BOOL useHeadLookAt) {
			Invoker::Invoke<void, 2258>(ped, patrolRouteName, alertState, canChatToPeds, useHeadLookAt);
		}

		void TASK_STAY_IN_COVER(Ped ped) {
			Invoker::Invoke<void, 2259>(ped);
		}

		void ADD_VEHICLE_SUBTASK_ATTACK_COORD(Ped ped, float x, float y, float z) {
			Invoker::Invoke<void, 2260>(ped, x, y, z);
		}

		void ADD_VEHICLE_SUBTASK_ATTACK_PED(Ped ped, Ped target) {
			Invoker::Invoke<void, 2261>(ped, target);
		}

		void TASK_VEHICLE_SHOOT_AT_PED(Ped ped, Ped target, float fireTolerance) {
			Invoker::Invoke<void, 2262>(ped, target, fireTolerance);
		}

		void TASK_VEHICLE_AIM_AT_PED(Ped ped, Ped target) {
			Invoker::Invoke<void, 2263>(ped, target);
		}

		void TASK_VEHICLE_SHOOT_AT_COORD(Ped ped, float x, float y, float z, float fireTolerance) {
			Invoker::Invoke<void, 2264>(ped, x, y, z, fireTolerance);
		}

		void TASK_VEHICLE_AIM_AT_COORD(Ped ped, float x, float y, float z) {
			Invoker::Invoke<void, 2265>(ped, x, y, z);
		}

		void TASK_VEHICLE_GOTO_NAVMESH(Ped ped, Vehicle vehicle, float x, float y, float z, float speed, int behaviorFlag, float stoppingRange) {
			Invoker::Invoke<void, 2266>(ped, vehicle, x, y, z, speed, behaviorFlag, stoppingRange);
		}

		void TASK_GO_TO_COORD_WHILE_AIMING_AT_COORD(Ped ped, float x, float y, float z, float aimAtX, float aimAtY, float aimAtZ, float moveBlendRatio, BOOL shoot, float targetRadius, float slowDistance, BOOL useNavMesh, int navFlags, BOOL instantBlendToAim, Hash firingPattern) {
			Invoker::Invoke<void, 2267>(ped, x, y, z, aimAtX, aimAtY, aimAtZ, moveBlendRatio, shoot, targetRadius, slowDistance, useNavMesh, navFlags, instantBlendToAim, firingPattern);
		}

		void TASK_GO_TO_COORD_WHILE_AIMING_AT_ENTITY(Ped ped, float x, float y, float z, Entity aimAtID, float moveBlendRatio, BOOL shoot, float targetRadius, float slowDistance, BOOL useNavMesh, int navFlags, BOOL instantBlendToAim, Hash firingPattern, int time) {
			Invoker::Invoke<void, 2268>(ped, x, y, z, aimAtID, moveBlendRatio, shoot, targetRadius, slowDistance, useNavMesh, navFlags, instantBlendToAim, firingPattern, time);
		}

		void TASK_GO_TO_COORD_AND_AIM_AT_HATED_ENTITIES_NEAR_COORD(Ped pedHandle, float goToLocationX, float goToLocationY, float goToLocationZ, float focusLocationX, float focusLocationY, float focusLocationZ, float speed, BOOL shootAtEnemies, float distanceToStopAt, float noRoadsDistance, BOOL useNavMesh, int navFlags, int taskFlags, Hash firingPattern) {
			Invoker::Invoke<void, 2269>(pedHandle, goToLocationX, goToLocationY, goToLocationZ, focusLocationX, focusLocationY, focusLocationZ, speed, shootAtEnemies, distanceToStopAt, noRoadsDistance, useNavMesh, navFlags, taskFlags, firingPattern);
		}

		void TASK_GO_TO_ENTITY_WHILE_AIMING_AT_COORD(Ped ped, Entity entity, float aimX, float aimY, float aimZ, float moveBlendRatio, BOOL shoot, float targetRadius, float slowDistance, BOOL useNavMesh, BOOL instantBlendToAim, Hash firingPattern) {
			Invoker::Invoke<void, 2270>(ped, entity, aimX, aimY, aimZ, moveBlendRatio, shoot, targetRadius, slowDistance, useNavMesh, instantBlendToAim, firingPattern);
		}

		void TASK_GO_TO_ENTITY_WHILE_AIMING_AT_ENTITY(Ped ped, Entity entityToWalkTo, Entity entityToAimAt, float speed, BOOL shootatEntity, float targetRadius, float slowDistance, BOOL useNavMesh, BOOL instantBlendToAim, Hash firingPattern) {
			Invoker::Invoke<void, 2271>(ped, entityToWalkTo, entityToAimAt, speed, shootatEntity, targetRadius, slowDistance, useNavMesh, instantBlendToAim, firingPattern);
		}

		void SET_HIGH_FALL_TASK(Ped ped, int minTime, int maxTime, int entryType) {
			Invoker::Invoke<void, 2272>(ped, minTime, maxTime, entryType);
		}

		void REQUEST_WAYPOINT_RECORDING(const char* name) {
			Invoker::Invoke<void, 2273>(name);
		}

		BOOL GET_IS_WAYPOINT_RECORDING_LOADED(const char* name) {
			return Invoker::Invoke<BOOL, 2274>(name);
		}

		void REMOVE_WAYPOINT_RECORDING(const char* name) {
			Invoker::Invoke<void, 2275>(name);
		}

		BOOL WAYPOINT_RECORDING_GET_NUM_POINTS(const char* name, int* points) {
			return Invoker::Invoke<BOOL, 2276>(name, points);
		}

		BOOL WAYPOINT_RECORDING_GET_COORD(const char* name, int point, Vector3* coord) {
			return Invoker::Invoke<BOOL, 2277>(name, point, coord);
		}

		float WAYPOINT_RECORDING_GET_SPEED_AT_POINT(const char* name, int point) {
			return Invoker::Invoke<float, 2278>(name, point);
		}

		BOOL WAYPOINT_RECORDING_GET_CLOSEST_WAYPOINT(const char* name, float x, float y, float z, int* point) {
			return Invoker::Invoke<BOOL, 2279>(name, x, y, z, point);
		}

		void TASK_FOLLOW_WAYPOINT_RECORDING(Ped ped, const char* name, int p2, int p3, int p4) {
			Invoker::Invoke<void, 2280>(ped, name, p2, p3, p4);
		}

		BOOL IS_WAYPOINT_PLAYBACK_GOING_ON_FOR_PED(Ped ped) {
			return Invoker::Invoke<BOOL, 2281>(ped);
		}

		int GET_PED_WAYPOINT_PROGRESS(Ped ped) {
			return Invoker::Invoke<int, 2282>(ped);
		}

		void SET_PED_WAYPOINT_PROGRESS(Ped ped, int progress) {
			Invoker::Invoke<void, 6707>(ped, progress);
		}

		float GET_PED_WAYPOINT_DISTANCE(Any p0) {
			return Invoker::Invoke<float, 2283>(p0);
		}

		BOOL SET_PED_WAYPOINT_ROUTE_OFFSET(Ped ped, float x, float y, float z) {
			return Invoker::Invoke<BOOL, 2284>(ped, x, y, z);
		}

		float GET_WAYPOINT_DISTANCE_ALONG_ROUTE(const char* name, int point) {
			return Invoker::Invoke<float, 2285>(name, point);
		}

		BOOL WAYPOINT_PLAYBACK_GET_IS_PAUSED(Any p0) {
			return Invoker::Invoke<BOOL, 2286>(p0);
		}

		void WAYPOINT_PLAYBACK_PAUSE(Any p0, BOOL p1, BOOL p2) {
			Invoker::Invoke<void, 2287>(p0, p1, p2);
		}

		void WAYPOINT_PLAYBACK_RESUME(Any p0, BOOL p1, Any p2, Any p3) {
			Invoker::Invoke<void, 2288>(p0, p1, p2, p3);
		}

		void WAYPOINT_PLAYBACK_OVERRIDE_SPEED(Any p0, float p1, BOOL p2) {
			Invoker::Invoke<void, 2289>(p0, p1, p2);
		}

		void WAYPOINT_PLAYBACK_USE_DEFAULT_SPEED(Any p0) {
			Invoker::Invoke<void, 2290>(p0);
		}

		void USE_WAYPOINT_RECORDING_AS_ASSISTED_MOVEMENT_ROUTE(const char* name, BOOL p1, float p2, float p3) {
			Invoker::Invoke<void, 2291>(name, p1, p2, p3);
		}

		void WAYPOINT_PLAYBACK_START_AIMING_AT_PED(Ped ped, Ped target, BOOL p2) {
			Invoker::Invoke<void, 2292>(ped, target, p2);
		}

		void WAYPOINT_PLAYBACK_START_AIMING_AT_COORD(Ped ped, float x, float y, float z, BOOL p4) {
			Invoker::Invoke<void, 2293>(ped, x, y, z, p4);
		}

		void WAYPOINT_PLAYBACK_START_SHOOTING_AT_PED(Ped ped, Ped ped2, BOOL p2, BOOL p3) {
			Invoker::Invoke<void, 2294>(ped, ped2, p2, p3);
		}

		void WAYPOINT_PLAYBACK_START_SHOOTING_AT_COORD(Ped ped, float x, float y, float z, BOOL p4, Hash firingPattern) {
			Invoker::Invoke<void, 2295>(ped, x, y, z, p4, firingPattern);
		}

		void WAYPOINT_PLAYBACK_STOP_AIMING_OR_SHOOTING(Ped ped) {
			Invoker::Invoke<void, 2296>(ped);
		}

		void ASSISTED_MOVEMENT_REQUEST_ROUTE(const char* route) {
			Invoker::Invoke<void, 2297>(route);
		}

		void ASSISTED_MOVEMENT_REMOVE_ROUTE(const char* route) {
			Invoker::Invoke<void, 2298>(route);
		}

		BOOL ASSISTED_MOVEMENT_IS_ROUTE_LOADED(const char* route) {
			return Invoker::Invoke<BOOL, 2299>(route);
		}

		void ASSISTED_MOVEMENT_SET_ROUTE_PROPERTIES(const char* route, int props) {
			Invoker::Invoke<void, 2300>(route, props);
		}

		void ASSISTED_MOVEMENT_OVERRIDE_LOAD_DISTANCE_THIS_FRAME(float dist) {
			Invoker::Invoke<void, 2301>(dist);
		}

		void TASK_VEHICLE_FOLLOW_WAYPOINT_RECORDING(Ped ped, Vehicle vehicle, const char* WPRecording, int p3, int p4, int p5, int p6, float p7, BOOL p8, float p9) {
			Invoker::Invoke<void, 2302>(ped, vehicle, WPRecording, p3, p4, p5, p6, p7, p8, p9);
		}

		BOOL IS_WAYPOINT_PLAYBACK_GOING_ON_FOR_VEHICLE(Vehicle vehicle) {
			return Invoker::Invoke<BOOL, 2303>(vehicle);
		}

		int GET_VEHICLE_WAYPOINT_PROGRESS(Vehicle vehicle) {
			return Invoker::Invoke<int, 2304>(vehicle);
		}

		int GET_VEHICLE_WAYPOINT_TARGET_POINT(Vehicle vehicle) {
			return Invoker::Invoke<int, 2305>(vehicle);
		}

		void VEHICLE_WAYPOINT_PLAYBACK_PAUSE(Vehicle vehicle) {
			Invoker::Invoke<void, 2306>(vehicle);
		}

		BOOL VEHICLE_WAYPOINT_PLAYBACK_GET_IS_PAUSED(Vehicle vehicle) {
			return Invoker::Invoke<BOOL, 6708>(vehicle);
		}

		void VEHICLE_WAYPOINT_PLAYBACK_RESUME(Vehicle vehicle) {
			Invoker::Invoke<void, 2307>(vehicle);
		}

		void VEHICLE_WAYPOINT_PLAYBACK_USE_DEFAULT_SPEED(Vehicle vehicle) {
			Invoker::Invoke<void, 2308>(vehicle);
		}

		void VEHICLE_WAYPOINT_PLAYBACK_OVERRIDE_SPEED(Vehicle vehicle, float speed) {
			Invoker::Invoke<void, 2309>(vehicle, speed);
		}

		void TASK_SET_BLOCKING_OF_NON_TEMPORARY_EVENTS(Ped ped, BOOL toggle) {
			Invoker::Invoke<void, 2310>(ped, toggle);
		}

		void TASK_FORCE_MOTION_STATE(Ped ped, Hash state, BOOL forceRestart) {
			Invoker::Invoke<void, 2311>(ped, state, forceRestart);
		}

		void TASK_MOVE_NETWORK_BY_NAME(Ped ped, const char* task, float multiplier, BOOL allowOverrideCloneUpdate, const char* animDict, int flags) {
			Invoker::Invoke<void, 2312>(ped, task, multiplier, allowOverrideCloneUpdate, animDict, flags);
		}

		void TASK_MOVE_NETWORK_ADVANCED_BY_NAME(Ped ped, const char* network, float x, float y, float z, float rotX, float rotY, float rotZ, int rotOrder, float blendDuration, BOOL allowOverrideCloneUpdate, const char* animDict, int flags) {
			Invoker::Invoke<void, 2313>(ped, network, x, y, z, rotX, rotY, rotZ, rotOrder, blendDuration, allowOverrideCloneUpdate, animDict, flags);
		}

		void TASK_MOVE_NETWORK_BY_NAME_WITH_INIT_PARAMS(Ped ped, const char* network, int* initialParameters, float blendDuration, BOOL allowOverrideCloneUpdate, const char* animDict, int flags) {
			Invoker::Invoke<void, 2314>(ped, network, initialParameters, blendDuration, allowOverrideCloneUpdate, animDict, flags);
		}

		void TASK_MOVE_NETWORK_ADVANCED_BY_NAME_WITH_INIT_PARAMS(Ped ped, const char* network, int* initialParameters, float x, float y, float z, float rotX, float rotY, float rotZ, int rotOrder, float blendDuration, BOOL allowOverrideCloneUpdate, const char* dictionary, int flags) {
			Invoker::Invoke<void, 2315>(ped, network, initialParameters, x, y, z, rotX, rotY, rotZ, rotOrder, blendDuration, allowOverrideCloneUpdate, dictionary, flags);
		}

		BOOL IS_TASK_MOVE_NETWORK_ACTIVE(Ped ped) {
			return Invoker::Invoke<BOOL, 2316>(ped);
		}

		BOOL IS_TASK_MOVE_NETWORK_READY_FOR_TRANSITION(Ped ped) {
			return Invoker::Invoke<BOOL, 2317>(ped);
		}

		BOOL REQUEST_TASK_MOVE_NETWORK_STATE_TRANSITION(Ped ped, const char* name) {
			return Invoker::Invoke<BOOL, 2318>(ped, name);
		}

		BOOL SET_EXPECTED_CLONE_NEXT_TASK_MOVE_NETWORK_STATE(Ped ped, const char* state) {
			return Invoker::Invoke<BOOL, 2319>(ped, state);
		}

		const char* GET_TASK_MOVE_NETWORK_STATE(Ped ped) {
			return Invoker::Invoke<const char*, 2320>(ped);
		}

		void SET_TASK_MOVE_NETWORK_ANIM_SET(Ped ped, Hash clipSet, Hash variableClipSet) {
			Invoker::Invoke<void, 2321>(ped, clipSet, variableClipSet);
		}

		void SET_TASK_MOVE_NETWORK_SIGNAL_FLOAT(Ped ped, const char* signalName, float value) {
			Invoker::Invoke<void, 2322>(ped, signalName, value);
		}

		void SET_TASK_MOVE_NETWORK_SIGNAL_LOCAL_FLOAT(Ped ped, const char* signalName, float value) {
			Invoker::Invoke<void, 2323>(ped, signalName, value);
		}

		void SET_TASK_MOVE_NETWORK_SIGNAL_FLOAT_LERP_RATE(Ped ped, const char* signalName, float value) {
			Invoker::Invoke<void, 2324>(ped, signalName, value);
		}

		void SET_TASK_MOVE_NETWORK_SIGNAL_BOOL(Ped ped, const char* signalName, BOOL value) {
			Invoker::Invoke<void, 2325>(ped, signalName, value);
		}

		float GET_TASK_MOVE_NETWORK_SIGNAL_FLOAT(Ped ped, const char* signalName) {
			return Invoker::Invoke<float, 2326>(ped, signalName);
		}

		BOOL GET_TASK_MOVE_NETWORK_SIGNAL_BOOL(Ped ped, const char* signalName) {
			return Invoker::Invoke<BOOL, 2327>(ped, signalName);
		}

		BOOL GET_TASK_MOVE_NETWORK_EVENT(Ped ped, const char* eventName) {
			return Invoker::Invoke<BOOL, 2328>(ped, eventName);
		}

		BOOL SET_TASK_MOVE_NETWORK_ENABLE_COLLISION_ON_NETWORK_CLONE_WHEN_FIXED(Ped ped, BOOL enable) {
			return Invoker::Invoke<BOOL, 2329>(ped, enable);
		}

		void _SET_SCRIPT_TASK_ENABLE_COLLISION_ON_NETWORK_CLONE_WHEN_FIXED(Ped ped, BOOL enable) {
			Invoker::Invoke<void, 2330>(ped, enable);
		}

		void _SET_AMBIENT_PED_ENABLE_COLLISION_ON_NETWORK_CLONE_WHEN_FIXED(Ped ped, BOOL enable) {
			Invoker::Invoke<void, 6709>(ped, enable);
		}

		BOOL IS_MOVE_BLEND_RATIO_STILL(Ped ped) {
			return Invoker::Invoke<BOOL, 2331>(ped);
		}

		BOOL IS_MOVE_BLEND_RATIO_WALKING(Ped ped) {
			return Invoker::Invoke<BOOL, 2332>(ped);
		}

		BOOL IS_MOVE_BLEND_RATIO_RUNNING(Ped ped) {
			return Invoker::Invoke<BOOL, 2333>(ped);
		}

		BOOL IS_MOVE_BLEND_RATIO_SPRINTING(Ped ped) {
			return Invoker::Invoke<BOOL, 2334>(ped);
		}

		BOOL IS_PED_STILL(Ped ped) {
			return Invoker::Invoke<BOOL, 2335>(ped);
		}

		BOOL IS_PED_WALKING(Ped ped) {
			return Invoker::Invoke<BOOL, 2336>(ped);
		}

		BOOL IS_PED_RUNNING(Ped ped) {
			return Invoker::Invoke<BOOL, 2337>(ped);
		}

		BOOL IS_PED_SPRINTING(Ped ped) {
			return Invoker::Invoke<BOOL, 2338>(ped);
		}

		BOOL IS_PED_STRAFING(Ped ped) {
			return Invoker::Invoke<BOOL, 2339>(ped);
		}

		void TASK_SYNCHRONIZED_SCENE(Ped ped, int scene, const char* animDictionary, const char* animationName, float blendIn, float blendOut, int flags, int ragdollBlockingFlags, float moverBlendDelta, int ikFlags) {
			Invoker::Invoke<void, 2340>(ped, scene, animDictionary, animationName, blendIn, blendOut, flags, ragdollBlockingFlags, moverBlendDelta, ikFlags);
		}

		void TASK_AGITATED_ACTION_CONFRONT_RESPONSE(Ped ped, Ped ped2) {
			Invoker::Invoke<void, 2341>(ped, ped2);
		}

		void TASK_SWEEP_AIM_ENTITY(Ped ped, const char* animDict, const char* lowAnimName, const char* medAnimName, const char* hiAnimName, int runtime, Entity targetEntity, float turnRate, float blendInDuration) {
			Invoker::Invoke<void, 2342>(ped, animDict, lowAnimName, medAnimName, hiAnimName, runtime, targetEntity, turnRate, blendInDuration);
		}

		void UPDATE_TASK_SWEEP_AIM_ENTITY(Ped ped, Entity entity) {
			Invoker::Invoke<void, 2343>(ped, entity);
		}

		void TASK_SWEEP_AIM_POSITION(Ped ped, const char* animDict, const char* lowAnimName, const char* medAnimName, const char* hiAnimName, int runtime, float x, float y, float z, float turnRate, float blendInDuration) {
			Invoker::Invoke<void, 2344>(ped, animDict, lowAnimName, medAnimName, hiAnimName, runtime, x, y, z, turnRate, blendInDuration);
		}

		void UPDATE_TASK_SWEEP_AIM_POSITION(Ped ped, float x, float y, float z) {
			Invoker::Invoke<void, 2345>(ped, x, y, z);
		}

		void TASK_ARREST_PED(Ped ped, Ped target) {
			Invoker::Invoke<void, 2346>(ped, target);
		}

		BOOL IS_PED_RUNNING_ARREST_TASK(Ped ped) {
			return Invoker::Invoke<BOOL, 2347>(ped);
		}

		BOOL IS_PED_BEING_ARRESTED(Ped ped) {
			return Invoker::Invoke<BOOL, 2348>(ped);
		}

		void UNCUFF_PED(Ped ped) {
			Invoker::Invoke<void, 2349>(ped);
		}

		BOOL IS_PED_CUFFED(Ped ped) {
			return Invoker::Invoke<BOOL, 2350>(ped);
		}

	} // namespace TASK

	namespace VEHICLE {

		Vehicle CREATE_VEHICLE(Hash modelHash, float x, float y, float z, float heading, BOOL isNetwork, BOOL bScriptHostVeh, BOOL p7) {
			return Invoker::Invoke<Vehicle, 4035>(modelHash, x, y, z, heading, isNetwork, bScriptHostVeh, p7);
		}

		void DELETE_VEHICLE(Vehicle* vehicle) {
			Invoker::Invoke<void, 4036>(vehicle);
		}

		void SET_VEHICLE_ALLOW_HOMING_MISSLE_LOCKON(Vehicle vehicle, BOOL toggle, BOOL p2) {
			Invoker::Invoke<void, 4037>(vehicle, toggle, p2);
		}

		BOOL _GET_VEHICLE_ALLOW_HOMING_MISSLE_LOCKON_SYNCED(Vehicle vehicle) {
			return Invoker::Invoke<BOOL, 6710>(vehicle);
		}

		void SET_VEHICLE_ALLOW_HOMING_MISSLE_LOCKON_SYNCED(Vehicle vehicle, BOOL canBeLockedOn, BOOL p2) {
			Invoker::Invoke<void, 4038>(vehicle, canBeLockedOn, p2);
		}

		void SET_VEHICLE_ALLOW_NO_PASSENGERS_LOCKON(Vehicle veh, BOOL toggle) {
			Invoker::Invoke<void, 4039>(veh, toggle);
		}

		int GET_VEHICLE_HOMING_LOCKON_STATE(Vehicle vehicle) {
			return Invoker::Invoke<int, 4040>(vehicle);
		}

		int GET_VEHICLE_HOMING_LOCKEDONTO_STATE(Any p0) {
			return Invoker::Invoke<int, 4041>(p0);
		}

		void SET_VEHICLE_HOMING_LOCKEDONTO_STATE(Any p0, Any p1) {
			Invoker::Invoke<void, 4042>(p0, p1);
		}

		BOOL IS_VEHICLE_MODEL(Vehicle vehicle, Hash model) {
			return Invoker::Invoke<BOOL, 4043>(vehicle, model);
		}

		BOOL DOES_SCRIPT_VEHICLE_GENERATOR_EXIST(int vehicleGenerator) {
			return Invoker::Invoke<BOOL, 4044>(vehicleGenerator);
		}

		int CREATE_SCRIPT_VEHICLE_GENERATOR(float x, float y, float z, float heading, float p4, float p5, Hash modelHash, int p7, int p8, int p9, int p10, BOOL p11, BOOL p12, BOOL p13, BOOL p14, BOOL p15, int p16) {
			return Invoker::Invoke<int, 4045>(x, y, z, heading, p4, p5, modelHash, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16);
		}

		void DELETE_SCRIPT_VEHICLE_GENERATOR(int vehicleGenerator) {
			Invoker::Invoke<void, 4046>(vehicleGenerator);
		}

		void SET_SCRIPT_VEHICLE_GENERATOR(int vehicleGenerator, BOOL enabled) {
			Invoker::Invoke<void, 4047>(vehicleGenerator, enabled);
		}

		void SET_ALL_VEHICLE_GENERATORS_ACTIVE_IN_AREA(float x1, float y1, float z1, float x2, float y2, float z2, BOOL toggle, BOOL p7) {
			Invoker::Invoke<void, 4048>(x1, y1, z1, x2, y2, z2, toggle, p7);
		}

		void SET_ALL_VEHICLE_GENERATORS_ACTIVE() {
			Invoker::Invoke<void, 4049>();
		}

		void SET_ALL_LOW_PRIORITY_VEHICLE_GENERATORS_ACTIVE(BOOL active) {
			Invoker::Invoke<void, 4050>(active);
		}

		void SET_VEHICLE_GENERATOR_AREA_OF_INTEREST(float x, float y, float z, float radius) {
			Invoker::Invoke<void, 4051>(x, y, z, radius);
		}

		void CLEAR_VEHICLE_GENERATOR_AREA_OF_INTEREST() {
			Invoker::Invoke<void, 4052>();
		}

		BOOL SET_VEHICLE_ON_GROUND_PROPERLY(Vehicle vehicle, float p1) {
			return Invoker::Invoke<BOOL, 4053>(vehicle, p1);
		}

		BOOL SET_VEHICLE_USE_CUTSCENE_WHEEL_COMPRESSION(Vehicle p0, BOOL p1, BOOL p2, BOOL p3) {
			return Invoker::Invoke<BOOL, 4054>(p0, p1, p2, p3);
		}

		BOOL IS_VEHICLE_STUCK_ON_ROOF(Vehicle vehicle) {
			return Invoker::Invoke<BOOL, 4055>(vehicle);
		}

		void ADD_VEHICLE_UPSIDEDOWN_CHECK(Vehicle vehicle) {
			Invoker::Invoke<void, 4056>(vehicle);
		}

		void REMOVE_VEHICLE_UPSIDEDOWN_CHECK(Vehicle vehicle) {
			Invoker::Invoke<void, 4057>(vehicle);
		}

		BOOL IS_VEHICLE_STOPPED(Vehicle vehicle) {
			return Invoker::Invoke<BOOL, 4058>(vehicle);
		}

		int GET_VEHICLE_NUMBER_OF_PASSENGERS(Vehicle vehicle, BOOL includeDriver, BOOL includeDeadOccupants) {
			return Invoker::Invoke<int, 4059>(vehicle, includeDriver, includeDeadOccupants);
		}

		int GET_VEHICLE_MAX_NUMBER_OF_PASSENGERS(Vehicle vehicle) {
			return Invoker::Invoke<int, 4060>(vehicle);
		}

		int GET_VEHICLE_MODEL_NUMBER_OF_SEATS(Hash modelHash) {
			return Invoker::Invoke<int, 4061>(modelHash);
		}

		BOOL IS_SEAT_WARP_ONLY(Vehicle vehicle, int seatIndex) {
			return Invoker::Invoke<BOOL, 4062>(vehicle, seatIndex);
		}

		BOOL IS_TURRET_SEAT(Vehicle vehicle, int seatIndex) {
			return Invoker::Invoke<BOOL, 4063>(vehicle, seatIndex);
		}

		BOOL DOES_VEHICLE_ALLOW_RAPPEL(Vehicle vehicle) {
			return Invoker::Invoke<BOOL, 4064>(vehicle);
		}

		void SET_VEHICLE_DENSITY_MULTIPLIER_THIS_FRAME(float multiplier) {
			Invoker::Invoke<void, 4065>(multiplier);
		}

		void SET_RANDOM_VEHICLE_DENSITY_MULTIPLIER_THIS_FRAME(float multiplier) {
			Invoker::Invoke<void, 4066>(multiplier);
		}

		void SET_PARKED_VEHICLE_DENSITY_MULTIPLIER_THIS_FRAME(float multiplier) {
			Invoker::Invoke<void, 4067>(multiplier);
		}

		void SET_DISABLE_RANDOM_TRAINS_THIS_FRAME(BOOL toggle) {
			Invoker::Invoke<void, 4068>(toggle);
		}

		void SET_AMBIENT_VEHICLE_RANGE_MULTIPLIER_THIS_FRAME(float value) {
			Invoker::Invoke<void, 4069>(value);
		}

		void SET_FAR_DRAW_VEHICLES(BOOL toggle) {
			Invoker::Invoke<void, 4070>(toggle);
		}

		void SET_NUMBER_OF_PARKED_VEHICLES(int value) {
			Invoker::Invoke<void, 4071>(value);
		}

		void SET_VEHICLE_DOORS_LOCKED(Vehicle vehicle, int doorLockStatus) {
			Invoker::Invoke<void, 4072>(vehicle, doorLockStatus);
		}

		void SET_VEHICLE_INDIVIDUAL_DOORS_LOCKED(Vehicle vehicle, int doorId, int doorLockStatus) {
			Invoker::Invoke<void, 4073>(vehicle, doorId, doorLockStatus);
		}

		void SET_VEHICLE_HAS_MUTED_SIRENS(Vehicle vehicle, BOOL toggle) {
			Invoker::Invoke<void, 4074>(vehicle, toggle);
		}

		void SET_VEHICLE_DOORS_LOCKED_FOR_PLAYER(Vehicle vehicle, Player player, BOOL toggle) {
			Invoker::Invoke<void, 4075>(vehicle, player, toggle);
		}

		BOOL GET_VEHICLE_DOORS_LOCKED_FOR_PLAYER(Vehicle vehicle, Player player) {
			return Invoker::Invoke<BOOL, 4076>(vehicle, player);
		}

		void SET_VEHICLE_DOORS_LOCKED_FOR_ALL_PLAYERS(Vehicle vehicle, BOOL toggle) {
			Invoker::Invoke<void, 4077>(vehicle, toggle);
		}

		void SET_VEHICLE_DOORS_LOCKED_FOR_NON_SCRIPT_PLAYERS(Vehicle vehicle, BOOL toggle) {
			Invoker::Invoke<void, 4078>(vehicle, toggle);
		}

		void SET_VEHICLE_DOORS_LOCKED_FOR_TEAM(Vehicle vehicle, int team, BOOL toggle) {
			Invoker::Invoke<void, 4079>(vehicle, team, toggle);
		}

		void SET_VEHICLE_DOORS_LOCKED_FOR_ALL_TEAMS(Vehicle vehicle, BOOL toggle) {
			Invoker::Invoke<void, 4080>(vehicle, toggle);
		}

		void SET_VEHICLE_DONT_TERMINATE_TASK_WHEN_ACHIEVED(Vehicle vehicle) {
			Invoker::Invoke<void, 4081>(vehicle);
		}

		void _SET_VEHICLE_MAX_LAUNCH_ENGINE_REVS(Vehicle vehicle, float modifier) {
			Invoker::Invoke<void, 4082>(vehicle, modifier);
		}

		float _GET_VEHICLE_THROTTLE(Vehicle vehicle) {
			return Invoker::Invoke<float, 4083>(vehicle);
		}

		void EXPLODE_VEHICLE(Vehicle vehicle, BOOL isAudible, BOOL isInvisible) {
			Invoker::Invoke<void, 4084>(vehicle, isAudible, isInvisible);
		}

		void SET_VEHICLE_OUT_OF_CONTROL(Vehicle vehicle, BOOL killDriver, BOOL explodeOnImpact) {
			Invoker::Invoke<void, 4085>(vehicle, killDriver, explodeOnImpact);
		}

		void SET_VEHICLE_TIMED_EXPLOSION(Vehicle vehicle, Ped ped, BOOL toggle) {
			Invoker::Invoke<void, 4086>(vehicle, ped, toggle);
		}

		void ADD_VEHICLE_PHONE_EXPLOSIVE_DEVICE(Vehicle vehicle) {
			Invoker::Invoke<void, 4087>(vehicle);
		}

		void CLEAR_VEHICLE_PHONE_EXPLOSIVE_DEVICE() {
			Invoker::Invoke<void, 4088>();
		}

		BOOL HAS_VEHICLE_PHONE_EXPLOSIVE_DEVICE() {
			return Invoker::Invoke<BOOL, 4089>();
		}

		void DETONATE_VEHICLE_PHONE_EXPLOSIVE_DEVICE() {
			Invoker::Invoke<void, 4090>();
		}

		BOOL HAVE_VEHICLE_REAR_DOORS_BEEN_BLOWN_OPEN_BY_STICKYBOMB(Vehicle vehicle) {
			return Invoker::Invoke<BOOL, 4091>(vehicle);
		}

		void SET_TAXI_LIGHTS(Vehicle vehicle, BOOL state) {
			Invoker::Invoke<void, 4092>(vehicle, state);
		}

		BOOL IS_TAXI_LIGHT_ON(Vehicle vehicle) {
			return Invoker::Invoke<BOOL, 4093>(vehicle);
		}

		BOOL IS_VEHICLE_IN_GARAGE_AREA(const char* garageName, Vehicle vehicle) {
			return Invoker::Invoke<BOOL, 4094>(garageName, vehicle);
		}

		void SET_VEHICLE_COLOURS(Vehicle vehicle, int colorPrimary, int colorSecondary) {
			Invoker::Invoke<void, 4095>(vehicle, colorPrimary, colorSecondary);
		}

		void SET_VEHICLE_FULLBEAM(Vehicle vehicle, BOOL toggle) {
			Invoker::Invoke<void, 4096>(vehicle, toggle);
		}

		void SET_VEHICLE_IS_RACING(Vehicle vehicle, BOOL toggle) {
			Invoker::Invoke<void, 4097>(vehicle, toggle);
		}

		void SET_VEHICLE_CUSTOM_PRIMARY_COLOUR(Vehicle vehicle, int r, int g, int b) {
			Invoker::Invoke<void, 4098>(vehicle, r, g, b);
		}

		void GET_VEHICLE_CUSTOM_PRIMARY_COLOUR(Vehicle vehicle, int* r, int* g, int* b) {
			Invoker::Invoke<void, 4099>(vehicle, r, g, b);
		}

		void CLEAR_VEHICLE_CUSTOM_PRIMARY_COLOUR(Vehicle vehicle) {
			Invoker::Invoke<void, 4100>(vehicle);
		}

		BOOL GET_IS_VEHICLE_PRIMARY_COLOUR_CUSTOM(Vehicle vehicle) {
			return Invoker::Invoke<BOOL, 4101>(vehicle);
		}

		void SET_VEHICLE_CUSTOM_SECONDARY_COLOUR(Vehicle vehicle, int r, int g, int b) {
			Invoker::Invoke<void, 4102>(vehicle, r, g, b);
		}

		void GET_VEHICLE_CUSTOM_SECONDARY_COLOUR(Vehicle vehicle, int* r, int* g, int* b) {
			Invoker::Invoke<void, 4103>(vehicle, r, g, b);
		}

		void CLEAR_VEHICLE_CUSTOM_SECONDARY_COLOUR(Vehicle vehicle) {
			Invoker::Invoke<void, 4104>(vehicle);
		}

		BOOL GET_IS_VEHICLE_SECONDARY_COLOUR_CUSTOM(Vehicle vehicle) {
			return Invoker::Invoke<BOOL, 4105>(vehicle);
		}

		void SET_VEHICLE_ENVEFF_SCALE(Vehicle vehicle, float fade) {
			Invoker::Invoke<void, 4106>(vehicle, fade);
		}

		float GET_VEHICLE_ENVEFF_SCALE(Vehicle vehicle) {
			return Invoker::Invoke<float, 4107>(vehicle);
		}

		void SET_CAN_RESPRAY_VEHICLE(Vehicle vehicle, BOOL state) {
			Invoker::Invoke<void, 4108>(vehicle, state);
		}

		void SET_GOON_BOSS_VEHICLE(Vehicle vehicle, BOOL toggle) {
			Invoker::Invoke<void, 4109>(vehicle, toggle);
		}

		void SET_OPEN_REAR_DOORS_ON_EXPLOSION(Vehicle vehicle, BOOL toggle) {
			Invoker::Invoke<void, 4110>(vehicle, toggle);
		}

		void FORCE_SUBMARINE_SURFACE_MODE(Vehicle vehicle, BOOL toggle) {
			Invoker::Invoke<void, 4111>(vehicle, toggle);
		}

		void FORCE_SUBMARINE_NEURTAL_BUOYANCY(Any p0, Any p1) {
			Invoker::Invoke<void, 4112>(p0, p1);
		}

		void SET_SUBMARINE_CRUSH_DEPTHS(Vehicle vehicle, BOOL p1, float depth1, float depth2, float depth3) {
			Invoker::Invoke<void, 4113>(vehicle, p1, depth1, depth2, depth3);
		}

		BOOL GET_SUBMARINE_IS_UNDER_DESIGN_DEPTH(Vehicle submarine) {
			return Invoker::Invoke<BOOL, 4114>(submarine);
		}

		int GET_SUBMARINE_NUMBER_OF_AIR_LEAKS(Vehicle submarine) {
			return Invoker::Invoke<int, 4115>(submarine);
		}

		void SET_BOAT_IGNORE_LAND_PROBES(Any p0, Any p1) {
			Invoker::Invoke<void, 4116>(p0, p1);
		}

		void _SET_BOUNDS_AFFECT_WATER_PROBES(Vehicle vehicle, BOOL toggle) {
			Invoker::Invoke<void, 4117>(vehicle, toggle);
		}

		void SET_BOAT_ANCHOR(Vehicle vehicle, BOOL toggle) {
			Invoker::Invoke<void, 4118>(vehicle, toggle);
		}

		BOOL CAN_ANCHOR_BOAT_HERE(Vehicle vehicle) {
			return Invoker::Invoke<BOOL, 4119>(vehicle);
		}

		BOOL CAN_ANCHOR_BOAT_HERE_IGNORE_PLAYERS(Vehicle vehicle) {
			return Invoker::Invoke<BOOL, 4120>(vehicle);
		}

		void SET_BOAT_REMAINS_ANCHORED_WHILE_PLAYER_IS_DRIVER(Vehicle vehicle, BOOL toggle) {
			Invoker::Invoke<void, 4121>(vehicle, toggle);
		}

		void SET_FORCE_LOW_LOD_ANCHOR_MODE(Vehicle vehicle, BOOL p1) {
			Invoker::Invoke<void, 4122>(vehicle, p1);
		}

		void SET_BOAT_LOW_LOD_ANCHOR_DISTANCE(Vehicle vehicle, float value) {
			Invoker::Invoke<void, 4123>(vehicle, value);
		}

		BOOL IS_BOAT_ANCHORED(Vehicle vehicle) {
			return Invoker::Invoke<BOOL, 4124>(vehicle);
		}

		void SET_BOAT_SINKS_WHEN_WRECKED(Vehicle vehicle, BOOL toggle) {
			Invoker::Invoke<void, 4125>(vehicle, toggle);
		}

		void SET_BOAT_WRECKED(Vehicle vehicle) {
			Invoker::Invoke<void, 4126>(vehicle);
		}

		void SET_VEHICLE_SIREN(Vehicle vehicle, BOOL toggle) {
			Invoker::Invoke<void, 4127>(vehicle, toggle);
		}

		BOOL IS_VEHICLE_SIREN_ON(Vehicle vehicle) {
			return Invoker::Invoke<BOOL, 4128>(vehicle);
		}

		BOOL IS_VEHICLE_SIREN_AUDIO_ON(Vehicle vehicle) {
			return Invoker::Invoke<BOOL, 4129>(vehicle);
		}

		void SET_VEHICLE_STRONG(Vehicle vehicle, BOOL toggle) {
			Invoker::Invoke<void, 4130>(vehicle, toggle);
		}

		void REMOVE_VEHICLE_STUCK_CHECK(Vehicle vehicle) {
			Invoker::Invoke<void, 4131>(vehicle);
		}

		void GET_VEHICLE_COLOURS(Vehicle vehicle, int* colorPrimary, int* colorSecondary) {
			Invoker::Invoke<void, 4132>(vehicle, colorPrimary, colorSecondary);
		}

		BOOL IS_VEHICLE_SEAT_FREE(Vehicle vehicle, int seatIndex, BOOL isTaskRunning) {
			return Invoker::Invoke<BOOL, 4133>(vehicle, seatIndex, isTaskRunning);
		}

		Ped GET_PED_IN_VEHICLE_SEAT(Vehicle vehicle, int seatIndex, BOOL p2) {
			return Invoker::Invoke<Ped, 4134>(vehicle, seatIndex, p2);
		}

		Ped GET_LAST_PED_IN_VEHICLE_SEAT(Vehicle vehicle, int seatIndex) {
			return Invoker::Invoke<Ped, 4135>(vehicle, seatIndex);
		}

		BOOL GET_VEHICLE_LIGHTS_STATE(Vehicle vehicle, BOOL* lightsOn, BOOL* highbeamsOn) {
			return Invoker::Invoke<BOOL, 4136>(vehicle, lightsOn, highbeamsOn);
		}

		BOOL IS_VEHICLE_TYRE_BURST(Vehicle vehicle, int wheelID, BOOL completely) {
			return Invoker::Invoke<BOOL, 4137>(vehicle, wheelID, completely);
		}

		void SET_VEHICLE_FORWARD_SPEED(Vehicle vehicle, float speed) {
			Invoker::Invoke<void, 4138>(vehicle, speed);
		}

		void SET_VEHICLE_FORWARD_SPEED_XY(Vehicle vehicle, float speed) {
			Invoker::Invoke<void, 4139>(vehicle, speed);
		}

		void BRING_VEHICLE_TO_HALT(Vehicle vehicle, float distance, int duration, BOOL p3) {
			Invoker::Invoke<void, 4140>(vehicle, distance, duration, p3);
		}

		void SET_VEHICLE_STEER_FOR_BUILDINGS(Vehicle vehicle, Any p1) {
			Invoker::Invoke<void, 4141>(vehicle, p1);
		}

		void SET_VEHICLE_CAUSES_SWERVING(Vehicle vehicle, BOOL toggle) {
			Invoker::Invoke<void, 4142>(vehicle, toggle);
		}

		void SET_IGNORE_PLANES_SMALL_PITCH_CHANGE(Any p0, Any p1) {
			Invoker::Invoke<void, 4143>(p0, p1);
		}

		void STOP_BRINGING_VEHICLE_TO_HALT(Vehicle vehicle) {
			Invoker::Invoke<void, 4144>(vehicle);
		}

		BOOL IS_VEHICLE_BEING_BROUGHT_TO_HALT(Vehicle vehicle) {
			return Invoker::Invoke<BOOL, 4145>(vehicle);
		}

		void LOWER_FORKLIFT_FORKS(Vehicle forklift) {
			Invoker::Invoke<void, 4146>(forklift);
		}

		void SET_FORKLIFT_FORK_HEIGHT(Vehicle vehicle, float height) {
			Invoker::Invoke<void, 4147>(vehicle, height);
		}

		BOOL IS_ENTITY_ATTACHED_TO_HANDLER_FRAME(Vehicle vehicle, Entity entity) {
			return Invoker::Invoke<BOOL, 4148>(vehicle, entity);
		}

		BOOL IS_ANY_ENTITY_ATTACHED_TO_HANDLER_FRAME(Vehicle vehicle) {
			return Invoker::Invoke<BOOL, 4149>(vehicle);
		}

		Vehicle FIND_HANDLER_VEHICLE_CONTAINER_IS_ATTACHED_TO(Entity entity) {
			return Invoker::Invoke<Vehicle, 4150>(entity);
		}

		BOOL IS_HANDLER_FRAME_LINED_UP_WITH_CONTAINER(Vehicle vehicle, Entity entity) {
			return Invoker::Invoke<BOOL, 4151>(vehicle, entity);
		}

		void ATTACH_CONTAINER_TO_HANDLER_FRAME_WHEN_LINED_UP(Vehicle vehicle, Entity entity) {
			Invoker::Invoke<void, 4152>(vehicle, entity);
		}

		void DETACH_CONTAINER_FROM_HANDLER_FRAME(Vehicle vehicle) {
			Invoker::Invoke<void, 4153>(vehicle);
		}

		void SET_VEHICLE_DISABLE_HEIGHT_MAP_AVOIDANCE(Vehicle vehicle, BOOL p1) {
			Invoker::Invoke<void, 4154>(vehicle, p1);
		}

		void _SET_PLANE_AVOIDS_OTHERS(Vehicle vehicle, BOOL toggle) {
			Invoker::Invoke<void, 6711>(vehicle, toggle);
		}

		void SET_BOAT_DISABLE_AVOIDANCE(Vehicle vehicle, BOOL p1) {
			Invoker::Invoke<void, 4155>(vehicle, p1);
		}

		BOOL IS_HELI_LANDING_AREA_BLOCKED(Vehicle vehicle) {
			return Invoker::Invoke<BOOL, 4156>(vehicle);
		}

		void SET_SHORT_SLOWDOWN_FOR_LANDING(Vehicle vehicle) {
			Invoker::Invoke<void, 4157>(vehicle);
		}

		void SET_HELI_TURBULENCE_SCALAR(Vehicle vehicle, float p1) {
			Invoker::Invoke<void, 4158>(vehicle, p1);
		}

		void SET_CAR_BOOT_OPEN(Vehicle vehicle) {
			Invoker::Invoke<void, 4159>(vehicle);
		}

		void SET_VEHICLE_TYRE_BURST(Vehicle vehicle, int index, BOOL onRim, float p3) {
			Invoker::Invoke<void, 4160>(vehicle, index, onRim, p3);
		}

		void SET_VEHICLE_DOORS_SHUT(Vehicle vehicle, BOOL closeInstantly) {
			Invoker::Invoke<void, 4161>(vehicle, closeInstantly);
		}

		void SET_VEHICLE_TYRES_CAN_BURST(Vehicle vehicle, BOOL toggle) {
			Invoker::Invoke<void, 4162>(vehicle, toggle);
		}

		BOOL GET_VEHICLE_TYRES_CAN_BURST(Vehicle vehicle) {
			return Invoker::Invoke<BOOL, 4163>(vehicle);
		}

		void SET_VEHICLE_WHEELS_CAN_BREAK(Vehicle vehicle, BOOL enabled) {
			Invoker::Invoke<void, 4164>(vehicle, enabled);
		}

		void SET_VEHICLE_DOOR_OPEN(Vehicle vehicle, int doorId, BOOL loose, BOOL openInstantly) {
			Invoker::Invoke<void, 4165>(vehicle, doorId, loose, openInstantly);
		}

		void SET_VEHICLE_DOOR_AUTO_LOCK(Vehicle vehicle, int doorId, BOOL toggle) {
			Invoker::Invoke<void, 4166>(vehicle, doorId, toggle);
		}

		void SET_FLEEING_VEHICLES_USE_SWITCHED_OFF_NODES(Any p0) {
			Invoker::Invoke<void, 4167>(p0);
		}

		void REMOVE_VEHICLE_WINDOW(Vehicle vehicle, int windowIndex) {
			Invoker::Invoke<void, 4168>(vehicle, windowIndex);
		}

		void ROLL_DOWN_WINDOWS(Vehicle vehicle) {
			Invoker::Invoke<void, 4169>(vehicle);
		}

		void ROLL_DOWN_WINDOW(Vehicle vehicle, int windowIndex) {
			Invoker::Invoke<void, 4170>(vehicle, windowIndex);
		}

		void ROLL_UP_WINDOW(Vehicle vehicle, int windowIndex) {
			Invoker::Invoke<void, 4171>(vehicle, windowIndex);
		}

		void SMASH_VEHICLE_WINDOW(Vehicle vehicle, int windowIndex) {
			Invoker::Invoke<void, 4172>(vehicle, windowIndex);
		}

		void FIX_VEHICLE_WINDOW(Vehicle vehicle, int windowIndex) {
			Invoker::Invoke<void, 4173>(vehicle, windowIndex);
		}

		void POP_OUT_VEHICLE_WINDSCREEN(Vehicle vehicle) {
			Invoker::Invoke<void, 4174>(vehicle);
		}

		void POP_OFF_VEHICLE_ROOF_WITH_IMPULSE(Vehicle vehicle, float x, float y, float z) {
			Invoker::Invoke<void, 4175>(vehicle, x, y, z);
		}

		void SET_VEHICLE_LIGHTS(Vehicle vehicle, int state) {
			Invoker::Invoke<void, 4176>(vehicle, state);
		}

		void SET_VEHICLE_USE_PLAYER_LIGHT_SETTINGS(Vehicle vehicle, BOOL toggle) {
			Invoker::Invoke<void, 4177>(vehicle, toggle);
		}

		void SET_VEHICLE_HEADLIGHT_SHADOWS(Vehicle vehicle, int p1) {
			Invoker::Invoke<void, 4178>(vehicle, p1);
		}

		void SET_VEHICLE_ALARM(Vehicle vehicle, BOOL state) {
			Invoker::Invoke<void, 4179>(vehicle, state);
		}

		void START_VEHICLE_ALARM(Vehicle vehicle) {
			Invoker::Invoke<void, 4180>(vehicle);
		}

		BOOL IS_VEHICLE_ALARM_ACTIVATED(Vehicle vehicle) {
			return Invoker::Invoke<BOOL, 4181>(vehicle);
		}

		void SET_VEHICLE_INTERIORLIGHT(Vehicle vehicle, BOOL toggle) {
			Invoker::Invoke<void, 4182>(vehicle, toggle);
		}

		void SET_VEHICLE_FORCE_INTERIORLIGHT(Vehicle vehicle, BOOL toggle) {
			Invoker::Invoke<void, 4183>(vehicle, toggle);
		}

		void SET_VEHICLE_LIGHT_MULTIPLIER(Vehicle vehicle, float multiplier) {
			Invoker::Invoke<void, 4184>(vehicle, multiplier);
		}

		void ATTACH_VEHICLE_TO_TRAILER(Vehicle vehicle, Vehicle trailer, float radius) {
			Invoker::Invoke<void, 4185>(vehicle, trailer, radius);
		}

		void ATTACH_VEHICLE_ON_TO_TRAILER(Vehicle vehicle, Vehicle trailer, float offsetX, float offsetY, float offsetZ, float coordsX, float coordsY, float coordsZ, float rotationX, float rotationY, float rotationZ, float disableCollisions) {
			Invoker::Invoke<void, 4186>(vehicle, trailer, offsetX, offsetY, offsetZ, coordsX, coordsY, coordsZ, rotationX, rotationY, rotationZ, disableCollisions);
		}

		void STABILISE_ENTITY_ATTACHED_TO_HELI(Vehicle vehicle, Entity entity, float p2) {
			Invoker::Invoke<void, 4187>(vehicle, entity, p2);
		}

		void DETACH_VEHICLE_FROM_TRAILER(Vehicle vehicle) {
			Invoker::Invoke<void, 4188>(vehicle);
		}

		BOOL IS_VEHICLE_ATTACHED_TO_TRAILER(Vehicle vehicle) {
			return Invoker::Invoke<BOOL, 4189>(vehicle);
		}

		void SET_TRAILER_INVERSE_MASS_SCALE(Vehicle vehicle, float p1) {
			Invoker::Invoke<void, 4190>(vehicle, p1);
		}

		void SET_TRAILER_LEGS_RAISED(Vehicle vehicle) {
			Invoker::Invoke<void, 4191>(vehicle);
		}

		void SET_TRAILER_LEGS_LOWERED(Vehicle vehicle) {
			Invoker::Invoke<void, 4192>(vehicle);
		}

		void SET_VEHICLE_TYRE_FIXED(Vehicle vehicle, int tyreIndex) {
			Invoker::Invoke<void, 4193>(vehicle, tyreIndex);
		}

		void SET_VEHICLE_NUMBER_PLATE_TEXT(Vehicle vehicle, const char* plateText) {
			Invoker::Invoke<void, 4194>(vehicle, plateText);
		}

		const char* GET_VEHICLE_NUMBER_PLATE_TEXT(Vehicle vehicle) {
			return Invoker::Invoke<const char*, 4195>(vehicle);
		}

		int GET_NUMBER_OF_VEHICLE_NUMBER_PLATES() {
			return Invoker::Invoke<int, 4196>();
		}

		void SET_VEHICLE_NUMBER_PLATE_TEXT_INDEX(Vehicle vehicle, int plateIndex) {
			Invoker::Invoke<void, 4197>(vehicle, plateIndex);
		}

		int GET_VEHICLE_NUMBER_PLATE_TEXT_INDEX(Vehicle vehicle) {
			return Invoker::Invoke<int, 4198>(vehicle);
		}

		void SET_RANDOM_TRAINS(BOOL toggle) {
			Invoker::Invoke<void, 4199>(toggle);
		}

		Vehicle CREATE_MISSION_TRAIN(int variation, float x, float y, float z, BOOL direction, Any p5, Any p6) {
			return Invoker::Invoke<Vehicle, 4200>(variation, x, y, z, direction, p5, p6);
		}

		void SWITCH_TRAIN_TRACK(int trackId, BOOL state) {
			Invoker::Invoke<void, 4201>(trackId, state);
		}

		void SET_TRAIN_TRACK_SPAWN_FREQUENCY(int trackIndex, int frequency) {
			Invoker::Invoke<void, 4202>(trackIndex, frequency);
		}

		void ALLOW_TRAIN_TO_BE_REMOVED_BY_POPULATION(Any p0) {
			Invoker::Invoke<void, 4203>(p0);
		}

		void DELETE_ALL_TRAINS() {
			Invoker::Invoke<void, 4204>();
		}

		void SET_TRAIN_SPEED(Vehicle train, float speed) {
			Invoker::Invoke<void, 4205>(train, speed);
		}

		void SET_TRAIN_CRUISE_SPEED(Vehicle train, float speed) {
			Invoker::Invoke<void, 4206>(train, speed);
		}

		void SET_RANDOM_BOATS(BOOL toggle) {
			Invoker::Invoke<void, 4207>(toggle);
		}

		void SET_RANDOM_BOATS_MP(BOOL toggle) {
			Invoker::Invoke<void, 4208>(toggle);
		}

		void SET_GARBAGE_TRUCKS(BOOL toggle) {
			Invoker::Invoke<void, 4209>(toggle);
		}

		BOOL DOES_VEHICLE_HAVE_STUCK_VEHICLE_CHECK(Vehicle vehicle) {
			return Invoker::Invoke<BOOL, 4210>(vehicle);
		}

		int GET_VEHICLE_RECORDING_ID(int recording, const char* script) {
			return Invoker::Invoke<int, 4211>(recording, script);
		}

		void REQUEST_VEHICLE_RECORDING(int recording, const char* script) {
			Invoker::Invoke<void, 4212>(recording, script);
		}

		BOOL HAS_VEHICLE_RECORDING_BEEN_LOADED(int recording, const char* script) {
			return Invoker::Invoke<BOOL, 4213>(recording, script);
		}

		void REMOVE_VEHICLE_RECORDING(int recording, const char* script) {
			Invoker::Invoke<void, 4214>(recording, script);
		}

		Vector3 GET_POSITION_OF_VEHICLE_RECORDING_ID_AT_TIME(int id, float time) {
			return Invoker::Invoke<Vector3, 4215>(id, time);
		}

		Vector3 GET_POSITION_OF_VEHICLE_RECORDING_AT_TIME(int recording, float time, const char* script) {
			return Invoker::Invoke<Vector3, 4216>(recording, time, script);
		}

		Vector3 GET_ROTATION_OF_VEHICLE_RECORDING_ID_AT_TIME(int id, float time) {
			return Invoker::Invoke<Vector3, 4217>(id, time);
		}

		Vector3 GET_ROTATION_OF_VEHICLE_RECORDING_AT_TIME(int recording, float time, const char* script) {
			return Invoker::Invoke<Vector3, 4218>(recording, time, script);
		}

		float GET_TOTAL_DURATION_OF_VEHICLE_RECORDING_ID(int id) {
			return Invoker::Invoke<float, 4219>(id);
		}

		float GET_TOTAL_DURATION_OF_VEHICLE_RECORDING(int recording, const char* script) {
			return Invoker::Invoke<float, 4220>(recording, script);
		}

		float GET_POSITION_IN_RECORDING(Vehicle vehicle) {
			return Invoker::Invoke<float, 4221>(vehicle);
		}

		float GET_TIME_POSITION_IN_RECORDING(Vehicle vehicle) {
			return Invoker::Invoke<float, 4222>(vehicle);
		}

		void START_PLAYBACK_RECORDED_VEHICLE(Vehicle vehicle, int recording, const char* script, BOOL p3) {
			Invoker::Invoke<void, 4223>(vehicle, recording, script, p3);
		}

		void START_PLAYBACK_RECORDED_VEHICLE_WITH_FLAGS(Vehicle vehicle, int recording, const char* script, int flags, int time, int drivingStyle) {
			Invoker::Invoke<void, 4224>(vehicle, recording, script, flags, time, drivingStyle);
		}

		void FORCE_PLAYBACK_RECORDED_VEHICLE_UPDATE(Vehicle vehicle, BOOL p1) {
			Invoker::Invoke<void, 4225>(vehicle, p1);
		}

		void STOP_PLAYBACK_RECORDED_VEHICLE(Vehicle vehicle) {
			Invoker::Invoke<void, 4226>(vehicle);
		}

		void PAUSE_PLAYBACK_RECORDED_VEHICLE(Vehicle vehicle) {
			Invoker::Invoke<void, 4227>(vehicle);
		}

		void UNPAUSE_PLAYBACK_RECORDED_VEHICLE(Vehicle vehicle) {
			Invoker::Invoke<void, 4228>(vehicle);
		}

		BOOL IS_PLAYBACK_GOING_ON_FOR_VEHICLE(Vehicle vehicle) {
			return Invoker::Invoke<BOOL, 4229>(vehicle);
		}

		BOOL IS_PLAYBACK_USING_AI_GOING_ON_FOR_VEHICLE(Vehicle vehicle) {
			return Invoker::Invoke<BOOL, 4230>(vehicle);
		}

		int GET_CURRENT_PLAYBACK_FOR_VEHICLE(Vehicle vehicle) {
			return Invoker::Invoke<int, 4231>(vehicle);
		}

		void SKIP_TO_END_AND_STOP_PLAYBACK_RECORDED_VEHICLE(Vehicle vehicle) {
			Invoker::Invoke<void, 4232>(vehicle);
		}

		void SET_PLAYBACK_SPEED(Vehicle vehicle, float speed) {
			Invoker::Invoke<void, 4233>(vehicle, speed);
		}

		void START_PLAYBACK_RECORDED_VEHICLE_USING_AI(Vehicle vehicle, int recording, const char* script, float speed, int drivingStyle) {
			Invoker::Invoke<void, 4234>(vehicle, recording, script, speed, drivingStyle);
		}

		void SKIP_TIME_IN_PLAYBACK_RECORDED_VEHICLE(Vehicle vehicle, float time) {
			Invoker::Invoke<void, 4235>(vehicle, time);
		}

		void SET_PLAYBACK_TO_USE_AI(Vehicle vehicle, int drivingStyle) {
			Invoker::Invoke<void, 4236>(vehicle, drivingStyle);
		}

		void SET_PLAYBACK_TO_USE_AI_TRY_TO_REVERT_BACK_LATER(Vehicle vehicle, int time, int drivingStyle, BOOL p3) {
			Invoker::Invoke<void, 4237>(vehicle, time, drivingStyle, p3);
		}

		void SET_ADDITIONAL_ROTATION_FOR_RECORDED_VEHICLE_PLAYBACK(Vehicle vehicle, float x, float y, float z, Any p4) {
			Invoker::Invoke<void, 4238>(vehicle, x, y, z, p4);
		}

		void SET_POSITION_OFFSET_FOR_RECORDED_VEHICLE_PLAYBACK(Vehicle vehicle, float x, float y, float z) {
			Invoker::Invoke<void, 4239>(vehicle, x, y, z);
		}

		void SET_GLOBAL_POSITION_OFFSET_FOR_RECORDED_VEHICLE_PLAYBACK(Vehicle vehicle, float x, float y, float z) {
			Invoker::Invoke<void, 4240>(vehicle, x, y, z);
		}

		void SET_SHOULD_LERP_FROM_AI_TO_FULL_RECORDING(Vehicle vehicle, BOOL p1) {
			Invoker::Invoke<void, 4241>(vehicle, p1);
		}

		void EXPLODE_VEHICLE_IN_CUTSCENE(Vehicle vehicle, BOOL p1) {
			Invoker::Invoke<void, 4242>(vehicle, p1);
		}

		void ADD_VEHICLE_STUCK_CHECK_WITH_WARP(Any p0, float p1, Any p2, BOOL p3, BOOL p4, BOOL p5, Any p6) {
			Invoker::Invoke<void, 4243>(p0, p1, p2, p3, p4, p5, p6);
		}

		void SET_VEHICLE_MODEL_IS_SUPPRESSED(Hash model, BOOL suppressed) {
			Invoker::Invoke<void, 4244>(model, suppressed);
		}

		Vehicle GET_RANDOM_VEHICLE_IN_SPHERE(float x, float y, float z, float radius, Hash modelHash, int flags) {
			return Invoker::Invoke<Vehicle, 4245>(x, y, z, radius, modelHash, flags);
		}

		Vehicle GET_RANDOM_VEHICLE_FRONT_BUMPER_IN_SPHERE(float p0, float p1, float p2, float p3, int p4, int p5, int p6) {
			return Invoker::Invoke<Vehicle, 4246>(p0, p1, p2, p3, p4, p5, p6);
		}

		Vehicle GET_RANDOM_VEHICLE_BACK_BUMPER_IN_SPHERE(float p0, float p1, float p2, float p3, int p4, int p5, int p6) {
			return Invoker::Invoke<Vehicle, 4247>(p0, p1, p2, p3, p4, p5, p6);
		}

		Vehicle GET_CLOSEST_VEHICLE(float x, float y, float z, float radius, Hash modelHash, int flags) {
			return Invoker::Invoke<Vehicle, 4248>(x, y, z, radius, modelHash, flags);
		}

		Vehicle GET_TRAIN_CARRIAGE(Vehicle train, int trailerNumber) {
			return Invoker::Invoke<Vehicle, 4249>(train, trailerNumber);
		}

		void _SET_MAKE_TRAIN_SCAN_FOR_BLOCKING_ENTITIES(Vehicle train, BOOL toggle) {
			Invoker::Invoke<void, 6712>(train, toggle);
		}

		int _GET_MISSION_TRAIN_CONFIG_INDEX_BY_NAME(const char* name) {
			return Invoker::Invoke<int, 6713>(name);
		}

		BOOL IS_MISSION_TRAIN(Vehicle train) {
			return Invoker::Invoke<BOOL, 4250>(train);
		}

		void DELETE_MISSION_TRAIN(Vehicle* train) {
			Invoker::Invoke<void, 4251>(train);
		}

		void SET_MISSION_TRAIN_AS_NO_LONGER_NEEDED(Vehicle* train, BOOL p1) {
			Invoker::Invoke<void, 4252>(train, p1);
		}

		void SET_MISSION_TRAIN_COORDS(Vehicle train, float x, float y, float z) {
			Invoker::Invoke<void, 4253>(train, x, y, z);
		}

		BOOL IS_THIS_MODEL_A_BOAT(Hash model) {
			return Invoker::Invoke<BOOL, 4254>(model);
		}

		BOOL IS_THIS_MODEL_A_JETSKI(Hash model) {
			return Invoker::Invoke<BOOL, 4255>(model);
		}

		BOOL IS_THIS_MODEL_A_PLANE(Hash model) {
			return Invoker::Invoke<BOOL, 4256>(model);
		}

		BOOL IS_THIS_MODEL_A_HELI(Hash model) {
			return Invoker::Invoke<BOOL, 4257>(model);
		}

		BOOL IS_THIS_MODEL_A_CAR(Hash model) {
			return Invoker::Invoke<BOOL, 4258>(model);
		}

		BOOL IS_THIS_MODEL_A_TRAIN(Hash model) {
			return Invoker::Invoke<BOOL, 4259>(model);
		}

		BOOL IS_THIS_MODEL_A_BIKE(Hash model) {
			return Invoker::Invoke<BOOL, 4260>(model);
		}

		BOOL IS_THIS_MODEL_A_BICYCLE(Hash model) {
			return Invoker::Invoke<BOOL, 4261>(model);
		}

		BOOL IS_THIS_MODEL_A_QUADBIKE(Hash model) {
			return Invoker::Invoke<BOOL, 4262>(model);
		}

		BOOL IS_THIS_MODEL_AN_AMPHIBIOUS_CAR(Hash model) {
			return Invoker::Invoke<BOOL, 4263>(model);
		}

		BOOL IS_THIS_MODEL_AN_AMPHIBIOUS_QUADBIKE(Hash model) {
			return Invoker::Invoke<BOOL, 4264>(model);
		}

		void SET_HELI_BLADES_FULL_SPEED(Vehicle vehicle) {
			Invoker::Invoke<void, 4265>(vehicle);
		}

		void SET_HELI_BLADES_SPEED(Vehicle vehicle, float speed) {
			Invoker::Invoke<void, 4266>(vehicle, speed);
		}

		void FORCE_SUB_THROTTLE_FOR_TIME(Vehicle vehicle, float p1, float p2) {
			Invoker::Invoke<void, 4267>(vehicle, p1, p2);
		}

		void SET_VEHICLE_CAN_BE_TARGETTED(Vehicle vehicle, BOOL state) {
			Invoker::Invoke<void, 4268>(vehicle, state);
		}

		void SET_DONT_ALLOW_PLAYER_TO_ENTER_VEHICLE_IF_LOCKED_FOR_PLAYER(Vehicle vehicle, BOOL p1) {
			Invoker::Invoke<void, 4269>(vehicle, p1);
		}

		void SET_VEHICLE_CAN_BE_VISIBLY_DAMAGED(Vehicle vehicle, BOOL state) {
			Invoker::Invoke<void, 4270>(vehicle, state);
		}

		void SET_VEHICLE_HAS_UNBREAKABLE_LIGHTS(Vehicle vehicle, BOOL toggle) {
			Invoker::Invoke<void, 4271>(vehicle, toggle);
		}

		void SET_VEHICLE_RESPECTS_LOCKS_WHEN_HAS_DRIVER(Vehicle vehicle, BOOL p1) {
			Invoker::Invoke<void, 4272>(vehicle, p1);
		}

		void SET_VEHICLE_CAN_EJECT_PASSENGERS_IF_LOCKED(Any p0, Any p1) {
			Invoker::Invoke<void, 4273>(p0, p1);
		}

		float GET_VEHICLE_DIRT_LEVEL(Vehicle vehicle) {
			return Invoker::Invoke<float, 4274>(vehicle);
		}

		void SET_VEHICLE_DIRT_LEVEL(Vehicle vehicle, float dirtLevel) {
			Invoker::Invoke<void, 4275>(vehicle, dirtLevel);
		}

		BOOL GET_DOES_VEHICLE_HAVE_DAMAGE_DECALS(Vehicle vehicle) {
			return Invoker::Invoke<BOOL, 4276>(vehicle);
		}

		BOOL IS_VEHICLE_DOOR_FULLY_OPEN(Vehicle vehicle, int doorId) {
			return Invoker::Invoke<BOOL, 4277>(vehicle, doorId);
		}

		void SET_VEHICLE_ENGINE_ON(Vehicle vehicle, BOOL value, BOOL instantly, BOOL disableAutoStart) {
			Invoker::Invoke<void, 4278>(vehicle, value, instantly, disableAutoStart);
		}

		void SET_VEHICLE_UNDRIVEABLE(Vehicle vehicle, BOOL toggle) {
			Invoker::Invoke<void, 4279>(vehicle, toggle);
		}

		void SET_VEHICLE_PROVIDES_COVER(Vehicle vehicle, BOOL toggle) {
			Invoker::Invoke<void, 4280>(vehicle, toggle);
		}

		void SET_VEHICLE_DOOR_CONTROL(Vehicle vehicle, int doorId, int speed, float angle) {
			Invoker::Invoke<void, 4281>(vehicle, doorId, speed, angle);
		}

		void SET_VEHICLE_DOOR_LATCHED(Vehicle vehicle, int doorId, BOOL p2, BOOL p3, BOOL p4) {
			Invoker::Invoke<void, 4282>(vehicle, doorId, p2, p3, p4);
		}

		float GET_VEHICLE_DOOR_ANGLE_RATIO(Vehicle vehicle, int doorId) {
			return Invoker::Invoke<float, 4283>(vehicle, doorId);
		}

		Ped GET_PED_USING_VEHICLE_DOOR(Vehicle vehicle, int doord) {
			return Invoker::Invoke<Ped, 4284>(vehicle, doord);
		}

		void SET_VEHICLE_DOOR_SHUT(Vehicle vehicle, int doorId, BOOL closeInstantly) {
			Invoker::Invoke<void, 4285>(vehicle, doorId, closeInstantly);
		}

		void SET_VEHICLE_DOOR_BROKEN(Vehicle vehicle, int doorId, BOOL deleteDoor) {
			Invoker::Invoke<void, 4286>(vehicle, doorId, deleteDoor);
		}

		void SET_VEHICLE_CAN_BREAK(Vehicle vehicle, BOOL toggle) {
			Invoker::Invoke<void, 4287>(vehicle, toggle);
		}

		BOOL DOES_VEHICLE_HAVE_ROOF(Vehicle vehicle) {
			return Invoker::Invoke<BOOL, 4288>(vehicle);
		}

		void SET_VEHICLE_REMOVE_AGGRESSIVE_CARJACK_MISSION(Any p0) {
			Invoker::Invoke<void, 4289>(p0);
		}

		void SET_VEHICLE_AVOID_PLAYER_VEHICLE_RIOT_VAN_MISSION(Any p0) {
			Invoker::Invoke<void, 4290>(p0);
		}

		void SET_CARJACK_MISSION_REMOVAL_PARAMETERS(Any p0, Any p1) {
			Invoker::Invoke<void, 4291>(p0, p1);
		}

		BOOL IS_BIG_VEHICLE(Vehicle vehicle) {
			return Invoker::Invoke<BOOL, 4292>(vehicle);
		}

		int GET_NUMBER_OF_VEHICLE_COLOURS(Vehicle vehicle) {
			return Invoker::Invoke<int, 4293>(vehicle);
		}

		void SET_VEHICLE_COLOUR_COMBINATION(Vehicle vehicle, int colorCombination) {
			Invoker::Invoke<void, 4294>(vehicle, colorCombination);
		}

		int GET_VEHICLE_COLOUR_COMBINATION(Vehicle vehicle) {
			return Invoker::Invoke<int, 4295>(vehicle);
		}

		void SET_VEHICLE_XENON_LIGHT_COLOR_INDEX(Vehicle vehicle, int colorIndex) {
			Invoker::Invoke<void, 4296>(vehicle, colorIndex);
		}

		int GET_VEHICLE_XENON_LIGHT_COLOR_INDEX(Vehicle vehicle) {
			return Invoker::Invoke<int, 4297>(vehicle);
		}

		void SET_VEHICLE_IS_CONSIDERED_BY_PLAYER(Vehicle vehicle, BOOL toggle) {
			Invoker::Invoke<void, 4298>(vehicle, toggle);
		}

		void SET_VEHICLE_WILL_FORCE_OTHER_VEHICLES_TO_STOP(Vehicle vehicle, BOOL toggle) {
			Invoker::Invoke<void, 4299>(vehicle, toggle);
		}

		void SET_VEHICLE_ACT_AS_IF_HAS_SIREN_ON(Vehicle vehicle, BOOL p1) {
			Invoker::Invoke<void, 4300>(vehicle, p1);
		}

		void SET_VEHICLE_USE_MORE_RESTRICTIVE_SPAWN_CHECKS(Vehicle vehicle, BOOL p1) {
			Invoker::Invoke<void, 4301>(vehicle, p1);
		}

		void SET_VEHICLE_MAY_BE_USED_BY_GOTO_POINT_ANY_MEANS(Vehicle vehicle, BOOL p1) {
			Invoker::Invoke<void, 4302>(vehicle, p1);
		}

		void GET_RANDOM_VEHICLE_MODEL_IN_MEMORY(BOOL p0, Hash* modelHash, int* successIndicator) {
			Invoker::Invoke<void, 4303>(p0, modelHash, successIndicator);
		}

		int GET_VEHICLE_DOOR_LOCK_STATUS(Vehicle vehicle) {
			return Invoker::Invoke<int, 4304>(vehicle);
		}

		int GET_VEHICLE_INDIVIDUAL_DOOR_LOCK_STATUS(Vehicle vehicle, int doorId) {
			return Invoker::Invoke<int, 4305>(vehicle, doorId);
		}

		BOOL IS_VEHICLE_DOOR_DAMAGED(Vehicle veh, int doorID) {
			return Invoker::Invoke<BOOL, 4306>(veh, doorID);
		}

		void SET_DOOR_ALLOWED_TO_BE_BROKEN_OFF(Vehicle vehicle, int doorId, BOOL isBreakable) {
			Invoker::Invoke<void, 4307>(vehicle, doorId, isBreakable);
		}

		BOOL IS_VEHICLE_BUMPER_BOUNCING(Vehicle vehicle, BOOL frontBumper) {
			return Invoker::Invoke<BOOL, 4308>(vehicle, frontBumper);
		}

		BOOL IS_VEHICLE_BUMPER_BROKEN_OFF(Vehicle vehicle, BOOL frontBumper) {
			return Invoker::Invoke<BOOL, 4309>(vehicle, frontBumper);
		}

		BOOL IS_COP_VEHICLE_IN_AREA_3D(float x1, float x2, float y1, float y2, float z1, float z2) {
			return Invoker::Invoke<BOOL, 4310>(x1, x2, y1, y2, z1, z2);
		}

		BOOL IS_VEHICLE_ON_ALL_WHEELS(Vehicle vehicle) {
			return Invoker::Invoke<BOOL, 4311>(vehicle);
		}

		int GET_VEHICLE_MODEL_VALUE(Hash vehicleModel) {
			return Invoker::Invoke<int, 4312>(vehicleModel);
		}

		Hash GET_VEHICLE_LAYOUT_HASH(Vehicle vehicle) {
			return Invoker::Invoke<Hash, 4313>(vehicle);
		}

		Hash GET_IN_VEHICLE_CLIPSET_HASH_FOR_SEAT(Vehicle vehicle, int p1) {
			return Invoker::Invoke<Hash, 4314>(vehicle, p1);
		}

		void SET_RENDER_TRAIN_AS_DERAILED(Vehicle train, BOOL toggle) {
			Invoker::Invoke<void, 4315>(train, toggle);
		}

		void SET_VEHICLE_EXTRA_COLOURS(Vehicle vehicle, int pearlescentColor, int wheelColor) {
			Invoker::Invoke<void, 4316>(vehicle, pearlescentColor, wheelColor);
		}

		void GET_VEHICLE_EXTRA_COLOURS(Vehicle vehicle, int* pearlescentColor, int* wheelColor) {
			Invoker::Invoke<void, 4317>(vehicle, pearlescentColor, wheelColor);
		}

		void SET_VEHICLE_EXTRA_COLOUR_5(Vehicle vehicle, int color) {
			Invoker::Invoke<void, 4318>(vehicle, color);
		}

		void GET_VEHICLE_EXTRA_COLOUR_5(Vehicle vehicle, int* color) {
			Invoker::Invoke<void, 4319>(vehicle, color);
		}

		void SET_VEHICLE_EXTRA_COLOUR_6(Vehicle vehicle, int color) {
			Invoker::Invoke<void, 4320>(vehicle, color);
		}

		void GET_VEHICLE_EXTRA_COLOUR_6(Vehicle vehicle, int* color) {
			Invoker::Invoke<void, 4321>(vehicle, color);
		}

		void STOP_ALL_GARAGE_ACTIVITY() {
			Invoker::Invoke<void, 4322>();
		}

		void SET_VEHICLE_FIXED(Vehicle vehicle) {
			Invoker::Invoke<void, 4323>(vehicle);
		}

		void SET_VEHICLE_DEFORMATION_FIXED(Vehicle vehicle) {
			Invoker::Invoke<void, 4324>(vehicle);
		}

		void SET_VEHICLE_CAN_ENGINE_MISSFIRE(Vehicle vehicle, BOOL toggle) {
			Invoker::Invoke<void, 4325>(vehicle, toggle);
		}

		void SET_VEHICLE_CAN_LEAK_OIL(Vehicle vehicle, BOOL toggle) {
			Invoker::Invoke<void, 4326>(vehicle, toggle);
		}

		void SET_VEHICLE_CAN_LEAK_PETROL(Vehicle vehicle, BOOL toggle) {
			Invoker::Invoke<void, 4327>(vehicle, toggle);
		}

		void SET_DISABLE_VEHICLE_PETROL_TANK_FIRES(Vehicle vehicle, BOOL toggle) {
			Invoker::Invoke<void, 4328>(vehicle, toggle);
		}

		void SET_DISABLE_VEHICLE_PETROL_TANK_DAMAGE(Vehicle vehicle, BOOL toggle) {
			Invoker::Invoke<void, 4329>(vehicle, toggle);
		}

		void SET_DISABLE_VEHICLE_ENGINE_FIRES(Vehicle vehicle, BOOL toggle) {
			Invoker::Invoke<void, 4330>(vehicle, toggle);
		}

		void SET_VEHICLE_LIMIT_SPEED_WHEN_PLAYER_INACTIVE(Vehicle vehicle, BOOL toggle) {
			Invoker::Invoke<void, 4331>(vehicle, toggle);
		}

		void SET_VEHICLE_STOP_INSTANTLY_WHEN_PLAYER_INACTIVE(Vehicle vehicle, BOOL toggle) {
			Invoker::Invoke<void, 4332>(vehicle, toggle);
		}

		void SET_DISABLE_PRETEND_OCCUPANTS(Vehicle vehicle, BOOL toggle) {
			Invoker::Invoke<void, 4333>(vehicle, toggle);
		}

		void REMOVE_VEHICLES_FROM_GENERATORS_IN_AREA(float x1, float y1, float z1, float x2, float y2, float z2, Any p6) {
			Invoker::Invoke<void, 4334>(x1, y1, z1, x2, y2, z2, p6);
		}

		void SET_VEHICLE_STEER_BIAS(Vehicle vehicle, float value) {
			Invoker::Invoke<void, 4335>(vehicle, value);
		}

		BOOL IS_VEHICLE_EXTRA_TURNED_ON(Vehicle vehicle, int extraId) {
			return Invoker::Invoke<BOOL, 4336>(vehicle, extraId);
		}

		void SET_VEHICLE_EXTRA(Vehicle vehicle, int extraId, BOOL disable) {
			Invoker::Invoke<void, 4337>(vehicle, extraId, disable);
		}

		BOOL DOES_EXTRA_EXIST(Vehicle vehicle, int extraId) {
			return Invoker::Invoke<BOOL, 4338>(vehicle, extraId);
		}

		BOOL IS_EXTRA_BROKEN_OFF(Vehicle vehicle, int extraId) {
			return Invoker::Invoke<BOOL, 4339>(vehicle, extraId);
		}

		void SET_CONVERTIBLE_ROOF(Vehicle vehicle, BOOL p1) {
			Invoker::Invoke<void, 4340>(vehicle, p1);
		}

		void LOWER_CONVERTIBLE_ROOF(Vehicle vehicle, BOOL instantlyLower) {
			Invoker::Invoke<void, 4341>(vehicle, instantlyLower);
		}

		void RAISE_CONVERTIBLE_ROOF(Vehicle vehicle, BOOL instantlyRaise) {
			Invoker::Invoke<void, 4342>(vehicle, instantlyRaise);
		}

		int GET_CONVERTIBLE_ROOF_STATE(Vehicle vehicle) {
			return Invoker::Invoke<int, 4343>(vehicle);
		}

		BOOL IS_VEHICLE_A_CONVERTIBLE(Vehicle vehicle, BOOL p1) {
			return Invoker::Invoke<BOOL, 4344>(vehicle, p1);
		}

		BOOL TRANSFORM_TO_SUBMARINE(Vehicle vehicle, BOOL noAnimation) {
			return Invoker::Invoke<BOOL, 4345>(vehicle, noAnimation);
		}

		void TRANSFORM_TO_CAR(Vehicle vehicle, BOOL noAnimation) {
			Invoker::Invoke<void, 4346>(vehicle, noAnimation);
		}

		BOOL IS_VEHICLE_IN_SUBMARINE_MODE(Vehicle vehicle) {
			return Invoker::Invoke<BOOL, 4347>(vehicle);
		}

		BOOL IS_VEHICLE_STOPPED_AT_TRAFFIC_LIGHTS(Vehicle vehicle) {
			return Invoker::Invoke<BOOL, 4348>(vehicle);
		}

		void SET_VEHICLE_DAMAGE(Vehicle vehicle, float xOffset, float yOffset, float zOffset, float damage, float radius, BOOL focusOnModel) {
			Invoker::Invoke<void, 4349>(vehicle, xOffset, yOffset, zOffset, damage, radius, focusOnModel);
		}

		void SET_VEHICLE_OCCUPANTS_TAKE_EXPLOSIVE_DAMAGE(Vehicle vehicle, BOOL toggle) {
			Invoker::Invoke<void, 4350>(vehicle, toggle);
		}

		float GET_VEHICLE_ENGINE_HEALTH(Vehicle vehicle) {
			return Invoker::Invoke<float, 4351>(vehicle);
		}

		void SET_VEHICLE_ENGINE_HEALTH(Vehicle vehicle, float health) {
			Invoker::Invoke<void, 4352>(vehicle, health);
		}

		void SET_PLANE_ENGINE_HEALTH(Vehicle vehicle, float health) {
			Invoker::Invoke<void, 4353>(vehicle, health);
		}

		float GET_VEHICLE_PETROL_TANK_HEALTH(Vehicle vehicle) {
			return Invoker::Invoke<float, 4354>(vehicle);
		}

		void SET_VEHICLE_PETROL_TANK_HEALTH(Vehicle vehicle, float health) {
			Invoker::Invoke<void, 4355>(vehicle, health);
		}

		BOOL IS_VEHICLE_STUCK_TIMER_UP(Vehicle vehicle, int p1, int ms) {
			return Invoker::Invoke<BOOL, 4356>(vehicle, p1, ms);
		}

		void RESET_VEHICLE_STUCK_TIMER(Vehicle vehicle, int nullAttributes) {
			Invoker::Invoke<void, 4357>(vehicle, nullAttributes);
		}

		BOOL IS_VEHICLE_DRIVEABLE(Vehicle vehicle, BOOL isOnFireCheck) {
			return Invoker::Invoke<BOOL, 4358>(vehicle, isOnFireCheck);
		}

		void SET_VEHICLE_HAS_BEEN_OWNED_BY_PLAYER(Vehicle vehicle, BOOL owned) {
			Invoker::Invoke<void, 4359>(vehicle, owned);
		}

		void SET_VEHICLE_NEEDS_TO_BE_HOTWIRED(Vehicle vehicle, BOOL toggle) {
			Invoker::Invoke<void, 4360>(vehicle, toggle);
		}

		void SET_VEHICLE_BLIP_THROTTLE_RANDOMLY(Vehicle vehicle, BOOL p1) {
			Invoker::Invoke<void, 4361>(vehicle, p1);
		}

		void SET_POLICE_FOCUS_WILL_TRACK_VEHICLE(Vehicle vehicle, BOOL toggle) {
			Invoker::Invoke<void, 4362>(vehicle, toggle);
		}

		void START_VEHICLE_HORN(Vehicle vehicle, int duration, Hash mode, BOOL forever) {
			Invoker::Invoke<void, 4363>(vehicle, duration, mode, forever);
		}

		void SET_VEHICLE_IN_CAR_MOD_SHOP(Vehicle vehicle, BOOL toggle) {
			Invoker::Invoke<void, 4364>(vehicle, toggle);
		}

		void SET_VEHICLE_HAS_STRONG_AXLES(Vehicle vehicle, BOOL toggle) {
			Invoker::Invoke<void, 4365>(vehicle, toggle);
		}

		const char* GET_DISPLAY_NAME_FROM_VEHICLE_MODEL(Hash modelHash) {
			return Invoker::Invoke<const char*, 4366>(modelHash);
		}

		const char* GET_MAKE_NAME_FROM_VEHICLE_MODEL(Hash modelHash) {
			return Invoker::Invoke<const char*, 4367>(modelHash);
		}

		Vector3 GET_VEHICLE_DEFORMATION_AT_POS(Vehicle vehicle, float offsetX, float offsetY, float offsetZ) {
			return Invoker::Invoke<Vector3, 4368>(vehicle, offsetX, offsetY, offsetZ);
		}

		void SET_VEHICLE_LIVERY(Vehicle vehicle, int livery) {
			Invoker::Invoke<void, 4369>(vehicle, livery);
		}

		int GET_VEHICLE_LIVERY(Vehicle vehicle) {
			return Invoker::Invoke<int, 4370>(vehicle);
		}

		int GET_VEHICLE_LIVERY_COUNT(Vehicle vehicle) {
			return Invoker::Invoke<int, 4371>(vehicle);
		}

		void SET_VEHICLE_LIVERY2(Vehicle vehicle, int livery) {
			Invoker::Invoke<void, 4372>(vehicle, livery);
		}

		int GET_VEHICLE_LIVERY2(Vehicle vehicle) {
			return Invoker::Invoke<int, 4373>(vehicle);
		}

		int GET_VEHICLE_LIVERY2_COUNT(Vehicle vehicle) {
			return Invoker::Invoke<int, 4374>(vehicle);
		}

		BOOL IS_VEHICLE_WINDOW_INTACT(Vehicle vehicle, int windowIndex) {
			return Invoker::Invoke<BOOL, 4375>(vehicle, windowIndex);
		}

		BOOL ARE_ALL_VEHICLE_WINDOWS_INTACT(Vehicle vehicle) {
			return Invoker::Invoke<BOOL, 4376>(vehicle);
		}

		BOOL ARE_ANY_VEHICLE_SEATS_FREE(Vehicle vehicle) {
			return Invoker::Invoke<BOOL, 4377>(vehicle);
		}

		void RESET_VEHICLE_WHEELS(Vehicle vehicle, BOOL toggle) {
			Invoker::Invoke<void, 4378>(vehicle, toggle);
		}

		BOOL IS_HELI_PART_BROKEN(Vehicle vehicle, BOOL p1, BOOL p2, BOOL p3) {
			return Invoker::Invoke<BOOL, 4379>(vehicle, p1, p2, p3);
		}

		float GET_HELI_MAIN_ROTOR_HEALTH(Vehicle vehicle) {
			return Invoker::Invoke<float, 4380>(vehicle);
		}

		float GET_HELI_TAIL_ROTOR_HEALTH(Vehicle vehicle) {
			return Invoker::Invoke<float, 4381>(vehicle);
		}

		float GET_HELI_TAIL_BOOM_HEALTH(Vehicle vehicle) {
			return Invoker::Invoke<float, 4382>(vehicle);
		}

		void SET_HELI_MAIN_ROTOR_HEALTH(Vehicle vehicle, float health) {
			Invoker::Invoke<void, 4383>(vehicle, health);
		}

		void SET_HELI_TAIL_ROTOR_HEALTH(Vehicle vehicle, float health) {
			Invoker::Invoke<void, 4384>(vehicle, health);
		}

		BOOL SET_HELI_TAIL_BOOM_CAN_BREAK_OFF(Vehicle vehicle, BOOL toggle) {
			return Invoker::Invoke<BOOL, 4385>(vehicle, toggle);
		}

		void SET_VEHICLE_NAME_DEBUG(Vehicle vehicle, const char* name) {
			Invoker::Invoke<void, 4386>(vehicle, name);
		}

		void SET_VEHICLE_EXPLODES_ON_HIGH_EXPLOSION_DAMAGE(Vehicle vehicle, BOOL toggle) {
			Invoker::Invoke<void, 4387>(vehicle, toggle);
		}

		void SET_VEHICLE_EXPLODES_ON_EXPLOSION_DAMAGE_AT_ZERO_BODY_HEALTH(Vehicle vehicle, BOOL toggle) {
			Invoker::Invoke<void, 4388>(vehicle, toggle);
		}

		void SET_ALLOW_VEHICLE_EXPLODES_ON_CONTACT(Vehicle vehicle, BOOL toggle) {
			Invoker::Invoke<void, 4389>(vehicle, toggle);
		}

		void SET_VEHICLE_DISABLE_TOWING(Vehicle vehicle, BOOL toggle) {
			Invoker::Invoke<void, 4390>(vehicle, toggle);
		}

		BOOL GET_VEHICLE_HAS_LANDING_GEAR(Vehicle vehicle) {
			return Invoker::Invoke<BOOL, 4391>(vehicle);
		}

		void CONTROL_LANDING_GEAR(Vehicle vehicle, int state) {
			Invoker::Invoke<void, 4392>(vehicle, state);
		}

		int GET_LANDING_GEAR_STATE(Vehicle vehicle) {
			return Invoker::Invoke<int, 4393>(vehicle);
		}

		BOOL IS_ANY_VEHICLE_NEAR_POINT(float x, float y, float z, float radius) {
			return Invoker::Invoke<BOOL, 4394>(x, y, z, radius);
		}

		void REQUEST_VEHICLE_HIGH_DETAIL_MODEL(Vehicle vehicle) {
			Invoker::Invoke<void, 4395>(vehicle);
		}

		int _GET_VEHICLE_MODEL_NUM_DRIVE_GEARS(Hash vehicleModel) {
			return Invoker::Invoke<int, 4396>(vehicleModel);
		}

		int _GET_VEHICLE_MAX_DRIVE_GEAR_COUNT(Vehicle vehicle) {
			return Invoker::Invoke<int, 4397>(vehicle);
		}

		BOOL _GET_IS_VEHICLE_ELECTRIC(Hash vehicleModel) {
			return Invoker::Invoke<BOOL, 6593>(vehicleModel);
		}

		int _GET_VEHICLE_DRIVETRAIN_TYPE(Hash vehicleModel) {
			return Invoker::Invoke<int, 6594>(vehicleModel);
		}

		int _GET_VEHICLE_CURRENT_DRIVE_GEAR(Vehicle vehicle) {
			return Invoker::Invoke<int, 4398>(vehicle);
		}

		float _GET_VEHICLE_CURRENT_REV_RATIO(Vehicle vehicle) {
			return Invoker::Invoke<float, 4399>(vehicle);
		}

		void REMOVE_VEHICLE_HIGH_DETAIL_MODEL(Vehicle vehicle) {
			Invoker::Invoke<void, 4400>(vehicle);
		}

		BOOL IS_VEHICLE_HIGH_DETAIL(Vehicle vehicle) {
			return Invoker::Invoke<BOOL, 4401>(vehicle);
		}

		void REQUEST_VEHICLE_ASSET(Hash vehicleHash, int vehicleAsset) {
			Invoker::Invoke<void, 4402>(vehicleHash, vehicleAsset);
		}

		BOOL HAS_VEHICLE_ASSET_LOADED(int vehicleAsset) {
			return Invoker::Invoke<BOOL, 4403>(vehicleAsset);
		}

		void REMOVE_VEHICLE_ASSET(int vehicleAsset) {
			Invoker::Invoke<void, 4404>(vehicleAsset);
		}

		void SET_VEHICLE_TOW_TRUCK_ARM_POSITION(Vehicle vehicle, float position) {
			Invoker::Invoke<void, 4405>(vehicle, position);
		}

		void _SET_ATTACHED_VEHICLE_TO_TOW_TRUCK_ARM(Vehicle towTruck, Vehicle vehicle) {
			Invoker::Invoke<void, 4406>(towTruck, vehicle);
		}

		void ATTACH_VEHICLE_TO_TOW_TRUCK(Vehicle towTruck, Vehicle vehicle, BOOL rear, float hookOffsetX, float hookOffsetY, float hookOffsetZ) {
			Invoker::Invoke<void, 4407>(towTruck, vehicle, rear, hookOffsetX, hookOffsetY, hookOffsetZ);
		}

		void DETACH_VEHICLE_FROM_TOW_TRUCK(Vehicle towTruck, Vehicle vehicle) {
			Invoker::Invoke<void, 4408>(towTruck, vehicle);
		}

		BOOL DETACH_VEHICLE_FROM_ANY_TOW_TRUCK(Vehicle vehicle) {
			return Invoker::Invoke<BOOL, 4409>(vehicle);
		}

		BOOL IS_VEHICLE_ATTACHED_TO_TOW_TRUCK(Vehicle towTruck, Vehicle vehicle) {
			return Invoker::Invoke<BOOL, 4410>(towTruck, vehicle);
		}

		Entity GET_ENTITY_ATTACHED_TO_TOW_TRUCK(Vehicle towTruck) {
			return Invoker::Invoke<Entity, 4411>(towTruck);
		}

		Entity SET_VEHICLE_AUTOMATICALLY_ATTACHES(Vehicle vehicle, BOOL p1, Any p2) {
			return Invoker::Invoke<Entity, 4412>(vehicle, p1, p2);
		}

		void SET_VEHICLE_BULLDOZER_ARM_POSITION(Vehicle vehicle, float position, BOOL p2) {
			Invoker::Invoke<void, 4413>(vehicle, position, p2);
		}

		void SET_VEHICLE_TANK_TURRET_POSITION(Vehicle vehicle, float position, BOOL p2) {
			Invoker::Invoke<void, 4414>(vehicle, position, p2);
		}

		void SET_VEHICLE_TURRET_TARGET(Vehicle vehicle, BOOL p1, float x, float y, float z, BOOL p5) {
			Invoker::Invoke<void, 4415>(vehicle, p1, x, y, z, p5);
		}

		void SET_VEHICLE_TANK_STATIONARY(Vehicle vehicle, BOOL p1) {
			Invoker::Invoke<void, 4416>(vehicle, p1);
		}

		void SET_VEHICLE_TURRET_SPEED_THIS_FRAME(Vehicle vehicle, float speed) {
			Invoker::Invoke<void, 4417>(vehicle, speed);
		}

		void DISABLE_VEHICLE_TURRET_MOVEMENT_THIS_FRAME(Vehicle vehicle) {
			Invoker::Invoke<void, 4418>(vehicle);
		}

		void SET_VEHICLE_FLIGHT_NOZZLE_POSITION(Vehicle vehicle, float angleRatio) {
			Invoker::Invoke<void, 4419>(vehicle, angleRatio);
		}

		void SET_VEHICLE_FLIGHT_NOZZLE_POSITION_IMMEDIATE(Vehicle vehicle, float angle) {
			Invoker::Invoke<void, 4420>(vehicle, angle);
		}

		float GET_VEHICLE_FLIGHT_NOZZLE_POSITION(Vehicle plane) {
			return Invoker::Invoke<float, 4421>(plane);
		}

		void SET_DISABLE_VERTICAL_FLIGHT_MODE_TRANSITION(Vehicle vehicle, BOOL toggle) {
			Invoker::Invoke<void, 4422>(vehicle, toggle);
		}

		BOOL GENERATE_VEHICLE_CREATION_POS_FROM_PATHS(Vector3* outVec, Any p1, Vector3* outVec1, Any p3, Any p4, Any p5, Any p6, Any p7, Any p8) {
			return Invoker::Invoke<BOOL, 4423>(outVec, p1, outVec1, p3, p4, p5, p6, p7, p8);
		}

		void SET_VEHICLE_BURNOUT(Vehicle vehicle, BOOL toggle) {
			Invoker::Invoke<void, 4424>(vehicle, toggle);
		}

		BOOL IS_VEHICLE_IN_BURNOUT(Vehicle vehicle) {
			return Invoker::Invoke<BOOL, 4425>(vehicle);
		}

		void SET_VEHICLE_REDUCE_GRIP(Vehicle vehicle, BOOL toggle) {
			Invoker::Invoke<void, 4426>(vehicle, toggle);
		}

		void SET_VEHICLE_REDUCE_GRIP_LEVEL(Vehicle vehicle, int val) {
			Invoker::Invoke<void, 4427>(vehicle, val);
		}

		void SET_VEHICLE_INDICATOR_LIGHTS(Vehicle vehicle, int turnSignal, BOOL toggle) {
			Invoker::Invoke<void, 4428>(vehicle, turnSignal, toggle);
		}

		void SET_VEHICLE_BRAKE_LIGHTS(Vehicle vehicle, BOOL toggle) {
			Invoker::Invoke<void, 4429>(vehicle, toggle);
		}

		void SET_VEHICLE_TAIL_LIGHTS(Vehicle vehicle, BOOL toggle) {
			Invoker::Invoke<void, 4430>(vehicle, toggle);
		}

		void SET_VEHICLE_HANDBRAKE(Vehicle vehicle, BOOL toggle) {
			Invoker::Invoke<void, 4431>(vehicle, toggle);
		}

		void SET_VEHICLE_BRAKE(Vehicle vehicle, BOOL toggle) {
			Invoker::Invoke<void, 4432>(vehicle, toggle);
		}

		void INSTANTLY_FILL_VEHICLE_POPULATION() {
			Invoker::Invoke<void, 4433>();
		}

		BOOL HAS_INSTANT_FILL_VEHICLE_POPULATION_FINISHED() {
			return Invoker::Invoke<BOOL, 4434>();
		}

		void NETWORK_ENABLE_EMPTY_CROWDING_VEHICLES_REMOVAL(BOOL toggle) {
			Invoker::Invoke<void, 4435>(toggle);
		}

		void NETWORK_CAP_EMPTY_CROWDING_VEHICLES_REMOVAL(int p0) {
			Invoker::Invoke<void, 4436>(p0);
		}

		BOOL GET_VEHICLE_TRAILER_VEHICLE(Vehicle vehicle, Vehicle* trailer) {
			return Invoker::Invoke<BOOL, 4437>(vehicle, trailer);
		}

		Vehicle _GET_VEHICLE_TRAILER_PARENT_VEHICLE(Vehicle trailer) {
			return Invoker::Invoke<Vehicle, 4438>(trailer);
		}

		void SET_VEHICLE_USES_LARGE_REAR_RAMP(Vehicle vehicle, BOOL toggle) {
			Invoker::Invoke<void, 4439>(vehicle, toggle);
		}

		void SET_VEHICLE_RUDDER_BROKEN(Vehicle vehicle, BOOL toggle) {
			Invoker::Invoke<void, 4440>(vehicle, toggle);
		}

		void SET_VEHICLE_TAIL_BROKEN(Vehicle vehicle, BOOL toggle) {
			Invoker::Invoke<void, 6714>(vehicle, toggle);
		}

		void SET_CONVERTIBLE_ROOF_LATCH_STATE(Vehicle vehicle, BOOL state) {
			Invoker::Invoke<void, 4441>(vehicle, state);
		}

		float GET_VEHICLE_ESTIMATED_MAX_SPEED(Vehicle vehicle) {
			return Invoker::Invoke<float, 4442>(vehicle);
		}

		float GET_VEHICLE_MAX_BRAKING(Vehicle vehicle) {
			return Invoker::Invoke<float, 4443>(vehicle);
		}

		float GET_VEHICLE_MAX_TRACTION(Vehicle vehicle) {
			return Invoker::Invoke<float, 4444>(vehicle);
		}

		float GET_VEHICLE_ACCELERATION(Vehicle vehicle) {
			return Invoker::Invoke<float, 4445>(vehicle);
		}

		float GET_VEHICLE_MODEL_ESTIMATED_MAX_SPEED(Hash modelHash) {
			return Invoker::Invoke<float, 4446>(modelHash);
		}

		float GET_VEHICLE_MODEL_MAX_BRAKING(Hash modelHash) {
			return Invoker::Invoke<float, 4447>(modelHash);
		}

		float GET_VEHICLE_MODEL_MAX_BRAKING_MAX_MODS(Hash modelHash) {
			return Invoker::Invoke<float, 4448>(modelHash);
		}

		float GET_VEHICLE_MODEL_MAX_TRACTION(Hash modelHash) {
			return Invoker::Invoke<float, 4449>(modelHash);
		}

		float GET_VEHICLE_MODEL_ACCELERATION(Hash modelHash) {
			return Invoker::Invoke<float, 4450>(modelHash);
		}

		float GET_VEHICLE_MODEL_ACCELERATION_MAX_MODS(Hash modelHash) {
			return Invoker::Invoke<float, 4451>(modelHash);
		}

		float GET_FLYING_VEHICLE_MODEL_AGILITY(Hash modelHash) {
			return Invoker::Invoke<float, 4452>(modelHash);
		}

		float GET_BOAT_VEHICLE_MODEL_AGILITY(Hash modelHash) {
			return Invoker::Invoke<float, 4453>(modelHash);
		}

		float GET_VEHICLE_CLASS_ESTIMATED_MAX_SPEED(int vehicleClass) {
			return Invoker::Invoke<float, 4454>(vehicleClass);
		}

		float GET_VEHICLE_CLASS_MAX_TRACTION(int vehicleClass) {
			return Invoker::Invoke<float, 4455>(vehicleClass);
		}

		float GET_VEHICLE_CLASS_MAX_AGILITY(int vehicleClass) {
			return Invoker::Invoke<float, 4456>(vehicleClass);
		}

		float GET_VEHICLE_CLASS_MAX_ACCELERATION(int vehicleClass) {
			return Invoker::Invoke<float, 4457>(vehicleClass);
		}

		float GET_VEHICLE_CLASS_MAX_BRAKING(int vehicleClass) {
			return Invoker::Invoke<float, 4458>(vehicleClass);
		}

		int ADD_ROAD_NODE_SPEED_ZONE(float x, float y, float z, float radius, float speed, BOOL p5) {
			return Invoker::Invoke<int, 4459>(x, y, z, radius, speed, p5);
		}

		BOOL REMOVE_ROAD_NODE_SPEED_ZONE(int speedzone) {
			return Invoker::Invoke<BOOL, 4460>(speedzone);
		}

		void OPEN_BOMB_BAY_DOORS(Vehicle vehicle) {
			Invoker::Invoke<void, 4461>(vehicle);
		}

		void CLOSE_BOMB_BAY_DOORS(Vehicle vehicle) {
			Invoker::Invoke<void, 4462>(vehicle);
		}

		BOOL GET_ARE_BOMB_BAY_DOORS_OPEN(Vehicle aircraft) {
			return Invoker::Invoke<BOOL, 4463>(aircraft);
		}

		BOOL IS_VEHICLE_SEARCHLIGHT_ON(Vehicle vehicle) {
			return Invoker::Invoke<BOOL, 4464>(vehicle);
		}

		void SET_VEHICLE_SEARCHLIGHT(Vehicle heli, BOOL toggle, BOOL canBeUsedByAI) {
			Invoker::Invoke<void, 4465>(heli, toggle, canBeUsedByAI);
		}

		BOOL DOES_VEHICLE_HAVE_SEARCHLIGHT(Vehicle vehicle) {
			return Invoker::Invoke<BOOL, 4466>(vehicle);
		}

		BOOL IS_ENTRY_POINT_FOR_SEAT_CLEAR(Ped ped, Vehicle vehicle, int seatIndex, BOOL side, BOOL onEnter) {
			return Invoker::Invoke<BOOL, 4467>(ped, vehicle, seatIndex, side, onEnter);
		}

		Vector3 GET_ENTRY_POINT_POSITION(Vehicle vehicle, int doorId) {
			return Invoker::Invoke<Vector3, 4468>(vehicle, doorId);
		}

		BOOL CAN_SHUFFLE_SEAT(Vehicle vehicle, int seatIndex) {
			return Invoker::Invoke<BOOL, 4469>(vehicle, seatIndex);
		}

		int GET_NUM_MOD_KITS(Vehicle vehicle) {
			return Invoker::Invoke<int, 4470>(vehicle);
		}

		void SET_VEHICLE_MOD_KIT(Vehicle vehicle, int modKit) {
			Invoker::Invoke<void, 4471>(vehicle, modKit);
		}

		int GET_VEHICLE_MOD_KIT(Vehicle vehicle) {
			return Invoker::Invoke<int, 4472>(vehicle);
		}

		int GET_VEHICLE_MOD_KIT_TYPE(Vehicle vehicle) {
			return Invoker::Invoke<int, 4473>(vehicle);
		}

		int GET_VEHICLE_WHEEL_TYPE(Vehicle vehicle) {
			return Invoker::Invoke<int, 4474>(vehicle);
		}

		void SET_VEHICLE_WHEEL_TYPE(Vehicle vehicle, int WheelType) {
			Invoker::Invoke<void, 4475>(vehicle, WheelType);
		}

		int GET_NUM_MOD_COLORS(int paintType, BOOL p1) {
			return Invoker::Invoke<int, 4476>(paintType, p1);
		}

		void SET_VEHICLE_MOD_COLOR_1(Vehicle vehicle, int paintType, int color, int pearlescentColor) {
			Invoker::Invoke<void, 4477>(vehicle, paintType, color, pearlescentColor);
		}

		void SET_VEHICLE_MOD_COLOR_2(Vehicle vehicle, int paintType, int color) {
			Invoker::Invoke<void, 4478>(vehicle, paintType, color);
		}

		void GET_VEHICLE_MOD_COLOR_1(Vehicle vehicle, int* paintType, int* color, int* pearlescentColor) {
			Invoker::Invoke<void, 4479>(vehicle, paintType, color, pearlescentColor);
		}

		void GET_VEHICLE_MOD_COLOR_2(Vehicle vehicle, int* paintType, int* color) {
			Invoker::Invoke<void, 4480>(vehicle, paintType, color);
		}

		const char* GET_VEHICLE_MOD_COLOR_1_NAME(Vehicle vehicle, BOOL p1) {
			return Invoker::Invoke<const char*, 4481>(vehicle, p1);
		}

		const char* GET_VEHICLE_MOD_COLOR_2_NAME(Vehicle vehicle) {
			return Invoker::Invoke<const char*, 4482>(vehicle);
		}

		BOOL HAVE_VEHICLE_MODS_STREAMED_IN(Vehicle vehicle) {
			return Invoker::Invoke<BOOL, 4483>(vehicle);
		}

		BOOL IS_VEHICLE_MOD_GEN9_EXCLUSIVE(Vehicle vehicle, int modType, int modIndex) {
			return Invoker::Invoke<BOOL, 4484>(vehicle, modType, modIndex);
		}

		void SET_VEHICLE_MOD(Vehicle vehicle, int modType, int modIndex, BOOL customTires) {
			Invoker::Invoke<void, 4485>(vehicle, modType, modIndex, customTires);
		}

		int GET_VEHICLE_MOD(Vehicle vehicle, int modType) {
			return Invoker::Invoke<int, 4486>(vehicle, modType);
		}

		int GET_VEHICLE_MOD_VARIATION(Vehicle vehicle, int modType) {
			return Invoker::Invoke<int, 4487>(vehicle, modType);
		}

		int GET_NUM_VEHICLE_MODS(Vehicle vehicle, int modType) {
			return Invoker::Invoke<int, 4488>(vehicle, modType);
		}

		void REMOVE_VEHICLE_MOD(Vehicle vehicle, int modType) {
			Invoker::Invoke<void, 4489>(vehicle, modType);
		}

		void TOGGLE_VEHICLE_MOD(Vehicle vehicle, int modType, BOOL toggle) {
			Invoker::Invoke<void, 4490>(vehicle, modType, toggle);
		}

		BOOL IS_TOGGLE_MOD_ON(Vehicle vehicle, int modType) {
			return Invoker::Invoke<BOOL, 4491>(vehicle, modType);
		}

		const char* GET_MOD_TEXT_LABEL(Vehicle vehicle, int modType, int modValue) {
			return Invoker::Invoke<const char*, 4492>(vehicle, modType, modValue);
		}

		const char* GET_MOD_SLOT_NAME(Vehicle vehicle, int modType) {
			return Invoker::Invoke<const char*, 4493>(vehicle, modType);
		}

		const char* GET_LIVERY_NAME(Vehicle vehicle, int liveryIndex) {
			return Invoker::Invoke<const char*, 4494>(vehicle, liveryIndex);
		}

		int GET_VEHICLE_MOD_MODIFIER_VALUE(Vehicle vehicle, int modType, int modIndex) {
			return Invoker::Invoke<int, 4495>(vehicle, modType, modIndex);
		}

		Hash GET_VEHICLE_MOD_IDENTIFIER_HASH(Vehicle vehicle, int modType, int modIndex) {
			return Invoker::Invoke<Hash, 4496>(vehicle, modType, modIndex);
		}

		void PRELOAD_VEHICLE_MOD(Vehicle vehicle, int modType, int modIndex) {
			Invoker::Invoke<void, 4497>(vehicle, modType, modIndex);
		}

		BOOL HAS_PRELOAD_MODS_FINISHED(Vehicle vehicle) {
			return Invoker::Invoke<BOOL, 4498>(vehicle);
		}

		void RELEASE_PRELOAD_MODS(Vehicle vehicle) {
			Invoker::Invoke<void, 4499>(vehicle);
		}

		void SET_VEHICLE_TYRE_SMOKE_COLOR(Vehicle vehicle, int r, int g, int b) {
			Invoker::Invoke<void, 4500>(vehicle, r, g, b);
		}

		void GET_VEHICLE_TYRE_SMOKE_COLOR(Vehicle vehicle, int* r, int* g, int* b) {
			Invoker::Invoke<void, 4501>(vehicle, r, g, b);
		}

		void SET_VEHICLE_WINDOW_TINT(Vehicle vehicle, int tint) {
			Invoker::Invoke<void, 4502>(vehicle, tint);
		}

		int GET_VEHICLE_WINDOW_TINT(Vehicle vehicle) {
			return Invoker::Invoke<int, 4503>(vehicle);
		}

		int GET_NUM_VEHICLE_WINDOW_TINTS() {
			return Invoker::Invoke<int, 4504>();
		}

		void GET_VEHICLE_COLOR(Vehicle vehicle, int* r, int* g, int* b) {
			Invoker::Invoke<void, 4505>(vehicle, r, g, b);
		}

		int GET_VEHICLE_COLOURS_WHICH_CAN_BE_SET(Vehicle vehicle) {
			return Invoker::Invoke<int, 4506>(vehicle);
		}

		Hash GET_VEHICLE_CAUSE_OF_DESTRUCTION(Vehicle vehicle) {
			return Invoker::Invoke<Hash, 4507>(vehicle);
		}

		void OVERRIDE_PLANE_DAMAGE_THREHSOLD(Vehicle vehicle, float health) {
			Invoker::Invoke<void, 4508>(vehicle, health);
		}

		void _SET_TRANSMISSION_REDUCED_GEAR_RATIO(Vehicle vehicle, BOOL toggle) {
			Invoker::Invoke<void, 4509>(vehicle, toggle);
		}

		int _GET_VEHICLE_DESIRED_DRIVE_GEAR(Vehicle vehicle) {
			return Invoker::Invoke<int, 4510>(vehicle);
		}

		BOOL GET_IS_LEFT_VEHICLE_HEADLIGHT_DAMAGED(Vehicle vehicle) {
			return Invoker::Invoke<BOOL, 4511>(vehicle);
		}

		BOOL GET_IS_RIGHT_VEHICLE_HEADLIGHT_DAMAGED(Vehicle vehicle) {
			return Invoker::Invoke<BOOL, 4512>(vehicle);
		}

		BOOL GET_BOTH_VEHICLE_HEADLIGHTS_DAMAGED(Vehicle vehicle) {
			return Invoker::Invoke<BOOL, 4513>(vehicle);
		}

		void MODIFY_VEHICLE_TOP_SPEED(Vehicle vehicle, float value) {
			Invoker::Invoke<void, 4514>(vehicle, value);
		}

		void SET_VEHICLE_MAX_SPEED(Vehicle vehicle, float speed) {
			Invoker::Invoke<void, 4515>(vehicle, speed);
		}

		void SET_VEHICLE_STAYS_FROZEN_WHEN_CLEANED_UP(Vehicle vehicle, BOOL toggle) {
			Invoker::Invoke<void, 4516>(vehicle, toggle);
		}

		void SET_VEHICLE_ACT_AS_IF_HIGH_SPEED_FOR_FRAG_SMASHING(Vehicle vehicle, BOOL p1) {
			Invoker::Invoke<void, 4517>(vehicle, p1);
		}

		void SET_PEDS_CAN_FALL_OFF_THIS_VEHICLE_FROM_LARGE_FALL_DAMAGE(Vehicle vehicle, BOOL toggle, float p2) {
			Invoker::Invoke<void, 4518>(vehicle, toggle, p2);
		}

		int ADD_VEHICLE_COMBAT_ANGLED_AVOIDANCE_AREA(float p0, float p1, float p2, float p3, float p4, float p5, float p6) {
			return Invoker::Invoke<int, 4519>(p0, p1, p2, p3, p4, p5, p6);
		}

		void REMOVE_VEHICLE_COMBAT_AVOIDANCE_AREA(int p0) {
			Invoker::Invoke<void, 4520>(p0);
		}

		BOOL IS_ANY_PED_RAPPELLING_FROM_HELI(Vehicle vehicle) {
			return Invoker::Invoke<BOOL, 4521>(vehicle);
		}

		void SET_VEHICLE_CHEAT_POWER_INCREASE(Vehicle vehicle, float value) {
			Invoker::Invoke<void, 4522>(vehicle, value);
		}

		void SET_VEHICLE_INFLUENCES_WANTED_LEVEL(Any p0, BOOL p1) {
			Invoker::Invoke<void, 4523>(p0, p1);
		}

		void SET_VEHICLE_IS_WANTED(Vehicle vehicle, BOOL state) {
			Invoker::Invoke<void, 4524>(vehicle, state);
		}

		void SWING_BOAT_BOOM_TO_RATIO(Vehicle vehicle, float ratio) {
			Invoker::Invoke<void, 4525>(vehicle, ratio);
		}

		void SWING_BOAT_BOOM_FREELY(Vehicle vehicle, BOOL toggle) {
			Invoker::Invoke<void, 4526>(vehicle, toggle);
		}

		void ALLOW_BOAT_BOOM_TO_ANIMATE(Vehicle vehicle, BOOL toggle) {
			Invoker::Invoke<void, 4527>(vehicle, toggle);
		}

		float GET_BOAT_BOOM_POSITION_RATIO(Vehicle vehicle) {
			return Invoker::Invoke<float, 4528>(vehicle);
		}

		void DISABLE_PLANE_AILERON(Vehicle vehicle, BOOL p1, BOOL p2) {
			Invoker::Invoke<void, 4529>(vehicle, p1, p2);
		}

		BOOL GET_IS_VEHICLE_ENGINE_RUNNING(Vehicle vehicle) {
			return Invoker::Invoke<BOOL, 4530>(vehicle);
		}

		void SET_VEHICLE_USE_ALTERNATE_HANDLING(Vehicle vehicle, BOOL toggle) {
			Invoker::Invoke<void, 4531>(vehicle, toggle);
		}

		void SET_BIKE_ON_STAND(Vehicle vehicle, float x, float y) {
			Invoker::Invoke<void, 4532>(vehicle, x, y);
		}

		void SET_VEHICLE_NOT_STEALABLE_AMBIENTLY(Vehicle vehicle, BOOL p1) {
			Invoker::Invoke<void, 4533>(vehicle, p1);
		}

		void LOCK_DOORS_WHEN_NO_LONGER_NEEDED(Vehicle vehicle) {
			Invoker::Invoke<void, 4534>(vehicle);
		}

		void SET_LAST_DRIVEN_VEHICLE(Vehicle vehicle) {
			Invoker::Invoke<void, 4535>(vehicle);
		}

		Vehicle GET_LAST_DRIVEN_VEHICLE() {
			return Invoker::Invoke<Vehicle, 4536>();
		}

		void CLEAR_LAST_DRIVEN_VEHICLE() {
			Invoker::Invoke<void, 4537>();
		}

		void SET_VEHICLE_HAS_BEEN_DRIVEN_FLAG(Vehicle vehicle, BOOL toggle) {
			Invoker::Invoke<void, 4538>(vehicle, toggle);
		}

		void SET_TASK_VEHICLE_GOTO_PLANE_MIN_HEIGHT_ABOVE_TERRAIN(Vehicle plane, int height) {
			Invoker::Invoke<void, 4539>(plane, height);
		}

		void SET_VEHICLE_LOD_MULTIPLIER(Vehicle vehicle, float multiplier) {
			Invoker::Invoke<void, 4540>(vehicle, multiplier);
		}

		void SET_VEHICLE_CAN_SAVE_IN_GARAGE(Vehicle vehicle, BOOL toggle) {
			Invoker::Invoke<void, 4541>(vehicle, toggle);
		}

		int GET_VEHICLE_NUM_OF_BROKEN_OFF_PARTS(Vehicle vehicle) {
			return Invoker::Invoke<int, 4542>(vehicle);
		}

		int GET_VEHICLE_NUM_OF_BROKEN_LOOSEN_PARTS(Vehicle vehicle) {
			return Invoker::Invoke<int, 4543>(vehicle);
		}

		void SET_FORCE_VEHICLE_ENGINE_DAMAGE_BY_BULLET(Any p0, BOOL p1) {
			Invoker::Invoke<void, 4544>(p0, p1);
		}

		void SET_VEHICLE_GENERATES_ENGINE_SHOCKING_EVENTS(Vehicle vehicle, BOOL toggle) {
			Invoker::Invoke<void, 4545>(vehicle, toggle);
		}

		void COPY_VEHICLE_DAMAGES(Vehicle sourceVehicle, Vehicle targetVehicle) {
			Invoker::Invoke<void, 4546>(sourceVehicle, targetVehicle);
		}

		void DISABLE_VEHICLE_EXPLOSION_BREAK_OFF_PARTS() {
			Invoker::Invoke<void, 4547>();
		}

		void SET_LIGHTS_CUTOFF_DISTANCE_TWEAK(float distance) {
			Invoker::Invoke<void, 4548>(distance);
		}

		void SET_VEHICLE_SHOOT_AT_TARGET(Ped driver, Entity entity, float xTarget, float yTarget, float zTarget) {
			Invoker::Invoke<void, 4549>(driver, entity, xTarget, yTarget, zTarget);
		}

		BOOL GET_VEHICLE_LOCK_ON_TARGET(Vehicle vehicle, Entity* entity) {
			return Invoker::Invoke<BOOL, 4550>(vehicle, entity);
		}

		void SET_FORCE_HD_VEHICLE(Vehicle vehicle, BOOL toggle) {
			Invoker::Invoke<void, 4551>(vehicle, toggle);
		}

		void SET_VEHICLE_CUSTOM_PATH_NODE_STREAMING_RADIUS(Vehicle vehicle, float p1) {
			Invoker::Invoke<void, 4552>(vehicle, p1);
		}

		int GET_VEHICLE_PLATE_TYPE(Vehicle vehicle) {
			return Invoker::Invoke<int, 4553>(vehicle);
		}

		void TRACK_VEHICLE_VISIBILITY(Vehicle vehicle) {
			Invoker::Invoke<void, 4554>(vehicle);
		}

		BOOL IS_VEHICLE_VISIBLE(Vehicle vehicle) {
			return Invoker::Invoke<BOOL, 4555>(vehicle);
		}

		void SET_VEHICLE_GRAVITY(Vehicle vehicle, BOOL toggle) {
			Invoker::Invoke<void, 4556>(vehicle, toggle);
		}

		void SET_ENABLE_VEHICLE_SLIPSTREAMING(BOOL toggle) {
			Invoker::Invoke<void, 4557>(toggle);
		}

		void SET_VEHICLE_SLIPSTREAMING_SHOULD_TIME_OUT(BOOL toggle) {
			Invoker::Invoke<void, 4558>(toggle);
		}

		float GET_VEHICLE_CURRENT_TIME_IN_SLIP_STREAM(Vehicle vehicle) {
			return Invoker::Invoke<float, 4559>(vehicle);
		}

		BOOL IS_VEHICLE_PRODUCING_SLIP_STREAM(Vehicle vehicle) {
			return Invoker::Invoke<BOOL, 4560>(vehicle);
		}

		void SET_VEHICLE_INACTIVE_DURING_PLAYBACK(Vehicle vehicle, BOOL toggle) {
			Invoker::Invoke<void, 4561>(vehicle, toggle);
		}

		void SET_VEHICLE_ACTIVE_DURING_PLAYBACK(Vehicle vehicle, BOOL toggle) {
			Invoker::Invoke<void, 4562>(vehicle, toggle);
		}

		BOOL IS_VEHICLE_SPRAYABLE(Vehicle vehicle) {
			return Invoker::Invoke<BOOL, 4563>(vehicle);
		}

		void SET_VEHICLE_ENGINE_CAN_DEGRADE(Vehicle vehicle, BOOL toggle) {
			Invoker::Invoke<void, 4564>(vehicle, toggle);
		}

		void DISABLE_VEHCILE_DYNAMIC_AMBIENT_SCALES(Vehicle vehicle, int p1, int p2) {
			Invoker::Invoke<void, 4565>(vehicle, p1, p2);
		}

		void ENABLE_VEHICLE_DYNAMIC_AMBIENT_SCALES(Vehicle vehicle) {
			Invoker::Invoke<void, 4566>(vehicle);
		}

		BOOL IS_PLANE_LANDING_GEAR_INTACT(Vehicle plane) {
			return Invoker::Invoke<BOOL, 4567>(plane);
		}

		BOOL ARE_PLANE_PROPELLERS_INTACT(Vehicle plane) {
			return Invoker::Invoke<BOOL, 4568>(plane);
		}

		BOOL SET_PLANE_PROPELLER_HEALTH(Vehicle plane, float health) {
			return Invoker::Invoke<BOOL, 4569>(plane, health);
		}

		void SET_VEHICLE_CAN_DEFORM_WHEELS(Vehicle vehicle, BOOL toggle) {
			Invoker::Invoke<void, 4570>(vehicle, toggle);
		}

		BOOL IS_VEHICLE_STOLEN(Vehicle vehicle) {
			return Invoker::Invoke<BOOL, 4571>(vehicle);
		}

		void SET_VEHICLE_IS_STOLEN(Vehicle vehicle, BOOL isStolen) {
			Invoker::Invoke<void, 4572>(vehicle, isStolen);
		}

		void SET_PLANE_TURBULENCE_MULTIPLIER(Vehicle vehicle, float multiplier) {
			Invoker::Invoke<void, 4573>(vehicle, multiplier);
		}

		BOOL ARE_WINGS_OF_PLANE_INTACT(Vehicle plane) {
			return Invoker::Invoke<BOOL, 4574>(plane);
		}

		void ALLOW_AMBIENT_VEHICLES_TO_AVOID_ADVERSE_CONDITIONS(Vehicle vehicle) {
			Invoker::Invoke<void, 4575>(vehicle);
		}

		void DETACH_VEHICLE_FROM_CARGOBOB(Vehicle vehicle, Vehicle cargobob) {
			Invoker::Invoke<void, 4576>(vehicle, cargobob);
		}

		BOOL DETACH_VEHICLE_FROM_ANY_CARGOBOB(Vehicle vehicle) {
			return Invoker::Invoke<BOOL, 4577>(vehicle);
		}

		BOOL DETACH_ENTITY_FROM_CARGOBOB(Vehicle cargobob, Entity entity) {
			return Invoker::Invoke<BOOL, 4578>(cargobob, entity);
		}

		BOOL IS_VEHICLE_ATTACHED_TO_CARGOBOB(Vehicle cargobob, Vehicle vehicleAttached) {
			return Invoker::Invoke<BOOL, 4579>(cargobob, vehicleAttached);
		}

		Vehicle GET_VEHICLE_ATTACHED_TO_CARGOBOB(Vehicle cargobob) {
			return Invoker::Invoke<Vehicle, 4580>(cargobob);
		}

		Entity GET_ENTITY_ATTACHED_TO_CARGOBOB(Any p0) {
			return Invoker::Invoke<Entity, 4581>(p0);
		}

		void ATTACH_VEHICLE_TO_CARGOBOB(Vehicle cargobob, Vehicle vehicle, int p2, float x, float y, float z) {
			Invoker::Invoke<void, 4582>(cargobob, vehicle, p2, x, y, z);
		}

		void ATTACH_ENTITY_TO_CARGOBOB(Any p0, Any p1, Any p2, Any p3, Any p4, Any p5) {
			Invoker::Invoke<void, 4583>(p0, p1, p2, p3, p4, p5);
		}

		void SET_CARGOBOB_FORCE_DONT_DETACH_VEHICLE(Vehicle cargobob, BOOL toggle) {
			Invoker::Invoke<void, 4584>(cargobob, toggle);
		}

		void SET_CARGOBOB_EXCLUDE_FROM_PICKUP_ENTITY(Any p0, Any p1) {
			Invoker::Invoke<void, 4585>(p0, p1);
		}

		BOOL CAN_CARGOBOB_PICK_UP_ENTITY(Any p0, Any p1) {
			return Invoker::Invoke<BOOL, 4586>(p0, p1);
		}

		Vector3 GET_ATTACHED_PICK_UP_HOOK_POSITION(Vehicle cargobob) {
			return Invoker::Invoke<Vector3, 4587>(cargobob);
		}

		BOOL DOES_CARGOBOB_HAVE_PICK_UP_ROPE(Vehicle cargobob) {
			return Invoker::Invoke<BOOL, 4588>(cargobob);
		}

		void CREATE_PICK_UP_ROPE_FOR_CARGOBOB(Vehicle cargobob, int state) {
			Invoker::Invoke<void, 4589>(cargobob, state);
		}

		void REMOVE_PICK_UP_ROPE_FOR_CARGOBOB(Vehicle cargobob) {
			Invoker::Invoke<void, 4590>(cargobob);
		}

		void SET_PICKUP_ROPE_LENGTH_FOR_CARGOBOB(Vehicle cargobob, float length1, float length2, BOOL p3) {
			Invoker::Invoke<void, 4591>(cargobob, length1, length2, p3);
		}

		void SET_PICKUP_ROPE_LENGTH_WITHOUT_CREATING_ROPE_FOR_CARGOBOB(Any p0, Any p1, Any p2) {
			Invoker::Invoke<void, 4592>(p0, p1, p2);
		}

		void SET_CARGOBOB_PICKUP_ROPE_DAMPING_MULTIPLIER(Any p0, Any p1) {
			Invoker::Invoke<void, 4593>(p0, p1);
		}

		void SET_CARGOBOB_PICKUP_ROPE_TYPE(Any p0, Any p1) {
			Invoker::Invoke<void, 4594>(p0, p1);
		}

		BOOL DOES_CARGOBOB_HAVE_PICKUP_MAGNET(Vehicle cargobob) {
			return Invoker::Invoke<BOOL, 4595>(cargobob);
		}

		void SET_CARGOBOB_PICKUP_MAGNET_ACTIVE(Vehicle cargobob, BOOL isActive) {
			Invoker::Invoke<void, 4596>(cargobob, isActive);
		}

		void SET_CARGOBOB_PICKUP_MAGNET_STRENGTH(Vehicle cargobob, float strength) {
			Invoker::Invoke<void, 4597>(cargobob, strength);
		}

		void SET_CARGOBOB_PICKUP_MAGNET_FALLOFF(Vehicle cargobob, float p1) {
			Invoker::Invoke<void, 4598>(cargobob, p1);
		}

		void SET_CARGOBOB_PICKUP_MAGNET_REDUCED_STRENGTH(Vehicle cargobob, float p1) {
			Invoker::Invoke<void, 4599>(cargobob, p1);
		}

		void SET_CARGOBOB_PICKUP_MAGNET_REDUCED_FALLOFF(Vehicle cargobob, float p1) {
			Invoker::Invoke<void, 4600>(cargobob, p1);
		}

		void SET_CARGOBOB_PICKUP_MAGNET_PULL_STRENGTH(Vehicle cargobob, float p1) {
			Invoker::Invoke<void, 4601>(cargobob, p1);
		}

		void SET_CARGOBOB_PICKUP_MAGNET_PULL_ROPE_LENGTH(Vehicle vehicle, float p1) {
			Invoker::Invoke<void, 4602>(vehicle, p1);
		}

		void SET_CARGOBOB_PICKUP_MAGNET_SET_TARGETED_MODE(Vehicle vehicle, Vehicle cargobob) {
			Invoker::Invoke<void, 4603>(vehicle, cargobob);
		}

		void SET_CARGOBOB_PICKUP_MAGNET_SET_AMBIENT_MODE(Vehicle vehicle, BOOL p1, BOOL p2) {
			Invoker::Invoke<void, 4604>(vehicle, p1, p2);
		}

		void SET_CARGOBOB_PICKUP_MAGNET_ENSURE_PICKUP_ENTITY_UPRIGHT(Vehicle vehicle, BOOL p1) {
			Invoker::Invoke<void, 4605>(vehicle, p1);
		}

		BOOL DOES_VEHICLE_HAVE_WEAPONS(Vehicle vehicle) {
			return Invoker::Invoke<BOOL, 4606>(vehicle);
		}

		void SET_VEHICLE_WILL_TELL_OTHERS_TO_HURRY(Vehicle vehicle, BOOL p1) {
			Invoker::Invoke<void, 4607>(vehicle, p1);
		}

		void DISABLE_VEHICLE_WEAPON(BOOL disabled, Hash weaponHash, Vehicle vehicle, Ped owner) {
			Invoker::Invoke<void, 4608>(disabled, weaponHash, vehicle, owner);
		}

		BOOL IS_VEHICLE_WEAPON_DISABLED(Hash weaponHash, Vehicle vehicle, Ped owner) {
			return Invoker::Invoke<BOOL, 4609>(weaponHash, vehicle, owner);
		}

		void SET_VEHICLE_USED_FOR_PILOT_SCHOOL(Vehicle vehicle, BOOL toggle) {
			Invoker::Invoke<void, 4610>(vehicle, toggle);
		}

		void SET_VEHICLE_ACTIVE_FOR_PED_NAVIGATION(Vehicle vehicle, BOOL toggle) {
			Invoker::Invoke<void, 4611>(vehicle, toggle);
		}

		int GET_VEHICLE_CLASS(Vehicle vehicle) {
			return Invoker::Invoke<int, 4612>(vehicle);
		}

		int GET_VEHICLE_CLASS_FROM_NAME(Hash modelHash) {
			return Invoker::Invoke<int, 4613>(modelHash);
		}

		void SET_PLAYERS_LAST_VEHICLE(Vehicle vehicle) {
			Invoker::Invoke<void, 4614>(vehicle);
		}

		void SET_VEHICLE_CAN_BE_USED_BY_FLEEING_PEDS(Vehicle vehicle, BOOL toggle) {
			Invoker::Invoke<void, 4615>(vehicle, toggle);
		}

		void SET_AIRCRAFT_PILOT_SKILL_NOISE_SCALAR(Vehicle vehicle, float p1) {
			Invoker::Invoke<void, 4616>(vehicle, p1);
		}

		void SET_VEHICLE_DROPS_MONEY_WHEN_BLOWN_UP(Vehicle vehicle, BOOL toggle) {
			Invoker::Invoke<void, 4617>(vehicle, toggle);
		}

		void SET_VEHICLE_KEEP_ENGINE_ON_WHEN_ABANDONED(Vehicle vehicle, BOOL toggle) {
			Invoker::Invoke<void, 4618>(vehicle, toggle);
		}

		void SET_VEHICLE_IMPATIENCE_TIMER(Vehicle vehicle, Any p1) {
			Invoker::Invoke<void, 4619>(vehicle, p1);
		}

		void SET_VEHICLE_HANDLING_OVERRIDE(Vehicle vehicle, Hash hash) {
			Invoker::Invoke<void, 4620>(vehicle, hash);
		}

		void SET_VEHICLE_EXTENDED_REMOVAL_RANGE(Vehicle vehicle, int range) {
			Invoker::Invoke<void, 4621>(vehicle, range);
		}

		void SET_VEHICLE_STEERING_BIAS_SCALAR(Any p0, float p1) {
			Invoker::Invoke<void, 4622>(p0, p1);
		}

		void SET_HELI_CONTROL_LAGGING_RATE_SCALAR(Vehicle helicopter, float multiplier) {
			Invoker::Invoke<void, 4623>(helicopter, multiplier);
		}

		void SET_VEHICLE_FRICTION_OVERRIDE(Vehicle vehicle, float friction) {
			Invoker::Invoke<void, 4624>(vehicle, friction);
		}

		void SET_VEHICLE_WHEELS_CAN_BREAK_OFF_WHEN_BLOW_UP(Vehicle vehicle, BOOL toggle) {
			Invoker::Invoke<void, 4625>(vehicle, toggle);
		}

		BOOL ARE_PLANE_CONTROL_PANELS_INTACT(Vehicle vehicle, BOOL p1) {
			return Invoker::Invoke<BOOL, 4626>(vehicle, p1);
		}

		void SET_VEHICLE_CEILING_HEIGHT(Vehicle vehicle, float height) {
			Invoker::Invoke<void, 4627>(vehicle, height);
		}

		void SET_VEHICLE_NO_EXPLOSION_DAMAGE_FROM_DRIVER(Vehicle vehicle, BOOL toggle) {
			Invoker::Invoke<void, 4628>(vehicle, toggle);
		}

		void CLEAR_VEHICLE_ROUTE_HISTORY(Vehicle vehicle) {
			Invoker::Invoke<void, 4629>(vehicle);
		}

		Vehicle DOES_VEHICLE_EXIST_WITH_DECORATOR(const char* decorator) {
			return Invoker::Invoke<Vehicle, 4630>(decorator);
		}

		void SET_VEHICLE_AI_CAN_USE_EXCLUSIVE_SEATS(Vehicle vehicle, BOOL toggle) {
			Invoker::Invoke<void, 4631>(vehicle, toggle);
		}

		void SET_VEHICLE_EXCLUSIVE_DRIVER(Vehicle vehicle, Ped ped, int index) {
			Invoker::Invoke<void, 4632>(vehicle, ped, index);
		}

		BOOL IS_PED_EXCLUSIVE_DRIVER_OF_VEHICLE(Ped ped, Vehicle vehicle, int* outIndex) {
			return Invoker::Invoke<BOOL, 4633>(ped, vehicle, outIndex);
		}

		void DISABLE_INDIVIDUAL_PLANE_PROPELLER(Vehicle vehicle, int propeller) {
			Invoker::Invoke<void, 4634>(vehicle, propeller);
		}

		void _ENABLE_INDIVIDUAL_PLANE_PROPELLER(Vehicle vehicle, int propeller) {
			Invoker::Invoke<void, 6715>(vehicle, propeller);
		}

		void SET_VEHICLE_FORCE_AFTERBURNER(Vehicle vehicle, BOOL toggle) {
			Invoker::Invoke<void, 4635>(vehicle, toggle);
		}

		void SET_DONT_PROCESS_VEHICLE_GLASS(Vehicle vehicle, BOOL toggle) {
			Invoker::Invoke<void, 4636>(vehicle, toggle);
		}

		void SET_DISABLE_WANTED_CONES_RESPONSE(Vehicle vehicle, BOOL toggle) {
			Invoker::Invoke<void, 4637>(vehicle, toggle);
		}

		void SET_USE_DESIRED_Z_CRUISE_SPEED_FOR_LANDING(Vehicle vehicle, BOOL toggle) {
			Invoker::Invoke<void, 4638>(vehicle, toggle);
		}

		void SET_ARRIVE_DISTANCE_OVERRIDE_FOR_VEHICLE_PERSUIT_ATTACK(Vehicle vehicle, float p1) {
			Invoker::Invoke<void, 4639>(vehicle, p1);
		}

		void SET_VEHICLE_READY_FOR_CLEANUP(Any p0) {
			Invoker::Invoke<void, 4640>(p0);
		}

		void SET_DISTANT_CARS_ENABLED(BOOL toggle) {
			Invoker::Invoke<void, 4641>(toggle);
		}

		void SET_VEHICLE_NEON_COLOUR(Vehicle vehicle, int r, int g, int b) {
			Invoker::Invoke<void, 4642>(vehicle, r, g, b);
		}

		void SET_VEHICLE_NEON_INDEX_COLOUR(Vehicle vehicle, int index) {
			Invoker::Invoke<void, 4643>(vehicle, index);
		}

		void GET_VEHICLE_NEON_COLOUR(Vehicle vehicle, int* r, int* g, int* b) {
			Invoker::Invoke<void, 4644>(vehicle, r, g, b);
		}

		void SET_VEHICLE_NEON_ENABLED(Vehicle vehicle, int index, BOOL toggle) {
			Invoker::Invoke<void, 4645>(vehicle, index, toggle);
		}

		BOOL GET_VEHICLE_NEON_ENABLED(Vehicle vehicle, int index) {
			return Invoker::Invoke<BOOL, 4646>(vehicle, index);
		}

		void SET_AMBIENT_VEHICLE_NEON_ENABLED(BOOL p0) {
			Invoker::Invoke<void, 4647>(p0);
		}

		void SUPPRESS_NEONS_ON_VEHICLE(Vehicle vehicle, BOOL toggle) {
			Invoker::Invoke<void, 4648>(vehicle, toggle);
		}

		void SET_DISABLE_SUPERDUMMY(Vehicle vehicle, BOOL p1) {
			Invoker::Invoke<void, 4649>(vehicle, p1);
		}

		void REQUEST_VEHICLE_DIAL(Vehicle vehicle) {
			Invoker::Invoke<void, 4650>(vehicle);
		}

		float GET_VEHICLE_BODY_HEALTH(Vehicle vehicle) {
			return Invoker::Invoke<float, 4651>(vehicle);
		}

		void SET_VEHICLE_BODY_HEALTH(Vehicle vehicle, float value) {
			Invoker::Invoke<void, 4652>(vehicle, value);
		}

		void GET_VEHICLE_SIZE(Vehicle vehicle, Vector3* out1, Vector3* out2) {
			Invoker::Invoke<void, 4653>(vehicle, out1, out2);
		}

		float GET_FAKE_SUSPENSION_LOWERING_AMOUNT(Vehicle vehicle) {
			return Invoker::Invoke<float, 4654>(vehicle);
		}

		void SET_CAR_HIGH_SPEED_BUMP_SEVERITY_MULTIPLIER(float multiplier) {
			Invoker::Invoke<void, 4655>(multiplier);
		}

		int GET_NUMBER_OF_VEHICLE_DOORS(Vehicle vehicle) {
			return Invoker::Invoke<int, 4656>(vehicle);
		}

		void SET_HYDRAULICS_CONTROL(Vehicle vehicle, BOOL toggle) {
			Invoker::Invoke<void, 4657>(vehicle, toggle);
		}

		void SET_CAN_ADJUST_GROUND_CLEARANCE(Vehicle vehicle, BOOL p1) {
			Invoker::Invoke<void, 4658>(vehicle, p1);
		}

		float GET_VEHICLE_HEALTH_PERCENTAGE(Vehicle vehicle, float maxEngineHealth, float maxPetrolTankHealth, float maxBodyHealth, float maxMainRotorHealth, float maxTailRotorHealth, float maxUnkHealth) {
			return Invoker::Invoke<float, 4659>(vehicle, maxEngineHealth, maxPetrolTankHealth, maxBodyHealth, maxMainRotorHealth, maxTailRotorHealth, maxUnkHealth);
		}

		BOOL GET_VEHICLE_IS_MERCENARY(Vehicle vehicle) {
			return Invoker::Invoke<BOOL, 4660>(vehicle);
		}

		void SET_VEHICLE_BROKEN_PARTS_DONT_AFFECT_AI_HANDLING(Vehicle vehicle, BOOL p1) {
			Invoker::Invoke<void, 4661>(vehicle, p1);
		}

		void SET_VEHICLE_KERS_ALLOWED(Vehicle vehicle, BOOL toggle) {
			Invoker::Invoke<void, 4662>(vehicle, toggle);
		}

		BOOL GET_VEHICLE_HAS_KERS(Vehicle vehicle) {
			return Invoker::Invoke<BOOL, 4663>(vehicle);
		}

		void SET_PLANE_RESIST_TO_EXPLOSION(Vehicle vehicle, BOOL toggle) {
			Invoker::Invoke<void, 4664>(vehicle, toggle);
		}

		void SET_HELI_RESIST_TO_EXPLOSION(Vehicle vehicle, BOOL toggle) {
			Invoker::Invoke<void, 4665>(vehicle, toggle);
		}

		void SET_DISABLE_BMX_EXTRA_TRICK_FORCES(Any p0) {
			Invoker::Invoke<void, 4666>(p0);
		}

		void SET_HYDRAULIC_SUSPENSION_RAISE_FACTOR(Vehicle vehicle, int wheelId, float value) {
			Invoker::Invoke<void, 4667>(vehicle, wheelId, value);
		}

		float GET_HYDRAULIC_SUSPENSION_RAISE_FACTOR(Vehicle vehicle, int wheelId) {
			return Invoker::Invoke<float, 4668>(vehicle, wheelId);
		}

		void SET_CAN_USE_HYDRAULICS(Vehicle vehicle, BOOL toggle) {
			Invoker::Invoke<void, 4669>(vehicle, toggle);
		}

		void SET_HYDRAULIC_VEHICLE_STATE(Vehicle vehicle, int state) {
			Invoker::Invoke<void, 4670>(vehicle, state);
		}

		void SET_HYDRAULIC_WHEEL_STATE(Vehicle vehicle, int wheelId, int state, float value, Any p4) {
			Invoker::Invoke<void, 4671>(vehicle, wheelId, state, value, p4);
		}

		BOOL HAS_VEHICLE_PETROLTANK_SET_ON_FIRE_BY_ENTITY(Any p0, Any p1) {
			return Invoker::Invoke<BOOL, 4672>(p0, p1);
		}

		void CLEAR_VEHICLE_PETROLTANK_FIRE_CULPRIT(Vehicle vehicle) {
			Invoker::Invoke<void, 4673>(vehicle);
		}

		void SET_VEHICLE_BOBBLEHEAD_VELOCITY(float x, float y, float p2) {
			Invoker::Invoke<void, 4674>(x, y, p2);
		}

		BOOL GET_VEHICLE_IS_DUMMY(Any p0) {
			return Invoker::Invoke<BOOL, 4675>(p0);
		}

		BOOL SET_VEHICLE_DAMAGE_SCALE(Vehicle vehicle, float p1) {
			return Invoker::Invoke<BOOL, 4676>(vehicle, p1);
		}

		Any _SET_VEHICLE_EXPLOSIVE_DAMAGE_SCALE(Vehicle vehicle, float scale) {
			return Invoker::Invoke<Any, 6716>(vehicle, scale);
		}

		BOOL SET_VEHICLE_WEAPON_DAMAGE_SCALE(Vehicle vehicle, float multiplier) {
			return Invoker::Invoke<BOOL, 4677>(vehicle, multiplier);
		}

		BOOL SET_DISABLE_DAMAGE_WITH_PICKED_UP_ENTITY(Any p0, Any p1) {
			return Invoker::Invoke<BOOL, 4678>(p0, p1);
		}

		void SET_VEHICLE_USES_MP_PLAYER_DAMAGE_MULTIPLIER(Any p0, Any p1) {
			Invoker::Invoke<void, 4679>(p0, p1);
		}

		void SET_BIKE_EASY_TO_LAND(Vehicle vehicle, BOOL toggle) {
			Invoker::Invoke<void, 4680>(vehicle, toggle);
		}

		void SET_INVERT_VEHICLE_CONTROLS(Vehicle vehicle, BOOL state) {
			Invoker::Invoke<void, 4681>(vehicle, state);
		}

		void SET_SPEED_BOOST_EFFECT_DISABLED(BOOL disabled) {
			Invoker::Invoke<void, 4682>(disabled);
		}

		void SET_SLOW_DOWN_EFFECT_DISABLED(BOOL disabled) {
			Invoker::Invoke<void, 4683>(disabled);
		}

		void SET_FORMATION_LEADER(Vehicle vehicle, float x, float y, float z, float p4) {
			Invoker::Invoke<void, 4684>(vehicle, x, y, z, p4);
		}

		void RESET_FORMATION_LEADER() {
			Invoker::Invoke<void, 4685>();
		}

		BOOL GET_IS_BOAT_CAPSIZED(Vehicle vehicle) {
			return Invoker::Invoke<BOOL, 4686>(vehicle);
		}

		void SET_ALLOW_RAMMING_SOOP_OR_RAMP(Any p0, Any p1) {
			Invoker::Invoke<void, 4687>(p0, p1);
		}

		void SET_SCRIPT_RAMP_IMPULSE_SCALE(Vehicle vehicle, float impulseScale) {
			Invoker::Invoke<void, 4688>(vehicle, impulseScale);
		}

		BOOL GET_IS_DOOR_VALID(Vehicle vehicle, int doorId) {
			return Invoker::Invoke<BOOL, 4689>(vehicle, doorId);
		}

		void SET_SCRIPT_ROCKET_BOOST_RECHARGE_TIME(Vehicle vehicle, float seconds) {
			Invoker::Invoke<void, 4690>(vehicle, seconds);
		}

		BOOL GET_HAS_ROCKET_BOOST(Vehicle vehicle) {
			return Invoker::Invoke<BOOL, 4691>(vehicle);
		}

		BOOL IS_ROCKET_BOOST_ACTIVE(Vehicle vehicle) {
			return Invoker::Invoke<BOOL, 4692>(vehicle);
		}

		void SET_ROCKET_BOOST_ACTIVE(Vehicle vehicle, BOOL active) {
			Invoker::Invoke<void, 4693>(vehicle, active);
		}

		BOOL GET_HAS_RETRACTABLE_WHEELS(Vehicle vehicle) {
			return Invoker::Invoke<BOOL, 4694>(vehicle);
		}

		BOOL GET_IS_WHEELS_RETRACTED(Vehicle vehicle) {
			return Invoker::Invoke<BOOL, 4695>(vehicle);
		}

		void SET_WHEELS_EXTENDED_INSTANTLY(Vehicle vehicle) {
			Invoker::Invoke<void, 4696>(vehicle);
		}

		void SET_WHEELS_RETRACTED_INSTANTLY(Vehicle vehicle) {
			Invoker::Invoke<void, 4697>(vehicle);
		}

		BOOL GET_CAR_HAS_JUMP(Vehicle vehicle) {
			return Invoker::Invoke<BOOL, 4698>(vehicle);
		}

		void SET_USE_HIGHER_CAR_JUMP(Vehicle vehicle, BOOL toggle) {
			Invoker::Invoke<void, 4699>(vehicle, toggle);
		}

		void SET_CLEAR_FREEZE_WAITING_ON_COLLISION_ONCE_PLAYER_ENTERS(Vehicle vehicle, BOOL toggle) {
			Invoker::Invoke<void, 4700>(vehicle, toggle);
		}

		void SET_VEHICLE_WEAPON_RESTRICTED_AMMO(Vehicle vehicle, int weaponIndex, int capacity) {
			Invoker::Invoke<void, 4701>(vehicle, weaponIndex, capacity);
		}

		int GET_VEHICLE_WEAPON_RESTRICTED_AMMO(Vehicle vehicle, int weaponIndex) {
			return Invoker::Invoke<int, 4702>(vehicle, weaponIndex);
		}

		BOOL GET_VEHICLE_HAS_PARACHUTE(Vehicle vehicle) {
			return Invoker::Invoke<BOOL, 4703>(vehicle);
		}

		BOOL GET_VEHICLE_CAN_DEPLOY_PARACHUTE(Vehicle vehicle) {
			return Invoker::Invoke<BOOL, 4704>(vehicle);
		}

		void VEHICLE_START_PARACHUTING(Vehicle vehicle, BOOL active) {
			Invoker::Invoke<void, 4705>(vehicle, active);
		}

		BOOL IS_VEHICLE_PARACHUTE_DEPLOYED(Vehicle vehicle) {
			return Invoker::Invoke<BOOL, 4706>(vehicle);
		}

		void VEHICLE_SET_RAMP_AND_RAMMING_CARS_TAKE_DAMAGE(Vehicle vehicle, BOOL toggle) {
			Invoker::Invoke<void, 4707>(vehicle, toggle);
		}

		void VEHICLE_SET_ENABLE_RAMP_CAR_SIDE_IMPULSE(Any p0, Any p1) {
			Invoker::Invoke<void, 4708>(p0, p1);
		}

		void VEHICLE_SET_ENABLE_NORMALISE_RAMP_CAR_VERTICAL_VELOCTIY(Any p0, Any p1) {
			Invoker::Invoke<void, 4709>(p0, p1);
		}

		void VEHICLE_SET_JET_WASH_FORCE_ENABLED(Any p0) {
			Invoker::Invoke<void, 4710>(p0);
		}

		void SET_VEHICLE_WEAPON_CAN_TARGET_OBJECTS(Vehicle vehicle, BOOL toggle) {
			Invoker::Invoke<void, 4711>(vehicle, toggle);
		}

		void SET_VEHICLE_USE_BOOST_BUTTON_FOR_WHEEL_RETRACT(BOOL toggle) {
			Invoker::Invoke<void, 4712>(toggle);
		}

		void _SET_VEHICLE_USE_HORN_BUTTON_FOR_NITROUS(BOOL toggle) {
			Invoker::Invoke<void, 4713>(toggle);
		}

		void VEHICLE_SET_PARACHUTE_MODEL_OVERRIDE(Vehicle vehicle, Hash modelHash) {
			Invoker::Invoke<void, 4714>(vehicle, modelHash);
		}

		void VEHICLE_SET_PARACHUTE_MODEL_TINT_INDEX(Vehicle vehicle, int textureVariation) {
			Invoker::Invoke<void, 4715>(vehicle, textureVariation);
		}

		int VEHICLE_SET_OVERRIDE_EXTENABLE_SIDE_RATIO(Any p0, Any p1) {
			return Invoker::Invoke<int, 4716>(p0, p1);
		}

		int VEHICLE_SET_EXTENABLE_SIDE_TARGET_RATIO(Any p0, Any p1) {
			return Invoker::Invoke<int, 4717>(p0, p1);
		}

		int VEHICLE_SET_OVERRIDE_SIDE_RATIO(Any p0, Any p1) {
			return Invoker::Invoke<int, 4718>(p0, p1);
		}

		int GET_ALL_VEHICLES(Any* vehsStruct) {
			return Invoker::Invoke<int, 4719>(vehsStruct);
		}

		void SET_CARGOBOB_EXTA_PICKUP_RANGE(Any p0, Any p1) {
			Invoker::Invoke<void, 4720>(p0, p1);
		}

		void SET_OVERRIDE_VEHICLE_DOOR_TORQUE(Any p0, Any p1, Any p2) {
			Invoker::Invoke<void, 4721>(p0, p1, p2);
		}

		void SET_WHEELIE_ENABLED(Vehicle vehicle, BOOL enabled) {
			Invoker::Invoke<void, 4722>(vehicle, enabled);
		}

		void SET_DISABLE_HELI_EXPLODE_FROM_BODY_DAMAGE(Vehicle vehicle, BOOL disable) {
			Invoker::Invoke<void, 4723>(vehicle, disable);
		}

		void SET_DISABLE_EXPLODE_FROM_BODY_DAMAGE_ON_COLLISION(Vehicle vehicle, BOOL disable) {
			Invoker::Invoke<void, 4724>(vehicle, disable);
		}

		void _SET_DISABLE_EXPLODE_FROM_BODY_DAMAGE_RECEIVED_BY_AI_VEHICLE(Vehicle vehicle, BOOL disable) {
			Invoker::Invoke<void, 6717>(vehicle, disable);
		}

		void SET_TRAILER_ATTACHMENT_ENABLED(Any p0, Any p1) {
			Invoker::Invoke<void, 4725>(p0, p1);
		}

		void SET_ROCKET_BOOST_FILL(Vehicle vehicle, float percentage) {
			Invoker::Invoke<void, 4726>(vehicle, percentage);
		}

		void SET_GLIDER_ACTIVE(Vehicle vehicle, BOOL state) {
			Invoker::Invoke<void, 4727>(vehicle, state);
		}

		void SET_SHOULD_RESET_TURRET_IN_SCRIPTED_CAMERAS(Vehicle vehicle, BOOL shouldReset) {
			Invoker::Invoke<void, 4728>(vehicle, shouldReset);
		}

		void SET_VEHICLE_DISABLE_COLLISION_UPON_CREATION(Vehicle vehicle, BOOL disable) {
			Invoker::Invoke<void, 4729>(vehicle, disable);
		}

		void SET_GROUND_EFFECT_REDUCES_DRAG(BOOL toggle) {
			Invoker::Invoke<void, 4730>(toggle);
		}

		void SET_DISABLE_MAP_COLLISION(Vehicle vehicle) {
			Invoker::Invoke<void, 4731>(vehicle);
		}

		void SET_DISABLE_PED_STAND_ON_TOP(Vehicle vehicle, BOOL toggle) {
			Invoker::Invoke<void, 4732>(vehicle, toggle);
		}

		void SET_VEHICLE_DAMAGE_SCALES(Vehicle vehicle, Any p1, Any p2, Any p3, Any p4) {
			Invoker::Invoke<void, 4733>(vehicle, p1, p2, p3, p4);
		}

		void SET_PLANE_SECTION_DAMAGE_SCALE(Vehicle vehicle, Any p1, Any p2) {
			Invoker::Invoke<void, 4734>(vehicle, p1, p2);
		}

		void SET_PLANE_CONTROL_SECTIONS_SHOULD_BREAK_OFF_FROM_EXPLOSIONS(Vehicle vehicle, BOOL toggle) {
			Invoker::Invoke<void, 6718>(vehicle, toggle);
		}

		void SET_HELI_CAN_PICKUP_ENTITY_THAT_HAS_PICK_UP_DISABLED(Vehicle vehicle, BOOL toggle) {
			Invoker::Invoke<void, 4735>(vehicle, toggle);
		}

		void SET_VEHICLE_BOMB_AMMO(Vehicle vehicle, int bombCount) {
			Invoker::Invoke<void, 4736>(vehicle, bombCount);
		}

		int GET_VEHICLE_BOMB_AMMO(Vehicle vehicle) {
			return Invoker::Invoke<int, 4737>(vehicle);
		}

		void SET_VEHICLE_COUNTERMEASURE_AMMO(Vehicle vehicle, int counterMeasureCount) {
			Invoker::Invoke<void, 4738>(vehicle, counterMeasureCount);
		}

		int GET_VEHICLE_COUNTERMEASURE_AMMO(Vehicle vehicle) {
			return Invoker::Invoke<int, 4739>(vehicle);
		}

		void SET_HELI_COMBAT_OFFSET(Vehicle vehicle, float x, float y, float z) {
			Invoker::Invoke<void, 4740>(vehicle, x, y, z);
		}

		BOOL GET_CAN_VEHICLE_BE_PLACED_HERE(Vehicle vehicle, float x, float y, float z, float rotX, float rotY, float rotZ, int p7, Any p8) {
			return Invoker::Invoke<BOOL, 4741>(vehicle, x, y, z, rotX, rotY, rotZ, p7, p8);
		}

		void SET_DISABLE_AUTOMATIC_CRASH_TASK(Vehicle vehicle, BOOL toggle) {
			Invoker::Invoke<void, 4742>(vehicle, toggle);
		}

		void SET_SPECIAL_FLIGHT_MODE_RATIO(Vehicle vehicle, float ratio) {
			Invoker::Invoke<void, 4743>(vehicle, ratio);
		}

		void SET_SPECIAL_FLIGHT_MODE_TARGET_RATIO(Vehicle vehicle, float targetRatio) {
			Invoker::Invoke<void, 4744>(vehicle, targetRatio);
		}

		void SET_SPECIAL_FLIGHT_MODE_ALLOWED(Vehicle vehicle, BOOL toggle) {
			Invoker::Invoke<void, 4745>(vehicle, toggle);
		}

		void SET_DISABLE_HOVER_MODE_FLIGHT(Vehicle vehicle, BOOL toggle) {
			Invoker::Invoke<void, 4746>(vehicle, toggle);
		}

		BOOL GET_OUTRIGGERS_DEPLOYED(Vehicle vehicle) {
			return Invoker::Invoke<BOOL, 4747>(vehicle);
		}

		Vector3 FIND_SPAWN_COORDINATES_FOR_HELI(Ped ped) {
			return Invoker::Invoke<Vector3, 4748>(ped);
		}

		void SET_DEPLOY_FOLDING_WINGS(Vehicle vehicle, BOOL deploy, BOOL p2) {
			Invoker::Invoke<void, 4749>(vehicle, deploy, p2);
		}

		BOOL ARE_FOLDING_WINGS_DEPLOYED(Vehicle vehicle) {
			return Invoker::Invoke<BOOL, 4750>(vehicle);
		}

		void _SET_DEPLOY_MISSILE_BAYS(Vehicle vehicle, BOOL deploy) {
			Invoker::Invoke<void, 4751>(vehicle, deploy);
		}

		BOOL _ARE_MISSILE_BAYS_DEPLOYED(Vehicle vehicle) {
			return Invoker::Invoke<BOOL, 4752>(vehicle);
		}

		void SET_DIP_STRAIGHT_DOWN_WHEN_CRASHING_PLANE(Vehicle vehicle, BOOL toggle) {
			Invoker::Invoke<void, 4753>(vehicle, toggle);
		}

		void SET_TURRET_HIDDEN(Vehicle vehicle, int index, BOOL toggle) {
			Invoker::Invoke<void, 4754>(vehicle, index, toggle);
		}

		void SET_HOVER_MODE_WING_RATIO(Vehicle vehicle, float ratio) {
			Invoker::Invoke<void, 4755>(vehicle, ratio);
		}

		void SET_DISABLE_TURRET_MOVEMENT(Vehicle vehicle, int turretId) {
			Invoker::Invoke<void, 4756>(vehicle, turretId);
		}

		void SET_FORCE_FIX_LINK_MATRICES(Vehicle vehicle) {
			Invoker::Invoke<void, 4757>(vehicle);
		}

		void SET_TRANSFORM_RATE_FOR_ANIMATION(Vehicle vehicle, float transformRate) {
			Invoker::Invoke<void, 4758>(vehicle, transformRate);
		}

		void SET_TRANSFORM_TO_SUBMARINE_USES_ALTERNATE_INPUT(Vehicle vehicle, BOOL toggle) {
			Invoker::Invoke<void, 4759>(vehicle, toggle);
		}

		void SET_VEHICLE_COMBAT_MODE(BOOL toggle) {
			Invoker::Invoke<void, 4760>(toggle);
		}

		void SET_VEHICLE_DETONATION_MODE(BOOL toggle) {
			Invoker::Invoke<void, 4761>(toggle);
		}

		void SET_VEHICLE_SHUNT_ON_STICK(BOOL toggle) {
			Invoker::Invoke<void, 4762>(toggle);
		}

		BOOL _IS_VEHICLE_ON_BOOST_PAD(Vehicle vehicle) {
			return Invoker::Invoke<BOOL, 6719>(vehicle);
		}

		BOOL GET_IS_VEHICLE_SHUNTING(Vehicle vehicle) {
			return Invoker::Invoke<BOOL, 4763>(vehicle);
		}

		BOOL GET_HAS_VEHICLE_BEEN_HIT_BY_SHUNT(Vehicle vehicle) {
			return Invoker::Invoke<BOOL, 4764>(vehicle);
		}

		Vehicle GET_LAST_SHUNT_VEHICLE(Vehicle vehicle) {
			return Invoker::Invoke<Vehicle, 4765>(vehicle);
		}

		void SET_DISABLE_VEHICLE_EXPLOSIONS_DAMAGE(BOOL toggle) {
			Invoker::Invoke<void, 4766>(toggle);
		}

		void SET_OVERRIDE_NITROUS_LEVEL(Vehicle vehicle, BOOL toggle, float level, float power, float rechargeTime, BOOL disableSound) {
			Invoker::Invoke<void, 4767>(vehicle, toggle, level, power, rechargeTime, disableSound);
		}

		void _SET_NITROUS_IS_VISIBLE(Vehicle vehicle, BOOL enabled) {
			Invoker::Invoke<void, 4768>(vehicle, enabled);
		}

		void _SET_OVERRIDE_TRACTION_LOSS_MULTIPLIER(Vehicle vehicle, float modifier) {
			Invoker::Invoke<void, 4769>(vehicle, modifier);
		}

		void _SET_DRIFT_SLIP_ANGLE_LIMITS(Vehicle vehicle, float durationScalar, float amplitudeScalar, float slipAngleLimit) {
			Invoker::Invoke<void, 4770>(vehicle, durationScalar, amplitudeScalar, slipAngleLimit);
		}

		void _SET_MINIMUM_TIME_BETWEEN_GEAR_SHIFTS(Vehicle vehicle, int time) {
			Invoker::Invoke<void, 4771>(vehicle, time);
		}

		void FULLY_CHARGE_NITROUS(Vehicle vehicle) {
			Invoker::Invoke<void, 4772>(vehicle);
		}

		void _SET_REMAINING_NITROUS_DURATION(Vehicle vehicle, float duration) {
			Invoker::Invoke<void, 6720>(vehicle, duration);
		}

		float _GET_REMAINING_NITROUS_DURATION(Vehicle vehicle) {
			return Invoker::Invoke<float, 4773>(vehicle);
		}

		BOOL IS_NITROUS_ACTIVE(Vehicle vehicle) {
			return Invoker::Invoke<BOOL, 4774>(vehicle);
		}

		void CLEAR_NITROUS(Vehicle vehicle) {
			Invoker::Invoke<void, 4775>(vehicle);
		}

		void SET_NITROUS_IS_ACTIVE(Vehicle vehicle, BOOL toggle) {
			Invoker::Invoke<void, 6721>(vehicle, toggle);
		}

		void SET_INCREASE_WHEEL_CRUSH_DAMAGE(Vehicle vehicle, BOOL toggle) {
			Invoker::Invoke<void, 4776>(vehicle, toggle);
		}

		void SET_DISABLE_WEAPON_BLADE_FORCES(BOOL toggle) {
			Invoker::Invoke<void, 4777>(toggle);
		}

		void SET_USE_DOUBLE_CLICK_FOR_CAR_JUMP(BOOL toggle) {
			Invoker::Invoke<void, 4778>(toggle);
		}

		BOOL GET_DOES_VEHICLE_HAVE_TOMBSTONE(Vehicle vehicle) {
			return Invoker::Invoke<BOOL, 4779>(vehicle);
		}

		void HIDE_TOMBSTONE(Vehicle vehicle, BOOL toggle) {
			Invoker::Invoke<void, 4780>(vehicle, toggle);
		}

		void APPLY_EMP_EFFECT(Vehicle vehicle) {
			Invoker::Invoke<void, 4781>(vehicle);
		}

		BOOL GET_IS_VEHICLE_DISABLED_BY_EMP(Vehicle vehicle) {
			return Invoker::Invoke<BOOL, 4782>(vehicle);
		}

		void SET_DISABLE_RETRACTING_WEAPON_BLADES(BOOL toggle) {
			Invoker::Invoke<void, 4783>(toggle);
		}

		float GET_TYRE_HEALTH(Vehicle vehicle, int wheelIndex) {
			return Invoker::Invoke<float, 4784>(vehicle, wheelIndex);
		}

		void SET_TYRE_HEALTH(Vehicle vehicle, int wheelIndex, float health) {
			Invoker::Invoke<void, 4785>(vehicle, wheelIndex, health);
		}

		float GET_TYRE_WEAR_RATE(Vehicle vehicle, int wheelIndex) {
			return Invoker::Invoke<float, 4786>(vehicle, wheelIndex);
		}

		void SET_TYRE_WEAR_RATE(Vehicle vehicle, int wheelIndex, float multiplier) {
			Invoker::Invoke<void, 4787>(vehicle, wheelIndex, multiplier);
		}

		void SET_TYRE_WEAR_RATE_SCALE(Vehicle vehicle, int wheelIndex, float multiplier) {
			Invoker::Invoke<void, 4788>(vehicle, wheelIndex, multiplier);
		}

		void SET_TYRE_MAXIMUM_GRIP_DIFFERENCE_DUE_TO_WEAR_RATE(Vehicle vehicle, int wheelIndex, float multiplier) {
			Invoker::Invoke<void, 4789>(vehicle, wheelIndex, multiplier);
		}

		void SET_AIRCRAFT_IGNORE_HIGHTMAP_OPTIMISATION(Vehicle vehicle, int p1) {
			Invoker::Invoke<void, 4790>(vehicle, p1);
		}

		void SET_REDUCED_SUSPENSION_FORCE(Vehicle vehicle, BOOL enable) {
			Invoker::Invoke<void, 4791>(vehicle, enable);
		}

		void SET_DRIFT_TYRES(Vehicle vehicle, BOOL toggle) {
			Invoker::Invoke<void, 4792>(vehicle, toggle);
		}

		BOOL GET_DRIFT_TYRES_SET(Vehicle vehicle) {
			return Invoker::Invoke<BOOL, 4793>(vehicle);
		}

		void NETWORK_USE_HIGH_PRECISION_TRAIN_BLENDING(Vehicle vehicle, BOOL toggle) {
			Invoker::Invoke<void, 4794>(vehicle, toggle);
		}

		void SET_CHECK_FOR_ENOUGH_ROOM_FOR_PED(Vehicle vehicle, BOOL p1) {
			Invoker::Invoke<void, 4795>(vehicle, p1);
		}

		void _SET_ALLOW_COLLISION_WHEN_IN_VEHICLE(Vehicle vehicle, BOOL toggle) {
			Invoker::Invoke<void, 4796>(vehicle, toggle);
		}

		int _GET_VEHICLE_MAX_EXHAUST_BONE_COUNT() {
			return Invoker::Invoke<int, 4798>();
		}

		BOOL _GET_VEHICLE_EXHAUST_BONE(Vehicle vehicle, int index, int* boneIndex, BOOL* axisX) {
			return Invoker::Invoke<BOOL, 4799>(vehicle, index, boneIndex, axisX);
		}

	} // namespace VEHICLE

	namespace WATER {

		BOOL GET_WATER_HEIGHT(float x, float y, float z, float* height) {
			return Invoker::Invoke<BOOL, 4821>(x, y, z, height);
		}

		BOOL GET_WATER_HEIGHT_NO_WAVES(float x, float y, float z, float* height) {
			return Invoker::Invoke<BOOL, 4822>(x, y, z, height);
		}

		BOOL TEST_PROBE_AGAINST_WATER(float x1, float y1, float z1, float x2, float y2, float z2, Vector3* result) {
			return Invoker::Invoke<BOOL, 4823>(x1, y1, z1, x2, y2, z2, result);
		}

		int TEST_PROBE_AGAINST_ALL_WATER(float x1, float y1, float z1, float x2, float y2, float z2, int flags, float* waterHeight) {
			return Invoker::Invoke<int, 4824>(x1, y1, z1, x2, y2, z2, flags, waterHeight);
		}

		int TEST_VERTICAL_PROBE_AGAINST_ALL_WATER(float x, float y, float z, int flags, float* waterHeight) {
			return Invoker::Invoke<int, 4825>(x, y, z, flags, waterHeight);
		}

		void MODIFY_WATER(float x, float y, float radius, float height) {
			Invoker::Invoke<void, 4826>(x, y, radius, height);
		}

		int ADD_EXTRA_CALMING_QUAD(float xLow, float yLow, float xHigh, float yHigh, float height) {
			return Invoker::Invoke<int, 4827>(xLow, yLow, xHigh, yHigh, height);
		}

		void REMOVE_EXTRA_CALMING_QUAD(int calmingQuad) {
			Invoker::Invoke<void, 4828>(calmingQuad);
		}

		void SET_DEEP_OCEAN_SCALER(float intensity) {
			Invoker::Invoke<void, 4829>(intensity);
		}

		float GET_DEEP_OCEAN_SCALER() {
			return Invoker::Invoke<float, 4830>();
		}

		void SET_CALMED_WAVE_HEIGHT_SCALER(float height) {
			Invoker::Invoke<void, 4831>(height);
		}

		void RESET_DEEP_OCEAN_SCALER() {
			Invoker::Invoke<void, 4832>();
		}

	} // namespace WATER

	namespace WEAPON {

		void ENABLE_LASER_SIGHT_RENDERING(BOOL toggle) {
			Invoker::Invoke<void, 283>(toggle);
		}

		Hash GET_WEAPON_COMPONENT_TYPE_MODEL(Hash componentHash) {
			return Invoker::Invoke<Hash, 284>(componentHash);
		}

		Hash GET_WEAPONTYPE_MODEL(Hash weaponHash) {
			return Invoker::Invoke<Hash, 285>(weaponHash);
		}

		Hash GET_WEAPONTYPE_SLOT(Hash weaponHash) {
			return Invoker::Invoke<Hash, 286>(weaponHash);
		}

		Hash GET_WEAPONTYPE_GROUP(Hash weaponHash) {
			return Invoker::Invoke<Hash, 287>(weaponHash);
		}

		int GET_WEAPON_COMPONENT_VARIANT_EXTRA_COUNT(Hash componentHash) {
			return Invoker::Invoke<int, 288>(componentHash);
		}

		Hash GET_WEAPON_COMPONENT_VARIANT_EXTRA_MODEL(Hash componentHash, int extraComponentIndex) {
			return Invoker::Invoke<Hash, 289>(componentHash, extraComponentIndex);
		}

		void SET_CURRENT_PED_WEAPON(Ped ped, Hash weaponHash, BOOL bForceInHand) {
			Invoker::Invoke<void, 290>(ped, weaponHash, bForceInHand);
		}

		BOOL GET_CURRENT_PED_WEAPON(Ped ped, Hash* weaponHash, BOOL p2) {
			return Invoker::Invoke<BOOL, 291>(ped, weaponHash, p2);
		}

		Entity GET_CURRENT_PED_WEAPON_ENTITY_INDEX(Ped ped, Any p1) {
			return Invoker::Invoke<Entity, 292>(ped, p1);
		}

		Hash GET_BEST_PED_WEAPON(Ped ped, BOOL p1) {
			return Invoker::Invoke<Hash, 293>(ped, p1);
		}

		BOOL SET_CURRENT_PED_VEHICLE_WEAPON(Ped ped, Hash weaponHash) {
			return Invoker::Invoke<BOOL, 294>(ped, weaponHash);
		}

		BOOL GET_CURRENT_PED_VEHICLE_WEAPON(Ped ped, Hash* weaponHash) {
			return Invoker::Invoke<BOOL, 295>(ped, weaponHash);
		}

		void SET_PED_CYCLE_VEHICLE_WEAPONS_ONLY(Ped ped) {
			Invoker::Invoke<void, 296>(ped);
		}

		BOOL IS_PED_ARMED(Ped ped, int typeFlags) {
			return Invoker::Invoke<BOOL, 297>(ped, typeFlags);
		}

		BOOL IS_WEAPON_VALID(Hash weaponHash) {
			return Invoker::Invoke<BOOL, 298>(weaponHash);
		}

		BOOL HAS_PED_GOT_WEAPON(Ped ped, Hash weaponHash, BOOL p2) {
			return Invoker::Invoke<BOOL, 299>(ped, weaponHash, p2);
		}

		BOOL IS_PED_WEAPON_READY_TO_SHOOT(Ped ped) {
			return Invoker::Invoke<BOOL, 300>(ped);
		}

		Hash GET_PED_WEAPONTYPE_IN_SLOT(Ped ped, Hash weaponSlot) {
			return Invoker::Invoke<Hash, 301>(ped, weaponSlot);
		}

		int GET_AMMO_IN_PED_WEAPON(Ped ped, Hash weaponhash) {
			return Invoker::Invoke<int, 302>(ped, weaponhash);
		}

		void ADD_AMMO_TO_PED(Ped ped, Hash weaponHash, int ammo) {
			Invoker::Invoke<void, 303>(ped, weaponHash, ammo);
		}

		void SET_PED_AMMO(Ped ped, Hash weaponHash, int ammo, BOOL p3) {
			Invoker::Invoke<void, 304>(ped, weaponHash, ammo, p3);
		}

		void SET_PED_INFINITE_AMMO(Ped ped, BOOL toggle, Hash weaponHash) {
			Invoker::Invoke<void, 305>(ped, toggle, weaponHash);
		}

		void SET_PED_INFINITE_AMMO_CLIP(Ped ped, BOOL toggle) {
			Invoker::Invoke<void, 306>(ped, toggle);
		}

		void SET_PED_STUN_GUN_FINITE_AMMO(Any p0, Any p1) {
			Invoker::Invoke<void, 307>(p0, p1);
		}

		void GIVE_WEAPON_TO_PED(Ped ped, Hash weaponHash, int ammoCount, BOOL isHidden, BOOL bForceInHand) {
			Invoker::Invoke<void, 308>(ped, weaponHash, ammoCount, isHidden, bForceInHand);
		}

		void GIVE_DELAYED_WEAPON_TO_PED(Ped ped, Hash weaponHash, int ammoCount, BOOL bForceInHand) {
			Invoker::Invoke<void, 309>(ped, weaponHash, ammoCount, bForceInHand);
		}

		void REMOVE_ALL_PED_WEAPONS(Ped ped, BOOL p1) {
			Invoker::Invoke<void, 310>(ped, p1);
		}

		void REMOVE_WEAPON_FROM_PED(Ped ped, Hash weaponHash) {
			Invoker::Invoke<void, 311>(ped, weaponHash);
		}

		void HIDE_PED_WEAPON_FOR_SCRIPTED_CUTSCENE(Ped ped, BOOL toggle) {
			Invoker::Invoke<void, 312>(ped, toggle);
		}

		void SET_PED_CURRENT_WEAPON_VISIBLE(Ped ped, BOOL visible, BOOL deselectWeapon, BOOL p3, BOOL p4) {
			Invoker::Invoke<void, 313>(ped, visible, deselectWeapon, p3, p4);
		}

		void SET_PED_DROPS_WEAPONS_WHEN_DEAD(Ped ped, BOOL toggle) {
			Invoker::Invoke<void, 314>(ped, toggle);
		}

		BOOL HAS_PED_BEEN_DAMAGED_BY_WEAPON(Ped ped, Hash weaponHash, int weaponType) {
			return Invoker::Invoke<BOOL, 315>(ped, weaponHash, weaponType);
		}

		void CLEAR_PED_LAST_WEAPON_DAMAGE(Ped ped) {
			Invoker::Invoke<void, 316>(ped);
		}

		BOOL HAS_ENTITY_BEEN_DAMAGED_BY_WEAPON(Entity entity, Hash weaponHash, int weaponType) {
			return Invoker::Invoke<BOOL, 317>(entity, weaponHash, weaponType);
		}

		void CLEAR_ENTITY_LAST_WEAPON_DAMAGE(Entity entity) {
			Invoker::Invoke<void, 318>(entity);
		}

		void SET_PED_DROPS_WEAPON(Ped ped) {
			Invoker::Invoke<void, 319>(ped);
		}

		void SET_PED_DROPS_INVENTORY_WEAPON(Ped ped, Hash weaponHash, float xOffset, float yOffset, float zOffset, int ammoCount) {
			Invoker::Invoke<void, 320>(ped, weaponHash, xOffset, yOffset, zOffset, ammoCount);
		}

		int GET_MAX_AMMO_IN_CLIP(Ped ped, Hash weaponHash, BOOL p2) {
			return Invoker::Invoke<int, 321>(ped, weaponHash, p2);
		}

		int _GET_TIME_BEFORE_VEHICLE_WEAPON_RELOAD_FINISHES(Vehicle vehicle, int seat) {
			return Invoker::Invoke<int, 6722>(vehicle, seat);
		}

		BOOL _HAS_WEAPON_RELOADING_IN_VEHICLE(Vehicle vehicle, int seat) {
			return Invoker::Invoke<BOOL, 6723>(vehicle, seat);
		}

		float _GET_VEHICLE_WEAPON_RELOAD_TIME(Vehicle vehicle, int seat) {
			return Invoker::Invoke<float, 6724>(vehicle, seat);
		}

		BOOL _GET_AMMO_IN_VEHICLE_WEAPON_CLIP(Vehicle vehicle, int seat, int* ammo) {
			return Invoker::Invoke<BOOL, 6725>(vehicle, seat, ammo);
		}

		BOOL GET_AMMO_IN_CLIP(Ped ped, Hash weaponHash, int* ammo) {
			return Invoker::Invoke<BOOL, 322>(ped, weaponHash, ammo);
		}

		BOOL _SET_AMMO_IN_VEHICLE_WEAPON_CLIP(Vehicle vehicle, int seat, int ammo) {
			return Invoker::Invoke<BOOL, 6726>(vehicle, seat, ammo);
		}

		BOOL SET_AMMO_IN_CLIP(Ped ped, Hash weaponHash, int ammo) {
			return Invoker::Invoke<BOOL, 323>(ped, weaponHash, ammo);
		}

		BOOL GET_MAX_AMMO(Ped ped, Hash weaponHash, int* ammo) {
			return Invoker::Invoke<BOOL, 324>(ped, weaponHash, ammo);
		}

		BOOL GET_MAX_AMMO_BY_TYPE(Ped ped, Hash ammoTypeHash, int* ammo) {
			return Invoker::Invoke<BOOL, 325>(ped, ammoTypeHash, ammo);
		}

		void ADD_PED_AMMO_BY_TYPE(Ped ped, Hash ammoTypeHash, int ammo) {
			Invoker::Invoke<void, 326>(ped, ammoTypeHash, ammo);
		}

		void SET_PED_AMMO_BY_TYPE(Ped ped, Hash ammoTypeHash, int ammo) {
			Invoker::Invoke<void, 327>(ped, ammoTypeHash, ammo);
		}

		int GET_PED_AMMO_BY_TYPE(Ped ped, Hash ammoTypeHash) {
			return Invoker::Invoke<int, 328>(ped, ammoTypeHash);
		}

		void SET_PED_AMMO_TO_DROP(Ped ped, int p1) {
			Invoker::Invoke<void, 329>(ped, p1);
		}

		void SET_PICKUP_AMMO_AMOUNT_SCALER(float p0) {
			Invoker::Invoke<void, 330>(p0);
		}

		Hash GET_PED_AMMO_TYPE_FROM_WEAPON(Ped ped, Hash weaponHash) {
			return Invoker::Invoke<Hash, 331>(ped, weaponHash);
		}

		Hash GET_PED_ORIGINAL_AMMO_TYPE_FROM_WEAPON(Ped ped, Hash weaponHash) {
			return Invoker::Invoke<Hash, 332>(ped, weaponHash);
		}

		BOOL GET_PED_LAST_WEAPON_IMPACT_COORD(Ped ped, Vector3* coords) {
			return Invoker::Invoke<BOOL, 333>(ped, coords);
		}

		void SET_PED_GADGET(Ped ped, Hash gadgetHash, BOOL p2) {
			Invoker::Invoke<void, 334>(ped, gadgetHash, p2);
		}

		BOOL GET_IS_PED_GADGET_EQUIPPED(Ped ped, Hash gadgetHash) {
			return Invoker::Invoke<BOOL, 335>(ped, gadgetHash);
		}

		Hash GET_SELECTED_PED_WEAPON(Ped ped) {
			return Invoker::Invoke<Hash, 336>(ped);
		}

		void EXPLODE_PROJECTILES(Ped ped, Hash weaponHash, BOOL p2) {
			Invoker::Invoke<void, 337>(ped, weaponHash, p2);
		}

		void REMOVE_ALL_PROJECTILES_OF_TYPE(Hash weaponHash, BOOL explode) {
			Invoker::Invoke<void, 338>(weaponHash, explode);
		}

		float GET_LOCKON_DISTANCE_OF_CURRENT_PED_WEAPON(Ped ped) {
			return Invoker::Invoke<float, 339>(ped);
		}

		float GET_MAX_RANGE_OF_CURRENT_PED_WEAPON(Ped ped) {
			return Invoker::Invoke<float, 340>(ped);
		}

		BOOL HAS_VEHICLE_GOT_PROJECTILE_ATTACHED(Ped driver, Vehicle vehicle, Hash weaponHash, Any p3) {
			return Invoker::Invoke<BOOL, 341>(driver, vehicle, weaponHash, p3);
		}

		void GIVE_WEAPON_COMPONENT_TO_PED(Ped ped, Hash weaponHash, Hash componentHash) {
			Invoker::Invoke<void, 342>(ped, weaponHash, componentHash);
		}

		void REMOVE_WEAPON_COMPONENT_FROM_PED(Ped ped, Hash weaponHash, Hash componentHash) {
			Invoker::Invoke<void, 343>(ped, weaponHash, componentHash);
		}

		BOOL HAS_PED_GOT_WEAPON_COMPONENT(Ped ped, Hash weaponHash, Hash componentHash) {
			return Invoker::Invoke<BOOL, 344>(ped, weaponHash, componentHash);
		}

		BOOL IS_PED_WEAPON_COMPONENT_ACTIVE(Ped ped, Hash weaponHash, Hash componentHash) {
			return Invoker::Invoke<BOOL, 345>(ped, weaponHash, componentHash);
		}

		BOOL REFILL_AMMO_INSTANTLY(Ped ped) {
			return Invoker::Invoke<BOOL, 346>(ped);
		}

		BOOL MAKE_PED_RELOAD(Ped ped) {
			return Invoker::Invoke<BOOL, 347>(ped);
		}

		BOOL _TRIGGER_VEHICLE_WEAPON_RELOAD(Vehicle vehicle, int seat, Ped ped) {
			return Invoker::Invoke<BOOL, 6727>(vehicle, seat, ped);
		}

		void REQUEST_WEAPON_ASSET(Hash weaponHash, int p1, int p2) {
			Invoker::Invoke<void, 348>(weaponHash, p1, p2);
		}

		BOOL HAS_WEAPON_ASSET_LOADED(Hash weaponHash) {
			return Invoker::Invoke<BOOL, 349>(weaponHash);
		}

		void REMOVE_WEAPON_ASSET(Hash weaponHash) {
			Invoker::Invoke<void, 350>(weaponHash);
		}

		Object CREATE_WEAPON_OBJECT(Hash weaponHash, int ammoCount, float x, float y, float z, BOOL showWorldModel, float scale, Any p7, Any p8, Any p9) {
			return Invoker::Invoke<Object, 351>(weaponHash, ammoCount, x, y, z, showWorldModel, scale, p7, p8, p9);
		}

		void GIVE_WEAPON_COMPONENT_TO_WEAPON_OBJECT(Object weaponObject, Hash componentHash) {
			Invoker::Invoke<void, 352>(weaponObject, componentHash);
		}

		void REMOVE_WEAPON_COMPONENT_FROM_WEAPON_OBJECT(Object object, Hash componentHash) {
			Invoker::Invoke<void, 353>(object, componentHash);
		}

		BOOL HAS_WEAPON_GOT_WEAPON_COMPONENT(Object weapon, Hash componentHash) {
			return Invoker::Invoke<BOOL, 354>(weapon, componentHash);
		}

		void GIVE_WEAPON_OBJECT_TO_PED(Object weaponObject, Ped ped) {
			Invoker::Invoke<void, 355>(weaponObject, ped);
		}

		BOOL DOES_WEAPON_TAKE_WEAPON_COMPONENT(Hash weaponHash, Hash componentHash) {
			return Invoker::Invoke<BOOL, 356>(weaponHash, componentHash);
		}

		Object GET_WEAPON_OBJECT_FROM_PED(Ped ped, BOOL p1) {
			return Invoker::Invoke<Object, 357>(ped, p1);
		}

		void GIVE_LOADOUT_TO_PED(Ped ped, Hash loadoutHash) {
			Invoker::Invoke<void, 358>(ped, loadoutHash);
		}

		void SET_PED_WEAPON_TINT_INDEX(Ped ped, Hash weaponHash, int tintIndex) {
			Invoker::Invoke<void, 359>(ped, weaponHash, tintIndex);
		}

		int GET_PED_WEAPON_TINT_INDEX(Ped ped, Hash weaponHash) {
			return Invoker::Invoke<int, 360>(ped, weaponHash);
		}

		void SET_WEAPON_OBJECT_TINT_INDEX(Object weapon, int tintIndex) {
			Invoker::Invoke<void, 361>(weapon, tintIndex);
		}

		int GET_WEAPON_OBJECT_TINT_INDEX(Object weapon) {
			return Invoker::Invoke<int, 362>(weapon);
		}

		int GET_WEAPON_TINT_COUNT(Hash weaponHash) {
			return Invoker::Invoke<int, 363>(weaponHash);
		}

		void SET_PED_WEAPON_COMPONENT_TINT_INDEX(Ped ped, Hash weaponHash, Hash camoComponentHash, int colorIndex) {
			Invoker::Invoke<void, 364>(ped, weaponHash, camoComponentHash, colorIndex);
		}

		int GET_PED_WEAPON_COMPONENT_TINT_INDEX(Ped ped, Hash weaponHash, Hash camoComponentHash) {
			return Invoker::Invoke<int, 365>(ped, weaponHash, camoComponentHash);
		}

		void SET_WEAPON_OBJECT_COMPONENT_TINT_INDEX(Object weaponObject, Hash camoComponentHash, int colorIndex) {
			Invoker::Invoke<void, 366>(weaponObject, camoComponentHash, colorIndex);
		}

		int GET_WEAPON_OBJECT_COMPONENT_TINT_INDEX(Object weaponObject, Hash camoComponentHash) {
			return Invoker::Invoke<int, 367>(weaponObject, camoComponentHash);
		}

		int GET_PED_WEAPON_CAMO_INDEX(Ped ped, Hash weaponHash) {
			return Invoker::Invoke<int, 368>(ped, weaponHash);
		}

		void SET_WEAPON_OBJECT_CAMO_INDEX(Object weaponObject, int p1) {
			Invoker::Invoke<void, 369>(weaponObject, p1);
		}

		BOOL GET_WEAPON_HUD_STATS(Hash weaponHash, Any* outData) {
			return Invoker::Invoke<BOOL, 370>(weaponHash, outData);
		}

		BOOL GET_WEAPON_COMPONENT_HUD_STATS(Hash componentHash, Any* outData) {
			return Invoker::Invoke<BOOL, 371>(componentHash, outData);
		}

		float GET_WEAPON_DAMAGE(Hash weaponHash, Hash componentHash) {
			return Invoker::Invoke<float, 372>(weaponHash, componentHash);
		}

		int GET_WEAPON_CLIP_SIZE(Hash weaponHash) {
			return Invoker::Invoke<int, 373>(weaponHash);
		}

		float GET_WEAPON_TIME_BETWEEN_SHOTS(Hash weaponHash) {
			return Invoker::Invoke<float, 374>(weaponHash);
		}

		void SET_PED_CHANCE_OF_FIRING_BLANKS(Ped ped, float xBias, float yBias) {
			Invoker::Invoke<void, 375>(ped, xBias, yBias);
		}

		Object SET_PED_SHOOT_ORDNANCE_WEAPON(Ped ped, float p1) {
			return Invoker::Invoke<Object, 376>(ped, p1);
		}

		void REQUEST_WEAPON_HIGH_DETAIL_MODEL(Entity weaponObject) {
			Invoker::Invoke<void, 377>(weaponObject);
		}

		void _SET_WEAPON_PED_DAMAGE_MODIFIER(Hash weapon, float damageModifier) {
			Invoker::Invoke<void, 378>(weapon, damageModifier);
		}

		void SET_WEAPON_DAMAGE_MODIFIER(Hash weaponHash, float damageMultiplier) {
			Invoker::Invoke<void, 379>(weaponHash, damageMultiplier);
		}

		void SET_WEAPON_AOE_MODIFIER(Hash weaponHash, float multiplier) {
			Invoker::Invoke<void, 380>(weaponHash, multiplier);
		}

		void SET_WEAPON_EFFECT_DURATION_MODIFIER(Hash p0, float p1) {
			Invoker::Invoke<void, 381>(p0, p1);
		}

		BOOL IS_PED_CURRENT_WEAPON_SILENCED(Ped ped) {
			return Invoker::Invoke<BOOL, 382>(ped);
		}

		BOOL IS_FLASH_LIGHT_ON(Ped ped) {
			return Invoker::Invoke<BOOL, 383>(ped);
		}

		BOOL SET_FLASH_LIGHT_FADE_DISTANCE(float distance) {
			return Invoker::Invoke<BOOL, 384>(distance);
		}

		void SET_FLASH_LIGHT_ACTIVE_HISTORY(Ped ped, BOOL toggle) {
			Invoker::Invoke<void, 385>(ped, toggle);
		}

		void SET_WEAPON_ANIMATION_OVERRIDE(Ped ped, Hash animStyle) {
			Invoker::Invoke<void, 386>(ped, animStyle);
		}

		int GET_WEAPON_DAMAGE_TYPE(Hash weaponHash) {
			return Invoker::Invoke<int, 387>(weaponHash);
		}

		void SET_EQIPPED_WEAPON_START_SPINNING_AT_FULL_SPEED(Ped ped) {
			Invoker::Invoke<void, 388>(ped);
		}

		BOOL CAN_USE_WEAPON_ON_PARACHUTE(Hash weaponHash) {
			return Invoker::Invoke<BOOL, 389>(weaponHash);
		}

		int CREATE_AIR_DEFENCE_SPHERE(float x, float y, float z, float radius, float p4, float p5, float p6, Hash weaponHash) {
			return Invoker::Invoke<int, 390>(x, y, z, radius, p4, p5, p6, weaponHash);
		}

		int CREATE_AIR_DEFENCE_ANGLED_AREA(float p0, float p1, float p2, float p3, float p4, float p5, float p6, float p7, float p8, float radius, Hash weaponHash) {
			return Invoker::Invoke<int, 391>(p0, p1, p2, p3, p4, p5, p6, p7, p8, radius, weaponHash);
		}

		BOOL REMOVE_AIR_DEFENCE_SPHERE(int zoneId) {
			return Invoker::Invoke<BOOL, 392>(zoneId);
		}

		void REMOVE_ALL_AIR_DEFENCE_SPHERES() {
			Invoker::Invoke<void, 393>();
		}

		void SET_PLAYER_TARGETTABLE_FOR_AIR_DEFENCE_SPHERE(Player player, int zoneId, BOOL enable) {
			Invoker::Invoke<void, 394>(player, zoneId, enable);
		}

		BOOL IS_AIR_DEFENCE_SPHERE_IN_AREA(float x, float y, float z, float radius, int* outZoneId) {
			return Invoker::Invoke<BOOL, 395>(x, y, z, radius, outZoneId);
		}

		void FIRE_AIR_DEFENCE_SPHERE_WEAPON_AT_POSITION(int zoneId, float x, float y, float z) {
			Invoker::Invoke<void, 396>(zoneId, x, y, z);
		}

		BOOL DOES_AIR_DEFENCE_SPHERE_EXIST(int zoneId) {
			return Invoker::Invoke<BOOL, 397>(zoneId);
		}

		void SET_CAN_PED_SELECT_INVENTORY_WEAPON(Ped ped, Hash weaponHash, BOOL toggle) {
			Invoker::Invoke<void, 398>(ped, weaponHash, toggle);
		}

		void SET_CAN_PED_SELECT_ALL_WEAPONS(Ped ped, BOOL toggle) {
			Invoker::Invoke<void, 399>(ped, toggle);
		}

	} // namespace WEAPON

	namespace ZONE {

		int GET_ZONE_AT_COORDS(float x, float y, float z) {
			return Invoker::Invoke<int, 2351>(x, y, z);
		}

		int GET_ZONE_FROM_NAME_ID(const char* zoneName) {
			return Invoker::Invoke<int, 2352>(zoneName);
		}

		int GET_ZONE_POPSCHEDULE(int zoneId) {
			return Invoker::Invoke<int, 2353>(zoneId);
		}

		const char* GET_NAME_OF_ZONE(float x, float y, float z) {
			return Invoker::Invoke<const char*, 2354>(x, y, z);
		}

		void SET_ZONE_ENABLED(int zoneId, BOOL toggle) {
			Invoker::Invoke<void, 2355>(zoneId, toggle);
		}

		int GET_ZONE_SCUMMINESS(int zoneId) {
			return Invoker::Invoke<int, 2356>(zoneId);
		}

		void OVERRIDE_POPSCHEDULE_VEHICLE_MODEL(int scheduleId, Hash vehicleHash) {
			Invoker::Invoke<void, 2357>(scheduleId, vehicleHash);
		}

		void CLEAR_POPSCHEDULE_OVERRIDE_VEHICLE_MODEL(int scheduleId) {
			Invoker::Invoke<void, 2358>(scheduleId);
		}

		Hash GET_HASH_OF_MAP_AREA_AT_COORDS(float x, float y, float z) {
			return Invoker::Invoke<Hash, 2359>(x, y, z);
		}

	} // namespace ZONE

} // namespace base::menu::natives

#pragma warning(pop)
