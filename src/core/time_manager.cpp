
static double _DeltaTime;
static double _PrevFrameTime;

bool calculate_delta_time(double rCurrentTime, double desiredFrameRate)
{
	_DeltaTime = rCurrentTime - _PrevFrameTime;
	if (_DeltaTime <= (1.0f / desiredFrameRate)) 
	{
		return false;
	}
	else
	{
		_PrevFrameTime = rCurrentTime;
		return true;
	}
}

double get_delta_time()
{
	return _DeltaTime;
}