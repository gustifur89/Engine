#pragma once
class Toggle
{
private:
	bool internalToggle;
public:
	Toggle()
	{
		internalToggle = true;
	}

	bool toggle(bool condition)
	{
		if (condition)
		{
			if (internalToggle)
			{
				internalToggle = false;
				return true;
			}
		}
		else
		{
			internalToggle = true;
		}
		return false;
	}




};
