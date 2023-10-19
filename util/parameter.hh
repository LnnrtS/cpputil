#pragma once
#include "util/math.hh"

template<typename T, unsigned MinChangeNumerator, unsigned MinChangeDenom = 1>
struct LatchedParam {
	static constexpr T min_change = T(MinChangeNumerator) / T(MinChangeDenom);

	T val{0};
	bool changed{false};

	// TODO:
	// use a tracking_ctr to track value 1:1 for N number
	// of reads after a change > min_change

	bool store_changed(T new_val) {
		if (MathTools::abs_diff<T>(new_val, val) >= min_change) {
			val = new_val;
			changed = true;
			return true;
		} else
			return false;
	}

	bool did_change() {
		if (changed) {
			changed = false;
			return true;
		}
		return false;
	}

	void operator=(T x) {
		val = x;
	}
	operator T() {
		return val;
	}
};

template<typename T>
class Parameter {
public:
	Parameter()
		: currentVal(0) {
	}

	Parameter(T initVal)
		: currentVal(initVal) {
	}

	void setValue(const T input) {
		if (input != currentVal) {
			changed = true;
			currentVal = input;
		}
	}

	// Returns true if inner value was set to a different value at any point since isChanged() was last called
	// Todo: is this the behavior we want? Or do we just care if it's different at this point in time?
	// The edge-case is: x.setValue(0); x.isChanged(); x.setValue(1); x.setValue(0);
	// ...should isChanged() return true or false?
	bool isChanged() {
		bool changeStatus = changed;
		changed = false;
		return changeStatus;
	}

	T getValue() const {
		return currentVal;
	}

	operator T() const {
		return getValue();
	}

	void operator=(const T &that) {
		setValue(that);
	}

private:
	T currentVal = 0;
	bool changed = true;
};

// Todo: NOT thread-safe!
template<typename T>
class RefParameter {
public:
	RefParameter(T &initVal)
		: currentVal{initVal}
		, lastVal{0} {
	}

	void setValue(const T input) {
		currentVal = input;
	}

	// Returns true if inner value is different than it was the last time isChanged() was called
	bool isChanged() {
		if (currentVal == lastVal)
			return false;

		lastVal = currentVal;
		return true;
	}

	T getValue() const {
		return currentVal;
	}

	operator T() const {
		return getValue();
	}

	void operator=(const T &that) {
		this->setValue(that);
	}

private:
	T &currentVal;
	T lastVal;
};
