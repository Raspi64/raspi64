local x = 250
local y = 250
local r = 4
local d_r = 7
local t = 0.0001

clear()

for i=1,50 do
	for angle=0, 360, 1 do
		d_x = r * math.cos(angle * math.pi / 180);
		d_y = r * math.sin(angle * math.pi / 180);
		local c_r = -0.035 * ((angle - 60) * (angle - 60)) + 255
		local c_g = -0.035 * ((angle - 180) * (angle - 180)) + 255
		local c_b = -0.035 * ((angle - 240) * (angle - 240)) + 255


		if c_r < 0 then
			c_r = 0
		end

		if c_g < 0 then
			c_g = 0
		end

		if c_b < 0 then
			c_b = 0
		end

		print(x + d_x, y + d_y, c_r, c_g, c_b)
		draw(math.floor(x + d_x), math.floor(y + d_y), math.floor(c_r), math.floor(c_g), math.floor(c_b), 255, 5);
		sleep(t)
		r = r + d_r / 360
	end
end

