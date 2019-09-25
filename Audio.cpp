#include "Audio.h"

// ================== Audio =============

ALCdevice * Audio::device = NULL;
ALCcontext * Audio::context = NULL;

void Audio::configureListener(glm::vec3 pos, glm::vec3 dir, glm::vec3 vel)
{
	alListener3f(AL_POSITION, pos.x, pos.y, pos.z);
	alListener3f(AL_VELOCITY, vel.x, vel.y, vel.z);
	glm::vec3 direction = Transform::getTransformedZ(dir);
	glm::vec3 up = Transform::getTransformedY(dir);
	ALfloat listenerOri[] = {direction.x, direction.y, direction.z, up.x, up.y, up.z };
	alListenerfv(AL_ORIENTATION, listenerOri);
}

void Audio::init()
{
	//open the device
	
	device = alcOpenDevice(NULL);
	if (!device)
	{
		std::cout << "Failed to open Audio Device\n";
	}

	context = alcCreateContext(device, NULL);
	if (!alcMakeContextCurrent(context))
	{
		// failed to make context current
		std::cout << "Failed to make Audio context current\n";
	}

	alDistanceModel(AL_INVERSE_DISTANCE_CLAMPED);
}

void Audio::cleanUp()
{
	device = alcGetContextsDevice(context);
	alcMakeContextCurrent(NULL);
	alcDestroyContext(context);
	alcCloseDevice(device);
}

void Audio::setDistanceModel(ALint model)
{
	alDistanceModel(model);
}

// ================= Sound ===============

Sound::Sound()
{

}

Sound::~Sound()
{
	alDeleteBuffers(1, &bufferID);
	delete[] data;
}

bool Sound::isBigEndian()
{
	int a = 1;
	return !((char*)&a)[0];
}

int Sound::convertToInt(char* buffer, int len)
{
	int a = 0;
	if (!isBigEndian())
		for (int i = 0; i<len; i++)
			((char*)&a)[i] = buffer[i];
	else
		for (int i = 0; i<len; i++)
			((char*)&a)[3 - i] = buffer[i];
	return a;
}

char* Sound::loadWAV(std::string fileName, int& chan, int& samplerate, int& bps, int& size)
{
	char buffer[4];
	std::ifstream in(fileName, std::ios::binary);
	in.read(buffer, 4);
	if (strncmp(buffer, "RIFF", 4) != 0)
	{
		std::cout << "this is not a valid WAVE file" << std::endl;
		return NULL;
	}
	in.read(buffer, 4);
	in.read(buffer, 4);      //WAVE
	in.read(buffer, 4);      //fmt
	in.read(buffer, 4);      //16
	in.read(buffer, 2);      //1
	in.read(buffer, 2);
	chan = convertToInt(buffer, 2);
	in.read(buffer, 4);
	samplerate = convertToInt(buffer, 4);
	in.read(buffer, 4);
	in.read(buffer, 2);
	in.read(buffer, 2);
	bps = convertToInt(buffer, 2);
	in.read(buffer, 4);      //data
	in.read(buffer, 4);
	size = convertToInt(buffer, 4);
	char* data = new char[size];
	in.read(data, size);
	return data;
}

std::shared_ptr<Sound> Sound::loadFromFile(std::string fileName)
{
	std::shared_ptr<Sound> sound = std::shared_ptr<Sound>(new Sound());
	//sound->sndBuffer = std::shared_ptr<sf::SoundBuffer>(new sf::SoundBuffer);
	fileName = std::string("src/sounds/") + fileName + std::string(".wav");
	
	sound->data = loadWAV(fileName, sound->channel, sound->sampleRate, sound->bps, sound->size);

	alGenBuffers(1, &sound->bufferID);
	if (sound->channel == 1)
	{
		if (sound->bps == 8)
		{
			sound->format = AL_FORMAT_MONO8;
		}
		else {
			sound->format = AL_FORMAT_MONO16;
		}
	}
	else {
		if (sound->bps == 8)
		{
			sound->format = AL_FORMAT_STEREO8;
		}
		else {
			sound->format = AL_FORMAT_STEREO16;
		}
	}
	alBufferData(sound->bufferID, sound->format, sound->data, sound->size, sound->sampleRate);

	return sound;
}

// ==================== SoundEmitter ======================

SoundEmitter::SoundEmitter(std::shared_ptr<Sound> sound)
{
	this->sound = sound;
	alGenSources((ALuint)1, &sourceID);

	alSourcef(sourceID, AL_PITCH, 1);
	
	alSourcef(sourceID, AL_GAIN, 1);
	
	alSource3f(sourceID, AL_POSITION, 0, 0, 0);
	
	alSource3f(sourceID, AL_VELOCITY, 0, 0, 0);
	
	alSourcei(sourceID, AL_LOOPING, AL_FALSE);

	alSourcei(sourceID, AL_BUFFER, sound->bufferID);
}

SoundEmitter::~SoundEmitter()
{
	stop();
	alDeleteSources(1, &sourceID);
}

void SoundEmitter::setSound(std::shared_ptr<Sound> sound)
{
	
}

void SoundEmitter::setAttenuation(double attenuation)
{
	alSourcef(sourceID, AL_ROLLOFF_FACTOR, attenuation);
}

void SoundEmitter::setMinDistance(double minDistance)
{
	alSourcef(sourceID, AL_REFERENCE_DISTANCE, minDistance);
}

void SoundEmitter::setMaxDistance(double maxDistance)
{
	alSourcef(sourceID, AL_MAX_DISTANCE, maxDistance);
}

void SoundEmitter::play()
{
	stop();
	alSource3f(sourceID, AL_POSITION, transform.position.x, transform.position.y, transform.position.z);
	alSourcePlay(sourceID);
}

void SoundEmitter::setLoop(bool loop)
{
	alSourcei(sourceID, AL_LOOPING, (loop?AL_TRUE:AL_FALSE));
}

void SoundEmitter::setVolume(double volume)
{
	alSourcef(sourceID, AL_GAIN, volume);
}

void SoundEmitter::setPitch(double pitch)
{
	alSourcef(sourceID, AL_PITCH, pitch);
}

void SoundEmitter::setVelocity(glm::vec3 vel)
{
	alSource3f(sourceID, AL_VELOCITY, vel.x, vel.y, vel.z);
}

bool SoundEmitter::isPlaying()
{
	int state;
	alGetSourcei(sourceID, AL_SOURCE_STATE, &state);
	return state == AL_PLAYING;
}

void SoundEmitter::pause()
{
	alSourcePause(sourceID);
}

void SoundEmitter::continuePlaying()
{
	alSourcePlay(sourceID);
}

void SoundEmitter::stop()
{
	alSourceStop(sourceID);
}
