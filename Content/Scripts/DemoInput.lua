
-- Globals

function Input(input,currentIndex,light)
	local x = 0.0;
	local y = 0.0
	local z = 0.0
	local dir = light.Direction

	input.UseGamepad = input:GamePadWasConnected(currentIndex)
	-- keyboard input
	if(input:IsKeyDown(Keys.DIK_L) ) then
		
		x = dir.x;
		x = x + 0.1;
		if(x > 1.0) then
			x = 1.0;
		end
		dir = Vector3(x,dir.y,dir.z);

	elseif (input:IsKeyDown(Keys.DIK_J)) then

		
		x = dir.x;
				
		x = x + -0.1;
		if(x < -1.0) then
			x = -1.0;
		end
		dir = Vector3(x,dir.y,dir.z);
	end

	if (input:IsKeyDown(Keys.DIK_K)) then
		
		y = dir.y;

		y = y + -0.1;
		if(y < -1.0) then
			y = -1.0;
		end
		dir = Vector3(dir.x,y,dir.z);
	elseif (input:IsKeyDown(Keys.DIK_I)) then
		
		y = dir.y;

		y= y + 0.1;
		if(y > 1.0) then 
			y = 1.0;
		end
		dir = Vector3(dir.x,y,dir.z);
	end

	if (input:IsKeyDown(Keys.DIK_U)) then
		
		z = dir.z;

		z = z + 0.1;
		if(z > 1.0) then 
			z = 1.0;
		end
		dir = Vector3(dir.x,dir.y,z);
	elseif (input:IsKeyDown(Keys.DIK_H)) then
		
		z = dir.z;

		z = z + -0.1;
		if(z < -1.0) then
			z = -1.0;
		end
		dir = Vector3(dir.x,dir.y,z);
	end

	-- gamepad input
	if(input.UseGamepad == true) then
		local thumbx = input:CurrentGamePadState(currentIndex).thumbSticks.rightX;
		local thumby = input:CurrentGamePadState(currentIndex).thumbSticks.rightY;
		x = x + thumbx * 0.1;
		y = y + thumby * 0.1;
		z = z + input:CurrentGamePadState(currentIndex).triggers.right * 0.1
		z = z - input:CurrentGamePadState(currentIndex).triggers.left * 0.1
		dir = Vector3(dir.x + x * MathHelper.Pi/2,dir.y + y * MathHelper.Pi/2,dir.z + z *MathHelper.Pi/2) 
		Vector3.Normalize(dir)
	end

	light.Direction  = dir
end
