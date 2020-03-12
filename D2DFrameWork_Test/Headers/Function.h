#pragma once

template <typename T>
void SafeDelete(T& ptr)
{
	if (ptr)
	{
		delete ptr;
		ptr = nullptr;
	}
}

template <typename T>
void SafeDelete_Array(T& ptr)
{
	if (ptr)
	{
		delete[] ptr;
		ptr = nullptr;
	}
}

template <typename T>
float Clamp01(T& val)
{
	float temp = val;
	if (temp < 0)
		temp = 0.f;
	if (temp > 1)
		temp =1.f;

	return temp;
}