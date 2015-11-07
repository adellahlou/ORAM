require "agent"
require "random_agent"

function hslToRgb(h, s, l, a)
  local r, g, b

  if s == 0 then
    r, g, b = l, l, l -- achromatic
  else
    function hue2rgb(p, q, t)
      if t < 0   then t = t + 1 end
      if t > 1   then t = t - 1 end
      if t < 1/6 then return p + (q - p) * 6 * t end
      if t < 1/2 then return q end
      if t < 2/3 then return p + (q - p) * (2/3 - t) * 6 end
      return p
    end

    local q
    if l < 0.5 then q = l * (1 + s) else q = l + s - l * s end
    local p = 2 * l - q

    r = hue2rgb(p, q, h + 1/3)
    g = hue2rgb(p, q, h)
    b = hue2rgb(p, q, h - 1/3)
  end

  return r * 255, g * 255, b * 255, a * 255
end

function love.load(args)
	love.graphics.setBackgroundColor(255, 255, 255)
	love.graphics.setNewFont(28)
	
	setup()

	ctr = 1 -- Used to generate the initial blocks
	active = 10 -- Number of unique blocks

	timestep = 0.1 -- Update at 10hz
	time = timestep

	paused = false
end

function love.update(dt)
	if not paused then
		time = time + dt
	end

	if time > timestep then
		if ctr <= active then
			-- Initally generate one of every block
			write(ctr)
			ctr = ctr + 1
		else
			-- Uncomment next line to simulate the randomisation agent
			-- random_agent(block_map)
		end
		
		time = time - timestep
	end

end

function read(bid)
	if not bid then
		bid = love.math.random(active)
	end

	access("READ", bid, block_map)
end

function write(bid)
	if not bid then
		bid = love.math.random(active)
	end

   	access("WRITE", bid, block_map, "eleven")
end

function love.mousepressed(x, y, button)
	if button == 'l' then
		read()
	elseif button == 'r' then
   		write()
   	end
end

function love.keypressed(key)
	if key == "p" then
		paused = not paused
	end
end

function block_colour(bid)
	local hue = bid * 1.6180339887498
	hue = hue - math.floor(hue)
	
	return hslToRgb(hue, 0.75, 0.75, 1)
end

function love.draw(dt)
	if #buffer > 0 then
		love.graphics.setColor(0, 0, 0)
		love.graphics.printf("buffer size = " .. #buffer, 10, 10, 400)
	end

	-- Draw the randomisation agents buffer
	for x,elem in pairs(buffer) do
		love.graphics.setColor(block_colour(elem.bid));
		love.graphics.rectangle("fill", 200 + 64*x, 0, 60, 60)

		love.graphics.setColor(0, 0, 0)
		love.graphics.rectangle("line", 200 + 64*x, 0, 60, 60)
		
		if changed[elem.bid] then
			love.graphics.setColor(255, 255, 255)
			love.graphics.printf("c", 200 + 64*x, 14/2, 60, "center")
		end
	end
	
	local pos_map = build_posmap(block_map)

	c = 0
	for y = 1, 10 do
		for x = 1, 10 do
			c = c + 1
			if pos_map[c] ~= -1 then
				-- Draw each square
				love.graphics.setColor(block_colour(pos_map[c]))
				love.graphics.rectangle("fill", 64*x, 64*y, 60, 60)
			
				-- Fill in the square
				love.graphics.setColor(0, 0, 0)
				love.graphics.rectangle("line", 64*x, 64*y, 60, 60)
				
				-- Draw the block ID
				love.graphics.setColor(255, 255, 255)
				love.graphics.printf(pos_map[c], 64*x, 64*y +28/2, 60, "center")
			end
		end
	end

	if paused then
		love.graphics.setColor(0, 0, 0)
		love.graphics.printf("paused", 600, 10, 400)
	end
end
