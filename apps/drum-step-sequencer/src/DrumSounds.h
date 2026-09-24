#pragma once

#include <stdint.h>

struct DrumSound {
  uint8_t midiNote;
  const char* name;
  const char* displayName;
};

constexpr DrumSound DRUM_SOUNDS[] = {
    {38, "Acoustic Snare", "AC SNARE"},
    {40, "Electric Snare", "EL SNARE"},
    {39, "Hand Clap", "CLAP"},
    {75, "Claves", "CLAVES"},
    {42, "Closed Hi-Hat", "C HIHAT"},
    {44, "Pedal Hi-Hat", "P HIHAT"},
    {46, "Open Hi-Hat", "O HIHAT"},
    {54, "Tambourine", "TAMBO"},
    {58, "Vibraslap", "VIBRA"},
    {79, "Open Cuica", "CUICA"},
    {55, "Splash Cymbal", "SPLASH"},
    {52, "Chinese Cymbal", "CHINESE"},
    {51, "Ride Cymbal 1", "RIDE 1"},
    {59, "Ride Cymbal 2", "RIDE 2"},
    {53, "Ride Bell", "RIDE BELL"},
    {56, "Cowbell", "COWBELL"},
    {67, "High Agogo", "H AGOGO"},
    {68, "Low Agogo", "L AGOGO"},
    {45, "Low Tom", "LOW TOM"},
    {70, "Maracas", "MARACAS"},
};

constexpr uint8_t DRUM_SOUND_COUNT =
    sizeof(DRUM_SOUNDS) / sizeof(DRUM_SOUNDS[0]);

static_assert(DRUM_SOUND_COUNT == 20,
              "The sound layer maps one sound to each Calculator key");
