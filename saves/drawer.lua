function sleep(n)
    os.execute("sleep " .. tonumber(n))
end

function sub(r, g, b, t)
    for i = 0, 250, 10 do
        draw(i, i, r, g, b, 255, 10)
        draw(i, 500 - i, r, g, b, 255, 10)
        draw(500 - i, i, r, g, b, 255, 10)
        draw(500 - i, 500 - i, r, g, b, 255, 10)
        sleep(t)
    end
end

function drawer()
    local t = 0.05
    while true do
        sub(255, 0, 0, t)
        sub(0, 255, 0, t)
        sub(0, 0, 255, t)
    end
end

drawer()
