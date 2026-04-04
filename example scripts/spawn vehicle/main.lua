local ped = natives.player.PlayerPedId()
LOG.info("Ped ID: {}", ped)

local hash = natives.misc.GetHashKey("adder")
LOG.info("Hash: {}", hash)

natives.streaming.RequestModel(hash)
LOG.info("Requested model")

while not natives.streaming.HasModelLoaded(hash) do coro.yield() end
LOG.info("Model loaded!")

local coords = natives.entity.GetEntityCoords(ped)
LOG.info("Coords: x={}, y={}, z={}", coords.x, coords.y, coords.z)

local heading = natives.entity.GetEntityHeading(ped)
LOG.info("Heading: {}", heading)

local veh = natives.vehicle.CreateVehicle(hash, coords.x, coords.y, coords.z, heading, true, false)
LOG.info("Vehicle ID: {}", veh)

if veh and veh > 0 then
    LOG.info("Vehicle created successfully!")
    natives.ped.SetPedIntoVehicle(ped, veh, -1)
    LOG.info("Ped set into vehicle")
else
    LOG.info("Vehicle creation failed!")
end

natives.streaming.SetModelAsNoLongerNeeded(hash)
LOG.info("done")