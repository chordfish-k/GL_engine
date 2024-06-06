local ball = {
    this,
    rb,
}

local first = false;

function ball:Start()
    ball.rb = ball.this:ToRigidBody2D();
    velocity = ball.rb.velocity;
    velocity.x = 300;
    velocity.y = math.random(100, 500);
    ball.rb.velocity = velocity;
    dtCnt = 0;
    first = true;
end

function ball:Update(dt)

end

return ball;