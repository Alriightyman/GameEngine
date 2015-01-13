function Initialize(camera)
	camera.position = Vector3(0.0,0.0,-10.0)
end
-- Handles Camera Control -> WASD
function HandleInput(camera,input)
	
    local move = 0.05;

	 -- zoom camera in/out
    if(input:IsKeyDown(Keys.DIK_EQUALS)) then
        camera.position = Vector3(camera.position.x,camera.position.y,camera.position.z + move);
    elseif(input:IsKeyDown(Keys.DIK_MINUS)) then
        camera.position = Vector3(camera.position.x,camera.position.y,camera.position.z - move);
    end

    -- move camera up/down
    if (input:IsKeyDown(Keys.DIK_W)) then       
        camera.position = Vector3(camera.position.x, camera.position.y + move,camera.position.z);
    elseif (input:IsKeyDown(Keys.DIK_S)) then
        camera.position = Vector3(camera.position.x,camera.position.y - move,camera.position.z);
    end
    -- move camera left/right
    if (input:IsKeyDown(Keys.DIK_A)) then
        camera.position = Vector3(camera.position.x - move,camera.position.y,camera.position.z);
    elseif (input:IsKeyDown(Keys.DIK_D)) then
        camera.position = Vector3(camera.position.x + move,camera.position.y,camera.position.z);
    end

	-- rotate camera over y-axis
	if(input:IsKeyDown(Keys.DIK_Q)) then
		if (input:IsKeyDown(Keys.DIK_LCONTROL)) then
			camera.rotation = Vector3(camera.rotation.x,camera.rotation.y + move * 10, camera.rotation.z)
		else
			camera.rotation = Vector3(camera.rotation.x,camera.rotation.y - move * 10, camera.rotation.z)
		end
	end
	-- rotate camera over x-axis
	if(input:IsKeyDown(Keys.DIK_E)) then
		if (input:IsKeyDown(Keys.DIK_LCONTROL)) then
			camera.rotation = Vector3(camera.rotation.x + move * 10,camera.rotation.y, camera.rotation.z)
		else
			camera.rotation = Vector3(camera.rotation.x - move * 10,camera.rotation.y, camera.rotation.z)
		end
	end

    if(input:IsKeyDown(Keys.DIK_R)) then
        camera.reloadScript = true;
    end

end