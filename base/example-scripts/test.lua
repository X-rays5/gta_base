config = {
	gameroot = "C:/Users/username/Documents/My Games/My Game",
	-- The path to the game's root directory
	-- This is where the game will look for the game's data files
	-- This is also where the game will save its data files
}

function T()

end

function Init()
	log.info("This is a formatting test: nil: {}, bool: {}, number: {}, string: {}, {}, {}", nil, true, 123, "Hello World!", config, T)
end