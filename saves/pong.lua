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
local min_v_y = 10
local max_v_y = 30

local ball_size = 20

local ball_x = max_x / 2 - ball_size / 2
local ball_y = max_y / 2 - ball_size / 2

local ball_v_x = 10
local ball_v_y = -15

local difficulty = false

function draw_box(x, y, h, w)
	for a = x, x + w, 2 do
		for b = y, y + h, 2 do
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
	ball_x = math.floor(ball_x + ball_v_x)
	ball_y = math.floor(ball_y + ball_v_y)

	-- 'AI' paddle move
	local paddle_left_middle_y = paddle_left_pos + paddle_height / 2
	local ball_middle_y = ball_y + ball_size / 2

	ai_up = paddle_left_middle_y > ball_middle_y + ball_size / difficulty
	ai_down = paddle_left_middle_y < ball_middle_y - ball_size / difficulty

	-- move AI paddle
	if ai_up and paddle_left_pos >= min_y + paddle_move_delta then
		paddle_left_pos = paddle_left_pos - paddle_move_delta
	elseif ai_down and paddle_left_pos <= max_y - paddle_height then
		paddle_left_pos = paddle_left_pos + paddle_move_delta
	end

	-- left-right collision
	if ball_x <= min_x - ball_size then
		print("GAME OVER!")
		running = false
	elseif ball_x + ball_size >= max_x + ball_size then
		print("YOU WIN!")
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
			-- print("Vel:", ball_v_y)
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
			-- print("Vel:", ball_v_y)
			ball_v_x = velocity
			ball_x = min_x + paddle_width
		end
	end

	velocity = velocity + 0.005
	-- print(velocity)
end

function render()
	clear()
	-- draw paddles
	draw_box(min_x, paddle_left_pos, paddle_height, paddle_width)
	draw_box(max_x - paddle_width, paddle_right_pos, paddle_height, paddle_width)
	-- draw ball
	draw_box(ball_x, ball_y, ball_size, ball_size)
	-- draw borders
	for x=min_x,max_x do
		draw(x, min_y, 255, 0, 0, 255, 1)
		draw(x, max_y, 255, 0, 0, 255, 1)
	end
	for y=min_y,max_y do
		draw(min_x, y, 255, 0, 0, 255, 1)
		draw(max_x, y, 255, 0, 0, 255, 1)
	end
	draw(min_x, min_y, 255, 0, 0, 255, 10)
	draw(min_x, max_y, 255, 0, 0, 255, 10)
	draw(max_x, min_y, 255, 0, 0, 255, 10)
	draw(max_x, max_y, 255, 0, 0, 255, 10)
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


while difficulty == false do
	print("Schwierigkeitsstufe? [1,2,3]")

	local input = io.read("l")
	if input == "1" then
		difficulty = 0.5
	elseif input == "2" then
		difficulty = 1
	elseif input == "3" then
		difficulty = 100
	end
end


while running do
	-- main loop
	update()
	render()
	sleep(0.07)
end


