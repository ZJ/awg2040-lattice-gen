#include "awg.hpp"

#include "pulseTrain.hpp"

std::string awg::programmingString() {
	// Say where we're going to store this one
	std::string programmingString = "DATA:DESTINATION \"" + _waveName + "\"\n";

	// Load up our points in temp buffers
	unsigned long numPoints = outputPulses.getNumPoints(_sampleRate, true);
	std::string curveString = outputPulses.getWaveChars(_sampleRate, numPoints, true);
	std::string markerString = outputPulses.getMarkerChars(_sampleRate, numPoints, true, round(_startPulseLength/_sampleRate));
	char buffer[128];
	unsigned int charLength = sprintf(buffer, "%lu", numPoints);
	sprintf(buffer, "#%u%lu", charLength, numPoints);
	std::string lengthString = buffer;

	// Set up configuration
	programmingString += "DATA:WIDTH 1\n";
	// Copy the curve
	programmingString += "CURVE " + lengthString;
	programmingString.append(curveString);
	programmingString += "\n";
	// Copy the markers
	programmingString += "MARKER:DATA " + lengthString;
	programmingString.append(markerString);
	programmingString += "\n";
	// Set clock frequency
	snprintf(buffer, 128, "CLOCK:FREQUENCY %fMHz\n", _clockFrequency);
	programmingString += buffer;
	// Set the file to use
	programmingString += "CH1:WAVEFORM \"" + _waveName + "\"\n";

	return programmingString;
}
