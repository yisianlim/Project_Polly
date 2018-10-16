#include "noise.hpp"

double PerlinNoise::noise(double x, double y) {
	return m_perlinModule.GetValue(x, y, 0);
}

double ComplexNoise::noise(double x, double y) {
	double flat_terrain = m_base_flat_terrain.GetValue(x, y, 0) * 0.125 + (-0.75);

	double controlValue = m_terrain_type.GetValue(x, y, 0);
	double edge_fall_off = 0.125;
	double lower_bound = 0;
	double upper_bound = 1000;
	double alpha;
	if (edge_fall_off > 0.0) {
		if (controlValue < (lower_bound - edge_fall_off)) {
			// The output value from the control module is below the selector
			// threshold; return the output value from the first source module.
			return flat_terrain;

		}
		else if (controlValue < (lower_bound + edge_fall_off)) {
			// The output value from the control module is near the lower end of the
			// selector threshold and within the smooth curve. Interpolate between
			// the output values from the first and second source modules.
			double lowerCurve = (lower_bound - edge_fall_off);
			double upperCurve = (lower_bound + edge_fall_off);
			alpha = noise::SCurve3(
				(controlValue - lowerCurve) / (upperCurve - lowerCurve));
			return LinearInterp(flat_terrain,m_mountain_terrain.GetValue(x, y, 0),
				alpha);

		}
		else if (controlValue < (upper_bound - edge_fall_off)) {
			// The output value from the control module is within the selector
			// threshold; return the output value from the second source module.
			return m_mountain_terrain.GetValue(x, y, 0);

		}
		else if (controlValue < (upper_bound + edge_fall_off)) {
			// The output value from the control module is near the upper end of the
			// selector threshold and within the smooth curve. Interpolate between
			// the output values from the first and second source modules.
			double lowerCurve = (upper_bound - edge_fall_off);
			double upperCurve = (upper_bound + edge_fall_off);
			alpha = SCurve3(
				(controlValue - lowerCurve) / (upperCurve - lowerCurve));
			return LinearInterp(m_mountain_terrain.GetValue(x, y, 0),
				flat_terrain,
				alpha);

		}
		else {
			// Output value from the control module is above the selector threshold;
			// return the output value from the first source module.
			return flat_terrain;
		}
	}
	else {
		if (controlValue < lower_bound || controlValue > upper_bound) {
			return flat_terrain;
		}
		else {
			return m_mountain_terrain.GetValue(x, y, 0);
		}
	}


	//return m_terrain_type.GetValue(x, y, 0);
}
