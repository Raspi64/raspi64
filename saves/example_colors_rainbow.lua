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
		d_x = r * math.cos(angle * math.pi / 180);
		d_y = r * math.sin(angle * math.pi / 180);

		local a = angle % 120

		if angle < 120 then
			color.red = 255 - a / 120 * 255
			color.green = a / 120 * 255
			color.blue = 0
		elseif angle < 240 then
			color.red = 0
			color.green = 255 - a / 120 * 255
			color.blue = a / 120 * 255
		else
			color.red = a / 120 * 255
			color.green = 0
			color.blue = 255 - a / 120 * 255
		end

		draw(x + d_x, y + d_y, 5, color)
		sleep(t)
		r = r + d_r / 360
	end
end


