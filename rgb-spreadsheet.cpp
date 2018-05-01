#include "lodepng/lodepng.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>

using namespace std;

void decodeImage(vector<unsigned char>& image, unsigned int& width, unsigned int& height, string& filename) {
	unsigned int error = lodepng::decode(image, width, height, filename);

	if (error) {
		cout << "Error: " << error << lodepng_error_text(error) << endl;
		image.clear();
	}
}

void writeFile(vector<unsigned char>& image, unsigned int& width, unsigned int& height, ofstream& outfile) {
	int indicesPassed = 0; // Accounts for 1D image vector
	unsigned int r, g, b;

	for (unsigned int i = 0; i < height; ++i) {
		for (unsigned int j = 0; j < width * 4; j += 4) { // Width must be multiplied by 4 because values are stored in RGBA order for each pixel
			r = image.at(indicesPassed + j);
			g = image.at(indicesPassed + j + 1);
			b = image.at(indicesPassed + j + 2);
			outfile << "\"(" << r << ", " << g << ", " << b << ")\"" << ",";		
		}

		outfile << "\n";
		indicesPassed += width * 4;
	}
}

int main() {
	// Image decoding
	vector<unsigned char> image;
	unsigned int width, height;

	string filename;
	cout << "File name/path (PNG): ";
	cin >> filename;

	decodeImage(image, width, height, filename);
	
	if (image.size() == 0) {
		return 1;
	}

	// File output
	string filenameTruncated;
	filenameTruncated = filename.substr(0, filename.find('.'));

	stringstream outputFilename;
	outputFilename << filenameTruncated << "_rgbvalues.csv";

	ofstream outfile;
	outfile.open(outputFilename.str());

	writeFile(image, width, height, outfile);

	outfile.close();
	return 0;
}
