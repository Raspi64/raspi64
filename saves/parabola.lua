local t = 0.001
local color = {}
color.red = 255
color.green = 255
color.blue = 255
color.alpha = 255

clear()


for x=0,500,0.2 do
	y = -0.008 * (x - 250)^2 + 255
	print(x ,"", y)
	draw(x, y, 5, color);
	sleep(t)
end



