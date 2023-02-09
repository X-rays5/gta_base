function Init()
    log.info("Hello {}!", "World")
	
	local player_id = natives.PLAYER.PLAYER_ID()

	local max_armour = natives.PLAYER.GET_PLAYER_MAX_ARMOUR(player_id)
	log.info("max armour: {}", max_armour)
	natives.PED.SET_PED_ARMOUR(natives.PLAYER.GET_PLAYER_PED_SCRIPT_INDEX(player_id), max_armour)

	log.debug("debug")
 	log.info("info")
	log.warn("warn")
	log.error("error")
	log.critical("critical")
end

function Tick()
	log.info("tick")
end

function Shutdown()
	natives.PED.SET_PED_ARMOUR(natives.PLAYER.GET_PLAYER_PED_SCRIPT_INDEX(natives.PLAYER.PLAYER_ID()), 0)
end