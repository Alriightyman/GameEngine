
Experimenting with Direct3D 11 and creating a Game Engine.

This "Engine" requires the use of the a modified version of the DirectX Tool Kit.  The modification is quite simple, under SimpleMath.inl change all occurences of RH to LH.

For example, changing XMMatrixPerspectiveFovRH to XMMatrixPerspectiveFovLH.