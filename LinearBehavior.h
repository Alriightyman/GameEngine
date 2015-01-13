#pragma once
#include "Behavior.h"

class Tank;

class LinearBehavior : public Behavior
{
public:
	LinearBehavior(Tank* tank);

	void Update(float dt) override;

};