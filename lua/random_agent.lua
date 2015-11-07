require "agent"

buffer = {}
buffer_length = 5

function fill_buffer(blockmap)
	while #buffer < buffer_length do
		local bid = math.random(10)
	
		changed[bid] = false

		local elem = {}
		elem.bid = bid
		elem.data = access("READ", bid, blockmap)
		--print(elem.data)
		table.insert(buffer, elem)
	end
end

function random_agent(blockmap)
	fill_buffer(blockmap)
	
	-- Pick a random element
	local index = random(buffer)
	local elem = buffer[index]
	
	-- Randomise the block
	elem.data = encrypt(decrypt(elem.data))
	
	-- Write the data
	local posmap = build_posmap(blockmap)
	local new_pos = takePosition(posmap, blockmap)
	server_write(new_pos, elem.data)

	if changed[elem.bid] then
		-- Remove element from buffer
		buffer[index] = buffer[#buffer]
		buffer[#buffer] = nil
		
		changed[elem.bid] = false
	else
		-- It's adding to the set for that block
		table.insert(blockmap[elem.bid], new_pos)
	end
end
