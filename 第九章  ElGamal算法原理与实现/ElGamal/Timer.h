#ifndef _QT_TIMER_H_INCLUDED_
#define _QT_TIMER_H_INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <windows.h>
//for LARGE_INTEGER, QueryPerformanceCounter()
//and QueryPerformanceFrequency()

class Timer  
{
private:
	LARGE_INTEGER begin_;
	LARGE_INTEGER end_;
	LARGE_INTEGER counter_;
	bool running_;
	
public:
	static unsigned long frequency(void)
	{
		LARGE_INTEGER frqc;
		::QueryPerformanceFrequency(&frqc);
		return frqc.LowPart;
	}
	Timer(void): running_(false)
	{
		begin_.QuadPart = end_.QuadPart = counter_.QuadPart = 0;
	}
	void start(void)   //��ʼ��ʱ 
	{
		if(running_ == false){
	           running_ = true;
			   ::QueryPerformanceCounter(&begin_);
		}
	}
	void stop(void)   //������ʱ 
	{
		if(running_ == true){
	           ::QueryPerformanceCounter(&end_);
			   running_ = false;
			   counter_.QuadPart += end_.QuadPart - begin_.QuadPart;
		}
	}
	void reset(void)   //������¼�ʱ 
	{
		begin_.QuadPart = end_.QuadPart = counter_.QuadPart = 0;
		running_ = false;
	}
	double seconds(void) //����ʱ�� 
	{
		LARGE_INTEGER frqc;
		::QueryPerformanceFrequency(&frqc);
		if(running_ == false)
			return counter_.QuadPart / static_cast<double>(frqc.QuadPart);
		::QueryPerformanceCounter(&end_);
		return counter_.QuadPart + (end_.QuadPart - begin_.QuadPart)
            / static_cast<double>(frqc.QuadPart);
	}
	bool running(void) const
	{
		return running_; 
	}
};//~class Timer
#endif //~ #infdef _QT_TIMER_H_INCLUDED_
//~Timer.h
