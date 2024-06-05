local node = {
    this,
    rb,
    tr,
    spr,
    anim
}

local speed = 130;
local maxSpeed = 200;
local jumpSpeed = 600;
local runIndex = 0;
local dtCounter = 0;
local accel = 700;
local friction = 500;
local sign = 0;

local velocity;

function node:Start()
    print(node.this.name .. " has started.");
    node.rb = node.this:ToRigidBody2D();
    node.tr = node.this.transform;
    node.spr = node.this.children[2]:ToSpriteRenderer();
    node.anim = node.spr:GetAnimation();
end


function node:Update(dt)
    dtCounter = dtCounter + dt;
    velocity = node.rb.velocity;

    node:Move(dt);
    node:Jump(dt);

    node.rb.velocity = velocity;
end


function node:Move(dt)
    -- 移动方向控制
    if (Input.IsKeyDown(Keys.a)) then
        sign = -1;
        node.spr.flipX = true;
    elseif (Input.IsKeyDown(Keys.d)) then
        sign = 1;
        node.spr.flipX = false;
    else
        sign = 0;
    end

    -- 速度控制
    if sign ~= 0 then
        if dtCounter > 0.08 then
            runIndex = runIndex + 1;
            if (runIndex == 4) then
                runIndex = 0;
            end
            dtCounter = 0;
            node.anim:SetFrame(runIndex);
        end

        velocity.x = MoveTowards(velocity.x, maxSpeed * sign, accel * dt);
    else
        local vx = velocity.x;
        if math.abs(vx) > 0 then
            velocity.x = MoveTowards(velocity.x, 0, friction * dt);
        end

        if runIndex ~= 0 then
            runIndex = 0;
            node.anim:SetFrame(runIndex);
        end

        startRunDtCnt = 0;
    end
end

function node:Jump(dt)
    if Input.IsKeyPressed(Keys.space) then
        velocity.y = jumpSpeed;
    end
end

-- 插值
function MoveTowards(current, target, maxDelta)
    if current < target then
        local delta = target - current;
        local move = math.min(delta, maxDelta);
        if delta > 0 then
            current = current + move;
        else
            current = current - move;
        end
    else
        local delta = current - target;
        local move = math.min(delta, maxDelta);
        if delta > 0 then
            current = current - move;
        else
            current = current + move;

        end
    end

    return current;
end

return node;