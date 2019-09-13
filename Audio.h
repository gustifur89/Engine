#pragma once
#include "Headers.h"
#include <al.h>
#include <alc.h>
#include "Transform.h"
//#include <SFML/Audio.hpp>

// OVERHAUL AUDIO.   SMFL doesn't do it right.

class Audio
{
public:
	static void configureListener(glm::vec3 pos, glm::vec3 dir, glm::vec3 vel = glm::vec3(0,0,0));
	static void init();
	static void cleanUp();
	static void setDistanceModel(ALint model);
	static ALCdevice * device;
	static ALCcontext * context;
};

class Sound
{
private:
	ALuint format;
	ALint channel, sampleRate, bps, size;
	char * data;

	static bool isBigEndian();
	static int convertToInt(char* buffer, int len);
	static char* loadWAV(std::string fileName, int& chan, int& samplerate, int& bps, int& size);
public:
	Sound();
	~Sound();
	ALuint bufferID;
	static std::shared_ptr<Sound> loadFromFile(std::string fileName);
};

class SoundEmitter
{
public:
	SoundEmitter(std::shared_ptr<Sound> sound);
	~SoundEmitter();
	
	void setSound(std::shared_ptr<Sound> sound);
	void setAttenuation(double attenuation);
	void setMinDistance(double minDistance);
	void setMaxDistance(double maxDistance);
	void play();
	void setLoop(bool loop);
	void setVolume(double volume);
	void setPitch(double pitch);
	void setVelocity(glm::vec3 vel);
	bool isPlaying();
	void pause();
	void continuePlaying();
	void stop();

	ALuint sourceID;

	std::shared_ptr<Sound> sound;
	Transform transform;
};
