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

		color.red = 125 * math.sin(0.5 * angle - 0) + 75
		color.green = 125 * math.sin(0.5 * angle - 120) + 75
		color.blue = 125 * math.sin(0.5 * angle - 240) + 75

		draw(x + d_x, y + d_y, 5, color)
		sleep(t)
		r = r + d_r / 360
	end
	a = a + 0.0005
	print(a)
end



