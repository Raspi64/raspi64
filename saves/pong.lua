local paddle_left_pos = 0
local paddle_right_pos = 0

local paddle_height = 100
local paddle_width = 15

local max_x = 480
local max_y = 450

local ball_x = 100
local ball_y = 100

local ball_v_x = 10
local ball_v_y = 10

local ball_size = 30

function draw_box(x, y, h, w, step)
	for a=x, x+w, step do
		for b=y, y+h, step do
			draw(a,b,0,0,0,255,step)
		end
	end
end

local key_up = false
local key_down = false


function update()
	if key_up and paddle_right_pos >= 20 then
		paddle_right_pos = paddle_right_pos - 20
	elseif key_down and paddle_right_pos <= max_y - paddle_height then
		paddle_right_pos = paddle_right_pos + 20
	end

	ball_x = ball_x + ball_v_x
	ball_y = ball_y + ball_v_y

	if ball_x <= 0 then
		ball_v_x = 10
		ball_x = 0
	elseif ball_x + ball_size >= max_x then
		ball_v_x = - 10
		ball_x = max_x - ball_size
	end

	if ball_y <= 0 then
		ball_v_y = 10
		ball_y = 0
	elseif ball_y + ball_size >= max_y then
		ball_v_y = - 10
		ball_y = max_y - ball_size
	end
end

function render()
	clear()
	draw_box(0,   paddle_left_pos,  paddle_height, paddle_width, 10)
	draw_box(max_x - paddle_width, paddle_right_pos, paddle_height, paddle_width, 10)
	draw_box(ball_x, ball_y, ball_size, ball_size, ball_size)
end

--draw_box(100,200,300,400,10)

function on_key_press(key)
	if key == "up" then
		key_up = true
	end
	if key == "down" then
		key_down = true
	end
end

function on_key_release(key)
	if key == "up" then
		key_up = false
	end
	if key == "down" then
		key_down = false
	end
end

register_key_listeners(on_key_press, on_key_release)


while true do
	-- main loop
	update()
	render()
	sleep(0.1)
end



