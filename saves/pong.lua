local min_x = 50
local min_y = 50
local max_x = 400
local max_y = 400

local paddle_height = 80
local paddle_width = 10

local paddle_left_pos = (max_y - min_y) / 2 - paddle_height / 2
local paddle_right_pos = paddle_left_pos


local paddle_move_delta = 15

local velocity = 10
local min_v_y = 15
local max_v_y = 40

local ball_size = 20

local ball_x = max_x/2-ball_size/2
local ball_y = max_y/2-ball_size/2

local ball_v_x = 10
local ball_v_y = -15


function draw_box(x, y, h, w)
	for a=x, x + w, 2 do
		for b=y, y + h, 2 do
			draw(a, b, 0, 0, 0, 255, 2)
		end
	end
end

local key_up = false
local key_down = false

local ai_up = false
local ai_down = false

local running = true

function calculate_vertical_velocity(paddle_pos)
	local paddle_middle_y = paddle_pos + paddle_height / 2
	local ball_middle_y = ball_y + ball_size / 2

	-- print("Ball:", ball_middle_y)
	-- print("Paddle:", paddle_middle_y)

	local delta_y = ball_middle_y - paddle_middle_y + 0.0
	-- print("delta_y:", delta_y)
	local identity_delta_y = delta_y / (paddle_height / 2)
	-- print("identity_delta_y:", identity_delta_y)
	local new_delta_y = identity_delta_y * max_v_y
	-- print("x:", x)
	if new_delta_y == math.abs(new_delta_y) then
		return math.min(math.max(new_delta_y, min_v_y), max_v_y)
	else
		return math.max(math.min(new_delta_y, -min_v_y), -max_v_y)
	end
end

function update()
	-- move player paddle
	if key_up and paddle_right_pos >= min_y + paddle_move_delta then
		paddle_right_pos = paddle_right_pos - paddle_move_delta
	elseif key_down and paddle_right_pos <= max_y - paddle_height then
		paddle_right_pos = paddle_right_pos + paddle_move_delta
	end

	-- apply ball velocity
	ball_x = ball_x + ball_v_x
	ball_y = ball_y + ball_v_y

	-- 'AI' paddle move
	local paddle_left_middle_y = paddle_left_pos + paddle_height / 2
	local ball_middle_y = ball_y + ball_size / 2

	ai_up = paddle_left_middle_y > ball_middle_y + ball_size / 2
	ai_down = paddle_left_middle_y < ball_middle_y - ball_size / 2

	-- move ai paddle
	if ai_up and paddle_left_pos >= min_y + paddle_move_delta then
		paddle_left_pos = paddle_left_pos - paddle_move_delta
	elseif ai_down and paddle_left_pos <= max_y - paddle_height then
		paddle_left_pos = paddle_left_pos + paddle_move_delta
	end

	-- left-right collision
	if ball_x <= min_x - ball_size then
		-- ball_v_x = velocity
		-- ball_x = 0
		print("Left lost!")
		running = false
	elseif ball_x + ball_size >= max_x + ball_size then
		-- ball_v_x = -velocity
		-- ball_x = max_x - ball_size
		print("Right lost!")
		running = false
	end

	-- top-bottom collision
	if ball_y <= min_y then
		ball_v_y = math.abs(ball_v_y)
		ball_y = min_y
	elseif ball_y + ball_size >= max_y then
		ball_v_y = -math.abs(ball_v_y)
		ball_y = max_y - ball_size
	end

	-- right paddle collision
	if ball_x + ball_size >= max_x - paddle_width then -- right edge
		if ball_y < paddle_right_pos + paddle_height and -- bottom corner of paddle
			ball_y + ball_size > paddle_right_pos then -- top corner of paddle
			-- paddle hits ball
			ball_v_y = calculate_vertical_velocity(paddle_right_pos)
			print("Vel:", ball_v_y)
			ball_v_x = -velocity
			ball_x = max_x - paddle_width - ball_size
		end
	end

	-- left paddle collision
	if ball_x <= min_x + paddle_width then -- left edge
		if ball_y < paddle_left_pos + paddle_height and -- bottom corner of paddle
			ball_y + ball_size > paddle_left_pos then -- top corner of paddle
			-- paddle hits ball
			ball_v_y = calculate_vertical_velocity(paddle_left_pos)
			print("Vel:", ball_v_y)
			ball_v_x = velocity
			ball_x = min_x + paddle_width
		end
	end
end

function render()
	clear()
	-- draw paddles
	draw_box(min_x,                paddle_left_pos,  paddle_height, paddle_width)
	draw_box(max_x - paddle_width, paddle_right_pos, paddle_height, paddle_width)
	-- draw ball
	draw_box(ball_x, ball_y, ball_size, ball_size)
	-- draw borders
	draw(min_x, min_y, 255, 0, 0, 255, 2)
	draw(min_x, max_y, 255, 0, 0, 255, 2)
	draw(max_x, min_y, 255, 0, 0, 255, 2)
	draw(max_x, max_y, 255, 0, 0, 255, 2)
end

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


while running do
	-- main loop
	update()
	render()
	sleep(0.1)
end




