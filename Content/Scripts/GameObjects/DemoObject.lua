
-- DemoObject script --

-- ======================================================
-- Set some initial values and return the model filename
-- ======================================================
g_rotx = -MathHelper.Pi /2
g_roty = MathHelper.Pi/2

function Init(object)
	
    object.position = Vector3(0.0,0.0,0.0);
    object.velocity = Vector3(0.0,0.0,0.0);
	object.direction = Vector3(0.0,-1.0,0.0);
	object.rotation = Vector3(g_rotx,g_roty,0.0)

    return "Content/Models/guy.rtx";
end

-- =========================================
-- handle the input for the game object
-- =========================================
function Input(object, input,currentIndex,playerIndex)
	
    local x = 0.0
    local y = 0.0
    local z = 0.0
    local speed = 5.0	-- move speed
	local dir = Vector3.Zero;
	input.UseGamepad = true -- set gamepad to true
	local rotation =  object.rotation.z

	-- check if the "run" button was pressed.
	if (input:IsKeyDown(Keys.DIK_Z) or ((input.UseGamepad == true) and
		(input:CurrentGamePadState(currentIndex).buttons.b)) ) then
		speed = speed * 2
	end
	-- check if the "up" button was pressed, other wise check the "down" button
    if (input:IsKeyDown(Keys.DIK_UP) ) then
		dir = Vector3(dir.x,MathHelper.Pi/2,dir.z);
        y = speed
		
    elseif (input:IsKeyDown(Keys.DIK_DOWN) ) then
		dir = Vector3(dir.x,-MathHelper.Pi/2,dir.z);
        y = -speed
		
    end
	-- check if the "left" button was pressed, other wise check the "right" button
    if (input:IsKeyDown(Keys.DIK_LEFT)  ) then
		dir = Vector3(-MathHelper.Pi/2,dir.y,dir.z);
        x = -speed
		
    elseif (input:IsKeyDown(Keys.DIK_RIGHT)  ) then
		dir = Vector3(MathHelper.Pi/2,dir.y,dir.z);
        x = speed
		
    end
	
	if( input:IsKeyDown(Keys.DIK_X)) then
		rotation = rotation + MathHelper.Pi/32
	elseif(input:IsKeyDown(Keys.DIK_C)) then
		rotation = rotation - MathHelper.Pi/32
	end
	
	object.rotation = Vector3(g_rotx,g_roty,rotation)
	
	-- get left stick input values
	if ((input.UseGamepad == true)) then
		local thumbx = input:CurrentGamePadState(currentIndex).thumbSticks.leftX;
		local thumby = input:CurrentGamePadState(currentIndex).thumbSticks.leftY;
		x = x + thumbx * speed;
		y = y + thumby * speed;
		dir = Vector3(dir.x + x * MathHelper.Pi/2,dir.y + y * MathHelper.Pi/2,dir.z) 
	end
	-- make sure the length is not zero
	if(dir:Length() > 0 or dir:Length() < 0) then
		object.desiredDirection = Vector3(dir.x,dir.y,dir.z);
		--object.desiredDirection = Vector3.Normalize(object.desiredDirection)
	end
	-- update the velocity
    object.velocity = Vector3(x,y,z);
    
end

-- =====================================
-- update the game object
-- =====================================
function Update(object,dt)
	-- set direction	
	local rotAmount = 0.25
	local dir = object.direction
	local ddir = object.desiredDirection
	object.direction = Vector3.Lerp(dir,ddir,rotAmount)

	-- Update position
    UpdatePosition(object,dt)

end

-- =====================================
-- update the game objects postion
-- =====================================
function UpdatePosition(object, dt)
	-- Update position
    local posx = object.position.x + object.velocity.x * dt;
    local posy = object.position.y + object.velocity.y * dt;
    local posz = object.position.z + object.velocity.z * dt;
    object.position = Vector3(posx, posy, posz);
	--object.rotation = Vector3(g_rotx,g_roty, object.rotation.z)

	-- create a position to "look at" and get the direction vector
	local dirPos = Vector3(object.position.x + object.direction.x,object.position.y + object.direction.y,0.0)
	local direction = Vector3(object.position.x - dirPos.x,object.position.y - dirPos.y,0.0)

	-- get the angle between the x and y direction
	local angle = MathHelper.AngleFromXY(direction.x, direction.y)
	-- scale down the large model
	local scale = Matrix.CreateScale(0.01)
 
	-- create a rotation matrix based on the angle we are facing 
	local rotz = Matrix.CreateRotationZ(object.rotation.z + angle)
	-- create a rotation matrix for the x and y axis
	local rotxy = Matrix.Multiply(Matrix.CreateRotationY(object.rotation.y), Matrix.CreateRotationX(object.rotation.x))
	-- get the final rotation, translation, and scale matrices
	local rot = Matrix.Multiply(rotxy,rotz)
	local translate = Matrix.CreateTranslation(object.position)
	local scaleRot = Matrix.Multiply(scale,rot)
	-- create the new world matrix
	object.worldMatrix = Matrix.Multiply(scaleRot,translate )

end