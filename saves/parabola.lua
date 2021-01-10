local t = 0.01

clear()

for x=0,360,0.2 do
	y = -0.008 * ((x - 180) * (x - 180)) + 255
	print(x ,"", y)
	draw(math.floor(x), math.floor(y), 255, 255, 255, 255, 5);
	sleep(t)
end


