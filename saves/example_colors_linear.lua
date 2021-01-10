local x = 250
local y = 250
local r = 4
local d_r = 7
local t = 0.0001

local a = -0.035

clear()

for i=1,50 do
	for angle=0, 360, 1 do
		-- 
		d_x = r * math.cos(angle * math.pi / 180);
		d_y = r * math.sin(angle * math.pi / 180);

		local a = angle % 120

		if angle < 120 then
			c_r = 255 - a / 120 * 255
			c_g = a / 120 * 255
			c_b = 0
		elseif angle < 240 then
			c_r = 0
			c_g = 255 - a / 120 * 255
			c_b = a / 120 * 255
		else
			c_r = a / 120 * 255
			c_g = 0
			c_b = 255 - a / 120 * 255
		end
		--print(x + d_x, y + d_y, c_r, c_g, c_b)
		draw(math.floor(x + d_x), math.floor(y + d_y), math.floor(c_r), math.floor(c_g), math.floor(c_b), 255, 5);
		sleep(t)
		r = r + d_r / 360
	end
	a = a + 0.0005
	print(a)
end


