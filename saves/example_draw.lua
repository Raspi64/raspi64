local t = 0.05
local color = {}
color.alpha = 255

function sub()
	for i = 0, 250, 10 do
		draw(i, i, 10, color)
		draw(i, 500 - i, 10, color)
		draw(500 - i, i, 10, color)
		draw(500 - i, 500 - i, 10, color)
		sleep(t)
	end
end

while true do
	color.red = 255
	color.green = 0
	color.blue = 0
	sub()
	color.red = 0
	color.green = 255
	sub()
	color.green = 0
	color.blue = 255
	sub()
end
