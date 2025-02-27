#include "music_player.h"

//Sound is just a wrapper for an SDL_Mixer Mix_Chunk
/*void Sound::play_music(const int play_channel)
{
	channel = Mix_PlayChannel(play_channel, pass_my_sfx(), -1);
}*/

MusicChannel::MusicChannel()
{
	free();
}

void MusicChannel::create(const int music_value, const int channel_addition)
{
	if (music_value != -1)
	{
		set_value(music_value);
		Sound* sound = ResourceManager::pass_music(music_value);
		if (sound)
		{
			const int base_channel = 0 + channel_addition;
			set_channel(base_channel);
			set_volume(0);
			set_fade_type(FadeType::NotFadingType);
			sound->play_music(base_channel);
		}
	}

	else
		free();
}

void MusicChannel::set_volume(const long double set)
{
	if (get_channel() != -1)
	{
		volume = set;
		Mix_Volume(get_channel(), int(get_volume()));
	}
}

void MusicChannel::silence()
{
	if (get_channel() != -1)
	{
		set_volume(0);
		set_fade_type(FadeType::NotFadingType);
	}
}

void MusicChannel::free()
{
	set_value(-1);
	set_channel(-1);
	set_volume(0);
	set_fade_type(FadeType::NotFadingType);
}

MusicPlayer::MusicPlayer() : music_player_fade_type(MusicChannel::FadeType::NotFadingType), deleted(false) {}

const bool MusicPlayer::create(const int first_channel_value, const int second_channel_value, const int channel_base_value)
{
	first_channel.create(first_channel_value, channel_base_value);
	first_channel.set_fade_type(MusicChannel::FadingInType);

	if (second_channel_value != get_second_channel().get_value())
		second_channel.create(second_channel_value, channel_base_value + 1);

	set_music_player_fade_type(MusicChannel::FadeType::NotFadingType);
	set_deleted(false);
	return true;
}

void MusicPlayer::silence()
{
	first_channel.silence();
	second_channel.silence();
}

void MusicPlayer::update()
{
	const long double fade_velocity = 0.42;//The amount needed to fade in/out over 5 seconds

	if (get_music_player_fade_type() == MusicChannel::FadingOutType)
	{
		if (first_channel.get_channel() != -1)
		{
			first_channel.set_volume(get_first_channel().get_volume() - fade_velocity);
			if (get_first_channel().get_volume() <= 0)
				first_channel.set_volume(0);
		}

		if (second_channel.get_channel() != -1)
		{
			second_channel.set_volume(get_second_channel().get_volume() - fade_velocity);
			if (get_second_channel().get_volume() <= 0)
				second_channel.set_volume(0);
		}

		if (get_first_channel().get_volume() <= 0 ||
			get_first_channel().get_channel() == -1)
		{
			if (get_second_channel().get_volume() <= 0 ||
				get_second_channel().get_channel() == -1)
				free();
		}

		return;
	}

	if (get_first_channel().get_value() != -1 ||
		get_second_channel().get_value() != -1)
	{
		const int active_channel_volume = int(long double(ResourceManager::get_music_volume()) * 6.4);

		if (get_first_channel().get_value() != -1)
		{
			if (get_first_channel().get_channel() != -1)
			{
				switch (get_first_channel().get_fade_type())
				{
				case MusicChannel::FadeType::FadingInType:
					first_channel.set_volume(get_first_channel().get_volume() + fade_velocity);
					if (get_first_channel().get_volume() >= active_channel_volume)
					{
						first_channel.set_volume(active_channel_volume);
						first_channel.set_fade_type(MusicChannel::FadeType::NotFadingType);
					}
					break;
				case MusicChannel::FadeType::FadingOutType:
					first_channel.set_volume(get_first_channel().get_volume() - fade_velocity);
					if (get_first_channel().get_volume() <= 0)
					{
						first_channel.set_volume(0);
						first_channel.set_fade_type(MusicChannel::FadeType::NotFadingType);
					}
					break;
				}
			}

			else
				first_channel.set_volume(0);
		}

		if (get_second_channel().get_value() != -1)
		{
			if (get_second_channel().get_channel() != -1)
			{
				switch (get_second_channel().get_fade_type())
				{
				case MusicChannel::FadeType::FadingInType:
					second_channel.set_volume(get_second_channel().get_volume() + fade_velocity);
					if (get_second_channel().get_volume() >= active_channel_volume)
					{
						second_channel.set_volume(active_channel_volume);
						second_channel.set_fade_type(MusicChannel::FadeType::NotFadingType);
					}
					break;
				case MusicChannel::FadeType::FadingOutType:
					second_channel.set_volume(get_second_channel().get_volume() - fade_velocity);
					if (get_second_channel().get_volume() <= 0)
					{
						second_channel.set_volume(0);
						second_channel.set_fade_type(MusicChannel::FadeType::NotFadingType);
					}
					break;
				}
			}

			else
				second_channel.set_volume(0);
		}
	}

	else if (!get_first_channel().get_volume() &&
		!get_second_channel().get_volume())
		free();
}

void MusicPlayer::free()
{
	set_deleted(true);
	set_music_player_fade_type(MusicChannel::FadeType::NotFadingType);
	first_channel.free();
	second_channel.free();
}
