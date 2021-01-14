local x = 250
local y = 250
local r = 4
local d_r = 10
local t = 0.0001

clear()

local color = {}
color.alpha = 255

while true do
	for i=1,45 do
		for angle=0, 360, 1 do
			d_x = r * math.cos(angle * math.pi / 180);
			d_y = r * math.sin(angle * math.pi / 180);
			color.red = 255 * angle / 360
			color.green = 255 - (255 * angle / 360)
			color.blue = 255 - ((x+d_x+y + d_y) / 1000 * 255)

			draw(x + d_x, y + d_y, 5, color);
			sleep(t)
			r = r + d_r / 500
		end
	end
	clear()
	r = 4
end

