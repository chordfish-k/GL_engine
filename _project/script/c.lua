local shooter = {
    this,
    tr
}

local dtCounter = 0;

function shooter:Start()
    shooter.tr = shooter.this.transform;
end

function shooter:Update(dt)
    dtCounter = dtCounter + dt;

    if Input.IsKeyPressed(Keys.space) then
        shooter:ShootOnce();
    end

    if dtCounter > 0.15 then
        dtCounter = 0;
        shooter:ShootOnce();
    end
end

-- 随机速度偏角发射
function shooter:ShootOnce()
    local ball = Prefab.CreateByFile("prefab/Ball.pfb");
    shooter.this:AddChildNode(ball);

    local rb = ball:ToRigidBody2D();
    velocity = rb.velocity;
    velocity.x = math.random(-300, 300);
    velocity.y = math.random(400, 700);
    rb.velocity = velocity;
    dtCnt = 0;
    first = true;

end

return shooter;