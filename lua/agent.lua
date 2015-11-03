require "string"

function server_read(pos)
	return server[pos]
end

function server_write(pos, data)
	server[pos] = data
end

-- Simulate performing an encryption
function encrypt(str)
	return string.reverse(str)
end

decrypt = encrypt

function setup()
	num = 100
	block_map = create_block_map(num)
	changed = create_change_map(num)

	server = {}
end

function random(t)
	return math.random(#t)
end

-- Removes all occurances of x in an array
function remove(t, x)
	for i = 1, #t do
		if t[i] == x then
			t[i] = t[#t]
			t[#t] = nil
		end
	end
end

function takePosition(pos_map, block_map)
	local rand_pos, rand_block, rand_list

	repeat
		rand_pos = random(pos_map)
		rand_block = pos_map[rand_pos]
		rand_list = block_map[rand_block]
	until rand_block == -1 or #rand_list > 1

	if rand_block ~= -1 then 
		remove(rand_list, rand_pos)
		pos_map[rand_pos] = -1
	end

	return rand_pos
end

function build_posmap(blockmap)
	local posmap = {}
	
	for i = 1, 100 do
		posmap[i] = -1
	end

	for bid,positions in pairs(blockmap) do
		for _,pos in ipairs(positions) do
			posmap[pos] = bid
		end
	end

	return posmap
end

function access(op, bid, blockmap, data)
	-- Build the position map
	local posmap = build_posmap(blockmap)

	local plaintext
	if op == "READ" then
	-- Find the blocks position
	local positions = blockmap[bid]
	local pos = positions[random(positions)]

		-- Retrive the block
	local ciphertext = server_read(pos)
	plaintext = decrypt(ciphertext)
	end

	-- Re-encrypt the block
	data = data or plaintext or "moo"
	ciphertext = encrypt(data)

	-- Write it back
	local new_pos = takePosition(posmap, blockmap)
	server_write(new_pos, ciphertext)

	-- Update the block map
	if op == "READ" then
		table.insert(blockmap[bid], new_pos)
	elseif op == "WRITE" then
		blockmap[bid] = {new_pos}

		changed[bid] = true
	end

	return plaintext
end

function create_change_map(num)
	local change_map = {}

	for i = 1, num do
		change_map[i] = false
	end

	return change_map
end

function create_block_map(num)
	local block_map = {}
	for i = 1, num do
		block_map[i] = {}
	end

	return block_map
end
