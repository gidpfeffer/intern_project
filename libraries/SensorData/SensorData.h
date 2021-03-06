/*
Written by: Gideon Pfeffer

This class stored data and determines whether or not threats are caught. It has
API's to make storing, accessing, and interpreting data more straight forward.
*/

#ifndef SensorData_h
#define SensorData_h

#include "Arduino.h"
#include "MathHelper.h"

template <class T> class SensorData
{
	public:
		/*
		the threshold signifies how many times of the standard deviation
		the data will accept before considering it a threat
		1.65 is 90%
		2 is 95% and so on
		*/
		SensorData(double threshold, int capacity);
		~SensorData();
		void updateData(T value);
		bool isCalibrated();
		bool isThreatened();
		double getAverage();
		double getStdDev();
		String dataToString();
		void reset();
		bool shouldBeLogged();
		void setLogged();
		T getThreatCausingVal();

	private:
		double updateAverage(T newValue, T oldValue);
		double updateStdDev(double newAverage);
		bool checkForThreats(T newValue);
		void updateMetrics(T newValue);
		T* _data;
		double _average;
		double _stdDeviation;
		int _index;
		bool _isThreatened;
		bool _isCalibrated;
		double _threshold;
		bool _hasBeenLogged;
		T _threatCausingVal;
		int _capacity;
};

#include "SensorData.hxx"

#endif