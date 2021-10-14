#ifndef DONKEY_KONG_1981_SOUNDS_HH
#define DONKEY_KONG_1981_SOUNDS_HH

#include <allegro5/allegro_audio.h>
#include <optional>

enum sound_id
{
  SND_MUSIC_1,
  SND_MUSIC_2,
  SND_MUSIC_HAMMER,
  SND_DEATH,
  SND_SMASH,
  SND_HOW_HIGH,
  SND_INTRO,
  SND_JUMP,
  SND_SCORE,
  SND_RUN,
  SND_FALL,
  SND_WIN_1,
  SND_WIN_2,
  SND_LEVEL_WIN,
  SND_SPRING_BOUNCE,
  SND_SPRING_FALL,
  SND_OIL_LIGHT,
  SND_BOOT,
  SND_COUNTDOWN,
  SND_COUNT
};

/** Returns false if failed to load any sound file. */
bool load_sounds();

/** Must call at end to free sound memory. */
void unload_sounds();

using snd_play_id = std::optional<ALLEGRO_SAMPLE_ID>;

/** If sound successfully play returns a sample if which can be used to later
 *  stop the playback; returns no value if sound failed to play. */
snd_play_id play_sound(sound_id snd, float speed = 1.f, bool loop = false, float gain = 1.0);

/** Stop a playing sound; 'sample_id' is an optional so the caller to 'play_sound'
 *  need not check if a value was returned. */
void stop_sound(snd_play_id id);

#endif //DONKEY_KONG_1981_SOUNDS_HH
