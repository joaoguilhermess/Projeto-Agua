const int sensors[3][2] = {
	{27, 13},
	{26, 12},
	{25, 14}
};

const char* alias[3] = {"A", "B", "C"};

const float sizes[8][2] = {
	{1.16/2, 0.00},
	{1.21/2, 0.19},
	{1.25/2, 0.20},
	{1.29/2, 0.37},
	{1.33/2, 0.38},
	{1.40/2, 0.65},
	{1.46/2, 0.66},
	{1.51/2, 0.76}
};

float getDistance(int t, int e) {
	digitalWrite(t, LOW);

	delayMicroseconds(2);
	
	digitalWrite(t, HIGH);
	
	delayMicroseconds(5);
	
	digitalWrite(t, LOW);

	long duration = pulseIn(e, HIGH);

	// distance = (float) ((int) (duration / 29 / 2 * 10))/10;
	float distance = duration * 0.034 / 2;
	// distance = duration / 29 / 2;

	distance += 0.3;

	if (distance > 400) {
		distance = 0;
	}

	return round(distance) / 100;
}

float getVolume(float distance) {
	if (distance < 0) {
		return 0;
	}

	float volume = 0;

	for (int s = 1; s < sizeof(sizes) / sizeof(sizes[0]); s++) {
		float r = sizes[s - 1][0];

		if (distance >= sizes[s][1]) {
			float R = sizes[s][0];
			float h = sizes[s][1] - sizes[s - 1][1];

			volume += 1.0 / 3.0 * PI * h * ((R * R) + (R * r) + (r * r)) * 1000.0;
		} else {
			float R = sizes[s - 1][0] + ((sizes[s][0] - sizes[s - 1][0]) * (distance / sizes[s][1]));
			float h = distance - sizes[s - 1][1];

			volume += 1.0 / 3.0 * PI * h * ((R * R) + (R * r) + (r * r)) * 1000.0;

			break;
		}
	}

	volume *= 0.9663889908966157;

	return volume;
}

void setup() {
	Serial.begin(115200);

	for (int i = 0; i < sizeof(sensors) / sizeof(sensors[0]); i++) {
		pinMode(sensors[i][0], OUTPUT);
		pinMode(sensors[i][1], INPUT);
	}
}

void loop() {
	for (int i = 0; i < sizeof(sensors) / sizeof(sensors[0]); i++) {
		float distance = getDistance(sensors[i][0], sensors[i][1]);

		distance -= 0.21;

		distance = 0.76 - distance;

		Serial.print(alias[i]);
		Serial.print(":");
		Serial.print(" ");

		Serial.print(distance * 100);
		Serial.print("cm");
		Serial.print(" ");

		float volume = getVolume(distance);

		Serial.print(volume, 2);
		Serial.print("L");
		Serial.print(" ");

		float percent = volume / 1000 * 100;

		Serial.print(percent);
		Serial.print("%");
		Serial.print(" ");

		delay(1000/5);
	}

	Serial.println("");
}