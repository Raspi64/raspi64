local x = 250
local y = 250
local r = 4
local d_r = 7
local t = 0.0001

clear()

local color = {}
color.alpha = 255

for i = 1, 50 do
	for angle = 0, 360, 1 do
		d_x = r * math.cos(angle * math.pi / 180)
		d_y = r * math.sin(angle * math.pi / 180)

		color.red = -0.035 * math.pow(angle - 60, 2) + 255
		color.green = -0.035 * math.pow(angle - 180, 2) + 255
		color.blue = -0.035 * math.pow(angle - 240, 2) + 255

		draw_pixel(x + d_x, y + d_y, 5, color);
		sleep(t)
		r = r + d_r / 360
	end
end

