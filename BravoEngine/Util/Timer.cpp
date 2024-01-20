#include "Timer.h"
#include <Windows.h>

Timer::Timer() : _SecPerFrame(0.0f), _DeltaTime(0), _BaseFrame(0), _PausedFrames(0), _PrevFrame(0), _CurrFrame(0), _StopFrame(0), _bStopped(false)
{
	__int64 FramePerSec;
	QueryPerformanceFrequency((LARGE_INTEGER*)&FramePerSec);
	_SecPerFrame = 1 / (double)FramePerSec;
}

float Timer::GetElapsedTime() const
{
	if (_bStopped == true)
	{
		return static_cast<float>(((_StopFrame - _BaseFrame) - _PausedFrames) * _SecPerFrame);
	}
	else
	{
		return static_cast<float>(((_CurrFrame - _BaseFrame) - _PausedFrames) * _SecPerFrame);
	}
}

float Timer::GetDeltaTime() const
{
	return static_cast<float>(_DeltaTime);
}

void Timer::Reset()
{
	__int64 currTime;
	QueryPerformanceCounter((LARGE_INTEGER*) & currTime);

	_BaseFrame = currTime;
	_PrevFrame = currTime;
	_StopFrame = 0;
	_bStopped = false;
}

void Timer::Start()
{
	__int64 startTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&startTime);

	if (_bStopped)
	{
		_PausedFrames += startTime - _StopFrame;
		_PrevFrame = startTime;
		_StopFrame = 0;
		_bStopped = false;
	}
}

void Timer::Stop()
{
	if (_bStopped == false)
	{
		__int64 stopTime;
		QueryPerformanceCounter((LARGE_INTEGER*)&stopTime);

		_StopFrame = stopTime;
		_bStopped = true;
	}
}

void Timer::Tick()
{
	if (_bStopped = true)
	{
		_DeltaTime = 0.0f;
		return;
	}

	__int64 currTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&currTime);
	_CurrFrame = currTime;

	_DeltaTime = (_CurrFrame - _PrevFrame) * _SecPerFrame;
	_PrevFrame = _CurrFrame;

	if (_DeltaTime < 0.0f)
	{
		_DeltaTime = 0.0f;
	}
}