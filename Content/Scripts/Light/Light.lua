

function Initialize(light)

    light.AmbientColor = Color(.15,.15,.15,1.0);
    light.DiffuseColor = Color(0.5,0.5,0.5,1.0);
    light.SpecularColor = Color(1,1,1,1);
    light.SpecularPower = 32.0;
    light.Direction = Vector3(0.0,0.0,1.0);

end
