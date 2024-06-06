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

    if dtCounter > 0.3 then
        dtCounter = 0;
        shooter:ShootOnce();
    end
end

function shooter:ShootOnce()
    local ballNode = Prefab.CreateByFile("prefab/Ball.pfb");
    shooter.this:AddChildNode(ballNode);

end

return shooter;