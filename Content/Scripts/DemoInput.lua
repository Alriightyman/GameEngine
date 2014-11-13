
-- Globals

Pi = 3.14159274

function RotateXLeft(x)
	RotX = x + Pi  * 0.1
	print(RotX)
end

function RotateXRight(x)
	RotX = x - Pi * 0.1 
end

function RotateYLeft(x)
	RotY = x + Pi * 0.1
end

function RotateYRight(x)
	RotY = x - Pi * 0.1
end

function RotateZLeft(x)
	RotZ = x + Pi * 0.02
end

function RotateZRight(x)
	RotZ = x - Pi * 0.02
end

function AddColorValue(x)
    x = x + 0.01;

    if (x > 1.0 ) then
        x = 0.0;
    end
    return x;
end

function SubtractColorValue(x)
    x = x - 0.1;

    if (x < 0.0 ) then
        x = 0.0;
    end
    return x;
end