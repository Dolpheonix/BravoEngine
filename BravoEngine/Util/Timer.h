#pragma once

class Timer
{
public:
	Timer();

	float GetElapsedTime() const;
	float GetDeltaTime() const;

	void Reset();
	void Start();
	void Stop();
	void Tick();

private:
	double _SecPerFrame;
	double _DeltaTime;
		
	__int64 _BaseFrame;	// 시작 프레임
	__int64 _PausedFrames;	// pause 상태였던 프레임 수
	__int64 _StopFrame;	// 멈추기 시작한 프레임
	__int64 _PrevFrame;
	__int64 _CurrFrame;

	bool _bStopped;
};