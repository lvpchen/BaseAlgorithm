BYTE* resizePixels(BYTE* pixels, int w1, int h1, int w2, int h2) {
	BYTE * temp;
	temp = new BYTE[w2 * h2];
	double x_ratio = w1 / (double)w2;
	double y_ratio = h1 / (double)h2;
	double px, py;
	for (int i = 0; i<h2; i++) {
		for (int j = 0; j<w2; j++) {
			px = floor(j*x_ratio);
			py = floor(i*y_ratio);
			temp[(i*w2) + j] = pixels[(int)((py*w1) + px)];
		}
	}
	return temp;
}

