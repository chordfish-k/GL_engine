local ball = {
    this,
    rb,
}

local first = false;

function ball:Start()

end

function ball:Update(dt)

end

function ball:OnCollision(other)
    other:Destroy();
end

return ball;