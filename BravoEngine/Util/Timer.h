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
		
	__int64 _BaseFrame;	// ���� ������
	__int64 _PausedFrames;	// pause ���¿��� ������ ��
	__int64 _StopFrame;	// ���߱� ������ ������
	__int64 _PrevFrame;
	__int64 _CurrFrame;

	bool _bStopped;
};