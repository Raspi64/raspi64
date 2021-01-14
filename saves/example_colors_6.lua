local x = 250
local y = 250
local r = 4
local d_r = 7
local t = 0.0001

local a = -0.035

clear()

local color = {}
color.alpha = 255

for i = 1, 50 do
	for angle = 0, 360, 1 do
		d_x = r * math.cos(angle * math.pi / 180)
		d_y = r * math.sin(angle * math.pi / 180)

		color.red = 250 * math.sin(0.8 * angle + 47.8) - 50
		color.green = -0.02 * (angle - 180)^2 + 200
		color.blue = 294 * math.sin(1.1 * angle - 246) - 95

		draw(x + d_x, y + d_y, 5, color)
		sleep(t)
		r = r + d_r / 360
	end
	a = a + 0.0005
	print(a)
end


