config = {
	gameroot = "C:/Users/username/Documents/My Games/My Game",
}

function T()

end

function Init()
	log.info("This is a formatting test: nil: {}, bool: {}, number: {}, string: {}, {}, {}", nil, true, 123, "Hello World!", config, T)
end