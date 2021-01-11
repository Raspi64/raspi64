local x = 250
local y = 250
local r = 4
local d_r = 10
local t = 0.0001

clear()

while true do
	for i=1,45 do
		for angle=0, 360, 1 do
			d_x = r * math.cos(angle * math.pi / 180);
			d_y = r * math.sin(angle * math.pi / 180);
			local c_r = 255 * angle / 360
			local c_g = 255 - (255 * angle / 360)
			local c_b = 255 - ((x+d_x+y + d_y) / 1000 * 255)

			-- print(x + d_x, y + d_y, c_r, c_g, c_b)
			draw(math.floor(x + d_x), math.floor(y + d_y), math.floor(c_r), math.floor(c_g), math.floor(c_b), 255, 5);
			sleep(t)
			r = r + d_r / 500
		end
	end
	clear()
	r = 4
end

