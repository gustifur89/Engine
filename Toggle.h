#pragma once
class Toggle
{
private:
	bool internalToggle = false;
public:
	bool toggle(bool condition)
	{
		if (condition)
		{
			if (internalToggle)
			{
				return true;
			}
			internalToggle = false;
		}
		else
		{
			internalToggle = true;
		}
		return false;
	}
};
