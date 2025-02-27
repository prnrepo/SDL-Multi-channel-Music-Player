#pragma once

#include "sound.h"

class MusicChannel
{
public:
	enum FadeType { NotFadingType, FadingInType, FadingOutType };
protected:
	Sound music;
	int value;
	int channel;
	long double volume;
	FadeType fade_type;

public:
	MusicChannel();
	void create(const int music_value, const int channel_addition);
	~MusicChannel() { free(); }

	const Sound& get_music() const { return music; }
	const int& get_value() const { return value; }
	const int& get_channel() const { return channel; }
	const long double& get_volume() const { return volume; }
	const FadeType& get_fade_type() const { return fade_type; }

	void set_value(const int set) { value = set; }
	void set_channel(const int set) { channel = set; }
	void set_volume(const long double set);
	void set_fade_type(const FadeType set) { fade_type = set; }

	void silence();

	void free();
};

class MusicPlayer
{
protected:
	MusicChannel first_channel;
	MusicChannel second_channel;
	MusicChannel::FadeType music_player_fade_type;
	bool deleted;

public:
	MusicPlayer();
	const bool create(const int first_channel_value, const int second_channel_value, const int channel_base_value);
	~MusicPlayer() { free(); }

	const MusicChannel& get_first_channel() const { return first_channel; }
	const MusicChannel& get_second_channel() const { return second_channel; }
	const MusicChannel::FadeType& get_music_player_fade_type() const { return music_player_fade_type; }
	const bool& get_deleted() const { return deleted; }

	MusicChannel& pass_first_channel() { return first_channel; }
	MusicChannel& pass_second_channel() { return second_channel; }

	void set_music_player_fade_type(const MusicChannel::FadeType set) { music_player_fade_type = set; }
	void set_deleted(const bool set) { deleted = set; }

	void silence();

	void update();

	void free();
};