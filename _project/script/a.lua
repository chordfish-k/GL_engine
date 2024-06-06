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
local lineD = 0
local accel = 700;
local friction = 500;
local sign = 0;

local color = Vec4();
color.x = 1;
color.y = 1;
color.z = 1;
color.w = 1;

local velocity;

List = {}

-- first用于队列头部索引    last用于队列尾部索引
function List.new()
    return {first = 0, last = -1}
end


function List.pushFront(list, value)
    -- first用于向负数进攻
    local first = list.first - 1
    list.first = first
    list[first] = value
end


function List.pushBack(list, value)
    -- last用于向正数进攻
    local last = list.last + 1
    list.last = last
    list[last] = value
end


function List.popFront(list)
    local first = list.first
    if first > list.last then
        error("List is empty")
    end
    local value = list[first]
    list[first] = nil
    list.first = first + 1
    return value
end


function List.popBack(list)
    local last = list.last
    if list.first > last then
        error("List is empty")
    end
    local value = list[last]
    list[last] = nil
    list.last = last - 1
    return value
end

local prePos = List.new();

function node:Start()
    print(node.this.name .. " has started.");
    node.rb = node.this:ToRigidBody2D();
    node.tr = node.this.transform;
    node.spr = node.this.children[2]:ToSpriteRenderer();
    node.anim = node.spr:GetAnimation();
    List.pushBack(prePos, node.this:GetWorldPos());
end


function node:Update(dt)
    dtCounter = dtCounter + dt;
    velocity = node.rb.velocity;

    node:Move(dt);
    node:Jump(dt);
    local a = Vec2();
    a.x=0;
    a.y=0;
    local b = Vec2();
    b.x = 20;
    b.y = 0;
    Draw.Line(a, b, color);

    for i = prePos.first, prePos.last-1 do
        Draw.Line(prePos[i],prePos[i+1], color);
    end

    if Input.IsKeyPressed(Keys.w) then

    end

    if lineD > 0.018 then
        List.popFront(prePos);
        lineD = 0;
    end
    lineD = lineD + dt;
    List.pushBack(prePos, node.this:GetWorldPos());
    node.rb.velocity = velocity;
end

function node:Move(dt)


    -- 移动方向控制
    if Input.IsKeyDown(Keys.a) then
        sign = -1;
        node.spr.flipX = true;
    elseif Input.IsKeyDown(Keys.d) then
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